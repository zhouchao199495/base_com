#pragma once
#include <string.h>

//�ܵ���Ϣ�ṹ
struct pipemsg {
	int op;
	int fd;
	unsigned int ip;
	unsigned short port;
};

// TODO �����epoll����˺Ϳͻ���Լ����������
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