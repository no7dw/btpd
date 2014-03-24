//#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <vector>

#include "Util.h"

#include <stdarg.h>
#include <time.h>
//#include "./torrent/bnbt.h"

// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


using namespace std;


void UTIL_LogPrint( const char *format, ... )
{
	
/*
	FILE *gpErrorLog ;
	unsigned long giErrorLogCount = 0;
	int giFlushInterval = 0;

	 
	time_t tNow = time( NULL );
	char *szTime = asctime( localtime( &tNow ) );
	szTime[strlen( szTime ) - 1] = '\0';

	//gmtxOutput.Claim( );

	printf( "[%s] ", szTime );

	va_list args;
	va_start( args, format );
	vprintf( format, args );
	va_end( args );

	if( !gstrErrorLogDir.empty( ) )
	{
		char pTime[256];
		memset( pTime, 0, sizeof( char ) * 256 );
		strftime( pTime, sizeof( char ) * 256, "%Y-%m-%d", localtime( &tNow ) );

		string strFile = gstrErrorLogDir + pTime + "e.log";

		if( gstrErrorLogFile != strFile )
		{
			// start a new log

			gstrErrorLogFile = strFile;

			if( gpErrorLog )
			{
				fclose( gpErrorLog );

				gpErrorLog = NULL;
			}

			gpErrorLog = fopen( strFile.c_str( ), "ab" );
		}

		if( gpErrorLog )
		{
			fprintf( gpErrorLog, "[%s] ", szTime );

			va_list args;
			va_start( args, format );
			vfprintf( gpErrorLog, format, args );
			va_end( args );

			giErrorLogCount++;

			if( giErrorLogCount % giFlushInterval == 0 )
				fflush( gpErrorLog );
		}
	}
*/
	//gmtxOutput.Release( );
	
	
}


string UTIL_StringToHash( const string &strString )
{
	// convert a readable string hash to a 20 character hash

	string strHash;

	if( strString.size( ) != 40 )
		return string( );

	for( unsigned long i = 0; i < strString.size( ); i += 2 )
	{
		char pBuf[4];

		memset( pBuf, 0, sizeof( char ) * 4 );

		pBuf[0] = strString[i];
		pBuf[1] = strString[i + 1];

		unsigned int c;

		sscanf( pBuf, "%02x", &c );

		strHash += c;
	}

	return strHash;
}

string UTIL_HashToString( const string &strHash )
{
	// convert a 20 character hash to a readable string

	string strString;

	if( strHash.size( ) != 20 )
		return string( );

	for( unsigned long i = 0; i < strHash.size( ); i++ )
	{
		char pBuf[4];

		memset( pBuf, 0, sizeof( char ) * 4 );

		// this must be unsigned or some really strange errors appear (i.e. the value of i is reset to zero every loop)

		unsigned char c = strHash[i];

		sprintf( pBuf, "%02x", c );

		strString += pBuf;
	}

	return strString;
}

#ifndef ONLY_XML_DIR
string UTIL_BytesToString( int64 iBytes )
{
	int iB = iBytes % 1024;
	iBytes /= 1024;
	int iKB = iBytes % 1024;
	iBytes /= 1024;
	int iMB = iBytes % 1024;
	iBytes /= 1024;
	int iGB = iBytes % 1024;
	iBytes /= 1024;
	int iTB = iBytes % 1024;
	iBytes /= 1024;
	int iPB = iBytes;

	// B -> KB -> MB -> GB -> TB -> PB -> EB -> ZB -> YB

	string strBytes;

	if( iPB > 0 )
	{
		int iFrac = (int)( (float)iTB / (float)1024 * (float)100 );

		strBytes += CAtomInt( iPB ).toString( );
		strBytes += ".";

		if( CAtomInt( iFrac ).toString( ).size( ) == 1 )
			strBytes += "0";

		strBytes += CAtomInt( iFrac ).toString( );
		strBytes += " PB";
	}
	else if( iTB > 0 )
	{
		int iFrac = (int)( (float)iGB / (float)1024 * (float)100 );

		strBytes += CAtomInt( iTB ).toString( );
		strBytes += ".";

		if( CAtomInt( iFrac ).toString( ).size( ) == 1 )
			strBytes += "0";

		strBytes += CAtomInt( iFrac ).toString( );
		strBytes += " TB";
	}
	else if( iGB > 0 )
	{
		int iFrac = (int)( (float)iMB / (float)1024 * (float)100 );

		strBytes += CAtomInt( iGB ).toString( );
		strBytes += ".";

		if( CAtomInt( iFrac ).toString( ).size( ) == 1 )
			strBytes += "0";

		strBytes += CAtomInt( iFrac ).toString( );
		strBytes += " GB";
	}
	else if( iMB > 0 )
	{
		int iFrac = (int)( (float)iKB / (float)1024 * (float)100 );

		strBytes += CAtomInt( iMB ).toString( );
		strBytes += ".";

		if( CAtomInt( iFrac ).toString( ).size( ) == 1 )
			strBytes += "0";

		strBytes += CAtomInt( iFrac ).toString( );
		strBytes += " MB";
	}
	else if( iKB > 0 )
	{
		int iFrac = (int)( (float)iB / (float)1024 * (float)100 );

		strBytes += CAtomInt( iKB ).toString( );
		strBytes += ".";

		if( CAtomInt( iFrac ).toString( ).size( ) == 1 )
			strBytes += "0";

		strBytes += CAtomInt( iFrac ).toString( );
		strBytes += " KB";
	}
	else
	{
		strBytes += CAtomInt( iB ).toString( );
		strBytes += " B";
	}

	return strBytes;
}

#endif

string UTIL_StripPath( string strPath )
{
	string :: size_type iFileStart = strPath.rfind( '\\' );

	if( iFileStart == string :: npos )
	{
		iFileStart = strPath.rfind( '/' );

		if( iFileStart == string :: npos )
			iFileStart = 0;
		else
			iFileStart++;
	}
	else
		iFileStart++;

	return strPath.substr( iFileStart );
}

#ifndef ONLY_XML_DIR
string UTIL_InfoHash( CAtom *pTorrent )
{
	if( pTorrent && pTorrent->isDicti( ) )
	{
		CAtom *pInfo = ( (CAtomDicti *)pTorrent )->getItem( "info" );

		if( pInfo && pInfo->isDicti( ) )
		{
			CAtomDicti *pInfoDicti = (CAtomDicti *)pInfo;

			// encode the string

			string strData = Encode( pInfo );

			// hash it

			CSHA1 hasher;

			hasher.Update( (unsigned char *)strData.c_str( ), strData.size( ) );
			hasher.Final( );

			char szInfoHash[64];
			memset( szInfoHash, 0, sizeof( char ) * 64 );

			hasher.ReportHash( szInfoHash );

			return UTIL_StringToHash( szInfoHash );
		}
	}

	return string( );
}
#endif

void UTIL_DeleteFile( const char *szFile )
{
	if( unlink( szFile ) == 0 )
		UTIL_LogPrint( "deleted \"%s\"\n", szFile );
	else
	{
#ifdef WIN32
		UTIL_LogPrint( "error deleting \"%s\"\n", szFile );
#else
		UTIL_LogPrint( "error deleting \"%s\" - %s\n", szFile, strerror( errno ) );
#endif
	}
}


void UTIL_MakeFile( const char *szFile, string strContents )
{
	FILE *pFile = NULL;

	if( ( pFile = fopen( szFile, "wb" ) ) == NULL )
	{
		UTIL_LogPrint( "warning - unable to open %s for writing\n", szFile );

		return;
	}

	fwrite( (void *)strContents.c_str( ), sizeof( char ), strContents.size( ), pFile );
	fclose( pFile );
}
/*
void UTIL_MakeFile( const char *szFile, string strContents, bool Cover )
{
	UTIL_DeleteFile( szFile);

	UTIL_MakeFile( szFile, strContents );
}
*/
bool UTIL_CheckFile( const char *szFile )
{
	// check if file exists

	FILE *pFile = NULL;

	if( ( pFile = fopen( szFile, "r" ) ) == NULL )
		return false;

	fclose( pFile );

	return true;
}

bool UTIL_CheckDir( const char *szDir )
{
	// check if Dir exists	
	DIR *dp;
	
	if((dp = opendir(szDir)) == NULL)
	{
		fprintf(stderr ,"cannot open dirctory : %s \n", szDir);
		return false;
	}
	return true;
}

string UTIL_ReadFile( const char *szFile )
{
	FILE *pFile = NULL;

	if( ( pFile = fopen( szFile, "rb" ) ) == NULL )
	{
		UTIL_LogPrint( "warning - unable to open %s for reading\n", szFile );

		return string( );
	}
	UTIL_LogPrint( "open %s for reading\n", szFile );

	fseek( pFile, 0, SEEK_END );
	unsigned long ulFileSize = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );
	char *pData = (char *)malloc( sizeof( char ) * ulFileSize );
	memset( pData, 0, sizeof( char ) * ulFileSize );
	fread( (void *)pData, sizeof( char ), ulFileSize, pFile );
	fclose( pFile );
	string strFile( pData, ulFileSize );
	free( pData );

	return strFile;
}

string UTIL_ReadFile( const char *szFile ,  long &lFileSize )
 {
	FILE *pFile = NULL;

	if( ( pFile = fopen( szFile, "rb" ) ) == NULL )
	{
	  return string();
	}
	fseek( pFile, 0, SEEK_END );
	long ulFileSize = ftell( pFile );
	lFileSize = ulFileSize;
	  
	return   UTIL_ReadFile( szFile );
 }
void UTIL_MoveFile( const char *szFile, const char *szDest , bool Cover )
{
	 
	if( UTIL_CheckFile( szDest ) && Cover == false )
		UTIL_LogPrint( "error archiving \"%s\" - destination file already exists\n", szDest );
	else//覆盖	
	//	UTIL_MakeFile( szDest, UTIL_ReadFile( szFile ), Cover );
		UTIL_MakeFile( szDest, UTIL_ReadFile( szFile ) );
	

	// thanks MrMister

	UTIL_DeleteFile( szFile );
}

void UTIL_CopyFile( const char *szFile, const char *szDest  , bool Cover )
{
	if( UTIL_CheckFile( szDest ) && Cover == false )
		UTIL_LogPrint( "error archiving \"%s\" - destination file already exists\n", szDest );
	else
//		UTIL_MakeFile( szDest, UTIL_ReadFile( szFile ) , Cover );
		UTIL_MakeFile( szDest, UTIL_ReadFile( szFile )  );

}


string UTIL_GetLocalTimeString()
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime ); 
	char year[5];
	char mon[3];
	char day[3];
	char hour[3];
	
	char min[3];
	char sec[3];

	strftime(year,5,"%Y",timeinfo);
	strftime(mon,3,"%m",timeinfo);
	strftime(day,3,"%d",timeinfo);
	strftime(hour,3,"%H",timeinfo);
	strftime(min,3,"%M",timeinfo);
	strftime(sec,3,"%S",timeinfo);
	 
	string timestr;
	timestr.append(year);
	timestr.append(mon);
	timestr.append(day);
	timestr.append(hour);
	timestr.append(min);
	timestr.append(sec);

	return timestr;
}

void UTIL_CreateDir(const char *DirPath)
{
#ifdef WIN32	
	CreateDirectory(DirPath,NULL);
#else
	mkdir(DirPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}


//splitString函数的参数说明：  
//返回值是分割后字符串数组大小  
//strSrc 原始字符串  
//strDelims 自定义的分割字符串的分割数组  
//strDest 分割后的字符串数组，引用传递  
int  splitString(const string & strSrc, const std::string& strDelims, vector<string>& strDest)  
{  
        typedef std::string::size_type ST;  
        string delims = strDelims;  
        std::string STR;  
        if(delims.empty()) 
			delims = "\n\r";  
      
        ST pos=0, LEN = strSrc.size();  
        while(pos < LEN )
		{
            STR="";   
            while( (delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN) ) 
				++pos;  
            if(pos==LEN) 
				return strDest.size();  
            while( (delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN) ) 
				STR += strSrc[pos++];  
            //std::cout << "[" << STR << "]";  
            if( ! STR.empty() ) 
				strDest.push_back(STR);
        }
        return strDest.size();  
} 
      

void UTIL_CountFileSize(const char *dir, long &fsize)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	
	if((dp = opendir(dir)) == NULL)
	{
		fprintf(stderr ,"cannot open dirctory : %s \n", dir);
		return ;
	}
	chdir(dir);
	while((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
			{
				continue;
			}
			else
			{
				fsize += statbuf.st_size;
				UTIL_CountFileSize(entry->d_name, fsize);
			}
		}
		fsize += statbuf.st_size;
		
	}
	chdir("..");//needed 
	closedir(dp);
}


void UTIL_Debug_Print(const char *file, int line , const char *function, const char *description ,bool bOutPut )
{
	
#ifdef BTPD_DEBUG
	if(bOutPut)
	{
		if(description)
			printf("\t @@@ \t%s\t[%s::%s()] Line_No:%d\n" ,description ,file, function ,line );
		else	
			printf("\t @@@ \t[%s::%s()] Line_No:%d\n",file, function ,line);
	}
#endif
	
}


void UTIL_CopyDir(const char *SrcDir,const char *TargetDir )
{	

	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"\n\n\n\n\n");	
	printf("copy dir : %s -> %s \n" , SrcDir, TargetDir);
	
	//return ;//2011-8-24
	DIR *dp,*dp2;
	struct dirent *entry;
	struct stat statbuf;
	
	if((dp = opendir(SrcDir)) == NULL)
	{
		fprintf(stderr ,"cannot open dirctory : %s \n", SrcDir);
		return ;
	}	
	if((dp2 = opendir(TargetDir)) == NULL)
	{
		mkdir(TargetDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//八进制600表示同样的权限
	}
	chdir(SrcDir);
	while((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
			{
				continue;
			}
			else
			{
				//new a dir
				//mkdir(entry->d_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
				//then loop
				char SubTargetDir[260]={0};
				char SubSrcDir[260]={0};

				sprintf(SubSrcDir,"%s/%s",SrcDir,entry->d_name );
				sprintf(SubTargetDir,"%s/%s",TargetDir,entry->d_name );
				printf("%s\t%s\n", SubSrcDir, SubTargetDir );
				UTIL_CopyDir(SubSrcDir,SubTargetDir );
			}
		}
		//copy file
		char SubSrcFile[260]={0};
		sprintf(SubSrcFile ,"%s/%s",SrcDir,entry->d_name );
		char SubTargetFile[260]={0};
		sprintf(SubTargetFile,"%s/%s",TargetDir,entry->d_name );

		UTIL_CopyFile(SubSrcFile,SubTargetFile );
		
	}
	chdir("..");//needed 
	closedir(dp);
}

void UTIL_String_Replace(string &srcString, const char *strfind, const char *replaceStr)
{
	string strapp;
	int pospre = 0;//index of srcString found start point
	string sub;
	for(int pos = srcString.find(strfind, pospre ) ;pos!=string::npos && pos< strlen(srcString.c_str()); )
	{		

		sub = srcString.substr( pospre , pos-pospre );//1 substr

		strapp.append(sub);// 2 append the substr

		strapp.append(replaceStr);

		pospre = pos+strlen(strfind);//jump the replaceStr
		
		pos = srcString.find(strfind, pospre ) ;		
		
	}
	if(pospre<strlen(srcString.c_str()))//still have last string
	{
		sub = srcString.substr( pospre , strlen(srcString.c_str())-pospre );//3 last substr
		strapp.append(sub);
	}
	srcString = strapp;
}

int UTIL_WriteErrUploadData(const string &strErHash, int iErrorType)
{
	printf("UTIL_WriteErrUploadData ()  Hash: %s \n", strErHash.c_str());
	//上锁	
	//记录
	//解锁
	return 0;
}

int UTIL_Read_MAX_Speed_ETH0(const char *szFile)
{
	string str = UTIL_ReadFile(szFile);
	return atoi(str.c_str());
}

/*
void UTIL_String_Replace(string &srcString, const string &strfind, const string &targetString)
{
    
        for(int pos = srcString.find(strfind.c_str());pos!=string::npos;)
        {   
                srcString.replace(pos, strlen(targetString.c_str()),targetString);
                pos = srcString.find(strfind.c_str());
        }   
    
}
*/
