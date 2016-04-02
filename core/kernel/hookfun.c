#ifndef __HOOKFUN_C_
#define __HOOKFUN_C_

#include "haze.h"
#include "hookfun.h"
#include "haze_path.h"
#include "haze_socket.h"

int (*sys_socket_post_connect)(struct socket *sock,struct sockaddr *vaddr,int sockaddr_len, int flags);
int (*sys_socket_post_accept)(struct socket *,struct socket *, int);
int (*sys_socket_post_sendmsg)(struct kiocb *,struct socket *,struct msghdr *,size_t);
int (*sys_socket_post_recvmsg)(struct kiocb *iocb, struct socket *sock,struct msghdr *, size_t,int);

int (*lsm_socket_connect) (struct socket *sock,struct sockaddr *address, int addrlen);
static int (*lsm_alloc_security)(struct sock *sk, int family, gfp_t priority);
static void (*lsm_free_security)(struct sock *sk);

int haze_socket_connect(struct socket *sock,struct sockaddr *address, int addrlen);
int haze_sk_alloc(struct sock *sk, int family, gfp_t priority);
void haze_sk_free(struct sock *sk);

struct security_operations *haze_lsm_hooks = NULL;

int haze_sk_alloc(struct sock *sk, int family, gfp_t priority)
{
	struct haze_task_info *hti = NULL;
        hti = kmalloc(sizeof(struct haze_task_info), priority);
        if(hti == NULL){
                printk("[%s] kmalloc error\n",__FUNCTION__);
                return -ENOMEM;
        }
        sk->sk_security = hti;
        return 0;
}
void haze_sk_free(struct sock *sk)
{
        if(sk->sk_security){
                kfree(sk->sk_security);
        }
	sk->sk_security = NULL;
}

int haze_socket_connect(struct socket *sock,struct sockaddr *address, int addrlen)
{
	char *p = NULL;
	struct socket *haze_sk = NULL;
	struct sockaddr_in *sp = (struct sockaddr_in *)address;
	struct sockaddr_in addr;
	int ret = 0;
	char path[256] = {0};
	char message[1024] = {0};

	if(sock == NULL){
		goto EXIT;
	}
	if(sock->sk == NULL){
		goto EXIT;
	}

	if(sock->type != SOCK_STREAM){
		goto EXIT;
	}

	if(sock->sk->sk_family != AF_INET){
        	goto EXIT;
	}

	p = sec_task_path(current, path, sizeof(path));
	if(!p){
		goto EXIT;
	}
	//socket 
	ret = sock_create_kern(PF_INET ,SOCK_STREAM ,IPPROTO_TCP ,&haze_sk);
	if(ret < 0){
		goto EXIT;
	}

	haze_sk->sk->sk_reuse = 1;

	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family     = AF_INET;
	addr.sin_addr.s_addr     = htonl(0x7f000001);
	addr.sin_port        = htons(8888);

	if(haze_sk->ops->connect){
		ret = haze_sk->ops->connect(haze_sk,
				(struct sockaddr *)&addr,
				sizeof(struct sockaddr), 0);
		if(ret < 0){
			printk("connect failed:%d\n" ,ret);
			goto EXIT;
		}
	}
	printk("send msg:%s\n" ,p);
	sprintf(message ,"%s->%x:%d" ,p ,sp->sin_addr ,sp->sin_port);
	haze_send_message(haze_sk ,message ,strlen(message));
//	haze_recv_message(haze_sk ,message ,sizeof(message));
//	printk("recv:%s\n" ,message);
EXIT:
	haze_close_socket(haze_sk);
	return 0;
}

int haze_lsm_load(void)
{
	unsigned long cr0;
	haze_lsm_hooks =find_struct_address("security_ops" ,0);
	if(haze_lsm_hooks == NULL){
		printk("find failed\n");
		return -1;
	}

#ifndef _XEN_
	cr0 = clear_and_return_cr0();
#endif
	/********************
	 *load LSM hook	
	 *****************/
	lsm_alloc_security = haze_lsm_hooks->sk_alloc_security;
	haze_lsm_hooks->sk_alloc_security = haze_sk_alloc;

	lsm_free_security = haze_lsm_hooks->sk_free_security;
	haze_lsm_hooks->sk_free_security = haze_sk_free;

	lsm_socket_connect = haze_lsm_hooks->socket_connect;
	haze_lsm_hooks->socket_connect = haze_socket_connect;
#ifndef _XEN
        setback_cr0(cr0);
#endif

	return 0;
}
void haze_lsm_unload(void)
{
	unsigned long cr0;
#ifndef _XEN_
	cr0 = clear_and_return_cr0();
#endif
	if(haze_lsm_hooks != NULL){
		haze_lsm_hooks->socket_connect = lsm_socket_connect;
		haze_lsm_hooks->sk_alloc_security = lsm_alloc_security;
		haze_lsm_hooks->sk_free_security = lsm_free_security;
	}
#ifndef _XEN_
	setback_cr0(cr0);
#endif
}

#endif
