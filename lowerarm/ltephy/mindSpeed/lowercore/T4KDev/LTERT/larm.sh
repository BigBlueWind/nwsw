#!/bin/sh

source ./rvds-env.sh

make -C ../.. -s -f T4KDev/LTERT/makefile-larm $*
