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


/**********************************************************************

     Name:     LTE-RRC User
  
     Type:    
  
     Desc:     

     Ret :     ROK - success
               RFAILED - failure

     File:     lve.x

     Sid:      lve.x@@/main/5 - Mon Feb 20 15:54:20 2012

     Prg:  

**********************************************************************/
#ifndef __LVEX__
#define __LVEX__

#ifdef __cplusplus
extern "C" {
#endif

/* lve_x_001.main_4: Added new structure and enum */   
#ifdef LTE_HO_SUPPORT
typedef struct veEcgiCfg
{
   /* 24 bit long */
   U32 plmnId;
   /* 28 bit long */
   U32 eutranCellId;
} VeEcgiCfg;

typedef struct veGuGrpIdCfg
{
   /* 24 bit long */
   U32 plmnId;
   U16 mmeGrpId;
} VeGuGrpIdCfg;

typedef struct veNghInfoCfg
{
   VeEcgiCfg   ecgi;
   U16         physCellId;
   U32         earfcn;
}VeNghInfoCfg;
typedef struct veNghCellCfg
{
   U8          enbType;                        /* home / macro eNodeB */
   U32         enbAddr;                        /* eNodeB IP Address */
   U16         physCellId;                     /* Physical Cell ID */
   U16         tac;                            /* Tracking Area Code */
   U32         cellId;                         /* Cell ID */
   U32         plmnId;                         /* PLMN ID - 24 bits */
   U32         peerId;                         /* Peer ID */
   Bool        trgrX2;                         /* flag to indicate whether 
                                                  to trigger the X2SET Up towards this 
                                                  eNB or not */
   Bool        hoAllowed;                       /* Handover is allowed or not */ 
   Bool        dynCfg;                          /* To identify the dynamic cfg */
} VeNghCellCfg;

typedef struct veNghCellCfgLst
{
   Pst           lmPst;  
   U8            noOfCfg;
   VeNghCellCfg  **NghCellCfg;
} VeNghCellCfgLst;

typedef struct _veCellFddCfg
{
   U32 ulEarfcn;
   U32 dlEarfcn;
   U8 ulTxBndWdth;
   U8 dlTxBndWdth;
} VeCellFddCfg;

typedef struct _veCellTddCfg
{
   U32 earfcn;
   U8 txBndWdth;
   U8 sfAssgnmnt;
   U8 splSfPatrn;
   U8 dlCycPrefix;
   U8 ulCycPrefix;
} VeCellTddCfg;

typedef struct _veNghENBCfg
{
  U32            peerId;          /**< Peer identifier. */
  U16            eAValue;         /**< Encryption algorithm supported by
                                          this eNodeB. */
  U16            iPAValue;        /**< Integrity algorithm supported for
                                    this eNodeB. */
  U32            priDstAddr;       /**< Primary destination address. */
  U16            dstPort;         /**< This parameter identifies the
                                    destination port number which, together 
                                    with the dstNAddr, forms
                                    the destination transport address. */
} VeNghENBCfg;

typedef struct _veEcgiVal
{
   /* 24 bit long */
   U32 plmnId;
   /* 28 bit long */
   U32 eutranCellId;
} VeEcgiVal;

typedef struct _veNghInfo
{
   VeEcgiVal   ecgi;
   U16      physCellId;
   U32      earfcn;
} VeNghInfo;

typedef struct _veGuGrpId
{
   /* 24 bit long */
   U32 plmnId;
   U16 mmeGrpId;
} VeGuGrpId;
#endif /* End of LTE_HO_SUPPORT */

typedef enum veeNBType
{
   VE_HOME_ENB,
   VE_MACRO_ENB
}VeeNBType;

#ifdef VE_RELAY
/*----------------------------------------------------------------------
                    Configuration
 ----------------------------------------------------------------------*/

typedef struct veGenUeCfg
{
   /* Security related info */
  U8 intgAlgo;
  U8 ciphAlgo;
} VeGenUeCfg;

typedef struct veCellCfg
{
   /* lve_x_001.main_4: changed the datatype from U16 to U32 */   
   /* Cell cfg related info */
   U32          cellId;
   /* Modulation Type */ 
   U8  modType;
   /* duplexing mode : TDD/FDD/HD_FDD */
   U16 duplexMode;
   U8  maxUeSupp; /* maximum number of UEs supported by each eNodeB */
   /* PLMN Id */
   TknU8 mcc[3];
   TknU8 mnc[3]; /* Token since size could be of 2 to 3 */
   /* Tracking Area Code */
   U16 trackAreaCode;
   /* Frequency Band Indicator */
   U8 freqBandInd;
   VeGenUeCfg genUeCfg;
   /* lve_x_001.main_4: Added new data memberes in the structure */   
   U16 macRntiStart;
   U16 maxMacRntis;
   U8  numOfMacPrm;
   U16 maxRrmRntis;
   U16 rrmPrmblStart;
   U16 maxRrmPrmbls;
   /* PHY configuration parameters */
   U16      opMode;
   U32      counter;
   U32      period;
   U16 physCellId;   /* Physical Cell ID */
   U8  cellGrpId;    /* cell group id */
#ifdef LTE_HO_SUPPORT
   U32 rsrpTrshldRange;            /* Serving Cell threshold 
                                                   RSRP value */
   VeCellFddCfg    fddCfg;                     /* FDD config INFO */
   VeCellTddCfg    tddCfg;                     /* TDD config info */
   U8              noOfBrdCstPlmn;             /* numbre of broadcasr plmns */
   U8              plmnId[MAX_NO_OF_PLMNS][3];

#endif
} VeCellCfg;


#endif
/* lve_x_001.main_4: TIC_ID:ccpu00116701: MOD: Fixed warning for g++ compilation on Linux */
typedef struct VeGenCfg 
{
   Pst         lmPst;  
#ifdef VE_RELAY
   VeCellCfg   cellCfg;
#endif
   CmTptAddr dataAppAddr;
   /* lve_x_001.main_4: Added new data memberes in the structure */   
   VeeNBType eNBType;
#ifdef LTE_HO_SUPPORT
   U16         noOfNghInfo;
   VeNghInfo   **nghInfo;
   U8          noOfguGrp; /* MAX of 16 group items are possible */
   VeGuGrpId   **guGrpId;
   U32         s1PrepTimerVal;             /* Preparation Timer Value */
   U32         s1OvrAllTimerVal;           /* Overall Timer Value */
   U32         ueHoAttachTimerVal;         /* UE Ho Attach Timer Value */
#endif
   U8          enbName[MAX_ENB_NAME_LEN];  /* Enodeb Name Adding */
   /* TODO 
    * Add Cfg for MME/RELAY related operations */
} VeGenCfg;


/* lve_x_001.main_4: TIC_ID:ccpu00116701: MOD: Fixed warning for g++ compilation on Linux */
typedef struct VeLSapCfg
/* TIC_ID:ccpu00116701: MOD_END: Fixed warning for g++ compilation on Linux */
{
   SuId        suId;      
   SpId        spId;   
   Ent         srcEnt;
   Inst        srcInst;
   ProcId      dstProcId; 
   Ent         dstEnt;    
   Inst        dstInst;   
   Priority    dstPrior;  
   Route       dstRoute;  
   Selector    dstSel;    
   Mem         mem;
   U8          maxBndRetry;
   TmrCfg      bndTmr;    
   TmrCfg      connTmr;   
} VeLSapCfg;

typedef struct veCfg
{
   union
   {
      VeGenCfg         genCfg;          /* Framing - General          */
      VeLSapCfg        lSap;            /* Framing - Transport SAP    */
      /* lve_x_001.main_4: Added new data memberes in the structure */   
#ifdef LTE_HO_SUPPORT
      VeNghCellCfgLst  NghCellCfgLst;   /* Framing - Neighbor info */
#endif
   } u;

} VeCfg;

/* lve_x_001.main_2: Adding structures for log file print changes */
#ifdef DEBUGP
/**
 * @struct veDbgCntrl
 * Debug printing control. ENB App layer implementation supports debug logging in
 * two versions:
   - Console output - Debug prints are directed to a console.
   - File output - Debug prints are directed to a file. This option expects
                   the file path and number of debug lines per debug file as 
                   a part of the debug control request.

 */
 /** @brief Debug Printing Control structure parameters*/
typedef struct veDbgCntrl 
{
   U32       dbgMsk;                       /*!< Debug Mask parameter. 
                                            This field can be set to a 
                                            combination of the below allowed 
                                            values by using OR "|" operator.
                                            For example, if the 
                                            debug and info debugging prints  
                                            to be enabled/disabled, the dbgMask
                                            is set to "LVE_DBGMASK_INFO|
                                            LVE_DBGMASK_DBG".
                                              <b> Allowed values </b>
                                                - LVE_DBGMASK_INFO
                                                   To print specific behaviour
                                                   /logic flow.
                                                - LVE_DBGMASK_DBG
                                                   To print extensive logs.
                                                - LVE_DBGMASK_PARAM
                                                   To print function
                                                   parameters values.
                                                - LVE_DBGMASK_ERROR
                                                   To print in error legs.
                                                - LVE_DBGMASK_FATAL
                                                   To print any system effecting
                                                   errors such as resouce
                                                   unavailability.
                                            */

#ifdef VE_SM_LOG_TO_FILE
   Txt       filePath[LVE_MAX_FILE_PATH];  /*!< Path to store log files. */  
   U32       nmbDbgLines;                  /*!< Number of lines per Debug file. */
#endif /* VE_SM_LOG_TO_FILE */
} VeDbgCntrl;
#endif /* DEBUGP */

/* lve_x_001.main_4: Added new structure */   
typedef struct _vex2Reset
{
   U32 peerId;       /*!< Peer eNB Id */
   U8  causeType;    /*!< cause Type */
   U32 causeVal;     /*!< cause Value */
} VeX2Reset;

/*----------------------------------------------------------------------
                       Control related structure
 ----------------------------------------------------------------------*/
/* lve_x_001.main_4: Added new structure */   
/* X2_CFG_UPD_CHANGES*/
typedef struct veCellEcgi
{
   /* 24 bit long */
   U32 plmnId;
   /* 28 bit long */
   U32 eutranCellId;
} VeCellEcgi;

typedef struct veGuId
{
  U32 plmnId;
  U32 mmeGrpId;
} VeGuId;

typedef struct veSrvdCellMod
{
   VeCellEcgi      oldCellEcgi;
   U16             newCellId;
} VeSrvdCellMod;

typedef struct veCfgUpdInfo
{
   U8                        numSrvdCellAdded;
   U8                        numSrvdCellDeleted;
   U8                        numSrvdCellModified;
   U8                        numGrpIdAdded;
   U8                        numGrpIdDeleted;
   U16                       srvdCellAdd[/*VE_MAX_CELL*/1];
   VeSrvdCellMod             srvdCellMod[/*VE_MAX_CELL*/1];
   VeCellEcgi                oldEcgiToDel[/*VE_MAX_CELL*/1];
   VeGuId                    guIdAddLst[/*VE_MAX_GROUP*/1];
   VeGuId                    guIdDelLst[/*VE_MAX_GROUP*/1];
} VeCfgUpdInfo;

typedef struct s1Setup
{
   U32      *peerId;     /* Peerid to which S1Setup needs to be sent */
   U16       numPeers;   /* Number of peer */
}VeS1Setup;

typedef struct veS1Reset
{
   U32    peerId;   /* Peer Id to which reset needs to be sent */
   U16    cause;    /* Cause of the reset */
   U8     type;     /* Type of reset, partial or complete */
   U16     nmbUes;  /* Number of UEs in case of partial reset */
   U32    *conIds;  /* Ue ids incase of partial reset */
}VeS1Reset;

typedef struct veCntrl
{
   Action    action;    
   Action    subAction; 
   union
   {
      SpId       sapId; 
      /* X2_CFG_UPD_CHANGES */
      VeCfgUpdInfo cfgUpdInfo;
      
      VeX2Reset    x2Reset;
#ifdef VE_RELAY
      U16        cellId;
#endif
#ifdef DEBUGP
      U32        dbgMask;
      /* lve_x_001.main_2: Added debug cntrl */
      VeDbgCntrl dbg;        /*!< The layer manager can selectively enable
                               or disable various levels of debug 
                               printing. */
#endif
      U8         oper;
      /* lve_x_001.main_4: Added new data memberes in the structure */   
      VeS1Reset  s1Reset;  /* S1AP reset information */
      VeS1Setup  s1Setup;  /* S1-Setup Information */
      U32        peerId;
   } u;

} VeCntrl;

#ifdef VE_RELAY
typedef struct veUstaDgn
{
   U8         type;            /* Type of USTA alarm */
   union
   {
      SpId       sapId;        /* SAP ID on which event received */
      struct {
         U16 ueId;
         U16 cellId;
      } s;/* Cell and UE ID */
   }u;
} VeUstaDgn;
#endif

typedef struct veUsta
{
   CmAlarm   alarm;            /* Alarms */
#ifdef VE_RELAY
   VeUstaDgn dgn;              /* Alarm diagnostics */
#endif
} VeUsta;


/*----------------------------------------------------------------------
                    Layer configuration
 ----------------------------------------------------------------------*/
typedef struct veMngmt
{
   Header     hdr;   /* Header for mngmt struct           */
   CmStatus   cfm;   /* result of the cntrl/cfg operation */
   union
   {
      VeCfg    cfg;    /* configuration        */
      VeCntrl  cntrl;  /* control              */
      VeUsta    usta;          /* Unsolicited status/alarms */
   } u;

} VeMngmt;



/* lve_x_001.main_4: Added new structure */   
/** @brief This structure is used to store the ENB Overload related Configuration
* Information through OAM Interface.
*/
typedef struct _wrEnbOvldCfg
{
   U8   critThrldVal;            /*!< EnodeB Overload Critical Threshold value */
   U8   majThrldVal;             /*!< EnodeB Overload Major threshold value */
   U8   minThrldVal;             /*!< EnodeB Overload minor threshold value */
   U8   critRcvryVal;             /*!< EnodeB Overload recovery critical threshold  value */
   U8   majRcvryVal;             /*!< EnodeB Overload recovery major threshold  value */
   U8   minRcvryVal;             /*!< EnodeB Overload recovery minor threshold  value */
   U32  eNBOvldMonTimerVal;       /*!< EnodeB Overload monitor timer value */
} WrEnbOvldCfg;


/***********************************************************************

 ***********************************************************************/


typedef S16 (*LveCfgReq)   ARGS((
Pst     *pst, 
VeMngmt *cfg
));

typedef S16 (*LveCfgCfm)   ARGS((
Pst     *pst, 
VeMngmt *cfg
));

typedef S16 (*LveCntrlReq) ARGS((
Pst     *pst, 
VeMngmt *cntrl
));

typedef S16 (*LveCntrlCfm) ARGS((
Pst     *pst, 
VeMngmt *cntrl
));

typedef S16 (*LveStaInd) ARGS((
Pst *pst,
VeMngmt *sta
));

EXTERN S16 cmPkVeGenCfg      ARGS((VeGenCfg *param, Buffer *mBuf));
/* lve_x_001.main_4: Added new parameters in the function prototype */   
EXTERN S16 cmUnpkVeGenCfg    ARGS((VeGenCfg *param, Buffer *mBuf, Pst *pst));
EXTERN S16 cmPkVeLSapCfg     ARGS((VeLSapCfg *param, Buffer *mBuf));
EXTERN S16 cmUnpkVeLSapCfg   ARGS((VeLSapCfg *param, Buffer *mBuf));
EXTERN S16 cmPkVeCfg         ARGS((VeCfg *param, U16 element, Buffer *mBuf));
EXTERN S16 cmUnpkVeCfg       ARGS((VeCfg *param, U16 element, Buffer *mBuf, Pst *pst));
EXTERN S16 cmPkVeCntrl       ARGS((VeCntrl *param, U16 elmnt, Buffer *mBuf));
EXTERN S16 cmUnpkVeCntrl     ARGS((VeCntrl *param, U16 elmnt, Buffer *mBuf, Pst *pst));
EXTERN S16 cmPkVeMngmt       ARGS((VeMngmt *param, U8 event, Buffer *mBuf));
EXTERN S16 cmUnpkVeMngmt     ARGS((VeMngmt *param, U8 event, Ptr ptr, Buffer *mBuf, Pst *pst));
/* lve_x_001.main_4: Added new function prototype */   
#ifdef LTE_HO_SUPPORT
EXTERN S16 cmPkVeNghCfg      ARGS((VeNghCellCfgLst *param, Buffer *mBuf));
EXTERN S16 cmUnpkVeNghCfg    ARGS((VeNghCellCfgLst *param, Buffer *mBuf, Pst *pst));
#endif
EXTERN S16 cmPkX2Reset       ARGS((VeX2Reset *x2Reset, Buffer *mBuf));
EXTERN S16 cmUnpkX2Reset     ARGS((VeX2Reset *x2Reset, Buffer *mBuf));
#ifdef DEBUGP
/* lve_x_001.main_3: Fix for CID- 1657-01-01 DefectId- 115433 */
/* Added following 2 function protoype to remove warning */
EXTERN S16 cmPkVeDbgCntrl    ARGS((VeDbgCntrl *param,Buffer *mBuf));
EXTERN S16 cmUnpkVeDbgCntrl  ARGS((VeDbgCntrl *param,Buffer *mBuf));
#endif

#ifdef VE_RELAY
EXTERN S16 cmPkVeUstaDgn ARGS((VeUstaDgn *param, Buffer *mBuf));
EXTERN S16 cmUnpkVeUstaDgn ARGS((VeUstaDgn *param, Buffer *mBuf));
#endif /* VE_RELAY */
EXTERN S16 cmPkVeUsta ARGS((VeUsta *param, Buffer *mBuf));
EXTERN S16 cmUnpkVeUsta ARGS((VeUsta *param, Buffer *mBuf));

/* Packing Interfaces */
EXTERN S16 cmPkLveCfgReq   ARGS((Pst *pst, VeMngmt *cfg));
EXTERN S16 cmPkLveCfgCfm   ARGS((Pst *pst, VeMngmt *cfg));
EXTERN S16 cmPkLveCntrlReq ARGS((Pst *pst, VeMngmt *cntrl));
EXTERN S16 cmPkLveCntrlCfm ARGS((Pst *pst, VeMngmt *cntrl));
EXTERN S16 cmPkLveStaInd   ARGS ((Pst *pst, VeMngmt *staInd));

/* Unpacking Interfaces */
EXTERN S16 cmUnpkLveCfgReq   ARGS((LveCfgReq func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLveCfgCfm   ARGS((LveCfgCfm func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLveCntrlReq ARGS((LveCntrlReq func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLveCntrlCfm ARGS((LveCfgCfm func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLveStaInd   ARGS((LveStaInd func, Pst *pst, Buffer *mBuf));

EXTERN S16 VeMiLveCfgReq     ARGS ((Pst *pst, VeMngmt *cfg));
EXTERN S16 VeMiLveCfgCfm     ARGS ((Pst *pst, VeMngmt *cfg));
EXTERN S16 VeMiLveCntrlReq   ARGS ((Pst *pst, VeMngmt *cntrl));
EXTERN S16 VeMiLveCntrlCfm   ARGS ((Pst *pst, VeMngmt *cntrl));
EXTERN S16 VeMiLveStaInd ARGS((Pst *pst, VeMngmt *sta));


/*---- Layer manager interface primitives ----*/
#ifdef EU_DAT_APP
EXTERN S16 VeMiLveCfgReq     ARGS ((Pst *pst, VeMngmt *cfg));
EXTERN S16 VeMiLveCfgCfm     ARGS ((Pst *pst, VeMngmt *cfg));
EXTERN S16 VeMiLveCntrlReq   ARGS ((Pst *pst, VeMngmt *cntrl));
EXTERN S16 VeMiLveCntrlCfm   ARGS ((Pst *pst, VeMngmt *cntrl));
EXTERN S16 VeMiLveStaInd ARGS((Pst *pst, VeMngmt *sta));
#endif


#ifdef SM
EXTERN S16 SmMiLveCfgReq     ARGS ((Pst *pst, VeMngmt *cfg));
EXTERN S16 SmMiLveCfgCfm     ARGS ((Pst *pst, VeMngmt *cfg));
EXTERN S16 SmMiLveCntrlReq   ARGS ((Pst *pst, VeMngmt *cntrl));
EXTERN S16 SmMiLveCntrlCfm   ARGS ((Pst *pst, VeMngmt *cntrl));
EXTERN S16 SmMiLveStaInd     ARGS((Pst *pst, VeMngmt *sta));
#endif /* SM */


#ifdef SM
/*-- stack manager initialization function ---*/
EXTERN S16 smVeActvInit  ARGS ((Ent ent, Inst inst,
                                Region region, Reason reason));
/*---- stack manager activation function -----*/
EXTERN S16 smVeActvTsk   ARGS ((Pst *pst, Buffer *mBuf));
#endif

#ifdef VE
/*-- stack manager initialization function ---*/
EXTERN S16 veActvInit  ARGS ((Ent ent, Inst inst,
                                Region region, Reason reason));
/*---- stack manager activation function -----*/
EXTERN S16 veActvTsk   ARGS ((Pst *pst, Buffer *mBuf));
#endif

#ifdef EU_DAT_APP
/*-- stack manager initialization function ---*/
EXTERN S16 euActvInit  ARGS ((Ent ent, Inst inst,
                                Region region, Reason reason));
/*---- stack manager activation function -----*/
EXTERN S16 euActvTsk   ARGS ((Pst *pst, Buffer *mBuf));
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LVEX__ */
/*********************************************************************

         End of file:     lve.x@@/main/5 - Mon Feb 20 15:54:20 2012

**********************************************************************/

/**********************************************************************

        Notes:

**********************************************************************/

/**********************************************************************

**********************************************************************/

/********************************************************************

        Revision history:

**********************************************************************/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/2      ---     aj           Initial release
/main/3      ---      lve_x_001.main_2  rk    1. Support for logging of console 
                                    prints to a file.
/main/4      ---      lve_x_001.main_3  mn    1. Added function prototype to remove warning
                                    Fix for CID- 1657-01-01 DefectId- 115433
/main/5      ---      lve_x_001.main_4  sagarwal  1. Added new structure and enum.
                                      2. Changed the datatype from U16 to U32.
                                      3. Added new data memberes in the structure.
                                      4. TIC_ID:ccpu00116701: MOD: Fixed warning for g++ compilation on Linux.
                                      5. Added new parameters in the function prototype.
*********************************************************************91*/
