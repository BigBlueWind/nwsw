#!/bin/bash 
# Purpose: start, stop pwav-bs
#
# 09July2010, Manu: Created

# commands: start, stop, restart, status, collectlogs, config , bslock ,bsunlock, startcli
# This is the script for x86 environment. The difference in bs_x86.sh and bs_4gmx.sh are: 
# 1. in list of exes, exes = { radioup.exe/lowerarm.exe } . Rest all executables will remain same.
# 2. Added "bootarm $PICOBS_HOME/rel/inst_A/bin/lowerarm.elf" to boot lowerarm image for 4gmx environment only 
# NOTE:: Any change in this file should also be replicated in bs_4gmx.sh

cmd=$1;
export PICOBS_HOME=$PWD/..
#For WIH
export TZ=PST
exes=(cp.exe gtp.exe radioup.exe bsm.exe deh.exe fth.exe lth.exe pmh.exe cih.exe wih.exe sih.exe prm.exe lighttpd.exe rh_sim.exe);

usage()
{
    echo "usage: $0 start|stop|status|restart|collectlogs|lock|unlock|alarms|logmsgcnt|cli|clean";
    # NOTE Do not add 'config' option above.
    # This option is only for dev team.

    echo "start       - start enb executables"
    echo "stop        - kill enb executables (not graceful stop)"
    echo "status      - show which exes are running and not running"
    echo "restart     - stop + start"
    echo "collectlogs - collect ENB logs"
    echo "lock        - lock ENB using CLI"
    echo "unlock      - unlock ENB using CLI"
    echo "alarms      - To get the Alarms"
    echo "logmsgcnt   - To send the SIGUSR1 to all from modules"
    echo "cli         - start CLI"
    echo "clean       - To clean all logs and coredumps"
    exit;
}

stopbs()
{
    pkill -9 "bs.sh"
    exit;
}

printEnbStatus()
{
  if [[ `pgrep cih.exe` ]]; then
    echo "CIH is already running so can not show Operational state of ENB";
  else
    if [[ `pgrep prm.exe` ]]; then
      adminState=$(echo `$PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "get 106" | grep -s ADMINSTATE | cut -d '[' -f2 |cut -d ']' -f1`)
      if [[ $adminState == "ADMINSTATE_LOCKED" ]]; then
        echo "Admin State: LOCKED"
      elif [[ $adminState == "ADMINSTATE_UNLOCKED" ]]; then
        operationalState=$(echo `$PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "get 106" | grep -s OPERATIONALSTATE | cut -d '[' -f2 |cut -d ']' -f1`)
        if [[ $operationalState == "OPERATIONALSTATE_ENABLED" ]]; then
          echo "Admin State: UNLOCKED"
          echo "Operational State: ENABLED"
        elif [[ $operationalState == "OPERATIONALSTATE_DISABLED" ]]; then
          echo "Admin State: UNLOCKED"
          echo "Operational State: DISABLED"
        else
          echo "eNodeB did not respond for GET on EnbStatus."
        fi
      else
        echo "eNodeB did not respond for GET on EnbStatus."
      fi
    else
      echo -e "\neNodeB is not Running.\n";
    fi
  fi
}
trap stopbs 3 #SIGQUIT handler


unixSockDir="/tmp/enb"
if [ ! -d $unixSockDir ]; then
  echo -n "creating $unixSockDir for unix sockets ..."
  mkdir $unixSockDir && echo "DONE" || { echo "FAILED ... EXITING"; exit; }
fi

Secs=5

# Logging Issued command to log.bs.sh file.
echo "[`date`][Command Issued = '$*' ]" >>log.bs.sh
case $cmd in
  start)
    sysctl net.core.rmem_max=524288
    # setting the unix socket buffer
    sysctl net.unix.max_dgram_qlen=5000
    echo core_%e_%t > /proc/sys/kernel/core_pattern
    ulimit -c unlimited
    ../simulators/rh_sim.exe &
    ./prm.exe &
    sleep 12
    $0 status;
    ;;
  stop)
    for name in ${exes[@]}; do
      pkill -9 $name;
    done
    ;;
  restart)
    $0 stop;
    $0 start;
    ;;
  status)
    echo "STATE COMMAND PID PCPU PMEM"
    echo "---------------------------"
    for name in ${exes[@]}; do
      state=$(ps --no-heading -C $name -o cmd,pid,pcpu,pmem);
      case "$?" in
        0)
           echo "RUNNING     $state";
           ;;
        *) echo "NOT-RUNNING $name"
           ;;
      esac
    done
    printEnbStatus
    ;;  
  collectlogs)
    # Checking for Prefix in command received
    if [ -z "$2" ]; then
      echo "ERR: Prefix not specified";
      echo "USAGE: ./bs.sh collectlogs <prefix>, prefix can be JIRA bug id, example BS806"
      exit
    fi
    prefix=$2
    filename=$prefix"_enb_log_"$(date +%d%b%y_%H%M%S)".tgz"
    env>$PICOBS_HOME/run/env.log
    echo "Starting Log collection in" $filename "......."
    tar --ignore-failed-read -czf $PICOBS_HOME/run/$filename ../log/ ../download/fth_trace.log \
        ../rel/inst_A/log/ ../rel/inst_A/config/ ../rel/inst_B/log/ ../rel/inst_B/config/ \
        ../run/*.log  ../run/*.cfg ../run/core* ../run/log.bs.sh &> /dev/null
    echo "All the available Logs/Configs/Coredumps are collected in "$filename" file."
    #cleaning the env.log
    rm -rf $PICOBS_HOME/run/env.log
    ;;
  config)
     if [ -z "$2" ]; then
        echo "ERROR: Test Setup not specified."
        echo "usage: bs.sh config <TSx> where x is 1..7"
        echo "example: bs.sh config TS3"
        echo "The above generates configuration for TS3"
        exit
     fi
     case $2 in
       TS[1-7])
         echo "Setting up the configuration for $2"
         ../rel/inst_A/createTSConfig.sh $2
       ;;
       *)
         echo "Invalid Test Setup."
         echo "usage: bs.sh config <TSx> where x is 1..7"
       ;;
    esac
  ;;
  lock)
    if [[ `pgrep cih.exe` ]]; then
      echo "CLI is already running so can not LOCK ENB";
    else
      if [[ `pgrep prm.exe` ]]; then
        $PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "set 1.1.2=1"
        sleep 1;
        printEnbStatus
      else
        echo -e "\neNodeB is not Running.\n";
      fi
    fi    
    ;;
  unlock)
    if [[ `pgrep cih.exe` ]]; then
      echo "CLI is already running so can not UNLOCK ENB";
    else
      if [[ `pgrep prm.exe` ]]; then
        $PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "set 1.1.2=2"    
	    sleep 3;
	    printEnbStatus
      else
        echo -e "\neNodeB is not Running.\n";\
      fi
    fi   
    ;;
  alarms)
    if [[ `pgrep cih.exe` ]]; then
      echo "CLI is already running so can not show Alarms";
    else
      $PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "get 120" | grep -q -E -s 'CRITICAL|MAJOR'
      if [ $? -eq 0 ]; then
        $PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "get 120"
        echo "Alarms Found!";      
      else
        echo "No Alarms Found.";
      fi  	
    fi
    ;;
  logmsgcnt)
    for name in bsm.exe deh.exe fth.exe lth.exe pmh.exe wih.exe sih.exe prm.exe ; do
      if [[ `pgrep $name` ]]; then
        kill -SIGUSR1 $(echo `pgrep $name`) 
      else
        echo "Signal SIGUSR1 is not sent to $name because $name is not running."
      fi
	done
    ;;  
  cli)
    pgrep cih.exe
    if [ "$?" -eq 0 ]; then
       echo "CLI is already running";
    else
    $PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A $2 "$3;"
    fi
    ;;
   monitor)
    echo "Monitoring ENB Status every $Secs secs. Press ctrl+\ to stop... "  
    echo "OK " 
    while :
    do
      if [ `pgrep bsm.exe` ] && [ `pgrep prm.exe` ]; then
	
         $PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "get 120" | grep  ALARMSEVERITY_CRITICAL
        if [ $? -eq 0 ]; then
          echo "Critical Alarm Occured";
          $PICOBS_HOME/rel/inst_A/bin/cih.exe --rootDir $PICOBS_HOME/rel/inst_A -c "get 120" > restartBS.log
          echo "Restarting the ENB" 
          $0 restart;
          echo "Restarted the ENB "  
        fi
      else 
         echo "Restarting the ENB" 
          $0 restart;
          echo "Restarted the ENB "  
      fi 
        echo -n "."   
        sleep $Secs
    done
    ;;
  clean)
    ps --no-heading -C prm.exe | grep -q prm.exe
    if [ $? -eq 0 ]; then      
      echo "ERR: Base Station is running. Stop Base Station to do clean.";      
    else
      rm -f $PICOBS_HOME/log/* $PICOBS_HOME/download/fth_trace.log \
        $PICOBS_HOME/rel/inst_A/log/*  $PICOBS_HOME/rel/inst_B/log/* \
        $PICOBS_HOME/run/*.log  $PICOBS_HOME/run/core* $PICOBS_HOME/run/*.dmp
      echo "All logs and coredumps deleted."   
    fi  	
    ;;        
  *)
    echo "error: unknown cmd '$cmd'";
    usage;
    ;;
esac

