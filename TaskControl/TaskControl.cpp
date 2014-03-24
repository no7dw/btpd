#include "TaskControl.h"
#define MIPSEL

//#include "FileSort_Del.h"
#include "Sort.h"
extern char BNBT_SERVER_IP[16];
extern char BNBT_SERVER_PORT[5];

#define Max_Jobs 20


TaskControl::TaskControl()
{

}

TaskControl::~TaskControl()
{

}

int TaskControl::Task_Pause(const CourseInfo& CI)
{
	int id = 0;
	id = Status::WhatsMyJobID(CI);
	cout <<" id = Status::WhatsMyJobID(CI) "<< CI.Hash << CI.Name << " Result id = "<< id << endl;
	if(id <0 )
		return -1;
#ifdef MIPSEL
	return bt_torrent_pause (id);
#endif
    
}

int TaskControl::Task_Pause(int id)
{
    
#ifdef MIPSEL
    return bt_torrent_pause (id);
#endif
    
}

int TaskControl::Task_Start(const CourseInfo& CI)
{
    int id = 0;
    id = Status::WhatsMyJobID(CI);
    if(id <0 )
	    return -1;
#ifdef MIPSEL
    return bt_torrent_start(id);
#endif
    
}

int TaskControl::Task_Start(int id)
{

#ifdef MIPSEL
    return bt_torrent_start(id);
#endif
    
}

int TaskControl::Task_Del(const CourseInfo& CI)
{
	int id = 0;
	id = Status::WhatsMyJobID(CI);
	if(id <0 )
		return -1;
#ifdef MIPSEL
    
	int iret = bt_torrent_del_taskfile(id);//which del the files too
	//防止未删除任务
	char cpath[255] = {0};
	sprintf(cpath, "%s%s", BT_SAVE_PATH, CI.Name);
	if(UTIL_CheckDir(cpath))
	{
		printf("after Task_Del ,%s is still exist ,not remove it\n",cpath);
		unlink(cpath);
	}
	
	return iret;
#endif
    
}

int TaskControl::Task_Del(int id)
{
#ifdef MIPSEL
    return bt_torrent_del_taskfile(id);//which del the files too
#endif
    
}

int TaskControl::Task_DelWithout(int id)
{
	if(id <0 )
		return -1;
#ifdef MIPSEL
	return bt_torrent_del(id);//without deleting files
#endif
}

int TaskControl::Task_DelWithout(const CourseInfo& CI)
{
    int id = 0;
    id = Status::WhatsMyJobID(CI);
#ifdef MIPSEL
    return bt_torrent_del(id);//without deleting files
#endif
    
}

int TaskControl::Task_Kill(int sec)
{
#ifdef MIPSEL
    return bt_torrent_kill(5);//BUG::dno't work
#endif
    
}

int TaskControl::Task_Add(const char *TorrentHash)//hash
{
    //first step : wget http://IP:port/torrent?=....hash
    //BUG ::how to know the wget error
    //FIXME:: -o -O

// example    http://192.168.2.41:6969/torrent.html?info_hash=4adc577ae4a06687ee31c70ecd3688dcc3277f9f
// example command line wget http://192.168.2.41:6969/torrent.html?info_hash=4adc577ae4a06687ee31c70ecd3688dcc3277f9f -O /tmp/a.torrent -o log

	if(strcmp(TorrentHash,"")==0)//检查输入
		return -1;

	//构造下载命令
	string strTorrentPath =  "http://";
	strTorrentPath.append(BNBT_SERVER_IP);
	strTorrentPath.append(":");
	strTorrentPath.append(BNBT_SERVER_PORT);
	
	
	strTorrentPath.append("/torrent.html?info_hash=");
	strTorrentPath.append(TorrentHash);//hash
		
	string strWgetCmd = "wget ";
	//need add some parameters to download @ -O /tmp/xx.torrent for only it's writable
	//-o,  --output-file=FILE    log messages to FILE.
	//-O,  --output-document=FILE    write documents to FILE
	strWgetCmd.append(strTorrentPath);    
	strWgetCmd.append(" -O ");//-o will output to 
	
	string objTorrentPath = TORRENT_SAVE_PATH;
	strWgetCmd.append(objTorrentPath);    
	
	system(strWgetCmd.c_str());
	
	cout <<"call system : "<< strWgetCmd << endl;
	
	//now compute the hash to compare the download url in order to check whether system has downloaded the right one
	string strHash = TorrentHashString(objTorrentPath.c_str());    
	if(strcmp(strHash.c_str(),TorrentHash ) != 0)
	{
		//计算种子文件大小，如果是0kb 证明没有该hash的种子
		cout << "Analyse Hash: " << strHash << endl;
		cout << "Param Hash: " << TorrentHash << endl;
		if(strcmp( strHash.c_str() , "" ) != 0 )// Hash不为空
		{
			UTIL_WriteErrUploadData(strHash,1);
		}
		
		perror("Download error Hash not equal !");
		return -1;
	}
	else
	{
		cout << "HASH EQUAL !" << endl;	
	}
		
	//Post a Torrent
	int iOPRet = WebBrowser::PostTorrent(HTML_IP , objTorrentPath.c_str() );
	UTIL_Debug_Print(__FILE__,__LINE__,__func__ ,"after PostTorrent");
	return iOPRet;
}

int TaskControl::Task_Book(const CourseInfo& CI)
{
    return Task_BookUnBook(CI, true);
}

int TaskControl::Task_UnBook(const CourseInfo& CI, bool blnDelResource)
{
    return Task_BookUnBook(CI, false, true, blnDelResource);
}

int TaskControl::Task_BookUnBook(const CourseInfo &CI, bool blnBook, bool blnNeedAddTorrent ,bool blnDelTask, bool blnDelResource)
{    
	UTIL_Debug_Print(__FILE__,__LINE__,__func__);
	
	int grade = CI.Grade;        
	int iOPRet;
	Deq *pDeqx = new Deq(grade);
	if(blnBook)
		iOPRet = pDeqx->AddItem(CI);//which load xml
	else
		iOPRet = pDeqx->RemoveItem(CI.ID);//which load xml
	    
   
	if (iOPRet>=0 )
	{
		if(blnBook)//Book
		{	    	    
			if(blnNeedAddTorrent)//2 call AddTorrent
			{			 
				CheckDisk((StudyMode)CI.SendbySM);	//find out whether hard disk has enough space	
				if(Task_Add(CI.Hash) < 0 )
				{
					//BUG
					pDeqx->RemoveItem(CI.ID);//remove the one just add because we add fail
					return -1;
				}
			}		    	    
			//3 call Pause when Q.size() > SIMULANEITY_TASK_SIZE
			//cout << "Deq::QueueAllSize = " << Deq::QueueAllSize << endl;
			
			CgiXML *pCGIXmlObj = new CgiXML();
			
			if(pCGIXmlObj->LoadXml(JOB_XML_PATH) < 0)
				return -1;
			
			int downloadingCount  = pCGIXmlObj->GetJobInfo_Hash_Downloading()->size();
			printf( "downloadingCount : %d \n",downloadingCount );
			if( downloadingCount > BTSetting::Simulaneity_Task_Size )		
			{		    
				Task_Pause(CI);
				cout << "calling Pause CI.Hash "<< CI.Hash << endl;
			}
			
			return 0;
		}
		else if(blnDelTask)//need to del the task ?
		{
			if(blnDelResource)
			{				
				Task_Del(CI);//Del the task and the resource 
				//Erase it from queue_x.xml  RemoveItem this has been done upper switch case 
			}
			else
			{
				Task_DelWithout(CI);//Del the task but NOT the resource 
			}
		}
		return 0;
			
	}    
	else
		return -1;
}

//1 add to new ToGrade ---Book
//2 remove from old ToGrade ---UnBook
int TaskControl::Task_AdjustGrade(const CourseInfo& CI, int OldGrade)
{    
    CourseInfo OldCI;
    memcpy(&OldCI, &CI, sizeof(CourseInfo));
    OldCI.Grade = OldGrade;
    Task_BookUnBook(CI, true, false, false);//BUG::this will call Add //FIXME:: add a parameter to decide whethe to call Add
    Task_BookUnBook(OldCI, false, false, false);
    return 0;
}


int TaskControl::GetTorrentList(vector< JobInfo >& vJobInfoListU)
{
	
	int id = 0;
	printf("doing list ...\n ");	
	struct bt_jobitems *pbt_jobitems;
	
	struct bt_item *pBT_ITEM ;
	for( id = 0 ; id < Max_Jobs ; id++ )
	{
		JobInfo JI;
		printf("\nbt_torrent_list( %d): ",id);		
		pbt_jobitems = bt_torrent_list(id);		
		pBT_ITEM = pbt_jobitems ->head;
		   
		if(pBT_ITEM == NULL) 
		{
			printf("pBT_ITEM == NULL");
			break;
		}
		
		char cID[3] = {0};
		sprintf(cID, "%d",pBT_ITEM->num);
		JI.ID = cID;
		JI.Name = pBT_ITEM->name;
		//JI.Status = pBT_ITEM->st;
		JI.DwRate = pBT_ITEM->dwrate;
		JI.UpRate = pBT_ITEM->uprate;
		JI.Npeers = pBT_ITEM->npeers;
		JI.UpRate = pBT_ITEM->uprate;
		JI.Downloaded = pBT_ITEM->cgot;
		JI.Size = pBT_ITEM->csize;
		  
		printf("\tnum: %d \t name: %s \t  dwrate: %3.2f \tuprate: %3.2f \t st: %d \tcgot : %f \t  csize : %f \t totup : %f \tnpeers: %d " ,
		       pBT_ITEM->num, pBT_ITEM->name,pBT_ITEM->dwrate,pBT_ITEM->uprate,pBT_ITEM->st,pBT_ITEM->cgot, pBT_ITEM->csize, pBT_ITEM->totup ,pBT_ITEM->npeers);
		
		//BUG::pBT_ITEM->st don't work
		
		vJobInfoListU.push_back(JI);
		pBT_ITEM = pBT_ITEM ->next;
		if(pBT_ITEM == NULL) 
		{
			printf("pBT_ITEM == NULL");
			break;
		}
		
	}
	
	 
	std::sort(vJobInfoListU.begin(),vJobInfoListU.end(),compare_index_smaller_vJILU);//from small to great
	Status::PrintContents(vJobInfoListU);
}

int TaskControl::compare_index_smaller_vJILU(const JobInfo& j1, const JobInfo& j2)
{
	//UTIL_Debug_Print(__FILE__,__LINE__,__func__,"begin sort");
	//printf("\nj2.Name = %s\t j1.Name = %s\n",j1.Name.c_str(), j1.Name.c_str());
	int iresult =  strcmp(j2.Name.c_str() , j1.Name.c_str());
	
	if(iresult>=0)
		return true;
	else
		return false;
}

