/**
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains functions to find version of Base station modules.
 * 
 * @author     : 
 * Date       : 01-07-2010
 * Description: Basic version
 *              Generated from make_version.sh
 */

#ifndef VERSION_H
#define VERSION_H

#include "bs_tables.h"

typedef struct s_VersionInfo
{
  SwType     swType;
/* version = releaseVersion.featureVersion.bugFixVersion.customerId.svnVersion */
  UINT8      version[MAX_SIZE_SWPACKAGESTATUS_VERSION + 1 ];
  UINT8      builtOn[MAX_SIZE_SWPACKAGESTATUS_BUILTON + 1 ];
} VersionInfo;

typedef struct s_VersionCfgContents
{
  SwType    swType;
  UINT32    releaseVersion;
  UINT32    featureVersion;
  UINT32    bugFixVersion;
  UINT32    customerId;
  UINT32    svnVersion;
  UINT8     builtOn[MAX_SIZE_SWPACKAGESTATUS_BUILTON + 1 ];
} VersionCfgContents;

#endif /* VERSION_H */
