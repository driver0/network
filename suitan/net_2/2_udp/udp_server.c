//udp网络编程服务器
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
	struct sockaddr_in  serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};
	ssize_t bytes;

	//bzero memset
	bzero(&serveraddr,sizeof(serveraddr));
	bzero(&clientaddr,sizeof(serveraddr));

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

	//第二步：填充服务器网络信息结构体
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	//将套接字与服务器网络信息结构体绑定
	if(bind(sockfd,(const struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	{
		errlog("fail to bind");
	}
    
	while(1)
	{
		if((bytes = recvfrom(sockfd,buf,N,0,(struct sockaddr *)&clientaddr,&addlen)) < 0)
		{
			errlog("fail to recvfrom");
		}
		else if(bytes == 0)
		{
			printf("NO DATA\n");
			exit(1);
		}
		else
		{
			printf("%s---> %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
			else
			{
				printf("from client : %s\n",buf);

				strcat(buf,"*_*");
				
				if(sendto(sockfd,buf,N,0,(struct sockaddr *)&clientaddr,addlen) < 0)
				{
					errlog("fail to sendto");
				}	
			}
		}
	}
	
	close(sockfd);

	return 0;
} 
