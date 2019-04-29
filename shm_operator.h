#ifndef _ShmOperator_H
#define _ShmOperator_H

#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

// �����ڴ������
class ShmOperator
{
public:
	ShmOperator(int key, int size, int flag);
	~ShmOperator();

public:
	bool CreateMemory();					//���������ڴ淽��
	bool DestroyMemory();					//���ٹ����ڴ淽��
	bool AttachMemory(char*& pmemoryAddr);			//attach�����ڴ淽��
	bool DetachMemory(char* pmemoryAddr);			//detach�����ڴ淽��

private:
	int m_key;									//�����ڴ�key
	int m_size;									//�����ڴ��С
	int m_flag;									//�����ڴ洴����־
	int m_shmid;									//�����ڴ�������
	char *m_memoryAddr;
};


#endif // !_ShmOperator_H