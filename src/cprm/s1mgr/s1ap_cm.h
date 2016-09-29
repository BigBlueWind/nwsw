/**
 * @file    s1ap_cm.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details
 * This file contains S1AP structures, these are used by RRM
 * We use wrap functions to convert these structures into CCPU structures and
 * Unwrap functions are needed to convert the data from CCPU structure to PWAV structures
 *
 * @author Lakshmi Narayana Madala
 *         Shweta Polepally :30th Aug 2011 : Added Code under HO_SUPPORTED
 */

#ifndef S1AP_CM_H
#define S1AP_CM_H

/* Common include files */
#include "cmn_types.h"
#include "ueh_cfg.h"

#define MAX_SIZE_ENBID                          4
#define MAX_SIZE_CSGID                          4
#define MAX_SIZE_MCCMNC                         3
#define SIZE_PLMNIDENTITY                       3
#define SIZE_MMEGROUPID                         2
#define SIZE_MMECODE                            1
#define SIZE_TAC                                2
#define MAX_SIZE_NAS_PDU                        256
#define MAX_SIZE_UE_RADIO_CAPABILITY            100
#define MAX_SIZE_M_TIMSI                        4
#define MAX_SIZE_CELLID                         4
#define MAX_SIZE_UE_CAPBILITY_INFO              256

/***** Macros related to Write-Replace Warning Message from MME ***********/
#define MAX_NUM_OF_CELL_IDS_FOR_WARN_MSG_BROADCAST            65535
#define MAX_NUM_OF_TAIS_FOR_WARN_MSG_BROADCAST                65535
#define MAX_NUM_OF_EMERGENCY_AREA_IDS_FOR_WARN_MSG_BROADCAST  65535
#define MAX_SIZE_WARN_MSG_CONTENTS                            9600
#define SIZE_EMERGENCYAREAID                                  3
#define SIZE_WARNINGTYPE                                      2
#define SIZE_WARNINGSECURITYINFO                              50
/* OCTECT STRING: used for Unwrapping message */
#define MAX_SIZE_WARNMSGID                                        2 /* BITSTRING = 16 */
#define MAX_SIZE_SERIALNUM                                        2 /* BITSTRING = 16 */
#define MAX_SIZE_DATACODINGSCHEME                                 1 /* BITSTRING = 8 */
/* BITSTRING: used for wrapping message */
#define SIZE_WARNMSGID                                        16 /* BITSTRING = 16 */
#define SIZE_SERIALNUM                                        16 /* BITSTRING = 16 */

/*****************************************************************************/

/*********** Macros related to Handover ************/
/* S1AP Handover */
#define SIZE_LAC                                2
#define SIZE_RAC                                1
#define SIZE_CI                                 2
#define SIZE_NAS_SECURITY_PARAMS_FROM_EUTRAN    2
#define MAX_SIZE_MS_CLASSMARK_2                 5   /* Check this value is 48.008 spec */
#define MAX_SIZE_MS_CLASSMARK_3                 34  /* Check this value is 48.008 spec */
#define MAX_SIZE_SRC_TO_TGT_TPRNT_CONT          512
#define MAX_SIZE_TGT_TO_SRC_TPRNT_CONT          512
#define MAX_HANDOVER_PREP_INFO_MSG              512
#define MAX_HANDOVER_CMD_MSG                    512
/*****************************************************/
#define MAX_SIZE_MMENAME                        150
#define MAX_SIZE_ENBNAME                        150
#define MAX_SIZE_ENBIPV4ADDR                    15

#define MAX_BROADCASTPLMN                       10
#define MAX_TAC_SUPPORTED                       10
#define MAX_MME_GROUP_ID                        10
#define MAX_MME_SERVED                          10
#define MAX_GUMMEIES_SERVED                     10
#define MAX_ENODEB_SERVED                       10
#define MAX_CSG_LIST_SUPPORTED                  10
#define MAX_IE_CRITICAL_DIAGNOSTIC              10
#define MAX_ERAB_ITEMS_IN_LIST                  8

#define MAX_NUM_OF_CELLS                        16
#define MAX_ERAB_ITEMS_INFO_EACH_UE             256

/* Bit String Sizes */
#define SIZE_MACROENBID                         20
#define SIZE_HOMEENBID                          28
#define SIZE_CELLID                             28
#define SIZE_CSGID                              27

typedef enum e_LocReportTypeEvent
{
  LOCREQTYPE_EVENT_DIRECT,
  LOCREQTYPE_EVENT_CHANGE_OF_SERVICE_CELL,
  LOCREQTYPE_EVENT_STOP_CHANGE_OF_SERVICE_CELL
} LocReportTypeEvent;

/* Location report Area */
typedef enum e_LocReportArea
{
  LOCREQTYPE_REPORT_AREA_ECGI
} LocReportArea;

typedef enum e_EnBType
{
  EnbType_MacroEnodeB,
  EnbType_HomeEnodeB
}EnbType;

typedef enum e_ResetType
{
  ResetType_GlobalS1Reset,
  ResetType_UeS1Reset
}ResetType;

/* These are as per 3GPP TS 36.413 */
typedef enum e_CauseType
{
  CauseType_RadioNw,
  CauseType_Transport,
  CauseType_Nas,
  CauseType_Prototcol,
  CauseType_Misc
} CauseType;

typedef enum e_TimeToWait
{
  TimeToWait_OneSec,
  TimeToWait_TwoSec,
  TimeToWait_FiveSec,
  TimeToWait_TenSec,
  TimeToWait_TwentySec,
  TimeToWait_SixtySec
}TimeToWait;

typedef enum e_TriggeringMsg
{
  TriggeringMsg_InitMsg,
  TriggeringMsg_SuccOutCome,
  TriggeringMsg_UnsuccOutCome
}TriggeringMsg;

typedef enum e_Criticality
{
  Criticality_Reject,
  Criticality_Ignore,
  Criticality_Notify
}Criticality;

typedef enum e_TypeOfError
{
  TypeOfError_NotUnderstood,
  TypeOfError_Missing
}TypeOfError;

typedef enum e_RrcEstablishmentCause
{
  RrcEstablishmentCause_Emergency,
  RrcEstablishmentCause_HighPriorityAccess,
  RrcEstablishmentCause_MtAccess,
  RrcEstablishmentCause_MoSignalling,
  RrcEstablishmentCause_MoData
}RrcEstablishmentCause;

typedef enum e_OverloadAction
{
  OverloadAction_RejectAllRrcNonEmergencyMoData,
  OverloadAction_RejectAllRrcConnEstForSignalling,
  OverloadAction_PermitEmergencyOnly,
  OverloadAction_PermitAllCalls                /* This is a special value, propetiary; means allow all calls no overload */
}OverloadAction;

typedef enum e_PreemptionCability
{
  PreemptionCability_ShallNotTriggerPreemption,
  PreemptionCability_MayTriggerPreemption
}PreemptionCability;

typedef enum e_PreemptionVulnerability
{
  PreemptionVulnerability_NotPreemptable,
  PreemptionVulnerability_Preemptable
}PreemptionVulnerability;

typedef enum e_SrvccOperationPossible
{
  SrvccOperationPossible_possible
}SrvccOperationPossible;

typedef enum e_CsFallBackIndicator
{
  CsFallBackIndicator_CsFallBackRequired,
  CsFallBackIndicator_CsFallBackHighPriority
}CsFallBackIndicator;

typedef enum e_Cdma2000HoRequiredInd
{
  Cdma2000HoRequiredInd_True
}Cdma2000HoRequiredInd;

typedef Cdma2000HoRequiredInd SubscriberProfileIdForRat;
typedef enum e_TraceDepth
{
  TraceDepth_Minimum,
  TraceDepth_Medium,
  TraceDepth_Maximum,
  TraceDepth_MinimumWithoutVendorSpecificExtns,
  TraceDepth_MediumWithoutVendorSpecificExtns,
  TraceDepth_MaximumWithoutVendorSpecificExtns
}TraceDepth;

#define SIZE_IPV4_ADDR                  4
#define SIZE_IPV6_ADDR                  16
#define SIZE_IPV4_AND_IPV6_ADDR         20
typedef enum e_TrasportAddrType
{
  TrasportAddrType_Ipv4,
  TrasportAddrType_Ipv6,
  TrasportAddrType_Both
} TrasportAddrType;

typedef UINT8     RelativeMmeCapacity;
typedef UINT32    MmeUeS1apId;
typedef UINT32    EnbUeS1apId;
typedef UINT8     ProcedureCode;
typedef UINT16    IeId;

typedef UINT8     ERabId;
typedef UINT8     QCI;
typedef UINT8     PriorityLevel;
typedef UINT8     InterfaceToTrace;

#define BITRATE_LEN             5
typedef struct s_BitRate
{
UINT8       bitRate[BITRATE_LEN];
}BitRate;

typedef struct s_NasPdu
{
  UINT16  pduLen;
  UINT8   pdu[MAX_SIZE_NAS_PDU];
}NasPdu;

typedef struct s_UeCapInfo
{
  UINT16  infoLen;
  UINT8   capInfopdu[MAX_SIZE_UE_CAPBILITY_INFO];
}UeRadioCapInfo;

typedef struct s_S1Cause
{
  CauseType causeType;
  UINT32    causeVal;
}S1Cause;

typedef struct s_IeCriticalDiagnostic
{
  Criticality             ieCriticality;
  IeId                    ieId;
  TypeOfError             typeOfError;
}IeCriticalDiagnostic;

typedef struct s_CriticalDiagnostic
{
  TBOOL                   isProcedureCode;
  ProcedureCode           procedureCode;
  TBOOL                   isTriggeringMsg;
  TriggeringMsg           triggeringMsg;
  TBOOL                   isProcedureCriticality;
  Criticality             procedureCriticality;
  UINT16                  noOfIeCriticalDiagnostics;
  IeCriticalDiagnostic    ieCriticalDiagnostic[MAX_IE_CRITICAL_DIAGNOSTIC];
}CriticalDiagnostic;

typedef struct s_S1UeAssocLogConnItem
{
  TBOOL          isMmeUeS1apId;
  MmeUeS1apId    mmeUeS1apId;
  TBOOL          isEnbUeS1apId;
  EnbUeS1apId    enbUeS1apId;
}S1UeAssocLogConnItem;

typedef struct s_S1UeAssocLogConnList
{
  UINT32     numOfS1UeAssocLogConnItems;
  S1UeAssocLogConnItem    s1ConnItem[MAX_UE_SUPP];
}S1UeAssocLogConnList;

typedef struct s_ServedPlmns
{
  UINT16   numPlmns;
  UINT8    plmnId[MAX_BROADCASTPLMN][SIZE_PLMNIDENTITY];
}ServedPlmns;

typedef struct s_ServedMmeGrpIds
{
  UINT16   numMmeGrp;
  UINT8    mmeGroupId[MAX_MME_GROUP_ID][SIZE_MMEGROUPID];
}ServedMmeGrpIds;

typedef struct s_ServedMmes
{
  UINT16   numMme;
  UINT8    mmeId[MAX_MME_SERVED][SIZE_MMECODE];
}ServedMmes;

typedef struct S_ServedGummeie
{
  ServedPlmns     servedPlmns;
  ServedMmeGrpIds servedMmeGrpIds;
  ServedMmes      servedMmes;
}ServedGummeie;

typedef struct S_ServedGummeies
{
  UINT16        numOfGummeies;
  ServedGummeie servedGummeIe[MAX_GUMMEIES_SERVED];
}ServedGummeies;

typedef struct s_Gummei
{
  UINT8    plmnId[SIZE_PLMNIDENTITY];
  UINT8    mmeGroupId[SIZE_MMEGROUPID];
  UINT8    mmeId;
}Gummei;

typedef struct s_SupportedTac
{
  UINT8      tac[SIZE_TAC];
  ServedPlmns servedBroadcastPlmns;
}SupportedTac;

typedef struct s_SupportedTacs
{
  UINT16            numOfSupportedTac;
  SupportedTac      tacs[MAX_TAC_SUPPORTED];
}SupportedTacs;

typedef struct   s_CsgId
{
  UINT8      csgId[MAX_SIZE_CSGID];
}CsgId;

typedef struct s_CsgIdList
{
  UINT16            numOfSupportedCsg;
  CsgId             csgId[MAX_CSG_LIST_SUPPORTED];
}CsgIdList;

typedef struct s_Tai
{
  UINT8    plmnIdentity[SIZE_PLMNIDENTITY];
  UINT8    tac[SIZE_TAC];
}Tai;

typedef struct s_EUtranCgi
{
  UINT8          plmnIdentity[SIZE_PLMNIDENTITY];
  UINT8          cellIdentity[MAX_SIZE_CELLID];
}EUtranCgi;

typedef struct s_MTimsi
{
  UINT8         mTimsi[MAX_SIZE_M_TIMSI];
}MTimsi;

typedef struct s_STimsi
{
  UINT8         mmeCode;
  MTimsi        mTimsi;
}STimsi;

typedef struct s_GlobalEnodeBId
{
  UINT8    plmnIdentity[SIZE_PLMNIDENTITY];
  EnbType  enodeBType;
  UINT8    enbId[MAX_SIZE_ENBID];
}GlobalEnodeBId;

typedef struct s_UeAggMaxBitRate
{
  BitRate ueAggMaxBitRateDl;
  BitRate ueAggMaxBitRateUl;
}UeAggMaxBitRate;

typedef struct s_GbrQosInfo
{
  BitRate ueMaxBitRateDl;
  BitRate ueMaxBitRateUl;
  BitRate ueGurantedBitRateDl;
  BitRate ueGurantedBitRateUl;
}GbrQosInfo;

typedef struct s_AllocRetenPriority
{
  PriorityLevel                 priorityLevel;
  PreemptionCability            preemptionCability;
  PreemptionVulnerability       PreemptionVulnerability;
}AllocRetenPriority;

typedef struct s_ERabLevelQosParams
{
  QCI                   qci;
  AllocRetenPriority    allocRetenPriority;
  TBOOL                 isGbrQosInfo;
  GbrQosInfo            gbrQosInfo;
}ERabLevelQosParams;

#define TRANSPORT_LAYER_ADDR_LEN              20
typedef struct s_TransportLayerAddr
{
  TrasportAddrType transportAddrType;
  UINT8            transportLevelAddr[TRANSPORT_LAYER_ADDR_LEN];
}TransportLayerAddr;

#define GTP_TE_ID_LEN                   4
typedef struct s_GtPTeId
{
  UINT8         gtPTeId[GTP_TE_ID_LEN];
}GtPTeId;

typedef struct s_ERabToSetupItem
{
  ERabId                eRabId;
  ERabLevelQosParams    eRabLevelQosParams;
  TransportLayerAddr    transportLayerAddr;
  GtPTeId               gtPTeId;
  TBOOL                 isNasPdu;
  NasPdu                nasPdu;
}ERabToSetupItem;

typedef struct s_ERabToModifyItem
{
  ERabId                eRabId;
  ERabLevelQosParams    eRabLevelQosParams;
  NasPdu                nasPdu;
}ERabToModifyItem;

typedef struct s_ERabToReleaseItem
{
  ERabId       eRabId;
  S1Cause      cause;
}ERabReleaseItem;

typedef struct s_ERabSetupItem
{
  ERabId                eRabId;
  TransportLayerAddr    transportLayerAddr;
  GtPTeId               gtPTeId;
}ERabSetupItem;

typedef struct s_ERabModifyItem
{
  ERabId                eRabId;
}ERabModifyItem;

typedef struct s_ERabToSetupList
{
  UINT16          noComp;
  ERabToSetupItem eRabToSetupItem[MAX_ERAB_ITEMS_IN_LIST];
}ERabToSetupList;

typedef struct s_ERabToModifyList
{
  UINT16                noComp;
  ERabToModifyItem      eRabToModifyItem[MAX_ERAB_ITEMS_IN_LIST];
}ERabToModifyList;

typedef struct s_ERabToReleaseList
{
  UINT16              noComp;
  ERabReleaseItem     eRabToReleaseItem[MAX_ERAB_ITEMS_IN_LIST];
}ERabReleaseList;

typedef struct s_ERabSetupList
{
  UINT16         noComp;
  ERabSetupItem  eRabSetupItem[MAX_ERAB_ITEMS_IN_LIST];
}ERabSetupList;

typedef struct s_ERabModifyList
{
  UINT16          noComp;
  ERabModifyItem  eRabModifyItem[MAX_ERAB_ITEMS_IN_LIST];
}ERabModifyList;

typedef struct s_UeRadioCapability
{
  UINT16 len;
  UINT8  UeRadioCapability[MAX_SIZE_UE_RADIO_CAPABILITY];
}UeRadioCapability;

#define EUTRAN_TRACE_ID_LEN              8
typedef struct s_EUtranTraceId
{
  UINT8         eUtranTraceId[EUTRAN_TRACE_ID_LEN];
}EUtranTraceId;

typedef struct s_TraceActivation
{
  EUtranTraceId         eUtranTraceId;
  InterfaceToTrace      interfaceToTrace;
  TraceDepth            traceDepth;
  TransportLayerAddr    transportLayerAddr;
}TraceActivation;

#define SECURITY_KEY_LEN        32
typedef struct s_SecurityKey
{
  UINT8       securityKey[SECURITY_KEY_LEN];
}SecurityKey;

#define UE_SECURITY_CAP_SUPP_ALGO_LEN     2
typedef struct s_UeSecurityCapability
{
  UINT8         encryptionAlgorithms[UE_SECURITY_CAP_SUPP_ALGO_LEN];
  UINT8         integrityAlgorithms[UE_SECURITY_CAP_SUPP_ALGO_LEN];
}UeSecurityCapability;

typedef struct s_ERabListItem
{
  ERabId       eRabId;
  S1Cause      cause;
}ERabListItem;

typedef struct s_ERabList
{
  UINT16         noItems;
  ERabListItem   eRabListItem[MAX_ERAB_ITEMS_IN_LIST];
}ERabList;

typedef struct s_UeS1apIdPair
{
  MmeUeS1apId    mmeUeS1apId;
  EnbUeS1apId    enbUeS1apId;
}UeS1apIdPair;

typedef enum e_UeS1apIdChoice
{
  UeS1apIdChoice_UeIdPair,
  UeS1apIdChoice_MmeUeS1apId
}UeS1apIdChoice;
typedef struct s_UeS1apId
{
  UINT8   choice;
  union
  {
    UeS1apIdPair   ueS1apIdPair;
    MmeUeS1apId    mmeUeS1apId;
  }u;
}UeS1apId;

/* Messages Between EbodeB <-> MME */
typedef struct s_S1SetupReq
{
  GlobalEnodeBId    globalEnodeBId;
  TBOOL             isEnodeBNamePresent;
  UINT8             enbName[MAX_SIZE_ENBNAME];
  SupportedTacs     supportedTacs;
  TBOOL             isCsgListPresent;
  CsgIdList         csgIdList;
  UINT16            pagingDrx;
}S1SetupReq;

typedef struct s_S1SetupRsp
{
  TBOOL               isMmeName;
  UINT8               mmeName[MAX_SIZE_MMENAME];
  ServedGummeies      servedGummeIes;
  RelativeMmeCapacity relativeMmeCapacity;
  TBOOL               isCriticalDiagnostic;
  CriticalDiagnostic  criticalDiagnostic;
}S1SetupRsp;

typedef struct s_S1SetupFail
{
  S1Cause    failCause;
  TBOOL      isTimeToWait;
  TimeToWait timetoWait;
  TBOOL      isCriticalDiagnostic;
  CriticalDiagnostic criticalDiagnostic;
}S1SetupFail;

typedef struct s_S1ResetReq
{
  S1Cause      resetCause;
  ResetType    resetType;
  union Reset
  {
    TBOOL        isGlobalReset;
    S1UeAssocLogConnList ueAssocLogConnList;
  }u;
}S1ResetReq;

typedef struct s_S1ResetAck
{
  TBOOL                isUeAssociatedLogConnList;
  S1UeAssocLogConnList ueAssocLogConnList;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}S1ResetAck;

typedef struct s_InitialUeMessage
{
  EnbUeS1apId    enbUeS1apId;
  NasPdu         nasPdu;
  Tai            tai;
  EUtranCgi      eUtranCgi;
  TBOOL          isSTimsi;
  STimsi         sTimsi;
  TBOOL          isCsgId;
  CsgId          csgId;
  RrcEstablishmentCause rrcEstablishmentCause;
  TBOOL          isGummei;
  Gummei         gummei;
}InitialUeMessage;

typedef struct s_DlNasMessage
{
  MmeUeS1apId    mmeUeS1apId;
  EnbUeS1apId    enbUeS1apId;
  NasPdu         nasPdu;
  /* TODO Add support for Handover Restricted List */
}DlNasMessage;

typedef struct s_UlNasMessage
{
  MmeUeS1apId    mmeUeS1apId;
  EnbUeS1apId    enbUeS1apId;
  NasPdu         nasPdu;
  EUtranCgi      eUtranCgi;
  Tai            tai;
}UlNasMessage;

typedef struct s_InitialContextSetup
{
  MmeUeS1apId                   mmeUeS1apId;
  EnbUeS1apId                   enbUeS1apId;
  UeAggMaxBitRate               ueAggMaxBitRate;
  ERabToSetupList               eRabToSetupList;
  UeSecurityCapability          ueSecurityCapability;
  SecurityKey                   securityKey;
  TBOOL                         isTraceActivation;
  TraceActivation               traceActivation;
  /* TODO Add support for Handover Restriction List */
  TBOOL                         isUeRadioCapability;
  UeRadioCapability             ueRadioCapability;
  TBOOL                         isSubscriberProfileIdForRat;
  SubscriberProfileIdForRat     subscriberProfileIdForRat;
  TBOOL                         isCsFallBackIndicator;
  CsFallBackIndicator           csFallBackIndicator;
  TBOOL                         isSrvccOperationPossible;
  SrvccOperationPossible        srvccOperationPossible;
}InitialContextSetup;

typedef struct s_InitialContextSetupRsp
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  ERabSetupList        eRabSetupList;
  TBOOL                isERabList;
  ERabList             eRabList;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}InitialContextSetupRsp;

typedef struct s_InitialContextSetupFail
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  S1Cause              cause;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}InitialContextSetupFail;

typedef struct s_ErrInd
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  S1Cause              cause;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}ErrInd;

typedef struct s_UeContextRelReq
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  S1Cause              cause;
}UeContextRelReq;

typedef struct s_UeContextRelCmp
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}UeContextRelCmp;

typedef struct s_UeContextRelCmd
{
  UeS1apId          ueS1apId;
  S1Cause           cause;
}UeContextRelCmd;

typedef struct s_NasNonDeliveryInd
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  NasPdu               nasPdu;
  S1Cause              cause;
}NasNonDeliveryInd;

typedef struct s_UeCapInfoInd
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  UeRadioCapInfo       ueRadioCapInfoPdu;
}UeCapInfoInd;

typedef enum ueIdChoice
{
  UEID_STMSI,
  UEID_IMSI
}UeIdChoice;

typedef struct s1STmsi
{
  U8   mmec;
  U32  sTMSIVal;
} S1STmsi;

typedef struct s_OverloadResponse
{
 OverloadAction overloadAction;
}OverloadResponse;

typedef struct s1imsi
{
  U8   noOfImsiDigits;
  U8   imsiVal[21];
} S1Imsi;

typedef struct pagingDrx
{
  Bool pagingDrxPres;
  U32  ueSpcPagDrx;
} PagingDrx;

typedef struct pagingPlmn
{
  U8	  mcc[MAX_SIZE_CELLCONFIG_PLMNMCC];
  U8      mnc[MAX_SIZE_CELLCONFIG_PLMNMNC];
  TBOOL   fillerBitInMnc;
} PagingPlmn;

typedef struct pagingTai
{
  PagingPlmn	plmnId;
  U16           tac;
} PagingTai;

typedef struct s_S1PagingInfo
{
  U32            ueId;
  UeIdChoice     choice;
  union
  {
    S1Imsi   imsi;
    S1STmsi  sTmsi;
  } val;
  PagingDrx      ueSpecDrx;
  U8             cnDomain;
  U16            noOfTaiPresent;
  PagingTai      taiList[6];
} S1PagingInfo;

typedef struct s_MmeConfigUpdate
{
  TBOOL               isMmeName;
  UINT8               mmeName[MAX_SIZE_MMENAME];
  TBOOL               isServedGummeIes;
  ServedGummeies      servedGummeIes;
  TBOOL               isRelativeMmeCapacity;
  RelativeMmeCapacity relativeMmeCapacity;
}MmeCfgUpdate;

typedef struct s_MmeConfigUpdateAck
{
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}MmeCfgUpdateAck;

typedef struct s_MmeConfigUpdateFail
{
  S1Cause    failCause;
  TBOOL      isTimeToWait;
  TimeToWait timetoWait;
  TBOOL      isCriticalDiagnostic;
  CriticalDiagnostic criticalDiagnostic;
}MmeCfgUpdateFail;


/* Location report */
typedef struct s_LocReportCntrlReqType
{
  LocReportTypeEvent   locReportTypeEvent;
  LocReportArea        locReportArea;
} LocReportCntrlReqType;

typedef struct s_LocReqReportCntrl
{
  MmeUeS1apId            mmeUeS1apId;
  EnbUeS1apId            enbUeS1apId;
  LocReportCntrlReqType  locReportCntrlReqType;
}LocReqReportCntrl;

typedef struct s_LocReqReportFail
{
  MmeUeS1apId            mmeUeS1apId;
  EnbUeS1apId            enbUeS1apId;
  S1Cause                cause;
}LocReqReportFail;

typedef struct s_LocReqReport
{
  MmeUeS1apId            mmeUeS1apId;
  EnbUeS1apId            enbUeS1apId;
  EUtranCgi              eUtranCgi;
  Tai                    tai;
  LocReportCntrlReqType  locReportCntrlReqType;
}LocReqReport;

typedef struct s_TraceStart
{
  MmeUeS1apId            mmeUeS1apId;
  EnbUeS1apId            enbUeS1apId;
  TraceActivation        traceActivation;
}TraceStart;

typedef struct s_DeactivateTrace
{
  MmeUeS1apId            mmeUeS1apId;
  EnbUeS1apId            enbUeS1apId;
  EUtranTraceId          eUtranTraceId;
}DeactivateTrace;

typedef struct s_TraceFailInd
{
  MmeUeS1apId            mmeUeS1apId;
  EnbUeS1apId            enbUeS1apId;
  EUtranTraceId          eUtranTraceId;
  S1Cause                cause;
}TraceFailInd;

typedef struct s_ERabSetupReq
{
  MmeUeS1apId                   mmeUeS1apId;
  EnbUeS1apId                   enbUeS1apId;
  TBOOL                         isUeAggMaxBitRate;
  UeAggMaxBitRate               ueAggMaxBitRate;
  ERabToSetupList               eRabToSetupList;
}ERabSetupReq;

typedef struct s_ERabSetupRsp
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  TBOOL                isERabSetupList;
  ERabSetupList        eRabSetupList;
  TBOOL                isERabFailedSetupList;
  ERabList             eRabFailedSetupList;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}ERabSetupRsp;

typedef struct s_ERabModifyReq
{
  MmeUeS1apId                   mmeUeS1apId;
  EnbUeS1apId                   enbUeS1apId;
  TBOOL                         isUeAggMaxBitRate;
  UeAggMaxBitRate               ueAggMaxBitRate;
  ERabToModifyList              eRabToModifyList;
}ERabModifyReq;

typedef struct s_ERabModifyRsp
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  TBOOL                isERabModifyList;
  ERabModifyList       eRabModifyList;
  TBOOL                isERabFailedModifyList;
  ERabList             eRabFailedModifyList;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}ERabModifyRsp;

typedef struct s_ERabReleaseReq
{
  MmeUeS1apId                   mmeUeS1apId;
  EnbUeS1apId                   enbUeS1apId;
  TBOOL                         isUeAggMaxBitRate;
  UeAggMaxBitRate               ueAggMaxBitRate;
  ERabReleaseList               eRabToReleaseList;
  TBOOL                         isNasPdu;
  NasPdu                        nasPdu;
}ERabReleaseReq;

typedef struct s_ERabReleaseRsp
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  TBOOL                isERabReleaseList;
  ERabReleaseList      eRabReleaseList;
  TBOOL                isERabFailedReleaseList;
  ERabList             eRabFailedReleaseList;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}ERabReleaseRsp;

typedef struct s_OverloadStart
{
  OverloadResponse overloadResponse;
}OverloadStart;

typedef struct s_UeContextModReq
{
  MmeUeS1apId                   mmeUeS1apId;
  EnbUeS1apId                   enbUeS1apId;
  TBOOL                         isSecurityKey;
  SecurityKey                   securityKey;
  TBOOL                         isUeAggMaxBitRate;
  UeAggMaxBitRate               ueAggMaxBitRate;
  TBOOL                         isSubscriberProfileIdForRat;
  SubscriberProfileIdForRat     subscriberProfileIdForRat;
  TBOOL                         isCsFallBackIndicator;
  CsFallBackIndicator           csFallBackIndicator;
}UeContextModReq;

typedef struct s_UeContextModRsp
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}UeContextModRsp;

typedef struct s_UeContextModFail
{
  MmeUeS1apId          mmeUeS1apId;
  EnbUeS1apId          enbUeS1apId;
  S1Cause              cause;
  TBOOL                isCriticalDiagnostic;
  CriticalDiagnostic   criticalDiagnostic;
}UeContextModFail;

typedef struct s_CellIdListForWarning
{
  UINT16        numOfCellIds;
  EUtranCgi     eUtranCgi[MAX_NUM_OF_CELL_IDS_FOR_WARN_MSG_BROADCAST];
} CellIdListForWarning;

typedef struct s_TaiListForWarning
{
  UINT16        numOfTais;
  Tai           tai[MAX_NUM_OF_TAIS_FOR_WARN_MSG_BROADCAST];
} TaiListForWarning;

typedef struct s_EmergencyAreaId
{
  UINT8  areaId[SIZE_EMERGENCYAREAID];
} EmergencyAreaId;

typedef struct s_EmergencyAreaIdListForWarning
{
  UINT16           numOfEmergencyAreaIds;
  EmergencyAreaId  emergencyAreaId[MAX_NUM_OF_EMERGENCY_AREA_IDS_FOR_WARN_MSG_BROADCAST];
} EmergencyAreaIdListForWarning;

typedef enum e_WarnAreaListType
{
  WarnAreaListType_CellIdList,
  WarnAreaListType_TaiList,
  WarnAreaListType_EmergencyAreaIdList
} WarnAreaListType;

typedef struct s_WarnAreaList
{
  WarnAreaListType warnAreaListType;
  union {
    CellIdListForWarning          cellIdListForWarning;
    TaiListForWarning             taiListForWarning;
    EmergencyAreaIdListForWarning emergencyAreaIdListForWarning;
  } val;
} WarnAreaList;

typedef struct s_WarnMsgContents
{
  UINT16 warnMsgLen;
  UINT8  warnMsg[MAX_SIZE_WARN_MSG_CONTENTS];
} WarnMsgContents;

typedef struct s_WriteReplcWarnReq
{
  UINT8            warnMsgId[MAX_SIZE_WARNMSGID];
  UINT8            serialNum[MAX_SIZE_SERIALNUM];
  TBOOL            isWarnAreaListPresent;
  WarnAreaList     warnAreaList;
  UINT16           repetitionPeriod;             /* Range (0..4095) */
  UINT16           numOfBroadcastRequested;      /* Range (0..65535) */
  TBOOL            isWarningTypePresent;
  UINT8            warningType[SIZE_WARNINGTYPE];
  TBOOL            isWarningSecurityInfoPresent;
  UINT8            warningSecurityInfo[SIZE_WARNINGSECURITYINFO];
  TBOOL            isDataCodingSchemePresent;
  UINT8            dataCodingScheme[MAX_SIZE_DATACODINGSCHEME];
  TBOOL            isWarnMsgContentsPresent;
  WarnMsgContents  warnMsgContents;
} WriteReplcWarnReq;

typedef struct s_WriteReplcWarnRsp
{
  UINT8                    warnMsgId[MAX_SIZE_WARNMSGID];
  UINT8                    serialNum[MAX_SIZE_SERIALNUM];
  TBOOL                    isBroadcastCmpltAreaListPresent;
/*BroadcastCmpltAreaList   broadcastCmpltAreaList; (TODO when supported)  */
  TBOOL                    isCriticalDiagnosticPresent;
  CriticalDiagnostic       criticalDiagnostic;
} WriteReplcWarnRsp;

/******************************
 * Added for S1AP Handover *
 ******************************/
typedef enum e_HandoverType
{
  HandoverType_IntraLte,
  HandoverType_LteToUtran,
  HandoverType_LteToGeran,
  HandoverType_UtranToLte,
  HandoverType_GeranToLte
} HandoverType;

typedef enum e_TargetIdType
{
  TargetType_TargetEnbId,
  TargetType_TargetRncId,
  TargetType_Cgi
} TargetIdType;

typedef enum e_DirectFwdPathAvailability
{
  DirectFwdPathAvailability_DirectPathAvailable
} DirectFwdPathAvailability;

typedef enum e_SrvccHoIndication
{
  SrvccHoIndication_PsAndCs,
  SrvccHoIndication_CsOnly
} SrvccHoIndication;

typedef enum e_CellSize
{
  CellSize_VerySmall,
  CellSize_Small,
  CellSize_Medium,
  CellSize_Large
} CellSize;

typedef enum UtranCellType
{
  CellType_Macro,
  CellType_Micro,
  CellType_Pico,
  CellType_Femto
} UtranCellType;

typedef enum e_LastVisitedCellType
{
  LastVisitedCellType_Eutran,
  LastVisitedCellType_Utran,
  LastVisitedCellType_Geran
} LastVisitedCellType;

typedef enum e_DlFwdingList
{
  DlFwdingList_DlFowdingProposed
} DlFwdingList;

typedef struct s_TargetEnbId
{
  GlobalEnodeBId   globalEnodeBId;
  Tai              tai;
} TargetEnbId;

typedef struct s_Lai
{
  UINT8      plmnId[SIZE_PLMNIDENTITY];
  UINT8      lac[SIZE_LAC];
} Lai;

typedef struct s_TargetRncId
{
  Lai       lai;
  UINT8     rac[SIZE_RAC];
  UINT16    rncId;
  UINT16    extendedRncId;
} TargetRncId;

typedef struct s_Cgi
{
  UINT8      plmnId[SIZE_PLMNIDENTITY];
  UINT8      lac[SIZE_LAC];
  UINT8      ci[SIZE_CI];
  UINT8      rac[SIZE_RAC];
} Cgi;

typedef struct u_TargetId
{
  TargetIdType  targetIdType;
  union {
     TargetEnbId targetEnbID;
     TargetRncId targetRncId;
     Cgi         cgi;
  } val;
} TargetId;

typedef struct s_UtranCellId
{
  UINT8          plmnIdentity[SIZE_PLMNIDENTITY];
  UINT32         cellId;   /* Range 0..268435455 */
} UtranCellId;

typedef struct s_LastVisitedEutranCellInfo
{
  EUtranCgi      eUtranCgi;  /* Global Cell ID */
  CellSize       cellSize;
  UINT16         timeUeStayedInCell; /* Range 0 sec .. 4095 sec. */
} LastVisitedEutranCellInfo;

typedef struct s_LastVisitedUtranCellInfo
{
  UtranCellId           utranCellId;
  UtranCellType         utrancellType;
  UINT16                timeUeStayedInCell; /* Range 0 sec .. 4095 sec. */
} LastVisitedUtranCellInfo;

typedef struct s_LastVisitedCellInfo
{
  LastVisitedCellType  lastVisitedCellType;
  union {
     LastVisitedEutranCellInfo lastVisitedEutranCellInfo;
     LastVisitedUtranCellInfo  lastVisitedUtranCellInfo;
     /*LastVisitedGeranCellInfo  lastVisitedGeranCellInfo;*/ /* Not yet defined by spec */
  } val;
} LastVisitedCellInfo;

typedef struct s_LastVisitedCellList
{
  UINT8                 numOfCells;
  LastVisitedCellInfo   lastVisitedCellInfo[MAX_NUM_OF_CELLS];
} LastVisitedCellList;

typedef struct s_UeHistoryInfo
{
  LastVisitedCellList   lastVisitedCellList;
} UeHistoryInfo;

typedef struct s_ERabsSubjectToFwdItem
{
  ERabId                eRabId;
  TBOOL                 isDlTransportLayerAddrUsed;
  TransportLayerAddr    dlTransportLayerAddr;
  TBOOL                 isDlGtPTeIdUsed;
  GtPTeId               dlGtPTeId;
  TBOOL                 isUlTransportLayerAddrUsed;
  TransportLayerAddr    ulTransportLayerAddr;
  TBOOL                 isUlGtPTeIdUsed;
  GtPTeId               ulGtPTeId;
} ERabsSubjectToFwdItem;

typedef struct s_ERabsSubjectToFwdList
{
  UINT16                  noComp;
  ERabsSubjectToFwdItem   eRabSubToFwdItem[MAX_ERAB_ITEMS_IN_LIST];
} ERabsSubjectToFwdList;

typedef struct s_ErabsInfoItem
{
  ERabId         eRabId;
  TBOOL          isDlFwdingListPresent;
  DlFwdingList   dlFwdingList;
} ErabsInfoItem;

typedef struct s_ErabsInfoList
{
  UINT8           numOfErabs;
  ErabsInfoItem   erabsInfoItem[MAX_ERAB_ITEMS_INFO_EACH_UE];
} ErabsInfoList;

typedef struct s_RrcContainerHandoverPrepInfo
{
  UINT16  msgLen;
  UINT8   handoverPrepInfoMsgPdu[MAX_HANDOVER_PREP_INFO_MSG];
} RrcContainerHandoverPrepInfo;

typedef struct s_RrcContainerHandoverCmdMsg
{
  /* TODO Verify from 36.331 Sect 10.2.2 */
  UINT16  cmdMsgLen;
  UINT8   handoverCmdMsgPdu[MAX_HANDOVER_CMD_MSG];
} RrcContainerHandoverCmdMsg;

typedef struct s_SrcEnbToTargetEnbTransparentCont
{
  RrcContainerHandoverPrepInfo  rrcContainer;
  EUtranCgi                     targetCellId;
  TBOOL                         isSubProfileIdOrFreqPriorityPresent;
  UINT16                        subProfileIdOrFreqPriority;  /* range : 1.. 256 */
  TBOOL                         isErabsInfoListPresent;
  ErabsInfoList                 erabsInfoList;
  UeHistoryInfo                 ueHistoryInfo;
} SrcEnbToTargetEnbTransparentCont;

typedef struct s_TargetEnbToSrcEnbTransparentCont
{
  RrcContainerHandoverCmdMsg   rrcContainerHandoverCmdMsg;
} TargetEnbToSrcEnbTransparentCont;

typedef union u_SrcToTargetTransparentContainer
{
  SrcEnbToTargetEnbTransparentCont   srcEnbToTargetEnb;
  /* Not supported yet - to add when required ---
  SrcRncToTargetRncTransparentCont   srcRncToTargetRnc;
  SrcBssToTargetBssTransparentCont   srcBssToTargetBss;
  OldBssToNewBssInfo                 oldBssToNewBssInfo;
  SrcBssToTargetBssTransparentContOldBssToNewBssInfo  srcBssToTargetBssOldBssToNewBssInfo; */
} SrcToTargetTransparentContainer;

typedef struct s_TargetToSrcTransparentContainer
{
  TargetEnbToSrcEnbTransparentCont   targetEnbToSrcEnb;
  /* Not supported yet - to add when required ---
  TargetRncToTargetRncTransparentCont         targetRncToSrcRnc;
  TargetBssToTargetBssTransparentCont         targetBssToSrcBss;
  Layer3Info                                  layer3Info;
  TargetBssToSrcBssTransparentContLayer3Info  targetBssToSrcBssTransparentContLayer3Info; */
} TargetToSrcTransparentContainer;

typedef struct s_SrcToTgtTprntCont
{
  UINT32   msgLen;
  UINT8    msgPdu[MAX_SIZE_SRC_TO_TGT_TPRNT_CONT];
} SrcToTgtTprntCont;

typedef struct s_TgtToSrcTprntCont
{
  UINT32   msgLen;
  UINT8    msgPdu[MAX_SIZE_TGT_TO_SRC_TPRNT_CONT];
} TgtToSrcTprntCont;

/* Handover Required Message */
typedef struct s_HandoverRequired
{
  MmeUeS1apId                     mmeUeS1apId;
  EnbUeS1apId                     enbUeS1apId;
  HandoverType                    handoverType;
  S1Cause                         cause;
  TargetId                        targetId;
  TBOOL                           isDirectFwdPathAvailabilityUsed;
  DirectFwdPathAvailability       directFwdPathAvailability;
  TBOOL                           isSrvccHoIndicationUsed;
  SrvccHoIndication               srvccHoIndication;
  SrcToTgtTprntCont               srcToTgtTprntCont;
  TBOOL                           isSrcToTgtTprntContSecondary;
  SrcToTgtTprntCont               srcToTgtTprntContSecondary;
  TBOOL                           isMsClassmark2Used;
  UINT8                           msClassmark2[MAX_SIZE_MS_CLASSMARK_2];
  TBOOL                           isMsClassmark3Used;
  UINT8                           msClassmark3[MAX_SIZE_MS_CLASSMARK_3];
} HandoverRequired;

/* Handover Command Message */
typedef struct s_HandoverCmd
{
  MmeUeS1apId                         mmeUeS1apId;
  EnbUeS1apId                         enbUeS1apId;
  HandoverType                        handoverType;
  UINT8                               nasSecurityParamsFromEUtran[SIZE_NAS_SECURITY_PARAMS_FROM_EUTRAN];
  TBOOL                               isERabsSubjectToFwdListUsed;
  ERabsSubjectToFwdList               eRabsSubjectToFwdList;
  TBOOL                               isERabsToReleaseListUsed;
  ERabReleaseList                     eRabsToReleaseList;
  TgtToSrcTprntCont                   tgtToSrcTprntCont;
  TBOOL                               isTgtToSrcTprntContSecondary;
  TgtToSrcTprntCont                   tgtToSrcTprntContSecondary;
  TBOOL                               isCriticalDiagnosticUsed;
  CriticalDiagnostic                  criticalDiagnostic;
}HandoverCmd;

/* Handover Preparation Failed Message */
typedef struct s_HandoverPrepFailed
{
  MmeUeS1apId             mmeUeS1apId;
  EnbUeS1apId             enbUeS1apId;
  S1Cause                 cause;
  TBOOL                   isCriticalDiagnosticUsed;
  CriticalDiagnostic      criticalDiagnostic;
} HandoverPrepFailed;

#endif /* S1AP_CM_H */
