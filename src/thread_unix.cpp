#include "thread_unix.h"
#include <unistd.h>

void* CThread::run1()
{
	threadStatus = THREAD_STATUS_RUNNING;
	tid = pthread_self();
	run();
	threadStatus = THREAD_STATUS_EXIT;
	tid = 0;
	pthread_exit(NULL);
}

CThread::CThread()
{
	tid = 0;
	threadStatus = THREAD_STATUS_NEW;
}

bool CThread::start()
{
	int iRet = 0;
	pthread_create(&tid, NULL, thread_proxy_func, this) == 0;
}

pthread_t CThread::getThreadID()
{
	return tid;
}

int CThread::getState()
{
	return threadStatus;
}

void CThread::join()
{
	if (tid > 0)
	{
		pthread_join(tid, NULL);
	}
}
void * CThread::thread_proxy_func(void * args)
{
	CThread * pThread = static_cast<CThread *>(args);

	pThread->run();

	return NULL;
}

void CThread::join(unsigned long millisTime)
{
	if (tid == 0)
	{
		return;
	}
	if (millisTime == 0)
	{
		join();
	}
	else
	{
		unsigned long k = 0;
		while (threadStatus != THREAD_STATUS_EXIT && k <= millisTime)
		{
			usleep(100);
			k++;
		}
	}
}
