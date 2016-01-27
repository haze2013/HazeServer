#ifndef __HAZE_HOOKFUN__
#define __HAZE_HOOKFUN__

#include "common.h"
#include <linux/atomic.h>

extern wait_queue_head_t my_queue;
extern atomic_t cond;
int haze_lsm_load(void);
void haze_lsm_unload(void);

#endif
