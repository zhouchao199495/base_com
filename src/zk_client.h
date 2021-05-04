#pragma once

#include <string>
#include <iostream>
#include <include/zookeeper.h>
#include <include/zookeeper_log.h>
#include <include/zookeeper_version.h>

using std::string;
using std::cout;
using std::endl;

class ZKClient
{
public:
	ZKClient(string zkServer);
	~ZKClient();
	
public:
	/*
	*	����zookeeper
	*/
	bool connectZk();

	/*
	*	�Ͽ�zookeeper����
	*/
	bool disConnectZk();


	/*
	*	zookeeper�ڵ����
	*/
	void update();

	/*
	*	�����ڵ�
	*/
	int create(string path, string value);

	/*
	*	���ýڵ�
	*/
	int set(string path, string value);

	/*
	*	ɾ���ڵ�
	*/
	int deleteNode(string path);

	/*
	*	��ȡ�ڵ�
	*/
	string getNode(string path);

private:
	/*
	*	zookeeper����ע��ص�����
	*/
	static void watchHandler(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

	/*
	*	zookeeper�ڵ����ע��ص�����
	*/
	static void serviceWatch(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);


	static void zktestDumpStat(const struct Stat* stat);
	static void zktestStatCompletion(int rc, const char *value, const void *data);
	static void zktestStatCompletion2(int rc, const struct Stat *stat, const void *data);


	static void zktestVoidCompletion(int rc, const void* data);

private:
	zhandle_t* zhandle;
	string zkServer;
	unsigned short timeout;
	bool connectted;
	string rootPath;
};

