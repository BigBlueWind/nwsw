/********************************************************************16**

                         (c) COPYRIGHT 2011 by 
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

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_uim.c 
  
     Sid:      gk_uim.c@@/main/3 - Sat Jul 30 02:21:57 2011
  
     Prg:      sd
  
**********************************************************************/

/** @file rg_uim.c.
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files -- defines (.h) */

/* header/extern include files (.x) */
/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "crg.h"           /* CRG defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "tfu.h"
#include "rgu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"        /* customizable defines and macros for MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "rg_err.h"        /* RG error defines */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "crg.x"           /* CRG types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "tfu.x"
#include "rgu.x"
#include "rg_sch_inf.x"
#include "rg.x"            /* typedefs for MAC */
#include "log.h"

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


/**
 * @brief Handler for Bind request.
 *
 * @details
 *
 *     Function : RgUiRguBndReq
 *     
 *     This function handles the bind request from MAC Service User.
 *     
 *           
 *  @param[in]  Pst  *pst
 *  @param[in]  SuId suId
 *  @param[in]  SpId spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguBndReq
(
Pst  *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 RgUiRguBndReq(pst, suId, spId)
Pst  *pst;
SuId suId;
SpId spId;
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   /* rg003.201: Changed from TRC2 to TRC3 */
   TRC3(RgUiRguBndReq)

   RGDBGPRM((rgPBuf,"RgUiRguBndReq(): suId = %d; spId = %d;", suId, spId));

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgCb.rgInit.region;
   tmpPst.pool        = rgCb.rgInit.pool;
   tmpPst.srcProcId   = rgCb.rgInit.procId;
   tmpPst.srcEnt      = rgCb.rgInit.ent;
   tmpPst.srcInst     = rgCb.rgInit.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   if(spId == rgCb.rguSap.sapCfg.spId)
   {
      /* Check the state of the SAP */
      switch (rgCb.rguSap.sapSta.sapState)
      {
         case LRG_NOT_CFG: /* SAP Not configured */
            RGDBGINFO((rgPBuf, "SAP Not Configured\n"));
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_NOT_CFG,
                  LCM_CAUSE_INV_SAP, &dgn);
            ret = RgUiRguBndCfm(&tmpPst, suId, CM_BND_NOK);
            break;
         case LRG_UNBND: /* SAP is not bound */
            RGDBGINFO((rgPBuf,"SAP Not yet bound\n"));
            rgCb.rguSap.sapSta.sapState = LRG_BND;
            rgCb.rguSap.sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgUIMRguBndCfm(suId, CM_BND_OK);
            /* Indicate to Layer manager */ 
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_EVENT_RGUSAP_ENB,
                  LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            RGDBGINFO((rgPBuf,"SAP already bound\n"));
            ret = rgUIMRguBndCfm(suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG008, (ErrVal)rgCb.rguSap.sapSta.sapState,
                  "Invalid SAP State:RgUiRguBndReq failed\n");
#endif
            ret = rgUIMRguBndCfm(suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG009, (ErrVal)rgCb.rguSap.sapCfg.spId,
            "Invalid SAP Id:RgUiRguBndReq failed\n");
#endif
      ret = rgUIMRguBndCfm(suId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* RgUiRguBndReq */


/**
 * @brief Handler for Unbind request.
 *
 * @details
 *
 *     Function : RgUiRguUbndReq
 *     
 *     This function handles the unbind request from MAC Service User.
 *     
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SpId   spId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 RgUiRguUbndReq(pst, spId, reason)
Pst    *pst;
SpId   spId;
Reason reason;
#endif
{
   TRC3(RgUiRguUbndReq)
   
   RGDBGPRM((rgPBuf,"RgUiRguUbndReq(): spId = %d; reason = %d;", spId, reason));

   /* SAP Id validation */
   if (spId == rgCb.rguSap.sapCfg.spId)
   {
      switch(rgCb.rguSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            RGDBGINFO((rgPBuf,"SAP already bound\n"));
            /* setting SAP state to UN BOUND */
            rgCb.rguSap.sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG010, (ErrVal)rgCb.rguSap.sapSta.sapState,
                  "Invalid SAP State:RgUiRguUbndReq failed\n");
#endif
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG011, (ErrVal)rgCb.rguSap.sapCfg.spId,
            "Invalid SAP Id:RgUiRguUbndReq failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* RgUiRguUbndReq */
/**
 * @brief API for sending bind confirm from MAC to RLC
 *
 * @details
 *
 *     Function: rgUIMRguBndCfm
 *     
 *     This API is invoked to send bind confirm from MAC to RLC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     bind confirm API towards RLC.
 *           
 *  @param[in]  SuId          suId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMRguBndCfm
(
SuId suId,
U8 status
)
#else
PUBLIC S16 rgUIMRguBndCfm(suId, status)
SuId          suId;
U8            status;
#endif
{
   S16  ret = ROK;
   
   TRC2(rgUIMRguBndCfm)
   
   RGDBGPRM((rgPBuf,"rgUIMRguBndCfm(): suId = %d; status = %d;", suId, status));

   ret = RgUiRguBndCfm(&rgCb.rguSap.sapCfg.sapPst, rgCb.rguSap.sapCfg.suId, status);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"RgUiRguBndCfm Failed \n"));
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMRguBndCfm*/

#if 0
/**
 * @brief Handler for dedicated DatReq from RGU
 *
 * @details
 *
 *     Function : RgUiRguDDatReq
 *     
 *     This function validates SAP and invokes ROM for further processing
 *     
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguDDatReqInfo  *datReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguDDatReq
(
Pst             *pst,
SpId            spId,
RguDDatReqInfo  *datReq
)
#else
PUBLIC S16 RgUiRguDDatReq(pst, spId, datReq)
Pst             *pst;
SpId            spId;
RguDDatReqInfo  *datReq;
#endif
{
   S16   ret = ROK;

#ifndef NO_ERRCLS 
   U32   id;
   U32   id1;
   U32   id2;
#endif
   
   TRC3(RgUiRguDDatReq)

   RGDBGPRM((rgPBuf,"RgUiRguDDatReq(): spId = %d; datReq = %p;",spId, (void *)datReq));

   /* rg005.201 Removing basic validations at interface for data performance path */
#ifndef NO_ERRCLS
   if (datReq == NULLP)
   {
      RGDBGERR((rgPBuf,"Input Message Buffer is NULL\n"));
      RETVALUE(RFAILED);
   }
   
   if(rgCb.rguSap.sapCfg.spId == spId)
   {
      switch (rgCb.rguSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG012, (ErrVal)rgCb.rguSap.sapSta.sapState,
                  "Invalid SAP State:RgUiRguDDatReq failed\n");
#endif
            RG_DROP_RGUDDATREQ_MBUF(datReq);
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG013, (ErrVal)spId,
            "Invalid SAP Id:RgUiRguDDatReq failed\n");
#endif
      RG_DROP_RGUDDATREQ_MBUF(datReq);
      RETVALUE(RFAILED);
   }

   /* Update RGU SAP statistics for received sdu count */
   /*ccpu00118201 - ADD - Send trace only when its enabled*/
   if(rgCb.rgInit.trc)
   {
      for (id = 0; id < datReq->nmbOfTbs; id++)
      {
         for (id1 = 0; id1 < datReq->datReqTb[id].nmbLch; id1++)
         {
            /* rg004.201 patch - ccpu00110079 */
            /* rgCb.rguSap.sapSts.numPduRcvd is updated by 
             * rgROMDedDatReq -> rgUpdtRguDedSts function
             * So numPduRcvd updation is commented here */
            /* rgCb.rguSap.sapSts.numPduRcvd +=
               datReq->datReqTb[id].lchData[id1].pdu.numPdu; */
            for (id2 = 0; id2 < datReq->datReqTb[id].lchData[id1].pdu.numPdu; id2++)
            {
               RG_SEND_TRC_IND(datReq->datReqTb[id].
                     lchData[id1].pdu.mBuf[id2], EVTRGUDDATREQ);
            }
         }
      }
   }
#endif

   /* Call Ownership module for further processing */
   ret = rgROMDedDatReq(datReq);
   if (ret == RFAILED)
   {
      RGDBGERR((rgPBuf,"RgUiRguDDatReq:processing Of Dat Req Failed\n"));
      RG_DROP_RGUDDATREQ_MBUF(datReq);
   }
   SPutSBuf (pst->region, pst->pool, (Data *)datReq,
         sizeof(RguDDatReqInfo));
   RETVALUE(ret);
}  /* RgUiRguDDatReq */


/**
 * @brief Handler for common DatReq from RGU
 *
 * @details
 *
 *     Function : RgUiRguCDatReq
 *     
 *     This function validates SAP invokes ROM for further processing
 *     
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguCDatReqInfo  *datReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguCDatReq
(
Pst             *pst,
SpId            spId,
RguCDatReqInfo  *datReq
)
#else
PUBLIC S16 RgUiRguCDatReq(pst, spId, datReq)
Pst             *pst;
SpId            spId;
RguCDatReqInfo  *datReq;
#endif
{
   S16   ret = ROK;
   
   TRC3(RgUiRguCDatReq)

   RGDBGPRM((rgPBuf,"RgUiRguCDatReq(): spId = %d; datReq = %p;",spId, (void *)datReq));

   /* rg005.201  SAP validation may not be necessary for performance path */
#ifndef NO_ERRCLS
   if (datReq == NULLP)
   {
      RGDBGERR((rgPBuf,"Input Message Buffer is NULL\n"));
      RETVALUE(RFAILED);
   }
   
   if(rgCb.rguSap.sapCfg.spId == spId)
   {
      switch (rgCb.rguSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG014, (ErrVal)rgCb.rguSap.sapSta.sapState,
                  "Invalid SAP State:RgUiRguCDatReq failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG015, (ErrVal)spId,
            "Invalid SAP Id:RgUiRguCDatReq failed \n");
#endif
      RETVALUE(RFAILED);
   }
#endif

   /* Update RGU SAP statistics for received sdu count */
   /* rg004.201 patch - ccpu00110079 */
   /* rgCb.rguSap.sapSts.numPduRcvd is updated by 
    * rgROMCmnDatReq ->rgUpdtRguCmnSts function
    * So numPduRcvd updation is commented here */
   /* rgCb.rguSap.sapSts.numPduRcvd++; */

   ret = rgROMCmnDatReq(datReq);
   /*ccpu00118201 - ADD - Send trace only when its enabled*/
   if(rgCb.rgInit.trc)
   {
      RG_SEND_TRC_IND(datReq->pdu, EVTRGUCDATREQ);
   }
   if (ret == RFAILED)
   {
      RG_DROP_RGUCDATREQ_MBUF(datReq);
   }
   SPutSBuf (pst->region, pst->pool, (Data *)datReq,
         sizeof(RguCDatReqInfo));
   RETVALUE(ret);
}  /* RgUiRguCDatReq */
#endif

/**
 * @brief Handler for dedicated StaRsp from RGU
 *
 * @details
 *
 *     Function : RgUiRguDStaRsp
 *     
 *     This function validates SAP and invokes ROM for further processing
 *           
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguDStaRspInfo  *staRsp
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguDStaRsp
(
Pst             *pst,
SpId            spId,
RguDStaRspInfo  *staRsp
)
#else
PUBLIC S16 RgUiRguDStaRsp(pst, spId, staRsp)
Pst             *pst;
SpId            spId;
RguDStaRspInfo  *staRsp;
#endif
{
   S16   ret = ROK;
   volatile int t;
   U32 t2;

   TRC3(RgUiRguDStaRsp)
#ifdef MSPD 
   t = MacGetTick();	   
#endif
#if 0 /*MS_WORKAROUND for ccpu00122915 */
   RGDBGPRM((rgPBuf,"RgUiRguDStaRsp(): spId = %d; staRsp = %p;",spId, (void *)staRsp));

   /* rg005.201 Remvoing SAP & basic validation for data performance path */
#ifndef NO_ERRCLS
   if (staRsp == NULLP)
   {
      RGDBGERR((rgPBuf,"Input Response Buffer is NULL\n"));
      RETVALUE(RFAILED);
   }

   if (spId == rgCb.rguSap.sapCfg.spId)
   {
      switch (rgCb.rguSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG016, (ErrVal)rgCb.rguSap.sapSta.sapState,
                  "Invalid SAP State:RgUiRguDStaRsp failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG017, (ErrVal)rgCb.rguSap.sapCfg.spId,
            "Invalid SAP Id:RgUiRguDStaRsp failed\n");
#endif
      RETVALUE(RFAILED);
   }
#endif
#endif
#if 0 /* for l2sim */  
   ret = rgROMDedStaRsp(staRsp);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"RgUiRguDStaRsp : Processing Of Status Response Failed\n"));
   }
#endif 
   SPutSBuf (pst->region, pst->pool, (Data *)staRsp,
         sizeof(RguDStaRspInfo));
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_STA_RSP, RESOURCE_LARM, t, t2 = MacGetTick());
#endif
   RETVALUE(ret);
}  /* RgUiRguDStaRsp */

/**
 * @brief Handler for common StaRsp from RGU
 *
 * @details
 *
 *     Function : RgUiRguCStaRsp
 *     
 *     This function validates SAP and invokes ROM 
 *     for further processing
 *     
 *           
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguCStaRspInfo  *staRsp
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguCStaRsp
(
Pst             *pst,
SpId            spId,
RguCStaRspInfo  *staRsp
)
#else
PUBLIC S16 RgUiRguCStaRsp(pst, spId, staRsp)
Pst             *pst;
SpId            spId;
RguCStaRspInfo  *staRsp;
#endif
{
   S16   ret = ROK;

   TRC3(RgUiRguCStaRsp)
   
   logInfo("RgUiRguCStaRsp(): spId = %d; staRsp = %p;",spId, (void *)staRsp);

   /* rg005.201 Validation may not be required in performance path */
#ifndef NO_ERRCLS
   if (staRsp == NULLP)
   {
      RGDBGERR((rgPBuf,"Input Response Buffer is NULL\n"));
      RETVALUE(RFAILED);
   }

   if (spId == rgCb.rguSap.sapCfg.spId)
   {
      switch (rgCb.rguSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG018, (ErrVal)rgCb.rguSap.sapSta.sapState,
                  "Invalid SAP State:RgUiRguCStaRsp failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG019, (ErrVal)rgCb.rguSap.sapCfg.spId,
            "Invalid SAP Id:RgUiRguCStaRsp failed\n");
#endif
      RETVALUE(RFAILED);
   }
#endif
#if 0
   ret = rgROMCmnStaRsp(staRsp);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"RgUiRguCStaRsp : Processing Of Status Response Failed\n"));
      RETVALUE(ret);
   }
#endif
   SPutSBuf (pst->region, pst->pool, (Data *)staRsp,
         sizeof(RguCStaRspInfo));
   RETVALUE(ret);
}  /* RgUiRguCStaRsp */
#if 0
#ifdef LTE_L2_MEAS
#ifdef LTEMAC_R9
/**
 * @brief Handler for L2M MeasReq from RGU
 *
 * @details
 *
 *     Function :RgUiRguL2MUlThrpMeasReq 
 *     
 *     This function validates SAP and invokes ROM for further processing
 *           
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguL2MUlThrpMeasReqInfo  *measReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguL2MUlThrpMeasReq 
(
Pst             *pst,
SpId            spId,
RguL2MUlThrpMeasReqInfo  *measReq
)
#else
PUBLIC S16 RgUiRguL2MUlThrpMeasReq(pst, spId, measReq)
Pst             *pst;
SpId            spId;
RguL2MUlThrpMeasReqInfo  *measReq;
#endif
{
   S16   ret = ROK;

   TRC3(RgUiRguL2MUlThrpMeasReq)
   
   RGDBGPRM((rgPBuf,"RgUiRguL2MUlThrpMeasReq(): spId = %d; measReq = %p;",spId, (void *)measReq));

#ifndef NO_ERRCLS
   if (measReq == NULLP)
   {
      RGDBGERR((rgPBuf,"Input Response Buffer is NULL\n"));
      RETVALUE(RFAILED);
   }

   if (spId == rgCb.rguSap.sapCfg.spId)
   {
      switch (rgCb.rguSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG020, (ErrVal)rgCb.rguSap.sapSta.sapState,
                  "Invalid SAP State:RgUiRguL2MUlThrpMeasReq failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG021, (ErrVal)rgCb.rguSap.sapCfg.spId,
            "Invalid SAP Id:RgUiRguL2MUlThrpMeasReq failed\n");
#endif
      RETVALUE(RFAILED);
   }
#endif

   ret = rgROML2MUlThrpMeasReq(measReq);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"RgUiRguL2MUlThrpMeasReq : Processing Of Meas Request Failed\n"));
   }

   SPutSBuf (pst->region, pst->pool, (Data *)measReq,
         sizeof(RguL2MUlThrpMeasReqInfo));
   RETVALUE(ret);
}  /* RgUiRguL2MUlThrpMeasReq */
#endif
#endif

/**
 * @brief Handler for sending staInd to dedicated logical channels of a UE 
 *
 * @details
 *
 *     Function : rgUIMSndDedStaInd
 *     
 *     This function fills SAP and Pst information to send the staInd to
 *     a UE.
 *     
 *           
 *  @param[in]  RgRguDedStaInd  *staInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndDedStaInd
(
RgRguDedStaInd  *staInd
)
#else
PUBLIC S16 rgUIMSndDedStaInd(staInd)
RgRguDedStaInd  *staInd;
#endif
{
   S16  ret = ROK;
   
   TRC2(rgUIMSndDedStaInd)
   
   RGDBGPRM((rgPBuf,"rgUIMSndDedStaInd(): staInd = %p;", (void *)staInd));
   
   ret = RgUiRguDStaInd(&rgCb.rguSap.sapCfg.sapPst, rgCb.rguSap.sapCfg.suId, 
         staInd);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"rgUIMSndDedStaInd: RgUiRguDStaInd Failed\n"));
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMSndDedStaInd */


/**
 * @brief Handler for sending staInd to a common logical channel.
 *
 * @details
 *
 *     Function : rgUIMSndCmnStaInd
 *     
 *     This function fills SAP and Pst information to send the staInd to
 *     a common logical channel.
 *     
 *           
 *  @param[in]  RgRguCmnStaInd  *staInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndCmnStaInd
(
RgRguCmnStaInd  *staInd
)
#else
PUBLIC S16 rgUIMSndCmnStaInd(staInd)
RgRguCmnStaInd  *staInd;
#endif
{
   S16  ret = ROK;

   TRC2(rgUIMSndCmnStaInd)
   
   RGDBGPRM((rgPBuf,"rgUIMSndCmnStaInd(): staInd = %p", (void *)staInd));

   ret = RgUiRguCStaInd(&rgCb.rguSap.sapCfg.sapPst, rgCb.rguSap.sapCfg.suId, 
         staInd);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"rgUIMSndCmnStaInd: RgUiRguCStaInd Failed\n"));
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMSndCmnStaInd */


/**
 * @brief Handler for sending datInd to dedicated logical channels of a UE 
 *
 * @details
 *
 *     Function : rgUIMSndDedDatInd
 *     
 *     This function fills SAP and Pst information to send the datInd to
 *     a UE.
 *     
 *           
 *  @param[in]  RgRguDedDatInd  *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndDedDatInd
(
RgRguDedDatInd  *datInd
)
#else
PUBLIC S16 rgUIMSndDedDatInd(datInd)
RgRguDedDatInd  *datInd;
#endif
{
   S16  ret = ROK;

   TRC2(rgUIMSndDedDatInd)
   
   RGDBGPRM((rgPBuf,"rgUIMSndDedDatInd(): staInd = %p;", (void *)datInd));

   /* rg003.301:ccpu00122591-ADD-Updating the numPduTxmit 
    * statistics variable.
    * */
   rgCb.rguSap.sapSts.numPduTxmit += datInd->numLch;

   ret = RgUiRguDDatInd(&rgCb.rguSap.sapCfg.sapPst, rgCb.rguSap.sapCfg.suId, 
         datInd);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"rgUIMSndDedDatInd: RgUiRguDdatInd Failed\n"));
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMSndDedDatInd */


/**
 * @brief Handler for sending datInd to a common logical channel.
 *
 * @details
 *
 *     Function : rgUIMSndCmnDatInd
 *     
 *     This function fills SAP and Pst information to send the datInd to
 *     a common logical channel.
 *     
 *           
 *  @param[in]  RgRguCmnDatInd  *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndCmnDatInd
(
RgRguCmnDatInd  *datInd
)
#else
PUBLIC S16 rgUIMSndCmnDatInd(datInd)
RgRguCmnDatInd  *datInd;
#endif
{
   S16  ret = ROK;

   TRC2(rgUIMSndCmnDatInd)

   RGDBGPRM((rgPBuf,"rgUIMSndCmnDatInd(): staInd = %p;", (void *)datInd));

   /* rg003.301:ccpu00122591-ADD-Updating the numPduTxmit 
    * statistics variable.
    * */
   rgCb.rguSap.sapSts.numPduTxmit++;
   
   ret = RgUiRguCDatInd(&rgCb.rguSap.sapCfg.sapPst, rgCb.rguSap.sapCfg.suId, 
         datInd);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"rgUIMSndCmnDatInd: RgUiRguCDatInd Failed\n"));
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMSndCmnDatInd */
#endif
/**

 * @brief API for bind request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgBndReq
 *     
 *     This API is invoked by RRC towards MAC to bind CRG SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to RRC.
 *
 *           
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiCrgBndReq
(
Pst   *pst, 
SuId  suId,
SpId  spId
)
#else
PUBLIC S16 RgUiCrgBndReq(pst, suId, spId)
Pst   *pst; 
SuId  suId;
SpId  spId;
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC3(RgUiCrgBndReq)

   RGDBGPRM((rgPBuf,"RgUiCrgBndReq(): suId = %d; spId = %d;", suId, spId));

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgCb.rgInit.region;
   tmpPst.pool        = rgCb.rgInit.pool;
   tmpPst.srcProcId   = rgCb.rgInit.procId;
   tmpPst.srcEnt      = rgCb.rgInit.ent;
   tmpPst.srcInst     = rgCb.rgInit.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;


   if(spId == rgCb.crgSap.sapCfg.spId)
   {
      /* Check the state of the SAP */
      switch (rgCb.crgSap.sapSta.sapState)
      {
         case LRG_NOT_CFG: /* SAP Not configured */
            RGDBGINFO((rgPBuf, "SAP Not Configured\n"));
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_NOT_CFG,
                  LCM_CAUSE_INV_SAP, &dgn);
            ret = RgUiCrgBndCfm(&tmpPst, suId, CM_BND_NOK);
            break;
         case LRG_UNBND: /* SAP is not bound */
            RGDBGINFO((rgPBuf,"SAP Not yet bound\n"));
            rgCb.crgSap.sapSta.sapState = LRG_BND;
            rgCb.crgSap.sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgUIMCrgBndCfm(suId, CM_BND_OK);
            /* Indicate to Layer manager */
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_EVENT_CRGSAP_ENB,
                  LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            ret = rgUIMCrgBndCfm(suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG022, (ErrVal)rgCb.crgSap.sapSta.sapState,
                  "Invalid SAP State:RgUiCrgBndReq failed\n");
#endif
            ret = rgUIMCrgBndCfm(suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG023, (ErrVal)rgCb.crgSap.sapCfg.spId,
            "Invalid SAP Id:RgUiCrgBndReq failed\n");
#endif
      ret = rgUIMCrgBndCfm(suId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* RgUiCrgBndReq */


/**
 * @brief API for unbind request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgUbndReq
 *     
 *     This API is invoked by RRC towards MAC to unbind CRG SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to RRC.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiCrgUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 RgUiCrgUbndReq(pst, spId, reason)
Pst    *pst; 
SpId   spId;
Reason reason;
#endif
{
   TRC3(RgUiCrgUbndReq)

   RGDBGPRM((rgPBuf,"RgUiCrgUbndReq(): spId = %d; reason = %d;", spId, reason));

   /* SAP Id validation */
   if (spId == rgCb.crgSap.sapCfg.spId)
   {
      switch(rgCb.crgSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            /* setting SAP state to UN BOUND */
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            rgCb.crgSap.sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG024, (ErrVal)rgCb.crgSap.sapSta.sapState,
                  "Invalid SAP State:RgUiCrgUbndReq failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG025, (ErrVal)rgCb.crgSap.sapCfg.spId,
            "Invalid SAP Id:RgUiCrgUbndReq failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* RgUiCrgUbndReq */

/**
 * @brief API for sending bind confirm from MAC to RRC
 *
 * @details
 *
 *     Function: rgUIMRgrBndCfm
 *     
 *     This API is invoked to send bind confirm from MAC to RRC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     bind confirm API towards RRC.
 *           
 *  @param[in]  SuId          suId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMCrgBndCfm
(
SuId suId,
U8 status
)
#else
PUBLIC S16 rgUIMCrgBndCfm(suId, status)
SuId          suId;
U8            status;
#endif
{
   TRC2(rgUIMCrgBndCfm)
   
   RGDBGPRM((rgPBuf,"rgUIMCrgBndCfm(): suId = %d; status = %d;", suId, status));

   if(RgUiCrgBndCfm(&rgCb.crgSap.sapCfg.sapPst, rgCb.crgSap.sapCfg.suId, status) != ROK)
   {
      RGDBGERR((rgPBuf,"RgUiCrgBndCfm Failed \n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgUIMCrgBndCfm*/
#if 0
/**
 * @brief API for configuration request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgCfgReq
 *     
 *     This API is invoked by RRC towards MAC to configure MAC. 
 *     These API validates the Pst, spId, suId and transfers the config request 
 *     specific information to corresponding ownership module (COM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  CrgCfgTransId transId
 *  @param[in]  CrgCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiCrgCfgReq
(
Pst           *pst, 
SpId          spId,
CrgCfgTransId transId,
CrgCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 RgUiCrgCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
CrgCfgTransId transId;
CrgCfgReqInfo *cfgReqInfo;
#endif
{
   S16       ret       = ROK;
   U8        cfmStatus = 0x00ff;
   U8        prntTrans[CRG_CFG_TRANSID_SIZE+1];

   TRC3(RgUiCrgCfgReq);

   /* rg002.101:ccpu00101624: Ensuring transId is always Null terminated. */
   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, CRG_CFG_TRANSID_SIZE);
   prntTrans[CRG_CFG_TRANSID_SIZE] = '\0';

   RGDBGPRM((rgPBuf,"RgUiCrgCfgReq(): spId = %d; transId = %s;", spId, prntTrans));

   /* CrgCfgReqInfo Validation for NULLP */
   if (cfgReqInfo == NULLP)
   {
      RGDBGERR((rgPBuf,"Input Param crgReqInfo is NULL \n"));
      rgUIMCrgCfgCfm(transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /* Upper SAP Id and State validation */
   if (spId == rgCb.crgSap.sapCfg.spId)
   {
      switch(rgCb.crgSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound */
            RGDBGINFO((rgPBuf,"SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG026, (ErrVal)rgCb.crgSap.sapSta.sapState,
                  "Invalid SAP State:RgUiCrgCfgReq failed\n");
#endif
         SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
               sizeof(CrgCfgReqInfo));

            rgUIMCrgCfgCfm(transId, cfmStatus);
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG027, (ErrVal)rgCb.crgSap.sapCfg.spId,
            "Invalid SAP Id:RgUiCrgCfgReq failed\n");
#endif
      SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
            sizeof(CrgCfgReqInfo));
      rgUIMCrgCfgCfm(transId, cfmStatus); 
      RETVALUE(RFAILED);
   }
   /*rg008.201 - Removed commented code*/
   ret = rgCOMCfgReq(transId, cfgReqInfo);

   SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
         sizeof(CrgCfgReqInfo));

   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"RgUiCrgCfgReq:Configuration Request Handling Failed \n"));
      RETVALUE(RFAILED);
   }

   RGDBGINFO((rgPBuf,"RgUiCrgCfgReq:Configuration Request Processed\n"));
   RETVALUE(ROK);
}  /* RgUiCrgCfgReq */
#endif
/**
 * @brief API for sending configuration confirm from MAC to RRC
 *
 * @details
 *
 *     Function: rgUIMCrgCfgCfm
 *     
 *     This API is invoked to send configuration confirm from MAC to RRC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     config confirm API towards RRC.
 *           
 *  @param[in]  CrgCfgTransId transId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMCrgCfgCfm
(
CrgCfgTransId transId,
U8            status
)
#else
PUBLIC S16 rgUIMCrgCfgCfm(transId, status)
CrgCfgTransId transId;
U8            status;
#endif
{
   S16  ret = ROK;
   U8   prntTrans[CRG_CFG_TRANSID_SIZE+1];

   TRC2(rgUIMCrgCfgCfm)
   
   /* rg002.101:ccpu00101624: Ensuring transId is always Null terminated. */
   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, CRG_CFG_TRANSID_SIZE);
   prntTrans[CRG_CFG_TRANSID_SIZE] = '\0';

   RGDBGPRM((rgPBuf,"rgUIMCrgCfgCfm: transId = %s; status = %d\n", prntTrans, status));

   ret = RgUiCrgCfgCfm(&rgCb.crgSap.sapCfg.sapPst, rgCb.crgSap.sapCfg.suId, transId, status);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf,"rgUIMCrgCfgCfm: RgUiCrgCfgCfm Failed \n"));
      RETVALUE(ret);
   }

   RETVALUE(ret);
}  /* rgUIMCrgCfgCfm */
#if 0
#ifdef LTE_L2_MEAS
/* TODO: Function Header */
#ifdef ANSI
PUBLIC S16 rgUIMSndharqStaInd
(
CmLteCellId     cellId,
RgInfUeHqInfo   *ueHqInfo,
RgDlHqProcCb    *hqP,
RgErrInfo       *err
)
#else
PUBLIC S16 rgUIMSndharqStaInd(cellId, ueHqInfo, hqP, err)
CmLteCellId     cellId;
RgInfUeHqInfo   *ueHqInfo;
RgDlHqProcCb    *hqP;
RgErrInfo       *err;
#endif
{
   RguHarqStatusInd *hqStaInd;
   U32              idx;
   U32              ret = ROK;

   TRC3(rgUIMSndharqStaInd)
  
   if ( (hqP->rguSnMapInfo[0] != NULLP) || (hqP->rguSnMapInfo[1] != NULLP ))
   {
      if ((ret = rgAllocSBuf ((Data**)&hqStaInd, sizeof(RguHarqStatusInd))) != ROK)
      {
         err->errType  = RGERR_DHM_SND_HARQ_STA_IND;
         err->errCause = RG_DHM_MEM_ALLOC_FAIL;
         RETVALUE(RFAILED);
      }
      /* Fill CellId and UeId */
      hqStaInd->cellId = cellId;
      hqStaInd->ueId = ueHqInfo->rnti;
      
      /* Fill Mapping Info and Status for both TB's from DL HARQ proc */
      for (idx = 0; idx < hqP->numOfTBs; idx++)
      {
         hqStaInd->rguSnMapInfo[idx] = hqP->rguSnMapInfo[idx];
         hqStaInd->status[idx] = ueHqInfo->status[idx];
      }
      hqStaInd->numTbs = hqP->numOfTBs;
      if ((ret = RgUiRguHqStaInd(&rgCb.rguSap.sapCfg.sapPst, rgCb.rguSap.sapCfg.suId,
            hqStaInd)) != ROK)
      {
         err->errType  = RGERR_DHM_SND_HARQ_STA_IND;
         err->errCause = RG_DHM_MEM_ALLOC_FAIL; /* TODO: Check Reason Here */
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
} /* rgUIMSndharqStaInd */
#endif /* LTE_L2_MEAS */
#endif
/**********************************************************************
 
         End of file:     gk_uim.c@@/main/3 - Sat Jul 30 02:21:57 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat      init                  description
------------ --------   ----  ----------------------------------------------
/main/1      ---        sd           1. Initial Release.
/main/2      ---        sd           1. LTE MAC 2.1 release
         rg003.201      ns           1. Trace added for functions.
         rg004.201      ms           1. Statistics update of the DL SDUs/PDUs
         rg005.201      sm           1. moved some validations under errcls for
                                        optimization.
         rg008.201   rsharon         1. Removed commented code.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
