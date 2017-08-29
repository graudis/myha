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

FILES		=	Config_INI.cc \
				ProcessCheck.cc \
				bnf.cc \
				cbilling.cc \
				ccenter.cc \
				ccenter_accept.cc \
				cdbworker.cc \
				cmonitor.cc \
				connmonitorservice.cc \
				connmonitorservice_accept.cc \
				cserver_session.cc \
				cserver_session_manager.cc \
				gmapsetting.cc \
				gmobdatamanager.cc \
				grandomoption.cc \
				gsystemcontrol.cc \
				instancecooltimemanager.cc \
				instancemapmanager.cc \
				listen_sessionTcp.cc \
				logsystem.cc \
				mcenteraccept.cc \
				mcenterservice.cc \
				mdbworker.cc \
				mloginaccept.cc \
				mloginservice.cc \
				mservercmd.cc \
				mservercmd_service.cc \
				mservercommandmanager.cc \
				mservermanager.cc \
				myhaMaster.cc \
				myhaSlave.cc \
				myha_main.cc \
				network_util.cc \
				random_machine.cc \
				rnSocketIOServiceTcp.cc \
				rnipdata.cc \
				rnpacket.cc \
				rnsocketiohandler.cc \
				threaddata.cc \
				timer_session.cc

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
	
