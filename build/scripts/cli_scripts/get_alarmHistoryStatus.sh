#!/bin/bash
# Purpose: Get active alram History
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..

#Scripts runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

#Get the Status of active alram history
$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "get 121"
