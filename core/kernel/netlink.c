#include "netlink.h"

netlink_socket_t nelkskt;
struct nl_condition nc;
struct nl_status_list nl_rtlist;
extern wait_queue_head_t my_queue;
extern atomic_t cond;

static int nelk_recv_helo(struct sk_buff *skb)
{
	int ret = 0;
	pid_t pid = NETLINK_CREDS(skb)->pid;
	netlink_lock(nelkskt.lock);
	nelkskt.master_pid = pid;
	printk("Receive agent hello message [%d]\n", pid);
	netlink_unlock(nelkskt.lock);
	return ret;
}

int _nelk_recv(struct sk_buff *skb)
{
	struct nlmsghdr *nlhdr = NULL;
	struct nl_change_buf *node = NULL ,*tmp = NULL;
	if(skb == NULL){
		printk("netlink recvive error\n");
		return -1;
	}

	nlhdr = (struct nlmsghdr *)(skb->data);

	if(nelkskt.master_pid){
		nelk_recv_helo(skb);
		tmp =  NLMSG_DATA(nlhdr);
		if(tmp){
			printk("recv buf:%s\n" ,tmp);
		}
	}

	if(!(nelkskt.master_pid)){
		nelk_recv_helo(skb);
	}
	return 0;
}
void nelk_recv(struct sk_buff *skb)
{
	if( _nelk_recv(skb) < 0){
		;
	}
	return ;
}

//nelk_send(nelkskt.master_pid, "Hello", 5, 1, 1);
int nelk_send(pid_t pid, void *data, int datalen)
{
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlhdr = NULL;
	char *msg = NULL;
	size_t size = 0;
	int ret = 0;
	if(nelkskt.sock == NULL){
		printk("netlink socket error\n");
		return -1;
	}
	size = (data && datalen) ? datalen:0;
	skb = alloc_skb(NLMSG_SPACE(size), GFP_KERNEL);
	if(skb == NULL){
		printk("netlink send error: no enough memory\n");
		return -ENOMEM;
	}
	nlhdr = NLMSG_PUT(skb, pid, 0, 0, size);
	if(datalen){
		msg = (char *)(NLMSG_DATA(nlhdr));
		if(!msg){
			return -ENOMEM;
		}
		memcpy(msg, data, datalen);
	}
	ret = netlink_unicast(nelkskt.sock, skb, pid, 0);
	if(ret < 0){
		printk("netlink send error ret:%d\n",ret);
	}
	return 0;
nlmsg_failure:
	kfree_skb(skb);
	return ret;
}

int nelk_init(void)
{
	nelkskt.master_pid = 0;
	nelkskt.unit = 31;
	netlink_lock_init(nelkskt.lock);
	nelkskt.sock = netlink_kernel_create(&init_net, nelkskt.unit, 0, nelk_recv, NULL, THIS_MODULE); 

	if(nelkskt.sock == NULL){
		printk("create kernel netlink error\n");
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
		netlink_kernel_release(nelk->sock);
	}
	netlik_lock_destory(nelk->lock);
}
