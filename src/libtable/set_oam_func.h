/**
 * @file    set_oam_func.h
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains SET functions Prototypes for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 11-02-2013 14:40:18
 */

#ifndef SET_OAM_FUNC_H
#define SET_OAM_FUNC_H
 
#include "generic_table.h"

/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enbAdminstructure.
 * */
void tlv2StructEnbAdminTable(
  IO TlvArray           *tlvArray,
  IO EnbAdmin           *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills fileUploadstructure.
 * */
void tlv2StructFileUploadTable(
  IO TlvArray           *tlvArray,
  IO FileUpload         *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills fileDownloadstructure.
 * */
void tlv2StructFileDownloadTable(
  IO TlvArray           *tlvArray,
  IO FileDownload       *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills swActivationstructure.
 * */
void tlv2StructSwActivationTable(
  IO TlvArray           *tlvArray,
  IO SwActivation       *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills swInstallstructure.
 * */
void tlv2StructSwInstallTable(
  IO TlvArray           *tlvArray,
  IO SwInstall          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills traceControlstructure.
 * */
void tlv2StructTraceControlTable(
  IO TlvArray           *tlvArray,
  IO TraceControl       *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills cellAdminstructure.
 * */
void tlv2StructCellAdminTable(
  IO TlvArray           *tlvArray,
  IO CellAdmin          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills counterControlstructure.
 * */
void tlv2StructCounterControlTable(
  IO TlvArray           *tlvArray,
  IO CounterControl     *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enbRegistrationConfigstructure.
 * */
void tlv2StructEnbRegistrationConfigTable(
  IO TlvArray           *tlvArray,
  IO EnbRegistrationConfig
                        *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enodebConfigstructure.
 * */
void tlv2StructEnodebConfigTable(
  IO TlvArray           *tlvArray,
  IO EnodebConfig       *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills mmeConfigstructure.
 * */
void tlv2StructMmeConfigTable(
  IO TlvArray           *tlvArray,
  IO MmeConfig          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills cellConfigstructure.
 * */
void tlv2StructCellConfigTable(
  IO TlvArray           *tlvArray,
  IO CellConfig         *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills schedulerConfigstructure.
 * */
void tlv2StructSchedulerConfigTable(
  IO TlvArray           *tlvArray,
  IO SchedulerConfig    *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills phyConfigstructure.
 * */
void tlv2StructPhyConfigTable(
  IO TlvArray           *tlvArray,
  IO PhyConfig          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sysInfo1Configstructure.
 * */
void tlv2StructSysInfo1ConfigTable(
  IO TlvArray           *tlvArray,
  IO SysInfo1Config     *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sysInfo2Configstructure.
 * */
void tlv2StructSysInfo2ConfigTable(
  IO TlvArray           *tlvArray,
  IO SysInfo2Config     *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills logConfigstructure.
 * */
void tlv2StructLogConfigTable(
  IO TlvArray           *tlvArray,
  IO LogConfig          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills radioHeadConfigstructure.
 * */
void tlv2StructRadioHeadConfigTable(
  IO TlvArray           *tlvArray,
  IO RadioHeadConfig    *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills qosConfigstructure.
 * */
void tlv2StructQosConfigTable(
  IO TlvArray           *tlvArray,
  IO QosConfig          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills pmCounterLogConfigstructure.
 * */
void tlv2StructPmCounterLogConfigTable(
  IO TlvArray           *tlvArray,
  IO PmCounterLogConfig *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills eutraHoProfileConfigstructure.
 * */
void tlv2StructEutraHoProfileConfigTable(
  IO TlvArray           *tlvArray,
  IO EutraHoProfileConfig
                        *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills eutraNeighCellConfigstructure.
 * */
void tlv2StructEutraNeighCellConfigTable(
  IO TlvArray           *tlvArray,
  IO EutraNeighCellConfig
                        *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sysInfo3Configstructure.
 * */
void tlv2StructSysInfo3ConfigTable(
  IO TlvArray           *tlvArray,
  IO SysInfo3Config     *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sonConfigstructure.
 * */
void tlv2StructSonConfigTable(
  IO TlvArray           *tlvArray,
  IO SonConfig          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills srsConfigDedicatedstructure.
 * */
void tlv2StructSrsConfigDedicatedTable(
  IO TlvArray           *tlvArray,
  IO SrsConfigDedicated *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills securityConfigstructure.
 * */
void tlv2StructSecurityConfigTable(
  IO TlvArray           *tlvArray,
  IO SecurityConfig     *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills ueEventHistoryLogConfigstructure.
 * */
void tlv2StructUeEventHistoryLogConfigTable(
  IO TlvArray           *tlvArray,
  IO UeEventHistoryLogConfig
                        *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills counterConfigstructure.
 * */
void tlv2StructCounterConfigTable(
  IO TlvArray           *tlvArray,
  IO CounterConfig      *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills traceConfigstructure.
 * */
void tlv2StructTraceConfigTable(
  IO TlvArray           *tlvArray,
  IO TraceConfig        *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills msgTracestructure.
 * */
void tlv2StructMsgTraceTable(
  IO TlvArray           *tlvArray,
  IO MsgTrace           *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills timersConfigstructure.
 * */
void tlv2StructTimersConfigTable(
  IO TlvArray           *tlvArray,
  IO TimersConfig       *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills processStartstructure.
 * */
void tlv2StructProcessStartTable(
  IO TlvArray           *tlvArray,
  IO ProcessStart       *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills moduleStopstructure.
 * */
void tlv2StructModuleStopTable(
  IO TlvArray           *tlvArray,
  IO ModuleStop         *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills eventThresholdsstructure.
 * */
void tlv2StructEventThresholdsTable(
  IO TlvArray           *tlvArray,
  IO EventThresholds    *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enbAddressesstructure.
 * */
void tlv2StructEnbAddressesTable(
  IO TlvArray           *tlvArray,
  IO EnbAddresses       *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills udhConfigstructure.
 * */
void tlv2StructUdhConfigTable(
  IO TlvArray           *tlvArray,
  IO UdhConfig          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills uehConfigstructure.
 * */
void tlv2StructUehConfigTable(
  IO TlvArray           *tlvArray,
  IO UehConfig          *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills egtpConfigstructure.
 * */
void tlv2StructEgtpConfigTable(
  IO TlvArray           *tlvArray,
  IO EgtpConfig         *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sctpConfigstructure.
 * */
void tlv2StructSctpConfigTable(
  IO TlvArray           *tlvArray,
  IO SctpConfig         *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills s1apConfigstructure.
 * */
void tlv2StructS1apConfigTable(
  IO TlvArray           *tlvArray,
  IO S1apConfig         *config
  );
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills deviceMonitorConfigstructure.
 * */
void tlv2StructDeviceMonitorConfigTable(
  IO TlvArray           *tlvArray,
  IO DeviceMonitorConfig*config
  );
#endif  /* SET_OAM_FUNC.H */
