#ifndef __HAZE_VERIFY__
#define __HAZE_VERIFY__
#include <assert.h>
#include "haze_ui.h"
#include "haze_signal.h"
#include "haze_event.h"

struct haze_gloabl_struct{
	int (*init)(int argc ,char **argv);
	void (*signal)(void);
	void (*cycle)(void);
	void *private;
};

extern struct haze_gloabl_struct hgs;
#endif
