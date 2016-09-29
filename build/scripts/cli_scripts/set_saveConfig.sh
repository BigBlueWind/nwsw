#!/bin/bash
# Purpose: Save configuration 
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..

#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A
	
# Save the configuration
$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 1.1.2=4"

