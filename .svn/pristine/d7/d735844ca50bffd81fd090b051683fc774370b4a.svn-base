# config.mk
#
# Copyright (c) 2008 by Mindspeed Technologies
#
# Configuration file used by make
#

#
# Determine Host OS
#
ifdef windir
HOST_OS = WINDOWS
else ifdef WinDir
HOST_OS = WINDOWS
else ifdef WINDIR
HOST_OS = WINDOWS
else
HOST_OS = LINUX
endif
export HOST_OS

#
# Set up default shell
#
ifeq ($(HOST_OS),WINDOWS)
SHELL = $(COMSPEC)
export SHELL
RM = rm -f
else
RM = rm -f
endif
export RM
