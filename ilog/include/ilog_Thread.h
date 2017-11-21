/**
 * description:线程接口类定义
 * author:wangzhia
 * create date:2013-10-28
 */
 
#ifndef _ILOG_THREAD_H_
#define _ILOG_THREAD_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

const int THREAD_STACK_SIZE = 52428800;		//50M
typedef unsigned int		UnInt32;
typedef unsigned long long	UnInt64;
typedef int					Int32;
typedef long long			Int64;

/*线程状态*/
enum _THREAD_STAT
{
	THREAD_STAT_INITIALIZE		= 0,
	THREAD_STAT_RUNNING			= 1,
	THREAD_STAT_SUSPEND			= 2,
	THREAD_STAT_EXIT			= 3
};

typedef struct _ILOG_THREAD_INFO
{
	UnInt32			serialno;				//一组线程中线程序列号,默认0开始
	pthread_t		pid;					//线程ID
	pthread_attr_t	attr;					//线程属性
	char           	describe[1024+1]; 		//线程描述
	UnInt32			stat;					//0:初始化,1:正常运行2:线程挂起,3:线程退出
	void *			pclass;					//线程所属对象指针
	
	_ILOG_THREAD_INFO()
	{
		serialno = 0;
		pid = 0;
		stat = THREAD_STAT_INITIALIZE;
		pclass = NULL;
		memset (describe, 0, sizeof(describe));
	}
} sThreadInfo;

class Thread
{
private:
//	int m_ErrCode;
	
	sThreadInfo m_ThreadInfo;
	
	pthread_cond_t cv;
	pthread_mutex_t mutex;
	
protected:

	static void* threadFunction (void* data);
	
public:
	Thread();
	
	Thread (bool createSuspened, bool detach);
	
	virtual ~Thread();
	
	virtual void run (void) = 0;
	
	void setThreadState (UnInt32 state) {m_ThreadInfo.stat = state;}
	
	bool terminate (void);
	
	int wait();
	
	UnInt32 start (void);
	
	void exit (void);
	
	bool wakeUp (void);
	
	UnInt32 getThreadState (void) {return m_ThreadInfo.stat;};
	
	void getThreadInfo (sThreadInfo *threadInfo);
	
//	int getLastError (void) {return m_ErrCode;};
	
//	void setThreadName (char *threadName){ strcpy( m_ThreadName, threadName ); };
	
//	char* getThreadName (void){ return m_ThreadName; };
	
	pthread_t getThreadId (void) {return m_ThreadInfo.pid;};
	
	bool setPriority (int priority);
	
	UnInt32 getPriority (void);
	
	UnInt32 getConcurrency (void);
	
	void setConcurrency (int num);
	
	UnInt32 detach (void);
	
	UnInt32 join (void);
	
	bool yield (void);
	
	pthread_t self (void);
};

#endif
