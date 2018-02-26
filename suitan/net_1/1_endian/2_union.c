//使用共用体测试当前主机字节序
#include <stdio.h>

union un
	{
		int a;
		char b;
	};
int main(int argc, const char *argv[])
{
	union un myun; 
	myun.a=0x12345678;
	printf("myun.a = %#x\n",myun.a);
	printf("myun.b = %#x\n",myun.b);
	if(myun.b == 0x78)
	{
		printf("小端存储\n");
	}
	else
	{
		printf("大端存储\n");
	}
	return 0;
}
