//
//  ilog.h
//  ibf
//
//  Created by wangzhia on 2017/5/25.
//  Copyright 2017 wangzhia. All rights reserved.
//

#ifndef _ILOG_H_
#define _ILOG_H_

#include <stdio.h>
#include "ilog_manager.h"
#include "ilog_Singleton.h"

using namespace std;

class ilog
{
public:
	ilog(void);
	~ilog(void);
	
	//��־��ʼ���������̣�
	bool ilog_Init (ilog_cfgserver *pCfgInfo);
	
	//��־��ʼ��������̣�
	bool ilog_Init (ilog_cfgserver *pCfgInfo,char *processNum);
	bool ilog_Init (ilog_cfgserver *pCfgInfo,int processNum);
	
	//��־��������
	void ilog_Run ();
	
	//��־���̽���
	bool ilog_Close (void);
	
	ilog_manager* GetLogManager(string log_name);

private:
	map<string,ilog_manager*> m_LogManagers;

};

typedef GDF_Singleton<ilog> App_iLog;

//�ж���־�����Ƿ�����
extern bool ilog_IsRun (string log_name);
//����ʱ������������ܿ�ʼ
extern void ilog_Begin(string log_name);
//����ʱ������������ܵ�
extern void ilog_Mark(string log_name);
//����ʱ������������ܵ�
extern void ilog_Mark(string log_name,const char *name);
//����ʱ������������ܽ���
extern void ilog_End(string log_name);
//д��־
extern void WriteLog (string log_name,UnInt32 nLogType, const char *fmt, ...);

//����ʱ������������ܿ�ʼ�����̰߳汾��
extern void ilog_Begin(string log_name,UnInt32 threadNum);
//����ʱ������������ܵ㣨���̰߳汾��
extern void ilog_Mark(string log_name,UnInt32 threadNum);
//����ʱ������������ܵ㣨���̰߳汾��
extern void ilog_Mark(string log_name,const char *name,UnInt32 threadNum);
//����ʱ������������ܽ��������̰߳汾��
extern void ilog_End(string log_name,UnInt32 threadNum);
//д��־�����̰߳汾��
extern void WriteLog (string log_name,UnInt32 nLogType, UnInt32 threadNum, const char *fmt, ...);

#endif /* ilog_h */
