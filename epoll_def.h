#pragma once
#include <string.h>

//管道消息结构
struct pipemsg {
	int op;
	int fd;
	unsigned int ip;
	unsigned short port;
};

// TODO 这个是epoll服务端和客户端约定的数据类
class CDataObj
{
public:
	CDataObj() { a = 0; b = 0; memset(name, 0, 100); };
	~CDataObj() {};
public:
	int a;
	int b;
	char name[100];
};