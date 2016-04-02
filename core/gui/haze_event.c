#include "haze_event.h"
#include "haze_sock.h"
#include "haze_signal.h"

void haze_work_init(struct haze_ui_struct *arg)
{
	int haze_sock ,haze_client_sk;
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr);
	char msg[1024] = {0};

	haze_init_sock(&haze_sock);
	if(haze_sock < 0){
		g_print("socket init failed\n");
		goto EXIT;
	}
	
	g_print("create socket ok cycle\n");
	while(1){
		haze_client_sk = accept(haze_sock ,(struct sockaddr *)&addr ,&len);
		if(haze_client_sk < 0){
			g_print("accept socket failed:%d\n" ,errno);
			goto EXIT;
		}
		g_print("accept socket ok\n");
		read(haze_client_sk ,msg ,sizeof(msg));
		if(strlen(msg)){
			g_print("hide label\n");
			gdk_threads_enter();
			gtk_label_set_text(arg->label ,(const gchar *)msg);
			gtk_widget_show_all(arg->window);
			gdk_threads_leave();

		}

		haze_close_sock(haze_client_sk);
	}

EXIT:
	haze_close_sock(haze_sock);
	haze_close_sock(haze_client_sk);
	return;
}

