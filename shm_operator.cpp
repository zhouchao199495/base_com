#include "shm_operator.h"
#include <errno.h>

ShmOperator::ShmOperator(int key, int size, int flag)
{
	m_flag = flag;								//初始化共享内存类数据成员
	m_key = key;
	m_size = size;
	m_flag = flag;
	m_shmid = -1;
	m_memoryAddr = NULL;
}

/*
* 函数功能：共享内存操作析构函数
* 函数参数：无
* 返 回 值：无
*/
ShmOperator::~ShmOperator()
{
	if (m_memoryAddr != NULL)						//获取共享内存地址不为空
	{
		DetachMemory(m_memoryAddr);				       //将会调用detach方法来脱离共享内存
	}
}
/*
* 函数功能：共享内存创建操作方法
* 函数参数：无
* 返 回 值：成功创建返回true，否则返回false
*/
bool ShmOperator::CreateMemory()
{
	m_shmid = shmget(m_key, m_size, m_flag);			      //调用创建共享内存方法，返回值初始化描述符
	if (m_shmid == -1)						      //如果创建不成功
	{
		return false;						      //返回false
	}
	m_memoryAddr = reinterpret_cast<char*>(shmat(m_shmid, NULL, 0));      //attach到共享内存，返回首地址

	//SHM_FAILED = -1
	if (m_memoryAddr == (char*)-1)			              //如果返回地址为空
	{
		return false;						      //说明attach不成功返回false
	}
	return true;							      //否则成功返回true
}
/*
* 函数功能：共享内存创销毁操作方法
* 函数参数：无
* 返 回 值：成功创建返回true，否则返回false
*/
bool ShmOperator::DestroyMemory()
{
	if (m_shmid == -1)							//判断相应的共享内存描述符是否为-1
		return false;								//为-1则直接返回false
	if (m_memoryAddr != NULL)						//判断相应首地址是否为空
	{
		if (shmdt(m_memoryAddr) != 0)					//如果不为空则detach脱离共享内存
			return false;						//方法调用不成功则返回false
		m_memoryAddr = NULL;						//否则脱离之后将其直接置NULL空值
	}
	if (shmctl(m_shmid, IPC_RMID, NULL) != 0)			//调用共享内存控制方法，指定删除命令操作
		return false;						//方法不成功返回false
	m_shmid = -1;							//设置相应的描述符为-1初始值
	m_memoryAddr = NULL;						//设置首地址指针为空
	return true;							//成功返回true
}
/*
* 函数功能：共享内存挂接方法
* 函数参数：pmemoryAddr，共享内存挂接获取的首地址指针
* 返 回 值：成功创建返回true，否则返回false
*/
bool ShmOperator::AttachMemory(char*& pmemoryAddr)
{
	pmemoryAddr = NULL;							//初始化设置传入参数地址指针指向为空
	if (m_shmid == -1)							//判断相应的共享内存id是否存在
	{
		m_shmid = shmget(m_key, 0, SHM_R | SHM_W);	                //不存在则根据其数据成员重新创建
	}
	m_memoryAddr = reinterpret_cast<char*>(shmat(m_shmid, NULL, 0));	//随后调用attach方法，返回地址
	if (m_memoryAddr == (char*)-1)			                //判断地址是否为空
		return false;							//为空则返回false
	pmemoryAddr = reinterpret_cast<char*>(m_memoryAddr);                    //否则强制转换类型赋给传入的指针参数
	return true;							//成功返回true
}
/*
* 函数功能：共享内存脱离方法
* 函数参数：pmemoryAddr，共享内存挂接获取的首地址指针
* 返 回 值：成功创建返回true，否则返回false
*/
bool ShmOperator::DetachMemory(char* pmemoryAddr)
{
	if (m_shmid == -1 || m_memoryAddr == NULL)			//判断当前共享内存描述符以及首地址是否有效
		return true;						//如果已经无效则直接返回true
	if (pmemoryAddr != (m_memoryAddr))				        //判断获取到的首地址与当前首地址是否同一指向
		return false;							//如果不同则说明不是脱离当前指定共享内存块
	int nCode = shmdt(m_memoryAddr);				//调用相应detach方法，返回值赋给nCode
	if (nCode != 0)							//如果该方法执行不成功，进行判断诊断
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
