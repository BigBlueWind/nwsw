
RM := rm -f

# All of the sources participating in the build are defined here
O_SRCS :=
CPP_SRCS :=
C_UPPER_SRCS :=
C_SRCS :=
S_UPPER_SRCS :=
S_SRCS :=
ASM_SRCS :=
CXX_SRCS :=
CC_SRCS :=
OBJS :=
ASM_DEPS :=
C_DEPS :=
CC_DEPS :=
ARCHIVES :=
CPP_DEPS :=
S_DEPS :=
CXX_DEPS :=
C_UPPER_DEPS :=
S_UPPER_DEPS :=
OSLIB := os.a
C_HDRS :=
HDRS :=

# Flags controlling optimization and debugging
OPT :=

# C sources
C_SRCS += \

# Object files
C_OBJS = $(C_SRCS:.c=.o)
OBJS = $(C_OBJS) 

ifeq ($(MAKECMDGOALS),debug)
OPT += --debug -O0 -D_MX_DEBUG -D_APP_ASSERT_HANDLER -DMX_CREATE_OBJ_LOG_ENABLED
else
OPT += -O3 -Otime
endif

INC := \
 -I".." \
 -I"./include" \
 -I"../../SERVICES" \
 -I"../../DRIVERS" \
 -I"../../DRIVERS/CPRI" \
 -I"../../DRIVERS/FEC" \
 -I"../../DRIVERS/MAP" \
 -I"../../../4GMX/Inc" \
 -I"../../../4GMX/ARM/Inc"

DEF = -DMX_ARM_ENABLED 

ifeq ($(CCPUBUILD),1)
DEF += -DCCPU_MAC
endif


CFLAGS = \
 --diag_style=ide \
 --depend_format=unix_escaped \
 --no_depend_system_headers \
 --arm \
 --cpu=Cortex-A9.no_neon.no_vfp \
 --fpu=SoftVFP \
 --no_unaligned_access \
 $(OPT) \
 $(INC) \
 $(DEF)

# Each subdirectory must supply rules for building sources it contributes

%.o: %.c %.h
	@echo $<
	@armcc -c $(CFLAGS) -o"$@" "$<"

USER_OBJS :=

LIBS := fs/fs.a

# Targets

.PHONY : all
all:    $(OSLIB) $(OBJS) $(USER_OBJS) $(LIBS)	


$(OSLIB): $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Create $(NETLIB)
	@armar --create --diag_style=ide $(OSLIB) $(OBJS) $(LIBS)	
	@echo Ok

fs/fs.a:
	make -C fs

.PHONY : clean
clean:
	@del *.o /F
	@del *.a /F
	@del fs\*.o /F
	@del fs\*.a /F



