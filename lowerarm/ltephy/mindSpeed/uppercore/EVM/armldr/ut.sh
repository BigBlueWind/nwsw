#!/bin/bash
failed=0
loops=0
numloops=100
rmmod armldr 2> /dev/null
while [ $loops -lt $numloops ]; do
	modprobe armldr
	sleep 3
	if echo "`dmesg | tail`" | grep -q fail
	then
		echo loop $loops has failed
		echo loop $loops has failed >> error.log
		echo "`dmesg | tail`" >> error.log
		echo "****************************************************" >> error.log
		failed=1
	else
		echo loop $loops has passed > /dev/null
	fi
	rmmod armldr
	let "loops += 1"	
done
if [ "$failed" -lt "1" ] 
then
	echo TEST PASSED
else
	echo TEST FAILED
fi
