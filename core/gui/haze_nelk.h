#ifndef __NETLINK_H__
#define __NETLINK_H__

#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/net.h>
#include <net/sock.h>

/*
* Define muxtex lock for netlink
*/
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

int nelk_init(void);
void nelk_rele(netlink_socket_t *nelk);
extern netlink_socket_t nelkskt;
int nelk_send(pid_t pid, void *data, int datalen, u16 msg_type, u16 msg_flags);

#endif//__NETLINK_H__
