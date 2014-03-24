//we must note that just /tmp/ /usr/local/etc are readwritable
//so we store file like xmlfile dest_file must store in /tmp/ or /usr/local/etc

	
//when the system start should do the following
//1 check the status 
//1.1 load status
//1.2 load setting
//2 check the booklist
//2.1 load booklist
//2.2 load ...

#include <stdio.h>
#include <pthread.h>
#include "server.h"

string gstrErrorLogDir;
string gstrErrorLogFile;
pthread_t pid_Status_timer;
char BNBT_SERVER_IP[16];
char BNBT_SERVER_PORT[5];


int main(int argc, char **argv)
{		
	
	cout <<"version 9.53" << endl;
	if(InitSys(argc, argv)<0)
		return -1;
			
	int i,ret;
#ifdef THREAD_STATUS
        ret = pthread_create(&pid_Status_timer,NULL,thread_timer,NULL);//定时器的线程
        if(ret!=0)
        {
                printf("Create pthread thread_timer error!\n");
                exit(-1);
        }        	
#endif

#ifdef WATCH_TRAFFIC
	ret = pthread_create(&TimerFunc::pid_traffic_watch , NULL , TimerFunc::thread_Net_Traffic_Watch , NULL);//网卡检测的进程
        if(ret!=0)
        {
                printf("Create pthread thread_Net_Traffic_Watch error!\n");
                exit(-1);
        }  
#endif        
	
	//system("pidof serverM > /tmp/serverM_pids");
	int server_fifo_fd;
 	int client_fifo_fd;

	int res;
	char client_fifo_name[NAME_SIZE];

	MessageClient MC1;

	//if(UTIL_CheckFile(SERVER_FIFO_NAME))//this will fopen r and will make system block
	{
		UTIL_DeleteFile(SERVER_FIFO_NAME);//do we need to delete it ervery time ? why not just open it
		cout <<"Delete "<< SERVER_FIFO_NAME << endl;
	
	}

	if (mkfifo(SERVER_FIFO_NAME, 0777) == -1)
	{
		fprintf(stderr, "Sorry, create server fifo failure!\n");
		exit(EXIT_FAILURE);
	}
	
	printf(" created server fifo !\n");
	
	server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY/*|O_NONBLOCK*/ );
	if (server_fifo_fd == -1)
	{
		fprintf(stderr, "Sorry, server fifo open failure!\n");
		exit(EXIT_FAILURE);
	}	
    
	int count =0;
	
	while (1)
	{				
		//1 read client_pid and OT
		//2 read the specify type size of message
	
		//BUG 1: Always res = 8 ;
		//FIXME 1 : adding && MC1.client_pid >0 && MC1.OT< OpTypeNum ,to  prevent the repeat read
		//First read Operation Type           
		
		//if((res = read(server_fifo_fd, &MC1, sizeof(MessageClient))) > 0 && MC1.client_pid >0 && MC1.OT< OT_OpTypeNum )
		
RESET:
		res = read(server_fifo_fd, &MC1, sizeof(MessageClient) );
//		printf("\n\t@@@ after  read res = %d\n", res);
		if( res > 0 )
		{							
			ResultInfo RI;
			RI.result = 0;
			
			RI.MC.client_pid = MC1.client_pid;
			CourseInfo CI,*pCI;
			pCI = &CI; 
			LimitInfo LI;
			int OT = MC1.OT;
			JobInfoC JIC1;	
			bool bIsSoftWareUpdate = false;
						
			printf("[%s:%d]:%s()- #### -Receive a request, OT = %d !!\n",__FILE__,__LINE__,__func__, OT);
			
			TaskControl *pTC = new TaskControl();			
			
			switch(OT)//类型
			{								
				case OT_Add:
				{	
					/*
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"Receive OT_Add");
					int iOPRet = pTC->Task_Add(CI.Hash);
									
					if (iOPRet>=0)
						RI.result = 0;
					else
						RI.result = -1;				
					*/
				}
				break;
					
				case OT_Book://预约
				{
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"Receive OT_Book");
					if((res = read( server_fifo_fd, &CI, sizeof(CI))) > 0)
					{
										
						printf("\t@@@\tHASH : %s",CI.Hash);
						int iOPRet   = -1;
						//如果是软件升级包，判断版本看是否需要进行unbook
						if(KeepSingleSoftUpdatePackage(CI) >= 0)
						{
							iOPRet = pTC->Task_Book(CI);						
							if (iOPRet>=0)
								RI.result = 0;
							else
								RI.result = -1;
						}
						cout << "Got a book  :" << "Result " << iOPRet << " " << CI.Hash <<" " << MC1.client_pid << endl;						
					}
					bIsSoftWareUpdate = bIsSoftPackageMessage(NULL, &CI);
				}
				break;
					
				case OT_UnBook://取消预约
				case OT_UnBook_But_NO_Del://取消预约但不删除对应的资源
				{
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"Receive OT_UnBook");
					if((res = read( server_fifo_fd, &CI, sizeof(CI))) > 0)
					{						
						int iOPRet;
						if(OT_UnBook == OT)
						{
							iOPRet = pTC->Task_UnBook(CI);
						}
						else if(OT_UnBook_But_NO_Del == OT)
						{
							iOPRet = pTC->Task_UnBook(CI,false);
						}	
							
						if (iOPRet>=0)
							RI.result = 0;
						else
							RI.result = -1;
										
						cout << "Got a UnBook  :" << CI.Hash <<" " << MC1.client_pid << endl;						
					}
					bIsSoftWareUpdate = bIsSoftPackageMessage(NULL, &CI);
				}
				break;
					
				case OT_LimitSpeed://限制网速与回复网速都是这个信息，限制网速：提供给限制的数值。 回复网速： 给 0 ， 0 
				{
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"Receive OT_LimitSpeed");
					if((res = read(server_fifo_fd, &LI, sizeof(LI))) > 0)
					{
						//do the Limit thing				
						BTSetting::LimitDownUploadSpeed(LI.DownloadSpeedLimitTo,LI.UploadSpeedLimitTo);						
						cout << "LI.DownloadSpeedLimitTo :" << LI.DownloadSpeedLimitTo << endl;
						RI.result = 0;
					}
					
				}
				break;

				case OT_Status://咨询
				{					
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"Receive OT_Status");
					
					
					if((res = read( server_fifo_fd, &JIC1, sizeof(JobInfoC))) > 0)
					{
						printf("###@@receive from message JIC1.ID: %s \tJIC1.Name: %s \n", JIC1.ID, JIC1.Name);
						
						char result[200] = {0};
						JobInfo JI,*pJI;												
						
						//no such JobInfoC <0 
						//otherwise JobInfoC will be effected
						if(Status::AskForStatus(JIC1)<0)
						{
							//no such JobInfoC <0  then make downloaded -1;
							JIC1.Downloaded = -1;//没有此任务
							strcpy( JIC1.Percent , "0%" );							
						}
						printf("###@@after AskForStatus JIC1.ID   : %s \t JIC1.Percent :%s\t JIC1.Name : %s \t JIC1.Downloaded = %f \n", JIC1.ID , JIC1.Percent , JIC1.Name, JIC1.Downloaded );												
					}
					bIsSoftWareUpdate = bIsSoftPackageMessage(&JIC1, NULL);
				}
				break;
				case OT_FeedBack://反馈升级、紧急下载需求
				{
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"Receive OT_FeedBack of Download Compelete");
					Message_FeedBack MF;
					if((res = read( server_fifo_fd, &MF, sizeof(Message_FeedBack))) > 0)
					{
						Event_Current_Course_Very_Limited(MF.FB.CurreentLevel, MF.FB.UnLearnCourseCount, MF.FB.SMode);
					}
				}
				default:
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"Receive OT_FeedBack of Download Compelete");
					break;
			}
			delete pTC;
			pTC = NULL;
			
			
			sprintf(client_fifo_name, CLIENT_FIFO_NAME, MC1.client_pid);
			
			 
			if(bIsSoftWareUpdate)//判断是否软件升级包的消息
			{
				client_fifo_fd = open(SOFT_UPDATE_FIFO_NAME, O_WRONLY);//软件升级包的专属fifo
			}
			else
			{
				client_fifo_fd = open(CLIENT_ALL_FIFO_NAME, O_WRONLY);//2011-8-17 所有学习模式的统一fifo
			}
			 
			if (client_fifo_fd == -1)
			{
				fprintf(stderr, "Sorry, client fifo open failure! in server\n");//BUG 1::always go to this  
				fflush(stdout);
				//exit(EXIT_FAILURE);
				goto RESET;
			}	
			
			//write the return client_fifo_fd according to the OpType									
			switch(OT)
			{
				case OT_Add:
					
				case OT_Book:
				case OT_UnBook:
				case OT_UnBook_But_NO_Del://取消预约但不删除对应的资源
					
				
				case OT_LimitSpeed:
					write(client_fifo_fd, &RI, sizeof(ResultInfo));
					break;
				
				case OT_Status:
					{
						Message_Status MS;
						MS.JIC =JIC1;
						MS.MC.client_pid=MC1.client_pid;
						MS.MC.OT = OT_Status;
						write(client_fifo_fd, &MS, sizeof(Message_Status));
						
						cout << "server write client_fifo_fd,  MS /tmp/client_fifo" << endl;
					}
					break;
				
				default:
				break;
			}	
			close(client_fifo_fd);
			
			//FIXME::need to  close(server_fifo_fd) and REopen(SERVER_FIFO_NAME, O_RDONLY) ?
		}
		else//keep try open
		{
			sleep(1);//need to sleep for a few second for it will make 100%cpu
#ifdef WATCH_DOG
			CommunicateToWatchdog('s');
#endif
			printf( "\tsleeping res = %d \t",res);
		}
	}
	
	
	close(server_fifo_fd);
	unlink(SERVER_FIFO_NAME); 
#ifdef THREAD_STATUS
	pthread_join(pid_Status_timer,NULL);    
#endif
	exit(EXIT_SUCCESS);
        
}

int InitSys(int argc, char **argv)
{
	if(argc<=2)
	{
		printf("Usage: serverM BNBT_SERVER_IP BNBT_SERVER_PORT\n");
		return -1;
	}
	else
	{
		strcpy(BNBT_SERVER_IP,argv[1]);
		strcpy(BNBT_SERVER_PORT,argv[2]);		  
	}
	
	gstrErrorLogDir="/tmp/hdd/volumes/HDD1/";
	gstrErrorLogFile="log.log";

	//other some filepath 

	//各信号处理函数
	signal(SIGINT, EndSys);
	signal(SIGQUIT, EndSys);
	signal(SIGKILL, EndSys);
	signal(SIGTERM, EndSys);
    
}


void* thread_timer(void*)
{
	printf("This is a thread_timer\n");
		
	Status::GetFinisthJob(FINISH_JOBS_RECORD_FILE);	
	// Status::m_vJIC_Finish = *(Status::GetFinisthJob(FINISH_JOBS_RECORD_FILE));
 	Status::PrintContents(Status::m_vJIC_Finish);
		
	Status::ReMatch();			
	Status::PrintContents(*(Status::GetCurrentJobInfo_CourseInfo()));
					
	pthread_mutex_init(&Status::mtx_status, NULL);
	
        TimerFunc::StartTimer();//repeat check for status should be a thread 
}

void EndSys(int sign_no)
{
	if(sign_no == SIGINT )
	{
		printf("Got SIGINT\n");
	}
	else if(sign_no == SIGQUIT )
	{
		printf("Got SIGQUIT\n");
	}
	else if(sign_no == SIGKILL )
	{
		printf("Got SIGKILL\n");
	}
	else if(sign_no == SIGTERM )
	{
		printf("Got SIGTERM\n");
	}
#ifdef THREAD_STATUS	
	pthread_cancel(pid_Status_timer);
	
#ifdef WATCH_TRAFFIC	
	pthread_cancel(TimerFunc::pid_traffic_watch);
	printf("pthread_cancel(%ld) , (%ld)\n",TimerFunc::pid_traffic_watch );		
#endif
	
	printf("pthread_cancel(%ld)\n",pid_Status_timer);		
#endif	
	
	pthread_mutex_destroy(&Status::mtx_status);
	exit(1); 
}


bool bIsSoftPackageMessage(JobInfoC *pJIC,CourseInfo *pCI )
{
	if(pJIC!=NULL)
	{
		if(strlen(pJIC->ID)==40)
		{
			printf("@@@ got a message of SoftPackageMessage Status :%s\t%s\n" , pJIC->ID,pJIC->Name);
			return true;
		}
		else
		{
			printf("### NOT a message of SoftPackageMessage Status :%s\t%s\n" , pJIC->ID,pJIC->Name);
			return false;
		}
	}
	else if(pCI!=NULL)		
	{
		if(strlen(pCI->ID)==40)
		{
			printf("@@@ got a message of SoftPackageMessage BOOK/UNBOOK : ID: %s\t Hash: %s\t Name: %s\t SendbySM: %d\n" , pCI->ID, pCI->Hash,pCI->Name,pCI->SendbySM);
			return true;
		}
		else
		{
			printf("@@@ NOT a message of SoftPackageMessage BOOK/UNBOOK : ID: %s\t Hash: %s\t Name: %s\t SendbySM: %d\n" , pCI->ID, pCI->Hash,pCI->Name,pCI->SendbySM);
			return false;
		}
	}
	return false;
}


int GetDownloadingSoftUpdatePackage(JobInfo_CourseInfo &JICI)
{
	vector<JobInfo_CourseInfo> *pvjici = Status::GetCurrentJobInfo_CourseInfo();
	for(vector<JobInfo_CourseInfo>::iterator iter =  pvjici->begin(); iter != pvjici->end(); iter++ )
	{		
		if(strlen(iter->CI.ID) == 40 )//已经有软件升级包在下载
		{
			memcpy(&(iter->CI), &(JICI.CI) , sizeof(CourseInfo) );
			memcpy(&(iter->JI), &(JICI.JI) , sizeof(JobInfo) );
			return 0;
		}
	}	
	return -1;
}

bool IsNewInSoftUpdatePackageNewer(const CourseInfo& CI_IN_BT_LIST ,const CourseInfo& CI)
{
	if(strcmp(CI_IN_BT_LIST.Name , CI.Name) < 0   )//新传入的软件升级包比现在下载中的软件升级包要高	
		return false;	
	else
		return true;
}

bool HaveFinshedJobInDisk(const char* pJobName, bool isFile)
{
	for(vector<JobInfoC>::iterator iter =  (Status::m_vJIC_Finish).begin(); iter != (Status::m_vJIC_Finish).end(); iter++ )
	{
		if(strcmp( pJobName ,iter->Name ) == 0)//名字与已完成的列表中的任务一致
		{
			char updata_package_path[200] = {0};
			sprintf(updata_package_path, "%s%s" , BT_SAVE_PATH ,pJobName );
			
			if(isFile && UTIL_CheckFile(updata_package_path))	//磁盘上有此文件 注意是文件
				return true;
			else if(!isFile && UTIL_CheckDir(updata_package_path))	//磁盘上有此文件夹 注意是文件夹
				return true;			
		}
	}
	return false;
}

int KeepSingleSoftUpdatePackage(const CourseInfo& CI_Comein)
{
	JobInfo_CourseInfo old_JICI;
	if(GetDownloadingSoftUpdatePackage( old_JICI ) < 0 )//没有下载中的升级包
	{
		return 0;//代表下载中的升级包
	}
	else if( IsNewInSoftUpdatePackageNewer( old_JICI.CI , CI_Comein) )	//新传入的软件升级包比现在下载中的软件升级包要高	
	{
		//取消当前下载中的软件升级包任务
		printf("previous SOft_Update_Package is older than now ,now unbook it\n");
		TaskControl *pTC = new TaskControl();				
		pTC->Task_UnBook(old_JICI.CI);
		delete pTC;
		pTC = NULL;
			
		if(HaveFinshedJobInDisk(old_JICI.CI.Name, true))
		{
			char updata_package_path[200] = {0};			
			sprintf(updata_package_path, "%s%s" , BT_SAVE_PATH, old_JICI.CI.Name );
			unlink(updata_package_path);
		}		
		//BOOK 
		
		return 0;
	}
	return -1;//代表有升级包 , 但 新传入的软件升级包比现在下载中的软件升级包要  低  没有必要BOOK			
}

/*
void SingleSoftUpdatePackage(const CourseInfo &CI)
{
	vector<JobInfo_CourseInfo> *pvjici = Status::GetCurrentJobInfo_CourseInfo();
	for(vector<JobInfo_CourseInfo>::iterator iter =  pvjici->begin(); iter != pvjici->end(); iter++ )
	{		
		if(strlen(iter->CI.ID) == 40 )//已经有软件升级包在下载
		{
			if(strcmp(iter->CI.Name , CI.Name) < 0   )//新传入的软件升级包比现在下载中的软件升级包要高
			{
				//取消当前下载中的软件升级包任务
				printf("previous SOft_Update_Package is older than now ,now unbook it\n");
				TaskControl *pTC = new TaskControl();				
				pTC->Task_UnBook(iter->CI);
				delete pTC;
				pTC = NULL;
				break;
			}
		}
	}
	
	//查找之前是否有升级包在硬盘	
	for(vector<JobInfoC>::iterator iter =  (Status::m_vJIC_Finish).begin(); iter != (Status::m_vJIC_Finish).end(); iter++ )
	{
		char updata_package_path[200] = {0};
		//sprintf(updata_package_path, "/tmp/hdd/volumes/HDD1/BT/%s" ,iter->Name );//BUG
		sprintf(updata_package_path, "%s%s" , BT_SAVE_PATH ,iter->Name );
		if(UTIL_CheckFile(updata_package_path))
		{
			printf("delete updata_package_path : %s\n",updata_package_path);
			unlink(updata_package_path);
			break;
		}
	}
	
	return ;
}
*/
#ifdef WATCH_DOG
void CommunicateToWatchdog(char feedchar )
{
	int watchdog_fifo_fd = open(WATCH_FIFO, O_WRONLY);
			
	if (watchdog_fifo_fd == -1)
	{
		//fprintf(stderr, "Sorry, WATCH_FIFO : %s   open failure! \n" ,WATCH_FIFO);//BUG 1::always go to this  
		return ;
	}	
	char cwrite[2] = {0};
	cwrite[0]=feedchar;
	cwrite[1]='\0';
	//strcpy(cwrite, "a");
	write(watchdog_fifo_fd, &cwrite, sizeof(cwrite));
	printf("\tfeed dog : %s !\n",cwrite);
	close(watchdog_fifo_fd);
}
#endif
