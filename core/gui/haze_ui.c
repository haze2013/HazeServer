#include "haze_ui.h"

GtkWidget *window ,*frame ,*plus ,*minus;
GtkWidget *label;

void haze_ui_init(void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window) ,"Trust Verify:");
	gtk_window_set_default_size(GTK_WINDOW(window) ,250 ,180);
	gtk_window_set_position(GTK_WINDOW(window) ,GTK_WIN_POS_CENTER);

	frame = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(window) ,frame);
        
	plus = gtk_button_new_with_label("Yes");
	gtk_widget_set_size_request(plus ,80 ,35);
	gtk_fixed_put(GTK_FIXED(frame) ,plus ,30 ,120);

	minus = gtk_button_new_with_label("No");
	gtk_widget_set_size_request(minus ,80 ,35);
	gtk_fixed_put(GTK_FIXED(frame) ,minus ,140 ,120);

	label = gtk_label_new("Proc info:");
        gtk_fixed_put(GTK_FIXED(frame) ,label ,30 ,20);

        label = gtk_label_new("");
        gtk_fixed_put(GTK_FIXED(frame) ,label ,60 ,50);
        gtk_label_set_text(GTK_LABEL(label) ,(const gchar *)("this is haze world."));
        gtk_widget_show_all(window);
}


