//组播发送者
#include <stdio.h> //printf
#include <arpa/inet.h> //inet_addr (inet_ntoa) htons htonl (ntohs ntohl)
#include <sys/types.h>
#include <sys/socket.h> //socket bind listen accept connect
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <string.h> //strlen 

#define N 128
#define errlog(errmsg) do{perror("errmsg");\
	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
	exit(1);\
}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in groupcastaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N]={0};

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

	//第二步：填充组播信息结构体
	//inet_addr:将点分十进制的IP地址转化为网络能够识别的整型数据
	//htons:将主机字节序转化为网络字节序
	//atoi :将数字型字符串转化为整型数据
	groupcastaddr.sin_family = AF_INET;
	groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
	groupcastaddr.sin_port = htons(atoi(argv[2]));

	while(1)
	{
		fgets(buf,N,stdin);
		buf[strlen(buf) - 1] = '\0';

		if(sendto(sockfd,buf,N,0,(struct sockaddr *)&groupcastaddr,addlen) < 0)
		{
			errlog("fail to sendto");
		}
	}

	close(sockfd);
	return 0;
}
