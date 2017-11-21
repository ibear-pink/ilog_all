/**
 * description:线程接口类定义
 * author:wangzhia
 * create date:2013-10-28
 */

#include "ilog_Thread.h"

Thread::Thread()
{
	memset (&m_ThreadInfo, 0, sizeof(sThreadInfo));
	
	m_ThreadInfo.stat = THREAD_STAT_INITIALIZE;
}

Thread::~Thread()
{
}

bool Thread::terminate()
{
	return true;
}

UnInt32 Thread::start()
{
	if (0 != pthread_attr_init (&(m_ThreadInfo.attr)))
	{
		return -1;
	}
	
	/*判断默认栈大小和指定栈的大小，未实现*/
	
	if (0 != pthread_attr_setstacksize (&(m_ThreadInfo.attr), THREAD_STACK_SIZE))
	{
		pthread_attr_destroy (&(m_ThreadInfo.attr));
		return -1;
	}
	
	pthread_attr_setscope (&(m_ThreadInfo.attr), PTHREAD_SCOPE_SYSTEM);
	
	m_ThreadInfo.stat = THREAD_STAT_RUNNING;
	
	if (0 !=  pthread_create (&(m_ThreadInfo.pid), &(m_ThreadInfo.attr), Thread::threadFunction, static_cast<void *>(this)))
	{
		pthread_attr_destroy (&(m_ThreadInfo.attr));
		return -1;
	}
	
	return 0;
}

void Thread::exit()
{
	pthread_exit (NULL);
}

int Thread::wait()
{
	pthread_mutex_lock(&mutex);
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cv,NULL);
	timespec delay;
	delay.tv_sec = time(NULL) + 1;
	delay.tv_nsec = 0;
	pthread_cond_wait(&cv, &mutex);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cv);
	return 0;
}

bool Thread::wakeUp()
{
	pthread_cond_signal(&cv);
	return true;
}

bool Thread::setPriority (int priority)
{
	return true;
}

UnInt32 Thread::getPriority (void)
{
	return 0;
}

UnInt32 Thread::getConcurrency (void)
{
	return 0;
}

void Thread::setConcurrency (int num)
{
}

UnInt32 Thread::detach (void)
{
	return pthread_detach (m_ThreadInfo.pid);
}          

UnInt32 Thread::join (void)
{
	return pthread_join (m_ThreadInfo.pid, NULL);
}

bool Thread::yield (void)
{
	return true;
}

pthread_t self (void)
{
	return pthread_self();
}

void* Thread::threadFunction (void* data)
{
	Thread *thr = static_cast<Thread *>(data);
	
	thr->run();
	
	return NULL;
}

void Thread::getThreadInfo (sThreadInfo *threadInfo)
{
	memcpy (threadInfo, &m_ThreadInfo, sizeof(sThreadInfo));
}
