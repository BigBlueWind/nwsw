#!/bin/bash
# Purpose: install a package on eNB
#
# 26 Nov 2010, Ankur: Created
#set -x
picobsHomePath=$1; # path where all DIR logs,bin,download,rel,run 
inst_A=$2;

# PICOBS_HOME should start with '/'.

if [ ${picobsHomePath:0:1} != '/' ]; then
  echo "Please provide full path of PICOBS_HOME path.";
  exit 1;
fi

# Maximum allowed absolute length for $PICOBS_HOME/run/../rel/inst_A/config/xxx.cfg = 100 characters
# so Maximum allowed path for $PICOBS_HOME = 60 characters

if [ ${#picobsHomePath} -ge 60 ]; then
  echo "PICOBS_HOME path length is more then 60 characters.";
  exit 1;
fi
 
if [ -z "$picobsHomePath" ]; then
  echo "path of directory is not given";
  exit 1;
fi
if [ -z "$inst_A" ]; then
  echo "Please provide package name.";
  exit 1;
fi
if [ ! -d $picobsHomePath ]; then
  echo "$picobsHomePath is not a directory";
  exit 1;
fi
if [ ! -f $inst_A ];then
  echo "file $inst_A not found ";
fi

export PICOBS_HOME=$picobsHomePath

mkdir $picobsHomePath/simulators
mkdir $picobsHomePath/download
mkdir $picobsHomePath/log
mkdir $picobsHomePath/rel
mkdir $picobsHomePath/run
mkdir $picobsHomePath/rel/inst_A
mkdir $picobsHomePath/cli_scripts

tar -x -z --strip-components=1 -f $inst_A -C $picobsHomePath/rel/inst_A/
cp -r $picobsHomePath/rel/inst_A                    $picobsHomePath/rel/inst_B
mv $picobsHomePath/rel/inst_A/bin/prm.exe           $picobsHomePath/run/
mv $picobsHomePath/rel/inst_A/config/sw_package.cfg $picobsHomePath/run/
mv $picobsHomePath/rel/inst_A/bs.sh                 $picobsHomePath/run/
mv $picobsHomePath/rel/inst_A/dump_lowerarm.sh      $picobsHomePath/run/
mv $picobsHomePath/rel/inst_A/rdmem_arm             $picobsHomePath/run/
mv $picobsHomePath/rel/inst_A/bootarm_bs.sh         $picobsHomePath/run/
rm $picobsHomePath/rel/inst_B/bs.sh
rm $picobsHomePath/rel/inst_B/bootarm_bs.sh
rm $picobsHomePath/rel/inst_B/dump_lowerarm.sh
rm $picobsHomePath/rel/inst_B/rdmem_arm
rm $picobsHomePath/rel/inst_B/bin/prm.exe
rm $picobsHomePath/rel/inst_B/config/sw_package.cfg
mv $picobsHomePath/rel/inst_A/cli_scripts/*           $picobsHomePath/cli_scripts/
rm -rf $picobsHomePath/rel/inst_A/cli_scripts
rm -rf $picobsHomePath/rel/inst_B/cli_scripts
chmod 777 $picobsHomePath/rel/inst_A/log
chmod 777 $picobsHomePath/rel/inst_B/log

#Simulator related Setup 
#RH Simulator files 
mv $picobsHomePath/rel/inst_A/bin/rh_sim.exe              $picobsHomePath/simulators
mv $picobsHomePath/rel/inst_A/config/simulatorConfig.cfg  $picobsHomePath/simulators
mv $picobsHomePath/rel/inst_A/config/lksimconfig.cfg      $picobsHomePath/simulators
rm $picobsHomePath/rel/inst_B/bin/rh_sim.exe 
rm $picobsHomePath/rel/inst_B/config/simulatorConfig.cfg 
rm $picobsHomePath/rel/inst_B/config/lksimconfig.cfg

#EMS Simulator files
mv $picobsHomePath/rel/inst_A/bin/ems_sim.exe             $picobsHomePath/simulators
mv $picobsHomePath/rel/inst_A/bin/snmpset                 $picobsHomePath/simulators
mv $picobsHomePath/rel/inst_A/config/enodebagentport.cfg  $picobsHomePath/simulators
rm $picobsHomePath/rel/inst_B/bin/ems_sim.exe
rm $picobsHomePath/rel/inst_B/bin/snmpset
rm $picobsHomePath/rel/inst_B/config/enodebagentport.cfg 
 


		
