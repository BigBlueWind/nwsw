/**
 * @file    lueh.c
 * @brief This file contains the packing/unpacking functions
               for RRC Layer Manager primitives on LUEH interface
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */
 
/* header include files (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_tpt.h" 
#include "lueh.h"           /* SOT interface defines           */
#ifdef LTE_L2_MEAS
#include "lrg.h"        
#include "lpj.h"        
#include "lkw.h"        
#endif
/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_tpt.x" 
#ifdef LTE_L2_MEAS
#include "cm_lte.x"
#include "ueh.h"
#include "basic_types.h"        
#include "lrg.x"        
#include "lpj.x"        
#include "lkw.x"        
#endif
#include "lueh.x"          
#ifdef LTE_L2_MEAS
EXTERN S16 cmPkLrgSchL2MeasReq (Pst *pst, LrgSchMeasReqInfo *meas);
EXTERN S16 cmPkLkwL2MeasReq (Pst *pst, KwL2MeasReqEvt *measReqEvt);
EXTERN S16 cmPkLpjL2MeasReq (Pst *pst, PjL2MeasReqEvt *measReqEvt);

#ifdef LTE_L2_MEAS_STUB
EXTERN S16 cmPkLkwL2MeasCfm (Pst *pst,KwL2MeasCfmEvt *measCfmEvt);
EXTERN S16 cmPkLpjL2MeasCfm (Pst *pst,PjL2MeasCfmEvt *measCfmEvt);
EXTERN S16 cmPkLrgSchL2MeasCfm (Pst *pst, LrgSchMeasCfmInfo  *measInfo);
#endif
#endif
/*
*
*    Fun:    cmPkUehGenCfg
*
*    Desc:    pack the structure UehGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUehGenCfg
(
UehGenCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUehGenCfg(param ,mBuf)
UehGenCfg *param;
Buffer *mBuf;
#endif
{
   S8 idx = 0;
   TRC3(cmPkUehGenCfg);
   CMCHKPK(cmPkCmTptAddr, &param->dataAppAddr, mBuf);
   CMCHKPK(SPkU8, param->cellCfg.genUeCfg.ciphAlgo, mBuf);
   CMCHKPK(SPkU8, param->cellCfg.genUeCfg.intgAlgo, mBuf);
   CMCHKPK(SPkU8, param->cellCfg.freqBandInd, mBuf);
   CMCHKPK(SPkU16, param->cellCfg.trackAreaCode, mBuf);
   for(idx = 2; idx >= 0; idx--)
   {
     cmPkTknU8(&param->cellCfg.mnc[(U8)idx], mBuf);
     cmPkTknU8(&param->cellCfg.mcc[(U8)idx], mBuf);
   }
   CMCHKPK(SPkU8, param->cellCfg.maxUeSupp, mBuf);
   CMCHKPK(SPkU16, param->cellCfg.duplexMode, mBuf);
   CMCHKPK(SPkU8, param->cellCfg.modType, mBuf);
   CMCHKPK(SPkU16, param->cellCfg.cellId, mBuf);
   CMCHKPK(cmPkPst, &param->lmPst,mBuf);
   RETVALUE(ROK);
} /*end of function cmPkUehGenCfg*/

/*
*
*    Fun:    cmPkUehUstaDgn
*
*    Desc:    pack the structure UehUstaDgn
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUehUstaDgn
(
UehUstaDgn *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUehUstaDgn(param ,mBuf)
UehUstaDgn *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkUehUstaDgn)

    switch( param->type )
    {
       case  LUEH_USTA_DGNVAL_SAP :
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       case LUEH_USTA_DGNVAL_CELLUEID :
          CMCHKPK(SPkU16, param->u.s.cellId,mBuf);
          CMCHKPK(SPkU16, param->u.s.ueId,mBuf);
          break; 
       default:
          break;
    }
    CMCHKPK(SPkU8, param->type,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkUehUstaDgn*/

/*
*
*    Fun:    cmPkUehUsta
*
*    Desc:    pack the structure UehUsta
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUehUsta
(
UehUsta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUehUsta(param ,mBuf)
UehUsta *param;
Buffer *mBuf;
#endif
{
	TRC3(cmPkUehUsta)

	CMCHKPK(cmPkUehUstaDgn, &param->dgn,mBuf);
	CMCHKPK(cmPkCmAlarm, &param->alarm,mBuf);
	RETVALUE(ROK);
} /*end of function cmPkUehUsta*/

/*
*
*    Fun:    cmPkUehCntrl
*
*    Desc:    pack the structure UehCntrl
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUehCntrl
(
UehCntrl *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUehCntrl(param ,elmnt, mBuf)
UehCntrl *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmPkUehCntrl)

    switch( elmnt )
    {
       case  STUEHGEN :
          if (param->action != ASHUTDOWN)
          {
             switch (param->subAction)
             {
                case  SADBG :
#ifdef DEBUGP
                   CMCHKPK(SPkU32, param->u.dbgMask,mBuf);
#endif /*  DEBUGP  */
                   break;

                case SAUSTA:
                   break;  

                default:
                   RETVALUE(RFAILED);
             }
          }
          break;

       case STUEHRGRSAP:
       case STUEHCTFSAP:
       case STUEHNHUSAP: 
       case STUEHSZTSAP: 
       case STUEHUDHSAP: 
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       default:
          break;
    }
    CMCHKPK(SPkS16, param->subAction,mBuf);
    CMCHKPK(SPkS16, param->action,mBuf);

    RETVALUE(ROK);
} /*end of function cmPkUehCntrl*/
/*
*
*    Fun:    cmPkUehLSapCfg
*
*    Desc:    pack the structure UehLSapCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUehLSapCfg
(
UehLSapCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUehLSapCfg(param ,mBuf)
UehLSapCfg *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkUehLSapCfg)
    CMCHKPK(cmPkTmrCfg, &param->connTmr,mBuf);
    CMCHKPK(cmPkTmrCfg, &param->bndTmr,mBuf);
    CMCHKPK(SPkU8, param->maxBndRetry,mBuf);
    CMCHKPK(SPkU8, param->mem.pool,mBuf);
    CMCHKPK(SPkU8, param->mem.region,mBuf);
    CMCHKPK(SPkU8, param->dstSel,mBuf);
    CMCHKPK(SPkU8, param->dstRoute,mBuf);
    CMCHKPK(SPkU8, param->dstPrior,mBuf);
    CMCHKPK(SPkU8, param->dstInst,mBuf);
    CMCHKPK(SPkU8, param->dstEnt,mBuf);
    CMCHKPK(SPkU16, param->dstProcId,mBuf);
    CMCHKPK(SPkU8, param->srcInst,mBuf);
    CMCHKPK(SPkU8, param->srcEnt,mBuf);
    CMCHKPK(SPkS16, param->spId,mBuf);
    CMCHKPK(SPkS16, param->suId,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkUehLSapCfg*/
/*
*
*    Fun:    cmPkUehCfg
*
*    Desc:    pack the structure UehCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUehCfg
(
UehCfg *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUehCfg(param ,elmnt, mBuf)
UehCfg *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmPkUehCfg)

    switch( elmnt )
    {
       case STUEHRGRSAP:
       case STUEHCTFSAP:
       case STUEHNHUSAP: 
       case STUEHSZTSAP: 
       case STUEHUDHSAP: 
          CMCHKPK(cmPkUehLSapCfg, &param->u.lSap,mBuf);
          break;
       case  STUEHGEN :
          CMCHKPK(cmPkUehGenCfg, &param->u.genCfg,mBuf);
          break;

       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmPkUehCfg*/
/*
*
*    Fun:    cmPkUehMngmt
*
*    Desc:    pack the structure UehMngmt
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUehMngmt
(
UehMngmt *param,
U8 evnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUehMngmt(param ,evnt, mBuf)
UehMngmt *param;
U8 evnt;
Buffer *mBuf;
#endif
{
    S16 ret1= ROK;
    TRC3(cmPkUehMngmt)

    switch( evnt )
    {
       case  EVTLUEHCFGCFM :
          ret1 = cmPkUehCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHCFGREQ :
          ret1 = cmPkUehCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHCNTRLREQ :
          ret1 = cmPkUehCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHCNTRLCFM :
          ret1 = cmPkUehCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHSTAIND :
          CMCHKPK(cmPkUehUsta, &param->u.usta,mBuf);
          break;
       default:
          break;
    }
    CMCHKPK(cmPkCmStatus, &param->cfm,mBuf);
    CMCHKPK(cmPkHeader, &param->hdr,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkUehMngmt*/
/*
*
*    Fun:    cmPkLuehCfgReq
*
*    Desc:    pack the primitive LuehCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLuehCfgReq
(
Pst *pst,
UehMngmt *cfg
)
#else
PUBLIC S16 cmPkLuehCfgReq(pst, cfg)
Pst *pst;
UehMngmt *cfg;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLuehCfgReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELUEH001, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkUehMngmt(cfg, EVTLUEHCFGREQ ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELUEH002, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLUEHCFGREQ;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLuehCfgReq*/
/*
*
*    Fun:    cmPkLuehCntrlReq
*
*    Desc:    pack the primitive LuehCntrlReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLuehCntrlReq
(
Pst *pst,
UehMngmt *cntrl
)
#else
PUBLIC S16 cmPkLuehCntrlReq(pst, cntrl)
Pst *pst;
UehMngmt *cntrl;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLuehCntrlReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELUEH003, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkUehMngmt(cntrl, EVTLUEHCNTRLREQ ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELUEH004, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLUEHCNTRLREQ;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLuehCntrlReq*/
#ifdef LCUEHMILUEH
/*
*
*    Fun:    cmPkLuehCfgCfm
*
*    Desc:    pack the primitive LuehCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLuehCfgCfm
(
Pst *pst,
UehMngmt *cfm
)
#else
PUBLIC S16 cmPkLuehCfgCfm(pst, cfm)
Pst *pst;
UehMngmt *cfm;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLuehCfgCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELUEH005, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkUehMngmt(cfm, EVTLUEHCFGCFM ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELUEH006, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLUEHCFGCFM;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLuehCfgCfm*/

/*
*
*    Fun:    cmPkLuehStaInd
*
*    Desc:    pack the primitive LuehStaInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLuehStaInd
(
Pst *pst,
UehMngmt *sta
)
#else
PUBLIC S16 cmPkLuehStaInd(pst, sta)
Pst *pst;
UehMngmt *sta;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLuehStaInd)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELUEH007, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkUehMngmt(sta, EVTLUEHSTAIND ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELUEH008, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLUEHSTAIND;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLuehStaInd*/
/*
*
*    Fun:    cmPkLveCntrlCfm
*
*    Desc:    pack the primitive LuehCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLuehCntrlCfm
(
Pst *pst,
UehMngmt *cfm
)
#else
PUBLIC S16 cmPkLuehCntrlCfm(pst, cfm)
Pst *pst;
UehMngmt *cfm;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLuehCntrlCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELUEH009, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkUehMngmt(cfm, EVTLUEHCNTRLCFM ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELUEH010, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLUEHCNTRLCFM;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLuehCntrlCfm*/
#endif 

/*
*
*    Fun:    cmUnpkUehGenCfg
*
*    Desc:    unpack the structure uehGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehGenCfg
(
UehGenCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehGenCfg(param ,mBuf)
UehGenCfg *param;
Buffer *mBuf;
#endif
{
    U8 idx = 0;
    
    TRC3(cmUnpkUehGenCfg)

    CMCHKUNPK(cmUnpkPst, &param->lmPst,mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.cellId, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.modType, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.duplexMode, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.maxUeSupp, mBuf);
    for (idx = 0; idx < 3; idx++)
    {
       cmUnpkTknU8(&param->cellCfg.mcc[idx], mBuf);
       cmUnpkTknU8(&param->cellCfg.mnc[idx], mBuf);
    }
    CMCHKUNPK(SUnpkU16, &param->cellCfg.trackAreaCode, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.freqBandInd, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.genUeCfg.intgAlgo, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.genUeCfg.ciphAlgo, mBuf);
    CMCHKUNPK(cmUnpkCmTptAddr, &param->dataAppAddr, mBuf);
    RETVALUE(ROK);
} /*end of function cmUnpkUehGenCfg*/

/*
*
*    Fun:    cmUnpkUehUstaDgn
*
*    Desc:    unpack the structure uehUstaDgn
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehUstaDgn
(
UehUstaDgn *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehUstaDgn(param ,mBuf)
UehUstaDgn *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkUehUstaDgn)

    CMCHKUNPK(SUnpkU8, &param->type,mBuf);
    switch( param->type )
    {
       case LUEH_USTA_DGNVAL_SAP :
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       case LUEH_USTA_DGNVAL_CELLUEID :
          CMCHKUNPK(SUnpkU16, &param->u.s.ueId,mBuf);
          CMCHKUNPK(SUnpkU16, &param->u.s.cellId,mBuf);
          break; 
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkUehUstaDgn*/


/*
*
*    Fun:    cmUnpkUehUsta
*
*    Desc:    unpack the structure uehUsta
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehUsta
(
UehUsta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehUsta(param ,mBuf)
UehUsta *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkUehUsta)

    CMCHKUNPK(cmUnpkCmAlarm, &param->alarm,mBuf);
    CMCHKUNPK(cmUnpkUehUstaDgn, &param->dgn,mBuf);
    RETVALUE(ROK);
} /*end of function cmUnpkUehUsta*/
/*
*
*    Fun:    cmUnpkUehCntrl
*
*    Desc:    unpack the structure UehCntrl
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehCntrl
(
UehCntrl *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehCntrl(param ,elmnt, mBuf)
UehCntrl *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkUehCntrl)

    CMCHKUNPK(SUnpkS16, &param->action,mBuf);
    CMCHKUNPK(SUnpkS16, &param->subAction,mBuf);
    switch( elmnt )
    {
       case  STUEHGEN :
          if (param->action != ASHUTDOWN)
          {
             switch (param->subAction)
             {
                case  SADBG :
#ifdef DEBUGP
                   CMCHKUNPK(SUnpkU32, &param->u.dbgMask,mBuf);
#endif /*  DEBUGP  */
                   break;

               case SAUSTA:
                  break;  

                default:
                   RETVALUE(RFAILED);
             }
          }
          break;

       case STUEHRGRSAP:
       case STUEHCTFSAP:
       case STUEHNHUSAP: 
       case STUEHSZTSAP: 
       case STUEHUDHSAP: 
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkUehCntrl*/

/*
*
*    Fun:    cmUnpkUehLSapCfg
*
*    Desc:    unpack the structure uehLSapCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehLSapCfg
(
UehLSapCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehLSapCfg(param ,mBuf)
UehLSapCfg *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkUehLSapCfg)
    CMCHKUNPK(SUnpkS16, &param->suId,mBuf);
    CMCHKUNPK(SUnpkS16, &param->spId,mBuf);
    CMCHKUNPK(SUnpkU8, &param->srcEnt,mBuf);
    CMCHKUNPK(SUnpkU8, &param->srcInst,mBuf);
    CMCHKUNPK(SUnpkU16, &param->dstProcId,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstEnt,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstInst,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstPrior,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstRoute,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstSel,mBuf);
    CMCHKUNPK(SUnpkU8, &param->mem.region,mBuf);
    CMCHKUNPK(SUnpkU8, &param->mem.pool,mBuf);
    CMCHKUNPK(SUnpkU8, &param->maxBndRetry,mBuf);
    CMCHKUNPK(cmUnpkTmrCfg, &param->bndTmr,mBuf);
    CMCHKUNPK(cmUnpkTmrCfg, &param->connTmr,mBuf);
    RETVALUE(ROK);
} /*end of function cmUnpkUehLSapCfg*/
/*
*
*    Fun:    cmUnpkUehCfg
*
*    Desc:    unpack the structure UehCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehCfg
(
UehCfg *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehCfg(param ,elmnt, mBuf)
UehCfg *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkUehCfg)

    switch( elmnt )
    {
       case STUEHRGRSAP:
       case STUEHCTFSAP:
       case STUEHNHUSAP: 
       case STUEHSZTSAP: 
       case STUEHUDHSAP: 
          CMCHKUNPK(cmUnpkUehLSapCfg, &param->u.lSap,mBuf);
          break;
       case STUEHGEN:
          CMCHKUNPK(cmUnpkUehGenCfg, &param->u.genCfg,mBuf);
          break;

       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkUehCfg*/
/*
*
*    Fun:    cmUnpkUehMngmt
*
*    Desc:    unpack the structure UehMngmt
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehMngmt
(
UehMngmt *param,
U8 evnt,
Ptr ptr,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehMngmt(param ,evnt, ptr, mBuf)
UehMngmt *param;
U8 evnt;
Ptr ptr;
Buffer *mBuf;
#endif
{
    S16 ret1;
    TRC3(cmUnpkUehMngmt)

    UNUSED(ptr);

    CMCHKUNPK(cmUnpkHeader, &param->hdr,mBuf);
    CMCHKUNPK(cmUnpkCmStatus, &param->cfm,mBuf);
    switch( evnt )
    {
       case  EVTLUEHCFGCFM :
          ret1 = cmUnpkUehCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHCFGREQ :
          ret1 = cmUnpkUehCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHCNTRLREQ :
          ret1 = cmUnpkUehCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHCNTRLCFM :
          ret1 = cmUnpkUehCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLUEHSTAIND :
          CMCHKUNPK(cmUnpkUehUsta, &param->u.usta,mBuf);
          break;
       default:
          RETVALUE(RFAILED);
    }
    RETVALUE(ROK);
} /*end of function cmUnpkUehMngmt*/
#ifdef LCUEHMILUEH
/*
*
*    Fun:    cmUnpkLuehCfgReq
*
*    Desc:    unpack the primitive LuehCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLuehCfgReq
(
LuehCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLuehCfgReq(func, pst, mBuf)
LuehCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    UehMngmt cfg;
    
    TRC3(cmUnpkLuehCfgReq)

    ret1 = cmUnpkUehMngmt(&cfg, EVTLUEHCFGREQ , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELUEH011, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cfg));
} /*end of function cmUnpkLuehCfgReq*/
/*
*
*    Fun:    cmUnpkLuehCntrlReq
*
*    Desc:    unpack the primitive LuehCntrlReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLuehCntrlReq
(
LuehCntrlReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLuehCntrlReq(func, pst, mBuf)
LuehCntrlReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    UehMngmt cntrl;
    
    TRC3(cmUnpkLuehCntrlReq)

    ret1 = cmUnpkUehMngmt(&cntrl, EVTLUEHCNTRLREQ , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELUEH012, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cntrl));
} /*end of function cmUnpkLuehCntrlReq*/
#endif /* LCUEHMILUEH */

/*
*
*    Fun:    cmUnpkLuehCfgCfm
*
*    Desc:    unpack the primitive LuehCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLuehCfgCfm
(
LuehCfgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLuehCfgCfm(func, pst, mBuf)
LuehCfgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    UehMngmt cfm;
    
    TRC3(cmUnpkLuehCfgCfm)

    ret1 = cmUnpkUehMngmt(&cfm, EVTLUEHCFGCFM , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELUEH013, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cfm));
} /*end of function cmUnpkLuehCfgCfm*/

/*
*
*    Fun:    cmUnpkLuehStaInd
*
*    Desc:    unpack the primitive LuehStaInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLuehStaInd
(
LuehStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLuehStaInd(func, pst, mBuf)
LuehStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    UehMngmt sta;
    
    TRC3(cmUnpkLuehStaInd)

    ret1 = cmUnpkUehMngmt(&sta, EVTLUEHSTAIND , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELUEH014, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &sta));
} /*end of function cmUnpkLuehStaInd*/

/*
*
*    Fun:    cmUnpkLuehCntrlCfm
*
*    Desc:    unpack the primitive LuehCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lueh.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLuehCntrlCfm
(
LuehCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLuehCntrlCfm(func, pst, mBuf)
LuehCntrlCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    UehMngmt cfm;
    
    TRC3(cmUnpkLuehCntrlCfm)

    ret1 = cmUnpkUehMngmt(&cfm, EVTLUEHCNTRLCFM , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELUEH015, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cfm));
} /*end of function cmUnpkLuehCntrlCfm*/

#ifdef DEBUGP
/*
 *
 *    Fun:    cmUehDbgCntrl
 *
 *    Desc:    pack the structure veDbgCntrl
 *
 *    Ret:    ROK  -ok
 *
 *    Notes:    None
 *
 *    File:     lve.c
 *
 */
#ifdef ANSI
PUBLIC S16 cmPkUehDbgCntrl
(
 UehDbgCntrl *param,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmPkUehDbgCntrl(param ,mBuf)
 UehDbgCntrl *param;
 Buffer *mBuf;
#endif
{
     TRC3(cmPkUehDbgCntrl)

     CMCHKPK(SPkU32, param->dbgMsk,mBuf);
     RETVALUE(ROK);
} /*end of function cmPkUehDbgCntrl*/

/*
*
*    Fun:    cmUnpkUehDbgCntrl
*
*    Desc:    unpack the structure veDbgCntrl
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUehDbgCntrl
(
UehDbgCntrl *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUehDbgCntrl(param ,mBuf)
UehDbgCntrl *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkUehDbgCntrl)

    CMCHKUNPK(SUnpkU32, &param->dbgMsk,mBuf);

    RETVALUE(ROK);
} /*end of function cmUnpkUehDbgCntrl*/
#endif /*DEBUGP*/

#ifdef LTE_L2_MEAS
PUBLIC S16 smRlcFwdL2MeasCfm
(
Pst     *pst, 
Buffer  *mBuf
)
{
   TRC3(smRlcFwdL2MeasCfm);
   pst->selector  = SM_SELECTOR_LC;
   pst->srcEnt    = ENTSM;
   pst->dstEnt    = ENTUEH;
   pst->dstProcId = SM_UEH_PROC;
   pst->srcProcId = SM_SM_PROC;

   pst->event = (Event) EVTLUEHRLCL2MEASCFM; 
   RETVALUE(SPstTsk(pst,mBuf));
}

S16 smPdcpFwdL2MeasCfm
(
Pst     *pst, 
Buffer  *mBuf
)
{
   TRC3(smPdcpFwdL2MeasCfm);
   pst->selector  = SM_SELECTOR_LC;
   pst->srcEnt    = ENTSM;
   pst->dstEnt    = ENTUEH;
   pst->dstProcId = SM_UEH_PROC;
   pst->srcProcId = SM_SM_PROC;
    
   pst->event = (Event) EVTLUEHPDCPL2MEASCFM; 
   RETVALUE(SPstTsk(pst,mBuf));
}

S16 smMacFwdL2MeasCfm
(
Pst     *pst, 
Buffer  *mBuf
)
{
   TRC3(smMacFwdL2MeasCfm);
   pst->selector  = SM_SELECTOR_LC;
   pst->srcEnt    = ENTSM;
   pst->dstEnt    = ENTUEH;
   pst->dstProcId = SM_UEH_PROC;
   pst->srcProcId = SM_SM_PROC;
   pst->srcInst   = 0;
   pst->dstInst   = 0;
    
   pst->event = (Event) EVTLUEHMACL2MEASCFM; 
   RETVALUE(SPstTsk(pst,mBuf));
}

S16 smMacFwdL2MeasReq( Pst  *pst,  LrgSchMeasReqInfo *meas)
{
   TRC3(smMacFwdL2MeasReq);
   
   clearBytes(pst,sizeof(Pst));
   pst->selector  = SM_SELECTOR_LC;
   pst->srcEnt    = ENTSM;
   pst->srcProcId = SM_SM_PROC;
   pst->srcInst   = 0;
#ifdef LTE_L2_MEAS_STUB
   pst->dstEnt    = ENTSM;
   pst->dstProcId = SM_SM_PROC;
   pst->dstInst   = 0;
#else
   pst->dstEnt    = ENTRG;
   pst->dstProcId = SM_RG_PROC;
   pst->dstInst   = SCH_INST_ID;
#endif
   cmPkLrgSchL2MeasReq(pst , meas);
   return ROK;
}

S16 smRlcFwdL2MeasReq( Pst  *pst,  KwL2MeasReqEvt *meas)
{
   TRC3(smRlcFwdL2MeasReq)
   
   pst->selector  = SM_SELECTOR_LC;
   pst->srcEnt    = ENTSM;
   pst->srcProcId = SM_SM_PROC;
   pst->srcInst   = 0;
#ifdef LTE_L2_MEAS_STUB
   pst->dstEnt    = ENTSM;
   pst->dstProcId = SM_SM_PROC;
   pst->dstInst   = 0;
#else
   pst->dstEnt    = ENTKW;
   pst->dstProcId = SM_KW_PROC;
   pst->dstInst   = 0;
#endif
    
   cmPkLkwL2MeasReq(pst , meas);
   SPutSBuf(pst->region,pst->pool,(U8 *)meas,sizeof(KwL2MeasReqEvt));
   return ROK;
}

S16 smPdcpFwdL2MeasReq( Pst  *pst,  PjL2MeasReqEvt *meas)
{
   TRC3(smPdcpFwdL2MeasReq)

   pst->selector  = SM_SELECTOR_LC;
   pst->srcEnt    = ENTSM;
   pst->srcProcId = SM_SM_PROC;
   pst->srcInst   = 0;

#ifdef LTE_L2_MEAS_STUB
   pst->dstEnt    = ENTSM;
   pst->dstProcId = SM_SM_PROC;
   pst->dstInst   = 0;
#else
   pst->dstEnt    = ENTPJ;
   pst->dstProcId = SM_PJ_PROC;
   pst->dstInst   = 0;
#endif
   
   cmPkLpjL2MeasReq(pst , meas);
   return ROK;
}

#ifdef LTE_L2_MEAS_STUB
PUBLIC S16 sendRlcL2MeasCfm 
(
Pst            *pst, 
KwL2MeasReqEvt *measReqEvt 
)
{
   S16            ret = ROK;
   U8             numQci = 0;
   static U32     dataRlc = 0;
   KwL2MeasCfmEvt measCfmEvt;
   U32            i = 0; 
         
  clearBytes(&measCfmEvt, sizeof(KwL2MeasCfmEvt));

  measCfmEvt.transId       = measReqEvt->transId;
  measCfmEvt.measType      = measReqEvt->measReq.measType;
  measCfmEvt.status.status = LCM_PRIM_OK;
  measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
  numQci            = measReqEvt->measReq.val.nonIpThMeas.numQci;
  measCfmEvt.numCfm = numQci;
  dataRlc++;
  if (dataRlc == 51)
  {
    dataRlc = 1;
  }
  for(i = 0; i<numQci; i++)
  {
    measCfmEvt.measCfm[i].qci = measReqEvt->measReq.val.nonIpThMeas.qci[i];
    measCfmEvt.measCfm[i].val.nonIpThrput.numActUe = dataRlc;
    measCfmEvt.measCfm[i].val.nonIpThrput.uuLoss = dataRlc;
  }
  
  SPutSBuf(pst->region,pst->pool,(U8 *)measReqEvt,sizeof(KwL2MeasReqEvt));

  pst->selector  = SM_SELECTOR_LC;
   
  pst->dstEnt = ENTSM;
  pst->dstProcId = SM_SM_PROC;
  pst->dstInst   = 0;
  
  pst->srcProcId = SM_KW_PROC;
  pst->srcEnt = ENTKW;
  pst->srcInst   = 0;

  cmPkLkwL2MeasCfm(pst, &measCfmEvt);

  return ret;
}

 
PUBLIC S16 sendPdcpL2MeasCfm 
(
Pst            *pst, 
PjL2MeasReqEvt *measReqEvt 
)
{
   S16             ret = ROK;
   U8              numQci = 0;
   static U32      dataPdcp = 100;
   PjL2MeasCfmEvt  measCfmEvt;
   U32             i = 0; 
 
  dataPdcp++;
  if (dataPdcp == 151)
  {
    dataPdcp = 101;
  }
  clearBytes(&measCfmEvt, sizeof(PjL2MeasCfmEvt));

  measCfmEvt.transId       = measReqEvt->transId;
  measCfmEvt.measType      = measReqEvt->measReq.measType;
  measCfmEvt.status.status = LCM_PRIM_OK;
  measCfmEvt.status.reason = LCM_REASON_NOT_APPL;

  numQci = measReqEvt->measReq.numQci;
  measCfmEvt.numCfm = numQci;

  for(i = 0; i<numQci; i++)
  {
    measCfmEvt.measCfm[i].qci     = measReqEvt->measReq.qci[i];
    measCfmEvt.measCfm[i].dlDelay = dataPdcp;
    measCfmEvt.measCfm[i].dlDisc  = dataPdcp;
    measCfmEvt.measCfm[i].ulLoss  = dataPdcp;
  }
  
  pst->selector  = SM_SELECTOR_LC;
   
  pst->dstEnt    = ENTSM;
  pst->dstProcId = SM_SM_PROC;
  pst->dstInst   = 0;
  
  pst->srcProcId = SM_PJ_PROC;
  pst->srcEnt    = ENTPJ;
  pst->srcInst   = 0;

  cmPkLpjL2MeasCfm(pst, &measCfmEvt);

  return ret;
} 

PUBLIC S16 sendMacL2MeasCfm 
(
Pst                *pst, 
LrgSchMeasReqInfo  *measInfo
)
{
   S16                 ret = ROK;
   U8                  numQci = 0;
   static U32          dataMac = 200;
   LrgSchMeasCfmInfo   measCfm;
   U32                 i = 0; 
 
  dataMac++;
  if (dataMac == 251)
  {
    dataMac = 201;
  }
  clearBytes(&measCfm, sizeof(LrgSchMeasCfmInfo));

  measCfm.hdr.transId     = measInfo->hdr.transId;
  measCfm.measType        = measInfo->measType;
  measCfm.cfm.status      = LCM_PRIM_OK;
  measCfm.cfm.reason      = LCM_REASON_NOT_APPL;
  
  measCfm.avgPrbUl.prbPerc = dataMac; 
  measCfm.avgPrbDl.prbPerc = dataMac; 
  
  measCfm.raPrmbsCfm.dedPreambles = dataMac;  
  measCfm.raPrmbsCfm.randSelPreLowRange = dataMac;
  measCfm.raPrmbsCfm.randSelPreHighRange = dataMac;
   
  switch (measInfo->measType) 
  {
    case LRG_L2MEAS_AVG_PRB_PER_QCI_DL:
     numQci = measInfo->avgPrbQciDl.numQci;
     measCfm.avgPrbQciDlCfm.numQci = numQci; 
     for(i = 0; i<numQci; i++)
     {
       measCfm.avgPrbQciDlCfm.prbPercQci[i].prbPercQci = dataMac;
       measCfm.avgPrbQciDlCfm.prbPercQci[i].qciValue = measInfo->avgPrbQciDl.qci[i];
     }
     break;

    case LRG_L2MEAS_AVG_PRB_PER_QCI_UL:
     numQci = measInfo->avgPrbQciUl.numQci;
     measCfm.avgPrbQciUlCfm.numQci = numQci; 
     for(i = 0; i<numQci; i++)
     {
       measCfm.avgPrbQciUlCfm.prbPercQci[i].prbPercQci = dataMac;
       measCfm.avgPrbQciUlCfm.prbPercQci[i].qciValue = measInfo->avgPrbQciUl.qci[i];
     }
     break;

    case LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL:
     numQci = measInfo->nmbActvUeQciDl.numQci;
     measCfm.numUeQciDlCfm.numQci = numQci; 
     for(i = 0; i<numQci; i++)
     {
       measCfm.numUeQciDlCfm.numActvUeQci[i].numActvUeQci = dataMac;
       measCfm.numUeQciDlCfm.numActvUeQci[i].qciValue = measInfo->nmbActvUeQciDl.qci[i];
     }
     break;

    case LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL:
     numQci = measInfo->nmbActvUeQciUl.numQci;
     measCfm.numUeQciUlCfm.numQci = numQci; 
     for(i = 0; i<numQci; i++)
     {
       measCfm.numUeQciUlCfm.numActvUeQci[i].numActvUeQci = dataMac;
       measCfm.numUeQciUlCfm.numActvUeQci[i].qciValue = measInfo->nmbActvUeQciUl.qci[i];
     }
     break;

    default:
    break;
  }
  pst->selector  = SM_SELECTOR_LC;
  pst->dstEnt    = ENTSM;
  pst->dstProcId = SM_SM_PROC;
  pst->dstInst   = 0;
  pst->srcProcId = SM_RG_PROC;
  pst->srcEnt    = ENTRG;
  pst->srcInst   = 1;

  cmPkLrgSchL2MeasCfm(pst, &measCfm);

  return ret;
}
#endif /*LTE_L2_MEAS_STUB */
 
#endif /*LTE_L2_MEAS */

/********************************************************************

        Revision history:

**********************************************************************/
