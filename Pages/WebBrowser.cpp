#include "WebBrowser.h"
#include "../Util.h"

#define HEADSIZE 1000
#define BT_SETTING_PARA_SIZE 5

TorrentResult WebBrowser::AnalyseReceiveBuffer(const string &RecvBuffer, HtmlPage htmlpage)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__);
    TorrentResult tr_result  = TORRENT_RESULT_UNKNOW;
    
    if( RecvBuffer.find(STR_OK_200)!=string::npos )//found 
    {
	switch(htmlpage)
	{
	  case PAGE_ADD_TORRENT:
	  {
	      
	      if(RecvBuffer.find("torrent add failed") !=string::npos)
	      {
		  cout <<"### OH NO~ We catch a error called : torrent add failed" << endl;
		  tr_result = TORRENT_RESULT_TORRENT_ADD_FAIL;
	      }
	      else if(RecvBuffer.find("Error, torrent already exists") !=string::npos)
	      {  
		  cout <<"### OH NO~ We catch a error called : Error, torrent already exists" << endl;
		  tr_result = TORRENT_RESULT_TORRENT_EXISTS_Already;
	      }	      
	      else if(RecvBuffer.find("HTTP-EQUIV=") !=string::npos)//HTTP-EQUIV=""REFRESH""
	      {
		  cout <<"### WOW~ We did it" << endl;
		  tr_result = TORRENT_RESULT_SUCCESS;
	      }
	      else
	      {
		  cout <<"### Oh~ We don't know the result !" << endl;
		  tr_result = TORRENT_RESULT_UNKNOW;
	      }
	  }
	    break;
	  case PAGE_SETTING_GET:
		tr_result = TORRENT_RESULT_SUCCESS;  
		break;
	   case PAGE_SETTING_SET:
		tr_result = TORRENT_RESULT_SUCCESS;  
		break;
	   case PAGE_JOB_STATUS:
		tr_result = TORRENT_RESULT_SUCCESS;  
		break;
	  default :
		tr_result = TORRENT_RESULT_UNKNOW;
		break;
	}
    }
    else//error message
    {
      
    }
    return tr_result;  
      
}

int WebBrowser::PostTorrent(const char *IPAddressOfTargetHost , const char *TorrentFilePath)
{
	
	cout << " TorrentFilePath = " << TorrentFilePath << endl;
        int sockfd;
        int len;
        struct sockaddr_in address;
        int result;

/////////////////////ReadFile/////////////////////////////////////////////
	long ulFileSize = 0;
	
	string strRet = UTIL_ReadFile( TorrentFilePath ,ulFileSize );
	printf("ulFileSize = %ld" ,ulFileSize);
	
	//cout << " strRet = " << strRet.substr( 0, 100) << endl;//just printf 100 char
	char pData[ulFileSize];
	
	memset( pData, 0 , ulFileSize );
	memcpy( pData, strRet.c_str() , ulFileSize ); 
////////////////////////////////////////////////////////////////////////
	
        char pHeadOne[HEADSIZE] = "POST /cgi-bin/UniCGI.cgi?id=7 HTTP/1.1\r\nHost: 192.168.2.11:\r\nContent-Length:";//76
        char pHeadTwo[HEADSIZE] = "\r\nContent-Type: multipart/form-data; boundary=---------------------------7db4cf4022c\r\nConnection: Keep-Alive\r\n\r\n";//112
        char pHeadThree[HEADSIZE] = "-----------------------------7db4cf4022c\r\nContent-Disposition: form-data; name=\"addf\"; filename=\"/tmp/3.torrent\"\r\nContent-Type: application/x-bittorrent\r\n\r\n";//156

        char pHeadFour[HEADSIZE] = "\r\n-----------------------------7db4cf4022c--\r\n";//46
       
        char fileLengthBuffer[10];
	
        //itoa(lenOfContent + strlen(pHeadThree) + strlen(pHeadFour),fileLengthBuffer,10); //change int to ascii char *
	memset(fileLengthBuffer,0,sizeof(char)*5);
	long fileLength = ulFileSize + strlen(pHeadThree) + strlen(pHeadFour);
	sprintf(fileLengthBuffer, "%d",fileLength);

	
        long packLength = strlen(pHeadOne) + strlen(fileLengthBuffer) + strlen(pHeadTwo) + strlen(pHeadThree) + ulFileSize + strlen(pHeadFour);
        char pSndbuf[packLength+1]; 
	//char pSndbuf[packLength]; 
        memset(pSndbuf, 0 , sizeof(char)*packLength );
	char *pS2=pSndbuf;

	memcpy(pSndbuf,pHeadOne,strlen(pHeadOne));
        memcpy(pSndbuf+strlen(pHeadOne),fileLengthBuffer,strlen(fileLengthBuffer));
        memcpy(pSndbuf+strlen(pHeadOne)+strlen(fileLengthBuffer),pHeadTwo,strlen(pHeadTwo));
        memcpy(pSndbuf+strlen(pHeadOne)+strlen(fileLengthBuffer)+strlen(pHeadTwo),pHeadThree,strlen(pHeadThree));
        
	memcpy(pSndbuf+strlen(pHeadOne)+strlen(fileLengthBuffer)+strlen(pHeadTwo)+strlen(pHeadThree),pData,ulFileSize); 
        memcpy(pSndbuf+strlen(pHeadOne)+strlen(fileLengthBuffer)+strlen(pHeadTwo)+strlen(pHeadThree)+ulFileSize,pHeadFour,strlen(pHeadFour));

	pSndbuf[packLength]='\0';	
	
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd == -1)
        {
                perror("socket");
                return -1;
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr( IPAddressOfTargetHost );
        address.sin_port = htons(PORT);

        len = sizeof(address);
        result = connect(sockfd,  (struct sockaddr *)&address, len);
        if(result == -1)
        {
                perror("connect");
		UTIL_Debug_Print(__FILE__,__LINE__,__func__,"connect error");
                return -1;
        }

        printf("connect success!\n");

        // write(sockfd,strings,strlen(strings));
        
	int totalsize;
	totalsize =strlen(pHeadOne)+strlen(fileLengthBuffer)+strlen(pHeadTwo)+strlen(pHeadThree) +ulFileSize +strlen(pHeadFour)+1;
        int sendbytes;
        if( ( sendbytes = send(sockfd, pSndbuf, totalsize, 0)) == -1)
        {
		close(sockfd);
                perror("send");
                return -1;
        }

        printf("\nsendbytes = %d\n", sendbytes);
	printf("\n=======THE FOLLOWING IS RECEIVE FROM SOCKET========\n");

	char RecvBuffer[1000];
	int iRecv = 1;
	TorrentResult TR = TORRENT_RESULT_UNKNOW;
	while( iRecv > 0 )
	{
	    iRecv = recv( sockfd, RecvBuffer, sizeof(RecvBuffer), 0);
	    if( iRecv > 0 )
	        {RecvBuffer[iRecv]=0;}
	    else
	        {RecvBuffer[0]=0;}
	    
	    printf("\nReceived   bytes:%d\n",iRecv);
	    //printf("Result:%s\n",RecvBuffer);
	    
	    //just printf 100 char
	    string strsub;
	    strsub.assign(RecvBuffer, 100);
	    printf("Result of sub 100 %s\n", strsub.c_str());
	    	    
	    //analyse the return to find out whether succeed or not
	    string strRecvBuf ;
	    strRecvBuf.assign(RecvBuffer, strlen(RecvBuffer));
	    TR = AnalyseReceiveBuffer(RecvBuffer ,PAGE_ADD_TORRENT);
	    
	    close(sockfd);
	    printf("\n==================================================\n");		        
            printf("sizeof(pSndbuf)%d\n",sizeof(pSndbuf));
	    return 0;
	}
	
        //memset(strings,0,strlen(strings));
        return 0;

}


string WebBrowser::GetHtmlPage(const char *IP, HtmlPage htmlpage, SettingInfo *pSI/*, SocketError *pSocketError*/)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"\nWEWEWE~ begin to get a Htmlpage");
	string xmlPath;
	char GetBuffer[BUFFER_SIZE]="GET ";
	switch(htmlpage)
	{
	  case PAGE_JOB_STATUS:
	  {
		xmlPath = JOB_XML_PATH;
		strcat( GetBuffer, HTML_PAGECGI);
		printf("Get %s\n", HTML_PAGECGI);
	  }
	    break;
	  case PAGE_SETTING_GET:
	  {
		xmlPath = SETTING_XML_PATH;
		strcat( GetBuffer, HTML_PAGE_SETTING_GET);
		printf("Get %s\n", HTML_PAGE_SETTING_GET);
	  }
	    break;
	  case PAGE_SETTING_SET:
	    {	  
		xmlPath = SETTING_XML_PATH;
		//sprintf should be used here
		
		strcat( GetBuffer, HTML_PAGE_SETTING_SET);		
		
		strcat( GetBuffer, "&up=");
		char cUpload[BT_SETTING_PARA_SIZE];
		sprintf(cUpload, "%d", pSI->upload);
		printf("%d", pSI->upload);		
		strcat( GetBuffer, cUpload );
		
		strcat( GetBuffer, "&down=");
		char cDownload[BT_SETTING_PARA_SIZE];
		sprintf(cDownload, "%d", pSI->download);
		printf("%d", pSI->download);		
		strcat( GetBuffer, cDownload );
				
		strcat( GetBuffer, "&seed=");
		char cMaxSeedTime[BT_SETTING_PARA_SIZE];
		sprintf(cMaxSeedTime, "%d", pSI->maxseedtime);		
		strcat( GetBuffer, cMaxSeedTime );		
		
		strcat( GetBuffer, "&idmin=1");
		
		strcat( GetBuffer, "&nact=");
		char cMaxActiverTr[BT_SETTING_PARA_SIZE];
		sprintf(cMaxActiverTr, "%d", pSI->maxactivertr);		
		strcat( GetBuffer, cMaxActiverTr );
		
		strcat( GetBuffer, "&autodel=");		
		char cAutodel[BT_SETTING_PARA_SIZE];
		sprintf(cAutodel, "%d", pSI->autodelfinish);
		strcat( GetBuffer, cAutodel );
		
		printf("Get %s\n", HTML_PAGE_SETTING_SET);		
	    }
	    break;
	    
	    
	  default :
	    return "";
	    break;
	}
	
	strcat( GetBuffer, " HTTP/1.1\r\n");
	strcat( GetBuffer, "Host: ");
	strcat( GetBuffer, IP);
	strcat( GetBuffer, ":");
	strcat( GetBuffer, "80");
	strcat( GetBuffer, "\r\n");

	strcat( GetBuffer, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
	strcat( GetBuffer, "Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7\r\n");
	strcat( GetBuffer, "Accept-Encoding: gzip,deflate\r\n");

	strcat( GetBuffer, "Connection: Close\r\n\r\n");

	//printf("\nGetBuffer : %s\n",GetBuffer);
	
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	  
	if(sockfd == -1)
	{
	    perror("socket");
	    return "ERROR_SOCKET";
	}
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(IP);
	address.sin_port = htons(PORT);	
	//address.sin_port = htonERROR_CONNECTs(PORT);
	
	int len;
	len = sizeof(address);
	
	int result;
	result = connect(sockfd,  (struct sockaddr *)&address, len);
	
	if(result == -1)
	{
	    //BUG 2 : ask too frequently ?
	    //// connect: No route to host ////
	    perror("connect :");
	    return "ERROR_CONNECT";
	}


	//write(sockfd,GetBuffer,strlen(GetBuffer));
	//int retval= send( sockfd, GetBuffer, sizeof(GetBuffer), 0);
	int retval= send( sockfd, GetBuffer, strlen(GetBuffer), 0);
	if (retval == -1)
	{
	    perror("send error\n");
	    return "ERROR_SEND";
	}
	cout << endl <<"retval = " << retval << endl;
	//printf(GetBuffer);

	int dest_file;
	//sleep(3);//BUG::seem need to wait ,this is just a trial
	dest_file = open( xmlPath.c_str(), O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH );
	if ( dest_file<0 )
	{
	      printf("open file %s error \n",  xmlPath.c_str());
	      return("ERROR_OPEN");
	}
	
	char  WriteBuffer[BUFFER_SIZE];
	while(read(sockfd, WriteBuffer,strlen(GetBuffer)))
	{
	      write(dest_file, WriteBuffer,strlen(GetBuffer));
	      /*
	      printf("\n the following is read from socket ...:\n");
	      //printf( WriteBuffer);
		string strsub;
		strsub.assign(WriteBuffer, 30);
		printf("Result of sub 30 %s\n", strsub.c_str());
		*/
	}

	close(dest_file);
	close(sockfd);
	/*printf("exiting...\n");*/
	return xmlPath;		
}

string WebBrowser::TransToXml(const char *HtmlPagePath)
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__, "TransPage to Xml begin");

	string strRet = UTIL_ReadFile(HtmlPagePath);
	if(strRet.empty())
		return "";
	
	string strXML;
	int iFoundStart = strRet.find("<?xml version");
	int iFoundEnd = strRet.find("</report>");
	if(iFoundStart !=string::npos && iFoundEnd != string::npos)
	{
		strXML = strRet.substr(iFoundStart, iFoundEnd - iFoundStart + strlen("</report>") );
		if(strXML.empty())
			return "";
	
//		std::cout<< "strXML_sub = " << strXML.substr(strlen(strXML.c_str())-10 ) <<endl;
		UTIL_MakeFile( JOB_XML_PATH , strXML);
		UTIL_Debug_Print(__FILE__,__LINE__,__func__,"TransPage to Xml makefile Compete");
		return JOB_XML_PATH;
	}
	//return "ERROR_CANNOTFIND";
	return "";
	 
}

string WebBrowser::GetJobStatus()
{
	//need add some parameters to download @ -O /tmp/xx.torrent for only it's writable
	//-o,  --output-file=FILE    log messages to FILE.
	//-O,  --output-document=FILE    write documents to FILE
	char wget_cmd[200] ={0};
	sprintf(wget_cmd,"wget http://127.0.0.1/cgi-bin/UniCGI.cgi?id=7 -O %s",JOB_STATUS_FILE);
	system(wget_cmd);
	if(UTIL_CheckFile(JOB_STATUS_FILE))			
		return JOB_STATUS_FILE;
	else	
		return "";
}