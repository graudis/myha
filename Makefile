CC			=	g++

INCLUDES	=

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

FILES		=	Config_INI.cc \
				ProcessCheck.cc \
				BNF.cc \
				ccenter.cc \
				MasterClient.cc \
				cserver_session.cc \
				cserver_session_manager.cc \
				ListenSessionTcp.cc \
				LogSystem.cc \
				MasterClientAccept.cc \
				MasterClientService.cc \
				myhaMaster.cc \
				myhaSlave.cc \
				myhaMain.cc \
				network_util.cc \
				SocketIOServiceTcp.cc \
				Packet.cc \
				SocketIOHandler.cc \
				threaddata.cc \
				TimerSession.cc

OBJS		=	$(FILES:.cc=.o)

.SUFFIXES : .cc .o .s

TARGET		=	myha

all:	$(OBJS)
	$(CC) $(LNK_OPT) -o $(TARGET) $(OBJS) $(LIBS)
	rm -rf $(TARGET).gmon profile

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES)-o $@ $<

clean :
	rm -rf $(TARGET) $(OBJS) *.o
	
dep:
	$(CC) $(INCLUDES) -M $(CFLAGS) $(FILES) > .depend
