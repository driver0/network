//TCP网络编程服务器
#include <stdio.h>//printf
#include <arpa/inet.h>//inet_addr (inet_ntoa) htons (ntohs)
#include <sys/types.h>
#include <sys/socket.h>//socket bind listen connect
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>//exit
#include <unistd.h>//close
#include <string.h>//strcat
#include <dirent.h>//opendir
#include <errno.h> 
#include <sys/stat.h>
#include <fcntl.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
		        	printf("%s --> %s --> %d\n",__FILE__,__func__,__LINE__);\
						exit(1);\
					}while(0)

int do_list(int acceptfd)
{
	DIR *dir;
	struct dirent *dir_ent;
	char buf[N] = {};
	//打开目录
	if((dir = opendir(".")) == NULL)
	{
		errlog("fail to opendir");
	}

	//读取目录的文件名
	while((dir_ent = readdir(dir)) != NULL)
	{
		//隐藏文件不需要发送
		if(dir_ent->d_name[0] == '.')
		{
			continue;
		}

		//将读到的文件名发送给客户端
		strcpy(buf,dir_ent->d_name);
		send(acceptfd,buf,N,0);
	}

	printf("目录清单已发送完毕\n");

	close(acceptfd);

	return 0;
}

int do_download(int acceptfd,char *filename)
{
	char buf[N] = {};
	int fd;
	ssize_t bytes;

	//打开文件，判断文件是否存在
	if((fd  = open(filename,O_RDONLY)) < 0)
	{
		//如果不存在，发送N
		if(errno == ENOENT)
		{
			strcpy(buf,"N");
			send(acceptfd,buf,N,0);

			return -1;
		}
		else
		{
			errlog("fail to open");
		}
	}
	//如果存在，发送Y
	strcpy(buf,"Y");
	send(acceptfd,buf,N,0);

	//将读取到的数据发送给客户
	while((bytes = read(fd,buf,N)) > 0)
	{
		send(acceptfd,buf,bytes,0);

		//防止粘包
		usleep(45);
	}

	printf("文件发送完毕\n");
	close(fd);
	close(acceptfd);

	return 0;
}

int do_upload(int acceptfd,char *filename)
{
	char buf[N] = {};
	int fd;
	ssize_t bytes;

	//创建文件
	if((fd  = open(filename,O_CREAT | O_WRONLY | O_TRUNC,0664)) < 0)
	{
			errlog("fail to open");
	}
	//接收数据并写入
	while((bytes = recv(acceptfd,buf,N,0)) > 0)
	{
		write(fd,buf,bytes);

		usleep(25);
	}
	printf("文件接收完毕\n");

	close(fd);
	close(acceptfd);

	return 0;
}

int main(int argc, const char *argv[])
{
	int sockfd,acceptfd;
    struct sockaddr_in serveraddr,clientaddr;
	socklen_t addlen = sizeof(struct sockaddr_in);
	char buf[N] = {0};

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
	 serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	 serveraddr.sin_port = htons(atoi(argv[2]));

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
	 while(1)
	 {
		 //第五步：阻塞等待客户端的连接请求
		 if((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addlen)) < 0)
		 {
			 errlog("fail to accept");
		 }

		 //接收数据并作出相应的处理 L G P
		 if(recv(acceptfd,buf,N,0) < 0)
		 {
			 errlog("fail to recv");
		 }

		 switch(buf[0])
		 {
		 case 'L':do_list(acceptfd);break;
		 case 'G':do_download(acceptfd,buf+2);break;
		 case 'P':do_upload(acceptfd,buf+2);break;
		 }

	 }

	 close(acceptfd);
	 close(sockfd);

	return 0;
} 
