#pragma once

#include <deque>
#include <map>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

#include <string>
#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cstdlib>
#include <cctype>
#include <sstream>
#include <utility>
#include <stdexcept>

#include <sys/socket.h> 
#include <sys/epoll.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <iostream>
#include <signal.h>
#include <string.h>

#include "thread_unix.h"
#include "epoll_def.h"

using namespace std;

class PipeReadThread : public CThread
{
public:
	PipeReadThread();
	~PipeReadThread();

public:
	void SetReadPipe(int fd);
	// 设置socket非阻塞模式
	void SetNoBlocking(int fd)
	{
		int opts = fcntl(fd, F_GETFL);
		if (opts < 0)
		{
			perror("fcntl(sock,GETFL)");
			exit(1);
		}
		opts = opts | O_NONBLOCK;
		if (fcntl(fd, F_SETFL, opts) < 0)
		{
			perror("fcntl(sock,SETFL,opts)");
			exit(1);
		}
	}
private:
	void run();

	void ReadPipe();

private:
	int m_rfd;
};
