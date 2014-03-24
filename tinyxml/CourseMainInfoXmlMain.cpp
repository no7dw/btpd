#include "CourseMainInfoXml.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc , char** argv)
{
	if(argc<2)
	{
		cout<< "Usage: Version filename" << endl;
		return -1;
	}

	CourseMainInfoXml *CourseMainInfoXmlObj= new CourseMainInfoXml();
	
	CourseMainInfoXmlObj->LoadXml(argv[1]);
//	cout<< SettingObj->GetSystemSettingVersion() <<endl;
//	cout<< SettingObj->GetCourseListVersion(4) <<endl;
	
	CourseMainInfo *pCMI;
	vector<CourseMainInfo> VCMI1, *pVCMI1;
//	pCMI = CourseMainInfoXmlObj->GetPartInfo("000124");
/*
	if (pCMI!=NULL)
	{
	  cout<< pCMI->CourseID << endl;
	  cout<< pCMI->CourseName << endl;
	  cout<< pCMI->CourseIntro << endl;
	  cout<< pCMI->CoursePath << endl;
	  cout<< pCMI->FirstScore << endl;
	  cout<< pCMI->RecentScore << endl;	  

	}
	else
	  cout << "null" << endl;
*/	
	//method 1
	cout << endl << "method 1" << endl;
	pVCMI1 = CourseMainInfoXmlObj->GetPartInfo();	
	//memcpy(&VCMI1, pVCMI1 , sizeof(CourseMainInfo)*pVCMI1->size());
	
	int i = 0;
	vector<CourseMainInfo>::iterator iter;
	for( iter = pVCMI1->begin(); iter != pVCMI1->end(); ++i , ++iter)
	{
	    cout << i<<" " ;
	    cout << (*iter).CourseName << "\t" << (*iter).CourseID << "\t"<< (*iter).CourseIntro  << "\t"<< (*iter).FirstScore << "\t"<< (*iter).RecentScore << "\t"<< (*iter).Flower <<"\t" << (*iter).Hash << endl;
	}
		
	
	cout << endl << "method 2" << endl;
	//method 2
	VCMI1 = *CourseMainInfoXmlObj->GetPartInfo();
	for( iter = VCMI1.begin(); iter != VCMI1.end(); ++i , ++iter)
	{
	    cout << i<<" " ;
	    cout << (*iter).CourseName << "\t" << (*iter).CourseID << "\t"<< (*iter).CourseIntro  << "\t"<< (*iter).FirstScore << "\t"<< (*iter).RecentScore << "\t"<<(*iter).Flower <<"\t"<< (*iter).Hash<< endl;
	}
	VSID *pvNewID, *pvDelID;
	VSID::iterator iter1;
	pvNewID = CourseMainInfoXmlObj->GetNewIDVector();
	pvDelID = CourseMainInfoXmlObj->GetDelIDVector();

	for( iter1 = pvNewID->begin(); iter1 != pvNewID->end(); ++i , ++iter1)
	{
		cout << i << " " << (*iter1) << endl;
	}
	for( iter1 = pvDelID->begin(); iter1 != pvDelID->end(); ++i , ++iter1)
	{
		cout << i << " " << (*iter1) << endl;
	}
	delete CourseMainInfoXmlObj;
	CourseMainInfoXmlObj =NULL;
	
// 	delete pVCMI1;
// 	pVCMI1 = NULL;
	return 0;
	//_exit(1);
	
}
