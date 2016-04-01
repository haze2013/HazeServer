#ifndef __NETLINK_H__
#define __NETLINK_H__

#include "common.h"
#include <net/netlink.h>
#include <linux/skbuff.h>
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

/*
* Netlink data type define
*/
#define SEC_MSG_BASE 0x10
enum NLMSG_TYPE {
    SEC_NLMSG_HELO = SEC_MSG_BASE,
    SEC_NLMSG_QUIT,
    SEC_NLMSG_REQUEST,  //invoke by user  
    SEC_NLMSG_AUDIT,    //invoke by kernel
    SEC_NLMSG_SWINSTALL,
    SEC_NLMSG_PLUGIN,
    SEC_NLMSG_RESPONSE,
    SEC_NLMSG_SUPERINIT,
    SEC_NLMSG_AC,
    SEC_NLMSG_ISPROTECT,
    SEC_NLMSG_EXCEPT,// for gray file update 2014.8.22
    SEC_NLMSG_SSDO,
};
enum NLMSG_FLAG{
    SEC_NLMSG_FLAG_SUCCESS,
    SEC_NLMSG_FLAG_FAILED
};

enum REQUEST_TYPE{
	REQUEST_RELOAD = 1,
	REQUEST_LOGIN  = 2,
	REQUEST_SWITCH = 3,
};

typedef struct kexcept_node{
    int flag;
    char hash[FILE_HASH_SIZE];
    int operation;
}__attribute__((packed)) kexcept_node_t;

typedef struct kexcept_program{
    int n_size;
    int list_type;
    kexcept_node_t nodes[20];
}__attribute__((packed)) kexcepram_t;

int nelk_init(void);
void nelk_rele(netlink_socket_t *nelk);
extern netlink_socket_t nelkskt;
extern int sec_isprotect;
int nelk_send(pid_t pid, void *data, int datalen, u16 msg_type, u16 msg_flags);

#endif//__NETLINK_H__
