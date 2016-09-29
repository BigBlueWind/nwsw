/**
 * @file    generice_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This is auto generated file from Excel
 * @brief This file conatins generic functions used on OAM tables
 *
 * @author: Shweta Polepally
 * Date: 11-1-11
 * Description: Draft version
 */

#include "generic_table.h"

/*** Global Variables ***/
GenericTable  g_OprCmdTable[MAX_NUM_OPR_CMD_TABLES];
GenericTable  g_OprCfgTable[MAX_NUM_OPR_CFG_TABLES];
GenericTable  g_StatusTable[MAX_NUM_STATUS_TABLES];
GenericTable  g_PreCfgTable[MAX_NUM_PRE_CFG_TABLES];
/*************************/
/**
 * @param   tableId   Table Id
 * @details This function returns the type of the table as per the range of the OAM
 *          table category.
 */
OamTableType getTableTypeByTableId(
  TableId       tableId
  )
{
  if (tableId >= MIN_STATUS_TABLEID && tableId <= MAX_STATUS_TABLEID)
    return TABLE_STATUS;
  else if (tableId >= MIN_PRE_CFG_TABLEID && tableId <= MAX_PRE_CFG_TABLEID)
    return TABLE_PRECONFIG;
  else if (tableId >= MIN_OPR_CFG_TABLEID && tableId <= MAX_OPR_CFG_TABLEID)
    return TABLE_OPERCONFIG;
  else if (tableId >= MIN_OPR_CMD_TABLEID && tableId <= MAX_OPR_CMD_TABLEID)
    return TABLE_OPERCMD;
  else
    return TABLE_UNKNOWNTYPE;
}

/**
 * @param   tableId   Table Id
 * @details This function returns table functions of the table mentioned in tableId
 */
GenericTable *getTableFunctions(
  IN TableId tableId
  )
{
  OamTableType  tableType = getTableTypeByTableId(tableId);
  GenericTable  *tableFunc = NULL;
  switch(tableType)
  {
    case TABLE_OPERCONFIG:   return &(g_OprCfgTable[tableId-MIN_OPR_CFG_TABLEID]);
    case TABLE_OPERCMD:      return &(g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID]);
    case TABLE_STATUS:       return &(g_StatusTable[tableId-MIN_STATUS_TABLEID]);
    case TABLE_PRECONFIG:    return &(g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID]);
    case TABLE_UNKNOWNTYPE:
    {
      /* This condition should never happen. But just for logging */
      BS_ERROR_UNKNOWN_TABLE_TYPE( TABLE_UNKNOWNTYPE );
      return NULL;
    }
  }
  return tableFunc;
}

/**
 * @param   tableId   Table Id
 * @details This function returns the data pointer of the the first row of the table.
 */
DataHdlr *getFirstRowOfTable(
  IN TableId tableId
  )
{
  OamTableType  tableType = getTableTypeByTableId(tableId);
  switch(tableType)
  {
    case TABLE_OPERCONFIG:   return g_OprCfgTable[tableId-MIN_OPR_CFG_TABLEID].getData(0);
    case TABLE_OPERCMD:      return g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData(0);
    case TABLE_STATUS:       return g_StatusTable[tableId-MIN_STATUS_TABLEID].getData(0);
    case TABLE_PRECONFIG:    return g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData(0);
    case TABLE_UNKNOWNTYPE:
    {
      /* This condition should never happen. But just for logging */
      BS_ERROR_UNKNOWN_TABLE_TYPE( TABLE_UNKNOWNTYPE );
      return NULL;
    }
  }
  return NULL;
}

/**
 * @param   tableId   Table Id
 * @param   rowId     Row Id of the table
 * @details This function return the data pointer of the the requested row of the table.
 */
DataHdlr *getRowOfTable(
  IN TableId tableId,
  IN RowId   rowId
  )
{
  OamTableType  tableType = getTableTypeByTableId(tableId);
  switch(tableType)
  {
    case TABLE_OPERCONFIG:   return g_OprCfgTable[tableId-MIN_OPR_CFG_TABLEID].getData(rowId);
    case TABLE_OPERCMD:      return g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData(rowId);
    case TABLE_STATUS:       return g_StatusTable[tableId-MIN_STATUS_TABLEID].getData(rowId);
    case TABLE_PRECONFIG:    return g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData(rowId);
    case TABLE_UNKNOWNTYPE:
    {
      /* This condition should never happen. But just for logging */
      BS_ERROR_UNKNOWN_TABLE_TYPE( TABLE_UNKNOWNTYPE );
      return NULL;
    }
  }
  return NULL;
}
