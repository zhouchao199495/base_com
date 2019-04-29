#include "shm_operator.h"
#include <errno.h>

ShmOperator::ShmOperator(int key, int size, int flag)
{
	m_flag = flag;								//��ʼ�������ڴ������ݳ�Ա
	m_key = key;
	m_size = size;
	m_flag = flag;
	m_shmid = -1;
	m_memoryAddr = NULL;
}

/*
* �������ܣ������ڴ������������
* ������������
* �� �� ֵ����
*/
ShmOperator::~ShmOperator()
{
	if (m_memoryAddr != NULL)						//��ȡ�����ڴ��ַ��Ϊ��
	{
		DetachMemory(m_memoryAddr);				       //�������detach���������빲���ڴ�
	}
}
/*
* �������ܣ������ڴ洴����������
* ������������
* �� �� ֵ���ɹ���������true�����򷵻�false
*/
bool ShmOperator::CreateMemory()
{
	m_shmid = shmget(m_key, m_size, m_flag);			      //���ô��������ڴ淽��������ֵ��ʼ��������
	if (m_shmid == -1)						      //����������ɹ�
	{
		return false;						      //����false
	}
	m_memoryAddr = reinterpret_cast<char*>(shmat(m_shmid, NULL, 0));      //attach�������ڴ棬�����׵�ַ

	//SHM_FAILED = -1
	if (m_memoryAddr == (char*)-1)			              //������ص�ַΪ��
	{
		return false;						      //˵��attach���ɹ�����false
	}
	return true;							      //����ɹ�����true
}
/*
* �������ܣ������ڴ洴���ٲ�������
* ������������
* �� �� ֵ���ɹ���������true�����򷵻�false
*/
bool ShmOperator::DestroyMemory()
{
	if (m_shmid == -1)							//�ж���Ӧ�Ĺ����ڴ��������Ƿ�Ϊ-1
		return false;								//Ϊ-1��ֱ�ӷ���false
	if (m_memoryAddr != NULL)						//�ж���Ӧ�׵�ַ�Ƿ�Ϊ��
	{
		if (shmdt(m_memoryAddr) != 0)					//�����Ϊ����detach���빲���ڴ�
			return false;						//�������ò��ɹ��򷵻�false
		m_memoryAddr = NULL;						//��������֮����ֱ����NULL��ֵ
	}
	if (shmctl(m_shmid, IPC_RMID, NULL) != 0)			//���ù����ڴ���Ʒ�����ָ��ɾ���������
		return false;						//�������ɹ�����false
	m_shmid = -1;							//������Ӧ��������Ϊ-1��ʼֵ
	m_memoryAddr = NULL;						//�����׵�ַָ��Ϊ��
	return true;							//�ɹ�����true
}
/*
* �������ܣ������ڴ�ҽӷ���
* ����������pmemoryAddr�������ڴ�ҽӻ�ȡ���׵�ַָ��
* �� �� ֵ���ɹ���������true�����򷵻�false
*/
bool ShmOperator::AttachMemory(char*& pmemoryAddr)
{
	pmemoryAddr = NULL;							//��ʼ�����ô��������ַָ��ָ��Ϊ��
	if (m_shmid == -1)							//�ж���Ӧ�Ĺ����ڴ�id�Ƿ����
	{
		m_shmid = shmget(m_key, 0, SHM_R | SHM_W);	                //����������������ݳ�Ա���´���
	}
	m_memoryAddr = reinterpret_cast<char*>(shmat(m_shmid, NULL, 0));	//������attach���������ص�ַ
	if (m_memoryAddr == (char*)-1)			                //�жϵ�ַ�Ƿ�Ϊ��
		return false;							//Ϊ���򷵻�false
	pmemoryAddr = reinterpret_cast<char*>(m_memoryAddr);                    //����ǿ��ת�����͸��������ָ�����
	return true;							//�ɹ�����true
}
/*
* �������ܣ������ڴ����뷽��
* ����������pmemoryAddr�������ڴ�ҽӻ�ȡ���׵�ַָ��
* �� �� ֵ���ɹ���������true�����򷵻�false
*/
bool ShmOperator::DetachMemory(char* pmemoryAddr)
{
	if (m_shmid == -1 || m_memoryAddr == NULL)			//�жϵ�ǰ�����ڴ��������Լ��׵�ַ�Ƿ���Ч
		return true;						//����Ѿ���Ч��ֱ�ӷ���true
	if (pmemoryAddr != (m_memoryAddr))				        //�жϻ�ȡ�����׵�ַ�뵱ǰ�׵�ַ�Ƿ�ͬһָ��
		return false;							//�����ͬ��˵���������뵱ǰָ�������ڴ��
	int nCode = shmdt(m_memoryAddr);				//������Ӧdetach����������ֵ����nCode
	if (nCode != 0)							//����÷���ִ�в��ɹ��������ж����
	{
		if (errno == EINVAL)
		{
			m_memoryAddr = NULL;
			cout << "*Already shmdt*" << endl;
			return true;
		}
		return false;
	}
	m_memoryAddr = NULL;
	return true;
}
