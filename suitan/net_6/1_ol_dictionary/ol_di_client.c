//tcp本地网络编程客户端
#include <stdio.h>//printf
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strlen
#include <sqlite3.h>
#include <sys/socket.h>//socket bind listen accept connect
#include <netinet/in.h> //struct sockaddr_in
#include <arpa/inet.h>//inet_addr htons

#define N 16
#define R 1//user register
#define L 2//user login
#define Q 3 //query word
#define H 4 //history_record

#define  DATABASE "my.db"

typedef struct {
	int type;
	char name[N];
	char data[256];//password or word
}MSG;

void do_register(int socketfd,MSG *msg);
void do_login(int socketfd,MSG *msg);
void do_query(int socketfd,MSG *msg);
void do_history(int socketfd,MSG *msg);

int main(int argc, const char *argv[])
{
	int socketfd;
	struct sockaddr_in server_addr;
	MSG msg;

	if(argc < 3)
	{
		printf("Usage : %s <serv_ip> <serv_port>",argv[0]);
		exit(-1);
	}

	//第一步：创建套接字
	if((socketfd = socket(PF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}	
	
	//结构体清零
	bzero(&server_addr, sizeof(server_addr));

	//第二步：填充网络信息结构体
	//inet_addr:将点分十进制IP地址转化为网络能够识别的整形数据
	//htons:将主机字节序转化为网络字节序
	//atoi:将数字型字符串转化为整形数据
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	if(connect(socketfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
	{
		perror("fail to accept");
		exit(-1);
	}

	int n;
	while(1)
	{
		printf("***********************************\n");
		printf("*** 1:register  2:login  3:quit ***\n");
		printf("***********************************\n");
		printf("please choose :");
		if(scanf("%d",&n) <= 0)
		{
			perror("fail to scanf");
			exit(-1);
		}

		switch(n)
		{
		case 1: 
			do_register(socketfd,&msg);
			break;
		case 2:
			if(do_login(socketfd,&msg) == 1)
			{
				goto next;
			}
			break;
		case 3:
			close(socketfd);
			exit(0);
		}
	}
next:
    while(1)
	{
		printf("**********************************************\n");
		printf("*** 1:query_word  2:history_record  3:quit ***\n");
		printf("**********************************************\n");
		printf("please choose :");

		if(scanf("%d",&n) <= 0)
		{
			perror("fail to scanf");
			exit(-1);
		}

		switch(n)
		{
		case 1: 
			do_query(socketfd,&msg);
			break;
		case 2:
			do_history(socketfd,&msg);
			break;
		case 3:
			close(socketfd);
			exit(0);
	}
	return 0;
} 

//注册帐号
void do_register(int socketfd,MSG *msg)
{
	
	msg->type = R;

	printf("input your name:");
	scanf("%s",msg->name);//name

	printf("input your password:");
	scanf("%s",msg->data);//password

	send(socketfd,msg,sizeof(MSG),0);

	recv(socketfd,msg,sizeof(MSG),0);
	return ;
}

int do_login(int socketfd,MSG *msg)
{
	msg->type = L;

	printf("input your name:");
	scanf("%s",msg->name);//name

	printf("input your password:");
	scanf("%s",msg->data);//password

	send(socketfd,msg,sizeof(MSG),0);

	recv(socketfd,msg,sizeof(MSG),0);

	if(strncmp(msg->data,"OK",3) == 0)
	{
		printf("login : OK\n");
		return 1;
	}

	printf("login : %s\n",msg->data);
	return 0;
}

int do_query(int socketfd,MSG *msg)
{
	puts("query");
#if 0
	msg->types = 0;
	while(1)
	{
		scanf("%s",msg->name);//name
		if("#")break;
	}
	return 0;
#endif
}

int do_history(int socketfd,MSG *msg)
{
	puts("history");
#if 0
	msg->type = H;
	send(msg);
	while(1)
	{
		recv;
	}
	return 0;
#endif
}


