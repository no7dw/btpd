 
#include "BookListXml.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

BookListXml::BookListXml()
{

}

BookListXml::~BookListXml()
{
   
}


int BookListXml::LoadXml(const char *XmlFilePath)
{
	
	if(XXml::LoadXml(XmlFilePath)<0)
		return -1;		
	
	myDocument = new TiXmlDocument(XmlFilePath);
	myDocument ->LoadFile();
	
	//begin read
	TiXmlElement *RootElement = myDocument->RootElement();//BookList
	if(RootElement == NULL)
		return -1;
	else
	    pRootElement = RootElement;
	
	TiXmlElement *FirstCourseItem = RootElement->FirstChildElement();
	pFirstBookElem = FirstCourseItem;
	pLastBookElem = pFirstBookElem;
	
	int index = 0;
	
	while ( FirstCourseItem != NULL )
	{		
		CourseInfo CI;
		TiXmlElement *FirstCourseIDItem = FirstCourseItem->FirstChildElement();
		if(FirstCourseIDItem->FirstChild())
			strcpy( CI.ID , FirstCourseIDItem->FirstChild()->Value() );
		
		TiXmlElement *FirstCourseNameItem = FirstCourseIDItem->NextSiblingElement();
		if(FirstCourseNameItem->FirstChild())
			strcpy( CI.Name , FirstCourseNameItem->FirstChild()->Value() );

		TiXmlElement *FirstCoursePathItem = FirstCourseNameItem->NextSiblingElement();
		if(FirstCoursePathItem->FirstChild())
			strcpy( CI.Path , FirstCoursePathItem->FirstChild()->Value() );		
		
		TiXmlElement *FirstCourseHashItem = FirstCoursePathItem->NextSiblingElement();
		if(FirstCourseHashItem->FirstChild())
			strcpy( CI.Hash , FirstCourseHashItem->FirstChild()->Value() );
		
		TiXmlElement *FirstCourseLevelItem = FirstCourseHashItem->NextSiblingElement();
		if(FirstCourseLevelItem->FirstChild())
			CI.Level = atoi(FirstCourseLevelItem->FirstChild()->Value());
		
		TiXmlElement *FirstCourseGradeItem = FirstCourseLevelItem->NextSiblingElement();
		if(FirstCourseGradeItem->FirstChild())
			CI.Grade = atoi(FirstCourseGradeItem->FirstChild()->Value());
		
		TiXmlElement *FirstCourseSendbySMItem = FirstCourseGradeItem->NextSiblingElement();		
		if(FirstCourseSendbySMItem->FirstChild())
			CI.SendbySM = atoi(FirstCourseSendbySMItem->FirstChild()->Value());
		

		QCI.push_back(CI);
					
		index = index + 1;
		FirstCourseItem = FirstCourseItem->NextSiblingElement();
		if(FirstCourseItem != NULL)//FIXME::need to make sure it's not NULL
		    pLastBookElem = FirstCourseItem; //BUG:: It maybe NULL
	}
	BookListSize = index;
	
	cout <<"\nindex = "<< index << "#### Loop booklist done ####" <<endl;
	
	return 0;
}


int BookListXml::AddItem(const CourseInfo &CI)
{
	
	if( pRootElement == NULL && pLastBookElem == NULL)//now we find out that no such file and we will make a new one
	{
	    InitBookList();
	    LoadXml(m_XmlFilePath);
	}
	if( pRootElement != NULL /*&& pLastBookElem != NULL*/)
	{
		if(GetCourseInfo(CI.ID) != NULL)//已经存在该预约(以防重复预约情况)
		{
			printf("we already have a book id :%s\n",CI.ID);
			return 0;
		}
		TiXmlElement *pNewElemCourse = new TiXmlElement("Course");		
		pNewElemCourse->SetAttribute("NO",BookListSize++);
		
		TiXmlElement *pNewElemCourseID = new TiXmlElement("CourseID");	
		TiXmlText *pNewTextCourseID = new TiXmlText(CI.ID);
		pNewElemCourseID->LinkEndChild(pNewTextCourseID);
		pNewElemCourse->LinkEndChild(pNewElemCourseID);
		
		TiXmlElement *pNewElemCourseName = new TiXmlElement("CourseName");	
		TiXmlText *pNewTextCourseName = new TiXmlText(CI.Name);
		pNewElemCourseName->LinkEndChild(pNewTextCourseName);
		pNewElemCourse->LinkEndChild(pNewElemCourseName);
		
		TiXmlElement *pNewElemCoursePath = new TiXmlElement("Path");	
		TiXmlText *pNewTextCoursePath = new TiXmlText(CI.Path);
		pNewElemCoursePath->LinkEndChild(pNewTextCoursePath);
		pNewElemCourse->LinkEndChild(pNewElemCoursePath);
		
		TiXmlElement *pNewElemCourseHash = new TiXmlElement("Hash");	
		TiXmlText *pNewTextCourseHash = new TiXmlText(CI.Hash);
		pNewElemCourseHash->LinkEndChild(pNewTextCourseHash);
		pNewElemCourse->LinkEndChild(pNewElemCourseHash);
		
		TiXmlElement *pNewElemCourseLevel = new TiXmlElement("Level");	
		char cLevel[3] ;
		sprintf(cLevel, "%d" , CI.Level);
		TiXmlText *pNewTextCourseLevel = new TiXmlText(cLevel);
		pNewElemCourseLevel->LinkEndChild(pNewTextCourseLevel);
		pNewElemCourse->LinkEndChild(pNewElemCourseLevel);
		
		TiXmlElement *pNewElemCourseGrade = new TiXmlElement("Grade");	
		char cGrade[3] ;
		sprintf(cGrade, "%d" , CI.Grade);
		TiXmlText *pNewTextCourseGrade = new TiXmlText(cGrade);
		pNewElemCourseGrade->LinkEndChild(pNewTextCourseGrade);
		pNewElemCourse->LinkEndChild(pNewElemCourseGrade);
		
		TiXmlElement *pNewElemSendbySM = new TiXmlElement("SendbySM");	
		char cSendbySM[3] ;
		sprintf(cSendbySM, "%d" , CI.SendbySM);
		TiXmlText *pNewTexSendbySM = new TiXmlText(cSendbySM);
		pNewElemSendbySM->LinkEndChild(pNewTexSendbySM);
		pNewElemCourse->LinkEndChild(pNewElemSendbySM);
		
		//pLastBookElem->LinkEndChild(pNewElemCourse);//adding it to the LastBookElem
		pRootElement->LinkEndChild(pNewElemCourse);//adding it to the LastBookElem
		pLastBookElem = pNewElemSendbySM;
		printf("[%s:%d]:%s()- #### - !!\n",__FILE__,__LINE__,__func__);
		myDocument->SaveFile();
		return 0;
	}
	else
	    return -1;
	
}

int BookListXml::InitBookList()
{
	TiXmlDocument xmlDoc( m_XmlFilePath /*"/tmp/1.xml"*/ );

	TiXmlDeclaration Declaration( "1.0","utf-8", "" );

	xmlDoc.InsertEndChild( Declaration );

	TiXmlElement *pRootElm = new TiXmlElement( "BookList" );
	
	pRootElm->SetAttribute("Grade",1);

	TiXmlNode *pNode = xmlDoc.InsertEndChild(*pRootElm) ;
	pRootElm = pNode->ToElement();

	//TiXmlElement *CourseElem = new TiXmlElement("Course");	
// 	TiXmlText *CourseContent = new TiXmlText(CourseName);
// 	CourseElem->LinkEndChild(NameContent);
	//pNode = pRootElm->LinkEndChild(CourseElem);
	
	xmlDoc.SaveFile();

}

//BUG:: need to remove from Vector too
int BookListXml::RemoveItem(const string &CourseID)
{
// 	if( pRootElement == NULL && pLastBookElem == NULL)
// 	{
// 	    LoadXml(m_XmlFilePath);
// 	}
// 	
	if( pFirstBookElem != NULL && pRootElement != NULL)
	{
	    TiXmlElement *FirstCourseItem = pFirstBookElem;
	    while ( FirstCourseItem != NULL )
	    {
		
		    TiXmlElement *FirstCourseIDItem = FirstCourseItem->FirstChildElement();
		    string IDElem = FirstCourseIDItem->FirstChild()->Value();
		    if( strcmp(IDElem.c_str() , CourseID.c_str()) ==0)
		    {
			pRootElement->RemoveChild(FirstCourseItem);
			myDocument->SaveFile();
			return 0;
			
		    }
		    
		    FirstCourseItem = FirstCourseItem->NextSiblingElement();
	    }//not found
	    return -1;
	     
	}
	else
	{
		return -1;
	}
}

CourseInfo* BookListXml::GetCourseInfo(const std::string& CourseID)
{
	int i = 0;
	for(DQCI::iterator diter =QCI.begin(); diter!=QCI.end(); diter++, i++)//dqueue
	{
		if(strcmp((*diter).ID ,CourseID.c_str()) == 0)
		{
			return &QCI[i];
		}
	}
	return NULL;
}

int BookListXml::GetListSize()
{
	return BookListSize;
}

DQCI* BookListXml::GetBookListQueue()
{
	return &QCI;
}