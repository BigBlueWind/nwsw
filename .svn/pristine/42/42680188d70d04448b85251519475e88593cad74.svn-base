#!/bin/bash
# Purpose: Software Install in eNodeB
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..

#installationtype:Type of installation component to be installed at ENB
installationtype=""
#filename:Filename with which package will be installed
filename=""
#installer:Installer script/tool that should be used for installing the new SW
#installer=""
#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

if [ $# -ne 0 ]; then
  if [ $1 == "-i" ]; then

      INST_PATH=$PICOBS_HOME/rel/inst_A
      echo "Enter the following params"
      echo "Installation Type[ENUM :l2l3Release=1 ,l2l3Configuration=2,l2l3Patch=3,RHsw=4]:"
      read installationtype
      echo "FileName[String: Min(5Chars) Max(256 Chars)]:"
      read filename
      #echo "Installer[String: Min(2Chars) Max(256 Chars)]:"
      #read installer

      #set 5.1={index,installationType,filename,installer}
      $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 5.1={1,$installationtype,$filename,#}"
 else
      echo "Invalid Option. Try -i for interactive mode."
  fi

else
      $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 5.1={1,1,bin.tgz,#}"

fi

