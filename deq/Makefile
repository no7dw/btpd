MYCXX = g++
MYAR = ar
MYRANLIB = ranlib
MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)
AR=$(MYAR) cr
RANLIB=$(MYRANLIB)
LIBNAME= ../libs/libDeq.a

SRCS+=deq.cpp 
 
OBJS+=deq.o


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
