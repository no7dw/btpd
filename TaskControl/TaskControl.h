#ifndef __TASK_CONTROL_H
#define __TASK_CONTROL_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "btif.h"
#include "utf8_conv.h"

//#include "../Pages/PostPage.h"

#include "../Pages/WebBrowser.h"



#include "../TransType.h"
#include "Status.h"

#include "BTSetting.h"

#include "../torrent/TorrentHash.h"

#define TORRENT_SAVE_PATH "/tmp/a.torrent"

#define BT_SAVE_PATH "/tmp/hdd/volumes/HDD1/BT/"
//#define TorrentFilePath "/tmp/1.torrent"
//#define BUFFER_SIZE 100000      //100KB


#include <string.h>
#include <iostream>
using namespace std;

class TaskControl
{
public:
	TaskControl();
	~TaskControl();
	
public:
	int Task_Add(const char *TorrentHash);
	int Task_Pause(const CourseInfo& CI);
	int Task_Pause(int id);
	int Task_Start(const CourseInfo& CI);
	int Task_Start(int id);
	int Task_Del(int id);
	int Task_Del(const CourseInfo& CI);
	int Task_DelWithout(int id);
	int Task_DelWithout(const CourseInfo& CI);
	
	///@Summary: 预约
	int Task_Book(const CourseInfo & CI);
	
	///@Summary: 取消预约
	int Task_UnBook(const CourseInfo& CI, bool blnDelResource = true);
	
	///@Summary: 优先级别调整
	///@Param : 课程信息
	///@Param : 该课程的此前的优先级别
	int Task_AdjustGrade(const CourseInfo& CI, int OldGrade);
	
	static int Task_Kill(int sec);
	
	///@Summary: 获取任务列表
	static int GetTorrentList(vector< JobInfo >& vJobInfoListU);
	
private:
	
	///@Summary: 预约与取消预约
	///@Param ： 1 课程信息结构体
	///@Param ： 2 是否是预约
	///@Param ： 3 是否需要在预约的时候添加任务
	///@Param ： 4 是否需要在取消预约的时候删除任务 use for AdjustGrade ,when the task has already started before, it just modify the grade but no need to del the task
	int Task_BookUnBook(const CourseInfo& CI , bool blnBook , bool blnNeedAddTorrent = true, bool blnDelTask = true, bool blnDelResource = true);
	
	///@Summary: 根据名称做比较，用于根据名称排序
	static int compare_index_smaller_vJILU(const JobInfo &j1,const JobInfo &j2);
};

#endif
