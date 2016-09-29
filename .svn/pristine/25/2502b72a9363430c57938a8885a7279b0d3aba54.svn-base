#!/bin/bash
# Purpose: Pm Log Counter Config
#
# 17JAN2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..
#PmProfileId:Index is unique row indentifier
pmProfileId=1
#PmFileCreationInterval: It is the interval at which counter snap-shot should be taken
pmFileCreationInterval=1
#SampleInterval:It is the interval at which counter file should be generated
sampleInterval=1
#collectMacGenCounters: Collect MAC general Counters
collectMacGenCounters=0
#collectMacUplaneCounters:Collect MAC User plane Counters
collectMacUplaneCounters=0
#collectRlcGenCounters:Collect Rlc general Counters
collectRlcGenCounters=0
#collectPdcpGenCounters:Collect Pdcp general Counters
collectPdcpGenCounters=0
#collectPdcpUplaneCounters:Collect Pdcp User Plane Counters
collectPdcpUplaneCounters=0
#collectGtpGenCounters:Collect Gtp general counters
collectGtpGenCounters=0
#collectSctpGenCounters:Collect Sctp general counters
collectSctpGenCounters=0
#collectS1apGenCounters:Collect S1ap general counters
collectS1apGenCounters=0
#collectS1apPeerNodeCounters: Collect S1ap Peer Node Counters
collectS1apPeerNodeCounters=0
#collectRrcGenCounters: Collect RRC general counters
collectRrcGenCounters=0
#collectRrcCellCounters:Collect RRC Cell Counters
collectRrcCellCounters=0
#collectRrmCounters:Collect RRM Counters
collectRrmCounters=0
#collectIpCounters:Collect Ip Counters
collectIpCounters=0
#collectEthernetCounters:Collect Ethernet Counters
collectEthernetCounters=0

#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

if [ $# -ne 0 ]; then
  if [ $1 == "-i" ]; then

     INST_PATH=$PICOBS_HOME/rel/inst_A
     
     echo "Enter the following params"
     echo "Enter PmProfileId"
     read pmProfileId
     echo "Enter Sample Interval[ENUM:1min=1,5min=2,10min=3,15min=4,30min=5,1h=6]"
     read sampleInterval

     echo "Enter pmFile Creation Interval[ENUM:1h=1,2h=2,4h=3,8h=4]"
     read pmFileCreationInterval

     echo "CollectMacGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectMacGenCounters

     echo "CollectMacUplaneCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectMacUplaneCounters

     echo "CollectRlcGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectRlcGenCounters

     echo "CollectPdcpGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectPdcpGenCounters

     echo "CollectPdcpUplaneCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectPdcpUplaneCounters

     echo "CollectGtpGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read collectGtpGenCounters

     echo "CollectSctpGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectSctpGenCounters

     echo "CollectS1apGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectS1apGenCounters

     echo "CollectS1apPeerNodeCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectS1apPeerNodeCounters

     echo "CollectRrcGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectRrcGenCounters

     echo "CollectRrcCellCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectRrcCellCounters

     echo "CollectRrmCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectRrmCounters

     echo "CollectIpCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectIpCounters

     echo "CollectEthernetCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectEthernetCounters

     echo "CollectMacGenCounters[TBOOL: FALSE(0) TRUE(1)]:"
     read  collectMacGenCounters

     $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 63.1={$pmProfileId,$sampleInterval,$pmFileCreationInterval,$collectMacGenCounters,$collectMacUplaneCounters,$collectRlcGenCounters,$collectPdcpGenCounters,$collectPdcpUplaneCounters,$collectGtpGenCounters,$collectSctpGenCounters,$collectS1apGenCounters,$collectS1apPeerNodeCounters,$collectRrcGenCounters,$collectRrcCellCounters,$collectRrmCounters,$collectIpCounters,$collectEthernetCounters}"

  else
      echo "Invalid Option. Try -i for interactive mode."
  fi

else 
 
   $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 63.1={1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}"                                                            
fi
