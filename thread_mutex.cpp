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
	printf("�ӻ�����\n");
	pthread_mutex_lock(&mutex_lock);
}

void CThreadMutex::Release()
{
	printf("�ͷŻ�����\n");
	pthread_mutex_unlock(&mutex_lock);
}