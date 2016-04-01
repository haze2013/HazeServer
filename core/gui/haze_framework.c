#include "haze_framework.h"


struct haze_ui_struct hus;
int haze_init(int argc ,char **argv)
{
	int ret = 0;

	if(!g_thread_supported()){
		 g_thread_init(NULL);
	}
	gdk_threads_init();
	gtk_init(&argc ,&argv);

	//create ui thread
	haze_ui_init(&hus);

	//create work thread
	g_thread_create((GThreadFunc)haze_work_init, &hus, FALSE, NULL);

	return ret;
}

void haze_signal(void)
{
	GTK_REGISTER_SIGNAL_START
		BIND(hus.window ,"destroy" ,haze_main_fun ,NULL);
//	BIND(hus.btn_yes ,"clicked" ,haze_verify_ok ,hus.label);
//	BIND(hus.btn_no ,"clicked" ,haze_verify_ok ,NULL);
	GTK_REGISTER_SIGNAL_END

}

void haze_cycle(void)
{
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
}

struct haze_gloabl_struct hgs = {
	.init = haze_init,
	.signal = haze_signal,
	.cycle = haze_cycle,
	.private = NULL,
};

