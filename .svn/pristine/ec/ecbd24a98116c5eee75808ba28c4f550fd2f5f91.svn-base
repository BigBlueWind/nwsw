#!/bin/bash
# Purpose: install a configuration file on eNB
#
# 30 Mar2011, Ankur:Created
# 07 Apr2011, Ankur:Added support for configuration file verification
#To Enable the display for the Shell commands uncomment next line
#set -x

configFile=$1;
runningVersion=$3;
ckeckLevel=$5;
picobsHome=$PICOBS_HOME;      # path where the package is unpacked

((noOfTable = 0))
((noOfRowInCfg = 0))

###################################################################################
#  Function to exit from script and write to install_failed
###################################################################################
function exitFromScript {
  echo $1 >$picobsHome/log/install_failed   #errString
  echo $2 >>$picobsHome/log/install_failed  #bsError
  rm -f /tmp/enbConfigVerificationTemplate.txt
  echo "=== LOG CLOSED FOR CONFIG INSTALLER === $(date)" >>$picobsHome/log/config_install.log
  exit
}

###################################################################################
#  Function to read line of configuration file
###################################################################################
function readNextLineOfConfigFile
{
  read -u 3 CFG_LINE
  CFG_LINE=$(echo $CFG_LINE | cut -d '#' -f1 | tr -d " "| tr -d "\r\n")
  if [[ $CFG_LINE == "" ]];then
    readNextLineOfConfigFile
  fi
}

###################################################################################
#  Function to read line of template
###################################################################################
function readNextLineOfTplFile
{
  read -u 4 TPL_LINE
  TPL_LINE=$(echo $TPL_LINE | tr -d " "| tr -d "\n")
}

###################################################################################
#  Function to parse data and store in array
###################################################################################
function readTableFromTpl
{
  colName[$1]=$(echo $TPL_LINE | cut -d ',' -f1 )
  dataType[$1]=$(echo $TPL_LINE | cut -d ',' -f2 )
  minValue[$1]=$(echo $TPL_LINE | cut -d ',' -f3 )
  maxValue[$1]=$(echo $TPL_LINE | cut -d ',' -f4 )
  isSpecialValue[$1]=$(echo $TPL_LINE | cut -d ',' -f5 )
  specialValue[$1]=$(echo $TPL_LINE | cut -d ',' -f6 )
}


###################################################################################
#  Function to compare column data with template data which is stored in array.
###################################################################################
function compareColumnDetail
{
  if [[ $(echo $CFG_LINE | cut -d '=' -f1 ) == ${colName[$1]} ]];then
    colValue=$(echo $CFG_LINE | cut -d '=' -f2 | tr -d ';');
    if [[ ${dataType[$1]} == STRING || ${dataType[$1]} == IPV4ADDRESS || ${dataType[$1]} == MACADDRESS || ${dataType[$1]} == FILENAME || ${dataType[$1]} == LOCALTIME || ${dataType[$1]} == OFFSETTIME || ${dataType[$1]} == SWVERSION || ${dataType[$1]} == MCC || ${dataType[$1]} == MNC || ${dataType[$1]} == LATITUDE || ${dataType[$1]} == LONGITUDE || ${dataType[$1]} == ENODEBNAME || ${dataType[$1]} == PROFILENAME ]];then
      if [[ ${#colValue}-2 -le ${maxValue[$1]} && ${colValue:0:1} == '"' && ${colValue:${#colValue}-1:${#colValue}} == '"' ]];then
        echo "   Result: Successful: Column name ${colName[$1]} and range matched " >>$picobsHome/log/config_install.log
      else
        echo "   Result: Failed: Column ${colName[$1]} value is out of range " >>$picobsHome/log/config_install.log
        exitFromScript "Column ${colName[$1]} value is out of range in table $tableName during configuration file verification" 2
      fi
    else
      if [[ $colValue -ge ${minValue[$1]} && $colValue -le ${maxValue[$1]} ]];then
        echo "   Result: Successful: Column name ${colName[$1]} and range matched " >>$picobsHome/log/config_install.log
      else
        if [[ ${isSpecialValue[$1]} == 1 && $colValue == ${specialValue[$1]} ]];then
          echo "   Result: Successful: Column ${colName[$1]} has special value (out of range allowed) " >>$picobsHome/log/config_install.log
        else
          echo "   Result: Failed: Column ${colName[$1]} value is out of range " >>$picobsHome/log/config_install.log
          exitFromScript "Column ${colName[$1]} value is out of range in table $tableName during configuration file verification" 2
        fi
      fi
    fi
  else
    echo "   Result: Failed: Column name ${colName[$1]} not matched " >>$picobsHome/log/config_install.log
    exitFromScript "Column name ${colName[$1]} not matched during configuration file verification" 2
  fi  
}



###################################################################################
#  Function to match data of full table with template(data base)
###################################################################################
function matchTableData {
  isTableStarted="FALSE"
  for(( index=1; index<=$2; index++))
  do
    readNextLineOfTplFile
    readTableFromTpl $index-1
  done

  if [[ $1 == 1 ]];then  ########## Single row table ########

# for single row table it search the pattern
#    qosConfig : 
#    {                 
#                       
#    };              

    readNextLineOfConfigFile
    if [[ $CFG_LINE == "{" ]];then
      isTableStarted="TRUE"
    fi
    for(( index=1; index<=$noOfColumnInTpl; index++))
    do
      readNextLineOfConfigFile
      compareColumnDetail $index-1
    done
    readNextLineOfConfigFile
    if [[ $CFG_LINE == "};" ]];then
      if [[ $isTableStarted != "TRUE" ]];then
        echo "   Result: Failed: No of Column are not $noOfColumnInTpl for $tableName " >>$picobsHome/log/config_install.log
        exitFromScript "All Columns not found in $tableName during configuration file verification" 2
      fi
      echo "   Result: Successful: $noOfColumnInTpl Column names matched for $tableName " >>$picobsHome/log/config_install.log
      isTableStarted="FALSE"
    else
      echo "   Result: Failed: No of Columns $noOfColumnInTpl did not match for $tableName " >>$picobsHome/log/config_install.log
      exitFromScript "All Columns not found in $tableName  during configuration file verification" 2
    fi
  else ########## Multi row table ########
  
# for multi row table it search any one of the pattern
#    logConfig  = (     |  logConfig  = (
#    {                  |  {
#                       |
#    },                 |  },
#    {                  |  {
#                       |  
#    }                  |  } );
#    );                 |

    for(( rows=1; rows<=$1; rows++))
    do
      readNextLineOfConfigFile
      if [[ $CFG_LINE == "{" ]];then
        isTableStarted="TRUE"
      fi
      for(( index=1; index<=$noOfColumnInTpl; index++))
      do
        readNextLineOfConfigFile
        compareColumnDetail $index-1
      done
      readNextLineOfConfigFile
      if [[ $CFG_LINE == },* ]];then
        echo "   Result: Successful: $noOfColumnInTpl Column names matched of row number $rows for $tableName " >>$picobsHome/log/config_install.log
      else 
        if [[ $rows == $1 && $CFG_LINE == "});"* ]];then
          echo "   Result: Successful: $noOfColumnInTpl Column names matched of row number $rows for $tableName " >>$picobsHome/log/config_install.log
          echo "   Result: Successful: No of rows $1 are matched for $tableName " >>$picobsHome/log/config_install.log
        elif [[ $rows == $1 && $CFG_LINE == }* ]];then
          echo "   Result: Successful: $noOfColumnInTpl Column names matched of row number $rows for $tableName " >>$picobsHome/log/config_install.log
          readNextLineOfConfigFile
          if [[ $CFG_LINE == ");"* ]];then
            echo "   Result: Successful: No of rows $1 are matched for $tableName " >>$picobsHome/log/config_install.log
          else
            echo "   Result: Failed: No of rows $1 are not matched for $tableName " >>$picobsHome/log/config_install.log
            exitFromScript "All Columns not found in $tableName during configuration file verification" 2
          fi
        else
          echo "   Result: Failed: No of Columns $noOfColumnInTp did not match of row number $rows for $tableName " >>$picobsHome/log/config_install.log
          exitFromScript "All Columns not found in $tableName during configuration file verification" 2
        fi
      fi
    done
  fi
}

###################################################################################
#  Function to match table data and get noOfRows and noOfColumn from template
###################################################################################
function matchTable {
  isTableNameMatched="FALSE"
  while read -u 4 TPL_LINE
  do
    TPL_LINE=$(echo $TPL_LINE | tr -d "\r\n" | tr -d ' ')
    if [[ $TPL_LINE == Table:* ]];then
      if [[ $(echo $TPL_LINE | cut -d ':' -f2 | cut -d ',' -f1 ) == $1 ]];then
        noOfRowInTpl=$(echo $TPL_LINE | cut -d ':' -f2 | cut -d ',' -f2 );
        noOfColumnInTpl=$(echo $TPL_LINE | cut -d ':' -f2 | cut -d ',' -f3 );
        echo "   Result: Successful: Table name $1 matched " >>$picobsHome/log/config_install.log
        isTableNameMatched="TRUE"
       ((noOfTable++))
       matchTableData $noOfRowInTpl $noOfColumnInTpl
       break;
      fi
    fi
  done
  if [[ $isTableNameMatched == "FALSE" ]];then
    echo "   Result: Failed: Table name $1 not matched " >>$picobsHome/log/config_install.log
    exitFromScript "Table name $1 not matched  during configuration file verification" 2
  fi
}

###################################################################################
#  Function to verify version of configuration file
###################################################################################
function verifyVersionInfo {
  releaseVersionExpected=$(echo $runningVersion | cut -d '.' -f1 | tr -d ' ')
  featureVersionExpected=$(echo $runningVersion | cut -d '.' -f2 | tr -d ' ')
  customerIdExpected=$(echo $runningVersion | cut -d '.' -f4 | tr -d ' ')
  while read -u 3 CFG_LINE
  do
    if [[ $CFG_LINE == *version* ]];then
      CFG_LINE=$(echo $CFG_LINE | cut -d '#' -f2 |cut -d ':' -f2 | tr -d "\r\n")
      if [[ $(echo $CFG_LINE | cut -d '.' -f1 | tr -d ' ') == $releaseVersionExpected ]];then
        if [[ $(echo $CFG_LINE | cut -d '.' -f2 | tr -d ' ') == $featureVersionExpected ]];then
          if [[ $(echo $CFG_LINE | cut -d '.' -f4 | tr -d ' ') == $customerIdExpected ]];then
            echo "   Result: Successful: Version successfully matched " >>$picobsHome/log/config_install.log
          else
            echo "   Result: Failed: customerId miss matched during configuration file verification" >>$picobsHome/log/config_install.log
            exitFromScript "customerId miss matched during configuration file verification" 2
          fi
        else
          echo "   Result: Failed: FeatureVersion miss matched during configuration file verification" >>$picobsHome/log/config_install.log
          exitFromScript "FeatureVersion miss matched during configuration file verification" 2
        fi
      else
        echo "   Result: Failed: ReleaseVersion miss matched during configuration file verification" >>$picobsHome/log/config_install.log
        exitFromScript "ReleaseVersion miss matched during configuration file verification" 2
      fi
      break;
    fi
  done
}
if [[ $ckeckLevel == FirstLevel ]];then
  echo "=== LOG OPENED FOR CONFIG INSTALLER === $(date)" >$picobsHome/log/config_install.log
  #################################### CHECK FILE PRESENCE #####################################
  echo "1: Checking for configuration file presence " >>$picobsHome/log/config_install.log
  if [ ! -f $picobsHome/download/$configFile ];then
    echo "   Result: Failed: file $configFile not found in $picobsHome/download/ folder " >>$picobsHome/log/config_install.log
    exitFromScript "Installation File for configuration file installation is not Found at EnodeB" 5
  fi
    echo "   Result: Successful: file $configFile found in $picobsHome/download/ folder " >>$picobsHome/log/config_install.log

  #################################### CHECK FILE FORMAT #####################################
  echo "2: Checking for configuration file format " >>$picobsHome/log/config_install.log
  if [ cfg != ${configFile/*./} ];then
    echo "   Result: Failed: Configuration File format not supported for file $picobsHome/download/$configFile " >>$picobsHome/log/config_install.log
    exitFromScript "Unsupported installation File format encountered during configuration file installation " 6
  fi
  echo "   Result: Successful: Configuration File format supported for file $picobsHome/download/$configFile " >>$picobsHome/log/config_install.log
  echo "no error" >$picobsHome/log/install_success
else
  #################################### CONFIG FILE VERIFICATION #####################################
  echo "3: Verifying configuration file " >>$picobsHome/log/config_install.log

  ############# To get no of tables from template. #############
  exec 4</tmp/enbConfigVerificationTemplate.txt
  read -u 4 TPL_LINE
  noOfTableInTpl=$(echo $TPL_LINE| tr -d ' ')

  ############# To get no of tables from config. #############
  exec 3<$picobsHome/download/$configFile
  verifyVersionInfo 
  while read -u 3 CFG_LINE
  do
    CFG_LINE=$(echo $CFG_LINE | cut -d '#' -f1 | tr -d "\r\n")
    if [[ $CFG_LINE == *:* ]];then # For single row table
      tableName=$(echo $CFG_LINE | cut -d ':' -f1|tr -d ' ')
      matchTable $tableName
    elif [[ $(echo $CFG_LINE | cut -d '=' -f2 | tr -d ' ' ) == "(" ]];then # for multi row table
      tableName=$(echo $CFG_LINE | cut -d '=' -f1|tr -d ' ')
      matchTable $tableName
    fi
  done
  exec 3>&-
  exec 4>&-

  ############# To compare no of table in config file and in template #############
  if [[ $noOfTable != $noOfTableInTpl ]];then
    echo "   Result: Failed: All tables not found during configuration file verification" >>$picobsHome/log/config_install.log
    exitFromScript "All tables not found during configuration file verification" 2
  fi

  echo "no error" >$picobsHome/log/install_success
  rm -f /tmp/enbConfigVerificationTemplate.txt
  echo "=== LOG CLOSED FOR CONFIG INSTALLER === $(date)" >>$picobsHome/log/config_install.log
fi

# bsError
# 1 = BS_ERR_INSTALL_PKG_UNPACKING_FAILED
# 2 = BS_ERR_INSTALL_VERIFICATION_FAILED
# 3 = BS_ERR_INSTALL_PKG_DUPLICATE
# 4 = BS_ERR_INSTALL_FLASH_ACCESS_FAILED
# 5 = BS_ERR_INSTALL_FILE_NOT_FOUND
# 6 = BS_ERR_INSTALL_UNSUPPORTED_FILE_FORMAT