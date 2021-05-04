AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=syspub

syspub_SOURCES=epoll_def.cpp epoll_def.h epoll_reader.cpp epoll_reader.h epoll_server.cpp epoll_server.h event.cpp event.h msq_operator.cpp msq_operator.h select_client.cpp select_client.h sem_operator.cpp sem_operator.h shm_operator.cpp shm_operator.h thread_mutex.cpp thread_mutex.h thread_rw_lock.cpp thread_rw_lock.h thread_unix.cpp thread_unix.h my_log4cpp.cpp my_log4cpp.h main.cpp

syspub_CPPFLAGS=-I../include -L../lib -std=c++11
syspub_LDADD=-lpthread -llog4cpp
