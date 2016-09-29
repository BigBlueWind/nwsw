/**
 * @file    all_cfg.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains functions prototypes which will read the default configuration
 *          when the system comes up.
 * @author          : Shweta Polepally
 * Creation Date   : 14th June 2010
 * Change History  :
 *
 */


#ifndef  ALL_CFG_H
#define  ALL_CFG_H

#include "all_cmn.h"
#include "oper_config_read_write.h"
#include "pre_config_read_write.h"
#include "bsm_prm_if.h"
#include "bsm_fth_if.h"
#include "enumstr_oamtables.h"

NumEntry readFromFileProcessStartTable(
  IN UINT8                                       *fileToRead,
  IO ProcessStart                                *ptrProcessStart,
  IO UINT8                                       *error
  );
NumEntry readFromFileModuleStopTable(
  IN UINT8                                       *fileToRead,
  IO ModuleStop                                  *ptrModuleStop,
  IO UINT8                                       *error
  );
void readCol(
  IN config_t      *cfg,
  IN UINT32        *tableNum,         /* Temporary  table number for reading from tpl file */
  IO OamColInfo    *oamColInfo
  );
int parseOamCfg(
  IN UINT8*          configFilePath,
  IO OamTableContext *oamCtx
  );
TBOOL readLthConfigTable(
  IN UINT8              *fileToRead,
  IO TraceConfig        *entry,
  IO UINT8              *error
  );
void readFthConfigTable(
  IN UINT8         *fileToRead,
  IO FthCfgTable   *cfgTable,
  IO UINT8         *error
  );
NumEntry readFromFileSwPackageStatusTable(
  IN UINT8              *fileToRead,
  IO SwPackageStatus       *cfgTable,
  IO UINT8              *error
  );
void writeToFileSwPackageStatusTable(
  IN SwPackageStatus         *ptrSwPackageStatus,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error
  );
NumEntry readFromFileComponentStatusTable(
  IN UINT8              *fileToRead,
  IO ComponentStatus    *cfgTable,
  IO UINT8              *error
  );
TBOOL writeToFileComponentStatusTable(
  IN ComponentStatus    *ptrCfgTable,
  IO UINT8              *fileToWrite,
  IO UINT8              *error
  );
NumEntry readFromFileVersionCfgContentsTable(
  IN UINT8                  *fileToRead,
  IO VersionCfgContents     *cfgTable,
  IO UINT8                  *error
  );

TBOOL readFromFileEnbLimitsConfigTable(
  IN UINT8                   *fileToRead,
  IO EnbLimitsConfig         *enbLimitsConfig,
  IO UINT8                   *error
  );

NumEntry readFromFileAlarmDb(
  IN UINT8              *fileToRead,
  IO AlarmDb            *cfgTable,
  IO UINT8              *error
  );
NumEntry readFromFilePmLogFileStatusTable(
  IN UINT8                   *fileToRead,
  IO PmLogFileStatus         *ptrPmLogFileStatus,
  IO UINT8                   *error
  );
TBOOL writeToFilePmLogFileStatusTable(
  IN PmLogFileStatus         *ptrPmLogFileStatus,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error
  );
#endif /*ALL_CFG_H*/
