/**
 * @file    get_status_func.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains GET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 2/8/2013 1:08:25 PM
 */

 
#include "get_oam_func.h"

/*******************************************************************/
/***************        STATUS TABLE GET FUNCTIONS          ********/
/*******************************************************************/

/* GetData Status */
FileUploadStatus*         getDataFileUploadStatusTable( IN RowId rowId )
{
  return   &(g_FileUploadStatus[rowId]);
}
 
FileDownloadStatus*       getDataFileDownloadStatusTable( IN RowId rowId )
{
  return   &(g_FileDownloadStatus[rowId]);
}
 
SwActivationStatus*       getDataSwActivationStatusTable( IN RowId rowId )
{
  return   &(g_SwActivationStatus[rowId]);
}
 
SwPackageStatus*          getDataSwPackageStatusTable( IN RowId rowId )
{
  return   &(g_SwPackageStatus[rowId]);
}
 
ComponentStatus*          getDataComponentStatusTable( IN RowId rowId )
{
  return   &(g_ComponentStatus[rowId]);
}
 
EnbStatus*                getDataEnbStatusTable( IN RowId rowId )
{
  return   &(g_EnbStatus[rowId]);
}
 
ModuleStatus*             getDataModuleStatusTable( IN RowId rowId )
{
  return   &(g_ModuleStatus[rowId]);
}
 
RadioHeadStatus*          getDataRadioHeadStatusTable( IN RowId rowId )
{
  return   &(g_RadioHeadStatus[rowId]);
}
 
MacGenCounters*           getDataMacGenCountersTable( IN RowId rowId )
{
  return   &(g_MacGenCounters[rowId]);
}
 
MacUplaneCounters*        getDataMacUplaneCountersTable( IN RowId rowId )
{
  return   &(g_MacUplaneCounters[rowId]);
}
 
RlcGenCounters*           getDataRlcGenCountersTable( IN RowId rowId )
{
  return   &(g_RlcGenCounters[rowId]);
}
 
PdcpGenCounters*          getDataPdcpGenCountersTable( IN RowId rowId )
{
  return   &(g_PdcpGenCounters[rowId]);
}
 
PdcpUplaneCounters*       getDataPdcpUplaneCountersTable( IN RowId rowId )
{
  return   &(g_PdcpUplaneCounters[rowId]);
}
 
GtpGenCounters*           getDataGtpGenCountersTable( IN RowId rowId )
{
  return   &(g_GtpGenCounters[rowId]);
}
 
SctpGenCounters*          getDataSctpGenCountersTable( IN RowId rowId )
{
  return   &(g_SctpGenCounters[rowId]);
}
 
FileEventHistory*         getDataFileEventHistoryTable( IN RowId rowId )
{
  return   &(g_FileEventHistory[rowId]);
}
 
SwInstallHistory*         getDataSwInstallHistoryTable( IN RowId rowId )
{
  return   &(g_SwInstallHistory[rowId]);
}
 
ActiveUeStatus*           getDataActiveUeStatusTable( IN RowId rowId )
{
  return   &(g_ActiveUeStatus[rowId]);
}
 
ActiveRabStatus*          getDataActiveRabStatusTable( IN RowId rowId )
{
  return   &(g_ActiveRabStatus[rowId]);
}
 
UeEventHistory*           getDataUeEventHistoryTable( IN RowId rowId )
{
  return   &(g_UeEventHistory[rowId]);
}
 
RabHistory*               getDataRabHistoryTable( IN RowId rowId )
{
  return   &(g_RabHistory[rowId]);
}
 
PmLogFileStatus*          getDataPmLogFileStatusTable( IN RowId rowId )
{
  return   &(g_PmLogFileStatus[rowId]);
}
 
ModuleLogFileStatus*      getDataModuleLogFileStatusTable( IN RowId rowId )
{
  return   &(g_ModuleLogFileStatus[rowId]);
}
 
ActiveAlarmStatus*        getDataActiveAlarmStatusTable( IN RowId rowId )
{
  return   &(g_ActiveAlarmStatus[rowId]);
}
 
AlarmHistory*             getDataAlarmHistoryTable( IN RowId rowId )
{
  return   &(g_AlarmHistory[rowId]);
}
 
LocationStatus*           getDataLocationStatusTable( IN RowId rowId )
{
  return   &(g_LocationStatus[rowId]);
}
 
GpsStatus*                getDataGpsStatusTable( IN RowId rowId )
{
  return   &(g_GpsStatus[rowId]);
}
 
RrcGenCounters*           getDataRrcGenCountersTable( IN RowId rowId )
{
  return   &(g_RrcGenCounters[rowId]);
}
 
RrmCounters*              getDataRrmCountersTable( IN RowId rowId )
{
  return   &(g_RrmCounters[rowId]);
}
 
RrcCellCounters*          getDataRrcCellCountersTable( IN RowId rowId )
{
  return   &(g_RrcCellCounters[rowId]);
}
 
S1apGenCounters*          getDataS1apGenCountersTable( IN RowId rowId )
{
  return   &(g_S1apGenCounters[rowId]);
}
 
S1apPeerNodeCounters*     getDataS1apPeerNodeCountersTable( IN RowId rowId )
{
  return   &(g_S1apPeerNodeCounters[rowId]);
}
 
EthernetCounters*         getDataEthernetCountersTable( IN RowId rowId )
{
  return   &(g_EthernetCounters[rowId]);
}
 
IpCounters*               getDataIpCountersTable( IN RowId rowId )
{
  return   &(g_IpCounters[rowId]);
}
 
LoginStatus*              getDataLoginStatusTable( IN RowId rowId )
{
  return   &(g_LoginStatus[rowId]);
}
 
LedStatus*                getDataLedStatusTable( IN RowId rowId )
{
  return   &(g_LedStatus[rowId]);
}
 
CellStatus*               getDataCellStatusTable( IN RowId rowId )
{
  return   &(g_CellStatus[rowId]);
}
 
L2Counters*               getDataL2CountersTable( IN RowId rowId )
{
  return   &(g_L2Counters[rowId]);
}
 
CounterCollectionStatus*  getDataCounterCollectionStatusTable( IN RowId rowId )
{
  return   &(g_CounterCollectionStatus[rowId]);
}
 
TraceHistory*             getDataTraceHistoryTable( IN RowId rowId )
{
  return   &(g_TraceHistory[rowId]);
}
 
TraceStatus*              getDataTraceStatusTable( IN RowId rowId )
{
  return   &(g_TraceStatus[rowId]);
}
 
/* GetRow Status */
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUploadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowFileUploadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_FILEUPLOADSTATUS_TABLE; colId++)
  {
     getElementFileUploadStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileDownloadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowFileDownloadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_FILEDOWNLOADSTATUS_TABLE; colId++)
  {
     getElementFileDownloadStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swActivationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowSwActivationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_SWACTIVATIONSTATUS_TABLE; colId++)
  {
     getElementSwActivationStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swPackageStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowSwPackageStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_SWPACKAGESTATUS_TABLE; colId++)
  {
     getElementSwPackageStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill componentStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowComponentStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_COMPONENTSTATUS_TABLE; colId++)
  {
     getElementComponentStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowEnbStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ENBSTATUS_TABLE; colId++)
  {
     getElementEnbStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowModuleStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_MODULESTATUS_TABLE; colId++)
  {
     getElementModuleStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill radioHeadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowRadioHeadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_RADIOHEADSTATUS_TABLE; colId++)
  {
     getElementRadioHeadStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill macGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowMacGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_MACGENCOUNTERS_TABLE; colId++)
  {
     getElementMacGenCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill macUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowMacUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_MACUPLANECOUNTERS_TABLE; colId++)
  {
     getElementMacUplaneCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rlcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowRlcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_RLCGENCOUNTERS_TABLE; colId++)
  {
     getElementRlcGenCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pdcpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowPdcpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_PDCPGENCOUNTERS_TABLE; colId++)
  {
     getElementPdcpGenCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pdcpUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowPdcpUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_PDCPUPLANECOUNTERS_TABLE; colId++)
  {
     getElementPdcpUplaneCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill gtpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowGtpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_GTPGENCOUNTERS_TABLE; colId++)
  {
     getElementGtpGenCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill sctpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowSctpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_SCTPGENCOUNTERS_TABLE; colId++)
  {
     getElementSctpGenCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowFileEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_FILEEVENTHISTORY_TABLE; colId++)
  {
     getElementFileEventHistoryTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swInstallHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowSwInstallHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_SWINSTALLHISTORY_TABLE; colId++)
  {
     getElementSwInstallHistoryTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeUeStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowActiveUeStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ACTIVEUESTATUS_TABLE; colId++)
  {
     getElementActiveUeStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeRabStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowActiveRabStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ACTIVERABSTATUS_TABLE; colId++)
  {
     getElementActiveRabStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ueEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowUeEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_UEEVENTHISTORY_TABLE; colId++)
  {
     getElementUeEventHistoryTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rabHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowRabHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_RABHISTORY_TABLE; colId++)
  {
     getElementRabHistoryTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pmLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowPmLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_PMLOGFILESTATUS_TABLE; colId++)
  {
     getElementPmLogFileStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowModuleLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_MODULELOGFILESTATUS_TABLE; colId++)
  {
     getElementModuleLogFileStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeAlarmStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowActiveAlarmStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ACTIVEALARMSTATUS_TABLE; colId++)
  {
     getElementActiveAlarmStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill alarmHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowAlarmHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ALARMHISTORY_TABLE; colId++)
  {
     getElementAlarmHistoryTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill locationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowLocationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_LOCATIONSTATUS_TABLE; colId++)
  {
     getElementLocationStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill gpsStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowGpsStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_GPSSTATUS_TABLE; colId++)
  {
     getElementGpsStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowRrcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_RRCGENCOUNTERS_TABLE; colId++)
  {
     getElementRrcGenCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrmCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowRrmCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_RRMCOUNTERS_TABLE; colId++)
  {
     getElementRrmCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrcCellCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowRrcCellCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_RRCCELLCOUNTERS_TABLE; colId++)
  {
     getElementRrcCellCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowS1apGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_S1APGENCOUNTERS_TABLE; colId++)
  {
     getElementS1apGenCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apPeerNodeCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowS1apPeerNodeCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_S1APPEERNODECOUNTERS_TABLE; colId++)
  {
     getElementS1apPeerNodeCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ethernetCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowEthernetCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ETHERNETCOUNTERS_TABLE; colId++)
  {
     getElementEthernetCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ipCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowIpCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_IPCOUNTERS_TABLE; colId++)
  {
     getElementIpCountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill loginStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowLoginStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_LOGINSTATUS_TABLE; colId++)
  {
     getElementLoginStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ledStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowLedStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_LEDSTATUS_TABLE; colId++)
  {
     getElementLedStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill cellStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowCellStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_CELLSTATUS_TABLE; colId++)
  {
     getElementCellStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill l2Counters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowL2CountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_L2COUNTERS_TABLE; colId++)
  {
     getElementL2CountersTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill counterCollectionStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowCounterCollectionStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_COUNTERCOLLECTIONSTATUS_TABLE; colId++)
  {
     getElementCounterCollectionStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowTraceHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_TRACEHISTORY_TABLE; colId++)
  {
     getElementTraceHistoryTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowTraceStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_TRACESTATUS_TABLE; colId++)
  {
     getElementTraceStatusTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/* GetElement Status */
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUploadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementFileUploadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  FileUploadStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_FILEUPLOADSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_FILEUPLOADSTATUS_FILEUPLOADCOMMANDRECEIVED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fileUploadCommandReceived, tlvArray );
      break;
    case COL_FILEUPLOADSTATUS_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->fileName, strlen(config->fileName), tlvArray );
      break;
    case COL_FILEUPLOADSTATUS_UPLOADSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->uploadStatus, tlvArray );
      break;
    case COL_FILEUPLOADSTATUS_PERCENTAGECOMPLETE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->percentageComplete, tlvArray );
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
 * @section This function will get and fill fileDownloadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementFileDownloadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  FileDownloadStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_FILEDOWNLOADSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_FILEDOWNLOADSTATUS_FILEDOWNLOADCOMMANDRECEIVED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fileDownloadCommandReceived, tlvArray );
      break;
    case COL_FILEDOWNLOADSTATUS_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->fileName, strlen(config->fileName), tlvArray );
      break;
    case COL_FILEDOWNLOADSTATUS_DOWNLOADSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->downloadStatus, tlvArray );
      break;
    case COL_FILEDOWNLOADSTATUS_PERCENTAGECOMPLETE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->percentageComplete, tlvArray );
      break;
    case COL_FILEDOWNLOADSTATUS_SCHEDULEDDOWNLOADSTARTTIME:
      fillTagLenString( tableId, (rowId+1), colId, config->scheduledDownloadStartTime, strlen(config->scheduledDownloadStartTime), tlvArray );
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
 * @section This function will get and fill swActivationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementSwActivationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  SwActivationStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_SWACTIVATIONSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_SWACTIVATIONSTATUS_VERSION:
      fillTagLenString( tableId, (rowId+1), colId, config->version, strlen(config->version), tlvArray );
      break;
    case COL_SWACTIVATIONSTATUS_SWSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->swStatus, tlvArray );
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
 * @section This function will get and fill swPackageStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementSwPackageStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  SwPackageStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_SWPACKAGESTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_SWPACKAGESTATUS_SWTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->swType, tlvArray );
      break;
    case COL_SWPACKAGESTATUS_VERSION:
      fillTagLenString( tableId, (rowId+1), colId, config->version, strlen(config->version), tlvArray );
      break;
    case COL_SWPACKAGESTATUS_BUILTON:
      fillTagLenString( tableId, (rowId+1), colId, config->builtOn, strlen(config->builtOn), tlvArray );
      break;
    case COL_SWPACKAGESTATUS_SWNOTES:
      fillTagLenString( tableId, (rowId+1), colId, config->swNotes, strlen(config->swNotes), tlvArray );
      break;
    case COL_SWPACKAGESTATUS_ISRUNNING:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isRunning, tlvArray );
      break;
    case COL_SWPACKAGESTATUS_PACKAGEDIR:
      fillTagLenString( tableId, (rowId+1), colId, config->packageDir, strlen(config->packageDir), tlvArray );
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
 * @section This function will get and fill componentStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementComponentStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  ComponentStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_COMPONENTSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_COMPONENTSTATUS_COMPONENTTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->componentType, tlvArray );
      break;
    case COL_COMPONENTSTATUS_COMPONENTVERSION:
      fillTagLenString( tableId, (rowId+1), colId, config->componentVersion, strlen(config->componentVersion), tlvArray );
      break;
    case COL_COMPONENTSTATUS_COMPONENTINSTANCE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->componentInstance, tlvArray );
      break;
    case COL_COMPONENTSTATUS_COMPONENTOPERATIONALSTATE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->componentOperationalState, tlvArray );
      break;
    case COL_COMPONENTSTATUS_INFOSTRING:
      fillTagLenString( tableId, (rowId+1), colId, config->infoString, strlen(config->infoString), tlvArray );
      break;
    case COL_COMPONENTSTATUS_ERRORSTRING:
      fillTagLenString( tableId, (rowId+1), colId, config->errorString, strlen(config->errorString), tlvArray );
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
 * @section This function will get and fill enbStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementEnbStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  EnbStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_ENBSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_ENBSTATUS_ENODEBID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->enodebId, tlvArray );
      break;
    case COL_ENBSTATUS_VENDORNAME:
      fillTagLenString( tableId, (rowId+1), colId, config->vendorName, strlen(config->vendorName), tlvArray );
      break;
    case COL_ENBSTATUS_ENODEBIPADDRESS:
      fillTagLenString( tableId, (rowId+1), colId, config->enodebIpAddress, strlen(config->enodebIpAddress), tlvArray );
      break;
    case COL_ENBSTATUS_CONNECTEDTOMME:
      fillTagLenUint32( tableId, (rowId+1), colId, config->connectedToMME, tlvArray );
      break;
    case COL_ENBSTATUS_ADMINSTATE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->adminState, tlvArray );
      break;
    case COL_ENBSTATUS_OPERATIONALSTATE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->operationalState, tlvArray );
      break;
    case COL_ENBSTATUS_NUMOFUESCONNECTED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfUesConnected, tlvArray );
      break;
    case COL_ENBSTATUS_AGGREGATEDLTHROUGHPUTKBPS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->aggregateDLThroughputKbps, tlvArray );
      break;
    case COL_ENBSTATUS_AGGREGATEULTHROUGHPUTKBPS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->aggregateULThroughputKbps, tlvArray );
      break;
    case COL_ENBSTATUS_LOADSTATE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->loadState, tlvArray );
      break;
    case COL_ENBSTATUS_NUMACTIVECRITICALALARMS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numActiveCriticalAlarms, tlvArray );
      break;
    case COL_ENBSTATUS_NUMACTIVEMAJORALARMS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numActiveMajorAlarms, tlvArray );
      break;
    case COL_ENBSTATUS_UPTIME:
      fillTagLenUint32( tableId, (rowId+1), colId, config->uptime, tlvArray );
      break;
    case COL_ENBSTATUS_LOCALTIME:
      fillTagLenString( tableId, (rowId+1), colId, config->localTime, strlen(config->localTime), tlvArray );
      break;
    case COL_ENBSTATUS_LOCALTIMEOFFSETFROMUTC:
      fillTagLenString( tableId, (rowId+1), colId, config->localTimeOffsetFromUTC, strlen(config->localTimeOffsetFromUTC), tlvArray );
      break;
    case COL_ENBSTATUS_MIBVERSION:
      fillTagLenString( tableId, (rowId+1), colId, config->mibVersion, strlen(config->mibVersion), tlvArray );
      break;
    case COL_ENBSTATUS_DATEANDTIMESOURCE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dateAndTimeSource, tlvArray );
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
 * @section This function will get and fill moduleStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementModuleStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  ModuleStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_MODULESTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_MODULESTATUS_MODULEIDSTRING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->moduleIdString, tlvArray );
      break;
    case COL_MODULESTATUS_MODULEDESC:
      fillTagLenString( tableId, (rowId+1), colId, config->moduleDesc, strlen(config->moduleDesc), tlvArray );
      break;
    case COL_MODULESTATUS_VERSION:
      fillTagLenString( tableId, (rowId+1), colId, config->version, strlen(config->version), tlvArray );
      break;
    case COL_MODULESTATUS_ISRUNNING:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isRunning, tlvArray );
      break;
    case COL_MODULESTATUS_PROCESSORTASKID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->processOrTaskId, tlvArray );
      break;
    case COL_MODULESTATUS_MEMUSAGE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->memUsage, tlvArray );
      break;
    case COL_MODULESTATUS_STARTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->startedAt, strlen(config->startedAt), tlvArray );
      break;
    case COL_MODULESTATUS_NUMRESTARTSDUETOERRORS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numRestartsDueToErrors, tlvArray );
      break;
    case COL_MODULESTATUS_RESTARTREASON:
      fillTagLenUint32( tableId, (rowId+1), colId, config->restartReason, tlvArray );
      break;
    case COL_MODULESTATUS_LASTHEALTHCHECKAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastHealthCheckAt, strlen(config->lastHealthCheckAt), tlvArray );
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
 * @section This function will get and fill radioHeadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementRadioHeadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  RadioHeadStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_RADIOHEADSTATUS_RADIOHEADPROFILEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->radioHeadProfileId, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_C1IQMAP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->c1IQMap, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_C2IQMAP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->c2IQMap, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_C3IQMAP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->c3IQMap, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_C4IQMAP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->c4IQMap, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_FWMODE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->fwMode, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_NUMBEROFFA:
      fillTagLenUint8( tableId, (rowId+1), colId, config->numberOfFA, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_MAPPINGOF1FASTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mappingOf1FAStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_MAPPINGOF2FASTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mappingOf2FAStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_MAPPINGOF3FASTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mappingOf3FAStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_MAPPINGOF4FASTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mappingOf4FAStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_CVC1:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cvc1, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_CVC2:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cvc2, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_CVC3:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cvc3, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_CVC4:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cvc4, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TXFILTERBANDWIDTH:
      fillTagLenUint8( tableId, (rowId+1), colId, config->txFilterBandwidth, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RXFILTERBANDWIDTH:
      fillTagLenUint8( tableId, (rowId+1), colId, config->rxFilterBandwidth, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_DLRATIO:
      fillTagLenUint8( tableId, (rowId+1), colId, config->dlRatio, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_ULRATIO:
      fillTagLenUint8( tableId, (rowId+1), colId, config->ulRatio, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_VSWRSHUTDOWNALARMTHRESHOLD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->vswrShutdownAlarmThreshold, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_OVERPOWERSHUTDOWNALARMTHRESHOLD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->overpowerShutdownAlarmThreshold, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_LOWPOWERALARMTHRESHOLD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->lowpowerAlarmThreshold, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_CENTERFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->centerFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_FA1CHANNELBANDWIDTHSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fa1ChannelBandwidthStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_FA2CHANNELBANDWIDTHSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fa2ChannelBandwidthStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_FA3CHANNELBANDWIDTHSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fa3ChannelBandwidthStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_FA4CHANNELBANDWIDTHSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fa4ChannelBandwidthStatus, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TX1STFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->tx1stFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TX2NDFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->tx2ndFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TX3RDFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->tx3rdFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TX4THFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->tx4thFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RX1STFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rx1stFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RX2NDFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rx2ndFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RX3RDFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rx3rdFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RX4THFAFREQUENCY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rx4thFaFrequency, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TXDELAY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->txDelay, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RXDELAY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rxDelay, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TXPATHAFACONTROL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->txPathAFaControl, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_TXPATHBFACONTROL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->txPathBFaControl, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RXPATHAFACONTROL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->rxPathAFaControl, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_RXPATHBFACONTROL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->rxPathBFaControl, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_MULTIMODE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->multiMode, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_POWERBOOST:
      fillTagLenUint8( tableId, (rowId+1), colId, config->powerBoost, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_FILTERSWITHINGMODE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->filterSwithingMode, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHATXONOROFF:
      fillTagLenUint8( tableId, (rowId+1), colId, config->pathATxOnOrOff, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHATXENABLEORDISABLE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->pathATxEnableOrDisable, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHATEMPERATURE:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathATemperature, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHATXRFPOWER:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathATxRFPower, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHARETURNLOSS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathAReturnLoss, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA1RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa1RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA2RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa2RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA3RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa3RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA4RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa4RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA1TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa1TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA2TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa2TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA3TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa3TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA4TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathAFa4TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA1TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathAFa1TxAtten, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA2TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathAFa2TxAtten, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA3TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathAFa3TxAtten, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHAFA4TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathAFa4TxAtten, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBTXONOROFF:
      fillTagLenUint8( tableId, (rowId+1), colId, config->pathBTxOnOrOff, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBTXENABLEORDISABLE:
      fillTagLenUint8( tableId, (rowId+1), colId, config->pathBTxEnableOrDisable, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBTEMPERATURE:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathBTemperature, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBTXRFPOWER:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathBTxRFPower, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBRETURNLOSS:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathBReturnLoss, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA1RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa1RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA2RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa2RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA3RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa3RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA4RSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa4RssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA1TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa1TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA2TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa2TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA3TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa3TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA4TSSIDIGITALIQLEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathBFa4TssiDigitalIQLevel, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA1TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathBFa1TxAtten, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA2TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathBFa2TxAtten, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA3TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathBFa3TxAtten, tlvArray );
      break;
    case COL_RADIOHEADSTATUS_PATHBFA4TXATTEN:
      fillTagLenUint16( tableId, (rowId+1), colId, config->pathBFa4TxAtten, tlvArray );
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
 * @section This function will get and fill macGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementMacGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  MacGenCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_MACGENCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_MACGENCOUNTERS_NUMHARQFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numHarqFail, tlvArray );
      break;
    case COL_MACGENCOUNTERS_NUMUECFG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numUeCfg, tlvArray );
      break;
    case COL_MACGENCOUNTERS_NUMCELLCFG:
      fillTagLenUint16( tableId, (rowId+1), colId, config->numCellCfg, tlvArray );
      break;
    case COL_MACGENCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill macUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementMacUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  MacUplaneCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_MACUPLANECOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_MACUPLANECOUNTERS_NUMOFSDUTXRLCMACIF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfSDUTxRlcMacIf, tlvArray );
      break;
    case COL_MACUPLANECOUNTERS_NUMOFSDURXRLCMACIF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfSDURxRlcMacIf, tlvArray );
      break;
    case COL_MACUPLANECOUNTERS_NUMSDUDROPRLCMACIF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numSDUDropRlcMacIf, tlvArray );
      break;
    case COL_MACUPLANECOUNTERS_NUMOFPDUTXL2L1IF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfPduTxL2L1If, tlvArray );
      break;
    case COL_MACUPLANECOUNTERS_NUMOFPDURXL2L1IF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfPduRxL2L1If, tlvArray );
      break;
    case COL_MACUPLANECOUNTERS_NUMPDUDROPL2L1IF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numPduDropL2L1If, tlvArray );
      break;
    case COL_MACUPLANECOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill rlcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementRlcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  RlcGenCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_RLCGENCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_NUMUECFG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numUeCfg, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_PDUSRECV:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pdusRecv, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_PDUSSENT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pdusSent, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_PDUSRETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pdusRetx, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_BYTESRECV:
      fillTagLenUint32( tableId, (rowId+1), colId, config->bytesRecv, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_BYTESSENT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->bytesSent, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_UNEXPPDUSRECV:
      fillTagLenUint32( tableId, (rowId+1), colId, config->unexpPdusRecv, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_ERRORPDUSRECV:
      fillTagLenUint32( tableId, (rowId+1), colId, config->errorPdusRecv, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_PROTTIMEOUT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->protTimeOut, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_NUMOFRB:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfRb, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_NUMSDUDISC:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numSduDisc, tlvArray );
      break;
    case COL_RLCGENCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill pdcpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementPdcpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  PdcpGenCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_PDCPGENCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_RXPDUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rxPdus, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_TXPDUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->txPdus, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_ERRORPDUSRECV:
      fillTagLenUint32( tableId, (rowId+1), colId, config->errorPdusRecv, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMSDUSDISC:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numSdusDisc, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMCMPFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numCmpFails, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMDECMPFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numDecmpFails, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMINTGPRTFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numIntgPrtFails, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMINTGVRFFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numIntgVrfFails, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMCIPHFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numCiphFails, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMDECIPHFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numDeciphFails, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMPDUSDISCOBDTMREXP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numPdusDiscObdTmrExp, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMSDUSDISCOBDTMREXP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numSdusDiscObdTmrExp, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMPKTSFRWD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numPktsFrwd, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_NUMPKTSRCVD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numPktsRcvd, tlvArray );
      break;
    case COL_PDCPGENCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill pdcpUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementPdcpUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  PdcpUplaneCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_PDCPUPLANECOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_PDCPUPLANECOUNTERS_RXSDUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rxSdus, tlvArray );
      break;
    case COL_PDCPUPLANECOUNTERS_TXSDUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->txSdus, tlvArray );
      break;
    case COL_PDCPUPLANECOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill gtpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementGtpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  GtpGenCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_GTPGENCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_NUMTXMSG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numTxMsg, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_NUMRXMSG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numRxMsg, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_NUMACTVTNLS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numActvTnls, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_NUMERRIND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numErrInd, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_RXDESGMSG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rxDesgMsg, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVVERRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invVerRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVPROTTYPERX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invProtTypeRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVMSGRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invMsgRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVMSGLENRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invMsgLenRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVIEORDERRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invIeOrderRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVIELENRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invIeLenRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVIEFORMATRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invIeFormatRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVMANIELENRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invManIeLenRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_MISSMANDIERX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->missMandIeRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_UNSUPPEXTHDRRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->unSuppExtHdrRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_WRNGEXTHDRRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->wrngExtHdrRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_MSGUNABLECOMPRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->msgUnableCompRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_MSGUNABLECOMPTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->msgUnableCompTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_MISSMANDIETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->missMandIeTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_MEMALLOCFLDTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->memAllocFldTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVMSGTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invMsgTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVTEIDTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invTeidTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVTEIDRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invTeidRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_ERRSEQNOTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->errSeqNoTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_ERRSEQNORX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->errSeqNoRx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_TPDUEXCDTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->tpduExcdTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_INVDATTYPETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->invDatTypeTx, tlvArray );
      break;
    case COL_GTPGENCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill sctpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementSctpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  SctpGenCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_SCTPGENCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOINITTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noInitTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOINITRETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noInitReTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOINITRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noInitRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOIACKTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noIAckTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOIACKRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noIAckRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNRETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnReTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNACKTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnAckTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNACKRETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnAckReTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNACKRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnAckRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOCOOKIETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noCookieTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOCOOKIERETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noCookieReTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOCOOKIERX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noCookieRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOCKACKTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noCkAckTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOCKACKRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noCkAckRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NODATATX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noDataTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NODATARETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noDataReTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NODATARX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noDataRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NODACKTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noDAckTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NODACKRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noDAckRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNCMPLTTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnCmpltTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOSHDWNCMPLTRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noShDwnCmpltRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOHBEATTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noHBeatTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOHBEATRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noHBeatRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOHBACKTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noHBAckTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOHBACKRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noHBAckRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOABORTTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noAbortTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOABORTRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noAbortRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOERRORTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noErrorTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOERRORRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noErrorRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOECNETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noEcneTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOECNERETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noEcneReTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOECNERX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noEcneRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOCWRTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noCwrTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOCWRRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noCwrRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOQUERYTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noQueryTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOQUERYRETX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noQueryReTx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_NOQUERYRSPRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noQueryRspRx, tlvArray );
      break;
    case COL_SCTPGENCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill fileEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementFileEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  FileEventHistory  *config = getData(rowId);

  switch(colId)
  {
    case COL_FILEEVENTHISTORY_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_FILEEVENTHISTORY_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->fileName, strlen(config->fileName), tlvArray );
      break;
    case COL_FILEEVENTHISTORY_FILESTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fileStatus, tlvArray );
      break;
    case COL_FILEEVENTHISTORY_WASSUCCESSFUL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->wasSuccessful, tlvArray );
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
 * @section This function will get and fill swInstallHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementSwInstallHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  SwInstallHistory  *config = getData(rowId);

  switch(colId)
  {
    case COL_SWINSTALLHISTORY_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_SWINSTALLHISTORY_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->filename, strlen(config->filename), tlvArray );
      break;
    case COL_SWINSTALLHISTORY_WASSUCCESSFUL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->wasSuccessful, tlvArray );
      break;
    case COL_SWINSTALLHISTORY_INSTALLERRORDETAIL:
      fillTagLenString( tableId, (rowId+1), colId, config->installErrorDetail, strlen(config->installErrorDetail), tlvArray );
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
 * @section This function will get and fill activeUeStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementActiveUeStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  ActiveUeStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_ACTIVEUESTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_INTERNALUEIDENTITY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->internalUeIdentity, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_MMEC:
      fillTagLenUint8( tableId, (rowId+1), colId, config->mmec, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_MTMSI:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mTmsi, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_CRNTI:
      fillTagLenUint16( tableId, (rowId+1), colId, config->cRnti, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_S1APENBUEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->s1apEnbUeId, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_S1APMMEUEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->s1apMmeUeId, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_ENTRYTIME:
      fillTagLenString( tableId, (rowId+1), colId, config->entryTime, strlen(config->entryTime), tlvArray );
      break;
    case COL_ACTIVEUESTATUS_DLAMBR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dlAmbr, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_ULAMBR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ulAmbr, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_UECATEGORY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCategory, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_UEACCESSSTRATUMRELEASE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueAccessStratumRelease, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_ACTIVEINTPROTALGO:
      fillTagLenUint32( tableId, (rowId+1), colId, config->activeIntProtAlgo, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_ACTIVECIPHALGO:
      fillTagLenUint32( tableId, (rowId+1), colId, config->activeCiphAlgo, tlvArray );
      break;
    case COL_ACTIVEUESTATUS_NUMERABS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numERabs, tlvArray );
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
 * @section This function will get and fill activeRabStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementActiveRabStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  ActiveRabStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_ACTIVERABSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_INTERNALUEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->internalUeId, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_RABIDENTITY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->rabIdentity, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_DRBIDENTITY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->drbIdentity, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_RABQCI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->rabQci, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_RABSETUPTIME:
      fillTagLenString( tableId, (rowId+1), colId, config->rabSetupTime, strlen(config->rabSetupTime), tlvArray );
      break;
    case COL_ACTIVERABSTATUS_DLGTPTUNNELID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dlGtpTunnelId, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_ULGTPTUNNELID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ulGtpTunnelId, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_GTPKILOBYTESDL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->gtpKiloBytesDL, tlvArray );
      break;
    case COL_ACTIVERABSTATUS_GTPKILOBYTESUL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->gtpKiloBytesUL, tlvArray );
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
 * @section This function will get and fill ueEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementUeEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  UeEventHistory  *config = getData(rowId);

  switch(colId)
  {
    case COL_UEEVENTHISTORY_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_UEEVENTHISTORY_MMEC:
      fillTagLenUint8( tableId, (rowId+1), colId, config->mmec, tlvArray );
      break;
    case COL_UEEVENTHISTORY_MTMSI:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mTmsi, tlvArray );
      break;
    case COL_UEEVENTHISTORY_CRNTI:
      fillTagLenUint16( tableId, (rowId+1), colId, config->cRnti, tlvArray );
      break;
    case COL_UEEVENTHISTORY_EVENTTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eventType, tlvArray );
      break;
    case COL_UEEVENTHISTORY_EVENTDETAIL:
      fillTagLenString( tableId, (rowId+1), colId, config->eventDetail, strlen(config->eventDetail), tlvArray );
      break;
    case COL_UEEVENTHISTORY_EVENTTIMESTAMP:
      fillTagLenString( tableId, (rowId+1), colId, config->eventTimestamp, strlen(config->eventTimestamp), tlvArray );
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
 * @section This function will get and fill rabHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementRabHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  RabHistory  *config = getData(rowId);

  switch(colId)
  {
    case COL_RABHISTORY_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_RABHISTORY_INTERNALUEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->internalUeId, tlvArray );
      break;
    case COL_RABHISTORY_RABIDENTITY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->rabIdentity, tlvArray );
      break;
    case COL_RABHISTORY_DRBIDENTITY:
      fillTagLenUint8( tableId, (rowId+1), colId, config->drbIdentity, tlvArray );
      break;
    case COL_RABHISTORY_RABQCI:
      fillTagLenUint8( tableId, (rowId+1), colId, config->rabQci, tlvArray );
      break;
    case COL_RABHISTORY_RABSETUPTIME:
      fillTagLenString( tableId, (rowId+1), colId, config->rabSetupTime, strlen(config->rabSetupTime), tlvArray );
      break;
    case COL_RABHISTORY_RABRELEASETIME:
      fillTagLenString( tableId, (rowId+1), colId, config->rabReleaseTime, strlen(config->rabReleaseTime), tlvArray );
      break;
    case COL_RABHISTORY_RABRELEASECAUSE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->rabReleaseCause, tlvArray );
      break;
    case COL_RABHISTORY_DLGTPTUNNELID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dlGtpTunnelId, tlvArray );
      break;
    case COL_RABHISTORY_ULGTPTUNNELID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ulGtpTunnelId, tlvArray );
      break;
    case COL_RABHISTORY_GTPKILOBYTESDL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->gtpKiloBytesDL, tlvArray );
      break;
    case COL_RABHISTORY_GTPKILOBYTESUL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->gtpKiloBytesUL, tlvArray );
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
 * @section This function will get and fill pmLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementPmLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  PmLogFileStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_PMLOGFILESTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_PMLOGFILESTATUS_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->fileName, strlen(config->fileName), tlvArray );
      break;
    case COL_PMLOGFILESTATUS_FILESIZEINBYTES:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fileSizeInBytes, tlvArray );
      break;
    case COL_PMLOGFILESTATUS_OPENEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->openedAt, strlen(config->openedAt), tlvArray );
      break;
    case COL_PMLOGFILESTATUS_CLOSEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->closedAt, strlen(config->closedAt), tlvArray );
      break;
    case COL_PMLOGFILESTATUS_DELETEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->deletedAt, strlen(config->deletedAt), tlvArray );
      break;
    case COL_PMLOGFILESTATUS_DELETEREASON:
      fillTagLenString( tableId, (rowId+1), colId, config->deleteReason, strlen(config->deleteReason), tlvArray );
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
 * @section This function will get and fill moduleLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementModuleLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  ModuleLogFileStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_MODULELOGFILESTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_MODULELOGFILESTATUS_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->fileName, strlen(config->fileName), tlvArray );
      break;
    case COL_MODULELOGFILESTATUS_FILESIZEINKB:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fileSizeInKB, tlvArray );
      break;
    case COL_MODULELOGFILESTATUS_OPENEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->openedAt, strlen(config->openedAt), tlvArray );
      break;
    case COL_MODULELOGFILESTATUS_CLOSEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->closedAt, strlen(config->closedAt), tlvArray );
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
 * @section This function will get and fill activeAlarmStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementActiveAlarmStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  ActiveAlarmStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_ACTIVEALARMSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMSEQUENCENUMBER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmSequenceNumber, tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMDATEANDTIMESTAMP:
      fillTagLenString( tableId, (rowId+1), colId, config->alarmDateAndTimeStamp, strlen(config->alarmDateAndTimeStamp), tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmType, tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmId, tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMSEVERITY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmSeverity, tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMCAUSE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmCause, tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMLOCATION:
      fillTagLenString( tableId, (rowId+1), colId, config->alarmLocation, strlen(config->alarmLocation), tlvArray );
      break;
    case COL_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT:
      fillTagLenString( tableId, (rowId+1), colId, config->alarmAdditionalText, strlen(config->alarmAdditionalText), tlvArray );
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
 * @section This function will get and fill alarmHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementAlarmHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  AlarmHistory  *config = getData(rowId);

  switch(colId)
  {
    case COL_ALARMHISTORY_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMNOTIFICATIONTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmNotificationType, tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMSEQUENCENUMBER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmSequenceNumber, tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMDATEANDTIMESTAMP:
      fillTagLenString( tableId, (rowId+1), colId, config->alarmDateAndTimeStamp, strlen(config->alarmDateAndTimeStamp), tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmType, tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmId, tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMSEVERITY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmSeverity, tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMCAUSE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->alarmCause, tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMLOCATION:
      fillTagLenString( tableId, (rowId+1), colId, config->alarmLocation, strlen(config->alarmLocation), tlvArray );
      break;
    case COL_ALARMHISTORY_ALARMADDITIONALTEXT:
      fillTagLenString( tableId, (rowId+1), colId, config->alarmAdditionalText, strlen(config->alarmAdditionalText), tlvArray );
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
 * @section This function will get and fill locationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementLocationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  LocationStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_LOCATIONSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_LOCATIONSTATUS_LOCATIONSOURCE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->locationSource, tlvArray );
      break;
    case COL_LOCATIONSTATUS_LATITUDE:
      fillTagLenString( tableId, (rowId+1), colId, config->latitude, strlen(config->latitude), tlvArray );
      break;
    case COL_LOCATIONSTATUS_LONGITUDE:
      fillTagLenString( tableId, (rowId+1), colId, config->longitude, strlen(config->longitude), tlvArray );
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
 * @section This function will get and fill gpsStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementGpsStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  GpsStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_GPSSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_GPSSTATUS_GPSLATITUDE:
      fillTagLenString( tableId, (rowId+1), colId, config->gpsLatitude, strlen(config->gpsLatitude), tlvArray );
      break;
    case COL_GPSSTATUS_GPSLONGITUDE:
      fillTagLenString( tableId, (rowId+1), colId, config->gpsLongitude, strlen(config->gpsLongitude), tlvArray );
      break;
    case COL_GPSSTATUS_GPSDATEANDTIME:
      fillTagLenString( tableId, (rowId+1), colId, config->gpsDateAndTime, strlen(config->gpsDateAndTime), tlvArray );
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
 * @section This function will get and fill rrcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementRrcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  RrcGenCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_RRCGENCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMESTBCON:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numEstbCon, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMFAILCON:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numFailCon, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMRECONFIGSUCC:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numReConfigSucc, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMRECONFIGFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numReConfigFail, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMUNEXPTMSG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numUnExptMsg, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMLWLYFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numLwLyFail, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMDECERR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numDecErr, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMENCERR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numEncErr, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMPDCPERR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numPdcpErr, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_NUMRLCERR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numRlcErr, tlvArray );
      break;
    case COL_RRCGENCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill rrmCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementRrmCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  RrmCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_RRMCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUMRLFAILURES:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numRlFailures, tlvArray );
      break;
    case COL_RRMCOUNTERS_NS1SETUPREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nS1SetupReq, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUMINCOMINGHOSUCCESSFUL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numIncomingHoSuccessful, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUMINCOMINGHOFAILED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numIncomingHoFailed, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUMINCOMINGHOREJECTED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numIncomingHoRejected, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUMS1APREJECTED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numS1apRejected, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCCONNREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcConnReq, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCCONNSETUP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcConnSetup, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCCONNSETUPCMPL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcConnSetupCmpl, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCCONNREL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcConnRel, tlvArray );
      break;
    case COL_RRMCOUNTERS_NDLINFOTRF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nDlInfoTrf, tlvArray );
      break;
    case COL_RRMCOUNTERS_NULINFOTRF:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUlInfoTrf, tlvArray );
      break;
    case COL_RRMCOUNTERS_NULNASTRFMSG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUlNasTrfMsg, tlvArray );
      break;
    case COL_RRMCOUNTERS_NDLNASTRFMSG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nDlNasTrfMsg, tlvArray );
      break;
    case COL_RRMCOUNTERS_NINITCTXTSETREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nInitCtxtSetReq, tlvArray );
      break;
    case COL_RRMCOUNTERS_NINITCTXTSETRSP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nInitCtxtSetRsp, tlvArray );
      break;
    case COL_RRMCOUNTERS_NINITCTXTSETFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nInitCtxtSetFail, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCRECONFREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcReconfReq, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCRECONFCMPL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcReconfCmpl, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCREESTREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcReestReq, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUECAPENQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUeCapEnq, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUECAPINFO:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUeCapInfo, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUECAPINFOIND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUeCapInfoInd, tlvArray );
      break;
    case COL_RRMCOUNTERS_NSECMODECMD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nSecModeCmd, tlvArray );
      break;
    case COL_RRMCOUNTERS_NSECMODECMPL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nSecModeCmpl, tlvArray );
      break;
    case COL_RRMCOUNTERS_NSECMODEFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nSecModeFail, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUECTXTRELCMPL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUeCtxtRelCmpl, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUECTXTRELCMD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUeCtxtRelCmd, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUECTXTRELREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUeCtxtRelReq, tlvArray );
      break;
    case COL_RRMCOUNTERS_NUECTXTMODRESP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nUeCtxtModResp, tlvArray );
      break;
    case COL_RRMCOUNTERS_NS1SETUPFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nS1SetupFail, tlvArray );
      break;
    case COL_RRMCOUNTERS_NPAGING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nPaging, tlvArray );
      break;
    case COL_RRMCOUNTERS_NRRCCONNREJ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nRrcConnRej, tlvArray );
      break;
    case COL_RRMCOUNTERS_NS1SETUPRSP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nS1SetupRsp, tlvArray );
      break;
    case COL_RRMCOUNTERS_NS1RESETTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nS1ResetTx, tlvArray );
      break;
    case COL_RRMCOUNTERS_NS1RESETRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nS1ResetRx, tlvArray );
      break;
    case COL_RRMCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill rrcCellCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementRrcCellCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  RrcCellCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_RRCCELLCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_CELLID:
      fillTagLenUint16( tableId, (rowId+1), colId, config->cellId, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_PAG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pag, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_CONNSETUP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->connSetup, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_CONNREJECT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->connReject, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_CONNREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->connReq, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_CONNREESTABREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->connReestabReq, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_CONNREESTAB:
      fillTagLenUint32( tableId, (rowId+1), colId, config->connReestab, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_CONNREESTABREJ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->connReestabRej, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_NUMUES:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numUEs, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_HOTOEUTRASUCC:
      fillTagLenUint32( tableId, (rowId+1), colId, config->hoToEUTRASucc, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_HOFROMEUTRAFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->hoFromEUTRAFail, tlvArray );
      break;
    case COL_RRCCELLCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill s1apGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementS1apGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  S1apGenCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_S1APGENCOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBCONN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbConn, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBFAILCONN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbFailConn, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_MAXMEMALLOC:
      fillTagLenUint32( tableId, (rowId+1), colId, config->maxMemAlloc, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBDECERR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbDecErr, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBENCERR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbEncErr, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBAUDREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbAudReq, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBAUDCONN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbAudConn, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBAUDPEER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbAudPeer, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_NMBAUDFLC:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbAudFlc, tlvArray );
      break;
    case COL_S1APGENCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill s1apPeerNodeCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementS1apPeerNodeCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  S1apPeerNodeCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_S1APPEERNODECOUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_PEERID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->peerId, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_NMBUNEXPEVT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbUnexpEvt, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_NMBTRAFEXP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nmbTRafExp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERABSETUPREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eRABSetupReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERABSETUPRESP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eRABSetupResp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERABMODIFYREQUEST:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eRABModifyRequest, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERABMODRESP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eRABModResp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERABRELCMD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eRABRelCmd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERABRELCOMP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eRABRelComp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERABRELREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eRABRelReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_INITCXTSETUP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->initCxtSetup, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_INITCXTSETUPRESP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->initCxtSetupResp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_INITCXTSETUPFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->initCxtSetupFail, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UECXTRELREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCxtRelReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UECXTRELCMD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCxtRelCmd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UECXTRELCOMP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCxtRelComp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UECXTMODREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCxtModReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UECXTMODRESP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCxtModResp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UECXTMODFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCxtModFail, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_INITUEMSG:
      fillTagLenUint32( tableId, (rowId+1), colId, config->initUEMsg, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_DWNNASTPT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dwnNASTpt, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UPNASTPT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->upNASTpt, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_NONDELINDNAS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->nonDelIndNAS, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERRINDTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->errIndtx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ERRINDRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->errIndrx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_NODATATX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noDatatx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_NODATARX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->noDatarx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_RESETTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->resettx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_RESETRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->resetrx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_RESETACKTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->resetAcktx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_RESETACKRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->resetAckrx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_SETUPREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->setupReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_SETUPRESP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->setupResp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_SETUPFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->setupFail, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_PAGING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->paging, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDREQD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handReqd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDCMD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handCmd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDPREPFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handPrepFail, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDREQACK:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handReqAck, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handFail, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDNTFY:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handNtfy, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_PATHSWCHREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathSwchReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_PATHSWCHREQACK:
      fillTagLenUint32( tableId, (rowId+1), colId, config->pathSwchReqAck, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDCAN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handCan, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_HANDCANACK:
      fillTagLenUint32( tableId, (rowId+1), colId, config->handCanAck, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ENBSTATRANS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eNBStaTrans, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_MMESTATRANS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mmeStaTrans, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_DEACTTRACE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->deactTrace, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_TRACESTART:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceStart, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_TRACEFAILIND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceFailInd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_LOCREPORTCNTRL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->locReportCntrl, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_LOCREPORTFAILIND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->locReportFailInd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_LOCREPORT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->locReport, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ENBCFGUPD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->enbCfgUpd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ENBCFGUPDACK:
      fillTagLenUint32( tableId, (rowId+1), colId, config->enbCfgUpdAck, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ENBCFGUPDFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->enbCfgUpdFail, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_MMECFGUPD:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mmeCfgUpd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_MMECFGUPDACK:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mmeCfgUpdAck, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_MMECFGUPDFAIL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mmeCfgUpdFail, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UPS1CDMA2000TUNN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->upS1CDMA2000Tunn, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_DNS1CDMA2000TUNN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->dnS1CDMA2000Tunn, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_UECAPINFOIND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ueCapInfoInd, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_OVERLOADSTART:
      fillTagLenUint32( tableId, (rowId+1), colId, config->overloadStart, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_OVERLOADSTOP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->overloadStop, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_WRITEREPWARNREQ:
      fillTagLenUint32( tableId, (rowId+1), colId, config->writeRepWarnReq, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_WRITEREPWARNRSP:
      fillTagLenUint32( tableId, (rowId+1), colId, config->writeRepWarnRsp, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ENBDIRINFOTRANS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->enbDirInfoTrans, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_MMEDIRINFOTRANS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mmeDirInfoTrans, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_PRIVMSGTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->privMsgtx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_PRIVMSGRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->privMsgrx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_ENBCONFIGTFR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eNBConfigTfr, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_MMECONFIGTFR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->mmeConfigTfr, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_CELLTRAFFICTRACE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->cellTrafficTrace, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_SZDEDPROCSTSTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->szDedProcStstx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_SZDEDPROCSTSRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->szDedProcStsrx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_SZCOMPROCSTSTX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->szComProcStstx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_SZCOMPROCSTSRX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->szComProcStsrx, tlvArray );
      break;
    case COL_S1APPEERNODECOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill ethernetCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementEthernetCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  EthernetCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_ETHERNETCOUNTERS_IFINDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifIndex, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFDESCR:
      fillTagLenString( tableId, (rowId+1), colId, config->ifDescr, strlen(config->ifDescr), tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifType, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFMTU:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifMtu, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFSPEED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifSpeed, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFPHYSADDRESS:
      fillTagLenString( tableId, (rowId+1), colId, config->ifPhysAddress, strlen(config->ifPhysAddress), tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFADMINSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifAdminStatus, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFOPERSTATUS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifOperStatus, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFLASTCHANGE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifLastChange, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFINOCTETS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifInOctets, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFINUCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifInUcastPkts, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFINNUCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifInNUcastPkts, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFINDISCARDS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifInDiscards, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFINERRORS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifInErrors, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFINUNKNOWNPROTOS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifInUnknownProtos, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFOUTOCTETS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifOutOctets, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFOUTUCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifOutUcastPkts, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFOUTNUCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifOutNUcastPkts, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFOUTDISCARDS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifOutDiscards, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFOUTERRORS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifOutErrors, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_IFOUTQLEN:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ifOutQLen, tlvArray );
      break;
    case COL_ETHERNETCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill ipCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementIpCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  IpCounters  *config = getData(rowId);

  switch(colId)
  {
    case COL_IPCOUNTERS_IPSYSTEMSTATSIPVERSION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsIPVersion, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINRECEIVES:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInReceives, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINOCTETS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInOctets, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINHDRERRORS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInHdrErrors, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINNOROUTES:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInNoRoutes, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINADDRERRORS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInAddrErrors, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINUNKNOWNPROTOS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInUnknownProtos, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINTRUNCATEDPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInTruncatedPkts, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINFORWDATAGRAMS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInForwDatagrams, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSREASMREQDS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsReasmReqds, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSREASMOKS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsReasmOKs, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSREASMFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsReasmFails, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINDISCARDS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInDiscards, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINDELIVERS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInDelivers, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTREQUESTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutRequests, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTNOROUTES:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutNoRoutes, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTFORWDATAGRAMS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutForwDatagrams, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTDISCARDS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutDiscards, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTFRAGREQDS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutFragReqds, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTFRAGOKS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutFragOKs, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTFRAGFAILS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutFragFails, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTFRAGCREATES:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutFragCreates, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTTRANSMITS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutTransmits, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTOCTETS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutOctets, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINMCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInMcastPkts, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINMCASTOCTETS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInMcastOctets, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTMCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutMcastPkts, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTMCASTOCTETS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutMcastOctets, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSINBCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsInBcastPkts, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSOUTBCASTPKTS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsOutBcastPkts, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSDISCONTINUITYTIME:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsDiscontinuityTime, tlvArray );
      break;
    case COL_IPCOUNTERS_IPSYSTEMSTATSREFRESHRATE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ipSystemStatsRefreshRate, tlvArray );
      break;
    case COL_IPCOUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill loginStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementLoginStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  LoginStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_LOGINSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_LOGINSTATUS_WHO:
      fillTagLenString( tableId, (rowId+1), colId, config->who, strlen(config->who), tlvArray );
      break;
    case COL_LOGINSTATUS_PRIVILEDGELEVEL:
      fillTagLenUint32( tableId, (rowId+1), colId, config->priviledgeLevel, tlvArray );
      break;
    case COL_LOGINSTATUS_OAMINTERFACETYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->oamInterfaceType, tlvArray );
      break;
    case COL_LOGINSTATUS_OAMINTERFACESESSIONNUMBER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->oamInterfaceSessionNumber, tlvArray );
      break;
    case COL_LOGINSTATUS_LOGGEDINAT:
      fillTagLenString( tableId, (rowId+1), colId, config->loggedInAt, strlen(config->loggedInAt), tlvArray );
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
 * @section This function will get and fill ledStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementLedStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  LedStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_LEDSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_LEDSTATUS_LEDID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ledId, tlvArray );
      break;
    case COL_LEDSTATUS_LEDCOLOR:
      fillTagLenUint32( tableId, (rowId+1), colId, config->ledColor, tlvArray );
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
 * @section This function will get and fill cellStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementCellStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  CellStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_CELLSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_CELLSTATUS_CELLID:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cellId, tlvArray );
      break;
    case COL_CELLSTATUS_ISOPERATIONAL:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isOperational, tlvArray );
      break;
    case COL_CELLSTATUS_ISBARRED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isBarred, tlvArray );
      break;
    case COL_CELLSTATUS_ISOVERLOADED:
      fillTagLenUint8( tableId, (rowId+1), colId, config->isOverloaded, tlvArray );
      break;
    case COL_CELLSTATUS_SPARE1:
      fillTagLenUint8( tableId, (rowId+1), colId, config->spare1, tlvArray );
      break;
    case COL_CELLSTATUS_INFO:
      fillTagLenString( tableId, (rowId+1), colId, config->info, strlen(config->info), tlvArray );
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
 * @section This function will get and fill l2Counters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementL2CountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  L2Counters  *config = getData(rowId);

  switch(colId)
  {
    case COL_L2COUNTERS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_L2COUNTERS_COUNTERID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->counterId, tlvArray );
      break;
    case COL_L2COUNTERS_VALUE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->value, tlvArray );
      break;
    case COL_L2COUNTERS_LASTCOLLECTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->lastCollectedAt, strlen(config->lastCollectedAt), tlvArray );
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
 * @section This function will get and fill counterCollectionStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementCounterCollectionStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  CounterCollectionStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_COUNTERCOLLECTIONSTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_PROFILEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->profileId, tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_ISRUNNING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->isRunning, tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_MEASUREMENTDURATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->measurementDuration, tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_REPORTINGDURATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->reportingDuration, tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_NUMOFSAMPLESCOLLECTED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfSamplesCollected, tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_NUMOFAMPLESREMAINING:
      fillTagLenUint32( tableId, (rowId+1), colId, config->numOfamplesRemaining, tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_PMLOGFILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->pmLogFileName, strlen(config->pmLogFileName), tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_STARTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->startedAt, strlen(config->startedAt), tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_STOPPEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->stoppedAt, strlen(config->stoppedAt), tlvArray );
      break;
    case COL_COUNTERCOLLECTIONSTATUS_STOPPEDREASON:
      fillTagLenString( tableId, (rowId+1), colId, config->stoppedReason, strlen(config->stoppedReason), tlvArray );
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
 * @section This function will get and fill traceHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementTraceHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  TraceHistory  *config = getData(rowId);

  switch(colId)
  {
    case COL_TRACEHISTORY_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_TRACEHISTORY_TRACEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceId, tlvArray );
      break;
    case COL_TRACEHISTORY_TRACESTARTEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->traceStartedAt, strlen(config->traceStartedAt), tlvArray );
      break;
    case COL_TRACEHISTORY_TRACESTOPPEDAT:
      fillTagLenString( tableId, (rowId+1), colId, config->traceStoppedAt, strlen(config->traceStoppedAt), tlvArray );
      break;
    case COL_TRACEHISTORY_TRACEDURATIONELAPSEDINMINS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceDurationElapsedInMins, tlvArray );
      break;
    case COL_TRACEHISTORY_TRACEDESTINATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceDestination, tlvArray );
      break;
    case COL_TRACEHISTORY_TRACEFILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->traceFileName, strlen(config->traceFileName), tlvArray );
      break;
    case COL_TRACEHISTORY_TRACEENDCAUSE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceEndCause, tlvArray );
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
 * @section This function will get and fill traceStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementTraceStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_StatusTable[tableId-MIN_STATUS_TABLEID].getData;
  TraceStatus  *config = getData(rowId);

  switch(colId)
  {
    case COL_TRACESTATUS_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_TRACESTATUS_TRACEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceId, tlvArray );
      break;
    case COL_TRACESTATUS_TRACESTATE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceState, tlvArray );
      break;
    case COL_TRACESTATUS_TRACEDURATIONCONFIGUREDINMINS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceDurationConfiguredInMins, tlvArray );
      break;
    case COL_TRACESTATUS_TRACEDURATIONELAPSEDINMINS:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceDurationElapsedInMins, tlvArray );
      break;
    case COL_TRACESTATUS_TRACEMESSAGECOUNTER:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceMessageCounter, tlvArray );
      break;
    case COL_TRACESTATUS_TRACEDESTINATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceDestination, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/* GetTable Status */
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUploadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableFileUploadStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_FILEUPLOADSTATUS_TABLE; rowId++)
  {
     getRowFileUploadStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileDownloadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableFileDownloadStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_FILEDOWNLOADSTATUS_TABLE; rowId++)
  {
     getRowFileDownloadStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swActivationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableSwActivationStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_SWACTIVATIONSTATUS_TABLE; rowId++)
  {
     getRowSwActivationStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swPackageStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableSwPackageStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; rowId++)
  {
     getRowSwPackageStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill componentStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableComponentStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_COMPONENTSTATUS_TABLE; rowId++)
  {
     getRowComponentStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableEnbStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ENBSTATUS_TABLE; rowId++)
  {
     getRowEnbStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableModuleStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_MODULESTATUS_TABLE; rowId++)
  {
     getRowModuleStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill radioHeadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableRadioHeadStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_RADIOHEADSTATUS_TABLE; rowId++)
  {
     getRowRadioHeadStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill macGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableMacGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_MACGENCOUNTERS_TABLE; rowId++)
  {
     getRowMacGenCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill macUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableMacUplaneCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_MACUPLANECOUNTERS_TABLE; rowId++)
  {
     getRowMacUplaneCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rlcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableRlcGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_RLCGENCOUNTERS_TABLE; rowId++)
  {
     getRowRlcGenCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pdcpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTablePdcpGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_PDCPGENCOUNTERS_TABLE; rowId++)
  {
     getRowPdcpGenCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pdcpUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTablePdcpUplaneCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_PDCPUPLANECOUNTERS_TABLE; rowId++)
  {
     getRowPdcpUplaneCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill gtpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableGtpGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_GTPGENCOUNTERS_TABLE; rowId++)
  {
     getRowGtpGenCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill sctpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableSctpGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_SCTPGENCOUNTERS_TABLE; rowId++)
  {
     getRowSctpGenCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableFileEventHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_FILEEVENTHISTORY_TABLE; rowId++)
  {
     getRowFileEventHistoryTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swInstallHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableSwInstallHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_SWINSTALLHISTORY_TABLE; rowId++)
  {
     getRowSwInstallHistoryTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeUeStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableActiveUeStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ACTIVEUESTATUS_TABLE; rowId++)
  {
     getRowActiveUeStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeRabStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableActiveRabStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ACTIVERABSTATUS_TABLE; rowId++)
  {
     getRowActiveRabStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ueEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableUeEventHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_UEEVENTHISTORY_TABLE; rowId++)
  {
     getRowUeEventHistoryTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rabHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableRabHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_RABHISTORY_TABLE; rowId++)
  {
     getRowRabHistoryTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pmLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTablePmLogFileStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_PMLOGFILESTATUS_TABLE; rowId++)
  {
     getRowPmLogFileStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableModuleLogFileStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_MODULELOGFILESTATUS_TABLE; rowId++)
  {
     getRowModuleLogFileStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeAlarmStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableActiveAlarmStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ACTIVEALARMSTATUS_TABLE; rowId++)
  {
     getRowActiveAlarmStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill alarmHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableAlarmHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ALARMHISTORY_TABLE; rowId++)
  {
     getRowAlarmHistoryTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill locationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableLocationStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_LOCATIONSTATUS_TABLE; rowId++)
  {
     getRowLocationStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill gpsStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableGpsStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_GPSSTATUS_TABLE; rowId++)
  {
     getRowGpsStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableRrcGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_RRCGENCOUNTERS_TABLE; rowId++)
  {
     getRowRrcGenCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrmCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableRrmCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_RRMCOUNTERS_TABLE; rowId++)
  {
     getRowRrmCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrcCellCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableRrcCellCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_RRCCELLCOUNTERS_TABLE; rowId++)
  {
     getRowRrcCellCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableS1apGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_S1APGENCOUNTERS_TABLE; rowId++)
  {
     getRowS1apGenCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apPeerNodeCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableS1apPeerNodeCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_S1APPEERNODECOUNTERS_TABLE; rowId++)
  {
     getRowS1apPeerNodeCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ethernetCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableEthernetCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ETHERNETCOUNTERS_TABLE; rowId++)
  {
     getRowEthernetCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ipCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableIpCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_IPCOUNTERS_TABLE; rowId++)
  {
     getRowIpCountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill loginStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableLoginStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_LOGINSTATUS_TABLE; rowId++)
  {
     getRowLoginStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ledStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableLedStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_LEDSTATUS_TABLE; rowId++)
  {
     getRowLedStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill cellStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableCellStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_CELLSTATUS_TABLE; rowId++)
  {
     getRowCellStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill l2Counters the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableL2CountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_L2COUNTERS_TABLE; rowId++)
  {
     getRowL2CountersTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill counterCollectionStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableCounterCollectionStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_COUNTERCOLLECTIONSTATUS_TABLE; rowId++)
  {
     getRowCounterCollectionStatusTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableTraceHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_TRACEHISTORY_TABLE; rowId++)
  {
     getRowTraceHistoryTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableTraceStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_TRACESTATUS_TABLE; rowId++)
  {
     getRowTraceStatusTable( tableId, rowId, tlvArray);
  }
}
 
/* struct2Tlv Status */
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUploadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvFileUploadStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileDownloadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvFileDownloadStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swActivationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvSwActivationStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swPackageStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvSwPackageStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill componentStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvComponentStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvEnbStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvModuleStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill radioHeadStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvRadioHeadStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill macGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvMacGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill macUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvMacUplaneCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rlcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvRlcGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pdcpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvPdcpGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pdcpUplaneCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvPdcpUplaneCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill gtpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvGtpGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill sctpGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvSctpGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvFileEventHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swInstallHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvSwInstallHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeUeStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvActiveUeStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeRabStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvActiveRabStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ueEventHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvUeEventHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rabHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvRabHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill pmLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvPmLogFileStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill moduleLogFileStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvModuleLogFileStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill activeAlarmStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvActiveAlarmStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill alarmHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvAlarmHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill locationStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvLocationStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill gpsStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvGpsStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrcGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvRrcGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrmCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvRrmCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill rrcCellCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvRrcCellCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apGenCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvS1apGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill s1apPeerNodeCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvS1apPeerNodeCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ethernetCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvEthernetCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ipCounters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvIpCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill loginStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvLoginStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill ledStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvLedStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill cellStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvCellStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill l2Counters the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvL2CountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill counterCollectionStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvCounterCollectionStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceHistory the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvTraceHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceStatus the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvTraceStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_StatusTable[tagInfo->tableId-MIN_STATUS_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/* Extern Global Variables Status */

