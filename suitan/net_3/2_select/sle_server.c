//TCP网络编程服务器
#include <arpa/inet.h>//inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen connect
#include <stdio.h>//printf
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>//exit
#include <unistd.h>//close
#include <string.h>//strcat
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
	
     //第二步：填充网络信息结构体
	 //inet_addr:将点分十进制IP地址转化为网络能够识别的整形数据
	 //htons:将主机字节序转化为网络字节序
	 //atoi：将数字型字符串转化为整型数据
	 serveraddr.sin_family = AF_INET;
	 serveraddr.sin_port = htons(atoi(argv[2]));
	 serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

     //第三步：将套接字与网络信息结构体绑定
	 if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	 {
		 errlog("fail to bind");
	 }

     //第四步：将套接字设置为监听模式
	 if(listen(sockfd,5) < 0)
	 {
		 errlog("fail to listen");
	 }
	 
	 //使用select函数实现IO多路复用
	 //同步IO操作，允许一个程序操作多个文件描述符，阻
	 //塞等待附件描述符准备就绪，如果有文件描述符准备
	 //就绪，函数立即返回，并执行相应的IO操作

	 //第一步
	 FD_ZERO(&readfds);

	 maxfd = sockfd;
	 while(1)
	 { 
		 //第二步：将需要的文件描述符添加到集合里面
		 FD_SET(0,&readfds);
		 FD_SET(sockfd,&readfds);

		 //第三步：执行select函数，阻塞等待文件描述符准备就绪
		 if(select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0)
		 {
			 errlog("fail to select");
		 }

		// 第四步：判断准备就绪的文件描述符是否在集合里面，
		// 如果在，立即执行相应的IO操作
		for(i = 0;i <= maxfd; i++)
		{
			if(FD_ISSET(i,&readfds))
			{
				if(i == 0)
				{
					fgets(buf, N, stdin);
					buf[strlen(buf) - 1] = '\0';

					printf("buf >>> %s\n",buf);
				}
				else
				{
					//第五步：阻塞等待客户端的连接请求
					if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addlen)) < 0)
					{
						errlog("fail to accept");
					}
					printf("%s --> %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

				}
			}
		}

	 }

	 close(acceptfd);
	 close(sockfd);

	return 0;
} 
