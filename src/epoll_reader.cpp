#include "epoll_reader.h"


PipeReadThread::PipeReadThread()
{
	m_rfd = -1;
}
PipeReadThread::~PipeReadThread()
{

}

void PipeReadThread::ReadPipe()
{
	printf("read pip start!\n");
	struct epoll_event ev;
	int epfd = epoll_create(1024);
	if (epfd < 0)
	{
		printf("epoll_create fail!\n");
		return;
	}

	pipemsg msg;
	while (true)
	{
		int ret = read(m_rfd, &msg, 14);
		if (ret <= 0)
		{
			continue;
		}

		if (14 != ret)
		{
			printf("recive error message, not 14!\n");
			return;
		}

		if (msg.fd <= 0)
		{
			printf("recive error message, fd less 0!\n");
			return;
		}


		if (msg.op == 1)
		{
			cout << "ReadThread, recv connect:" << msg.fd << ",errno:" << errno << endl;

			SetNoBlocking(msg.fd);

			ev.data.fd = msg.fd;
			ev.events = EPOLLIN | EPOLLET;
			//ע��ev                 
			ret = epoll_ctl(epfd, EPOLL_CTL_ADD, msg.fd, &ev);
			if (ret != 0)
			{
				cout << "ReadThread, epoll_ctl fail:" << ret << ",errno:" << errno << endl;

				close(msg.fd);
			}
			else
			{

			}

		}

		struct epoll_event events[1024];
		int nfds = epoll_wait(epfd, events, 1024, 500);
		//�����������������¼�             
		for (int i = 0; i < nfds; ++i)
		{
			cout << "ReadThread, events:" << events[i].events << ",errno:" << errno << endl;

			if (events[i].events&EPOLLIN)   //�����ݿɶ�          
			{
				char buf[1024];
				bzero(buf, 1024);
				int nread = read(events[i].data.fd, buf, 1024);
				if (nread > 0)    //��������
				{
					cout << "ReadThread, read:" << nread << ",errno:" << errno << endl;

					CDataObj dd;
					memcpy(&dd, buf, sizeof(dd));
					printf("dd=%d,%d,%s\n", dd.a, dd.b, dd.name);

				}
				else if (nread < 0) //��ȡʧ��
				{
					if (errno == EAGAIN)    //û��������
					{
						cout << "ReadThread, read:" << nread << ",errno:" << errno << ",no data" << endl;

						break;
					}
					else if (errno == EINTR)        //���ܱ��ڲ��ж��źŴ��,������֤�Է�����socket��δ�յ��˴���,Ӧ�ÿ���ʡ���ò��ж�
					{
						cout << "ReadThread, read:" << nread << ",errno:" << errno << ",interrupt" << endl;
					}
					else    //�ͻ��������ر�
					{
						cout << "ReadThread, read:" << nread << ",errno:" << errno << ",peer error" << endl;

						break;
					}
				}
				else if (nread == 0) //�ͻ��������ر�
				{
					cout << "ReadThread, read:" << nread << ",errno:" << errno << ",peer close" << endl;

					close(events[i].data.fd);
					break;
				}

			}
			else if (events[i].events&EPOLLERR || events[i].events&EPOLLHUP)    //���쳣����
			{
				//cout << "ReadThread, read:" << nread << ",errno:" << errno << ",err or hup" << endl;

				close(events[i].data.fd);
			}
		}
	}
}

void PipeReadThread::run()
{
	ReadPipe();


}

void PipeReadThread::SetReadPipe(int fd)
{
	m_rfd = fd;
}