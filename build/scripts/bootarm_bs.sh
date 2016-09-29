#!/bin/sh
if [ -n "$1" ]
then
    echo "Loading" "$1"
else
    echo "Usage: bootarm <filename>"
    exit
fi
sctpconn stop
fid=$RANDOM
##`date +%N`
if [ -f $1 ]; then
    ln -s $1 /lib/firmware/lowarm$fid.elf
    #ln -s `pwd`/$1 /lib/firmware/lowarm$fid.elf
    if echo "`lsmod`" | grep -q armldr 
    then
	rmmod armldr
    fi    
    if [ -n "$2" ]
    then
	lowarm_core=$2
    else
	lowarm_core=4
    fi

    if [ -n "$3" ]
    then
	diag=$3
    else
	diag=1
    fi

    modprobe armldr coreid=$lowarm_core noipi=0 fileid=$fid diag=$diag
    if echo "`dmesg | tail`" | grep -q fail
    then
        echo "Boot failed, try disconnect RVI or boot again"
        rm -rf /lib/firmware/lowarm$fid.elf
	exit 
    fi    
    rm -rf /dev/lowarm
    mknod /dev/lowarm c 243 0
    rm -rf /lib/firmware/lowarm$fid.elf
    insmod /root/console.ko
    rmmod /root/console.ko
    sctpconn restart
else
    echo "File" $1 "not found"
fi
    
