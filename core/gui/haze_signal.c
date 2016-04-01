#include "haze_signal.h"
#include "haze_nelk.h"

void haze_verify_ok(GtkWidget *widget ,gpointer arg)
{
	g_print("button clicked\n");
	gtk_main_quit();
}

void haze_main_fun(GtkWidget *widget ,gpointer arg)
{
	g_print("main window quit\n");
	gtk_main_quit();
}
