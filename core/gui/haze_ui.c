#include "haze_ui.h"

void haze_ui_init(struct haze_ui_struct *arg)
{
	arg->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(arg->window) ,"Trust Verify:");
	gtk_window_set_default_size(GTK_WINDOW(arg->window) ,250 ,180);
	gtk_window_set_position(GTK_WINDOW(arg->window) ,GTK_WIN_POS_CENTER);

	arg->frame = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(arg->window) ,arg->frame);
       
	arg->btn_ok = gtk_button_new_with_label("Yes");
	gtk_widget_set_size_request(arg->btn_ok ,80 ,35);
	gtk_fixed_put(GTK_FIXED(arg->frame) ,arg->btn_ok ,30 ,120);
 
	arg->btn_no = gtk_button_new_with_label("No");
	gtk_widget_set_size_request(arg->btn_no ,80 ,35);
	gtk_fixed_put(GTK_FIXED(arg->frame) ,arg->btn_no ,140 ,120);

	arg->label = gtk_label_new("Proc info:");
        gtk_fixed_put(GTK_FIXED(arg->frame) ,arg->label ,30 ,20);

        arg->label = gtk_label_new("");
        gtk_fixed_put(GTK_FIXED(arg->frame) ,arg->label ,20 ,50);
        gtk_label_set_text(GTK_LABEL(arg->label) ,(const gchar *)("this is haze world."));
	arg->enable = 1;
	//gtk_widget_show_all(arg->window);
}



