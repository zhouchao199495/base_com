#pragma once
#include <pthread.h>

// ¶ÁĞ´Ëø
class CThreadRWLock
{
public:
	CThreadRWLock();
	~CThreadRWLock();

public:
	int Read();		// ¼Ó¶ÁËø
	int Write();	// ¼ÓĞ´Ëø
	int Unlock();	// ½âËø

private:
	pthread_rwlock_t rwlock;
};

