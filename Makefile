MYCXX = g++
MYAR = ar
MYRANLIB = ranlib
MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)
AR=$(MYAR) cr
RANLIB=$(MYRANLIB)
PROGNAME=serverM

#INCLUDES += -I./ -I./deq -I./Pages -I./TaskControl -I./timer -I./tinyxml -I./torrent

LIBS= -L./libs -lWeb -lXML -lDeq  -lTorrent -lTaskControl -lTimer

LIBS1= -L./libs -lbtif -lutf8


SRCS+=Util.cpp TransType.cpp MessageSend.cpp server.cpp 
 
OBJS+=Util.o TransType.o MessageSend.o  server.o


SUBDIRS+=deq Pages TaskControl timer tinyxml torrent 

CFLAGS+=-DBTPD_DEBUG -DTHREAD_STATUS -lpthread 

# LIBS1+=  ./libs/libbtif.a ./libs/libutf8.a 
# LIBS2+= ./libs/libDeq.a  ./libs/libTaskControl.a ./libs/libTimer.a \
# ./libs/libTorrent.a ./libs/libWeb.a ./libs/libXML.a 


#CFLAGS2 +=-L. $(LIBS1) $(LIBS2)

.cpp.o: 
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) $(CFLAGS)  $(WARNING) -c $*.cpp

$(PROGNAME) : $(OBJS)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
	${MYCXX} -o $(PROGNAME) $(CFLAGS) $(INCLUDES)  $(WARNING) $(OBJS)  $(LIBS) $(LIBS1)
	
all: $(PROGNAME)



clean:
#set -e; for i in $(SUBDIRS); do  $(rm) *.o *~  $$i; done
#$(SHOW_COMMAND)	$(rm) $(LIBS)
#$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBS) *.o core *~ serverM 
	$(SHOW_COMMAND)$(rm) $(OBJS)  *.o core *~ serverM 

link: RM all

RM: 
	$(SHOW_COMMAND)$(rm) $(LIBS)
