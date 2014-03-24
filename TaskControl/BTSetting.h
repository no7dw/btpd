#ifndef __BT_SETTING_H
#define __BT_SETTING_H


class BTSetting
{
public:
	static int Simulaneity_Task_Size ;//同时下载的任务数目
	static int Simulaneity_Max_Inactivetm;//最大同时不激活激活的任务限制
public:
	//限制网速与恢复网速都是这个信息，限制网速：提供给限制的数值。 恢复网速限制： 给 0 ， 0
	//限制单个的情况：仅限制上传， 下载设 -1  ,仅限制下载， 上传设 -1  ,
	///@Summary: limit the max download speed and max upload speed
	///DownSpeed 0  means no limit , -1 means not change it
	///UpSpeed 0  means no limit , -1 means not change it
	static int LimitDownUploadSpeed(int DownSpeed, int UpSpeed);

	///@Summary: limit the download simultaneous task allowed
	static int LimitSimulCounts(int LSCount);
	
	
};
#endif
