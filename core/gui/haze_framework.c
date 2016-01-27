#include "haze_framework.h"
#include "haze_ui.h"
#include "haze_nelk.h"
#include "haze_signal.h"

extern GtkWidget *window;
extern GtkWidget *plus;
extern GtkWidget *minus;
extern GtkWidget *label;
extern int haze_nl_sk;

int haze_init(int argc ,char **argv)
{
	int ret = 0;

	gtk_init(&argc ,&argv);

	ret = haze_nl_init(&haze_nl_sk ,31);
	if(ret < 0){
		g_print("haze netlink failed\n");
		goto EXIT;
	}
	haze_nl_send(haze_nl_sk,"Hello",5);
	haze_ui_init();	
EXIT:
	return ret;
}

void haze_signal(void)
{
	START_REGISTER_SIGNAL
		BIND(window ,"destroy" ,haze_main_fun ,NULL);
		BIND(plus ,"clicked" ,haze_verify_ok ,label);
		BIND(minus ,"clicked" ,haze_verify_ok ,NULL);
	END_REGISTER_SIGNAL
}

void haze_cycle(void)
{
	gtk_main();
}

void haze_exit(void)
{
	haze_nl_exit(haze_nl_sk);
}
struct haze_gloabl_struct hgs = {
	.init = haze_init,
	.signal = haze_signal,
	.cycle = haze_cycle,
	.exit = haze_exit,
	.private = NULL,
};

