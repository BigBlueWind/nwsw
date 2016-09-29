#!/bin/bash
# Purpose: install a package on eNB
#
# 29 Oct2010, Manu: Created
# 25 Nov2010, Ankur:Updated
# 31 Mar2011, Ankur:Added support for creating log file
# 01 Aug2011, Ankur: Removed check of package duplicate
#To Enable the display for the Shell commands uncomment next line
# This is the script for x86 environment. The difference in pkg_installer_x86.sh and pkg_installer_4gmx.sh are:
# 1. in list of exes, exes = { radioup.exe/lowerarm.exe } rest all executables will remain same.
# NOTE:: Any change in this file should also be replicated in pkg_installer_4gmx.sh
#set -x

###################################################################################
#  Function to exit from script and write to install_failed
###################################################################################
function exitFromScript {
  echo $1 >$picobsHome/log/install_failed   #errString
  echo $2 >>$picobsHome/log/install_failed  #bsError
  echo "=== LOG CLOSED FOR PACKAGE INSTALLER === $(date)" >>$picobsHome/log/package_install.log
  rm -rf $picobsHome/rel/inst_C
  exit
}

fileName=$1;
nonRunningRootDir=$2;
runningVersionInfo=$3;
runningRootDir=$4;
ckeckLevel=$5;

picobsHome=$PICOBS_HOME;      # path where the package is unpacked

exes=(bsm.exe cp.exe gtp.exe radioup.exe deh.exe sih.exe fth.exe pmh.exe cih.exe  wih.exe lighttpd.exe);
config=(enbLimits.cfg imc.cfg mapping_table.cfg enb_user_config.cfg sw_package.cfg version.cfg lighttpd.conf module_stop_table.cfg enb_pre_config.cfg fth.cfg oamConfig.tpl process_start_table.cfg snmpd.conf alarmDb.cfg);
topLevelFiles=(pkg_installer.sh patch_installer.sh config_installer.sh rh_installer.sh bs.sh);

if [[ $ckeckLevel == FirstLevel ]];then
  echo "=== LOG OPENED FOR PACKAGE INSTALLER === $(date)" >$picobsHome/log/package_install.log
  #################################### CHECK FILE PRESENCE #####################################
  echo "1: Checking for install file presence " >>$picobsHome/log/package_install.log
  if [ ! -f $picobsHome/download/$fileName ];then
    echo "   Result: Failed: file $fileName not found in $picobsHome/download/ folder " >>$picobsHome/log/package_install.log
    exitFromScript "Installation File for full release installation is not Found" 5
  fi
  echo "   Result: Successful: file $fileName found in $picobsHome/download/ folder " >>$picobsHome/log/package_install.log

  #################################### CHECK FILE FORMAT #####################################
  echo "2: Checking for install file format " >>$picobsHome/log/package_install.log
  if [ tgz != ${fileName/*./} ];then
    echo "   Result: Failed: Install File format not supported for file $picobsHome/download/$fileName " >>$picobsHome/log/package_install.log
    exitFromScript "Unsupported installation File format encountered during full release installation " 6
  fi
  echo "   Result: Successful: Install File format supported for file $picobsHome/download/$fileName " >>$picobsHome/log/package_install.log
  echo "no error" >$picobsHome/log/install_success
else
  #################################### UNPACK PACKAGE #####################################
  echo "3: Unpacking the package " >>$picobsHome/log/package_install.log
  mkdir $picobsHome/rel/inst_C	
  tar -x -z --strip-components=1 -f $picobsHome/download/$fileName -C $picobsHome/rel/inst_C     #untar the package
  if [ $? != 0 ];then
    echo "   Result: Failed: Unpacking install file $picobsHome/download/$fileName " >>$picobsHome/log/package_install.log
    exitFromScript "Unable to decompress installation file during full release installation" 1
  fi
  echo "   Result: Successful: Unpacking install file $picobsHome/download/$fileName " >>$picobsHome/log/package_install.log
   
  #################################### PACKAGE VERIFICATION #####################################
  echo "4: Verifying package files " >>$picobsHome/log/package_install.log
  # testing of bin folder
  if [ -d $picobsHome/rel/inst_C/bin ]; then
    for name in ${exes[@]}; do
      if [ ! -f $picobsHome/rel/inst_C//bin/$name ];then
        echo "   Result: Failed: $name not found in bin folder" >>$picobsHome/log/package_install.log
        exitFromScript "$name not found in bin folder during Package verification" 2
      fi
      echo "   Result: Successful: $name found in bin folder " >>$picobsHome/log/package_install.log
    done
  else
    echo "   Result: Failed: bin folder not found" >>$picobsHome/log/package_install.log
    exitFromScript "bin folder not found during package verification" 2
  fi

  # testing of config folder
  if [ -d $picobsHome/rel/inst_C/config ]; then
    for name in ${config[@]}; do
      if [ ! -f $picobsHome/rel/inst_C/config/$name ];then
        echo "   Result: Failed: $name not found in config folder" >>$picobsHome/log/package_install.log
        exitFromScript "$name not found in config folder during package verification" 2
      fi
      echo "   Result: Successful: $name found in config folder " >>$picobsHome/log/package_install.log
    done
  else
    echo "   Result: Failed: config folder not found during package verification" >>$picobsHome/log/package_install.log
    exitFromScript "config folder not found during package verification" 2
  fi

  # testing of web folder
  if [ ! -d $picobsHome/rel/inst_C/web ]; then
    echo "   Result: Failed: web folder not found" >>$picobsHome/log/package_install.log
    exitFromScript "web folder not found during package verification" 2
  fi

  # testing for top level files
  for name in ${topLevelFiles[@]}; do
    if [ ! -f $picobsHome/rel/inst_C/$name ];then
      echo "   Result: Failed: $name file not found in folder" >>$picobsHome/log/package_install.log
      exitFromScript "$name file not found in folder during package verification" 2
    fi
  done

  #################################### COPY EMS Ip Address FROM RUNNING PACKAGE ###################
  exec 4<$runningRootDir/config/enb_user_config.cfg
  exec 3<$picobsHome/rel/inst_C/config/enb_user_config.cfg
  while read -u 3 NONRUNNING_CFG_LINE
  do
    if [[ $NONRUNNING_CFG_LINE == *emsAddr* ]];then
      while read -u 4 RUNNING_CFG_LINE
      do
        if [[ $RUNNING_CFG_LINE == *emsAddr* ]];then
          echo "$RUNNING_CFG_LINE" >>$picobsHome/rel/inst_C/config/enb_user_config1.cfg
          break;
        fi
      done
    else  
      echo "$NONRUNNING_CFG_LINE" >>$picobsHome/rel/inst_C/config/enb_user_config1.cfg
    fi
  done
  exec 3>&-
  exec 4>&-
  mv $picobsHome/rel/inst_C/config/enb_user_config1.cfg $picobsHome/rel/inst_C/config/enb_user_config.cfg

  #################################### COPY TO NON-RUNNING PACKAGE #####################################
  # TODO: write to flash
  rm -rf $nonRunningRootDir/*
  cp -rf $picobsHome/rel/inst_C/* $nonRunningRootDir/

  rm -rf $nonRunningRootDir/bs.sh
  rm -rf $nonRunningRootDir/dump_lowerarm.sh

  rm -rf $nonRunningRootDir/bin/prm.exe
  rm -rf $nonRunningRootDir/bin/rh_sim.exe 
  rm -rf $nonRunningRootDir/bin/ems_sim.exe
  rm -rf $nonRunningRootDir/bin/snmpset
  rm -rf $nonRunningRootDir/rdmem_arm

  rm -rf $nonRunningRootDir/config/sw_package.cfg
  rm -rf $nonRunningRootDir/config/simulatorConfig.cfg 
  rm -rf $nonRunningRootDir/config/lksimconfig.cfg
  rm -rf $nonRunningRootDir/config/enodebagentport.cfg
  
  rm -rf $picobsHome/rel/inst_C/
  rm -rf $picobsHome/download/$fileName

  chmod 777 $nonRunningRootDir/log

  echo "no error" >$picobsHome/log/install_success
  echo "=== LOG CLOSED FOR PACKAGE INSTALLER === $(date)" >>$picobsHome/log/package_install.log
fi
# bsError
# 1 = BS_ERR_INSTALL_PKG_UNPACKING_FAILED
# 2 = BS_ERR_INSTALL_VERIFICATION_FAILED
# 4 = BS_ERR_INSTALL_FLASH_ACCESS_FAILED
# 5 = BS_ERR_INSTALL_FILE_NOT_FOUND
# 6 = BS_ERR_INSTALL_UNSUPPORTED_FILE_FORMAT
