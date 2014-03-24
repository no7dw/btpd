#include "Sort.h"
#include "../client.h"

#define MAX_PATH 200

using namespace std;


bool compare_index_smaller(const CourseMainInfo &t1,const CourseMainInfo &t2)// from smaller to greater
{
	int iresult =  strcmp(t2.CourseID.c_str() , t1.CourseID.c_str());
	//printf("strcmp:%s,%s",t2.CourseID.c_str(),t1.CourseID.c_str());
	//printf(" = %d \n", iresult);
	if(iresult>=0)
		return true;
	else
		return false;
}

bool compare_index_greater(const CourseMainInfo &t1,const CourseMainInfo &t2)// from greater to smaller
{
	//cout << ++countindex << endl;	
	int iresult =  strcmp(t2.CourseID.c_str() , t1.CourseID.c_str());
	//printf("strcmp:%s,%s",t2.CourseID.c_str(),t1.CourseID.c_str());
	//printf(" = %d \n", iresult);
	if(iresult>=0)
		return false;
	else
		return true;	
	
}

void printout(vector<CourseMainInfo> &VCMI)
{
	
	vector<CourseMainInfo>::iterator iter;
	for(iter = VCMI.begin(); iter != VCMI.end(); ++iter)
	{
		cout << (*iter).CourseID << "\t";
		cout << (*iter).CourseName << "\t";
		cout << (*iter).CourseIntro << "\t";
		cout << (*iter).Hash<< endl;
	}
	
		
}

int CheckDisk(StudyMode mode)
{
	printf("\nmode : %d",mode);
	UTIL_Debug_Print(__FILE__,__LINE__,__func__ ,"\n\n\nChecking Disk");
	
	string StudyModeResourcePath;
	
	switch(mode)
	{
		case AVTraining:
			StudyModeResourcePath=AVT_DIRPATH;
			break;
		case ViewPicture:
			StudyModeResourcePath=VP_DIRPATH;
			break;
		case LyricSpeak:
			StudyModeResourcePath=LS_DIRPATH;
			break;
		default:
			return -1;			
	}
	
	long fsize = 0;
	UTIL_CountFileSize(StudyModeResourcePath.c_str(), fsize );
	if(fsize <= MAX_CAPABILITY_EACH_STYLE)
		return 1;//no need to del files
		
	for(int CourseIndex = 1;CourseIndex <= 12 ;CourseIndex++)//sort and del course start from level 1 to level 12
	{
		
		char CourseInfoNXmlPath[MAX_PATH] = {0};
		sprintf(CourseInfoNXmlPath, "%sCourseInfo%d.xml",StudyModeResourcePath.c_str(),CourseIndex);//construst the path 
		vector<CourseMainInfo> VCMI1;
		printf("\nCourseInfoNXmlPath ： %s\n",CourseInfoNXmlPath);
		SortCourse( CourseInfoNXmlPath , VCMI1 );
		DelUntil(StudyModeResourcePath, VCMI1 ,MAX_CAPABILITY_EACH_STYLE );
		
	}
	return 0;
}



int SortCourse(const char *xmlpath, vector<CourseMainInfo> &VCMI1) 
{	
	UTIL_Debug_Print(__FILE__,__LINE__,__func__ ,"begin sorting Course");
	CourseMainInfoXml *CourseMainInfoXmlObj= new CourseMainInfoXml();
		
	CourseMainInfoXmlObj->LoadXml(xmlpath);	
		
	VCMI1 = *CourseMainInfoXmlObj->GetPartInfo();

	std::sort(VCMI1.begin(),VCMI1.end(),compare_index_smaller);//from small to great

	return 0;
}

int DelUntil(const string StudyModeResourcePath , vector<CourseMainInfo> &VCMI1 , long Size_Limited )
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__ ,"begin Del Course");
	long fsize = 0;
	//UTIL_CountFileSize(AVT_DIRPATH, fsize );
	
	int index = 1;	
	while(fsize > Size_Limited && index< VCMI1.size() )//Delete a dir and check will make so SLOW // small BUG
	{		
		cout << "cleaning " << endl;
		char CoursePath[MAX_PATH] = {0};
		ConstructCoursePath(AVTraining, index , VCMI1[index-1].CourseName , CoursePath);//del from Level1 which the xmlfilepath is also level1
		printf("\nDelete File: %s\n", CoursePath);
		//UTIL_DeleteFile(CoursePath);//del path
		//Delete dir
		
		UTIL_CountFileSize(StudyModeResourcePath.c_str() , fsize);
		index++;
	}
}


int ConstructCoursePath(StudyMode mode , int Level ,const string CourseName, char *CoursePath )
{
	switch (mode)
	{
		case AVTraining:// STXD/x10x/
			sprintf(CoursePath , "LGSystem/STXD/第%d阶/%s",Level , CourseName.c_str());
			break;
		case ViewPicture:
			sprintf(CoursePath , "LGSystem/KTXDC/第%d阶/%s",Level , CourseName.c_str());
			break;
		default:
			printf("should not come to here\n");
			break;
	}
	printf("ConstructCoursePath : %s",CoursePath);
}
