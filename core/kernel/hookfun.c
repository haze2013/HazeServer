#ifndef __HOOKFUN_C_
#define __HOOKFUN_C_

#include "haze.h"
#include "hookfun.h"
#include "netlink.h"
#include "haze_path.h"

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

struct proto_ops           *inet_stream_ops_hooks = NULL;
struct proto_ops           *inet_dgram_ops_hooks = NULL;
struct security_operations *haze_lsm_hooks = NULL;

//for key hook
ssize_t (*haze_sys_tty_read)(struct file *, char __user *, size_t , loff_t *);
ssize_t haze_tty_read(struct file *, char __user *, size_t count, loff_t *ppos);

ssize_t haze_tty_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	ssize_t ret;
	ret = haze_sys_tty_read(file ,buf ,count ,ppos);
	printk("[%s]-[%d]\n",__FUNCTION__,__LINE__);
	return ret;
}

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

struct proc_info{
	int serial;
	int len;
	char proc[256];
};

struct proc_info_cache{
	unsigned long time;
	struct proc_info store[5];
};

#define CACHE_FIND 0
#define CACHE_NOFIND 1
#define CACHE_MAX 5

int check_pic_cache(char *proc)
{
	static int index = 0;
	int cur = 0 ,ret = CACHE_NOFIND ,cur_serial;
	static struct proc_info_cache pic;
	
	cur_serial = index;
	while(cur <= cur_serial){
		if(pic.store[cur].len &&
				!memcmp(pic.store[cur].proc ,proc ,pic.store[cur].len)){
	//		printk("find cache :%s len:%d serial:%d\n" ,proc ,pic.store[cur].len ,pic.store[cur].serial);
			ret = CACHE_FIND;
			goto EXIT;
		}
		cur++;
	}
	if(index > CACHE_MAX){
		index = 0;
	}	

	pic.time = jiffies;
	pic.store[index].serial = index;
	pic.store[index].len = strlen(proc);
	memset(pic.store[index].proc ,0x00 ,sizeof(pic.store[index].proc));
	memcpy(pic.store[index].proc ,proc ,strlen(proc));
	printk("cache[%d]:proc:%s len:%d time:%x\n" ,pic.store[index].serial ,
			pic.store[index].proc ,pic.store[index].len ,pic.time);
	index++;
EXIT:
	return ret;
}

extern netlink_socket_t nelkskt;
wait_queue_head_t my_queue;
atomic_t cond = ATOMIC_INIT(0);

#define PROC_NOFIND -10
int haze_show_verifylist(pid_t pid)
{
	struct list_head *head = &(nl_rtlist.alist);
	struct list_head *pos = NULL,*tmp;
	struct nl_change_buf *node = NULL;

	printk("list num:%d\n" ,nl_rtlist.num);
	spin_lock(&(nl_rtlist.lock));
	list_for_each_safe(pos,tmp,head){
		node = list_entry(pos, struct nl_change_buf, list);
		printk("find verify:%d proc:%s pid:%d target pid:%d\n" ,node->result ,node->proc ,node->pid ,pid);
	}
	spin_unlock(&(nl_rtlist.lock));
	return 0;
}
int haze_check_verify(pid_t pid ,int *result)
{
	struct list_head *head = &(nl_rtlist.alist);
	struct list_head *pos = NULL,*tmp;
	struct nl_change_buf *node = NULL;
	int ret = PROC_NOFIND;

	spin_lock(&(nl_rtlist.lock));
	list_for_each_safe(pos,tmp,head){
		node = list_entry(pos, struct nl_change_buf, list);
		if(node->pid == pid){
			*result = node->result;
			ret = node->result;
			printk("find verify:result:%d pid:%d target pid:%d\n" ,node->result ,node->pid ,pid);
			list_del(pos);
			kfree(node);
			nl_rtlist.num--;
			goto EXIT;
		}
	}
	spin_unlock(&(nl_rtlist.lock));
EXIT:
	spin_unlock(&(nl_rtlist.lock));
	return ret;
}

int haze_exit_verify(void)
{
	struct list_head *head = &(nl_rtlist.alist);
	struct list_head *pos = NULL,*tmp;
	struct nl_change_buf *node = NULL;
	int ret = PROC_NOFIND;

	list_for_each_safe(pos,tmp,head){
		node = list_entry(pos, struct nl_change_buf, list);
		list_del(pos);
		kfree(node);
		nl_rtlist.num--;		
	}

EXIT:
	return ret;
}

#define VERIFY_PROC_TIMEOUT (3 * HZ)
int haze_socket_connect(struct socket *sock,struct sockaddr *address, int addrlen)
{
	char *p = NULL;
	int ret = 0;
	char path[256] = {0};

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
#if 0
	if(!check_pic_cache(p)){
		goto EXIT;
	}
#endif
	if(!(nelkskt.master_pid)){
		goto EXIT;
	}
	if(nelkskt.master_pid){
		nelk_send(nelkskt.master_pid, "Hello", 5);
	}
EXIT:
	return ret;
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
	haze_exit_verify();
}

#endif
