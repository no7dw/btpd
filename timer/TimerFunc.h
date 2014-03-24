#ifndef __TIMER_FUNC_H 
#define __TIMER_FUNC_H

#include "../TaskControl/EventsHandle.h"
#include "../TaskControl/Status.h"

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<sys/time.h>

#ifdef WATCH_TRAFFIC

	//网卡流量脚本 (带参数)
	//参数1  设备 如 eth0
	//参数2  更新数据的频率 (秒)
	//参数1  更新次数 （注意计算这个很重要）5分钟更新一次  5*60/频率
	#define TRAFFIC_WATCH_PATH "sh /usr/local/bin/traff2nos.sh eth0 1 60"
	//网卡eth0设备的流量的记录路径
	#define ETH0_TRAFFIC_WATCH "/tmp/watch_eth0"
	//网卡eth0设备的流量的记录的最大数值，需要单位: bytes/s
	#define ETH0_TRAFFIC_WATCH_MAX "/tmp/watch_eth0_max"

#endif


#define MAX_TIMER_COUNT 10  //设置最大的定时器个数

struct My_Timer //Timer结构体，用来保存一个定时器的信息
{   
	int total_time;  //每隔total_time秒
	int left_time;   //还剩left_time秒
	int func;        //该定时器超时，要执行的代码的标志
};    //定义Timer类型的数组，用来保存所有的定时器

class TimerFunc
{    
public:
	
	//start the some sec loop
	static int StartTimer();
#ifdef WATCH_TRAFFIC		
	static pthread_t pid_traffic_watch;
	
	static void watch_traffic();
	
	///@Summary: 线程启动检测网卡流量的脚本
	static void* thread_Net_Traffic_Watch(void*);
#endif
	
private:
	
	
	//int timer_totaltime; //i代表定时器的个数；t表示时间，逐秒递增
	static My_Timer my_timer[MAX_TIMER_COUNT];
	static int m_TimersCount;
	static int ticker;
	
	
	
	
private:

	//when some sec end do the THING
	static void Timeout(int sigty);
	
	///@Summary: set the ticker and function
	///@Param :计时器的间隙 
	///@Param :到时间后执行的函数序号。
	static void SetTimer(int iInterval,int ifunction);
	
};
    
#endif