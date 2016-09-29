#!/bin/bash
# Purpose: perform sanity checkes for RH-SW install
#
# 26 Apr2012, Ankur: Created
#set -x
#####################################################################################
## Script runs in two levels                                                       ##    
## FirstLevel:  1. Check install file presence                                     ##
##              2. Check install file format                                       ##
##              3. Check FTP server running                                        ##
## SecondLevel: 1. Decompress the package                                          ##
##              2. Verfify the package                                             ##
##                 a. Check presence of packageInfo.txt                            ##                 
##                 b. Read  packageInfo.txt and find the composite image name      ##                
##                 c. Check format of composite image. It should be in .bin format ##                 
##                 d. Check presence of composite image.                           ## 
##                 e. Check composite image size which whould not exceed 6MB       ##                
#####################################################################################

 
###################################################################################
#  Function to exit from script and write to install_failed
###################################################################################
function exitFromScript {
  echo $1 >$picobsHome/log/install_failed   #errString
  echo $2 >>$picobsHome/log/install_failed  #bsError
  echo "=== LOG CLOSED FOR RH SOFTWARE INSTALLER === $(date)" >>$picobsHome/log/rh_install.log
  rm -rf $picobsHome/download/tmp
  exit
}

checkLevel=$5;
fileName=$1;
picobsHome=$PICOBS_HOME;  
if [[ $checkLevel == FirstLevel ]];then
  echo "=== LOG OPENED FOR RH SOFTWARE INSTALLER === $(date)" >$picobsHome/log/rh_install.log
  #################################### CHECK FILE PRESENCE #####################################
  echo "1: Checking for install file presence " >>$picobsHome/log/rh_install.log
  if [ ! -f $picobsHome/download/$fileName ];then
    echo "   Result: Failed: file $fileName not found in $picobsHome/download/ folder " >>$picobsHome/log/rh_install.log
    exitFromScript "Installation File for RH-SW installation is not Found" 5
  fi
  echo "   Result: Successful: file $fileName found in $picobsHome/download/ folder " >>$picobsHome/log/rh_install.log

  #################################### CHECK FILE FORMAT #####################################
  echo "2: Checking for install file format " >>$picobsHome/log/rh_install.log
  if [ tgz != ${fileName/*./} ];then
    echo "   Result: Failed: Install File format not supported for file $picobsHome/download/$fileName " >>$picobsHome/log/rh_install.log
    exitFromScript "Unsupported installation File format encountered during RH-SW installation " 6
  fi
  echo "   Result: Successful: Install File format supported for file $picobsHome/download/$fileName " >>$picobsHome/log/rh_install.log

  ################################### CHECK FTP SERVER RUNNING ################################
  echo "3: Checking for FTP server running or not " >>$picobsHome/log/rh_install.log
  ftpStatus=$(echo `netstat -a |grep ftp |awk '{print $6}'`)
  if [[ $ftpStatus != "LISTEN" ]];then
    echo "   Result: Failed: FTP server is not running on eNodeB during RH-SW install" >>$picobsHome/log/rh_install.log
    exitFromScript "FTP server is not running on eNodeB during RH-SW install" 8
  fi
  echo "   Result: Successful: FTP server is running " >>$picobsHome/log/rh_install.log
  echo "no error" >$picobsHome/log/install_success

else
  #################################### UNPACK PACKAGE #####################################
  echo "4: Unpacking the package " >>$picobsHome/log/rh_install.log
  mkdir $picobsHome/download/tmp	
  tar -xzf $picobsHome/download/$fileName -C $picobsHome/download/tmp > /dev/null 2>&1    #untar the package
  if [ $? != 0 ];then
    echo "   Result: Failed: Unpacking install file $picobsHome/download/$fileName " >>$picobsHome/log/rh_install.log
    exitFromScript "Unable to decompress installation file during RH-SW installation" 1
  fi
  echo "   Result: Successful: Unpacking install file $picobsHome/download/$fileName " >>$picobsHome/log/rh_install.log

  #################################### PACKAGE VERIFICATION ##############################
  
  echo "5:  Verifying package files " >>$picobsHome/log/rh_install.log
  #Check presence of packageInfo.txt 
  if [ ! -f $picobsHome/download/tmp/packageInfo.txt ];then
    echo "   Result: Failed: packageInfo.txt not found in package" >>$picobsHome/log/rh_install.log
    exitFromScript "packageInfo.txt not found in package during Package verification" 2
  fi
  echo "   Result: Successful: packageInfo.txt found in package " >>$picobsHome/log/rh_install.log

  # Read the Composite image name from packageInfo.txt file
  exec 4<$picobsHome/download/tmp/packageInfo.txt
  while read -u 4 PACKAGE_INFO_LINE
  do
    if [[ $PACKAGE_INFO_LINE == *compositeImageName* ]];then
      compositeImageName=$(echo $PACKAGE_INFO_LINE | cut -d '#' -f1 | cut -d '=' -f2|tr -d " "| tr -d ";"|  tr -d '"'|tr -d "\r\n")
      #Check the extension of the composite image. It should be .bin
      if [ bin != ${compositeImageName/*./} ];then
        echo "   Result: Failed: Install File format not supported for file $compositeImageName " >>$picobsHome/log/rh_install.log
        exitFromScript "Unsupported compositeImage File format encountered during RH-SW installation " 2
      fi
      echo "   Result: Successful: Install File format supported for file $compositeImageName " >>$picobsHome/log/rh_install.log
      
      #Check the presence of composite image.
      if [ ! -f $picobsHome/download/tmp/$compositeImageName ];then
        echo "   Result: Failed: file $compositeImageName not found in package " >>$picobsHome/log/rh_install.log
        exitFromScript "Composite image $compositeImageName is not found during RH-SW installation" 2
      fi
      echo "   Result: Successful: file $compositeImageName found in package " >>$picobsHome/log/rh_install.log
      
      #Check the size of the Composite Image size <= 6291456 (6*1024*1024)
      fileSize=`du -sb $picobsHome/download/tmp/$compositeImageName | awk '{print $1}' `
      if [[ $fileSize -gt 6291456 ]];then
        echo "   Result: Failed: Comosite Image size is more then 6MB " >>$picobsHome/log/rh_install.log
        exitFromScript "Comosite Image size is more then 6MB during RH-SW installation " 7
      fi 
      echo "   Result: Successful: Comosite Image size is not more then 6MB " >>$picobsHome/log/rh_install.log
    fi
  done
  exec 4>&-

  mv $picobsHome/download/tmp/$compositeImageName $picobsHome/download/
  rm -rf $picobsHome/download/tmp/
  echo "=== LOG CLOSED FOR RH SOFTWARE INSTALLER === $(date)" >>$picobsHome/log/rh_install.log
  echo $compositeImageName >$picobsHome/log/install_success
  echo "no error" >>$picobsHome/log/install_success

fi

# bsError
# 1 = BS_ERR_INSTALL_PKG_UNPACKING_FAILED
# 2 = BS_ERR_INSTALL_VERIFICATION_FAILED
# 4 = BS_ERR_INSTALL_FLASH_ACCESS_FAILED
# 5 = BS_ERR_INSTALL_FILE_NOT_FOUND
# 6 = BS_ERR_INSTALL_UNSUPPORTED_FILE_FORMAT
# 7 = BS_ERR_INSTALL_FILE_SIZE_EXCEEDED
# 8 = BS_ERR_INSTALL_FTP_SERVER_NOT_RUNNING
