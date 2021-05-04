#include "event.h"
#include <sys/time.h>
#include <stdio.h>

CEvent::CEvent()
{
	pthread_mutex_init(&m_mutex, 0);
	pthread_cond_init(&m_cond, 0);
	m_bNotify = false;
}


CEvent::~CEvent()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
}

EVENTS CEvent::Wait(long millSec)
{
	// 计算绝对时间间隔，abstime
	struct timeval tv;
	gettimeofday(&tv, NULL);

	struct timespec abstime;
	abstime.tv_sec = tv.tv_sec + millSec / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (millSec % 1000) * 1000000;

	int ret = -1;
	EVENTS myEvent = EVENT_FAILED;

	pthread_mutex_lock(&m_mutex);
	printf("开始等待...%ld\n", abstime.tv_sec);
	while (!m_bNotify)
	{	
		//自动释放互斥体并且等待m_cond状态,并且限制了最大的等待时间  
		if ((ret = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
		{
			// 如果等待后，收到信号量，则停止
			if (0 == ret)
				break;
			else
			{
				pthread_mutex_unlock(&m_mutex);
				return EVENT_TIMEOUT;
			}
				
		}
	}

	printf("等待完成！\n");
	pthread_mutex_unlock(&m_mutex);

	myEvent = (ret == 0 ? EVENT_OK : EVENT_TIMEOUT);
	return myEvent;
}

void CEvent::Set()
{
	pthread_mutex_lock(&m_mutex);

	m_bNotify = true;
	pthread_cond_broadcast(&m_cond); // pthread_cond_broadcast会导致多线程惊群现象，可以用pthread_cond_signal唤醒单个

	pthread_mutex_unlock(&m_mutex);
}

void CEvent::Reset()
{
	pthread_mutex_lock(&m_mutex);

	m_bNotify = false;

	pthread_mutex_unlock(&m_mutex);
}
