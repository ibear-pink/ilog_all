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

/*��־���ݷ�ʽ*/
enum
{
	LOG_BACKUP_DATE				= 1,	//���ڷ�ʽ����
	LOG_BACKUP_SIZE				= 2,	//�ļ���С��ʽ����
	LOG_BACKUP_INTERVAL_TIME	= 3,	//ʱ������ʽ����
	LOG_BACKUP_HOUR				= 4		//Сʱ��ʽ����
};

enum
{
	WRITE_FILE_SIZE_TYPE			= 1,//�ļ���С
	WRITE_FILE_INTERVAL_TIME_TYPE	= 2,//ʱ����
	WRITE_FILE_END_FILE_TYPE		= 3,//����ļ�������ʱ����
	WRITE_FILE_IMMEDIATE_TYPE		= 4//��ʱ����
};

///////////////
//����˴���ģʽ
enum
{
	SERVER_SYNC_MODE			= 1,//ͬ��
	SERVER_ASYNC_MODE			= 2//�첽
};

/*��־��������*/
enum
{
	LOG_SYS_NORMAL					= 1000,			//ͨ����־
	LOG_SYS_RUNNING					= 1001,			//������־
	LOG_SYS_WARNING					= 1002,			//�澯��־
	LOG_SYS_ERROR					= 1003,			//������־
	LOG_APP_STATINFO				= 1004,			//ͳ����־
	LOG_APP_PERFORMANCE				= 1009			//���ܷ���
};


#endif /* ilog_define_hpp */
