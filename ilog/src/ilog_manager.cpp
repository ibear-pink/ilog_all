/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
 
#include "ilog_manager.h"

ilog_manager::ilog_manager (void)
{
	m_LogServer.clear ();
	m_IsRun = false;
}

ilog_manager::~ilog_manager (void)
{
	if (m_LogServer.size() != 0)
	{
		this->Close();
	}
	
	for (map<int,ilog_performance*>::iterator it = m_Performances.begin();it!=m_Performances.end();it++)
	{
		ilog_performance *per = it->second;
		delete per;
		per = NULL;
	}
	m_Performances.clear();
	for (map<int,ilog_statinfo*>::iterator it = m_Statinfos.begin();it!=m_Statinfos.end();it++)
	{
		ilog_statinfo *stat = it->second;
		delete stat;
		stat = NULL;
	}
	m_Statinfos.clear();
}

ilog_cfgserver *ilog_manager::getCfgserver()
{
	return m_CfgInfo;
}

bool ilog_manager::IsRun ()
{
	return m_IsRun;
}

bool ilog_manager::Close (void)
{
	setThreadState(THREAD_STAT_EXIT);
	this->join();
	mapLogServer::iterator iter;
	for (iter = m_LogServer.begin(); iter != m_LogServer.end(); iter++)
	{
		delete (iter->second);
	}
	m_LogServer.clear();
	
	return true;
}

void ilog_manager::run (void)
{
	UnInt32 nLogType = LOG_APP_STATINFO;
	while (getThreadState() == THREAD_STAT_RUNNING)
	{
		for (mapLogServer::iterator iter = m_LogServer.begin(); iter != m_LogServer.end(); iter++)
		{
			if (0 != iter->second->Backup())
			{
				printf ("FILE[%s]LINE[%d]ERR_MSG[Backup Is Failed.]\n", __FILE__, __LINE__);
			}
			//生成统计日志
			if (m_CfgInfo->log_thread_num == 0)
			{
				ilog_file *pLogFile = GetLogFile(GetLogName(m_CfgInfo->log_name), nLogType);
				if (NULL != pLogFile && pLogFile->IsBackup())
				{
					char buffer[MAX_LOG_LENGTH] = {0};
					ilog_statinfo* statinfo = getStatinfo(0);
					if (statinfo->num == 0 && statinfo->all_time == 0)
					{
						continue;
					}
					sprintf(buffer,"%d|%d|%d|%d|%d\n",statinfo->num,statinfo->all_time,statinfo->average_time,statinfo->max_time,statinfo->min_time);
					clearStatinfo(0);
					if (0!=pLogFile->DoLogAndBackUp (buffer))
					{
						printf("FILE OPEN IS FAIL!!!!\n");
						setThreadState(THREAD_STAT_EXIT);
					}
				}
			}
			else
			{
				for (int threadNum = 0; threadNum< m_CfgInfo->log_thread_num; threadNum++)
				{
					ilog_file *pLogFile = GetLogFile(GetLogName(m_CfgInfo->log_name,threadNum), nLogType);
					if (NULL != pLogFile && pLogFile->IsBackup())
					{
						char buffer[MAX_LOG_LENGTH] = {0};
						ilog_statinfo* statinfo = getStatinfo(threadNum);
						if (statinfo->num == 0 && statinfo->all_time == 0)
						{
							continue;
						}
						sprintf(buffer,"%d|%d|%d|%d|%d\n",statinfo->num,statinfo->all_time,statinfo->average_time,statinfo->max_time,statinfo->min_time);
						clearStatinfo(threadNum);
						if (0!=pLogFile->DoLogAndBackUp (buffer))
						{
							printf("FILE OPEN IS FAIL!!!!\n");
							setThreadState(THREAD_STAT_EXIT);
						}
					}
				}
			}
		}
		sleep (5);
	}
	return;
}

bool ilog_manager::Init (ilog_cfgserver *pCfgInfo)
{
	strcpy(m_processNum,"");
	
	return InitLogServer(pCfgInfo);
}

bool ilog_manager::Init (ilog_cfgserver *pCfgInfo,char *processNum)
{
	strcpy(m_processNum,processNum);

	return InitLogServer(pCfgInfo);
}

bool ilog_manager::InitLogServer(ilog_cfgserver *pCfgInfo)
{
	m_CfgInfo = pCfgInfo;
	int iRet = 0;
	if (pCfgInfo->log_thread_num == 0)
	{
		ilog_server *pServer = new ilog_server ();
		pServer->setFileType(pCfgInfo->file_type);
		
		char log_name[128] = {0};
		if (strcmp(m_processNum,"") == 0)
		{
			sprintf(log_name,"%s",pCfgInfo->log_name);
		}
		else
		{
			sprintf(log_name,"%s_%s",pCfgInfo->log_name,m_processNum);
		}
		pServer->SetLogInfo (pCfgInfo->log_path, log_name);
		
		for (int j = 0; j < pCfgInfo->log_file_num; j++)
		{
			struct ilog_cfgfile *pFileNode = &(pCfgInfo->logfile_infos[j]);
			iRet = pServer->AddLogFile (pFileNode->log_type, pFileNode->log_interval, pFileNode->log_backup_size, pFileNode->log_procmode, pFileNode->log_backup_type);
			if (0 != iRet)
			{
				printf ("FILE[%s]LINE[%d]ERR_MSG[AddLogFile Is Failed.]\n", __FILE__, __LINE__);
				return false;
			}
		}
		
		iRet = RegisterLog (string(log_name), pServer);
		if (0 != iRet)
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[RegisterLog Is Failed.\n]", __FILE__, __LINE__);
			return false;
		}
		ilog_statinfo* statinfo = new ilog_statinfo();
		m_Statinfos.insert(pair<int, ilog_statinfo*>(0,statinfo));
	}
	else
	{
		for (int j = 0; j<= pCfgInfo->log_thread_num; j++)
		{
			ilog_server *pServer = new ilog_server ();
			pServer->setFileType(pCfgInfo->file_type);
			char log_name[128] = {0};
			if (strcmp(m_processNum,"") == 0)
			{
				sprintf(log_name,"%s_%d",pCfgInfo->log_name,j);
			}
			else
			{
				sprintf(log_name,"%s_%s_%d",pCfgInfo->log_name,m_processNum,j);
			}
			
			pServer->SetLogInfo (pCfgInfo->log_path, log_name);
			for (int z = 0; z < pCfgInfo->log_file_num; z++)
			{
				struct ilog_cfgfile *pFileNode = &(pCfgInfo->logfile_infos[z]);
				iRet = pServer->AddLogFile (pFileNode->log_type, pFileNode->log_interval, pFileNode->log_backup_size, pFileNode->log_procmode, pFileNode->log_backup_type);
				if (0 != iRet)
				{
					printf ("FILE[%s]LINE[%d]ERR_MSG[AddLogFile Is Failed.]\n", __FILE__, __LINE__);
					return false;
				}
			}
			
			iRet = RegisterLog (string(log_name), pServer);
			if (0 != iRet)
			{
				printf ("FILE[%s]LINE[%d]ERR_MSG[RegisterLog Is Failed.\n]", __FILE__, __LINE__);
				return false;
			}
			ilog_statinfo* statinfo = new ilog_statinfo();
			m_Statinfos.insert(pair<int, ilog_statinfo*>(j,statinfo));
		}
	}
	
	return true;
}

ilog_file *ilog_manager::GetLogFile (const string m_StrLogName, UnInt32 nLogType)
{
	mapLogServer::iterator iter = m_LogServer.find(m_StrLogName);
	if (iter != m_LogServer.end())
	{
		return (iter->second)->GetLogFile(nLogType);
	}
	else
	{
		return NULL;
	}
}

int ilog_manager::Backup (void)
{
	mapLogServer::iterator iter;
	for (iter = m_LogServer.begin(); iter != m_LogServer.end(); iter++)
	{
		ilog_server *pNode = iter->second;
		
		if (0 != pNode->Backup())
		{
			return -1;
		}
	}
	
	return 0;
}

int ilog_manager::RegisterLog (string m_StrLogName, ilog_server *pServer)
{
	mapLogServer::iterator iter = m_LogServer.find(m_StrLogName);
	
	if (iter == m_LogServer.end())
	{
		m_LogServer.insert (pair<string, ilog_server *>(m_StrLogName, pServer));
	}
	
	return 0;
}

int ilog_manager::UnRegisterLog (string m_StrLogName, ilog_server *pServer)
{
	mapLogServer::iterator iter = m_LogServer.find(m_StrLogName);
	
	if (iter != m_LogServer.end())
	{
		delete (iter->second);
		
		m_LogServer.erase(iter);
	}
	
	return 0;
}

string ilog_manager::GetLogName (const string m_StrLogName)
{
	char logname[128] = {0};
	if (strcmp(m_processNum,"") == 0)
	{
		sprintf(logname,"%s",m_StrLogName.c_str());
	}
	else
	{
		sprintf(logname,"%s_%s",m_StrLogName.c_str(),m_processNum);
	}
	return string(logname);
}

string ilog_manager::GetLogName (const string m_StrLogName,UnInt32 threadNum)
{
	char logname[128] = {0};
	if (strcmp(m_processNum,"") == 0)
	{
		sprintf(logname,"%s_%d",m_StrLogName.c_str(),threadNum);
	}
	else
	{
		sprintf(logname,"%s_%s_%d",m_StrLogName.c_str(),m_processNum,threadNum);
	}
	return string(logname);
}

void ilog_manager::SetPerformance (int threadNum,ilog_performance *performance)
{
	map<int,ilog_performance*>::iterator it = m_Performances.find(threadNum);
	if (it!=m_Performances.end())
	{
		ilog_performance *temp = it->second;
		temp->begin_time = performance->begin_time;
		temp->last_time = performance->last_time;
		temp->end_time = performance->end_time;
	}
	else
		m_Performances.insert(pair<int, ilog_performance*>(threadNum,performance));
	return;
}

ilog_performance * ilog_manager::GetPerformance (int threadNum)
{
	map<int,ilog_performance*>::iterator it = m_Performances.find(threadNum);
	if (it!=m_Performances.end())
		return it->second;
	else
		return NULL;
}

void ilog_manager::ClearPerformance (int threadNum)
{
	map<int,ilog_performance*>::iterator it = m_Performances.find(threadNum);
	if (it!=m_Performances.end())
	{
		ilog_performance* performance = it->second;
		performance->begin_time = 0;
		performance->end_time = 0;
		performance->last_time = 0;
		return;
	}
	else
		return;
}

char * ilog_manager::getTimeDiff()
{
	return m_timeDiff;
}

void ilog_manager::setTimeDiff(char *timeDiff)
{
	strcpy(m_timeDiff,timeDiff);
	return;
}

void ilog_manager::clearTimeDiff ()
{
	strcpy(m_timeDiff,"");
	return;
}

ilog_statinfo * ilog_manager::getStatinfo(int threadNum)
{
	map<int,ilog_statinfo*>::iterator it = m_Statinfos.find(threadNum);
	if (it!=m_Statinfos.end())
		return it->second;
	else
		return NULL;
}

void ilog_manager::setStatinfo(int threadNum,ilog_statinfo *statinfo)
{
	map<int,ilog_statinfo*>::iterator it = m_Statinfos.find(threadNum);
	if (it!=m_Statinfos.end())
	{
		ilog_statinfo *temp = it->second;
		temp->average_time = statinfo->average_time;
		temp->all_time = statinfo->all_time;
		temp->max_time = statinfo->max_time;
		temp->min_time = statinfo->min_time;
		temp->num = statinfo->num;
	}
	else
		m_Statinfos.insert(pair<int, ilog_statinfo*>(threadNum,statinfo));
	return;
}

void ilog_manager::clearStatinfo(int threadNum)
{
	map<int,ilog_statinfo*>::iterator it = m_Statinfos.find(threadNum);
	if (it!=m_Statinfos.end())
	{
		ilog_statinfo* statinfo = it->second;
		statinfo->all_time = 0;
		statinfo->average_time = 0;
		statinfo->max_time = 0;
		statinfo->min_time = 0;
		statinfo->num = 0;
		return;
	}
	else
		return;
}

void ilog_manager::ilog_Begin(UnInt32 threadNum)
{
	ilog_performance* performance = GetPerformance(threadNum);
	if (performance == NULL)
	{
		performance = new ilog_performance();
	}
	performance->begin_time = getUTime();
	performance->last_time = performance->begin_time;
	SetPerformance(threadNum,performance);
	clearTimeDiff();
}

void ilog_manager::ilog_End(UnInt32 threadNum)
{
	char timeDiff[100] = {0};
	ilog_performance* performance = GetPerformance(threadNum);
	performance->end_time = getUTime();
	SetPerformance(threadNum,performance);
	int diff_time = performance->end_time - performance->begin_time;
	
	sprintf(timeDiff,"[%d]%s|end:%ld",diff_time,getTimeDiff(),performance->end_time - performance->last_time);
	setTimeDiff(timeDiff);
	//统计日志
	ilog_statinfo* statinfo = getStatinfo(threadNum);
	
	statinfo->all_time += diff_time;
	statinfo->max_time = statinfo->max_time<diff_time?diff_time:statinfo->max_time;
	statinfo->min_time = statinfo->num==0?diff_time:statinfo->min_time;
	statinfo->min_time = statinfo->min_time>diff_time?diff_time:statinfo->min_time;
	statinfo->num++;
	statinfo->average_time = statinfo->all_time/statinfo->num;
	setStatinfo(threadNum,statinfo);
}

void ilog_manager::ilog_Mark(const char *name,UnInt32 threadNum)
{
	ilog_performance* performance = GetPerformance(threadNum);
	long last_time = getUTime();
	char timeDiff[100] = {0};
	if (strcmp(getTimeDiff(),"")==0)
	{
		if (strcmp(name,"") == 0)
		{
			sprintf(timeDiff,"%ld",last_time - performance->last_time);
		}
		else
		{
			sprintf(timeDiff,"%s:%ld",name,last_time - performance->last_time);
		}
	}
	else
	{
		if (strcmp(name,"") == 0)
		{
			sprintf(timeDiff,"%s|%ld",getTimeDiff(),last_time - performance->last_time);
		}
		else
		{
			sprintf(timeDiff,"%s|%s:%ld",getTimeDiff(),name,last_time - performance->last_time);
		}
	}
	setTimeDiff(timeDiff);
	performance->last_time = last_time;
	SetPerformance(threadNum,performance);
}

void ilog_manager::WriteLog (string log_name,UnInt32 nLogType, UnInt32 threadNum, char *description)
{
	string m_StrLogName = string(m_CfgInfo->log_name);
#ifdef _DEBUG_INFO_
	printf("[%d]%s_%d=%s\n",nLogType,log_name.c_str(),threadNum,description);
#endif
	long t1 = getUTime();
	char buffer[MAX_LOG_LENGTH] = {0};
	char szDescription[MAX_LOG_LENGTH] = {0};
	strcpy(szDescription, description);
	int len = strlen(szDescription);
	if (szDescription[len-1] == '\n')
	{
		szDescription[len-1] = '\0';
	}
	
	if (nLogType == LOG_SYS_RUNNING ||
		nLogType == LOG_SYS_NORMAL)
	{
		sprintf(buffer,"%s\n", szDescription);
	}
	else if (nLogType == LOG_SYS_ERROR ||
			 nLogType == LOG_SYS_WARNING)
	{
		char szTime[25];
		memset(szTime, 0, sizeof(szTime));
		getSysDatetoStr(szTime);
		sprintf(buffer,"[%s] %s\n", szTime, szDescription);
	}
	else if (nLogType == LOG_APP_PERFORMANCE)
	{
		sprintf(buffer,"%s %s\n",szDescription,getTimeDiff());
		ClearPerformance(threadNum);
	}
	else if (nLogType == LOG_APP_STATINFO)
	{
		ilog_statinfo* statinfo = getStatinfo(threadNum);
		sprintf(buffer,"%s %d|%d|%d|%d|%d\n",szDescription,statinfo->num,statinfo->all_time,statinfo->average_time,statinfo->max_time,statinfo->min_time);
		clearStatinfo(threadNum);
	}
	long t2 = getUTime();
	
	ilog_file *pLogFile = GetLogFile(GetLogName(log_name,threadNum), nLogType);
	long t3 = getUTime();
	if (NULL != pLogFile)
	{
		pLogFile->DoLog (buffer);
	}
	long t4 = getUTime();
#ifdef _DEBUG_INFO_
	printf("[%d]%ld,%ld,%ld\n",nLogType,t2-t1,t3-t2,t4-t3);
#endif
}


