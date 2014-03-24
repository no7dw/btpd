#ifndef __STUDY_RECORD_XML_H
#define __STUDY_RECORD_XML_H

// #include "tinystr.h"
// #include "tinyxml.h"
// #include "Types.h"
// 
// #include <iostream>
// #include <string.h>
// #include <vector> 

#include "XXml.h"

using namespace std;

typedef struct {
		string StudyRecordInfoID;
		string CourseID;
		int Study_Interval;
		int Study_Score;
		string Study_Time;/*time_t*/
		int Study_PersonCount;
		int PersonA_Score;
		int PersonB_Score;
		int PersonC_Score;
		int PersonD_Score;
	}StudyRecordInfo;
	

class StudyRecordXml: public XXml
{
public:

	StudyRecordXml();
	~StudyRecordXml();

private:
	vector <StudyRecordInfo> VStudyRecordInfoArray;

public:

	int LoadXml(const char *XmlFilePath);

	StudyRecordInfo* GetStudyRecordInfo(string id);//It shoule by <T>
	vector <StudyRecordInfo>* GetStudyRecordInfo(); 
	
	bool SetStudyRecordInfo(string id, StudyRecordInfo info);
	//bool SetAVCourseSetting(int part, AVTrainningPartSettingKind SettingKind, int value);


};
#endif