#ifndef __NETLINK_H__
#define __NETLINK_H__

#include <net/netlink.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <net/sock.h>
#include  <linux/mutex.h>

#define netlink_lock_init(lock) do{ \
    mutex_init(&(lock)); \
}while(0)
#define netlink_lock(lock) do{ \
    mutex_lock(&(lock)); \
}while(0)
#define netlink_unlock(lock) do{ \
    mutex_unlock(&(lock)); \
}while(0)
#define netlik_lock_destory(lock) do{ \
}while(0)

typedef struct netlink_socket_{
    struct sock *sock; 
    int unit;
    pid_t master_pid;
    struct mutex lock;
} netlink_socket_t;

struct nl_change_buf{
	struct list_head list;
	int pid;
	int result;
	char proc[256];
};

struct nl_status_list{
	struct list_head alist;
	int num;
	spinlock_t  lock;
};

struct nl_condition{
	int cond;
	spinlock_t lock;
};

extern struct nl_condition nc;
extern netlink_socket_t nelkskt;
extern struct nl_status_list nl_rtlist; 

int nelk_init(void);
void nelk_rele(netlink_socket_t *nelk);
int nelk_send(pid_t pid, void *data, int datalen);

#endif//__NETLINK_H__
