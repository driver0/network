//udpserver
#include <stdio.h> //perror,printf
#include <arpa/inet.h> //htons,inet_addr
#include <sys/types.h>  //socket,bind,recvfrom,sendto
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr ,sockaddr_in
#include <stdlib.h> //exit
#include <unistd.h>  //close
#include <string.h>	//strcat
//#include <strings.h>

#define N 128
int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};
	ssize_t bytes;
	if(argc < 3)
	{
		printf("argument is few!\n");
		exit(1);
	}

	//第一步：创建套接字	
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("fail to sockfd!\n");
		exit(1);
	}

	//第二步：填充服务器网络信息结构体
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));


	//第四步：通信
	while(1)
	{
		fgets(buf,N,stdin);

		buf[strlen(buf) -1] = '\0';

		if(sendto(sockfd,buf,N,0,(struct sockaddr *)&serveraddr,addlen) < 0)
		{
			perror("fail to sendto\n");
			exit(1);
		}
		if((bytes = recvfrom(sockfd,buf,N,0,(struct sockaddr *)&serveraddr,&addlen)) < 0)
		{
			perror("fail to recv\n");
			exit(1);
		}
		else if(bytes == 0)
		{
			printf("NO DATA!\n");
		}
		else
		{
			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
			else{
				printf("from client : %s\n",buf);

				fflush(stdout);
			}
		}
	}
	close(sockfd);
	return 0;
}
