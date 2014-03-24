#include "StudyRecordXml.h"
#include <stdio.h>
#include <iostream>
#include <string.h>


using namespace std;

StudyRecordXml::StudyRecordXml()
{

}
/*
{
	LoadXml();
}*/
StudyRecordXml::~StudyRecordXml()
{
}

int StudyRecordXml::LoadXml(const char* XmlFilePath)
{
	XXml::LoadXml(XmlFilePath);
	myDocument = new TiXmlDocument(XmlFilePath);//should declear in header
	myDocument ->LoadFile();
		
	//begin read
	TiXmlElement *RootElement = myDocument->RootElement();//StudyHistory

	TiXmlElement *FirstStudyRecordItem = RootElement->FirstChildElement();
	
	int index = 0;

	StudyRecordInfo SRI;
	
	
	while ( FirstStudyRecordItem != NULL )
	{
		SRI.StudyRecordInfoID = FirstStudyRecordItem->Attribute("ID");
		   
		TiXmlElement *FirstCourseIDItem = FirstStudyRecordItem->FirstChildElement();
		SRI.CourseID= FirstCourseIDItem->FirstChild()->Value();
		
		TiXmlElement *FirstStudy_IntervalItem = FirstCourseIDItem->NextSiblingElement();
		SRI.Study_Interval = atoi(FirstStudy_IntervalItem->FirstChild()->Value());
		
		TiXmlElement *FirstStudy_ScoreItem = FirstStudy_IntervalItem->NextSiblingElement();
		SRI.Study_Score= atoi(FirstStudy_ScoreItem->FirstChild()->Value());
		
		TiXmlElement *FirstStudy_TimeItem = FirstStudy_ScoreItem->NextSiblingElement();
		SRI.Study_Time= FirstStudy_TimeItem->FirstChild()->Value();
		
		TiXmlElement *FirstStudyPersonCountItem = FirstStudy_TimeItem->NextSiblingElement();
		SRI.Study_PersonCount= atoi(FirstStudyPersonCountItem->FirstChild()->Value());

		if( FirstStudyPersonCountItem->FirstChild() != NULL )
		{
			int count = atoi(FirstStudyPersonCountItem->FirstChild()->Value());
			SRI.Study_PersonCount = count;
			
			TiXmlElement *FirstPerson_ScoreItem = FirstStudyPersonCountItem->NextSiblingElement();

			for(int i = 0 ; i < count && FirstPerson_ScoreItem!=NULL ; i++ , FirstPerson_ScoreItem = FirstPerson_ScoreItem->NextSiblingElement())
			{
				if( FirstPerson_ScoreItem->FirstChild() != NULL )
				{
					int score = atoi(FirstPerson_ScoreItem->FirstChild()->Value());
					cout << i << " " << score << endl;
					switch(i)
					{
					  case 0:SRI.PersonA_Score = score;
						  break;
					  case 1:SRI.PersonB_Score = score;
						 break;
					  case 2:SRI.PersonC_Score = score;
						 break;
					  case 3:SRI.PersonD_Score = score;
						 break;
					  default:break;
					}
					
				}
				
				
			}
		}

		index = index + 1;
		
		VStudyRecordInfoArray.push_back(SRI);
//		cout <<" VStudyRecordInfoArray sizeof =" << sizeof(VStudyRecordInfoArray)<< endl;
			
		FirstStudyRecordItem = FirstStudyRecordItem->NextSiblingElement();
	}
	//end read
	
	cout <<"\nindex = "<< index << "#### Loop done ####" <<endl;
	
	return 1;
}


StudyRecordInfo* StudyRecordXml::GetStudyRecordInfo(string id)
{
    vector<StudyRecordInfo>::iterator iter;
    int i = 0;
    for( iter = VStudyRecordInfoArray.begin(); iter != VStudyRecordInfoArray.end(); ++i , ++iter)
    {
	//int iFound = id.find(iter->StudyRecordInfoID);
	//if (iFound != string::npos )
	if(iter->StudyRecordInfoID == id)
	  return &VStudyRecordInfoArray[i];
	 
	
    }
    return NULL;
    
}

vector <StudyRecordInfo>* StudyRecordXml::GetStudyRecordInfo()
{
    return &VStudyRecordInfoArray;
}