//tcp网络编程服务器
#include <stdio.h> //printf()
#include <arpa/inet.h> //inet_addr (inet_ntoa) htons htonl (ntohs ntohl)
#include <sys/types.h>
#include <sys/socket.h> //socket bind listen accept connect
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <string.h> //strcat
#include <sys/select.h>
#include <sys/time.h>

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
	fd_set readfds;
	int maxfd;
	int i = 0;
	int ret = 0;
	ssize_t bytes;
	fd_set tempfds;

	if(argc < 3)
	{
		printf("argument is too few\n");
		exit(1);
	}

	//第一步：创建套接字
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}

	printf("sockfd = %d\n",sockfd);

	//第二步：填充网络信息结构体
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	//第三步：将套接字与网络信息结构提绑定
	if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	{
		errlog("fail to bind");
	}

	//第四步：将套接字设置为监听模式
	if(listen(sockfd,5) < 0)
	{
		errlog("fail to listen");
	}

	//使用select函数实现tcp并发服务器
	
	//第一步：清空集合
	FD_ZERO(&readfds);

	//第二步：将需要的文件描述符添加到集合里面
	FD_SET(sockfd,&readfds);

	maxfd = sockfd;
	while(1)
	{
		tempfds = readfds;

		//第三步：执行select函数，阻塞等待文件描述符准备就绪
		if((ret = select(maxfd + 1,&tempfds,NULL,NULL,NULL)) < 0)
		{
			errlog("fail to select");
		}

		//第四步：判断准备就绪的文件描述符是否在集合里面，如果在
		//立即执行相应的操作
		for(i = 0; i <= maxfd; i++)
		{
			if(FD_ISSET(i, &tempfds))
			{
				if(i == sockfd)
				{
					//第五步：阻塞等待客户端的连接请求
					if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addlen)) < 0)
					{
						errlog("fail to accept");
					}

					printf("%s --> %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
					printf("acceptfd = %d\n",acceptfd);

					//需要将acceptfd添加到集合里面
					FD_SET(acceptfd,&readfds);

					//确定最大的文件描述符
					maxfd = acceptfd >maxfd ? acceptfd:maxfd;
				}
				else
				{
					if((bytes = recv(i,buf,N,0)) < 0)
					{
						errlog("fail to recv");
					}
					else if(bytes == 0)
					{
						printf("NO DATA\n");
						FD_CLR(i,&readfds);
						close(i);

						break;
					}
					else
					{
						if(strncmp(buf,"quit",4) == 0)
						{
							FD_CLR(i,&readfds);
							close(i);
							break;
						}
						else
						{
							printf("from client : %s\n",buf);

							strcat(buf,"*_*");

							if(send(i, buf, N, 0) < 0)
							{
								errlog("fail to send");
							}
						}
					}
				}
			}
		}
	}
	close(acceptfd);
	close(sockfd);

	return 0;
}
