#include "haze_signal.h"

void haze_verify_ok(GtkWidget *widget ,gpointer arg)
{
	g_print("button clicked\n");
	gtk_main_quit();
}
