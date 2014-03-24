//
// Copyright (C) 2003 Trevor Hogan
//

// Trinity Edition Modified Source File

#ifndef BNBT_H
 #define BNBT_H

//for unix dengwei
#ifndef WIN32
	#include <stdio.h>
	#include <string.h>
#endif

#include <time.h>

#ifdef WIN32
 #define WIN32_LEAN_AND_MEAN
#endif

//
// SOLARIS USERS - IF YOUR SYSTEM IS LITTLE ENDIAN, REMOVE THE NEXT 3 LINES
//  see sha1.h
//

#if defined( __APPLE__ ) || defined( Solaris )
 #define BNBT_BIG_ENDIAN
#endif

#ifdef FreeBSD
 #include <stdio.h>
#endif

#include <iostream>

// large integers

#ifdef WIN32
 typedef __int64 int64;
 typedef unsigned __int64 uint64;
#else
 typedef long long int64;
 typedef unsigned long long uint64;
#endif

// stl

#ifdef WIN32
 #pragma warning( disable : 4786 )
#endif

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <utility>

using namespace std;

// path seperator

#ifdef WIN32
 #define PATH_SEP '\\'
#else
 #define PATH_SEP '/'
#endif

// this fixes MSVC loop scoping issues

/*

#ifdef WIN32
 #define for if( 0 ) { } else for
#endif

*/

// time stuff

unsigned long GetTime( );

#ifdef WIN32
 #define MILLISLEEP( x ) Sleep( x )
#else
 #define MILLISLEEP( x ) usleep( ( x ) * 1000 )
#endif

// mutex

#ifdef WIN32
 #include <windows.h>
 #include <process.h>
#else
 #include <pthread.h>
#endif

class CMutex
{
public:
#ifdef WIN32
	void Initialize( ) { InitializeCriticalSection( &cs ); }
	void Destroy( ) { DeleteCriticalSection( &cs ); }
	void Claim( ) { EnterCriticalSection( &cs ); }
	void Release( ) { LeaveCriticalSection( &cs ); }

	CRITICAL_SECTION cs;
#else
	void Initialize( ) { pthread_mutex_init( &mtx, NULL ); }
	void Destroy( ) { pthread_mutex_destroy( &mtx ); }
	void Claim( ) { pthread_mutex_lock( &mtx ); }
	void Release( ) { pthread_mutex_unlock( &mtx ); }

	pthread_mutex_t mtx;
#endif
};

// network

#ifdef WIN32
 #include <winsock.h>
#else
 #include <arpa/inet.h>
 #include <netdb.h>
 #include <netinet/in.h>
 #include <sys/ioctl.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <unistd.h>

 #include <errno.h>

 #define SOCKET int

 #define INVALID_SOCKET -1
 #define SOCKET_ERROR -1

 #define closesocket close

 extern int GetLastError( );
#endif

#ifdef __APPLE__
 typedef int socklen_t;
 typedef int sockopt_len_t;
#endif

#ifdef FreeBSD
 #include <sys/stat.h>
#endif

#ifdef Solaris
 #ifndef INADDR_NONE
  #define INADDR_NONE -1
 #endif
#endif

#ifndef MSG_NOSIGNAL
 #define MSG_NOSIGNAL 0
#endif

class CAtom;
class CAtomInt;
class CAtomLong;
class CAtomString;
class CAtomList;
class CAtomDicti;

class CServer;
class CTracker;
class CClient;

class CLink;
class CLinkServer;

struct request_t
{
	struct sockaddr_in sin;
	string strMethod;
	string strURL;
	map<string, string> mapParams;
	map<string, string> mapHeaders;
	map<string, string> mapCookies;
};

struct response_t
{
	string strCode;
	multimap<string, string> mapHeaders;
	string strContent;
	bool bCompressOK;
};

// current version

#define BNBT_VER "Beta 7.7"

#ifdef WIN32
// The Trinity Edition - Modification Begins
// Removes the NT Service Name Definition; Defined elsewhere
/* Original Source Code:
 #define BNBT_SERVICE_NAME "BNBT Service"
*/
// ---------------------------------- End of Modification
#endif

extern CServer *gpServer;
extern CLink *gpLink;
extern CLinkServer *gpLinkServer;
extern CMutex gmtxOutput;
extern string gstrErrorLogDir;
extern string gstrErrorLogFile;
extern FILE *gpErrorLog;
extern string gstrAccessLogDir;
extern string gstrAccessLogFile;
extern FILE *gpAccessLog;
extern unsigned long giErrorLogCount;
extern unsigned long giAccessLogCount;
extern int giFlushInterval;
extern bool gbDebug;
extern int giMaxConns;
extern string gstrStyle;
extern string gstrCharSet;
extern string gstrRealm;
// The Trinity Edition - Addition Begins
// Sets the NT Service Name variable as a global variable
extern string gstrNTServiceName;
// Sets the "bnbt_alt_signup_url" variable as a global variable
extern string gstrAltSignupURL;
// Sets the "bnbt_tracker_title" variable as a global variable
extern string gstrTrackerTitle;
// Sets the "bnbt_external_js" variable as a global variable
extern string gstrExternalJavascript;
// ----------------------------------- End of Addition

// this is basically the old main( ), but it's here to make the NT Service code neater

extern int bnbtmain( );

#endif
