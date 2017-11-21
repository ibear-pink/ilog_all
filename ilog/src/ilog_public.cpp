//
//  ilog_public.cpp
//  ibf
//
//  Created by wangzhia on 2017/5/18.
//  Copyright 2017 wangzhia. All rights reserved.
//

#include "ilog_public.h"

void getCurDateTime (char *sDateTime)
{
	struct tm tDateTime;
	time_t lSeconds;
	time( &lSeconds );
	localtime_r( &lSeconds, &tDateTime );
	
	sprintf (sDateTime, "%04d%02d%02d%02d%02d%02d", tDateTime.tm_year + 1900, tDateTime.tm_mon + 1, tDateTime.tm_mday,\
 			tDateTime.tm_hour, tDateTime.tm_min, tDateTime.tm_sec);
	
	sDateTime[14] = '\0';
}

void getSysDatetoStr (char *sysDate)
{
	time_t sec;
	struct tm t;
	
	sec = time(NULL);
	localtime_r(&sec,&t);
	sprintf (sysDate,"%04d-%02d-%02d %02d:%02d:%02d",\
			 t.tm_year+1900,t.tm_mon+1,t.tm_mday,\
			 t.tm_hour,t.tm_min,t.tm_sec);
	
	sysDate[20] = '\0';
}

long getUTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (((unsigned long)tv.tv_sec) * 1000*1000 + ((unsigned long)tv.tv_usec));
}
