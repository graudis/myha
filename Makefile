CC			=	g++

INCLUDES	= -Iinclude

CFLAGS		=	-c -Wall -g -D_DEBUG -fPIC

LNK_OPT		=

LIBS		=	-lpthread \
				-llog4cxx \
				-lboost_thread \
				-lboost_filesystem \
				-lboost_system \
				-lboost_regex \
				-lboost_date_time \
				-lboost_chrono \
				-lboost_atomic


SRCS      = $(wildcard src/*.cc) 
OBJS      = $(patsubst %.cc, %.o, $(SRCS))
DEST      = /usr/local/mysqlha
TARGET	  =	myha

.SUFFIXES : .cc .o .s 
.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

#$(OBJS) : $(SRCS)
#	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $^
    
TARGET		=	myha

all:	$(OBJS)
	$(CC) $(LNK_OPT) -o $(TARGET) $(OBJS) $(LIBS)
	rm -rf $(TARGET).gmon profile 

install : $(TARGET) 
	mkdir -p $(DEST)/bin/ 
	mkdir -p $(DEST)/cfg/
	cp -f $(TARGET) $(DEST)/bin/$(TARGET)
	cp -f script/config.ini $(DEST)/bin/config.ini
	cp -f script/cfg/log.xml $(DEST)/cfg/log.xml

clean :
	rm -rf $(OBJS) *.o  $(TARGET)

#.cc.o:
#	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

# -- will delete this line --


#clean :
#	rm -rf $(TARGET) $(OBJS) *.o 

#dep:
#	$(CC) $(INCLUDES) -M $(CFLAGS) $(FILES) > .depend 
