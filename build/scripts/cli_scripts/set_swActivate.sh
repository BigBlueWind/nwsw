#!/bin/bash
# Purpose: Software Activation
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..

#version:The software version of the Sw release is in release 
version=""
#swType:Type of software component to be activated/switched
swType=0

# -i supports the interactive mode

#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

if [ $# -ne 0 ]; then
  if [ $1 == "-i" ]; then
        
	INST_PATH=$PICOBS_HOME/rel/inst_A
	echo "Enter the following params:"
	echo "Software type[ENUM[enb_l2l3_sw=1]:"
	read swType
	echo "Version[String :Min(9 Chars) Max (19 Chars)]:"
	read version	

	$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 1.1.2=1"
	$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 4.1={1,$swType,$version}"

  else
      echo "Invalid Option. Try -i for interactive mode."
  fi
else
	$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 1.1.2=1"
	$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 4.1={1,1,1.3.0.0.6012}"

fi
