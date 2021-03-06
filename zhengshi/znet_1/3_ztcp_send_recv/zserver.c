//tcp网络编程服务器
#include <stdio.h> //printf
#include <arpa/inet.h> //inet_addr (inet_ntoa) htons htonl(ntohs ntonl)
#include <sys/types.h>
#include <sys/socket.h> //socket bind listen accept connect
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h>   //exit
#include <unistd.h>  //close
#include <string.h>  //strcat

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
		exit(1);\
}while(0)

int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
	struct sockaddr_in serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};
	ssize_t bytes;

	if(argc < 3)
	{
		printf("srgument is too few\n");
		exit(1);
	}

	//第一步：创建套接字
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}
//	printf("sockfd = %d\n",sockfd);

	//第二步：填充服务器网络信息结构体
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	//第三步：将套接字与服务器网络信息结构体绑定
	if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	{
		errlog("fail to bind");
	}

	//第四步：将套接字设置为监听模式
	if(listen(sockfd,5) < 0)
	{
		errlog("fail to listen");
	}

	//第五步：阻塞等待客户端的连接请求
	if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addlen)) < 0)
	{
		errlog("fail to accept");
	}

	printf("%s --> %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

	while(1)
	{
		if((bytes = recv(acceptfd,buf,N,0)) < 0)
		{
			errlog("fail to recv");
		}
		else if(bytes == 0)
		{
			printf("NO DATA\n");
			exit(1);
		}
		else
		{
			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
			else
			{
				printf("from client : %s",buf);

				strcat(buf,"**_**");

				if(send(acceptfd,buf,N,0) < 0)
				{
					errlog("fail to send");
				}
			}
		}
	}
	
	close(sockfd);
	close(acceptfd);
	return 0;
}
