#include "thread_mutex.h"
#include <stdio.h>

CThreadMutex::CThreadMutex()
{
	pthread_mutex_init(&mutex_lock, NULL);
}

CThreadMutex::~CThreadMutex()
{
	pthread_mutex_destroy(&mutex_lock);
}

void CThreadMutex::Acqure()
{
	printf("¼Ó»¥³âËø\n");
	pthread_mutex_lock(&mutex_lock);
}

void CThreadMutex::Release()
{
	printf("ÊÍ·Å»¥³âËø\n");
	pthread_mutex_unlock(&mutex_lock);
}