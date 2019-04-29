#ifndef _CEvent_H
#define _CEvent_H

#include <pthread.h>

///等待事件时返回值
/**
*EVENT_FAILED 失败
*EVENT_OK 有事件发生
*EVENT_TIMEOUT 等待时间超时
*EVENT_ABANDONED 事件对象非法使用
*/
enum EVENTS
{
	EVENT_FAILED = -1, EVENT_OK = 0, EVENT_TIMEOUT, EVENT_ABANDONED
};

// 多线程事件操作
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
	///保护条件变量的互斥子
	pthread_mutex_t   m_mutex;
	///条件变量句柄
	pthread_cond_t  m_cond;

	bool			m_bNotify;
};

#endif // _CEvent_H