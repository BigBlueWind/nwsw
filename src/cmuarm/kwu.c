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
  
        Name:    KWU Interface
    
        Type:    C file
  
        Desc:    This file Contains the packing and unpacking functions for
                 KWU Interface
 
        File:    kwu.c

        Sid:      kwu.c@@/main/4 - Fri Sep  2 18:27:18 2011
  
        Prg:     chakrapani
  
*********************************************************************21*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "kwu.h"           /* KWU defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "kwu.x"           /* KWU */

#ifdef LCKWU

/****************************************************************************
 *                      Packing Functions
 ***************************************************************************/
#ifdef ANSI
PUBLIC S16 cmPkKwuBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 cmPkKwuBndReq(pst, suId, spId)
Pst * pst;
SuId suId;
SpId spId;
#endif
{
   S16    ret1;
   Buffer *mBuf;

   TRC3(cmPkKwuBndReq)

   mBuf = NULLP;

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU001, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }
   CMCHKPKLOG(SPkS16, spId, mBuf, EKWU002, pst);
   CMCHKPKLOG(SPkS16, suId, mBuf, EKWU003, pst);
   pst->event = (Event) KWU_EVT_BND_REQ;
   RETVALUE(SPstTsk(pst,mBuf));
}


#ifdef ANSI
PUBLIC S16 cmPkKwuBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 cmPkKwuBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{
   S16    ret1;
   Buffer *mBuf;

   TRC3(cmPkKwuBndCfm)

   mBuf = NULLP;

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU004, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   CMCHKPKLOG(SPkU8, status, mBuf, EKWU005, pst);
   CMCHKPKLOG(SPkS16, suId, mBuf, EKWU006, pst);
   pst->event = (Event) KWU_EVT_BND_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}


#ifdef ANSI
PUBLIC S16 cmPkKwuUbndReq
(
Pst * pst,
SuId suId,
Reason reason
)
#else
PUBLIC S16 cmPkKwuUbndReq(pst, suId, reason)
Pst * pst;
SuId suId;
Reason reason;
#endif
{
   S16 ret1;
   Buffer *mBuf;

   TRC3(cmPkKwuUbndReq)

   mBuf = NULLP;

   if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU007, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }
   CMCHKPKLOG(SPkS16, reason, mBuf, EKWU008, pst);
   CMCHKPKLOG(SPkS16, suId, mBuf, EKWU009, pst);

   pst->event = (Event) KWU_EVT_UBND_REQ;
   RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuUbndReq */

#ifdef ANSI
PUBLIC S16 cmPkKwuDatReqInfo
(
KwuDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkKwuDatReqInfo(param, mBuf)
KwuDatReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkKwuDatReqInfo);

#ifdef CCPU_OPT
   switch(param->lcType) {
      case CM_LTE_LCH_CCCH:
         CMCHKPK(cmPkLteRnti, param->tm.rnti, mBuf);
         break;
      case CM_LTE_LCH_BCCH:
      case  CM_LTE_LCH_PCCH:
         CMCHKPK(cmPkLteTimingInfo, &param->tm.tmg, mBuf);
         break;
      case CM_LTE_LCH_DTCH:
      case CM_LTE_LCH_DCCH:
         break;
      default :
         RETVALUE(RFAILED);
   }
   CMCHKPK(cmPkLteLcType, param->lcType, mBuf);

#endif
   CMCHKPK(SPkU32, param->sduId, mBuf);
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   RETVALUE(ROK);
} /* cmPkKwuDatReqInfo */


#ifdef ANSI
PUBLIC S16 cmPkKwuDatReq
(
Pst * pst,
SpId spId,
KwuDatReqInfo* datReq,
Buffer * mBuf
)
#else
PUBLIC S16 cmPkKwuDatReq(pst, spId, datReq, mBuf)
Pst * pst;
SpId spId;
KwuDatReqInfo* datReq;
Buffer * mBuf;
#endif
{
#ifdef LCKWU
#ifndef	NO_ERRCLS
    S16 ret1;
#endif    
#endif /* LCKWU */

    TRC3(cmPkKwuDatReq)

    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
          {
#ifdef	NO_ERRCLS
             cmPkKwuDatReqInfo( (datReq), mBuf);
#endif	     
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkKwuDatReqInfo( (datReq), mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU010, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)datReq,
                      sizeof(KwuDatReqInfo)) != ROK)
             {
                SPutMsg(mBuf);
                RETVALUE(RFAILED);
             }

             break;
          }
#endif /* LCKWU */
    }

    CMCHKPKLOG(SPkS16, spId, mBuf, EKWU011, pst);
    pst->event = (Event) KWU_EVT_DAT_REQ;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuDatReq */


#ifdef ANSI
PUBLIC S16 cmPkKwuDatIndInfo
(
KwuDatIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkKwuDatIndInfo(param, mBuf)
KwuDatIndInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkKwuDatIndInfo);

#ifdef CCPU_OPT
   CMCHKPK(cmPkLteRnti, param->tCrnti, mBuf);
#endif
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkKwuDatInd
(
Pst * pst,
SuId suId,
KwuDatIndInfo* datInd,
Buffer * mBuf
)
#else
PUBLIC S16 cmPkKwuDatInd(pst, suId, datInd, mBuf)
Pst * pst;
SuId suId;
KwuDatIndInfo* datInd;
Buffer * mBuf;
#endif
{
#ifdef LCKWU
#ifndef	NO_ERRCLS
    S16 ret1;
#endif    
#endif /* LCKWU */

    TRC3(cmPkKwuDatInd)

    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
          {
#ifdef	NO_ERRCLS
             cmPkKwuDatIndInfo( (datInd), mBuf);
#endif	     
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkKwuDatIndInfo( (datInd), mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU012, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)datInd,
                      sizeof(KwuDatIndInfo)) != ROK)
             {
                SPutMsg(mBuf);
                RETVALUE(RFAILED);
             }

             break;
          }
#endif /* LCKWU */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EKWU013, pst);
    pst->event = (Event) KWU_EVT_DAT_IND;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuDatInd */


#ifdef ANSI
PUBLIC S16 cmPkKwuDatCfmInfo
(
KwuDatCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkKwuDatCfmInfo(param, mBuf)
KwuDatCfmInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkKwuDatCfmInfo);

   CMCHKPK(SPkU32, param->sduId, mBuf);
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   RETVALUE(ROK);
} /* cmPkKwuDatCfmInfo */


#ifdef ANSI
PUBLIC S16 cmPkKwuDatCfm
(
Pst * pst,
SuId suId,
KwuDatCfmInfo* datCfm
)
#else
PUBLIC S16 cmPkKwuDatCfm(pst, suId, datCfm)
Pst * pst;
SuId suId;
KwuDatCfmInfo* datCfm;
#endif
{
#ifdef LCKWU
#ifndef	NO_ERRCLS
   S16 ret1;
#endif   
#endif /* LCKWU */
   Buffer *mBuf;

   TRC3(cmPkKwuDatCfm)

   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
          {
#ifdef	NO_ERRCLS
             cmPkKwuDatCfmInfo( (datCfm), mBuf);
#endif	     
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkKwuDatCfmInfo( (datCfm), mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU014, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)datCfm,
                      sizeof(KwuDatCfmInfo)) != ROK)
             {
                SPutMsg(mBuf);
                RETVALUE(RFAILED);
             }

             break;
          }
#endif /* LCKWU */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EKWU015, pst);
    pst->event = (Event) KWU_EVT_DAT_CFM;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuDatCfm */


#ifdef ANSI
PUBLIC S16 cmPkKwuDiscSduReq
(
Pst * pst,
SpId spId,
KwuDiscSduInfo* discSdu
)
#else
PUBLIC S16 cmPkKwuDiscSduReq(pst, spId, discSdu)
Pst * pst;
SpId spId;
KwuDiscSduInfo* discSdu;
#endif
{
#ifdef LCKWU
#ifndef	NO_ERRCLS
   S16 ret1;
#endif   
#endif /* LCKWU */
   Buffer *mBuf;

   TRC3(cmPkKwuDiscSduReq)

   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
          {
#ifdef	NO_ERRCLS
             cmPkKwuDiscSduInfo( (discSdu), mBuf);
#endif	     
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkKwuDiscSduInfo( (discSdu), mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU016, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)discSdu,
                      sizeof(KwuDiscSduInfo)) != ROK)
             {
                SPutMsg(mBuf);
                RETVALUE(RFAILED);
             }

             break;
          }
#endif /* LCKWU */
    }

    CMCHKPKLOG(SPkS16, spId, mBuf, EKWU017, pst);
    pst->event = (Event) KWU_EVT_DISC_SDU_REQ;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuDiscSduReq */


#ifdef ANSI
PUBLIC S16 cmPkKwuStaIndInfo
(
KwuStaIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkKwuStaIndInfo(param, mBuf)
KwuStaIndInfo *param;
Buffer *mBuf;
#endif
{
   S16 i;

   TRC3(cmPkKwuStaIndInfo);

   for (i = (param->numSdu - 1); i >= 0; i--)
   {
      CMCHKPK(SPkU32, param->sduId[(U16)i], mBuf);
   }
   CMCHKPK(SPkU16, param->numSdu, mBuf);
   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkKwuStaInd
(
Pst * pst,
SuId suId,
KwuStaIndInfo* staInd
)
#else
PUBLIC S16 cmPkKwuStaInd(pst, suId, staInd)
Pst * pst;
SuId suId;
KwuStaIndInfo* staInd;
#endif
{
#ifdef LCKWU
#ifndef	NO_ERRCLS
   S16 ret1;
#endif   
#endif /* LCKWU */
   Buffer *mBuf;

   TRC3(cmPkKwuStaInd)

   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
          {
#ifdef	NO_ERRCLS
             cmPkKwuStaIndInfo( (staInd), mBuf);
#endif	     
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkKwuStaIndInfo( (staInd), mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU018, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)staInd,
                      sizeof(KwuStaIndInfo)) != ROK)
             {
                SPutMsg(mBuf);
                RETVALUE(RFAILED);
             }

             break;
          }
#endif /* LCKWU */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EKWU019, pst);
    pst->event = (Event) KWU_EVT_STA_IND;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuStaInd */


#ifdef ANSI
PUBLIC S16 cmPkKwuReEstCmpInd
(
Pst * pst,
SuId suId,
CmLteRlcId rlcId
)
#else
PUBLIC S16 cmPkKwuReEstCmpInd(pst, suId, rlcId)
Pst * pst;
SuId suId;
CmLteRlcId rlcId;
#endif
{
#ifdef LCKWU
#ifndef	NO_ERRCLS
   S16 ret1;
#endif   
#endif /* LCKWU */
   Buffer *mBuf;

   TRC3(cmPkKwuReEstCmpInd)

   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
          {
#ifdef	NO_ERRCLS
             cmPkLteRlcId( &rlcId, mBuf);
#endif	     
#if (ERRCLASS & ERRCLS_ADD_RES)
             ret1 = cmPkLteRlcId( &rlcId, mBuf);
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EKWU018, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             break;
          }
#endif /* LCKWU */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EKWU019, pst);
    pst->event = (Event) KWU_EVT_REEST_CMP_IND;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuReEstCmpInd */

/* kwu_c_001.main_3 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 cmPkKwuDiscSduCfm
(
Pst            *pst,
SpId           spId,
KwuDiscSduInfo *discCfmSdu
)
#else
PUBLIC S16 cmPkKwuDiscSduCfm(pst, spId, discCfmSdu)
Pst            *pst;
SpId           spId;
KwuDiscSduInfo *discCfmSdu;
#endif
{
   S16 ret1;
   Buffer *mBuf;

   TRC3(cmPkKwuDiscSduCfm)

   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   ret1 = cmPkKwuDiscSduInfo((discCfmSdu), mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRKWU, (ErrVal)ret1, "Packing failure");
      RETVALUE( ret1 );
   }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
   if (SPutSBuf(pst->region, pst->pool, (Data *)discCfmSdu,
            sizeof(KwuDiscSduInfo)) != ROK)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkS16, spId, mBuf, ERRKWU, pst);
   pst->event = (Event) KWU_EVT_DISC_SDU_CFM;

   RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuDiscSduCfm */

#ifdef ANSI
PUBLIC S16 cmPkKwuDatAckInfo
(
KwuDatAckInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkKwuDatAckInfo(param, mBuf)
KwuDatAckInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkKwuDatAckIndInfo);

   CMCHKPK(cmPkLteRlcId, &param->rlcId, mBuf);
   CMCHKPK(SPkU32, param->sduId, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmPkKwuDatAckInd
(
Pst * pst,
SuId suId,
KwuDatAckInfo* datInd
)
#else
PUBLIC S16 cmPkKwuDatAckInd(pst, suId, datInd)
Pst * pst;
SuId suId;
KwuDatAckInfo* datInd;
#endif
{
    S16 ret1;

   Buffer *mBuf;

   TRC3(cmPkKwuDatAckInd);

   mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   ret1 = cmPkKwuDatAckInfo( (datInd), mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRKWU, (ErrVal)ret1, "Packing failure");
      RETVALUE( ret1 );
   }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
   if (SPutSBuf(pst->region, pst->pool, (Data *)datInd,
            sizeof(KwuDatAckInfo)) != ROK)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   CMCHKPKLOG(SPkS16, suId, mBuf, ERRKWU, pst);
   pst->event = (Event) KWU_EVT_DAT_ACK_IND;

   RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkKwuDatAckInd */
#endif /* LTE_L2_MEAS */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuBndReq
(
KwuBndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuBndReq(func, pst, mBuf)
KwuBndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId = 0;
   SpId spId = 0;

   TRC3(cmUnpkKwuBndReq)

   CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EKWU020, pst);
   CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EKWU021, pst);
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, suId, spId));
}


#ifdef ANSI
PUBLIC S16 cmUnpkKwuBndCfm
(
KwuBndCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuBndCfm(func, pst, mBuf)
KwuBndCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId = 0;
   U8 status = 0;

   TRC3(cmUnpkKwuBndCfm)

   CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EKWU022, pst);
   CMCHKUNPKLOG(SUnpkU8, &status, mBuf, EKWU023, pst);
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, suId, status));
}


#ifdef ANSI
PUBLIC S16 cmUnpkKwuUbndReq
(
KwuUbndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuUbndReq(func, pst, mBuf)
KwuUbndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId = 0;
   Reason reason = 0;
   
   TRC3(cmUnpkKwuUbndReq)

   CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EKWU024, pst);
   CMCHKUNPKLOG(SUnpkS16, &reason, mBuf, EKWU025, pst);
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, spId, reason));
} /* cmUnpkKwuUbndReq */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatReqInfo
(
KwuDatReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatReqInfo(param, mBuf)
KwuDatReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkKwuDatReqInfo);

   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->sduId, mBuf);

#ifdef CCPU_OPT
   CMCHKUNPK(cmUnpkLteLcType, &param->lcType, mBuf);
   switch(param->lcType) {
      case CM_LTE_LCH_BCCH:
      case  CM_LTE_LCH_PCCH:
         CMCHKUNPK(cmUnpkLteTimingInfo, &param->tm.tmg, mBuf);
         break;
      case CM_LTE_LCH_CCCH:
         CMCHKUNPK(cmUnpkLteRnti, &param->tm.rnti, mBuf);
         break;
      case CM_LTE_LCH_DTCH:
      case CM_LTE_LCH_DCCH:
         break;
      default :
         RETVALUE(RFAILED);
   }
#endif
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatReq
(
KwuDatReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatReq(func, pst, mBuf)
KwuDatReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    SpId          spId = 0;
    KwuDatReqInfo *datReq = NULLP;
    
    TRC3(cmUnpkKwuDatReq)

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datReq,\
                sizeof(KwuDatReqInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU026, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

    cmMemset((U8 *)datReq, 0, sizeof(KwuDatReqInfo));

    CMCHKUNPK(SUnpkS16, &(spId), mBuf);
    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
       {
          ret1 = cmUnpkKwuDatReqInfo( (datReq), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EKWU027, (ErrVal)ret1, "Unpacking failure");
             RETVALUE( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
          break;
       }
#endif /* LCKWU */
    }

   RETVALUE((*func)(pst, spId, datReq, mBuf));
} /* cmUnpkKwuDatReq */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatIndInfo
(
KwuDatIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatIndInfo(param, mBuf)
KwuDatIndInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkKwuDatIndInfo);

   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);

#ifdef CCPU_OPT
   CMCHKUNPK(cmUnpkLteRnti, &param->tCrnti, mBuf);
#endif
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatInd
(
KwuDatInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatInd(func, pst, mBuf)
KwuDatInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    SuId          suId = 0;
    KwuDatIndInfo *datInd = NULLP;
    
    TRC3(cmUnpkKwuDatInd)

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datInd,\
                sizeof(KwuDatIndInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU028, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

    cmMemset((U8 *)datInd, 0, sizeof(KwuDatIndInfo));

    CMCHKUNPK(SUnpkS16, &(suId), mBuf);
    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
       {
          ret1 = cmUnpkKwuDatIndInfo( (datInd), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EKWU029, (ErrVal)ret1, "Unpacking failure");
             RETVALUE( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
          break;
       }
#endif /* LCKWU */
    }

   RETVALUE((*func)(pst, suId, datInd, mBuf));
} /* cmUnpkKwuDatInd */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatCfmInfo
(
KwuDatCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatCfmInfo(param, mBuf)
KwuDatCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkKwuDatCfmInfo);

   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->sduId, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatCfm
(
KwuDatCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatCfm(func, pst, mBuf)
KwuDatCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    SuId          suId = 0;
   KwuDatCfmInfo *datCfm = NULLP;
    
   TRC3(cmUnpkKwuDatCfm)

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datCfm,\
                sizeof(KwuDatCfmInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EKWU030, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

    cmMemset((U8 *)datCfm, 0, sizeof(KwuDatCfmInfo));

    CMCHKUNPK(SUnpkS16, &(suId), mBuf);
    switch(pst->selector)
    {
#ifdef LCKWU
       case KWU_SEL_LC:
       {
          ret1 = cmUnpkKwuDatCfmInfo( (datCfm), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EKWU031, (ErrVal)ret1, "Unpacking failure");
             RETVALUE( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
          break;
       }
#endif /* LCKWU */
    }
    SPutMsg(mBuf);

   RETVALUE((*func)(pst, suId, datCfm));
} /* cmUnpkKwuDatCfm */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuDiscSduReq
(
KwuDiscSduReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDiscSduReq(func, pst, mBuf)
KwuDiscSduReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16 ret1;
   SpId          spId = 0;
   KwuDiscSduInfo *discSdu = NULLP;

   TRC3(cmUnpkKwuDiscSduReq)

      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&discSdu,\
                  sizeof(KwuDiscSduInfo))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         if(ret1 != ROK)
         {
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)EKWU032, (ErrVal)0, "SGetMsg() failed");
         }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

   cmMemset((U8 *)discSdu, 0, sizeof(KwuDiscSduInfo));

   CMCHKUNPK(SUnpkS16, &(spId), mBuf);
   switch(pst->selector)
   {
#ifdef LCKWU
      case KWU_SEL_LC:
         {
            ret1 = cmUnpkKwuDiscSduInfo( (discSdu), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)EKWU033, (ErrVal)ret1, "Unpacking failure");
               RETVALUE( ret1 );
            }
#endif /* ERRCLASS & ERRCLS_DEBUG */
            break;
         }
#endif /* LCKWU */
   }
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, spId, discSdu));
} /* cmUnpkKwuDiscSduReq */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuStaIndInfo
(
KwuStaIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuStaIndInfo(param, mBuf)
KwuStaIndInfo *param;
Buffer *mBuf;
#endif
{
   U32 i;

   TRC3(cmUnpkKwuStaIndInfo);

   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);
   CMCHKUNPK(SUnpkU16, &param->numSdu, mBuf);

   for (i = 0; i < param->numSdu; i++)
   {
      CMCHKUNPK(SUnpkU32, &param->sduId[i], mBuf);
   }

   RETVALUE(ROK);
} /* cmUnpkKwuStaIndInfo */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuStaInd
(
KwuStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuStaInd(func, pst, mBuf)
KwuStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16 ret1;
   SuId          suId = 0;
   KwuStaIndInfo *staInd = NULLP;

   TRC3(cmUnpkKwuStaInd)

      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staInd,\
                  sizeof(KwuStaIndInfo))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         if(ret1 != ROK)
         {
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)EKWU034, (ErrVal)0, "SGetMsg() failed");
         }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

   cmMemset((U8 *)staInd, 0, sizeof(KwuStaIndInfo));

   CMCHKUNPK(SUnpkS16, &(suId), mBuf);
   switch(pst->selector)
   {
#ifdef LCKWU
      case KWU_SEL_LC:
         {
            ret1 = cmUnpkKwuStaIndInfo( (staInd), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)EKWU035, (ErrVal)ret1, "Unpacking failure");
               RETVALUE( ret1 );
            }
#endif /* ERRCLASS & ERRCLS_DEBUG */
            break;
         }
#endif /* LCKWU */
   }
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, suId, staInd));
} /* cmUnpkKwuStaInd */


#ifdef ANSI
PUBLIC S16 cmUnpkKwuReEstCmpInd
(
KwuReEstCmpInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuReEstCmpInd(func, pst, mBuf)
KwuReEstCmpInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
#ifndef	NO_ERRCLS
   S16 ret1;
#endif   
   SuId          suId = 0;
   CmLteRlcId    rlcId;

   TRC3(cmUnpkKwuReEstCmpInd)

   cmMemset((U8 *)&rlcId, 0, sizeof(CmLteRlcId));

   CMCHKUNPK(SUnpkS16, &(suId), mBuf);
   switch(pst->selector)
   {
#ifdef LCKWU
      case KWU_SEL_LC:
         {
#ifdef	NO_ERRCLS
            cmUnpkLteRlcId( &rlcId, mBuf);
#endif	    
#if(ERRCLASS & ERRCLS_DEBUG)
            ret1 = cmUnpkLteRlcId( &rlcId, mBuf);
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)EKWU035, (ErrVal)ret1, "Unpacking failure");
               RETVALUE( ret1 );
            }
#endif /* ERRCLASS & ERRCLS_DEBUG */
            break;
         }
#endif /* LCKWU */
   }
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, suId, rlcId));
} /* cmUnpkKwuReEstCmpInd */

/* kwu_c_001.main_3 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 cmUnpkKwuDiscSduCfm
(
KwuDiscSduCfm  func,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDiscSduCfm(func, pst, mBuf)
KwuDiscSduCfm  func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 ret1;
   SpId          spId = 0;
   KwuDiscSduInfo *discSdu = NULLP;

   TRC3(cmUnpkKwuDiscSduCfm);

   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&discSdu,\
               sizeof(KwuDiscSduInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRKWU, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }
   cmMemset((U8 *)discSdu, 0, sizeof(KwuDiscSduInfo));

   CMCHKUNPK(SUnpkS16, &(spId), mBuf);

   ret1 = cmUnpkKwuDiscSduInfo( (discSdu), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
      SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ERRKWU, (ErrVal)ret1, "Unpacking failure");
      RETVALUE( ret1 );
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, spId, discSdu));
} /* cmUnpkKwuDiscSduCfm */

#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatAckInfo
(
KwuDatAckInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatAckInfo(param, mBuf)
KwuDatAckInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkKwuDatAckInfo);

   CMCHKUNPK(SUnpkU32, &param->sduId, mBuf);
   CMCHKUNPK(cmUnpkLteRlcId, &param->rlcId, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkKwuDatAckInd
(
KwuDatAckInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwuDatAckInd(func, pst, mBuf)
KwuDatAckInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
    SuId          suId = 0;
    KwuDatAckInfo *datInd = NULLP;
    
    TRC3(cmUnpkKwuDatAckInd);

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datInd,\
                sizeof(KwuDatAckInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ERRKWU, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

    cmMemset((U8 *)datInd, 0, sizeof(KwuDatAckInfo));

    CMCHKUNPK(SUnpkS16, &(suId), mBuf);
    ret1 = cmUnpkKwuDatAckInfo( (datInd), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
             (ErrVal)ERRKWU, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /* ERRCLASS & ERRCLS_DEBUG */

    RETVALUE((*func)(pst, suId, datInd));
} /* cmUnpkKwuDatAckInd */
#endif /* LTE_L2_MEAS */
#endif /* LCKWU */

/********************************************************************30**

         End of file:     kwu.c@@/main/4 - Fri Sep  2 18:27:18 2011

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
/main/1      ---       cp  1. initial release.
/main/3      ---      gk   1. LTERLC Release 2.1.
/main/4     kwu_c_001.main_3 ap 1. Added support for L2 Measurement.
*********************************************************************91*/

