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
  
     File:     rg_utl.c
  
     Sid:      gk_utl.c@@/main/3 - Sat Jul 30 02:21:57 2011
  
     Prg:      st
  
**********************************************************************/

/** @file rg_utl.c
@brief This file implements utility functions for LTE MAC
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE */

#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* Timer */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"           /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg.x"            /* MAC includes */

/* local defines */
#define RG_NON_MIMO_IDX 0

/* local typedefs */
 
/* local externs */
 
/* forward references */


/***********************************************************
 *
 *     Func : rgAllocSBuf
 *
 *     Desc : Utility Function to Allocate static buffer. 
 *            Memory allocated is assumed contiguous.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgAllocSBuf
(
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
#else
PUBLIC S16 rgAllocSBuf(pData, size)
Data    **pData;            /* Pointer of the data to be returned */
Size    size;               /* size */
#endif
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC2(rgAllocSBuf)

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      RETVALUE(RFAILED);
   }

   /* allocate buffer */
   if (SGetSBuf(rgCb.rgInit.region, rgCb.rgInit.pool, pData, size) != ROK)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb.rgInit.region;
     dgn.u.mem.pool = rgCb.rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(ERRCLS_DEBUG, ERG028, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }

#ifndef ALIGN_64BIT
   RGDBGINFO((rgPBuf, "SGetSBuf(Region (%d), Pool (%d), Size (%ld)), Data (0x%p))\n",
             rgCb.rgInit.region, rgCb.rgInit.pool, size, *pData));
#else
   RGDBGINFO((rgPBuf, "SGetSBuf(Region (%d), Pool (%d), Size (%d)), Data (0x%p))\n",
             rgCb.rgInit.region, rgCb.rgInit.pool, size, *pData));
#endif

   /* zero out the allocated memory */
   cmMemset((U8 *)*pData, 0x00, size);

   RETVALUE(ROK);

} /* end of rgAllocSBuf */


/*
*
*       Fun:   rgFreeSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   RETVOID
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgFreeSBuf
(
Data **data,         /* address of pointer to data */
Size size            /* size */
)
#else
PUBLIC Void rgFreeSBuf(data, size)
Data **data;         /* address of pointer to data */
Size size;          /* size */
#endif
{

   S16 ret;

   TRC2(rgFreeSBuf)

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      RETVOID;
   }

#ifndef ALIGN_64BIT
   RGDBGINFO((rgPBuf, "SPutSBuf(Region (%d), Pool (%d), Size (%ld), Data (0x%lx))\n",
              rgCb.rgInit.region, rgCb.rgInit.pool, size, (PTR) *data));
#else
   RGDBGINFO((rgPBuf, "SPutSBuf(Region (%d), Pool (%d), Size (%d), Data (0x%lx))\n",
              rgCb.rgInit.region, rgCb.rgInit.pool, size, (PTR) *data));
#endif

   /* Deallocate buffer */
   ret = SPutSBuf(rgCb.rgInit.region, rgCb.rgInit.pool, *data, size);

   if (ret != ROK)
   {
      RGLOGERROR(ERRCLS_DEBUG, ERG029, (ErrVal) 0, "rgFreeSBuf failed.\n");
      RETVOID;
   }

   *data = NULLP;

   RETVOID;

} /* end of rgFreeSBuf */


/***********************************************************
 *
 *     Func : rgGetMsg
 *
 *     Desc : Utility Function to Allocate message buffer. 
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgGetMsg
(
Buffer    **mBuf            /* Message Buffer pointer be returned */
)
#else
PUBLIC S16 rgGetMsg(mBuf)
Buffer    **mBuf;           /* Message Buffer pointer be returned */
#endif
{
   S16         ret;

   TRC2(rgGetMsg)

   ret = SGetMsg(RG_GET_MEM_REGION(rgCb), RG_GET_MEM_POOL(rgCb), mBuf);

   if (ROK != ret) 
   {
      /* rg005.201 Moving diagnostics structure to limited scope for optimization */
      RgUstaDgn   dgn;      /* Alarm diagnostics structure */

      rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM);

      /*  Send an alarm to Layer Manager */
      rgLMMStaInd(LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(ERRCLS_DEBUG, ERG030, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of rgGetMsg */


/***********************************************************
 *
 *     Func : rgFillDgnParams
 *
 *     Desc : Utility Function to Fill Diagonostic params. 
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgFillDgnParams
(
RgUstaDgn   *dgn,
U8          dgnType
)
#else
PUBLIC Void rgFillDgnParams(dgn, dgnType)
RgUstaDgn   *dgn;
U8          dgnType;
#endif
{

   TRC2(rgFillDgnParams)

   switch(dgnType)
   {
      case LRG_USTA_DGNVAL_MEM:
         /* rg004.201 : Memory Fix */
         dgn->type = (U8) LRG_USTA_DGNVAL_MEM;
         dgn->u.mem.region  = rgCb.rgInit.region;
         dgn->u.mem.pool    = rgCb.rgInit.pool;
      break;

      default:
      break;
   }

   RETVOID;
} /* end of rgFillDgnParams */


/***********************************************************
 *
 *     Func : rgUpdtRguDedSts
 *
 *     Desc : Utility Function to update rgu sap statistics for dedicated
 *            DatReqs.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtRguDedSts
(
U8             stsType,   /* Statistics type to update */
RgRguDedDatReq *datReq    /* DatReq pointer */
)
#else
PUBLIC Void rgUpdtRguDedSts(stsType, datReq)
U8             stsType;   /* Statistics type to update */
RgRguDedDatReq *datReq;   /* DatReq pointer */
#endif
{
   U8 idx1,idx2;

   TRC2(rgUpdtRguDedSts)

   switch(stsType)
   {
      case RG_RGU_SDU_RCVD:
         for (idx1 = 0; idx1 < datReq->nmbOfTbs; idx1++) 
         {
            for(idx2 = 0; idx2 < datReq->datReqTb[idx1].nmbLch; idx2++) 
            {
               rgCb.rguSap.sapSts.numPduRcvd +=
               datReq->datReqTb[idx1].lchData[idx2].pdu.numPdu;
            }
         }

         break;
      case RG_RGU_SDU_DROP:
         for (idx1 = 0; idx1 < datReq->nmbOfTbs; idx1++) 
         {
            for(idx2 = 0; idx2 < datReq->datReqTb[idx1].nmbLch; idx2++) 
            {
               rgCb.rguSap.sapSts.numPduRcvd +=
               datReq->datReqTb[idx1].lchData[idx2].pdu.numPdu;
               rgCb.rguSap.sapSts.numPduDrop +=
               datReq->datReqTb[idx1].lchData[idx2].pdu.numPdu;
            }
         }

         break;
   }
   
   RETVOID;
} /* rgUpdtRguDedSts */


/***********************************************************
 *
 *     Func : rgUpdtRguCmnSts
 *
 *     Desc : Utility Function to update rgu sap statistics for common
 *            DatReqs.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtRguCmnSts
(
U8             stsType   /* Statistics type to update */
)
#else
PUBLIC Void rgUpdtRguCmnSts(stsType)
U8             stsType;   /* Statistics type to update */
#endif
{
   TRC2(rgUpdtRguCmnSts)

   switch(stsType)
   {
      case RG_RGU_SDU_RCVD:
         rgCb.rguSap.sapSts.numPduRcvd ++;
         break;
      case RG_RGU_SDU_DROP:
         rgCb.rguSap.sapSts.numPduRcvd ++;
         rgCb.rguSap.sapSts.numPduDrop ++;
         break;
   }
   
   RETVOID;
} /* rgUpdtRguCmnSts */


/***********************************************************
 *
 *     Func : rgUpdtCellCnt
 *
 *     Desc : Utility Function to update cell count. It gives number of active
 *     cells
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called only after cell is added/deleted 
 *     from the globlal hashlist
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtCellCnt
(
U8 updtType
)
#else
PUBLIC Void rgUpdtCellCnt(updtType)
U8 updtType;
#endif
{
   TRC2(rgUpdtCellCnt);

   switch (updtType)
   {
      case RG_CFG_ADD:
         rgCb.genSts.numCellCfg++;
         break;
      case RG_CFG_DEL:
         rgCb.genSts.numCellCfg--;
         break;
      default:
         break;
   }

   RETVOID;
} /* rgUpdtCellCnt */


/***********************************************************
 *
 *     Func : rgUpdtUeCnt
 *
 *     Desc : Utility Function to update ue count. It gives number of active
 *     Ues.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called only after ue is added/deleted 
 *     from the globlal hashlist
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtUeCnt
(
U8 updtType
)
#else
PUBLIC Void rgUpdtUeCnt (updtType)
U8 updtType;
#endif
{
   TRC2(rgUpdtUeCnt);

   switch (updtType)
   {
      case RG_CFG_ADD:
         rgCb.genSts.numUeCfg++;
         break;
      case RG_CFG_DEL:
         rgCb.genSts.numUeCfg--;
         break;
      default:
         break;
   }
   RETVOID;
} /* rgUpdtUeCnt */

/*
*
*       Fun:   rgAllocEventMem
*
*       Desc:  This function allocates event memory 
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgAllocEventMem
(
Ptr       *memPtr,
Size      memSize
)
#else
PUBLIC S16 rgAllocEventMem(memPtr, memSize)
Ptr       *memPtr;
Size      memSize;
#endif
{
   Mem   sMem;
#ifdef MSPD_MLOG_NEW
   VOLATILE U32 t=0, t2=0;
#endif

   TRC2(rgAllocEventMem)

   sMem.region = rgCb.rgInit.region;
   sMem.pool = rgCb.rgInit.pool;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (memSize<= 0)
   {
      RGLOGERROR(ERRCLS_INT_PAR, ERG031, memSize,
                   "rgAllocEventMem(): memSize invalid\n");
      RETVALUE (RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */


#ifdef MSPD_MLOG_NEW
   t=GetTIMETICK();
#endif

   if(ROK != cmAllocEvnt(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
   {
      RGDBGERR((rgPBuf,"rgAllocEventMem(): cmAllocEvnt Failed.\n")) 
      MSPD_ERR("rgAllocEventMem(): cmAllocEvnt Failed.\n"); 
      RETVALUE(RFAILED);
   }
   

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MACUTL_CMALLCEVT, RESOURCE_LARM, t, t2=GetTIMETICK());
#endif

   RETVALUE(ROK);
} /* end of rgAllocEventMem*/

/*
*
*       Fun:   rgGetEventMem
*
*       Desc:  This function allocates event memory 
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgGetEventMem
(
Ptr       *ptr,
Size      len,
Ptr       memCp
)
#else
PUBLIC S16 rgGetEventMem(ptr, len, memCp)
Ptr       *ptr;
Size      len;
Ptr       memCp;
#endif
{
   S16   ret;

   TRC2(rgGetEventMem)
   ret = cmGetMem(memCp, len, (Ptr *)ptr);
   RETVALUE(ret);
} /* end of rgGetEventMem*/

/***********************************************************
 *
 *     Func : rgGetPstToInst
 *
 *     Desc : Utility Function to get the pst structure to post a message to
 *     scheduler instance
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called while sending a msg from 
 *     MAC to a scheduler instance
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgGetPstToInst
(
Pst           *pst,
Inst          schInst
)
#else
PUBLIC Void rgGetPstToInst (pst, schInst)
Pst           *pst;
Inst          schInst;
#endif
{
   /* rg003.201: Added function ttrace */
   TRC2(rgGetPstToInst);

   pst->srcEnt = rgCb.rgInit.ent; 
   pst->srcInst = rgCb.rgInit.inst;
   pst->srcProcId = rgCb.rgInit.procId;
   pst->region = rgCb.rgInit.region;
   pst->pool = rgCb.rgInit.pool;

   pst->dstProcId = rgCb.rgInit.procId;
   pst->dstEnt = rgCb.rgInit.ent; 
   pst->dstInst = schInst;
   pst->selector = 0;
   pst->prior     = PRIOR0;
   pst->intfVer   = 0;
   pst->route   = RTESPEC;

   RETVOID; 
} /* end of rgGetPstToInst */
/*rg008.201 - Added support for SPS*/

#ifdef LTEMAC_SPS

/***********************************************************
 *
 *     Func : RgSchMacSpsLcRegReq
 *
 *     Desc : Utility Function to register the set of uplink SPS logical
 *        channels for a SPS UE.
 *        Invoked at the time of activation of a UE for UL-SPS.
 *        Whenever there is data on these LCs MAC shall inform scheduler
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given in lcInfo            
 *           - Store the sps-rnti and set the bits corresponding to the
 *             logical channel ids in ueUlCb->spsLcMask.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time UL SPS is activated 
 *     for a UE at scheduler
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcRegReq
(
Pst            *pst,
RgInfSpsLcInfo *lcInfo
)
#else
PUBLIC S16 RgSchMacSpsLcRegReq (pst, lcInfo)
Pst            *pst;
RgInfSpsLcInfo *lcInfo;
#endif
{
   RgCellCb   *cell;
   RgUeCb     *ue;
   U8         idx;

   TRC2(RgSchMacSpsLcRegReq);

   /* Fetch the cell and then the UE */
   if ((cell = rgDBMGetCellCb(lcInfo->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Cell does not exist %d\n", lcInfo->cellId));
      MSPD_ERR("Cell does not exist %d\n", lcInfo->cellId);
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, lcInfo->crnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Ue does not exist %d\n", lcInfo->crnti));
      MSPD_ERR("Ue does not exist %d\n", lcInfo->crnti);
      RETVALUE(RFAILED);
   }

   /* Store the sps-rnti and SPS LC information in the UE */ 
   ue->spsRnti = lcInfo->spsRnti;
   for (idx=0; idx < lcInfo->spsLcCnt; idx++)
   {
      /* KWORK_FIX: Modified the index from lcId to lcId-1 for handling lcId 10 properly */
      ue->ul.spsLcId[(lcInfo->spsLcId[idx])-1] = TRUE;
   }
   ue->ul.implRelCnt = lcInfo->implRelCnt + 1;
   ue->ul.explRelCnt = ue->ul.implRelCnt + 1; /*(lcInfo->implRelCnt * lcInfo->spsPrd);*/

   /* Insert the UE into SPS UE List */
   if (rgDBMInsSpsUeCb(cell, ue) == RFAILED)
   {
      RGDBGERR((rgPBuf, "Ue insertion into SPS list failed %d\n", ue->spsRnti));
      MSPD_ERR("Ue insertion into SPS list failed %d\n", ue->spsRnti);
      RETVALUE(RFAILED);
   } 

   RETVALUE(ROK); 
} /* end of RgSchMacSpsLcRegReq */


/***********************************************************
 *
 *     Func : RgSchMacSpsLcDeregReq
 *
 *     Desc : Utility Function to deregister the set of uplink SPS 
 *        logical channels for a UE.
 *        Invoked at the time of release of UL-SPS for an activated UE.
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given
 *           - Reset the bits corresponding to the logical channel ids in
 *             ueUlCb->spsLcMask.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time UL SPS is released 
 *     for a UE at scheduler
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcDeregReq
(
Pst            *pst,
CmLteCellId    cellId,
CmLteRnti      crnti
)
#else
PUBLIC S16 RgSchMacSpsLcDeregReq (pst, cellId, crnti)
Pst            *pst;
CmLteCellId    cellId;
CmLteRnti      crnti;
#endif
{
   RgCellCb    *cell;
   RgUeCb      *ue;

   TRC2(RgSchMacSpsLcDeregReq);

   /* Fetch the cell and then the UE */
   if ((cell = rgDBMGetCellCb(cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Cell does not exist %d\n", cellId));
      MSPD_ERR("Cell does not exist %d\n", cellId);
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, crnti)) == NULLP)
   {
      RGDBGERR((rgPBuf, "Ue does not exist %d\n", crnti));
      MSPD_ERR("Ue does not exist %d\n", crnti);
      RETVALUE(RFAILED);
   }

   /* No need to reset the SPS LC Ids as they will not be looked at*/

   /* Delete UE from the SPS UE List */
   rgDBMDelSpsUeCb(cell, ue);
   
   RETVALUE(ROK); 
} /* end of RgSchMacSpsLcDeregReq */

#endif /* LTEMAC_SPS */


/**********************************************************************
 
         End of file:     gk_utl.c@@/main/3 - Sat Jul 30 02:21:57 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     st  1. Initial Release.
/main/2      ---     st  1. LTE MAC 2.1 release
          rg003.201  ns  1. Trace added for functions.
          rg004.201  ns  1. Memory related Fix
          rg005.201  sm  1. Changes for optimization.
          rg008.201 rsharon  2. Changes for optimization.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
