#include "XXml.h"
#include <stdio.h>


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
	printf("int XXml::XXml()\n");
}
XXml::~XXml()
{
	
    delete myDocument;    
    
    delete pFirstBookElem;
    delete pLastBookElem;
    delete pRootElement;
    
    myDocument = NULL;
    pFirstBookElem = NULL; 
    pLastBookElem= NULL; 
    pRootElement= NULL;
}
int XXml::LoadXml(const char* XmlFilePath)
{
	printf("int XXml::LoadXml(const char* XmlFilePath)\n");
	if(XmlFilePath != NULL)
		strcpy(m_XmlFilePath, XmlFilePath);
    
	myDocument = NULL;
	pFirstBookElem = NULL; 
	pLastBookElem= NULL; 
	pRootElement= NULL;
}
