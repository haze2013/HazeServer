#ifndef __HAZE_SOCKET__
#define __HAZE_SOCKET__

#include <sys/types.h>       
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int haze_init_sock(int *sk);

void haze_close_sock(int sock);
#endif
