#ifndef _CEvent_H
#define _CEvent_H

#include <pthread.h>

///�ȴ��¼�ʱ����ֵ
/**
*EVENT_FAILED ʧ��
*EVENT_OK ���¼�����
*EVENT_TIMEOUT �ȴ�ʱ�䳬ʱ
*EVENT_ABANDONED �¼�����Ƿ�ʹ��
*/
enum EVENTS
{
	EVENT_FAILED = -1, EVENT_OK = 0, EVENT_TIMEOUT, EVENT_ABANDONED
};

// ���߳��¼�����
class CEvent
{
public:
	CEvent();
	~CEvent();

public:
	EVENTS Wait(long millSec);

	void Set();

	void Reset();
private:
	///�������������Ļ�����
	pthread_mutex_t   m_mutex;
	///�����������
	pthread_cond_t  m_cond;

	bool			m_bNotify;
};

#endif // _CEvent_H