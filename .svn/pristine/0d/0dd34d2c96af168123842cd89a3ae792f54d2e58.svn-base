#ifndef UEH_GLOBAL_DEF_H
#define UEH_GLOBAL_DEF_H


#include "ccpu_cmn.h"
//#include "cm5.x"
#include "ludh.h"

#include "bsm_s1ap_if.h"
#include "../../s1mgr/s1ap_cm.h"
#include "bsm_ueh_if.h"
#include "ueh_pucch_res_mgmt.h"
#include "ueh_srs_res_mgmt.h"
#ifdef LTE_L2_MEAS
#include "lrg.x"
#include "ueh_pmh_if.h"
#endif 
#include "ueh_cfg.h"
#include "ueh_alias.h"
//#include "s1ap_mgr.h"
//#include "s1ap_sm.h"
#ifdef RRM_PRL_FLAG
#include "trace.h"
#endif

/*CELL Specific events*/

typedef enum e_UehCellEvent {
  CELL_EVT_OP_STOP        =     0,
  CELL_EVT_S1_LINK_DOWN   =     1,
  CELL_EVT_S1_RESET       =     2,
  CELL_EVT_OP_START       =     3,
  CELL_EVT_S1_LINK_UP     =     4,
  CELL_EVT_OP_STOP_TMR_EXPIRY = 5,
  CELL_EVT_UE_CNT_ZERO    =     6,
  CELL_EVT_PAGING         =     7,
  CELL_MAX_EVT
} UehCellEvent;


/* Cell procedues */
typedef enum e_UehCellProc
{
  UEH_CELL_PROC_NONE                   =  0,
  UEH_CELL_PROC_OP_STOP_ONGOING        =  1,
  UEH_CELL_PROC_S1_LINK_DOWN_ONGOING   =  2,
  UEH_CELL_PROC_S1_RESET_ONGOING       =  3,
  UEH_CELL_PROC_MAX
} UehCellProc;



typedef enum e_UehRrcProcedure 
{
  UEH_PROC_RRC_CON_SETUP                 =0,
  UEH_PROC_RRC_RECONF                    =1, 
  UEH_PROC_RRC_CON_REEST                 =2,
  UEH_PROC_RRC_CON_REL                   =3,
} UehRrcProcedure;

typedef enum e_UehS1apProcedure 
{
  UEH_PROC_S1AP_NO_PROC                   =     0,
  UEH_PROC_S1AP_ERAB_SETUP                =     1,
  UEH_PROC_S1AP_ERAB_MOD                  =     2,
  UEH_PROC_S1AP_ERAB_REL                  =     3,
  UEH_PROC_S1AP_INTI_CXT_SETUP            =     4,
  UEH_PROC_S1AP_S1_SETUP                  =     5,
  UEH_PROC_S1AP_UE_CXT_REL                =     6,
  UEH_PROC_S1AP_UE_CXT_MOD                =     7,
  UEH_PROC_S1AP_ENB_CONFIG_UPDATE         =     8,
  UEH_PROC_S1AP_MME_CONFIG_UPDATE         =     9,
  UEH_PROC_S1AP_UECAP_INFO_IND            =     10,
  UEH_PROC_S1AP_ERR_IND                   =     11,
  UEH_PROC_S1AP_HAND_PREPARATION          =     12,
  UEH_PROC_S1AP_HAND_CANCLE               =     13,
   
    
} UehS1apProcedure;

typedef enum e_UehInternalProcedure 
{
  UEH_IN_PROC_NONE    = 0,
  UEH_IN_PROC_ADD     = 1, /* same as configure */
  UEH_IN_PROC_RECONF  = 2,
  UEH_IN_PROC_DEL     = 3,
  UEH_IN_PROC_ENABLE  = 4,
  UEH_IN_PROC_DISABLE = 5,
  UEH_IN_PROC_MAX
} UehInternalProcedure;

typedef enum e_UehUeProcedure
{
  UEH_NO_PROCEDURE                = 0,
  UEH_RRC_CON_SETUP               = 1,
  UEH_INITIAL_CXT_SETUP           = 2,          
  UEH_UE_CXT_REL                  = 3,                 
  UEH_ERAB_SETUP                  = 4,                
  UEH_ERAB_REL                    = 5,               
  UEH_MEAS_CFG                    = 6,             
  UEH_OUT_HO_ONGOING              = 7,             
  UEH_IN_HO_ONGOING               = 8,  
  UEH_REEST_ONGOING               = 9,
  UEH_UE_CXT_REL_DUE_TO_RL_FAIL   = 10,  
  UEH_UE_CONN_REL_ONGOING         = 11, 
  UEH_UE_CTXT_CLEAR_ONGOING       = 12,         
  UEH_INITIAL_CXT_SETUP_FAILURE   = 13,   
  UEH_UE_REL_REQ_SENT             = 14, 
  UEH_UNUSED_PROC                 = 15,
  UEH_PUCCH_PWR_REC_ONGOING       = 16,
  UEH_INITIAL_UE_MSG              = 17
} UehUeProcedure;


typedef enum  {
    UEH_SM_STATE_INIT = 0,
    UEH_SM_STATE_TUCL_CFG_DONE,
    UEH_SM_STATE_SCTP_CFG_DONE,
    UEH_SM_STATE_S1AP_CFG_DONE,
    UEH_SM_STATE_EGTP_CFG_DONE,
    UEH_SM_STATE_UDH_CFG_DONE,
    UEH_SM_STATE_APP_CFG_DONE,
    UEH_SM_STATE_RRC_CFG_DONE,
    UEH_SM_STATE_PDCP_CFG_DONE,
    UEH_SM_STATE_RLC_CFG_DONE,
    UEH_SM_STATE_MAC_CFG_DONE,
    UEH_SM_STATE_CL_CFG_DONE,
    UEH_SM_STATE_S1AP_STK_BND_DONE,
    UEH_SM_STATE_EGTP_STK_BND_DONE,
    UEH_SM_STATE_RRC_STK_BND_DONE,
    UEH_SM_STATE_ENB_DEBUGS_DONE,
    UEH_SM_STATE_ENB_ALRMS_DONE,
    UEH_SM_STATE_AWAIT_S1_CON,
    UEH_SM_STATE_AWAIT_CELL_UP,
    UEH_SM_STATE_CELL_UP,
    UEH_SM_BSM_CELL_CONFIG_REQ,
    UEH_SM_BSM_ENB_CONFIG_REQ
}UehSmState;

typedef struct s_Bw2RbMapping 
{
   DlBandwidth   oamBw;
   EnumNhuSysInfoBlockTyp2freqInfoul_Bandwidth rrcSpecRbs;
   NhuSysBw      nhurgrRbs;
   CtfBandwidth  ctfRbs;
} Bw2RbMapping;


typedef struct s_ueCbCtxt
{

int localRrc        :1;
int localS1ap       :1;
int localSctp       :1;
int localRlc        :1;
int localPdcp       :1;
int localMac        :1;
int localGtp        :1;

int peerRrc         :1;
int peerS1ap        :1;
int peerSctp        :1;
int peerRlc         :1;
int peerPdcp        :1;
int peerMac         :1;
int peerGtp         :1;
} UeCbCtxt;

typedef struct s_pcmTimer
{
  CmTimer   cmTimer;
  U32       sentEvt;
  U32       expEvt;
  U8        isTmrRunning;
}PcmTimer;


typedef struct s_MsgBsmRrmCellConfigReq
{
  U32 cellConfig;	
}MsgBsmRrmCellConfigReq;

typedef struct _smCfgCb
{
   U16          cellId;           /* cell Id */
   U8           modType;          /* modulation type */
   U16          duplexMode;       /* Duplex Mode */
   U8           maxUeSupp;        /* max UE supported */
   TknU8        mcc[3];           /* mcc   */
   TknU8        mnc[3];           /* mnc  */
   U16          trackAreaCode;    /* Track area code */
   U8           freqBandInd;      /* Frequency Band Indicator */

   U32          enbIpAddr;        /* EnodeB IP address */
   U32          mmeIpAddr;        /* MME IP Address    */
   /* IP Address for UU Perf Pcap setup */    
   U32          dfltExGwIpAddr;   /* External GW IP Address */   
   U32          ueStartIpAddr;    /* Ue Appn Ip Addr */ 

   /* Patch ve001.101 */   
   U16          smDbgMsk;
   MsgBsmS1apConfigReq  s1apCfgInfo;   /* Configuration parameters for S1AP from OAM*/
   /* New structures which are added in enb_pre_config table */
   UehConfig     uehConfig;
   SctpConfig    sctpConfig;
   EgtpConfig    egtpConfig;
   S1apConfig    s1apConfig;
}SmCfgCb;

typedef struct _smCb
{
   TskInit         init;       /* Task Init info */
   UehSmState      smState;    /* state for state machine */
   SmCfgCb         cfgCb;      /* Configurations read from the file */
   U16             enbTransId; /*store the BSM initiated enodeB config transaction id*/
   U16             egtpTransId; /*store the BSM initiated EGTP config transaction id*/
   U16             sctpTransId; /*store the BSM initiated SCTP config transaction id*/
   U16             s1apTransId; /*store the BSM initiated S1AP config transaction id*/
}SmCb;


typedef enum e_UehErrorEventAction
{
  NO_ACTION       = 0, /* No action */
  REL_CTX         = 1, /* Release Context */
  RAISE_ALARM     = 2  /* Let OAM take action */
} UehErrorEventAction;

typedef struct s_UehUeCb UehUeCb;
typedef enum e_ObjType
{
  UE    =1,
  CELL,
  S1AP_CONNECTION,
  SAP,
  INST_OTHER
} ObjType;

typedef enum e_ProceId
{
  PROC_RRC_CONN_SETUP   = 1,
  PROC_RRC_SEC_MODE     = 2,
  PROC_RRC_CONN_RECONF  = 3           
} ProceId;  

/*UE Specific States*/
typedef enum  e_UeState
{
 UE_STATE_RRC_IDLE            =0,
 UE_STATE_RRC_CONNECTING      =1,
 UE_STATE_S1_CONNECTING       =2,
 UE_STATE_CONNECTED           =3,
 UE_STATE_SMC_ONGOING         =4,
 UE_STATE_UE_CAP_ONGOING      =5,
 UE_STATE_REC_ONGOING         =6,
 UE_STATE_CALL_CONNECTED      =7,
 UE_STATE_REL_ONGOING         =8,
 UE_STATE_HANDOVER_ONGOING    =9,
 UE_STATE_MSG_SEND_FAILED     =10,
 /* Add more states above this line */
 UE_MAX_STATE

} UeState;

typedef enum e_UeTimerDurVal
{
  UETMRDURVAL_MS50 = 50,
  UETMRDURVAL_MS100 =100,
  UETMRDURVAL_MS200 = 200,
  UETMRDURVAL_MS500 = 500,
  UETMRDURVAL_MS1000 = 1000,
  UETMRDURVAL_MS2000 = 2000,
  UERSPTIMER_MS5000 =5000
}UeTimerDurVal;

typedef MsgBsmUehEnbConfigReq  UehCfgCb;

typedef struct _bitRate
{
U32  dl; /* downlink */
U32  ul; /* uplink */
} UehBitRate;

/* SRB or DRB */
typedef struct _uehUeDrb
{
#ifdef FT_INACTIVITY_TMR
  U8           inUse;
#endif
  U8           rgrCfgMask;
  U8           rgrSubState;
  U8           isEgtpTnlEnbl;
  U8           isUdhRabEnbl;
  U8           rbType;
  U8           rbId;
  U32          eRABId;
  U8           pdcpId;
  U32          qci;            /* QoS Class Identifier  */
  UehBitRate   mbr;            /* Max Bit rate */
  UehBitRate   gbr;            /* Guaranteed Bit Rate */
  S32          remEgtpTunId;   /* remote tunnel id */
  S32          locEgtpTunId;   /* local tunnel Id */
  CmTptAddr    sgwAddr;        /* serving GW addr */
} UehUeRAB;

typedef struct _uehUeRABInfo
{
  U8           numOfSrbCfgd;
  UehUeRAB     srb[UEH_MAX_SRBS];                 /* SRB1 & SRB2 */
  U8           numOfRbCfgd;                       /* number of DRB in use max 8 [3..10] */
  UehUeRAB     drb[UEH_MAX_RABS_IN_UE];           /* DRBs */
  U8           numOfDrbAdd;                       /* number of DRBs for establishment */
  U8           drbAddLstIndx[UEH_MAX_RABS_IN_UE]; /* establishment ongoing DRBs list */
  U16          noNasPdus;
  NasPdu       nasPdu[UEH_MAX_RABS_IN_UE];        /* NAS PDU Received from MME */
} UehUeRABInfo;

typedef struct _uehUeSecCfg
{
  /* Algorithms supported by UE, This is a bitmap */
  UINT16  ciphAlgoSupported;
  UINT16  intgAlgoSupported;

  /* Only one algorithm is selected and saved, based one EnodeB Capability */
  UINT8  ciphAlgoSelected;
  UINT8  intgAlgoSelected;

  U8 cpCiphKey[UEH_CIPH_KEY_LEN]; /* Control Plane */
  U8 upCiphKey[UEH_CIPH_KEY_LEN]; /* User plane */
  U8 intgKey[UEH_INTG_KEY_LEN];   /* Integrity */
} UehUeSecCfg;

typedef struct _uehLiSapCb
{
  Pst                  pst;
  SuId                 suId;
  SpId                 spId;
  State                sapState;
  Mem                  mem;
  PcmTimer              timer;
  U8                   bndRetryCnt;
  U8                   maxBndRetry;
  TmrCfg               bndTmr;    
}UehLiSapCb;


typedef struct _uehMmeCb
{
  U16           mmeId;
  UehLiSapCb   *s1apSap;
  Bool         s1Setup;
  CmTptAddr    mmeAddr;
  U32          locEgtpTunCntr;
}UehMmeCb;


typedef struct s_UehConnState
{
  int localCtxReleased   :1;
  int localCtxEstInProg  :1;
  int localCtxEst        :1;
  int localCtxRelInProg  :1;

  int peerCtxReleased    :1;
  int peerCtxEstInProg   :1;
  int peerCtxEst         :1;
  int peerCtxRelInProg   :1;
} UehConnState;

typedef struct _uehS1ConCb
{
  UConnId      suConnId;
  UConnId      spConnId;
  U32          mme_ue_s1ap_id;
  U32          enb_ue_s1ap_id;
  UehConnState s1apConnState;
  U32          nmbERAB;
  UehUeCb      *ueCb;
  UehMmeCb     *mmeCb;
  U8            s1Cause;
  CauseType     s1CauseType;
} UehS1ConCb;

typedef struct _uehUeEutraCap
{
  U8            accessStratumRls;   /* Access Stratum Release */
  TBOOL         rgrUeCatMask;       /* To avoid sending of UE Capability multiple times to MAC */
  U8            ueCategory;         /* UE Category */ 
   /*TODO: PDCP params needs to be added */
  U8            isFeatureGrpInd;    /* presence of  Feature Group Indicators */
  U8            featureGrpInd[4];     /* Feature Group Indicators */
}UehUeEutraCap;

typedef struct _sTmsi
{
  U8 mmeCode;
  U32 mTmsi;
}UehSTmsi;

typedef struct s_UeGummei
{
  U8 mmeId;
  U8 mmeGrpId[UEH_MMEGRP_ID_SIZE];
  U8 mcc[UEH_MCC_SIZE];
  U8 mnc[UEH_MNC_SIZE];
}UeGummei;



typedef struct s_UecmSaveQNode {
  int event;
  void *cb;
  void *sdu;
}UecmSaveQNode;

typedef struct _uehUeRadioCap
{
  U8                   isUeRadAccCap;  /* Indicates the presence of UE Radio Access Capability */
  UeRadioCapability    ueRadAccCap;    /* UE Radio Access Capability */
  U8                   isUeCapInfo;    /* Indicates the presence of UE Capability Information */
  UeRadioCapInfo       ueCapInfo;      /* UE Capability Information */
}UehUeRadioCap;

typedef struct _uehUeCapRatCntr
{
  U8                   ratType;              /* RAT type EUTRAN */
  U8                   infoLen;              /* Info len */
  U8                   ueCapRatCntr[MAX_SIZE_UE_CAPBILITY_RAT_INFO];
}UehUeCapRatCntr;

/* Akash: SaveInput prototype - end */
/* Akash: Call Trace - start */


/* Akash: Call Trace - end */

typedef struct s_UehUeHovrInfo
{
  HandoverType                  handoverType;
  TargetIdType                  targetIdType;
  U8                            isRrcCtnrHovrPrepInfo; /* indicates presence of RRC container of Handover Prep Info */
  RrcContainerHandoverPrepInfo  rrcCtnrHovrPrepInfo;
} UehUeHovrInfo;



typedef U32   UehEnbUeInternalId;

typedef struct s_UehCtxHang
{
  UeState                prevUeState;
  TBOOL                  isLastStateAuditPassed;
  U8                     prevUeProcedure;         
  TBOOL                  isLastProcedureAuditPassed;
} UehCtxHang;

typedef struct s_UeEventCounterTable{
  U32                   ueEventCounter[MAX_UE_SUPP][EVENTTYPE_OTHER+1];  
}UeEventCounterTable;

UehCtxHang uehCtxHangList[MAX_UE_SUPP];
struct s_UehUeCb
{
  U8                     isUeCbAllocated;
  UehEnbUeInternalId     enbUeInternalId;
  U8                     ueCat;                       /* Ue category */
  U8                     transId;                     /* trans identifier */
  U16                    crnti;                       /* C-RNTI */
  CmLteCellId            cellId;
  UeState                ueState;
  U32                    establishCause;
  U8                     ueRgrCfgState;
  UehBitRate             aggMaxBr;                    /* UE Aggregate Maximum Bit Rate */
  UehUeRABInfo           rbInfo;                      /* RAB Info (Both SRB & Data RBs) */

  U16                    pmiCfgIdx;                   /* To be removed acParams.cqipmiriInfo.cqipmiConfigIndex will be used instead - PWAV */
  U16                    srCfgIdx;                    /* To be removed acParams.srInfo.srConfigIndex will be used instead - PWAV */

  UehUeSecCfg             secCfg;                      /* Security related info */
  UehS1ConCb              s1ConCb;                    /* Add any S1AP/GTP related info */
  SuId                    sapId;
  TBOOL                   isSTmsi;
  UehSTmsi                sTmsi;
  STimsi                  s1Stmsi;
  U16                     ueId;                        /*NH Identifier*/

  AcParams                acParams;                    /* Admission Control Parameters for PUCCH_RESOURCE_MGMT */ /* TODO Check Inilization values */

  PcmTimer                 ueRspTmr;
  PcmTimer                 crntiRtnTmr;
  PcmTimer                 ueCtxRelTmr;
  PcmTimer                 ueSrb1Tmr;
  PcmTimer                 ueRrcConRelL2Ack;
  PcmTimer                 mmeRelIndTmr;
  PcmTimer                 ueInternalConfTmr;
  PcmTimer                 uePwrReconfTmr;
  PcmTimer                 ueCqiBasedRlfDetectionTmr;
  S8                      uecmSaveQNextFreeIdx;        /* SaveQ support */
  UecmSaveQNode           uecmSaveQ[SAVEQLEN];         /* SaveQ support */
  U8                      isCtfConfigured;             /*This variable is set when the Phy configuration success */
  UehUeProcedure          ueProcedure;                 /* Current HIGH LEVEL ongoing procedure */
  UehUeRadioCap           ueRadCap;                    /* UE Radio Capability */
  UehUeCapRatCntr         ueCapRatCntr;                /* UE Capability RAT Container */
  UehUeEutraCap           ueEutraCap;                  /* UE E-UTRAN Capability Information */
  U8                      numOfTunnelCreReq;           /* ve001.102: Number of Tunnels created */
  UINT8                   ueEntryTime[MAX_SIZE_ACTIVEUESTATUS_ENTRYTIME + 1 ];
  UeAccessStratumRelease  ueAccessStratumRelease;
  ActiveIntProtAlgo       activeIntProtAlgo;
  UINT32                  numERabs;
  UeSrsInfo               ueSrsInfo;
  U8                      selectedPlmnInfo;
  U32                     ueReleaseCauseVal;           /* UE Release Cause value*/
  RabReleaseCause         rabReleaseCause;             /* UE RAB Release cause */
    /* S1AP Handover related information */
  PcmTimer                 ueS1apHandOverReqdRsp;
  UehUeHovrInfo           ueHovrInfo;
  
  U8 rlsOnReEstbl;


  S32                     poUePucch; /* to toggle UE power */
  U8                      reCfgProcComp;
  UeCbCtxt     isAllocatedFor;
  UeCbCtxt     isFreedFor;
  };

typedef struct s_UehPagingRecord
{
  CmLList           lnk;
  U8                i_s;
  UeIdChoice        choice;
  union
  {
     S1Imsi   imsi;
     S1STmsi  sTmsi;
  } val;
  U8 cnDomain;
} UehPagingRecord;



typedef struct s_UehPagingRecordInfo
{
  U8           noOfRecInGroup[UEH_MAX_SFN_PAGING][UEH_MAX_I_S];
  CmLListCp    uehPagingRecordInfo[UEH_MAX_SFN_PAGING];
} UehPagingRecordInfo;



typedef struct _uehSysInfoRel
{
   /* PLMN Id */
   TknU8 mcc[3];
   TknU8 mnc[3]; /* Token since size could be of 2 to 3 */
   /* Tracking Area Code */
   U16 trackAreaCode;
   /* Frequency Band Indicator */
   U8 freqBandInd;
} UehSysInfoRel;

typedef struct _uehSiSchInfo
{
  U16                 siPrdcty;         /* SI Periodicity */
  U8                  sibMapInfo;       /* SIB mapped to SI for transmission based on their periodicity */
  U8                  siSchGap;         /* Gap interms of FLOOR(X/10) */ 
  U8                  siSchSubframe;    /* SI Schedule Subframe */ 
} UehSiSchInfo;

// RRM Changes
typedef struct _uehRrmCellCounters
{
  U32                 numRlFailures;             /* Total number of radio link failures */
  U32                 numRrcConRejected;         /* Total number of RRC connection rejects */
  U32                 numIncomingHoSuccessful;   /* Total number of successful incoming HandOver*/
  U32                 numIncomingHoFailed;       /* Total number of Unsuccessful incoming HandOver*/
  U32                 numIncomingHoRejected;     /* Total number of Rejected incoming HandOver*/
  U32                 numS1apSetupReq;           /* Total number of S1AP setup requests*/
  U32                 numS1apSetupRsp;           /* Total number of S1AP setup responses */
  U32                 numS1apRejected;           /* Total number of S1AP connection rejects */
  U32                 numS1apResetTx;            /* Total number of S1AP reset transmissions */
  U32                 numS1apResetRx;            /* Total number of S1AP reset receives */
  

}UehRrmCellCounters;

typedef enum e_UehTimerId
{
  UEH_MIN_TIMER_ID         = 0, 
  UEH_TMR_NHU_SAP_BND, 
  UEH_TMR_RGR_SAP_BND,
  UEH_TMR_CTF_SAP_BND,             
  UEH_TMR_SZT_SAP_BND,             
  UEH_TMR_EGT_SAP_BND,             
  UEH_TMR_PJU_SAP_BND,             
  UEH_TMR_S1AP_SETUP,              
  UEH_TMR_S1AP_PROC_GUARD,         
  UE_TMR_UE_RESPONSE,              
  UE_TMR_CRNTI_RETENTION,          
  UE_TMR_UE_CTX_REL,
  UE_TMR_UE_SRB1,                  
  UE_TMR_RRCCONNRELL2ACK,          
  UE_TMR_OP_STOP_REQ,              
  UEH_TMR_UDH_SAP_BND,             
#ifdef FT_INACTIVITY_TMR
  UEH_TMR_INACTIVITY,  
#endif
  UEH_TMR_S1AP_TS1_RELOC_PREP,     
  UEH_TMR_WAIT_MME_REL,            
  UE_TMR_INTERNAL_CONF,             
  UE_PWR_RECONF_TMR,               
  UEH_L2M_SAMPLE_PROFILE1,        
  UEH_L2M_SAMPLE_PROFILE2,
  UEH_PUCCH_AUDIT_TMR,
  UEH_UESTATE_AUDIT_TMR,
  UEH_UEPROCEDURE_AUDIT_TMR, 
  UEH_CQI_BASED_RLF_DETECTION_TMR,
  UEH_MAX_TIMER_ID                                                
}UehTimerId;


typedef struct _uehFailureProc
{
  EventType ueFailureEvt;
  UINT8     ueProcStatus;
  UINT32    reConfCompCount;
  UINT32    reConfCount;
}UehEvtInfo;

#ifdef LTE_L2_MEAS

typedef enum _uehProfileStatus
{
PROFILE_STOPPED = 0,
PROFILE_RUNNING
}UehProfileStatus;  

typedef struct _counterLink
{
    U32 identity;
    void *addr;
}CounterLink; 

typedef struct _uehL2MeasProfileCfg
{
  ProfileId          profileId;
  UehProfileStatus   status;
  U32                numSamples;
  SamplingInterval   samplingIntervalDur;
  PcmTimer            samplingIntervalTmr;
  U8                 numL2Counters;
  CounterLink        counterLink[MAX_NUM_COUNTERS];
  U32                mask;
  U32                sampleSeqNo;
}UehL2MeasProfileCfg;

typedef  struct _avgNumActvUeQciInfo
{
  U8         qciId;
  CounterId  counterId;
  U32        numActUe;
}AvgNumActvUeQciInfo;

typedef struct _uehAvgNumActvUeQci
{
  UehL2MeasProfileCfg   *profile;
  U32                    numSamples;
  U8                     numQci;
  AvgNumActvUeQciInfo    qciInfo[MAX_NUM_QCI];
}UehAvgNumActvUeQci;

typedef  struct _pktUuLossRateDlQciInfo
{
  U8         qciId;
  CounterId  counterId;
  U32        uuLossDl;
}PktUuLossRateDlQciInfo;

typedef struct _uehPktUuLossRateDlQci
{
  UehL2MeasProfileCfg    *profile;
  U32                     numSamples;
  U8                      numQci;
  PktUuLossRateDlQciInfo  qciInfo[MAX_NUM_QCI];
}UehPktUuLossRateDlQci;

typedef  struct _pktDelayDlQciInfo
{
  U8         qciId;
  CounterId  counterId;
  U32        dlDelay;
}PktDelayDlQciInfo;

typedef struct _uehPktDelayDlQci
{
  UehL2MeasProfileCfg    *profile;
  U32                     transId;
  U8                      numQci;
  PktDelayDlQciInfo       qciInfo[MAX_NUM_QCI];
}UehPktDelayDlQci;

typedef  struct _pktDiscRateDlQciInfo
{
  U8         qciId;
  CounterId  counterId;
  U32        dlDisc;
}PktDiscRateDlQciInfo;


typedef struct _uehPktDiscRateDlQci
{
  UehL2MeasProfileCfg    *profile;
  U32                     transId;
  U8                      numQci;
  PktDiscRateDlQciInfo    qciInfo[MAX_NUM_QCI];
}UehPktDiscRateDlQci;

typedef  struct _pktLossUlQciInfo
{
  U8         qciId;
  CounterId  counterId;
  U32        ulLoss; 
}PktLossUlQciInfo;

typedef struct _uehPktLossUlQci
{
  UehL2MeasProfileCfg    *profile;
  U32                     transId;
  U8                      numQci;
  PktLossUlQciInfo        qciInfo[MAX_NUM_QCI];
}UehPktLossUlQci;

typedef struct _uehTotalPrbUsage
{
  CounterId               counterId;
  UehL2MeasProfileCfg    *profile;
  U32                     prbPerc;
}UehTotalPrbUsage;

typedef struct _prbUsageQciInfo
{
  U8          qciId;
  CounterId   counterId;
  U32         prbPerc; 
}PrbUsageQciInfo;

typedef struct _uehPrbUsageQci
{
  UehL2MeasProfileCfg    *profile;
  U32                     transId;
  U8                      numQci;
  PrbUsageQciInfo         qciInfo[MAX_NUM_QCI];
}UehPrbUsageQci;

typedef struct _uehRecvRaPreambles
{
  U8                      flag;
  CounterId               dedPreCounterId;
  CounterId               randLowCounterId;
  CounterId               randHighCounterId;
  UehL2MeasProfileCfg    *profile;
  U32                     transId;
  U16                     dedPreambles;
  U16                     randSelPreLowRange; 
  U16                     randSelPreHighRange; 
}UehRecvRaPreambles;   

typedef struct _numActvUeQciInfo
{
  U8           qciId;
  CounterId    counterId;
  U32          numActvUe; 
}NumActvUeQciInfo;

typedef struct _uehNumActvUesQci
{
  UehL2MeasProfileCfg    *profile;
  U32                     transId;
  U32                     l2SamplePeriod;
  U8                      numQci;
  NumActvUeQciInfo        qciInfo[MAX_NUM_QCI];
}UehNumActvUesQci;

typedef struct _uehL2MeasCtx
{
  U8                      numProfiles;
  UehL2MeasProfileCfg     profileCfg[MAX_NUM_PROFILES];
  UehAvgNumActvUeQci      avgNumActvUeQci;
  UehPktUuLossRateDlQci   pktUuLossRateDlQci;   
  UehPktDelayDlQci        pktDelayDlQci;
  UehPktDiscRateDlQci     pktDiscRateDlQci;
  UehPktLossUlQci         pktLossUlQci;
  UehTotalPrbUsage        totalPrbUsageDl;  
  UehTotalPrbUsage        totalPrbUsageUl;
  UehPrbUsageQci          prbUsageQciDl;
  UehPrbUsageQci          prbUsageQciUl;
  UehRecvRaPreambles      recvRaPreambles;   
  UehNumActvUesQci        numActvUesQciDl;
  UehNumActvUesQci        numActvUesQciUl;
}UehL2MeasCtx;

#endif  /*LTE_L2_MEAS*/

typedef enum e_UehCellAvState 
{
  UEH_CELL_STATE_UNAVAILABLE = 0,
  UEH_CELL_STATE_AVAILABLE   = 1,
  UEH_CELL_MAX_STATE
} UehCellAvState;


// Done
typedef struct _uehCellCb
{
  /* State Variables */
  TBOOL               isS1LinkUp;
  TBOOL               isUnlocked;             
  TBOOL               isUnbarred;             
  TBOOL               isConfigured;             
  U32                 enbDisableReason;
  UehCellProc         proc;

  /* Context Variables */
  U16                 sfn;                    /* System Frame Number */
  PcmTimer            opStopTmr;
  UehUeCb             ueCbLst[MAX_UE_SUPP];              /* UE CallBlocks of this Cell */
  UehPagingRecordInfo uehPagingRecordInfo;
  U8                  ns;                     /* ns Value depends on nb(SIB2) received from operator */
  U8                  numOfSis;               /* Total number of SI messages sent to MAC Scheduler from UEH */
  UehSiSchInfo        siSchInfo[4];           /* SI scheduling Information */
  UINT16              modiPeriodTmr;          /* After Modification period new system inforamtion will be sent*/
  TBOOL               isSysInfoNeedToSend;    /* Will be set to TRUE when system information changed*/
  U32                 maxRntiPool;
#ifdef LTE_L2_MEAS
  UehL2MeasCtx        uehL2MeasCtx;           /* Profile and counter config for L2 Measurement */
#endif

  /* Counters */
  U32                 tickCount;
  U32                 numMibsTx;          /** Number of Mibs Tx */
  S16                 numUeCfg;           /* number of active Ues configured */
  UehRrmCellCounters  uehRrmCellCounters;

  /* Configuration */
  U32                 cellId;                 /* Cell Identity is of 28 bits long */
  U8                  modType;                /* Modulation Type BPSK / QPSK / 16QAM / 64QAM */
  U16                 duplexMode;             /* duplexing mode : TDD/FDD/HD_FDD */
  U8                  maxUeSupp;              /* maximum number of UEs supported by each eNodeB */
  UehSysInfoRel       sysRelInfo;             /* Data related to SysInfo  BroadCast for the respective cell */
  UeTimerDurVal       opStopTmrDur;
  UeTimerDurVal       ueRspTmrDur;
  UeTimerDurVal       crntiRtnTmrDur;
  UeTimerDurVal       ueCtxRelTmrDur;              /* Crnti retention timer to close the UE context in the MAC scheduler */
  UeTimerDurVal       ueSrb1TmrDur;
  UeTimerDurVal       ueRrcConRelL2AckDur;
  UeTimerDurVal       mmeRelIndTmrDur;
  UeTimerDurVal       ueInternalConfTmrDur;
  UeTimerDurVal       uePwrReconfTmrDurinMs;
  UeTimerDurVal       ueS1apHandOverReqdRspDur;
  UeTimerDurVal       uePucchAuditDur;
} UehCellCb;
/* NOTE: In UeEventHistory UE Event details must be changed to ENUM values */

/* PLMN Change*/
/* Added to support 2/3 digit MNC for PLMN ID*/
typedef struct _uehPlmnId
{
   UINT8               plmnMCC[UEH_MCC_SIZE]; 
   UINT8               numOfMNCDigits; 
   UINT8               plmnMNC[UEH_MNC_SIZE];
} UehPlmnId; 

/* 
 * ueList:
 * It is used to store last three UEs CRNTI values. These CRNTIs are used when user ask for log all events for all the UEs.
 * Then UEH will send all the events of last three UEs. UEH is sending only three UEs because if UE is properly attached then 
 * every UE will have atleast 35 to 40 events. Then totally we will have more than 100 events. Here we are having the limitation 
 * of 100 events to send to BSM. Thats the reason its value is three.
 *
 * ueListProc:
 * Is is used to store the last twenty UEs CRNTI values. For every UE we have total six procedures. Then for twenty UEs we have
 * more than 100 events.Thats the reason its value is twenty.
 */
typedef struct _uehRecentUes
{
  U16                     ueList[MAX_UE_SENT_TO_BSM];
  U16                     ueListProc[MAX_UE_PROC_SENT_TO_BSM];
}UehRecentUes;

typedef struct s_UehEventTab
{
  U8         n;
#define MAX_EVENTS_TO_LOG 5
  EventType  val[MAX_EVENTS_TO_LOG];
} UehEventTab;

#if 0
/** UE History Design
 *  There are 2 categories of events:
 *   a) Where each occurance of the event should be recorded
 *   b) Where only the last/latest occurance is to be recorded
 *
 *  This requirement is met as follows:
 *  1. An event counter shall be implemented (called ueEventCounterTable)
 *  2. This shall record event on per UE basis i.e.
 *     ueEventCounter[ueId][eventId] = numberOfTimersEventIdHasHappened;
 *  3. Event counting shall be done for all events.
 *  4. For events that are to be recorded with details, a cicular buffer shall be implemented.
 *     (called ueEventHistoryTable)
 *  5. In ueEventHistoryTable:
 *  a) Events will be recorded in the order of arrival (in time).
 *  b) Events will be stored in a single table (circular), without any sorting i.e. insert at the end.
 *  c) The size of the circular buffer shall be defined in ueh_cfg.h.
 *  d) Filters shall be applied on this table to provide the required details such as events for a specific UE.
 */

typedef struct s_UehEventInfo
{
  /**
   *  Example of ueEventCounter Table
   *  UEId | EVENT_RRCCONNREQ | EVENT_RRCCONNSETUP | EVENT_DLINFOTRF | EVENT_ULINFOTRF | ...
   *  102  |    1             |    1               |   3             | 3
   *  105  |    1             |    1               |   4             | 3
   *  107  |    1             |    1               |   1             | 2
   *           
   */
  U32              ueEventCounter[MAX_UE_SUPP][EVENTTYPE_OTHER+1];
  U32              cmnEventCounter[EVENTTYPE_OTHER+1];

  /* numEventsLogged is the number of events that have been written to the buffer since start. */
  /* if(numEventsLogged < MAX_UE_EVENTS_TO_STORE)
   *   It means the buffer has not been filled up yet.
   *   In that case, only o to numEventsLogged-1, indexes have valid events stored.
   *   The writeIndex points to an empty location.
   *  Otherwise:
   *   All events in the table are valid.
   *   The writeIndex points to the oldest one.
   *   Most N recent events are (writeIndex - 1) to (writeIndex -1) - N logically.
   */
  U32              numEventsLogged;
  /* writeIndex is the index of ueEventHistory array, where next element can be written */
  U32              writeIndex;
  UeEventHistory   ueEventHistory[MAX_UE_EVENTS_TO_STORE];
} UehEventInfo;

/** APIs
 *  UehUeRecordEvent() - will be called by applications to record an event.
 *    When called, first the counter table (ueEventCounter) will be updated.
 *    Then, it will be checked if the event is to be recorded in the history also.
 *    
 *  The UeEventCounters shall be cleared when RRCCONNREQ is received since it is the first event.
 *     clearBytes(&(ueEventCounter[ueId]), ((EVENTTYPE_OTHER+1) * U32 )
 * 
 * incrementUeEventCounter(ueId, eventId) - will increment the counter of eventId for ueId
 *      ueEventCounter[ueId][eventId]++;
 * 
 * logic for recording to History:
 *  If (ueEventCounter[ueId][eventId] > N), then do not record.
 *  For instance, if N is 2, then first 2 will be recorded.
 *  else addToUeEventHistory(event, data)
 * 
 *  N will be defined in ueh_cfg.h.
 *  It can be per event. For now, for simplicity, use a common value for all events.
 *  Default value can be 5 i.e. first 5 occurances will be recorded.
 *
 *  addToUeEventHistory(event, data) - will insert the event to ueEventHistory Table.
 *   Implementation: ueEventHistory[writeIndex] = {parameters derived from event and data};
 *                   writeIndex = (writeIndex + 1)%MAX_UE_EVENTS_TO_STORE;
 *
 *  clearUeEventHistory() - will clear the currently stored events.
 *   Implementation: numEventsLogged = 0, writeIndex = 0.
 *  
 *  Reponse to OAM Reqest for UE Event History
 *  Case 1: OAM requested last N events:  Fill most recent N events (using writeIndex).
 *  Case 2: OAM requested events of a specific UE (ueId):
 *     Search Table with for all events associated with ueId.
 *     Respond with these.
 *  Case 3: OAM requested only failure events:
 *     Search Table for all failure events, where failure-events are decided at compile time.
 *     Logic: Say EVENT_X and EVENT_Y are failure events.
 *        foreach event in ueEventHistory:
 *         if(ueEventHistory[event].eventType EQ EVENT_X || ueEventHistory[event].eventType EQ EVENT_Y)
 *            ...
 *     Provide those.
 *  Case 4: OAM requested only failure events of a specific UE:

 */
 
#endif

typedef struct s_uehUeHisTbl
{
  UeEventHistory          ueHis[MAX_ROWS_IN_UEEVENTHISTORY_TABLE];
  UINT32                  ueHisCurrIdx;
  UINT32                  totalEvtInUeEvtHisTbl;
} UehUeHisTbl;

/*UEH Timer Description Table LIst*/
typedef struct _UehTimerDescTable
{
  
  U32 duration;
  U32 sentEvt;
  U32 expEvt;
  char name[MAX_TIMER_NAME_LEN];
}UehTimerDescTable;


typedef struct _UehCtxHangDetection
{
   /*UE Ctx Detection Variables and Timers*/
   UehEnbUeInternalId      ueCounter;
   PcmTimer                uehUeStateAuditTmr;
   PcmTimer                uehUeProcedureAuditTmr;
   PcmTimer                uehPucchAuditTmr;
   PcmTimer                pucchAuditTmr;
   PcmTimer                ueStateAuditTmr;
   PcmTimer                ueProcedureAuditTmr;
   U32                     pucchAuditCounter; 
   U32                     ueStateAuditCounter; 
   U32                     ueProcedureAuditCounter;
}UehCtxHangDetection;

typedef struct _uehCb
{
   /* memory pool info */
   Mem                     mem;
   TskInit                 init;
   CmTqCp                  tqCp;                  /* Timing Queue Control point */
   CmTqType                tq[UEH_NUM_TQENTRY];   /* Timing Queue for a resln. */
   Header                  lmHdr;

   U8                      cfgValid;              /*0 if the configuration below is invalid, 1 if it is valid*/
   UehCfgCb                uehCfgCb;              /*Configuration received from BSM */ 
   UehCfgCb                modiUehCfgCb;          /*operator modified parameters in ENB unlock state will be stored here, Every modification period
                                                       content of this paramters are copied in to uehCfgCb*/
   UConnId                 s1apConId;             /*Last used connection id; counter*/
   U8                      systemOperational;     /*This variable is set when ueh can start sending system information*/
   UehCellCb               cellCb[1];
   UehMmeCb                mmeCb;
   UehUeCb                 *s1UeConLst [MAX_UE_SUPP];
   UehLiSapCb              **nhuSap;
   UehLiSapCb              **ctfSap;
   UehLiSapCb              **rgrSap;
   UehLiSapCb              **sztSap;
   UehLiSapCb              **udhSap;
#ifdef RRM_PRL_FLAG
   Buff                    buff;
#endif
   CmTptAddr               datAppAddr;
   U32                     nhuTransId;
   S8                      callTrcBuf[CALLTRCBUFLEN]; /* Akash: Call Trace */
   UehUeCb                 *locTeIdToUeCb[UEH_MAX_TUNNEL_ID];
   UeEventHistoryLogConfig ueEventHistoryLogConfig; /* UE Event History Status table */
   UehUeHisTbl             uehUeHisTbl;
   UehPlmnId               uehPlmnId;
   UehTimerDescTable       uehTimerDescTable[UEH_MAX_TIMER_ID];/*UEH Timers Description Table */
   /** ueCounter has range 1..MAX_INTERNAL_UEID.
    *  Value 0 is reserved for special purpose.
    *  This counter is incremented whenever a UE CB is allocated.
    *  enbUeInternalId = ueCounter at time of UE CB allocation.
    *  This counter is never decremented.
    *  It wraps around to 1 on reaching MAX_INTERNAL_UEID.
    *  The purpose is to identify a UE Context with a number that persists even after context is cleared.
    *  
    */  
   UehEnbUeInternalId      ueCounter;
   /*UE Ctx Detection Structure*/
   UehCtxHangDetection     uehCtxHangDetection;
   RgrTtiIndInfo           rgrTtiIndInfo; /* TTI Indication is unpacked into this */
   UehEventTab             eventTab; 
   UINT32                  uehCounters[EVENTTYPE_OTHER+1];
   SINT32                  lastRcvdSfn;
} UehCb;






typedef struct _UehRrcCtxt
{
   U8 val;
   struct _UehRrcCtxt *next;
}UehRrcCtxt;

typedef struct _ueRgrCfgCfm
{
  U8 lchId;
  U8 status;
  U16 cfgType;
} UeRgrCfgCfm;

typedef struct _ueCtfCfgCfm
{
  U8  crnti; 
  U8  status;
  U8  cfgType;
}UeCtfCfgCfm;
typedef struct _uehEgtpTunCb UehEgtpTunCb;

typedef struct _uehPdcpCb
{
  CmLtePdcpId  pdcpId;
  U32          locTeId;
  UehEgtpTunCb *egtpTunCb;
  UehLiSapCb   *liSap;
  U32          sduIdCntr;
}UehPdcpCb;

struct _uehEgtpTunCb
{
  U32           locTeId;
  U32           remTeid;
  CmTptAddr     dstIpAddr;                   /**< Destination IP Address */
  CmTptAddr     frmDstIpAddr;                /**< From Destination IP Address */
  CmLtePdcpId   pdcpId;
  U8            state;
  UehPdcpCb     *pdcpCb;
  UehLiSapCb    *liSap;
};


typedef struct _uehEuCb
{
   /* memory pool info */
  Mem                  mem;
  TskInit              init;
  CmTqCp               tqCp;           /* Timing Queue Control point    */
  CmTqType             tq[UEH_NUM_TQENTRY]; /* Timing Queue for a resln. */
  Header               lmHdr;
  CmTptAddr            srcAddr;

  UehLiSapCb           **lsapLst;
  UehEgtpTunCb         **egtpTunLst;
  UehPdcpCb            **pdcpIdLst;
#ifdef UEH_PERF_MEAS 
  CmTimer              loadTimers[1];   /* Load is generator timer : UEH_PERF_MAX_TMR */ 
  CmLtePdcpId          ueLst[MAX_RNTI_POOL];  /* UEH_MAX_UE  Max no of UEs */ 
  U32                  sduCntr; 
#endif
} UehEuCb;



typedef struct _uehFreqBand
{
  S8     EutraOprBand;
  S32    FreqDlLow;
  S32    FreqOffsetDl;
  S32    FreqRangeDl;
  S32    FreqUlLow;
  S32    FreqOffsetUl;
  S32    FreqRangeUl;
} UehFreqBand;




#endif
