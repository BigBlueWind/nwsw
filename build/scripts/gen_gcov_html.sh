#!/bin/bash
# Purpose: Create html page to link all gcov files (*.c.gcov) and show percentage of coverage 
#
# 01 Jun2011, Ankur: Created
#To Enable the display for the Shell commands uncomment next line
#set -x

if [ ! -d coverage ]; then
    mkdir coverage
fi
cd coverage
totalCov=0;
((totalNoOfFile = 0))

echo " " > index.html
echo "<html><body><table>" >> index.html 
for i in `find .. -name "*.gcda" -maxdepth 1 | sort`; do
    percent=`gcov $i -o .. | grep "%" | awk -F'%' {'print $1'}`
    name=`echo $i | awk -F'/' {'print $2'}`
    basename=`echo $name | awk -F'.' {'print $1'}` 
    percent=$(echo $percent |cut -d ':' -f2 | tr -d ' Lines executed' )
    
    if [ "x$percent" = "x" ]; then
        echo "<tr>" >> index.html
        echo "<td bgcolor=#ffffff> Error generating data for  	$basename<br>" >> index.html
        continue;   
    fi
    intpercent=`echo "$percent/1" | bc`
    totalCov=$(($totalCov+$intpercent))
    if [ $intpercent -le 49 ];then
        color="#f75d59"
    else if [ $intpercent -le 79 ]; then
        color="#ffff77"
        else
            color="#aaffaa"
        fi
    fi
    echo "<tr>" >> index.html
    if [[ $basename != wih* ]]; then
      echo "<td bgcolor=\"$color\"><a href=\"$basename.c.gcov\">         $basename</a>" >> index.html
    else
      echo "<td bgcolor=\"$color\"><a href=\"$basename.cpp.gcov\">         $basename</a>" >> index.html
    fi
    echo "<td bgcolor=\"$color\">$percent% tested <br>" >> index.html
    ((totalNoOfFile++))

done
echo "</table>" >> index.html
totalPercent=$(echo "scale=2; $totalCov/$totalNoOfFile" |bc -l)
echo "<p> Total Number of Files = $totalNoOfFile</p>" >> index.html
echo "<p> Total Code Coverage $totalPercent%</p>" >> index.html
echo "<p> generated on `date`</p>" >> index.html
echo "</body></html>" >> index.html

