#include <stdio.h> //printf
#include <arpa/inet.h> //inet_addr (inet_ntoa) i htons htonl (ntohs ntohl)
#include <sys/types.h>
#include <sys/socket.h> //socket bind sendro recvfrom
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <string.h> //strcat
#include <strings.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
	exit(1);
}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct 
	return 0;
}

