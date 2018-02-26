//TCP网络编程服务器
#include <arpa/inet.h>//inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen connect
#include <stdio.h>//printf
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>//exit
#include <unistd.h>//close
#include <string.h>
#include <signal.h>
#include <errno.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
		        	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
						exit(1);\
					}while(0)

void handler(int sig)
{
}
int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
    struct sockaddr_in serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};
	int bytes;
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
	 //inet_addr:将点分十进制IP地址转化为网络能够识别的整形数据
	 //htons:将主机字节序转化为网络字节序
	 //atoi：将数字型字符串转化为整型数据
	 serveraddr.sin_family = AF_INET;
	 serveraddr.sin_port = htons(atoi(argv[2]));
	 serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

     //第三步：将套接字与网络信息结构体绑定
	 if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
	 {
		 errlog("fail to bind");
	 }

     //第四步：将套接字设置为监听模式
	 if(listen(sockfd,5) < 0)
	 {
		 errlog("fail to listen");
	 }

	 //使用alarm闹钟实现超时检测

	 //如果直接使用alarm设置时间，当时间到达时，会结束整个进程
	 //当时如果结合信号，如果时间到达时，触发SIGALRM信号，处理信号
	 //处理函数，当处理完信号处理函数后，代码还会接着当前的位置运行
	 //，这个性质叫做自重启属性，如果要设置超时，需要将其关闭

	 //注意：对寄存器或者位操作，执行读、改、写三步

	 //第一步：读取之前的行为（标志位）
	 struct sigaction act;
	 if(sigaction(SIGALRM,NULL,&act) , 0)
	 {
		 errlog("fail to sigaction");
	 }

	 //第二步：修改对应的行为
	 act.sa_handler = handler;
	 act.sa_flags = act.sa_flags & (~ SA_RESTART);

	 //第三步:将新的行为写入
	 if(sigaction(SIGALRM ,&act,NULL) < 0)
	 {
		 errlog("fail to sigaction");
	 }

	 while(1)
	 {
		 alarm(5);
		 //第五步：阻塞等待客户端的连接请求
		 if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addlen)) < 0)
		 {
			 //printf("errno = %d\n",errno);
			 if(errno == 4)
			 {
				 printf("accept timeout ...\n");
			 }
			 else
			 {
				 errlog("fail to accept");
			 }
		 }
		 else
		 {
			 printf("%s --> %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			 while(1)
			 {
				 alarm(5);
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
						 break;
					 }
					 else
					 {
						 printf("from client : %s\n",buf);
						 strcat(buf,"*_*");
						 if(send(acceptfd,buf,N,0) < 0)
						 {
							 errlog("fail to send\n");
						 }
					 }
				 }
			 }
		 }
	 }
	 close(acceptfd);
	 close(sockfd);

	return 0;
} 
