#ifndef __CGI_SETTING_XML_H
#define __CGI_SETTING_XML_H

#include "XXml.h"

using namespace std;

typedef struct {
		
		int upload;
		int download;
		int maxseedtime;//做种时间
		int maxactivertr;//最大同时激活的任务限制
		int maxinactivetm;//最大同时不激活激活的任务限制
		int autodelfinish;//bool
				
	}SettingInfo;
	
class CgiSettingXML : public XXml
{
public:
	CgiSettingXML();
	~CgiSettingXML();
private:
	string DaemonStatus;
	string CurrSotrageName;
	SettingInfo SI;
	
	
public:
	int LoadXml(const char *XmlFilePath);
	//int LoadXml(const char *XmlFilePath);
	SettingInfo* GetSettingInfo();
	
};
#endif