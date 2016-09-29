#!/usr/bin/env bash
VERSION_H=$1

echo "/**" > $VERSION_H;
echo " * @file    version.h" >> $CFG_PATH_H;
echo " * @details Copyright Powerwave Technologies, Inc., 2010" >> $VERSION_H;
echo " * @brief This file contains functions to find version of Base station modules." >> $VERSION_H;
echo " * " >> $VERSION_H;
echo " * @author     : " >> $VERSION_H;
echo " * Date       : 01-07-2010" >> $VERSION_H;
echo " * Description: Basic version" >> $VERSION_H;
echo " *              Generated from make_version.sh" >> $VERSION_H;
echo " */" >> $VERSION_H;

echo "" >> $VERSION_H;
echo "#ifndef VERSION_H" >> $VERSION_H;
echo "#define VERSION_H" >> $VERSION_H;
echo "" >> $VERSION_H;

CUR_VERSION=$(grep SW_VERSION $VERSION_H | tr -s '[[:space:]]' ':' | cut -d":" -f3)
echo "Current Version is $CUR_VERSION"
SVN_VERSION=$(svn info | grep Revision | cut -d":" -f2 | tr  -d '[[:space:]]')
echo "SVN Version is $SVN_VERSION"
TODAY=$(date)
echo "Date is $TODAY"

if [[ "$CUR_VERSION" == "$SVN_VERSION" ]]; then
  echo $VERSION_H is upto date
else
  echo "Updating $VERSION_H to version $SVN_VERSION";
  echo                                       >> $VERSION_H;
  echo "/* Generated on $TODAY */"           >> $VERSION_H;
  echo "#define VENDOR_NAME \"Powerwave\""   >> $VERSION_H;
  echo "#define SW_VERSION   $SVN_VERSION"   >> $VERSION_H;
  echo "#define BUILT_ON    \"$TODAY\""      >> $VERSION_H;
  
  echo "UINT8  *getVendorName(void);"        >> $VERSION_H;
  echo "UINT32  getSwVersion(void);"         >> $VERSION_H;
  echo "UINT8  *getDateOfBuild(void);"       >> $VERSION_H;

fi

echo "" >> $VERSION_H;
echo "#endif /* VERSION_H */" >> $VERSION_H;

  
	


