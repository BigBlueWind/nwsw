/**
 * @file    init_tables.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains SET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 11/29/2012 12:22:22 PM
 */

 
#include "set_oam_func.h"

#include "get_oam_func.h"
#include "generic_table.h"
#include "empty_tables.h"

EnbAdmin                     g_EnbAdmin[SINGLE_ROW_TABLE];
FileUpload                   g_FileUpload[SINGLE_ROW_TABLE];
FileDownload                 g_FileDownload[SINGLE_ROW_TABLE];
SwActivation                 g_SwActivation[SINGLE_ROW_TABLE];
SwInstall                    g_SwInstall[SINGLE_ROW_TABLE];
TraceControl                 g_TraceControl[SINGLE_ROW_TABLE];
CellAdmin                    g_CellAdmin[SINGLE_ROW_TABLE];
CounterControl               g_CounterControl[SINGLE_ROW_TABLE];
FileUploadStatus             g_FileUploadStatus[SINGLE_ROW_TABLE];
FileDownloadStatus           g_FileDownloadStatus[SINGLE_ROW_TABLE];
SwActivationStatus           g_SwActivationStatus[SINGLE_ROW_TABLE];
SwPackageStatus              g_SwPackageStatus[MAX_ROWS_IN_SWPACKAGESTATUS_TABLE];
ComponentStatus              g_ComponentStatus[MAX_ROWS_IN_COMPONENTSTATUS_TABLE];
EnbStatus                    g_EnbStatus[SINGLE_ROW_TABLE];
ModuleStatus                 g_ModuleStatus[MAX_ROWS_IN_MODULESTATUS_TABLE];
RadioHeadStatus              g_RadioHeadStatus[SINGLE_ROW_TABLE];
MacGenCounters               g_MacGenCounters[SINGLE_ROW_TABLE];
MacUplaneCounters            g_MacUplaneCounters[SINGLE_ROW_TABLE];
RlcGenCounters               g_RlcGenCounters[SINGLE_ROW_TABLE];
PdcpGenCounters              g_PdcpGenCounters[SINGLE_ROW_TABLE];
PdcpUplaneCounters           g_PdcpUplaneCounters[SINGLE_ROW_TABLE];
GtpGenCounters               g_GtpGenCounters[SINGLE_ROW_TABLE];
SctpGenCounters              g_SctpGenCounters[SINGLE_ROW_TABLE];
FileEventHistory             g_FileEventHistory[MAX_ROWS_IN_FILEEVENTHISTORY_TABLE];
SwInstallHistory             g_SwInstallHistory[MAX_ROWS_IN_SWINSTALLHISTORY_TABLE];
ActiveUeStatus               g_ActiveUeStatus[MAX_ROWS_IN_ACTIVEUESTATUS_TABLE];
ActiveRabStatus              g_ActiveRabStatus[MAX_ROWS_IN_ACTIVERABSTATUS_TABLE];
UeEventHistory               g_UeEventHistory[MAX_ROWS_IN_UEEVENTHISTORY_TABLE];
RabHistory                   g_RabHistory[MAX_ROWS_IN_RABHISTORY_TABLE];
PmLogFileStatus              g_PmLogFileStatus[MAX_ROWS_IN_PMLOGFILESTATUS_TABLE];
ModuleLogFileStatus          g_ModuleLogFileStatus[MAX_ROWS_IN_MODULELOGFILESTATUS_TABLE];
ActiveAlarmStatus            g_ActiveAlarmStatus[MAX_ROWS_IN_ACTIVEALARMSTATUS_TABLE];
AlarmHistory                 g_AlarmHistory[MAX_ROWS_IN_ALARMHISTORY_TABLE];
LocationStatus               g_LocationStatus[SINGLE_ROW_TABLE];
GpsStatus                    g_GpsStatus[SINGLE_ROW_TABLE];
RrcGenCounters               g_RrcGenCounters[SINGLE_ROW_TABLE];
RrmCounters                  g_RrmCounters[SINGLE_ROW_TABLE];
RrcCellCounters              g_RrcCellCounters[SINGLE_ROW_TABLE];
S1apGenCounters              g_S1apGenCounters[SINGLE_ROW_TABLE];
S1apPeerNodeCounters         g_S1apPeerNodeCounters[SINGLE_ROW_TABLE];
EthernetCounters             g_EthernetCounters[MAX_ROWS_IN_ETHERNETCOUNTERS_TABLE];
IpCounters                   g_IpCounters[MAX_ROWS_IN_IPCOUNTERS_TABLE];
LoginStatus                  g_LoginStatus[MAX_ROWS_IN_LOGINSTATUS_TABLE];
LedStatus                    g_LedStatus[MAX_ROWS_IN_LEDSTATUS_TABLE];
CellStatus                   g_CellStatus[SINGLE_ROW_TABLE];
L2Counters                   g_L2Counters[MAX_ROWS_IN_L2COUNTERS_TABLE];
CounterCollectionStatus      g_CounterCollectionStatus[MAX_ROWS_IN_COUNTERCOLLECTIONSTATUS_TABLE];
TraceHistory                 g_TraceHistory[MAX_ROWS_IN_TRACEHISTORY_TABLE];
TraceStatus                  g_TraceStatus[SINGLE_ROW_TABLE];
EnbRegistrationConfig        g_EnbRegistrationConfig[SINGLE_ROW_TABLE];
EnodebConfig                 g_EnodebConfig[SINGLE_ROW_TABLE];
MmeConfig                    g_MmeConfig[SINGLE_ROW_TABLE];
CellConfig                   g_CellConfig[SINGLE_ROW_TABLE];
SchedulerConfig              g_SchedulerConfig[SINGLE_ROW_TABLE];
PhyConfig                    g_PhyConfig[SINGLE_ROW_TABLE];
SysInfo1Config               g_SysInfo1Config[SINGLE_ROW_TABLE];
SysInfo2Config               g_SysInfo2Config[SINGLE_ROW_TABLE];
LogConfig                    g_LogConfig[MAX_ROWS_IN_LOGCONFIG_TABLE];
RadioHeadConfig              g_RadioHeadConfig[SINGLE_ROW_TABLE];
QosConfig                    g_QosConfig[SINGLE_ROW_TABLE];
PmCounterLogConfig           g_PmCounterLogConfig[SINGLE_ROW_TABLE];
EutraHoProfileConfig         g_EutraHoProfileConfig[SINGLE_ROW_TABLE];
EutraNeighCellConfig         g_EutraNeighCellConfig[MAX_ROWS_IN_EUTRANEIGHCELLCONFIG_TABLE];
SysInfo3Config               g_SysInfo3Config[SINGLE_ROW_TABLE];
SonConfig                    g_SonConfig[SINGLE_ROW_TABLE];
SrsConfigDedicated           g_SrsConfigDedicated[SINGLE_ROW_TABLE];
SecurityConfig               g_SecurityConfig[SINGLE_ROW_TABLE];
UeEventHistoryLogConfig      g_UeEventHistoryLogConfig[SINGLE_ROW_TABLE];
CounterConfig                g_CounterConfig[MAX_ROWS_IN_COUNTERCONFIG_TABLE];
TraceConfig                  g_TraceConfig[SINGLE_ROW_TABLE];
MsgTrace                     g_MsgTrace[MAX_ROWS_IN_MSGTRACE_TABLE];
TimersConfig                 g_TimersConfig[MAX_ROWS_IN_TIMERSCONFIG_TABLE];
ProcessStart                 g_ProcessStart[MAX_ROWS_IN_PROCESSSTART_TABLE];
ModuleStop                   g_ModuleStop[MAX_ROWS_IN_MODULESTOP_TABLE];
EventThresholds              g_EventThresholds[SINGLE_ROW_TABLE];
EnbAddresses                 g_EnbAddresses[SINGLE_ROW_TABLE];
UdhConfig                    g_UdhConfig[SINGLE_ROW_TABLE];
UehConfig                    g_UehConfig[SINGLE_ROW_TABLE];
EgtpConfig                   g_EgtpConfig[SINGLE_ROW_TABLE];
SctpConfig                   g_SctpConfig[SINGLE_ROW_TABLE];
S1apConfig                   g_S1apConfig[SINGLE_ROW_TABLE];
DeviceMonitorConfig          g_DeviceMonitorConfig[SINGLE_ROW_TABLE];

static void fillOprCmdFunctionTable(void)
{
  clearBytes(&g_OprCmdTable, (sizeof(GenericTable)* MAX_NUM_OPR_CMD_TABLES));
  
  /* EnbAdmin Table */
  
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getData                               = (void*)getDataEnbAdminTable; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getTableFunc                          = (void*)getTableEnbAdminTable; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getRowFunc                            = (void*)getRowEnbAdminTable; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getElementFunc                        = (void*)getElementEnbAdminTable; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc                     = (void*)struct2TlvEnbAdminTable; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getTlv2StructFunc                     = (void*)tlv2StructEnbAdminTable; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getReadFromFileFunc                   = NULL; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getWriteToFileFunc                    = NULL; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc                   = NULL;/* (void*)writeAllowedEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getPrintFunc                          = NULL;/* (void*)printEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getEmptyTableFunc                     = (void*)emptyTableEnbAdminTable; 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getCreateTableFunc                    = NULL;/* (void*)createTableEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getDeleteTableFunc                    = NULL;/* (void*)deleteTableEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getNumOfRows                          = NULL;/* (void*)numOfRowsEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getInsertRowFunc                      = NULL;/* (void*)insertRowEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                      = NULL;/* (void*)deleteRowEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                      = NULL;/* (void*)updateRowEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getInsertElementFunc                  = NULL;/* (void*)insertElementEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getDeleteElementFunc                  = NULL;/* (void*)deleteElementEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc               = NULL;/* (void*)isElementPresentEnbAdminTable; */ 
  g_OprCmdTable[ENBADMIN-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc                   = NULL;/* (void*)isRowPresentEnbAdminTable; */ 
  
  /* FileUpload Table */
  
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getData                             = (void*)getDataFileUploadTable; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getTableFunc                        = (void*)getTableFileUploadTable; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getRowFunc                          = (void*)getRowFileUploadTable; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getElementFunc                      = (void*)getElementFileUploadTable; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvFileUploadTable; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getTlv2StructFunc                   = (void*)tlv2StructFileUploadTable; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getReadFromFileFunc                 = NULL; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getWriteToFileFunc                  = NULL; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getPrintFunc                        = NULL;/* (void*)printFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getEmptyTableFunc                   = (void*)emptyTableFileUploadTable; 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentFileUploadTable; */ 
  g_OprCmdTable[FILEUPLOAD-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentFileUploadTable; */ 
  
  /* FileDownload Table */
  
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getData                           = (void*)getDataFileDownloadTable; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getTableFunc                      = (void*)getTableFileDownloadTable; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getRowFunc                        = (void*)getRowFileDownloadTable; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getElementFunc                    = (void*)getElementFileDownloadTable; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc                 = (void*)struct2TlvFileDownloadTable; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getTlv2StructFunc                 = (void*)tlv2StructFileDownloadTable; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getReadFromFileFunc               = NULL; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getWriteToFileFunc                = NULL; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc               = NULL;/* (void*)writeAllowedFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getPrintFunc                      = NULL;/* (void*)printFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getEmptyTableFunc                 = (void*)emptyTableFileDownloadTable; 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getCreateTableFunc                = NULL;/* (void*)createTableFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getDeleteTableFunc                = NULL;/* (void*)deleteTableFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getNumOfRows                      = NULL;/* (void*)numOfRowsFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getInsertRowFunc                  = NULL;/* (void*)insertRowFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                  = NULL;/* (void*)deleteRowFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                  = NULL;/* (void*)updateRowFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getInsertElementFunc              = NULL;/* (void*)insertElementFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getDeleteElementFunc              = NULL;/* (void*)deleteElementFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc           = NULL;/* (void*)isElementPresentFileDownloadTable; */ 
  g_OprCmdTable[FILEDOWNLOAD-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc               = NULL;/* (void*)isRowPresentFileDownloadTable; */ 
  
  /* SwActivation Table */
  
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getData                           = (void*)getDataSwActivationTable; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getTableFunc                      = (void*)getTableSwActivationTable; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getRowFunc                        = (void*)getRowSwActivationTable; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getElementFunc                    = (void*)getElementSwActivationTable; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc                 = (void*)struct2TlvSwActivationTable; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getTlv2StructFunc                 = (void*)tlv2StructSwActivationTable; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getReadFromFileFunc               = NULL; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getWriteToFileFunc                = NULL; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc               = NULL;/* (void*)writeAllowedSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getPrintFunc                      = NULL;/* (void*)printSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getEmptyTableFunc                 = (void*)emptyTableSwActivationTable; 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getCreateTableFunc                = NULL;/* (void*)createTableSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getDeleteTableFunc                = NULL;/* (void*)deleteTableSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getNumOfRows                      = NULL;/* (void*)numOfRowsSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getInsertRowFunc                  = NULL;/* (void*)insertRowSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                  = NULL;/* (void*)deleteRowSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                  = NULL;/* (void*)updateRowSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getInsertElementFunc              = NULL;/* (void*)insertElementSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getDeleteElementFunc              = NULL;/* (void*)deleteElementSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc           = NULL;/* (void*)isElementPresentSwActivationTable; */ 
  g_OprCmdTable[SWACTIVATION-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc               = NULL;/* (void*)isRowPresentSwActivationTable; */ 
  
  /* SwInstall Table */
  
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getData                              = (void*)getDataSwInstallTable; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getTableFunc                         = (void*)getTableSwInstallTable; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getRowFunc                           = (void*)getRowSwInstallTable; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getElementFunc                       = (void*)getElementSwInstallTable; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvSwInstallTable; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructSwInstallTable; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getReadFromFileFunc                  = NULL; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getWriteToFileFunc                   = NULL; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getPrintFunc                         = NULL;/* (void*)printSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getEmptyTableFunc                    = (void*)emptyTableSwInstallTable; 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentSwInstallTable; */ 
  g_OprCmdTable[SWINSTALL-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentSwInstallTable; */ 
  
  /* TraceControl Table */
  
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getData                           = (void*)getDataTraceControlTable; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getTableFunc                      = (void*)getTableTraceControlTable; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getRowFunc                        = (void*)getRowTraceControlTable; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getElementFunc                    = (void*)getElementTraceControlTable; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc                 = (void*)struct2TlvTraceControlTable; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getTlv2StructFunc                 = (void*)tlv2StructTraceControlTable; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getReadFromFileFunc               = NULL; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getWriteToFileFunc                = NULL; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc               = NULL;/* (void*)writeAllowedTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getPrintFunc                      = NULL;/* (void*)printTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getEmptyTableFunc                 = (void*)emptyTableTraceControlTable; 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getCreateTableFunc                = NULL;/* (void*)createTableTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getDeleteTableFunc                = NULL;/* (void*)deleteTableTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getNumOfRows                      = NULL;/* (void*)numOfRowsTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getInsertRowFunc                  = NULL;/* (void*)insertRowTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                  = NULL;/* (void*)deleteRowTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                  = NULL;/* (void*)updateRowTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getInsertElementFunc              = NULL;/* (void*)insertElementTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getDeleteElementFunc              = NULL;/* (void*)deleteElementTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc           = NULL;/* (void*)isElementPresentTraceControlTable; */ 
  g_OprCmdTable[TRACECONTROL-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc               = NULL;/* (void*)isRowPresentTraceControlTable; */ 
  
  /* CellAdmin Table */
  
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getData                              = (void*)getDataCellAdminTable; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getTableFunc                         = (void*)getTableCellAdminTable; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getRowFunc                           = (void*)getRowCellAdminTable; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getElementFunc                       = (void*)getElementCellAdminTable; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvCellAdminTable; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructCellAdminTable; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getReadFromFileFunc                  = NULL; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getWriteToFileFunc                   = NULL; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getPrintFunc                         = NULL;/* (void*)printCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getEmptyTableFunc                    = (void*)emptyTableCellAdminTable; 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentCellAdminTable; */ 
  g_OprCmdTable[CELLADMIN-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentCellAdminTable; */ 
  
  /* CounterControl Table */
  
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getData                         = (void*)getDataCounterControlTable; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getTableFunc                    = (void*)getTableCounterControlTable; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getRowFunc                      = (void*)getRowCounterControlTable; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getElementFunc                  = (void*)getElementCounterControlTable; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvCounterControlTable; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getTlv2StructFunc               = (void*)tlv2StructCounterControlTable; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getReadFromFileFunc             = NULL; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getWriteToFileFunc              = NULL; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getPrintFunc                    = NULL;/* (void*)printCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getEmptyTableFunc               = (void*)emptyTableCounterControlTable; 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentCounterControlTable; */ 
  g_OprCmdTable[COUNTERCONTROL-MIN_OPR_CMD_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentCounterControlTable; */ 
}
static void fillOprCfgFunctionTable(void)
{
  clearBytes(g_OprCfgTable, (sizeof(GenericTable)* MAX_NUM_OPR_CFG_TABLES));
  
  /* EnbRegistrationConfig Table */
  
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getData                  = (void*)getDataEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc             = (void*)getTableEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc               = (void*)getRowEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc           = (void*)getElementEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc        = (void*)struct2TlvEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc        = (void*)tlv2StructEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc      = (void*)readFromFileEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc       = (void*)writeToFileEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc      = NULL;/* (void*)writeAllowedEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc             = NULL;/* (void*)printEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc        = (void*)emptyTableEnbRegistrationConfigTable; 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc       = NULL;/* (void*)createTableEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc       = NULL;/* (void*)deleteTableEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows             = NULL;/* (void*)numOfRowsEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc         = NULL;/* (void*)insertRowEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc         = NULL;/* (void*)deleteRowEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc         = NULL;/* (void*)updateRowEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc     = NULL;/* (void*)insertElementEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc     = NULL;/* (void*)deleteElementEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc  = NULL;/* (void*)isElementPresentEnbRegistrationConfigTable; */ 
  g_OprCfgTable[ENBREGISTRATIONCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc      = NULL;/* (void*)isRowPresentEnbRegistrationConfigTable; */ 
  
  /* EnodebConfig Table */
  
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getData                           = (void*)getDataEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                      = (void*)getTableEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                        = (void*)getRowEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                    = (void*)getElementEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                 = (void*)struct2TlvEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                 = (void*)tlv2StructEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc               = (void*)readFromFileEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                = (void*)writeToFileEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc               = NULL;/* (void*)writeAllowedEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                      = NULL;/* (void*)printEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                 = (void*)emptyTableEnodebConfigTable; 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                = NULL;/* (void*)createTableEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                = NULL;/* (void*)deleteTableEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                      = NULL;/* (void*)numOfRowsEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                  = NULL;/* (void*)insertRowEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                  = NULL;/* (void*)deleteRowEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                  = NULL;/* (void*)updateRowEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc              = NULL;/* (void*)insertElementEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc              = NULL;/* (void*)deleteElementEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc           = NULL;/* (void*)isElementPresentEnodebConfigTable; */ 
  g_OprCfgTable[ENODEBCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc               = NULL;/* (void*)isRowPresentEnodebConfigTable; */ 
  
  /* MmeConfig Table */
  
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getData                              = (void*)getDataMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                         = (void*)getTableMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                           = (void*)getRowMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                       = (void*)getElementMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc                  = (void*)readFromFileMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                   = (void*)writeToFileMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                         = NULL;/* (void*)printMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                    = (void*)emptyTableMmeConfigTable; 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentMmeConfigTable; */ 
  g_OprCfgTable[MMECONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentMmeConfigTable; */ 
  
  /* CellConfig Table */
  
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getData                             = (void*)getDataCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                        = (void*)getTableCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                          = (void*)getRowCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                      = (void*)getElementCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                   = (void*)tlv2StructCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc                 = (void*)readFromFileCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                  = (void*)writeToFileCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                        = NULL;/* (void*)printCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                   = (void*)emptyTableCellConfigTable; 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentCellConfigTable; */ 
  g_OprCfgTable[CELLCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentCellConfigTable; */ 
  
  /* SchedulerConfig Table */
  
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getData                        = (void*)getDataSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                   = (void*)getTableSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                     = (void*)getRowSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                 = (void*)getElementSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc              = (void*)struct2TlvSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc              = (void*)tlv2StructSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc            = (void*)readFromFileSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc             = (void*)writeToFileSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc            = NULL;/* (void*)writeAllowedSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                   = NULL;/* (void*)printSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc              = (void*)emptyTableSchedulerConfigTable; 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc             = NULL;/* (void*)createTableSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc             = NULL;/* (void*)deleteTableSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                   = NULL;/* (void*)numOfRowsSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc               = NULL;/* (void*)insertRowSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc               = NULL;/* (void*)deleteRowSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc               = NULL;/* (void*)updateRowSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc           = NULL;/* (void*)insertElementSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc           = NULL;/* (void*)deleteElementSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc        = NULL;/* (void*)isElementPresentSchedulerConfigTable; */ 
  g_OprCfgTable[SCHEDULERCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc            = NULL;/* (void*)isRowPresentSchedulerConfigTable; */ 
  
  /* PhyConfig Table */
  
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getData                              = (void*)getDataPhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                         = (void*)getTablePhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                           = (void*)getRowPhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                       = (void*)getElementPhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvPhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructPhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc                  = (void*)readFromFilePhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                   = (void*)writeToFilePhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                         = NULL;/* (void*)printPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                    = (void*)emptyTablePhyConfigTable; 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTablePhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTablePhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentPhyConfigTable; */ 
  g_OprCfgTable[PHYCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentPhyConfigTable; */ 
  
  /* SysInfo1Config Table */
  
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getData                         = (void*)getDataSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                    = (void*)getTableSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                      = (void*)getRowSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                  = (void*)getElementSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc               = (void*)tlv2StructSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc             = (void*)readFromFileSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc              = (void*)writeToFileSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                    = NULL;/* (void*)printSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc               = (void*)emptyTableSysInfo1ConfigTable; 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentSysInfo1ConfigTable; */ 
  g_OprCfgTable[SYSINFO1CONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentSysInfo1ConfigTable; */ 
  
  /* SysInfo2Config Table */
  
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getData                         = (void*)getDataSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                    = (void*)getTableSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                      = (void*)getRowSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                  = (void*)getElementSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc               = (void*)tlv2StructSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc             = (void*)readFromFileSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc              = (void*)writeToFileSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                    = NULL;/* (void*)printSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc               = (void*)emptyTableSysInfo2ConfigTable; 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentSysInfo2ConfigTable; */ 
  g_OprCfgTable[SYSINFO2CONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentSysInfo2ConfigTable; */ 
  
  /* LogConfig Table */
  
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getData                              = (void*)getDataLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                         = (void*)getTableLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                           = (void*)getRowLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                       = (void*)getElementLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc                  = (void*)readFromFileLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                   = (void*)writeToFileLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                         = NULL;/* (void*)printLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                    = (void*)emptyTableLogConfigTable; 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentLogConfigTable; */ 
  g_OprCfgTable[LOGCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentLogConfigTable; */ 
  
  /* RadioHeadConfig Table */
  
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getData                        = (void*)getDataRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                   = (void*)getTableRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                     = (void*)getRowRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                 = (void*)getElementRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc              = (void*)struct2TlvRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc              = (void*)tlv2StructRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc            = (void*)readFromFileRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc             = (void*)writeToFileRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc            = NULL;/* (void*)writeAllowedRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                   = NULL;/* (void*)printRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc              = (void*)emptyTableRadioHeadConfigTable; 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc             = NULL;/* (void*)createTableRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc             = NULL;/* (void*)deleteTableRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                   = NULL;/* (void*)numOfRowsRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc               = NULL;/* (void*)insertRowRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc               = NULL;/* (void*)deleteRowRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc               = NULL;/* (void*)updateRowRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc           = NULL;/* (void*)insertElementRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc           = NULL;/* (void*)deleteElementRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc        = NULL;/* (void*)isElementPresentRadioHeadConfigTable; */ 
  g_OprCfgTable[RADIOHEADCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc            = NULL;/* (void*)isRowPresentRadioHeadConfigTable; */ 
  
  /* QosConfig Table */
  
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getData                              = (void*)getDataQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                         = (void*)getTableQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                           = (void*)getRowQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                       = (void*)getElementQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc                  = (void*)readFromFileQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                   = (void*)writeToFileQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                         = NULL;/* (void*)printQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                    = (void*)emptyTableQosConfigTable; 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentQosConfigTable; */ 
  g_OprCfgTable[QOSCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentQosConfigTable; */ 
  
  /* PmCounterLogConfig Table */
  
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getData                     = (void*)getDataPmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                = (void*)getTablePmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                  = (void*)getRowPmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc              = (void*)getElementPmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc           = (void*)struct2TlvPmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc           = (void*)tlv2StructPmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc         = (void*)readFromFilePmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc          = (void*)writeToFilePmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc         = NULL;/* (void*)writeAllowedPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                = NULL;/* (void*)printPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc           = (void*)emptyTablePmCounterLogConfigTable; 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc          = NULL;/* (void*)createTablePmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc          = NULL;/* (void*)deleteTablePmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                = NULL;/* (void*)numOfRowsPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc            = NULL;/* (void*)insertRowPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc            = NULL;/* (void*)deleteRowPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc            = NULL;/* (void*)updateRowPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc        = NULL;/* (void*)insertElementPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc        = NULL;/* (void*)deleteElementPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc     = NULL;/* (void*)isElementPresentPmCounterLogConfigTable; */ 
  g_OprCfgTable[PMCOUNTERLOGCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc         = NULL;/* (void*)isRowPresentPmCounterLogConfigTable; */ 
  
  /* EutraHoProfileConfig Table */
  
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getData                   = (void*)getDataEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getTableFunc              = (void*)getTableEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                = (void*)getRowEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getElementFunc            = (void*)getElementEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc         = (void*)struct2TlvEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc         = (void*)tlv2StructEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc       = (void*)readFromFileEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc        = (void*)writeToFileEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc       = NULL;/* (void*)writeAllowedEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc              = NULL;/* (void*)printEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc         = (void*)emptyTableEutraHoProfileConfigTable; 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc        = NULL;/* (void*)createTableEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc        = NULL;/* (void*)deleteTableEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows              = NULL;/* (void*)numOfRowsEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc          = NULL;/* (void*)insertRowEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc          = NULL;/* (void*)deleteRowEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc          = NULL;/* (void*)updateRowEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc      = NULL;/* (void*)insertElementEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc      = NULL;/* (void*)deleteElementEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc   = NULL;/* (void*)isElementPresentEutraHoProfileConfigTable; */ 
  g_OprCfgTable[EUTRAHOPROFILECONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc       = NULL;/* (void*)isRowPresentEutraHoProfileConfigTable; */ 
  
  /* EutraNeighCellConfig Table */
  
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getData                   = (void*)getDataEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc              = (void*)getTableEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                = (void*)getRowEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc            = (void*)getElementEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc         = (void*)struct2TlvEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc         = (void*)tlv2StructEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc       = (void*)readFromFileEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc        = (void*)writeToFileEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc       = NULL;/* (void*)writeAllowedEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc              = NULL;/* (void*)printEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc         = (void*)emptyTableEutraNeighCellConfigTable; 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc        = NULL;/* (void*)createTableEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc        = NULL;/* (void*)deleteTableEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows              = NULL;/* (void*)numOfRowsEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc          = NULL;/* (void*)insertRowEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc          = NULL;/* (void*)deleteRowEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc          = NULL;/* (void*)updateRowEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc      = NULL;/* (void*)insertElementEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc      = NULL;/* (void*)deleteElementEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc   = NULL;/* (void*)isElementPresentEutraNeighCellConfigTable; */ 
  g_OprCfgTable[EUTRANEIGHCELLCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc       = NULL;/* (void*)isRowPresentEutraNeighCellConfigTable; */ 
  
  /* SysInfo3Config Table */
  
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getData                         = (void*)getDataSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                    = (void*)getTableSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                      = (void*)getRowSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                  = (void*)getElementSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc               = (void*)tlv2StructSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc             = (void*)readFromFileSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc              = (void*)writeToFileSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                    = NULL;/* (void*)printSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc               = (void*)emptyTableSysInfo3ConfigTable; 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentSysInfo3ConfigTable; */ 
  g_OprCfgTable[SYSINFO3CONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentSysInfo3ConfigTable; */ 
  
  /* SonConfig Table */
  
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getData                              = (void*)getDataSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                         = (void*)getTableSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                           = (void*)getRowSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                       = (void*)getElementSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc                  = (void*)readFromFileSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                   = (void*)writeToFileSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                         = NULL;/* (void*)printSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                    = (void*)emptyTableSonConfigTable; 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentSonConfigTable; */ 
  g_OprCfgTable[SONCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentSonConfigTable; */ 
  
  /* SrsConfigDedicated Table */
  
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getData                     = (void*)getDataSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getTableFunc                = (void*)getTableSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getRowFunc                  = (void*)getRowSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getElementFunc              = (void*)getElementSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc           = (void*)struct2TlvSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getTlv2StructFunc           = (void*)tlv2StructSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getReadFromFileFunc         = (void*)readFromFileSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getWriteToFileFunc          = (void*)writeToFileSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc         = NULL;/* (void*)writeAllowedSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getPrintFunc                = NULL;/* (void*)printSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getEmptyTableFunc           = (void*)emptyTableSrsConfigDedicatedTable; 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getCreateTableFunc          = NULL;/* (void*)createTableSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getDeleteTableFunc          = NULL;/* (void*)deleteTableSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getNumOfRows                = NULL;/* (void*)numOfRowsSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getInsertRowFunc            = NULL;/* (void*)insertRowSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getDeleteRowFunc            = NULL;/* (void*)deleteRowSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getUpdateRowFunc            = NULL;/* (void*)updateRowSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getInsertElementFunc        = NULL;/* (void*)insertElementSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getDeleteElementFunc        = NULL;/* (void*)deleteElementSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc     = NULL;/* (void*)isElementPresentSrsConfigDedicatedTable; */ 
  g_OprCfgTable[SRSCONFIGDEDICATED-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc         = NULL;/* (void*)isRowPresentSrsConfigDedicatedTable; */ 
  
  /* SecurityConfig Table */
  
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getData                         = (void*)getDataSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                    = (void*)getTableSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                      = (void*)getRowSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                  = (void*)getElementSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc               = (void*)tlv2StructSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc             = (void*)readFromFileSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc              = (void*)writeToFileSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                    = NULL;/* (void*)printSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc               = (void*)emptyTableSecurityConfigTable; 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentSecurityConfigTable; */ 
  g_OprCfgTable[SECURITYCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentSecurityConfigTable; */ 
  
  /* UeEventHistoryLogConfig Table */
  
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getData                = (void*)getDataUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc           = (void*)getTableUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc             = (void*)getRowUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc         = (void*)getElementUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc      = (void*)struct2TlvUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc      = (void*)tlv2StructUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc    = (void*)readFromFileUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc     = (void*)writeToFileUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc    = NULL;/* (void*)writeAllowedUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc           = NULL;/* (void*)printUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc      = (void*)emptyTableUeEventHistoryLogConfigTable; 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc     = NULL;/* (void*)createTableUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc     = NULL;/* (void*)deleteTableUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows           = NULL;/* (void*)numOfRowsUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc       = NULL;/* (void*)insertRowUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc       = NULL;/* (void*)deleteRowUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc       = NULL;/* (void*)updateRowUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc   = NULL;/* (void*)insertElementUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc   = NULL;/* (void*)deleteElementUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc= NULL;/* (void*)isElementPresentUeEventHistoryLogConfigTable; */ 
  g_OprCfgTable[UEEVENTHISTORYLOGCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc    = NULL;/* (void*)isRowPresentUeEventHistoryLogConfigTable; */ 
  
  /* CounterConfig Table */
  
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getData                          = (void*)getDataCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                     = (void*)getTableCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                       = (void*)getRowCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                   = (void*)getElementCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                = (void*)tlv2StructCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc              = (void*)readFromFileCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc               = (void*)writeToFileCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                     = NULL;/* (void*)printCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                = (void*)emptyTableCounterConfigTable; 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentCounterConfigTable; */ 
  g_OprCfgTable[COUNTERCONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentCounterConfigTable; */ 
  
  /* TraceConfig Table */
  
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getData                            = (void*)getDataTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getTableFunc                       = (void*)getTableTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getRowFunc                         = (void*)getRowTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getElementFunc                     = (void*)getElementTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getStruct2TlvFunc                  = (void*)struct2TlvTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getTlv2StructFunc                  = (void*)tlv2StructTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getReadFromFileFunc                = (void*)readFromFileTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getWriteToFileFunc                 = (void*)writeToFileTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getWriteAllowedFunc                = NULL;/* (void*)writeAllowedTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getPrintFunc                       = NULL;/* (void*)printTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getEmptyTableFunc                  = (void*)emptyTableTraceConfigTable; 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getCreateTableFunc                 = NULL;/* (void*)createTableTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getDeleteTableFunc                 = NULL;/* (void*)deleteTableTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getNumOfRows                       = NULL;/* (void*)numOfRowsTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getInsertRowFunc                   = NULL;/* (void*)insertRowTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getDeleteRowFunc                   = NULL;/* (void*)deleteRowTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getUpdateRowFunc                   = NULL;/* (void*)updateRowTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getInsertElementFunc               = NULL;/* (void*)insertElementTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getDeleteElementFunc               = NULL;/* (void*)deleteElementTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getIsElementPresentFunc            = NULL;/* (void*)isElementPresentTraceConfigTable; */ 
  g_OprCfgTable[TRACECONFIG-MIN_OPR_CFG_TABLEID].getIsRowPresentFunc                = NULL;/* (void*)isRowPresentTraceConfigTable; */ 
}
static void fillStatusFunctionTable(void)
{
  clearBytes(g_StatusTable, (sizeof(GenericTable)* MAX_NUM_STATUS_TABLES));
  
  /* FileUploadStatus Table */
  
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getData                        = (void*)getDataFileUploadStatusTable; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getTableFunc                   = (void*)getTableFileUploadStatusTable; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getRowFunc                     = (void*)getRowFileUploadStatusTable; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getElementFunc                 = (void*)getElementFileUploadStatusTable; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc              = (void*)struct2TlvFileUploadStatusTable; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc              = NULL; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc            = NULL; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc             = NULL; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc            = NULL;/* (void*)writeAllowedFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getPrintFunc                   = NULL;/* (void*)printFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc              = (void*)emptyTableFileUploadStatusTable; 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc             = NULL;/* (void*)createTableFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc             = NULL;/* (void*)deleteTableFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getNumOfRows                   = NULL;/* (void*)numOfRowsFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc               = NULL;/* (void*)insertRowFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc               = NULL;/* (void*)deleteRowFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc               = NULL;/* (void*)updateRowFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc           = NULL;/* (void*)insertElementFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc           = NULL;/* (void*)deleteElementFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc        = NULL;/* (void*)isElementPresentFileUploadStatusTable; */ 
  g_StatusTable[FILEUPLOADSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc            = NULL;/* (void*)isRowPresentFileUploadStatusTable; */ 
  
  /* FileDownloadStatus Table */
  
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getData                      = (void*)getDataFileDownloadStatusTable; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getTableFunc                 = (void*)getTableFileDownloadStatusTable; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getRowFunc                   = (void*)getRowFileDownloadStatusTable; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getElementFunc               = (void*)getElementFileDownloadStatusTable; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc            = (void*)struct2TlvFileDownloadStatusTable; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc            = NULL; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc          = NULL; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc           = NULL; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc          = NULL;/* (void*)writeAllowedFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getPrintFunc                 = NULL;/* (void*)printFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc            = (void*)emptyTableFileDownloadStatusTable; 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc           = NULL;/* (void*)createTableFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc           = NULL;/* (void*)deleteTableFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getNumOfRows                 = NULL;/* (void*)numOfRowsFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc             = NULL;/* (void*)insertRowFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc             = NULL;/* (void*)deleteRowFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc             = NULL;/* (void*)updateRowFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc         = NULL;/* (void*)insertElementFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc         = NULL;/* (void*)deleteElementFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc      = NULL;/* (void*)isElementPresentFileDownloadStatusTable; */ 
  g_StatusTable[FILEDOWNLOADSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc          = NULL;/* (void*)isRowPresentFileDownloadStatusTable; */ 
  
  /* SwActivationStatus Table */
  
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getData                      = (void*)getDataSwActivationStatusTable; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getTableFunc                 = (void*)getTableSwActivationStatusTable; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getRowFunc                   = (void*)getRowSwActivationStatusTable; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getElementFunc               = (void*)getElementSwActivationStatusTable; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc            = (void*)struct2TlvSwActivationStatusTable; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc            = NULL; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc          = NULL; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc           = NULL; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc          = NULL;/* (void*)writeAllowedSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getPrintFunc                 = NULL;/* (void*)printSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc            = (void*)emptyTableSwActivationStatusTable; 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc           = NULL;/* (void*)createTableSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc           = NULL;/* (void*)deleteTableSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getNumOfRows                 = NULL;/* (void*)numOfRowsSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc             = NULL;/* (void*)insertRowSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc             = NULL;/* (void*)deleteRowSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc             = NULL;/* (void*)updateRowSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc         = NULL;/* (void*)insertElementSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc         = NULL;/* (void*)deleteElementSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc      = NULL;/* (void*)isElementPresentSwActivationStatusTable; */ 
  g_StatusTable[SWACTIVATIONSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc          = NULL;/* (void*)isRowPresentSwActivationStatusTable; */ 
  
  /* SwPackageStatus Table */
  
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getData                         = (void*)getDataSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTableSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc             = (void*)readFromFileSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc              = (void*)writeToFileSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTableSwPackageStatusTable; 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentSwPackageStatusTable; */ 
  g_StatusTable[SWPACKAGESTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentSwPackageStatusTable; */ 
  
  /* ComponentStatus Table */
  
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getData                         = (void*)getDataComponentStatusTable; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTableComponentStatusTable; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowComponentStatusTable; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementComponentStatusTable; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvComponentStatusTable; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc             = NULL; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc              = NULL; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTableComponentStatusTable; 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentComponentStatusTable; */ 
  g_StatusTable[COMPONENTSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentComponentStatusTable; */ 
  
  /* EnbStatus Table */
  
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getData                               = (void*)getDataEnbStatusTable; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getTableFunc                          = (void*)getTableEnbStatusTable; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getRowFunc                            = (void*)getRowEnbStatusTable; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getElementFunc                        = (void*)getElementEnbStatusTable; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                     = (void*)struct2TlvEnbStatusTable; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                     = NULL; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc                   = NULL; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc                    = NULL; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc                   = NULL;/* (void*)writeAllowedEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getPrintFunc                          = NULL;/* (void*)printEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                     = (void*)emptyTableEnbStatusTable; 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc                    = NULL;/* (void*)createTableEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc                    = NULL;/* (void*)deleteTableEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getNumOfRows                          = NULL;/* (void*)numOfRowsEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                      = NULL;/* (void*)insertRowEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                      = NULL;/* (void*)deleteRowEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                      = NULL;/* (void*)updateRowEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc                  = NULL;/* (void*)insertElementEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc                  = NULL;/* (void*)deleteElementEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc               = NULL;/* (void*)isElementPresentEnbStatusTable; */ 
  g_StatusTable[ENBSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc                   = NULL;/* (void*)isRowPresentEnbStatusTable; */ 
  
  /* ModuleStatus Table */
  
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getData                            = (void*)getDataModuleStatusTable; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getTableFunc                       = (void*)getTableModuleStatusTable; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getRowFunc                         = (void*)getRowModuleStatusTable; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getElementFunc                     = (void*)getElementModuleStatusTable; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                  = (void*)struct2TlvModuleStatusTable; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                  = NULL; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc                = NULL; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc                 = NULL; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc                = NULL;/* (void*)writeAllowedModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getPrintFunc                       = NULL;/* (void*)printModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                  = (void*)emptyTableModuleStatusTable; 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getCreateTableFunc                 = NULL;/* (void*)createTableModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc                 = NULL;/* (void*)deleteTableModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getNumOfRows                       = NULL;/* (void*)numOfRowsModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                   = NULL;/* (void*)insertRowModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                   = NULL;/* (void*)deleteRowModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                   = NULL;/* (void*)updateRowModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getInsertElementFunc               = NULL;/* (void*)insertElementModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc               = NULL;/* (void*)deleteElementModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc            = NULL;/* (void*)isElementPresentModuleStatusTable; */ 
  g_StatusTable[MODULESTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc                = NULL;/* (void*)isRowPresentModuleStatusTable; */ 
  
  /* RadioHeadStatus Table */
  
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getData                         = (void*)getDataRadioHeadStatusTable; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTableRadioHeadStatusTable; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowRadioHeadStatusTable; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementRadioHeadStatusTable; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvRadioHeadStatusTable; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc             = NULL; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc              = NULL; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTableRadioHeadStatusTable; 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentRadioHeadStatusTable; */ 
  g_StatusTable[RADIOHEADSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentRadioHeadStatusTable; */ 
  
  /* MacGenCounters Table */
  
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getData                          = (void*)getDataMacGenCountersTable; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                     = (void*)getTableMacGenCountersTable; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                       = (void*)getRowMacGenCountersTable; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                   = (void*)getElementMacGenCountersTable; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvMacGenCountersTable; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc                = NULL; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc              = NULL; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc               = NULL; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                     = NULL;/* (void*)printMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc                = (void*)emptyTableMacGenCountersTable; 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentMacGenCountersTable; */ 
  g_StatusTable[MACGENCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentMacGenCountersTable; */ 
  
  /* MacUplaneCounters Table */
  
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getData                       = (void*)getDataMacUplaneCountersTable; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getTableFunc                  = (void*)getTableMacUplaneCountersTable; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getRowFunc                    = (void*)getRowMacUplaneCountersTable; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getElementFunc                = (void*)getElementMacUplaneCountersTable; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc             = (void*)struct2TlvMacUplaneCountersTable; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc             = NULL; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc           = NULL; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc            = NULL; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc           = NULL;/* (void*)writeAllowedMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                  = NULL;/* (void*)printMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc             = (void*)emptyTableMacUplaneCountersTable; 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc            = NULL;/* (void*)createTableMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc            = NULL;/* (void*)deleteTableMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                  = NULL;/* (void*)numOfRowsMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc              = NULL;/* (void*)insertRowMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc              = NULL;/* (void*)deleteRowMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc              = NULL;/* (void*)updateRowMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc          = NULL;/* (void*)insertElementMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc          = NULL;/* (void*)deleteElementMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc       = NULL;/* (void*)isElementPresentMacUplaneCountersTable; */ 
  g_StatusTable[MACUPLANECOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc           = NULL;/* (void*)isRowPresentMacUplaneCountersTable; */ 
  
  /* RlcGenCounters Table */
  
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getData                          = (void*)getDataRlcGenCountersTable; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                     = (void*)getTableRlcGenCountersTable; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                       = (void*)getRowRlcGenCountersTable; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                   = (void*)getElementRlcGenCountersTable; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvRlcGenCountersTable; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc                = NULL; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc              = NULL; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc               = NULL; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                     = NULL;/* (void*)printRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc                = (void*)emptyTableRlcGenCountersTable; 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentRlcGenCountersTable; */ 
  g_StatusTable[RLCGENCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentRlcGenCountersTable; */ 
  
  /* PdcpGenCounters Table */
  
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getData                         = (void*)getDataPdcpGenCountersTable; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTablePdcpGenCountersTable; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowPdcpGenCountersTable; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementPdcpGenCountersTable; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvPdcpGenCountersTable; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc             = NULL; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc              = NULL; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTablePdcpGenCountersTable; 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTablePdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTablePdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentPdcpGenCountersTable; */ 
  g_StatusTable[PDCPGENCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentPdcpGenCountersTable; */ 
  
  /* PdcpUplaneCounters Table */
  
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getData                      = (void*)getDataPdcpUplaneCountersTable; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getTableFunc                 = (void*)getTablePdcpUplaneCountersTable; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getRowFunc                   = (void*)getRowPdcpUplaneCountersTable; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getElementFunc               = (void*)getElementPdcpUplaneCountersTable; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc            = (void*)struct2TlvPdcpUplaneCountersTable; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc            = NULL; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc          = NULL; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc           = NULL; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc          = NULL;/* (void*)writeAllowedPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                 = NULL;/* (void*)printPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc            = (void*)emptyTablePdcpUplaneCountersTable; 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc           = NULL;/* (void*)createTablePdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc           = NULL;/* (void*)deleteTablePdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                 = NULL;/* (void*)numOfRowsPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc             = NULL;/* (void*)insertRowPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc             = NULL;/* (void*)deleteRowPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc             = NULL;/* (void*)updateRowPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc         = NULL;/* (void*)insertElementPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc         = NULL;/* (void*)deleteElementPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc      = NULL;/* (void*)isElementPresentPdcpUplaneCountersTable; */ 
  g_StatusTable[PDCPUPLANECOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc          = NULL;/* (void*)isRowPresentPdcpUplaneCountersTable; */ 
  
  /* GtpGenCounters Table */
  
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getData                          = (void*)getDataGtpGenCountersTable; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                     = (void*)getTableGtpGenCountersTable; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                       = (void*)getRowGtpGenCountersTable; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                   = (void*)getElementGtpGenCountersTable; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvGtpGenCountersTable; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc                = NULL; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc              = NULL; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc               = NULL; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                     = NULL;/* (void*)printGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc                = (void*)emptyTableGtpGenCountersTable; 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentGtpGenCountersTable; */ 
  g_StatusTable[GTPGENCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentGtpGenCountersTable; */ 
  
  /* SctpGenCounters Table */
  
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getData                         = (void*)getDataSctpGenCountersTable; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTableSctpGenCountersTable; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowSctpGenCountersTable; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementSctpGenCountersTable; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvSctpGenCountersTable; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc             = NULL; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc              = NULL; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTableSctpGenCountersTable; 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentSctpGenCountersTable; */ 
  g_StatusTable[SCTPGENCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentSctpGenCountersTable; */ 
  
  /* FileEventHistory Table */
  
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getData                        = (void*)getDataFileEventHistoryTable; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getTableFunc                   = (void*)getTableFileEventHistoryTable; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getRowFunc                     = (void*)getRowFileEventHistoryTable; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getElementFunc                 = (void*)getElementFileEventHistoryTable; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getStruct2TlvFunc              = (void*)struct2TlvFileEventHistoryTable; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getTlv2StructFunc              = NULL; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getReadFromFileFunc            = NULL; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getWriteToFileFunc             = NULL; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getWriteAllowedFunc            = NULL;/* (void*)writeAllowedFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getPrintFunc                   = NULL;/* (void*)printFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getEmptyTableFunc              = (void*)emptyTableFileEventHistoryTable; 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getCreateTableFunc             = NULL;/* (void*)createTableFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getDeleteTableFunc             = NULL;/* (void*)deleteTableFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getNumOfRows                   = NULL;/* (void*)numOfRowsFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getInsertRowFunc               = NULL;/* (void*)insertRowFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getDeleteRowFunc               = NULL;/* (void*)deleteRowFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getUpdateRowFunc               = NULL;/* (void*)updateRowFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getInsertElementFunc           = NULL;/* (void*)insertElementFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getDeleteElementFunc           = NULL;/* (void*)deleteElementFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getIsElementPresentFunc        = NULL;/* (void*)isElementPresentFileEventHistoryTable; */ 
  g_StatusTable[FILEEVENTHISTORY-MIN_STATUS_TABLEID].getIsRowPresentFunc            = NULL;/* (void*)isRowPresentFileEventHistoryTable; */ 
  
  /* SwInstallHistory Table */
  
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getData                        = (void*)getDataSwInstallHistoryTable; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getTableFunc                   = (void*)getTableSwInstallHistoryTable; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getRowFunc                     = (void*)getRowSwInstallHistoryTable; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getElementFunc                 = (void*)getElementSwInstallHistoryTable; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getStruct2TlvFunc              = (void*)struct2TlvSwInstallHistoryTable; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getTlv2StructFunc              = NULL; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getReadFromFileFunc            = NULL; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getWriteToFileFunc             = NULL; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getWriteAllowedFunc            = NULL;/* (void*)writeAllowedSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getPrintFunc                   = NULL;/* (void*)printSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getEmptyTableFunc              = (void*)emptyTableSwInstallHistoryTable; 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getCreateTableFunc             = NULL;/* (void*)createTableSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getDeleteTableFunc             = NULL;/* (void*)deleteTableSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getNumOfRows                   = NULL;/* (void*)numOfRowsSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getInsertRowFunc               = NULL;/* (void*)insertRowSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getDeleteRowFunc               = NULL;/* (void*)deleteRowSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getUpdateRowFunc               = NULL;/* (void*)updateRowSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getInsertElementFunc           = NULL;/* (void*)insertElementSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getDeleteElementFunc           = NULL;/* (void*)deleteElementSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getIsElementPresentFunc        = NULL;/* (void*)isElementPresentSwInstallHistoryTable; */ 
  g_StatusTable[SWINSTALLHISTORY-MIN_STATUS_TABLEID].getIsRowPresentFunc            = NULL;/* (void*)isRowPresentSwInstallHistoryTable; */ 
  
  /* ActiveUeStatus Table */
  
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getData                          = (void*)getDataActiveUeStatusTable; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getTableFunc                     = (void*)getTableActiveUeStatusTable; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getRowFunc                       = (void*)getRowActiveUeStatusTable; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getElementFunc                   = (void*)getElementActiveUeStatusTable; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvActiveUeStatusTable; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                = NULL; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc              = NULL; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc               = NULL; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getPrintFunc                     = NULL;/* (void*)printActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                = (void*)emptyTableActiveUeStatusTable; 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentActiveUeStatusTable; */ 
  g_StatusTable[ACTIVEUESTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentActiveUeStatusTable; */ 
  
  /* ActiveRabStatus Table */
  
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getData                         = (void*)getDataActiveRabStatusTable; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTableActiveRabStatusTable; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowActiveRabStatusTable; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementActiveRabStatusTable; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvActiveRabStatusTable; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc             = NULL; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc              = NULL; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTableActiveRabStatusTable; 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentActiveRabStatusTable; */ 
  g_StatusTable[ACTIVERABSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentActiveRabStatusTable; */ 
  
  /* UeEventHistory Table */
  
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getData                          = (void*)getDataUeEventHistoryTable; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getTableFunc                     = (void*)getTableUeEventHistoryTable; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getRowFunc                       = (void*)getRowUeEventHistoryTable; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getElementFunc                   = (void*)getElementUeEventHistoryTable; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvUeEventHistoryTable; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getTlv2StructFunc                = NULL; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getReadFromFileFunc              = NULL; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getWriteToFileFunc               = NULL; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getPrintFunc                     = NULL;/* (void*)printUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getEmptyTableFunc                = (void*)emptyTableUeEventHistoryTable; 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentUeEventHistoryTable; */ 
  g_StatusTable[UEEVENTHISTORY-MIN_STATUS_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentUeEventHistoryTable; */ 
  
  /* RabHistory Table */
  
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getData                              = (void*)getDataRabHistoryTable; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getTableFunc                         = (void*)getTableRabHistoryTable; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getRowFunc                           = (void*)getRowRabHistoryTable; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getElementFunc                       = (void*)getElementRabHistoryTable; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvRabHistoryTable; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getTlv2StructFunc                    = NULL; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getReadFromFileFunc                  = NULL; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getWriteToFileFunc                   = NULL; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getPrintFunc                         = NULL;/* (void*)printRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getEmptyTableFunc                    = (void*)emptyTableRabHistoryTable; 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentRabHistoryTable; */ 
  g_StatusTable[RABHISTORY-MIN_STATUS_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentRabHistoryTable; */ 
  
  /* PmLogFileStatus Table */
  
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getData                         = (void*)getDataPmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTablePmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowPmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementPmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvPmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc             = (void*)readFromFilePmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc              = (void*)writeToFilePmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTablePmLogFileStatusTable; 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTablePmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTablePmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentPmLogFileStatusTable; */ 
  g_StatusTable[PMLOGFILESTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentPmLogFileStatusTable; */ 
  
  /* ModuleLogFileStatus Table */
  
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getData                     = (void*)getDataModuleLogFileStatusTable; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getTableFunc                = (void*)getTableModuleLogFileStatusTable; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getRowFunc                  = (void*)getRowModuleLogFileStatusTable; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getElementFunc              = (void*)getElementModuleLogFileStatusTable; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc           = (void*)struct2TlvModuleLogFileStatusTable; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc           = NULL; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc         = NULL; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc          = NULL; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc         = NULL;/* (void*)writeAllowedModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getPrintFunc                = NULL;/* (void*)printModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc           = (void*)emptyTableModuleLogFileStatusTable; 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getCreateTableFunc          = NULL;/* (void*)createTableModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc          = NULL;/* (void*)deleteTableModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getNumOfRows                = NULL;/* (void*)numOfRowsModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getInsertRowFunc            = NULL;/* (void*)insertRowModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc            = NULL;/* (void*)deleteRowModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc            = NULL;/* (void*)updateRowModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getInsertElementFunc        = NULL;/* (void*)insertElementModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc        = NULL;/* (void*)deleteElementModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc     = NULL;/* (void*)isElementPresentModuleLogFileStatusTable; */ 
  g_StatusTable[MODULELOGFILESTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc         = NULL;/* (void*)isRowPresentModuleLogFileStatusTable; */ 
  
  /* ActiveAlarmStatus Table */
  
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getData                       = (void*)getDataActiveAlarmStatusTable; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getTableFunc                  = (void*)getTableActiveAlarmStatusTable; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getRowFunc                    = (void*)getRowActiveAlarmStatusTable; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getElementFunc                = (void*)getElementActiveAlarmStatusTable; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc             = (void*)struct2TlvActiveAlarmStatusTable; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc             = NULL; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc           = NULL; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc            = NULL; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc           = NULL;/* (void*)writeAllowedActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getPrintFunc                  = NULL;/* (void*)printActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc             = (void*)emptyTableActiveAlarmStatusTable; 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc            = NULL;/* (void*)createTableActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc            = NULL;/* (void*)deleteTableActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getNumOfRows                  = NULL;/* (void*)numOfRowsActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc              = NULL;/* (void*)insertRowActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc              = NULL;/* (void*)deleteRowActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc              = NULL;/* (void*)updateRowActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc          = NULL;/* (void*)insertElementActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc          = NULL;/* (void*)deleteElementActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc       = NULL;/* (void*)isElementPresentActiveAlarmStatusTable; */ 
  g_StatusTable[ACTIVEALARMSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc           = NULL;/* (void*)isRowPresentActiveAlarmStatusTable; */ 
  
  /* AlarmHistory Table */
  
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getData                            = (void*)getDataAlarmHistoryTable; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getTableFunc                       = (void*)getTableAlarmHistoryTable; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getRowFunc                         = (void*)getRowAlarmHistoryTable; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getElementFunc                     = (void*)getElementAlarmHistoryTable; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getStruct2TlvFunc                  = (void*)struct2TlvAlarmHistoryTable; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getTlv2StructFunc                  = NULL; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getReadFromFileFunc                = NULL; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getWriteToFileFunc                 = NULL; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getWriteAllowedFunc                = NULL;/* (void*)writeAllowedAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getPrintFunc                       = NULL;/* (void*)printAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getEmptyTableFunc                  = (void*)emptyTableAlarmHistoryTable; 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getCreateTableFunc                 = NULL;/* (void*)createTableAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getDeleteTableFunc                 = NULL;/* (void*)deleteTableAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getNumOfRows                       = NULL;/* (void*)numOfRowsAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getInsertRowFunc                   = NULL;/* (void*)insertRowAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getDeleteRowFunc                   = NULL;/* (void*)deleteRowAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getUpdateRowFunc                   = NULL;/* (void*)updateRowAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getInsertElementFunc               = NULL;/* (void*)insertElementAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getDeleteElementFunc               = NULL;/* (void*)deleteElementAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getIsElementPresentFunc            = NULL;/* (void*)isElementPresentAlarmHistoryTable; */ 
  g_StatusTable[ALARMHISTORY-MIN_STATUS_TABLEID].getIsRowPresentFunc                = NULL;/* (void*)isRowPresentAlarmHistoryTable; */ 
  
  /* LocationStatus Table */
  
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getData                          = (void*)getDataLocationStatusTable; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getTableFunc                     = (void*)getTableLocationStatusTable; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getRowFunc                       = (void*)getRowLocationStatusTable; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getElementFunc                   = (void*)getElementLocationStatusTable; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvLocationStatusTable; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                = NULL; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc              = NULL; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc               = NULL; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getPrintFunc                     = NULL;/* (void*)printLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                = (void*)emptyTableLocationStatusTable; 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentLocationStatusTable; */ 
  g_StatusTable[LOCATIONSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentLocationStatusTable; */ 
  
  /* GpsStatus Table */
  
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getData                               = (void*)getDataGpsStatusTable; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getTableFunc                          = (void*)getTableGpsStatusTable; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getRowFunc                            = (void*)getRowGpsStatusTable; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getElementFunc                        = (void*)getElementGpsStatusTable; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                     = (void*)struct2TlvGpsStatusTable; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                     = NULL; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc                   = NULL; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc                    = NULL; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc                   = NULL;/* (void*)writeAllowedGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getPrintFunc                          = NULL;/* (void*)printGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                     = (void*)emptyTableGpsStatusTable; 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc                    = NULL;/* (void*)createTableGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc                    = NULL;/* (void*)deleteTableGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getNumOfRows                          = NULL;/* (void*)numOfRowsGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                      = NULL;/* (void*)insertRowGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                      = NULL;/* (void*)deleteRowGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                      = NULL;/* (void*)updateRowGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc                  = NULL;/* (void*)insertElementGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc                  = NULL;/* (void*)deleteElementGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc               = NULL;/* (void*)isElementPresentGpsStatusTable; */ 
  g_StatusTable[GPSSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc                   = NULL;/* (void*)isRowPresentGpsStatusTable; */ 
  
  /* RrcGenCounters Table */
  
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getData                          = (void*)getDataRrcGenCountersTable; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                     = (void*)getTableRrcGenCountersTable; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                       = (void*)getRowRrcGenCountersTable; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                   = (void*)getElementRrcGenCountersTable; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc                = (void*)struct2TlvRrcGenCountersTable; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc                = NULL; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc              = NULL; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc               = NULL; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc              = NULL;/* (void*)writeAllowedRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                     = NULL;/* (void*)printRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc                = (void*)emptyTableRrcGenCountersTable; 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc               = NULL;/* (void*)createTableRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc               = NULL;/* (void*)deleteTableRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                     = NULL;/* (void*)numOfRowsRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                 = NULL;/* (void*)insertRowRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                 = NULL;/* (void*)deleteRowRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                 = NULL;/* (void*)updateRowRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc             = NULL;/* (void*)insertElementRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc             = NULL;/* (void*)deleteElementRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc          = NULL;/* (void*)isElementPresentRrcGenCountersTable; */ 
  g_StatusTable[RRCGENCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc              = NULL;/* (void*)isRowPresentRrcGenCountersTable; */ 
  
  /* RrmCounters Table */
  
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getData                             = (void*)getDataRrmCountersTable; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                        = (void*)getTableRrmCountersTable; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                          = (void*)getRowRrmCountersTable; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                      = (void*)getElementRrmCountersTable; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvRrmCountersTable; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc                   = NULL; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc                 = NULL; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc                  = NULL; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                        = NULL;/* (void*)printRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc                   = (void*)emptyTableRrmCountersTable; 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentRrmCountersTable; */ 
  g_StatusTable[RRMCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentRrmCountersTable; */ 
  
  /* RrcCellCounters Table */
  
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getData                         = (void*)getDataRrcCellCountersTable; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTableRrcCellCountersTable; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowRrcCellCountersTable; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementRrcCellCountersTable; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvRrcCellCountersTable; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc             = NULL; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc              = NULL; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTableRrcCellCountersTable; 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentRrcCellCountersTable; */ 
  g_StatusTable[RRCCELLCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentRrcCellCountersTable; */ 
  
  /* S1apGenCounters Table */
  
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getData                         = (void*)getDataS1apGenCountersTable; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                    = (void*)getTableS1apGenCountersTable; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                      = (void*)getRowS1apGenCountersTable; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                  = (void*)getElementS1apGenCountersTable; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc               = (void*)struct2TlvS1apGenCountersTable; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc               = NULL; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc             = NULL; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc              = NULL; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc             = NULL;/* (void*)writeAllowedS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                    = NULL;/* (void*)printS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc               = (void*)emptyTableS1apGenCountersTable; 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc              = NULL;/* (void*)createTableS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc              = NULL;/* (void*)deleteTableS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                    = NULL;/* (void*)numOfRowsS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                = NULL;/* (void*)insertRowS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                = NULL;/* (void*)deleteRowS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                = NULL;/* (void*)updateRowS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc            = NULL;/* (void*)insertElementS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc            = NULL;/* (void*)deleteElementS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc         = NULL;/* (void*)isElementPresentS1apGenCountersTable; */ 
  g_StatusTable[S1APGENCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc             = NULL;/* (void*)isRowPresentS1apGenCountersTable; */ 
  
  /* S1apPeerNodeCounters Table */
  
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getData                    = (void*)getDataS1apPeerNodeCountersTable; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getTableFunc               = (void*)getTableS1apPeerNodeCountersTable; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getRowFunc                 = (void*)getRowS1apPeerNodeCountersTable; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getElementFunc             = (void*)getElementS1apPeerNodeCountersTable; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc          = (void*)struct2TlvS1apPeerNodeCountersTable; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc          = NULL; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc        = NULL; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc         = NULL; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc        = NULL;/* (void*)writeAllowedS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getPrintFunc               = NULL;/* (void*)printS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc          = (void*)emptyTableS1apPeerNodeCountersTable; 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc         = NULL;/* (void*)createTableS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc         = NULL;/* (void*)deleteTableS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getNumOfRows               = NULL;/* (void*)numOfRowsS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc           = NULL;/* (void*)insertRowS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc           = NULL;/* (void*)deleteRowS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc           = NULL;/* (void*)updateRowS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc       = NULL;/* (void*)insertElementS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc       = NULL;/* (void*)deleteElementS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc    = NULL;/* (void*)isElementPresentS1apPeerNodeCountersTable; */ 
  g_StatusTable[S1APPEERNODECOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc        = NULL;/* (void*)isRowPresentS1apPeerNodeCountersTable; */ 
  
  /* EthernetCounters Table */
  
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getData                        = (void*)getDataEthernetCountersTable; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                   = (void*)getTableEthernetCountersTable; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                     = (void*)getRowEthernetCountersTable; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                 = (void*)getElementEthernetCountersTable; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc              = (void*)struct2TlvEthernetCountersTable; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc              = NULL; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc            = NULL; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc             = NULL; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc            = NULL;/* (void*)writeAllowedEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                   = NULL;/* (void*)printEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc              = (void*)emptyTableEthernetCountersTable; 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc             = NULL;/* (void*)createTableEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc             = NULL;/* (void*)deleteTableEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                   = NULL;/* (void*)numOfRowsEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc               = NULL;/* (void*)insertRowEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc               = NULL;/* (void*)deleteRowEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc               = NULL;/* (void*)updateRowEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc           = NULL;/* (void*)insertElementEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc           = NULL;/* (void*)deleteElementEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc        = NULL;/* (void*)isElementPresentEthernetCountersTable; */ 
  g_StatusTable[ETHERNETCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc            = NULL;/* (void*)isRowPresentEthernetCountersTable; */ 
  
  /* IpCounters Table */
  
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getData                              = (void*)getDataIpCountersTable; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getTableFunc                         = (void*)getTableIpCountersTable; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getRowFunc                           = (void*)getRowIpCountersTable; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getElementFunc                       = (void*)getElementIpCountersTable; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvIpCountersTable; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc                    = NULL; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc                  = NULL; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc                   = NULL; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getPrintFunc                         = NULL;/* (void*)printIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc                    = (void*)emptyTableIpCountersTable; 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentIpCountersTable; */ 
  g_StatusTable[IPCOUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentIpCountersTable; */ 
  
  /* LoginStatus Table */
  
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getData                             = (void*)getDataLoginStatusTable; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getTableFunc                        = (void*)getTableLoginStatusTable; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getRowFunc                          = (void*)getRowLoginStatusTable; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getElementFunc                      = (void*)getElementLoginStatusTable; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvLoginStatusTable; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                   = NULL; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc                 = NULL; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc                  = NULL; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getPrintFunc                        = NULL;/* (void*)printLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                   = (void*)emptyTableLoginStatusTable; 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentLoginStatusTable; */ 
  g_StatusTable[LOGINSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentLoginStatusTable; */ 
  
  /* LedStatus Table */
  
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getData                               = (void*)getDataLedStatusTable; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getTableFunc                          = (void*)getTableLedStatusTable; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getRowFunc                            = (void*)getRowLedStatusTable; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getElementFunc                        = (void*)getElementLedStatusTable; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                     = (void*)struct2TlvLedStatusTable; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                     = NULL; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc                   = NULL; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc                    = NULL; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc                   = NULL;/* (void*)writeAllowedLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getPrintFunc                          = NULL;/* (void*)printLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                     = (void*)emptyTableLedStatusTable; 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc                    = NULL;/* (void*)createTableLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc                    = NULL;/* (void*)deleteTableLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getNumOfRows                          = NULL;/* (void*)numOfRowsLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                      = NULL;/* (void*)insertRowLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                      = NULL;/* (void*)deleteRowLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                      = NULL;/* (void*)updateRowLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc                  = NULL;/* (void*)insertElementLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc                  = NULL;/* (void*)deleteElementLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc               = NULL;/* (void*)isElementPresentLedStatusTable; */ 
  g_StatusTable[LEDSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc                   = NULL;/* (void*)isRowPresentLedStatusTable; */ 
  
  /* CellStatus Table */
  
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getData                              = (void*)getDataCellStatusTable; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getTableFunc                         = (void*)getTableCellStatusTable; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getRowFunc                           = (void*)getRowCellStatusTable; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getElementFunc                       = (void*)getElementCellStatusTable; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvCellStatusTable; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                    = NULL; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc                  = NULL; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc                   = NULL; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getPrintFunc                         = NULL;/* (void*)printCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                    = (void*)emptyTableCellStatusTable; 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentCellStatusTable; */ 
  g_StatusTable[CELLSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentCellStatusTable; */ 
  
  /* L2Counters Table */
  
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getData                              = (void*)getDataL2CountersTable; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getTableFunc                         = (void*)getTableL2CountersTable; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getRowFunc                           = (void*)getRowL2CountersTable; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getElementFunc                       = (void*)getElementL2CountersTable; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvL2CountersTable; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getTlv2StructFunc                    = NULL; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getReadFromFileFunc                  = NULL; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getWriteToFileFunc                   = NULL; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getPrintFunc                         = NULL;/* (void*)printL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getEmptyTableFunc                    = (void*)emptyTableL2CountersTable; 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentL2CountersTable; */ 
  g_StatusTable[L2COUNTERS-MIN_STATUS_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentL2CountersTable; */ 
  
  /* CounterCollectionStatus Table */
  
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getData                 = (void*)getDataCounterCollectionStatusTable; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getTableFunc            = (void*)getTableCounterCollectionStatusTable; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getRowFunc              = (void*)getRowCounterCollectionStatusTable; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getElementFunc          = (void*)getElementCounterCollectionStatusTable; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc       = (void*)struct2TlvCounterCollectionStatusTable; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc       = NULL; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc     = NULL; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc      = NULL; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc     = NULL;/* (void*)writeAllowedCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getPrintFunc            = NULL;/* (void*)printCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc       = (void*)emptyTableCounterCollectionStatusTable; 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getCreateTableFunc      = NULL;/* (void*)createTableCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc      = NULL;/* (void*)deleteTableCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getNumOfRows            = NULL;/* (void*)numOfRowsCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getInsertRowFunc        = NULL;/* (void*)insertRowCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc        = NULL;/* (void*)deleteRowCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc        = NULL;/* (void*)updateRowCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getInsertElementFunc    = NULL;/* (void*)insertElementCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc    = NULL;/* (void*)deleteElementCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc = NULL;/* (void*)isElementPresentCounterCollectionStatusTable; */ 
  g_StatusTable[COUNTERCOLLECTIONSTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc     = NULL;/* (void*)isRowPresentCounterCollectionStatusTable; */ 
  
  /* TraceHistory Table */
  
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getData                            = (void*)getDataTraceHistoryTable; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getTableFunc                       = (void*)getTableTraceHistoryTable; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getRowFunc                         = (void*)getRowTraceHistoryTable; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getElementFunc                     = (void*)getElementTraceHistoryTable; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getStruct2TlvFunc                  = (void*)struct2TlvTraceHistoryTable; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getTlv2StructFunc                  = NULL; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getReadFromFileFunc                = NULL; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getWriteToFileFunc                 = NULL; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getWriteAllowedFunc                = NULL;/* (void*)writeAllowedTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getPrintFunc                       = NULL;/* (void*)printTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getEmptyTableFunc                  = (void*)emptyTableTraceHistoryTable; 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getCreateTableFunc                 = NULL;/* (void*)createTableTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getDeleteTableFunc                 = NULL;/* (void*)deleteTableTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getNumOfRows                       = NULL;/* (void*)numOfRowsTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getInsertRowFunc                   = NULL;/* (void*)insertRowTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getDeleteRowFunc                   = NULL;/* (void*)deleteRowTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getUpdateRowFunc                   = NULL;/* (void*)updateRowTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getInsertElementFunc               = NULL;/* (void*)insertElementTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getDeleteElementFunc               = NULL;/* (void*)deleteElementTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getIsElementPresentFunc            = NULL;/* (void*)isElementPresentTraceHistoryTable; */ 
  g_StatusTable[TRACEHISTORY-MIN_STATUS_TABLEID].getIsRowPresentFunc                = NULL;/* (void*)isRowPresentTraceHistoryTable; */ 
  
  /* TraceStatus Table */
  
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getData                             = (void*)getDataTraceStatusTable; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getTableFunc                        = (void*)getTableTraceStatusTable; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getRowFunc                          = (void*)getRowTraceStatusTable; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getElementFunc                      = (void*)getElementTraceStatusTable; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvTraceStatusTable; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getTlv2StructFunc                   = NULL; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getReadFromFileFunc                 = NULL; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getWriteToFileFunc                  = NULL; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getPrintFunc                        = NULL;/* (void*)printTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getEmptyTableFunc                   = (void*)emptyTableTraceStatusTable; 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentTraceStatusTable; */ 
  g_StatusTable[TRACESTATUS-MIN_STATUS_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentTraceStatusTable; */ 
}
static void fillPreCfgFunctionTable(void)
{
  clearBytes(g_PreCfgTable, (sizeof(GenericTable)* MAX_NUM_PRE_CFG_TABLES));
  
  /* MsgTrace Table */
  
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getData                               = (void*)getDataMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getTableFunc                          = (void*)getTableMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getRowFunc                            = (void*)getRowMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getElementFunc                        = (void*)getElementMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                     = (void*)struct2TlvMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                     = (void*)tlv2StructMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getReadFromFileFunc                   = (void*)readFromFileMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                    = (void*)writeToFileMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc                   = NULL;/* (void*)writeAllowedMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getPrintFunc                          = NULL;/* (void*)printMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                     = (void*)emptyTableMsgTraceTable; 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getCreateTableFunc                    = NULL;/* (void*)createTableMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                    = NULL;/* (void*)deleteTableMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getNumOfRows                          = NULL;/* (void*)numOfRowsMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getInsertRowFunc                      = NULL;/* (void*)insertRowMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                      = NULL;/* (void*)deleteRowMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                      = NULL;/* (void*)updateRowMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getInsertElementFunc                  = NULL;/* (void*)insertElementMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getDeleteElementFunc                  = NULL;/* (void*)deleteElementMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc               = NULL;/* (void*)isElementPresentMsgTraceTable; */ 
  g_PreCfgTable[MSGTRACE-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc                   = NULL;/* (void*)isRowPresentMsgTraceTable; */ 
  
  /* TimersConfig Table */
  
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getData                           = (void*)getDataTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getTableFunc                      = (void*)getTableTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getRowFunc                        = (void*)getRowTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getElementFunc                    = (void*)getElementTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                 = (void*)struct2TlvTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                 = (void*)tlv2StructTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getReadFromFileFunc               = (void*)readFromFileTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                = (void*)writeToFileTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc               = NULL;/* (void*)writeAllowedTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getPrintFunc                      = NULL;/* (void*)printTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                 = (void*)emptyTableTimersConfigTable; 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getCreateTableFunc                = NULL;/* (void*)createTableTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                = NULL;/* (void*)deleteTableTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getNumOfRows                      = NULL;/* (void*)numOfRowsTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getInsertRowFunc                  = NULL;/* (void*)insertRowTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                  = NULL;/* (void*)deleteRowTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                  = NULL;/* (void*)updateRowTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getInsertElementFunc              = NULL;/* (void*)insertElementTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getDeleteElementFunc              = NULL;/* (void*)deleteElementTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc           = NULL;/* (void*)isElementPresentTimersConfigTable; */ 
  g_PreCfgTable[TIMERSCONFIG-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc               = NULL;/* (void*)isRowPresentTimersConfigTable; */ 
  
  /* ProcessStart Table */
  
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getData                           = (void*)getDataProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getTableFunc                      = (void*)getTableProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getRowFunc                        = (void*)getRowProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getElementFunc                    = (void*)getElementProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                 = (void*)struct2TlvProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                 = (void*)tlv2StructProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getReadFromFileFunc               = (void*)readFromFileProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                = NULL; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc               = NULL;/* (void*)writeAllowedProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getPrintFunc                      = NULL;/* (void*)printProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                 = (void*)emptyTableProcessStartTable; 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getCreateTableFunc                = NULL;/* (void*)createTableProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                = NULL;/* (void*)deleteTableProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getNumOfRows                      = NULL;/* (void*)numOfRowsProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getInsertRowFunc                  = NULL;/* (void*)insertRowProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                  = NULL;/* (void*)deleteRowProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                  = NULL;/* (void*)updateRowProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getInsertElementFunc              = NULL;/* (void*)insertElementProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getDeleteElementFunc              = NULL;/* (void*)deleteElementProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc           = NULL;/* (void*)isElementPresentProcessStartTable; */ 
  g_PreCfgTable[PROCESSSTART-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc               = NULL;/* (void*)isRowPresentProcessStartTable; */ 
  
  /* ModuleStop Table */
  
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getData                             = (void*)getDataModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getTableFunc                        = (void*)getTableModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getRowFunc                          = (void*)getRowModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getElementFunc                      = (void*)getElementModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                   = (void*)tlv2StructModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getReadFromFileFunc                 = (void*)readFromFileModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                  = NULL; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getPrintFunc                        = NULL;/* (void*)printModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                   = (void*)emptyTableModuleStopTable; 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentModuleStopTable; */ 
  g_PreCfgTable[MODULESTOP-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentModuleStopTable; */ 
  
  /* EventThresholds Table */
  
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getData                        = (void*)getDataEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getTableFunc                   = (void*)getTableEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getRowFunc                     = (void*)getRowEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getElementFunc                 = (void*)getElementEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc              = (void*)struct2TlvEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getTlv2StructFunc              = (void*)tlv2StructEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getReadFromFileFunc            = (void*)readFromFileEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getWriteToFileFunc             = (void*)writeToFileEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc            = NULL;/* (void*)writeAllowedEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getPrintFunc                   = NULL;/* (void*)printEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getEmptyTableFunc              = (void*)emptyTableEventThresholdsTable; 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getCreateTableFunc             = NULL;/* (void*)createTableEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getDeleteTableFunc             = NULL;/* (void*)deleteTableEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getNumOfRows                   = NULL;/* (void*)numOfRowsEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getInsertRowFunc               = NULL;/* (void*)insertRowEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getDeleteRowFunc               = NULL;/* (void*)deleteRowEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getUpdateRowFunc               = NULL;/* (void*)updateRowEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getInsertElementFunc           = NULL;/* (void*)insertElementEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getDeleteElementFunc           = NULL;/* (void*)deleteElementEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc        = NULL;/* (void*)isElementPresentEventThresholdsTable; */ 
  g_PreCfgTable[EVENTTHRESHOLDS-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc            = NULL;/* (void*)isRowPresentEventThresholdsTable; */ 
  
  /* EnbAddresses Table */
  
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getData                           = (void*)getDataEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getTableFunc                      = (void*)getTableEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getRowFunc                        = (void*)getRowEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getElementFunc                    = (void*)getElementEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                 = (void*)struct2TlvEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                 = (void*)tlv2StructEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getReadFromFileFunc               = (void*)readFromFileEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                = (void*)writeToFileEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc               = NULL;/* (void*)writeAllowedEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getPrintFunc                      = NULL;/* (void*)printEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                 = (void*)emptyTableEnbAddressesTable; 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getCreateTableFunc                = NULL;/* (void*)createTableEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                = NULL;/* (void*)deleteTableEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getNumOfRows                      = NULL;/* (void*)numOfRowsEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getInsertRowFunc                  = NULL;/* (void*)insertRowEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                  = NULL;/* (void*)deleteRowEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                  = NULL;/* (void*)updateRowEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getInsertElementFunc              = NULL;/* (void*)insertElementEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getDeleteElementFunc              = NULL;/* (void*)deleteElementEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc           = NULL;/* (void*)isElementPresentEnbAddressesTable; */ 
  g_PreCfgTable[ENBADDRESSES-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc               = NULL;/* (void*)isRowPresentEnbAddressesTable; */ 
  
  /* UdhConfig Table */
  
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getData                              = (void*)getDataUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getTableFunc                         = (void*)getTableUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getRowFunc                           = (void*)getRowUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getElementFunc                       = (void*)getElementUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getReadFromFileFunc                  = (void*)readFromFileUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                   = (void*)writeToFileUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getPrintFunc                         = NULL;/* (void*)printUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                    = (void*)emptyTableUdhConfigTable; 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentUdhConfigTable; */ 
  g_PreCfgTable[UDHCONFIG-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentUdhConfigTable; */ 
  
  /* UehConfig Table */
  
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getData                              = (void*)getDataUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getTableFunc                         = (void*)getTableUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getRowFunc                           = (void*)getRowUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getElementFunc                       = (void*)getElementUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                    = (void*)struct2TlvUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                    = (void*)tlv2StructUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getReadFromFileFunc                  = (void*)readFromFileUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                   = (void*)writeToFileUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc                  = NULL;/* (void*)writeAllowedUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getPrintFunc                         = NULL;/* (void*)printUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                    = (void*)emptyTableUehConfigTable; 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getCreateTableFunc                   = NULL;/* (void*)createTableUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                   = NULL;/* (void*)deleteTableUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getNumOfRows                         = NULL;/* (void*)numOfRowsUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getInsertRowFunc                     = NULL;/* (void*)insertRowUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                     = NULL;/* (void*)deleteRowUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                     = NULL;/* (void*)updateRowUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getInsertElementFunc                 = NULL;/* (void*)insertElementUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getDeleteElementFunc                 = NULL;/* (void*)deleteElementUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc              = NULL;/* (void*)isElementPresentUehConfigTable; */ 
  g_PreCfgTable[UEHCONFIG-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc                  = NULL;/* (void*)isRowPresentUehConfigTable; */ 
  
  /* EgtpConfig Table */
  
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getData                             = (void*)getDataEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getTableFunc                        = (void*)getTableEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getRowFunc                          = (void*)getRowEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getElementFunc                      = (void*)getElementEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                   = (void*)tlv2StructEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getReadFromFileFunc                 = (void*)readFromFileEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                  = (void*)writeToFileEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getPrintFunc                        = NULL;/* (void*)printEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                   = (void*)emptyTableEgtpConfigTable; 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentEgtpConfigTable; */ 
  g_PreCfgTable[EGTPCONFIG-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentEgtpConfigTable; */ 
  
  /* SctpConfig Table */
  
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getData                             = (void*)getDataSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getTableFunc                        = (void*)getTableSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getRowFunc                          = (void*)getRowSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getElementFunc                      = (void*)getElementSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                   = (void*)tlv2StructSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getReadFromFileFunc                 = (void*)readFromFileSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                  = (void*)writeToFileSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getPrintFunc                        = NULL;/* (void*)printSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                   = (void*)emptyTableSctpConfigTable; 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentSctpConfigTable; */ 
  g_PreCfgTable[SCTPCONFIG-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentSctpConfigTable; */ 
  
  /* S1apConfig Table */
  
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getData                             = (void*)getDataS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getTableFunc                        = (void*)getTableS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getRowFunc                          = (void*)getRowS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getElementFunc                      = (void*)getElementS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc                   = (void*)struct2TlvS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getTlv2StructFunc                   = (void*)tlv2StructS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getReadFromFileFunc                 = (void*)readFromFileS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getWriteToFileFunc                  = (void*)writeToFileS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc                 = NULL;/* (void*)writeAllowedS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getPrintFunc                        = NULL;/* (void*)printS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getEmptyTableFunc                   = (void*)emptyTableS1apConfigTable; 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getCreateTableFunc                  = NULL;/* (void*)createTableS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getDeleteTableFunc                  = NULL;/* (void*)deleteTableS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getNumOfRows                        = NULL;/* (void*)numOfRowsS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getInsertRowFunc                    = NULL;/* (void*)insertRowS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getDeleteRowFunc                    = NULL;/* (void*)deleteRowS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getUpdateRowFunc                    = NULL;/* (void*)updateRowS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getInsertElementFunc                = NULL;/* (void*)insertElementS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getDeleteElementFunc                = NULL;/* (void*)deleteElementS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc             = NULL;/* (void*)isElementPresentS1apConfigTable; */ 
  g_PreCfgTable[S1APCONFIG-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc                 = NULL;/* (void*)isRowPresentS1apConfigTable; */ 
  
  /* DeviceMonitorConfig Table */
  
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getData                    = (void*)getDataDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getTableFunc               = (void*)getTableDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getRowFunc                 = (void*)getRowDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getElementFunc             = (void*)getElementDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getStruct2TlvFunc          = (void*)struct2TlvDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getTlv2StructFunc          = (void*)tlv2StructDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getReadFromFileFunc        = (void*)readFromFileDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getWriteToFileFunc         = (void*)writeToFileDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getWriteAllowedFunc        = NULL;/* (void*)writeAllowedDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getPrintFunc               = NULL;/* (void*)printDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getEmptyTableFunc          = (void*)emptyTableDeviceMonitorConfigTable; 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getCreateTableFunc         = NULL;/* (void*)createTableDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getDeleteTableFunc         = NULL;/* (void*)deleteTableDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getNumOfRows               = NULL;/* (void*)numOfRowsDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getInsertRowFunc           = NULL;/* (void*)insertRowDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getDeleteRowFunc           = NULL;/* (void*)deleteRowDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getUpdateRowFunc           = NULL;/* (void*)updateRowDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getInsertElementFunc       = NULL;/* (void*)insertElementDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getDeleteElementFunc       = NULL;/* (void*)deleteElementDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getIsElementPresentFunc    = NULL;/* (void*)isElementPresentDeviceMonitorConfigTable; */ 
  g_PreCfgTable[DEVICEMONITORCONFIG-MIN_PRE_CFG_TABLEID].getIsRowPresentFunc        = NULL;/* (void*)isRowPresentDeviceMonitorConfigTable; */ 
}
void fillGenericFunctionTable(void)
{
  fillOprCmdFunctionTable();
  fillOprCfgFunctionTable();
  fillStatusFunctionTable();
  fillPreCfgFunctionTable();
}
