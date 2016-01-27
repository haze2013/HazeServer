#include "haze_signal.h"
#include "haze_nelk.h"

extern int haze_nl_sk;

void haze_verify_ok(GtkWidget *widget ,gpointer arg)
{
	g_print("button clicked\n");
	gtk_widget_show_all(widget);
	gtk_main_quit();
}

void haze_main_fun(GtkWidget *widget ,gpointer arg)
{
	char msg[128] = {0};

	haze_nl_recv(haze_nl_sk ,msg ,sizeof(msg));
	g_print("recv :%s\n" ,msg);
	gtk_main_quit();
}
