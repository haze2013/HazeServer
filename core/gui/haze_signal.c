#include "haze_signal.h"

void haze_verify_ok(GtkWidget *widget ,gpointer arg)
{
	GtkWidget *window = (GtkWidget *)arg;
	g_print("button clicked\n");
	gtk_widget_hide_all(window);	
}

void haze_verify_no(GtkWidget *widget ,gpointer arg)
{
	GtkWidget *window = (GtkWidget *)arg;
	g_print("button clicked\n");
	gtk_widget_hide_all(window);	
}

void haze_main_fun(GtkWidget *widget ,gpointer arg)
{
	g_print("main window quit\n");
	gtk_widget_hide_all(widget);	
//	gtk_main_quit();
}
