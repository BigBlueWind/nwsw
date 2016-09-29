#!/bin/bash
# Creates a tar-ball of eNB run-time software
set -e # exit if any command returns with error
#set -x 


srcDir=../../src
buildDir=../


usage ()
{
 echo "usage: $0 <4gmx|arm|x86|jffs> <relName> <releaseVersion>"; 
 echo "example: $0 x86 rel-1.2.3 2";
 exit;
}

if [ -z "$1" ]; then
 echo "xxx: platform not specified";
 usage;
fi

if [ -z "$2" ]; then
 echo "xxx: relName not specified";
 usage;
fi

if [ -z "$3" ]; then
 echo "Release version not specified";
 usage;
fi

platform=$1;
relDir=$2;
releaseVersionFromUser=$3
installFile=$relDir.tgz

if [ -e $relDir ]; then
  echo "xxx: $relDir exists.";
  echo "xxx: This script does not overwrite existing directories";
  echo "xxx: Exiting";
  exit; 
fi

case $platform in
 x86)
   ;;
 arm)
   ;;
 4gmx)
   ;; 
 jffs)
   ;;
 *) 
   echo "unknown platform $platform";
   usage;
   ;;
esac

#--------------------------
#Function to get dateTime of package built and svn version information
#-------------------------- 
function getVersionAndBuiltOnInfo
{
currVersion=$(echo `svnversion`) 
currVersion=$(echo $currVersion |cut -d ':' -f2| tr -d 'M'| tr -d 'S'| tr -d 'P'| tr -d ' ')
dateAndTime=$(echo `date`) 

year=$(echo $dateAndTime |cut -d ' ' -f6)
month=$(echo $dateAndTime |cut -d ' ' -f2)
date=$(echo $dateAndTime |cut -d ' ' -f3)
time=$(echo $dateAndTime |cut -d ' ' -f4)

  if [[ $month == Jan ]];then month="01"
elif [[ $month == Feb ]];then month="02"
elif [[ $month == Mar ]];then month="03"
elif [[ $month == Apr ]];then month="04"
elif [[ $month == May ]];then month="05"
elif [[ $month == Jun ]];then month="06"
elif [[ $month == Jul ]];then month="07"
elif [[ $month == Aug ]];then month="08"
elif [[ $month == Sep ]];then month="09"
elif [[ $month == Oct ]];then month="10"
elif [[ $month == Nov ]];then month="11"
elif [[ $month == Dec ]];then month="12"
fi

if [[ $date -le 9 ]];then
date="0$date" 
fi

hour=$(echo $time |cut -d ':' -f1)
min=$(echo $time |cut -d ':' -f2)
sec=$(echo $time |cut -d ':' -f3)
msec="000"
dateAndTime="$year$month$date$hour$min$sec$msec"

while read LINE
do
if [[ $LINE == *svnVersion* ]]; then
  echo "svnVersion = $currVersion;" >>$relDir/config/version1.cfg
elif [[ $LINE == *releaseVersion* ]]; then
  #copy the release version provided by user
  releaseVersion=$(echo $LINE |cut -d '#' -f1| cut -d '=' -f2 | tr -d ' '| tr -d ';')
  if [ $releaseVersion -eq "$releaseVersionFromUser" ]; then
    echo -e "\n Provide Single digit Release version other then $releaseVersion"
    rm -rf $relDir
    exit
  fi
  echo "releaseVersion = $releaseVersionFromUser;" >>$relDir/config/version1.cfg
elif [[ $LINE == *builtOn* ]]; then
  echo "builtOn	= \"$dateAndTime\";" >>$relDir/config/version1.cfg
else
  echo "$LINE" >>$relDir/config/version1.cfg
fi
done < $relDir/config/version.cfg

mv $relDir/config/version1.cfg $relDir/config/version.cfg

}

#--------------------------
#Function to Get version information fron version.cfg and update enb_user_config.cfg
#-------------------------- 
function getVersionAndUpdateEnbUserConfig
{
while read LINE
do
if [[ $LINE == *releaseVersion* ]]; then
  releaseVersionInCfg=$(echo $LINE |cut -d '=' -f2| tr -d ' '| tr -d ';')
elif [[ $LINE == *featureVersion* ]]; then
  featureVersionInCfg=$(echo $LINE |cut -d '=' -f2| tr -d ' '| tr -d ';')
elif [[ $LINE == *bugFixVersion* ]]; then
  bugFixVersionInCfg=$(echo $LINE |cut -d '=' -f2| tr -d ' '| tr -d ';')
elif [[ $LINE == *customerId* ]]; then
  customerIdInCfg=$(echo $LINE |cut -d '=' -f2| tr -d ' '| tr -d ';')
elif [[ $LINE == *svnVersion* ]]; then
  svnVersionInCfg=$(echo $LINE |cut -d '=' -f2| tr -d ' '| tr -d ';')
fi
done < $relDir/config/version.cfg

while read LINE
do
if [[ $LINE == *version* ]]; then
  echo "# version : $releaseVersionInCfg.$featureVersionInCfg.$bugFixVersionInCfg.$customerIdInCfg.$svnVersionInCfg" >>$relDir/config/enb_user_config1.cfg
else
  echo "$LINE" >>$relDir/config/enb_user_config1.cfg
fi
done < $relDir/config/enb_user_config.cfg

mv $relDir/config/enb_user_config1.cfg $relDir/config/enb_user_config.cfg

}

mkdir $relDir

cp -rf $buildDir/config         $relDir/
cp -rf $buildDir/scripts/cli_scripts         $relDir/


cp -f  $buildDir/scripts/rdmem_arm  $relDir/
chmod 755 $relDir/rdmem_arm

if [[ $platform == x86 ]]; then
cp -f  $buildDir/scripts/bs_x86.sh                 $relDir/bs.sh
cp -f  $buildDir/scripts/pkg_installer_x86.sh      $relDir/pkg_installer.sh
cp -f  $buildDir/scripts/patch_installer_x86.sh    $relDir/patch_installer.sh
elif [[ $platform == 4gmx ]]; then
cp -f  $buildDir/scripts/bs.sh                     $relDir/bs.sh
cp -f  $buildDir/scripts/pkg_installer_4gmx.sh     $relDir/pkg_installer.sh
cp -f  $buildDir/scripts/patch_installer_4gmx.sh   $relDir/patch_installer.sh 
cp -f  $buildDir/scripts/dump_lowerarm_512.sh      $relDir/dump_lowerarm.sh
elif [[ $platform == jffs ]]; then
cp -f  $buildDir/scripts/bs.sh                     $relDir/bs.sh
cp -f  $buildDir/scripts/pkg_installer_jffs.sh     $relDir/pkg_installer.sh
cp -f  $buildDir/scripts/patch_installer_4gmx.sh   $relDir/patch_installer.sh 
cp -f  $buildDir/scripts/dump_lowerarm_512.sh      $relDir/dump_lowerarm.sh

fi

cp -f  $buildDir/scripts/rh_installer.sh   $relDir/
cp -f  $buildDir/scripts/config_installer.sh  $relDir/


rm $relDir/config/bs_doxy_conf_file
rm $relDir/config/ubinize.cfg

chmod 755 $relDir/bs.sh
chmod 755 $relDir/pkg_installer.sh
chmod 755 $relDir/patch_installer.sh
chmod 755 $relDir/rh_installer.sh
chmod 755 $relDir/dump_lowerarm.sh
chmod 755 $relDir/config_installer.sh


cp -f  $buildDir/scripts/bootarm_bs.sh  $relDir/
chmod 755 $relDir/bootarm_bs.sh

mkdir $relDir/bin

if [[ $platform == x86 ]]; then 
find  $buildDir/bin/$platform/ -name *.exe -exec cp '{}' $relDir/bin \;
find  $buildDir/bin/$platform/ -name snmpset -exec cp '{}' $relDir/bin \;

elif [[ $platform == 4gmx ]]; then 
find  $buildDir/bin/$platform/ -name *.exe -exec cp '{}' $relDir/bin \;
find  $buildDir/bin/$platform/ -name snmpset -exec cp '{}' $relDir/bin \;
cp -f $buildDir/bin/4gmx/lowerarm/lowerarm_512.elf $relDir/bin/lowerarm.elf
find  $buildDir/bin/4gmx/ -name snmpset -exec cp '{}' $relDir/bin \;

elif [[ $platform == jffs ]]; then 
find  $buildDir/bin/4gmx/ -name *.exe -exec cp '{}' $relDir/bin \;
find  $buildDir/bin/4gmx/ -name snmpset -exec cp '{}' $relDir/bin \;
cp -f $buildDir/bin/4gmx/lowerarm/lowerarm_512.elf $relDir/bin/lowerarm.elf
find  $buildDir/bin/4gmx/ -name snmpset -exec cp '{}' $relDir/bin \;
fi


mkdir  $relDir/log
cp -rf $srcDir/web $relDir/

mkdir  $relDir/trace
mkdir  $relDir/pmlogs

find $relDir -name .svn | xargs rm -rf;

#Get version information, date and time at which package was built
getVersionAndBuiltOnInfo
#Get version information from version.cfg and update enb_user_config.cfg
getVersionAndUpdateEnbUserConfig

tar -c -v --remove-files --gzip --totals -f $installFile $relDir;

case "$?" in
  0) echo "--> Created $installFile for $platform";;
  *) echo "xxx: Failed"; rm -rf $relDir/;;
esac

