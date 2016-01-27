#ifndef __HAZE_SIGNAL__
#define __HAZE_SIGNAL__
#include <gtk/gtk.h>
#include <gtk/gtklabel.h>

#define START_REGISTER_SIGNAL
#define END_REGISTER_SIGNAL

#define BIND(target ,event ,func ,args) \
do{	\
	g_signal_connect(G_OBJECT(target) , \
			event,   \
	                G_CALLBACK(func), \
			args); \
}while(0)

void haze_verify_ok(GtkWidget *widget ,gpointer arg);
#endif
