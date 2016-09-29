/**
 * @file      cih_oamtable_func.c
 * @details   Copyright Powerwave Technologies, Inc., 2010
 * @brief     This file contains functions related to OAM table.
 *
 * @author    Madhusudana Rao Kata
 * @date      10-01-2011
 */

#include "cih.h"
#include "cih_tecla.h"


OamTableInfo* getTablePtrByTableId(
  IN TableId      tableId
  )
{
  OamTableInfo *tableInfo = NULL ;
  OamTableType  tableType;

  tableType= getTableTypeByTableId(tableId);
  switch(tableType)
  {
    case TABLE_OPERCONFIG:
    tableInfo = cihGetOprCfgTableInfo(tableId-MIN_OPR_CFG_TABLEID);
    break;

    case TABLE_OPERCMD:
    tableInfo = cihGetOprCmdTableInfo(tableId-MIN_OPR_CMD_TABLEID);
    break;

    case TABLE_STATUS:
    tableInfo = cihGetStatusTableInfo(tableId-MIN_STATUS_TABLEID);
    break;

    case TABLE_PRECONFIG:
    tableInfo = cihGetPreCfgTableInfo(tableId-MIN_PRE_CFG_TABLEID);
    break;

    case TABLE_UNKNOWNTYPE:
    default:
    tableInfo = NULL ;
  }
  return tableInfo;
}
