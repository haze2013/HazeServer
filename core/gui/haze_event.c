#include "haze_event.h"

void haze_work_init(struct haze_ui_struct *arg)
{
	gint i = 0;

	while(1){
		g_print("ui disabled:%d\n" ,arg->enable);
		sleep(1);
		if((i++)%2){
			gtk_widget_show_all(arg->label);
		}else{
			gtk_widget_hide_all(arg->label);
		}
		g_print("hide failed\n");
	}
}

