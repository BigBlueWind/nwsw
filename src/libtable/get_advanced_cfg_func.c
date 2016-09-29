/**
 * @file    get_advanced_cfg_func.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains GET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 11/29/2012 12:22:22 PM
 */

 
#include "get_oam_func.h"

/*******************************************************************/
/***************    PRECONFIG CONFIG TABLE GET FUNCTIONS    ********/
/*******************************************************************/

/* GetData PreConfig */
MsgTrace*                 getDataMsgTraceTable( IN RowId rowId )
{
  return   &(g_MsgTrace[rowId]);
}
 
TimersConfig*             getDataTimersConfigTable( IN RowId rowId )
{
  return   &(g_TimersConfig[rowId]);
}
 
ProcessStart*             getDataProcessStartTable( IN RowId rowId )
{
  return   &(g_ProcessStart[rowId]);
}
 
ModuleStop*               getDataModuleStopTable( IN RowId rowId )
{
  return   &(g_ModuleStop[rowId]);
}
 
EventThresholds*          getDataEventThresholdsTable( IN RowId rowId )
{
  return   &(g_EventThresholds[rowId]);
}
 
EnbAddresses*             getDataEnbAddressesTable( IN RowId rowId )
{
  return   &(g_EnbAddresses[rowId]);
}
 
UdhConfig*                getDataUdhConfigTable( IN RowId rowId )
{
  return   &(g_UdhConfig[rowId]);
}
 
UehConfig*                getDataUehConfigTable( IN RowId rowId )
{
  return   &(g_UehConfig[rowId]);
}
 
EgtpConfig*               getDataEgtpConfigTable( IN RowId rowId )
{
  return   &(g_EgtpConfig[rowId]);
}
 
SctpConfig*               getDataSctpConfigTable( IN RowId rowId )
{
  return   &(g_SctpConfig[rowId]);
}
 
S1apConfig*               getDataS1apConfigTable( IN RowId rowId )
{
  return   &(g_S1apConfig[rowId]);
}
 
DeviceMonitorConfig*      getDataDeviceMonitorConfigTable( IN RowId rowId )
{
  return   &(g_DeviceMonitorConfig[rowId]);
}
 
/* GetRow PreConfig */
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill msgTrace the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowMsgTraceTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_MSGTRACE_TABLE; colId++)
  {
     getElementMsgTraceTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill timersConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowTimersConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_TIMERSCONFIG_TABLE; colId++)
  {
     getElementTimersConfigTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill processStart the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowProcessStartTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_PROCESSSTART_TABLE; colId++)
  {
     getElementProcessStartTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleStop the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowModuleStopTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_MODULESTOP_TABLE; colId++)
  {
     getElementModuleStopTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill eventThresholds the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowEventThresholdsTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_EVENTTHRESHOLDS_TABLE; colId++)
  {
     getElementEventThresholdsTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAddresses the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowEnbAddressesTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ENBADDRESSES_TABLE; colId++)
  {
     getElementEnbAddressesTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill udhConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowUdhConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_UDHCONFIG_TABLE; colId++)
  {
     getElementUdhConfigTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill uehConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowUehConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_UEHCONFIG_TABLE; colId++)
  {
     getElementUehConfigTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill egtpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowEgtpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_EGTPCONFIG_TABLE; colId++)
  {
     getElementEgtpConfigTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill sctpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowSctpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_SCTPCONFIG_TABLE; colId++)
  {
     getElementSctpConfigTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowS1apConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_S1APCONFIG_TABLE; colId++)
  {
     getElementS1apConfigTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill deviceMonitorConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowDeviceMonitorConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_DEVICEMONITORCONFIG_TABLE; colId++)
  {
     getElementDeviceMonitorConfigTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/* GetElement PreConfig */
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill msgTrace the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementMsgTraceTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  MsgTrace  *config = getData(rowId);

  switch(colId)
  {
    case COL_MSGTRACE_MSGID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->msgId, tlvArray );
      break;
    case COL_MSGTRACE_MSGTRACELEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->msgTraceLevel, tlvArray );
      break;
    case COL_MSGTRACE_MSGTRACEDIRECTION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->msgTraceDirection, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill timersConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementTimersConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  TimersConfig  *config = getData(rowId);

  switch(colId)
  {
    case COL_TIMERSCONFIG_TIMERNAME:
      fillTagLenUint32( tableId, (rowId+1), colId, config->timerName, tlvArray );
      break;
    case COL_TIMERSCONFIG_TIMEUNIT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->timeUnit, tlvArray );
      break;
    case COL_TIMERSCONFIG_DURATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->duration, tlvArray );
      break;
    case COL_TIMERSCONFIG_DESCRIPTION:
      fillTagLenString( tableId, (rowId+1), colId, config->description, strlen(config->description), tlvArray );
      break;
    case COL_TIMERSCONFIG_MODULEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->moduleId, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill processStart the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementProcessStartTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  ProcessStart  *config = getData(rowId);

  switch(colId)
  {
    case COL_PROCESSSTART_PATH:
      fillTagLenString( tableId, (rowId+1), colId, config->path, strlen(config->path), tlvArray );
      break;
    case COL_PROCESSSTART_EXENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->exeName, strlen(config->exeName), tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleStop the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementModuleStopTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  ModuleStop  *config = getData(rowId);

  switch(colId)
  {
    case COL_MODULESTOP_MODULEIDSTRING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->moduleIdString, tlvArray );
      break;
    case COL_MODULESTOP_MODULEDESC:
      fillTagLenString( tableId, (rowId+1), colId, config->moduleDesc, strlen(config->moduleDesc), tlvArray );
      break;
    case COL_MODULESTOP_EXITPRIORITY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->exitPriority, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill eventThresholds the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementEventThresholdsTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  EventThresholds  *config = getData(rowId);

  switch(colId)
  {
    case COL_EVENTTHRESHOLDS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_EVENTTHRESHOLDS_CPUUSAGEHIGH:
      fillTagLenUint32( tableId, (rowId+1), colId, config->cpuUsageHigh, tlvArray );
      break;
    case COL_EVENTTHRESHOLDS_CPUUSAGENORMAL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->cpuUsageNormal, tlvArray );
      break;
    case COL_EVENTTHRESHOLDS_MEMUSAGEHIGH:
      fillTagLenUint32( tableId, (rowId+1), colId, config->memUsageHigh, tlvArray );
      break;
    case COL_EVENTTHRESHOLDS_MEMUSAGENORMAL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->memUsageNormal, tlvArray );
      break;
    case COL_EVENTTHRESHOLDS_ENBLOADHIGHMBPS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->enbLoadHighMbps, tlvArray );
      break;
    case COL_EVENTTHRESHOLDS_ENBLOADNORMALMBPS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->enbLoadNormalMbps, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAddresses the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementEnbAddressesTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  EnbAddresses  *config = getData(rowId);

  switch(colId)
  {
    case COL_ENBADDRESSES_BACKHAULPORTMACADDRESS:
      fillTagLenString( tableId, (rowId+1), colId, config->backhaulPortMacAddress, strlen(config->backhaulPortMacAddress), tlvArray );
      break;
    case COL_ENBADDRESSES_DEBUGPORTMACADDRESS:
      fillTagLenString( tableId, (rowId+1), colId, config->debugPortMacAddress, strlen(config->debugPortMacAddress), tlvArray );
      break;
    case COL_ENBADDRESSES_UNIQUEPRODUCTID:
      fillTagLenString( tableId, (rowId+1), colId, config->uniqueProductId, strlen(config->uniqueProductId), tlvArray );
      break;
    case COL_ENBADDRESSES_STATICIPADDRESSUSEDATDEBUGPORT:
      fillTagLenString( tableId, (rowId+1), colId, config->staticIPAddressUsedAtDebugPort, strlen(config->staticIPAddressUsedAtDebugPort), tlvArray );
      break;
    case COL_ENBADDRESSES_NETMASKFORDEBUGPORT:
      fillTagLenString( tableId, (rowId+1), colId, config->netmaskForDebugPort, strlen(config->netmaskForDebugPort), tlvArray );
      break;
    case COL_ENBADDRESSES_RADIOHEADIPADDRESS:
      fillTagLenString( tableId, (rowId+1), colId, config->radioHeadIpAddress, strlen(config->radioHeadIpAddress), tlvArray );
      break;
    case COL_ENBADDRESSES_RADIOHEADMACADDRESS:
      fillTagLenString( tableId, (rowId+1), colId, config->radioHeadMacAddress, strlen(config->radioHeadMacAddress), tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill udhConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementUdhConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  UdhConfig  *config = getData(rowId);

  switch(colId)
  {
    case COL_UDHCONFIG_ULINACTIVITYINTERVALINSECS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->ulInactivityIntervalInSecs, tlvArray );
      break;
    case COL_UDHCONFIG_UETHROUGHTPUTTIMERINSECS:
      fillTagLenUint8( tableId, (rowId+1), colId, config->ueThroughtputTimerInSecs, tlvArray );
      break;
    case COL_UDHCONFIG_MEMORYDIAGCFG:
      fillTagLenUint8( tableId, (rowId+1), colId, config->memoryDiagCfg, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill uehConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementUehConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  UehConfig  *config = getData(rowId);

  switch(colId)
  {
    case COL_UEHCONFIG_SYSTEMINFOVALUETAG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->systemInfoValueTag, tlvArray );
      break;
    case COL_UEHCONFIG_PRACHRESOURCE:
      fillTagLenUint16( tableId, (rowId+1), colId, config->prachResource, tlvArray );
      break;
    case COL_UEHCONFIG_SCSPACING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->scSpacing, tlvArray );
      break;
    case COL_UEHCONFIG_CYCPFXTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->cycPfxType, tlvArray );
      break;
    case COL_UEHCONFIG_SRSCFGPRD:
      fillTagLenUint16( tableId, (rowId+1), colId, config->srsCfgPrd, tlvArray );
      break;
    case COL_UEHCONFIG_SRBPOLLRETRANS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->srbPollRetrans, tlvArray );
      break;
    case COL_UEHCONFIG_SRBPOLLPDU:
      fillTagLenUint32( tableId, (rowId+1), colId, config->srbPollPdu, tlvArray );
      break;
    case COL_UEHCONFIG_SRBPOLLBYTE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->srbPollByte, tlvArray );
      break;
    case COL_UEHCONFIG_SRBMAXRETXTHRESH:
      fillTagLenUint32( tableId, (rowId+1), colId, config->srbMaxReTxThresh, tlvArray );
      break;
    case COL_UEHCONFIG_SRBTREORDRNG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->srbTreordrng, tlvArray );
      break;
    case COL_UEHCONFIG_SRBTSTATPROHIBIT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->srbTStatProhibit, tlvArray );
      break;
    case COL_UEHCONFIG_SRB1LOGCHCFGPRIOR:
      fillTagLenUint8( tableId, (rowId+1), colId, config->srb1LogchcfgPrior, tlvArray );
      break;
    case COL_UEHCONFIG_SRB2LOGCHCFGPRIOR:
      fillTagLenUint8( tableId, (rowId+1), colId, config->srb2LogchcfgPrior, tlvArray );
      break;
    case COL_UEHCONFIG_MACCFGTTIBUNDLING:
      fillTagLenUint8( tableId, (rowId+1), colId, config->macCfgTtiBundling, tlvArray );
      break;
    case COL_UEHCONFIG_DELTAMCSENABLED:
      fillTagLenUint16( tableId, (rowId+1), colId, config->deltaMcsEnabled, tlvArray );
      break;
    case COL_UEHCONFIG_ACCUMULATIONENABLED:
      fillTagLenUint16( tableId, (rowId+1), colId, config->accumulationEnabled, tlvArray );
      break;
    case COL_UEHCONFIG_CQIPUCCHRESIDX:
      fillTagLenUint16( tableId, (rowId+1), colId, config->cqiPUCCHResIdx, tlvArray );
      break;
    case COL_UEHCONFIG_CQIPMICONFIGIDX:
      fillTagLenUint16( tableId, (rowId+1), colId, config->cqiPmiConfigIdx, tlvArray );
      break;
    case COL_UEHCONFIG_RICONFIGIDX:
      fillTagLenUint16( tableId, (rowId+1), colId, config->riConfigIdx, tlvArray );
      break;
    case COL_UEHCONFIG_SIMULTACKNACKANDCQI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->simultAckNackAndCQI, tlvArray );
      break;
    case COL_UEHCONFIG_TPCRNTI:
      fillTagLenUint16( tableId, (rowId+1), colId, config->tpcRNTI, tlvArray );
      break;
    case COL_UEHCONFIG_INDEXOFFORMAT3:
      fillTagLenUint8( tableId, (rowId+1), colId, config->indexOfFormat3, tlvArray );
      break;
    case COL_UEHCONFIG_SRSPERIODICITY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->srsPeriodicity, tlvArray );
      break;
    case COL_UEHCONFIG_TRANSMISSIONCOMB:
      fillTagLenUint8( tableId, (rowId+1), colId, config->transmissionComb, tlvArray );
      break;
    case COL_UEHCONFIG_DEDCYCLICSHIFT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dedCyclicShift, tlvArray );
      break;
    case COL_UEHCONFIG_SRPUCCHRESIDX:
      fillTagLenUint16( tableId, (rowId+1), colId, config->srPucchResIdx, tlvArray );
      break;
    case COL_UEHCONFIG_SRCONFIGIDX:
      fillTagLenUint8( tableId, (rowId+1), colId, config->srConfigIdx, tlvArray );
      break;
    case COL_UEHCONFIG_DSRTRANSMAX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dsrTransMax, tlvArray );
      break;
    case COL_UEHCONFIG_DRBGRP:
      fillTagLenUint16( tableId, (rowId+1), colId, config->drbGrp, tlvArray );
      break;
    case COL_UEHCONFIG_UECATEGORY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCategory, tlvArray );
      break;
    case COL_UEHCONFIG_REPFACT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->repFact, tlvArray );
      break;
    case COL_UEHCONFIG_N1PUCCHANREP:
      fillTagLenUint16( tableId, (rowId+1), colId, config->n1PucchAnRep, tlvArray );
      break;
    case COL_UEHCONFIG_FORMATIND:
      fillTagLenUint8( tableId, (rowId+1), colId, config->formatInd, tlvArray );
      break;
    case COL_UEHCONFIG_ANTENNASEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->antennaSel, tlvArray );
      break;
    case COL_UEHCONFIG_APERIODICMODE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->aPeriodicMode, tlvArray );
      break;
    case COL_UEHCONFIG_PERIODICMODE:
      fillTagLenUint16( tableId, (rowId+1), colId, config->periodicMode, tlvArray );
      break;
    case COL_UEHCONFIG_PRDICITYENUM:
      fillTagLenUint16( tableId, (rowId+1), colId, config->prdicityEnum, tlvArray );
      break;
    case COL_UEHCONFIG_CQIOFFST:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cqiOffst, tlvArray );
      break;
    case COL_UEHCONFIG_CQICFGK:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cqiCfgk, tlvArray );
      break;
    case COL_UEHCONFIG_MAXULHQTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->maxUlHqTx, tlvArray );
      break;
    case COL_UEHCONFIG_UEULPWRTRGCQI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->ueUlPwrTrgCqi, tlvArray );
      break;
    case COL_UEHCONFIG_ACKNACKREPFACTOR:
      fillTagLenUint8( tableId, (rowId+1), colId, config->ackNackRepFactor, tlvArray );
      break;
    case COL_UEHCONFIG_MACINST:
      fillTagLenUint8( tableId, (rowId+1), colId, config->macInst, tlvArray );
      break;
    case COL_UEHCONFIG_MAXUEPERULSF:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxUePerUlSf, tlvArray );
      break;
    case COL_UEHCONFIG_MAXUEPERDLSF:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxUePerDlSf, tlvArray );
      break;
    case COL_UEHCONFIG_MAXULBWPERUE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxUlBwPerUe, tlvArray );
      break;
    case COL_UEHCONFIG_MAXDLBWPERUE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxDlBwPerUe, tlvArray );
      break;
    case COL_UEHCONFIG_MAXDLRETXBW:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxDlRetxBw, tlvArray );
      break;
    case COL_UEHCONFIG_MAXDLUENEWTXPERTTI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxDlUeNewTxPerTti, tlvArray );
      break;
    case COL_UEHCONFIG_MAXULUENEWTXPERTTI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxUlUeNewTxPerTti, tlvArray );
      break;
    case COL_UEHCONFIG_ISCYCLICPREFIXULEXT :
      fillTagLenUint16( tableId, (rowId+1), colId, config->isCyclicPrefixUlExt , tlvArray );
      break;
    case COL_UEHCONFIG_ISCYCLICPREFIXDLEXT :
      fillTagLenUint16( tableId, (rowId+1), colId, config->isCyclicPrefixDlExt , tlvArray );
      break;
    case COL_UEHCONFIG_CELLMODSCHM:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cellModSchm, tlvArray );
      break;
    case COL_UEHCONFIG_BCCHPCHRACODERATE:
      fillTagLenUint16( tableId, (rowId+1), colId, config->bcchPchRaCodeRate, tlvArray );
      break;
    case COL_UEHCONFIG_PDCCHCODERATE:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pdcchCodeRate, tlvArray );
      break;
    case COL_UEHCONFIG_DLCMNCODRATECCCHCQI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->dlCmnCodRateCcchCqi, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHSUBBNDSTRT:
      fillTagLenUint8( tableId, (rowId+1), colId, config->puschSubBndStrt, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHNOSUBBNDS:
      fillTagLenUint8( tableId, (rowId+1), colId, config->puschNoSubbnds, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHSUBBANDSZ:
      fillTagLenUint8( tableId, (rowId+1), colId, config->puschSubBandSz, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHSUBBANDDMRS:
      fillTagLenUint8( tableId, (rowId+1), colId, config->puschSubBandDmrs, tlvArray );
      break;
    case COL_UEHCONFIG_ULCMNCODRATECCCHCQI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->ulCmnCodRateCcchCqi, tlvArray );
      break;
    case COL_UEHCONFIG_ISDLFREQSEL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isDlFreqSel, tlvArray );
      break;
    case COL_UEHCONFIG_THRESHOLDCQI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->thresholdCqi, tlvArray );
      break;
    case COL_UEHCONFIG_PREAMBLEFRMT:
      fillTagLenUint8( tableId, (rowId+1), colId, config->preambleFrmt, tlvArray );
      break;
    case COL_UEHCONFIG_RAWINSZ:
      fillTagLenUint8( tableId, (rowId+1), colId, config->raWinSz, tlvArray );
      break;
    case COL_UEHCONFIG_RAOCCSZ:
      fillTagLenUint8( tableId, (rowId+1), colId, config->raOccSz, tlvArray );
      break;
    case COL_UEHCONFIG_RAOCCSFNENUM:
      fillTagLenUint16( tableId, (rowId+1), colId, config->raOccSfnEnum, tlvArray );
      break;
    case COL_UEHCONFIG_PUCCHPWRFMT3RNTIST:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pucchPwrFmt3RntiSt, tlvArray );
      break;
    case COL_UEHCONFIG_PUCCHPWRFMT3RNTISZ:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pucchPwrFmt3RntiSz, tlvArray );
      break;
    case COL_UEHCONFIG_PUCCHPWRFMT3ARNTIST:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pucchPwrFmt3aRntiSt, tlvArray );
      break;
    case COL_UEHCONFIG_PUCCHPWRFMT3ARNTISZ:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pucchPwrFmt3aRntiSz, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHPWRFMT3RNTIST:
      fillTagLenUint16( tableId, (rowId+1), colId, config->puschPwrFmt3RntiSt, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHPWRFMT3RNTISZ:
      fillTagLenUint16( tableId, (rowId+1), colId, config->puschPwrFmt3RntiSz, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHPWRFMT3ARNTIST:
      fillTagLenUint16( tableId, (rowId+1), colId, config->puschPwrFmt3aRntiSt, tlvArray );
      break;
    case COL_UEHCONFIG_PUSCHPWRFMT3ARNTISZ:
      fillTagLenUint16( tableId, (rowId+1), colId, config->puschPwrFmt3aRntiSz, tlvArray );
      break;
    case COL_UEHCONFIG_UEULBYTERATE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueUlByteRate, tlvArray );
      break;
    case COL_UEHCONFIG_PREAMBLESETCFGPRES:
      fillTagLenUint8( tableId, (rowId+1), colId, config->preambleSetCfgPres, tlvArray );
      break;
    case COL_UEHCONFIG_PREAMBLESETCFGSTART:
      fillTagLenUint8( tableId, (rowId+1), colId, config->preambleSetCfgStart, tlvArray );
      break;
    case COL_UEHCONFIG_PREAMBLESETCFGSZ:
      fillTagLenUint8( tableId, (rowId+1), colId, config->preambleSetCfgSz, tlvArray );
      break;
    case COL_UEHCONFIG_PRACHNUMRES:
      fillTagLenUint8( tableId, (rowId+1), colId, config->prachNumRes, tlvArray );
      break;
    case COL_UEHCONFIG_CRNTIRTNTIMER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->crntiRtnTimer, tlvArray );
      break;
    case COL_UEHCONFIG_NUMOFTXANT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfTxAnt, tlvArray );
      break;
    case COL_UEHCONFIG_STANDALONESRB1TIMER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->standaloneSrb1Timer, tlvArray );
      break;
    case COL_UEHCONFIG_CONTEXTRELEASETIMER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->contextReleaseTimer, tlvArray );
      break;
    case COL_UEHCONFIG_MMERELINDTIMER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mmeRelIndTimer, tlvArray );
      break;
    case COL_UEHCONFIG_RRCCONNRELL2ACKTIMER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rrcConnRelL2AckTimer, tlvArray );
      break;
    case COL_UEHCONFIG_ISUESECENBLE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isUeSecEnble, tlvArray );
      break;
    case COL_UEHCONFIG_UECATPOLLBYTEIDX:
      fillTagLenUint8( tableId, (rowId+1), colId, config->ueCatPollByteIdx, tlvArray );
      break;
    case COL_UEHCONFIG_DUPMODE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->dupMode, tlvArray );
      break;
    case COL_UEHCONFIG_ULUMSNFIELDLEN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ulUmSnFieldLen, tlvArray );
      break;
    case COL_UEHCONFIG_DLUMSNFIELDLEN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dlUmSnFieldLen, tlvArray );
      break;
    case COL_UEHCONFIG_UMRLCTREORDERING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->umRlcTReordering, tlvArray );
      break;
    case COL_UEHCONFIG_ULCPLENGTH:
      fillTagLenUint8( tableId, (rowId+1), colId, config->ulCpLength, tlvArray );
      break;
    case COL_UEHCONFIG_SRBPRIORITISEDBITRATE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->srbPrioritisedBitRate, tlvArray );
      break;
    case COL_UEHCONFIG_INTERNALUECONFIGTIMER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->internalUeConfigTimer, tlvArray );
      break;
    case COL_UEHCONFIG_UEPWRRECONFTIMERINSEC:
      fillTagLenUint16( tableId, (rowId+1), colId, config->uePwrReconfTimerinSec, tlvArray );
      break;
    case COL_UEHCONFIG_ISDLLAENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isDlLaEnabled, tlvArray );
      break;
    case COL_UEHCONFIG_UEHSTATEAUDITTMRDURINSECS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->uehStateAuditTmrDurInSecs, tlvArray );
      break;
    case COL_UEHCONFIG_UEHPROCEDUREAUDITTMRDURINSECS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->uehProcedureAuditTmrDurInSecs, tlvArray );
      break;
    case COL_UEHCONFIG_UEHPUCCHAUDITTMRDURINSECS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->uehPucchAuditTmrDurInSecs, tlvArray );
      break;
    case COL_UEHCONFIG_MEMORYDIAGCFG:
      fillTagLenUint8( tableId, (rowId+1), colId, config->memoryDiagCfg, tlvArray );
      break;
    case COL_UEHCONFIG_MAXMSG3PERULSF:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxMsg3PerUlSf, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill egtpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementEgtpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  EgtpConfig  *config = getData(rowId);

  switch(colId)
  {
    case COL_EGTPCONFIG_NMBUSAPS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->nmbUSaps, tlvArray );
      break;
    case COL_EGTPCONFIG_MAXNMBTSAPS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbTSaps, tlvArray );
      break;
    case COL_EGTPCONFIG_NMBSRVS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->nmbSrvs, tlvArray );
      break;
    case COL_EGTPCONFIG_TIMERRES:
      fillTagLenUint16( tableId, (rowId+1), colId, config->timerRes, tlvArray );
      break;
    case COL_EGTPCONFIG_RESTHRESHUPPER:
      fillTagLenUint16( tableId, (rowId+1), colId, config->resThreshUpper, tlvArray );
      break;
    case COL_EGTPCONFIG_RESTHRESHLOWER:
      fillTagLenUint16( tableId, (rowId+1), colId, config->resThreshLower, tlvArray );
      break;
    case COL_EGTPCONFIG_EGUT3TIMERENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->eguT3TimerEnabled, tlvArray );
      break;
    case COL_EGTPCONFIG_EGUT3TIMERVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->eguT3TimerVal, tlvArray );
      break;
    case COL_EGTPCONFIG_NMBRETRIES:
      fillTagLenUint8( tableId, (rowId+1), colId, config->nmbRetries, tlvArray );
      break;
    case COL_EGTPCONFIG_ECHOTMRENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->echoTmrEnabled, tlvArray );
      break;
    case COL_EGTPCONFIG_ECHOTMRVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->echoTmrVal, tlvArray );
      break;
    case COL_EGTPCONFIG_REORDERTMRENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reOrderTmrEnabled, tlvArray );
      break;
    case COL_EGTPCONFIG_REORDERTMRVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reOrderTmrVal, tlvArray );
      break;
    case COL_EGTPCONFIG_PVTEXTID:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pvtExtId, tlvArray );
      break;
    case COL_EGTPCONFIG_PDUWINDSIZE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pduWindSize, tlvArray );
      break;
    case COL_EGTPCONFIG_MAXNUMOFIP:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxNumOfIp, tlvArray );
      break;
    case COL_EGTPCONFIG_NODEARCH:
      fillTagLenUint8( tableId, (rowId+1), colId, config->nodeArch, tlvArray );
      break;
    case COL_EGTPCONFIG_NMBWORKERTHREAD:
      fillTagLenUint8( tableId, (rowId+1), colId, config->nmbWorkerThread, tlvArray );
      break;
    case COL_EGTPCONFIG_LOADDISTTMRENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->loadDistTmrEnabled, tlvArray );
      break;
    case COL_EGTPCONFIG_LOADDISTTMRVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->loadDistTmrVal, tlvArray );
      break;
    case COL_EGTPCONFIG_TINITTMRENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->tInitTmrEnabled, tlvArray );
      break;
    case COL_EGTPCONFIG_TINITTMRVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->tInitTmrVal, tlvArray );
      break;
    case COL_EGTPCONFIG_MAXBNDRETRY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxBndRetry, tlvArray );
      break;
    case COL_EGTPCONFIG_BNDTMCFGENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->bndTmCfgEnabled, tlvArray );
      break;
    case COL_EGTPCONFIG_BNDTMCFGVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->bndTmCfgVal, tlvArray );
      break;
    case COL_EGTPCONFIG_TPAR_U_SOCKPARAM_LISTENQSIZE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->tPar_u_sockParam_listenQSize, tlvArray );
      break;
    case COL_EGTPCONFIG_TPAR_U_SOCKPARAM_NUMOPTS:
      fillTagLenUint8( tableId, (rowId+1), colId, config->tPar_u_sockParam_numOpts, tlvArray );
      break;
    case COL_EGTPCONFIG_IPINFO_ECHOREQPVTEXTVALLENGTH:
      fillTagLenUint16( tableId, (rowId+1), colId, config->ipInfo_echoReqPvtExtValLength, tlvArray );
      break;
    case COL_EGTPCONFIG_IPINFO_ECHOREQPVTEXTVALBUFF:
      fillTagLenString( tableId, (rowId+1), colId, config->ipInfo_echoReqPvtExtValBuff, strlen(config->ipInfo_echoReqPvtExtValBuff), tlvArray );
      break;
    case COL_EGTPCONFIG_IPINFO_ECHORSPPVTEXTVALLENGTH:
      fillTagLenUint16( tableId, (rowId+1), colId, config->ipInfo_echoRspPvtExtValLength, tlvArray );
      break;
    case COL_EGTPCONFIG_IPINFO_ECHORSPPVTEXTVALBUFF:
      fillTagLenString( tableId, (rowId+1), colId, config->ipInfo_echoRspPvtExtValBuff, strlen(config->ipInfo_echoRspPvtExtValBuff), tlvArray );
      break;
    case COL_EGTPCONFIG_IPINFO_ERRINDPVTEXTVALLENGTH:
      fillTagLenUint16( tableId, (rowId+1), colId, config->ipInfo_errIndPvtExtValLength, tlvArray );
      break;
    case COL_EGTPCONFIG_IPINFO_ERRINDPVTEXTVALBUFF:
      fillTagLenString( tableId, (rowId+1), colId, config->ipInfo_errIndPvtExtValBuff, strlen(config->ipInfo_errIndPvtExtValBuff), tlvArray );
      break;
    case COL_EGTPCONFIG_TSERV_TPAR_U_SOCKPARAM_NUMOPTS:
      fillTagLenUint8( tableId, (rowId+1), colId, config->tserv_tPar_u_sockParam_numOpts, tlvArray );
      break;
    case COL_EGTPCONFIG_TSERV_TPAR_U_SOCKPARAM_LISTENQSIZE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->tserv_tPar_u_sockParam_listenQSize, tlvArray );
      break;
    case COL_EGTPCONFIG_OPNSRVRETRYCNT:
      fillTagLenUint16( tableId, (rowId+1), colId, config->opnSrvRetryCnt, tlvArray );
      break;
    case COL_EGTPCONFIG_OPNSRVTMRENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->opnSrvTmrEnabled, tlvArray );
      break;
    case COL_EGTPCONFIG_OPNSRVTMRVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->opnSrvTmrVal, tlvArray );
      break;
    case COL_EGTPCONFIG_HOSTNAMEPRESENT:
      fillTagLenUint8( tableId, (rowId+1), colId, config->hostNamePresent, tlvArray );
      break;
    case COL_EGTPCONFIG_HOSTNAMELEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->hostNameLen, tlvArray );
      break;
    case COL_EGTPCONFIG_GTP_C_OR_U:
      fillTagLenUint8( tableId, (rowId+1), colId, config->gtp_C_Or_U, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill sctpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementSctpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  SctpConfig  *config = getData(rowId);

  switch(colId)
  {
    case COL_SCTPCONFIG_MAXNMBSCTSAPS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbSctSaps, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBTSAPS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbTSaps, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBASSOC:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbAssoc, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBENDP:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbEndp, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBDSTADDR:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbDstAddr, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBSRCADDR:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbSrcAddr, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBTXCHUNKS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->maxNmbTxChunks, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBRXCHUNKS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->maxNmbRxChunks, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBINSTRMS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbInStrms, tlvArray );
      break;
    case COL_SCTPCONFIG_MAXNMBOUTSTRMS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxNmbOutStrms, tlvArray );
      break;
    case COL_SCTPCONFIG_INITARWND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->initARwnd, tlvArray );
      break;
    case COL_SCTPCONFIG_MTUINITIAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->mtuInitial, tlvArray );
      break;
    case COL_SCTPCONFIG_MTUMININITIAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->mtuMinInitial, tlvArray );
      break;
    case COL_SCTPCONFIG_MTUMAXINITIAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->mtuMaxInitial, tlvArray );
      break;
    case COL_SCTPCONFIG_PERFORMMTU:
      fillTagLenUint8( tableId, (rowId+1), colId, config->performMtu, tlvArray );
      break;
    case COL_SCTPCONFIG_TIMERES:
      fillTagLenUint16( tableId, (rowId+1), colId, config->timeRes, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXINITRETX:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_MaxInitReTx, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXASSOCRETX:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_MaxAssocReTx, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXPATHRETX:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_MaxPathReTx, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_ALTACCEPTFLG:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_AltAcceptFlg, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_KEYTM:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_KeyTm, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_ALPHA:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_Alpha, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_BETA:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_Beta, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_TINTTMR:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_TIntTmr, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXBNDRETRY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_MaxBndRetry, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXACKDELAYTM:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_MaxAckDelayTm, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXACKDELAYDG:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_MaxAckDelayDg, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_RTOINITIAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_RtoInitial, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_RTOMIN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_RtoMin, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_RTOMAX:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_RtoMax, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_FREEZETM:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_FreezeTm, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_BUNDLETIME:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_BundleTime, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_COOKIELIFE:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_CookieLife, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_HBEATINTERVALTM:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_HbeatIntervalTm, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXBURST:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_MaxBurst, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_MAXHBBURST:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_MaxHbBurst, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_T5SDOWNGRDTM:
      fillTagLenUint16( tableId, (rowId+1), colId, config->reConfig_T5SdownGrdTm, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_HANDLEINITFLG:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_HandleInitFlg, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_NEGABRTFLG:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_NegAbrtFlg, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_HBEATENABLE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->reConfig_HBeatEnable, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_FLCUPTHR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->reConfig_FlcUpThr, tlvArray );
      break;
    case COL_SCTPCONFIG_RECONFIG_FLCLOWTHR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->reConfig_FlcLowThr, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementS1apConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  S1apConfig  *config = getData(rowId);

  switch(colId)
  {
    case COL_S1APCONFIG_NMBSZTSAPS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->nmbSztSaps, tlvArray );
      break;
    case COL_S1APCONFIG_NMBSCTSAPS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->nmbSctSaps, tlvArray );
      break;
    case COL_S1APCONFIG_NMBCONN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbConn, tlvArray );
      break;
    case COL_S1APCONFIG_NMBPEER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbPeer, tlvArray );
      break;
    case COL_S1APCONFIG_TIMERES:
      fillTagLenUint16( tableId, (rowId+1), colId, config->timeRes, tlvArray );
      break;
    case COL_S1APCONFIG_MAXBNDRETRY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->maxBndRetry, tlvArray );
      break;
    case COL_S1APCONFIG_BNDTMCFGENABLED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->bndTmCfgEnabled, tlvArray );
      break;
    case COL_S1APCONFIG_BNDTMCFGVAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->bndTmCfgVal, tlvArray );
      break;
    case COL_S1APCONFIG_S1APLOCALPORT:
      fillTagLenUint16( tableId, (rowId+1), colId, config->s1apLocalPort, tlvArray );
      break;
    case COL_S1APCONFIG_S1APGUARDTIMER:
      fillTagLenUint16( tableId, (rowId+1), colId, config->s1apGuardTimer, tlvArray );
      break;
    case COL_S1APCONFIG_ASSOCCFG_NMBRETRY:
      fillTagLenUint16( tableId, (rowId+1), colId, config->assocCfg_nmbRetry, tlvArray );
      break;
    case COL_S1APCONFIG_ASSOCCFG_TMRASSOCRECOV_ENB:
      fillTagLenUint8( tableId, (rowId+1), colId, config->assocCfg_tmrAssocRecov_enb, tlvArray );
      break;
    case COL_S1APCONFIG_ASSOCCFG_TMRASSOCRECOV_VAL:
      fillTagLenUint16( tableId, (rowId+1), colId, config->assocCfg_tmrAssocRecov_val, tlvArray );
      break;
    case COL_S1APCONFIG_MAXS1SETUPALLOWED:
      fillTagLenUint16( tableId, (rowId+1), colId, config->maxS1SetupAllowed, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill deviceMonitorConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementDeviceMonitorConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_PreCfgTable[tableId-MIN_PRE_CFG_TABLEID].getData;
  DeviceMonitorConfig  *config = getData(rowId);

  switch(colId)
  {
    case COL_DEVICEMONITORCONFIG_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_BASEBAND:
      fillTagLenUint8( tableId, (rowId+1), colId, config->baseBand, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_RADIOHEAD:
      fillTagLenUint8( tableId, (rowId+1), colId, config->radioHead, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_BACKHAULPORT:
      fillTagLenUint8( tableId, (rowId+1), colId, config->backHaulPort, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_DEBUGPORT:
      fillTagLenUint8( tableId, (rowId+1), colId, config->debugPort, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_TEMPSENSOR:
      fillTagLenUint8( tableId, (rowId+1), colId, config->tempSensor, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_GPS:
      fillTagLenUint8( tableId, (rowId+1), colId, config->gps, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_CPRI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cpri, tlvArray );
      break;
    case COL_DEVICEMONITORCONFIG_CLOCK:
      fillTagLenUint8( tableId, (rowId+1), colId, config->clock, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/* GetTable PreConfig */
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill msgTrace the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableMsgTraceTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_MSGTRACE_TABLE; rowId++)
  {
     getRowMsgTraceTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill timersConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableTimersConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_TIMERSCONFIG_TABLE; rowId++)
  {
     getRowTimersConfigTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill processStart the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableProcessStartTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_PROCESSSTART_TABLE; rowId++)
  {
     getRowProcessStartTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleStop the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableModuleStopTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_MODULESTOP_TABLE; rowId++)
  {
     getRowModuleStopTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill eventThresholds the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableEventThresholdsTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_EVENTTHRESHOLDS_TABLE; rowId++)
  {
     getRowEventThresholdsTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAddresses the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableEnbAddressesTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ENBADDRESSES_TABLE; rowId++)
  {
     getRowEnbAddressesTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill udhConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableUdhConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_UDHCONFIG_TABLE; rowId++)
  {
     getRowUdhConfigTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill uehConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableUehConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_UEHCONFIG_TABLE; rowId++)
  {
     getRowUehConfigTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill egtpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableEgtpConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_EGTPCONFIG_TABLE; rowId++)
  {
     getRowEgtpConfigTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill sctpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableSctpConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_SCTPCONFIG_TABLE; rowId++)
  {
     getRowSctpConfigTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableS1apConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_S1APCONFIG_TABLE; rowId++)
  {
     getRowS1apConfigTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill deviceMonitorConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableDeviceMonitorConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_DEVICEMONITORCONFIG_TABLE; rowId++)
  {
     getRowDeviceMonitorConfigTable( tableId, rowId, tlvArray);
  }
}
 
/* struct2Tlv PreConfig */
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill msgTrace the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvMsgTraceTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill timersConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvTimersConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill processStart the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvProcessStartTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleStop the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvModuleStopTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill eventThresholds the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvEventThresholdsTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAddresses the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvEnbAddressesTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill udhConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvUdhConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill uehConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvUehConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill egtpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvEgtpConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill sctpConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvSctpConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvS1apConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill deviceMonitorConfig the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvDeviceMonitorConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_PreCfgTable[tagInfo->tableId-MIN_PRE_CFG_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/* Extern Global Variables PreConfig */

