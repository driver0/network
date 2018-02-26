//client
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h>
#include <unistd.h> //close
#include <string.h> //strlen

#define N 128
/*#define errlog(errmsg)	do{perror(errmsg);\
	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
	exit(1);\
}while(0)
*/
int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in	serveraddr;
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

	//第二步：填充服务器网络信息结构提信息
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	//第五步：发送客户端的连接请求
	if((connect(sockfd,(struct sockaddr *)&serveraddr,addrlen)) < 0)
	{
		perror("fail to connect");
		exit(1);
	}
	while(1)
	{
		fgets(buf,N,stdin);

		buf[strlen(buf) -1] = '\0';//strlen求出实际的长度

		if((send(sockfd,buf,N,0)) < 0)
		{
			perror("fail to send");
		}
		else
		{
			if(recv(sockfd,buf,N,0) < 0)
			{
				perror("fail to recv");
			}

			printf("from server : %s\n",buf);
		}
	}
	
	close(sockfd);
	return 0;
}
