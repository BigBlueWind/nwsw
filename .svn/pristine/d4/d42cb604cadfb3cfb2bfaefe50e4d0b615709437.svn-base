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
  
        Name:    LTE-PDCP Layer - Lower Interface Functions
    
        Type:    C file
  
     Desc:     Source code for PDCP Lower Interface Module.
               This file contains following functions
      
                    --pjLimDatInd
                    --pjLimDatCfm
                    --pjLimStaInd

     File:     gp_pj_lim.c

     Sid:      gp_pj_lim.c@@/main/1 - Fri Nov 13 14:13:46 2009

     Prg:      mn 

**********************************************************************/
/** @file gp_pj_lim.c
@brief PDCP Lower Interface Module
*/


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000241     LTE-MAC
 *      1000369     LTE-RRC
 */


/* header (.h) include files */
#include "envopt.h"             /* environment options */
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm5.h"                /* Timer Functions */
#include "cm_lte.h"             /* common LTE header file */
#include "cm_hash.h"            /* common hash module  file */
#include "cm_llist.h"           /* common list header file */
#include "cpj.h"                /* RRC layer */
#include "pju.h"                /* PDCP service user */
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
#include "lpj.h"                /* RRC layer */
#include "kwu.h"                /* RLC service user */
#include "lkw.h"                /* LM Interface */
#include "rgu.h"                /* MAC layer */
#include "kw_env.h"             /* RLC environment options */
#include "kw.h"                 /* RLC layer */
#include "kw_err.h"

/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* Timer Functions */
#include "cm_hash.x"            /* common hash module */
#include "cm_lte.x"             /* common LTE file */
#include "cm_llist.x"           /* common list header file */
#include "cpj.x"                /* RRC layer */
#include "pju.x"                /* PDCP service user */
#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "lpj.x"                /* LM Interface */
#include "rgu.x"                /* MAC later */
#include "kw.x"                 /* RLC layer */
 
#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


/*****************************************************************************
 *                          PDCP LOWER INTERFACE
 ****************************************************************************/


/**
 *
 * @brief 
 *
 *        Handler for getting the PDU from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void pjLimDatInd
(
PjRbCb        *pjRbCb,        /* RB Control Block */
Buffer        *mBuf           /* RLC PDU */
)
#else
PUBLIC Void pjLimDatInd(pjRbCb, mBuf)
PjRbCb        *pjRbCb;        /* RB Control Block */
Buffer        *mBuf;          /* RLC PDU */
#endif
{
   S16         ret;           /* Return Value */

   TRC3(pjLimDatInd);


   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimDatInd(pjRbCb(%d), mBuf)\n", pjRbCb->rbId));

   ret = ROK;
   PJ_STS_INC_GEN_CNT(rxPdus);

   /* Extract the header */
   if(pjRbCb->rbType == PJ_SRB)
   {
      /* If UE state is REEST then, we should allow packets on
         SRB1 if reestablishment of SRB1 is complete */
      if ((pjRbCb->ueCb->libInfo.state == PJ_STATE_NORMAL) ||
            (pjRbCb->rbId == PJ_SRB1_ID && pjRbCb->state != PJ_STATE_REEST)) /*SRB1*/
      {
         ret = pjUlmHdlSrbPkt(pjRbCb, mBuf);
      }
      else /* SRB2 */
      {
         /* SRB2 data can be dropped*/
         PJ_FREE_BUF(mBuf);
      }
   }
   else if(pjRbCb->rbType == PJ_DRB)
   {
      ret = pjUlmHdlDrbPkt(pjRbCb, mBuf);
   }

   /* kw006.201, ccpu00120574, fixed the warning */
   if (ret != ROK)
   {
      KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR), (_kwPBuf,
               "pjLimDatInd: pjHdlProcessPdu Failed\n"));
   }

   RETVOID;
} /* pjLimDatInd */


/**
 *
 * @brief 
 *
 *        Handler for getting the Data Cfm from RLC.
 *
 * @b Description:
 *
 *        This function receives the Data Cfm from RLC 
 *        and invokes the handler for processing it.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] datCfm    -   Data Confirmation Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void pjLimDatCfm
(
PjRbCb        *pjRbCb,     /* RB Control Block */
PjDatCfm      *datCfm      /* Data Confirmation Information */
)
#else
PUBLIC Void pjLimDatCfm(pjRbCb, datCfm)
PjRbCb        *pjRbCb;     /* RB Control Block */
PjDatCfm      *datCfm;     /* Data Confirmation Information */
#endif
{
   S16         ret = ROK;  /* Return Value */

   TRC3(pjLimDatCfm);


   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimCDatInd(pjRbCb(%d), datCfm)\n",pjRbCb->rbId));

   /* Initialisations */

   /* Process the PDU */
   ret = pjDlmProcessCfm(pjRbCb, datCfm, PJ_CFM_OK);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR), (_kwPBuf,
               "pjLimDatCfm: pjDlmProcessCfm Failed\n"));
   }
#endif
   /* kw002.201 : Removed freeing and allocation of datCfm */
   RETVOID;
} /* pjLimDatCfm */


/**
 *
 * @brief 
 *
 *        Handler for trigerring the error indication when RLC times out
 *        for sending AM data.
 *
 * @b Description:
 *
 *        This function receives the pduIds of the PDU that were not
 *        successfully sent to the layer.
 *
 *  @param[in] pjRbCb  -   PDCP RB Control Block
 *  @param[in] staInd  -   Status Indication Information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void pjLimStaInd
(
PjRbCb       *pjRbCb,            /* PDCP RB Control Block */
PjDatCfm     *staInd             /* Status Indication Information */
)
#else
PUBLIC Void pjLimStaInd(pjRbCb, staInd)
PjRbCb       *pjRbCb;           /* PDCP RB control Block */
PjDatCfm     *staInd;           /* Status Indication Information */
#endif
{
   S16         ret = ROK;       /* Return Value */

   TRC3(pjLimStaInd)

   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimStaInd(pjRbCb(%d), staInd)\n", pjRbCb->rbId));

   /* Call the confirm code */
   ret = pjDlmProcessCfm(pjRbCb, staInd, PJ_CFM_NOK);
   
   /* kw006.201, ccpu00120574, fixed the warning */
   if (ret != ROK)
   {
      KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR), (_kwPBuf,
               "pjLimStaInd: pjDlmProcessCfm Failed\n"));
   }
   /* kw002.201 : Removed freeing and allocation of staInd */
   RETVOID;
} /* pjLimStaInd */


/**
 *
 * @brief 
 *
 *        Handler for informing the PDCP that RLC is done with sending
 *        all data that it can during reestablishment.
 *
 * @b Description:
 *
 *        This function receives the pointer to the PDCP control block of the
 *        RB that has finished transmitting its data to PDCP.
 *
 *  @param[in] pjRbCb  -   PDCP RB Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void pjLimReEstCmpInd
(
PjRbCb       *pjRbCb             /* PDCP RB Control Block */
)
#else
PUBLIC Void pjLimReEstCmpInd(pjRbCb)
PjRbCb       *pjRbCb;           /* PDCP RB control Block */
#endif
{
   TRC3(pjLimReEstCmpInd)

   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimReEstCmpInd(pjRbCb(%d))\n", pjRbCb->rbId));

   /* No need to update state for SRB */
   if(pjRbCb->rbType == PJ_SRB)
   {
    
      if (pjRbCb->rbId == PJ_SRB1_ID && pjRbCb->state == PJ_STATE_REEST)
      {
         pjUtlSecUpdAndSndReestCfm(pjRbCb->ueCb, pjRbCb);
      }
      else /* SRB2 */
      {
         /* Just change the state, Re-establishment for SRB2
            is already done in PDCP and RLC */
         pjRbCb->state = PJ_STATE_NORMAL;
      }

      RETVOID;
   }

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   /* If Async mode, set variable to true and return */
   pjRbCb->ulCb.transCmp = TRUE;
   if(pjRbCb->ulCb.obdCnt == 0)
   {
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
      /* In synchronous mode, we can proceed with reestablishment */
      if ( pjRbCb->state != PJ_STATE_NORMAL )
      {
         if (pjRbCb->mode  ==  PJ_DRB_UM)
         {
            PJ_ULM_DRBUM_REEST(pjRbCb);
         }
         else
         {
            (Void)pjUlmReEstDrbAm(pjRbCb);
         }
      }
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

   RETVOID;
} /* pjLimReEstCmpInd */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief 
 *
 *        Handler for informing the PDCP that RLC has received a HarqAckInd
 *        which signifies that the messages has been received at the peer.
 *
 * @b Description:
 *
 *        This function receives the pointer to the PDCP control block of the
 *        RB that has finished transmitting its data to PDCP.
 *
 *  @param[in] pjRbCb  -   PDCP RB Control Block
 *  @param[in] count   -   Count of the PduId being acked
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
/* kw006.201 ccpu00120058, fixed non ansi compilation problem */
#ifdef ANSI
PUBLIC S16 pjLimDatAckInd
(
PjRbCb   *pjRbCb,       /* PDCP RB Control Block */
U32      count          /* Count of the PDU Id  */
)
#else
PUBLIC S16 pjLimDatAckInd(pjRbCb, count)
PjRbCb   *pjRbCb;       /* PDCP RB control Block */
U32      count;          /* Ack Information */
#endif
{
   PjTxEnt *txEnt;
   EpcTime ackTime;
   TRC3(pjLimDatAckInd);
#ifndef ALIGN_64BIT
   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimDatAckInd(pjRbCb(%d), count(%ld))\n", pjRbCb->rbId, count));
#else
   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimDatAckInd(pjRbCb(%d), count(%d))\n", pjRbCb->rbId, count));
#endif
   /* No need to update state for SRB */
   if((pjRbCb->rbType == PJ_SRB )|| 
      (!(pjRbCb->rbL2Cb.measOn & LPJ_L2MEAS_DL_DELAY)))
   {
      RETVALUE(ROK);
   }

   txEnt = (PjTxEnt *)pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
   if(txEnt == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
#ifndef ALIGN_64BIT
      KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR), (_kwPBuf,
               "pjLimDatAckInd: count(%ld) does not exist \n", count));
#else
      KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR), (_kwPBuf,
               "pjLimDatAckInd: count(%d) does not exist \n", count));
#endif
#endif
      RETVALUE(RFAILED);
   }

   SGetEpcTime(&ackTime);
   if(txEnt->epcTime > 0)
   {
      PJ_UPD_L2_DLDELAY_STS(pjRbCb, (ackTime - txEnt->epcTime));
   }

   if((pjRbCb->rbType == PJ_DRB) && (pjRbCb->mode == PJ_DRB_UM))
   {
      pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), txEnt->count);
   }

   RETVALUE(ROK);
} /* pjLimDatAckInd */


/**
 *
 * @brief 
 *
 *        Handler for informing the PDCP that RLC has successfully discarded
 *        a certain PDU.
 *
 * @b Description:
 *
 *        This function receives the pointer to the PDCP control block of the
 *        RB that has finished transmitting its data to PDCP.
 *
 *  @param[in] pjRbCb  -   PDCP RB Control Block
 *  @param[in] count   -   Count of the PduId being acked
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
/* kw006.201 ccpu00120058, fixed non ansi compilation problem */
#ifdef ANSI
PUBLIC S16 pjLimDiscSduCfm
(
PjRbCb   *pjRbCb,       /* PDCP RB Control Block */
U32      count          /* Count of the PDU Id  */
)
#else
PUBLIC S16 pjLimDiscSduCfm(pjRbCb, count)
PjRbCb   *pjRbCb;       /* PDCP RB control Block */
U32      count;         /* Ack Information */
#endif
{
   TRC3(pjLimDiscSduCfm);
#ifndef ALIGN_64BIT
   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimDiscSduCfm(pjRbCb(%d), count(%ld))\n", pjRbCb->rbId, count));
#else
   KWDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "pjLimDiscSduCfm(pjRbCb(%d), count(%d))\n", pjRbCb->rbId, count));
#endif
   PJ_UPD_L2_DLDISC_STS(pjRbCb);

   RETVALUE(ROK);
} /* pjLimDatAckInd */
#endif /* LTE_L2_MEAS */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************************************30**
         End of file:     gp_pj_lim.c@@/main/1 - Fri Nov 13 14:13:46 2009
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
/main/1      ---       mn   1. LTERLC Release 2.1
/main/2      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/3      kw005.201 ap   1. Added support for L2 Measurement.
/main/4      kw006.201 ap   1. Fixed non-ansi compilation problem.
                            2. Replaced macro with function.
*********************************************************************91*/
