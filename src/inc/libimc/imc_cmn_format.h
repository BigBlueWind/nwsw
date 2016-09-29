/**
 * @file    imc_cmn_format.h
 * @brief Common formats functions.
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author : Shweta Polepally
 * Date:    11 April 2011
 * Description:   Initial version.
 *
 */


#ifndef IMC_CMN_FORMAT_H
#define IMC_CMN_FORMAT_H

#include "all_cmn.h"

void getAndFillDateAndTimeStamp(
  IN UINT32   maxLenOfField,
  IO UINT8   *dataAndTimeStampField
  );
void convertVersionInfoToVersionCfgContents(
  IN VersionInfo         *versionInfo,
  IO VersionCfgContents  *versionCfgContents
  );
void convertVersionCfgContentsToVersionInfo(
  IN VersionCfgContents  *versionCfgContents,
  IO VersionInfo         *versionInfo
  );
#endif
