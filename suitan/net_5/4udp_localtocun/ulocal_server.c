//TCP本地网络编程服务器
#include <arpa/inet.h>//inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen connect
#include <sys/un.h>//sockaddr_un
#include <stdio.h>//printf
#include <stdlib.h>//exit
#include <unistd.h>//close
#include <string.h>//strcpy

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
		        	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
						exit(1);\
					}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
    struct sockaddr_un serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_un);
	char buf[N] = {0};
	ssize_t bytes;

	//第一步：创建套接字
	if((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}
	
     //第二步：填充网络信息结构体
	 serveraddr.sun_family = AF_UNIX;
	 strcpy(serveraddr.sun_path,"server_socket");


     //第三步：将套接字与网络信息结构体绑定
	 if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	 {
		 errlog("fail to bind");
	 }


	 while(1)
	 {
		 if((bytes = recvfrom(sockfd,buf,N,0,(struct sockaddr *)&clientaddr,&addlen)) < 0)
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
				 printf("from client : %s\n",buf);
				 strcat(buf,"*_*");
				 if(sendto(sockfd,buf,N,0,(struct sockaddr)&clientaddr,addlen) < 0)
				 {
					 errlog("fail to send");
				 }
			 }
		 }
	 }
	 close(sockfd);

	return 0;
} 
