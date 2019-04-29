#include "thread_rw_lock.h"


CThreadRWLock::CThreadRWLock()
{
	pthread_rwlock_init(&rwlock, 0);
}


CThreadRWLock::~CThreadRWLock()
{
	pthread_rwlock_destroy(&rwlock);
}


int CThreadRWLock::Read()
{
	return pthread_rwlock_rdlock(&rwlock);
}

int CThreadRWLock::Write()
{
	return pthread_rwlock_wrlock(&rwlock);
}

int CThreadRWLock::Unlock()
{
	return pthread_rwlock_unlock(&rwlock);
}