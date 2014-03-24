#include "TimerFunc.h"
#include "../TaskControl/EventsHandle.h"

#include "../server.h"


#define DEBUG_OUTPUT true

My_Timer TimerFunc::my_timer[MAX_TIMER_COUNT]={0};
int TimerFunc::m_TimersCount=0;
int TimerFunc::ticker=0;

#ifdef WATCH_TRAFFIC
	pthread_t TimerFunc::pid_traffic_watch;
#endif

void TimerFunc::SetTimer(int iInterval,int ifunction) //新建一个计时器
{   
    struct My_Timer a;
    a.total_time = iInterval;
    a.left_time = iInterval;
    a.func = ifunction;
    
    my_timer[m_TimersCount++] = a;
    
    
}

void TimerFunc::Timeout(int sigty)  //判断定时器是否超时，以及超时时所要执行的动作
{  

	int j;
	
	for( j = 0; j <= m_TimersCount; j++ )
	{  
		if(my_timer[j].left_time > 0 )
		{
			my_timer[j].left_time--;//continue the counter
			
		}
		else
		{   
			
			switch(my_timer[j].func) //通过匹配my_timer[j].func，判断下一步选择哪种操作
			{				
				case 1://任务状态的扫描
				{
#ifdef WATCH_DOG
					CommunicateToWatchdog();
#endif
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"going to refresh Jobs");
					JobInfoC JIC1;
					char result[200] = {0};				    
					JobInfo JI,*pJI;
					vector<JobInfo> VJI ;
				
			
					Status::ReMatch();//BUG this refresh the data ,but the AskStatus will ask for the data
					Status::PrintContents(*(Status::GetCurrentJobInfo_CourseInfo()));
					
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"\n\ngoing to finished Jobs\n");
					Status::GetFinisthJob(FINISH_JOBS_RECORD_FILE);						
					Status::PrintContents(Status::m_vJIC_Finish);		
				}
				break;
				case 2://下载情况检查
				{
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"going to refresh speed check");
					
					Status::LowSpeedCheck( *Status::GetBeforeJobInfo_CourseInfo() , *Status::GetCurrentJobInfo_CourseInfo() );
					Status::Copy_Vector_JobInfo_CourseInfo();//定时更新数据
				}
				break;
#ifdef WATCH_TRAFFIC				
				case 3://网卡速度检查（读取最大数值）
				{
					UTIL_Debug_Print(__FILE__,__LINE__,__func__,"going to refresh max eth0 speed check");
					watch_traffic();
					int max_eth0_speed = UTIL_Read_MAX_Speed_ETH0(ETH0_TRAFFIC_WATCH_MAX);
					printf("max_eth0_speed : %d \n", max_eth0_speed );					
					Event_Simutane_Allow_Change(max_eth0_speed);
				}
				break;
#endif
			}
			if(my_timer[j].total_time != 0)
			{
				my_timer[j].total_time =my_timer[j].total_time ;
				my_timer[j].left_time = my_timer[j].total_time;     //循环计时				
				printf("Reset Timer to %d \n",my_timer[j].total_time);
			}
			
		}
	}
}

#ifdef WATCH_TRAFFIC
void* TimerFunc::thread_Net_Traffic_Watch(void*)
{
	printf("This is a thread_Net_Traffic_Watch\n");
	system(TRAFFIC_WATCH_PATH);//5分钟后结束 1秒刷新一次 的话 计算300次，注意参数
	printf("system end\n");
	pthread_exit(NULL);
	
}

void TimerFunc::watch_traffic()
{
	//启动
	int ret = pthread_create(&pid_traffic_watch,NULL,thread_Net_Traffic_Watch,NULL);
        if(ret!=0)
        {
                printf("Create pthread error!\n");
                return ;
        }    
}
#endif


//int main()  //测试函数，定义三个定时器
int TimerFunc::StartTimer()
{   
    UTIL_Debug_Print(__FILE__,__LINE__,__func__);
    m_TimersCount = 0;
    
    if(ticker>0 )
    {
	return 0;
	printf("already StartTimer\n");
    }
    else
    {
	ticker = 0;
	printf("start StartTimer\n");
    }
    
    pid_t pid;
    if((pid = fork()) < 0)
    {
	return -1;
    }
    else if(pid == 0) //child
    {
	SetTimer( 25, 1);//计时器1：25秒进行一次任务状态的扫描
	
	SetTimer( 60*60, 2);//计时器2： 1小时进行一次下载情况检查
	
#ifdef WATCH_TRAFFIC	
	SetTimer( 5*60, 3);//计时器3： 5分钟进行一次网卡速度检查（读取最大数值）
#endif	    
	
	signal(SIGALRM,Timeout);  //接到SIGALRM信号，则执行timeout函数
	while(1)
	{  
	    sleep(1);  //每隔一秒发送一个SIGALRM
	    kill(getpid(),SIGALRM);//the  same  as raise(SIGALRM)
	}
    
    }
    else
	return 0;
    
    
}
