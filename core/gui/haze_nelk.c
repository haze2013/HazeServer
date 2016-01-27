#ifndef __HAZE_APP__
#define __HAZE_APP__
#include "haze_nelk.h"

int haze_nl_sk = 0;

int haze_nl_init(int *nl_sd ,int protocol)
{
	int ret=0,on;
	struct sockaddr_nl ker_addr;
	*nl_sd = socket(AF_NETLINK,SOCK_RAW,protocol);
	if(!(*nl_sd)){
		perror("socket");
		return(-1);
	}
	memset(&ker_addr,0,sizeof(ker_addr));
	ker_addr.nl_family = AF_NETLINK;
	ker_addr.nl_pid = getpid();
	ker_addr.nl_groups = 0;
	on = 1;
	ret = setsockopt(*nl_sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if(ret == -1){
		perror("setsockopt");
	}
	ret = bind(*nl_sd,(struct sockaddr *)&ker_addr,sizeof(ker_addr));
	if(ret == -1){
		perror("bind error lxsec_init_netlink");
	}
	return ret;
}

int haze_nl_send(int sk_fd,void *emsg,size_t len)
{
	struct nlmsghdr *nlh = NULL;
	struct sockaddr_nl dst_addr;
	int payload_len;
	int rc;
	payload_len = emsg ? (sizeof(struct nlmsghdr) + len):0;
	nlh = malloc(NLMSG_SPACE(payload_len));
	if(!nlh){
		perror("malloc");
		rc = -ENOMEM;
		goto out;
	}
	memset(&dst_addr,0,sizeof(dst_addr));
	dst_addr.nl_family = AF_NETLINK;
	dst_addr.nl_pid = 0;
	dst_addr.nl_groups = 0;

	nlh->nlmsg_len = NLMSG_LENGTH(payload_len);
	nlh->nlmsg_pid = getpid();

	if(payload_len){
		memcpy(NLMSG_DATA(nlh),emsg,payload_len);
	}
	rc = sendto(sk_fd,nlh,nlh->nlmsg_len,0,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
	if(rc < 0){
		printf("send netlink packet to kernel false\n");
		goto out;
	}
out:
	if(nlh)
		free(nlh);
	return rc;
}

int haze_nl_recv(int sk_fd,void **emsg,int *emsg_len)
{
	struct nlmsghdr *nlh = NULL;
	struct sockaddr_nl nladdr;
	socklen_t nladdr_len = sizeof(nladdr);
	int buf_len = sizeof(struct nlmsghdr);
	int pl_len;
	int flags = MSG_PEEK;
	int ret;
receive:
	nlh = (struct nlmsghdr *)realloc(nlh,buf_len);
	if(!nlh){
		ret = -errno;
		printf("Failed to allcate memmory for netlink message:%s\n",strerror(errno));
		return ret;
	}
	ret = recvfrom(sk_fd,nlh,buf_len,flags,(struct sockaddr *)&nladdr,&nladdr_len);
	if(ret < 0){
		ret = -errno;
		printf("Failed to receive netlink header; errno = [%d]; errno msg = [%s]\n", errno, strerror(errno));
		free(nlh);
		return ret;
	}

	if(flags & MSG_PEEK){
		buf_len = nlh->nlmsg_len;
		flags &= ~MSG_PEEK;
		goto receive;
	}
	if(nladdr_len != sizeof(nladdr)) {
		ret = -EPROTO;
		printf("Received invalid netlink message");
		free(nlh);
		return ret;
	}
	if (nladdr.nl_pid) {
		ret = -ENOMSG;
		printf("Received netlink packet from a userspace application;\n\
				pid [%d] may be trying to spoof wsafe netlink packets",nladdr.nl_pid);
		free(nlh);
		return -1;
	}
	pl_len = NLMSG_PAYLOAD(nlh, 0);
	*emsg_len = pl_len;
	*emsg = malloc(pl_len+1);
	if(*emsg == NULL){
		perror("malloc");
		if(nlh){
			free(nlh);
			nlh = NULL;
		}
		return 0;
	}
	memset(*emsg,0,pl_len+1);
	memcpy(*emsg, NLMSG_DATA(nlh), pl_len);
	if(nlh){
		free(nlh);
		nlh = NULL;
	}
	return 1;
}

void haze_nl_exit(int fd)
{
	if(fd > 0){
		close(fd);
	}
}
#if 0
#define OS_NLMSG_REQUEST 12
int main(int argc ,char **argv)
{
	int ret = 0;
	struct nl_change_buf *netlink_bufp = NULL;
	struct nl_change_buf ncb;
	int     netlink_buflen=0;
	int msg_seq, msg_type,msg_flags;
	char iput[1024];
	ret = haze_nl_init(&nl_sk ,31);
	if(ret < 0){
		goto EXIT;
	}

	ret = haze_nl_send(nl_sk, "This is applacation", sizeof("This is applacation")-1, OS_NLMSG_REQUEST, 0, 0);
	if(ret < 0){
		printf("send message failed\n");
		goto EXIT;
	}
	//send message
	while(1){
		ret = haze_nl_recv(nl_sk ,&netlink_bufp, &netlink_buflen, &msg_seq,&msg_type,&msg_flags);
		if(ret != 1){
			printf("recv message failed\n");
			goto EXIT;
		}
		printf("Proces Connect[y/n]:[%5d]:%s\n" ,netlink_bufp->pid ,netlink_bufp->proc);
		#if 0
		ncb.result = 0;
		#else
		memset(iput, 0x00, sizeof(iput));	
		ret = scanf("%s",iput);
		if(ret <= 0){
			ncb.result = -1;
		}
		if(strcmp(iput, "y") == 0)
			ncb.result = 0;
		else
			ncb.result = -1;
		#endif
		ncb.pid = netlink_bufp->pid;
		memset(ncb.proc ,0x00 ,sizeof(ncb.proc));
		memcpy(ncb.proc ,netlink_bufp->proc ,strlen(netlink_bufp->proc));
		ret = haze_nl_send(nl_sk, &ncb, sizeof(struct nl_change_buf), OS_NLMSG_REQUEST, 0, 0);
		if(ret < 0){
			printf("send message failed\n");
			goto EXIT;
		}
		printf("send message success:This is applacation\n");
	}

EXIT:
	if(netlink_bufp){
		free(netlink_bufp);
	}

	if(nl_sk > 0){
		haze_nl_exit(nl_sk);
	}
	return ret;
}
#endif
#endif
