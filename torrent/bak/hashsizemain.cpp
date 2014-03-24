// torrent_hash.cpp : 定义控制台应用程序的入口点。
//

#include "atom.h"
//#include "util.h"
#include "../Util.h"
#include "bencode.h"
#include "tracker.h"
#include "TorrentHash.h"

 

int download(const char *TorrentHash,const char *TorrentSavePath, const char *BNBT_SERVER_IP, const char *BNBT_SERVER_PORT)//hash
{


	// example    http://192.168.2.41:6969/torrent.html?info_hash=4adc577ae4a06687ee31c70ecd3688dcc3277f9f
	// example command line wget http://192.168.2.41:6969/torrent.html?info_hash=4adc577ae4a06687ee31c70ecd3688dcc3277f9f -O /tmp/a.torrent -o log

	string strTorrentPath =  "http://";
	strTorrentPath.append(BNBT_SERVER_IP);
	strTorrentPath.append(":");
	strTorrentPath.append(BNBT_SERVER_PORT);
	
	
	strTorrentPath.append("/torrent.html?info_hash=");
	strTorrentPath.append(TorrentHash);//hash
		
	string strWgetCmd = "wget ";
	
	strWgetCmd.append(strTorrentPath);    
	strWgetCmd.append(" -O ");//-o will output to 
	
	 
	strWgetCmd.append(TorrentSavePath);    
	
	system(strWgetCmd.c_str());
	
	cout <<"call system : "<< strWgetCmd << endl;
	
	//now compute the hash to compare the download url in order to check whether system has downloaded the right one
	string strHash = TorrentHashString(TorrentSavePath);    
	if(strcmp(strHash.c_str(),TorrentHash ) != 0)
	{
		cout << "Analyse Hash: " << strHash << endl;
		cout << "Paramet Hash: " << TorrentHash << endl;
		perror("Download error Hash not equal !");
		return -1;
	}
	else
	{
		cout << "HASH EQUAL !" << endl;	
	}
		
	
	
	return 0;
}

int main(int argc, char* argv[])
{
	const char *torrent_save_path= "/tmp/up.torrent";
		
	if(argc<2 ) 
	{
		printf("Usage:Hash TorrentHash\n");		
		download("957d464049d3f8e820965ad9d028076088af8a67" ,torrent_save_path , "192.168.2.41","6969");
	}
	else
	{
		download(argv[1] ,torrent_save_path , "192.168.2.41","6969");
	}
	
	//name //size
	char name[100] = {0};
	int isize = 0;
	TorrentNameSize(torrent_save_path,name ,&isize);
	
	
	
	printf("\n after paseTorrent\n");
	printf(name);
	printf("\n%ld B\n",isize);
			
	return 0;
}

 
