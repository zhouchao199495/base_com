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
	*	连接zookeeper
	*/
	bool connectZk();

	/*
	*	断开zookeeper连接
	*/
	bool disConnectZk();


	/*
	*	zookeeper节点更新
	*/
	void update();

	/*
	*	创建节点
	*/
	int create(string path, string value);

	/*
	*	设置节点
	*/
	int set(string path, string value);

	/*
	*	删除节点
	*/
	int deleteNode(string path);

	/*
	*	获取节点
	*/
	string getNode(string path);

private:
	/*
	*	zookeeper连接注册回调函数
	*/
	static void watchHandler(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

	/*
	*	zookeeper节点更新注册回调函数
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

