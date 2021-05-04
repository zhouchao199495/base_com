#include "my_log4cpp.h"

#define RollingFileName "base_log4cpp.log"

MyLog4Cpp* MyLog4Cpp::_instance = new MyLog4Cpp();

MyLog4Cpp* MyLog4Cpp::getInstance()
{ //单例模式
    return _instance;
}

/**
  Appender：
    log4cpp::FileAppender             // 输出到文件
    log4cpp::RollingFileAppender      // 输出到回卷文件，即当文件到达某个大小后回卷
    log4cpp::OstreamAppender          // 输出到一个ostream类,比如cout
    log4cpp::StringQueueAppender      // 内存队列，比如queue<string>,可以往kafka推送
    log4cpp::Win32DebugAppender       // 发送到缺省系统调试器
    log4cpp::NTEventLogAppender       // 发送到win 事件日志

    // 以下的syslog均为linux系统日志，即/var/log/message
    log4cpp::RemoteSyslogAppender      // 输出到远程syslog服务器
    log4cpp::SyslogAppender            // 本地syslog
 **/
MyLog4Cpp::MyLog4Cpp():_root(Category::getRoot())
{
    PatternLayout * _ptnLyout = new PatternLayout();
    _ptnLyout->setConversionPattern("%d{%Y/%m/%d %H:%M:%S} [%5p] :%m%n");             //PatternLayout定义布局格式 
    //设置回卷格式fileappender
    // 超过指定文件大小循环写文件，超过5k重新写
    RollingFileAppender * _rollingFileAppender = new RollingFileAppender("rollingFileAppender", RollingFileName, 1 * 1024 * 1024,2) ; 
    _rollingFileAppender->setLayout(_ptnLyout);
 
    _root.addAppender(_rollingFileAppender);   //可以多次add不同类型的appender 实现向不同流输出
    _root.setPriority(Priority::DEBUG);
}

MyLog4Cpp::~MyLog4Cpp()
{
    Category::shutdown();
}

void MyLog4Cpp::printLog(Priority::PriorityLevel logLevel, string s)
{
    switch(logLevel)
    {
        case Priority::ERROR:
            _root.error(s);
            break;
         case Priority::WARN:
            _root.warn(s);
            break;
        case Priority::INFO:
            _root.info(s);
            break;
        case Priority::DEBUG:
            _root.debug(s);
            break;
        default:
            _root.info(s);
            break;
    }
}