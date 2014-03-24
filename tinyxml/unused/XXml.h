#ifndef __X_XML_H
#define __X_XML_H

#include "tinystr.h"
#include "tinyxml.h"
#include "Types.h"

#include <iostream>
#include <string.h>
#include <vector> 

//template <typename T>

class XXml
{
public:

	XXml();
	~XXml();

protected:
	TiXmlDocument *myDocument;
	char m_XmlFilePath[100];
	TiXmlElement *pFirstBookElem, *pLastBookElem, *pRootElement;
	
	int LoadXml(const char *XmlFilePath);
	
public:

	// /*virtual */int LoadXml() = 0;
	
	//virtual T* GetInfo(string ID) = 0;
	
};
#endif