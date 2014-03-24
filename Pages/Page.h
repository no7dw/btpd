#ifndef __PAGE_H
#define __PAGE_H


#define PORT 80
#define BUFFER_SIZE 10000 

//#define HTML_PAGE "http://127.0.0.1/cgi-bin/UniCGI.cgi?id=7"
#define HTML_IP "127.0.0.1"
//#define HTML_IP "192.168.4.129"
#define HTML_PAGECGI "/cgi-bin/UniCGI.cgi?id=7"

//"/WebTorrentSetting.html"
//op=13 get_setting
#define HTML_PAGE_SETTING_GET "/cgi-bin/UniCGI.cgi?id=7&op=13&opid=0" 
//op=14 set_setting
#define HTML_PAGE_SETTING_SET "/cgi-bin/UniCGI.cgi?id=7&op=14&opid=0" 

#define JOB_XML_PATH "/tmp/hdd/volumes/HDD1/JobInfoCGI.xml"
#define SETTING_XML_PATH "/tmp/hdd/volumes/HDD1/SettingCGI.xml"

typedef enum{	
  PAGE_JOB_STATUS,
  PAGE_SETTING_GET,
  PAGE_SETTING_SET,
  PAGE_ADD_TORRENT
  
  
}HtmlPage;

typedef enum{
  ACTION_TYPE_ADD_TORRENT = 0,
  ACTION_TYPE_CONFIGURE_BT,
  ACTION_TYPE_GET_TASK_STATUS
  
}ActionType;

typedef enum{
  TORRENT_RESULT_UNKNOW = -1,
  TORRENT_RESULT_SUCCESS = 0,
  TORRENT_RESULT_TORRENT_EXISTS_Already,
  TORRENT_RESULT_TORRENT_ADD_FAIL
  
}TorrentResult ; 


#endif
