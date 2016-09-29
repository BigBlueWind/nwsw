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
  
     File:     rg_dhm.c
  
     Sid:      gk_dhm.c@@/main/3 - Sat Jul 30 02:21:26 2011
  
     Prg:      ps
  
**********************************************************************/

/** @file rg_dhm.c
@brief APIs related to Downlink HARQ.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "rg_err.h"
#include "rg.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_hash.x"       /* common hash list */
#include "cm_llist.x"      /* common linked list library */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common LTE */
#include "lrg.x"
#include "crg.x"
#include "rgu.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg.x"

/* local typedefs */
 
/* local externs */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
PRIVATE Void rgDHMBldTfuDatReq ARGS((RgDlSf *dlSf, RgDlHqProcCb *hqP,
                           RgTfuDatReqPduInfo *datReq));


/* forward references */

/**
 * @brief This function initializes the DL HARQ Entity of UE
 *
 * @details
 *
 *     Function: rgDHMHqEntInit
 *     Purpose:  This function initializes the DL HARQ entity of 
 *               UE control block. This is performed at the time
 *               of creating UE control block.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgCellCb*  cell
 *  @param[in]  RgUeCb*    ue
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 rgDHMHqEntInit
(
RgDlHqEnt          *hqE,
U8                 maxHqProcs
)
#else
PUBLIC S16 rgDHMHqEntInit(hqE, maxHqProcs)
RgDlHqEnt          *hqE;
U8                 maxHqProcs;
#endif
{
   U8 idx1,idx2;

   TRC2(rgDHMHqEntInit)

   /* Init the HARQ data structure */
   if (rgAllocSBuf((Data **)&(hqE->procs),
            sizeof(RgDlHqProcCb)*maxHqProcs) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   hqE->numHqProcs = maxHqProcs;
   /* for each harq process */
   for (idx1 = 0; idx1 < hqE->numHqProcs; idx1++)
   {
      hqE->procs[idx1].procId      = idx1;
      for(idx2 = 0; idx2 < RG_MAX_TB_PER_UE; idx2++)
      {
         hqE->procs[idx1].tbInfo[idx2].tb = NULLP;
      }
   }

   RGDBGINFO((rgPBuf, "Harq Entity Initialized\n"));

   RETVALUE(ROK);
} /* rgDHMHqEntInit */

/* rg007.201 - Changes for MIMO feature addition */
/**
 * @brief This function releases a HARQ process
 *
 * @details
 *
 *     Function: rgDHMUeReset
 *     Purpose:  This function resets TB in each HarqProc.
 *     
 *     Invoked by: CFG UE Reset 
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC Void rgDHMUeReset
(
RgDlHqEnt          *hqE
)
#else
PUBLIC Void rgDHMUeReset(hqE)
RgDlHqEnt          *hqE;
#endif
{
   U8       i = 0;

   TRC2(rgDHMUeReset)

   if(hqE->procs)
   {
      /* Free all the TB memory associated with HARQ */
      for (i=0; i < hqE->numHqProcs; i++)
      {
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependncy on MIMO compile-time flag */
         rgDHMRlsHqProcTB(&hqE->procs[i], 1);
         rgDHMRlsHqProcTB(&hqE->procs[i], 2);
      }
   }
   RETVOID;
} /* rgDHMUeReset*/


/**
 * @brief This function releases a HARQ process
 *
 * @details
 *
 *     Function: rgDHMRlsHqProcTB
 *     Purpose:  This function returns a HARQ process to HARQ Entity 
 *               in the DL direction.
 *     
 *               1. Add the HARQ process to the free queue.
 *     Invoked by: scheduler and HARQ processing
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC S16 rgDHMRlsHqProcTB
(
RgDlHqProcCb         *hqP,
U8                   tbIndex
)
#else
PUBLIC S16 rgDHMRlsHqProcTB(hqP, tbIndex)
RgDlHqProcCb         *hqP;
U8                   tbIndex;
#endif
{
   TRC2(rgDHMRlsHqProcTB)

   if((tbIndex > RG_MAX_TB_PER_UE) ||
      (tbIndex == 0))
   {
      RETVALUE(RFAILED);
   }

   hqP->tbInfo[tbIndex-1].numSchLch = 0;
   RG_FREE_MSG(hqP->tbInfo[tbIndex-1].tb);
   hqP->tbInfo[tbIndex-1].schdTa.pres = FALSE;
   /* Decrementing might lead to roundoff error in case of say UE reset
    * where all the HqProcs irrespective whether in use are called for rls.
    * Hence to avoid the same shift operator is being used. */
   hqP->numOfTBs = hqP->numOfTBs >> 1;
   if (hqP->tbInfo[tbIndex-1].sfLnk.node != NULLP)
   {
      cmLListDelFrm(&hqP->tbInfo[tbIndex-1].sf->tbs,
                    &hqP->tbInfo[tbIndex-1].sfLnk);
      hqP->tbInfo[tbIndex-1].sfLnk.node  = (PTR)NULLP;
   }
   /*rg002.301 ccpu00120298 ADD sf is reset to NULLP*/
   hqP->tbInfo[tbIndex-1].sf = NULLP;
   RGDBGINFO((rgPBuf, "Harq Proc (%d) released.\n", hqP->procId));

   RETVALUE(ROK);
} /* rgDHMRlsHqProc */

/**
 * @brief This function gets HARQ process with the given Id
 *
 * @details
 *
 *     Function: rgDHMGetHqProcFrmId
 *     Purpose:  This function returns the HARQ process with the given ID.
 *     Invoked by: ROM
 *     
 *  @param[in]  RgUeCb        *ue
 *  @param[in]  U8            idx
 *  @param[in]  RgDlHqProc    **hqP
 *  @return  S16       
 *         -#   ROK     if successful
 *         -#   RFAILED otherwise
 *
 **/
#ifdef ANSI
PUBLIC S16 rgDHMGetHqProcFrmId
(
RgUeCb               *ue,
U8                   idx,
RgDlHqProcCb         **hqP
)
#else
PUBLIC S16 rgDHMGetHqProcFrmId(ue, idx, hqP)
RgUeCb               *ue;
U8                   idx;
RgDlHqProcCb         **hqP;
#endif
{
   TRC2(rgDHMGetHqProcFrmId)

   RGDBGINFO((rgPBuf, "Fetching the harq proc for index(%d)\n", idx));
   /* Pick the proc based on the index provided */
   *hqP = &(ue->dl.hqEnt.procs[idx]);

   RETVALUE(ROK);
} /* rgDHMGetHqProcFrmId */

/*PRIVATE U32 dataAvl; */
/**
 * @brief Handler for sending data to PHY
 *
 * @details
 *
 *     Function : rgDHMSndDatReq
 *     
 *     This function shall send the MAC PDU built for the UE to TOM
 *     when invoked as part of TTI processing and keep track of the number of
 *     transmissions for this TB.
 *     
 *           
 *  @param[in]  RgCellCb      *cell
 *  @param[in]  RgDlHqProcCb  *hqE 
 *  @param[out] RgErrInfo     *err 
 *  @return     S16
 *      -#ROK 
 *      -#RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMSndDatReq
(
RgDlSf        *dlSf,
RgTfuDatReqInfo *datInfo,
RgDlHqProcCb   *hqP,
RgErrInfo      *err 
)
#else
PUBLIC S16 rgDHMSndDatReq(dlSf, datInfo, hqP, err)
RgDlSf        *dlSf;
RgTfuDatReqInfo  *datInfo;
RgDlHqProcCb   *hqP;
RgErrInfo      *err;
#endif
{
   U8 i;
   RgTfuDatReqPduInfo   *datReq;
   RgBldPduInfo      bldPdu;
   /*Added this variable to figure out that whether this UE data
     has to be inclueded in the TFU Data request.*/
   Bool  dataAvlblUe;

   TRC2(rgDHMSndDatReq)
  
      dataAvlblUe = TRUE;
      for(i=0;i< RG_MAX_TB_PER_UE;i++)
      {
         if (hqP->tbInfo[i].sf == dlSf)
         {
            /* Check if data from RLC had been received and got muxed. */
            if (hqP->tbInfo[i].tb == NULLP) 
            {
               if (hqP->tbInfo[i].schdTa.pres == TRUE ||
                     hqP->tbInfo[i].contResCe == PRSNT_NODEF)
               {
                  /* Data not received but ta needs to be sent. */
                  /* MUX TA and send it */
                  bldPdu.datReq    =  NULLP;
                  bldPdu.reqType   =  EVTTFUTTIIND;
                  /* rg007.201 - Changes for MIMO feature addition */
                  /* rg008.201 - Removed dependncy on MIMO compile-time flag */
                  bldPdu.schdTbSz  =  hqP->tbInfo[i].tbSz;
                  bldPdu.ta        =  hqP->tbInfo[i].schdTa;
                 /*rg009.201 - changes for CR timer implementation*/
                  bldPdu.contResId =  hqP->tbInfo[i].contResId;
                  /* MSPD_DBG("LTEMAC SchdTbSz %d tbIdx %d\n", bldPdu.schdTbSz,i); */               
                  if (ROK != rgMUXBldPdu(&bldPdu, &(hqP->tbInfo[i].tb), err))  
                  {
                     RETVALUE(RFAILED);
                  }
                  /* rg008.201.ccpu00114417.lpr- Removed break statment */
               }
               else   /* rg003.201: ccpu00108099 start */
               {
#ifdef LTEMAC_RGU_PAD
                  /* Data not received from RLC. Padding at MAC */
                  bldPdu.datReq    =  NULLP;
                  bldPdu.reqType   =  EVTTFUTTIIND;
                  /* rg008.201 - Removed dependncy on MIMO compile-time flag */
                  bldPdu.schdTbSz  =  hqP->tbInfo[i].tbSz;
                  bldPdu.ta        =  hqP->tbInfo[i].schdTa;
                  bldPdu.ta.val    =  0;
                  bldPdu.contResId =  NULLP;
				 

                  MSPD_DBG("CRFIX: Padding PDU Getting added SchdTbSz %d tbIdx %d UE %d hqPId %d\n", 
				        bldPdu.schdTbSz,i,hqP->tbInfo[i].pdcch.rnti,hqP->procId);
                  if (ROK != rgMUXBldPdu(&bldPdu, &(hqP->tbInfo[i].tb), err))  
                  {
                     RETVALUE(RFAILED);
                  }
                  /* rg008.201.ccpu00114417.lpr- Removed break statment */
#else
                  /*Padding is not done so data for this UE will not be
                  included.*/
                  dataAvlblUe = FALSE;
#endif
               }
            }
	    else
	    {
           /*MSPD_DBG("<HARQ> MAC HQ TB already has the data(%d)\n", ++dataAvl);*/		    
	    }
         }
		 else
		 {
            MSPD_DBG("CRFIX: dlSf %x sf %x TbIdx %d \n",dlSf,hqP->tbInfo[i].sf,i);		    
		 }
      }

   /*If Data/Padding is not available for UE, then we can not include
     any Data for this UE in TFU Data Request.*/
   if(!dataAvlblUe)
   {
      /*Free up the HARQ process for this allocation.*/
#ifdef MSPD
      MSPD_DBG("dataAvlblUe False\n");
#endif
      /* Release First TB, as this would be anyway there*/
      rgDHMRlsHqProcTB(hqP, 1);
      if(2 == hqP->numOfTBs)
      {
         rgDHMRlsHqProcTB(hqP, 2);
      }
      
      RETVALUE(ROK);
   }

   if (rgGetEventMem((Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
            &(datInfo->memCp)) != ROK)
   {
      RETVALUE(RFAILED);
   }
   /* Fill the TFU Dat Req with information from Harq Proc */
  
   rgDHMBldTfuDatReq(dlSf, hqP, datReq);

   /* MS_WORKAROUND for ccpu00122894 */
   for(i=0;i< RG_MAX_TB_PER_UE;i++)
   {
      if (hqP->tbInfo[i].sf == dlSf)
      {
         cmLListDelFrm(&dlSf->tbs, &hqP->tbInfo[i].sfLnk);
         hqP->tbInfo[i].sfLnk.node = NULLP;
      }
      hqP->tbInfo[i].sf = NULLP;
   }
   cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
   datReq->lnk.node = (PTR)datReq;

   RETVALUE(ROK);
}  /* rgDHMSndDatReq */

/**
 * @brief Function to handle RGU datReq received from ROM
 *
 * @details
 *
 *     Function : rgDHMHndlDedDatReq
 *     
 *     This function shall act on the datReq received on RGU. It shall 
 *     store the data IDs for all the logical channels and get the MAC 
 *     PDU built.
 *     
 *           
 *  @param[in]  RgDlHqProcCb   *hqProc 
 *  @param[in]  RgRguDedDatReq *datReq
 *  @param[out] RgErrInfo      *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMHndlDedDatReq
(
RgDlHqProcCb   *hqProc,
RgRguDedDatReq *datReq,
RgErrInfo      *err
)
#else
PUBLIC S16 rgDHMHndlDedDatReq(hqProc, datReq, err)
RgDlHqProcCb   *hqProc;
RgRguDedDatReq *datReq;
RgErrInfo      *err;
#endif
{
   U8             i;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   U8             j;
   RgBldPduInfo   bldPdu;
   U8             tbIndex;

   TRC2(rgDHMHndlDedDatReq);

   tbIndex = (U8)(datReq->transId & 0x03);
   /* Accept all the data requests even if delayed in case nothing
    * has been sent earlier on the harq proc.
    */
   if((datReq->nmbOfTbs > RG_MAX_TB_PER_UE) ||
         (tbIndex == 0))
   {
      /* rg007.201 - Changes for MIMO feature addition */   
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      /* release corresponding TBs from SF tbs List */
      for(j=0;j<datReq->nmbOfTbs;j++)
      {
         if (!(tbIndex & (j+1)))
         {
            j++;
         } 
         rgDHMRlsHqProcTB(hqProc, (U8)(j+1));
      }
#ifdef MSPD
      MSPD_DBG("Failed\n");
#endif
      RETVALUE(RFAILED);
   }
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */

   for(i=0;i<datReq->nmbOfTbs;i++)
   {
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      /* tbIndex 01 corresponds to presence of 1st TB
       * 10 corresponds 2nd TB
       * 11 corresponds two TBs of UE */
      /* rg001.301 - MOD - [ccpu00118831] Changes for the TB indexing issue */
      if (!(tbIndex & (i+1)))
      {
          continue ;
      }
      if (hqProc->tbInfo[i].sfLnk.node == NULLP)
      {
         /* release corresponding TBs from SF tbs List */
         for(j=0;j<datReq->nmbOfTbs;j++)
         {
            if (!(tbIndex & (j+1)))
            {
               j++;
            }
            rgDHMRlsHqProcTB(hqProc, (U8)(j+1));
         }
#ifdef MSPD
	 MSPD_DBG("Failed\n");
#endif
         RETVALUE(RFAILED);

      }
      RG_FREE_MSG(hqProc->tbInfo[i].tb);
      bldPdu.datReq    =  datReq;
      bldPdu.reqType   =  EVTRGUDDATREQ;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      bldPdu.schdTbSz  =  hqProc->tbInfo[i].tbSz;
      bldPdu.tbIndex   =  i+1;
      bldPdu.ta        =  hqProc->tbInfo[i].schdTa;
      bldPdu.contResId =  NULLP;
#ifdef LTE_L2_MEAS
      if (datReq->datReqTb[i].rguSnInfo != NULLP)
      { /* Store Mapping Info if exists from RLC in DDatRequest */
         hqProc->rguSnMapInfo[i] = datReq->datReqTb[i].rguSnInfo;
         hqProc->status[i] =  FALSE;
      }
#endif /* LTE_L2_MEAS */
      /* MSPD_DBG("LTEMAC SchdTbSz %d tbIdx %d\n", bldPdu.schdTbSz,i);*/                
      if(rgMUXBldPdu(&bldPdu, &(hqProc->tbInfo[i].tb), err) != ROK)
      {
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependncy on MIMO compile-time flag */
         /* release corresponding TBs from SF tbs List */
         for(j=0;j<datReq->nmbOfTbs;j++)
         {
            if (!(tbIndex & (j+1)))
            {
               j++;
            }
            rgDHMRlsHqProcTB(hqProc, (U8)(j+1));
         }
#ifdef MSPD
         MSPD_DBG("Failed\n");
#endif
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}  /* rgDHMHndlDedDatReq */

/**
 * @brief Function to handle RGU datReq received from ROM
 *
 * @details
 *
 *     Function : rgDHMHndlCmnDatReq
 *     
 *     This function shall act on the datReq received on RGU. It shall 
 *     store the data IDs for all the logical channels and get the MAC 
 *     PDU built.
 *     
 *           
 *  @param[in]  RgDlHqProcCb   *hqProc 
 *  @param[in]  RgRguCmnDatReq *datReq
 *  @param[out] RgErrInfo      *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMHndlCmnDatReq
(
RgDlHqProcCb   *hqProc,
RgRguCmnDatReq *datReq,
RgErrInfo      *err
)
#else
PUBLIC S16 rgDHMHndlCmnDatReq(hqProc, datReq, err)
RgDlHqProcCb   *hqProc;
RgRguCmnDatReq *datReq;
RgErrInfo      *err;
#endif
{
   RgUstaDgn      dgn;
   RgBldPduInfo   bldPdu;

   TRC2(rgDHMHndlCmnDatReq)

      if (hqProc->tbInfo[0].tb != NULLP)
      {
         /* datReq discarded. Generate an alarm */
         rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_HARQ); 
         rgLMMStaInd(LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
               LRG_CAUSE_HQ_PROC_BUSY, &dgn);
#ifdef PWAV
			   MSPD_DBG(" TB not NULL for which Data Request received\n");
#endif
         RETVALUE(RFAILED);
      }

   bldPdu.datReq    =  datReq;
   bldPdu.reqType   =  EVTRGUCDATREQ;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   bldPdu.schdTbSz  =  hqProc->tbInfo[0].tbSz;
   bldPdu.ta        =  hqProc->tbInfo[0].schdTa;
   /*rg009.201 - changes for CR timer implementation*/
   bldPdu.contResId  =  hqProc->tbInfo[0].contResId;

   /* MSPD_DBG("LTEMAC SchdTbSz %d tbIdx 0\n", bldPdu.schdTbSz); */               
   if(rgMUXBldPdu(&bldPdu, &(hqProc->tbInfo[0].tb), err) != ROK)
   {
      RG_FREE_MSG(datReq->pdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgDHMHndlCmnDatReq */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/**
 * @brief Function to get the scheduled logical channel information
 *
 * @details
 *
 *     Function : rgDHMSndStaInd
 *     
 *     This function shall be invoked by Scheduler to trigger DHM to send status
 *     indication with the scheduled information.
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgInfUeAlloc   *allocInfo,
 *  @param[in]  CmLteTimingInfo timingInfo,
 *  @param[in]  RgDlHqProcCb   *hqP
 *  @param[out] RgErrInfo      err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMSndStaInd
(
RgCellCb       *cell,
RgUeCb         *ue,
RgInfUeAlloc   *allocInfo,
CmLteTimingInfo timingInfo,
RgDlHqProcCb   *hqP,
RgErrInfo      *err
)
#else
PUBLIC S16 rgDHMSndStaInd(cell, ue, allocInfo, timingInfo, hqP, err)
RgCellCb       *cell;
RgUeCb         *ue;
RgInfUeAlloc   *allocInfo;
CmLteTimingInfo timingInfo;
RgDlHqProcCb   *hqP;
RgErrInfo      *err;
#endif
{
   RgRguDedStaInd  *dStaInd = NULLP;
   RgRguCmnStaInd  *cStaInd;
   S16             ret;
   U8              idx;
   U8              lcIdx;
   U8              tbIndex=0,idx1;
   RgDlSf          *dlSf = &cell->subFrms[timingInfo.subframe];

   int lchBufSize =0;

   TRC2(rgDHMSndStaInd)

 
   for(idx=allocInfo->tbStrtIdx;((idx-allocInfo->tbStrtIdx) <\
                                  allocInfo->nmbOfTBs); idx++)
   {
#ifdef PWAV
   if((allocInfo->tbInfo[idx].schdDat[0].lcId == RG_CCCH_LCID) || (hqP == NULLP))
   {
      MSPD_DBG("RNTI:%d, idx:%d, Num TBs:%d \n",allocInfo->rnti, idx, allocInfo->nmbOfTBs);
	  if(hqP == NULLP)
	  {
	    MSPD_DBG("Harq Proc == NULLP!!!!\n");
	  }
   }
#endif
#ifdef TFU_UPGRADE
       /* update pA value */
      hqP->tbInfo[idx].pa = allocInfo->pa;
#endif
      /*rg008.201 - Changes as part of performance testing*/
      /* rg001.301 - MOD- [ccpu00119023] Moving common code outside */
      hqP->numOfTBs =  allocInfo->nmbOfTBs;
      hqP->tbInfo[idx].sfLnk.node = (PTR)hqP;
      hqP->tbInfo[idx].sf = dlSf;
      cmLListAdd2Tail(&dlSf->tbs,&hqP->tbInfo[idx].sfLnk);
		
      /*rg008.201 - Changes as part of performance testing*/
      /*   hqP->numOfTBs++;*/
      hqP->tbInfo[idx].doa = allocInfo->doa;
      hqP->tbInfo[idx].txMode = allocInfo->txMode;
      hqP->tbInfo[idx].puschRptUsd = allocInfo->puschRptUsd;
      hqP->tbInfo[idx].puschPmiInfo = allocInfo->puschPmiInfo;
#ifdef LTEMAC_SPS
      hqP->tbInfo[idx].pdcch.rnti = allocInfo->pdcchRnti;
#else
      hqP->tbInfo[idx].pdcch.rnti = allocInfo->rnti;
#endif
      /* rg001.301 MOD -[ccpu00119023]- moved the retransmission check outside  
       * If reTx, update dci (as rv among other params would change)
       * */
      if(allocInfo->tbInfo[idx].isReTx == TRUE)
      {
         MSPD_DBG("<CRASH> RETX for UE(%d) hqP(%d)\n", allocInfo->rnti, hqP->procId);	      
         hqP->tbInfo[idx].pdcch.dci = allocInfo->dciInfo;
         continue;
      }
      
      if (allocInfo->tbInfo[idx].schdDat[0].lcId == RG_CCCH_LCID)        
      {
         /* rg001.201: Moved Processing before calling interface function */
         hqP->tbInfo[idx].pdcch.dci = allocInfo->dciInfo;
         RG_FREE_MSG(hqP->tbInfo[idx].tb);
         hqP->tbInfo[idx].timingInfo = timingInfo;
         hqP->tbInfo[idx].tbSz = allocInfo->tbInfo[idx].schdTbSz; 

         hqP->tbInfo[idx].schdTa.pres = allocInfo->tbInfo[idx].ta.pres;
         hqP->tbInfo[idx].schdTa.val  = allocInfo->tbInfo[idx].ta.val;
         /*rg009.201 - changes for CR timer implementation*/
         hqP->tbInfo[0].contResCe  = allocInfo->tbInfo[0].contResCe;
         if(allocInfo->tbInfo[0].contResCe)
         {
            hqP->tbInfo[0].contResId = &ue->contResId;
         }


         if(allocInfo->tbInfo[idx].numSchLch == 0)
         {
            MSPD_DBG("<CRASH> MSG4 with only contResId for UE(%d) hqP(%d)\n", allocInfo->rnti, hqP->procId);	      
            hqP->tbInfo[idx].numSchLch = 0;
            continue;
         }

         /* rg008.201.ccpu00114417.lpr - Increamenting the tbIndex instead of
            assigning it to constant */
         tbIndex++;


         hqP->tbInfo[idx].numSchLch = 1;
         hqP->tbInfo[idx].schdData[0].lcId = 
           allocInfo->tbInfo[idx].schdDat[0].lcId;
         hqP->tbInfo[idx].schdData[0].schdSz = 
           allocInfo->tbInfo[idx].schdDat[0].numBytes;

         if ((ret = rgAllocSBuf ((Data**)&cStaInd, sizeof(RguCStaIndInfo))) != ROK)
         {
            err->errType  = RGERR_DHM_SND_STA_IND;
            err->errCause = RG_DHM_MEM_ALLOC_FAIL;
            RETVALUE(RFAILED);
         }

         /* rg008.201.ccpu00114417.lpr - removed hardcoding tbIndex value */

         idx1 = (hqP->procId << 2) | tbIndex;
         /*rg009.201 - Read from cell CB*/
         cStaInd->cellId    = cell->cellId;
         cStaInd->rnti      = allocInfo->rnti;
         /*rg009.201 - Read from cell CB*/
         cStaInd->lcId      = cell->dlCcchId;
         cStaInd->transId   = (timingInfo.sfn << 16) | 
                              (timingInfo.subframe << 8) | idx1;
         /*rg002.301 ccpu00119012-Add-Changed the ordering*/
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
         dlSf->remDatReqCnt++;
#endif
         MSPD_DBG("<CRASH> MSG4 grant for CCCH for UE(%d) hqP(%d)\n", allocInfo->rnti, hqP->procId);	      
         /*rg002.301 ccpu00119012-Mod-Modified the ordering of function invocation.*/
         ret = rgUIMSndCmnStaInd(cStaInd);

         RETVALUE(ret);
      }
      else
      {
         /* rg001.201: Moved Processing before calling interface function */
       /* rg001.301 -DEL- [ccpu00119023] Removed retransmission check */
		 hqP->tbInfo[idx].pdcch.dci = allocInfo->dciInfo;
		 hqP->tbInfo[idx].schdTa.pres = allocInfo->tbInfo[idx].ta.pres;
         hqP->tbInfo[idx].schdTa.val  = allocInfo->tbInfo[idx].ta.val;
         tbIndex+=idx+1;
         RG_FREE_MSG(hqP->tbInfo[idx].tb);

        
         hqP->tbInfo[idx].timingInfo = timingInfo;
         hqP->tbInfo[idx].tbSz = allocInfo->tbInfo[idx].schdTbSz; 
         if((NULLP == dStaInd) && (allocInfo->tbInfo[idx].numSchLch))
         {
            if ((ret = rgAllocSBuf ((Data**)&dStaInd, sizeof(RguDStaIndInfo))) != ROK)
            {
               err->errType  = RGERR_DHM_SND_STA_IND;
               err->errCause = RG_DHM_MEM_ALLOC_FAIL;
               RETVALUE(RFAILED);
            }
         }

         for (lcIdx = 0; 
               lcIdx < allocInfo->tbInfo[idx].numSchLch; lcIdx++)
         {
            hqP->tbInfo[idx].schdData[lcIdx].lcId = 
            allocInfo->tbInfo[idx].schdDat[lcIdx].lcId;
	    if (hqP->tbInfo[idx].schdData[lcIdx].lcId == 0)
	    {
#ifdef MSPD
               stop_printf("<CRASH> CCCH grant in DStaInd\n");		    
#endif
	    }
            hqP->tbInfo[idx].schdData[lcIdx].schdSz = 
            allocInfo->tbInfo[idx].schdDat[lcIdx].numBytes;
            dStaInd->staIndTb[idx].lchStaInd[lcIdx].lcId = 
                  allocInfo->tbInfo[idx].schdDat[lcIdx].lcId;
            dStaInd->staIndTb[idx].lchStaInd[lcIdx].totBufSize = 
            allocInfo->tbInfo[idx].schdDat[lcIdx].numBytes;

			lchBufSize+=allocInfo->tbInfo[idx].schdDat[lcIdx].numBytes;// Sudhanshu
#ifdef PWAV
			if(hqP->tbInfo[idx].schdData[lcIdx].lcId == 1)
			{
			  MSPD_DBG("Got SF Alloc request for UE ID:%d @ (%d, %d)\n", ue->ueId, cell->crntTime.sfn, cell->crntTime.subframe);
			}
#endif
         }
         hqP->tbInfo[idx].numSchLch = 
            allocInfo->tbInfo[idx].numSchLch;
         if(dStaInd)
         {
            dStaInd->staIndTb[idx].nmbLch =
               allocInfo->tbInfo[idx].numSchLch;
         }
	     /* MSPD_DBG("Tb[%d] size %d  Total LCH Buf %d\n", 
          *          idx,hqP->tbInfo[idx].tbSz,lchBufSize);*/
		 lchBufSize=0;
      }
   }
   if((dStaInd) && (tbIndex))
   {
      idx1 = (hqP->procId << 2) | tbIndex;
      /* Create RguDStaInd struct and send to UIM */
      /*rg009.201 - Read from cell CB*/
      dStaInd->cellId  = cell->cellId;
      dStaInd->rnti    = allocInfo->rnti;
      /*
      dStaInd->transId = (hqP->timingInfo.sfn << 16) | 
                              (hqP->timingInfo.subframe << 8) | hqP->procId;
      */
      dStaInd->transId = (timingInfo.sfn << 16) | 
                         (timingInfo.subframe << 8) | idx1;
      dStaInd->nmbOfTbs = hqP->numOfTBs;

/* ADD Changes for Downlink UE Timing Optimization */
      /*rg002.301 ccpu00119012-Add-Changed the ordering*/
#ifdef LTEMAC_DLUE_TMGOPTMZ
      dlSf->remDatReqCnt++;
#endif

      /*rg002.301 ccpu00119012-Mod-Modified the ordering of function invocation.*/
      ret = rgUIMSndDedStaInd(dStaInd);

      RETVALUE(ret);
   }
   RETVALUE(ROK);
}  /* rgDHMSndStaInd */

/* rg008.201 - Removed dependncy on MIMO compile-time flag */

/**
 * @brief Function to handle building the TFU Data Request
 *
 * @details
 *
 *     Function : rgDHMBldTfuDatReq
 *     
 *     This function builds the TFU Data Request with the details 
 *     present in HARQ Process.
 *           
 *  @param[in]  RgDlHqProcCb     *hqP 
 *  @param[out] TfuDatReqPduInfo  *datReq 
 *  @return     Void
 *              None 
 **/
#ifdef ANSI
PRIVATE Void rgDHMBldTfuDatReq
(
RgDlSf             *dlSf,
RgDlHqProcCb       *hqP,
RgTfuDatReqPduInfo *datReq
)
#else
PRIVATE Void rgDHMBldTfuDatReq(dlSf, hqP, datReq)
RgDlSf             *dlSf;
RgDlHqProcCb       *hqP;
RgTfuDatReqPduInfo *datReq;
#endif
{
   U8 i;
   TRC2(rgDHMBldTfuDatReq)
 
   datReq->nmbOfTBs = 0;
   /*MS_WORKAROUND  for ccpu00123904*/
   datReq->isTApres = FALSE;
   for(i=0;i<RG_MAX_TB_PER_UE;i++)
   {
      if ((hqP->tbInfo[i].sf == dlSf) && (hqP->tbInfo[i].tb != NULLP))
      {
         datReq->rnti           =  hqP->tbInfo[i].pdcch.rnti;
         datReq->dciInfo        =  hqP->tbInfo[i].pdcch.dci;
#ifdef LTEMAC_MIMO   
         datReq->doa            =  hqP->tbInfo[i].doa;
         datReq->transMode      =  hqP->tbInfo[i].txMode;
         datReq->puschRptUsd    =  hqP->tbInfo[i].puschRptUsd;
         datReq->puschPmiInfo   =  hqP->tbInfo[i].puschPmiInfo;
#endif
         /*MS_WORKAROUND  for ccpu00123904*/
         if (hqP->tbInfo[i].schdTa.pres)
         {
            datReq->isTApres       =  TRUE; 
         }
#ifdef   TFU_UPGRADE
         /* update pA value */
         datReq->pa             =  hqP->tbInfo[i].pa;
#endif
         SCpyMsgMsg(hqP->tbInfo[i].tb, RG_GET_MEM_REGION(rgCb),
               RG_GET_MEM_POOL(rgCb), &datReq->mBuf[i]);
         {
            MsgLen   dbgBufLen;
            if(SFndLenMsg(datReq->mBuf[i], &dbgBufLen))
            {
               if(dbgBufLen == 0)
               {			   	
                  MSPD_DBG("[%d][%d,%d] tbIdx [%d]Sdu Length 0 taPrs [%d] numOfTb [%d] format[%d]\n", datReq->rnti, hqP->tbInfo[i].timingInfo.sfn,
                  hqP->tbInfo[i].timingInfo.subframe, i, datReq->isTApres, hqP->numOfTBs, datReq->dciInfo.format);  
               }				  	
            }            
         }		 
         datReq->nmbOfTBs++;
      }
   }
   RETVOID;
}  /* rgDHMBldTfuDatReq */



/**
 * @brief Handler for freeing up the harq related information from ueCb
 *
 * @details
 *
 *     Function : rgDHMFreeUe
 *     
 *     This function shall free up the HARQ specific information from ueCb.
 *           
 *  @param[in]  RgDlHqEnt     *hqE 
 *
 *  @return     None.
 *
 **/
#ifdef ANSI
PUBLIC Void rgDHMFreeUe
(
RgDlHqEnt          *hqE
)
#else
PUBLIC Void rgDHMFreeUe(hqE)
RgDlHqEnt          *hqE;
#endif
{
   U8             i;

   TRC2(rgDHMFreeUe)

   if(hqE->procs)
   {
      /* Free all the memory associated with HARQ */
      for (i=0; i < hqE->numHqProcs; i++)
      {
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependncy on MIMO compile-time flag */
         rgDHMRlsHqProcTB(&hqE->procs[i], 1);
         rgDHMRlsHqProcTB(&hqE->procs[i], 2);
      }

      /*ccpu00117052 - MOD - Passing double pointer for proper NULLP
                            assignment */
      rgFreeSBuf((Data **)&(hqE->procs), 
                 sizeof(RgDlHqProcCb)*(hqE->numHqProcs));
   }

   RETVOID;

}  /* rgDHMFreeUe */

/**
 * @brief Function for handling RaResp request received from scheduler to MAC
 *
 * @details
 *
 *     Function : RgSchMacRlsHqReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of random access responses for a subframe.
 *     This shall invoke RAM to create ueCbs for all the rapIds allocated and 
 *     shall invoke MUX to create RAR PDUs for raRntis allocated.
 *     
 *           
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RaRespInfo          *rarInfo
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacRlsHqReq
(
Pst                 *pst,
RgInfRlsHqInfo      *rlshqUeInfo
)
#else
PUBLIC S16 RgSchMacRlsHqReq(pst, rlshqUeInfo)
Pst                 *pst;
RgInfRlsHqInfo      *rlshqUeInfo;
#endif
{
   RgCellCb       *cell;
   RgUeCb         *ue;
   RgDlHqProcCb   *hqP;
   U8             idx1,idx2;
#ifdef LTE_L2_MEAS
   RgErrInfo      err;
#endif /* LTE_L2_MEAS */
#ifdef _MSPD_MLOG_NEW 
   U32        startTick, endTick;

   startTick = GetTIMETICK();
#endif
   TRC2(RgSchMacRlsHqReq)

   if(NULLP == rlshqUeInfo)
   {
      RETVALUE(RFAILED);
   }

   if((cell = rgDBMGetCellCb(rlshqUeInfo->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "RgSchMacRlsHqReq(): " 
               "No cellCb found with cellId = (%d)\n", rlshqUeInfo->cellId));
      MSPD_ERR("RgSchMacRlsHqReq(): " 
               "No cellCb found with cellId = (%d)\n", rlshqUeInfo->cellId);
      RETVALUE(RFAILED);
   }

   if(NULLP == rlshqUeInfo->ueHqInfo)
   {
      RETVALUE(RFAILED);
   }

   for(idx1 = 0; idx1 < rlshqUeInfo->numUes; idx1++)
   {
      if((ue=rgDBMGetUeCb (cell, rlshqUeInfo->ueHqInfo[idx1].rnti)) == NULLP)
      {
#ifdef MSPD
         MSPD_DBG("Failed to get UE(%d) from cell->ueLst\n", rlshqUeInfo->ueHqInfo[idx1].rnti);
#endif
         /* Check in RachLst */
         if((ue=rgDBMGetUeCbFromRachLst (cell, 
                     rlshqUeInfo->ueHqInfo[idx1].rnti)) == NULLP)
         {
            RGDBGERR((rgPBuf, "RgSchMacRlsHqReq(): " 
                     "No ueCb found with rnti = (%d)\n", 
                        rlshqUeInfo->ueHqInfo[idx1].rnti));
            MSPD_ERR("RgSchMacRlsHqReq(): " 
                     "No ueCb found with rnti = (%d)\n", 
                        rlshqUeInfo->ueHqInfo[idx1].rnti);
#ifdef MSPD
            MSPD_DBG("Failed to get UE(%d) from cell->rachLst\n", rlshqUeInfo->ueHqInfo[idx1].rnti);
#endif
            continue;
         }
      }

      rgDHMGetHqProcFrmId(ue,rlshqUeInfo->ueHqInfo[idx1].hqProcId,&hqP);
#ifdef PWAV
	    if(hqP == NULLP)
	  {
	    MSPD_DBG("Harq Proc Id:%d for UE :%d\n", rlshqUeInfo->ueHqInfo[idx1].hqProcId, rlshqUeInfo->ueHqInfo[idx1].rnti);
	  }
#endif
#ifdef MSPD
	   if(rlshqUeInfo->ueHqInfo[idx1].status[0] != TRUE)
        {
            rgCb.genSts.numHarqFail++;
        }
#endif /* MSPD */

#ifdef LTE_L2_MEAS
      /* Call UIM function to send HARQ Status Indication fior this UE */ 
      if ( rgUIMSndharqStaInd(cell->cellId, &(rlshqUeInfo->ueHqInfo[idx1]), hqP, &err) != ROK)
      {
         /* Do i need to FAIL here or Just Ignore */
      }
#endif /* LTE_L2_MEAS */

      for(idx2=0; idx2 < rlshqUeInfo->ueHqInfo[idx1].numOfTBs; idx2++)
      {
         if(rgDHMRlsHqProcTB(hqP, 
               rlshqUeInfo->ueHqInfo[idx1].tbId[idx2]) != ROK)
         {
            RGDBGERR((rgPBuf, "RgSchMacRlsHqReq():" 
                     "Failure in releasing hq TB for UE = (%d)\n",
                        rlshqUeInfo->ueHqInfo[idx1].rnti));
            MSPD_ERR("RgSchMacRlsHqReq():" 
                     "Failure in releasing hq TB for UE = (%d)\n",
                        rlshqUeInfo->ueHqInfo[idx1].rnti);
            continue;
         }
      }
   } /* end of ues loop */
#ifdef _MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   MLogTask(PID_MAC_AM_HARQ_RLS , RESOURCE_LARM, startTick, endTick);
#endif
   RETVALUE(ROK);
} /* end of RgSchMacRlsHqReq */
/**********************************************************************
 
         End of file:     gk_dhm.c@@/main/3 - Sat Jul 30 02:21:26 2011
 
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
/main/1      ---     ps              1. Initial Release.
/main/2      ---     ps              1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg003.201 sm 1. changes for CR ccpu00108099. 
             rg004.201 sm 1. ABR fixed
             rg005.201 sm 1. Changes for optimization.
             rg007.201   ap  1. Added support for MIMO
             rg008.201 sd  1. Removed dependncy on MIMO compile-time flag
             rg009.201 rsharon 1.[ccpu00112372] Changes for CR timer implementation.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1         
      rg001.301   nudupi   1. Changes for the TB indexing issue.
                           2. Added the changes corresponding to the CR ccpu00119023
      rg002.301  asehgal   1. ccpu00120298 ADD sf is reset to NULLP
*********************************************************************91*/
