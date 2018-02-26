#include <stdio.h>
#include <sqlite3.h>

//由于系统没有定义这些数据库函数，所以需要手动连接
//库文件gcc *.c -lsqlite3

#define DATABASE "student.db"
#define N 128

do_insert(sqlite3 *db)
{
	int id;
	char name[N] = {};
	int score;
	char sq1[N] = {};
	char *errmsg;

	printf("please input id : ");
	scanf("%d",&id);
	getchar();

	printf("please input name : ");
	scanf("%s",name);
	getchar();

	printf("please input score : ");
	scanf("%d",&score);
	getchar();

	sprintf(sq1,"insert into stu values(%d, '%s',%d)",id,name,score);

	if(sqlite3_exec(db,sq1,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		printf("数据插入成功\n");
	}
	return 0;
}

int main(int argc, const char *argv[])
{
	sqlite3 *db;
	char *errmsg;
	int num;

	//第一步:创建（打开）一个数据库
	if(sqlite3_open(DATABASE,&db) != SQLITE_OK)
	{
		printf("%s\n",sqlite3_errmsg(db));
	}
	else
	{
		printf("数据库文件已经建立（打开）成功！\n");
	}

	//第二步：创建一张表
	if(sqlite3_exec(db,"create table stu(id int,name char,score int)",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		printf("表已经创建成功\n");
	}

	//第三步：对当前表进行增删改查
	while(1)
	{
		printf("*************************************************************\n");
		printf("***1: 插入数据 2：查询数据 3：修改数据 4：删除数据 5：退出***\n");
		printf("*************************************************************\n");
		printf("input >>> ");

		scanf("%d",&num);

		switch(num)
		{
		case 1:
			do_insert(db);
			break;
		case 2:
			do_select(db);
			break;
		case 3:
			do_update();
			break;
		case 4:
			do_delete();
			break;
		case 5:
			sqlite3_close(db);
			return -1;
		default:
			printf("请输入正确的选项\n");
		}
	}
	return 0;
}
