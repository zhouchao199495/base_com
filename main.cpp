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

// EpollÍøÂçÍ¨ÐÅµÄ²âÊÔº¯Êý
void TestEpoll()
{
	// Ïß³Ì¼ä¹ÜµÀÍ¨ÐÅ
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

// ²âÊÔ¶à½ø³Ì
void TestFork(int argc, char** argv)
{
	for(int i=0;i<argc;i++)
        {
                printf("22   i=%s\n",argv[i]);
        }

	pid_t id = fork();
	if (id < 0)
	{
		printf("fork error!\n");
		return ;
	}
	//¿¿¿¿¿¿¿¿¿¿¿¿¿
	setsid();// set sub-process independent
	if (id == 0)// sub process
	{
		execlp("mytest", NULL);
		for (int i = 0; i < 2; i++)
		{
			printf("new porcess,id=%d\n", getpid());
		}
		int b;
		scanf("%d", &b);
	}
	else
	{
		while(1) {sleep(1);}
		printf("old process,the return value is %d! myid=%d\n", id, getpid());
	}
}

int main(int argc, char** argv)
{
	for(int i=0;i<argc;i++)
	{
		printf("i=%s\n",argv[i]);
	}
	TestFork(argc-1,argv+1);
	return 0;
}
