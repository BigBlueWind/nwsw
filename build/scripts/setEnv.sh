#!/bin/sh
#Environment variables to be executed before running eNodeB'


echo $PWD
#For log4c
export LOG4C_RCPATH=$PWD/build/conf 
export LTEBSCONFIGDIR=$PWD/config
export LTEBSLOGDIR=$PWD/log



