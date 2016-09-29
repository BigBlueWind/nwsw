#!/bin/sh

source ./rvds-config.sh

# ARM RVDS includes
TPATH="${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/include/unix/arm_linux:${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/include/unix"
TPATH="${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/include/unix/arm_linux_compat/sys:${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/include/unix/arm_linux_compat/asm:$TPATH"

# ARM RVDS lib
TPATH="${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/lib/armlib:${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/lib:$TPATH"

# ARM RVDS bin
TPATH="${RVDSBASE}/RVCT/Programs/4.0/${RVDS_BUILD_NUMBER}/linux-pentium:$TPATH"

export RVCT40INC="${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/include/unix"
export RVCT40LIB="${RVDSBASE}/RVCT/Data/4.0/${RVDS_BUILD_NUMBER}/lib"
export RVCT40BIN="${RVDSBASE}/RVCT/Programs/4.0/${RVDS_BUILD_NUMBER}/linux-pentium"

# PATH
export PATH="$TPATH:$PATH"

#test the paths
echo "PATH is $PATH"
echo "RVCT40INC is $RVCT40INC"
echo "RVCT40LIB is $RVCT40LIB"
echo "RVCT40BIN is $RVCT40BIN"
