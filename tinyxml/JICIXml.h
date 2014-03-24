#ifndef __JI_CI_XML_H
#define __JI_CI_XML_H


#include <stdlib.h>
#include "XXml.h"
#include "../client.h"

#define MaxJob  4

using namespace std;
/*
typedef struct {
		string ID;
		int Priority;
		string Partition;
		string Space;
		string Name;
		string Status;
		double Size;
		double Downloaded;
		string TimeLeft;
		string Percent;
		double DwRate;
		double UpRate;
		int Npeers;
		
	}JobInfo;
	*/

/*	
typedef struct {
		JobInfo JI;
		string Hash;//char[49]
		
	}JobInfo_Hash;
*/

typedef struct {
	JobInfo JI;
	CourseInfo CI;
}JobInfo_CourseInfo;

class JICIXml : public XXml
{
public:
	JICIXml();
	~JICIXml();
private:
	string DaemonStatus;
	int TorrentCount;
	
	vector<JobInfo_CourseInfo> VJobInfo_CourseInfo;

	vector<JobInfo_CourseInfo> VJobInfo_CourseInfo_Downloading;//status is downloading
	vector<JobInfo_CourseInfo> VJobInfo_CourseInfo_InActive;//status is inactive/stop
	
		
public:

	int LoadXml(const char *XmlFilePath);
	
	JobInfo_CourseInfo* GetJobInfo_CourseInfo(string task_id);
	
	//CourseID will set to JobInfo
	int GetJobInfo(const char *CourseID, JobInfo &JI);
	
	//update JobInfo part
	int UpdateJobInfo(vector<JobInfo> &VJI);
	//also we  need additem
	
	//remove item
	
	
	vector<JobInfo_CourseInfo>* GetJobInfo_CourseInfo();
	vector<JobInfo_CourseInfo>* GetJobInfo_CourseInfo_Downloading();
	vector<JobInfo_CourseInfo>* GetJobInfo_CourseInfo_InActive();
		
	int WhatsMyID(CourseInfo CI);
	int GetTorrentCount();
	
};
#endif
