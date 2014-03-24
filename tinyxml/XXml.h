#ifndef __X_XML_H
#define __X_XML_H

#include "tinystr.h"
#include "tinyxml.h"
#include "Types.h"

#include <iostream>
#include <string.h>
#include <vector> 
#include "../Util.h"
//template <typename T>

class XXml
{
public:
// 	XXml(const char *XmlFilePath);
	XXml();
	~XXml();
	
	//T TobjV;
	
protected:
	TiXmlDocument *myDocument;
	char m_XmlFilePath[100];
	TiXmlElement *pFirstBookElem;
	TiXmlElement *pLastBookElem;
	TiXmlElement *pRootElement;
	
	int LoadXml(const char *XmlFilePath);
	
public:

	// /*virtual */int LoadXml() = 0;
	
	//virtual T* GetInfo(string ID) = 0;
	
};
#endif