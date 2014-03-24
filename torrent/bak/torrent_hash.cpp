// torrent_hash.cpp : 定义控制台应用程序的入口点。
//

#include "atom.h"
//#include "util.h"
#include "../Util.h"
#include "bencode.h"
#include "tracker.h"
#include "TorrentHash.h"

int main(int argc, char* argv[])
{
	if(argc<2 ) 
	{
		printf("Usage:Hash TorrentPath\n");
		return 0;
	}

	printf(TorrentHashString(argv[1]).c_str());	
	
	//name //size
	char name[100] = {0};
	int isize = 0;
	TorrentNameSize(argv[1],name ,&isize);
	
	printf("\n after paseTorrent\n");
	printf(name);
	printf("\n%ld\n",isize);
			
	return 0;
}

