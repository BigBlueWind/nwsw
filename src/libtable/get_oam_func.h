/**
 * @file    get_oam_func.h
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains GET functions Prototypes for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 11/29/2012 12:22:22 PM
 */

#ifndef GET_OAM_FUNC_H
#define GET_OAM_FUNC_H
 
#include "generic_table.h"

extern EnbAdmin                   g_EnbAdmin[SINGLE_ROW_TABLE];
extern FileUpload                 g_FileUpload[SINGLE_ROW_TABLE];
extern FileDownload               g_FileDownload[SINGLE_ROW_TABLE];
extern SwActivation               g_SwActivation[SINGLE_ROW_TABLE];
extern SwInstall                  g_SwInstall[SINGLE_ROW_TABLE];
extern TraceControl               g_TraceControl[SINGLE_ROW_TABLE];
extern CellAdmin                  g_CellAdmin[SINGLE_ROW_TABLE];
extern CounterControl             g_CounterControl[SINGLE_ROW_TABLE];
/* GetData OprCommands */
EnbAdmin*                 getDataEnbAdminTable( IN RowId rowId );
 
FileUpload*               getDataFileUploadTable( IN RowId rowId );
 
FileDownload*             getDataFileDownloadTable( IN RowId rowId );
 
SwActivation*             getDataSwActivationTable( IN RowId rowId );
 
SwInstall*                getDataSwInstallTable( IN RowId rowId );
 
TraceControl*             getDataTraceControlTable( IN RowId rowId );
 
CellAdmin*                getDataCellAdminTable( IN RowId rowId );
 
CounterControl*           getDataCounterControlTable( IN RowId rowId );
 
/* GetRow OprCommands */
void getRowEnbAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowFileUploadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowFileDownloadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSwActivationTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSwInstallTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowTraceControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowCellAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowCounterControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
/* GetElement OprCommands */
void getElementEnbAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementFileUploadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementFileDownloadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSwActivationTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSwInstallTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementTraceControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementCellAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementCounterControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
/* GetTable OprCommands */
void getTableEnbAdminTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableFileUploadTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableFileDownloadTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSwActivationTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSwInstallTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableTraceControlTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableCellAdminTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableCounterControlTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
/* struct2Tlv OprCommands */
void struct2TlvEnbAdminTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvFileUploadTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvFileDownloadTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSwActivationTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSwInstallTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvTraceControlTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvCellAdminTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvCounterControlTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
extern FileUploadStatus           g_FileUploadStatus[SINGLE_ROW_TABLE];
extern FileDownloadStatus         g_FileDownloadStatus[SINGLE_ROW_TABLE];
extern SwActivationStatus         g_SwActivationStatus[SINGLE_ROW_TABLE];
extern SwPackageStatus            g_SwPackageStatus[MAX_ROWS_IN_SWPACKAGESTATUS_TABLE];
extern ComponentStatus            g_ComponentStatus[MAX_ROWS_IN_COMPONENTSTATUS_TABLE];
extern EnbStatus                  g_EnbStatus[SINGLE_ROW_TABLE];
extern ModuleStatus               g_ModuleStatus[MAX_ROWS_IN_MODULESTATUS_TABLE];
extern RadioHeadStatus            g_RadioHeadStatus[SINGLE_ROW_TABLE];
extern MacGenCounters             g_MacGenCounters[SINGLE_ROW_TABLE];
extern MacUplaneCounters          g_MacUplaneCounters[SINGLE_ROW_TABLE];
extern RlcGenCounters             g_RlcGenCounters[SINGLE_ROW_TABLE];
extern PdcpGenCounters            g_PdcpGenCounters[SINGLE_ROW_TABLE];
extern PdcpUplaneCounters         g_PdcpUplaneCounters[SINGLE_ROW_TABLE];
extern GtpGenCounters             g_GtpGenCounters[SINGLE_ROW_TABLE];
extern SctpGenCounters            g_SctpGenCounters[SINGLE_ROW_TABLE];
extern FileEventHistory           g_FileEventHistory[MAX_ROWS_IN_FILEEVENTHISTORY_TABLE];
extern SwInstallHistory           g_SwInstallHistory[MAX_ROWS_IN_SWINSTALLHISTORY_TABLE];
extern ActiveUeStatus             g_ActiveUeStatus[MAX_ROWS_IN_ACTIVEUESTATUS_TABLE];
extern ActiveRabStatus            g_ActiveRabStatus[MAX_ROWS_IN_ACTIVERABSTATUS_TABLE];
extern UeEventHistory             g_UeEventHistory[MAX_ROWS_IN_UEEVENTHISTORY_TABLE];
extern RabHistory                 g_RabHistory[MAX_ROWS_IN_RABHISTORY_TABLE];
extern PmLogFileStatus            g_PmLogFileStatus[MAX_ROWS_IN_PMLOGFILESTATUS_TABLE];
extern ModuleLogFileStatus        g_ModuleLogFileStatus[MAX_ROWS_IN_MODULELOGFILESTATUS_TABLE];
extern ActiveAlarmStatus          g_ActiveAlarmStatus[MAX_ROWS_IN_ACTIVEALARMSTATUS_TABLE];
extern AlarmHistory               g_AlarmHistory[MAX_ROWS_IN_ALARMHISTORY_TABLE];
extern LocationStatus             g_LocationStatus[SINGLE_ROW_TABLE];
extern GpsStatus                  g_GpsStatus[SINGLE_ROW_TABLE];
extern RrcGenCounters             g_RrcGenCounters[SINGLE_ROW_TABLE];
extern RrmCounters                g_RrmCounters[SINGLE_ROW_TABLE];
extern RrcCellCounters            g_RrcCellCounters[SINGLE_ROW_TABLE];
extern S1apGenCounters            g_S1apGenCounters[SINGLE_ROW_TABLE];
extern S1apPeerNodeCounters       g_S1apPeerNodeCounters[SINGLE_ROW_TABLE];
extern EthernetCounters           g_EthernetCounters[MAX_ROWS_IN_ETHERNETCOUNTERS_TABLE];
extern IpCounters                 g_IpCounters[MAX_ROWS_IN_IPCOUNTERS_TABLE];
extern LoginStatus                g_LoginStatus[MAX_ROWS_IN_LOGINSTATUS_TABLE];
extern LedStatus                  g_LedStatus[MAX_ROWS_IN_LEDSTATUS_TABLE];
extern CellStatus                 g_CellStatus[SINGLE_ROW_TABLE];
extern L2Counters                 g_L2Counters[MAX_ROWS_IN_L2COUNTERS_TABLE];
extern CounterCollectionStatus    g_CounterCollectionStatus[MAX_ROWS_IN_COUNTERCOLLECTIONSTATUS_TABLE];
extern TraceHistory               g_TraceHistory[MAX_ROWS_IN_TRACEHISTORY_TABLE];
extern TraceStatus                g_TraceStatus[SINGLE_ROW_TABLE];
/* GetData Status */
FileUploadStatus*         getDataFileUploadStatusTable( IN RowId rowId );
 
FileDownloadStatus*       getDataFileDownloadStatusTable( IN RowId rowId );
 
SwActivationStatus*       getDataSwActivationStatusTable( IN RowId rowId );
 
SwPackageStatus*          getDataSwPackageStatusTable( IN RowId rowId );
 
ComponentStatus*          getDataComponentStatusTable( IN RowId rowId );
 
EnbStatus*                getDataEnbStatusTable( IN RowId rowId );
 
ModuleStatus*             getDataModuleStatusTable( IN RowId rowId );
 
RadioHeadStatus*          getDataRadioHeadStatusTable( IN RowId rowId );
 
MacGenCounters*           getDataMacGenCountersTable( IN RowId rowId );
 
MacUplaneCounters*        getDataMacUplaneCountersTable( IN RowId rowId );
 
RlcGenCounters*           getDataRlcGenCountersTable( IN RowId rowId );
 
PdcpGenCounters*          getDataPdcpGenCountersTable( IN RowId rowId );
 
PdcpUplaneCounters*       getDataPdcpUplaneCountersTable( IN RowId rowId );
 
GtpGenCounters*           getDataGtpGenCountersTable( IN RowId rowId );
 
SctpGenCounters*          getDataSctpGenCountersTable( IN RowId rowId );
 
FileEventHistory*         getDataFileEventHistoryTable( IN RowId rowId );
 
SwInstallHistory*         getDataSwInstallHistoryTable( IN RowId rowId );
 
ActiveUeStatus*           getDataActiveUeStatusTable( IN RowId rowId );
 
ActiveRabStatus*          getDataActiveRabStatusTable( IN RowId rowId );
 
UeEventHistory*           getDataUeEventHistoryTable( IN RowId rowId );
 
RabHistory*               getDataRabHistoryTable( IN RowId rowId );
 
PmLogFileStatus*          getDataPmLogFileStatusTable( IN RowId rowId );
 
ModuleLogFileStatus*      getDataModuleLogFileStatusTable( IN RowId rowId );
 
ActiveAlarmStatus*        getDataActiveAlarmStatusTable( IN RowId rowId );
 
AlarmHistory*             getDataAlarmHistoryTable( IN RowId rowId );
 
LocationStatus*           getDataLocationStatusTable( IN RowId rowId );
 
GpsStatus*                getDataGpsStatusTable( IN RowId rowId );
 
RrcGenCounters*           getDataRrcGenCountersTable( IN RowId rowId );
 
RrmCounters*              getDataRrmCountersTable( IN RowId rowId );
 
RrcCellCounters*          getDataRrcCellCountersTable( IN RowId rowId );
 
S1apGenCounters*          getDataS1apGenCountersTable( IN RowId rowId );
 
S1apPeerNodeCounters*     getDataS1apPeerNodeCountersTable( IN RowId rowId );
 
EthernetCounters*         getDataEthernetCountersTable( IN RowId rowId );
 
IpCounters*               getDataIpCountersTable( IN RowId rowId );
 
LoginStatus*              getDataLoginStatusTable( IN RowId rowId );
 
LedStatus*                getDataLedStatusTable( IN RowId rowId );
 
CellStatus*               getDataCellStatusTable( IN RowId rowId );
 
L2Counters*               getDataL2CountersTable( IN RowId rowId );
 
CounterCollectionStatus*  getDataCounterCollectionStatusTable( IN RowId rowId );
 
TraceHistory*             getDataTraceHistoryTable( IN RowId rowId );
 
TraceStatus*              getDataTraceStatusTable( IN RowId rowId );
 
/* GetRow Status */
void getRowFileUploadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowFileDownloadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSwActivationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSwPackageStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowComponentStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEnbStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowModuleStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowRadioHeadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowMacGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowMacUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowRlcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowPdcpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowPdcpUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowGtpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSctpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowFileEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSwInstallHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowActiveUeStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowActiveRabStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowUeEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowRabHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowPmLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowModuleLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowActiveAlarmStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowAlarmHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowLocationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowGpsStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowRrcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowRrmCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowRrcCellCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowS1apGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowS1apPeerNodeCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEthernetCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowIpCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowLoginStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowLedStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowCellStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowL2CountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowCounterCollectionStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowTraceHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowTraceStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
/* GetElement Status */
void getElementFileUploadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementFileDownloadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSwActivationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSwPackageStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementComponentStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEnbStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementModuleStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementRadioHeadStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementMacGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementMacUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementRlcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementPdcpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementPdcpUplaneCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementGtpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSctpGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementFileEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSwInstallHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementActiveUeStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementActiveRabStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementUeEventHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementRabHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementPmLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementModuleLogFileStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementActiveAlarmStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementAlarmHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementLocationStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementGpsStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementRrcGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementRrmCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementRrcCellCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementS1apGenCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementS1apPeerNodeCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEthernetCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementIpCountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementLoginStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementLedStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementCellStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementL2CountersTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementCounterCollectionStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementTraceHistoryTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementTraceStatusTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
/* GetTable Status */
void getTableFileUploadStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableFileDownloadStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSwActivationStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSwPackageStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableComponentStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEnbStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableModuleStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableRadioHeadStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableMacGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableMacUplaneCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableRlcGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTablePdcpGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTablePdcpUplaneCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableGtpGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSctpGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableFileEventHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSwInstallHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableActiveUeStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableActiveRabStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableUeEventHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableRabHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTablePmLogFileStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableModuleLogFileStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableActiveAlarmStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableAlarmHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableLocationStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableGpsStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableRrcGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableRrmCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableRrcCellCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableS1apGenCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableS1apPeerNodeCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEthernetCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableIpCountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableLoginStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableLedStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableCellStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableL2CountersTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableCounterCollectionStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableTraceHistoryTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableTraceStatusTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
/* struct2Tlv Status */
void struct2TlvFileUploadStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvFileDownloadStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSwActivationStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSwPackageStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvComponentStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEnbStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvModuleStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvRadioHeadStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvMacGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvMacUplaneCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvRlcGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvPdcpGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvPdcpUplaneCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvGtpGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSctpGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvFileEventHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSwInstallHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvActiveUeStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvActiveRabStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvUeEventHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvRabHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvPmLogFileStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvModuleLogFileStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvActiveAlarmStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvAlarmHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvLocationStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvGpsStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvRrcGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvRrmCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvRrcCellCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvS1apGenCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvS1apPeerNodeCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEthernetCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvIpCountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvLoginStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvLedStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvCellStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvL2CountersTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvCounterCollectionStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvTraceHistoryTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvTraceStatusTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
extern EnbRegistrationConfig      g_EnbRegistrationConfig[SINGLE_ROW_TABLE];
extern EnodebConfig               g_EnodebConfig[SINGLE_ROW_TABLE];
extern MmeConfig                  g_MmeConfig[SINGLE_ROW_TABLE];
extern CellConfig                 g_CellConfig[SINGLE_ROW_TABLE];
extern SchedulerConfig            g_SchedulerConfig[SINGLE_ROW_TABLE];
extern PhyConfig                  g_PhyConfig[SINGLE_ROW_TABLE];
extern SysInfo1Config             g_SysInfo1Config[SINGLE_ROW_TABLE];
extern SysInfo2Config             g_SysInfo2Config[SINGLE_ROW_TABLE];
extern LogConfig                  g_LogConfig[MAX_ROWS_IN_LOGCONFIG_TABLE];
extern RadioHeadConfig            g_RadioHeadConfig[SINGLE_ROW_TABLE];
extern QosConfig                  g_QosConfig[SINGLE_ROW_TABLE];
extern PmCounterLogConfig         g_PmCounterLogConfig[SINGLE_ROW_TABLE];
extern EutraHoProfileConfig       g_EutraHoProfileConfig[SINGLE_ROW_TABLE];
extern EutraNeighCellConfig       g_EutraNeighCellConfig[MAX_ROWS_IN_EUTRANEIGHCELLCONFIG_TABLE];
extern SysInfo3Config             g_SysInfo3Config[SINGLE_ROW_TABLE];
extern SonConfig                  g_SonConfig[SINGLE_ROW_TABLE];
extern SrsConfigDedicated         g_SrsConfigDedicated[SINGLE_ROW_TABLE];
extern SecurityConfig             g_SecurityConfig[SINGLE_ROW_TABLE];
extern UeEventHistoryLogConfig    g_UeEventHistoryLogConfig[SINGLE_ROW_TABLE];
extern CounterConfig              g_CounterConfig[MAX_ROWS_IN_COUNTERCONFIG_TABLE];
extern TraceConfig                g_TraceConfig[SINGLE_ROW_TABLE];
/* GetData OprConfig */
EnbRegistrationConfig*    getDataEnbRegistrationConfigTable( IN RowId rowId );
 
EnodebConfig*             getDataEnodebConfigTable( IN RowId rowId );
 
MmeConfig*                getDataMmeConfigTable( IN RowId rowId );
 
CellConfig*               getDataCellConfigTable( IN RowId rowId );
 
SchedulerConfig*          getDataSchedulerConfigTable( IN RowId rowId );
 
PhyConfig*                getDataPhyConfigTable( IN RowId rowId );
 
SysInfo1Config*           getDataSysInfo1ConfigTable( IN RowId rowId );
 
SysInfo2Config*           getDataSysInfo2ConfigTable( IN RowId rowId );
 
LogConfig*                getDataLogConfigTable( IN RowId rowId );
 
RadioHeadConfig*          getDataRadioHeadConfigTable( IN RowId rowId );
 
QosConfig*                getDataQosConfigTable( IN RowId rowId );
 
PmCounterLogConfig*       getDataPmCounterLogConfigTable( IN RowId rowId );
 
EutraHoProfileConfig*     getDataEutraHoProfileConfigTable( IN RowId rowId );
 
EutraNeighCellConfig*     getDataEutraNeighCellConfigTable( IN RowId rowId );
 
SysInfo3Config*           getDataSysInfo3ConfigTable( IN RowId rowId );
 
SonConfig*                getDataSonConfigTable( IN RowId rowId );
 
SrsConfigDedicated*       getDataSrsConfigDedicatedTable( IN RowId rowId );
 
SecurityConfig*           getDataSecurityConfigTable( IN RowId rowId );
 
UeEventHistoryLogConfig*  getDataUeEventHistoryLogConfigTable( IN RowId rowId );
 
CounterConfig*            getDataCounterConfigTable( IN RowId rowId );
 
TraceConfig*              getDataTraceConfigTable( IN RowId rowId );
 
/* GetRow OprConfig */
void getRowEnbRegistrationConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEnodebConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowMmeConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowCellConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSchedulerConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowPhyConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSysInfo1ConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSysInfo2ConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowLogConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowRadioHeadConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowQosConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowPmCounterLogConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEutraHoProfileConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEutraNeighCellConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSysInfo3ConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSonConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSrsConfigDedicatedTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSecurityConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowUeEventHistoryLogConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowCounterConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowTraceConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
/* GetElement OprConfig */
void getElementEnbRegistrationConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEnodebConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementMmeConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementCellConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSchedulerConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementPhyConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSysInfo1ConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSysInfo2ConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementLogConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementRadioHeadConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementQosConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementPmCounterLogConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEutraHoProfileConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEutraNeighCellConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSysInfo3ConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSonConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSrsConfigDedicatedTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSecurityConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementUeEventHistoryLogConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementCounterConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementTraceConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
/* GetTable OprConfig */
void getTableEnbRegistrationConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEnodebConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableMmeConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableCellConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSchedulerConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTablePhyConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSysInfo1ConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSysInfo2ConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableLogConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableRadioHeadConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableQosConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTablePmCounterLogConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEutraHoProfileConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEutraNeighCellConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSysInfo3ConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSonConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSrsConfigDedicatedTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSecurityConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableUeEventHistoryLogConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableCounterConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableTraceConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
/* struct2Tlv OprConfig */
void struct2TlvEnbRegistrationConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEnodebConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvMmeConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvCellConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSchedulerConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvPhyConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSysInfo1ConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSysInfo2ConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvLogConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvRadioHeadConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvQosConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvPmCounterLogConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEutraHoProfileConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEutraNeighCellConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSysInfo3ConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSonConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSrsConfigDedicatedTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSecurityConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvUeEventHistoryLogConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvCounterConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvTraceConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
extern MsgTrace                   g_MsgTrace[MAX_ROWS_IN_MSGTRACE_TABLE];
extern TimersConfig               g_TimersConfig[MAX_ROWS_IN_TIMERSCONFIG_TABLE];
extern ProcessStart               g_ProcessStart[MAX_ROWS_IN_PROCESSSTART_TABLE];
extern ModuleStop                 g_ModuleStop[MAX_ROWS_IN_MODULESTOP_TABLE];
extern EventThresholds            g_EventThresholds[SINGLE_ROW_TABLE];
extern EnbAddresses               g_EnbAddresses[SINGLE_ROW_TABLE];
extern UdhConfig                  g_UdhConfig[SINGLE_ROW_TABLE];
extern UehConfig                  g_UehConfig[SINGLE_ROW_TABLE];
extern EgtpConfig                 g_EgtpConfig[SINGLE_ROW_TABLE];
extern SctpConfig                 g_SctpConfig[SINGLE_ROW_TABLE];
extern S1apConfig                 g_S1apConfig[SINGLE_ROW_TABLE];
extern DeviceMonitorConfig        g_DeviceMonitorConfig[SINGLE_ROW_TABLE];
/* GetData PreConfig */
MsgTrace*                 getDataMsgTraceTable( IN RowId rowId );
 
TimersConfig*             getDataTimersConfigTable( IN RowId rowId );
 
ProcessStart*             getDataProcessStartTable( IN RowId rowId );
 
ModuleStop*               getDataModuleStopTable( IN RowId rowId );
 
EventThresholds*          getDataEventThresholdsTable( IN RowId rowId );
 
EnbAddresses*             getDataEnbAddressesTable( IN RowId rowId );
 
UdhConfig*                getDataUdhConfigTable( IN RowId rowId );
 
UehConfig*                getDataUehConfigTable( IN RowId rowId );
 
EgtpConfig*               getDataEgtpConfigTable( IN RowId rowId );
 
SctpConfig*               getDataSctpConfigTable( IN RowId rowId );
 
S1apConfig*               getDataS1apConfigTable( IN RowId rowId );
 
DeviceMonitorConfig*      getDataDeviceMonitorConfigTable( IN RowId rowId );
 
/* GetRow PreConfig */
void getRowMsgTraceTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowTimersConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowProcessStartTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowModuleStopTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEventThresholdsTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEnbAddressesTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowUdhConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowUehConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowEgtpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowSctpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowS1apConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
void getRowDeviceMonitorConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ); 
/* GetElement PreConfig */
void getElementMsgTraceTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementTimersConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementProcessStartTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementModuleStopTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEventThresholdsTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEnbAddressesTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementUdhConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementUehConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementEgtpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementSctpConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementS1apConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
void getElementDeviceMonitorConfigTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  );
/* GetTable PreConfig */
void getTableMsgTraceTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableTimersConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableProcessStartTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableModuleStopTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEventThresholdsTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEnbAddressesTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableUdhConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableUehConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableEgtpConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableSctpConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableS1apConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
void getTableDeviceMonitorConfigTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  );
/* struct2Tlv PreConfig */
void struct2TlvMsgTraceTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvTimersConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvProcessStartTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvModuleStopTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEventThresholdsTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEnbAddressesTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvUdhConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvUehConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvEgtpConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvSctpConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvS1apConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
void struct2TlvDeviceMonitorConfigTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  );
#endif  /* GET_OAM_FUNC.H */
