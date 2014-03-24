CXX=g++
MCXX=mipsel-linux-g++

SRCS=./tinyxml/tinystr.cpp ./tinyxml/tinyxml.cpp ./tinyxml/tinyxmlerror.cpp ./tinyxml/tinyxmlparser.cpp 
SRCS+=./tinyxml/XXml.cpp ./tinyxml/CgiSettingXml.cpp ./tinyxml/CgiXml.cpp ./tinyxml/JICIXml.cpp ./tinyxml/BookListXml.cpp ./tinyxml/CourseMainInfoXml.cpp 
#SRCS+=./Pages/GetHtmlPage.cpp ./Pages/TransToXml.cpp ./Pages/AnalyseHtml.cpp ./Pages/PostPage.cpp
SRCS+=./Pages/WebBrowser.cpp 
SRCS+=Util.cpp TransType.cpp
SRCS+=./deq/deq.cpp 

SRCS+=./TaskControl/CodesetConvert.cpp 
SRCS+=./TaskControl/TaskControl.cpp ./TaskControl/Status.cpp ./TaskControl/EventsHandle.cpp ./TaskControl/Sort.cpp ./TaskControl/BTSetting.cpp 
#SRCS+=./TaskControl/FileSort_Del.cpp 

#Hash
SRCS+=./torrent/atom.cpp ./torrent/bencode.cpp ./torrent/sha1.cpp ./torrent/tracker.cpp ./torrent/TorrentHash.cpp 

SRCS+=MessageSend.cpp 
#Timer
SRCS+=./timer/TimerFunc.cpp 


SRCS+=server.cpp 

OBJS=./tinyxml/tinystr.o ./tinyxml/tinyxml.o ./tinyxml/tinyxmlerror.o ./tinyxml/tinyxmlparser.o 
OBJS+=./tinyxml/XXml.o ./tinyxml/CgiSettingXml.o ./tinyxml/CgiXml.o ./tinyxml/JICIXml.o ./tinyxml/BookListXml.o ./tinyxml/CourseMainInfoXml.o 
#OBJS+=./Pages/GetHtmlPage.o ./Pages/TransToXml.o ./Pages/AnalyseHtml.o ./Pages/PostPage.o 
OBJS+=./Pages/WebBrowser.o 
OBJS+=Util.o TransType.o 
OBJS+=./deq/deq.o 

OBJS+=./TaskControl/CodesetConvert.o 
OBJS+=./TaskControl/TaskControl.o ./TaskControl/Status.o ./TaskControl/EventsHandle.o 
#OBJS+=./TaskControl/FileSort_Del.o 

#Hash
OBJS+=./torrent/atom.o ./torrent/bencode.o ./torrent/sha1.o ./torrent/tracker.o ./torrent/TorrentHash.o ./TaskControl/Sort.o ./TaskControl/BTSetting.o 

OBJS+=MessageSend.o  

#Timer
OBJS+=./timer/TimerFunc.o 


OBJS+=server.o

CFLAG=-o 

#CFLAG2=-I ./TaskControl/ 

CFLAG2+=-lpthread
 
#debug模式，更多输出
CFLAG2+= -DBTPD_DEBUG 
#用于进程死掉后自动重启
# CFLAG2+= -DWATCH_DOG

#检测网卡流量
#CFLAG2+= -DWATCH_TRAFFIC

#a thread to query status
CFLAG2+= -DTHREAD_STATUS

#ref TaskControl.cpp #define MIPSEL
CFLAG2+=-L. ./TaskControl/libbtif.a ./TaskControl/libutf8.a -DMIPSEL
#codeset convert
#CFLAG2+= -I ./TaskControl/ -L./TaskControl/ -liconv -DCODESET_CONVERT
#repare name 乱码
#CFLAG2+= -DREPARE_NAME

#-w  means turn off warnings
#-Wall  means allow output useful warnings
CFLAG3=-g -w 

PROGRAM=server
PROGRAMM=serverM


all:
	$(CXX) $(CFLAG3)  $(SRCS) $(CFLAG) $(PROGRAM) $(CFLAG2)
#	$(MCXX) -g  $(SRCS) $(CFLAG) $(PROGRAMM) 
#	cp $(PROGRAM) /mnt/DVD2
M:
	$(MCXX) $(CFLAG3)  $(SRCS) $(CFLAG) $(PROGRAMM) $(CFLAG2)

#	cp $(PROGRAMM) /mnt/dengwei
#	cp traff2nos.sh /mnt/dengwei

#	cp $(PROGRAMM) /mnt/DVD3
#	cp traff2nos.sh /mnt/DVD3

#	cp $(PROGRAMM) /home/dengwei/RTD1073/RTD_1073_source_code_2/image_file_avhdd_release/components/packages/package2/AP/bin/

clean:
	rm $(OBJS) -f
	rm $(PROGRAM) -f
	rm $(PROGRAMM) -f
	rm cgiXML.xml dest_file -f
	
