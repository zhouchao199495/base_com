#pragma once
#include <pthread.h>

// ��д��
class CThreadRWLock
{
public:
	CThreadRWLock();
	~CThreadRWLock();

public:
	int Read();		// �Ӷ���
	int Write();	// ��д��
	int Unlock();	// ����

private:
	pthread_rwlock_t rwlock;
};

