#ifndef __STATUS_H
#define __STATUS_H

#include "../tinyxml/CgiXml.h"
// #include "../Pages/GetHtmlPage.h"
// #include "../Pages/TransToXml.h"

#include "../TransType.h"
#include "EventsHandle.h"
#include "../deq/deq.h"
#include "../Util.h"
#include "../tinyxml/JICIXml.h"


class Status
{
private:
	
	static vector<JobInfo> m_vJI;//当前时刻的任务信息
	static vector<CourseInfo> m_vCI;//当前时刻的信息
			
	static vector<JobInfo_CourseInfo> m_vJI_CI;//当前时刻的任务_课程信息	
	static vector<JobInfo_CourseInfo> m_vJI_CI_before_for_Check_LowSpeed;//之前一段时间记录的任务_课程信息	
	
	static int m_totalDownloadSpeed;//当前时刻的总下载速度
	static int m_totalUploadSpeed;//当前时刻的总上传速度
	
public:
	static pthread_mutex_t mtx_status;//更新是的互斥锁
	
	static vector<JobInfoC> m_vJIC_Finish;//当前时刻已完成的任务信息
private:

	
	///@Summary:匹配一对 课程信息 与 任务信息
	///@Param : 课程信息 
	///@Param : 任务信息 
	static void AMatch(const CourseInfo &match_CI1,const JobInfo &match_JI1);
	
	static int GetAllGradeCourseInfo(vector<CourseInfo> &vCI);
	
	///@Summary： 匹配传输的 课程信息vector 与 任务信息vector
	///@Param : 课程信息 vector
	///@Param : 任务信息 vector
	static void ReMatchVJVC(vector< JobInfo >& vJI, vector< CourseInfo >& vCI);
	
	///@Summary：比较名称 
	///@Param : 课程信息的名称
	///@Param : 任务信息的名称
	///@Param : 名称字符比较的长度
	///@Return :equal return 0
	///cfromutf8 < ctogbk return -1
	///cfromutf8 > ctogbk return 1
	static int CompareNames( char* cCourseName, char* cJobName , int compare_length);
	
#ifdef REPARE_NAME	
	//修复vector<JobInfo> &vJI中乱码的名称
	//vJI need to be sorted
	static int RepaieJobInfoName(vector<JobInfo> &vJI);
#endif
	
public:
	
	//@Summary: ask for TASK_ID by prividing CourseInfo
	static int WhatsMyJobID(CourseInfo CI);			
		
public:
	///@Summary: 根据队列xml文件的预约记录， 匹配当前任务的信息	
	static int ReMatch();
	
	//how to make sure outside does not change the ref vecotr ??BUG
	///@Summary: 获取当前任务信息
	static vector<JobInfo>* GetCurrentJobInfo(); 
	
	///@Summary： 获取当前的任务_课程信息
	static vector<JobInfo_CourseInfo>* GetCurrentJobInfo_CourseInfo();
	
	///@Summary： 获取前一个时间段的任务_课程信息
	static vector<JobInfo_CourseInfo>* GetBeforeJobInfo_CourseInfo();
	
	///@Summary： 外部调用 ， 每隔1小时 copy 以更新数据
	/// clear 后， 逐个 push_back()	
	static int Copy_Vector_JobInfo_CourseInfo();
	
	///@Summary： 输出任务信息
	static void PrintContents(vector<JobInfo> &vJI);
	
	///@Summary： 输出任务信息
	static void PrintContents(vector<JobInfoC> &vJIC);
	
	///@Summary： 输出任务_课程信息
	static void PrintContents(vector<JobInfo_CourseInfo> &vJI_CI);	
	
	///@Summary： 请求任务信息	
	static int AskForStatus(JobInfoC &JI);
	
	///@Summary: 长时间低速度任务检查
	///@Param : 前一段时间的任务_课程信息
	///@Param : 新的任务_课程信息
	static void LowSpeedCheck(vector<JobInfo_CourseInfo> &m_vJI_CI_before_for_Check_LowSpeed_Old,vector<JobInfo_CourseInfo> &m_vJI_CI_before_for_Check_LowSpeed_New);
	
	///@Summary: 获取 m_vJI_CI 中第一个未启动的任务
	static void GetFirstInactiveCourse(CourseInfo &CI, int &index);
	
	///@Summary: 获取已下载完成的任务
	///注意这里存储的是 CourseInfo
	///但返回的是 JobInfoC 的 vector 类型
	static void  GetFinisthJob( const char *szFinishRecordFile);
	
	///m_vJI_CI
	static void RemoveTheFinishJob(int index);
};
#endif
