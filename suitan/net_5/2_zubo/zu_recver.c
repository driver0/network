//组播接收者
#include <stdio.h>//printf()
#include <arpa/inet.h>//htons (ntohs) inet_addr (inet_ntoa)
#include <sys/types.h>
#include <sys/socket.h>//socket bind recvfrom sendto 
#include <netinet/in.h>//sockaddr sockaddr_in
#include <stdlib.h>//exit
#include <unistd.h>//close
#include <string.h>
#include <strings.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
		printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
		exit(1);\
}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in  groupcastaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};

	if(argc < 3)
	{
		printf("argument is too few\n");
		exit(1);
	}

	//第一步：创建套接字
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		errlog("fail to socket");
	}

	//第二步：填充广播信息结构体
	groupcastaddr.sin_family=AF_INET;
	groupcastaddr.sin_port = htons(atoi(argv[2]));
	groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);

	//第三步：将套接字与广播信息结构体绑定
	if(bind(sockfd,(const struct sockaddr *)&groupcastaddr,sizeof(groupcastaddr)) < 0)
	{
		errlog("fail to bind");
	}
    
	//第四步：加入多播组
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)
	{
		errlog("fail to setsockopt");
	}

	while(1)
	{
		if(recvfrom(sockfd,buf,N,0,NULL,NULL) < 0)
		{
			errlog("fail to recvfrom");
		}
		printf("from zubo : %s\n",buf);
	}
	
	close(sockfd);

	return 0;
} 
