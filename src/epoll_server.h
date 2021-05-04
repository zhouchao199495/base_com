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

#include "thread_unix.h"
#include "epoll_def.h"

using namespace std;

// IO多路复用epoll
class EpollServer :public CThread
{
public:
	EpollServer();
	~EpollServer();

public:
	void SetListenPort(int port);
	void SetMaxEventSize(int size);
	void SetWritePipe(int rfd);
	
	void* BindEpollRecv();

private:
	// 设置socket为非阻塞模式
	void SetNoBlocking(int fd);

	// 设置复用地址
	void SetReuseAddr(int fd);

	// 注册epoll事件
	int SetEpollCtl();

	int SetListen();

	void run();

	void FreeFd();
private:
	unsigned short m_uListenPort;
	int m_maxEventSize;
	int m_backStroreSize;
	int m_backlog;

	int m_listenfd;
	int m_epfd;
	int m_wfd;
	
};


