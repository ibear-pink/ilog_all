//
//  ilog.cpp
//  ibf
//
//  Created by wangzhia on 2017/5/25.
//  Copyright 2017 wangzhia. All rights reserved.
//

#include "ilog.h"

ilog::ilog()
{

}

ilog::~ilog()
{
	ilog_Close();
}

bool ilog::ilog_Close (void)
{
	for (map<string,ilog_manager*>::iterator it = m_LogManagers.begin(); it != m_LogManagers.end(); it++)
	{
		ilog_manager *imanage = it->second;
		if (!imanage->Close())
		{
			return false;
		}
		delete imanage;
		imanage = NULL;
	}
	m_LogManagers.clear();
	return true;
}

bool ilog::ilog_Init (ilog_cfgserver *pCfgInfo)
{
	char log_name[64] = {0};
	sprintf(log_name,"%s",pCfgInfo->log_name);
	map<string,ilog_manager*>::iterator it = m_LogManagers.find(string(log_name));
	if (it == m_LogManagers.end())
	{
		ilog_manager *im = new ilog_manager();
		m_LogManagers.insert(pair<string,ilog_manager*>(string(log_name),im));
		return im->Init(pCfgInfo);
	}
	else
	{
		return it->second->Init(pCfgInfo);
	}
}

bool ilog::ilog_Init (ilog_cfgserver *pCfgInfo,char *processNum)
{
	char log_name[64] = {0};
	sprintf(log_name,"%s_%s",pCfgInfo->log_name,processNum);
	map<string,ilog_manager*>::iterator it = m_LogManagers.find(string(log_name));
	if (it == m_LogManagers.end())
	{
		ilog_manager *im = new ilog_manager();
		m_LogManagers.insert(pair<string,ilog_manager*>(string(log_name),im));
		return im->Init(pCfgInfo,processNum);
	}
	else
	{
		return it->second->Init(pCfgInfo,processNum);
	}
}

bool ilog::ilog_Init (ilog_cfgserver *pCfgInfo,int processNum)
{
	char log_name[64] = {0};
	char cprocessNum[64] = {0};
	sprintf(cprocessNum,"%d",processNum);
	sprintf(log_name,"%s_%d",pCfgInfo->log_name,processNum);
	map<string,ilog_manager*>::iterator it = m_LogManagers.find(string(log_name));
	if (it == m_LogManagers.end())
	{
		ilog_manager *im = new ilog_manager();
		m_LogManagers.insert(pair<string,ilog_manager*>(string(log_name),im));
		return im->Init(pCfgInfo,cprocessNum);
	}
	else
	{
		return it->second->Init(pCfgInfo,cprocessNum);
	}
}

void ilog::ilog_Run ()
{
	for(map<string,ilog_manager*>::iterator it = m_LogManagers.begin();it!=m_LogManagers.end();it++)
	{
		ilog_cfgserver *serv = it->second->getCfgserver();
		if (!it->second->IsRun())
		{
			it->second->start();
		}
	}
}

ilog_manager* ilog::GetLogManager(string log_name)
{
	map<string,ilog_manager*>::iterator it = m_LogManagers.find(log_name);
	if (it != m_LogManagers.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

bool ilog_IsRun (string log_name)
{
	App_iLog::Instance()->GetLogManager(log_name)->IsRun();
}

void ilog_Begin(string log_name)
{
	App_iLog::Instance()->GetLogManager(log_name)->ilog_Begin(0);
}

void ilog_End(string log_name)
{
	App_iLog::Instance()->GetLogManager(log_name)->ilog_End(0);
}

void ilog_Mark(string log_name)
{
	char name[2]="";
	App_iLog::Instance()->GetLogManager(log_name)->ilog_Mark(name,0);
}

void ilog_Mark(string log_name,const char *name)
{
	App_iLog::Instance()->GetLogManager(log_name)->ilog_Mark(name,0);
}

void WriteLog (string log_name,UnInt32 nLogType, const char *fmt, ...)
{
	char szDescription[MAX_LOG_LENGTH];
	memset(szDescription, 0, sizeof(szDescription));
	va_list args;
	va_start(args, fmt);
	vsnprintf (szDescription, MAX_LOG_LENGTH, fmt, args);
	va_end (args);
	ilog_cfgserver *serv = App_iLog::Instance()->GetLogManager(log_name)->getCfgserver();
	App_iLog::Instance()->GetLogManager(log_name)->WriteLog(serv->log_name,nLogType, 0, szDescription);
}

void ilog_Begin(string log_name,UnInt32 threadNum)
{
	App_iLog::Instance()->GetLogManager(log_name)->ilog_Begin(threadNum);
}

void ilog_End(string log_name,UnInt32 threadNum)
{
	App_iLog::Instance()->GetLogManager(log_name)->ilog_End(threadNum);
}

void ilog_Mark(string log_name,UnInt32 threadNum)
{
	char name[2]="";
	App_iLog::Instance()->GetLogManager(log_name)->ilog_Mark(name,threadNum);
}

void ilog_Mark(string log_name,const char *name,UnInt32 threadNum)
{
	App_iLog::Instance()->GetLogManager(log_name)->ilog_Mark(name,threadNum);
}

void WriteLog (string log_name,UnInt32 nLogType, UnInt32 threadNum, const char *fmt, ...)
{
	char szDescription[MAX_LOG_LENGTH];
	memset(szDescription, 0, sizeof(szDescription));
	va_list args;
	va_start(args, fmt);
	vsnprintf (szDescription, MAX_LOG_LENGTH, fmt, args);
	va_end (args);
	ilog_cfgserver *serv = App_iLog::Instance()->GetLogManager(log_name)->getCfgserver();
	App_iLog::Instance()->GetLogManager(log_name)->WriteLog(serv->log_name,nLogType, threadNum, szDescription);
}
