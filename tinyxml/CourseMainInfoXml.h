#ifndef __COURSE_MAIN_INFO_XML_H
#define __COURSE_MAIN_INFO_XML_H

// #include "tinystr.h"
// #include "tinyxml.h"
// #include "Types.h"
// 
// #include <iostream>
// #include <string.h>
// #include <vector> 
#include "XXml.h"
#include <vector>
using namespace std;

typedef vector<string> VSID;
typedef struct {
		string CourseID;
		string CourseName;
		string CourseIntro;
		int FirstScore;
		int RecentScore;
		int Flower;
		string Hash;

	}CourseMainInfo;
	
class CourseMainInfoXml : public XXml
{
public:

	CourseMainInfoXml();
	~CourseMainInfoXml();
	
private:
   
	vector<CourseMainInfo> VCourseMainInfo;
 	VSID vNewID;
  	VSID vDelID;
	vector<string> VCourseID;
	
public:

	int LoadXml(const char *XmlFilePath);
 	VSID* GetNewIDVector();
 	VSID* GetDelIDVector();

	CourseMainInfo* GetPartInfo(string id);
	vector<CourseMainInfo>* GetPartInfo();
	//vector<string>* GetCourseID();
	bool SetCourseMainInfo(string id, CourseMainInfo info);
	//bool SetAVCourseSetting(int part, AVTrainningPartSettingKind SettingKind, int value);


};
#endif
