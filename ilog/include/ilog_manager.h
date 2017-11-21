/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
 
#ifndef _CILOG_MANAGER_H_
#define _CILOG_MANAGER_H_

#include <iostream>
#include <map>
#include "ilog_server.h"

struct ilog_cfgfile
{
	int log_type;		//日志类型(#1000:通用日志,1001:运行日志,1002:告警日志,1003:错误日志,1004:统计日志,1009:性能日志)
	int log_interval;	//时间间隔，单位秒
	int log_backup_size;//备份文件大小，单位M
	int log_procmode;	//#日志处理模式(1:同步方式,2:异步方式)
	int log_backup_type;//日志备份类型（1:日期,2:文件大小,3:时间间隔,4:每小时）
};

struct ilog_cfgserver
{
	char log_path[512+1];
	char log_name[32+1];
	int log_file_num;
	int log_thread_num;
	int file_type;//0,sdfs 1,普通文件
	struct ilog_cfgfile logfile_infos[6];
};

struct ilog_performance
{
	long begin_time;//首次时间
	long last_time;//上次时间
	long end_time;//末次时间
};

struct ilog_statinfo
{
	int num;//总数量
	int all_time;//总耗时
	int max_time;//最大耗时
	int min_time;//最小耗时
	int average_time;//平均耗时
};

class ilog_manager : public Thread
{
	
public:
	ilog_manager (void);
	
	~ilog_manager(void);
	
	ilog_cfgserver *getCfgserver();
	
	//设置时间戳，计算性能开始（多线程版本）
	void ilog_Begin(UnInt32 threadNum);
	
	//设置时间戳，计算性能点（多线程版本）
	void ilog_Mark(const char *name,UnInt32 threadNum);
	
	//设置时间戳，计算性能结束（多线程版本）
	void ilog_End(UnInt32 threadNum);
	
	//写日志（多线程版本）
	void WriteLog (string log_name,UnInt32 nLogType, UnInt32 threadNum, char *szDescription);
	
	bool IsRun ();
	
	bool Close (void);
	
	bool Init (ilog_cfgserver *pCfgInfo);
	
	bool Init (ilog_cfgserver *pCfgInfo,char *processNum);
	
	void run (void);

private:
	ilog_file *GetLogFile (const string m_StrLogName, UnInt32 nLogType);
	
	string GetLogName (const string m_StrLogName);
	
	string GetLogName (const string m_StrLogName,UnInt32 threadNum);
	
	char * getTimeDiff();
	
	void setTimeDiff(char *timeDiff);
	
	void clearTimeDiff();

	void SetPerformance (int threadNum,ilog_performance *performance);
	
	ilog_performance * GetPerformance (int threadNum);
	
	void ClearPerformance (int threadNum);
	
	ilog_statinfo * getStatinfo(int threadNum);
	
	void setStatinfo(int threadNum,ilog_statinfo *statinfo);
	
	void clearStatinfo(int threadNum);

	bool InitLogServer(ilog_cfgserver *pCfgInfo);
	
	int Backup (void);
	
	int RegisterLog (string m_StrLogName, ilog_server *pServer);
	
	int UnRegisterLog (string m_StrLogName, ilog_server *pServer);

	typedef map<string, ilog_server *> mapLogServer;
	
	mapLogServer m_LogServer;
	
	ilog_cfgserver *m_CfgInfo;
		
	char m_processNum[10];
	
	bool m_IsRun;
	
	char m_timeDiff[100];
	
	map<int,ilog_performance*> m_Performances;
	
	map<int,ilog_statinfo*> m_Statinfos;
};

#endif
