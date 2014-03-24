// torrent_hash.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "atom.h"
//#include "util.h"
#include "../Util.h"
#include "bencode.h"
#include "tracker.h"


string TorrentHashString(const char *FilePath)
{
	CAtom *pTorrent = DecodeFile( FilePath );
	string strHash = UTIL_InfoHash( pTorrent );
	return UTIL_HashToString(strHash);
	
	
}

//char name[100] = {0};
//int isize = 0;
void TorrentNameSize( const char *szFile, char *RetName = NULL , int *iRetSize = NULL)
{
	CTracker *m_pTracker;
	m_pTracker = new CTracker( );
	//name //size
	
	m_pTracker->parseTorrent( szFile ,RetName ,iRetSize);
	
}

