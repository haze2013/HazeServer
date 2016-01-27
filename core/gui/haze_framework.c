#include "haze_framework.h"
#include "haze_nelk.h"

GtkWidget *window ,*frame ,*plus ,*minus;
GtkWidget *label;

void haze_ui_init(void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window) ,"可信网络进程验证");
	gtk_window_set_default_size(GTK_WINDOW(window) ,250 ,180);
	gtk_window_set_position(GTK_WINDOW(window) ,GTK_WIN_POS_CENTER);

	frame = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(window) ,frame);
        
	plus = gtk_button_new_with_label("放行");
	gtk_widget_set_size_request(plus ,80 ,35);
	gtk_fixed_put(GTK_FIXED(frame) ,plus ,30 ,120);

	minus = gtk_button_new_with_label("阻止");
	gtk_widget_set_size_request(minus ,80 ,35);
	gtk_fixed_put(GTK_FIXED(frame) ,minus ,140 ,120);

	label = gtk_label_new("进程访问详细:");
        gtk_fixed_put(GTK_FIXED(frame) ,label ,30 ,20);
        gtk_widget_show_all(window);

        label = gtk_label_new("");
        gtk_fixed_put(GTK_FIXED(frame) ,label ,60 ,50);
        gtk_label_set_text(GTK_LABEL(label) ,(const gchar *)("this is haze world."));
        gtk_widget_show_all(window);
}

int haze_init(int argc ,char **argv)
{
	int ret = 0;

	gtk_init(&argc ,&argv);
	haze_ui_init();	
	
	ret = haze_nl_init(&haze_nl_sk ,31);
	if(ret < 0){
		g_print("haze netlink failed\n");
		goto EXIT;
	}
EXIT:
	return ret;
}

void haze_verify_ok(GtkWidget *widget ,gpointer arg)
{
	g_print("button clicked\n");
      //  gtk_main_quit();
}

void haze_signal(void)
{
	g_signal_connect(G_OBJECT(window) ,"destroy" ,
                        G_CALLBACK(gtk_main_quit) ,NULL);
        g_signal_connect(G_OBJECT(plus) ,"clicked" ,
                        G_CALLBACK(haze_verify_ok) ,label);
        g_signal_connect(G_OBJECT(minus) ,"clicked" ,
                        G_CALLBACK(haze_verify_ok) ,NULL);
}

void haze_cycle(void)
{
	gtk_main();
}

struct haze_gloabl_struct hgs = {
	.init = haze_init,
	.signal = haze_signal,
	.cycle = haze_cycle,
	.private = NULL,
};

