MYCXX = g++
MYAR = ar
MYRANLIB = ranlib
MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)
AR=$(MYAR) cr
RANLIB=$(MYRANLIB)
LIBNAME= ../libs/libXML.a

SRCS=tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp 
SRCS+=XXml.cpp CgiSettingXml.cpp CgiXml.cpp JICIXml.cpp BookListXml.cpp CourseMainInfoXml.cpp 

OBJS=tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o 
OBJS+=XXml.o CgiSettingXml.o CgiXml.o JICIXml.o BookListXml.o CourseMainInfoXml.o 




CFLAGS= $(INCLUDES) $(DEFINES)  $(MYDEFS) 
.cpp.o: 
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) *.o core *~

link: RM all

RM: 
	$(SHOW_COMMAND)$(rm) $(LIBNAME)
