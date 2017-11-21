/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
 
#include "ilog_server.h"

ilog_server::ilog_server (void)
{
	m_nCount = 0;
	m_LogPath = "";
	m_StrLogName = "";
	m_FileType = 0;
}

ilog_server::~ilog_server (void)
{
	if (m_MapLogFile.size() != 0)
	{
		this->Close();
	}
}

void ilog_server::SetLogInfo (const char *sLogPath, const char *sLogName)
{
	m_LogPath = string (sLogPath);
	m_StrLogName = string (sLogName);
}

int ilog_server::Backup ()
{
	for (mapLogFile::iterator iter = m_MapLogFile.begin(); iter != m_MapLogFile.end(); iter++)
	{
		ilog_file *pNode = (iter->second);
		if (pNode->getLogType() != LOG_APP_STATINFO)
			pNode->BackupLog();
	}
	
	return 0;
}

int ilog_server::getFileType()
{
	return m_FileType;
}

void ilog_server::setFileType(int file_type)
{
	m_FileType = file_type;
}

int ilog_server::AddLogFile (int nLogType, int nInterval, int nBackupSize, int nProcMode, int backupType)
{
	mapLogFile::iterator iter = m_MapLogFile.find(nLogType);
	
	if (iter != m_MapLogFile.end())
	{
		return 0;
	}
	
	ilog_file *pNode = new ilog_file ();
	if (getFileType() == 0)
	{
		pNode->setSDFSType();
	}
	else if (getFileType() == 1)
	{
		pNode->setSYSTEMType();
	}
	else
	{
		printf("文件系统类型标示错误，请检查%d (0,sdfs;1,普通文件--默认为0)\n",getFileType());
		return false;
	}
	
	if (0 != pNode->InitLogFile (m_LogPath, m_StrLogName, nLogType, nInterval, nBackupSize, nProcMode, backupType))
	{
		printf ("FILE[%s]LINE[%d]ERR_MSG[File Pointer Is NULL.]\n", __FILE__, __LINE__);
		return -1;
	}
	m_MapLogFile.insert (pair<int, ilog_file *>(nLogType, pNode));
	m_nCount++;
	
	return 0;
}

int ilog_server::DelLogFile (int nLogType)
{
	mapLogFile::iterator iter = m_MapLogFile.find(nLogType);
	
	if (iter != m_MapLogFile.end())
	{
		delete (iter->second);
		
		m_MapLogFile.erase(iter);
	}
	
	return 0;
}

void ilog_server::Close (void)
{
	for (mapLogFile::iterator iter = m_MapLogFile.begin(); iter != m_MapLogFile.end(); iter++)
	{
		delete (iter->second);
	}
	
	m_MapLogFile.clear();
}

ilog_file *ilog_server::GetLogFile (int nLogType)
{
	mapLogFile::iterator iter = m_MapLogFile.find(nLogType);
		
	if (iter != m_MapLogFile.end())
	{
		return (iter->second);
	}

	return NULL;
}
