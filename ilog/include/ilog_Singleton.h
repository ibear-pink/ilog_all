/**
 * description:µ•ÃÂ¿‡
 * create date:
 * author:
 */
 
#ifndef _ILOG_SINGLETON_H_
#define _ILOG_SINGLETON_H_

#include <iostream>

using namespace std;

template<class TYPE>
class GDF_Singleton
{
public:
	
	static TYPE *Instance(void);
	
	static void Close (void);

private:
	static TYPE *m_Instance;
};

template<class TYPE>
TYPE * GDF_Singleton<TYPE>::m_Instance = NULL;

template<class TYPE>
TYPE * GDF_Singleton<TYPE>::Instance (void)
{
	if (NULL != m_Instance)
	{
		return m_Instance;
	}
	else
	{
		m_Instance = new TYPE ();

		return m_Instance;
	}
}

template<class TYPE>
void GDF_Singleton<TYPE>::Close (void)
{
	if (NULL != m_Instance)
	{
		delete m_Instance;

		GDF_Singleton<TYPE>::m_Instance = NULL;
	}
}

#endif
