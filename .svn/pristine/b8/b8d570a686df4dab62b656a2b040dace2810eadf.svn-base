#!/bin/bash
# Purpose: install a patch on eNB
#
# 30 Mar2011, Ankur:Created
#To Enable the display for the Shell commands uncomment next line
# This is the script for 4gmx environment. The difference in patch_install_x86.sh and patch_install_4gmx.sh are: 
# 1. in list of exes, exes = { radioup.exe/lowerarm.exe } rest all executables will remain same.
# 2. mapConfigFile radioup.exe $configFile ==> present in x86 only 
# Any change in this file should also be replicated in patch_install_x86.sh
#set -x

###################################################################################
#  Function to exit from script and write to install_failed
###################################################################################

function exitFromScript {
  echo $1 >$picobsHome/log/install_failed   #errString
  echo $2 >>$picobsHome/log/install_failed  #bsError
  echo "=== LOG CLOSED FOR PATCH INSTALLER === $(date)" >>$picobsHome/log/patch_install.log
  rm -rf $picobsHome/rel/inst_C
  exit
}

exes=(bsm.exe cp.exe gtp.exe lowerarm.elf deh.exe sih.exe fth.exe pmh.exe wih.exe lighttpd.exe lth.exe spawn-fcgi.exe);
configs=(alarmDb.cfg enbLimits.cfg fth.cfg lighttpd.conf snmpd.conf);
fileName=$1;
ckeckLevel=$5;

picobsHome=$PICOBS_HOME;      # path where the package is unpacked
errDetail="no error"
patchFile="none"
isFileFound="FALSE"
((noOfExe = 0))

###################################################################################
#  Function to map configuration file with its respective exe
###################################################################################
function mapConfigFile {
  for exeList in $(find $picobsHome/rel/inst_C/bin -type f ); do
    exeFile=${exeList##*/} # get file name from full file Path  
      if [[ $exeFile == $1 ]];then
        echo "   Result: Successful: Mapping exe found for configuration file $2 " >>$picobsHome/log/patch_install.log
        isFileFound="TRUE"
      fi      
  done
}

if [[ $ckeckLevel == FirstLevel ]];then
  echo "=== LOG OPENED FOR PATCH INSTALLER === $(date)" >$picobsHome/log/patch_install.log
  #################################### CHECK FILE PRESENCE #####################################
  echo "1: Checking for install file presence " >>$picobsHome/log/patch_install.log
  if [ ! -f $picobsHome/download/$fileName ];then
    echo "   Result: Failed: file $fileName not found in $picobsHome/download/ folder " >>$picobsHome/log/patch_install.log
    exitFromScript "Installation File for patch installation is not Found at EnodeB" 5
  fi
  echo "   Result: Successful: file $fileName found in $picobsHome/download/ folder " >>$picobsHome/log/patch_install.log

  #################################### CHECK FILE FORMAT #####################################
  echo "2: Checking for install file format " >>$picobsHome/log/patch_install.log
  if [ tgz != ${fileName/*./} ];then
    echo "   Result: Failed: Install File format not supported for file $picobsHome/download/$fileName " >>$picobsHome/log/patch_install.log
    exitFromScript "Unsupported installation File format encountered during patch installation " 6
  fi
  echo "   Result: Successful: Install File format supported for file $picobsHome/download/$fileName " >>$picobsHome/log/patch_install.log
  echo "no error" >$picobsHome/log/install_success
else
  #################################### UNPACK PATCH #####################################
  echo "3: Unpacking the package " >>$picobsHome/log/patch_install.log
  mkdir $picobsHome/rel/inst_C	
  tar -xzf $picobsHome/download/$fileName -C $picobsHome/rel/inst_C > /dev/null 2>&1     #untar the package
  if [ $? != 0 ];then
    echo "   Result: Failed: Unpacking install file $picobsHome/download/$fileName " >>$picobsHome/log/patch_install.log
    exitFromScript "Unable to decompress installation file during patch installation" 1
  fi
  echo "   Result: Successful: Unpacking install file $picobsHome/download/$fileName " >>$picobsHome/log/patch_install.log

  #################################### PATCH VERIFICATION #####################################
  echo "4: Verifying patch files " >>$picobsHome/log/patch_install.log
  mkdir $picobsHome/rel/inst_C/bin
  mkdir $picobsHome/rel/inst_C/config

  for file in $(find $picobsHome/rel/inst_C -type f ); do
    patchFile=${file##*/} # get file name from full file Path
    if [ exe == ${patchFile/*./} ];then
      if [[ $patchFile == prm.exe || $patchFile == cih.exe ]];then
        echo "   Result: Failed: Patch of $patchFile is not supported " >>$picobsHome/log/patch_install.log
        exitFromScript " Patch of $patchFile executable is not supported " 2
      fi
      for name in ${exes[@]}; do
        if [ $name == $patchFile ];then
          echo "   Result: Successful: Patch of $patchFile executable is supported " >>$picobsHome/log/patch_install.log
          mv $file $picobsHome/rel/inst_C/bin
          ((noOfExe++))
          isFileFound="TRUE"
        fi
      done
      if [ $isFileFound == FALSE ];then
        echo "   Result: Failed: unexpected executable $patchFile found" >>$picobsHome/log/patch_install.log
        exitFromScript " unexpected executable $patchFile found in Patch installation file" 2
      fi
      isFileFound="FALSE"
    
    elif [[ cfg == ${patchFile/*./} || txt == ${patchFile/*./} || conf == ${patchFile/*./} ]];then
      for name in ${configs[@]}; do
        if [ $name == $patchFile ];then
          echo "   Result: Successful: Patch of $patchFile configuration file is supported " >>$picobsHome/log/patch_install.log
          mv $file $picobsHome/rel/inst_C/config
          isFileFound="TRUE"
        fi
      done
      if [ $isFileFound == FALSE ];then
        echo "   Result: Failed: unexpected configuration file $patchFile found" >>$picobsHome/log/patch_install.log
        exitFromScript " unsupported configuration file $patchFile found in Patch installation File" 2
      fi  
      isFileFound="FALSE"
    else
      echo "   Result: Failed: unexpected file $patchFile found" >>$picobsHome/log/patch_install.log
      exitFromScript " unexpected file $patchFile found in patch installation file" 2
    fi
  done
  if [[ $noOfExe == 0 ]];then
    echo "   Result: Failed: No exe found in patch" >>$picobsHome/log/patch_install.log
    exitFromScript "No exe found in patch installation file" 2
  fi
  #################################### MAPPING OF PATCH FILES #####################################
  for configList in $(find $picobsHome/rel/inst_C/config -type f ); do
    configFile=${configList##*/} # get file name from full file Path
    if [[ $configFile == alarmDb.cfg ]];then
      mapConfigFile bsm.exe $configFile
    elif [[ $configFile == fth.cfg ]];then
      mapConfigFile fth.exe $configFile
    elif [[ $configFile == lighttpd.conf ]];then
      mapConfigFile lighttpd.exe $configFile
    elif [[ $configFile == snmpd.conf ]];then
      mapConfigFile sih.exe $configFile
    else
      mapConfigFile lth.exe $configFile
      mapConfigFile pmh.exe $configFile
    fi
 
    if [ $isFileFound == FALSE ];then
	  echo "   Result: Failed: Mapping exe not found for configuration file $configFile" >>$picobsHome/log/patch_install.log
	  exitFromScript " Mapping exe not found for config file $configFile in patch installation file" 2
    fi
    isFileFound="FALSE"
  done

  echo "no error" >$picobsHome/log/install_success
  echo "=== LOG CLOSED FOR PATCH INSTALLER === $(date)" >>$picobsHome/log/patch_install.log

  mkdir $picobsHome/rel/inst_C/backup
  mkdir $picobsHome/rel/inst_C/backup/bin
  mkdir $picobsHome/rel/inst_C/backup/config
fi

# bsError
# 1 = BS_ERR_INSTALL_PKG_UNPACKING_FAILED
# 2 = BS_ERR_INSTALL_VERIFICATION_FAILED
# 3 = BS_ERR_INSTALL_PKG_DUPLICATE
# 4 = BS_ERR_INSTALL_FLASH_ACCESS_FAILED
# 5 = BS_ERR_INSTALL_FILE_NOT_FOUND
# 6 = BS_ERR_INSTALL_UNSUPPORTED_FILE_FORMAT
