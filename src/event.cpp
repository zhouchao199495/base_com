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
	// �������ʱ������abstime
	struct timeval tv;
	gettimeofday(&tv, NULL);

	struct timespec abstime;
	abstime.tv_sec = tv.tv_sec + millSec / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (millSec % 1000) * 1000000;

	int ret = -1;
	EVENTS myEvent = EVENT_FAILED;

	pthread_mutex_lock(&m_mutex);
	printf("��ʼ�ȴ�...%ld\n", abstime.tv_sec);
	while (!m_bNotify)
	{	
		//�Զ��ͷŻ����岢�ҵȴ�m_cond״̬,�������������ĵȴ�ʱ��  
		if ((ret = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
		{
			// ����ȴ����յ��ź�������ֹͣ
			if (0 == ret)
				break;
			else
			{
				pthread_mutex_unlock(&m_mutex);
				return EVENT_TIMEOUT;
			}
				
		}
	}

	printf("�ȴ���ɣ�\n");
	pthread_mutex_unlock(&m_mutex);

	myEvent = (ret == 0 ? EVENT_OK : EVENT_TIMEOUT);
	return myEvent;
}

void CEvent::Set()
{
	pthread_mutex_lock(&m_mutex);

	m_bNotify = true;
	pthread_cond_broadcast(&m_cond); // pthread_cond_broadcast�ᵼ�¶��߳̾�Ⱥ���󣬿�����pthread_cond_signal���ѵ���

	pthread_mutex_unlock(&m_mutex);
}

void CEvent::Reset()
{
	pthread_mutex_lock(&m_mutex);

	m_bNotify = false;

	pthread_mutex_unlock(&m_mutex);
}
