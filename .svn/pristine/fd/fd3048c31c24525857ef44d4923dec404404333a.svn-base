#!/bin/bash
# Purpose: Software Activation Status
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..
#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

#Get the status of Softwatre activation 
$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "get 103"
