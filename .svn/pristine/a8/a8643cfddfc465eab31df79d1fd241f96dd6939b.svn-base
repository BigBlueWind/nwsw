/**
 * @file    udh.h
 * @brief This file contains All Include files used in UDH module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Madhu K, 05May2011 :
 *     a) Created initial version
 */

#ifndef UDH__H
#define UDH__H

#include "udh_includes.h"

#define UDH_MEM_REGION  0
#define UDH_MEM_POOL    0

typedef struct s_udhRabCounters
{
  U32         pktsReceivedFromGtpInUeInActvInterval;
  U32         bytesReceivedFromGtpInCurrentInterval;
  U32         totalPktsReceivedFromGtp;
  U32         totalKBytesReceivedFromGtp;

  U32         pktsReceivedFromPdcpInUlInActvInterval;
  U32         pktsReceivedFromPdcpInUeInActvInterval;
  U32         bytesReceivedFromPdcpInCurrentInterval;
  U32         totalKBytesReceviedFromPdcp;
  U32         totalPktsReceviedFromPdcp;
} UdhRabCounters;

typedef struct s_udhRabCb
{
  Bool               isActive;         /* TRUE means this RAB has Added otherwise No RAB */
  U32                locTeId;
  U32                remTeid;
  U32                qci;              /* QoS Class Identifier  */
  U32                rabId;
  CmTptAddr          srcIpAddr;
  CmTptAddr          dstIpAddr;        /**< Destination IP Address */
  CmTptAddr          frmDstIpAddr;     /**< From Destination IP Address */
  CmLtePdcpId        pdcpId;
  UdhRabCounters     rabCounters;
  Bool               isMontiored;
  /* Manu, 11Sep2012: Fixed accuracy of inactivity timer.
   * Cause: Inactivity is checked every inactivity interval; 
   * Fix: Check inactivity every second, instead of every inactivity interval.
   * Implementation: 
   *  - Added secsSinceInactivityCheck 
   *  - When RAB is added, set to 0.
   *  - Not changed till it is not monitored.
   *  - At inactivity check timeout, increment.
   *  - if (secsSinceInactivityCheck >= inactivity interval)
   *    { 
   *      check inactivity
   *      set to 0
   *    }
   */
  U16                secsSinceInactivityCheck;
  ActiveRabStatus    activeRabStatus;  /* Active RAB Status */
} UdhRabCb;

typedef struct s_udhUeCb
{
  U8        numRabs;
  UdhRabCb  rabCbTable[UEH_MAX_RABS_IN_UE];
} UdhUeCb;

typedef struct s_udhDatThrput
{
  U32        dlDataCntCrntIntvl;   /* Downlink data for all UEs in Current Interval */
  U32        dlDataCntPrvIntvl;    /* Downlink data for all UEs in Previous Interval */
  U32        ulDataCntCrntIntvl;   /* Uplink data for all UEs in Current Interval */
  U32        ulDataCntPrvIntvl;    /* Uplink data for all UEs in Previous Interval */
} UdhDatThrput;


/** UDH Context **/
typedef struct s_udhCb
{
  SapCb         gtpSap;
  SapCb         pdcpSap;
  SapCb         uehSap;
  SapCb         smSap;

  U16           startRnti;           /* RNTI start number used in MAC, configurable from OAM */
  U16           maxConnectedUes;     /* Maximum number of UEs supported, configurable from OAM */
  U16           totalActvRabs;       /* Number of Active Rabs */
  UdhUeCb       **ueInfoPtrs;
  /** Mappings needed are   1) lclTeId -> rabCb  2) pdcpId -> rabCb  **/
  UdhRabCb      *lclTeId2RabCb[/* lclTeId */UEH_MAX_TUNNEL_ID]; /** Mapping between localTunnelId and rabCb **/

  U16           ueInactivityCheckIntervalSecs;  /* Inactivity timer, configurable from OAM   */
  U16           ulInactivityChkIntvInSecs;      /* UL Inactivity timer, configured by OAM through ConfigReq */

  U8            ueThrptTmrInvlSec;              /* Throughput timer -- Internal  to UDH */

  RabHistory    rabHistory[MAX_ROWS_IN_RABHISTORY_TABLE]; /* Rab History Table */
  U16           totalNumOfRabs; /* Total number of Rabs */
  U16           rabIdx; /* Rab index */

  UdhDatThrput  udhDatThrput;

  U32           numUesConnected;  /* Number of Active UEs */
  U32           numUeInactivityTimeouts;
  U8            memoryDiagCfg;
} UdhCb;

extern UdhCb g_udhCb;

#define DRB_START_INDEX_MASK  3
#define getUdhCb()        (&g_udhCb)
#define getUehSapCfg()    (&(g_udhCb.uehSap.sapCfg))
#define getGtpSapCfg()    (&(g_udhCb.gtpSap.sapCfg))
#define getPdcpSapCfg()   (&(g_udhCb.pdcpSap.sapCfg))
#define getSmSapCfg()     (&(g_udhCb.smSap.sapCfg))

#define getUdhUehPst()    (&(g_udhCb.uehSap.sapCfg.pst))
#define getUdhGtpPst()    (&(g_udhCb.gtpSap.sapCfg.pst))
#define getUdhPdcpPst()   (&(g_udhCb.pdcpSap.sapCfg.pst))

#define udhGetRabCbByLocalTunnelId(udhCb, locTeid) ((udhCb)->lclTeId2RabCb[(locTeid)])
#define udhSetLocTeId2RabCb(udhCb, locTeId, rabCb) ((udhCb)->lclTeId2RabCb[(locTeId)] = (rabCb))

#define UDH_GET_UEIDX_BY_UEID(_ueId) ((_ueId) - g_udhCb.startRnti)
#define UDH_GET_RBIDX_BY_RBID(_rbId) ((_rbId) - DRB_START_INDEX_MASK)

#define SM_MAX_UDH_SEL        2

#define UEH_UDH_GTP_SAP_CFG   1
#define UEH_UDH_PDCP_SAP_CFG  2
#define UEH_UDH_UEH_SAP_CFG   4

#define UDH_RAB_ACTIVE        1
#define UDH_RAB_INACTIVE      0
#define UDH_MAX_UE_POOL (g_udhCb.maxConnectedUes + UEH_RESERVED_RNTI_POOL_SIZE)
#define UEH_SM_UDH_CONFIGURED  (UEH_UDH_GTP_SAP_CFG | UEH_UDH_PDCP_SAP_CFG | UEH_UDH_UEH_SAP_CFG)
#define AGGREGATE_THROUGHPUT_KBPS(_dataCntPrvIntvl,_tmrInvlSec,_aggrThrptKbps)\
_aggrThrptKbps =((_dataCntPrvIntvl NOTEQ 0) && (_dataCntPrvIntvl < _tmrInvlSec*1024))? 1 : ((_dataCntPrvIntvl/(_tmrInvlSec*1024))*8)  


/* Proto Types  */

/* UEH - UDH  Interface */


/*********************************** Common-- FUNC  Ends   ****************************/
UdhUeCb* getUeInfoPtrByUeId(
  CmLteRnti   ueId
  );
PUBLIC S16 udhInitUeCb( void );
PUBLIC S16 udhClearRabCb( void );
PUBLIC S16 udhCpyCmTptAddr
(
CmTptAddr *dst,         /* pointer to the destination CmTptAddr. */
CmTptAddr *src          /* pointer to the source CmTptAddr. */
);
/**************************************************************************************/
/***********************************  SM  -- FUNC  Start   ****************************/
/**************************************************************************************/
void udhProcOpStartReq(void);
void udhProcOpStopReq(void);
TBOOL udhProcBsmUdhCfgReq(
  Buffer   *mBuf
  );

/* Save configuration, Send Succ/Unsucc Confirm Message to SM */
PUBLIC S16 udhProcSapCfgReq(
  Pst       *pst,
  CmSapCfg  *sapCfg
  );
/* Process Sm-Udh Bind Request */
PUBLIC S16 udhProcSmUdhBndReq
(
  Pst           *pst,
  CmSapBindReq  *cmSapBindReq
);

PUBLIC S16 udhDeleteAllContexts( void );

PUBLIC S16 udhFillActvRabStats(
    UdhUeCb                     *udhUeCb,
    MsgUdhBsmActiveRabStatusRsp *actRabStsRsp
  );
void udhProcActiveRabStatusReq(
    Buffer *mBuf
  );
void udhProcModStateReq(
     Buffer  *mBuf
  );
void udhProcRabHistoryReq(
    Buffer *mbuf
  );
void udhProcDstatReq (
  Buffer   *mBuf
);
S16 udhReportDebugStats(void);

/* Process Debug control information from SM i.e log level settings from SM  */
PUBLIC S16 udhProcSmUdhDbgCntrl(
  SmUdhLogConfig  *smUdhLogConfig
  );

/***********************************   SM -- FUNC  Ends    ****************************/

/**************************************************************************************/
/***********************************  UEH  -- FUNC  Start  ****************************/
/**************************************************************************************/

/***********************************  UEH  -- FUNC  Ends   ****************************/


/**************************************************************************************/
/***********************************  GTP  -- FUNC  Start  ****************************/
/**************************************************************************************/
PUBLIC S16 udhProcEgtBndCfm(
  Pst     *pst,
  SuId    suId,
  U8      status
  );
PUBLIC S16 PtLiEgtEguDatReq
(
  Pst          *pst,              /* post structure */
  SuId         suId,              /* service user id */
  EgtUEvnt     *egtUEvnt
);
PUBLIC S16 udhProcEgtDatInd(
  Pst         *pst,
  SuId         suId,
  EgtUEvnt    *eguMsg
  );
PUBLIC S16 UdhLiPjuDatReq
(
  Pst*         pst,
  SpId         spId,
  CmLtePdcpId *pdcpId,
  PjuSduId    sduId,
  Buffer      *mBuf
);
/***********************************  GTP  -- FUNC  Ends   ****************************/

/**************************************************************************************/
/***********************************  PDCP -- FUNC  Start  ****************************/
/**************************************************************************************/

PUBLIC S16 udhProcPjuBndCfm(
  Pst         *pst,
  SuId        suId,
  U8          status
  );
PUBLIC S16 PtLiPjuDatReq
(
  Pst* pst,
  SpId spId,
  CmLtePdcpId * pdcpId,
  PjuSduId sduId,
  Buffer *mBuf
);
PUBLIC S16 udhProcPjuDatInd(
  Pst                 *pst,
  SuId                suId,
  CmLtePdcpId         *pdcpId,
  Buffer              *mBuf
  );
PUBLIC S16 UdhLiEgtEguDatReq
(
  Pst          *pst,              /* post structure */
  SuId         suId,              /* service user id */
  EgtUEvnt     *egtUEvnt
);
/*********************************** PDCP -- FUNC  Ends ********************************/
/** Checks if the given rab is inactive */
TBOOL isRabInactive(
  UdhRabCounters* rabCounters
  );

PUBLIC S16 udhHandleThroughputTimeout(void);
PUBLIC S16 udhHandleInactivityTimeout(void);
PUBLIC S16 udhHandleUlInactivityTimeout(void);
/*********************************************************/
/*************************** ********** ******************/
/*********************************************************/
PUBLIC S16 sendUdhMsgToFrm(
    Ent     dstEnt,
    Event   event,
    MsgLen     len,
    U8     *data
    );
PUBLIC S16 sendUdhMsgToBsm(
    Event   event,
    MsgLen     len,
    U8     *data
    );
void udhSendBsmUdhConfigReqFail(
  IN UINT16       transId,
  IN BsErrorValue errorNum,
  IN UINT8        *failReason
  );

PUBLIC S16 sendUdhMsgToPrm(
    Event   event,
    MsgLen     len,
    U8     *data
    );
void sendUdhStartIndToPrm(void);
void sendUdhHelloAckToPrm(
  Buffer   *mBuf
  );

void udhUpdateRabStats(
  UdhRabCb         *rabCb
  );

void udhUpdateUlStats(
  UdhRabCb      *udhRabCb,
  MsgLen        msgLen
  );

void udhUpdateDlStats(
  UdhRabCb      *udhRabCb,
  MsgLen        msgLen
  );

TBOOL isRabInactiveInUlPath(
  UdhRabCounters* rabCounters
  );

PUBLIC S16 udhUehUlInactiveUesInd
(
  Pst                 *pst,              /* post structure */
  SpId                *spId,             /* service Provider id */
  UdhUehUlInactiveInd *udhUehUlInactiveUeInd
);

void createAndSendUdhUehUlInactiveInd(
  UdhUehUlInactiveInd *udhUehUlInactiveInd
  );


/*********************************************************/
/*************************** udh_main.c ******************/
/*********************************************************/

void udhHandlePrmEvents(
  Pst      *pst,
  Buffer   *mBuf
  );
void udhHandleBsmEvents(
  Pst      *pst,
  Buffer   *mBuf
  );

void udhHandleSmEvents(
  Pst      *pst,
  Buffer   *mBuf
  );
void udhHandleGtpEvents(
  Pst     *pst,
  Buffer  *mBuf
  );
void udhHandleUehEvents(
  Pst    *pst,
  Buffer *mBuf
  );
void udhHandlePdcpEvents(
  Pst    *pst,
  Buffer *mBuf
  );
void udhHandlePmhEvents(
  Pst      *pst,
  Buffer   *mBuf
  );

PUBLIC S16 udhActvTsk (
  Pst      *pst,
  Buffer   *mBuf
);

/* Fill the Rab History Table */
void udhFillRabHistoryTable (
  UdhRabCb *rabCb,
  RabReleaseCause rabReleaseCause
  );

/* Fill eleasewnlink Data in KIlobytes in Rab History Table */ 
void udhFillDlKbInRabHisTable (
  UdhRabCb * udhRabCb
  );

/* Fill eleasewnlink Data in KIlobytes in Rab History Table */ 
void udhFillUlKbInRabHisTable (
  UdhRabCb * udhRabCb
  );
 
EXTERN S16 EuLiEgtBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
EXTERN S16 EuLiEgtEguDatInd ARGS ((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));


#endif /* UDH_H */
