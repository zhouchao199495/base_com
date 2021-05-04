#include "epoll_server.h"
#include <string.h>


EpollServer::EpollServer()
{
	m_uListenPort = 5000;
	m_maxEventSize = 1024;
	m_backStroreSize = 1024;
	m_backlog = 5;

	m_listenfd = -1;
	m_epfd = -1;
	m_wfd = -1;
}


EpollServer::~EpollServer()
{
}

void EpollServer::run()
{
	printf("启动run!!!\n");
	BindEpollRecv();
}

void* EpollServer::BindEpollRecv()
{
	//创建epoll,对2.6.8以后的版本,其参数无效,只要大于0的数值就行,内核自己动态分配
	m_epfd = epoll_create(m_backStroreSize);
	if (m_epfd < 0)
	{
		printf("epoll_create fail!\n");
		return NULL;
	}

	printf("创建socket\n");
	// 创建socket
	m_listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenfd < 0)
	{
		printf("socket create fail!\n");
		FreeFd();
		return NULL;
	}
	printf("设置非阻塞模式\n");
	// 设置非阻塞模式
	SetNoBlocking(m_listenfd);

	printf("设置监听socket为端口重用\n");
	// 设置监听socket为端口重用
	SetReuseAddr(m_listenfd);

	printf("注册epoll事件\n");
	// 注册epoll事件
	int ret = SetEpollCtl();
	if (0 != ret)
	{
		printf("epoll_ctl fail!\n");
		FreeFd();
		return NULL;
	}
	printf("绑定地址&设置监听,参数backlog是侦听队列的长度,一般这个值会小30以内\n");
	// 绑定地址&设置监听,参数backlog是侦听队列的长度,一般这个值会小30以内
	ret = SetListen();
	if (0 != ret)
	{
		printf("listen set fail!\n");
		FreeFd();
		return NULL;
	}

	struct epoll_event events[1024];
	// 开始进行socket事件触发处理
	while (true)
	{
		int nfds = epoll_wait(m_epfd, events, m_maxEventSize, 1000);

		for (int i = 0; i < nfds; ++i)
		{
			printf("开始进行socket事件触发处理2222...\n");
			if (events[i].data.fd != m_listenfd) // 监听此socket上的事件
			{
				printf("收到未监听的socket，listen_fd=%d, recive_fd=%d\n", m_listenfd, events[i].data.fd);
				continue;
			}
			
			// 收到异常epoll事件
			if (events[i].events&EPOLLERR || events[i].events&EPOLLHUP)
			{
				printf("Recive exception event!\n");
				continue;
			}

			struct sockaddr_in cliaddr;
			// 处理Epoll事件                   
			socklen_t acceptLen = sizeof(struct sockaddr);//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件 
			int acceptfd = accept(m_listenfd, (sockaddr *)&cliaddr, &acceptLen);
			if (acceptfd >= 0)
			{
				char* ip = (char*)inet_ntoa(cliaddr.sin_addr);
				printf("收到[%s:%d]消息,listen_fd=%d, acceptfd=%d\n", ip, cliaddr.sin_port, m_listenfd, acceptfd);
				pipemsg msg;
				msg.op = 1;
				msg.fd = acceptfd;
				msg.ip = cliaddr.sin_addr.s_addr;
				msg.port = cliaddr.sin_port;

				char buffer[40];
 				read(acceptfd, buffer, sizeof(buffer)-1);
				printf("cc=%s\n",buffer);
				ret = write(m_wfd, &msg, 14);
				if (ret != 14)
				{
					cout << "AcceptThread, write fail:" << ret << ",errno:" << errno << endl;
					close(acceptfd);
				}
			}
		}
	}
}

// 设置socket非阻塞模式
void EpollServer::SetNoBlocking(int fd)
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

// 设置复用地址
void EpollServer::SetReuseAddr(int fd)
{
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(&opt)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}
}

// 注册epoll事件
int EpollServer::SetEpollCtl()
{
	// 注册epoll事件
	struct epoll_event epEvent;
	epEvent.data.fd = m_listenfd;					//设置与要处理的事件相关的文件描述符        
	epEvent.events = EPOLLIN | EPOLLET;			//设置要处理的事件类型

	return	epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listenfd, &epEvent);
}

int EpollServer::SetListen()
{
	// 绑定地址
	struct sockaddr_in svraddr;
	bzero(&svraddr, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svraddr.sin_port = htons(m_uListenPort);
	bind(m_listenfd, (sockaddr *)&svraddr, sizeof(svraddr));

	// 设置监听,参数backlog是侦听队列的长度,一般这个值会小30以内
	return listen(m_listenfd, m_backlog);
}

void EpollServer::FreeFd()
{
	if (m_listenfd > 0)
	{
		close(m_listenfd);
		m_listenfd = -1;
	}

	if (m_epfd > 0)
	{
		close(m_epfd);
		m_epfd = -1;
	}

}
void EpollServer::SetListenPort(int port)
{
	m_uListenPort = port;
}

void EpollServer::SetMaxEventSize(int size)
{
	m_maxEventSize = size;
}

void EpollServer::SetWritePipe(int wfd)
{
	m_wfd = wfd;
}