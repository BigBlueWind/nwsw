#!/bin/bash
# Purpose:  Configuration eNodeB
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..
#enodebId:eNB uniquely within the PLMN
enodebId=0
#enodebName:eNodeB name
enodebName=""
#useStaticIpAddr: ENB will use the static IP address
useStaticIpAddr=""
#enodebStaticIpAddr:eNodeB first tries to aquire IP address using DHCP. If it cannot, then it tries to use this address.
enodebStaticIpAddr=""
#enodebNetmaskAddr:Netmask address to configure static IP 
enodebNetmaskAddr=""
#enodebRegestrationAccepted:This parameter can only be set from SNMP. TRUE means ENB is registered to the EMS (enbRegConfig.emsAddr). It is non-persistent. At reboot/restart, ENB sets it to FALSE and re-registers
enodebregaccepted=0
#configChangeAckByEms:TRUE means any change performed via non-EMS interfaces (CLI/Web) is acknowledged by EMS. When any configuration parameter is modified through CLI/Web this is set to FALSE and a notification is sent to EMS. EMS needs to set this value to TRUE after acknowledging the change towards configuration data synchronization
configChangeAckByEms=0
#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

if [ $# -ne 0 ]; then
  if [ $1 == "-i" ]; then

        INST_PATH=$PICOBS_HOME/rel/inst_A        
        echo "Enter the following params"
	echo "enodebId[UINT32:Min(0) Max(1048575)]"
	read enodebId
	echo "enodebName[String:Min(3) Max(150)]:"
	read enodebName
	echo "useStaticIpAddr[TBOOL:FALASE(0) TRUE(1)]:"
	read useStaticIpAddr
	echo "enodebStaticIpAddr[String:Min(7) Max(15)]:"
	read enodebStaticIpAddr
	echo "enodebNetmaskAddr[String:Min(7) Max(15)]:"
	read enodebNetmaskAddr
	echo "enodeb Regestration Accepted[TBOOL:FALASE(0) TRUE(1)]:"	
	read enodebregaccepted
	echo "configChangeAckByEms[TBOOL:FALASE(0) TRUE(1)]:"
	read configChangeAckByEms

	$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 52.1={$enodebId,$enodebName,$useStaticIpAddr,$enodebStaticIpAddr,$enodebNetmaskAddr,$enodebregaccepted,$configChangeAckByEms}"
 
else
      echo "Invalid Option. Try -i for interactive mode."
  fi
else
        $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 52.1={1,pwav-pico,0,172.17.5.8,255.255.255.0,0,0}"
fi
