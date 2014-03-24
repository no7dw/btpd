#ifndef __CGI_XML_H
#define __CGI_XML_H

// #include "tinystr.h"
// #include "tinyxml.h"
// #include "Types.h"
// 
// #include <iostream>
// #include <string.h>
// #include <vector> 

#include <stdlib.h>
#include "XXml.h"
#include "../client.h"

#define MaxJob  4

using namespace std;


	
typedef struct {
		JobInfo JI;
		string/*char[49]*/ Hash;
		
	}JobInfo_Hash;
	
class CgiXML : public XXml
{
public:
	CgiXML();
	~CgiXML();
private:
	string DaemonStatus;
	int TorrentCount;
	vector<JobInfo> VJobInfo;
	vector<JobInfo_Hash> VJobInfo_Hash_Downloading;//status is downloading
	vector<JobInfo_Hash> VJobInfo_Hash_InActive;//status is inactive/stop
	
	int checkNULL(TiXmlElement *pElem);//检查是否为空
public:

	int LoadXml(const char *XmlFilePath);
	JobInfo* GetJobInfo(string task_id);
	vector<JobInfo>* GetJobInfo();
	vector<JobInfo_Hash>* GetJobInfo_Hash_Downloading();
	vector<JobInfo_Hash>* GetJobInfo_Hash_InActive();
	
	///@Summary: 获取总体下载速度于上传速度
	///@Param : 返回的下载速度的引用
	///@Param : 返回的上传速度的引用
	int GetTotalSpeed(int &DownloadSpeed, int &UploadSpeed);
		
	int WhatsMyID(CourseInfo CI);
	
	int GetTorrentCount();
	
};
#endif