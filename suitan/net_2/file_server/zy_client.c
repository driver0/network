//tcp网络编程客户端
#include <stdio.h>//printf
#include <arpa/inet.h>//inet_addr (ntoa) htons 
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen accept connect
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h>//close
#include <string.h>//strlen
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
						exit(1);\
 						}while(0)

int do_help()
{
	printf("***************************************************\n");
	printf("**********选项     /    功能                 ******\n");
	printf("**********list     /    查看服务器所在目录的文件名*\n");
	printf("**get filename     /    下载文件             ******\n");
	printf("**put filename     /    上传文件             ******\n");
	printf("**********quit     /    退出                 ******\n");
	printf("***************************************************\n");

	 return 0;
}

//查看服务器所在目录文件名
int do_list(struct sockaddr_in serveraddr)
{
	
	int sockfd;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N]={}; 

	//第一步：创建套接字
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}

	//第五步：发送客户端的连接请求
	if(connect(sockfd,(struct sockaddr *)&serveraddr,addlen) < 0)
	{
		 errlog("fail to accept");
	}

	//发送查看目录的命令给服务器
	strcpy(buf,"L");
	send(sockfd, buf, N, 0);

	while(recv(sockfd, buf, N, 0) > 0)
	{
		printf("*** %s\n",buf);
	}

	printf("目录清单已接收完毕\n");
	close(sockfd);

	return 0;
}

//
int do_download(struct sockaddr_in serveraddr,char *filename)
{
	int sockfd;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {};
	int fd;
	ssize_t bytes;

	//第一步：创建套接字
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}

	//第五步：发送客户端的连接请求
	if(connect(sockfd,(struct sockaddr *)&serveraddr,addlen) < 0)
	{
		 errlog("fail to accept");
	}

	//首先需要将文件名和对应指令发送给服务器端
	sprintf(buf,"G %s",filename);
	send(sockfd, buf,N,0);

	//根据服务器所得到的信息（文件存不存在）作出相应的判断
	recv(sockfd,buf,N,0);

	//如果不存在
	if(buf[0] == 'N')
	{
		printf("%s is not exist\n",filename);
		return -1;
	}
	//如果存在，创建文件，接收数据，并写入
	if((fd = open(filename,O_CREAT | O_WRONLY | O_TRUNC,0664)) < 0)
	{
		errlog("fail to open");
	}

	//将接收的数据写入文件
	while((bytes = recv(sockfd,buf,N,0)) > 0)
	{
		write(fd,buf,bytes);
		//防止粘包
		usleep(25);
	}
	
	printf("文件接收完毕\n");
	close(fd);
	close(sockfd);
	
	return 0;
}

//
int do_upload(struct sockaddr_in serveraddr,char *filename)
{
	int sockfd;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {};
	int fd;
	ssize_t bytes;

	//第一步：创建套接字
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		errlog("fail to socket");
	}

	//第五步：发送客户端的连接请求
	if(connect(sockfd,(struct sockaddr *)&serveraddr,addlen) < 0)
	{
		 errlog("fail to accept");
	}

	//打开文件，判断是否存在
	if((fd = open(filename, O_RDONLY)) < 0)
	{
		if(errno == ENOENT)
		{
			printf("%s is not exist,cannot is upload\n",filename);
			return 0;
		}
		else
		{
			errlog("fail to open");
		}
	}
	//如果文件存在，发送文件名以及相应指令
	sprintf(buf,"P %s",filename);
	send(sockfd, buf,N,0);

	//读取数据并发送
	while((bytes = read(fd,buf,N)) > 0)
	{
		send(sockfd,buf,bytes,0);

		usleep(45);
	}

	printf("文件上传完毕\n");

	close(fd);
	close(sockfd);
	
	return 0;
}
int main(int argc, const  char*argv[])
{
	char buf[N] = {0};
	struct sockaddr_in serveraddr;


	//第二步：填充网络信息结构体
	//inet_addr:将点分十进制IP地址转化为网络能够识别的整形数据
	//htons:将主机字节序转化为网络字节序
	//atoi:将数字型字符串转化为整形数据
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	if(argc < 3)
	{
		printf("argument is too few\n");
		exit(1);
	}


	printf("******************\n");
	printf("请输入help查看选项\n");
	printf("******************\n");

	while(1)
	{
		//在终端输入内容
		printf("input >>>");
		fgets(buf,N,stdin);
		buf[strlen(buf) - 1] = '\0';

		//根据输入的内容做相应的事情 help quit list get+filename put+filename
		if(strncmp(buf,"help",4) == 0)
		{
			do_help();
		}
		else if(strncmp(buf,"list",4) == 0)
		{
			do_list(serveraddr);
		}
		else if(strncmp(buf,"get",3) == 0)
		{
			do_download(serveraddr,buf+4);
		}
		else if(strncmp(buf,"put",3) == 0)
		{
			do_upload(serveraddr,buf+4);
		}
		else if(strncmp(buf,"quit",4) == 0)
		{
			break;
		}
		else
		{
			printf("please input a right option\n");
		} 
	}

	return 0;
} 
