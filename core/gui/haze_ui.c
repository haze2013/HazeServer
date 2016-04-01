#include "haze_ui.h"

void haze_ui_init(struct haze_ui_struct *arg)
{
	arg->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(arg->window) ,"Trust Verify:");
	gtk_window_set_default_size(GTK_WINDOW(arg->window) ,250 ,180);
	gtk_window_set_position(GTK_WINDOW(arg->window) ,GTK_WIN_POS_CENTER);

	arg->frame = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(arg->window) ,arg->frame);
        
	arg->label = gtk_label_new("Proc info:");
        gtk_fixed_put(GTK_FIXED(arg->frame) ,arg->label ,30 ,20);

        arg->label = gtk_label_new("");
        gtk_fixed_put(GTK_FIXED(arg->frame) ,arg->label ,60 ,50);
        gtk_label_set_text(GTK_LABEL(arg->label) ,(const gchar *)("this is haze world."));
	arg->enable = 1;
	gtk_widget_show_all(arg->window);
}



