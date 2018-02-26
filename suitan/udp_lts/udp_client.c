//udp网络编程服务器
#include <stdio.h> //printf
#include <arpa/inet.h> //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h> //socket bind listen accept connect
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <string.h> //strlen
#include <signal.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
	exit(1);
}while(0)

#define L 1
#define B 2
#define Q 3	

typedef struct{
	int type;
	char name[N];
	char text[N];
}MSG;

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	MSG msg;
	pid_t pid;

	if(argc < 3)
	{
		printf("argument is too few");
		exit(1);
	}

	//第一步：创建套接字
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0)) < 0)
	{
		errlog("fail to socket");
	}

	//第二步：填充网络信息结构体
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	//登录
	//确定用户名和类型
	msg.type = L;
	
	printf("please input name >>>");
	fgets(msg.name,N,stdin);
	msg.name[strlen(msg.name) -1] = '\0';

	sendto(sockfd,&msg,&sizeof(msg),0,(struct sockaddr *)&serveraddr,addlen);

	//创建父子进程实现可以一边接收数据一边发送数据
	if((pid = fork()) < 0)
	{
		errlog("fail to fork");
	}
	else if(pid == 0)//子进程负责发送数据
	{
		while(1)
		{
			fgets(msg.text, N ,stdin);
			msg.text[strlen(msg.text) - 1] = '\0';

			//退出
			if(strncmp(msg.text,"quit",4) == 0)
			{
				msg.type = Q;

			}
		}
	}

	return 0;
}  
