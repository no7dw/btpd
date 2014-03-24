#ifndef __UTIL_H
#define __UTIL_H

#include <string.h>
#include <iostream>
#include <vector>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef ONLY_XML_DIR
	#include "./torrent/bnbt.h"
	#include "./torrent/atom.h"
	#include "./torrent/bencode.h"
	#include "./torrent/sha1.h"
	#include "./torrent/tracker.h"
	//#include "./torrent/util.h"
#endif

using namespace std;

void UTIL_LogPrint( const char *format, ... );

string UTIL_StringToHash( const string &strString );
string UTIL_HashToString( const string &strHash );

#ifndef ONLY_XML_DIR
	string UTIL_BytesToString( int64 iBytes );
	string UTIL_InfoHash( CAtom *pTorrent );
#endif

string UTIL_StripPath( string strPath );

bool UTIL_CheckFile( const char *szFile );
void UTIL_MakeFile( const char *szFile, string strContents );
//void UTIL_MakeFile( const char *szFile, string strContents, bool Cover  = false );
void UTIL_DeleteFile( const char *szFile );
void UTIL_MoveFile( const char *szFile, const char *szDest ,bool Cover = false );
void UTIL_CopyFile( const char *szFile, const char *szDest ,bool Cover = false);
string UTIL_ReadFile( const char *szFile );
string UTIL_ReadFile( const char *szFile ,  long &lFileSize );
string UTIL_GetLocalTimeString();


void UTIL_CreateDir(const char *DirPath);
//should has no /
bool UTIL_CheckDir( const char *szDir );
void UTIL_CopyDir(const char *SrcDir,const char *TargetDir );
string UTIL_GetFilePathFromPath(const char *szFilePath);
string UTIL_GetFileNameFromPath(const char *szFilePath);
int  splitString(const string & strSrc, const std::string& strDelims, vector<string>& strDest) ;
void UTIL_CountFileSize(const char *dir, long &fsize);
void UTIL_Debug_Print(const char *file, int line , const char *function , const char *description = NULL, bool bOutPut  =true );
void UTIL_String_Replace(string &srcString, const char *strfind, const char *replaceStr);

///@Summary: 写到BT种子错误到记录中
///@Param :hash字符串，多个以|隔开
///@Param :错误类型 1 种子错误（下载不到 / 0KB）<NoTorrent/> 2 下载长时间速度为低速（或 0KB）<LowSpeed/>
///@Return :成功 0 失败-1
int UTIL_WriteErrUploadData(const string &strErHash, int iErrorType);
//void UTIL_String_Replace(string &srcString, const string &strfind, const string &targetString);

///@Summary: 返回最大的网卡流量 单位 bytes/s 
int UTIL_Read_MAX_Speed_ETH0(const char *szFile);

#endif
