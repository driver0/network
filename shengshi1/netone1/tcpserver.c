//server
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define N 128

int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
	struct sockaddr_in	serveraddr ,clientaddr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};
	ssize_t bytes;

	if(argc < 3)
	{
		printf("argument is too few\n");
		exit(1);
	}

	//第一步：创建套接字
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("fail to socket");
	}

	//第二步：填充服务器网络信息结构体
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	//第三步：将套接字与服务器网络信息结构体绑定
	if((bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in))) < 0 )
	{
		perror("fail to bind");
	}

	//第四步：将套接字设者为监听模式
	if(listen(sockfd,5) < 0)
	{
		perror("fail to listen");
	}

	//第五步：阻塞等待客户的连接请求
	if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen)) < 0)
	{
		perror("fail to accept");
	}

	printf("%s --> %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
	while(1)
	{
		if((bytes = recv(acceptfd,buf,N,0)) < 0)
		{
			perror("fail to recv");
		}
		else if(bytes == 0)
		{
			printf("NO DATA\n");
		}
		else{
			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
			else{
				printf("from client : %s\n",buf);

				fflush(stdout);
				strcat(buf,"**_**");

				if(send(acceptfd,buf,N,0) < 0)
				{
					perror("fail to send");
				}
			}
		}
	}
	close(sockfd);
	close(acceptfd);
	return 0;
}
