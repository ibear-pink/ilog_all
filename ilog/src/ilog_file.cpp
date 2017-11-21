/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
 
#include "ilog_file.h"

ilog_file::ilog_file (void)
{
	m_LogType = 0;
	
	m_IntervalTime = 0;
	
	m_ProcMode = 0;
	
	m_BackUpType = 0;
	
	m_BackupSize = 0;
	
	m_File = NULL;
	
	m_MutexLocker = new ilog_CMutexLock();
	
	m_OTime = time(NULL);
	
#ifdef _SDFS_LOG_
	m_Filetype = 0;
#else
	m_Filetype = 1;
#endif
	
	memset (m_CreateTime, 0, sizeof(m_CreateTime));
	
	getCurDateTime (m_CreateTime);
}

ilog_file::~ilog_file (void)
{
	if (NULL != m_File)
	{
		delete m_File;
		m_File = NULL;
	}
	if (NULL != m_MutexLocker)
	{
		delete m_MutexLocker;
		m_MutexLocker = NULL;
	}
	
	setThreadState(THREAD_STAT_EXIT);
	this->join();
	printf("ilog_file is distory!!!\n");
}

void ilog_file::setSDFSType()
{
#ifdef _SDFS_LOG_
	m_Filetype = 0;
#else
	printf("没有SDFS环境，默认本地文件系统!!\n");
	m_Filetype = 1;
#endif
}

void ilog_file::setSYSTEMType()
{
	m_Filetype = 1;
}

UnInt32 ilog_file::getLogType()
{
	return m_LogType;
}

int ilog_file::InitLogFile (const string &sLogPath, const string &sLogName, UnInt32 nLogType, UnInt32 nInterval, UnInt32 nBackupSize, Int32 nProcMode, Int32 backupType)
{
	m_LogPath = sLogPath;
	
	m_StrLogName = sLogName;
	
	m_LogType = nLogType;
	
	m_IntervalTime = nInterval;
	
	m_BackupSize = nBackupSize;
	
	m_ProcMode = nProcMode;

	m_BackUpType = backupType;
	
	char szCurrTime[14+1] = {0};
	char tLogPath[1024] = {0};
	char tLogName[1024] = {0};
	
	sprintf (tLogPath, "%s/%s", m_LogPath.c_str(), STR_PATH_TEMP);
	
	if (m_BackUpType == LOG_BACKUP_DATE)
	{
		char tCurTime[14+1] = {0};
		getCurDateTime (tCurTime);
		strncpy (szCurrTime, tCurTime, 8);
		szCurrTime[8] = '\0';
	}
	else if (m_BackUpType == LOG_BACKUP_HOUR)
	{
		char tCurTime[14+1] = {0};
		getCurDateTime (tCurTime);
		strncpy (szCurrTime, tCurTime, 10);
		szCurrTime[10] = '\0';
	}
	else
	{
		getCurDateTime (szCurrTime);
	}

	switch (m_LogType)
	{
		case LOG_SYS_NORMAL:
			sprintf (tLogName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_NORMAL, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_RUNNING:
			sprintf (tLogName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_RUNNING, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_WARNING:
			sprintf (tLogName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_WARNING, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_ERROR:
			sprintf (tLogName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_ERROR, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_APP_STATINFO:
			sprintf (tLogName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_APP_STATINFO, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_APP_PERFORMANCE:
			sprintf (tLogName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_APP_PERFORMANCE, szCurrTime, FILE_EXTEN_NAME);
			break;
		default:
			break;
	}
	if (m_Filetype == 0)
	{
#ifdef _SDFS_LOG_
		m_File = new SDFS_GDF_FILE (tLogPath, tLogName);
#endif
		if (m_File == NULL || 0 != m_File->Open ("a+"))
		{
			m_Filetype = 1;
			if (m_File != NULL)
			{
				delete m_File;
				m_File = NULL;
			}
			m_File = new SYSTEM_GDF_FILE (tLogPath, tLogName);
			if (0 != m_File->Open ("a+"))
			{
				printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
				return -1;
			}
		}
	}
	else
	{
		m_File = new SYSTEM_GDF_FILE (tLogPath, tLogName);
		if (0 != m_File->Open ("a+"))
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
			
			return -1;
		}
	}
	
	
	if (SERVER_ASYNC_MODE == m_ProcMode)
	{
		this->m_LogQueue.setMaxNum(MAX_DEQUE_NUM);
		this->m_LogQueue.setDelayTime(1);
		this->start();
	}
	
	return 0;
}

void ilog_file::CreateNewLogName(char *backupName)
{
	char szCurrTime[14+1] = {0};
	
	memset (szCurrTime, 0, sizeof(szCurrTime));
	
	if (m_BackUpType == LOG_BACKUP_DATE)
	{
		char tCurTime[14+1] = {0};
		getCurDateTime (tCurTime);
		strncpy (szCurrTime, tCurTime, 8);
		szCurrTime[8] = '\0';
	}
	else if (m_BackUpType == LOG_BACKUP_HOUR)
	{
		char tCurTime[14+1] = {0};
		getCurDateTime (tCurTime);
		strncpy (szCurrTime, tCurTime, 10);
		szCurrTime[10] = '\0';
	}
	else
	{
		getCurDateTime (szCurrTime);
	}
	
	switch (m_LogType)
	{
		case LOG_SYS_NORMAL:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_NORMAL, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_RUNNING:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_RUNNING, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_WARNING:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_WARNING, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_ERROR:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_ERROR, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_APP_STATINFO:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_APP_STATINFO, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_APP_PERFORMANCE:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_APP_PERFORMANCE, szCurrTime, FILE_EXTEN_NAME);
			break;
		default:
			break;
	}
	return ;
}

int ilog_file::DoLogAndBackUp (const char *sBuf)
{
	char backupPath[1024] = {0};
	char backupFile[1024] = {0};
	char newsourceFile[1024] = {0};
	char sourcePath[1024] = {0};

	m_File->Write (sBuf, strlen(sBuf), 1);
	m_File->Fflush();
	CreateNewLogName(newsourceFile);
	sprintf (sourcePath, "%s/%s", m_LogPath.c_str(), STR_PATH_TEMP);
	sprintf (backupPath, "%s/%s", m_LogPath.c_str(), STR_PATH_RUNLOG);
	sprintf (backupFile, "%s/%s", backupPath, m_File->GetFileName());
	
	GDF_FILE *pSourFile = m_File;
	if(NULL != pSourFile)
	{
		pSourFile->Close ();
		pSourFile->Rename(backupFile);
		pSourFile->Remove();
	}
	if (NULL != pSourFile)
	{
		delete pSourFile;
		pSourFile = NULL;
	}
	
	GDF_FILE *pTemp = NULL;
	if (m_Filetype == 0)
	{
#ifdef _SDFS_LOG_
		pTemp = new SDFS_GDF_FILE (sourcePath, newsourceFile);
#endif
		if (0 != pTemp->Open ("a+"))
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
			return -1;
		}
	}
	else
	{
		pTemp = new SYSTEM_GDF_FILE (sourcePath, newsourceFile);
		if (0 != pTemp->Open ("a+"))
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
			return -1;
		}
	}
	m_File = pTemp;

	return 0;
}

void ilog_file::DoLog (const char *sBuf)
{
	if (m_ProcMode == SERVER_SYNC_MODE)
	{
		m_File->Write (sBuf, strlen(sBuf), 1);

		m_File->Fflush();
	}
	else if (m_ProcMode == SERVER_ASYNC_MODE)
	{
		stLogInfo *pNode = (stLogInfo *)malloc(sizeof(stLogInfo));
		if (pNode != NULL)
		{
			strcpy (pNode->buff, sBuf);
			pNode->size = strlen (pNode->buff);
			m_LogQueue.push_first (pNode);
		}
	}
}

void ilog_file::run (void)
{
	stLogInfo *pStr = NULL;
	while (getThreadState() == THREAD_STAT_RUNNING)
	{
		pStr = m_LogQueue.pop();
		if (NULL == pStr)
		{
			usleep (20);
			continue;
		}
		m_File->Write (pStr->buff, pStr->size, 1);
		m_File->Fflush();
		free(pStr);
		pStr = NULL;
	}
}

int ilog_file::BackupLog (void)
{
	struct stat buf;

	if (!this->IsBackup())
	{
		return 0;
	}
	char backupPath[1024] = {0};
	char backupName[1024] = {0};
	char backupFile[1024] = {0};
	char sourPath[1024] = {0};
	char szCurrTime[14+1] = {0};
	
	memset (backupPath, 0, sizeof(backupPath));
	memset (backupName, 0, sizeof(backupName));
	memset (backupFile, 0, sizeof(backupFile));
	memset (sourPath, 0, sizeof(sourPath));
	memset (szCurrTime, 0, sizeof(szCurrTime));
	
	if (m_BackUpType == LOG_BACKUP_DATE)
	{
		char tCurTime[14+1] = {0};
		getCurDateTime (tCurTime);
		strncpy (szCurrTime, tCurTime, 8);
		szCurrTime[8] = '\0';
	}
	else if (m_BackUpType == LOG_BACKUP_HOUR)
	{
		char tCurTime[14+1] = {0};
		getCurDateTime (tCurTime);
		strncpy (szCurrTime, tCurTime, 10);
		szCurrTime[10] = '\0';
	}
	else
	{
		getCurDateTime (szCurrTime);
	}
	
	sprintf (backupPath, "%s/%s", m_LogPath.c_str(), STR_PATH_RUNLOG);
	sprintf (sourPath, "%s/%s", m_LogPath.c_str(), STR_PATH_TEMP);

	switch (m_LogType)
	{
		case LOG_SYS_NORMAL:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_NORMAL, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_RUNNING:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_RUNNING, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_WARNING:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_WARNING, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_SYS_ERROR:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_SYS_ERROR, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_APP_STATINFO:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_APP_STATINFO, szCurrTime, FILE_EXTEN_NAME);
			break;
		case LOG_APP_PERFORMANCE:
			sprintf (backupName, "%s_%s_%s.%s", m_StrLogName.c_str(), STR_LOG_APP_PERFORMANCE, szCurrTime, FILE_EXTEN_NAME);
			break;
		default:
			break;
	}
	
	//sprintf (backupFile, "%s/%s", sourPath, m_File->GetFileName());
	sprintf (backupFile, "%s/%s", backupPath, m_File->GetFileName());
	
	m_MutexLocker->Lock();
	
	GDF_FILE *pTemp = NULL;
	if (m_Filetype == 0)
	{
#ifdef _SDFS_LOG_
		pTemp = new SDFS_GDF_FILE (sourPath, backupName);
#endif
		if (0 != pTemp->Open ("a+"))
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
			return -1;
		}
	}
	else
	{
		pTemp = new SYSTEM_GDF_FILE (sourPath, backupName);
		if (0 != pTemp->Open ("a+"))
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
			return -1;
		}
	}
	
	GDF_FILE *pSourFile = m_File;
	m_File = pTemp;

	if(NULL != pSourFile)
	{
		pSourFile->Close ();
		pSourFile->Rename (backupFile);
		pSourFile->Remove ();
	}
	
	m_MutexLocker->UnLock();
	
	if (NULL != pSourFile)
	{
		delete pSourFile;
		pSourFile = NULL;
	}
	
	return 0;
}

bool ilog_file::IsBackup (void)
{
	switch (m_BackUpType)
	{
		case LOG_BACKUP_DATE:
		{
			char tCurTime[14+1] = {0};
			getCurDateTime (tCurTime);
			
			if (strncmp(tCurTime, m_CreateTime, 8) != 0)
			{
				strcpy (m_CreateTime, tCurTime);
				
				return true;
			}
			
			break;
		}
		case LOG_BACKUP_SIZE:
		{
			struct stat buf;
	
			stat (m_File->GetFullFileName(), &buf);
			
			if (buf.st_size > (m_BackupSize/1024/1024))
			{
				return true;
			}
			
			break;
		}
		case LOG_BACKUP_INTERVAL_TIME:
		{
			time_t tmpT = time(NULL);
			if (tmpT/m_IntervalTime != m_OTime/m_IntervalTime)
			{
				m_OTime = time (NULL);
				
				return true;
			}
			
			break;
		}
		case LOG_BACKUP_HOUR:
		{
			char tCurTime[14+1] = {0};
			getCurDateTime (tCurTime);
			
			if (strncmp(tCurTime, m_CreateTime, 10) != 0)
			{
				strcpy (m_CreateTime, tCurTime);
				
				return true;
			}
			
			break;
		}
		default:
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[m_BackUpType<%d> Is Failed.]\n", __FILE__, __LINE__, m_BackUpType);
			
			break;
		}
	}
	
	return false;
}
