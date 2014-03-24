#include "XXml.h"
#include <stdio.h>
#include "../Util.h"

using namespace std;

// XXml::XXml(const char *XmlFilePath)
// {
//     if(XmlFilePath != NULL)
// 	strcpy(m_XmlFilePath, XmlFilePath);
//     
//     myDocument = NULL;
//     pFirstBookElem = NULL; 
//     pLastBookElem= NULL; 
//     pRootElement= NULL;
// }
XXml::XXml()
{
	//printf("int XXml::XXml(%s)\n",m_XmlFilePath);
}
XXml::~XXml()
{	
	//printf("int XXml::~XXml(%s)\n",m_XmlFilePath);           
	if(myDocument!=NULL)
	{
		delete myDocument;           
		myDocument = NULL;        
	}       
    
	pFirstBookElem = NULL; 
	pLastBookElem= NULL; 
	pRootElement= NULL;		
    
}
int XXml::LoadXml(const char* XmlFilePath)
{
	printf("int XXml::LoadXml(%s)\n",XmlFilePath);
	if(XmlFilePath != NULL)
	{
		strcpy(m_XmlFilePath, XmlFilePath);
		if(UTIL_CheckFile(XmlFilePath)==false)
		{
			perror("XmlFilePath is NOT Exist!");
			return -1;
		}			
	}
	else
	{
		perror("XmlFilePath is NULL!");
		return -1;
	}
    
	myDocument = NULL;
	pFirstBookElem = NULL; 
	pLastBookElem= NULL; 
	pRootElement= NULL;
	return 0;
}
