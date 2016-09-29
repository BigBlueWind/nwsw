/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/********************************************************************20**

     Name:     EnodeB Application

     Type:     C source file

     Desc:     Data structures  

     File:     ve.x

     Sid:      ve.x@@/main/2 - Wed Dec  1 08:44:21 2010

     Prg:      rbhat

*********************************************************************21*/


#ifndef __VEX__
#define __VEX__

#include <stdarg.h> /* TODO : temporary kept here */


#ifdef __cplusplus
extern "C" {
#endif



#ifdef VE_RELAY

typedef struct _veUeCb VeUeCb;

typedef struct _bitRate
{
   U32 dl; /* downlink */
   U32 ul; /* uplink */
} VeBitRate;

/* E-RAB Modify */
typedef struct veRabInfo
{                   
   Bool            rabInfoModified;
   VeBitRate       mbr;             /* vsk  Max Bit rate */
   VeBitRate       gbr;             /* Gauranteed Bit Rate */
   U32             qci;             /* QoS Class Identifier  */
} VeRabInfo;
/* E-RAB Modify End */
typedef struct _veUeRAB
{
   /* SRB or DRB */
   U8              inUse; 
   U8              rbType;/* Type of Bearer.*/
   U8              rbDir;/* UL,DL and BI-DIR*/
   U8              rbMode;
   U8              rbId; /* Radio Bearer ID.*/
   U32             eRABId;/* E-RAB Id*/
   U8              pdcpId; 
   U32             qci;        /* QoS Class Identifier  */
   VeBitRate       mbr; /* Max Bit rate */
   VeBitRate       gbr; /* Gauranteed Bit Rate */
   SztGTP_TEID     remEgtpTunId; /* remote tunnel id */
   U32             locEgtpTunId;   /* local tunnel Id */
   CmTptAddr       sgwAddr; /* serving GW addr */
#ifdef LTE_HO_SUPPORT
   /* Indicates that Data forwarding is available or not for particular bearer.*/
   Bool            isDataFwdAvl;
   /* Data Forwarding tunnel info (used at s-enb) */
   SztGTP_TEID     remUlDataFwdTeId; /* UL remote tunnel id */ 
   SztGTP_TEID     remDlDataFwdTeId; /* DL remote tunnel id */
   CmTptAddr       datFwdPeerAddr; /* data forwarding peer entity addr */
   /* Data Forwarding tunnel info (used at t-enb) */
   U32             datFwdUlLocEgtpTunId; /* UL Local tunnel id */
   U32             datFwdDlLocEgtpTunId; /* DL Local tunnel id */
#endif
   /* E-RAB Modify */
   VeRabInfo       rabMdfyInfo;          /* Store the RABINFO when RAB info before commit */
} VeUeRAB;

typedef struct _veUeRABInfo
{
   U8          rgrCfgMask;
   /* number of SRBs in use max 2 */
   U8          numOfSrbCfgd;
   VeUeRAB     srb[VE_MAX_SRBS]; /* SRB1 & SRB2 */
   /* number of DRB in use max 8 [3..10] */
   U8          numOfRbCfgd;
   VeUeRAB     rab[VE_MAX_RABS_IN_UE]; /* DRBs */
   VeBitRate   aggBr;/* UE Aggregate Maximum Bit Rate */
   VeBitRate   tempAggBr;  
   U8          rabLst[VE_MAX_RABS_IN_UE];
   U8          rabMdfyFailLst[VE_MAX_RABS_IN_UE];

} VeUeRABInfo;

typedef struct _veUeSecCfg
{
   /* Ciphering Algorithm */
   U16 ciphAlgo;
   U8 cpCiphKey[VE_CIPH_KEY_LEN]; /* Control Plane */
   U8 upCiphKey[VE_CIPH_KEY_LEN]; /* User plane */
   /* Integrity Protection Algorithm */
   U16 intgAlgo;
   /* Integrity Key for Control plane. */
   U8 intgKey[VE_INTG_KEY_LEN];
} VeUeSecCfg;

/* Ue Capability Info */
#ifdef UE_RAD_CAP
typedef struct _veUeCapInfo
{
  NhuUE_EUTRA_Cap  eUTRA_Cap;      /* decocde eUTra Cap Info */
  TknStrOSXL       ueEutraOctStr;  /* eUtra cap Oct Str */  
} VeUeCapInfo;
#endif
typedef struct _veLiSapCb
{
   Pst                  pst;
   SuId                 suId;
   SpId                 spId;
   State                sapState;
   Mem                  mem;
   CmTimer              timer;
   U8                   bndRetryCnt;
   U8                   maxBndRetry;
   TmrCfg               bndTmr;    
}VeLiSapCb;

/* MCC and MNC are digits taking values 0 to 9. Each element represents */
/* a digit that takes the same range                                    */
typedef struct vePlmnId
{
   U8                        numMncDigits;
   U8                        mcc[3];
   U8                        mnc[3];
} VePlmnId;

typedef struct  veGummei
{

   Bool                      pres;
   VePlmnId                  plmnId;
   U16                       mmeGrpId;
   U8                        mmec;
}VeGummei;

typedef struct _veMmeCb
{
   U16               mmeId;
   U32               peerId;
   U32               suConnId;
   U32               spConnId;
   CmTimer           timer;
   VeLiSapCb         *s1apSap;
   U8                state;
   Bool              s1Setup;
   CmTptAddr         mmeAddr;
   U32               locEgtpTunCntr;
   U8                numPlmnIds;
   VePlmnId          plmnIds[VE_MAX_PLMNS_PER_MME];
   U16               numGrpIds;
   U16               groupIds[VE_MAX_GRPS_PER_MME];
   U16               numCodes;
   U8                codes[VE_MAX_CODES_PER_MME];
   U8                relCapacity;
   Bool              mmeOvldFlag;             /*!< MME Overload Flag */
   U32               mmeOvldAction;         /*!< MME Overload Action */
   CmTimer           s1SetupTmr;
   CmTimer           s1Reset;
   CmTimer           enbUpdTmr;
   U8                cfgUpdRtxCnt;
   U8                s1ResetRtxCnt;
   S1apPdu           *rstPdu;
#ifdef VE_TEST_CODE
   CmTimer          tstS1Rest;
   CmTimer          tstenbUpd;
#endif /* VE_TEST_CODE */
}VeMmeCb;

/* suConnId is same the index for MME. This allows efficient search.      */
typedef struct veMmeCont
{
   U8                       numMmes;
   VeMmeCb                  *mmes[VE_MAX_MMES];
} VeMmeCont;

typedef struct veS1ConId
{
   U8      type;       /* Type of connection id, either MME ID or eNB ID */
   U32     conId;         /* Connection Id */
}VeS1ConId;

#ifdef LTE_HO_SUPPORT
typedef struct _vex2ResetCb
{
  U8  causeType;  /* Cause Type */
  U32 causeVal;  /* cause value */ 
  U8  maxResetRetry;  /* Total number of ret */
} VeX2ResetCb;
/* HO States . mobCtrlStates */
typedef enum 
{
   VE_HO_NORMAL = 0,    /* Before initiation of HO.*/
   VE_HO_INITIATED,     /* After initiation of HO.*/
   VE_HO_IN_PROGRESS,   /* Handover is in Progress.*/
   VE_HO_UE_RELEASE,    /* Release the UE Context Once UE has been Handovered Successfully or timer expiry.*/
   VE_HO_MAX_STATES     /* Maximum of Handover state.*/
} VeHoState;

typedef enum 
{
   VE_EVNT_HO_DECISION,              /* Event of Handover Decision.*/
   VE_EVNT_HO_PREP_FAILURE,          /* Event of Handover Preparation failure.*/
   VE_EVNT_HO_PREP_TMR_EXPIRY,       /* Event of Handover Preparation timer expiry.*/
   VE_EVNT_HO_ACK_RCVD,              /* Event of Handover ack received.*/
   VE_EVNT_HO_UE_CNTXT_RLS,          /* Event of Handover UE context release.*/
   VE_EVNT_HO_OVERALL_TMR_EXPIRY,    /* Event of Handover overall timer expiry.*/
   VE_HO_SRC_MAX_EVENTS              /* Maximum num of events at Src eNB.*/
}VeSrcFsmEvnts;


typedef enum 
{
   VE_EVNT_HO_PREP_SUCCESS,          /* Event of Handover Preparation success.*/
   VE_EVNT_HO_ADM_FAILURE,           /* Event of Handover Admission Failure.*/
   VE_EVNT_HO_RECONFIG_CMPL_RCVD,    /* Event of Handover Reconfiguration complete.*/
   VE_EVNT_HO_PATH_SWTH_FAILED,      /* Event of Handover path switch request fail.*/
   VE_EVNT_HO_UE_ATTACH_TMR_EXPIRY,  /* Event of Handover UE Attach timer expiry.*/
   VE_EVNT_HO_CANCEL_RCVD,           /* Event of Handover Cancel received.*/
   VE_HO_TGT_MAX_EVENTS              /* Maximum num of events at target eNB.*/
}VeTgtFsmEvnts;
  
/* Bearer Direction.*/
typedef enum
{
   VE_UL = 0,
   VE_DL,
   VE_DL_UL
} VeEuRbDir;

/* Transmission Bandwidth - BW6 = Number of Resource Block 6 and so on */
typedef enum
{
   BW6 = 0,
   BW15,
   BW25,
   BW50,
   BW75,
   BW100
} VeTxBndWdth;

/* Sub Frame Assignment - Uplink-downlink subframe configuration
                          information */
typedef enum
{
   SA0 = 0,
   SA1,
   SA2,
   SA3,
   SA4,
   SA5,
   SA6
} VeSubFrameAssignment;

/* Cyclic Prefix */
typedef enum
{
   NORMAL = 0,
   EXTENDED
} VeCyclicPrefix;

/* Special Sub-Frame Pattern - SSP0 = spl sf pattern 0 and so on */
typedef enum
{
   SSP0 = 0,
   SSP1,
   SSP2,
   SSP3,
   SSP4,
   SSP5,
   SSP6,
   SSP7,
   SSP8
} VeSplSubFrmPattern;

/* Handover Type.*/
typedef enum
{
   VE_HO_TYPE_NONE,
   VE_HO_TYPE_S1,  
   VE_HO_TYPE_X2  
} VeHoType;

/* EnodeB Overload Flag Indication */

/* FDD Control block.*/
typedef struct _veCellFddCb
{
   U32 ulEarfcn;
   U32 dlEarfcn;
   VeTxBndWdth ulTxBndWdth;
   VeTxBndWdth dlTxBndWdth;
} VeCellFddCb;
/* special sub frame information.*/
typedef struct _veSplSubFrmInfo
{
   VeSplSubFrmPattern splSfPatrn;
   VeCyclicPrefix dlCycPrefix;
   VeCyclicPrefix ulCycPrefix;
} VeSplSubFrmInfo;
/* TDD Control block.*/
typedef struct _veCellTddCb
{
   U32 earfcn;
   VeTxBndWdth txBndWdth;
   VeSubFrameAssignment sfAssgnmnt;
   VeSplSubFrmInfo splSfInfo;
} VeCellTddCb;

typedef struct _veCellProperties
{
   /* Physical Cell ID */
   U16 physCellId;
   /* Tracking Area Code */
   U16 tac;
   /* Cell Identity is of 28 bits long */
   U32 cellId;
   U8 numBcastPlmn;/* Num of broadcasted plmns*/
   /* PLMN ID is of 24 bits long */
   U32 plmnId[VE_MAX_NO_OF_PLMNS];
   /* duplexing mode : TDD/FDD/HD_FDD */
   NhuDuplexMode duplexMode;
   /* EUTRA Mode */
   union 
   {
      VeCellFddCb fddCb;
      VeCellTddCb tddCb;
   } u;
} VeCellPrp;

typedef struct _veEcgi
{
   /* 24 bit long */
   U32 plmnId;
   /* 28 bit long */
   U32 eutranCellId;
} VeEcgi;

typedef struct _veGuGrpIdLst
{
   /* 24 bit long */
   U32 plmnId;
   U16 mmeGrpId;
} VeGuGrpIdLst;

typedef struct _veNghInfoLst
{
   VeEcgi   ecgi;
   U16      physCellId;
   U32      earfcn;
} VeNghInfoLst;

typedef struct _veNghCellCb
{
   CmLList     lnk;
   VeLiSapCb   *x2apSap;
   U32         peerId;
   U8          enbType;         /* home / macro eNodeB */
   CmTptAddr   tEnbAddr;
   U32         plmnId;          /* PLMN ID - 24 bits */
   U32         locEgtpTunCntr;
   Bool        isX2ConnAvl;
   U16         numNghInfo;
   VeCellPrp   veCellPrp;
   /* X2_CFG_UPD_CHANGES */
   VeNghInfoLst   **nghInfo;
   U8          numguGrp; /* MAX of 16 group items are possible */
   VeGuGrpId   *guGrpId;
   U16         x2apEnbUeIdCntr;
   Bool        trgrX2;                         /* flag to indicate whether 
                                                  to trigger the X2SET Up towards this 
                                                  eNB or not */
   /* X2_CFG_UPD_CHANGES */
   CmTimer     cfgUpdTmr;
   U8          maxCfgupdRetry;  /* Number of Retransmitted X2 Config update */
   CmTimer     x2SetUpTmr;   /* X2-Setup retransmission timer */ 
   U8          maxSetupRetry;  /* Number of Retransmitted X2Setup */
   CmTimer     x2ResetTmr;    /*!< X2 Reset Timer */
   VeX2ResetCb x2ResetCb;
   VeCfgUpdInfo cfgUpdInfo; 
} VeNghCellCb;

typedef struct _veX2ConCb
{
   U32       oldEnbUeX2apId;
   U32       newEnbUeX2apId;
   U32       peerId;
   VeUeCb    *ueCb;
   CztCause  cause;
   U8        hoNodeType;  /* To identify source/target enodeB */
} VeX2ConCb;

typedef struct _veCandCellDb
{
   CmLList                lnk;
   VeNghCellCb            *nghCellCb;
   VeX2ConCb              *x2ConCb;
} VeCandCellDb;

typedef struct _veSnSts
{
  U8               rbId;
  NhuHoPdcpSduInfo nhuHoPdcpSduInfo;
} VeSnSts;

typedef struct _veSnStsInfo
{
  U8               numOfRb;
  VeSnSts          veSnSts[11];
} VeSnStsInfo;
#endif
typedef struct _veRntiNode
{
   CmLList  lnk;
   U8       rnti;
} VeRntinode;
typedef struct _veRntiDb
{
   U16        rntiStart;  /*!< Start RNTI */
   U16        maxRrmRntis;   /*!< Maximum number of RNTIs managed by RRM */
   CmLListCp freeRntiLst; /*!< List of Available C-RNTIs(VeRntiNode) */
   CmLListCp inUseRntiLst; /*!< List of used RNTIs(VeRntiNode)  */
} VeRntiDb;

typedef struct _vePrmblNode
{
   CmLList  lnk;
   U8       prmblId;
} VePrmblnode;
typedef struct _vePrmblDb
{
   U16         prmblStart;  /*!< Start Preamble */
   U16         maxPrmbls;   /*!< Maximum number of Preambles managed by RRM */
   CmLListCp freePrmblLst; /*!< List of Available preambles(VePrmblNode) */
   CmLListCp inUsePrmblLst; /*!< List of used preambles(VePrmblNode)  */
} VePrmblDb;

typedef struct _veS1ConCb
{
   UConnId  suConnId;
   UConnId  spConnId;
   U32      mme_ue_s1ap_id;
   U32      enb_ue_s1ap_id;
   Bool     s1apConnState;
   U32      nmbERAB;
   VeUeCb   *ueCb;
   VeMmeCb  *mmeCb;
   SztCause cause;
   U32      peerId;
} VeS1ConCb;

typedef struct veSTmsi
{
   Bool pres;
   U8   mmec;
   U32  mTMSI;

} VeSTmsi;

/* NAS NON Delivery Indication start */
typedef struct veNasInfo
{
   CmHashListEnt hlEnt;                 /* Entry into the hash list   */
   CmLList     nasPdu;
   U32         transId;
   TknStrOSXL  *asn;
   U16         crnti;
}VeNasInfo;
/* NAS NON Delivery Indication end */


struct _veUeCb
{
   /* Ue category */
   U8                     ueCat;
   /* trans identifier */
   U8                     transId;
   /* C-RNTI */
   U16                    crnti;
   CmLteCellId            cellId;
   Bool                   rrcConnState;
   U32                    establishCause; 

   /* RAB Info (Both SRB & Data RBs) */
   VeUeRABInfo            rbInfo;
   /* Cqi related info */
   U16                    pmiCfgIdx;
   U16                    srCfgIdx;
   U16                    cqiPmiCfgIdx;
   /* Security related info */
   VeUeSecCfg             secCfg;
/* Store Ue Capability Info */
#ifdef UE_RAD_CAP
   /* Ue Capability Info */
   VeUeCapInfo ueCap;
#endif
   /* Add any S1AP/GTP related info */
   VeS1ConCb               *s1ConCb;
   SuId                    sapId;

   /* S1ap Pdu */
   S1apPdu                 *pdu;
   VeSTmsi                 sTMSI;
   VeGummei                gummei;
   VePlmnId                plmnId;
   U32                     selectedPLMNIdentity;

   /* Number of Tunnels created */
   U8                      numOfTunnelCreReq;
 /* RRC-ReEstab */
   U8  s1MsgType; /*For rrc re-establishment*/
   /* Pradeep 30_12_11 Number of Tunnels deleted */
   U8                      numOfTunnelDelReq;
#ifdef LTE_HO_SUPPORT
   U8                      isLwrLyrRestNeeded;   
   U8                      lowLyrCfgSts; /* indicates whether the configuration of 
                                            lower layers is complete or in progress */
   VeHoType                hoType;       /* indicates the type of HO - (1 - S1 or 2 - X2) */
   VeHoState               mobCtrlState; /* indicates the UE HO state */
   SztUE_HistoryInform     ueHistory; /* UE History Information */
   CmLListCp               candCellList; /* linked List to store VeCandCellDb */
   VeX2ConCb               *x2ConCb;   /* X2 Connection control Block*/
   NhuEncCfmSdus           *trnspContainer; /* Container which contains encoded data.*/
   U16                     dedicatedPrmbl; /* Dedicated preamble allocated for
                                              the UE during the successful
                                              admission of the UE*/
   U32                     src_mme_ue_s1ap_id; /* source MMEUES1AP Id received through 
                                                  X2 HO REQ. This will be used to fill
                                                  the same in the PATH SWITCH Request */
   CmTimer                 ueHoAttachTmr; /* Timer which is used by UE During synchronization.*/
   CmTimer                 s1RelocTmr;/* Timer which is used by eNB.*/
   CmTimer                 s1OvrallTmr;/* Timer which is used by eNB.*/
#endif
   /*  UE Con  Timer*/
   CmTimer    ueConTmr;
   U16        reEstCause;
   /* NAS NON Delivery Indication start */
   CmLListCp      vePduLLst;
   U8             crntTransIdModifier;
   /* NAS NON Delivery Indication end */
};

typedef struct _veSysInfoRel
{
   /* PLMN Id */
   TknU8 mcc[3];
   TknU8 mnc[3]; /* Token since size could be of 2 to 3 */
  /* Variable MNC digits CRID:ccpu00118127*/
   /*Number of MNC digits*/
   U8 mnc_nocomp;
   /* Tracking Area Code */
   U16 trackAreaCode;
   /* Frequency Band Indicator */
   U8 freqBandInd;
} VeSysInfoRel;

typedef struct _veUePagingInfo
{
   CmLList  lnk;
   VeSTmsi  sTMSI;
   U16      crnti;
   U16          T;
   U8           N;
   U8          Ns;
   U32      ue_Id;
   U16         pf;
   U8          po;
   NhuDatReqSdus *pagMsg;
   U8          domIndType;
}VeUePagingInfo;

typedef struct _veCellCb
{
   /* Cell Identity is of 28 bits long */
   U32              cellId;    
   /* State */
   U8               state;
   /* System Frame Number */
   U16              sfn;
   /* Modulation Type 
    * BPSK / QPSK / 16QAM / 64QAM */
   U8               modType;
   /* duplexing mode : TDD/FDD/HD_FDD */
   U16              duplexMode;

   U8  maxUeSupp; /* maximum number of UEs supported by each eNodeB */
   U8  numUeCfg;  /* number of active Ues configured */

#ifdef LTE_HO_SUPPORT
   /* Physical Cell ID */
   U16              physCellId;
   /* Tracking Area Code */
   U16              tac;
   /* No of Broadcasted plmns.*/
   U8               numBrdCstPlmn;
   /* PLMN ID is of 24 bits long */
   U8              plmnId[VE_MAX_NO_OF_PLMNS][3];
   /* EUTRA Mode */
   union 
   {
      VeCellFddCb   fddInfo;
      VeCellTddCb   tddInfo;
   } u;
   CmLListCp        nghCellLst; /* List of VeNghCellCb */ 
   U32              rsrpTrshldRange;            /* Serving Cell threshold 
                                                   RSRP value */            
#endif
   VeRntiDb         rntiDb; /* Collection of RNTI.*/
   VePrmblDb        prmblDb; /* Collection of preamble.*/
   /* Data related to SysInfo  BroadCast for the respective cell */ 
   VeSysInfoRel     sysRelInfo;
   /* UE Cbs */
   VeUeCb           **ueCbLst;
   CmLListCp        pendPagUesLst;   /* Pending Paging UEs VeUePagingInfo*/
   U16              veDfltMacRntisize;          /* Max RNTIs managed by MAC */
   U16              veDfltStartRnti;            /* Start RNTI value for MAC */
   U8               veDfltNumRaPreamble;        /* Number of RA Preambles supported at MAC */
   /* PHY configuration parameters */
   U16      opMode;
   U32      counter;
   U32      period;
   /* NAS NON Delivery Indication */
   CmHashListCp vePduHashLst; 
} VeCellCb;
typedef struct _veEnbOvldParams
{
   U8       eNBCritThrldVal;            /*!< EnodeB Overload Critical Threshold value */
   U8       eNBMajThrldVal;             /*!< EnodeB Overload Major threshold value */
   U8       eNBMinThrldVal;             /*!< EnodeB Overload minor threshold value */
   U32         eNBOvldMonTimerVal;       /*!< EnodeB Overload monitor timer value */
   U8          ovldCntVal;               /*!< EnodeB Overload counter value */
   U8          maxCntVal;               /*!< EnodeB Overload counter value */
   U8          eNBOvldFlag;
} VeEnbOvldParams;

/* X2_CFG_UPD_CHANGES */
typedef struct veIeInfo
{
   Bool pres;        /**<  Whether Present or not */
   U8   ieId;        /**<  IE ID */
   U8   crit;        /**<  IE Criticality */
   U8   typOfErr;    /**<  Type of Error */
}VeIeInfo;

typedef struct _veCb
{
   /* memory pool info */
   Mem mem;
   TskInit     init;
   CmTqCp      tqCp;           /* Timing Queue Control point    */
   CmTqType    tq[VE_NUM_TQENTRY]; /* Timing Queue for a resln. */
   Header      lmHdr;
   
   UConnId s1apConId;                  /* Last used connection id; counter */

   VeCellCb    **cellCb;
   VeMmeCont   mmeCont;
   VeS1ConCb   **s1apConLst;   
   VeLiSapCb   **nhuSap;
   VeLiSapCb   **ctfSap;
   VeLiSapCb   **rgrSap;
   VeLiSapCb   **sztSap;
   CmTptAddr   datAppAddr;
   U32         nhuTransId;
   VeeNBType      eNBType;   /* Home / Macro */ 
#ifdef LTE_HO_SUPPORT
   U32            x2apOldEnbUeIdCntr;
   VeX2ConCb      **x2apConLst; /* X2ap Con list.*/
   U16            numNghInfo;/* No of Neighbours.*/
   VeNghInfoLst   **nghInfo; /* Neighbour Information.*/
   U8             numguGrp; /* MAX of 16 group items are possible */
   VeGuGrpIdLst   **guGrpId;
   VeLiSapCb      **cztSap;
   /* S1 Handover Timers - Placing on a eNodeB level (common for all cells) */
   U32             s1PrepTimerVal;             /* Preparation Timer Value */
   U32             s1OvrAllTimerVal;           /* Overall Timer Value */
   U32             ueHoAttachTimerVal;         /* Time within which UE \
                                                  must detach from source\
                                                  cell and attach to \
                                                  target cell during \
                                                  Handover. */
#endif
   /* Adding log file informations */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
   FILE*         dbgFp;                         /*!< Debug file pointer */
   Txt           filePath[LVE_MAX_FILE_PATH];   /*!< Path to store log files */
   U32           nmbDbgLines;                   /*!< Number of lines per debug file */
   U32           cfgDbgLines;                   /*!< Cfgd number of lines/Dbg file */
#endif /* end of (VE_SM_LOG_TO_FILE && DEBUGP) */
   CmTimer                    eNBOvldTmr;       /*!< EnodeB Overload monitor timer */
   U32                        eNBOvldMonTimerVal;       /*!< EnodeB Overload monitor timer value */
   VeEnbOvldParams           eNBOvldParams;
} VeCb;

EXTERN  VeCb      veCb;

/* Structure used while encoding and decoding S1 messages. */
typedef struct _s1apEncDecIe
{
   CmMemListCp  memCp;
   Void         *iePtr;
}S1apEncDecIE;


#ifdef EU_DAT_APP

typedef struct _veEgtpTunCb VeEgtpTunCb;

typedef struct _vePdcpCb
{
   CmLtePdcpId  pdcpId;
   U32 locTeId;
   VeEgtpTunCb *egtpTunCb; /* normal tunnel */
#ifdef LTE_HO_SUPPORT
   VeEgtpTunCb *datFwdUlEgtpTunCb;/* UL Data forwarding tunnel details.*/
   VeEgtpTunCb *datFwdDlEgtpTunCb;/* DL Data forwarding tunnel details.*/
#endif
   VeLiSapCb *liSap;
   U32 sduIdCntr;
}VePdcpCb;

struct _veEgtpTunCb
{
   U32           locTeId;
   U32           remTeid;
   CmTptAddr     dstIpAddr;                   /**< Destination IP Address */
   CmTptAddr     frmDstIpAddr;                /**< From Destination IP Address */
   CmLtePdcpId   pdcpId;
   U8            state;
   VePdcpCb      *pdcpCb;
   VeLiSapCb     *liSap;
#ifdef LTE_HO_SUPPORT
   U8            datFwdIndFlag; /* To indicate, which packets needs to be
                                   forwarded. 
                                   VE_EU_NO_FWD       - no forwarding 
                                   VE_EU_FWD_NEW_PKTS - Forward new incoming
                                   packets from S-GW
                                   VE_EU_FWD_OLD_PKTS - Forward old packets from
                                   PDCP */
   CmLListCp     dataBuffLst;   /* At Source Side : 
                                   linked list of EuEgtpBuffInfo (for normal
                                   tunnel id) to store fresh pkts from S-GW
                                   linked list of PjuDatFwdInfo (for forwarding
                                   tunnel ids) to store forwarded pkts from PDCP */

                                /* At Target Side :
                                   linked list of EuEgtpBuffInfo (for normal
                                   tunnel id) to store fresh pkts from S-GW after path switch
                                   linked list of PjuDatFwdInfo (for forwarding
                                   tunnel id ) to store forwarded pkts (without
                                   SN) from S-ENB */
                                /* NOTE: Forwarded pkts with SN are directly forwarded to PDCP */
#endif
};

typedef struct _veEuUeCb
{
   CmTimer    inactivityTmr;
   U32        ueId;
   Bool       datRcvd;
   U8         expiryCnt;
}VeEuUeCb;

typedef struct _veEuCb
{
   /* memory pool info */
   Mem mem;
   TskInit              init;
   CmTqCp               tqCp;           /* Timing Queue Control point    */
   CmTqType             tq[VE_NUM_TQENTRY]; /* Timing Queue for a resln. */
   Header               lmHdr;
   CmTptAddr            srcAddr;

   VeLiSapCb **lsapLst;
   VeEgtpTunCb **egtpTunLst;
   VePdcpCb **pdcpIdLst;
   U16   pdcpIdLstMaxSize;    
   VeEuUeCb  **ueCbLst;
#ifdef VE_PERF_MEAS 
  CmTimer      loadTimers[1];   /* Load is generator timer : VE_PERF_MAX_TMR */ 
  CmLtePdcpId  ueLst[VE_MAX_UE];  /* VE_MAX_UE  Max no of UEs */ 
  U32          sduCntr; 
#endif
} VeEuCb;

#ifdef MSPD
/* This structure is used for storing the MSPD specific parameters */
typedef struct _VeMspdSpecific
{
   U16           opMode;
   U32           counter;
   U32           period;
}VeMspdSpecific;
#endif


#ifdef YS_PICO
/* This structure is used for storing the PICO specific parameters */
typedef struct _VePicoSpecific
{  
   U16               syncSigPowOs;              /*!< Synchronization signal power offset */
   Bool              syncSigPowOsPres;
   U16               cfiPowOs;                  /*!< Cfi Power Offset */
   Bool              cfiPowOsPres;
   U16               dciPowOs;                  /*!< Dci Power Offser */
   Bool              dciPowOsPres;
   U16               extWinMargin;              /*!< Extraction Window Marin */
   Bool              extWinMarginPres;
   U16               pucchNoiseGamma;           /*!< PUCCH Noise Estimation Gamma*/
   Bool              pucchNoiseGammaPres;
   U16               prachPkRatio4;             /*!< PRACH Format4 Peak Ratio */
   Bool              prachPkRatio4Pres;
   U16               prachPkRatio0;             /*!< PRACH Format0 Peak Ratio */
   Bool              prachPkRatio0Pres;
   U16               srsDopEstFactor;           /*!< Doppler Estimation Compensation Power */
   Bool              srsDopEstFactorPres;
   U16               puschProbDtxAck;           /*!< Portability Dtx Ack PUSCH */
   Bool              puschProbDtxAckPres;
   U16               pucchProbDtxAck;           /*!< Portability Dtx Ack PUCCH Format1 */
   Bool              pucchProbDtxAckPres;

   U16               txAntennaPorts ;           /*!< The number of cell specific transmit antenna ports */
   Bool              txAntennaPortsPres;
   U16               rxAntennaPorts ;           /*!< The number of cell specific receive antenna ports */
   Bool              rxAntennaPortsPres ;

   U16               phySyncMode;               /*!< The method used by the PHY to signal the 1ms subframe */
   Bool              phySyncModePres;
   U16               dataRepMode;               /*!< The data report mode for the uplink data */
   Bool              dataRepModePres ;
   U16               rachSrRepMode;             /*!< The reporting mode for RACH.indication and SR.indcation */
   Bool              rachSrRepModePres;
   /* Following fields are for CTF Cell Reconfiguarton*/
   U8                ctfReCfgType;              /*!< Type of CTF Reconfiguration. 0 - MAJOR RECONFIG 1 - MINOR RECONFIG */
   Bool              bwCfgPres;                
   Bool              txCfgPres;
   Bool              phyCellIdPres;
   U8                phyCellId;
   CtfBwCfgInfo      bwCfg;                     /*!<i Bandwidth onfiguration */
   CtfTxSchemeCfg    txCfg;                     /* !< Basic transmossion Scheme Configuration[36.300, 5.1.1] */
   CtfAntennaCfgInfo antennaCfg;                /* !< Antenna Configuration */

}VePicoSpecific;
#endif

EXTERN  VeEuCb     veEuCb;

/* Adding log file function */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
EXTERN Void veUtlCreateDbgFile ARGS((Void));
EXTERN S16  veUtlStrcpy ARGS ((S8   *tgt, CONSTANT S8    *src));
EXTERN S16  veUtlStrcat ARGS ((S8   *tgt, S8    *src));
#endif

PUBLIC S16 euLSapCntrl ARGS((VeCntrl *sapCntrl,CmStatus *status,Elmnt elmnt));
PUBLIC S16 VeLiEgtBndReq ARGS((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 VeLiPjuBndReq ARGS((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 VeLiEgtDatInd ARGS((Pst *pst, SuId suId, EgtUEvnt *eguMsg));
EXTERN S16 VeDatCreatEgtpTunel ARGS((Pst *pst, SuId suId, EgtpTunInf *tunelInf));
PUBLIC S16 VeDatDelEgtpTunel ARGS((Pst *pst, SuId suId, U32 locTeid));
PUBLIC S16 VeSndRelReq ARGS((Pst *pst, U32 ueId));
PUBLIC S16 veDatFillLocEgtpTunnel ARGS((VeEgtpTunCb *egtpTun,EgtUEvnt **eguEvtMsg,U8 action));
PUBLIC S16 veFillEgtpDatMsg ARGS((VeEgtpTunCb    *egtpCb,EgtUEvnt **eguEvtMsg, U8 msgType));
PUBLIC S16 VeLiPjuDatInd ARGS((Pst*pst,SuId suId,CmLtePdcpId *pdcpId,Buffer *mBuf));
PUBLIC S16 VeLiPjuDatReq ARGS((Pst*pst,SuId suId,CmLtePdcpId *pdcpId,PjuSduId sduId,Buffer *mBuf));
PUBLIC S16 VeLiPjuDatFwdReq ARGS((
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuDatFwdReqInfo * datFwdReq
));
PUBLIC S16 VeLiPjuUbndReq ARGS((
Pst* pst,
SpId spId,
Reason reason
));

PUBLIC S16 VeLiRgrUbndReq ARGS((
Pst* pst,
SpId spId,
Reason reason
));

PUBLIC S16 VeLiRgrCfgReq ARGS((
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrCfgReqInfo * cfgReqInfo
));

PUBLIC S16 VeLiSztAudReq ARGS((
Pst *pst,
SpId spId,
SztAudEvnt *audEvnt
));
PUBLIC S16 veLIMRrcMeasRprtHndlr ARGS((NhuDatIndSdus *datIndSdu));



EXTERN S16 VeLiEgtEguDatReq ARGS(( Pst *pst, SuId  suId, EgtUEvnt *egtUEvnt));
EXTERN S16 VeLiEgtEguLclTnlMgmtReq ARGS(( Pst  *pst, SuId  suId, EgtUEvnt  *egtUEvnt)); 
PUBLIC S16 EuLiEgtDatInd ARGS((Pst *pst, SuId suId, EgtUEvnt *eguMsg));
EXTERN S16 VeLiEgtUbndReq ARGS(( Pst *pst, SpId spId, Reason reason));
EXTERN S16 VeLiEgtEguStaReq ARGS(( Pst *pst,SuId suId, EgtUEvnt     *egtUEvnt));

#endif



EXTERN S16 VeHdlCreatEgtpTunelCfm ARGS(
(
Pst         *pst,
SuId        suId,
CmLtePdcpId *pdcpId
));

EXTERN S16 VeHdlDelEgtpTunelCfm ARGS(
(
Pst         *pst,
SuId        suId,
CmLtePdcpId *pdcpId
));
/*Timer related Functio*/

EXTERN S16 veRegInitTmr ARGS((Void));
EXTERN S16 veRegInitDatTmr ARGS((Void));
EXTERN Void veStartTmr ARGS(( PTR                 cb,
                              S16                 tmrEvnt,
                              U32                 delay
                              ));

EXTERN Void veInactvStartTmr ARGS ((
PTR                 cb,
S16                 tmrEvnt,
U32                 delay
));

EXTERN Void veInactvStopTmr ARGS((
PTR     cb,
S16     event
));

EXTERN Void veProcInactvTmr ARGS((
Ptr     cb,
S16     event
));

EXTERN Void veStopTmr ARGS((  PTR                 cb,
                              S16                 tmrEvnt));
EXTERN S16 veActvTmr ARGS (( void));
EXTERN S16 veActvDatTmr ARGS (( void));
EXTERN S16 veFreeBufLst ARGS((VeEgtpTunCb *egtpTunCb));



typedef S16 (* VeNhuLIMFSM)    ARGS((NhuDatIndSdus *nhuDatIndSdus));
PUBLIC S16 veSztS1SetReq ARGS((SpId spId,U32 peerId));
PUBLIC S16 veLiSztDlNasHandler ARGS ((VeS1ConCb *s1ConCb, S1apPdu  *pdu)); 
PUBLIC S16 veLiSztInitConSetHandler ARGS ((VeS1ConCb *s1ConCb, S1apPdu  *pdu)); 
PUBLIC S16 veLiSztOvldStartHandler ARGS ((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
PUBLIC S16 veLiSztOvldStopHandler ARGS ((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
PUBLIC S16 veSztNoOpr ARGS ((VeS1ConCb *s1ConCb, S1apPdu  *pdu)); 
PUBLIC S16 veLiSztUeRelCmdHandler ARGS ((VeS1ConCb *s1ConCb, S1apPdu  *pdu)); 
PUBLIC S16 veLiSztPagingHandler ARGS ((VeS1ConCb *s1ConCb, S1apPdu  *pdu)); 
PUBLIC S16 veUtlSztUeCntxtRelComp ARGS ((VeS1ConCb *s1ConCb, S1apPdu  **pdu)); 
PUBLIC S16 veUtlSztUeCntxtRel ARGS ((VeS1ConCb *s1Con, S1apPdu  **pdu));
PUBLIC S16 veLiSztERABSetHandler ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
PUBLIC S16 veLiSztERABModHandler ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
PUBLIC S16 veLiSztERABRelComHandler ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));

EXTERN S16 veLiSztHdlEnbCfgUpdAck ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
EXTERN S16 veLiSztHdlEnbCfgUpdFail ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
EXTERN S16 veLiResetHandler ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
EXTERN S16 veSztMmeCfgUpdHandler ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
EXTERN S16 veLiSztUeCntxModHandler ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
EXTERN S16 veHdlFullReset ARGS((U32 peerId));
EXTERN S16 veHdlPartReset ARGS((U32 peerId, TknU8 *rstTypeIE));
EXTERN S16 veGetPlmnVal ARGS((VePlmnId *plmnId, SztPLMNidentity *ie));
EXTERN S16 veAddGummei ARGS((U32 peerId, SztProtIE_Field_S1SetupRespIEs *ie));
EXTERN S16 veUpdateRelCap ARGS((U32 peerId, SztRelativeMMECapacity *relCap));
EXTERN S16 veSndMmeCfgUpdAck ARGS((U32 peerId));
EXTERN S16 veSztFilMmeCfgUpdAck ARGS((S1apPdu **pdu));
EXTERN S16 veSetupResp ARGS((U32 peerId, S1apPdu *pdu));
EXTERN S16 veUpdateGummei ARGS((U32 peerId, SztProtIE_Field_MMEConfigUpdIEs *ie));
EXTERN S16 veSndEnbCfgUpd ARGS((Void));
EXTERN S16 veUtlSztFillEnbCfgUpd ARGS((S1apPdu **pdu));
EXTERN S16 veHdlEnbCfgUpdTimOut ARGS((VeMmeCb  *mmeCb));
EXTERN S16 veUtlSztReset ARGS((S1apPdu **pdu, U8 rstType, U16 numCons,
                                   U8 choice, U32 cause, VeS1ConId *conIds));
EXTERN S16 veUtlFillReset ARGS((S1apPdu **pdu, U8 rstType, U16 numCons,
                                    U8 choice, U32 cause, VeS1ConId *conIds));
EXTERN S16 veLiResetAckHandler ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
EXTERN S16 veLiSztErrIndHdl ARGS ((VeS1ConCb *s1Con, S1apPdu  *pdu));
EXTERN S8 veCmpPlmnId ARGS ((VePlmnId *plmnId1, VePlmnId *plmnId2));
EXTERN S16 veSndUeCntxModRsp ARGS ((VeS1ConCb *s1Con));
EXTERN S16 veSndAmbrRgrCfg ARGS ((VeUeCb *ueCb, VeUeRAB *rabInfo));
EXTERN S16 veResetCntrl ARGS  ((VeS1Reset    *veReset));

PUBLIC S16 veFillAndAddPagingInfo ARGS (( S1apPdu *s1apPdu));

PUBLIC S16 veUtlSztFillS1SetupReq ARGS ((S1apPdu **pdu)); 
PUBLIC S16 veUtlSztFillInitUeMsg ARGS ((VeUeCb *ueCb, NhuDatIndSdus *datIndSdu,S1apPdu **pdu)); 
EXTERN S16 veUtlSztFillUlNasTrans ARGS ((VeS1ConCb *s1apCon,NhuDatIndSdus *datIndSdu,S1apPdu **pdu)); 
EXTERN S16 veUtlSztFillInitCntxtSetRsp ARGS ((VeS1ConCb *s1apCon, NhuDatIndSdus *datIndSdu, S1apPdu **pdu));
/* RRC-ReEstab */
EXTERN S16 veUtlSztFillERABSetRsp ARGS ((VeS1ConCb *s1apCon, S1apPdu  **pdu,Bool isReCfgSucc));
EXTERN S16 veUtlSztFillERABRelRsp ARGS ((VeS1ConCb *s1apCon, S1apPdu **pdu,Bool isReCfgSucc));
EXTERN S16 veUtlSztFillNasNonDlvInd ARGS ((VeS1ConCb *s1apCon, S1apPdu **s1ap_pdu, TknStrOSXL *nasPdu ));
EXTERN S16 veUtlSztFillInitCntxtSetFail ARGS ((VeS1ConCb  *s1apCon,S1apPdu **s1apPdu,U32 cause));
EXTERN S16 veUtlFillUeCntxModRsp ARGS ((S1apPdu **s1apPdu, U32 eNbId, U32 mmeId));

EXTERN S16 VeLiNhuBndReq  ARGS(( Pst *pst, SuId suId, SpId spId ));
EXTERN S16 VeLiNhuUbndReq ARGS(( Pst *pst, SpId spId, Reason reason ));
EXTERN S16 VeLiNhuDatReq  ARGS(( Pst *pst, SpId spId, NhuDatReqSdus *nhuDatReqSdus ));
EXTERN S16 VeLiNhuDatRsp  ARGS(( Pst *pst, SpId spId, NhuDatRspSdus *nhuDatRspSdus ));
EXTERN S16 VeLiNhuCfgReq  ARGS(( Pst *pst, SpId spId, NhuCfgReqSdus *nhuCfgReqSdus ));
EXTERN S16 VeLiNhuCnclUeReq  ARGS(( Pst *pst, SpId spId, NhuCellId cellId,
                                       NhuCrnti crnti ));
EXTERN S16 VeLiNhuCellCfgReq  ARGS(( Pst *pst, SpId spId, NhuCellCfgReqSdus *nhuCellCfgReqSdus ));
EXTERN S16 VeLiNhuDeleteTransReq  ARGS(( Pst *pst, SpId spId, 
                                         NhuCellId cellId, NhuCrnti ueId,      
                                         U32 transId ));

EXTERN S16 VeLiNhuDecodeReq ARGS(( Pst     *pst, SuId     suId, NhuDecodeSduReq *nhuDecodeSduReq));
EXTERN S16 VeLiNhuCountReq ARGS(( Pst     *pst, SuId     suId, NhuHdr *countHdr));
EXTERN S16 VeLiNhuEncodeReq ARGS(( Pst     *pst, SuId     suId, NhuEncReqSdus *nhuEncReqSdus));
EXTERN S16 VeLiNhuPdcpSduStaReq ARGS((Pst     *pst, SuId    suId, NhuHdr  nhuHdr));

PUBLIC S16 VeLiRgrBndReq ARGS((Pst *pst, SuId suId, SpId spId));

#ifdef RGR_RRM_TICK
EXTERN S16 veSibHdlTmrInd ARGS((RgrTtiIndInfo *ttiInd ));
PUBLIC S16 VeLiRgrTtiInd ARGS((Pst *pst, SuId suId, RgrTtiIndInfo *ttiInd));
#endif

PUBLIC S16 VeLiCtfBndReq ARGS((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 VeLiSztBndReq ARGS((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 VeLiSztUbndReq ARGS((Pst *pst, SpId spId,Reason reason));

PUBLIC S16 VeLiSztUDatReq ARGS((Pst *pst,SpId spId,SztUDatEvnt *uDatEvnt));
PUBLIC S16 VeLiSztRelRsp ARGS((Pst *pst,SpId spId,SztRelRsp *relRsp));

EXTERN S16 VeLiSztBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 VeLiSztUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
EXTERN S16 VeLiSztBndCfm       ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 VeLiSztStaInd       ARGS((Pst *pst, SuId suId, SztStaInd *sztSta));
EXTERN S16 VeLiSztFlcInd       ARGS((Pst *pst, SuId suId, SztFlcInd *flcInd));
EXTERN S16 VeLiSztErrInd       ARGS((Pst *pst, SuId suId, SztLclErrInd *sztErrInd));
EXTERN S16 VeLiSztUDatReq      ARGS((Pst *pst, SpId spId, SztUDatEvnt *uDatEvnt));
EXTERN S16 VeLiSztUDatInd      ARGS((Pst *pst, SuId suId, SztUDatEvnt *uDatEvnt));
EXTERN S16 VeLiSztConReq       ARGS((Pst *pst, SpId spId, SztConReq *conReq));
EXTERN S16 VeLiSztConInd       ARGS((Pst *pst, SuId suId, SztConInd *conInd));
EXTERN S16 VeLiSztConRsp       ARGS((Pst *pst, SpId spId, SztConRsp *conRsp));
EXTERN S16 VeLiSztConCfm       ARGS((Pst *pst, SuId suId, SztConCfm *conCfm));
EXTERN S16 VeLiSztRelReq       ARGS((Pst *pst, SpId spId, SztRelReq *relReq));
EXTERN S16 VeLiSztRelInd       ARGS((Pst *pst, SuId suId, SztRelInd *relInd));
EXTERN S16 VeLiSztRelRsp       ARGS((Pst *pst, SpId spId, SztRelRsp *relRsp));
EXTERN S16 VeLiSztRelCfm       ARGS((Pst *pst, SuId suId, SztRelCfm *relCfm));
EXTERN S16 VeLiSztDatReq       ARGS((Pst *pst, SpId spId, SztDatEvntReq *datEvnt));
EXTERN S16 VeLiSztDatInd       ARGS((Pst *pst, SuId suId, SztDatEvntInd *datEvnt));
EXTERN S16 VeLiSztEncDecReq    ARGS((Pst *pst, SpId spId, SztEncDecEvnt *sztEncDecEvnt));
EXTERN S16 VeLiSztEncDecCfm    ARGS((Pst *pst, SuId suId, SztEncDecEvnt *sztEncDecEvnt,SztReqStatus *status));
EXTERN S16 VeLiSztAudCfm       ARGS((Pst *pst, SuId suId, SztAudEvnt *audEvnt));
/* Fix for CID- 1657-01-01 DefectId- 115433 */
/* Added function prototype to remove warning */
EXTERN S16 veSndCmnNhuDatReq   ARGS((U8    msgCategory, U8 msgType, CmLteTimingInfo tmnInf)); 

EXTERN S16 TfLiCtfBndCfm ARGS((
Pst       *pst,         
SuId      suId,        
U8        status      
));

EXTERN S16 TfLiCtfCfgCfm ARGS((
Pst *pst,             
SuId suId,           
CtfCfgTransId transId,
U8 status            
));

EXTERN S16 TfLiCtfUeIdChgCfm ARGS((
Pst       *pst,            
SuId       suId,
CtfCfgTransId transId,
CtfUeInfo *ueInfo, 
U8 status
));

EXTERN S16 VeLiCtfBndReq ARGS((
Pst* pst,
SuId suId,
SpId spId
));

EXTERN S16 VeLiCtfUbndReq ARGS((
Pst* pst,
SpId spId,
Reason reason
));

EXTERN S16 VeLiCtfCfgReq ARGS((
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CtfCfgTransId transId,          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo       /* MAC Configuration information */
));

EXTERN S16 VeLiCtfUeIdChgReq ARGS((
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
));


/* ve_cell.c */
EXTERN S16 veFillCellRbCfg ARGS((NhuRbCfgList  **rbCfgList));
EXTERN S16 veSndNhuCellCfgReq ARGS((U32  cellId));
EXTERN S16 veSndCtfCellCfgReq ARGS((U32  cellId));
/* Cell reconfiguation changes */
EXTERN S16 veHdlRgrCellCfgReq ARGS((U32   cellId));
EXTERN S16 veHdlRgrCellReCfgReq ARGS((U32 cellId));
EXTERN S16 veSndCtfCellReCfgReq ARGS((U32 cellId));
EXTERN S16 veFillRgrDlHqCfg ARGS((RgrDlHqCfg *cfg));
EXTERN S16 veFillRgrRntiCfg ARGS((RgrRntiCfg *cfg));
EXTERN S16 veFillRgrCfiCfg ARGS((RgrCfiCfg *cfg));
EXTERN S16 veFillRgrUlTrgCqiCfg ARGS((RgrUlTrgCqiCfg *cfg));
EXTERN S16 veFillRgrDlCmnCodeRateCfg ARGS((RgrDlCmnCodeRateCfg *cfg));
EXTERN S16 veFillRgrPuschSubBandCfg ARGS((RgrPuschSubBandCfg *cfg));
EXTERN S16 veFillRgrUlCmnCodeRateCfg ARGS((RgrUlCmnCodeRateCfg *cfg));
EXTERN S16 veFillRgrDlfsCfg ARGS((RgrDlfsCfg *cfg));
EXTERN S16 veFillRgrBwCfg ARGS((RgrBwCfg *cfg));
EXTERN S16 veFillRgrPhichCfg ARGS((RgrPhichCfg *cfg));
EXTERN S16 veFillRgrPucchCfg ARGS((RgrPucchCfg *cfg));
EXTERN S16 veFillRgrSrsCfg ARGS((RgrSrsCfg *cfg));
EXTERN S16 veFillRgrSiCfg ARGS((RgrSiCfg *cfg));
EXTERN S16 veFillRgrPuschCfg ARGS((RgrPuschCfg *cfg));
EXTERN S16 veFillRgrPreambleSetCfg ARGS((RgrPreambleSetCfg *cfg));
#ifdef LTE_TDD
EXTERN S16 veFillRgrTddPrachRscInfo ARGS((RgrTddPrachRscInfo *cfg));
#endif 
EXTERN S16 veFillRgrRachCfg ARGS((RgrRachCfg *cfg));
EXTERN S16 veFillRgrCellDlPfs ARGS((RgrCellDlPfs *cfg));
EXTERN S16 veFillRgrCellUlPfs ARGS((RgrCellUlPfs *cfg));
EXTERN S16 veFillRgrCmnLcCfg ARGS((RgrCellCfg *cfg));
EXTERN S16 veFillRgrUlPwrCfg ARGS((RgrUlPwrCfg *cfg));


/* ve_sib.c */
EXTERN S16 veSibBldMIBMsg ARGS((NhuDatReqSdus *nhDatReqEvntSdu, U16           schedSFN));
EXTERN S16 veSibBldSIB1Msg ARGS((NhuDatReqSdus *nhDatReqEvntSdu, U16           schedSFN));
EXTERN S16 veSibBldSIB2Msg ARGS((NhuDatReqSdus *nhDatReqEvntSdu, U16 schedSFN, U8 subframe));
#ifdef RGR_RRM_TICK
EXTERN S16 veSibBldSIB3Msg ARGS((NhuDatReqSdus *nhDatReqEvntSdu, U16 schedSFN, U8 subframe));
EXTERN S16 veSibBldSIB4Msg ARGS((NhuDatReqSdus *nhDatReqEvntSdu, U16 schedSFN, U8 subframe));
EXTERN S16 veSibBldSIB5Msg ARGS((NhuDatReqSdus *nhDatReqEvntSdu, U16 schedSFN, U8 subframe));
EXTERN S16 veSibHdlTmrInd ARGS((RgrTtiIndInfo *tmngInfo));
EXTERN S16 vePagHdlTmrInd ARGS((RgrTtiIndInfo   *tmngInfo));
#endif
/* ve_li_hdl */
EXTERN S16 veHdlNhuDatInd ARGS((Pst     *pst, SuId     suId, NhuDatIndSdus *datIndSdu));
#ifdef VE_PERF_MEAS 
EXTERN S16 vePerfHdlNhuDatInd ARGS((Pst     *pst, SuId     suId, NhuDatIndSdus *datIndSdu));
EXTERN S16 vePerfStartData ARGS((
Pst *pst,
Buffer *mBuf,
Mem *sMem
));

#endif
#ifdef VE_UNUSED
EXTERN S16 veSndCmnNhuDatReq ARGS((U8    msgCategory, U8    msgType,
         CmLteTimingInfo tmnInf));
#endif
#ifdef SI_NEW
EXTERN S16 veSibBldPdu ARGS((U8 msgCategory, U8 msgType, Buffer **encBuf));
EXTERN S16 VeLiRgrSiCfgReq ARGS ((Pst* pst,SpId spId,RgrCfgTransId transId,
                                  RgrSiCfgReqInfo * cfgReqInfo));

#endif
EXTERN S16 veSndUENhuDatReq ARGS((NhuDatReqSdus *evt));
/* Ue Capability functions */
#ifdef UE_RAD_CAP
EXTERN S16 veSndNhuDecodeReq ARGS((NhuDecodeSduReq *evt));
EXTERN S16 veSndNhuEncodeReq ARGS((NhuEncReqSdus *evt));
EXTERN S16 veSndRrcUeCapDecReq ARGS((VeUeCb *ueCb,TknStrOSXL *str,
                                               U16 sduType));
EXTERN S16 veHdlNhuUeCapEncCfm ARGS((VeUeCb *ueCb, NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 veHdlNhuUeRaCapEncCfm ARGS((VeUeCb  *ueCb,NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 veHdlNhuUeCapDecCfm ARGS((VeUeCb   *ueCb,NhuDecodeSduCfm *nhuDecSduCfm));
EXTERN S16 veHdlNhuUeRaCapDecCfm ARGS((VeUeCb   *ueCb,NhuDecodeSduCfm *nhuDecSduCfm));
EXTERN S16 veHdlNhuEutraUeCapIeDecCfm ARGS((VeUeCb   *ueCb,NhuDecodeSduCfm *nhuDecSduCfm));
#endif
#ifdef LTE_HO_SUPPORT
EXTERN S16 veHdlNhuHoCmdDecCfm ARGS((VeUeCb   *ueCb,NhuDecodeSduCfm *nhuDecSduCfm));
EXTERN S16 veHdlNhuHoPrepEncCfm ARGS((VeUeCb  *ueCb,NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 veHdlNhuHoCmdEncCfm ARGS((VeUeCb  *ueCb,NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 veHdlNhuDlDcchEncCfm ARGS((VeUeCb  *ueCb,NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 veLiCztResetRqstHandler ARGS((VeX2ConCb *x2ConCb,CztEvnt *cztInd));
EXTERN S16 veLiCztResetRspHandler ARGS((VeX2ConCb *x2ConCb,CztEvnt *cztInd));
EXTERN S16 veSndX2ResetRqst ARGS((CztCause *cause, U32 peerId));
EXTERN S16 veSndX2ResetRsp ARGS((U32 peerId));
EXTERN S16 veAbortX2OngoingProc ARGS((U32 peerId));
EXTERN S16 veUtlCztFillResetRqst ARGS ((CztEvnt **pdu, CztCause *cause));     
EXTERN S16 veUtlCztFillResetRsp ARGS ((CztEvnt **pdu));     
#endif
EXTERN S16 veHdlNhuDatRsp ARGS((NhuDatRspSdus *nhuDatRspEvntSdu));
EXTERN S16 veSndRrcConReqSetup ARGS(( NhuDatIndSdus *datIndSdu));
EXTERN S16 veSndRrcRecfgReq ARGS((VeUeCb        *ueCb, U8 s1Event));
/* NAS NON Delivery Indication */
EXTERN S16 veSndRrcDlInfoTfr ARGS((VeUeCb *ueCb, TknStrOSXL *nasPdu));
EXTERN S16 veSndRrcConRel ARGS((VeUeCb        *ueCb));
EXTERN S16 veSndRrcSecModeCmd ARGS((VeUeCb        *ueCb, S1apPdu       *s1apPdu));
EXTERN S16 veSndRrcConReEst ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 veFetchSecInfo ARGS ((VeUeCb *ueCb, S1apPdu *s1apPdu));
EXTERN S16 veFillRrcRecfgInitCntxtSetup ARGS ((VeUeCb                *ueCb,
                                               NhuDatReqSdus         *nhDatReqEvntSdu,
                                               SztInitCntxtSetupRqst *s1apMsg));
EXTERN S16 veFillRrcRecfgERABSetReq ARGS ((VeUeCb   *ueCb, NhuDatReqSdus  *nhDatReqEvntSdu,
                                           SztE_RABSetupRqst   *s1apMsg));
EXTERN S16 veFillRrcRecfgERABModReq ARGS ((VeUeCb   *ueCb,NhuDatReqSdus  *nhDatReqEvntSdu,
                                           SztE_RABMdfyRqst   *s1apMsg));
EXTERN S16 veFillRrcRecfgERABRelCom ARGS ((VeUeCb    *ueCb,NhuDatReqSdus  *nhDatReqEvntSdu,
                                           SztE_RABRlsCmmd   *s1apMsg));
/* Ue Capability function */
#ifdef UE_RAD_CAP
EXTERN S16 veFillRRC_UeRadAccessCapInfo ARGS ((TknStrOSXL    *str,NhuEncReqSdus *encReq,
                                                VeUeCb *ueCb));
EXTERN S16 veFillRRC_UeCapEnq ARGS ((NhuDatReqSdus *nhuDatReqSdu,VeUeCb *ueCb));
EXTERN S16 veUtlSztFillUECapbltyInfoInd ARGS(( VeS1ConCb *s1apCon, TknStrOSXL  *octnStr,
                                               S1apPdu      **pdu));
EXTERN S16 veSndRrcUeCapEnq ARGS ((VeUeCb  *ueCb, S1apPdu  *s1apPdu));
EXTERN S16 veLIMRrcUeCapInfoHndlr ARGS((NhuDatIndSdus *datIndSdu));
#endif
EXTERN S16 veLIMRrcConSetCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 veLIMRrcUlInfoTfrHndlr ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 veLIMRrcSecModCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 veLIMRrcSecModFailHndlr ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 veLIMRrcReconfigCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 veNoOpr ARGS((NhuDatIndSdus *datIndSdu));
EXTERN S16 veSndCtfUeRls ARGS((VeUeCb        *ueCb));
EXTERN S16 veSndCtfUeCfg ARGS((VeUeCb        *ueCb));
EXTERN S16 veSndRgrDedLgChCfg ARGS((VeUeCb        *ueCb));
EXTERN S16 veSndRgrUeRls ARGS((VeUeCb        *ueCb, VeUeRAB *rabInfo, U8 delType));
EXTERN S16 veSndRrcConRej ARGS((VeUeCb        *ueCb));
PUBLIC S16 veMmeSelect ARGS((VeUeCb *ueCb, U16 *mmeId));
PUBLIC S16 veGetMmeByRelCap ARGS((VeUeCb *ueCb, VeMmeCb **mmeCb));
PUBLIC S16 veGetMmeAcceptStatus ARGS((U32 establishmentCause,  VeMmeCb *mmeCb));
#ifdef LTE_HO_SUPPORT
EXTERN S16 veSndX2UeLocRlsReq ARGS((VeX2ConCb *x2ConCb));
#endif
EXTERN S16 veSndS1UeLocRlsReq ARGS((VeS1ConCb *s1ConCb));

EXTERN S16 veSndRgrLchCfg ARGS((VeUeCb *ueCb, VeUeRAB *rabInfo, Bool lchRecfg));

EXTERN S16 veSndRgrLcgCfg ARGS((
VeUeCb        *ueCb,
U8            lcId,
U8            grpId
));

EXTERN S16 veFillRgrUeTxModeCfg ARGS((
RgrUeTxModeCfg *ueCfg
));

EXTERN S16 veFillRgrUeDlCqiCfg ARGS((
RgrUeDlCqiCfg *ueCfg,
VeUeCb        *ueCb
));

EXTERN S16 veFillRgrUeUlHqCfg ARGS((
RgrUeUlHqCfg *ueCfg
));

EXTERN S16 veFillRgrUeUlPwrCfg ARGS((
RgrUeUlPwrCfg *ueCfg
));

EXTERN S16 veFillRgrUeQosCfg ARGS((
RgrUeQosCfg *ueCfg
));

EXTERN S16 veFillRgrUeTaTmrCfg ARGS((
RgrUeTaTmrCfg *ueCfg
));

EXTERN S16 veFillRgrUeDrxCfg ARGS((
RgrUeDrxCfg *ueCfg
));


EXTERN S16 veFillRgrUeAckNackRepCfg ARGS((
RgrUeAckNackRepCfg *ueCfg
));

EXTERN S16 veFillRgrUeMeasGapCfg ARGS((
RgrUeMeasGapCfg *ueCfg
));

EXTERN S16 veFillRgrUeCapCfg ARGS((
RgrUeCapCfg *ueCfg
));

EXTERN S16 veFillRgrCodeBookRstCfg ARGS((
RgrCodeBookRstCfg *ueCfg
));

#ifdef LTE_TDD
EXTERN S16 veFillRgrTddAckNackMode ARGS((
RgrTddAckNackMode *ueCfg
));
#endif /* LTE_TDD */

EXTERN S16 veSndRgrUeCfg ARGS((
VeUeCb        *ueCb
));

EXTERN S16 VeLiEgtEguLclTnlMgmtCfm ARGS((
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
));

EXTERN S16 VeLiEgtEguDatInd ARGS((
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
));

EXTERN S16 VeLiEgtEguStaCfm ARGS((
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
));

EXTERN S16 VeLiEgtEguUStaInd ARGS((
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
));

EXTERN S16 VeLiEgtEguErrInd ARGS((
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
));

EXTERN S16 VeLiPjuBndCfm ARGS((
Pst *pst,
SuId suId,
U8 status
));

EXTERN S16 VeLiPjuDatCfm ARGS((
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatCfmInfo  *datCfm
));

EXTERN S16 VeLiPjuStaInd ARGS((
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuStaIndInfo  *staInd,
Buffer         *mBuf
));

EXTERN S16 VeLiPjuDatFwdInd ARGS((
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatFwdIndInfo   *datFwdInd
));


EXTERN S16 veInactivityDetected ARGS ((VeEuUeCb    *euUeCb)); 

EXTERN S16 veChngTnlState ARGS ((Pst *pst, SuId suId,VeEuChngTnlStInfo * stInfo));

/* ve_bldmsg.c */
EXTERN S16 veGetPMICfgIdx ARGS((TknU32 *tkn));
EXTERN S16 veGetSRCfgIdx ARGS((TknU8 *tkn));
EXTERN S16 veFillRRC_ConSetup ARGS((NhuDatRspSdus *nhuDatRspSdu,  VeUeCb *ueCb));
/* RRC-ReEstab */
EXTERN S16 veFillRRC_ConReest ARGS((NhuDatRspSdus *nhuDatRspSdu,VeUeCb *ueCb));
EXTERN S16 veFillRRC_ConReestRej ARGS((NhuDatRspSdus *nhuDatRspSdu,VeUeCb *ueCb));
EXTERN S16 veLIMRrcConReestabCmpHndlr ARGS ((NhuDatIndSdus *datIndSdu/* configuration */));
EXTERN S16 veUtlSztFillERABFailRspFromERABSetReqPdu ARGS((VeS1ConCb *s1apCon,S1apPdu **pdu));
EXTERN S16 veUtlSztFillERABRlsFailRspFromERABRlsCmmdPdu ARGS((VeS1ConCb *s1apCon,S1apPdu **pdu));
/*TODO:shishir*/
EXTERN S16 veFillRRC_ConReestRej ARGS((NhuDatRspSdus *nhuDatRspSdu,VeUeCb *ueCb));
EXTERN S16 veLIMRrcConReestabCmpHndlr ARGS ((NhuDatIndSdus *datIndSduconfiguration ));
EXTERN S16 veUtlSztFillERABFailRspFromERABSetReqPdu ARGS((VeS1ConCb *s1apCon,S1apPdu **pdu));
EXTERN S16 veUtlSztFillERABRlsFailRspFromERABRlsCmmdPdu ARGS((VeS1ConCb *s1apCon,S1apPdu **pdu));
EXTERN S16 veFillRRC_DlInfTr ARGS((NhuDatReqSdus *nhuDatReqSdu,U16 pduLen,U8 *nasPdu, U8 pdcpId));
EXTERN S16 veFillRRC_SecModeCmd ARGS((NhuDatReqSdus *nhuDatReqSdu, VeUeCb *ueCb));
EXTERN S16 veFillRRC_ConRls ARGS(( NhuDatReqSdus *nhuDatReqSdu));
EXTERN S16 veFillRRC_ConRej ARGS(( NhuDatRspSdus *nhuDatRspSdu,U32 waitTime));
EXTERN S16 veFillNhuRrcConReCfgn ARGS((VeUeCb *ueCb, NhuDatReqSdus *nhDatReqEvntSdu, U8 s1Event));
EXTERN S16 veFillNhuRrcConReCfgnWithMbuf ARGS(( VeUeCb  *ueCb, NhuDatReqSdus
         *nhDatReqEvntSdu, Buffer  *mBuf));

EXTERN S16  veFillRRCReCfgFromERABSetupReq ARGS((NhuDatReqSdus           *evnt, 
                     VeUeCb *ueCb, NhuRRCConRecfgn_r8_IEs *reCfgIes, 
                     SztProtIE_Field_E_RABSetupRqstIEs *member));
EXTERN S16 veFillRRCReCfgFromERABRelCmd  ARGS((NhuDatReqSdus           *evnt,
                     VeUeCb     *ueCb,NhuRRCConRecfgn_r8_IEs *reCfgIes,
                     SztProtIE_Field_E_RABRlsCmmdIEs *member));
/* ADD CR_ID:ccpu00117391 ADD ERAB_Release */
EXTERN S16 veFillRRCReCfgNASFromERABRelCmd  ARGS((NhuDatReqSdus           *evnt,
                     NhuRRCConRecfgn_r8_IEs *reCfgIes,
                     SztProtIE_Field_E_RABRlsCmmdIEs *member));

EXTERN S16 veSndRgrDelLchCfg ARGS((
VeUeCb        *ueCb,
VeUeRAB       *rabInfo
));




EXTERN S16 veUtlSztFillERABRspFromERABSetReqPdu ARGS((VeS1ConCb *s1apCon, 
                                                      S1apPdu      **pdu)); 
/* 30_12_11 Pradeep */
EXTERN S16 veUtlSztFillERABRspFromERABRelCmdPdu ARGS((VeS1ConCb *s1apCon, 
                                                      S1apPdu      **pdu)); 

EXTERN S16 veFillRRCReCfgFromInitCntxtERABSetup ARGS((Ptr  evnt, VeUeCb *ueCb, NhuRRCConRecfgn_r8_IEs *reCfgIes,SztProtIE_Field_InitCntxtSetupRqstIEs *member));
EXTERN S16 veFillRRCPagingMsg ARGS(( NhuDatReqSdus *datReqSdus, VeUePagingInfo   *uePagingInfo ));

EXTERN S16 veFillNhuSRBAddModLst ARGS((NhuRadioResourceConfigDedicated *radioResCfg, Ptr         evnt, U8          srbId));
EXTERN S16 veFillNhuMACMainCfg ARGS((NhuRadioResourceConfigDedicated *radioResCfg,Ptr         evnt, U8          rbId));
EXTERN S16 veFillRadResPhyCfgDed ARGS((NhuRadioResourceConfigDedicated *radioResCfg, Ptr evnt,U8 rbId,VeUeCb *ueCb));
EXTERN S16 veFillNhuUlUmRlc ARGS((NhuUL_UM_RLC  *ulUmRlc));
EXTERN S16 veFillNhuDlUmRlc ARGS((NhuDL_UM_RLC  *dlUmRlc));
EXTERN S16 veFillUeAddRbCfg ARGS((NhuRbCfgList  **rbCfgList,VeUeCb        *ueCb,U8            msgType));
EXTERN S16 veFillNhuUeCfgInfo ARGS((Ptr           evnt, NhuUeCfgInfo  *ueCfg, VeUeCb *ueCb,U8 msgType));
EXTERN S16 veFillPdcpCfg ARGS((NhuPdcpCfg    *pdcpCfg, U8            rbId, U8 cfmReqd));
EXTERN S16 veFillTrChCfg ARGS((NhuTrChCfg    *trChCfg));
EXTERN S16 veFillSecCfg ARGS((NhuSecurityCfg     *secCfg, VeUeCb  *ueCb));
EXTERN S16 veFillUlLogChCfg ARGS((NhuUlLogChMap *ulLogChMap,U8 rbId));
EXTERN S16 veFillUlRlcModeInfo ARGS((NhuUlRlcMode  *ulRlcMode,U8  rabId));
EXTERN S16 veFillDlRlcModeInfo ARGS((NhuDlRlcMode  *dlRlcMode,U8  rabId));
EXTERN S16 veFillNhuEvntHdr ARGS((NhuHdr      *nhuHdr, U32 cellId,U16 crnti, U8
         msgType));
/* NAS NON Delivery Indication start */
EXTERN S16 veInsrtNasPduToLst ARGS((VeUeCb *ueCb, TknStrOSXL  *nasPdu, U32 transId)); 
EXTERN S16 veRemoveNasPduFrmLst ARGS((VeUeCb *ueCb, U32 transId));
EXTERN S16 veDelAllNasPduFrmLst ARGS((VeUeCb *ueCb));
/* NAS NON Delivery Indication end */
typedef S16 (* VeSztLIMFSM)    ARGS((VeS1ConCb *s1ConCb,S1apPdu *pdu));

#endif

EXTERN void  veUeConStopTmr ARGS((PTR cb, S16 event));
EXTERN void  veUeConStartTmr ARGS((PTR cb,S16 tmrEvnt,U32 delay));
EXTERN S16 veSendUeCnclReq ARGS((VeUeCb *ueCb));
EXTERN S16 veFillTknU32 ARGS((TknU32 *encParam,U32 val));
EXTERN S16 veFillTknU8 ARGS((TknU8 *encParam,U8 val));
EXTERN S16 veFillTknU16 ARGS((TknU16 *encParam,U16 val));
EXTERN S16 veCpyCmTptAddr ARGS((CmTptAddr *dst, CmTptAddr *src));
EXTERN S16 veSztUtilsGetMsgIdx ARGS((U8 *msgIdx,S1apPdu *pdu));
EXTERN S16 veFillTknStr4 ARGS ((TknStr4 *ptr, U8 len, U8 val));
EXTERN S16 veFill2TknStr4 ARGS ((TknStr4 *ptr, U8 len, U8 *val));
EXTERN S16 veFillTknBitStr4 ARGS ((TknStr4 *ptr, U8 len, U8 *val));
EXTERN S16 veFillTknStrOSXL ARGS ((TknStrOSXL *ptr, U16 len, Data *val, S1apPdu  **pdu));
EXTERN S16 veFillTknStrOSXL1 ARGS ((TknStrOSXL *ptr, U16 len, U32 val, S1apPdu  **pdu));
EXTERN S16 veFillTknBStr32 ARGS ((TknBStr32  *ptr, U16  len, U32  val));
EXTERN S16 veFillEnbId ARGS ((TknBStr32  *ptr, U16  len, U32  val));
EXTERN S16 veFillTknStrBSXL ARGS ((TknStrBSXL *ptr, U16  len, Data  *val, S1apPdu    **pdu));


#ifdef EGTP_U
EXTERN S16 PtLiEgtEguLclTnlMgmtReq ARGS(( Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 PtLiEgtEguStaReq ARGS(( Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 PtLiEgtEguDatReq ARGS(( Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 EuLiEgtEguLclTnlMgmtReq ARGS(( Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 EuLiEgtEguStaReq ARGS(( Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 EuLiEgtEguDatReq ARGS(( Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 EuLiEgtEguLclTnlMgmtCfm ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 EuLiEgtEguStaCfm ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 EuLiEgtEguUStaInd ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 EuLiEgtEguErrInd ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
EXTERN S16 VeLiEgtBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 EuLiEgtBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
#endif

EXTERN S16 VeLiNhuDatReq  ARGS(( Pst *pst, SpId spId, NhuDatReqSdus *nhuDatReqSdus ));
EXTERN S16 VeLiNhuDatRsp  ARGS(( Pst *pst, SpId spId, NhuDatRspSdus *nhuDatRspSdus ));
EXTERN S16 VeLiNhuBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 VeLiNhuDatInd ARGS(( Pst *pst, SuId suId, NhuDatIndSdus *nhuDatIndSdus));
EXTERN S16 VeLiNhuDatCfm ARGS(( Pst *pst, SuId suId, NhuDatCfmSdus *nhDatCfmSdus));
EXTERN S16 VeLiNhuStaInd ARGS(( Pst *pst, SuId suId, NhuStaIndSdus *nhStaIndSdus));
EXTERN S16 VeLiNhuErrInd ARGS(( Pst *pst, SuId suId, NhuErrIndSdus *nhErrIndSdus));
EXTERN S16 VeLiNhuCfgCfm ARGS(( Pst *pst, SuId suId, NhuCfgCfmSdus *nhuCfgCfmSdus));
EXTERN S16 VeLiNhuCellCfgCfm ARGS(( Pst *pst, SuId suId, NhuCellCfgCfmSdus *nhuCellCfgCfmSdus));
EXTERN S16 VeLiNhuDecodeCfm ARGS(( Pst *pst, SuId suId, NhuDecodeSduCfm *nhuDecodeSduCfm));
EXTERN S16 VeLiNhuCountCfm ARGS(( Pst *pst, SuId suId, NhuDRBCountInfoLst *nhuDRBCountInfoLst));
EXTERN S16 VeLiNhuEncodeCfm ARGS(( Pst *pst, SuId suId, NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 VeLiNhuPdcpSduStaCfm ARGS(( Pst *pst, SuId suId, NhuPdcpSduStaCfm *nhuPdcpSduStaCfm));

EXTERN S16 VeLiRgrBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 VeLiRgrCfgCfm ARGS(( Pst *pst, SuId suId, RgrCfgTransId  transId,U8 status));
EXTERN S16 VeLiCtfBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 VeLiCtfCfgCfm ARGS(( Pst *pst, SuId suId, CtfCfgTransId transId,U8 status));
EXTERN S16 VeLiCtfUeIdChgCfm ARGS(( Pst *pst, SuId suId, CtfCfgTransId transId,CtfUeInfo *ueInfo,U8 status));

/* E-RAB Modify */
EXTERN S16 veFillRRCReCfgFromERABMdfyReq ARGS(( NhuDatReqSdus *evnt, VeUeCb *ueCb, NhuRRCConRecfgn_r8_IEs *reCfgIes, SztProtIE_Field_E_RABMdfyRqstIEs *member));
EXTERN S16 veUtlSztFillERABMdfyRsp ARGS(( VeS1ConCb *s1apCon, S1apPdu **pdu, Bool rspType ));


EXTERN S16 veSndEgtpCom ARGS((U8 egtpCom, VeS1ConCb *s1apConCb));
EXTERN S16 veSndEgtpComDed ARGS((U8 egtpCom, VeS1ConCb *s1apConCb));
EXTERN Void veSendLmAlarm ARGS((U16 category, U16 event, U16 cause));
EXTERN S16 veSzGetIE ARGS(( SztS1AP_PDU *pdu, U32 id, TknU8 **ie));
#ifndef VE_SRB2_SUPPORT
EXTERN S16 veInitUeSrbInfo ARGS(( VeUeRABInfo *srbCfg));
#else
EXTERN S16 veInitUeSrbInfo ARGS(( VeUeRABInfo *srbCfg, U8 rabId));
#endif

#ifdef LTE_HO_SUPPORT
typedef S16 (*VeS1SrcFSM) ARGS((VeS1ConCb *s1ConCb, void *ptr));
typedef S16 (*VeS1TgtFSM) ARGS((VeS1ConCb *s1ConCb, void *ptr));
typedef S16 (*VeX2SrcFSM) ARGS((VeX2ConCb *x2ConCb, void *ptr));
typedef S16 (*VeX2TgtFSM) ARGS((VeX2ConCb *x2ConCb, void  *ptr));
typedef S16 (* VeCztLIMFSM) ARGS((VeX2ConCb *x2ConCb,CztEvnt *cztInd));

EXTERN S16 VeDatCreatDatFwdEgtpTunel ARGS((Pst *pst, SuId suId, EuCrEGtpTunInf *tunelInf));
EXTERN S16 VeDatCreateEgtpTunelSet ARGS((Pst *pst, SuId suId, EuCrEGtpTunInf *tunelInf));
EXTERN S16 VeHdlCreatDatFwdEgtpTunelCfm ARGS((Pst *pst, SuId suId, EuCrEGtpTunInf *tunelInf));
EXTERN S16 VeDatCreateEgtpTunelSetCfm ARGS((Pst *pst, SuId suId, EuCrEGtpTunInf *tunelInf));
EXTERN S16 veLiSztHoPrepFlrHandler ARGS((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
EXTERN S16 veLiSztHoRqstHandler ARGS((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
EXTERN S16 veLiSztHoPthSwthReqAckHandler ARGS((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
EXTERN S16 veLiSztHoPthSwthReqFlrHandler ARGS((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
EXTERN S16 veLiSztCnclAckHandler ARGS((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
EXTERN S16 veLiSztMmeStsTfrHandler ARGS((VeS1ConCb *s1ConCb, S1apPdu  *pdu));
EXTERN S16 veLiSztHoCmdHandler ARGS (( VeS1ConCb *s1ConCb, S1apPdu   *pdu));

PUBLIC S16 veCztX2SetupReq ARGS((U32 peerId));
PUBLIC S16 veUtlSztFillHovrReqd ARGS ((VeS1ConCb *s1Con,S1apPdu **pdu,VeNghCellCb *nghCellCb, NhuEncCfmSdus *nhuEncCfmSdus));
PUBLIC S16 veUtlSztFillHovrReqAck ARGS ((VeS1ConCb *s1Con, S1apPdu **pdu, VeUeRABInfo rabInfo, NhuEncCfmSdus *nhuEncCfmSdus));
PUBLIC S16 veUtlSztFillHovrNtfy ARGS ((VeS1ConCb *s1Con, S1apPdu **pdu, VeCellCb *cellCb));
PUBLIC S16 veUtlSztFillENBStatusTfr ARGS ((VeS1ConCb *s1Con, S1apPdu **pdu, U8 numBearer, NhuPdcpSduStaCfm *nhuPdcpSduStaCfm));
PUBLIC S16 veUtlSztFillHovrFail ARGS ((VeS1ConCb *s1Con, S1apPdu **pdu));
PUBLIC S16 veUtlSztFillPathSwRqst ARGS ((VeS1ConCb *s1Con, S1apPdu **pdu));
PUBLIC S16 veFillErabFrmS1HoReq ARGS ((VeUeRABInfo *rbInfo, SztE_RABToBeSetupLstHOReq *erabLst, VeMmeCb *mmeCb));
PUBLIC S16 veFillErabFrmS1HoAck ARGS ((VeUeRABInfo *rbInfo, SztS1AP_PDU *s1HoAckPdu, VeMmeCb *mmeCb));
PUBLIC S16 veUpdUlTunPthSwtAck ARGS ((VeUeRABInfo *rbInfo, SztS1AP_PDU *s1PthSwtAckPdu));
PUBLIC S16 veHoSndPathSwtchReq ARGS((VeUeCb *ueCb));
PUBLIC S16 veHoSndX2HOCncl ARGS((VeX2ConCb *x2ConCb));
PUBLIC S16 veHoSndX2SNTfr ARGS((VeX2ConCb *x2ConCb,PTR  pdcpSduInfo));
PUBLIC S16 veHoSndS1SNTfr ARGS((VeS1ConCb *s1ConCb, NhuPdcpSduStaCfm *nhuPdcpSduStaCfm));
PUBLIC S16 VeLiCztGpReq ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PUBLIC S16 veHoCfgTunls ARGS ((VeUeCb *ueCb, U8 tunCreateType));
PUBLIC S16 veFillPjuDatFwdReq ARGS((PjuDatFwdReqInfo **pjuDatFwdReq, EgtUEvnt *eguMsg, U8 dir));
PUBLIC S16 veFillEgtpDatFwdMsg ARGS ((EgtUEvnt **eguEvtMsg, VeEgtpTunCb *egtpCb, PjuDatFwdInfo *datFwdInfo));
PUBLIC S16 veUtlFillHoCmdTrnspCntrWthMobility ARGS((VeUeCb *ueCb, NhuEncReqSdus *hoCmdEncReq, U16 crnti, U16 physCellId));
PUBLIC S16 veUtlFillHoCmdTrnspCntr ARGS((VeUeCb *ueCb, NhuEncReqSdus *hoCmdEncReq,NhuEncCfmSdus *nhuEncCfmSdus));
PUBLIC S16 VeLiCztBndCfm ARGS ((Pst *pst, SuId suId, U8 status));
PUBLIC S16 VeLiCztGpInd ARGS ((Pst *pst, SuId suId, CztEvnt *cztInd));
PUBLIC S16 VeLiCztGpCfm ARGS((Pst *pst, SuId suId, CztEvnt *cztCfm));
PUBLIC S16 VeLiCztBmpInd ARGS((Pst *pst, SuId suId, CztEvnt *cztInd));
PUBLIC S16 veUtlFillHoCmdTrnspCntrWthMobility ARGS ((VeUeCb *ueCb, NhuEncReqSdus *hoCmdEncReq, U16 crnti, U16 physCellId));
PUBLIC U8 veCztUtilsGetMsgIdx ARGS ((CztX2AP_PDU *pdu));
PUBLIC S16 VeLiCztBmpCfm ARGS((Pst *pst, SuId suId, CztEvnt *cztCfm));
PUBLIC S16 VeLiCztGpErrInd ARGS((Pst *pst, SuId suId, CztEvnt *cztEvnt));
PUBLIC S16 VeLiCztLclErrorInd ARGS((Pst *post, SuId suId, CztLclErrInd *cztLclErrInd));
PUBLIC S16 VeLiCztRelCfm ARGS((Pst *pst, SuId suId, CztRlsCfm *rlsCfm));
PUBLIC S16 VeLiCztStaInd ARGS ((Pst *post, SuId suId, CztStaInd *cztSta));
PUBLIC S16 VeLiCztDynamPeerAssocInd ARGS((Pst *pst, SuId suId, CztPeerAssocInd *peerAssocInd));
PUBLIC S16 VeLiCztAudCfm ARGS ((Pst *pst, SuId suId, CztAudEvnt *audEvnt));
PUBLIC S16 veSndCmnNhuDatReq ARGS ((U8 msgCategory, U8 msgType, CmLteTimingInfo tmnInf));
PUBLIC S16 veUtlFillHoPrepTrnspCntr ARGS ((VeUeCb *ueCb, NhuEncReqSdus *hoPrepEncReq));
PUBLIC S16 VeLiCztBndReq ARGS ((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 veCztX2SetupReq ARGS((U32 peerId));
PUBLIC S16 veRlsDedCrnti ARGS ((U16 rnti));
PUBLIC S16 veGetDedPrmbl ARGS ((U16 *prmblId));
PUBLIC S16 veRlsDedPrmbl ARGS((U16 prmblId));
PUBLIC S16 veUtlFillHoPreparationMsg ARGS ((VeUeCb *ueCb, NhuEncReqSdus *hoCmdEncReq));
PUBLIC S16 VeLiCztBndReq ARGS ((Pst *pst, SuId suId, SpId spId));
PUBLIC S16 VeLiCztUbndReq ARGS ((Pst *pst, SpId spId, Reason reason));
PUBLIC S16 VeLiCztGpRetxReq ARGS((Pst *pst, SpId spId, CztReTxInfo *cztReTxInfo));
PUBLIC S16 VeLiCztGpReq ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PUBLIC S16 VeLiCztGpRsp ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PUBLIC S16 VeLiCztGpErrReq ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PUBLIC S16 VeLiCztRelReq ARGS((Pst *pst, SpId spId, CztRlsReq *relReq));
PUBLIC S16 VeLiCztBmpReq ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PUBLIC S16 VeLiCztBmpRsp ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PUBLIC S16 VeLiCztDynamPeerAssocRsp ARGS((Pst *pst, SpId spId, CztPeerAssocRsp *peerAssocRsp));
PUBLIC S16 VeLiCztAudReq ARGS ((Pst *pst, SpId spId, CztAudEvnt *audEvnt));
PUBLIC S16 smBuildVeX2InitCntrl ARGS((Void));


EXTERN S16 veNghCfg ARGS((VeNghCellCfgLst *nghCfgLst,CmStatus *status, Pst *pst));
EXTERN S16 veUtlSztFillCnclReq ARGS(( S1apPdu **pdu, VeS1ConCb *s1Con));

EXTERN S16 veHOFillCandCells ARGS (( VeUeCb *ueCb, NhuMeasResults *measResults));
EXTERN S16 veHOFndInNghLstByPci ARGS (( VeCellCb *cellCb, U32 physCellId, VeNghCellCb **neighCellCb));
EXTERN S16 veHOFndInNghLstByPeerId ARGS (( VeCellCb *cellCb, U32 peerId, VeNghCellCb **neighCellCb));
EXTERN S16 veHOAddToCandLst ARGS (( VeUeCb *ueCb, VeNghCellCb *neighCellCb));

EXTERN S16 veUpdCandLstSndCncl ARGS (( VeUeCb    *ueCb));
EXTERN S16 veClrCandLst ARGS ((CmLListCp *cndLst));
EXTERN S16 veHoCfgFwdTunls ARGS (( VeUeCb    *ueCb));
EXTERN S16 veHoProcHoCfm ARGS (( VeUeCb *ueCb));
EXTERN S16 veHoPrcUeHoAttachTmrExpiry ARGS (( VeUeCb *ueCb));

EXTERN S16 vePdcpSnStsUpd ARGS(( VeUeCb *ueCb,VeSnStsInfo   *snStsInfo ));
 
EXTERN S16 veSndX2UeCtxtRls ARGS(( VeX2ConCb   *x2ConCb, PTR *notused));

EXTERN S16 veGetPdcpSnSts ARGS(( VeUeCb    *ueCb));

PUBLIC S16 VeLiCztBmpReq ARGS ((Pst *pst, SpId spId, CztEvnt *cztEvnt));

EXTERN S16 veLiCztHoRqstHandler ARGS((VeX2ConCb *x2ConCb,CztEvnt   *cztInd));
EXTERN S16 veLiCztHoAckHandler ARGS((VeX2ConCb *x2ConCb,CztEvnt   *cztInd));

EXTERN S16 veLiCztHoFlrHandler ARGS(( VeX2ConCb *x2ConCb, CztEvnt   *pdu));
EXTERN S16 veLiCztHoCnclHandler ARGS(( VeX2ConCb *x2ConCb, CztEvnt   *pdu));
EXTERN S16 veLiCztSnStsTrsfrHandler ARGS(( VeX2ConCb *x2ConCb, CztEvnt   *pdu));
EXTERN S16 veLiCztUeCntxtRlsHandler ARGS(( VeX2ConCb *x2ConCb, CztEvnt   *pdu));
EXTERN S16 veLiCztX2SetupRqstHandler ARGS(( VeX2ConCb *x2ConCb, CztEvnt   *pdu));
EXTERN S16 veLiCztX2SetupRspHandler ARGS(( VeX2ConCb *x2ConCb, CztEvnt   *pdu));
EXTERN S16 veLiCztX2SetupFlrHandler ARGS(( VeX2ConCb *x2ConCb, CztEvnt   *pdu));
EXTERN S16 veUtlGetCztIE ARGS ((CztX2AP_PDU *pdu, U32 id, TknU8 **ie));
EXTERN S16 veCztNoOpr ARGS ((VeX2ConCb *x2ConCb,CztEvnt  *pdu));

EXTERN S16 veCzFillTknStrOSXL ARGS ((TknStrOSXL  *ptr, U16 len, Data *val, CztEvnt **pdu));
EXTERN S16 veCzFillTknStrOSXL1 ARGS ((TknStrOSXL  *ptr, U16 len, U32 val, CztEvnt **pdu));
EXTERN S16 veCzFillTknStrBSXL ARGS((TknStrBSXL  *ptr,U16 len, Data *val, S1apPdu **pdu));

EXTERN S16 veUtlCztFillX2SetupReq ARGS((CztEvnt   **pdu));
EXTERN S16 veUtlCztFillX2SetupRsp ARGS((CztEvnt   **pdu));
EXTERN S16 veUtlCztFillX2SetupFail ARGS((CztEvnt  **pdu, U16 cause, U8 causeType, Bool sndTimeToWait, U32 timeToWait));
EXTERN S16 veUtlCztFillUeCxtRls ARGS((CztEvnt     **pdu, U32 oldUEX2APId, U32 newUEX2APId));
EXTERN S16 veUtlCztFillHovrRqst ARGS((CztEvnt **pdu, VeNghCellCb *nghCellCb, VeX2ConCb *x2ConCb, NhuEncCfmSdus *nhuEncCfmSdus));
EXTERN S16 veUtlCztFillHovrPrepFail ARGS((CztEvnt **pdu, VeX2ConCb *x2ConCb));
EXTERN S16 veUtlCztFillHovrCancel ARGS ((CztEvnt **pdu, VeX2ConCb  *x2ConCb,Bool isNeweNBUeX2apIdPres));
EXTERN S16 veUtlCztFillSnStatusTrfr ARGS ((CztEvnt **pdu, VeX2ConCb *x2ConCb, U8 numBearer, NhuPdcpSduStaInfo *nhuPdcpSduStaInfo));
EXTERN S16 veFillErabFrmX2HoReq ARGS ((VeUeRABInfo *rbInfo, CztE_RABs_ToBeSetup_Lst *erabLst, VeNghCellCb *nghCellCb, VeMmeCb *mmeCb));
EXTERN S16 veFillErabFrmX2HoAck ARGS ((VeUeCb *ueCb, CztX2AP_PDU *x2HoAckPdu));
EXTERN S16 veUtlCztFillHovrRqstAck ARGS ((CztEvnt **pdu, VeX2ConCb *x2ConCb, NhuEncCfmSdus  *nhuEncCfmSdus, U8 noofBearer, VeUeRAB *ueRab)); 
EXTERN  S16 veCzFillNghInfoFrmX2SetReq ARGS ((CztEvnt *x2SetReq, VeNghCellCb *nghCellCb));
EXTERN  S16 veCzFillNghInfoFrmX2SetRes ARGS ((CztEvnt *x2SetRes, VeNghCellCb *nghCellCb));


EXTERN S16 veHODecision ARGS (( VeCellCb *cellCb, NhuMeasResults *measResults, Bool  *hoNeeded));
EXTERN S16 veHoSndS1HOCncl ARGS (( VeUeCb    *ueCb));


EXTERN S16 veGetDedCrnti ARGS(( U16 *rnti));
EXTERN S16 veAdmnCntrlReq ARGS((Void ));
EXTERN S16 veFillNhuMeasCfgInfo ARGS((NhuMeasConfig *measCfg, Ptr         evnt));

EXTERN VeS1SrcFSM veS1SrcFSM[VE_HO_MAX_STATES][VE_HO_SRC_MAX_EVENTS];
EXTERN VeS1TgtFSM veS1TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS];
EXTERN VeX2SrcFSM veX2SrcFSM[VE_HO_MAX_STATES][VE_HO_SRC_MAX_EVENTS];
EXTERN VeX2TgtFSM veX2TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS];

/* X2_CFG_UPD_CHANGES */
EXTERN S16 veCztFillCfgUpdReq ARGS((VeCfgUpdInfo *updInfo, CztEvnt   **pdu));
EXTERN S16 veFillConfigUpdateAck ARGS((
   CztEvnt                   **pdu,
   VeIeInfo                  *idLst
));
EXTERN S16 veFillConfigUpdateFail ARGS((
   CztEvnt                  **pdu,
   VeIeInfo                  *idLst,
   U16                       cause,
   U8                        causeType,
   U32                       timeToWait
));

EXTERN PUBLIC S16 veFillErrInd ARGS((
  CztEvnt                    **pdu,
  U32                        oldeNBUeX2apId,
  U32                        neweNBUeX2apId,
  Bool                       causePres,
  U8                         causeType,
  U8                         causeVal,
  VeIeInfo                   *idLst
));


EXTERN S16 veCztCfgUpdReq ARGS((
U32 peerId,
VeCfgUpdInfo *veCfg
));

EXTERN S16 veCztErrInd ARGS((
U32                        peerId,
U32                        oldeNBUeX2apId,
U32                        neweNBUeX2apId,
Bool                       causePres,
U8                         causeType,
U8                         causeVal,
VeIeInfo                   *idLst
));


EXTERN S16 veLiCztCfgUpdReqHandler ARGS((
VeX2ConCb *x2ConCb,
CztEvnt   *pdu
));

EXTERN S16 veLiCztCfgUpdAckHandler ARGS((
VeX2ConCb *x2ConCb,
CztEvnt   *cfgAckPdu
));

EXTERN S16 veLiCztCfgUpdFailHandler ARGS((
VeX2ConCb *x2ConCb,
CztEvnt   *cfgFailPdu
));


EXTERN S16 veLiCztErrIndHandler ARGS((
VeX2ConCb *x2ConCb,
CztEvnt   *cfgErrIndPdu
));

/* X2_CFG_UPD_CHANGES END*/

#endif /* end of LTE_HO_SUPPORT */

#ifdef VE_PERF_DL_DATA
EXTERN Void vePerfTmrExp ARGS((PTR pCb, S16 tmrEvnt));
#endif

EXTERN S16 veFetchUeCntxModSecInfo ARGS((VeUeCb        *ueCb,
                                         S1apPdu       *s1apPdu));


EXTERN S16 veGetBitRateUeCntxtMod   ARGS((VeUeCb        *ueCb,
                                         SztUECntxtModificationRqst   *s1apMsg,
                                         U8                            *upd));

EXTERN S16 veSndRrcReCfgReqAftrReEst ARGS((VeUeCb *ueCb));
 
EXTERN S16 veUtlFillErrInd ARGS((S1apPdu      **pdu, 
                                 U32          cause,
                                 U8           choice, 
                                 U32          *eNbUeId,
                                 U32          *mmeUeId));

EXTERN S16 veSndUeCntxModFail ARGS((VeS1ConCb *s1ConCb));

EXTERN S16 veUpdSndUeCtxModInfo ARGS((VeUeCb   *ueCb,
                                      S1apPdu  *pdu));

EXTERN S16 veHdlEnbReset ARGS((U32       peerId,
                               U8        choice,
                               U32       cause,
                               U16       numCons,
                               U8        rstType,
                               VeS1ConId *conIds));

EXTERN S16 veUtlFillUeCntxModFail ARGS ((S1apPdu **pdu,
                                   U32          eNbId, 
                                   U32          mmeId,
                                   U32          cause,
                                   U8           choice));

EXTERN S16 veHdlEnbRsetTimOut ARGS ((VeMmeCb   *mmeCb));
#ifdef VE_PERF_MEAS

PUBLIC S16 vePerfLIMRrcConSetCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));

PUBLIC S16 vePerfSndSecModeCmnd ARGS((VeUeCb *ueCb));

PUBLIC S16 vePerfFillRRC_SecModeCmd ARGS ((NhuDatReqSdus *nhuDatReqSdu, VeUeCb *ueCb));

PUBLIC S16 vePerfFillUeCfgInfo ARGS((VeUeCb        *ueCb));

#endif  /* VE_PERF_MEAS */
PUBLIC S16 veMonEnbOvldTmrExpiry(void);
PUBLIC S16 veUtlSelMmeCbByMmec(U8 mmec, VeMmeCb **veMmeCb);
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VEX__ */

/********************************************************************30**
  
         End of file:     ve.x@@/main/2 - Wed Dec  1 08:44:21 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$      ----       mnawas        1. eNodeb 3.1 Release
*********************************************************************91*/
