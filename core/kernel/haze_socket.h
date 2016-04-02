#ifndef __HAZE_SOCKET__
#define __HAZE_SOCKET__

#include <asm/atomic.h>
#include <asm/checksum.h>
#include <asm/unaligned.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/types.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/skbuff.h>
#include <linux/string.h>
#include <linux/sysctl.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include <linux/route.h>
#include <linux/stddef.h>
#include <linux/mutex.h>
#include <linux/icmp.h>
#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/route.h>
#include <net/inet_connection_sock.h>
#include <net/request_sock.h>

int haze_send_message(struct socket *sock ,char *message ,int len);

int haze_recv_message(struct socket *sock ,char *message ,int len);

void haze_close_socket(struct socket *sock);

struct socket *haze_init_socket(struct socket * ,unsigned int remote ,unsigned int port);

#endif
