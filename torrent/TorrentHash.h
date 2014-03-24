#ifndef __TORRENT_HASH_H
#define __TORRENT_HASH_H


#include <string.h>
#include <iostream>
using namespace std;


string TorrentHashString(const char *FilePath);

void TorrentNameSize( const char *szFile, char *RetName = NULL , int *iRetSize = NULL);

#endif