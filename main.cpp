#include "thread_unix.h"
#include "shm_operator.h"
#include "epoll_server.h"
#include "epoll_reader.h"
#include <iostream>
#include <stdio.h>


#define FLAG IPC_CREAT|IPC_EXCL|SHM_R|SHM_W 
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

// Epoll网络通信的测试函数
void TestEpoll()
{
	// 线程间管道通信
	int fd[2];
	int ret = pipe(fd);
	if (ret != 0)
	{
		cout << "main, pipe fail:" << ret << ",errno:" << errno << endl;
		return ;
	}

	SetNoBlocking(fd[0]);

	printf("hello system public lib!fd[%d,%d] \n", fd[0], fd[1]);


	EpollServer* epSvr = new EpollServer();
	epSvr->SetWritePipe(fd[1]);
	epSvr->start();


	PipeReadThread* pr = new PipeReadThread();
	pr->SetReadPipe(fd[0]);
	pr->start();
}

// 测试多进程
void TestFork()
{
	pid_t id = fork();
	if (id < 0)
	{
		printf("fork error!\n");
	}
	else if (id == 0)
	{
		execlp("mytest", NULL);
		for (int i = 0; i < 100; i++)
		{
			printf("new porcess,id=%d\n", getpid());
		}
		
		exit(errno);
	}
	else
	{
		printf("old process,the return value is %d! myid=%d\n", id, getpid());
	}
}

int main()
{
	TestFork();
	int a = 0;
	scanf("%d", &a);
	return 0;
}
