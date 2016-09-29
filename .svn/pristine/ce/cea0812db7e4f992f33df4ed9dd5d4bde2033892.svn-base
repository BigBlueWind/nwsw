CROSS_COMPILE = arm-none-linux-gnueabi-

CC = $(CROSS_COMPILE)gcc
RM = rm -f

OS = linux
INC = -I. -Iosal -Iinc
LDLIBPATH = -Losal/$(OS)
CFLAGS = -c -g -Wall -DSIMULATION -DSTANDALONE -DUSE_LINUX $(INC) 
LDFLAGS = -lrt -losal

SOURCES = \
	PHY_simulator.c \
	simu_util.c \
	macMemUtil.c \
	ltemaccore.c \
	bsmac.c \
	msmac.c \
	LteTestTables.c

ALL_OBJECTS = $(SOURCES:.c=.o)

COMMON_OBJ = PHY_simulator.o simu_util.o macMemUtil.o ltemac.o LteTestTables.o
BS_OBJECTS = BS_simulator.o bsmac.o
MS_OBJECTS = MS_simulator.o msmac.o

BSEXE = simuBS
MSEXE = simuMS

.PHONY: all
all: $(BSEXE) $(MSEXE)

.PHONY: bsmac
bsmac: $(BSEXE)

.PHONY: msmac
msmac: $(MSEXE)

# Cancel implicit rule
%.o : %.c

$(BSEXE): $(BS_OBJECTS) $(COMMON_OBJ) osal/$(OS)/libosal.a
	$(CC) $(LDLIBPATH) $(BS_OBJECTS) $(COMMON_OBJ) $(LDFLAGS) -o $@

$(MSEXE): $(MS_OBJECTS) $(COMMON_OBJ) osal/$(OS)/libosal.a
	$(CC) $(LDLIBPATH) $(MS_OBJECTS) $(COMMON_OBJ) $(LDFLAGS) -o $@

osal/$(OS)/libosal.a:
	$(MAKE) -C osal/$(OS)
	
$(BS_OBJECTS): $(BS_OBJECTS:.o=.c)
	$(CC) $(CFLAGS) -DBSMAC -DENODEB -UMSMAC -UUE $?

$(MS_OBJECTS): $(MS_OBJECTS:.o=.c)
	$(CC) $(CFLAGS) -UBSMAC -UENODEB -DMSMAC -DUE $?

$(COMMON_OBJ): $(COMMON_OBJ:.o=.c)
	$(CC) $(CFLAGS) -UBSMAC -UENODEB -UMSMAC -UUE $?

.PHONY : clean
clean:
	-$(RM) $(COMMON_OBJ) $(BS_OBJECTS) $(MS_OBJECTS)
	-$(RM) *~
	-$(RM) $(BSEXE) $(MSEXE)
	-$(MAKE) -C osal/$(OS) clean
