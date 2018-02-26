//TCP网络编程服务器
#include <arpa/inet.h>//inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen connect
#include <stdio.h>//printf
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>//exit
#include <unistd.h>//close
#include <string.h>
#include <sys/wait.h>
#include <signal.h>


#define N 128
#define errlog(errmsg) do{perror(errmsg);\
		        	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
						exit(1);\
					}while(0)

void handler(int sig)
{
	wait(NULL);
}

int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
    struct sockaddr_in serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};
	ssize_t bytes;
	pid_t pid;

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
	
     //第二步：填充网络信息结构体
	 serveraddr.sin_family = AF_INET;
	 serveraddr.sin_port = htons(atoi(argv[2]));
	 serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

     //第三步：将套接字与网络信息结构体绑定
		 if(pid > 0)
	 if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	 {
		 errlog("fail to bind");
	 }

     //第四步：将套接字设置为监听模式
	 if(listen(sockfd,5) < 0)
	 {
		 errlog("fail to listen");
	 }
	 
	 //处理僵尸进程的方法二：使用信号
	 signal(SIGCHLD,handler);

     //处理僵尸进程的方法三：使用信号
	 //signal(SIGCHLD,SIG_IGH);

	 while(1)
	 {
		
		 //第五步：阻塞等待客户端的连接请求
		 if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addlen)) < 0)
		 {
			 errlog("fail to accept");
		 }
	
		//使用父子进程实现TCP并发服务器 
		 if(( pid = fork()) < 0)
		 {
			 errlog("fail  to fork");
		 }          
		 else if(pid > 0)//父进程负责连接
		 {
			 close(acceptfd);
			 //处理僵尸进程的方法一：使用waitpid设置非阻塞形式
			 //waitpid(-1,NULL,WNOHANG);
		 }
		 else //子程序负责通信
		 {
			 close(sockfd);
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
						 exit(1);
					 }
					 else
					 {
						 printf("from client : %s\n",buf);
						 strcat(buf,"*_*");
						 if(send(acceptfd,buf,N,0) < 0)
						 {
							 errlog("fail to send");
						 }
					 }
				 }
			 }
		}
	 }

	return 0;
} 
