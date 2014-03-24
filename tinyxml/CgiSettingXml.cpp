#include "CgiSettingXml.h"


CgiSettingXML::CgiSettingXML()/* : XXml()*/
{
	
}
CgiSettingXML::~CgiSettingXML()/* : ~XXml()*/
{
}
/*
 * BUG::Remember the ID will change,reboot will reset
 * FIXME::
 */
int CgiSettingXML::LoadXml(const char *XmlFilePath)
{

	if(XXml::LoadXml(XmlFilePath)<0)
		return -1;
	
	TiXmlDocument *myDocument = new TiXmlDocument(XmlFilePath);
	myDocument ->LoadFile();
		
	//begin read
	pRootElement = myDocument->RootElement();//report

	TiXmlElement *OperationItem = pRootElement->FirstChildElement();//Operation

	TiXmlElement *DaemonItem = OperationItem->NextSiblingElement();
	
	DaemonStatus =	DaemonItem->FirstChild()->Value();
	cout << " DaemonStatus = "<< DaemonStatus<<endl;
	
	
	if ( DaemonItem != NULL )
	{
		TiXmlElement *uploadItem = DaemonItem->FirstChildElement();
		SI.upload = atoi(uploadItem->FirstChild()->Value());
		
		TiXmlElement *downloadItem = uploadItem->NextSiblingElement();
		SI.download = atoi( downloadItem->FirstChild()->Value());
		
		TiXmlElement *maxseedtimeItem = downloadItem->NextSiblingElement();
		SI.maxseedtime =  atoi(maxseedtimeItem->FirstChild()->Value());
		
		TiXmlElement *maxactivertrItem = maxseedtimeItem->NextSiblingElement();
		SI.maxactivertr = atoi( maxactivertrItem->FirstChild()->Value());
		
		TiXmlElement *maxinactivetmItem = maxactivertrItem->NextSiblingElement();
		SI.maxinactivetm = atoi( maxinactivetmItem->FirstChild()->Value() );
			
		TiXmlElement *autodelfinishItem = maxinactivetmItem->NextSiblingElement();
		SI.autodelfinish = atoi( autodelfinishItem->FirstChild()->Value());
		
		CurrSotrageName = DaemonItem->NextSiblingElement()->FirstChildElement()->Value();		
		
	}
// 	
	//LastStartPoint Should be compute here 
	
	delete myDocument;//2011-5-13 bug
 
	//end read
	
	
	return 1;
}

SettingInfo* CgiSettingXML::GetSettingInfo()
{
//     if( pRootElement == NULL && pLastBookElem == NULL)
//     {
// 	LoadXml();
//     }
    return &SI;
}
 