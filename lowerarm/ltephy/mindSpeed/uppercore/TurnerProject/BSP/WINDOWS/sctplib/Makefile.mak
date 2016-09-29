# Empty file

#
# Source files
#
CSRC = adaptation.c auxiliary.c chunkhandler.c distribution.c errorhandler.c \
	flowcontrol.c globals.c md5.c pathmanagement.c rbundling.c recvctrl.c \
	reltransfer.c sbundling.c sctp-control.c streamengine.c timer_list.c uglib.c

#
# Object files
#
OBJS = $(CSRC:.c=.obj)

#
# Libraries
#
LIBS = ws2_32.lib

all: sctp.lib

sctp.lib: $(OBJS)
	$(AR) $(OBJS) $(LIBS) $(ARFLAGS) /OUT:sctp.lib

clean:
	-$(RM) $(OBJS)
	-$(RM) sctp.lib
