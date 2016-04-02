#ifndef __HAZE_SIGNAL__
#define __HAZE_SIGNAL__
#include "haze_common.h"
#include "haze_ui.h"

#define GTK_REGISTER_SIGNAL_START  \
do{ \
	if(hus.enable){

#define GTK_REGISTER_SIGNAL_END  \
	} \
}while(0);	
	

#define BIND(target ,event ,func ,args) \
do{	\
	g_signal_connect(G_OBJECT(target) , \
			event,   \
	                G_CALLBACK(func), \
			args); \
}while(0)

void haze_main_fun(GtkWidget *widget ,gpointer arg);
void haze_verify_ok(GtkWidget *widget ,gpointer arg);
void haze_verify_no(GtkWidget *widget ,gpointer arg);


#endif
