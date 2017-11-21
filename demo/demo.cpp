/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */

#include <string.h>
#include "ilog.h"
int g_runflag;//0: run  1：stop

/**********************************************************
 Function:		static void sig_quit(int signo)
 Description:	信号处理函数
 Input:			int signo, 信号编号
 Output:			无
 Return: 		无
 Others:			产生跳转
 **********************************************************/
static void sig_quit(int signo)
{
	g_runflag = 1;
}

int main(int argc, const char * argv[])
{
	ilog_cfgserver *log1 = new ilog_cfgserver();
	//strcpy(log1->log_path,".");
	strcpy(log1->log_path,"/sdfslog/CLUSTER_Z00/NRC/log");
	strcpy(log1->log_name,"demo_sdfs");
	log1->log_file_num = 4;
	log1->file_type = 0;
	log1->log_thread_num = 1;
	log1->logfile_infos[0].log_type = 1004;
	log1->logfile_infos[0].log_procmode = 2;
	log1->logfile_infos[0].log_backup_type = 3;
	log1->logfile_infos[0].log_interval = 10;
	
	log1->logfile_infos[1].log_type = 1009;
	log1->logfile_infos[1].log_procmode = 2;
	log1->logfile_infos[1].log_backup_type = 1;
	//log1->logfile_infos[1].log_interval = 10;
	
	log1->logfile_infos[2].log_type = 1003;
	log1->logfile_infos[2].log_procmode = 2;
	log1->logfile_infos[2].log_backup_type = 1;
	//log1->logfile_infos[2].log_interval = 10;
	
	log1->logfile_infos[3].log_type = 1001;
	log1->logfile_infos[3].log_procmode = 2;
	log1->logfile_infos[3].log_backup_type = 3;
	log1->logfile_infos[3].log_interval = 3600;
	/*log1->log_file_num = 1;
	log1->log_thread_num = 3;
	log1->logfile_infos[0].log_type = 1009;
	log1->logfile_infos[0].log_procmode = 2;
	log1->logfile_infos[0].log_backup_type = 1;
	log1->logfile_infos[0].log_interval = 10;*/
	string log_name;
	log_name = "demo_sdfs_ax";
	if (!App_iLog::Instance()->ilog_Init (log1,"ax"))
	{
		printf ("App_LogManager Init Is Failed.\n");
		return false;
	}
	//日志管理中心启动
	App_iLog::Instance()->ilog_Run ();
	bool flag = true;
	int i = 0;
	
	signal(SIGTERM, sig_quit);
	signal(SIGINT, sig_quit);
	signal(SIGQUIT, sig_quit);
	signal(SIGPIPE, SIG_IGN);
	
	while (g_runflag == 0)
	{
		ilog_Begin(log_name);
		ilog_Mark(log_name);
		WriteLog(log_name,LOG_SYS_ERROR,2,"的阿斯顿发送到:%d\n",i);
		ilog_Mark(log_name);
		i++;
		WriteLog(log_name,LOG_SYS_RUNNING,"gogogog:%d\n",i);
		ilog_End(log_name);
		WriteLog(log_name,LOG_APP_PERFORMANCE,"");
		sleep(1);
	}

	App_iLog::Instance()->ilog_Close();
	App_iLog::Close();
	delete log1;
	log1 = NULL;
	return 0;
}
