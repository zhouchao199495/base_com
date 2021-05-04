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
	printf("����run!!!\n");
	BindEpollRecv();
}

void* EpollServer::BindEpollRecv()
{
	//����epoll,��2.6.8�Ժ�İ汾,�������Ч,ֻҪ����0����ֵ����,�ں��Լ���̬����
	m_epfd = epoll_create(m_backStroreSize);
	if (m_epfd < 0)
	{
		printf("epoll_create fail!\n");
		return NULL;
	}

	printf("����socket\n");
	// ����socket
	m_listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenfd < 0)
	{
		printf("socket create fail!\n");
		FreeFd();
		return NULL;
	}
	printf("���÷�����ģʽ\n");
	// ���÷�����ģʽ
	SetNoBlocking(m_listenfd);

	printf("���ü���socketΪ�˿�����\n");
	// ���ü���socketΪ�˿�����
	SetReuseAddr(m_listenfd);

	printf("ע��epoll�¼�\n");
	// ע��epoll�¼�
	int ret = SetEpollCtl();
	if (0 != ret)
	{
		printf("epoll_ctl fail!\n");
		FreeFd();
		return NULL;
	}
	printf("�󶨵�ַ&���ü���,����backlog���������еĳ���,һ�����ֵ��С30����\n");
	// �󶨵�ַ&���ü���,����backlog���������еĳ���,һ�����ֵ��С30����
	ret = SetListen();
	if (0 != ret)
	{
		printf("listen set fail!\n");
		FreeFd();
		return NULL;
	}

	struct epoll_event events[1024];
	// ��ʼ����socket�¼���������
	while (true)
	{
		int nfds = epoll_wait(m_epfd, events, m_maxEventSize, 1000);

		for (int i = 0; i < nfds; ++i)
		{
			printf("��ʼ����socket�¼���������2222...\n");
			if (events[i].data.fd != m_listenfd) // ������socket�ϵ��¼�
			{
				printf("�յ�δ������socket��listen_fd=%d, recive_fd=%d\n", m_listenfd, events[i].data.fd);
				continue;
			}
			
			// �յ��쳣epoll�¼�
			if (events[i].events&EPOLLERR || events[i].events&EPOLLHUP)
			{
				printf("Recive exception event!\n");
				continue;
			}

			struct sockaddr_in cliaddr;
			// ����Epoll�¼�                   
			socklen_t acceptLen = sizeof(struct sockaddr);//����epoll_event�ṹ��ı���,ev����ע���¼�,�������ڻش�Ҫ������¼� 
			int acceptfd = accept(m_listenfd, (sockaddr *)&cliaddr, &acceptLen);
			if (acceptfd >= 0)
			{
				char* ip = (char*)inet_ntoa(cliaddr.sin_addr);
				printf("�յ�[%s:%d]��Ϣ,listen_fd=%d, acceptfd=%d\n", ip, cliaddr.sin_port, m_listenfd, acceptfd);
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

// ����socket������ģʽ
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

// ���ø��õ�ַ
void EpollServer::SetReuseAddr(int fd)
{
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(&opt)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}
}

// ע��epoll�¼�
int EpollServer::SetEpollCtl()
{
	// ע��epoll�¼�
	struct epoll_event epEvent;
	epEvent.data.fd = m_listenfd;					//������Ҫ������¼���ص��ļ�������        
	epEvent.events = EPOLLIN | EPOLLET;			//����Ҫ������¼�����

	return	epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listenfd, &epEvent);
}

int EpollServer::SetListen()
{
	// �󶨵�ַ
	struct sockaddr_in svraddr;
	bzero(&svraddr, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svraddr.sin_port = htons(m_uListenPort);
	bind(m_listenfd, (sockaddr *)&svraddr, sizeof(svraddr));

	// ���ü���,����backlog���������еĳ���,һ�����ֵ��С30����
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