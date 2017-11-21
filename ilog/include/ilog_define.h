//
//  ilog_define.hpp
//  ibf
//
//  Created by wangzhia on 2017/5/18.
//  Copyright 2017 wangzhia. All rights reserved.
//

#ifndef ilog_define_h
#define ilog_define_h

#include <stdio.h>

#define STR_LOG_SYS_NORMAL		"NORMAL"
#define STR_LOG_SYS_RUNNING		"RUNNING"
#define STR_LOG_SYS_WARNING		"WARNING"
#define STR_LOG_SYS_ERROR		"ERROR"
#define STR_LOG_APP_STATINFO	"STAT"
#define STR_LOG_APP_PERFORMANCE	"PERFORMANCE"

#define STR_PATH_RUNLOG			"runlog"
#define STR_PATH_TEMP			"temp"
#define FILE_EXTEN_NAME			"log"

#define MAX_DEQUE_NUM		10000
#define MAX_LOG_THREAD_NUM	100
#define MAX_LOG_LENGTH		204800

/*日志备份方式*/
enum
{
	LOG_BACKUP_DATE				= 1,	//日期方式备份
	LOG_BACKUP_SIZE				= 2,	//文件大小方式备份
	LOG_BACKUP_INTERVAL_TIME	= 3,	//时间间隔方式备份
	LOG_BACKUP_HOUR				= 4		//小时方式备份
};

enum
{
	WRITE_FILE_SIZE_TYPE			= 1,//文件大小
	WRITE_FILE_INTERVAL_TIME_TYPE	= 2,//时间间隔
	WRITE_FILE_END_FILE_TYPE		= 3,//入口文件处理完时备份
	WRITE_FILE_IMMEDIATE_TYPE		= 4//即时备份
};

///////////////
//服务端处理模式
enum
{
	SERVER_SYNC_MODE			= 1,//同步
	SERVER_ASYNC_MODE			= 2//异步
};

/*日志类型声明*/
enum
{
	LOG_SYS_NORMAL					= 1000,			//通用日志
	LOG_SYS_RUNNING					= 1001,			//运行日志
	LOG_SYS_WARNING					= 1002,			//告警日志
	LOG_SYS_ERROR					= 1003,			//错误日志
	LOG_APP_STATINFO				= 1004,			//统计日志
	LOG_APP_PERFORMANCE				= 1009			//性能分析
};


#endif /* ilog_define_hpp */
