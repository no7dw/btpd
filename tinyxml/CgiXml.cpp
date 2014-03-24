#include "CgiXml.h"
#include <stdio.h>
#include <iostream>
#include <string.h>


using namespace std;

CgiXML::CgiXML()/* : XXml()*/
{
   
}
CgiXML::~CgiXML()/* : ~XXml()*/
{
}

int CgiXML::checkNULL(TiXmlElement* pElem)
{
	if(pElem)
	{
		return 0;
	}
	else
	{
		printf("\nNULL TiXmlElement dectected\n");
		return -1;
	}
}


/*
 * BUG::Remember the ID will change,reboot will reset
 * FIXME::
 */
int CgiXML::LoadXml(const char *XmlFilePath)
{
		
	VJobInfo_Hash_Downloading.clear();
	VJobInfo_Hash_InActive.clear();
	
	if(XXml::LoadXml(XmlFilePath)<0)
		return -1;
		
	myDocument = new TiXmlDocument(m_XmlFilePath);
	if (!myDocument )
		return -1;
	
	if(!(myDocument ->LoadFile()))
		return -1;
		
	//begin read
	pRootElement = myDocument->RootElement();//report
	if (checkNULL(pRootElement) < 0 )
		return -1;
	
	TiXmlElement *OperationItem = pRootElement->FirstChildElement();//Operation
	if (checkNULL(OperationItem) < 0 )
		return -1;
	
	TiXmlElement *DaemonItem = OperationItem->NextSiblingElement();
	if (checkNULL(OperationItem) < 0 )
		return -1;
	
	DaemonStatus =	DaemonItem->FirstChild()->Value();	
	cout << " DaemonStatus = "<< DaemonStatus<<endl;
	
	TiXmlElement *TorrentItem = DaemonItem->NextSiblingElement();
	if (checkNULL(TorrentItem) < 0 )
		return -1;
	
	TiXmlElement *TotalCountItem = TorrentItem->FirstChildElement();
	if (checkNULL(TotalCountItem) < 0 )
		return -1;	
	TorrentCount =	atoi(TotalCountItem->FirstChild()->Value());

	TiXmlElement *JobItem = TotalCountItem->NextSiblingElement();
	if (checkNULL(JobItem) < 0 )
		return -1;
	
	int index = 0;
	JobInfo JI;
	while ( JobItem != NULL )
	{
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
	
		
		index = index + 1;
		
		VJobInfo.push_back(JI);
				
		JobInfo_Hash JIH;
		JIH.JI = JI;
		//handle the Hash
		JIH.Hash = "a577f6548e";
		
		//also save the downloading list and inactive list
		if(JI.Status == "downloading")
		{		    		    	    
		    VJobInfo_Hash_Downloading.push_back(JIH);		    
		}
		else if(JI.Status == "inactive/stopped")
		{		    		    
		    VJobInfo_Hash_InActive.push_back(JIH);		    
		}
		
		JobItem = JobItem->NextSiblingElement();
	}
// 	
	//LastStartPoint Should be compute here 
	
	//delete myDocument;//2011-5-13 bug
 
	//end read
	
	cout <<"\nindex = "<< index << "#### Loop done ####" <<endl;
	
	return 0;
}

JobInfo* CgiXML::GetJobInfo(string task_id)
{
  vector<JobInfo>::iterator iter;
    int i = 0;
    for( iter = VJobInfo.begin(); iter != VJobInfo.end(); ++i , ++iter)
    {
	//int iFound = task_id.find(iter->ID);
	//if (iFound != string::npos )
	if(iter->ID == task_id)
	  return &VJobInfo[i];	 
	
    }
    return NULL;
}
 
vector<JobInfo>* CgiXML::GetJobInfo()
{
	printf("[%s:%d]:%s()- #### - !!\n",__FILE__,__LINE__,__func__);
	return &VJobInfo;
}

vector<JobInfo_Hash>* CgiXML::GetJobInfo_Hash_Downloading()
{
    return &VJobInfo_Hash_Downloading;
}

vector<JobInfo_Hash>* CgiXML::GetJobInfo_Hash_InActive()
{
    return &VJobInfo_Hash_InActive;
}

int CgiXML::GetTotalSpeed(int &DownloadSpeed, int &UploadSpeed)
{
	vector<JobInfo>::iterator iter;    
	int totalDownLoadSpeed = 0, totalUpLoadSpeed = 0;
	for( iter = VJobInfo.begin(); iter != VJobInfo.end(); ++iter)
	{		
		 totalDownLoadSpeed+=iter->DwRate;
		 totalUpLoadSpeed+=iter->UpRate;
	}
	DownloadSpeed = totalDownLoadSpeed;
	UploadSpeed = totalUpLoadSpeed;
	return 0;
}

int CgiXML::WhatsMyID(CourseInfo CI)
{
    cout << " CI.hash "<< CI.Hash << endl;
    cout << " CI.Name "<< CI.Name << endl;
    
    vector<JobInfo>::iterator iter;    
    int i = 0;
    for( iter = VJobInfo.begin(); iter != VJobInfo.end(); ++i , ++iter)
    {
	 
	if(strcmp(CI.Name , iter->Name.c_str()) == 0 )//BUG:: here the CI.Name is not the cgiXML.xml 's NameList ,which is generate by  .torrent file
	{
	    cout << " iter->ID "<< iter->ID << endl;
	    return atoi(iter->ID.c_str());	
	}
    }
    return -1;
}

int CgiXML::GetTorrentCount()
{
    return TorrentCount;
}

