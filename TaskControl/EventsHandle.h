#ifndef __EVENTS_HANDLE_H
#define __EVENTS_HANDLE_H

#include "../client.h"
#include "../deq/deq.h"
#include "../TransType.h"
#include "./Status.h"
#include "./TaskControl.h"

#include "../MessageSend.h"
#include "BTSetting.h"

#include "../tinyxml/JICIXml.h"

#define VERY_LIMITED_OF_UNLEARN_COURSE_COUNT 5
#define LEVEL_SET_TO_WHEN_VERY_LIMITED 2

#define FINISH_JOBS_RECORD_FILE "/tmp/hdd/volumes/HDD1/finish_list.xml"


///@Summary: 检查下载完成事件
void Check_Event_Compelete( vector<JobInfo_CourseInfo> &v_JICI);

//step 1 remove from queuex
//step 2 set the next one start from queue1 to queue4
//step 3 Need to delete task when upload is long enough
//step 4 May need to notify the StudyMode Programme according to client.pid

///@Summary: 下载完成时间处理
///@Step 1: 调用写 100% 消息
///@Step 2:复制 该下载任务记录 到 下载完成记录 。 （注意处理重复性）
///@Step 3: 若 预约下载任务数 > 允许同时下载任务数 ，则进行 移除该任务（但不删除对应文件）。否则继续做种
///@Step 4: 开启下一个未启动的任务
void Event_Complete(const CourseInfo &CI);


///@Summary :写 100% 消息
int Write_100_Status(const char *CourseID, const char *CourseName);

///@Summary :写 下载任务记录 到 下载完成记录
int Write_Finish_Course(const CourseInfo &CI, const char *szFinishRecordFile);

//step 1 pause the last download list item
//step 2 move the last download list item from download list to pause list (modify 2 xml)
void Event_AllowDownload_Smaller();

///@Summary: CurrentLevel Course Count is less than or equal to five , set course of CurrentLevel in the booklist  in Grade 2
//step 1 find out the CurrentLevel Course 
//step 2 call set grade 2
int Event_Current_Course_Very_Limited(int CurrentLevel, int UnLearnCourseCount, StudyMode SMode);

///@Summary: User has update his Level is StudyMode
///step 1 find out the before Level Course of Studymode
///step 2 call set grade 4
int Event_StudyMode_Has_Updated(int CurrentLevel,  StudyMode SMode );

///@Summary: 长时间低速，写入日志，以便交到服务器进行检查
///@Param : 资源的 Hash
int Event_LowSpeed(const char *LowSpeedHash);

///@Summary : 根据网卡最大流入速度允许同时下载的任务数改变
///@Param : 网卡最大流入速度
///@Return : 成功 0 失败 -1
int Event_Simutane_Allow_Change(int Max_Download_Speed);



#endif

