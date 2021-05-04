#ifndef _ShmOperator_H
#define _ShmOperator_H

#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

// 共享内存操作类
class ShmOperator
{
public:
	ShmOperator(int key, int size, int flag);
	~ShmOperator();

public:
	bool CreateMemory();					//创建共享内存方法
	bool DestroyMemory();					//销毁共享内存方法
	bool AttachMemory(char*& pmemoryAddr);			//attach共享内存方法
	bool DetachMemory(char* pmemoryAddr);			//detach共享内存方法

private:
	int m_key;									//共享内存key
	int m_size;									//共享内存大小
	int m_flag;									//共享内存创建标志
	int m_shmid;									//共享内存描述符
	char *m_memoryAddr;
};


#endif // !_ShmOperator_H