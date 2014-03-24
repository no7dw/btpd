#ifndef __DEQUE_H
#define __DEQUE_H

#include <iostream>
#include <deque>
#include <string.h>
#include "../tinyxml/BookListXml.h"
#include "../client.h"
/*
#define QUEUEFILEPATH_1 "/tmp/hdd/volumes/HDD1/queue_1.xml"
#define QUEUEFILEPATH_2 "/tmp/hdd/volumes/HDD1/queue_2.xml"
#define QUEUEFILEPATH_3 "/tmp/hdd/volumes/HDD1/queue_3.xml"
#define QUEUEFILEPATH_4 "/tmp/hdd/volumes/HDD1/queue_4.xml"
*/


using namespace std;


typedef deque<CourseInfo> DQCI;



class Deq
{
	
private:	
	
	char m_queue_path[100];//path according to grade
	DQCI QCI;
	int m_DeqGrade;	
	
	
	BookListXml *pBookListXmlObj;

	
public:
	Deq(int grade);
	~Deq();
	
	int QueueSize;
	//@Summary: 
	//@RETURN: the pointer of first CourseItem 
	CourseInfo* GetFirstCourseInfo();
		
	
	DQCI* GetDequeCourseInfo();		
	//
	int AddItem(CourseInfo CI);	
	
	//@Summary: Remove the specify item according to CourseID
	int RemoveItem(const string &CourseID);
	
	//@Summary: Remove the first item
	//@RETURN: should we return size
	void RemoveItem();
	
	//@Summary: print out deque<CourseItem> DQCI	
	void printcontents();
	
};
#endif
