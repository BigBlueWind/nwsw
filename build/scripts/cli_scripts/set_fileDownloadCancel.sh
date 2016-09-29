# Purpose: fileDownloadCancel
#
# 28Sep2011, Zarnappa Shivaraya: Create#!/bin/bash
# Purpose: fileDownloadCancel
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..
#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

#$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 2.1={1,2,FileName,UserName,Password,cmd,FTPport,Speed}"
$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 2.1={1,2,package.tgz,lte,abc123,10.131.24.164,2,22,1};"
