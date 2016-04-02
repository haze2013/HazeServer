#ifndef __HAZE_UI__
#define __HAZE_UI__
#include "haze_common.h"

struct haze_ui_struct{
	GtkWidget *window;
	GtkWidget *frame;
	GtkWidget *btn_ok;
	GtkWidget *btn_no;
	GtkWidget *label;
	int enable;
};

void haze_ui_init(struct haze_ui_struct *);
#endif
