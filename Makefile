CC			=	g++

INCLUDES	=	-I/usr/include
INCLUDES	=

CFLAGS		=	-c -Wall -g -D_DEBUG -fPIC -DBOOST_LOG_DYN_LINK

LNK_OPT		=	-static
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

FILES		=	BNF.cc
			Config_INI.cc
			ListenSessionTcp.cc
			LogSystem.cc
			MasterClient.cc
			MasterClientAccept.cc
			MasterClientService.cc
			Packet.cc
			ProcessCheck.cc
			SocketIOHandler.cc
			SocketIOServiceTcp.cc
			TimerSession.cc
			ccenter.cc
			cserver_session.cc
			cserver_session_manager.cc
			myhaMain.cc
			myhaMaster.cc
			myhaSlave.cc
			network_util.cc
			threaddata.cc


OBJS		=	$(FILES:.cc=.o)

.SUFFIXES : .cc .o .s

TARGET		=	myha

all:	$(OBJS)
	$(CC) $(LNK_OPT) -o $(TARGET) $(OBJS) $(LIBS)
	rm -rf $(TARGET).gmon profile

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES)-o $@ $<

clean :
	rm -rf $(TARGET) $(OBJS)
	
