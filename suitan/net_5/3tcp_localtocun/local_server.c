//TCP本地网络编程服务器
#include <arpa/inet.h>//inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen connect
#include <stdio.h>//printf
#include <netinet/in.h>//
#include <stdlib.h>//exit
#include <unistd.h>//close
#include <string.h>//strcpy
#include <sys/un.h>//sockaddr_un

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
		        	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
						exit(1);\
					}while(0)

int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
    struct sockaddr_un serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_un);
	char buf[N] = {0};
	int bytes;

	//第一步：创建套接字
	if((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}
	
     //第二步：填充网络信息结构体
	 //inet_addr:将点分十进制IP地址转化为网络能够识别的整形数据
	 //htons:将主机字节序转化为网络字节序
	 //atoi：将数字型字符串转化为整型数据
	 serveraddr.sun_family = AF_UNIX;
	 strcpy(serveraddr.sun_path,"server_socket");


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
	 
     //第五步：阻塞等待客户端的连接请求
	 if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addlen)) < 0)
	 {
		 errlog("fail to accept");
	  }

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
				 printf("from client : %s\n",buf);
				 strcat(buf,"*_*");
				 if(send(acceptfd,buf,N,0) < 0)
				 {
					 errlog("fail to send\n");
				 }
			 }
		 }
	 }
	 close(acceptfd);
	 close(sockfd);

	return 0;
} 
