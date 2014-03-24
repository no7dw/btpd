#ifndef __CLIENT_H
#define __CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <iostream>
#include <vector>

using namespace std;

#define SERVER_FIFO_NAME "/tmp/server_fifo"
#define CLIENT_FIFO_NAME "/tmp/client_%d_fifo"
#define SOFT_UPDATE_FIFO_NAME "/tmp/client_soft_upgrade_fifo"
#define CLIENT_ALL_FIFO_NAME "/tmp/client_fifo"
#define WATCH_FIFO "/tmp/watch_fifo"
/*
 * the following 3 was used in client.cpp before 
 */
//#define BUFFER_SIZE PIPE_BUF
//#define MESSAGE_SIZE 20

//#define ID_SIZE 10+1
#define ID_SIZE 48+1 //由于软件升级，该为存取软件升级包的Hash
#define NAME_SIZE 256
#define PATH_SIZE 100+1
#define HASH_SIZE 48+1



#define FAIL = -1
#define SUCCEED = 0

typedef enum {
	OT_Unkown = -1,
	OT_Add = 0,
	OT_Del,
	OT_Pause,
	OT_Start,	
	OT_Book,
	OT_UnBook,
	OT_Status,
	OT_LimitSpeed,	
	OT_Event_Download_Compelete,
	OT_FeedBack,
	OT_UnBook_But_NO_Del,
	OT_OpTypeNum
	
}OpType;//kind of event

typedef struct{
	pid_t client_pid;
	OpType OT;

}MessageClient;

////////JobInfoC///////////////
struct JobInfoC{
		
		char ID[ID_SIZE];//this id is CourseID or JobID ??
		char Name[NAME_SIZE];
		double Downloaded;
		char TimeLeft[9];//99:99:99
		char Percent[7];//99.99%
		double DwRate;//kB/s
		double UpRate;//kB/s
		
		JobInfoC()
		{
		    memset(ID,0,sizeof(ID));
		    memset(Name,0,sizeof(Name));
		    memset(TimeLeft,0,sizeof(TimeLeft));
		    memset(Percent,0,sizeof(Percent));
		    Downloaded = 0;
		    DwRate = 0;
		    UpRate = 0;
		}

};

typedef struct{
	MessageClient MC;
	JobInfoC JIC;

}Message_Status;
//when download compelete write this Message_Status to client_fifo
//but the MC.OT is OT_Event_Download_Compelete

/*
struct CourseItem{
	string ID;
	string Name;	
	string Path;
	char Hash[HASH_SIZE];
	int Level;
	int Grade;
	int SendbySM;
	CourseItem()
	{
	    memset(&Hash,0,sizeof(Hash));	    
	    Level = 0;
	    Grade = 0;
	    SendbySM = 0;
	}
};
*/

////////CourseInfo///////////////
struct CourseInfo{
	
	char ID[ID_SIZE];
	char Name[NAME_SIZE];
	char Path[PATH_SIZE];
	char Hash[HASH_SIZE];
	int Level;//1-12阶段
	int Grade;//优先级别
	int SendbySM;
	CourseInfo()
	{
	    memset(ID,0,sizeof(ID));
	    memset(Name,0,sizeof(Name));
	    memset(Path,0,sizeof(Path));
	    memset(Hash,0,sizeof(Hash));	    
	    Level = 0;
	    Grade = 0;
	    SendbySM = 0;
	}
};

typedef struct{
	MessageClient MC;
	CourseInfo CI;

}Message_Book;

////////LimitInfo///////////////
typedef struct LimitInfo{
	
	char ModuleName[10];
	int DownloadSpeedLimitTo;
	int UploadSpeedLimitTo;

}LimitInfo;

typedef struct Message_LimitInfo
{
	MessageClient MC;
	LimitInfo LI;

}Message_LimitInfo;


////////ResultInfo///////////////
// typedef enum{
// 	FAIL = -1 ,
// 	SUCCEED = 0
// 	
// }Result;



typedef struct ResultInfo{
	MessageClient MC;	
	/*Result*/int result;

}ResultInfo;

typedef enum{
    
    AVTraining = 0,
    ViewPicture ,
    LyricSpeak ,
    SongSing ,
    
    StudyModeNum

}StudyMode;

////////FeedBack///////////////
typedef struct{
	int CurreentLevel;
	int UnLearnCourseCount;
	StudyMode SMode;
	
}FeedBack;

typedef struct{
	MessageClient MC;
	FeedBack FB;

}Message_FeedBack;

struct JobInfo{
		string ID;
		int Priority;
		string Partition;
		string Space;
		string Name;
		string Status;
		double Size;
		double Downloaded;
		string TimeLeft;
		string Percent;
		double DwRate;
		double UpRate;
		int Npeers;
		
		JobInfo()
		{
			ID="";
			Priority=0;
			Partition="";
			Space="";
			Name="";
			Status="";
			Size=0;
			Downloaded=0;
			TimeLeft="";
			Percent="";
			DwRate=0;
			UpRate=0;
			Npeers=0;
		}
	};
	
#endif
