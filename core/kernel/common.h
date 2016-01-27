#ifndef __COMMON_FILTER__
#define __COMMON_FILTER__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <net/sock.h>
#include <linux/skbuff.h>
#include <net/inet_sock.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/security.h>
#include <linux/version.h>
#include <linux/kprobes.h>
#include <linux/file.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/processor.h>
#include <asm/page.h>
#include <linux/utsname.h>
#include <linux/vmalloc.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/ratelimit.h>

struct haze_task_info {
        char path[256];
	char remain[0];
};

char *replace(char *buf, int len, char org, char dest);
unsigned long  clear_and_return_cr0(void);
void  setback_cr0(unsigned long val);
unsigned long find_symbol_addr(char *symname);
void *find_struct_address(char *func_name ,int flag);

#endif
