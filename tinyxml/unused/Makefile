CXX=g++
SRCS=tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp XXml.cpp ../Util.cpp 

SRCSVersion+=$(SRCS)
SRCSVersion+=VersionXml.cpp VersionXmlMain.cpp

SRCSSetting+=$(SRCS)
SRCSSetting+=SettingXml.cpp SettingXmlMain.cpp

SRCSPartsInfo+=$(SRCS)
SRCSPartsInfo+=PartsInfoXml.cpp PartsInfoXmlMain.cpp

SRCSCourseMainInfo+=$(SRCS)
SRCSCourseMainInfo+=CourseMainInfoXml.cpp CourseMainInfoXmlMain.cpp

SRCSStudyRecord+=$(SRCS)
SRCSStudyRecord+=StudyRecordXml.cpp StudyRecordXmlMain.cpp

SRCSCGI+=$(SRCS)
SRCSCGI+=CgiXml.cpp CgiXmlMain.cpp

SRCSJICI+=$(SRCS)
SRCSJICI+=CgiXml.cpp JICIXml.cpp JICIXmlMain.cpp

SRCSCGISETTING+=$(SRCS)
SRCSCGISETTING+=CgiSettingXml.cpp CgiSettingXmlMain.cpp

SRCSBOOK+=$(SRCS)
SRCSBOOK+=BookListXml.cpp BookListXmlMain.cpp



OBJS=tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o XXml.o Util.o 
OBJSVersion+=$(OBJS)
OBJSVersion+=VersionXml.o VersionXmlMain.o

OBJSSetting+=$(OBJS)
OBJSSetting+=SettingXml.o SettingXmlMain.o

OBJSPartsInfo+=$(OBJS)
OBJSPartsInfo+=PartsInfoXml.o PartsInfoXmlMain.o

OBJSCourseMainInfo+=$(OBJS)
OBJSCourseMainInfo+=CourseMainInfoXml.o CourseMainInfoXmlMain.o

OBJSStudyRecord+=$(OBJS)
OBJSStudyRecord+=StudyRecordXml.o StudyRecordXmlMain.o

OBJSCGI+=$(OBJS)
OBJSCGI+=CgiXml.o CgiXmlMain.o

OBJSJICI+=$(SRCS)
OBJSJICI+=CgiXml.o  JICIXml.o JICIXmlMain.o 

OBJSCGISetting+=$(OBJS)
OBJSCGISetting+=CgiSettingXml.o CgiSettingXmlMain.o

OBJSBOOK+=$(OBJS)
OBJSBOOK+=BookListXml.o BookListXmlMain.o


PROGRAMVersion=Version

PROGRAMSetting=Setting

PROGRAMPartsInfo=PartsInfo

PROGRAMCourseMainInfo=CourseMainInfo

PROGRAMStudyRecord=StudyRecord

PROGRAMCGI=CGI

PROGRAMJICI=JICI

PROGRAMCGISETTING=CGISetting

PROGRAMBOOK=Book


CFLAG=-o 
CFLAG2=-DONLY_XML_DIR 

all:
#	$(CXX) -g  $(SRCSVersion) $(CFLAG) $(PROGRAMVersion) 
#	$(CXX) -g  $(SRCSSetting) $(CFLAG) $(PROGRAMSetting) 
#	$(CXX) -g  $(SRCSPartsInfo) $(CFLAG) $(PROGRAMPartsInfo) 
	$(CXX) -g  $(SRCSCourseMainInfo) $(CFLAG) $(PROGRAMCourseMainInfo)  $(CFLAG2)
#	$(CXX) -g  $(SRCSStudyRecord) $(CFLAG) $(PROGRAMStudyRecord)
	$(CXX) -g  $(SRCSCGI) $(CFLAG) $(PROGRAMCGI)  $(CFLAG2)
	$(CXX) -g  $(SRCSCGISETTING) $(CFLAG) $(PROGRAMCGISETTING)  $(CFLAG2)
	$(CXX) -g  $(SRCSBOOK) $(CFLAG) $(PROGRAMBOOK)      $(CFLAG2)
	$(CXX) -g  $(SRCSJICI) $(CFLAG) $(PROGRAMJICI)   $(CFLAG2)

SR:
	$(CXX) -g  $(SRCSStudyRecord) $(CFLAG) $(PROGRAMStudyRecord)   $(CFLAG2)
ST:
	$(CXX) -g  $(SRCSSetting) $(CFLAG) $(PROGRAMSetting)   $(CFLAG2)
CM:
	$(CXX) -g  $(SRCSCourseMainInfo) $(CFLAG) $(PROGRAMCourseMainInfo)   $(CFLAG2)
PI:
	$(CXX) -g  $(SRCSPartsInfo) $(CFLAG) $(PROGRAMPartsInfo)   $(CFLAG2)
CGI:
	$(CXX) -g  $(SRCSCGI) $(CFLAG) $(PROGRAMCGI)   $(CFLAG2)
JICI:
	$(CXX) -g  $(SRCSJICI) $(CFLAG) $(PROGRAMJICI)   $(CFLAG2)

CGISet:
	$(CXX) -g  $(SRCSCGISETTING) $(CFLAG) $(PROGRAMCGISETTING)   $(CFLAG2)
BOOK:
	$(CXX) -g  $(SRCSBOOK) $(CFLAG) $(PROGRAMBOOK)      $(CFLAG2)
 

clean:
	rm $(OBJS) -f
	rm $(OBJSVersion) -f 
	rm $(OBJSSetting) -f
	rm $(OBJSPartsInfo)  -f
	rm $(OBJSCourseMainInfo) -f
	rm $(OBJSStudyRecord)  -f
	rm $(OBJSBOOK)  -f
	rm $(OBJSNot)  -f
	rm $(OBJSJICI)  -f

	rm $(PROGRAMVersion) -f 
	rm $(PROGRAMSetting) -f
	rm $(PROGRAMPartsInfo)  -f
	rm $(PROGRAMCourseMainInfo) -f
	rm $(PROGRAMStudyRecord)  -f
	rm $(PROGRAMCGI)  -f
	rm $(PROGRAMCGISETTING)  -f
	rm $(PROGRAMBOOK)  -f
	rm $(PROGRAMNot)  -f
	rm $(PROGRAMJICI)  -f
	rm *~ -f
