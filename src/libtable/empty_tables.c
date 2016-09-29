/**
 * @file    empty_tables.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains SET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-11-2010
 * Description: Draft version 
 * Generated on : 11/29/2012 12:22:23 PM
 */

 
#include "empty_tables.h"

/* OprCommands */
/**
 * @section This function will empty enbAdminstructure.
 * */
void emptyTableEnbAdminTable()
{
  EnbAdmin  *config = (EnbAdmin*)getFirstRowOfTable(ENBADMIN);
  clearBytes( config, sizeof(EnbAdmin) );
}

/**
 * @section This function will empty fileUploadstructure.
 * */
void emptyTableFileUploadTable()
{
  FileUpload  *config = (FileUpload*)getFirstRowOfTable(FILEUPLOAD);
  clearBytes( config, sizeof(FileUpload) );
}

/**
 * @section This function will empty fileDownloadstructure.
 * */
void emptyTableFileDownloadTable()
{
  FileDownload  *config = (FileDownload*)getFirstRowOfTable(FILEDOWNLOAD);
  clearBytes( config, sizeof(FileDownload) );
}

/**
 * @section This function will empty swActivationstructure.
 * */
void emptyTableSwActivationTable()
{
  SwActivation  *config = (SwActivation*)getFirstRowOfTable(SWACTIVATION);
  clearBytes( config, sizeof(SwActivation) );
}

/**
 * @section This function will empty swInstallstructure.
 * */
void emptyTableSwInstallTable()
{
  SwInstall  *config = (SwInstall*)getFirstRowOfTable(SWINSTALL);
  clearBytes( config, sizeof(SwInstall) );
}

/**
 * @section This function will empty traceControlstructure.
 * */
void emptyTableTraceControlTable()
{
  TraceControl  *config = (TraceControl*)getFirstRowOfTable(TRACECONTROL);
  clearBytes( config, sizeof(TraceControl) );
}

/**
 * @section This function will empty cellAdminstructure.
 * */
void emptyTableCellAdminTable()
{
  CellAdmin  *config = (CellAdmin*)getFirstRowOfTable(CELLADMIN);
  clearBytes( config, sizeof(CellAdmin) );
}

/**
 * @section This function will empty counterControlstructure.
 * */
void emptyTableCounterControlTable()
{
  CounterControl  *config = (CounterControl*)getFirstRowOfTable(COUNTERCONTROL);
  clearBytes( config, sizeof(CounterControl) );
}

/* Status */
/**
 * @section This function will empty fileUploadStatusstructure.
 * */
void emptyTableFileUploadStatusTable()
{
  FileUploadStatus  *config = (FileUploadStatus*)getFirstRowOfTable(FILEUPLOADSTATUS);
  clearBytes( config, sizeof(FileUploadStatus) );
}

/**
 * @section This function will empty fileDownloadStatusstructure.
 * */
void emptyTableFileDownloadStatusTable()
{
  FileDownloadStatus  *config = (FileDownloadStatus*)getFirstRowOfTable(FILEDOWNLOADSTATUS);
  clearBytes( config, sizeof(FileDownloadStatus) );
}

/**
 * @section This function will empty swActivationStatusstructure.
 * */
void emptyTableSwActivationStatusTable()
{
  SwActivationStatus  *config = (SwActivationStatus*)getFirstRowOfTable(SWACTIVATIONSTATUS);
  clearBytes( config, sizeof(SwActivationStatus) );
}

/**
 * @section This function will empty swPackageStatusstructure.
 * */
void emptyTableSwPackageStatusTable()
{
  SwPackageStatus  *config = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);
  clearBytes( config, sizeof(SwPackageStatus) );
}

/**
 * @section This function will empty componentStatusstructure.
 * */
void emptyTableComponentStatusTable()
{
  ComponentStatus  *config = (ComponentStatus*)getFirstRowOfTable(COMPONENTSTATUS);
  clearBytes( config, sizeof(ComponentStatus) );
}

/**
 * @section This function will empty enbStatusstructure.
 * */
void emptyTableEnbStatusTable()
{
  EnbStatus  *config = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  clearBytes( config, sizeof(EnbStatus) );
}

/**
 * @section This function will empty moduleStatusstructure.
 * */
void emptyTableModuleStatusTable()
{
  ModuleStatus  *config = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
  clearBytes( config, sizeof(ModuleStatus) );
}

/**
 * @section This function will empty radioHeadStatusstructure.
 * */
void emptyTableRadioHeadStatusTable()
{
  RadioHeadStatus  *config = (RadioHeadStatus*)getFirstRowOfTable(RADIOHEADSTATUS);
  clearBytes( config, sizeof(RadioHeadStatus) );
}

/**
 * @section This function will empty macGenCountersstructure.
 * */
void emptyTableMacGenCountersTable()
{
  MacGenCounters  *config = (MacGenCounters*)getFirstRowOfTable(MACGENCOUNTERS);
  clearBytes( config, sizeof(MacGenCounters) );
}

/**
 * @section This function will empty macUplaneCountersstructure.
 * */
void emptyTableMacUplaneCountersTable()
{
  MacUplaneCounters  *config = (MacUplaneCounters*)getFirstRowOfTable(MACUPLANECOUNTERS);
  clearBytes( config, sizeof(MacUplaneCounters) );
}

/**
 * @section This function will empty rlcGenCountersstructure.
 * */
void emptyTableRlcGenCountersTable()
{
  RlcGenCounters  *config = (RlcGenCounters*)getFirstRowOfTable(RLCGENCOUNTERS);
  clearBytes( config, sizeof(RlcGenCounters) );
}

/**
 * @section This function will empty pdcpGenCountersstructure.
 * */
void emptyTablePdcpGenCountersTable()
{
  PdcpGenCounters  *config = (PdcpGenCounters*)getFirstRowOfTable(PDCPGENCOUNTERS);
  clearBytes( config, sizeof(PdcpGenCounters) );
}

/**
 * @section This function will empty pdcpUplaneCountersstructure.
 * */
void emptyTablePdcpUplaneCountersTable()
{
  PdcpUplaneCounters  *config = (PdcpUplaneCounters*)getFirstRowOfTable(PDCPUPLANECOUNTERS);
  clearBytes( config, sizeof(PdcpUplaneCounters) );
}

/**
 * @section This function will empty gtpGenCountersstructure.
 * */
void emptyTableGtpGenCountersTable()
{
  GtpGenCounters  *config = (GtpGenCounters*)getFirstRowOfTable(GTPGENCOUNTERS);
  clearBytes( config, sizeof(GtpGenCounters) );
}

/**
 * @section This function will empty sctpGenCountersstructure.
 * */
void emptyTableSctpGenCountersTable()
{
  SctpGenCounters  *config = (SctpGenCounters*)getFirstRowOfTable(SCTPGENCOUNTERS);
  clearBytes( config, sizeof(SctpGenCounters) );
}

/**
 * @section This function will empty fileEventHistorystructure.
 * */
void emptyTableFileEventHistoryTable()
{
  FileEventHistory  *config = (FileEventHistory*)getFirstRowOfTable(FILEEVENTHISTORY);
  clearBytes( config, sizeof(FileEventHistory) );
}

/**
 * @section This function will empty swInstallHistorystructure.
 * */
void emptyTableSwInstallHistoryTable()
{
  SwInstallHistory  *config = (SwInstallHistory*)getFirstRowOfTable(SWINSTALLHISTORY);
  clearBytes( config, sizeof(SwInstallHistory) );
}

/**
 * @section This function will empty activeUeStatusstructure.
 * */
void emptyTableActiveUeStatusTable()
{
  ActiveUeStatus  *config = (ActiveUeStatus*)getFirstRowOfTable(ACTIVEUESTATUS);
  clearBytes( config, sizeof(ActiveUeStatus) );
}

/**
 * @section This function will empty activeRabStatusstructure.
 * */
void emptyTableActiveRabStatusTable()
{
  ActiveRabStatus  *config = (ActiveRabStatus*)getFirstRowOfTable(ACTIVERABSTATUS);
  clearBytes( config, sizeof(ActiveRabStatus) );
}

/**
 * @section This function will empty ueEventHistorystructure.
 * */
void emptyTableUeEventHistoryTable()
{
  UeEventHistory  *config = (UeEventHistory*)getFirstRowOfTable(UEEVENTHISTORY);
  clearBytes( config, sizeof(UeEventHistory) );
}

/**
 * @section This function will empty rabHistorystructure.
 * */
void emptyTableRabHistoryTable()
{
  RabHistory  *config = (RabHistory*)getFirstRowOfTable(RABHISTORY);
  clearBytes( config, sizeof(RabHistory) );
}

/**
 * @section This function will empty pmLogFileStatusstructure.
 * */
void emptyTablePmLogFileStatusTable()
{
  PmLogFileStatus  *config = (PmLogFileStatus*)getFirstRowOfTable(PMLOGFILESTATUS);
  clearBytes( config, sizeof(PmLogFileStatus) );
}

/**
 * @section This function will empty moduleLogFileStatusstructure.
 * */
void emptyTableModuleLogFileStatusTable()
{
  ModuleLogFileStatus  *config = (ModuleLogFileStatus*)getFirstRowOfTable(MODULELOGFILESTATUS);
  clearBytes( config, sizeof(ModuleLogFileStatus) );
}

/**
 * @section This function will empty activeAlarmStatusstructure.
 * */
void emptyTableActiveAlarmStatusTable()
{
  ActiveAlarmStatus  *config = (ActiveAlarmStatus*)getFirstRowOfTable(ACTIVEALARMSTATUS);
  clearBytes( config, sizeof(ActiveAlarmStatus) );
}

/**
 * @section This function will empty alarmHistorystructure.
 * */
void emptyTableAlarmHistoryTable()
{
  AlarmHistory  *config = (AlarmHistory*)getFirstRowOfTable(ALARMHISTORY);
  clearBytes( config, sizeof(AlarmHistory) );
}

/**
 * @section This function will empty locationStatusstructure.
 * */
void emptyTableLocationStatusTable()
{
  LocationStatus  *config = (LocationStatus*)getFirstRowOfTable(LOCATIONSTATUS);
  clearBytes( config, sizeof(LocationStatus) );
}

/**
 * @section This function will empty gpsStatusstructure.
 * */
void emptyTableGpsStatusTable()
{
  GpsStatus  *config = (GpsStatus*)getFirstRowOfTable(GPSSTATUS);
  clearBytes( config, sizeof(GpsStatus) );
}

/**
 * @section This function will empty rrcGenCountersstructure.
 * */
void emptyTableRrcGenCountersTable()
{
  RrcGenCounters  *config = (RrcGenCounters*)getFirstRowOfTable(RRCGENCOUNTERS);
  clearBytes( config, sizeof(RrcGenCounters) );
}

/**
 * @section This function will empty rrmCountersstructure.
 * */
void emptyTableRrmCountersTable()
{
  RrmCounters  *config = (RrmCounters*)getFirstRowOfTable(RRMCOUNTERS);
  clearBytes( config, sizeof(RrmCounters) );
}

/**
 * @section This function will empty rrcCellCountersstructure.
 * */
void emptyTableRrcCellCountersTable()
{
  RrcCellCounters  *config = (RrcCellCounters*)getFirstRowOfTable(RRCCELLCOUNTERS);
  clearBytes( config, sizeof(RrcCellCounters) );
}

/**
 * @section This function will empty s1apGenCountersstructure.
 * */
void emptyTableS1apGenCountersTable()
{
  S1apGenCounters  *config = (S1apGenCounters*)getFirstRowOfTable(S1APGENCOUNTERS);
  clearBytes( config, sizeof(S1apGenCounters) );
}

/**
 * @section This function will empty s1apPeerNodeCountersstructure.
 * */
void emptyTableS1apPeerNodeCountersTable()
{
  S1apPeerNodeCounters  *config = (S1apPeerNodeCounters*)getFirstRowOfTable(S1APPEERNODECOUNTERS);
  clearBytes( config, sizeof(S1apPeerNodeCounters) );
}

/**
 * @section This function will empty ethernetCountersstructure.
 * */
void emptyTableEthernetCountersTable()
{
  EthernetCounters  *config = (EthernetCounters*)getFirstRowOfTable(ETHERNETCOUNTERS);
  clearBytes( config, sizeof(EthernetCounters) );
}

/**
 * @section This function will empty ipCountersstructure.
 * */
void emptyTableIpCountersTable()
{
  IpCounters  *config = (IpCounters*)getFirstRowOfTable(IPCOUNTERS);
  clearBytes( config, sizeof(IpCounters) );
}

/**
 * @section This function will empty loginStatusstructure.
 * */
void emptyTableLoginStatusTable()
{
  LoginStatus  *config = (LoginStatus*)getFirstRowOfTable(LOGINSTATUS);
  clearBytes( config, sizeof(LoginStatus) );
}

/**
 * @section This function will empty ledStatusstructure.
 * */
void emptyTableLedStatusTable()
{
  LedStatus  *config = (LedStatus*)getFirstRowOfTable(LEDSTATUS);
  clearBytes( config, sizeof(LedStatus) );
}

/**
 * @section This function will empty cellStatusstructure.
 * */
void emptyTableCellStatusTable()
{
  CellStatus  *config = (CellStatus*)getFirstRowOfTable(CELLSTATUS);
  clearBytes( config, sizeof(CellStatus) );
}

/**
 * @section This function will empty l2Countersstructure.
 * */
void emptyTableL2CountersTable()
{
  L2Counters  *config = (L2Counters*)getFirstRowOfTable(L2COUNTERS);
  clearBytes( config, sizeof(L2Counters) );
}

/**
 * @section This function will empty counterCollectionStatusstructure.
 * */
void emptyTableCounterCollectionStatusTable()
{
  CounterCollectionStatus  *config = (CounterCollectionStatus*)getFirstRowOfTable(COUNTERCOLLECTIONSTATUS);
  clearBytes( config, sizeof(CounterCollectionStatus) );
}

/**
 * @section This function will empty traceHistorystructure.
 * */
void emptyTableTraceHistoryTable()
{
  TraceHistory  *config = (TraceHistory*)getFirstRowOfTable(TRACEHISTORY);
  clearBytes( config, sizeof(TraceHistory) );
}

/**
 * @section This function will empty traceStatusstructure.
 * */
void emptyTableTraceStatusTable()
{
  TraceStatus  *config = (TraceStatus*)getFirstRowOfTable(TRACESTATUS);
  clearBytes( config, sizeof(TraceStatus) );
}

/* OprConfig */
/**
 * @section This function will empty enbRegistrationConfigstructure.
 * */
void emptyTableEnbRegistrationConfigTable()
{
  EnbRegistrationConfig  *config = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);
  clearBytes( config, sizeof(EnbRegistrationConfig) );
}

/**
 * @section This function will empty enodebConfigstructure.
 * */
void emptyTableEnodebConfigTable()
{
  EnodebConfig  *config = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  clearBytes( config, sizeof(EnodebConfig) );
}

/**
 * @section This function will empty mmeConfigstructure.
 * */
void emptyTableMmeConfigTable()
{
  MmeConfig  *config = (MmeConfig*)getFirstRowOfTable(MMECONFIG);
  clearBytes( config, sizeof(MmeConfig) );
}

/**
 * @section This function will empty cellConfigstructure.
 * */
void emptyTableCellConfigTable()
{
  CellConfig  *config = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  clearBytes( config, sizeof(CellConfig) );
}

/**
 * @section This function will empty schedulerConfigstructure.
 * */
void emptyTableSchedulerConfigTable()
{
  SchedulerConfig  *config = (SchedulerConfig*)getFirstRowOfTable(SCHEDULERCONFIG);
  clearBytes( config, sizeof(SchedulerConfig) );
}

/**
 * @section This function will empty phyConfigstructure.
 * */
void emptyTablePhyConfigTable()
{
  PhyConfig  *config = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);
  clearBytes( config, sizeof(PhyConfig) );
}

/**
 * @section This function will empty sysInfo1Configstructure.
 * */
void emptyTableSysInfo1ConfigTable()
{
  SysInfo1Config  *config = (SysInfo1Config*)getFirstRowOfTable(SYSINFO1CONFIG);
  clearBytes( config, sizeof(SysInfo1Config) );
}

/**
 * @section This function will empty sysInfo2Configstructure.
 * */
void emptyTableSysInfo2ConfigTable()
{
  SysInfo2Config  *config = (SysInfo2Config*)getFirstRowOfTable(SYSINFO2CONFIG);
  clearBytes( config, sizeof(SysInfo2Config) );
}

/**
 * @section This function will empty logConfigstructure.
 * */
void emptyTableLogConfigTable()
{
  LogConfig  *config = (LogConfig*)getFirstRowOfTable(LOGCONFIG);
  clearBytes( config, sizeof(LogConfig) );
}

/**
 * @section This function will empty radioHeadConfigstructure.
 * */
void emptyTableRadioHeadConfigTable()
{
  RadioHeadConfig  *config = (RadioHeadConfig*)getFirstRowOfTable(RADIOHEADCONFIG);
  clearBytes( config, sizeof(RadioHeadConfig) );
}

/**
 * @section This function will empty qosConfigstructure.
 * */
void emptyTableQosConfigTable()
{
  QosConfig  *config = (QosConfig*)getFirstRowOfTable(QOSCONFIG);
  clearBytes( config, sizeof(QosConfig) );
}

/**
 * @section This function will empty pmCounterLogConfigstructure.
 * */
void emptyTablePmCounterLogConfigTable()
{
  PmCounterLogConfig  *config = (PmCounterLogConfig*)getFirstRowOfTable(PMCOUNTERLOGCONFIG);
  clearBytes( config, sizeof(PmCounterLogConfig) );
}

/**
 * @section This function will empty eutraHoProfileConfigstructure.
 * */
void emptyTableEutraHoProfileConfigTable()
{
  EutraHoProfileConfig  *config = (EutraHoProfileConfig*)getFirstRowOfTable(EUTRAHOPROFILECONFIG);
  clearBytes( config, sizeof(EutraHoProfileConfig) );
}

/**
 * @section This function will empty eutraNeighCellConfigstructure.
 * */
void emptyTableEutraNeighCellConfigTable()
{
  EutraNeighCellConfig  *config = (EutraNeighCellConfig*)getFirstRowOfTable(EUTRANEIGHCELLCONFIG);
  clearBytes( config, sizeof(EutraNeighCellConfig) );
}

/**
 * @section This function will empty sysInfo3Configstructure.
 * */
void emptyTableSysInfo3ConfigTable()
{
  SysInfo3Config  *config = (SysInfo3Config*)getFirstRowOfTable(SYSINFO3CONFIG);
  clearBytes( config, sizeof(SysInfo3Config) );
}

/**
 * @section This function will empty sonConfigstructure.
 * */
void emptyTableSonConfigTable()
{
  SonConfig  *config = (SonConfig*)getFirstRowOfTable(SONCONFIG);
  clearBytes( config, sizeof(SonConfig) );
}

/**
 * @section This function will empty srsConfigDedicatedstructure.
 * */
void emptyTableSrsConfigDedicatedTable()
{
  SrsConfigDedicated  *config = (SrsConfigDedicated*)getFirstRowOfTable(SRSCONFIGDEDICATED);
  clearBytes( config, sizeof(SrsConfigDedicated) );
}

/**
 * @section This function will empty securityConfigstructure.
 * */
void emptyTableSecurityConfigTable()
{
  SecurityConfig  *config = (SecurityConfig*)getFirstRowOfTable(SECURITYCONFIG);
  clearBytes( config, sizeof(SecurityConfig) );
}

/**
 * @section This function will empty ueEventHistoryLogConfigstructure.
 * */
void emptyTableUeEventHistoryLogConfigTable()
{
  UeEventHistoryLogConfig  *config = (UeEventHistoryLogConfig*)getFirstRowOfTable(UEEVENTHISTORYLOGCONFIG);
  clearBytes( config, sizeof(UeEventHistoryLogConfig) );
}

/**
 * @section This function will empty counterConfigstructure.
 * */
void emptyTableCounterConfigTable()
{
  CounterConfig  *config = (CounterConfig*)getFirstRowOfTable(COUNTERCONFIG);
  clearBytes( config, sizeof(CounterConfig) );
}

/**
 * @section This function will empty traceConfigstructure.
 * */
void emptyTableTraceConfigTable()
{
  TraceConfig  *config = (TraceConfig*)getFirstRowOfTable(TRACECONFIG);
  clearBytes( config, sizeof(TraceConfig) );
}

/* PreConfig */
/**
 * @section This function will empty msgTracestructure.
 * */
void emptyTableMsgTraceTable()
{
  MsgTrace  *config = (MsgTrace*)getFirstRowOfTable(MSGTRACE);
  clearBytes( config, sizeof(MsgTrace) );
}

/**
 * @section This function will empty timersConfigstructure.
 * */
void emptyTableTimersConfigTable()
{
  TimersConfig  *config = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
  clearBytes( config, sizeof(TimersConfig) );
}

/**
 * @section This function will empty processStartstructure.
 * */
void emptyTableProcessStartTable()
{
  ProcessStart  *config = (ProcessStart*)getFirstRowOfTable(PROCESSSTART);
  clearBytes( config, sizeof(ProcessStart) );
}

/**
 * @section This function will empty moduleStopstructure.
 * */
void emptyTableModuleStopTable()
{
  ModuleStop  *config = (ModuleStop*)getFirstRowOfTable(MODULESTOP);
  clearBytes( config, sizeof(ModuleStop) );
}

/**
 * @section This function will empty eventThresholdsstructure.
 * */
void emptyTableEventThresholdsTable()
{
  EventThresholds  *config = (EventThresholds*)getFirstRowOfTable(EVENTTHRESHOLDS);
  clearBytes( config, sizeof(EventThresholds) );
}

/**
 * @section This function will empty enbAddressesstructure.
 * */
void emptyTableEnbAddressesTable()
{
  EnbAddresses  *config = (EnbAddresses*)getFirstRowOfTable(ENBADDRESSES);
  clearBytes( config, sizeof(EnbAddresses) );
}

/**
 * @section This function will empty udhConfigstructure.
 * */
void emptyTableUdhConfigTable()
{
  UdhConfig  *config = (UdhConfig*)getFirstRowOfTable(UDHCONFIG);
  clearBytes( config, sizeof(UdhConfig) );
}

/**
 * @section This function will empty uehConfigstructure.
 * */
void emptyTableUehConfigTable()
{
  UehConfig  *config = (UehConfig*)getFirstRowOfTable(UEHCONFIG);
  clearBytes( config, sizeof(UehConfig) );
}

/**
 * @section This function will empty egtpConfigstructure.
 * */
void emptyTableEgtpConfigTable()
{
  EgtpConfig  *config = (EgtpConfig*)getFirstRowOfTable(EGTPCONFIG);
  clearBytes( config, sizeof(EgtpConfig) );
}

/**
 * @section This function will empty sctpConfigstructure.
 * */
void emptyTableSctpConfigTable()
{
  SctpConfig  *config = (SctpConfig*)getFirstRowOfTable(SCTPCONFIG);
  clearBytes( config, sizeof(SctpConfig) );
}

/**
 * @section This function will empty s1apConfigstructure.
 * */
void emptyTableS1apConfigTable()
{
  S1apConfig  *config = (S1apConfig*)getFirstRowOfTable(S1APCONFIG);
  clearBytes( config, sizeof(S1apConfig) );
}

/**
 * @section This function will empty deviceMonitorConfigstructure.
 * */
void emptyTableDeviceMonitorConfigTable()
{
  DeviceMonitorConfig  *config = (DeviceMonitorConfig*)getFirstRowOfTable(DEVICEMONITORCONFIG);
  clearBytes( config, sizeof(DeviceMonitorConfig) );
}

void emptyAllTables(void)
{
  emptyOprCommandTables();
  emptyStatusTables();
  emptyOprConfigTables();
  emptyPreConfigTables();
}
void emptyOprCommandTables(void)
{
  GenericTable  *tableFunc;
/* OprCommands */
  tableFunc = getTableFunctions(ENBADMIN);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ENBADMIN");
  }
  
  tableFunc = getTableFunctions(FILEUPLOAD);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("FILEUPLOAD");
  }
  
  tableFunc = getTableFunctions(FILEDOWNLOAD);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("FILEDOWNLOAD");
  }
  
  tableFunc = getTableFunctions(SWACTIVATION);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SWACTIVATION");
  }
  
  tableFunc = getTableFunctions(SWINSTALL);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SWINSTALL");
  }
  
  tableFunc = getTableFunctions(TRACECONTROL);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("TRACECONTROL");
  }
  
  tableFunc = getTableFunctions(CELLADMIN);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("CELLADMIN");
  }
  
  tableFunc = getTableFunctions(COUNTERCONTROL);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("COUNTERCONTROL");
  }
  
}
void emptyStatusTables(void)
{
  GenericTable  *tableFunc;
/* Status */
  tableFunc = getTableFunctions(FILEUPLOADSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("FILEUPLOADSTATUS");
  }
  
  tableFunc = getTableFunctions(FILEDOWNLOADSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("FILEDOWNLOADSTATUS");
  }
  
  tableFunc = getTableFunctions(SWACTIVATIONSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SWACTIVATIONSTATUS");
  }
  
  tableFunc = getTableFunctions(SWPACKAGESTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SWPACKAGESTATUS");
  }
  
  tableFunc = getTableFunctions(COMPONENTSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("COMPONENTSTATUS");
  }
  
  tableFunc = getTableFunctions(ENBSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ENBSTATUS");
  }
  
  tableFunc = getTableFunctions(MODULESTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MODULESTATUS");
  }
  
  tableFunc = getTableFunctions(RADIOHEADSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RADIOHEADSTATUS");
  }
  
  tableFunc = getTableFunctions(MACGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MACGENCOUNTERS");
  }
  
  tableFunc = getTableFunctions(MACUPLANECOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MACUPLANECOUNTERS");
  }
  
  tableFunc = getTableFunctions(RLCGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RLCGENCOUNTERS");
  }
  
  tableFunc = getTableFunctions(PDCPGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PDCPGENCOUNTERS");
  }
  
  tableFunc = getTableFunctions(PDCPUPLANECOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PDCPUPLANECOUNTERS");
  }
  
  tableFunc = getTableFunctions(GTPGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("GTPGENCOUNTERS");
  }
  
  tableFunc = getTableFunctions(SCTPGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SCTPGENCOUNTERS");
  }
  
  tableFunc = getTableFunctions(FILEEVENTHISTORY);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("FILEEVENTHISTORY");
  }
  
  tableFunc = getTableFunctions(SWINSTALLHISTORY);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SWINSTALLHISTORY");
  }
  
  tableFunc = getTableFunctions(ACTIVEUESTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ACTIVEUESTATUS");
  }
  
  tableFunc = getTableFunctions(ACTIVERABSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ACTIVERABSTATUS");
  }
  
  tableFunc = getTableFunctions(UEEVENTHISTORY);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("UEEVENTHISTORY");
  }
  
  tableFunc = getTableFunctions(RABHISTORY);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RABHISTORY");
  }
  
  tableFunc = getTableFunctions(PMLOGFILESTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PMLOGFILESTATUS");
  }
  
  tableFunc = getTableFunctions(MODULELOGFILESTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MODULELOGFILESTATUS");
  }
  
  tableFunc = getTableFunctions(ACTIVEALARMSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ACTIVEALARMSTATUS");
  }
  
  tableFunc = getTableFunctions(ALARMHISTORY);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ALARMHISTORY");
  }
  
  tableFunc = getTableFunctions(LOCATIONSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("LOCATIONSTATUS");
  }
  
  tableFunc = getTableFunctions(GPSSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("GPSSTATUS");
  }
  
  tableFunc = getTableFunctions(RRCGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RRCGENCOUNTERS");
  }
  
  tableFunc = getTableFunctions(RRMCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RRMCOUNTERS");
  }
  
  tableFunc = getTableFunctions(RRCCELLCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RRCCELLCOUNTERS");
  }
  
  tableFunc = getTableFunctions(S1APGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("S1APGENCOUNTERS");
  }
  
  tableFunc = getTableFunctions(S1APPEERNODECOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("S1APPEERNODECOUNTERS");
  }
  
  tableFunc = getTableFunctions(ETHERNETCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ETHERNETCOUNTERS");
  }
  
  tableFunc = getTableFunctions(IPCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("IPCOUNTERS");
  }
  
  tableFunc = getTableFunctions(LOGINSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("LOGINSTATUS");
  }
  
  tableFunc = getTableFunctions(LEDSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("LEDSTATUS");
  }
  
  tableFunc = getTableFunctions(CELLSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("CELLSTATUS");
  }
  
  tableFunc = getTableFunctions(L2COUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("L2COUNTERS");
  }
  
  tableFunc = getTableFunctions(COUNTERCOLLECTIONSTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("COUNTERCOLLECTIONSTATUS");
  }
  
  tableFunc = getTableFunctions(TRACEHISTORY);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("TRACEHISTORY");
  }
  
  tableFunc = getTableFunctions(TRACESTATUS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("TRACESTATUS");
  }
  
}
void emptyOprConfigTables(void)
{
  GenericTable  *tableFunc;
/* OprConfig */
  tableFunc = getTableFunctions(ENBREGISTRATIONCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ENBREGISTRATIONCONFIG");
  }
  
  tableFunc = getTableFunctions(ENODEBCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ENODEBCONFIG");
  }
  
  tableFunc = getTableFunctions(MMECONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MMECONFIG");
  }
  
  tableFunc = getTableFunctions(CELLCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("CELLCONFIG");
  }
  
  tableFunc = getTableFunctions(SCHEDULERCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SCHEDULERCONFIG");
  }
  
  tableFunc = getTableFunctions(PHYCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PHYCONFIG");
  }
  
  tableFunc = getTableFunctions(SYSINFO1CONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SYSINFO1CONFIG");
  }
  
  tableFunc = getTableFunctions(SYSINFO2CONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SYSINFO2CONFIG");
  }
  
  tableFunc = getTableFunctions(LOGCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("LOGCONFIG");
  }
  
  tableFunc = getTableFunctions(RADIOHEADCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RADIOHEADCONFIG");
  }
  
  tableFunc = getTableFunctions(QOSCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("QOSCONFIG");
  }
  
  tableFunc = getTableFunctions(PMCOUNTERLOGCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PMCOUNTERLOGCONFIG");
  }
  
  tableFunc = getTableFunctions(EUTRAHOPROFILECONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("EUTRAHOPROFILECONFIG");
  }
  
  tableFunc = getTableFunctions(EUTRANEIGHCELLCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("EUTRANEIGHCELLCONFIG");
  }
  
  tableFunc = getTableFunctions(SYSINFO3CONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SYSINFO3CONFIG");
  }
  
  tableFunc = getTableFunctions(SONCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SONCONFIG");
  }
  
  tableFunc = getTableFunctions(SRSCONFIGDEDICATED);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SRSCONFIGDEDICATED");
  }
  
  tableFunc = getTableFunctions(SECURITYCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SECURITYCONFIG");
  }
  
  tableFunc = getTableFunctions(UEEVENTHISTORYLOGCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("UEEVENTHISTORYLOGCONFIG");
  }
  
  tableFunc = getTableFunctions(COUNTERCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("COUNTERCONFIG");
  }
  
  tableFunc = getTableFunctions(TRACECONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("TRACECONFIG");
  }
  
}
void emptyPreConfigTables(void)
{
  GenericTable  *tableFunc;
/* PreConfig */
  tableFunc = getTableFunctions(MSGTRACE);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MSGTRACE");
  }
  
  tableFunc = getTableFunctions(TIMERSCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("TIMERSCONFIG");
  }
  
  tableFunc = getTableFunctions(PROCESSSTART);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PROCESSSTART");
  }
  
  tableFunc = getTableFunctions(MODULESTOP);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MODULESTOP");
  }
  
  tableFunc = getTableFunctions(EVENTTHRESHOLDS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("EVENTTHRESHOLDS");
  }
  
  tableFunc = getTableFunctions(ENBADDRESSES);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("ENBADDRESSES");
  }
  
  tableFunc = getTableFunctions(UDHCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("UDHCONFIG");
  }
  
  tableFunc = getTableFunctions(UEHCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("UEHCONFIG");
  }
  
  tableFunc = getTableFunctions(EGTPCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("EGTPCONFIG");
  }
  
  tableFunc = getTableFunctions(SCTPCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SCTPCONFIG");
  }
  
  tableFunc = getTableFunctions(S1APCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("S1APCONFIG");
  }
  
  tableFunc = getTableFunctions(DEVICEMONITORCONFIG);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("DEVICEMONITORCONFIG");
  }
  
}
