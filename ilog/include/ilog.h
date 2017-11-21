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
	
	//日志初始化（单进程）
	bool ilog_Init (ilog_cfgserver *pCfgInfo);
	
	//日志初始化（多进程）
	bool ilog_Init (ilog_cfgserver *pCfgInfo,char *processNum);
	bool ilog_Init (ilog_cfgserver *pCfgInfo,int processNum);
	
	//日志进程启动
	void ilog_Run ();
	
	//日志进程结束
	bool ilog_Close (void);
	
	ilog_manager* GetLogManager(string log_name);

private:
	map<string,ilog_manager*> m_LogManagers;

};

typedef GDF_Singleton<ilog> App_iLog;

//判断日志进程是否运行
extern bool ilog_IsRun (string log_name);
//设置时间戳，计算性能开始
extern void ilog_Begin(string log_name);
//设置时间戳，计算性能点
extern void ilog_Mark(string log_name);
//设置时间戳，计算性能点
extern void ilog_Mark(string log_name,const char *name);
//设置时间戳，计算性能结束
extern void ilog_End(string log_name);
//写日志
extern void WriteLog (string log_name,UnInt32 nLogType, const char *fmt, ...);

//设置时间戳，计算性能开始（多线程版本）
extern void ilog_Begin(string log_name,UnInt32 threadNum);
//设置时间戳，计算性能点（多线程版本）
extern void ilog_Mark(string log_name,UnInt32 threadNum);
//设置时间戳，计算性能点（多线程版本）
extern void ilog_Mark(string log_name,const char *name,UnInt32 threadNum);
//设置时间戳，计算性能结束（多线程版本）
extern void ilog_End(string log_name,UnInt32 threadNum);
//写日志（多线程版本）
extern void WriteLog (string log_name,UnInt32 nLogType, UnInt32 threadNum, const char *fmt, ...);

#endif /* ilog_h */
