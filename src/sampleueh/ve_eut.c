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

     Name:     LTE RRM-RELAY and DATA APP interface

     Type:     C source file

     Desc:     This file contains call back functions for NHU
               interface.

     File:     ve_eut.c

     Sid:      ve_eut.c@@/main/2 - Wed Dec  1 08:44:24 2010

     Prg:      

*********************************************************************21*/


#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_tpt.h"           /* system services */
#include "cm_lte.h"           /* system services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_mblk.h"       /* Memory Link List */
#include "szt.h"           /* Event structure file */
#include "szt_asn.h"
#include "ve_sm_init.h"    /* Stack Manager file */
#include "ve_eut.h"

#include "cm_llist.x"           /* system services */
#include"gen.x"           /* general layer */
#include"ssi.x"           /* system services */
#include"cm_tkns.x"       /* common tokens */
#include"cm_mblk.x"       /* Memory Link List typedefs */
#include"cm_lib.x"        /* common ss7 */
#include"cm_tpt.x"        /* common ss7 */
#include"cm_lte.x"        /* common ss7 */
#include "szt.x"
#include "szt_asn.x"
#include "szt_pk.x"
#include "szt_unpk.x"
#include "egt.x"
#include "ve_eut.x"

/*
*
*    Fun:    cmPkEuRelReq
*
*    Desc:   pack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkEuRelReq
(
Pst *pst,
SuId suId,
U32  ueId
)
#else
PUBLIC S16 cmPkEuRelReq(pst, suId, ueId)
Pst *pst;
SuId suId;
U32  ueId;
#endif
{
   S16      ret1 = ROK;
   Buffer   *mBuf = NULLP;

   TRC3(cmPkEuRelReq);

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   CMCHKPKLOG(SPkU32, ueId, mBuf,EUTXXX, pst);

   pst->event = (Event) EVTVERELREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmUnpkEuRelReq
*
*    Desc:   Unpack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkEuRelReq
(
S1APRelReq func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 cmUnpkEuRelReq(func, pst, mBuf,sMem)
S1APRelReq func;
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   U32      ueId;

   TRC3(cmUnpkEuRelReq);

   CMCHKUNPK(SUnpkU32, &ueId, mBuf);

   SPutMsg(mBuf);
   
   RETVALUE((*func)(pst, ueId));
} /* end of cmUnpkEuRelReq */


/*
*
*    Fun:    cmPkEuCreattLocTunReq
*
*    Desc:   pack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkEuCreattLocTunReq
(
Pst *pst,
SuId suId,
EgtpTunInf *tunelInf
)
#else
PUBLIC S16 cmPkEuCreattLocTunReq(pst, suId, tunelInf)
Pst *pst;
SuId suId;
EgtpTunInf *tunelInf;
#endif
{
   S16      ret1 = ROK;
   Buffer   *mBuf = NULLP;

   TRC3(cmPkEuCreattLocTunReq);

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   CMCHKPKLOG(cmPkLtePdcpId, &tunelInf->pdcpId, mBuf, EUTXXX, pst);
   CMCHKPKLOG(cmPkCmTptAddr, &tunelInf->frmDstIpAddr, mBuf, EUTXXX, pst);
   CMCHKPKLOG(cmPkCmTptAddr,&tunelInf->dstIpAddr, mBuf, EUTXXX, pst);
   CMCHKPKLOG(SPkU32, tunelInf->remTeid, mBuf,EUTXXX, pst);
   CMCHKPKLOG(SPkU32, tunelInf->locTeid, mBuf,EUTXXX, pst);

    pst->event = (Event) EVTVECREATEEGTPTUNEL;
   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmPkEuDelLocTunReq
*
*    Desc:   pack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkEuDelLocTunReq
(
Pst *pst,
SuId suId,
U32  locTeid
)
#else
PUBLIC S16 cmPkEuDelLocTunReq(pst, suId, locTeid)
Pst *pst;
SuId suId;
U32  locTeid;
#endif
{
   S16 ret1 = ROK;
   Buffer *mBuf = NULLP;

   TRC3(cmPkEuDelLocTunReq);

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   CMCHKPKLOG(SPkU32, locTeid, mBuf,EUTXXX, pst);

   CMCHKPKLOG(SPkS16, suId, mBuf, EUTXXX, pst);
    pst->event = (Event) EVTVEDELEGTPTUNEL;
   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmUnpkEuCreattLocTunReq
*
*    Desc:   Unpack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkEuCreattLocTunReq
(
EgtpTunCreat func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 cmUnpkEuCreattLocTunReq(func, pst, mBuf,sMem)
EgtpTunCreat func;
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   EgtpTunInf unpkStruct;

   TRC3(cmUnpkEuCreattLocTunReq);

   CMCHKUNPK(SUnpkU32,         &unpkStruct.locTeid, mBuf);
   CMCHKUNPK(SUnpkU32,         &unpkStruct.remTeid, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr,  &unpkStruct.dstIpAddr, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr,  &unpkStruct.frmDstIpAddr, mBuf);
   CMCHKUNPK(cmUnpkLtePdcpId,  &unpkStruct.pdcpId, mBuf);

   SPutMsg(mBuf);
   
   RETVALUE((*func)(pst, VE_EG_SUID, &unpkStruct));
}

/*
*
*    Fun:    cmUnpkEuDelLocTunReq
*
*    Desc:   Unpack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkEuDelLocTunReq
(
EgtpTunDel func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 cmUnpkEuDelLocTunReq(func, pst, mBuf,sMem)
EgtpTunDel func;
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   U32 locTeid;
   SuId suId;

   TRC3(cmUnpkEuDelLocTunReq);

   CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EUTXXX, pst);
   CMCHKUNPK(SUnpkU32,&locTeid, mBuf);
   SPutMsg(mBuf);
   
   RETVALUE((*func)(pst, suId,locTeid));
}

/*
*
*    Fun:    cmPkEuCreattLocTunCfm
*
*    Desc:   pack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkEuCreattLocTunCfm
(
Pst *pst,
SpId spId,
CmLtePdcpId *pdcpId
)
#else
PUBLIC S16 cmPkEuCreattLocTunCfm(pst, spId, pdcpId)
Pst *pst;
SpId spId;
CmLtePdcpId *pdcpId;
#endif
{
   S16     ret1 = ROK;
   Buffer  *mBuf = NULLP;

   TRC3(cmPkEuCreattLocTunCfm);

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   CMCHKPKLOG(cmPkLtePdcpId, pdcpId, mBuf, EUTXXX, pst);
   pst->event = (Event) EVTVECREATEEGTPTUNELCFM;

   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmUnpkEuCreattLocTunCfm
*
*    Desc:   Unpack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkEuCreattLocTunCfm
(
EgtpTunCreatCfm func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 cmUnpkEuCreattLocTunCfm(func, pst, mBuf,sMem)
EgtpTunCreatCfm func;
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   CmLtePdcpId pdcpId;

   TRC3(cmUnpkEuCreattLocTunCfm);

   CMCHKUNPK(cmUnpkLtePdcpId, &pdcpId, mBuf);
   SPutMsg(mBuf);
   
   RETVALUE((*func)(pst, VE_EG_SUID, &pdcpId));
}

/*
*
*    Fun:    cmPkEuDelLocTunCfm
*
*    Desc:   pack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkEuDelLocTunCfm
(
Pst *pst,
SpId spId,
CmLtePdcpId *pdcpId
)
#else
PUBLIC S16 cmPkEuDelLocTunCfm(pst, spId, pdcpId)
Pst *pst;
SpId spId;
CmLtePdcpId *pdcpId;
#endif
{
   S16     ret1 = ROK;
   Buffer  *mBuf = NULLP;

   TRC3(cmPkEuDelLocTunCfm);

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   CMCHKPKLOG(cmPkLtePdcpId, pdcpId, mBuf, EUTXXX, pst);
   pst->event = (Event) EVTVEDELEGTPTUNELCFM;
printf("\n posting EVTVEDELEGTPTUNELCFM to eNodeB App, got cfm from eGTP for DELETE Req \n");

   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmUnpkEuDelLocTunCfm
*
*    Desc:   Unpack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkEuDelLocTunCfm
(
EgtpTunCreatCfm func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 cmUnpkEuDelLocTunCfm(func, pst, mBuf,sMem)
EgtpTunCreatCfm func;
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   CmLtePdcpId pdcpId;

   TRC3(cmUnpkEuDelLocTunCfm);

   CMCHKUNPK(cmUnpkLtePdcpId, &pdcpId, mBuf);
   SPutMsg(mBuf);
   
   RETVALUE((*func)(pst, VE_EG_SUID, &pdcpId));
} /* End of cmUnpkEuDelLocTunCfm */
/* End of 29_12_11 */

/*
*
*    Fun:    cmPkChngStReq
*
*    Desc:   pack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkChngStReq
(
Pst *pst,
SuId suId,
VeEuChngTnlStInfo *stInfo
)
#else
PUBLIC S16 cmPkChngStReq(pst, suId, stInfo)
Pst *pst;
SuId suId;
VeEuChngTnlStInfo *stInfo;
#endif
{
   Buffer  *mBuf = NULLP;
   U16     ret1 = 0;
   
   TRC3(cmPkChngStReq);
   
   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkU8, stInfo->cause, mBuf,EUTXXX, pst);
   CMCHKPKLOG(SPkU8, stInfo->state, mBuf,EUTXXX, pst);
   CMCHKPKLOG(SPkU32, stInfo->teId, mBuf,EUTXXX, pst);

   pst->event = (Event) EVTVECHANGESTATE;
   RETVALUE(SPstTsk(pst,mBuf));
}/* end of cmPkChngStReq */

/*
*
*    Fun:    cmUnpkEuChngStReq
*
*    Desc:   Unpack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkEuChngStReq
(
EgtpChngState func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 cmUnpkEuChngStReq(func, pst, mBuf, sMem)
EgtpChngState func;
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   VeEuChngTnlStInfo stInfo;
   TRC3(cmUnpkEuChngStReq);

   CMCHKUNPK(SUnpkU32, &stInfo.teId, mBuf);
   CMCHKUNPK(SUnpkU8, &stInfo.state, mBuf);
   CMCHKUNPK(SUnpkU8, &stInfo.cause, mBuf);

   SPutMsg(mBuf);

   RETVALUE((*func)(pst, VE_EG_SUID, &stInfo));

} /* end of cmUnpkEuChngStReq */


#ifdef LTE_HO_SUPPORT
/*
*
*    Fun:    cmPkEuCreattLocTunSetReq
*
*    Desc:   pack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkEuCreattLocTunSetReq
(
Pst *pst,
SuId suId,
EuCrEGtpTunInf *tunelInf
)
#else
PUBLIC S16 cmPkEuCreattLocTunSetReq(pst, suId, tunelInf)
Pst *pst;
SuId suId;
EuCrEGtpTunInf *tunelInf;
#endif
{
   S16       idx = 0;
   S16      ret1 = ROK;
   Buffer   *mBuf = NULLP;
   EgtpTunInf  *tunCbInf = NULLP;

   TRC3(cmPkEuCreattLocTunSetReq);

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   for(idx = (tunelInf->numTunls - 1); idx >= 0; idx--)
   {
      tunCbInf = &tunelInf->tunLst[idx].tunInf;
      CMCHKPKLOG(cmPkLtePdcpId, &tunCbInf->pdcpId, mBuf, EUTXXX, pst);
      CMCHKPKLOG(cmPkCmTptAddr, &tunCbInf->frmDstIpAddr, mBuf, EUTXXX, pst);
      CMCHKPKLOG(cmPkCmTptAddr,&tunCbInf->dstIpAddr, mBuf, EUTXXX, pst);
      CMCHKPKLOG(SPkU32, tunCbInf->remTeid, mBuf,EUTXXX, pst);
      CMCHKPKLOG(SPkU32, tunCbInf->locTeid, mBuf,EUTXXX, pst);

      CMCHKPKLOG(SPkU32, tunelInf->tunLst[idx].tunnType, mBuf, EUTXXX, pst);
      CMCHKPKLOG(SPkU8, tunelInf->tunLst[idx].rbMode, mBuf, EUTXXX, pst);
      CMCHKPKLOG(SPkU8, tunelInf->tunLst[idx].rbDir, mBuf, EUTXXX, pst);
   }
   CMCHKPKLOG(SPkU8, tunelInf->numTunls, mBuf, EUTXXX, pst);

   pst->event = (Event) EVTVECREATEGTPTUNELSET;
   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmUnpkEuCreattLocTunSetReq
*
*    Desc:   Unpack the primitive 
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ve_eut.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkEuCreattLocTunSetReq
(
EgtpTunSetCreat func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 cmUnpkEuCreattLocTunSetReq(func, pst, mBuf,sMem)
EgtpTunSetCreat func;
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   S16             idx = 0;
   EgtpTunInf     *tunCbInf = NULLP;
   EuCrEGtpTunInf unpkStruct;

   TRC3(cmUnpkEuCreattLocTunSetReq);

   cmMemset((U8 *)&unpkStruct, 0, sizeof(EuCrEGtpTunInf));
   CMCHKUNPK(SUnpkU8,         &unpkStruct.numTunls, mBuf);
   for(idx = 0; idx < unpkStruct.numTunls; idx++)
   {
      CMCHKUNPK(SUnpkU8,          &unpkStruct.tunLst[idx].rbDir, mBuf);
      CMCHKUNPK(SUnpkU8,          &unpkStruct.tunLst[idx].rbMode, mBuf);
      CMCHKUNPK(SUnpkU32,        (U32 *) &unpkStruct.tunLst[idx].tunnType, mBuf);

      tunCbInf = &unpkStruct.tunLst[idx].tunInf;
      CMCHKUNPK(SUnpkU32,         &tunCbInf->locTeid, mBuf);
      CMCHKUNPK(SUnpkU32,         &tunCbInf->remTeid, mBuf);
      CMCHKUNPK(cmUnpkCmTptAddr,  &tunCbInf->dstIpAddr, mBuf);
      CMCHKUNPK(cmUnpkCmTptAddr,  &tunCbInf->frmDstIpAddr, mBuf);
      CMCHKUNPK(cmUnpkLtePdcpId,  &tunCbInf->pdcpId, mBuf);
   }

   SPutMsg(mBuf);
   
   RETVALUE((*func)(pst, VE_EG_SUID, &unpkStruct));
}
#endif

/********************************************************************30**

         End of file:     ve_eut.c@@/main/2 - Wed Dec  1 08:44:24 2010

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
