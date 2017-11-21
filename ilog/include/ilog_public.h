//
//  ilog_public.h
//  ibf
//
//  Created by wangzhia on 2017/5/18.
//  Copyright 2017 wangzhia. All rights reserved.
//

#ifndef _ilog_public_h
#define _ilog_public_h

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

extern void getCurDateTime (char *sDateTime);
extern void getSysDatetoStr (char *sysDate);
extern long getUTime();
#endif /* ilog_public_h */
