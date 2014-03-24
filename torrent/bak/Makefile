CXX=g++
MCXX=mipsel-linux-g++

SRCS=atom.cpp bencode.cpp sha1.cpp tracker.cpp ../Util.cpp TorrentHash.cpp 
OBJS=atom.o bencode.o sha1.o tracker.o ../Util.o TorrentHash.o 

SRCS+=hashsizemain.cpp 
OBJS+=hashsizemain.o 

CFLAG=-o 
CFLAG3=-g 
PROGRAM=Hash

all:
	$(CXX) $(CFLAG3) $(SRCS) $(CFLAG) $(PROGRAM)
clean:
	rm Hash *.o -f
