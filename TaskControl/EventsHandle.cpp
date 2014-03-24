#include "EventsHandle.h"


void Check_Event_Compelete( vector<JobInfo_CourseInfo> &v_JICI)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"begin checking Event Compelet");    
	vector<JobInfo_CourseInfo>::iterator iter;
	for(iter=v_JICI.begin();iter!=v_JICI.end();iter++)
	{		
		if((*iter).JI.Percent=="100%")
		{			
			Event_Complete((*iter).CI);
		}
	}
}


void Event_Complete(const CourseInfo &CI)
{   
	Write_100_Status( CI.ID ,CI.Name );
	Write_Finish_Course(CI, FINISH_JOBS_RECORD_FILE );	
			
	printf("\n==================Event_Complete===Begin=======================\n");
	int iOPRet;
	
	TaskControl *pTC = new TaskControl();
	
	//2.0 get the next in queue
	CourseInfo NextCourseInfo;
	
	Deq *pDeqx = new Deq(CI.Grade);//BUG: not only this Grade
	
	printf("pDeqx->QueueSize %d \t  BTSetting::Simulaneity_Task_Size %d\n",pDeqx->QueueSize,BTSetting::Simulaneity_Task_Size);
	if(pDeqx->QueueSize > BTSetting::Simulaneity_Task_Size)//若 预约下载任务数 > 允许同时下载任务数 ，则进行 移除该任务（但不删除对应文件）。
	{
		printf("complete event : remove item begin \t ID : %s\n",CI.ID); 
		iOPRet = pDeqx->RemoveItem(CI.ID);
		//this may effect m_vJI_CI //BUG do page fault
		//only remove when it's remove from BT WebHomePage ,which stands for it has uplpaded for over 48hours			
		printf("remove item compelete,result = %d\n",iOPRet);
		delete pDeqx;
		pDeqx = NULL;
		
	}
	
	if(pDeqx == NULL)
		pDeqx = new Deq(CI.Grade);
	
	//若 预约下载任务数 > 允许同时下载任务数 +最大同时不激活激活的任务限制
	//即还有任务未添加到任务中， 则进行 移除该任务（但不删除对应文件）。
	if(pDeqx->QueueSize > BTSetting::Simulaneity_Task_Size +  BTSetting::Simulaneity_Max_Inactivetm)	
	{
		pTC->Task_UnBook(CI, false);
		printf("unbook but NOT remove the Resource\n");
	}
			
	if(pDeqx->QueueSize > 0) //开启下一个未启动的任务
	{
		printf( "deq%d->QueueSize =  %d ",CI.Grade, pDeqx->QueueSize );
				    
		int index = -1;
		
		Status::GetFirstInactiveCourse(NextCourseInfo, index);
		printf("after GetFirstInactiveCourse ,NextCourseInfo.ID = %s, index : %d\n", NextCourseInfo.ID, index);
		Status::RemoveTheFinishJob(index);
		
		if(strcmp( NextCourseInfo.ID ,"") != 0)
		{
			printf("the next Course : %s\t%s\t%s \n", CI.ID, CI.Name, CI.Hash);
			//2.1 NextCourseItem.ID
						
			//2.2 start the next
			
			pTC->Task_Start(NextCourseInfo);
			printf("start the next Course ，NextCourseInfo %s , %s, %s",NextCourseInfo.ID, NextCourseInfo.Name ,  NextCourseInfo.Hash);						
		}
	}
	else//no more Job
	{
		printf( "no more Job ! Queue%d Is Empty! \n",CI.Grade);
	}
	delete pTC;
	pTC = NULL;
	delete pDeqx;
	printf("\n==================Event_Complete===End=======================\n");
	
}

int Write_100_Status(const char *CourseID, const char *CourseName )
{
 	
	//4 notify the program that the ID course has download compelete
	//4.1 compute the mode from the ID (find the BookListXml)
	//4.2 write the client_fifo that the job has been done
	    
	
	int client_fifo_fd;
	printf("\nstrlen(CourseID)== %d\n",strlen(CourseID));
	if(strlen(CourseID)==40)
	{
		client_fifo_fd = open(SOFT_UPDATE_FIFO_NAME, O_WRONLY);//软件升级包专用fifo
		printf("\n####Write_100_Status SOFT_UPDATE_FIFO_NAME : %s\t%s\n",CourseID, CourseName);
	}
	else
	{
		client_fifo_fd = open(CLIENT_ALL_FIFO_NAME, O_WRONLY);//学习模式通用fifo
		printf("\n####Write_100_Status CLIENT_ALL_FIFO_NAME : %s\t%s\n",CourseID, CourseName);
	}
	
	if(client_fifo_fd< 0)
		return -1;
	
	Message_Status MS;
	
	//construct
	JobInfoC JIC1;
	strcpy( JIC1.ID, CourseID);
	strcpy( JIC1.Name ,CourseName);
	strcpy(JIC1.Percent , "100%") ;
		
	MS.JIC = JIC1;
	MS.MC.client_pid = 0 ;
//	MS.MC.client_pid = client_pid ;/*MC1.client_pid*/
	MS.MC.OT = OT_Event_Download_Compelete;	
	write(client_fifo_fd, &MS, sizeof(Message_Status));///BUG:可能client_fifo_fd(软件升级包fifo已经unlink了)
	close(client_fifo_fd);
	printf("\n####Write_100_Status compelete");
	return 0;
    
}

int Write_Finish_Course(const CourseInfo &CI, const char *szFinishRecordFile)
{
	BookListXml *pBook = new BookListXml();
	if(pBook->LoadXml(szFinishRecordFile) >= 0)
	{
		printf("\nwe got one finished jobs to add :%s\t%s\t%s\n", CI.ID, CI.Name, CI.Hash);
		pBook->AddItem(CI);
	}
	else
	{
		return -1;
	}	
}

void Event_AllowDownload_Smaller()
{
    UTIL_Debug_Print(__FILE__,__LINE__,__func__);
    //step 1
    CgiXML *pCGIXmlObj = new CgiXML();
    if(pCGIXmlObj->LoadXml(JOB_XML_PATH) < 0)
	    return ;
    
    int LastID = pCGIXmlObj->GetTorrentCount();//should get the last downloading one
    TaskControl *pTC = new TaskControl();
    cout << "LastID = " << LastID << endl;
    pTC->Task_Pause(LastID -1);
    /*
    vector<JobInfo_Hash> VJobInfo_Hash_downloading = *(pCGIXmlObj->GetJobInfo_Hash_Downloading());
    JobInfo_Hash JIH =  VJobInfo_Hash_downloading[VJobInfo_Hash_downloading.size() - 1];
    CourseInfo CI;
    strcpy(CI.Name , JIH.JI.Name.c_str()); 
    int id = WhatsMyID(CI);
    pTC->Task_Pause(id);
    */
    delete pTC;
    pTC = NULL;
    //step 2    
    
    UTIL_Debug_Print(__FILE__,__LINE__,__func__);
}


int Event_Current_Course_Very_Limited(int CurrentLevel, int UnLearnCourseCount, StudyMode SMode)
{
	if(UnLearnCourseCount< VERY_LIMITED_OF_UNLEARN_COURSE_COUNT )
	{
		TaskControl *pTC = new TaskControl();
		//for()
		{			
			//step 1
			CourseInfo NewCI;
			//gain info 
			//....
			int oldgrade = NewCI.Grade;
			//step 2
			NewCI.Grade = LEVEL_SET_TO_WHEN_VERY_LIMITED;//reset the new level to 2			
			pTC->Task_AdjustGrade(NewCI, oldgrade);			
			//Task_AdjustGrade
		}				
		delete pTC;
		pTC = NULL;
	}
}

int Event_StudyMode_Has_Updated(int CurrentLevel,  StudyMode SMode )
{
	TaskControl *pTC= new TaskControl();
	
	for(int grade = 1; grade <= 4; grade++)
	{
		Deq *pDeqx = new Deq(grade);
		DQCI dqci = *(pDeqx->GetDequeCourseInfo());	
		DQCI::iterator diter;
		for(diter = dqci.begin(); diter != dqci.end(); diter++)
		{
			if((*diter).SendbySM == SMode)
			{
				int oldgrade = (*diter).Grade ;
				(*diter).Grade = 3;//pre course grade set to 3
				pTC->Task_AdjustGrade(*diter, oldgrade );
				printf("mode %d set a course %s %s to grade 3 \n", SMode, (*diter).ID, (*diter).Name  ) ;
			}
		}
		delete pDeqx;
		pDeqx = NULL;
	}
	
	delete pTC;
	pTC = NULL;
}

int Event_LowSpeed(const char *LowSpeedHash)
{
	return UTIL_WriteErrUploadData(LowSpeedHash , 2 );	
}

int Event_Simutane_Allow_Change(int Max_Download_Speed)
{
	int iNewSim = 4;
	
	int speed_in_hkB = (int)Max_Download_Speed/1024/8/100;  //多少 百KB/s
	printf("speed_in_kB  = %d\n ", speed_in_hkB);
	switch(speed_in_hkB)
	{
		case 0:
			return 0;//直接返回，不变
		case 1:// 100 KB/s <= Speed < 200 KB/s
			iNewSim = 1;
			break;
		case 2:// 200 KB/s <=Speed < 300 KB/s
			iNewSim = 2;
			break;
		case 3:// 300 KB/s <=Speed < 400 KB/s
			iNewSim = 3;
			break;
		default:
			return 0;
	}
	
	//改变setting
	BTSetting::LimitSimulCounts(iNewSim);
	return 0;
}