//tcp客户端网络编程
#include <stdio.h> //printf perror
#include <sys/types.h> 
#include <sys/socket.h> //socket 
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit
#include <string.h> //strlen strcat
#include <unistd.h> //close

#define N 128

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
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

	//第三步：请求连接（进行TCP三次握手的第一次握手，向服务器的监听套接字发送SYN=i的信号）
	if(connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	{
		perror("fail to connect");
		exit(1);
	}

	while(1)
	{
   		fgets(buf,N,stdin);
		buf[strlen(buf) -1] = '\0';

		if(send(sockfd,buf,N,0) < 0)
		{
			perror("fail to send");
			exit(1);
		}
		
		if(recv(sockfd,buf,N,0) < 0)
		{
			perror("fail to recv");
			exit(1);
		}

		printf("from server : %s\n",buf);
	}

	close(sockfd);
	return 0;
}
