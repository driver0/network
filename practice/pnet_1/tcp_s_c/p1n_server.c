//tcp服务器网络编程
#include <stdio.h> //printf
#include <sys/types.h>
#include <sys/socket.h>// socket bind listen
#include <netinet/in.h> // sockaddr_in
#include <stdlib.h> //exit
#include <arpa/inet.h> //inet_addr inet_atons inet_ntoa
#include <unistd.h> //close
#include <string.h> //strcat

#define N 128

int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
	struct sockaddr_in serveraddr,clientaddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};

	if(argc < 3)
	{
		printf("argument is too few\n");
		exit(1);
	}
	//第一步：创建套接字
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("fail to socket");
		exit(1);
	}

	//第二步：填充服务器网络信息结构体
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	//第三步：将套接字与服务器网络信息结构体进行绑定
	if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	{
		perror("fail to bind");
		exit(1);
	}

	//第四步：将套接字设为监听模式
	if(listen(sockfd,5) < 0)
	{
		perror("fail to listen");
		exit(1);
	}

	//在监听的套接字接收来在客户端的第一次握手连接信号SYN=i
	
	//第五步：阻塞等待客户端的连接请求（进行TCP三次握手的第二次握手，当等到
	//第四步的监听套接字接收到来自客户端的第一次握手连接请求SYN=i时，在第二步的
	//accept这一步作出回应）
    if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen)) < 0)
	{
		perror("fail to accept");
		exit(1);
	}

	while(1)
	{
		if(recv(acceptfd,buf,N,0) < 0)
		{
			perror("fail to recv");
			exit(1);
		}
		
		printf("from client : %s\n",buf);
		strcat(buf,"*_*");

		if(send(acceptfd,buf,N ,0) < 0)
		{
			perror("fail to send");
			exit(1);
		}
	}
	close(sockfd);
	close(acceptfd);
	return 0;
}
