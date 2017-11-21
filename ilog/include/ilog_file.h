/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
#ifndef _CILOG_FILE_H_
#define _CILOG_FILE_H_

#include <cstdarg>
#include <unistd.h>
#include <string>
#include <iostream>
#include <map>
#include "avltree.h"
#include "ifile.h"
#include "ilog_Thread.h"
#include "ilog_CMutexLock.h"
#include "ilog_public.h"
#include "ilog_define.h"
#include "ifile_system.h"
#ifdef _SDFS_LOG_
#include "ifile_sdfs.h"
#endif

typedef struct
{
	int size;
	char buff[MAX_LOG_LENGTH];
} stLogInfo;

using namespace std;

class ilog_file : public Thread
{
public:
	ilog_file (void);
	
	~ilog_file(void);
		
	void run (void);
	
	int InitLogFile (const string &sLogPath, const string &sLogName, UnInt32 nLogType, UnInt32 nInterval, UnInt32 nBackupSize, Int32 nProcMode, Int32 backupType);
	
	void DoLog (const char *sBuf);
	
	int DoLogAndBackUp (const char *sBuf);
	
	int BackupLog (void);
	
	bool IsBackup (void);
	
	UnInt32 getLogType();
	
	void setSDFSType();
	
	void setSYSTEMType();

private:
	void CreateNewLogName(char *backupName);
	
	string m_LogPath;
	
	string m_StrLogName;		//模块名字
	
	UnInt32 m_LogType;			//日志类型
	
	GDF_FILE *m_File;
	
	UnInt32 m_IntervalTime;
	
	UnInt32 m_BackupSize;
	
	Int32 m_ProcMode;
	
	UnInt32 m_BackUpType;
	
	time_t m_OTime;
	
	char m_CreateTime[14+1];
	
	ilog_CMutexLock *m_MutexLocker;
	
	deque2<stLogInfo> m_LogQueue;
	
	UnInt32 m_Filetype;//0,普通文件;1,sdfs
};

#endif
