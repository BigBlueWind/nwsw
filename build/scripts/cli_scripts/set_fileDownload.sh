#!/bin/bash
# Purpose: fileUpload
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..

#filename:File to download to BS, BS will pull the file from the server
filename=""
#login_id : Login at the server from which file has to be downloaded
login_id=""
#password : Password at the Server
password=""
#ipadd: Ip address  of the Server on which file is available.Destionation is always BS
ipadd=""
#Protocolused: Protocol used for the file Download Session FTP=1,SFTP=2 and SCP=3 are supported
protocolused=0
#sourceport : Port Number used by the protocol
sourceport=0
#maxdownloadspeed : Maximum Download Speed
maxdownloadspeed=0

# -i supports the interactive mode

#Script runs in inst_A 
INST_PATH=$PICOBS_HOME/rel/inst_A

if [ $# -ne 0 ]; then
  if [ $1 == "-i" ]; then

     INST_PATH=$PICOBS_HOME/rel/inst_A     
     echo "Enter the following params"

     echo "Full path of the Filename[String: Min(2Chars) Max(256 Chars)]:"
     read filename      

     echo "Login Id[String: Min(2Chars) Max(256 Chars)]:"
     read login_id

     echo "Password[String: Min(2Chars) Max(256 Chars)]:"
     read password

     echo "Ip address[String: Min(7 Chars) Max(15 Chars) Eg:172.17.3.5]:"
     read ipadd

     echo "Protocol Used[Enum :ftp=1,scp=2,sftp=3]:"
     read protocolused
     
     echo "Source Port[UINT32:Min(0) Max:(65535)]:"
     read sourceport

     echo "MaxDownloadSpeed[Enum:50kbps=1,100kbps=2,500kbps=3 1mbps=4 10mbps=5 unlimited=6]:"
     read maxdownloadspeed

     #$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 2.1={1,1,File name,User Name,Password,Ip address,Cmd,Ftp port,Speed}"
      $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 2.1={1,1,$filename,$login_id,$password,$ipadd,$protocolused,$sourceport,$maxdownloadspeed}"
  else
      echo "Invalid Option. Try -i for interactive mode."
  fi
else 
     #$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 2.1={1,1,File name,User Name,Password,Ip address,Cmd,Ftp port,Speed}"
     $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 2.1={1,1,/home/lte/package.tgz,lte,abc123,10.131.24.164,2,22,5}"
fi

