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
 
     Name:     LVE Interface
 
     Type:     C file
 
     Desc:     This file contains the packing/unpacking functions
               for RRC Layer Manager primitives on lve interface

     File:     lve.c

     Sid:      lve.c@@/main/3 - Fri Mar  4 12:31:11 2011
 
     Prg:      rcs
     
*********************************************************************21*/

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
#include "lve.h"           /* SOT interface defines           */

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
#include "lve.x"          

/*
*
*    Fun:    cmPkVeGenCfg
*
*    Desc:    pack the structure VeGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkVeGenCfg
(
VeGenCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkVeGenCfg(param ,mBuf)
VeGenCfg *param;
Buffer *mBuf;
#endif
{
   S8 idx = 0;
   TRC3(cmPkVeGenCfg);
   CMCHKPK(cmPkCmTptAddr, &param->dataAppAddr, mBuf);
#ifdef VE_RELAY
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
#endif /* end of VE_RELAY */
   CMCHKPK(cmPkPst, &param->lmPst,mBuf);
   RETVALUE(ROK);
} /*end of function cmPkVeGenCfg*/

#ifdef VE_RELAY
/*
*
*    Fun:    cmPkVeUstaDgn
*
*    Desc:    pack the structure VeUstaDgn
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkVeUstaDgn
(
VeUstaDgn *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkVeUstaDgn(param ,mBuf)
VeUstaDgn *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkVeUstaDgn)

    switch( param->type )
    {
       case  LVE_USTA_DGNVAL_SAP :
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       case LVE_USTA_DGNVAL_CELLUEID :
          CMCHKPK(SPkU16, param->u.s.cellId,mBuf);
          CMCHKPK(SPkU16, param->u.s.ueId,mBuf);
          break; 
       default:
          break;
    }
    CMCHKPK(SPkU8, param->type,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkVeUstaDgn*/
#endif 
/*
*
*    Fun:    cmPkVeUsta
*
*    Desc:    pack the structure VeUsta
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkVeUsta
(
VeUsta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkVeUsta(param ,mBuf)
VeUsta *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkVeUsta)

#ifdef VE_RELAY
    CMCHKPK(cmPkVeUstaDgn, &param->dgn,mBuf);
#endif
    CMCHKPK(cmPkCmAlarm, &param->alarm,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkVeUsta*/

/*
*
*    Fun:    cmPkVeCntrl
*
*    Desc:    pack the structure VeCntrl
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkVeCntrl
(
VeCntrl *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkVeCntrl(param ,elmnt, mBuf)
VeCntrl *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmPkVeCntrl)

    switch( elmnt )
    {
       case  STVEGEN :
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
#ifdef EU_DAT_APP
       case  STVEEGTSAP :
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       case  STVEPJUSAP :
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
#endif
       case STVERGRSAP:
       case STVECTFSAP:
       case STVENHUSAP: 
       case STVESZTSAP: 
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       default:
          break;
    }
    CMCHKPK(SPkS16, param->subAction,mBuf);
    CMCHKPK(SPkS16, param->action,mBuf);

    RETVALUE(ROK);
} /*end of function cmPkVeCntrl*/
/*
*
*    Fun:    cmPkVeLSapCfg
*
*    Desc:    pack the structure VeLSapCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkVeLSapCfg
(
VeLSapCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkVeLSapCfg(param ,mBuf)
VeLSapCfg *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkVeLSapCfg)
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
} /*end of function cmPkVeLSapCfg*/
/*
*
*    Fun:    cmPkVeCfg
*
*    Desc:    pack the structure VeCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkVeCfg
(
VeCfg *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkVeCfg(param ,elmnt, mBuf)
VeCfg *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmPkVeCfg)

    switch( elmnt )
    {
       case STVERGRSAP:
       case STVECTFSAP:
       case STVENHUSAP: 
       case STVESZTSAP: 

          CMCHKPK(cmPkVeLSapCfg, &param->u.lSap,mBuf);
          break;
       case  STVEGEN :
          CMCHKPK(cmPkVeGenCfg, &param->u.genCfg,mBuf);
          break;

#ifdef EU_DAT_APP
       case  STVEEGTSAP :
          CMCHKPK(cmPkVeLSapCfg, &param->u.lSap,mBuf);
          break;
       case  STVEPJUSAP :
          CMCHKPK(cmPkVeLSapCfg, &param->u.lSap,mBuf);
          break;
#endif
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmPkVeCfg*/
/*
*
*    Fun:    cmPkVeMngmt
*
*    Desc:    pack the structure VeMngmt
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkVeMngmt
(
VeMngmt *param,
U8 evnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkVeMngmt(param ,evnt, mBuf)
VeMngmt *param;
U8 evnt;
Buffer *mBuf;
#endif
{
    S16 ret1= ROK;
    TRC3(cmPkVeMngmt)

    switch( evnt )
    {
       case  EVTLVECFGCFM :
          ret1 = cmPkVeCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVECFGREQ :
          ret1 = cmPkVeCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVECNTRLREQ :
          ret1 = cmPkVeCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVECNTRLCFM :
          ret1 = cmPkVeCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVESTAIND :
          CMCHKPK(cmPkVeUsta, &param->u.usta,mBuf);
          break;
       default:
          break;
    }
    CMCHKPK(cmPkCmStatus, &param->cfm,mBuf);
    CMCHKPK(cmPkHeader, &param->hdr,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkVeMngmt*/
/*
*
*    Fun:    cmPkLveCfgReq
*
*    Desc:    pack the primitive LveCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLveCfgReq
(
Pst *pst,
VeMngmt *cfg
)
#else
PUBLIC S16 cmPkLveCfgReq(pst, cfg)
Pst *pst;
VeMngmt *cfg;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLveCfgReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELVE001, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkVeMngmt(cfg, EVTLVECFGREQ ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVE002, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLVECFGREQ;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLveCfgReq*/
/*
*
*    Fun:    cmPkLveCntrlReq
*
*    Desc:    pack the primitive LveCntrlReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLveCntrlReq
(
Pst *pst,
VeMngmt *cntrl
)
#else
PUBLIC S16 cmPkLveCntrlReq(pst, cntrl)
Pst *pst;
VeMngmt *cntrl;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLveCntrlReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELVE003, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkVeMngmt(cntrl, EVTLVECNTRLREQ ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVE004, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLVECNTRLREQ;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLveCntrlReq*/
#ifdef LCVEMILVE
/*
*
*    Fun:    cmPkLveCfgCfm
*
*    Desc:    pack the primitive LveCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLveCfgCfm
(
Pst *pst,
VeMngmt *cfm
)
#else
PUBLIC S16 cmPkLveCfgCfm(pst, cfm)
Pst *pst;
VeMngmt *cfm;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLveCfgCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELVE005, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkVeMngmt(cfm, EVTLVECFGCFM ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVE006, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLVECFGCFM;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLveCfgCfm*/

/*
*
*    Fun:    cmPkLveStaInd
*
*    Desc:    pack the primitive LveStaInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLveStaInd
(
Pst *pst,
VeMngmt *sta
)
#else
PUBLIC S16 cmPkLveStaInd(pst, sta)
Pst *pst;
VeMngmt *sta;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLveStaInd)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELVE007, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkVeMngmt(sta, EVTLVESTAIND ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVE008, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLVESTAIND;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLveStaInd*/
/*
*
*    Fun:    cmPkLveCntrlCfm
*
*    Desc:    pack the primitive LveCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLveCntrlCfm
(
Pst *pst,
VeMngmt *cfm
)
#else
PUBLIC S16 cmPkLveCntrlCfm(pst, cfm)
Pst *pst;
VeMngmt *cfm;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLveCntrlCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELVE009, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    ret1 = cmPkVeMngmt(cfm, EVTLVECNTRLCFM ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVE010, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    pst->event = (Event) EVTLVECNTRLCFM;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLveCntrlCfm*/
#endif 

/*
*
*    Fun:    cmUnpkVeGenCfg
*
*    Desc:    unpack the structure veGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkVeGenCfg
(
VeGenCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeGenCfg(param ,mBuf)
VeGenCfg *param;
Buffer *mBuf;
#endif
{
    U8 idx = 0;
    
    TRC3(cmUnpkVeGenCfg)

    CMCHKUNPK(cmUnpkPst, &param->lmPst,mBuf);
#ifdef VE_RELAY
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
#endif
    CMCHKUNPK(cmUnpkCmTptAddr, &param->dataAppAddr, mBuf);
    RETVALUE(ROK);
} /*end of function cmUnpkVeGenCfg*/
#ifdef VE_RELAY
/*
*
*    Fun:    cmUnpkVeUstaDgn
*
*    Desc:    unpack the structure veUstaDgn
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkVeUstaDgn
(
VeUstaDgn *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeUstaDgn(param ,mBuf)
VeUstaDgn *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkVeUstaDgn)

    CMCHKUNPK(SUnpkU8, &param->type,mBuf);
    switch( param->type )
    {
       case LVE_USTA_DGNVAL_SAP :
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       case LVE_USTA_DGNVAL_CELLUEID :
          CMCHKUNPK(SUnpkU16, &param->u.s.ueId,mBuf);
          CMCHKUNPK(SUnpkU16, &param->u.s.cellId,mBuf);
          break; 
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkVeUstaDgn*/
#endif 

/*
*
*    Fun:    cmUnpkVeUsta
*
*    Desc:    unpack the structure veUsta
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkVeUsta
(
VeUsta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeUsta(param ,mBuf)
VeUsta *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkVeUsta)

    CMCHKUNPK(cmUnpkCmAlarm, &param->alarm,mBuf);
#ifdef VE_RELAY
    CMCHKUNPK(cmUnpkVeUstaDgn, &param->dgn,mBuf);
#endif
    RETVALUE(ROK);
} /*end of function cmUnpkVeUsta*/
/*
*
*    Fun:    cmUnpkVeCntrl
*
*    Desc:    unpack the structure VeCntrl
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkVeCntrl
(
VeCntrl *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeCntrl(param ,elmnt, mBuf)
VeCntrl *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkVeCntrl)

    CMCHKUNPK(SUnpkS16, &param->action,mBuf);
    CMCHKUNPK(SUnpkS16, &param->subAction,mBuf);
    switch( elmnt )
    {
       case  STVEGEN :
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
#ifdef EU_DAT_APP
       case  STVEEGTSAP :
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       case  STVEPJUSAP :
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
#endif

       case STVERGRSAP:
       case STVECTFSAP:
       case STVENHUSAP: 
       case STVESZTSAP: 
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkVeCntrl*/

/*
*
*    Fun:    cmUnpkVeLSapCfg
*
*    Desc:    unpack the structure veLSapCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkVeLSapCfg
(
VeLSapCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeLSapCfg(param ,mBuf)
VeLSapCfg *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkVeLSapCfg)
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
} /*end of function cmUnpkVeLSapCfg*/
/*
*
*    Fun:    cmUnpkVeCfg
*
*    Desc:    unpack the structure VeCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkVeCfg
(
VeCfg *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeCfg(param ,elmnt, mBuf)
VeCfg *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkVeCfg)

    switch( elmnt )
    {
       case STVERGRSAP:
       case STVECTFSAP:
       case STVENHUSAP: 
       case STVESZTSAP: 
          CMCHKUNPK(cmUnpkVeLSapCfg, &param->u.lSap,mBuf);
          break;
       case STVEGEN:
          CMCHKUNPK(cmUnpkVeGenCfg, &param->u.genCfg,mBuf);
          break;
#ifdef EU_DAT_APP
       case  STVEEGTSAP :
          CMCHKUNPK(cmUnpkVeLSapCfg, &param->u.lSap,mBuf);
          break;
       case  STVEPJUSAP :
          CMCHKUNPK(cmUnpkVeLSapCfg, &param->u.lSap,mBuf);
          break;
#endif
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkVeCfg*/
/*
*
*    Fun:    cmUnpkVeMngmt
*
*    Desc:    unpack the structure VeMngmt
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkVeMngmt
(
VeMngmt *param,
U8 evnt,
Ptr ptr,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeMngmt(param ,evnt, ptr, mBuf)
VeMngmt *param;
U8 evnt;
Ptr ptr;
Buffer *mBuf;
#endif
{
    S16 ret1;
    TRC3(cmUnpkVeMngmt)

    UNUSED(ptr);

    CMCHKUNPK(cmUnpkHeader, &param->hdr,mBuf);
    CMCHKUNPK(cmUnpkCmStatus, &param->cfm,mBuf);
    switch( evnt )
    {
       case  EVTLVECFGCFM :
          ret1 = cmUnpkVeCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVECFGREQ :
          ret1 = cmUnpkVeCfg(&param->u.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVECNTRLREQ :
          ret1 = cmUnpkVeCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVECNTRLCFM :
          ret1 = cmUnpkVeCntrl(&param->u.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLVESTAIND :
          CMCHKUNPK(cmUnpkVeUsta, &param->u.usta,mBuf);
          break;
       default:
          RETVALUE(RFAILED);
    }
    RETVALUE(ROK);
} /*end of function cmUnpkVeMngmt*/
#ifdef LCVEMILVE
/*
*
*    Fun:    cmUnpkLveCfgReq
*
*    Desc:    unpack the primitive LveCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLveCfgReq
(
LveCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLveCfgReq(func, pst, mBuf)
LveCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    VeMngmt cfg;
    
    TRC3(cmUnpkLveCfgReq)

    ret1 = cmUnpkVeMngmt(&cfg, EVTLVECFGREQ , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELVE011, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cfg));
} /*end of function cmUnpkLveCfgReq*/
/*
*
*    Fun:    cmUnpkLveCntrlReq
*
*    Desc:    unpack the primitive LveCntrlReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLveCntrlReq
(
LveCntrlReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLveCntrlReq(func, pst, mBuf)
LveCntrlReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    VeMngmt cntrl;
    
    TRC3(cmUnpkLveCntrlReq)

    ret1 = cmUnpkVeMngmt(&cntrl, EVTLVECNTRLREQ , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELVE012, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cntrl));
} /*end of function cmUnpkLveCntrlReq*/
#endif /* LCVEMILVE */

/*
*
*    Fun:    cmUnpkLveCfgCfm
*
*    Desc:    unpack the primitive LveCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLveCfgCfm
(
LveCfgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLveCfgCfm(func, pst, mBuf)
LveCfgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    VeMngmt cfm;
    
    TRC3(cmUnpkLveCfgCfm)

    ret1 = cmUnpkVeMngmt(&cfm, EVTLVECFGCFM , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELVE013, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cfm));
} /*end of function cmUnpkLveCfgCfm*/

/*
*
*    Fun:    cmUnpkLveStaInd
*
*    Desc:    unpack the primitive LveStaInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLveStaInd
(
LveStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLveStaInd(func, pst, mBuf)
LveStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    VeMngmt sta;
    
    TRC3(cmUnpkLveStaInd)

    ret1 = cmUnpkVeMngmt(&sta, EVTLVESTAIND , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELVE014, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &sta));
} /*end of function cmUnpkLveStaInd*/

/*
*
*    Fun:    cmUnpkLveCntrlCfm
*
*    Desc:    unpack the primitive LveCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     lve.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLveCntrlCfm
(
LveCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLveCntrlCfm(func, pst, mBuf)
LveCntrlCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    VeMngmt cfm;
    
    TRC3(cmUnpkLveCntrlCfm)

    ret1 = cmUnpkVeMngmt(&cfm, EVTLVECNTRLCFM , NULLP ,mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELVE015, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &cfm));
} /*end of function cmUnpkLveCntrlCfm*/

/* lve_c_001.main_2: Adding functions for log file print changes */
#ifdef DEBUGP
/*
 *
 *    Fun:    cmVeDbgCntrl
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
PUBLIC S16 cmPkVeDbgCntrl
(
 VeDbgCntrl *param,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmPkVeDbgCntrl(param ,mBuf)
 VeDbgCntrl *param;
 Buffer *mBuf;
#endif
{
#ifdef VE_SM_LOG_TO_FILE
     Cntr i;
#endif

     TRC3(cmPkVeDbgCntrl)

#ifdef VE_SM_LOG_TO_FILE
     CMCHKPK(SPkU32, param->nmbDbgLines,mBuf);
     for (i=LVE_MAX_FILE_PATH -1;i>=0;i--)
     {
        CMCHKPK(SPkS8, param->filePath[i],mBuf);
     }
#endif /* VE_SM_LOG_TO_FILE */
     CMCHKPK(SPkU32, param->dbgMsk,mBuf);
     RETVALUE(ROK);
} /*end of function cmPkVeDbgCntrl*/

/*
*
*    Fun:    cmUnpkVeDbgCntrl
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
PUBLIC S16 cmUnpkVeDbgCntrl
(
VeDbgCntrl *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkVeDbgCntrl(param ,mBuf)
VeDbgCntrl *param;
Buffer *mBuf;
#endif
{
#ifdef VE_SM_LOG_TO_FILE
    Cntr i;
#endif
    TRC3(cmUnpkVeDbgCntrl)

    CMCHKUNPK(SUnpkU32, &param->dbgMsk,mBuf);
#ifdef VE_SM_LOG_TO_FILE
    for (i=0;i<LVE_MAX_FILE_PATH ;i++)
    {
       CMCHKUNPK(SUnpkS8, &param->filePath[i], mBuf);
    }
    CMCHKUNPK(SUnpkU32, &param->nmbDbgLines,mBuf);
#endif /* VE_SM_LOG_TO_FILE */
    RETVALUE(ROK);
} /*end of function cmUnpkVeDbgCntrl*/
#endif /*DEBUGP*/

/*********************************************************************

         End of file:     lve.c@@/main/3 - Fri Mar  4 12:31:11 2011

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
/main/3      ---      lve_c_001.main_2  rk    1. Support for logging of console 
                                     prints to a file.
*********************************************************************91*/
