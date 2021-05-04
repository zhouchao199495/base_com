#ifndef __MY_LOG4CPP_H
#define __MY_LOG4CPP_H

#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <iostream>
#include <sstream>
#include <string>
 
using std::cout;
using std::endl;
using std::ostringstream;
using std::string;
using namespace log4cpp;

#define LOGERROR(s) { LOGFORMATTER(s) LOGLEVEL(Priority::ERROR, log) }
#define LOGWARN(s)  { LOGFORMATTER(s) LOGLEVEL(Priority::WARN, log) }
#define LOGINFO(s)  { LOGFORMATTER(s) LOGLEVEL(Priority::INFO, log) }
#define LOGDEBUG(s) { LOGFORMATTER(s) LOGLEVEL(Priority::DEBUG, log) }

#define LOGFORMATTER(s) \
    ostringstream oss; \
    oss<<  __FILE__;\
    oss<<"|"; \
    oss<<__FUNCTION__; \
    oss<<"|"; \
    oss<<std::to_string(__LINE__);\
    oss<<"|"; \
    oss<<s; \
    string log = oss.str(); \

#define LOGLEVEL(level, log) MyLog4Cpp::getInstance()->printLog(level, log);
    

// 基于log4cpp日志组件封装的日志类
class MyLog4Cpp
{
public:
    static MyLog4Cpp* getInstance();
    ~MyLog4Cpp();

public:
    void printLog(Priority::PriorityLevel logLevel, string s);

private:
    static MyLog4Cpp* _instance;
    MyLog4Cpp();

    Category &_root;
};
#endif