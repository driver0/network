//tcp网络编程服务器
#include <stdio.h> //printf
#include <arpa/inet.h> //inet_addr (inet_ntoa) htons htonl (ntohs ntohs )
#include <sys/types.h> 
#include <sys/socket.h> //socket bind listen accept connect
#include <netinet/in.h> //sockaddr_in 
#include <stdlib.h> //exit
#include <unistd.h> //close

#define errlog(errmsg) do{perror(errmsg);\
printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
	exit(1);\
}while(0)

int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
	struct sockaddr_in serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);

	if(argc < 3)
	{
		printf("argument is to few\n");
		exit(1);
	}

	return 0;
}
