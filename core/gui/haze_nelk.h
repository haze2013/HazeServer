#ifndef __HAZE_NELK__
#define __HAZE_NELK__
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include "haze_list.h"

extern int haze_nl_sk;

int haze_nl_init(int *nl_sd ,int protocol);

int haze_nl_send(int sk_fd,void *emsg,size_t len);

int haze_nl_recv(int sk_fd,void **emsg,int *emsg_len);

void haze_nl_exit(int fd);


#endif
