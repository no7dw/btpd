#ifndef __SERVER_H
#define __SERVER_H

#include <string.h>
#include <iostream>
#include <assert.h>

#include "./tinyxml/CgiSettingXml.h"
#include "./tinyxml/CgiXml.h"

#include "./TaskControl/Status.h"
#include "./TaskControl/EventsHandle.h"
#include "./TaskControl/TaskControl.h"
#include "./TaskControl/BTSetting.h"

// #include "./Pages/GetHtmlPage.h"
// #include "./Pages/TransToXml.h"
// #include "./Pages/PostPage.h"
#include "./Pages/WebBrowser.h"

#include "TransType.h"
#include "./deq/deq.h"
#include "client.h"


#include "./timer/TimerFunc.h"

#include "Util.h"

#include <signal.h>

//#define WATCH_DOG
//#include "./torrent/bnbt.h"

using namespace std;

// extern  string gstrErrorLogDir;
// extern  string gstrErrorLogFile;


///@Summary: thread that start a timer to AskStatus
void* thread_timer(void*);

///@Summary: load system setting
int InitSys(int argc, char **argv);

///@Summary:  End System
void EndSys(int sign_no);


///@Summary: 消息是否是升级包传过来的信息 
bool bIsSoftPackageMessage(JobInfoC *pJIC,CourseInfo *pCI );

/*
///@Summary:检查是否已存在升级包，并对已存在的低版本的升级包取消预约
int SingleSoftUpdatePackage(const CourseInfo &CI);
*/
///@Summary:获取正在下载的下载包信息
int GetDownloadingSoftUpdatePackage(JobInfo_CourseInfo &JICI);

///@Summary:新传入的下载包信息 比 现在下载中的软件升级包要高	
bool IsNewInSoftUpdatePackageNewer(const CourseInfo& CI_IN_BT_LIST ,const CourseInfo& CI);

///@Summary: 磁盘中是否有给文件 / 文件夹
bool HaveFinshedJobInDisk(const char* pJobName, bool isFile);

///@Summary: 保持只有一个升级包任务
int KeepSingleSoftUpdatePackage(const CourseInfo& CI_Comein);

#ifdef WATCH_DOG
///@Summary: 发消息给 watchdog ，表明： I'm alive
void CommunicateToWatchdog(char feedchar ='a' );
#endif

#endif
