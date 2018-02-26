//tcp客户端网络服务器
#include <stdio.h>//printf
#include <arpa/inet.h> //inet_addr htons
#include <sys/types/h>
#include <sys/socket.h> //socket bind listen accept connect 
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strlen

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
	exit(1);\
}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};

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

	//第二步：填充我那刚落
	return 0;
}

