#!/bin/bash
# Purpose: Get active RAB Status
#
# 28Sep2011, Zarnappa Shivaraya: Created


export PICOBS_HOME=$PWD/..
#Scripts run in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

#Get the status of active  Rab
$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "get 117"
