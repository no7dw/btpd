#include "JICIXml.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "../Util.h"

using namespace std;

JICIXml::JICIXml()/* : XXml()*/
{
   
}
JICIXml::~JICIXml()/* : ~XXml()*/
{
}
/*
 * BUG::Remember the ID will change,reboot will reset
 * FIXME::
 */
int JICIXml::LoadXml(const char *XmlFilePath)
{
	
	VJobInfo_CourseInfo_Downloading.clear();
	VJobInfo_CourseInfo_Downloading.clear();
	
	if(XXml::LoadXml(XmlFilePath)<0)
		return -1;
	
	myDocument = new TiXmlDocument(m_XmlFilePath);	

	myDocument ->LoadFile();
		
	//begin read
	pRootElement = myDocument->RootElement();//report
	if(pRootElement == NULL)
		return -1;
	
	TiXmlElement *OperationItem = pRootElement->FirstChildElement();//Operation

	TiXmlElement *DaemonItem = OperationItem->NextSiblingElement();
	
	DaemonStatus =	DaemonItem->FirstChild()->Value();
	cout << " DaemonStatus = "<< DaemonStatus<<endl;
	
	TiXmlElement *TorrentItem = DaemonItem->NextSiblingElement();
	TiXmlElement *TotalCountItem = TorrentItem->FirstChildElement();
	TorrentCount =	atoi(TotalCountItem->FirstChild()->Value());

	TiXmlElement *JobItem = TotalCountItem->NextSiblingElement();
	
	int index = 0;
	JobInfo JI;
	CourseInfo CI;
	JobInfo_CourseInfo JI_CI;
	while ( JobItem != NULL )
	{
		//////////////////////JobInfo/////////////////////////////////////
		TiXmlElement *IDItem = JobItem->FirstChildElement();
		JI.ID = IDItem->FirstChild()->Value();
		
		TiXmlElement *PriorityItem = IDItem->NextSiblingElement();
		JI.Priority = atoi( PriorityItem->FirstChild()->Value());
		
		TiXmlElement *PartitionItem = PriorityItem->NextSiblingElement();
		JI.Partition = PartitionItem->FirstChild()->Value();
		
		TiXmlElement *SpaceItem = PartitionItem->NextSiblingElement();
		JI.Space = SpaceItem->FirstChild()->Value();
		
		TiXmlElement *NameItem = SpaceItem->NextSiblingElement();
		JI.Name = NameItem->FirstChild()->Value();
		
		TiXmlElement *StatusItem = NameItem->NextSiblingElement();
		JI.Status = StatusItem->FirstChild()->Value();		
		
		TiXmlElement *SizeItem = StatusItem->NextSiblingElement();
		JI.Size = atof( SizeItem->FirstChild()->Value());
		
		TiXmlElement *DownloadedItem = SizeItem->NextSiblingElement();
		JI.Downloaded = atof(DownloadedItem->FirstChild()->Value());
		
		TiXmlElement *TimeLeftItem = DownloadedItem->NextSiblingElement();
		JI.TimeLeft = TimeLeftItem->FirstChild()->Value();
		
		TiXmlElement *PercentItem = TimeLeftItem->NextSiblingElement();
		JI.Percent = PercentItem->FirstChild()->Value();
		
		TiXmlElement *DwRateItem = PercentItem->NextSiblingElement();
		JI.DwRate = atof( DwRateItem->FirstChild()->Value());//BUG:: this is a string contains kB/s
		
		TiXmlElement *UpRateItem = DwRateItem->NextSiblingElement();
		JI.UpRate = atof( UpRateItem->FirstChild()->Value());//BUG:: this is a string contains kB/s
		
		TiXmlElement *NpeersItem = UpRateItem->NextSiblingElement();
		JI.Npeers = atoi(NpeersItem->FirstChild()->Value());
	
		//////////////////////CourseInfo/////////////////////////////////////
		
		TiXmlElement *FirstCourseItem = NpeersItem->NextSiblingElement();
		
		TiXmlElement *FirstCourseIDItem = FirstCourseItem->FirstChildElement();
		strcpy(CI.ID , FirstCourseIDItem->FirstChild()->Value());
		
		TiXmlElement *FirstCourseNameItem = FirstCourseIDItem->NextSiblingElement();
		strcpy(CI.Name , FirstCourseNameItem->FirstChild()->Value());

		TiXmlElement *FirstCoursePathItem = FirstCourseNameItem->NextSiblingElement();
		strcpy(CI.Path , FirstCoursePathItem->FirstChild()->Value());
		
		TiXmlElement *FirstCourseHashItem = FirstCoursePathItem->NextSiblingElement();		
		strcpy(CI.Hash , FirstCourseHashItem->FirstChild()->Value());
		
		
		TiXmlElement *FirstCourseLevelItem = FirstCourseHashItem->NextSiblingElement();	
		CI.Level = atoi(FirstCourseLevelItem->FirstChild()->Value());

		TiXmlElement *FirstCourseGradeItem = FirstCourseLevelItem->NextSiblingElement();		
		CI.Grade = atoi(FirstCourseGradeItem->FirstChild()->Value());		

		TiXmlElement *FirstCourseStudyModeItem = FirstCourseGradeItem->NextSiblingElement();
		CI.SendbySM = atoi( FirstCourseStudyModeItem->FirstChild()->Value());		
		
		///////////////////////////////////////////////////////////
		
		index = index + 1;
		JI_CI.JI = JI;
		JI_CI.CI = CI;
		VJobInfo_CourseInfo.push_back(JI_CI);
					
		//also save the downloading list and inactive list
		if(JI.Status == "downloading")
		{		    		    	    
		    VJobInfo_CourseInfo_Downloading.push_back(JI_CI);		    
		}
		else if(JI.Status == "inactive/stopped")
		{		    		    
		    VJobInfo_CourseInfo_InActive.push_back(JI_CI);		    
		}
		
		JobItem = JobItem->NextSiblingElement();
	}

	
	cout <<"\nindex = "<< index << "#### Loop done ####" <<endl;
	
	return 0;
}

JobInfo_CourseInfo* JICIXml::GetJobInfo_CourseInfo(string task_id)
{
	vector<JobInfo_CourseInfo>::iterator iter;
	int i = 0;
	for( iter = VJobInfo_CourseInfo.begin(); iter != VJobInfo_CourseInfo.end(); ++i , ++iter)
	{
		//int iFound = task_id.find(iter->JI.ID);
		//if (iFound != string::npos )
		if(iter->JI.ID == task_id)
			return &VJobInfo_CourseInfo[i];	 

	}
	return NULL;
}

int JICIXml::GetJobInfo(const char *CourseID, JobInfo &JI)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"!! begin looking for JobInfo for CourseID !!");
	vector<JobInfo_CourseInfo>::iterator iter;
	int i = 0;
	for( iter = VJobInfo_CourseInfo.begin(); iter != VJobInfo_CourseInfo.end(); ++i , ++iter)
	{		
		if (strcmp(CourseID,(*iter).CI.ID) == 0 )
		{
			JI = VJobInfo_CourseInfo[i].JI;
			JI.ID = VJobInfo_CourseInfo[i].CI.ID;//对外JobInfo的ID为CourseID
			printf("\nbefore JI.Name = %s \n",JI.Name.c_str());
			JI.Name = VJobInfo_CourseInfo[i].CI.Name;//绕过乱码
			printf("\nafter  JI.Name = %s \nJI.Name = CI.Name\n",JI.Name.c_str());
			
			
			char SrcDirPath[100] = {0};
			char TargetDirPath[100] = {0};
			
			if(JI.Percent=="100%")
			{
				sprintf(SrcDirPath ,  "/tmp/hdd/volumes/HDD1/BT/%s" , VJobInfo_CourseInfo[i].CI.Name);
				sprintf(TargetDirPath , "/tmp/hdd/volumes/HDD1/LGSystem/STXD/第%d阶/%s",VJobInfo_CourseInfo[i].CI.Level, VJobInfo_CourseInfo[i].CI.Name );
				UTIL_CopyDir(SrcDirPath, TargetDirPath);				
			}
			return 0;
		}		
	}
	return 0;
}

int JICIXml::UpdateJobInfo(vector<JobInfo>& VJI)
{
	TiXmlElement *OperationItem = pRootElement->FirstChildElement();//Operation
	TiXmlElement *DaemonItem = OperationItem->NextSiblingElement();	
	TiXmlElement *TorrentItem = DaemonItem->NextSiblingElement();
	TiXmlElement *TotalCountItem = TorrentItem->FirstChildElement();	

	TiXmlElement *JobItem = TotalCountItem->NextSiblingElement();
	
	int index = 0;
	JobInfo JI;
	CourseInfo CI;
	JobInfo_CourseInfo JI_CI;
	while ( JobItem != NULL )
	{
		//////////////////////JobInfo/////////////////////////////////////
		TiXmlElement *IDItem = JobItem->FirstChildElement();
		JI.ID = IDItem->FirstChild()->Value();
		
		TiXmlElement *PriorityItem = IDItem->NextSiblingElement();
		JI.Priority = atoi( PriorityItem->FirstChild()->Value());
		
		TiXmlElement *PartitionItem = PriorityItem->NextSiblingElement();
		JI.Partition = PartitionItem->FirstChild()->Value();
		
		TiXmlElement *SpaceItem = PartitionItem->NextSiblingElement();
		JI.Space = SpaceItem->FirstChild()->Value();
		
		TiXmlElement *NameItem = SpaceItem->NextSiblingElement();
		JI.Name = NameItem->FirstChild()->Value();
		
		TiXmlElement *StatusItem = NameItem->NextSiblingElement();
		JI.Status = StatusItem->FirstChild()->Value();		
		
		TiXmlElement *SizeItem = StatusItem->NextSiblingElement();
		JI.Size = atof( SizeItem->FirstChild()->Value());
		
		TiXmlElement *DownloadedItem = SizeItem->NextSiblingElement();
		JI.Downloaded = atof(DownloadedItem->FirstChild()->Value());
		
		TiXmlElement *TimeLeftItem = DownloadedItem->NextSiblingElement();
		JI.TimeLeft = TimeLeftItem->FirstChild()->Value();
		
		TiXmlElement *PercentItem = TimeLeftItem->NextSiblingElement();
		JI.Percent = PercentItem->FirstChild()->Value();
		
		TiXmlElement *DwRateItem = PercentItem->NextSiblingElement();
		JI.DwRate = atof( DwRateItem->FirstChild()->Value());//BUG:: this is a string contains kB/s
		
		TiXmlElement *UpRateItem = DwRateItem->NextSiblingElement();
		JI.UpRate = atof( UpRateItem->FirstChild()->Value());//BUG:: this is a string contains kB/s
		
		TiXmlElement *NpeersItem = UpRateItem->NextSiblingElement();
		JI.Npeers = atoi(NpeersItem->FirstChild()->Value());
		
		///////////////match//////////////////
		vector<JobInfo>::iterator iter;
		for(iter= VJI.begin();iter!=VJI.end();iter++)
		{
			if(JI.Size == (*iter).Size /*&& other condiction*/)
			{
				UTIL_Debug_Print(__FILE__,__LINE__,__func__,"!! Find a Match !!");
				IDItem->FirstChild()->SetValue((*iter).ID.c_str());
				char Downloaded[10];
				sprintf(Downloaded, "%6.2f",(*iter).Downloaded );
				DownloadedItem->FirstChild()->SetValue(Downloaded);
				TimeLeftItem->FirstChild()->SetValue((*iter).TimeLeft.c_str());
				PercentItem->FirstChild()->SetValue((*iter).Percent.c_str());
				char DwRate[10];
				sprintf(DwRate, "%6.2f",(*iter).DwRate );
				DwRateItem->FirstChild()->SetValue(DwRate);
				char UpRate[10];
				sprintf(UpRate, "%6.2f",(*iter).UpRate );
				UpRateItem->FirstChild()->SetValue(UpRate);
				char Npeers[10];
				sprintf(Npeers, "%d",(*iter).Npeers );
				NpeersItem->FirstChild()->SetValue(Npeers);
				break;
			}
		}
		
		
		index = index + 1;			
		JobItem = JobItem->NextSiblingElement();
	}
	myDocument->SaveFile();
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"!! UpdateJobInfo Save Compelete !!");
	return 0;
}

 
vector<JobInfo_CourseInfo>* JICIXml::GetJobInfo_CourseInfo()
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__);
    return &VJobInfo_CourseInfo;
}

vector<JobInfo_CourseInfo>* JICIXml::GetJobInfo_CourseInfo_Downloading()
{
    return &VJobInfo_CourseInfo_Downloading;
}

vector<JobInfo_CourseInfo>* JICIXml::GetJobInfo_CourseInfo_InActive()
{
    return &VJobInfo_CourseInfo_InActive;
}

int JICIXml::WhatsMyID(CourseInfo CI)
{
    cout << " CI.hash "<< CI.Hash << endl;
    cout << " CI.Name "<< CI.Name << endl;
    
    vector<JobInfo_CourseInfo>::iterator iter;    
    int i = 0;
    for( iter = VJobInfo_CourseInfo.begin(); iter != VJobInfo_CourseInfo.end(); ++i , ++iter)
    {
	 
	//if(strcmp(CI.Name , iter->Name.c_str()) == 0 )//BUG:: here the CI.Name is not the cgiXML.xml 's NameList ,which is generate by  .torrent file
	if(strcmp(CI.Hash , (iter->CI).Hash) == 0 )
	{
	    cout << " (iter->CI).ID "<< (iter->CI).ID << endl;
	    return atoi((iter->CI).ID);	
	}
    }
    return -1;
}

int JICIXml::GetTorrentCount()
{
    return TorrentCount;
}

