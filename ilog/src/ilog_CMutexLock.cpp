/**
 * description:ª•≥‚À¯¿‡
 * author:wangzhia
 * create date:2013-10-28
 */
#include "ilog_CMutexLock.h"

ilog_CMutexLock::ilog_CMutexLock ()
{
	if (0 == pthread_mutex_init (&m_lock, NULL))
	{
		m_valid = true;
	}
	else
	{
		m_valid = false;
	}
}

ilog_CMutexLock::~ilog_CMutexLock ()
{
	pthread_mutex_destroy (&m_lock);
}

bool ilog_CMutexLock::IsValid ()
{
	return m_valid;
}
