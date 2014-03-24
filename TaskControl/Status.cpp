#include "Status.h"
#include "../Pages/WebBrowser.h"
#include "string.h"

#ifdef CODESET_CONVERT
#include "./CodesetConvert.h"
#define OUTPUT_CODESET_LEN 255
#endif

//#define REPARE_NAME


//initilize
vector<JobInfoC> Status::m_vJIC_Finish;
vector<JobInfo> Status::m_vJI ;
vector<CourseInfo> Status::m_vCI;
vector<JobInfo_CourseInfo> Status::m_vJI_CI;
vector<JobInfo_CourseInfo> Status::m_vJI_CI_before_for_Check_LowSpeed;
int Status::m_totalDownloadSpeed = 0;
int Status::m_totalUploadSpeed = 0;
pthread_mutex_t Status::mtx_status;

int Status::WhatsMyJobID(CourseInfo CI)
{      
	
	UTIL_Debug_Print(__FILE__,__LINE__,__func__);
	vector<JobInfo_CourseInfo>::iterator iter;
	
	for(iter = m_vJI_CI.begin(); iter != m_vJI_CI.end(); iter++ )
	{		
		//printf("m_vJI_CI[x].CI.ID : %s \t CI.ID: %s\n",(*iter).CI.ID, CI.ID);
		if(strcmp((*iter).CI.ID ,CI.ID ) == 0 )
		{
			//find one
			int ret = atoi((*iter).JI.ID.c_str());
			printf("\nCI.ID MyJobID is : %d\n", ret);
			return ret;
			
		}
	}
	printf("find no match in WhatsMyJobID CI.ID %s \n",CI.ID);
	return -1;
}

int Status::CompareNames( char* cCourseName,  char* cJobName , int compare_length)
{
#if defined(CODESET_CONVERT)
	char outgbk[OUTPUT_CODESET_LEN]; 
	//char inputCourseName[strlen(cCourseName)];
	//strcpy(inputCourseName, cCourseName);
	CodesetConvert::u2g(cCourseName, strlen(cCourseName), outgbk, OUTPUT_CODESET_LEN);
	return strncmp(cCourseName, outgbk ,strlen(outgbk));//only compare the length  of outgbk which is regonized
#elif defined(REPARE_NAME)
	return strcmp(cCourseName, cJobName );
#else
	if(compare_length <= 0)
		return strcmp(cCourseName, cJobName );
	else		
		return strncmp(cCourseName, cJobName ,compare_length);
#endif
	
}

void Status::ReMatchVJVC(vector< JobInfo >& vJI, vector< CourseInfo >& vCI)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__);
		
	vector<JobInfo>::iterator viter;
	vector<CourseInfo>::iterator citer;
		
	JobInfo match_JI1;
	CourseInfo match_CI1;
	
	int MatchCount = 0, TotoalMatchCount = 0;
	
	printf("\nvJI size :%d \t vCI size : %d\n",vJI.size(), vCI.size());
	
	for(citer=vCI.begin(); citer!=vCI.end(); citer++)
	{
		match_CI1 = *citer;
		MatchCount = 0;
		
		for(viter=vJI.begin(); viter!=vJI.end(); viter++)
		{

			char cJobName[255] = {0};
			strcpy(cJobName, (*viter).Name.c_str());
			//if(CompareNames((*citer).Name, cJobName , 2 ) == 0)
			if(CompareNames((*citer).Name, cJobName , 0 ) == 0)
			{
				printf("strncmp match: \t%s \t %s \n", (*citer).Name, (*viter).Name.c_str());
				MatchCount ++;
				match_JI1 = *viter;//first match
				break;
			}
		}
		if(MatchCount == 1)
		{
			AMatch(match_CI1, match_JI1);
			TotoalMatchCount++;
		}
		else if(MatchCount > 1)
		{
			UTIL_Debug_Print(__FILE__,__LINE__,__func__, "MatchCount > 1");
		}
		//UTIL_Debug_Print(__FILE__,__LINE__,__func__);
	}
	
}

void Status::AMatch(const CourseInfo &match_CI1,const JobInfo &match_JI1)
{
	//write file
	printf("match CI.ID :\t%s\tJI.ID :%s\n", match_CI1.ID ,match_JI1.ID.c_str());
	JobInfo_CourseInfo JICI;
	JICI.JI = match_JI1;
	JICI.CI = match_CI1;
	m_vJI_CI.push_back(JICI);
}



int Status::AskForStatus(JobInfoC& JIC)
{	
	printf("###@@ GOT a request from client AskForStatus call PrintContents(m_vJI_CI), request from client JIC.ID : %s \n", JIC.ID);
	//PrintContents(m_vJI_CI);	
	vector<JobInfo_CourseInfo>::iterator iter;
	int index = 0;
		
	for(iter = m_vJI_CI.begin(); iter != m_vJI_CI.end(); iter++ ,index++)
	{
		//printf("###@@ m_vJI_CI[%d] info : CI.ID : %s \t JI.ID: %s \t CI.Name: %s \t JI.Percent: %s \t \n", index, m_vJI_CI[index].CI.ID, m_vJI_CI[index].JI.ID.c_str(), m_vJI_CI[index].CI.Name,  m_vJI_CI[index].JI.Percent.c_str());
		if(strcmp((*iter).CI.ID , JIC.ID ) == 0 )//JIC.ID is CousrID
		{			
			JobInfo2JobInfoC(&(*iter).JI,&JIC);
			printf("###@@ find a match JIC from client, ID : %s \n", JIC.ID);
			return 0;
		}
	}
	
	vector<JobInfoC>::iterator jic_iter;
	for(jic_iter = m_vJIC_Finish.begin(); jic_iter != m_vJIC_Finish.end(); jic_iter++)
	{
		if(strcmp((*jic_iter).ID , JIC.ID ) == 0 )//JIC.ID is CousrID
		{			
			JIC = *jic_iter;
			printf("###@@ find a match JIC (finish list), from client, ID : %s \n", JIC.ID);
			return 0;
		}
	}
	
	printf("###@@ NOT find a match JIC from client, ID : %s \n", JIC.ID);
	return -1;
}

int Status::GetAllGradeCourseInfo(vector<CourseInfo> &vCI)
{
	
	deque<CourseInfo> dqci;
	deque<CourseInfo>::iterator citer;
	//get all the queuex to a vector
	for(int grade=1;grade <= 4; grade++)
	{
		 Deq *pDeqx = new Deq(grade);
		 dqci = *(pDeqx->GetDequeCourseInfo());
		 
		 for(citer=dqci.begin(); citer!=dqci.end(); citer++)
		 {
			 //printf("push_back ..%s \n",(*citer).Name );
			 vCI.push_back(*citer);
		 }
		 //printf("\ngrade %d 's queue has parse to vCI \n", grade);
		 delete pDeqx;
		 pDeqx = NULL;
	}
	//printf("grade 1~4 's queue have parse to vCI, now vCI.size() \n", vCI.size());
}

int Status::ReMatch()
{
	
	/*string HtmlPagePath = WebBrowser::GetHtmlPage(HTML_IP, PAGE_JOB_STATUS);
	string XmlPath = WebBrowser::TransToXml(HtmlPagePath.c_str());	*/	
	string XmlPath = WebBrowser::GetJobStatus();
	
	if (XmlPath!="")
	{
		pthread_mutex_lock(&Status::mtx_status);
		
		CgiXML *pCgiXMLObj= new CgiXML();
		if(pCgiXMLObj->LoadXml(XmlPath.c_str()) < 0)
		{
			return -1;//有错或者待处理的torrent添加
		}
		
		//vJI = *pCgiXMLObj->GetJobInfo();
		
		m_vJI_CI.clear();
		printf("m_vJI_CI.clear() m_vJI_CI.size = %d\n",m_vJI_CI.size());
		m_vJI.clear();
		printf("m_vJI.clear() m_vJI.size = %d\n then call GetJobInfo()\n",m_vJI.size());
		m_vJI = *pCgiXMLObj->GetJobInfo();
		pCgiXMLObj->GetTotalSpeed( m_totalDownloadSpeed, m_totalUploadSpeed);
		printf("m_totalDownloadSpeed = %d \t m_totalUploadSpeed = %d \n",m_totalDownloadSpeed , m_totalUploadSpeed);
		
#ifdef REPARE_NAME
		RepaieJobInfoName(m_vJI);
#endif		
		
		m_vCI.clear();
		printf("m_vCI.clear() m_vCI.size = %d \nthen call GetAllGradeCourseInfo()\n",m_vCI.size());
		GetAllGradeCourseInfo(m_vCI);
		
		ReMatchVJVC(m_vJI , m_vCI);//refresh m_vJI_CI
		
		pthread_mutex_unlock(&Status::mtx_status);
		
		Check_Event_Compelete(m_vJI_CI);
		UTIL_Debug_Print(__FILE__,__LINE__,__func__,"ReMatch Compelte");
		return 0;
	}
	return -1;	
}

#ifdef REPARE_NAME
int Status::RepaieJobInfoName(vector< JobInfo >& vJI)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"RepaieJobInfoName begin");
	vector< JobInfo > tmpvJI;
	TaskControl::GetTorrentList(tmpvJI );//sorted
	if(vJI.size()>0)
	{
		int index = 0;
		
		for(vector<JobInfo>::iterator tmp_iter = tmpvJI.begin() ; tmp_iter !=tmpvJI.end() && index < vJI.size() ; tmp_iter++, index++)
		{
			printf("\n let vJI[%d].Name = %s  (*tmp_iter).ID = %s ", index, (*tmp_iter).Name.c_str(), (*tmp_iter).ID.c_str());
			vJI[index].Name = (*tmp_iter).Name;			
		}
	}
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"RepaieJobInfoName end");
}
#endif


int Status::Copy_Vector_JobInfo_CourseInfo()
{	
	//memcpy(&m_vJI_CI_before_for_Check_LowSpeed, m_vJI_CI, sizeof(JobInfo_CourseInfo)*m_vJI_CI.size());
	m_vJI_CI_before_for_Check_LowSpeed.clear();
	
	for( int index = 0; index <m_vJI_CI.size() ;  index++)
	{
		m_vJI_CI_before_for_Check_LowSpeed.push_back(m_vJI_CI[index]);
	}
	return 0;
}

vector< JobInfo_CourseInfo >* Status::GetBeforeJobInfo_CourseInfo()
{
	return &m_vJI_CI_before_for_Check_LowSpeed;
}

vector<JobInfo_CourseInfo>* Status::GetCurrentJobInfo_CourseInfo()
{
	return &m_vJI_CI;
}

vector<JobInfo>* Status::GetCurrentJobInfo()
{
	return &m_vJI;
}

void Status::PrintContents(vector<JobInfoC> &vJIC)
{
	vector<JobInfoC>::iterator iter;
	cout << endl << "(finished jobs) vJIC size = " << vJIC.size()<< endl;
	cout << "The output of vector<JobInfoC> is:"<< endl;
	for(iter = vJIC.begin(); iter != vJIC.end(); iter++ )
	{
		cout << (*iter).ID << "\t"<< (*iter).Name << "\t"<< (*iter).Percent << "\t"<< (*iter).DwRate << " KB/s"<< endl;
	}
	//printf("###leaving PrintContents(vector<JobInfoC> &vJIC)\n");
}

void Status::PrintContents(vector<JobInfo> &vJI)
{
	vector<JobInfo>::iterator iter;
	cout << endl << "(downloading jobs) vJI size = " << vJI.size()<< endl;
	cout << "The output of vector<JobInfo> is:"<< endl;
	for(iter = vJI.begin(); iter != vJI.end(); iter++ )
	{
		cout << (*iter).ID << "\t"<< (*iter).Name << "\t"<< (*iter).Percent << "\t"<< (*iter).DwRate << " KB/s"<< endl;
	}
	//printf("###leaving PrintContents(vector<JobInfo> &vJI)\n");
}

void Status::PrintContents(vector<JobInfo_CourseInfo> &vJI_CI)
{
	vector<JobInfo_CourseInfo>::iterator iter;
	cout << "(downloading jobs according to booklist) vJI_CI size = " << vJI_CI.size()<< endl;
	cout << "The output of vector<JobInfo_CourseInfo> is:"<< endl;
	for(iter = vJI_CI.begin(); iter != vJI_CI.end(); iter++ )
	{
		cout << (*iter).JI.ID << "\t"<< (*iter).JI.Name << "\t"<< (*iter).JI.Percent << "\t"<< (*iter).JI.DwRate << " KB/s"<< endl;
		cout << (*iter).CI.ID << "\t"<< (*iter).CI.Name << "\t"<< (*iter).CI.Hash << "\t"<< (*iter).CI.Path << endl;
	}	
	//printf("###leaving PrintContents(vector<JobInfo_CourseInfo> &vJI_CI)\n");
}


/* 0 定义: 长时间低速: 平均 速度 < 10KB/s ( 1小时内下不足 35MB )
 * 1 定时器（1小时）先保存此前的 vector<>结构类型 的状态信息
 * 2 然后到时间后再获取一遍数据
 * ==================================
 * 3 率选，条件： 状态是：正在下载的任务，并且进度：【0% ～ 100%】
 * 4 刷选后的任务 前后的已下载的数值相减，数值 < 35 MB (35*1024 KB)
 * 5 调用 Event_LowSpeed (记录到UploadData.xml中 )
 */
void Status::LowSpeedCheck(vector<JobInfo_CourseInfo> &m_vJI_CI_before_for_Check_LowSpeed_Old , vector<JobInfo_CourseInfo> &m_vJI_CI_before_for_Check_LowSpeed_New)
{
	if(m_vJI_CI_before_for_Check_LowSpeed_Old.size() == 0 ) //第一次执行定时器没有数据 或者 还没有任务
		return ;
	
	vector<JobInfo_CourseInfo>::iterator iter_jici_new;
	vector<JobInfo_CourseInfo>::iterator iter_jici_old;
	for(iter_jici_new = m_vJI_CI_before_for_Check_LowSpeed_New.begin(); iter_jici_new != m_vJI_CI_before_for_Check_LowSpeed_New.end(); iter_jici_new++)
	{
		for(iter_jici_old = m_vJI_CI_before_for_Check_LowSpeed_Old.begin(); iter_jici_old != m_vJI_CI_before_for_Check_LowSpeed_Old.end(); iter_jici_old++)
		{
			if( strcmp(iter_jici_new->CI.Hash , iter_jici_old->CI.Hash) == 0)
			{
				if(iter_jici_new->JI.Status=="downloading" && iter_jici_new->JI.Percent  >="0%"  && iter_jici_new->JI.Percent  <"100%")
				{
					double minus_downloaded = iter_jici_new->JI.Downloaded-iter_jici_old->JI.Downloaded ;
					printf("Hash: %s \t minus_downloaded = %f\n", iter_jici_new->CI.Hash , minus_downloaded);
					if (minus_downloaded < 35 * 1024)//低速
					{
						Event_LowSpeed(iter_jici_new->CI.Hash );
					}
				}
				break;
			}
		}
	}
}

void Status::GetFirstInactiveCourse(CourseInfo &CI, int &index)
{
	vector<JobInfo_CourseInfo>::iterator iter;
	int i = 0;
	for(iter = m_vJI_CI.begin(), i = 0; iter != m_vJI_CI.end(); iter++ ,i++)
	{
		if( strncmp( iter->JI.Status.c_str() , "inactive/paused" ,strlen("inactive/paused") ) == 0 )
		{
			CI = iter->CI;
			index = i;
			return ;
		}
	}
	
	return ;
}

void Status::GetFinisthJob( const char *szFinishRecordFile)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"going to refresh FinisthJob");
	BookListXml *pBook = new BookListXml();
	m_vJIC_Finish.clear();
	if(pBook->LoadXml(szFinishRecordFile) >= 0)
	{
		DQCI *pQlist = pBook->GetBookListQueue();
				
		for(DQCI::iterator qiter = pQlist->begin(); qiter != pQlist->end() ; qiter ++)
		{
			JobInfoC jic;
			strcpy(jic.ID , qiter->ID);
			strcpy(jic.Name , qiter->Name);
			strcpy(jic.Percent , "100%");
			printf("push_back finished item to m_vJIC_Finish : %s\t%s\n",jic.ID, jic.Name);
			m_vJIC_Finish.push_back(jic);
		}
	}
	return ;
}

void Status::RemoveTheFinishJob(int index)
{
	if(index < 0)
		return;
	int i = 0;
	for(vector<JobInfo_CourseInfo>::iterator iter = m_vJI_CI.begin(); iter!=m_vJI_CI.end(); iter++,i++)
	{
		if(i==index)
		{
			m_vJI_CI.erase(iter);
			return ;
		}
	}
}







