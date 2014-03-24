#include "CourseMainInfoXml.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
//#include "../Util.h"
#include "../Util.h"

using namespace std;

CourseMainInfoXml::CourseMainInfoXml()
{
	
}

CourseMainInfoXml::~CourseMainInfoXml()
{
}

int CourseMainInfoXml::LoadXml(const char *XmlFilePath)
{
	if(XXml::LoadXml(XmlFilePath)<0)
		return -1;
	
	myDocument = new TiXmlDocument(XmlFilePath);
	myDocument ->LoadFile();
		
	//begin read
	pRootElement = myDocument->RootElement();//CourseMainInfo

	TiXmlElement *NewCourseIDItem = pRootElement->FirstChildElement();
	TiXmlElement *DelCourseIDItem = NewCourseIDItem->NextSiblingElement();
	if(NewCourseIDItem->FirstChild() != NULL)
	{
		string strNewID = NewCourseIDItem->FirstChild()->Value();
		splitString( strNewID, "|", vNewID) ;
	}

	if(DelCourseIDItem->FirstChild() != NULL)
	{
		string strDelID = DelCourseIDItem->FirstChild()->Value();
		splitString( strDelID, "|", vDelID) ;
	}

	TiXmlElement *CourseListItem = DelCourseIDItem->NextSiblingElement();
	TiXmlElement *FirstCourseItem = CourseListItem->FirstChildElement();
	
	int index = 0;
	CourseMainInfo CMI;
	while ( FirstCourseItem != NULL )
	{
		TiXmlElement *FirstCourseIDItem = FirstCourseItem->FirstChildElement();
		CMI.CourseID = FirstCourseIDItem->FirstChild()->Value();
		
		TiXmlElement *FirstCourseNameItem = FirstCourseIDItem->NextSiblingElement();
		CMI.CourseName = FirstCourseNameItem->FirstChild()->Value();

		TiXmlElement *FirstCourseIntroItem = FirstCourseNameItem->NextSiblingElement();
		CMI.CourseIntro = FirstCourseIntroItem->FirstChild()->Value();
		
		TiXmlElement *FirstCourseScoreItem = FirstCourseIntroItem->NextSiblingElement();
		CMI.FirstScore = atoi(FirstCourseScoreItem->FirstChild()->Value());
		
		TiXmlElement *FirstCourseRecentScoreItem = FirstCourseScoreItem->NextSiblingElement();
		CMI.RecentScore = atoi(FirstCourseRecentScoreItem->FirstChild()->Value());

		TiXmlElement *FirstCourseFlowerItem = FirstCourseRecentScoreItem->NextSiblingElement();
		CMI.Flower = atoi(FirstCourseFlowerItem->FirstChild()->Value());

		TiXmlElement *FirstCourseHashItem = FirstCourseFlowerItem->NextSiblingElement();
		CMI.Hash = FirstCourseHashItem->FirstChild()->Value();

		index = index + 1;

		VCourseMainInfo.push_back(CMI);
		
		FirstCourseItem = FirstCourseItem->NextSiblingElement();
	}
	//end read
	
	cout <<"\nindex = "<< index << "#### Loop done ####" <<endl;
	
	return 1;
}

CourseMainInfo* CourseMainInfoXml::GetPartInfo(string id)
{
    vector<CourseMainInfo>::iterator iter;
    int i = 0;
    for( iter = VCourseMainInfo.begin(); iter != VCourseMainInfo.end(); ++i , ++iter)
    {
	int iFound = id.find(iter->CourseID);
	if (iFound != string::npos )//if(iter->StudyRecordInfoID == id)
	  return &VCourseMainInfo[i];
	 
	
    }
    return NULL;
}

/*
vector<string>* CourseMainInfoXml::GetCourseID()
{
    vector<CourseMainInfo>::iterator iter;

    for( iter = VCourseMainInfo.begin(); iter != VCourseMainInfo.end(); ++iter)
    {	    
	VCourseID.push_back(*iter);
    }
    return &VCourseID;
	
}
*/

vector<CourseMainInfo>* CourseMainInfoXml::GetPartInfo()
{
	
	  return &VCourseMainInfo;
}

VSID* CourseMainInfoXml::GetNewIDVector()
{
	return &vNewID;
}

VSID* CourseMainInfoXml::GetDelIDVector()
{
	return &vDelID;
}
