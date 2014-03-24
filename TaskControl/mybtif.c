#include "btif.h"
//#include "../../../../../system/branch_src_sharedMemory_integration/Platform_Lib/libutf8/utf8_conv.h"
#include "utf8_conv.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define TorrentFilePath "/tmp/1.torrent"
#define BUFFER_SIZE 100000      //100KB




int ReadTorrentFile(const char *filepath, char *content);

int main(int argc , char** argv)
{
	printf("\nmain\n");
	if(argc<1)  
	{
		printf("Usage Operation TorrentFilePath/ID");
		return 0;
	}

        int result = -2;
	int err = -1;
	
	if(strcmp("add",argv[1]) == 0)
	{
		unsigned char buf[BUFFER_SIZE];
	//      ReadTorrentFile(TorrentFilePath,buf);
        	ReadTorrentFile(argv[2], buf);
        	result = bt_torrent_add(buf);
	}
	else
	{
	
		int id = atoi(argv[2]);
		if(strcmp("stop",argv[1]) ==0)
		{
			result = bt_torrent_pause (id);
		}
		else if(strcmp("start",argv[1]) ==0)
		{
			result = bt_torrent_start(id);
		}
		else if(strcmp("del",argv[1]) ==0)
		{
			result = bt_torrent_del_taskfile(id);
		}
		else if(strcmp("info",argv[1]) ==0)
		{
			printf("doing info ...\n ");		
			struct bt_fileinfo *pBT_FI;
			//printf("\ndir: %s ",pBT_FI->dir);
			//printf("\ncnt: %d ",pBT_FI->cnt);	
			printf("\nid : %d\n",id);
			pBT_FI =  bt_torrent_id (id , &err);
			struct bt_item *pBT_ITEM = pBT_FI ->head;
			while( pBT_ITEM != NULL)
			{
				
				printf("\nname: %s ",pBT_ITEM->name);
				printf("\nst: %d ",pBT_ITEM->st);//BUG::don't work
				printf("\ncgot : %f \t  csize : %f \t totup : %f ",pBT_ITEM->cgot, pBT_ITEM->csize, pBT_ITEM->totup);
				printf("\ndwrate: %3.2f ",pBT_ITEM->dwrate);
				printf("\nuprate: %3.2f ",pBT_ITEM->uprate);
				printf("\nnpeers: %d ",pBT_ITEM->npeers);
				pBT_ITEM = pBT_ITEM ->next;

			}
			printf("\ninfo output end");
		}
		else if(strcmp("list",argv[1]) == 0)
		{
			printf("doing list ...\n ");	
			struct bt_jobitems *pbt_jobitems;
			
			struct bt_item *pBT_ITEM ;
			for(id=-1;id<10;id++)
			{
				printf("\nid : %d",id);
				pbt_jobitems = bt_torrent_list(id);
				pBT_ITEM = pbt_jobitems ->head;
				if(pBT_ITEM == NULL) 
				{
					printf("pBT_ITEM == NULL");
					break;
				}
				printf("\nname: %s ",pBT_ITEM->name);
				printf("\nnum: %d ",pBT_ITEM->num);
				printf("\nst: %d ",pBT_ITEM->st);//BUG::don't work
				printf("\ncgot : %f \t  csize : %f \t totup : %f ",pBT_ITEM->cgot, pBT_ITEM->csize, pBT_ITEM->totup);
				printf("\ndwrate: %3.2f ",pBT_ITEM->dwrate);
				printf("\nuprate: %3.2f ",pBT_ITEM->uprate);
				printf("\nnpeers: %d ",pBT_ITEM->npeers);

				pBT_ITEM = pBT_ITEM ->next;
				

			}

		}
		else if(strcmp("kill", argv[1]) ==0)
		{
		        result = bt_torrent_kill(5);//BUG::dno't work
		}

	}
        printf("\nresult = %d\n",result);
        
	return 1;
}

int ReadTorrentFile(const char *filepath, char *content)
{
        int fileToRead;

        fileToRead = open(filepath, O_RDONLY, S_IRUSR);
        if (fileToRead<0)
        {
                printf("open file error !%s", filepath);
                exit(1);
        }

        int len=lseek(fileToRead, 0, SEEK_END);
        lseek(fileToRead, 0, SEEK_SET);
        read(fileToRead, content, len);

        close(fileToRead);
        return len;
}


