#include "BTSetting.h"

#include "../Pages/WebBrowser.h"

#ifdef WATCH_TRAFFIC
	#include "../timer/TimerFunc.h"
#endif

int BTSetting::Simulaneity_Task_Size = 4;//同时下载的任务数目
int BTSetting::Simulaneity_Max_Inactivetm = 8;//最大同时不激活激活的任务限制

int BTSetting::LimitDownUploadSpeed(int DownSpeed, int UpSpeed)
{	    
	SettingInfo SI;
	
	if(UpSpeed >=0 and DownSpeed >=0 )
	{
		SI.download = DownSpeed;
		SI.upload = UpSpeed;		
	}
	
	SI.autodelfinish = 1;	
	SI.maxactivertr = 8;//最大同时激活的任务限制
	SI.maxseedtime = 48;//做种时间
	SI.maxinactivetm = 8;//最大同时不激活激活的任务限制
    
	WebBrowser::GetHtmlPage(HTML_IP, PAGE_SETTING_SET , &SI);
	return 0;
}

int BTSetting::LimitSimulCounts(int LSCount)
{
	Simulaneity_Task_Size = LSCount;
	
	SettingInfo SI;
	SI.autodelfinish = 1;    
	SI.download = 0;
	SI.upload = 0;
	SI.maxactivertr = LSCount;//最大同时激活的任务限制
	SI.maxseedtime = 48;//做种时间
	SI.maxinactivetm = 8;//最大同时不激活激活的任务限制

	WebBrowser::GetHtmlPage(HTML_IP, PAGE_SETTING_SET,&SI);
	return 0;
}
