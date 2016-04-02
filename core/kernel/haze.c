#ifndef __HAZE__
#define __HAZE__

#include "haze.h"
#include "hookfun.h"

MODULE_LICENSE("GPL");

static int __init haze_init(void) 
{

#ifdef __HAZE_NF__
	haze_nf_load();
#endif

	if(!haze_lsm_load()){
		printk("register ok\n");
	}

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
