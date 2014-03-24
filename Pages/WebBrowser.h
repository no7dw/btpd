#ifndef __WEB_BROWSER_H
#define __WEB_BROWSER_H

#include "HTTP_Protocol.h"
#include "Page.h"
#include "../tinyxml/CgiSettingXml.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>
#include <iostream>
using namespace std;

#define JOB_STATUS_FILE "/tmp/bt_job_status"

class WebBrowser
{
public:
	//@Summary: according to the receive html page date contains in RecvBuffer to analyse the previous act is accepted by Host
	//@param1: html page data
	//@param2: previous ActionType
	//@RETURN: if error return error type .if not return succeed
	static TorrentResult AnalyseReceiveBuffer(const string &RecvBuffer, HtmlPage htmlpage);
    
	//@Summary: Post the .torrent file to the host
	//@param1: IP of Host
	//@param2: .torrent file path 
	//@RETURN: the post result (succeed or not)
	static int PostTorrent(const char *IPAddressOfTargetHost , const char *TorrentFilePath);
	
	static string GetHtmlPage(const char *IP, HtmlPage htmlpage, SettingInfo *pSI = NULL/*, SocketError *pSocketError = Error_No*/);

	//@Summary:according to html page data return , take </report>  as the last string transfrom the data into xml file
	//@RETURN the xml file path
	//BUG::sometime it just contains
	/*<?xml version="1.0" encoding="UTF-8" ?>
	*<report> 
	* has not </report>
	*/
	static string TransToXml(const char *HtmlPagePath);
	
	//download http://1270.0.01/cgi-bin/UniCGI.cgi?id=7
	//then return the path of the download file
	static string GetJobStatus();
	
};

#endif
