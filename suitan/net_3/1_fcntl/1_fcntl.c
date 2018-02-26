#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define N 128

int main(int argc, const char *argv[])
{
	char buf[N]={0};
	//使用fcntl函数实现非阻塞
	//注意：对寄存器或者位的操作，一般执行读、改、写三步
	
	//第一步：读取文件状态标志位
	int flags;
	if((flags = fcntl(0,F_GETFL)) < 0)
	{
		perror("fail to fcntl");
		return -1;
	}

	//第二步：修改标志位
	flags |= O_NONBLOCK;
	
	//第三步：将新的标志位写进去
	if(fcntl(0,F_SETFL,flags) < 0)
	{
		perror("fail to fcntl");
		return -1;
	}
	while(1)
	{
		sleep(3);

		fgets(buf, N, stdin);

		printf("buf >> %s\n",buf);

		sleep(1);

		printf("******************\n");
	}
	return 0;
}
