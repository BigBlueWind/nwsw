/**
 * @file    ludh.h
 * @brief API for User Data Handler (UDH)
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author         : Madhu K
 * Date           : 06 May 2011
 * Change History :
 */
#ifndef __LUDHH__
#define __LUDHH__

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_mblk.h"       /* Memory Link List */
#include "leg.h"           /* Event structure file */

/* header include files (.x) */
#include"gen.x"           /* general layer */
#include"ssi.x"           /* system services */
#include"cm_tkns.x"       /* common tokens */
#include"cm_mblk.x"       /* Memory Link List typedefs */
#include"cm_lib.x"        /* common ss7 */
#include"cm_tpt.h"
#include"cm_tpt.x"
#include"cm_lte.h"
#include"cm_lte.x"
#include"log.h"
#include"ueh_cfg.h"
#include "start_indication.h"

/* Interface Events */
#define EVT_SM_UDH_SAP_CFG_REQ    1       /* Config request */
#define EVT_UDH_SM_SAP_CFG_CFM    2       /* Config Confirm */
#define EVT_SM_UDH_SAP_BIND_REQ   3       /* Control Request */
#define EVT_UDH_SM_SAP_BIND_CFM   4       /* Control Confirm */
#define EVT_SM_UDH_STA_IND        5       /* Status Indication */
#define EVT_SM_UDH_DBG_CNTRL_REQ  6       /* Debug Control Request */


typedef enum  e_UehUdhEventTypes{
EVT_UDH_UEH_BND_CFM                       = 1,
EVT_UDH_UEH_ADD_RAB_CFM                   = 2,
EVT_UDH_UEH_DEL_RAB_CFM                   = 3,
EVT_UDH_UEH_DEL_UE_CFM                    = 4,
EVT_UDH_UEH_INACTIVE_UES_IND              = 5,
EVT_UEH_UDH_ADD_RAB_REQ                   = 6,
EVT_UEH_UDH_DEL_RAB_REQ                   = 7,
EVT_UEH_UDH_BND_REQ                       = 8,
EVT_UEH_UDH_DEL_UE_REQ                    = 9,
EVT_UDH_UEH_UL_INACTIVE_IND               = 10 
}UehUdhEventTypes;

/* Config status */
#define CM_CFG_OK                 1       /* Configuration request successful  in Confirm */
#define CM_CFG_NOK                2       /* Configuration request is unsuccessful in Confirm */

typedef enum e_SapState {
 SAP_NOT_CONFIGURED = 0,
 SAP_CONFIGURED,
 SAP_BOUND
} SapState;

typedef enum e_SapId {
  NAME_NOT_SET =0,
 /* xxx_SM_SAP --> These are never configured */
  UDH_GTP_SAP,
  UDH_PDCP_SAP,
  UDH_UEH_SAP
} SapId;

/** @struct  CmSapCfg
 *  @brief   Information that can be common to all SAP configurations
 */
typedef struct s_CmSapCfg
{
  SapId        sapId;
  SuId         suId;        /** ID of service-user */
  SpId         spId;        /** ID of service provider */
  Pst          pst;
  /** Post structure (Pst) for posting events to service-user.
   *  All fields in Pst are not meaningful to SAP configuration.
   *  Yet, Pst has been used:
   *       a) to avoid creating a sub-set structure of Pst and
   *       b) ease of use at receiver of SapCfg: the receiver can directly copy this into a Pst.
   *
   * Description of Pst fields sent in SapCfg.
   *
   * pst.srcEnt   =  NOTUSED(0)
   *                 EntityId of event originator.
   *                 srcEntity is not a SAP config.
   *                 Do not to set it to the correct value as it will set a wrong example.
   *
   * pst.srcInst  =  NOTUSED(0)
   *
   * pst.srcProcId = NOTUSED(0)
   *
   * pst.dstEnt   =  EntityId of destination entity i.e. to whom the events will be delivered.
   *
   * pst.dstInst  =  InstanceId of destination entity.
   *                 Meaningful if destination has multiple instances.
   *                 Use 0 for single-instance cases.
   *
   * pst.dstProcId = ProcessorId on which destination entity is running.
   *
   * pst.prior    =  PRIOR0
   *                 The priority to be used for messages sent out of this SAP.
   *                 PRIOR0 has been used as default as its value is 0.
   *                 It is also the highest priority.
   *                 This value is OK till all messages have the same priority.
   *
   * pst.route    =  RTESPEC
   *                 Means route to the specified destination instance.
   *                 Other choices can also be used (based on arch).
   *
   * pst.event    =  NOTUSED(0)
   *                 This is not a SAP configuration parameter.
   *                 An eventList could have been a SAP Configuration Parameter
   *                 but this is not how CCPU's SSI works.
   *
   * pst.region   =  DFLT_REGION
   *                 Memory region to be used for allocating memory used by interactions on this SAP.
   *                 Having a single memory region for whole entity is a good default.
   *
   * pst.pool     =  OBSOLETE(DFLT_POOL)
   *                 Pools are an obsolete concept but a value is to be filled for compatibility.
   *
   * pst.selector =  The kind of coupling used on this SAP: loose, tight or light-weight-loose.
   *                 Use one of SM_SELECTOR_LC, SM_SELECTOR_TC, SM_SELECTOR_LWLC as the case may be.
   *                 This is a very important parameter.
   *                 If tight coupling is used, the dstEnt, route parameters will not be used.
   *
   * pst.intfVer  =  NOTUSED(0)
   */
}CmSapCfg;

typedef struct s_CmSapBindReq
{
  SapId        sapId;
}CmSapBindReq;

typedef CmSapBindReq CmSapBindCfm;

/** common structure for SAP */
typedef struct s_SapCb
{
  CmSapCfg   sapCfg;
  SapState   sapState;
} SapCb;

typedef struct s_SmUdhLogConfig
{
  U32   logMask1;
}SmUdhLogConfig;

typedef S16 (*UdhSapsCfgReq) ARGS(( Pst  *pst, CmSapCfg *cmSapCfg  ));


typedef S16 (*UdhSapsBindReq) ARGS(( Pst  *pst, CmSapBindReq  *udhGtpSapBindReq  ));

typedef S16 (*UehBndCfm)    ARGS((
    Pst*  pst,
    SuId  suId,
    U8    status
    ));
/** UEH <-> UDH */
typedef struct s_UdhAddRabReq
{
  U32            locTeId;
  U32            remTeid;
  U32            qci;            /* QoS Class Identifier  */
  U32            rabId;
  CmTptAddr      srcIpAddr;
  CmTptAddr      dstIpAddr;                   /**< Destination IP Address */
  CmTptAddr      frmDstIpAddr;                /**< From Destination IP Address */
  CmLtePdcpId    pdcpId;
} UehUdhAddRabReq; /* EVT_UEH_UDH_ADD_RAB_REQ */

typedef struct s_UdhUehAddRabCfm
{
  Status         status;
  CmLtePdcpId    pdcpId;
} UdhUehAddRabCfm; /* EVT_UDH_UEH_ADD_RAB_CFM */

typedef struct s_UehUdhDelRabReq
{
  CmLtePdcpId     pdcpId;
  RabReleaseCause rabReleaseCause;
} UehUdhDelRabReq; /* EVT_UEH_UDH_DEL_RAB_REQ */

typedef struct s_UehUdhDelUeReq
{
  CmLteRnti      ueId;
} UehUdhDelUeReq; /* EVT_UEH_UDH_DEL_UE_REQ */

typedef struct s_UdhUehDelUeCfm
{
  Status         status;
  CmLteRnti      ueId;
} UdhUehDelUeCfm; /* EVT_UDH_UEH_DEL_UE_SUCC */

typedef struct s_UdhUehDelRabCfm
{
  Status         status;
  CmLtePdcpId    pdcpId;
} UdhUehDelRabCfm; /* EVT_UDH_UEH_DEL_RAB_CFM */

typedef struct s_UdhUehUeInactiveInd
{
  U16            noofInactiveUes;
  CmLteRnti      ueId[MAX_UE_SUPP];        /*!< UE ID */
} UdhUehUeInactiveInd; /* EVT_UDH_UEH_INACTIVE_UES_IND */

typedef UdhUehUeInactiveInd UdhUehUlInactiveInd; /* EVT_UDH_UEH_UL_INACTIVE_IND */

/* Packing function for UEH Bind Request */
PUBLIC S16 cmPkAndSendUehUdhBndReq
(
  Pst       *pst,
  SpId       spId,
  SuId       suId
);
/*  Unpacking function for UEH Bind Request */
typedef S16 (*ProcUehUdhBndRabReq) ARGS ((
               Pst              *pst,
               SuId             suId,
               SpId             spId));

PUBLIC S16 cmUnpkUehUdhBndReq
(
  ProcUehUdhBndRabReq  func,
  Pst                  *pst,
  Buffer               *mBuf
);
PUBLIC S16 cmUnpkUdhUehCfgCfm
(
  Pst       *pst,
  Buffer    *mBuf
);
/* Pack and post function for UDH -UEH Bind Confirm */
PUBLIC S16 cmPkAndSendUdhUehBndCfm
(
  Pst*  pst,
  SuId  suId,
  U8    status
);
/* Pack and Unpack functions for Add RAB Request */
/* Pack Function */
PUBLIC S16 cmPkAddRabReqMsg
(
  UehUdhAddRabReq  *uehUdhAddRabReq,
  Buffer           *mBuf
);
/* Pack and send UehUdhAddRabReq */
PUBLIC S16 cmPkAndSendUehUdhAddRabReq
(
  Pst*             pst,
  SuId             suId,
  UehUdhAddRabReq  *uehUdhAddRabReq
);
PUBLIC S16 cmUnpkAddRabReqMsg
(
  UehUdhAddRabReq  *uehUdhAddRabReq,
  Buffer           *mBuf
);


/* Un Pack function for UehUdhAddRabReq */

typedef S16 (*UdhBndReq) ARGS((Pst  *pst, SuId suId, SpId spId));
typedef S16 (*UdhAddRabReq) ARGS((Pst  *pst, SuId suId, UehUdhAddRabReq  *uehUdhAddRabReq));
typedef S16 (*UdhDelRabReq) ARGS((Pst  *pst, SuId suId, UehUdhDelRabReq  *uehUdhDelRabReq));
typedef S16 (*UdhDelUeReq) ARGS((Pst  *pst, SuId suId, UehUdhDelUeReq *uehUdhDelUeReq));

typedef S16 (*UdhDbgCntrl) ARGS((Pst  *pst, SmUdhLogConfig  *smUdhLogConfig));

typedef S16 (*UdhDelUeCfm)    ARGS((  Pst  *pst, UdhUehDelUeCfm  *udhUehDelUeCfm ));
typedef S16 (*UdhAddRabCfm)    ARGS((
    Pst              *pst,
    UdhUehAddRabCfm  *udhUehAddRabCfm
    ));

typedef S16 (*UdhUehInactiveUesIndFun) ARGS ((
               Pst                 *pst,        /* post structure */
               SpId                *spId,
               UdhUehUeInactiveInd *udhUehInactiveUeInd));   /* PdcpId Request */
typedef S16 (*UdhUehUlInactiveUesIndFun) ARGS ((
               Pst                 *pst,        /* post structure */
               SpId                *spId,
               UdhUehUlInactiveInd *udhUehUlInactiveUeInd));   /* PdcpId Request */
typedef S16 (*ProcUehUdhAddRabReq) ARGS ((
               Pst              *pst,
               SuId             suId,
               UehUdhAddRabReq  *uehUdhAddRabReq));
typedef S16 (*ProcUehUdhDelRabReq) ARGS ((
               Pst              *pst,
               SuId             suId,
               UehUdhDelRabReq  *uehUdhDelRabReq));

typedef S16 (*UdhDelRabCfm)    ARGS((
     Pst              *pst,
     UdhUehDelRabCfm  *udhUehDelRabCfm
    ));
typedef S16 (*UehAddRabCfm) ARGS ((
               Pst              *pst,
               UdhUehAddRabCfm  *udhUehAddRabCfm));
typedef S16 (*UehUeInactvInd) ARGS ((
               Pst                  *pst,
               UdhUehUeInactiveInd  *udhUehUeInactvInd));
typedef S16 (*UehUlInactvInd) ARGS ((
               Pst                  *pst,
               UdhUehUlInactiveInd  *udhUehUlInactvInd));
typedef S16 (*UehUdhBndCfm) ARGS ((
               Pst          *pst,
               SuId         suId,
               U8           status));
PUBLIC S16  cmPkAndSendDelUeCfm(
  Pst              *pst,
  UdhUehDelUeCfm  *udhUehDelUeCfm
  );



PUBLIC S16 cmUnpkUehUdhAddRabReq
(
  ProcUehUdhAddRabReq  func,
  Pst                  *pst,
  Buffer               *mBuf
);
/* Pack and Unpack functions for Delete RAB Request */
/* Pack Function */
PUBLIC S16 cmPkDelRabReqMsg
(
  UehUdhDelRabReq  *uehUdhDelRabReq,
  Buffer           *mBuf
);
/* Pack and send UehUdhDelRabReq */
PUBLIC S16 cmPkAndSendUehUdhDelRabReq
(
  Pst*             pst,
  SuId             suId,
  UehUdhDelRabReq  *uehUdhDelRabReq
);

/* Unpack and process Delete DRB request */

PUBLIC S16 cmUnpkUehUdhDelRabReq
(
  ProcUehUdhDelRabReq  func,
  Pst                  *pst,
  Buffer               *mBuf
);
PUBLIC S16 cmUnpkDelRabReqMsg
(
  UehUdhDelRabReq  *uehUdhDelRabReq,
  Buffer           *mBuf
);
/***********************************/
/*      Add RAB Success to UEH     */
/***********************************/
PUBLIC S16  cmPkAndSendAddRabCfm(
    Pst              *pst,
    UdhUehAddRabCfm  *udhUehAddRabCfm
  );

/**************************************/
/*      Delete RAB Success to UEH     */
/**************************************/
PUBLIC S16  cmPkAndSendDelRabCfm(
    Pst              *pst,
    UdhUehDelRabCfm  *udhUehDelRabCfm
  );

PUBLIC S16 cmUnpkDelRabCfm
(
  UdhDelRabCfm     func,
  Pst              *pst,
  Buffer           *mBuf
);

PUBLIC S16 cmUnpkDelUeCfm
(
  UdhDelUeCfm     func,
  Pst              *pst,
  Buffer           *mBuf
);

/* Pack and Send UE s Inactivity Detection */

PUBLIC S16  cmPkAndSendInactiveUesInd
(
  Pst                 *pst,
  SpId                *spId,
  UdhUehUeInactiveInd *udhUehInactiveUeInd
);
PUBLIC S16  cmPkAndSendUlInactiveUesInd(
  Pst                 *pst,
  SpId                *spId,
  UdhUehUlInactiveInd *udhUehUlInactiveUeInd
  );
PUBLIC S16 cmUnpkInactiveUesInd
(
  Buffer              *mBuf,
  SuId                *suId,
  UdhUehUeInactiveInd *udhUehUeInactiveInd
);

PUBLIC S16 cmUnpkUdhUehBndCfm
(
  UehUdhBndCfm         func,
  Pst                  *pst,
  Buffer               *mBuf
);
PUBLIC S16 cmUnpkAddRabCfm
(
  UehAddRabCfm       func,
  Pst                *pst,
  Buffer             *mBuf
);
PUBLIC S16 cmUnpkUeInactiveInd
(
  UehUeInactvInd       func,
  Pst                  *pst,
  Buffer               *mBuf
);
PUBLIC S16 cmUnpkUlInactiveInd
(
  UehUlInactvInd       func,
  Pst                  *pst,
  Buffer               *mBuf
);
PUBLIC S16 cmUnpkDelUeReqMsg
(
  UehUdhDelUeReq  *uehUdhDelUeReq,
  Buffer          *mBuf
);


/* Unpack and process Delete DRB request */
typedef S16 (*ProcUehUdhDelUeReq) ARGS ((
               Pst              *pst,
               SuId              suId,
               UehUdhDelUeReq   *uehUdhDelUeReq));

PUBLIC S16 cmUnpkUehUdhDelUeReq
(
  ProcUehUdhDelUeReq   func,
  Pst                  *pst,
  Buffer               *mBuf
);

PUBLIC S16 cmPkDelUeReqMsg
(
  UehUdhDelUeReq  *uehUdhDelUeReq,
  Buffer           *mBuf
);
PUBLIC S16 cmPkAndSendUehUdhDelUeReq
(
  Pst*             pst,
  SuId             suId,
  UehUdhDelUeReq  *uehUdhDelUeReq
);

/**************************************************************************************/
/*********************************** Common-- FUNC  Start  ****************************/
/**************************************************************************************/

PUBLIC S16 cmPkAndSendSmUdhSapCfg
(
  Pst       *pst,
  CmSapCfg  *sapCfg
);
PUBLIC S16 cmUnpkSmUdhSapCfg
(
  CmSapCfg  *sapCfg,
  Buffer    *mBuf
);
PUBLIC S16 udhPkAndSendSmUdhSapCfgCfm
(
  Pst*  pst,
  SpId  spId,
  U8    status
);

PUBLIC S16 cmPkAndSendSmUdhBndReq
(
  Pst           *pst,
  CmSapBindReq  *udhGtpSapBindReq
);
/* UnPack SM - UDH Bind Request */
PUBLIC S16 cmUnpkSmUdhBndReq
(
  Buffer        *mBuf,
  CmSapBindReq  *udhGtpSapBindReq
);

/* Pack and post function for UDH -SM Bind Confirm */
PUBLIC S16 cmPkAndSendUdhSmBndCfm
(
  Pst           *pst,
  CmSapBindCfm  *cmSapBindCfm,
  U8             status
);

typedef S16 (*UdhSmSapCfgCfm) ARGS ((
               Pst             *pst,
               SpId            spId,
               U8              status));
/* Un Pack function for UDH - SM SapCfg CFM */
PUBLIC S16 udhUnpkSmUdhSapCfgCfm
(
  UdhSmSapCfgCfm  func,
  Pst             *pst,
  Buffer          *mBuf
);

typedef S16 (*UdhSmBndCfm) ARGS ((
               Pst              *pst,
               CmSapBindCfm     *cmSapBindCfm,
               U8               *status));

/* Un Pack function for UDH - SM Bnd CFM */
PUBLIC S16 cmUnpkUdhSmBndCfm
(
  UdhSmBndCfm  func,
  Pst          *pst,
  Buffer       *mBuf
);


/* UnPack SM - UDH Debug Control */
/* UnPack SM - UDH Debug Control */
PUBLIC S16 cmUnPkAndSendSmUdhDbgCntrl
(
  Buffer          *mBuf,
  SmUdhLogConfig  *smUdhLogConfig
);
/* Pack and Unpack Functions for SM-UDH Debug Control Request */
PUBLIC S16 cmPkAndSendSmUdhDbgCntrl
(
  Pst             *pst,
  SmUdhLogConfig  *smUdhLogConfig
);

#endif /* __LUDHH__ */

/********************************************************************

        Revision history:

**********************************************************************/

