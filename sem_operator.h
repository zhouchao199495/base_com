#pragma once

#include <semaphore.h>
#include <sys/sem.h>
#include <pthread.h>

// �ź���������
class CSemOperator
{
public:
	CSemOperator(int key, int size, int semflg);
	~CSemOperator();

public:
	bool CreateSem();		// �����ź���
	bool AttachSem();		// �����ź���

private:
	sem_t m_sem;
	int m_semid;
	int m_key;
	int m_size;
	int m_semflg;
};
