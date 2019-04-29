#include "sem_operator.h"


CSemOperator::CSemOperator(int key, int size, int semflg)
{
	m_size = size;
	m_key = key;
	m_semflg = semflg;

	m_semid = -1;
	sem_init(&m_sem, 0, 0);
}


CSemOperator::~CSemOperator()
{
	sem_destroy(&m_sem);
}

bool CSemOperator::CreateSem()
{
	m_semid = semget((key_t)m_key, m_size, m_semflg);
	if (-1 == m_semid)
	{
		return false;
	}
	return true;
}

bool CSemOperator::AttachSem()
{
	return true;
}
