#!/bin/bash
# Purpose: Set cell config
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..

#cellid:Cell Indentity
cellid=0
#freqBandInd:Frequency band indicator for FDD mode
freqBandInd=13
#dlEarfcn:ARFCN downlink E-UTRA Carrier frequency
dlEarfcn=5230
#ulEfran:ARFCN uplink E-UTRA Carrier frequency
ulEarfcn=23230

#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

if [ $# -ne 0 ]; then
  if [ $1 == "-i" ]; then
     
     INST_PATH=$PICOBS_HOME/rel/inst_A
     echo "Enter the following params" 

     echo "CellId[UINT8:Min(1) Max(255)]:"
     read cellid
     echo "Frequncy Band indicator[UINT8:Min(1) Max(255)]:"
     read freqBandInd
     echo "DL EARFCN[UINT32:Min(0) Max(65535)]:"
     read dlEarfcn
     echo "UL EARFCN[UINT32:Min(0) Max(65535)]:"
     read ulEarfcn

     $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 53.1={$cellid,#,#,#,#,#,#,$freqBandInd,#,$dlEarfcn,#,$ulEarfcn,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,0,#,#,#,#,#,#,#}"
  else
      echo "Invalid Option. Try -i for interactive mode."
  fi
else

   $INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "set 53.1={1,#,#,#,#,#,#,11,#,5230,#,23230,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,0,#,#,#,#,#,#,#}"
fi
