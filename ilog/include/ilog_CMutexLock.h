/**
 * description:ª•≥‚À¯¿‡
 * author:wangzhia
 * create date:2013-10-28
 */

#ifndef _ILOG_LOCK_H_
#define _ILOG_LOCK_H_

#include <pthread.h>
#include <stdio.h>

class ilog_CMutexLock
{
public:
	ilog_CMutexLock ();
	
	~ilog_CMutexLock ();
	
	int Lock () 
	{
		return pthread_mutex_lock (&m_lock);
	}
	
	int UnLock () 
	{
		return pthread_mutex_unlock (&m_lock);
	}
	
	bool IsValid ();

public:
	pthread_mutex_t m_lock;
	
	bool m_valid;
};

#endif
