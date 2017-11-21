/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
#ifndef _CILOG_SERVER_H_
#define _CILOG_SERVER_H_

#include <cstdarg>
#include <unistd.h>
#include <string>
#include <iostream>
#include <map>
#include "ilog_file.h"

using namespace std;

class ilog_server
{
public:
	ilog_server (void);
	
	~ilog_server (void);
	
	void SetLogInfo (const char *sLogPath, const char *sLogName);
	
	int Backup ();
	
	void Close (void);
	
	int AddLogFile (int nLogType, int nInterval, int nBackupSize, int nProcMode, int backupType);
	
	int DelLogFile (int nLogType);
	
	ilog_file *GetLogFile (int nLogType);
	
	int getFileType();
	
	void setFileType(int file_type);
	
private:
	typedef map<int, ilog_file *> mapLogFile;
	
	mapLogFile m_MapLogFile;
	
	int m_nCount;
	
	string m_LogPath;
	
	string m_StrLogName;		//模块名字
	
	int m_FileType;//文件系统类型 1:sdfs 0:本地
};

#endif
