#ifndef __BOOK_LIST_XML_H
#define __BOOK_LIST_XML_H

#include "XXml.h"

//#include "../deq/deq.h"
#include "../client.h"
#include <deque>

typedef deque<CourseInfo> DQCI;

//same BookList 3 obj
//no static
class BookListXml : public XXml
{
public:
	BookListXml();
	//BookListXml();
	~BookListXml();
private:
	DQCI QCI;
	int BookListSize;
	
private:
	
	
	int InitBookList();
	
	/*TiXmlDocument *myDocument;
	char m_XmlFilePath[100];
	TiXmlElement *pFirstBookElem, *pLastBookElem, *pRootElement;*/
	
public:
	int LoadXml(const char *XmlFilePath);
	DQCI* GetBookListQueue();
	CourseInfo *GetCourseInfo(const string &CourseID);
	int AddItem(const CourseInfo &CI);
	int RemoveItem(const string &CourseID);
	int GetListSize();
	
    
	
};

#endif