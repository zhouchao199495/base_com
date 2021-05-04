#include <pthread.h>

// »¥³âËø
class CThreadMutex
{
public:
	CThreadMutex();
	~CThreadMutex();

public:
	void Acqure();
	void Release();
private:
	pthread_mutex_t mutex_lock;
};