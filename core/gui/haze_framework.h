#ifndef __HAZE_VERIFY__
#define __HAZE_VERIFY__
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <gtk/gtk.h>
#include <gtk/gtklabel.h>


struct haze_gloabl_struct{
	int (*init)(int argc ,char **argv);
	void (*signal)(void);
	void (*cycle)(void);
	void *private;
};

extern struct haze_gloabl_struct hgs;
#endif
