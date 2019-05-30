#include <generated/zookeeper.jute.h>
#include "zk_client.h"



ZKClient::ZKClient(string zkServer)
{
	this->zkServer = zkServer;
	this->zhandle = NULL;
	this->timeout = 5000;
	this->connectted = false;
	this->rootPath = "/";
}


ZKClient::~ZKClient()
{

}

bool ZKClient::connectZk()
{
	// �ж������Ƿ��Ѵ���
	if (connectted) 
		return true;

	// ��ʼ������
	zhandle = zookeeper_init(this->zkServer.c_str(), this->watchHandler, this->timeout, NULL, NULL, true);
	if (NULL == zhandle)
	{
		printf("connect to zookeeper [%s] fail, zk init fail!\n", this->zkServer.c_str());
		return false;
	}

	connectted = true;
	return true;
}

bool ZKClient::disConnectZk()
{
	if (zhandle)
	{
		zookeeper_close(zhandle);
		connectted = false;
	}
	return true;
}

void ZKClient::watchHandler(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
	printf("watchHandler:path[%s]\n", path);
}

void ZKClient::serviceWatch(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
	printf("serviceWatch:path[%s]\n", path);
}

void ZKClient::update()
{
	if (zhandle == NULL || connectted == false) 
	{
		return;
	}

	// ��÷������
	struct String_vector str_vec;
	int ret = zoo_wget_children(zhandle, rootPath.c_str(), this->serviceWatch, NULL, &str_vec);
	if (ret) 
	{
		cout << "Update --> read path:" << rootPath << " wrong, " << zerror(ret) << endl;
		return;
	}

	// ��ø��ݷ���ip:port
	for (int i = 0; i < str_vec.count; ++i) 
	{
		struct String_vector node_vec;
		string path = rootPath + "/" + str_vec.data[i];
		int ret = zoo_wget_children(zhandle, path.c_str(), this->serviceWatch, NULL, &node_vec);
		cout << "Update --> path:" << path << ", ret:" << ret << ", node's size:" << node_vec.count << endl;
		if (ret || node_vec.count != 1) 
		{
			continue;
		}

		
	}

}

/*
*	�����ڵ�
*/
int ZKClient::create(string path, string value)
{
	if (path.empty() || value.empty())
	{
		printf("create error, path[%s] or value[%s] is null!\n", path.c_str(), value.c_str());
		return -1;
	}

	int nRet = zoo_acreate(zhandle, path.c_str(), value.c_str(), value.length(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, this->zktestStatCompletion, "acreate");
	if (0 != nRet)
	{
		fprintf(stderr, "Error %d for %s\n", nRet, "acreate");
		return -1;
	}

	nRet = zoo_aexists(zhandle, path.c_str(), 1, this->zktestStatCompletion2, "aexists");
	if (0 != nRet)
	{
		fprintf(stderr, "Error %d for %s", nRet, "aexists");
		return -1;
	}

	return 0;
}

/*
*	���ýڵ�
*/
int ZKClient::set(string path, string value)
{
	if (path.empty() || value.empty())
	{
		printf("set error, path[%s] or value[%s] is null!\n", path.c_str(), value.c_str());
		return -1;
	}

	int  nRet = zoo_set(zhandle, path.c_str(), value.c_str(), value.length(), 0);
	if (0 != nRet)
	{
		printf("set path[%s] value[%s] failed, error %d!\n", path.c_str(), value.c_str(), nRet);
		return -1;
	}

	return 0;
}

/*
*	ɾ���ڵ�
*/
int ZKClient::deleteNode(string path)
{
	if (path.empty())
	{
		printf("delete node error, path[%s] is null!\n", path.c_str());
		return -1;
	}

	int  nRet = zoo_delete(zhandle, path.c_str(), 0);
	if (0 != nRet)
	{
		printf("delete node path[%s] failed, error %d!\n", path.c_str(), nRet);
		return -1;
	}
	
	return 0;
}

/*
*	��ȡ�ڵ�
*/
string ZKClient::getNode(string path)
{
	if (path.empty())
	{
		printf("set error, path[%s] or value[%s] is null!\n", path.c_str());
		return "";
	}
	char buff[1024] = { 0 };
	int buffLen = 1024;
	Stat stat;
	int  nRet = zoo_get(zhandle, path.c_str(), 0, buff, &buffLen, &stat);
	if (0 != nRet)
	{
		printf("delete node path[%s] failed, error %d!\n", path.c_str(), nRet);
		return "";
	}

	// ������صĻ��泤�Ⱥ�״̬�������򻺴治�㡣���»�ȡ
	if (buffLen != stat.dataLength)
	{
		printf("������Ϣ���ݳ���[%d]��ʵ��node���ݳ���[%d]����ȣ����°�node�������ݻ�ȡ��\n", buffLen, stat.dataLength);
		buffLen = stat.dataLength;
		char* newBuff = new char[stat.dataLength];
		nRet = zoo_get(zhandle, path.c_str(), 0, newBuff, &buffLen, &stat);

		string value = string(newBuff);
		delete[] newBuff;
		return value;
	}
	
	string value = string(buff);
	return value;
}


void ZKClient::zktestDumpStat(const struct Stat* stat)
{
	char tctimes[40];
	char tmtimes[40];

	time_t tctime;
	time_t tmtime;

	if (!stat)
	{
		fprintf(stderr, "null\n");
		return;
	}

	tctime = stat->ctime / 1000;
	tmtime = stat->mtime / 1000;

	ctime_r(&tmtime, tmtimes);
	ctime_r(&tctime, tctimes);

	fprintf(stderr, "\tctime = %s\tczxid=%llx\n"
		"\tmtime=%s\tmzxid=%llx\n"
		"\tversion=%x\taversion=%x\n"
		"\tephemearalOwner=%llx\n",
		tctimes, stat->czxid,
		tmtimes, stat->mzxid,
		(unsigned int)stat->version, (unsigned int)stat->aversion,
		stat->ephemeralOwner
		);
}

void ZKClient::zktestStatCompletion(int rc, const char *value, const void *data)
{
	fprintf(stderr, "%s: rc=%d Stat:\n", (char*)data, rc);

}

void ZKClient::zktestStatCompletion2(int rc, const struct Stat *stat, const void *data)
{
	fprintf(stderr, "%s: rc=%d Stat:\n", (char*)data, rc);
}

void ZKClient::zktestVoidCompletion(int rc, const void* data)
{
	fprintf(stderr, "[%s]: rc = %d\n", (char*)(data == 0 ? "null" : data), rc);
}

