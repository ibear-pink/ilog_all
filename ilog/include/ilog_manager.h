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
	int log_type;		//��־����(#1000:ͨ����־,1001:������־,1002:�澯��־,1003:������־,1004:ͳ����־,1009:������־)
	int log_interval;	//ʱ��������λ��
	int log_backup_size;//�����ļ���С����λM
	int log_procmode;	//#��־����ģʽ(1:ͬ����ʽ,2:�첽��ʽ)
	int log_backup_type;//��־�������ͣ�1:����,2:�ļ���С,3:ʱ����,4:ÿСʱ��
};

struct ilog_cfgserver
{
	char log_path[512+1];
	char log_name[32+1];
	int log_file_num;
	int log_thread_num;
	int file_type;//0,sdfs 1,��ͨ�ļ�
	struct ilog_cfgfile logfile_infos[6];
};

struct ilog_performance
{
	long begin_time;//�״�ʱ��
	long last_time;//�ϴ�ʱ��
	long end_time;//ĩ��ʱ��
};

struct ilog_statinfo
{
	int num;//������
	int all_time;//�ܺ�ʱ
	int max_time;//����ʱ
	int min_time;//��С��ʱ
	int average_time;//ƽ����ʱ
};

class ilog_manager : public Thread
{
	
public:
	ilog_manager (void);
	
	~ilog_manager(void);
	
	ilog_cfgserver *getCfgserver();
	
	//����ʱ������������ܿ�ʼ�����̰߳汾��
	void ilog_Begin(UnInt32 threadNum);
	
	//����ʱ������������ܵ㣨���̰߳汾��
	void ilog_Mark(const char *name,UnInt32 threadNum);
	
	//����ʱ������������ܽ��������̰߳汾��
	void ilog_End(UnInt32 threadNum);
	
	//д��־�����̰߳汾��
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
