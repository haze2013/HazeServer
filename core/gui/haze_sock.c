#include "haze_sock.h"

int haze_init_sock(int *sk)
{
	int ret = 0;
	struct sockaddr_in servaddr;

	*sk = socket(PF_INET, SOCK_STREAM, 0);
	if(*sk < 0){
		ret = *sk;
		goto EXIT;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(8888);

	ret = bind(*sk, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(ret < 0){
		goto EXIT;
	}

	ret = listen(*sk, 10);
	if(ret < 0){
		printf("listen failed");
		goto EXIT;
	}
EXIT:
	return ret;
}

void haze_close_sock(int sk)
{
	if(sk > 0){
		close(sk);
	}
}
