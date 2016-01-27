#include "haze_framework.h"
#include "haze_ui.h"
#include "haze_nelk.h"
#include "haze_signal.h"

int haze_init(int argc ,char **argv)
{
	int ret = 0;

	gtk_init(&argc ,&argv);
	haze_ui_init();	
#if 0	
	ret = haze_nl_init(&haze_nl_sk ,31);
	if(ret < 0){
		g_print("haze netlink failed\n");
		goto EXIT;
	}
#endif
EXIT:
	return ret;
}

extern GtkWidget *window;
extern GtkWidget *plus;
extern GtkWidget *label;

void haze_signal(void)
{
	START_REGISTER_SIGNAL
		BIND(window ,"destroy" ,gtk_main_quit ,NULL);
		BIND(plus ,"clicked" ,haze_verify_ok ,label);
//		BIND(minus ,"clicked" ,haze_verify_ok ,NULL);
	END_REGISTER_SIGNAL
}

void haze_cycle(void)
{
	gtk_main();
}

struct haze_gloabl_struct hgs = {
	.init = haze_init,
	.signal = haze_signal,
	.cycle = haze_cycle,
	.private = NULL,
};

