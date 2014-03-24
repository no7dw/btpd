#ifndef __SORT_H
#define __SORT_H

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <algorithm>

#include "../tinyxml/CourseMainInfoXml.h"
#include "../Util.h"
#include "../client.h"

//#define COURSE_INFO_PATH "/tmp/hdd/volumes/HDD1/LGSystem/STXD/CourseInfo7.xml"
#define AVT_DIRPATH "/tmp/hdd/volumes/HDD1/LGSystem/STXD/" //视听选答的资源的磁盘路径
#define VP_DIRPATH "/tmp/hdd/volumes/HDD1/LGSystem/KTXDC/"//看图学单词的资源的磁盘路径
#define LS_DIRPATH "/tmp/hdd/volumes/HDD1/LGSystem/SQLD/"//抒情朗读的资源的磁盘路径

//50G
//#define MAX_CAPABILITY_EACH_STYLE 50*1024*1024*1024
#define MAX_CAPABILITY_EACH_STYLE 100*1024*1024

//#include "./FileSort_Del.h"

int CheckDisk(StudyMode mode);

bool compare_index_smaller(const CourseMainInfo &t1,const CourseMainInfo &t2);//from smaller to greater
bool compare_index_greater(const CourseMainInfo &t1,const CourseMainInfo &t2);//from greater to smaller

void printout(vector<CourseMainInfo> &VCMI);

///@Summary sort the vector<CourseMainInfo> in param  
///@param 1 filePath
///@param 2 vector<CourseMainInfo> which will change ,not const type
///@Return succced or not
int SortCourse(const char *xmlpath, vector<CourseMainInfo> &VCMI1)  ;

int DelUntil(const string StudyModeResourcePath , vector<CourseMainInfo> &VCMI1 , long Size_Limited );

int ConstructCoursePath(StudyMode mode , int Level ,const string CourseName, char *CoursePath );



#endif 

