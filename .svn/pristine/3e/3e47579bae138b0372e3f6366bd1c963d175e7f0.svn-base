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

/**********************************************************************

     Name:     LTE-PDCP Layer 
  
     Type:     C file
  
     Desc:     Source code for PDCP Downlink module.
               This file contains following functions

                  --pjDlmProcessSdus
                  --pjDlmBldPdu
                  --pjDlmReEstSrb
                  --pjDlmReEstDrbUm
                  --pjDlmReEstDrbAm
                  --pjDlmProcessCfm
                  --pjDlmHndlStaRep
                  --pjDlmDiscSdu
                  --pjDlmDeliverPdu
                  --pjDlmReEstHoDrbAm
                  --pjDlmHndlDatFwdReq
                  --pjDlmProcSrb
                  --pjDlmProcDrb

     File:     gp_pj_dlm.c

     Sid:      gp_pj_dlm.c@@/main/1 - Fri Nov 13 14:13:44 2009

     Prg:      vp 

**********************************************************************/
/** @file gp_pj_dlm.c
@brief PDCP Downlink module
*/

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000372     LTE-MAC
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
#include "lpj.h"                /* LPJ defines */
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
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
#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "cpj.x"                /* RRC layer */
#include "pju.x"                /* PDCP service user */
#include "lpj.x"                /* LPJ defines */
#include "rgu.x"                /* MAC later */
#include "kw.x"                 /* RLC layer */

/* kw005.201 Moved definition of KW_PJ_ASYNC_WIN to kw.h file */
/** @addtogroup dldata */
/*@{*/

PRIVATE S16 pjDlmBldPdu(PjRbCb *pjRbCb,PjTxEnt *txEnt);


/**
 *
 * @brief 
 *
 *        Handler to process the SDU received from upper layer, form a PDU
 *        and send the PDU to the lower layer.
 *
 * @b Description: 
 *
 *        1. This function places the SDU in the transmission buffer and performs
 *        Compression for DRB SDUs and Integrity Protection for SRB SDUs.  @n
 *        2. After compression/integrity protection, it performs ciphering and then 
 *        constructs the PDU and sends it to the lower layer. @n
 *        3. The first DL message of SRBs is not ciphered and is just integrity 
 *        protected.  @n 
 *        4. This function checks the UE CB control block to check the flag for the 
 *        first DL message and performs only integrity protection and unsets 
 *        the flag.       @n 
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] sdu      SDU to be processed.
 *  @param[in] sduId    SDU ID. 
 *  @param[in] sn       SN to be assigned.
 *  @param[in] txHfn    TX_HFN to be used for current SDU.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 pjDlmProcessSdus
(
PjRbCb *pjRbCb,                   /* !< PDCP Control Block */
Buffer *sdu,                      /* !< SDU to be processed */
U32    sduId,                      /* !< PDCP SDU ID */
PjSn   sn,                        /* !< PDCP SN to be assigned */
U32    txHfn                      /* !< TX_HFN to be assigned */
)
#else 
PUBLIC S16 pjDlmProcessSdus(pjRbCb, sdu, sduId, sn, txHfn)
PjRbCb *pjRbCb;                   /* !< PDCP Control Block */
Buffer *sdu;                      /* !< SDU to be processed */
U32    sduId;                     /* !< PDCP SDU ID */
PjSn   sn;                        /* !< PDCP SN to be assigned */ 
U32    txHfn;                     /* !< TX_HFN to be assigned */
#endif
{
   S16      ret;                  /* Return Value */
   PjTxEnt  *txEnt;               /* Transmission Entity for sdu*/
   U32      count;                /* Count Value for this txEnt */
   U32      discInt;              /* Discard Timer Interval of a RB */
   Bool     tmrRunning;           /* Flag for checking the timer */

   TRC2(pjDlmProcessSdus)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
         (_kwPBuf, "pjDlmProcessSdus(pjRbCb(%d), sdu, sduId(%ld), \
                    sn(%u), txHfn) \n)", pjRbCb->rbId, sduId, sn));
#else
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
         (_kwPBuf, "pjDlmProcessSdus(pjRbCb(%d), sdu, sduId(%d), \
                    sn(%u), txHfn) \n)", pjRbCb->rbId, sduId, sn));
#endif

   txEnt      = NULLP;
   count      = 0;
   discInt    = pjRbCb->dlCb.discTmrInt;
   tmrRunning = FALSE;
   ret        = ROK;

   /* Allocate the transmission entity */
   PJ_ALLOC(txEnt, sizeof(PjTxEnt));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (txEnt == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW053, (ErrVal)sduId,
            "pjDlmProcessSdus: Allocation failed for txEnt\n");
      PJ_FREE_BUF(sdu);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   PJ_UPD_L2_DLDISC_PKT_STS(pjRbCb);
   /* Update COUNT to the current count in dlCb*/
   PJ_UPD_COUNT(pjRbCb, sn, txHfn, count);
   pjRbCb->dlCb.count = count;

   /* Fill TxEnt values */
   txEnt->sn = sn;
   txEnt->count = count;
   txEnt->txHfn = txHfn;
   txEnt->state = PJ_SDU_RECEIVED;
   txEnt->sduId = sduId;
   /* The received buffer is stored in the SDU and the output from 
    * Compression/Ciphering is stored in the PDU.
    */
   txEnt->sdu   = sdu;
   txEnt->pdu   = NULLP;
   /* Insert TxEnt into the transmission buffer */
   pjDbmInsTxEnt(&(pjRbCb->dlCb.txBuf), txEnt);

   /* Start the timer if it is not running */
   if ( discInt )
   {
      SGetSysTime(&txEnt->arrTime);
      if (pjRbCb->dlCb.discTmr.tmrEvnt == PJ_EVT_DISC_TMR)
      {
         tmrRunning = TRUE;
      }

      if ( tmrRunning == FALSE && pjRbCb->dlCb.discTmrInt != -1  )
      {
         pjStartTmr((PTR)pjRbCb, PJ_EVT_DISC_TMR);
      }
   }
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   /* To get the DL Delay , we need to get the arrical time */
   if(pjRbCb->rbL2Cb.measOn & PJ_L2MEAS_DL_DELAY)
   {
      SGetEpcTime(&txEnt->epcTime);
   }
#endif /* LTE_L2_MEAS */

   /* Process the SDU based on rbType */
   if (pjRbCb->rbType == PJ_SRB)
   {
      ret = pjDlmProcSrb(pjRbCb, txEnt);
   }
   else
   {
#ifdef MSPD
   {
      extern U32 dlrate_pju;
      MsgLen len;
      SFndLenMsg(sdu, (MsgLen *) &len);
      dlrate_pju += len;      
   }
#endif
      ret = pjDlmProcDrb(pjRbCb, txEnt);
   }

   RETVALUE(ret);
}

/**
*
 * @brief 
 *
 *        Handler to construct a data PDU/Control PDU.
 *
 * @b Description: 
 *
 *        1. This function constructs the PDU header based on the SN length
 *        configured.    @n
 *        2. The header is inserted at the start of the PDU and
 *        the constructed PDU is returned to the calling function. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 pjDlmBldPdu
(
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PRIVATE S16 pjDlmBldPdu(pjRbCb,txEnt)
PjRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   U16        hdr;                /* Header field */
   PjSn       sn;                 /* SN in the PDU */
   S16        ret;                /* Return Value */
   Buffer     *pdu;               /* PDU to be constructed */
   U32        numBytes;           /* Number of bytes to be added to the PDU */

   TRC3(pjDlmBldPdu)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                (_kwPBuf, "pjDlmBldPdu(pjRbCb, txEnt (%ld)) \n", txEnt->sduId));
#else
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                (_kwPBuf, "pjDlmBldPdu(pjRbCb, txEnt (%d)) \n", txEnt->sduId));
#endif
  
   hdr       = 0;
   sn        = txEnt->sn; 
   ret       = ROK;
   numBytes  = 0;
   pdu       = txEnt->pdu;

   /* Build a Data PDU */
   /* 5 bit SN is the only header field */
   if ( pjRbCb->rbType == PJ_SRB )
   {
      hdr = sn;
      numBytes = 1;
   }
   else 
   {
      /* The one byte header contains the MSB as '1' */
      if ( pjRbCb->snLen == PJ_7_BIT_SN )
      {
         hdr = sn;
         hdr |= PJ_DRB_7BIT_SN_HDR;
         numBytes = 1;
         //MSPD_DBG_RLC("7 Bit SN: [%d] PDCP hdr 0x%x\n", 
         //              pjRbCb->ueCb->key.ueId, hdr);
      }
      /* The two byte header has the SN and MSB as '1' */
      else 
      {
         hdr = sn;
         hdr |= PJ_DRB_12BIT_SN_HDR;
         numBytes = 2;
         //MSPD_DBG_RLC("12 Bit SN: [%d] PDCP hdr 0x%x\n", 
         //              pjRbCb->ueCb->key.ueId, hdr);
      }
   }

   /* Add the hdr(based on numBytes) at the start of the PDU */
   PJ_ADD_PRE_MSG( pdu, hdr, numBytes, ret);

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW054, (ErrVal)txEnt->count,
             "pjDlmBldPdu: SAddPreMsg failed for pdu \n");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */
   RETVALUE(ret);
}

/**
 *
 * @brief 
 *
 *        Handler to Re-establish a SRB.
 *
 * @b Description: 
 *
 *        1. This function is called when re-establishment request is
 *        received for a SRB.   @n
 *        2. The stored PDUs and SDUs are freed and the variables are reset. @n
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *
 */

#ifdef ANSI
PUBLIC Void pjDlmReEstSrb
(
PjRbCb *pjRbCb
)
#else
PUBLIC Void pjDlmReEstSrb(pjRbCb)
PjRbCb *pjRbCb;
#endif
{

   TRC3(pjDlmReEstSrb)

   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                      (_kwPBuf, "pjDlmReEstSrb(pjRbCb (%d)) \n", pjRbCb->rbId));
  

   /* Stop the off-board and discard timer if running */
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   if (pjRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
   {
       pjStopTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }
#endif
   if (pjRbCb->dlCb.discTmr.tmrEvnt == PJ_EVT_DISC_TMR)
   {
       pjStopTmr((PTR)pjRbCb, PJ_EVT_DISC_TMR);
   }

   /* Clean up all the SDUs present */ 
   pjDbmTxDelAll(&(pjRbCb->dlCb.txBuf));

   /* Reset the variables in rbCb */
   PJ_RESET_ALL_VAR(pjRbCb);
   /*kw004.201 Adding of Missing Trace in LTE RLC PDCP*/
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to Re-establish a UM DRB.
 *
 * @b Description: 
 *
 *        1. This function is called when re-establishment request is
 *           received for a UM DRB.   @n
 *        2. The stored PDUs are freed and the variables are reset.  @n
 *        3. The PDUs which were associated with a SN and were not sent 
 *           to the lower layer are now sent with the new ciphering 
 *           algorithm and keys. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmReEstDrbUm
(
PjRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmReEstDrbUm(pjRbCb)
PjRbCb *pjRbCb;
#endif
{
   S16            ret;                     /* Return Value */ 
   U32            count;                   /* COUNT of nxtToSub */
   U32            curCount;                /* Current COUNT */
   U32            newCount;                /* New Count of a PDU */
   PjTxEnt        *txEnt;                  /* Transmission Entity */
   /* kw006.201, ccpu00120574, fixed the warning */
   PjSn           sn;                      /* SN for the PDU */
   U32            txHfn;                   /* TX_HFN of the RB */
   PjBuf          tmpTxBuf;                /* Tx Buffer */ 
 
   TRC3(pjDlmReEstDrbUm)

   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
             (_kwPBuf, "pjDlmReEstDrbUm(pjRbCb (%d)) \n", pjRbCb->rbId));
  
   count    =  pjRbCb->dlCb.nxtToSub;
   curCount =  pjRbCb->dlCb.count;
   newCount =  0;
   txEnt    =  NULLP;
   ret      =  ROK;
   sn       =  0;
   txHfn    =  0;
   /* kw006.201, ccpu00120574, fixed the warning */

   /* Reset all the variable */
   PJ_RESET_ALL_VAR(pjRbCb);  
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   /* Stop the off board timer if running */
   if (pjRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
   {
       pjStopTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   } 
#endif

   if(pjRbCb->dlCb.txBuf.numEntries == 0)
   {
      RETVALUE(ROK);
   }

   /* Create the new buffer list  and store the old pointer in datQ */
   PJ_CREATE_NEW_LIST(pjRbCb, tmpTxBuf, ret);
#if (ERRCLASS & ERRCLS_ADD_RES) 
   if (ret != ROK)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW055, (ErrVal)0,
         "pjDlmReEstDrbUm: PJ_ALLOC Failed  for new list creation\n");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* kw006.201, ccpu00120574, fixed the warning */
   /* Process all the existing PDUs as received from above layer */
   while ( count <= curCount )
   {
      U8   hashKey = 0;
      txEnt = (PjTxEnt *)pjDbmGetTxEnt(&tmpTxBuf, count);
      /* Assign the new SN, TX_HFN and insert in the new transmission
       * buffer.
       */
      if ( txEnt != NULLP )
      {
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
         if ( txEnt->pdu != txEnt->sdu )
         {
            PJ_FREE_BUF(txEnt->pdu);
         }
         else
         {
            txEnt->pdu = NULLP;
         }
#endif
         PJ_DLM_UPD_VAR(pjRbCb, sn, txHfn);
         PJ_UPD_COUNT(pjRbCb, sn, txHfn, newCount);   

         txEnt->sn = sn;
         txEnt->count = newCount;
         txEnt->txHfn = txHfn;
         txEnt->state = PJ_SDU_RECEIVED;
         txEnt->pdu = NULLP;
         pjRbCb->dlCb.count = newCount;

         hashKey = PJ_HASH_FN((&tmpTxBuf), count);

         cmLListDelFrm(&tmpTxBuf.datQ[hashKey], &txEnt->lstEnt);
         tmpTxBuf.numEntries --;
         pjDbmInsTxEnt(&(pjRbCb->dlCb.txBuf), txEnt);
         /* Process the SDU - perform compression/ciphering */
         ret = pjDlmProcDrb(pjRbCb, txEnt);    
         if ( ret != ROK )
         {
            PJLOGERROR(ERRCLS_ADD_RES, EKW056, (ErrVal)txEnt->count,
               "pjDlmReEstDrbUm: pjDlmProcDrb Failed \n");
         }
      }
      count++;
   }
   /* Free up the old transmission buffer */
   pjDbmTxDeInit(&tmpTxBuf);
#ifdef MSPD 
   MSPD_DBG("[%d][%d] Reestablishment done at pdcp\n", pjRbCb->ueCb->key.ueId,
       pjRbCb->rbId);
#endif
   RETVALUE(ret);    
}

/**
 *
 * @brief 
 *
 *        Handler to Re-establish a AM DRB.
 *
 * @b Description: 
 *
 *        1. This function is called when re-establishment request is
 *        received for a AM DRB.     @n
 *        2. The stored PDUs are freed.   @n
 *        3. The PDUs which were not submitted to the lower
 *        layer and for which a confirm has not been received
 *        are now sent with the new ciphering algorithm and keys. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlmReEstDrbAm
(
PjRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmReEstDrbAm(pjRbCb)
PjRbCb *pjRbCb;
#endif
{
   U32            startCount;         /* COUNT of the first valid txEnt */
   U32            stopCount;          /* COUNT of the recent txEnt */
   PjTxEnt        *txEnt;             /* Transmission Entity */
   S16            ret;                /* RETURN value */

   TRC3(pjDlmReEstDrbAm)

   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                (_kwPBuf, "pjDlmReEstDrbAm(pjRbCb (%d)) \n",  pjRbCb->rbId));

#ifdef MSPD
   MSPD_DBG("[%d][%d] Reestablishment Start at pdcp\n", pjRbCb->ueCb->key.ueId,
   	  pjRbCb->rbId);
#endif
  
   startCount =   pjRbCb->dlCb.cfmExp;
   pjRbCb->dlCb.nxtToSub = pjRbCb->dlCb.cfmExp;
   stopCount  =   pjRbCb->dlCb.count;
   txEnt      =   NULLP;
  
   ret        =   ROK;
  
   /* Process all the unacknowledged SDUs with new keys for ciphering */
   while (( startCount <= stopCount ) && (pjRbCb->dlCb.txBuf.numEntries > 0))
   {
      txEnt = (PjTxEnt *)pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), startCount);
      if ( txEnt != NULLP)
      {
         /* Free the existing PDU and cpy the SDU to PDU */
         if ( txEnt->pdu != txEnt->sdu )
         {
            PJ_FREE_BUF(txEnt->pdu);
         }
         else
         {
            txEnt->pdu = NULLP;
         }
         txEnt->state = PJ_SDU_RECEIVED;
         /* Perform compression and ciphering and deliver to the lower layer */
         ret = pjDlmProcDrb(pjRbCb, txEnt);
         if ( ret != ROK )
         {
            PJLOGERROR(ERRCLS_ADD_RES, EKW057, (ErrVal)txEnt->count,
               "pjDlmReEstDrbAm: pjDlmProcDrb Failed \n");
         }
      }
      startCount++;
   } 
#ifdef MSPD
   MSPD_DBG("[%d][%d] Reestablishment done at pdcp\n", pjRbCb->ueCb->key.ueId,
   	  pjRbCb->rbId);
#endif
   RETVALUE(ret);
}

/**
 *
 * @brief 
 *
 *        Handler to send data confirmation to RRC.
 *
 * @b Description: 
 *
 *        1. This function is called when a status indication or a confirm is 
 *        received from the lower layer.  @n
 *        2. The confirm type can be a success or failure. @n 
 *        3. The corresponding SDUs and PDUs are cleaned up and
 *        if cfmReqd is configured, a confirm is sent to the upper layer. @n
 *            
 *  @param[in] pjRbCb    PDCP control block. 
 *  @param[in] staInd  Status Indication Information.
 *  @param[in] cfmType Confirm Type can be SUCCESS or FAILURE.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlmProcessCfm
(
PjRbCb        *pjRbCb,
PjDatCfm      *pjDatCfm,
U8            cfmType
)
#else
PUBLIC S16 pjDlmProcessCfm(pjRbCb,pjDatCfm,cfmType)
PjRbCb        *pjRbCb;
PjDatCfm      *pjDatCfm;
U8            cfmType;
#endif
{
   PjuDatCfmInfo  *datCfm;        /* Data Cfm to be sent to upper layer */
   U32            cnt;            /* Counter for number of SDUs in datCfm*/
   U32            datCfmCnt;      /* Counter for number of SDUs in datCfm*/
   KwPjuSapCb     *pjuSap;        /* Upper Sap of PDCP User */
   CmLtePdcpId    *pdcpId;        /* PDCP ID */
   PjDlCb         *dlCb;           /* Downlink control block */
   PjTxEnt        *txEnt;          /* Transmission entity */

   TRC3(pjDlmProcessCfm)

   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmProcessCfm(pjRbCb(%d), pjDatCfm, cfmType (%d)) \n",  pjRbCb->rbId, cfmType));
 
   dlCb     = &pjRbCb->dlCb;
   pdcpId   = NULLP;
   cnt      = 0;
   pjuSap   = NULLP;
   datCfm   = NULLP;
   txEnt    = NULLP;

   /* If cfmReqd flag is sent a confirmation is sent to the above layer.
    * For each SDU, get the sduId and status and fill in the datCfm
    * structure and discard the txEnt. Also update cfmExp count.
    */
   /* RLC_DL_MAX_RETX fix */
   if ( (pjRbCb->dlCb.cfmReqd) || (PJ_CFM_NOK == cfmType)) 
   {
      PJ_ALLOC(datCfm, sizeof(PjuDatCfmInfo));
      PJ_ALLOC(pdcpId, sizeof(CmLtePdcpId));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (datCfm == NULLP || pdcpId == NULLP)
      {
         PJ_FREE(datCfm, sizeof(PjuDatCfmInfo));
         PJ_FREE(pdcpId, sizeof(CmLtePdcpId));
         PJLOGERROR(ERRCLS_ADD_RES, EKW058, (ErrVal) RFAILED,
            "pjDlmProcessCfm: PJ_ALLOC Failed.");
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      pdcpId->rbId   = pjRbCb->rbId;
      pdcpId->rbType = pjRbCb->rbType;
      pdcpId->ueId   = pjRbCb->ueCb->key.ueId;
      pdcpId->cellId = pjRbCb->ueCb->key.cellId;

      if (pjRbCb->rbType == PJ_SRB)
         pjuSap = &(kwCb.pjuSap[PJ_SRB_SAP]);
      else
      {
         pjuSap = &(kwCb.pjuSap[PJ_DRB_SAP]);
         if (PJ_CFM_NOK == cfmType)
         {
            MSPD_DBG_RLC("Received Negative Confirm\n");
         }
      }

      datCfmCnt = 0;
      for ( cnt = 0; cnt < pjDatCfm->numSdu; cnt++)
      {
         txEnt = (PjTxEnt *)pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), 
                                       pjDatCfm->sduId[cnt]);
         if ( txEnt != NULLP )
         {
            datCfm->cfmSta[datCfmCnt].sduId  = txEnt->sduId;
            /* RLC_DL_MAX_RETX fix */
            if (PJ_CFM_OK != cfmType)
            {
               datCfm->cfmSta[datCfmCnt].status = PJU_RLC_TRANS_FAIL;
            }
            else
            {
               datCfm->cfmSta[datCfmCnt].status = cfmType;
            }
            datCfmCnt ++;
            pjDbmDelTxEnt(&(pjRbCb->dlCb.txBuf), txEnt->count);
         }
      }
      datCfm->numSdus = datCfmCnt;
      if(datCfm->numSdus)
      {
         /* If trace flag is enabled send the trace indication */
         if(kwCb.init.trc == TRUE)
         {
            /* Populate the trace params */
            pjLmmSendTrc(EVTPJUDATCFM, NULLP);
         }

         PjUiPjuDatCfm(&pjuSap->pst, pjuSap->suId, pdcpId, datCfm);
      }
      else
      {
         PJ_FREE(datCfm, sizeof(PjuDatCfmInfo));
         PJ_FREE(pdcpId, sizeof(CmLtePdcpId));
      }
   }
   /* If cfmReqd is not set, discard the txEnts and update the cfmExp count */
   else
   {
      for ( cnt = 0; cnt < pjDatCfm->numSdu; cnt++)
      {
         pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), pjDatCfm->sduId[cnt]);
      }
   
   }
   PJ_UPD_CFM_EXP(dlCb);

   if ( pjRbCb->dlCb.txBuf.numEntries == 0 )
   {
      if ((pjChkTmr((PTR)pjRbCb, PJ_EVT_DISC_TMR)) == TRUE)
      {
         pjStopTmr( (PTR)pjRbCb, PJ_EVT_DISC_TMR);
      }

   }
   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to send process the status report.
 *
 * @b Description: 
 *
 *        1. This function is called when a status report is received from the
 *        peer.    @n
 *        2. This function process the status report and sends a confirmation
 *        to the above layer and removes the PDUs and SDUs for which positive 
 *        acknowledgement is received.  @n
 *            
 *  @param[in] pjRbCb      PDCP control block. 
 *  @param[in] staPdu      Status report.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmHndlStaRep
(
PjRbCb   *pjRbCb,
PjSn     fms,
Buffer   *staPdu
)
#else
PUBLIC S16 pjDlmHndlStaRep(pjRbCb,fms,staPdu)
PjRbCb   *pjRbCb;
PjSn     fms;
Buffer   *staPdu;
#endif
{
   U8            byte;          /* One Byte of a Status Pdu */
   PjuDatCfmInfo *datCfm;       /* Dat Cfm to be sent to the upper layer */
   S16           ret;           /* Return Value */
   U8            cnt;           /* Counter for the bits in a byte */
   U32           bitPos;        /* Bit position in the Status PDU */
   MsgLen        bMapLen;       /* StaPdu Length in bytes */
   KwPjuSapCb    *pjuSap;       /* PJU SAP Control Block */
   PjSn          sn;            /* SN of the PDU */
   PjTxEnt       *txEnt;        /* Transmission entity */
   CmLtePdcpId   *pdcpId;       /* PDCP ID */
   U8            sduSta;        /* SDU Status of reception at the Peer */

   TRC3(pjDlmHndlStaRep)

   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
            (_kwPBuf, "pjDlmHndlStaRep(pjRbCb (%d), fms (%u), staPdu) \n",  pjRbCb->rbId, fms));

   datCfm  = NULLP;
   byte    = 0;
   bMapLen = 0;
   ret     = ROK;
   cnt     = 0;
   bitPos  = 1;
   pjuSap  = NULLP;
   pdcpId  = NULLP;

   SFndLenMsg(staPdu, &bMapLen);
   
   /* Allocate for datCfm structure is cfmReqd is set */
   if ( pjRbCb->dlCb.cfmReqd )
   {
      PJ_ALLOC(datCfm, sizeof(PjuDatCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (datCfm == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW059, (ErrVal)0,
             "pjDlmHndlStaRep: Allocation failed for datCfm \n");
         PJ_FREE_BUF(staPdu);
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_RES */
   }
   /* Fills the confirm information for all the PDUs whose COUNT < COUNT(FMS) 
    * and sets cfmExp to fms
    */
   PJ_FILL_CFM_OK(pjRbCb, fms, datCfm);

   /* Read one byte at a time from Status PDU */
   while ( bMapLen != 0 )
   {
      ret = SRemPreMsg(&byte,staPdu);
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (ret != ROK)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW060, (ErrVal)0,
            "pjDlmHndlStaRep: SRemPreMsg Failed \n");
         PJ_FREE_BUF(staPdu);
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_RES */
      /* Check if each bit in a byte is set or not.
       * If cfmReqd is set fill in the cfm info.
       * Discard the txEnt if the bit is set to 1.
       */
      for ( cnt = 0; cnt < PJ_BYTE_LEN; cnt++, bitPos++ )
      {
         sn      = (fms + bitPos) % PJ_TX_BUF_LEN;
         txEnt   = (PjTxEnt *)pjDbmGetTxEntSn( &(pjRbCb->dlCb.txBuf), sn );
         sduSta  = (byte << cnt) & PJ_FIRST_BIT;
         if ( txEnt != NULLP )
         {
            if ( sduSta )
            {
               if ( pjRbCb->dlCb.cfmReqd )
               {
                  datCfm->cfmSta[datCfm->numSdus].sduId  = txEnt->sduId;
                  datCfm->cfmSta[datCfm->numSdus].status = sduSta;
                  datCfm->numSdus++;;
               }
               pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), txEnt->count); 
            }
         }
      }
      bMapLen--;
   }
   /* Send a confirmation to the upper layer */
   if ( pjRbCb->dlCb.cfmReqd )
   {
      PJ_ALLOC(pdcpId, sizeof(CmLtePdcpId));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(pdcpId   == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW061, (ErrVal) RFAILED,
               "pjDlmHndlStaRep: PJ_ALLOC Failed.");
         RETVALUE(ROK);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

      pdcpId->rbId   = pjRbCb->rbId;
      pdcpId->rbType = pjRbCb->rbType;
      pdcpId->ueId   = pjRbCb->ueCb->key.ueId;
      pdcpId->cellId = pjRbCb->ueCb->key.cellId;

      pjuSap   =  &kwCb.pjuSap[PJ_DRB_SAP];
      /* If trace flag is enabled send the trace indication */
      if(kwCb.init.trc == TRUE)
      {
         /* Populate the trace params */
         pjLmmSendTrc(EVTPJUDATCFM, NULLP);
      }
      if ( datCfm-> numSdus > 0 )
      { 
         PjUiPjuDatCfm(&pjuSap->pst, pjuSap->suId, pdcpId, datCfm);
      }
      else
      {
         PJ_FREE(datCfm, sizeof(PjuDatCfmInfo));
         PJ_FREE(pdcpId, sizeof(CmLtePdcpId));
      }
   }

   PJ_FREE_BUF(staPdu);
   RETVALUE(ret); 
}


/**
 *
 * @brief 
 *
 *        Handler to forward the downlink data to the upper layer.
 *
 * @b Description: 
 *
 *        1. This function is used to send the downlink data to the upper layer
 *        during handover.  @n
 *        2. The unacknowledged data SDUs are sent to the upper
 *        layer.    @n
 *            
 *  @param[in] pjRbCb      PDCP control block. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmReEstHoDrbAm
(
PjRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmReEstHoDrbAm(pjRbCb)
PjRbCb  *pjRbCb;
#endif
{

   U32              count;            /* COUNT of the first txEnt */
   U32              curCount;         /* COUNT of the lastest txEnt */
   U32              numSdu;           /* Number of SDUs sent to upper layer */
   U32              sduCount;         /* Counter for SDUs */
   PjTxEnt          *txEnt;           /* Transmission Entity */
   PjuDatFwdIndInfo *datFwdInd;       /* DatFwd Indication Info */
   /* kw006.201, ccpu00120574, fixed the warning */

   TRC3(pjDlmReEstHoDrbAm)
   
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
                        (_kwPBuf, "pjDlmReEstHoDrbAm(pjRbCb (%d )) \n", pjRbCb->rbId));

   numSdu      = 0;
   count       = 0;
   curCount     = 0;
   sduCount    = 0;
   txEnt       = NULLP;
   /* kw006.201, ccpu00120574, fixed the warning */
   datFwdInd   = NULLP;
   /* kw006.201, ccpu00120425 sending SN info to application when txSN is 0 */

   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].pres = TRUE;
   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].rbId = pjRbCb->rbId;
   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].dir |= PJ_DIR_DL;
   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].dl.count = pjRbCb->dlCb.count + 1;

   /* Find the total count of the txEnts present */
   numSdu = pjRbCb->dlCb.txBuf.numEntries;

   PJ_ALLOC(datFwdInd, sizeof(PjuDatFwdIndInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (datFwdInd == NULLP)
   {
      /* Free memory for DatFwdInfo */
      PJ_FREE(datFwdInd, sizeof(PjuDatFwdIndInfo));

      PJLOGERROR(ERRCLS_ADD_RES, EKW062, (ErrVal) RFAILED,
            "pjDlmReEstHoDrbAm: PJ_ALLOC Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   /*========================================================*/
   /* sri - temporary fix. to be removed after data forwarding fix */
    numSdu = 0;
   /*========================================================*/

   /* kw005.201 sending DatFwdInd even if numSdu is zero */
   if (numSdu == 0)
   {
      PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
            (_kwPBuf, "pjDlmReEstHoDrbAm: No data available to forward"));
      MSPD_ERR("No data available to forward\n");
      datFwdInd->dir = PJ_DIR_DL;
      datFwdInd->numSdus = numSdu;
      pjUtlSndDatFwdInd(pjRbCb, datFwdInd);
      RETVALUE(ROK);
   }


   PJ_ALLOC( datFwdInd->datFwdInfo, (numSdu * sizeof(PjuDatFwdInfo)) );
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (datFwdInd->datFwdInfo == NULLP)
   {
      /* Free memory for DatFwdInfo */
      PJ_FREE(datFwdInd->datFwdInfo,  numSdu * sizeof(PjuDatFwdInfo));

      PJLOGERROR(ERRCLS_ADD_RES, EKW063, (ErrVal) RFAILED,
            "pjDlmReEstHoDrbAm: PJ_ALLOC Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   curCount = pjRbCb->dlCb.count;
   count    = pjRbCb->dlCb.cfmExp;

   datFwdInd->dir = PJ_DIR_DL;
   datFwdInd->numSdus = numSdu;

   /*Fill in the SDU, sduId and SN for all of the txEnts present */
   while ( count <= curCount )
   {
      txEnt = (PjTxEnt *)pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
      if ( txEnt != NULLP )
      {
         datFwdInd->datFwdInfo[sduCount].sduId = txEnt->sduId;
         datFwdInd->datFwdInfo[sduCount].sn    = txEnt->sn;
         SCpyMsgMsg(txEnt->sdu, 0, 0,
                  &datFwdInd->datFwdInfo[sduCount].sdu);
         sduCount++;
      }   
      count++;
   } 
  
   /* kw005.201 sending DatFwdInd even if numSdu is zero */
   pjUtlSndDatFwdInd(pjRbCb, datFwdInd);

   RETVALUE(ROK);

}

/**
 *
 * @brief 
 *
 *        Handler to process the forwaded data received from upper layer.
 *
 * @b Description: 
 *
 *        1. This function is used to process the SDUs received from the upper
 *        layer as part of handover.   @n
 *        2. This function calls pjDlmProcessSdus function with the correct 
 *        SN and HFN values.     @n
 *            
 *  @param[in] pjRbCb      PDCP control block. 
 *  @param[in] datFwdReq Data Forward Info.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlmHndlDatFwdReq
(
PjRbCb  *pjRbCb,
PjuDatFwdReqInfo *datFwdReq
)
#else
PUBLIC S16 pjDlmHndlDatFwdReq(pjRbCb,datFwdReq)
PjRbCb  *pjRbCb;
PjuDatFwdReqInfo *datFwdReq;
#endif
{

   U32      sduCount;            /* Number of received SDUs */
   U32      numSdus;             /* Counter for SDUs */
   /* kw006.201, ccpu00120574, fixed the warning */
   U32      txHfn;               /* TX_HFN of a SDU */
   U32      discInt;             /* Discard Timer Interval of a RB */
   S16      ret;                 /* Return Value */
   U32      count;               /* COUNT value of the SDU */
   PjTxEnt  *txEnt;              /* Transmission Entity */ 
   Bool     tmrRunning;          /* Timer Flag */

   TRC3(pjDlmHndlDatFwdReq)

   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
      (_kwPBuf, "pjDlmHndlDatFwdReq(pjRbCb (%d),datFwdReq(%d)) \n", 
      pjRbCb->rbId, datFwdReq->numSdus));

   sduCount = datFwdReq->numSdus;
   /* kw006.201, ccpu00120574, fixed the warning */
   txHfn    = pjRbCb->dlCb.txHfn;
   ret      = ROK;
   numSdus  = 0;
   count    = 0;
   tmrRunning = FALSE;
   discInt = pjRbCb->dlCb.discTmrInt; 
   
   if((pjRbCb->dlCb.nxtTxSn == 0) && ( pjRbCb->dlCb.txHfn == 0))
   {
      pjRbCb->dlCb.nxtTxSn =  datFwdReq->datFwdInfo[sduCount - 1].sn;
      pjRbCb->dlCb.count   =  datFwdReq->datFwdInfo[sduCount - 1].sn;
   }

   if ( sduCount > 0 )
   {
      PjSn firstSn = datFwdReq->datFwdInfo[numSdus].sn;
      U32  hfn = (firstSn > pjRbCb->dlCb.nxtTxSn ? txHfn - 1: txHfn);  
      PJ_UPD_COUNT(pjRbCb, firstSn, hfn, count);
      pjRbCb->dlCb.cfmExp   = count;
      pjRbCb->dlCb.nxtToSub = pjRbCb->dlCb.cfmExp;
   }
   kwCb.pjGenSts.numPktsRcvd += sduCount;
   /* Process each of the SDUs with received SN and sduId */ 
   for ( numSdus = 0; numSdus < sduCount; numSdus++ )
   {
      /* 
         The variables nxtTxSn and COUNT are assumed to be 
         already updated in dlCb to continue with the 
         transmission ( in the target eNodeB ).
      */
      PJ_ALLOC(txEnt, sizeof(PjTxEnt)); 

#if (ERRCLASS & ERRCLS_ADD_RES)
      if (txEnt == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW064, (ErrVal)0,
            "pjDlmProcessSdus: Allocation failed for txEnt \n");
         PJ_FREE_BUF(datFwdReq->datFwdInfo[numSdus].sdu);
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_RES */


      /* Fill TxEnt values */
      txEnt->sn = datFwdReq->datFwdInfo[numSdus].sn;
      txEnt->sduId = datFwdReq->datFwdInfo[numSdus].sduId;
      txEnt->state = PJ_SDU_RECEIVED;
      txEnt->sdu   = datFwdReq->datFwdInfo[numSdus].sdu;
      txEnt->pdu   = NULLP;

      if ( txEnt->sn  > pjRbCb->dlCb.nxtTxSn )
      {
         txHfn = pjRbCb->dlCb.txHfn -1;
      }
      else
      {
         txHfn = pjRbCb->dlCb.txHfn;
      }   

      PJ_UPD_COUNT(pjRbCb, txEnt->sn, txHfn, txEnt->count);
      pjDbmInsTxEnt(&(pjRbCb->dlCb.txBuf), txEnt);
   }
   count = pjRbCb->dlCb.cfmExp;
 
   while ( count <= (pjRbCb->dlCb.count) )
   {
      txEnt = (PjTxEnt *)pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
      if ((txEnt != NULLP) && (txEnt->state == PJ_SDU_RECEIVED))
      {
         if ( discInt )
         {
            SGetSysTime(&txEnt->arrTime);
            if (pjRbCb->dlCb.discTmr.tmrEvnt == PJ_EVT_DISC_TMR)
            {
               tmrRunning = TRUE;
            }

            if ( tmrRunning == FALSE && pjRbCb->dlCb.discTmrInt != -1  )
            {
               pjStartTmr((PTR)pjRbCb, PJ_EVT_DISC_TMR);
            }
         }
         ret = pjDlmProcDrb(pjRbCb,txEnt);
         if ( ret != ROK )
         {
            PJLOGERROR(ERRCLS_ADD_RES, EKW065, (ErrVal)0,
               "pjDlmHndlDatFwdReq: pjDlmProcDrb Failed \n");
            count++;
            continue;
         }
      }
      count++;
   } 

   RETVALUE(ret);
}

/**
 *
 * @brief 
 *
 *        Handler to construct a PDU and send it to the lower layer.
 *
 * @b Description: 
 *
 *        1. This function is used to deliver a PDU to the lower layer.@n
 *        2. It calls pjDlmBldPdu function and sends the PDU to 
 *        the lower layer using kwUimDatReq function. @n
 *        3. The txBuf is freed up if the cfmReqd is not configured or 
 *        if the rbType is UM.@n
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] txEnt    Transmission Entity.
 *  @param[in] mBuf     The PDU to be transmitted.
 *  @param[in] pduType  Type of the PDU.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef MSPD
extern U32 macGT;
#endif

#ifdef ANSI
PUBLIC S16 pjDlmDeliverPdu
(
PjRbCb     *pjRbCb,
PjTxEnt    *txEnt
)
#else
PUBLIC S16 pjDlmDeliverPdu(pjRbCb,txEnt)
PjRbCb     *pjRbCb;
PjTxEnt    *txEnt;
#endif
{
   PjDlCb         *dlCb;           /* Downlink Control Block */
   S16            ret;            /* Return Value */
   U8             rbCfgStat;      /* RB Config Status */
   U32            count;          /* COUNT value */

   TRC3(pjDlmDeliverPdu)

   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
             (_kwPBuf, "pjDlmDeliverPdu(pjRbCb (%d), txEnt) \n", pjRbCb->rbId));

   dlCb   = &pjRbCb->dlCb;
   ret    = ROK;
   count  = 0;

#ifdef MSPD
   if (((macGT > pjRbCb->dataSnt) && (macGT - pjRbCb->dataSnt > 100))
         || ((macGT < pjRbCb->dataSnt) && (10249 - pjRbCb->dataSnt + macGT > 100)))
   {
       MSPD_DBG_RLC("[%d]No data in DL from lastDatSnt %d macGT %d\n", 
                      pjRbCb->ueCb->key.ueId, pjRbCb->dataSnt, macGT);   
   }

   pjRbCb->dataSnt = macGT;
#endif

   if ( txEnt->pdu == NULLP )
   {
      if ( pjRbCb->mode == PJ_DRB_AM 
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
         || pjRbCb->mode == PJ_DRB_UM
#endif
         )
      {
         PJ_CPY_MSG(txEnt->sdu, &(txEnt->pdu), ret);
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (ret != ROK)
         {
            PJLOGERROR(ERRCLS_ADD_RES, EKW066, (ErrVal)txEnt->count,
              "pjDlmDeliverPdu: SAddMsgRef Failed \n");
            PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
            if ( pjRbCb->dlCb.cfmReqd)
            {
               PJ_SND_CFM(pjRbCb, txEnt, PJ_CPY_MSG_FAILED);
            }
            RETVALUE(RFAILED);
         }
#endif
      }
      else
      {
         txEnt->pdu = txEnt->sdu;
         txEnt->sdu = NULLP;
      }
   }
   /* Constructs a PDU */
   if ( txEnt->state != PJ_PDU_CONSTRUCTED )
   {
      ret = pjDlmBldPdu(pjRbCb, txEnt);
   }

   if ( ret != ROK )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW067, (ErrVal)txEnt->count,
         "pjDlmDeliverPdu: pjDlmBldPdu failed for txEnt \n");
      MSPD_ERR("pjDlmBldPdu failed for txEnt\n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(pjRbCb, txEnt, PJ_PDU_CONSTRUCT_FAILED);
      }
      RETVALUE(RFAILED);
   }
   txEnt->state = PJ_PDU_CONSTRUCTED;

   /* Check if the existence of the corresponding RLC entity and then only
      send the SDU to the lower layer. */
   if ( pjRbCb->rbType == CM_LTE_SRB )
   {
      rbCfgStat = pjRbCb->ueCb->srbCb[pjRbCb->rbId]->rbCfgStat;
   }
   else
   {
      rbCfgStat = pjRbCb->ueCb->drbCb[pjRbCb->rbId]->rbCfgStat;
   }
   if ( !(rbCfgStat & PJ_CFG_RLC) )
   {
      MSPD_ERR("RLC entity is not available, so dropping pdu\n");
      RETVALUE(RFAILED);
   }
   /* Deliver the PDU to the lower layer only if it is nxtToSub.
    * Also deliver the consecutive PDUs if they are constructed.
    * If not just store the PDU.
    */
   count = txEnt->count;
   if ( count == dlCb->nxtToSub )
   {
      /* kw002.201 : Window mechanism introduced to take care of wrap around */
	  U32 loopCnt = 0;
	  U32 maxTxEnts;

	  if (count > dlCb->count)
      {
         maxTxEnts = 0xffffffff - count + dlCb->count + 1;
      }
      else
      {
         maxTxEnts = dlCb->count - count + 1;
      }
      while (loopCnt < maxTxEnts)
      {
         /* COUNT is used as an sduId between PDCP and RLC as insertion of
          * txEnt is done based on count and this would help easy fetching 
          * when we receive a confirm 
          */
         if (txEnt != NULLP ) 
         {
            if (txEnt->state == PJ_PDU_CONSTRUCTED )
            {
               kwUimDatReq(pjRbCb, count, txEnt->pdu);

               kwCb.pjGenSts.txPdus++;
               txEnt->pdu   = NULLP;
               txEnt->state = PJ_PDU_SUBMITTED;
/* kw005.201 Added support for L2 Measurement */
               if ( (pjRbCb->mode == PJ_DRB_UM 
#ifdef LTE_L2_MEAS
                     /* Delete only if DL Delay measurement is not running */
                     && !(pjRbCb->rbL2Cb.measOn & LPJ_L2MEAS_DL_DELAY)
                          
#endif /* LTE_L2_MEAS */
                    ) || 
                  (!(pjRbCb->dlCb.cfmReqd) && pjRbCb->rbType == PJ_SRB))
               {
                  pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), txEnt->count);
               }
            }
            else
            {
               break;
            }
         } 
         count++;
		 loopCnt++;
         txEnt = (PjTxEnt *)pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
      }  
      dlCb->nxtToSub = count;
   }
   
   if ( pjRbCb->dlCb.txBuf.numEntries == 0 )
   {
      if ((pjChkTmr((PTR)pjRbCb, PJ_EVT_DISC_TMR)) == TRUE)
      {
         pjStopTmr( (PTR)pjRbCb, PJ_EVT_DISC_TMR);
      }

   }     
   RETVALUE(ret);
}
/**
*
 * @brief  
 * 
 *        Handler to construct a process a SRB SDU.
 * 
 * @b Description: 
 * 
 *        1. This function performs integrity protection if it is
 *        configured.    @n
 *        2. The first DL message of SRBs is not ciphered and is just integrity 
 *        protected.  @n 
 *        3. This function checks the UE CB control block to check the flag for the 
 *        first DL message and performs only integrity protection and unsets 
 *        the flag.       @n 
 *        4. Ciphering is performed if applicable.
 *        5. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */


#ifdef ANSI
PUBLIC S16 pjDlmProcSrb
(
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmProcSrb(pjRbCb, txEnt)
PjRbCb *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   S16             ret;           /* Return Value */
   /* kw006.201, ccpu00120574, fixed the warning */
   U32             macI;          /* MAC-I value to be padded to the PDU */
   Buffer          *pdu;          /* Pointer for the PDU in txEnt */

   TRC3(pjDlmProcSrb)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
      (_kwPBuf, "pjDlmProcSrb(pjRbCb (%d), txEnt(%ld)) \n", 
      pjRbCb->rbId, txEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
      (_kwPBuf, "pjDlmProcSrb(pjRbCb (%d), txEnt(%d)) \n",
      pjRbCb->rbId, txEnt->count));
#endif

   pdu    = txEnt->sdu;
   macI   = 0;
   ret    = ROK;
   /* kw006.201, ccpu00120574, fixed the warning */

   /* Perform security operation only if configured */
   if ( pjRbCb->ueCb->secInfo.secAct )
   {
      /* Perform integrity/ciphering */
      ret = pjDlmHdlIntProt(pjRbCb, txEnt);
      if ( ret != ROK )
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW068, (ErrVal)txEnt->count,
             "pjDlmProcSrb: pjDlmHdlIntProt failed for txEnt \n");
      }
   }
   else
   {
      /* If security is not configured, pad the PDU with 4 bytes 
       * and deliver to the lower layer
       */
      PJ_PACK_MACI(pdu, macI);
      ret = pjDlmDeliverPdu(pjRbCb, txEnt);
   }

   RETVALUE(ret);
}

/**
 *
 * @brief 
 * 
 *        Handler to construct a process a DRB SDU.
 * 
 * @b Description: 
 * 
 *        1. This function performs header compression if it is
 *        configured.    @n
 *        2. If security is configured, ciphering is performed. @n
 *        3. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmProcDrb
(
PjRbCb   *pjRbCb,
PjTxEnt  *txEnt
)
#else
PUBLIC S16 pjDlmProcDrb(pjRbCb,txEnt)
PjRbCb   *pjRbCb;
PjTxEnt  *txEnt;
#endif
{
   S16            ret;         /* Return Value */

   TRC3(pjDlmProcDrb)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmProcDrb(pjRbCb (%d), txEnt(%ld)) \n", pjRbCb->rbId, txEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmProcDrb(pjRbCb (%d), txEnt(%d)) \n", pjRbCb->rbId, txEnt->count));
#endif

   ret     = ROK;

   ret =   pjDlmHdlCmp(pjRbCb, txEnt);

   if ( ret != ROK )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW069, (ErrVal)txEnt->count,
         "pjDlmProcDrb: Compression failed for txEnt \n");
   }
   RETVALUE(ret);

}

/**
 *
 * @brief 
 * 
 *        Handler to process the Integrity Protection Request
 * 
 * @b Description: 
 * 
 *        1. This function performs integrity protection
 *        of a SDU by prepending the header.    @n
 *        2. If ciphering is applicable, it is performed after removing
 *        the header. @n
 *        3. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmHdlIntProt
(
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmHdlIntProt(pjRbCb,txEnt)
PjRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   S16             ret;           /* Return Value */
   U8              hdr;           /* Header Value */
   U32             macI;          /* MAC-I value to be padded to the PDU */
   Buffer          *pdu;          /* Pointer for the PDU in txEnt */
   PjSecInp        secInp;        /* Security Input for Ciphering and Int Prot */

   TRC3(pjDlmHdlIntProt)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
            (_kwPBuf, "pjDlmHdlIntProt(pjRbCb (%d), txEnt (%ld)) \n", 
            pjRbCb->rbId, txEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
            (_kwPBuf, "pjDlmHdlIntProt(pjRbCb (%d), txEnt (%d)) \n",
            pjRbCb->rbId, txEnt->count));
#endif

   pdu    = txEnt->sdu;
   macI   = 0;
   ret    = ROK;
   hdr    = 0;

   /* If firstDL msg after secCfg, unset the ueCb flag ans set
    * rbCb flag and set the SN
    */
   if (pjRbCb->ueCb->secInfo.selSecAct == TRUE &&
            pjRbCb->ueCb->secInfo.firstDl == TRUE )
   {
      pjRbCb->firstDlMsg = TRUE;
      pjRbCb->firstSn    = txEnt->sn;
      pjRbCb->ueCb->secInfo.firstDl = FALSE;
   }
   secInp.dir  = PJ_SEC_DIR_DL;
   secInp.rbId = pjRbCb->rbId;
   secInp.count= txEnt->count;

   /* Add the header and then send it for Integrity Protection */
   hdr = (U8)txEnt->sn;
   ret = SAddPreMsg((Data)hdr, pdu);
   txEnt->sdu = pdu;

#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( ret != ROK )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW070, (ErrVal)txEnt->count,
         "pjDlmHdlIntProt: SAddPreMsg failed for txEnt \n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(pjRbCb, txEnt, PJ_REM_MSG_FAILED);
      }
      RETVALUE(RFAILED);
   }
#endif

   txEnt->state = PJ_WAIT_FOR_INTPROT;
   ret = pjUtlIntProtReq(pjRbCb, secInp, pdu, &macI);

   if ( ret != ROK )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW071, (ErrVal)txEnt->count,
        "pjDlmhdlIntProt: Integrity Protection failed for txEnt \n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(pjRbCb, txEnt, PJU_INTPROT_FAILED);
      }
      kwCb.pjGenSts.numIntgPrtFails++; 
      RETVALUE(RFAILED);
   }
#ifndef PJ_SEC_ASYNC
   /* Append the macI at the end */
#ifdef TENB_AS_SECURITY
   /*PJ_PACK_MACI(pdu, macI);*/
#else
   PJ_PACK_MACI(pdu, macI);
#endif
   txEnt->pdu = pdu;

   /* Remove the header and then only send for ciphering */
   ret = SRemPreMsg(&hdr, txEnt->pdu);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( ret != ROK )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW072, (ErrVal)txEnt->count,
         "pjDlmHdlIntProt: SRemPreMsg for txEnt \n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(pjRbCb, txEnt, PJ_REM_MSG_FAILED);
      }
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */
   /* If rbCb flag is set => firstDL msg. So skip ciphering */
   if ( pjRbCb->firstDlMsg == TRUE  && pjRbCb->firstSn == txEnt->sn)
   {
      pjRbCb->firstDlMsg = FALSE;
      txEnt->sdu = NULLP;
   }
   else
   {
      txEnt->state  = PJ_WAIT_FOR_CIPHER;
      ret = pjDlmHdlCiph(pjRbCb, txEnt);

      if ( ret != ROK )
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW073, (ErrVal)txEnt->count,
            "pjDlmHdlIntProc: Ciphering failed for txEnt \n");
         RETVALUE(RFAILED);
      }
   }
   ret = pjDlmDeliverPdu(pjRbCb, txEnt);
#endif
   RETVALUE(ret);
}

/**
 *
 * @brief  
 * 
 *        Handler to process the Compression Request.
 * 
 * @b Description: 
 * 
 *        1. This function performs header compression if configured. @n
 *        2. If ciphering is applicable, it is performed. @n
 *        3. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */


#ifdef ANSI
PUBLIC S16 pjDlmHdlCmp
(
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmHdlCmp(pjRbCb,txEnt)
PjRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   S16            ret;         /* Return Value */
   Buffer         *opSdu;      /* Pointer for Output of Compression */
   
   TRC3(pjDlmHdlCmp)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmHdlCmp(pjRbCb (%d), txEnt (%ld)) \n", 
           pjRbCb->rbId, txEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmHdlCmp(pjRbCb (%d), txEnt (%d)) \n",
           pjRbCb->rbId, txEnt->count));
#endif

   ret     = ROK;
   opSdu   = NULLP;

   /* Perform compression if ocnfigured */
   if ( pjRbCb->rohc.hdrCmpUsed )
   {
      txEnt->pdu = txEnt->sdu;
      txEnt->state = PJ_WAIT_FOR_CMP;
      ret =   pjUtlCmpReq(pjRbCb, txEnt->pdu, &opSdu, txEnt->count);
      if ( ret != ROK )
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW074, (ErrVal)txEnt->count,
             "pjDlmHdlCmp: Compression failed for txEnt \n");
         PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
         if ( pjRbCb->dlCb.cfmReqd)
         {
            PJ_SND_CFM(pjRbCb, txEnt, PJU_COMP_FAILED);
         }
         kwCb.pjGenSts.numCmpFails++;
         RETVALUE(RFAILED);
      }
#ifdef PJ_CMP_ASYNC
      RETVALUE(ret);
#else
      txEnt->pdu = opSdu;
#endif
   }
   /* Perform ciphering if security is configured */
   if (  pjRbCb->ueCb->secInfo.secAct )
   {
#ifdef TENB_AS_SECURITY
      ret = pjDlmHdlCiph(pjRbCb, txEnt);
      if ( ret != ROK )
      {    
         PJLOGERROR(ERRCLS_ADD_RES, EKW075, (ErrVal)txEnt->count,
            "pjDlmProcSrb: Ciphering failed for txEnt \n");
         RETVALUE(RFAILED);
      }
#ifdef PJ_SEC_ASYNC
      RETVALUE(ret);
#endif
#endif
   }
   ret = pjDlmDeliverPdu(pjRbCb, txEnt);
   RETVALUE(ret);
}


/**
 * 
 * @brief  
 * 
 *        Handler to process the Ciphering Request.
 * 
 * @b Description: 
 * 
 *        1. This function performs ciphering.  @n
 *        2. If asynchronous flag is defined, the function returns . @n
 *        3. Else the function updates the PDU field in txEnt and returns. @n 
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlmHdlCiph
(
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmHdlCiph(pjRbCb,txEnt)
PjRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   PjSecInp       secInp;      /* Security Input for Ciphering */
   S16            ret;         /* Return Value */
   Buffer         *ciphSdu;    /* Pointer for Output of Ciphering */

   TRC3(pjDlmHdlCiph)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmHdlCiph(pjRbCb (%d), txEnt (%ld)) \n", 
           pjRbCb->rbId, txEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmHdlCiph(pjRbCb (%d), txEnt (%d)) \n",
           pjRbCb->rbId, txEnt->count));
#endif
   ret     = ROK;
   ciphSdu = NULLP;

   secInp.dir  = PJ_SEC_DIR_DL;
   secInp.rbId = pjRbCb->rbId;
   secInp.count= txEnt->count;
   txEnt->state  = PJ_WAIT_FOR_CIPHER;

   if ( txEnt->pdu == NULLP )
   {
      txEnt->pdu = txEnt->sdu;
   }
#ifdef TENB_AS_SECURITY
   /*chetan*/
   /*PJ_ALLOC_BUF(ciphSdu);*/
#endif
   ret = pjUtlCipherReq(pjRbCb, secInp, txEnt->pdu, &ciphSdu);

   if ( ret != ROK )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW076, (ErrVal)txEnt->count,
         "pjDlmHdlCiph: Ciphering failed for txEnt \n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(pjRbCb, txEnt, PJU_CIPHER_FAILED);
      }
      kwCb.pjGenSts.numCiphFails++;
      RETVALUE(RFAILED);
   }
#ifdef PJ_SEC_ASYNC
   RETVALUE(ret);
#else
   if ( pjRbCb->rohc.hdrCmpUsed )
   {
      PJ_FREE_BUF(txEnt->pdu);
   }
   txEnt->pdu = ciphSdu;
   RETVALUE(ret);
#endif
}

/**
 *
 * @brief Handler for discard timer expiry.
 *       
 *
 * @b Description
 *        This function is called when the discard timer expires.
 *        This function checks if the txEnts are stored for time 
 *        greater than discInt and cleans all such txEnt buffers.
 *        Starts the timer for the txEnt whose wait time (discInt -
 *        (currentTime - arrivalTime)) is greater than zero.
 *
 *
 *  @param[in] pjRbCb      PDCP control block.
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

#ifdef ANSI
PUBLIC S16 pjDlmDiscTmrExp
(
PjRbCb  *pjRbCb
)
#else
PUBLIC S16 pjDlmDiscTmrExp(pjRbCb)
PjRbCb  *pjRbCb;
#endif
{
   U32           count;         /* Count value of the first valid txEnt */
   U32           curCount;      /* Current DL count in pjRbCb */
   /* kw006.201, ccpu00120574, fixed the warning */
   U32           numBuf;        /* Number of tx Buffers in the RB */
   U32           discInt;       /* Discard Interval */
   Ticks         curTime;       /* Current Time */
   PjTxEnt       *txEnt;        /* Transmission Entity */
   PjuDatCfmInfo *datCfm;       /* DatCfm structure */
   CmLtePdcpId   *pdcpId;       /* PDCP ID */
   U32           numSdus;       /* Number of SDUs discarded */
   KwPjuSapCb    *pjuSap;       /* PJU SAP */
   /* kw006.201, ccpu00120574, fixed the warning */

   TRC3(pjHdlDiscTmrExp)

   PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF | KW_PJ_DBGMASK_UTL),
      (_kwPBuf, "pjDlmDiscTmrExp(pjRbCb (%d)) \n", pjRbCb->rbId));

   count     = pjRbCb->dlCb.count;
   curCount  = pjRbCb->dlCb.count;
   numBuf    = 0;
   txEnt     = NULLP;
   datCfm    = NULLP;
   discInt   = pjRbCb->dlCb.discTmrInt;
   pdcpId    = NULLP;
   numBuf    = pjRbCb->dlCb.txBuf.numEntries;
   numSdus   = 0;
   curTime   = 0;
   /* kw006.201, ccpu00120574, fixed the warning */

   if (numBuf > 0)
   {
      if ( pjRbCb->mode == PJ_DRB_AM || 
           ((pjRbCb->dlCb.cfmReqd) && pjRbCb->rbType == PJ_SRB))
      {
         txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), pjRbCb->dlCb.cfmExp);
      }
      else
      {
         txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), pjRbCb->dlCb.nxtToSub);
      }
      SGetSysTime(&curTime);
      if ( txEnt != NULLP )
      {
         count = txEnt->count;
      }
   }
   else
   {
      RETVALUE(ROK);
   }

   if ( pjRbCb->dlCb.cfmReqd )
   {
      PJ_ALLOC(datCfm, sizeof(PjuDatCfmInfo));
      PJ_ALLOC(pdcpId, sizeof(CmLtePdcpId));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (datCfm == NULLP || pdcpId == NULLP)
      { 
         PJ_FREE(datCfm, sizeof(PjuDatCfm));
         PJ_FREE(pdcpId, sizeof(CmLtePdcpId));
         PJLOGERROR(ERRCLS_ADD_RES, EKW077, (ErrVal) RFAILED,
            "pjDlmDiscTmrExp: PJ_ALLOC Failed.");
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   }
   /* kw006.201, ccpu00120574, fixed the warning */
   while ( count <= curCount )
   {
      if ( txEnt != NULLP )
      {
         if ((txEnt->arrTime + (discInt )) <= curTime)
         {
#ifndef ALIGN_64BIT
            PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF | KW_PJ_DBGMASK_DLM),
               (_kwPBuf, "pjDlmDiscTmrExp(pjRbCb (%d)): discarded txEnt with\
               count (%ld) \n", pjRbCb->rbId, txEnt->count));
#else
            PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF | KW_PJ_DBGMASK_DLM),
               (_kwPBuf, "pjDlmDiscTmrExp(pjRbCb (%d)): discarded txEnt with\
               count (%d) \n", pjRbCb->rbId, txEnt->count));
#endif
            if ( txEnt->state == PJ_PDU_SUBMITTED )
            {
               kwUimDiscSdu(pjRbCb, count);
            }
            if ( pjRbCb->dlCb.cfmReqd)
            {
               datCfm->cfmSta[numSdus].sduId  = txEnt->sduId;
               datCfm->cfmSta[numSdus++].status = PJU_DISCARD_TMR_EXP;
            }
            numBuf--;
            if ( txEnt->pdu == txEnt->sdu )
            {
               txEnt->sdu = NULLP;
            }
            pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), txEnt->count);
            kwCb.pjGenSts.numSdusDisc++;
         }
         else
         {
            break;
         }
      }
      count++;
      txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
   }

   if ( pjRbCb->mode == PJ_DRB_AM || 
        ((pjRbCb->dlCb.cfmReqd) && pjRbCb->rbType == PJ_SRB))
   {
      pjRbCb->dlCb.cfmExp = count;
   }
   if ( pjRbCb->dlCb.nxtToSub < count )
   {
      pjRbCb->dlCb.nxtToSub = count;
      txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
      if ( txEnt && txEnt->state == PJ_PDU_CONSTRUCTED )
      {
          /* kw006.201, ccpu00120574, fixed the warning */
          pjDlmDeliverPdu(pjRbCb, txEnt);
      } 
   }

   if ( numBuf > 0)
   {
      pjStartTmr( (PTR)pjRbCb, PJ_EVT_DISC_TMR);
   }
   else
   {
      pjRbCb->dlCb.discTmr.tmrEvnt = TMR_NONE;
   }

   if ( pjRbCb->dlCb.cfmReqd)
   {
      datCfm->numSdus = numSdus;

      pdcpId->rbId   = pjRbCb->rbId;
      pdcpId->rbType = pjRbCb->rbType;
      pdcpId->ueId   = pjRbCb->ueCb->key.ueId;
      pdcpId->cellId = pjRbCb->ueCb->key.cellId;     

      if (pjRbCb->rbType == PJ_SRB)
         pjuSap = &(kwCb.pjuSap[PJ_SRB_SAP]);
      else
         pjuSap = &(kwCb.pjuSap[PJ_DRB_SAP]);

      /* If trace flag is enabled send the trace indication */
      if(kwCb.init.trc == TRUE)
      {
         /* Populate the trace params */
         pjLmmSendTrc(EVTPJUDATCFM, NULLP);
      }

      PjUiPjuDatCfm(&pjuSap->pst, pjuSap->suId, pdcpId, datCfm);
   }
   RETVALUE(ROK);
}

#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
/**
 *
 * @brief Handler for  downlink off-board timer expiry.
 *       
 *
 * @b Description
 *        This function is called when the off-board timer expires.
 *        This function discards the txEnt of dlCb.obdCount if it is 
 *        is not submitted and sends the constructed PDUs to the lower
 *        layer and start the timer for the next valid txEnt.
 *
 *  @param[in] pjRbCb    PDCP control block.
 *
 * 
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 pjDlmObdTmrExp
(
PjRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmObdTmrExp(pjRbCb)
PjRbCb  *pjRbCb;
#endif
{
   U32           count;
   U32           curCount;
   PjTxEnt       *txEnt;
   PjuDatCfmInfo *datCfm;
   S16           ret;

   TRC3(pjDlmObdTmrExp)
   
   PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
           (_kwPBuf, "pjDlmObdTmrExp(pjRbCb(%d)) \n", pjRbCb->rbId));

   count      = pjRbCb->dlCb.obdCount;
   curCount   = pjRbCb->dlCb.count;
   txEnt      = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
   datCfm     = NULLP;
   ret        = ROK;

   if ( txEnt && txEnt->state != PJ_PDU_SUBMITTED )
   {
#ifndef ALIGN_64BIT
      PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
         (_kwPBuf, "pjDlmObdTmrExp(pjRbCb(%d)) : discarding txEnt with \
         count(%ld)\n", pjRbCb->rbId, txEnt->count));
#else
      PJDBGP((KW_PJ_DBGMASK_DLM | KW_DBGMASK_BRIEF | KW_DBGMASK_DL),
         (_kwPBuf, "pjDlmObdTmrExp(pjRbCb(%d)) : discarding txEnt with \
         count(%d)\n", pjRbCb->rbId, txEnt->count));
#endif
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      PJ_SND_CFM(pjRbCb, txEnt, PJ_OBD_TIMEOUT);
      kwCb.pjGenSts.numSdusDiscObdTmrExp++;
      count++;
      while ( count <= curCount )
      {
         txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
         if ( txEnt != NULLP )
         {
            if ( txEnt->state == PJ_PDU_CONSTRUCTED )
            {
               ret = pjDlmDeliverPdu(pjRbCb, txEnt);
               count = pjRbCb->dlCb.nxtToSub;
            }
            break;
         }
         else
         {
            PJ_UPD_DL_VAR(pjRbCb, count);
         }
         count++;
      }
   }
   else
   {
      count = pjRbCb->dlCb.nxtToSub;
   }
   if ( pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count) != NULLP )
   {
      pjRbCb->dlCb.obdCount = count;
      pjStartTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   RETVALUE(ret);
}

#endif

#ifdef ANSI
PUBLIC Void pjDlmAddRbToReestLst
(
PjRbCb                  *pjRbCb
)
#else
PUBLIC Void pjDlmAddRbToReestLst(pjRbCb)
PjRbCb                  *pjRbCb;
#endif
{
#ifdef MSPD
   U32 coreId = MxGetCpuID();
#endif

//   MSPD_DBG("Sudhanshu pjSelfPstReestLock taking locks %d\n ", coreId);

   KW_RDWR_LOCK(&kwCb.pjSelfPstReestLock);
   {
      if (pjRbCb->reestRbLnk.node == NULLP)
      {
         cmLListAdd2Tail(&kwCb.pjDrbReestLst, &pjRbCb->reestRbLnk);
         pjRbCb->reestRbLnk.node = (PTR)pjRbCb;
      }
   }
   KW_RDWR_UNLOCK(&kwCb.pjSelfPstReestLock);
//   MSPD_DBG("Sudhanshu pjSelfPstReestLock Releasing locks\n ");
   RETVOID;   
}

#ifdef ANSI
PUBLIC Void pjDlmGetRbFrmReestLst
(
PjRbCb                  **pjRbCb
)
#else
PUBLIC Void pjDlmGetRbFrmReestLst(pjRbCb)
PjRbCb                  **pjRbCb;
#endif
{
#ifdef MSPD
     U32 coreId = MxGetCpuID();
#endif

//   MSPD_DBG("Sudhanshu pjSelfPstReestLock taking locks %d\n ", coreId);

   KW_RDWR_LOCK(&kwCb.pjSelfPstReestLock);
   {
      if (kwCb.pjDrbReestLst.first == NULLP)
      {
         *pjRbCb = NULLP;
      }
      else
      {
         (*pjRbCb) = (PjRbCb *)(kwCb.pjDrbReestLst.first->node);
         kwCb.pjDrbReestLst.first->node = NULLP;
         cmLListDelFrm(&kwCb.pjDrbReestLst, kwCb.pjDrbReestLst.first);
      }
   }
   KW_RDWR_UNLOCK(&kwCb.pjSelfPstReestLock);
//MSPD_DBG("Sudhanshu pjSelfPstReestLock Releasing locks\n ");   

}
#ifdef ANSI
PUBLIC Void pjDlmRemRbFrmReestLst
(
PjRbCb                  *pjRbCb
)
#else
PUBLIC Void pjDlmRemRbFrmReestLst(pjRbCb)
PjRbCb                  *pjRbCb;
#endif
{   
#ifdef MSPD
   U32 coreId = MxGetCpuID();
#endif
   //MSPD_DBG("pjSelfPstReestLock taking locks %d\n ", coreId);
   KW_RDWR_LOCK(&kwCb.pjSelfPstReestLock);
   if(pjRbCb->reestRbLnk.node != NULLP)
   {
      cmLListDelFrm(&kwCb.pjDrbReestLst, &pjRbCb->reestRbLnk);
      pjRbCb->reestRbLnk.node = NULLP;
   }
   KW_RDWR_UNLOCK(&kwCb.pjSelfPstReestLock);
   //MSPD_DBG("pjSelfPstReestLock Releasing locks\n ");   
   RETVOID;
}

#ifdef ANSI
PUBLIC Void pjDlmProcessSelfMsg
(
void
)
#else
PUBLIC Void pjDlmProcessSelfMsg(Void)
#endif
{
   PjRbCb   *pjRbCb;
   U16      numRb = 0;   
   pjDlmGetRbFrmReestLst(&pjRbCb);
   if(pjRbCb != NULLP)
   {
     pjDlmReEstDrbAm(pjRbCb);
     pjRbCb->state = PJ_STATE_NORMAL;
   }
   else
   {
      RETVOID;   
   }   
   	
   pjDlmGetNumRbInReestLst(&numRb);
   if(numRb > 0)
   {
      MSPD_DBG("numRb %d\n", numRb);   
      pjDlmPostSelfMsg();
   }	 
  
   RETVOID;
}

#ifdef ANSI
PUBLIC Void pjDlmGetNumRbInReestLst
(
U16                     *numRb
)
#else
PUBLIC Void pjDlmGetNumRbInReestLst(numRb)
U16                     *numRb
#endif
{
#ifdef MSPD
   U32 coreId = MxGetCpuID();
#endif

//   MSPD_DBG("Sudhanshu pjSelfPstReestLock taking locks %d\n ", coreId);
   KW_RDWR_LOCK(&kwCb.pjSelfPstReestLock);
   {
      *numRb = cmLListLen(&(kwCb.pjDrbReestLst));
   }
   KW_RDWR_UNLOCK(&kwCb.pjSelfPstReestLock);
//   MSPD_DBG("Sudhanshu pjSelfPstReestLock Releasing locks\n ");
}

#ifdef ANSI
PUBLIC Void pjDlmPostSelfMsg
(
Void
)
#else
PUBLIC Void pjDlmPostSelfMsg(Void)
#endif
{
#ifdef MSPD
   U32 coreId = MxGetCpuID();
#endif
//   MSPD_DBG("Sudhanshu pjSelfPstDlLock takeing locks %d\n ", coreId);
   KW_RDWR_LOCK(&kwCb.pjSelfPstDlLock);
   if(SPstTsk(&pjSelfPstDl, kwCb.pjMBufDl) != ROK)
   {
   
   }
   KW_RDWR_UNLOCK(&kwCb.pjSelfPstDlLock);
//   MSPD_DBG("Sudhanshu pjSelfPstDlLock Releasing locks\n ");   
   RETVOID;
}

/*@}*/
/********************************************************************30**
 *          End of file:     gp_pj_dlm.c@@/main/1 - Fri Nov 13 14:13:44 2009
 **********************************************************************31*/

/********************************************************************40**
 *   
 *           Notes:
 *             
 *********************************************************************41*/

/********************************************************************50**
 *
 * *********************************************************************51*/

/********************************************************************60**
 *   
 *           Revision history:
 *             
 **********************************************************************61*/

/********************************************************************90**
 * ver       pat    init                  description
 * ------------ -------- ---- ----------------------------------------------
 * /main/1   ---       vp   1. LTERLC Release 2.1
/main/2      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/3      kw004.201 av   1.Adding of Missing Trace in LTE RLC PDCP
/main/4      kw005.201 ap  1. Moved definition of KW_PJ_ASYNC_WIN to
                              kw.h file
                           2. Added support for L2 Measurement.
                           3. Sending Data forward indication when numSdu=0
                           4. ccpu00120574, fixed the warning.
/main/5      kw006.201 ap  1. ccpu00120425 sending SN info to application when txSN is 0.
                           2. ccpu00120574, fixed the warning.
*********************************************************************91*/

