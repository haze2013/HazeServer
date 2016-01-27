#ifndef __HAZE__
#define __HAZE__

#include "haze.h"
#include "hookfun.h"
#include "netlink.h"

MODULE_LICENSE("GPL");

extern struct nl_condition nc;
extern struct nl_status_list nl_rtlist;
extern wait_queue_head_t my_queue;

static int __init haze_init(void) 
{

#ifdef __HAZE_NF__
	haze_nf_load();
#endif

	init_waitqueue_head(&my_queue);
	if(!haze_lsm_load()){
		printk("register ok\n");
	}

#ifdef __HAZE_NL__
	if(nelk_init() < 0){
		printk("netlink failed\n");
	}
#endif
	INIT_LIST_HEAD(&(nl_rtlist.alist));
	nl_rtlist.num = 0;
	spin_lock_init(&(nl_rtlist.lock));

	spin_lock_init(&(nc.lock));
#ifdef __HAZE_THREAD__
	haze_thread_load();
#endif
	printk("--------------------insmod module.ko----------------------\n");
EXIT:
	return 0;
}


static void __exit haze_exit(void) {
#ifdef __HAZE_NF__
	haze_nf_unload();
#endif

	haze_lsm_unload();

#ifdef __HAZE_NL__
	nelk_rele(&nelkskt);
#endif

#ifdef __HAZE_THREAD__
	haze_thread_unload();
#endif
	printk("--------------------rmmod module.ko----------------------\n");
}

module_init(haze_init);
module_exit(haze_exit); 
MODULE_AUTHOR("HAZE");
MODULE_DESCRIPTION("This is Haze Network system.");

#endif
