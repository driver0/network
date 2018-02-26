//广播发送者
#include <stdio.h> //printf()
#include <arpa/inet.h> //inet_addr (inet_ntoa) htons htonl (ntohs ntohl)
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen accept connect
#include <netinet/in.h> //sockaddr sockaddr_in
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <string.h> //strlen

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
					printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
					exit(1);\
}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
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
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	
	//第三步：设置允许发送广播权限
	int bool = 1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&bool,sizeof(bool)) <0)
	{
		errlog("fail to setsockopt");
	}
	while(1)
	{
		fgets(buf,N,stdin);
		buf[strlen(buf) - 1] = '\0';

		if(sendto(sockfd,buf,N,0,(struct sockaddr *)&serveraddr,addlen) < 0)
		{
			errlog("fail to sendto");
		}
	}

	close(sockfd);
	return 0;
}
