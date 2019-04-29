#pragma once

#include <semaphore.h>
#include <sys/sem.h>
#include <pthread.h>

// 信号量操作类
class CSemOperator
{
public:
	CSemOperator(int key, int size, int semflg);
	~CSemOperator();

public:
	bool CreateSem();		// 创建信号量
	bool AttachSem();		// 操作信号量

private:
	sem_t m_sem;
	int m_semid;
	int m_key;
	int m_size;
	int m_semflg;
};
