#!/bin/bash
# Creates a jffs2 image of eNB run-time software
set -e # exit if any command returns with error
#set -x 

srcDir=../../src
buildDir=../


usage ()
{
 echo "usage: $0 "; 
 echo "example: $0 ";
 exit;
}
setupDir=picobs;
installFile=$setupDir.jffs

if [ -e $setupDir ]; then
  echo "xxx: $setupDir exists.";
  echo "xxx: This script does not overwrite existing directories";
  echo "xxx: Exiting";
  exit; 
fi



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
	  echo "svnVersion = $currVersion;" >>$setupDir/rel/inst_A/config/version1.cfg
elif [[ $LINE == *builtOn* ]]; then
    echo "builtOn	= \"$dateAndTime\";" >>$setupDir/rel/inst_A/config/version1.cfg
else
  echo "$LINE" >>$setupDir/rel/inst_A/config/version1.cfg
fi
done < $setupDir/rel/inst_A/config/version.cfg

mv $setupDir/rel/inst_A/config/version1.cfg $setupDir/rel/inst_A/config/version.cfg

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
done < $setupDir/rel/inst_A/config/version.cfg

while read LINE
do
if [[ $LINE == *version* ]]; then
  echo "# version : $releaseVersionInCfg.$featureVersionInCfg.$bugFixVersionInCfg.$customerIdInCfg.$svnVersionInCfg" >>$setupDir/rel/inst_A/config/enb_user_config1.cfg
else
  echo "$LINE" >>$setupDir/rel/inst_A/config/enb_user_config1.cfg
fi
done < $setupDir/rel/inst_A/config/enb_user_config.cfg

mv $setupDir/rel/inst_A/config/enb_user_config1.cfg $setupDir/rel/inst_A/config/enb_user_config.cfg

}
#Creating directories for inst_A
mkdir $setupDir
mkdir $setupDir/log
mkdir $setupDir/simulators
mkdir $setupDir/download
mkdir $setupDir/rel
mkdir $setupDir/run
mkdir $setupDir/rel/inst_A
mkdir $setupDir/rel/inst_A/log
mkdir $setupDir/rel/inst_A/bin
mkdir $setupDir/rel/inst_A/config
mkdir $setupDir/rel/inst_A/trace
mkdir $setupDir/rel/inst_A/pmlogs

cp -rf $buildDir/config/                       $setupDir/rel/inst_A/
#Copying scripts to inst_A
cp -f  $buildDir/scripts/bootarm_bs.sh         $setupDir/rel/inst_A/
cp -f  $buildDir/scripts/dump_lowerarm_512.sh  $setupDir/rel/inst_A/dump_lowerarm.sh
cp -f  $buildDir/scripts/bs.sh                 $setupDir/rel/inst_A/
cp -f  $buildDir/scripts/rdmem_arm             $setupDir/rel/inst_A/
cp -f  $buildDir/scripts/bs_start_ffs2.sh      $setupDir/rel/inst_A/bs_start.sh

rm $setupDir/rel/inst_A/config/bs_doxy_conf_file
rm $setupDir/rel/inst_A/config/ubinize.cfg

#Setting permissions
chmod 755 $setupDir/rel/inst_A/bootarm_bs.sh
chmod 755 $setupDir/rel/inst_A/dump_lowerarm.sh
chmod 755 $setupDir/rel/inst_A/bs.sh
chmod 755 $setupDir/rel/inst_A/rdmem_arm
chmod 755 $setupDir/rel/inst_A/bs_start.sh

cp -f  $buildDir/scripts/pkg_installer_jffs.sh     $setupDir/rel/inst_A/pkg_installer.sh
cp -f  $buildDir/scripts/patch_installer_4gmx.sh   $setupDir/rel/inst_A/patch_installer.sh
cp -f  $buildDir/scripts/rh_installer.sh           $setupDir/rel/inst_A/ 
cp -f  $buildDir/scripts/config_installer.sh       $setupDir/rel/inst_A/

chmod 755 $setupDir/rel/inst_A/pkg_installer.sh
chmod 755 $setupDir/rel/inst_A/patch_installer.sh
chmod 755 $setupDir/rel/inst_A/config_installer.sh
chmod 755 $setupDir/rel/inst_A/rh_installer.sh


#Copying all the execs
find  $buildDir/bin/4gmx/ -name *.exe -exec cp '{}'   $setupDir/rel/inst_A/bin \;
find  $buildDir/bin/4gmx/ -name snmpset -exec cp '{}' $setupDir/rel/inst_A/bin \;

cp -f $buildDir/bin/4gmx/lowerarm/lowerarm_512.elf       $setupDir/rel/inst_A/bin/lowerarm.elf
find  $buildDir/bin/4gmx/ -name snmpset -exec cp '{}'    $setupDir/rel/inst_A/bin \;

mv $setupDir/rel/inst_A/config/sw_package.cfg       $setupDir/run/

#Simulator related Setup 
#RH Simulator files 
mv $setupDir/rel/inst_A/bin/rh_sim.exe              $setupDir/simulators
mv $setupDir/rel/inst_A/config/simulatorConfig.cfg  $setupDir/simulators
mv $setupDir/rel/inst_A/config/lksimconfig.cfg      $setupDir/simulators

#EMS Simulator files
mv $setupDir/rel/inst_A/bin/ems_sim.exe             $setupDir/simulators
mv $setupDir/rel/inst_A/bin/snmpset                 $setupDir/simulators
mv $setupDir/rel/inst_A/config/enodebagentport.cfg  $setupDir/simulators

cp -rf $srcDir/web $setupDir/rel/inst_A/

find $setupDir -name .svn | xargs rm -rf;

#Get version information, date and time at which package was built
getVersionAndBuiltOnInfo
#Get version information from version.cfg and update enb_user_config.cfg
getVersionAndUpdateEnbUserConfig

cp -rf $setupDir/rel/inst_A/ $setupDir/rel/inst_B

#Setting Symbolic links 
cd $setupDir/run
ln -s ../rel/inst_A/bin/prm.exe           prm.exe
ln -s ../rel/inst_A/bin/logutil.exe       logutil.exe
ln -s ../rel/inst_A/bs.sh                 bs.sh
ln -s ../rel/inst_A/bootarm_bs.sh         bootarm_bs.sh
ln -s ../rel/inst_A/bs_start.sh           bs_start.sh
ln -s ../rel/inst_A/dump_lowerarm.sh      dump_lowerarm.sh
ln -s ../rel/inst_A/rdmem_arm             rdmem_arm
cd -

#Moving sw_package.cfg
./../../../tools/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-strip $setupDir/rel/inst_B/bin/*.exe
./../../../tools/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-strip $setupDir/simulators/rh_sim.exe

mkfs.jffs2 -p -r $setupDir -s 4096 -e 0x80000 -n -o $setupDir_sum.jffs2
sumtool -n -e 0x80000 -p -i $setupDir_sum.jffs2 -o $setupDir.jffs2
rm -f $setupDir_sum.jffs2

rm -rf $setupDir

case "$?" in
  0) echo "--> Created JFFSFile:$installFile for 4gmx ";;
  *) echo "xxx: Failed"; rm -rf $setupDir/;;
esac

