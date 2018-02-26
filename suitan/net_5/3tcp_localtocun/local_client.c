//tcp本地网络编程客户端
#include <stdio.h>//printf
#include <arpa/inet.h>//inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen accept connect
#include <netinet/in.h>//
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strlen
#include <sys/un.h>//struct sockaddr_un

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
						exit(1);\
 						}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_un serveraddr;
	socklen_t addrlen = sizeof(struct sockaddr_un);
	char buf[N] = {0};

	//第一步：创建套接字
	if((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}	
	
	//第二步：填充本地信息结构体
	//inet_addr:将点分十进制IP地址转化为网络能够识别的整形数据
	//htons:将主机字节序转化为网络字节序
	//atoi:将数字型字符串转化为整形数据
	 serveraddr.sun_family = AF_UNIX;
	 strcpy(serveraddr.sun_path,"server_socket");
	
	 //第五步：发送客户端的连接请求
	 if(connect(sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
	 {
		 errlog("fail to accept");
	 }

	 while(1)
	 {
		 fgets(buf,N,stdin);
		 buf[strlen(buf) - 1] = '\0';

		 if(send(sockfd,buf,N,0) < 0)
		 {
			 errlog("fail to send");
		 }
		 else
		 {
			 if(recv(sockfd,buf,N,0) < 0)
			 {
				 errlog("fail to recv");
			 }
			 printf("from server : %s\n",buf);
		 }
	 }
	 close(sockfd);

	return 0;
} 
