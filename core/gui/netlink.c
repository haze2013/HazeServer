#include "netlink.h"
#include "wblist.h"
#include "policy.h"
#include "udte.h"

netlink_socket_t nelkskt;

/*
* netlink receive hello message from agent
*/
static int nelk_recv_helo(struct sk_buff *skb)
{
    int ret = 0;
    pid_t pid = NETLINK_CREDS(skb)->pid;
    netlink_lock(nelkskt.lock);
    if(nelkskt.master_pid){
        printkinfo("Agent is runing [%d]\n", nelkskt.master_pid);
        ret = -1;
    }else{
        nelkskt.master_pid = pid;
        printkinfo("Receive agent hello message [%d]\n", pid);
    }
    netlink_unlock(nelkskt.lock);
    return ret;
}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32)
    #define SEC_NLMSG_DATA(nlhder) NLMSG_DATA(nlhder)
#else
    #define SEC_NLMSG_DATA(nlhder) nlmsg_data(nlhder)
#endif
static int nelk_recv_request(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)(skb->data);
    int *msg = SEC_NLMSG_DATA(nlh);
    netlink_lock(nelkskt.lock);
    switch(msg[0]){
        case REQUEST_RELOAD:
            policy_update(msg[1]);  
            break;
        default:
            break;
    }
    netlink_unlock(nelkskt.lock);
    return 0;
}
static int nelk_recv_exceptprogram(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr*)skb->data;
    int i = 0;
    kexcepram_t *kp = SEC_NLMSG_DATA(nlh);  
    printkdbg("Execept program:[%d]\n",kp->n_size);
    for(i=0; i < kp->n_size; ++i){
    //XXX
       wblist_enter_one(&softwarelist, kp->nodes[i].hash, kp->nodes[i].flag);
    } 
    return 0;
}
static int nelk_recv_quit(struct sk_buff *skb)
{
	int ret = 0;
    pid_t pid = NETLINK_CREDS(skb)->pid;
    netlink_lock(nelkskt.lock);

    if(nelkskt.master_pid != pid){
        printkinfo("netlink quit\n");
    	ret = -1;
    }else{
        nelkskt.master_pid = 0;
	printkinfo("Userspace agent quit\n");
    }
    netlink_unlock(nelkskt.lock);
	return ret;
}
static int nelk_recv_ssdo(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr*)skb->data;
    struct kuser *kp = SEC_NLMSG_DATA(nlh);  
    return udte_user_enter(kp);
}

int sec_isprotect = 1;
static int nelk_recv_protect(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)(skb->data);
    int *msg = SEC_NLMSG_DATA(nlh);
    netlink_lock(nelkskt.lock);
    switch(msg[0]){
        case REQUEST_SWITCH:
            sec_isprotect = msg[1];  
            break;
        default:
            break;
    }
    netlink_unlock(nelkskt.lock);
    return 0;
}

int _nelk_recv(struct sk_buff *skb)
{
    struct nlmsghdr *nlhdr = NULL;
    if(skb == NULL){
        printkerr("netlink recvive error\n");
        return -1;
    }
   // printkdbg("come into nelk_recv()\n");


    nlhdr = (struct nlmsghdr *)(skb->data);
    #if 0
    if(skb->data_len == 0 || skb->data == NULL){
        return -1;
    }
    #endif
    switch(nlhdr->nlmsg_type){
        case SEC_NLMSG_HELO:
            if(nelk_recv_helo(skb) < 0){
            }
            break;
        case SEC_NLMSG_QUIT:
            if(nelk_recv_quit(skb) < 0){
                ;
	        }
            break;
        case SEC_NLMSG_EXCEPT:
            if(nelk_recv_exceptprogram(skb) < 0){
                ;
            }
	    break;
	case SEC_NLMSG_REQUEST:
	    if(nelk_recv_request(skb) < 0){
	    	;
	    }
	    break;
        case SEC_NLMSG_SSDO:
            if(nelk_recv_ssdo(skb) < 0){
                ;
	    }
	    break;
	case SEC_NLMSG_ISPROTECT:
	    if(nelk_recv_protect(skb) < 0){
		    ;
	    }
	    break;
	default:
            printkerr("netlink recvice unkown message type [%0x]\n",nlhdr->nlmsg_type);
            break;
    }
    //printkdbg("leave from nelk_recv()\n");
    return 0;
}
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,18))
void nelk_recv(struct sock *sk, int len)
{
    struct sk_buff *skb = NULL;
    int rc = 0;
re_receive:
    skb = skb_recv_datagram(sk, 0, 0, &rc);
    if(rc == -EINTR){
        goto re_receive;
    }
    if(rc < 0){
        printkerr("netlink recvice message error\n");
        return;
    }
    if(_nelk_recv(skb) < 0){
        ;
    }
    kfree_skb(skb);
    return;
}
#else
void nelk_recv(struct sk_buff *skb)
{
	if( _nelk_recv(skb) < 0){
        ;
    }
    return ;
}
#endif

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32) 
int nelk_send(pid_t pid, void *data, int datalen, u16 msg_type, u16 msg_flags)
{
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlhdr = NULL;
	char *msg = NULL;
	size_t size = 0;
	int ret = 0;
	if(nelkskt.sock == NULL){
		printkerr("netlink socket error\n");
		return -1;
	}
	size = (data && datalen) ? datalen:0;
	skb = alloc_skb(NLMSG_SPACE(size), GFP_KERNEL);
	if(skb == NULL){
		printkerr("netlink send error: no enough memory\n");
		return -ENOMEM;
	}
	nlhdr = NLMSG_PUT(skb, pid, 0, msg_type, size);
	nlhdr->nlmsg_flags = msg_flags;
	if(datalen){
		msg = (char *)(NLMSG_DATA(nlhdr));
		memcpy(msg, data, datalen);
	}
	/*XXX:Attention,netlink_unicast will free skb when data send out */
//	schedule_timeout_uninterruptible(sec_cfg.nltimeout);
	ret = netlink_unicast(nelkskt.sock, skb, pid, 0);
	if(ret < 0){
//		printkerr("netlink send error ret:%d\n",ret);
	}
	return 0;
nlmsg_failure:
	kfree_skb(skb);
	return ret;
}
#else
int nelk_send(pid_t pid, void *data, int datalen, u16 msg_type, u16 msg_flags)
{
    int ret = 0;
    struct sk_buff *skb = NULL;

    struct nlmsghdr *nlh = NULL;
    void  *msg = NULL;

    skb = nlmsg_new(datalen, GFP_KERNEL);
    if (!skb)
        return -ENOMEM;
    nlh = nlmsg_put(skb, pid, 0, msg_type, datalen, msg_flags);
    if (!nlh)
        goto out_kfree_skb;
    msg = nlmsg_data(nlh);
    memcpy(msg, data, datalen);
    ret = nlmsg_unicast(nelkskt.sock, skb, pid);
    if(ret < 0){
		printkerr("netlink unicast(send) error\n");
    }
    return 0;

out_kfree_skb:
    kfree_skb(skb);
    return -1;
}
#endif

int nelk_init(void)
{
	nelkskt.master_pid = 0;
	nelkskt.unit = sec_cfg.netlink?sec_cfg.netlink:31;
	netlink_lock_init(nelkskt.lock);
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,18))
	nelkskt.sock = netlink_kernel_create(nelkskt.unit, 0, nelk_recv, THIS_MODULE); 
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,18)) && (LINUX_VERSION_CODE <= KERNEL_VERSION(3,5,7)) 
	nelkskt.sock = netlink_kernel_create(&init_net, nelkskt.unit, 0, nelk_recv, NULL, THIS_MODULE); 
#else
{
	struct netlink_kernel_cfg nelkcfg;
	memset(&nelkcfg, 0x00, sizeof(nelkcfg));
	nelkcfg.input = nelk_recv;
	nelkskt.sock = netlink_kernel_create(&init_net, nelkskt.unit, &nelkcfg); 
}
#endif
	if(nelkskt.sock == NULL){
		printkerr("create kernel netlink error\n");
		goto err;
	}
	nelkskt.sock->sk_sndtimeo = 10;
	return 0;
err:
	return -1;
}
void nelk_rele(netlink_socket_t *nelk)
{
    if(nelk->sock){
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,18))
        sock_release(nelk->sock->sk_socket);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
        netlink_kernel_release(nelk->sock);
#endif
    }
}
