#include "StudyRecordXml.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc , char** argv)
{
	if(argc<2)
	{
		cout<< "Usage: Version filename" << endl;
		return 0;
	}
	
	StudyRecordXml *pStudyRecordXmlObj= new StudyRecordXml();
	pStudyRecordXmlObj->LoadXml(argv[1]);
	//method 1
/*	cout<< pStudyRecordXmlObj->GetStudyRecordInfo("000124")->CourseID << endl;
	cout<< pStudyRecordXmlObj->GetStudyRecordInfo("000124")->Study_Interval << endl;
	cout<< pStudyRecordXmlObj->GetStudyRecordInfo("000124")->Study_Score << endl;
	cout<< pStudyRecordXmlObj->GetStudyRecordInfo("000124")->Study_Time << endl;		
	cout<< pStudyRecordXmlObj->GetStudyRecordInfo("000124")->Study_PersonCount << endl;
	cout<< pStudyRecordXmlObj->GetStudyRecordInfo("000124")->PersonA_Score << endl;
	cout<< pStudyRecordXmlObj->GetStudyRecordInfo("000124")->PersonB_Score << endl;*/
	
	//method 2
	cout << endl;
	StudyRecordInfo *pSRI;
	pSRI = pStudyRecordXmlObj->GetStudyRecordInfo("000124");
	if (pSRI!=NULL)
	{
	  cout<< pSRI->CourseID << endl;
	  cout<< pSRI->Study_Interval << endl;
	  cout<< pSRI->Study_Time << endl;
	  cout<< pSRI->Study_PersonCount << endl;
	  cout<< pSRI->PersonA_Score << endl;
	  cout<< pSRI->PersonB_Score << endl;
	}
	else
	  cout << "null" << endl;
	
	vector <StudyRecordInfo> VSRI;
	vector <StudyRecordInfo>::iterator iter;
	VSRI = *pStudyRecordXmlObj->GetStudyRecordInfo();
	int i =0;
	for( iter = VSRI.begin(); iter != VSRI.end(); ++i , ++iter)
	{
	    cout << i<<" " ;
	    cout << (*iter).CourseID << "\t" << (*iter).Study_Interval << "\t"<< (*iter).Study_Score << "\t"<< (*iter).Study_Time << "\t"<< (*iter).Study_PersonCount << "\t"<< (*iter).PersonA_Score << "\t"<< endl;
	}

	delete pStudyRecordXmlObj;
	pStudyRecordXmlObj =NULL;
	
	//method 3
// 	cout << endl;
// 	StudyRecordInfo SRI,*pSRI2;
// 	pSRI2 = pStudyRecordXmlObj->GetStudyRecordInfo("000124");
// 	memcpy( &SRI , pSRI2 , sizeof(StudyRecordInfo) );
// 	cout<< SRI.Study_Score << endl;
	
	return 1;
	
}
