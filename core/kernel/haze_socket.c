#include "haze_socket.h"

int haze_send_message(struct socket *sock ,char *message ,int len)
{
	struct msghdr msg;
	struct kvec vec;

	vec.iov_base = message;
	vec.iov_len = len;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	return kernel_sendmsg(sock ,&msg ,&vec ,1, len);
}

int haze_recv_message(struct socket *sock ,char *message ,int len)
{
	struct msghdr msg;
	struct kvec vec;

	vec.iov_base = message;
	vec.iov_len = len;
	msg.msg_flags = 0;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	return kernel_recvmsg(sock ,&msg ,&vec ,1, len ,0);

}

void haze_close_socket(struct socket *sock)
{
	if(sock){
		sock_release(sock);
		sock = NULL;
	}
}
struct socket *haze_init_socket(struct socket *sock ,unsigned int remote ,unsigned int port)
{
	int ret = 0;
	struct sockaddr_in addr;

	ret = sock_create_kern(PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	if (ret < 0) {
		printk("sock_create_kern\n");
		goto EXIT;
	}

	sock->sk->sk_reuse = 1;

	memset(&addr, 0x00, sizeof(addr));    
	addr.sin_family     = AF_INET;
	addr.sin_addr.s_addr     = htonl(remote);
	addr.sin_port        = htons(port);
	if(sock && sock->ops->connect){
		ret = sock->ops->connect(sock,
				(struct sockaddr *)&addr,
				sizeof(struct sockaddr), 0);
		if(ret < 0){
			printk("connect failed:%d\n" ,ret);
			goto EXIT;
		}
	}
EXIT:
	haze_close_socket(sock);
	return sock;
}
