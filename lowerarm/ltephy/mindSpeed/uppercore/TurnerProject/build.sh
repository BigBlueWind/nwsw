#!/bin/sh

#
# Build default target
#

#
# These paths are used as example, must be set externally before invoking tool
#
export CSL_ROOT=~/Turner/arm2008-q1
export CSL_PREFIX=arm-none-linux-gnueabi
export GCC_VSN=4.2.3

export PATH=$PATH:$CSL_ROOT/bin

make $* "OS=LINUX" "TOOLCHAIN=CS"
