/**
 * @file    lueh.x
 * @brief Data Structures for User Equipment Handler (Abbrev: UEH) Mgmt
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Amar Ramteke
 * Date           : 16th Sept 2010
 * Change History :
 */
#ifndef __LUEHX__
#define __LUEHX__

#include "lpj.x"
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------
                    Configuration
 ----------------------------------------------------------------------*/

typedef struct uehGenUeCfg
{
   /* Security related info */
  U8 intgAlgo;
  U8 ciphAlgo;
} UehGenUeCfg;

typedef struct uehCellCfg
{
   /* Cell cfg related info */
   U16          cellId;
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
   UehGenUeCfg genUeCfg;
} UehCellCfg;


typedef struct uehGenCfg 
{
   Pst         lmPst;  
   UehCellCfg   cellCfg;
   CmTptAddr dataAppAddr;
   /* TODO 
    * Add Cfg for MME/RELAY related operations */
} UehGenCfg;


typedef struct uehLSapCfg
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
} UehLSapCfg;

typedef struct uehCfg
{
   union
   {
      UehGenCfg      genCfg;  /* Framing - General          */
      UehLSapCfg     lSap;    /* Framing - Transport SAP    */
   } u;

} UehCfg;

#ifdef DEBUGP
 /** @brief Debug Printing Control structure parameters*/
typedef struct s_UehDbgCntrl 
{
   U32       dbgMsk;          
} UehDbgCntrl;
#endif /* DEBUGP */

/*----------------------------------------------------------------------
                       Control related structure
 ----------------------------------------------------------------------*/

typedef struct uehCntrl
{
   Action    action;    
   Action    subAction; 
   union
   {
      SpId       sapId; 
      U16        cellId;
#ifdef DEBUGP
      U32        dbgMask;
      UehDbgCntrl      dbg;        /*!< The layer manager can selectively enable
                                     or disable various levels of debug 
                                     printing. */
      
#endif
      U8         oper;
   } u;

} UehCntrl;

typedef struct uehUstaDgn
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
} UehUstaDgn;

typedef struct uehUsta
{
   CmAlarm   alarm;            /* Alarms */
   UehUstaDgn dgn;              /* Alarm diagnostics */
} UehUsta;


/*----------------------------------------------------------------------
                    Layer configuration
 ----------------------------------------------------------------------*/
typedef struct uehMngmt
{
   Header     hdr;   /* Header for mngmt struct           */
   CmStatus   cfm;   /* result of the cntrl/cfg operation */
   union
   {
      UehCfg    cfg;    /* configuration        */
      UehCntrl  cntrl;  /* control              */
      UehUsta    usta;          /* Unsolicited status/alarms */
   } u;

} UehMngmt;



/***********************************************************************

 ***********************************************************************/


typedef S16 (*LuehCfgReq)   ARGS((
Pst     *pst, 
UehMngmt *cfg
));

typedef S16 (*LuehCfgCfm)   ARGS((
Pst     *pst, 
UehMngmt *cfg
));

typedef S16 (*LuehCntrlReq) ARGS((
Pst     *pst, 
UehMngmt *cntrl
));

typedef S16 (*LuehCntrlCfm) ARGS((
Pst     *pst, 
UehMngmt *cntrl
));

typedef S16 (*LuehStaInd) ARGS((
Pst *pst,
UehMngmt *sta
));


EXTERN S16 cmPkUehGenCfg      ARGS((UehGenCfg *param, Buffer *mBuf));
EXTERN S16 cmUnpkUehGenCfg    ARGS((UehGenCfg *param, Buffer *mBuf));
EXTERN S16 cmPkUehLSapCfg     ARGS((UehLSapCfg *param, Buffer *mBuf));
EXTERN S16 cmUnpkUehLSapCfg   ARGS((UehLSapCfg *param, Buffer *mBuf));
EXTERN S16 cmPkUehCfg         ARGS((UehCfg *param, U16 element, Buffer *mBuf));
EXTERN S16 cmUnpkUehCfg       ARGS((UehCfg *param, U16 element, Buffer *mBuf));
EXTERN S16 cmPkUehCntrl       ARGS((UehCntrl *param, U16 elmnt, Buffer *mBuf));
EXTERN S16 cmUnpkUehCntrl     ARGS((UehCntrl *param, U16 elmnt, Buffer *mBuf));
EXTERN S16 cmPkUehMngmt       ARGS((UehMngmt *param, U8 event, Buffer *mBuf));
EXTERN S16 cmUnpkUehMngmt     ARGS((UehMngmt *param, U8 event, Ptr ptr, Buffer *mBuf));

EXTERN S16 cmPkUehUstaDgn ARGS((UehUstaDgn *param, Buffer *mBuf));
EXTERN S16 cmUnpkUehUstaDgn ARGS((UehUstaDgn *param, Buffer *mBuf));
EXTERN S16 cmPkUehUsta ARGS((UehUsta *param, Buffer *mBuf));
EXTERN S16 cmUnpkUehUsta ARGS((UehUsta *param, Buffer *mBuf));

/* Packing Interfaces */
EXTERN S16 cmPkLuehCfgReq   ARGS((Pst *pst, UehMngmt *cfg));
EXTERN S16 cmPkLuehCfgCfm   ARGS((Pst *pst, UehMngmt *cfg));
EXTERN S16 cmPkLuehCntrlReq ARGS((Pst *pst, UehMngmt *cntrl));
EXTERN S16 cmPkLuehCntrlCfm ARGS((Pst *pst, UehMngmt *cntrl));
EXTERN S16 cmPkLuehStaInd   ARGS ((Pst *pst, UehMngmt *staInd));

/* Unpacking Interfaces */
EXTERN S16 cmUnpkLuehCfgReq   ARGS((LuehCfgReq func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLuehCfgCfm   ARGS((LuehCfgCfm func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLuehCntrlReq ARGS((LuehCntrlReq func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLuehCntrlCfm ARGS((LuehCfgCfm func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkLuehStaInd   ARGS((LuehStaInd func, Pst *pst, Buffer *mBuf));
EXTERN S16 cmPkUehDbgCntrl ARGS((UehDbgCntrl *param, Buffer *mBuf));
EXTERN S16 cmUnpkUehDbgCntrl ARGS(( UehDbgCntrl *param, Buffer *mBuf));

EXTERN S16 UehMiLuehCfgReq     ARGS ((Pst *pst, UehMngmt *cfg));
EXTERN S16 UehMiLuehCfgCfm     ARGS ((Pst *pst, UehMngmt *cfg));
EXTERN S16 UehMiLuehCntrlReq   ARGS ((Pst *pst, UehMngmt *cntrl));
EXTERN S16 UehMiLuehCntrlCfm   ARGS ((Pst *pst, UehMngmt *cntrl));
EXTERN S16 UehMiLuehStaInd ARGS((Pst *pst, UehMngmt *sta));


/*---- Layer manager interface primitives ----*/

#ifdef SM
EXTERN S16 SmMiLuehCfgReq     ARGS ((Pst *pst, UehMngmt *cfg));
EXTERN S16 SmMiLuehCfgCfm     ARGS ((Pst *pst, UehMngmt *cfg));
EXTERN S16 SmMiLuehCntrlReq   ARGS ((Pst *pst, UehMngmt *cntrl));
EXTERN S16 SmMiLuehCntrlCfm   ARGS ((Pst *pst, UehMngmt *cntrl));
EXTERN S16 SmMiLuehStaInd     ARGS((Pst *pst, UehMngmt *sta));
#endif /* SM */


#ifdef SM
/*-- stack manager initialization function ---*/
EXTERN S16 smUehActvInit  ARGS ((Ent ent, Inst inst,
                                Region region, Reason reason));
/*---- stack manager activation function -----*/
EXTERN S16 smUehActvTsk   ARGS ((Pst *pst, Buffer *mBuf));

#ifdef LTE_L2_MEAS
S16 smMacFwdL2MeasReq( Pst  *pst,  LrgSchMeasReqInfo *meas);
S16 smRlcFwdL2MeasReq( Pst  *pst,  KwL2MeasReqEvt *meas);
S16 smPdcpFwdL2MeasReq( Pst  *pst,  PjL2MeasReqEvt *meas);

S16 sendRlcL2MeasCfm (Pst  *pst, KwL2MeasReqEvt *measReqEvt);
S16 sendPdcpL2MeasCfm (Pst  *pst, PjL2MeasReqEvt *measReqEvt);
S16 sendMacL2MeasCfm (Pst   *pst, LrgSchMeasReqInfo  *measInfo);

#endif

void l2MeasStub ( Pst *pst, Buffer *mBuf );
#endif

#ifdef UEH
/*-- stack manager initialization function ---*/
EXTERN S16 uehActvInit  ARGS ((Ent ent, Inst inst,
                                Region region, Reason reason));
/*---- stack manager activation function -----*/
EXTERN S16 uehActvTsk   ARGS ((Pst *pst, Buffer *mBuf));
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LUEHX__ */

/********************************************************************

        Revision history:

**********************************************************************/

