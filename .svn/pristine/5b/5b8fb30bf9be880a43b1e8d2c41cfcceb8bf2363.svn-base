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
  
     Desc:     Source code for PDCP Uplink module.
               This file contains following functions
                  -- pjUlmHdlSrbPkt
                  -- pjUlmHdlDrbPkt
                  -- pjUlmProcessSrb
                  -- pjUlmProcessDrbUm
                  -- pjUlmProcessDrbAm
                  -- pjUlmDeliverSrb
                  -- pjUlmDeliverDrbUm
                  -- pjUlmDeliverDrbAm
                  -- pjUlmReEstSrb
                  -- pjUlmReEstDrbAm
                  -- pjUlmBldStaRep
                  -- pjUlmHdlDatFwdReq
                  -- pjUlmHdlObdTmrExpiry

     File:     gp_pj_ulm.c

     Sid:      gp_pj_ulm.c@@/main/1 - Fri Nov 13 14:13:50 2009

     Prg:      mn 

**********************************************************************/
/** @file gp_pj_ulm.c
@brief PDCP Uplink module
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
 

/* local defines */

/* local externs */

/* forward references */


/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */

/** @addtogroup uldata */
/*@{*/

/*****************************************************************************
 *    HEADER PARSING FUNCTIONS
 ****************************************************************************/

/**
 *
 * @brief 
 *
 *        Function to extract the SRB header.
 *
 * @b Description: 
 *
 *        This function extracts the SN and the MAC-I from the pdu buffer and
 *        places them in the SrbHdr structure.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] pdu      PDU to be processed
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 pjUlmHdlSrbPkt
(
PjRbCb *pjRbCb,               /* !< PDCP Control Block */  
Buffer *pdu                   /* !< PDU Buffer */ 
)
#else
PUBLIC S16 pjUlmHdlSrbPkt(pjRbCb, pdu)
PjRbCb  *pjRbCb;               /* !< PDCP Control Block */      
Buffer  *pdu;                  /* !< PDU Buffer */ 
#endif
{
   S16    ret     = ROK;    /* Return Value */
   PjSn   sn;               /* SN value */
   U8     hdrByte;          /* First byte storing hdr values */
   U8     res;              /* Reserved values */

   TRC2(pjUlmHdlSrbPkt);

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF ),
          (_kwPBuf, "pjUlmHdlSrbPkt(pjRbCb(%d,%d)) \n", \
            pjRbCb->rbId, pjRbCb->rbType));

   /* Initialistaions */

   /* Get the first byte */
   ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
             (_kwPBuf," SRemPreMsg Failed \n"));
      PJ_FREE_BUF(pdu);
      PJ_STS_INC_GEN_CNT(errorPdusRecv);
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* Verify that reserved values are zero */
   res = (hdrByte >> PJ_SRB_SN_LEN);
   if(res != 0)
   {
      PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
               (_kwPBuf," Reserved Values Non-zero \n"));
      MSPD_ERR("Reserved values in pdcp header are non zero\n");
      PJ_FREE_BUF(pdu);
      PJ_STS_INC_GEN_CNT(errorPdusRecv);
      RETVALUE(RFAILED);
   }

   /* Directly assigning since the reserved values are zero */
   sn = hdrByte;

   /* Start processing */
   ret =  pjUlmProcessSrb(pjRbCb, sn, pdu);

   RETVALUE(ret);
}/* pjUlmHdlSrbPkt */

/**
 *
 * @brief 
 *
 *        Function to extract the DRB header.
 *
 * @b Description: 
 *
 *        This function extracts the SN in case of a data packet 
 *        or feedback packet and the FMS and Bitmap incase of a 
 *        PDCP status packet places them in the DrbHdr structure.
 *
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] pdu      PDU to be processed
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDrbPkt
(
PjRbCb *pjRbCb,               /* !< PDCP Control Block */      
Buffer *pdu                   /* !< PDU Buffer */ 
)
#else
PUBLIC S16 pjUlmHdlDrbPkt(pjRbCb, pdu)
PjRbCb *pjRbCb;               /* !< PDCP Control Block */      
Buffer *pdu;                  /* !< PDU Buffer */ 
#endif
{
   S16    ret     = ROK;    /* Return Value */
   PjSn   sn;               /* SN value */
   U8     hdrByte;          /* First byte storing hdr values */
   U8     pduType;          /* Type of PDU */
   U8     res;              /* Reserved values */

   TRC2(pjUlmHdlDrbPkt)

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF ),
          (_kwPBuf, "pjUlmHdlDrbPkt(pjRbCb(%d,%d))\n", \
            pjRbCb->rbId, pjRbCb->rbType));
#ifdef MSPD
   {
       extern U32 ulrate_kwu;
       MsgLen len;
       SFndLenMsg(pdu, &len);
       ulrate_kwu += len;
    }
#endif
   /* Get the first byte */
   ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
             (_kwPBuf," SRemPreMsg Failed \n"));
      PJ_FREE_BUF(pdu);
      PJ_STS_INC_GEN_CNT(errorPdusRecv);
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* check if its a data PDU */
   if((hdrByte & PJ_TYPE_DRB_DATA) == PJ_TYPE_DRB_DATA)
   {

      /* Pack the SN */
      if(pjRbCb->snLen == PJ_7_BIT_SN)
      {
         sn   =  hdrByte & (0x7f);
      }
      else /* Its 12 bit */ 
      {
         /* Verify that reserved values are zero */
         res = (hdrByte >> 4);
         if (res != 8)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
               (_kwPBuf," Reserved Values Non-zero \n"));
            MSPD_ERR("Reserved values in pdcp header are non zero\n");
            PJ_FREE_BUF(pdu);
            PJ_STS_INC_GEN_CNT(errorPdusRecv);
            RETVALUE(RFAILED);
         }

         sn  = hdrByte & PJ_12_BIT_SN_MSB_MASK;
         sn  = sn << PJ_BYTE_SIZE;
         ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
         if (ret != ROK)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
                   (_kwPBuf," SRemPreMsg Failed \n"));
            PJ_FREE_BUF(pdu);
            PJ_STS_INC_GEN_CNT(errorPdusRecv);
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         sn |= hdrByte;
      }

#ifdef MSPD
#if 0
         {
            static U32  lastSn = -1;
            if (lastSn + 1 != sn && !(lastSn==127 && sn ==0))
               MSPD_DBG("Missed/dup packet, lastSn=%u, sn=%u\n",
	             lastSn, sn);
            lastSn = (U32)sn;
         }
#endif
#endif
      /* Call the processing function */
      if(pjRbCb->mode == PJ_DRB_AM)
      {
         ret = pjUlmProcessDrbAm(pjRbCb, sn, pdu);
      }
      else /* (pjRbCb->type == PJ_DRB_UM) */
      {
         ret = pjUlmProcessDrbUm(pjRbCb, sn, pdu);
      }
   }
   else  /* Its a control PDU */
   {
      pduType =  (hdrByte & PJ_PDU_BIT_MASK) >> 4;

      if(pduType == PJ_TYPE_STATUS_REPORT)
      {
      
         /* Copy the FMS */
         sn = hdrByte & PJ_12_BIT_SN_MSB_MASK;
         sn   =  sn << PJ_BYTE_SIZE;
         ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
         if(ret != ROK)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
                   (_kwPBuf," SRemPreMsg Failed \n"));
            PJ_FREE_BUF(pdu);
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         sn   |=  hdrByte;

         /* call processing function */
         MSPD_DBG("[%d][%d] Recived fms %d\n",pjRbCb->ueCb->key.ueId,
               pjRbCb->rbId, sn);		 
         ret = pjDlmHndlStaRep(pjRbCb, sn, pdu);


         /*MS_FIX ReEstablishment Stall fix */
         if(pjRbCb->ulCb.staRepReqd == TRUE)
         {
#ifndef SUD_CHANGE
            pjDlmAddRbToReestLst(pjRbCb);
            pjDlmPostSelfMsg();
#else
            pjDlmReEstDrbAm(pjRbCb);
            pjRbCb->state = PJ_STATE_NORMAL;
#endif
         }			   

      }
      else if(pduType == PJ_TYPE_ROHC_FEEDBACK)
      {

         /* validate that reserved values are 0*/
         res  =  hdrByte & PJ_CPDU_RES_VAL_MASK;
         if(res != 0)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
                   (_kwPBuf," Reserved values non-null \n"));
            MSPD_ERR("Reserved values in pdcp header for ROHC feedback are\
                      non zero\n");
            PJ_FREE_BUF(pdu);
            PJ_STS_INC_GEN_CNT(errorPdusRecv);
            RETVALUE(RFAILED);
         }

         ret = pjUtlSndFeedback(pjRbCb, pdu);
      }
      else
      {
         /* Invalid PDU type */
         PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
                (_kwPBuf," Invalid PDU Type \n"));
         MSPD_ERR("Invalid PDU type received from RLC in UL\n");
         PJ_FREE_BUF(pdu);
         PJ_STS_INC_GEN_CNT(errorPdusRecv);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ret);
}/* pjUlmHdlDrbPkt */

/*****************************************************************************
 *          PROCESSING FUNCTIONS
 ****************************************************************************/

/**
 *
 * @brief 
 *
 *        This function is called after header extraction to process the SRB pdu.
 *
 * @b Description: 
 *
 *        This function performs the follwing functions:                   @n
 *        1. Create and Insert the entry in the reception buffer.          @n
 *        2. Update the nxtRxSn and rxHfn values.                          @n
 *        3. Call pjUlmHdlDecipher to perform deciphering.                 @n
 *        4. Call pjUlmHdlIntVer to perform integrity verification.        @n
 *        5. Call pjUlmDeliverSrb for performing state updations and 
 *           submitting to upper layers.                                   @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] sn       SN value of the pdu
 *  @param[in] pdu      PDU message buffer.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmProcessSrb
(
PjRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
)
#else
PUBLIC S16 pjUlmProcessSrb(pjRbCb, sn, pdu)
PjRbCb    *pjRbCb;          /* !< PDCP Control Block */     
PjSn      sn;              /* !< SN value of PDU */           
Buffer   *pdu;             /* !< PDU message buffer */           
#endif
{
   PjUlCb   *ulCb;          /* PTR to UL Control Block        */
   PjRxEnt  *rxEnt;         /* Ptr to Rx Entry                */
   S16       ret     = ROK; /* Return value                   */
   U32       count;         /* Count value */

   TRC2(pjUlmProcessSrb)

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF ),
          (_kwPBuf, "pjUlmProcessSrb(pjRbCb(%d,%d),sn(%d)) \n", \
            pjRbCb->rbId, pjRbCb->rbType, sn));

   /* 1. Initialisations */
   ulCb  = &pjRbCb->ulCb;

   /* 2. Check for wrap around and update the Hfn */
   /* Set the nxtSubCnt value */
   PJ_GET_COUNT(count, PJ_SRB_SN_LEN, sn, (ulCb->rxHfn));
   PJ_ULM_UPD_RX_VAR(sn, pjRbCb);
   PJ_ULM_UPD_NXT2SUB(ulCb, count);

   /* 3. Create and insert the entry in the buffer */
   PJ_ALLOC(rxEnt, sizeof(PjRxEnt));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(rxEnt == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW107, (ErrVal)0,       \
            "pjUlmProcessSrb: Alloc Failed \n");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   rxEnt->count   =  count;
   rxEnt->state   =  PJ_RDY_TO_DCIPHER;
   rxEnt->mBuf    =  pdu;
   /*Insert the entry in the buffer */
   ret = pjDbmInsRxEnt(&ulCb->recBuf, rxEnt, FALSE); 

   /* 4. Process for deciphering */
   ret = pjUlmHdlDecipher(pjRbCb, rxEnt);

   RETVALUE(ret);
}/*pjUlmProcessSrb  */


/**
 *
 * @brief 
 *
 *        Function to process the DRB UM Pdu. It is called from pjUlmHdlDrbPkt.
 *
 * @b Description: 
 *
 *        This function performs the follwing steps:                       @n
 *        1. Create and Insert the entry in the reception buffer.          @n
 *        2. Check for firstSn in reestablish state.                       @n
 *        3. Update nxtSubCnt and nxtSubDeCmp.                             @n
 *        4. Call pjUtlDecipher to perform deciphering.                    @n
 *        5. Call pjUtlDecomp to perform integrity verification.           @n
 *        6. Call pjUlmDeliverDrbUm for performing state updations and 
 *           submitting to upper layers.                                   @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] sn       SN value of the pdu
 *  @param[in] pdu      PDU message buffer.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmProcessDrbUm
(
PjRbCb   *pjRbCb,          /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
)
#else
PUBLIC S16 pjUlmProcessDrbUm(pjRbCb, sn, pdu)
PjRbCb    *pjRbCb;         /* !< PDCP Control Block */     
PjSn      sn;              /* !< SN value of PDU */           
Buffer   *pdu;             /* !< PDU message buffer */           
#endif
{
   PjUlCb       *ulCb;            /* PTR to UL Control Block     */
   PjRxEnt      *rxEnt;           /* Ptr to Rx Entry             */
   S16          ret      = ROK;   /* Return value                */
   U32          count;           /* Count value */
   /* kw006.201, ccpu00120574, fixed the warning */
#ifdef PJ_SEC_ASYNC
   U32          oldRxCnt;        /* Old Count value used for decomp */
#endif

   TRC2(pjUlmProcessDrbUm)

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF ),
          (_kwPBuf, "pjUlmProcessDrbUm(pjRbCb(%d,%d),sn(%d)) \n", \
            pjRbCb->rbId, pjRbCb->rbType, sn));

   /* 1. Initialisations */
   sn    =  sn;
   ulCb  = &pjRbCb->ulCb;
   /* kw006.201, ccpu00120574, fixed the warning */
#ifdef PJ_SEC_ASYNC
   PJ_GET_COUNT(oldRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));
#endif
   /* 2. Check for wrap around */
   PJ_ULM_UPD_RX_VAR(sn, pjRbCb);
   PJ_GET_COUNT(count, (pjRbCb->snLen), sn, (ulCb->rxHfn));
   /* kw006.201, ccpu00120574, fixed the warning */
#ifdef PJ_SEC_ASYNC
   PJ_ULM_UPD_NXT2DCOMP(ulCb, (pjRbCb->snLen), sn, oldRxCnt);
#endif
   PJ_ULM_UPD_NXT2SUB(ulCb, count); 

   /* 3. Create the entry */
   PJ_ALLOC(rxEnt, sizeof(PjRxEnt));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(rxEnt == NULLP)
   {
      PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR ),
            (_kwPBuf, "pjUlmProcessDrbUm: Alloc Failed \n"));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* 4. Fill values and Insert into hash list */
   rxEnt->count   =  count;
   rxEnt->state   =  PJ_RDY_TO_DCIPHER;
   rxEnt->mBuf    =  pdu;
   ret = pjDbmInsRxEnt(&ulCb->recBuf, rxEnt, FALSE); 

   /* 5. Update OBD count */
   PJ_INC_OBD_COUNT(pjRbCb, rxEnt->count);

   /* 6. Send for deciphering */
   ret = pjUlmHdlDecipher(pjRbCb, rxEnt);

   RETVALUE(ret);
}/* pjUlmProcessDrbUm */


/**
 *
 *        Function to process the DRB AM Pdu. It is called from pjUlmHdlDrbPkt.
 *
 * @b Description: 
 *
 *        This function performs the follwing functions:                                            @n
 *        1. Create and Insert the entry in the reception buffer.                                   @n
 *        2. Perform checks to see if its a valid PDU and calculate RX_HFN                          @n
 *        3. Check for firstSn in reestablish state.                                                @n
 *        4. Update nxtSubCnt and nxtSubDeCmp.                                                      @n
 *        5. Call pjUtlDecipher to perform deciphering.                                             @n
 *        6. Call pjUtlDecomp to perform integrity verification.                                    @n
 *        7. Call pjUlmDeliverDrbUm for performing state updations and submitting to upper layers.  @n
 * @brief 
 *
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] sn       SN value of the pdu
 *  @param[in] pdu      PDU message buffer.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmProcessDrbAm
(
PjRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
)
#else
PUBLIC S16 pjUlmProcessDrbAm(pjRbCb, sn, pdu)
PjRbCb   *pjRbCb;          /* !< PDCP Control Block */     
PjSn     sn;              /* !< SN value of PDU */           
Buffer   *pdu;             /* !< PDU message buffer */           
#endif
{
   PjUlCb       *ulCb;            /* PTR to UL Control Block     */
   PjRxEnt      *rxEnt;           /* Ptr to Rx Entry             */
   S16          ret      = ROK;   /* Return value                */
   U16          ctxHfn;           /* Context Hfn for lib fn      */
   /* kw006.201, ccpu00120574, fixed the warning */
#ifdef PJ_SEC_ASYNC
   U32          oldRxCnt;        /* Old Count value used for decomp */
#endif
   Bool         discFlag;         /* Tmp Flag to store the value from rxEnt */
   Bool         dupEntry;         /* Tmp Flag to store the value from rxEnt */

   TRC2(pjUlmProcessDrbAm);

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_BRIEF ),
          (_kwPBuf, "pjUlmProcessDrbAm(pjRbCb(%d,%d),sn(%d)) \n", \
            pjRbCb->rbId, pjRbCb->rbType, sn));

   /* 1. Initialisations */
   sn     = sn;
   ulCb   = &pjRbCb->ulCb;
   ctxHfn = ulCb->rxHfn;
   /* kw006.201, ccpu00120574, fixed the warning */
#ifdef PJ_SEC_ASYNC
   PJ_GET_COUNT(oldRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));
#endif

   /* 2. Allocate memory for the rxEnt */
   PJ_ALLOC(rxEnt, sizeof(PjRxEnt));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(rxEnt == NULLP)
   {
      PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR ),
            (_kwPBuf, "pjUlmProcessDrbAm: Alloc Failed \n"));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* 3. Perform validations */
   /* 3.1 A SN already delivered to the upper layer has come */
   //MSPD_DBG("[%d][%d]PDCP Sn lstSubCnt %d %d\n", pjRbCb->ueCb->key.ueId,
   //            pjRbCb->rbId, sn, ulCb->lastSubCnt);	

   if(PJ_CHK_SN_OUTSIDE_WIN(ulCb, sn))
   {
      rxEnt->discFlag = TRUE;
      MSPD_ERR("[%d][%d] Discard OutSide window %d %d\n", pjRbCb->ueCb->key.ueId,
               pjRbCb->rbId, sn, ulCb->lastSubCnt);	
      if(sn > ulCb->nxtRxSn)
      {
         ctxHfn --;
      }
   }

   /* 3.2 Wrap around case */
   else if((ulCb->nxtRxSn - sn) > PJ_DRB_AM_WINSIZE)
   {
      ulCb->rxHfn ++;
      ctxHfn ++;
      ulCb->nxtRxSn = sn + 1;
      /* kw006.201, ccpu00120574, fixed the warning */
#ifdef PJ_SEC_ASYNC
      PJ_ULM_UPD_NXT2DCOMP(ulCb, (pjRbCb->snLen), sn, oldRxCnt);
#endif

   }
   /* 3.3 A missed out packet arriving after wrap around */
   else if((sn - ulCb->nxtRxSn) > PJ_DRB_AM_WINSIZE)
   {
         ctxHfn --;
   }
   /* 3.4 normal scenario */
   else if(sn >=  ulCb->nxtRxSn)
   {

      ulCb->nxtRxSn = sn + 1;
      if(ulCb->nxtRxSn ==  (1 << pjRbCb->snLen))
      {
         ulCb->nxtRxSn = 0;
         ulCb->rxHfn ++;
      }
   }

   /* 4. Fill up the structures and insert into hashlist */
   rxEnt->count   =  (ctxHfn << pjRbCb->snLen) | sn;
   rxEnt->state   =  PJ_RDY_TO_DCIPHER;
   rxEnt->mBuf    =  pdu;

   PJ_ULM_UPD_NXT2SUB(ulCb, rxEnt->count); 
   ret = pjDbmInsRxEnt(&ulCb->recBuf, rxEnt, TRUE); 
   if(ret == ROKDUP)
   {
      /* Duplicates are not to be inserted */
      /* marked to be freed up later */
      rxEnt->dupEntry =  TRUE;
   }

   /* 5. Update OBD count */
   if((rxEnt->dupEntry != TRUE) &&
      (rxEnt->discFlag != TRUE))
   {
      PJ_INC_OBD_COUNT(pjRbCb, rxEnt->count);
   }
   discFlag =  rxEnt->discFlag;
   dupEntry =  rxEnt->dupEntry;

   /* 6. Send for deciphering */
   ret = pjUlmHdlDecipher(pjRbCb, rxEnt);

   /* 7. Cleanup the duplicates */
   if(dupEntry == TRUE)
   {
      /* duplicate entry */
      MSPD_ERR("Cleaning up the duplicate entries\n");
      PJ_FREE_BUF(rxEnt->mBuf);
      PJ_FREE(rxEnt, sizeof(PjRxEnt));
   }
   else if(discFlag == TRUE)
   {
      /* duplicate entry */
      MSPD_ERR("Cleaning up the entries marked for discard\n");
      pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
   }

   RETVALUE(ROK);
}/* pjUlmProcessDrbAm */

/**
 *
 * @brief 
 *
 *        Function is called when the offboard timer expires.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. If the SDU associated with the timer is not delivered, we 
 *       delete the entry .                                              @n
 *    2. A status indication is sent to the user with error cause.       @n
 *    3. We associate the nextToSubmit value to the next SDU that has 
 *       to be submitted.                                                @n
 *    4. The timer is associated with the nextToSubmit SDU and restarted.@n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDecipher
(
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
)
#else
PUBLIC S16 pjUlmHdlDecipher(pjRbCb, rxEnt)
PjRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* !< Rx Entry */
#endif
{
   PjUlCb   *ulCb;            /* ULM Control Block        */
   PjSecInp  inParam;         /* Input parameters for Lib */
   S16       ret     = ROK;   /* Return values            */
   Buffer   *mBuf    = NULLP; /* Output Buffer            */

   TRC2(pjUlmHdlDecipher);

#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlDecipher(pjRbCb(%d,%d),rxEnt(%ld)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlDecipher(pjRbCb(%d,%d),rxEnt(%d)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#endif

   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;

   /* 2. Process for Deciphering */
   if(pjRbCb->ueCb->secInfo.secAct == TRUE)
   {
      /* 2.1 Check that it is not the first pkt, with selectively enabled */
      if((pjRbCb->rbType == PJ_SRB) && 
         (pjRbCb->ueCb->secInfo.firstUl && pjRbCb->ueCb->secInfo.selSecAct))
      {
         /* first pkt */
         pjRbCb->ueCb->secInfo.firstUl   =  FALSE;
      }
      else
      {
         /* 2.2 Prepare the input Parameters */
         inParam.dir    =  PJ_SEC_DIR_UL;
         inParam.rbId   =  pjRbCb->rbId;
         inParam.count  =  rxEnt->count;
#ifdef TENB_AS_SECURITY
         /*chetan*/
         /*PJ_ALLOC_BUF(mBuf);*/
#endif
         /* 2.3 call deciphering hook */
         if( pjUtlDecipherReq(pjRbCb, inParam, rxEnt->mBuf, &mBuf) != ROK)
         {
            PJLOGERROR(ERRCLS_ADD_RES, EKW108, (ErrVal)rxEnt->count,    \
                  "pjUlmHdlDecipher: Deciphering Failed \n");
            /* kw006.201 replaced macro with function */
            pjUlmSndPjuStaInd(pjRbCb, rxEnt);
            pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
            PJ_STS_INC_GEN_CNT(numDeciphFails);
            RETVALUE(RFAILED);
         }

#ifdef PJ_SEC_ASYNC
         /* 2.4 wait for output before processing further */
         rxEnt->state   =  PJ_SENT_TO_DCIPHER;
         RETVALUE(ROK);
#else   /* PJ_SEC_ASYNC */
      /* 2.3 Validate output of deciphering */
      if(mBuf == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW109, (ErrVal)rxEnt->count,\
               "pjUlmProcessDrbUm: Decipher Failed \n");
         /* kw006.201 replaced macro with function */
         pjUlmSndPjuStaInd(pjRbCb, rxEnt);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(numDeciphFails);
         RETVALUE(RFAILED);
      }

         /* 2.5 copy output buffer */
#ifdef TENB_AS_SECURITY
         /*PJ_FREE_BUF(rxEnt->mBuf);*/ /*- commented out, new mBuf same as original */
#endif
         rxEnt->mBuf     =  mBuf;
#endif
      }
   }
   /* 3. Post ciphering updations */
   if(pjRbCb->rbType == PJ_DRB)
   {
      rxEnt->state   =  PJ_RDY_TO_DCOMP;
      ret = pjUlmHdlDeCmp(pjRbCb, rxEnt);
   }
   else
   {
      rxEnt->state   =  PJ_RDY_TO_INTVER;
      ret = pjUlmHdlIntVer(pjRbCb, rxEnt);
   }

   RETVALUE(ret);
}/* end of pjUlmHdlDecipher */


/**
 *
 * @brief 
 *
 *        Function is called to perform integrity verification.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. Extract the mac-I.
 *    2. Add sn to the buffer.
 *    3. Call Integrity Verification hook.
 *    4. Return in case of Async.
 *    5. Validate output and submit for delivery.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlIntVer
(
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
)
#else
PUBLIC S16 pjUlmHdlIntVer(pjRbCb, rxEnt)
PjRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* !< Rx Entry */
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
   Data     sn;            /* Sn Value */
   S16      ret = ROK;     /* Return value */
   U32      macI = 0;      /* Mac-I value */
   PjSecInp inParam;       /* Input Params */
   Status   status;        /* Status of IntVer */

   TRC2(pjUlmHdlIntVer);

#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlIntVer(pjRbCb(%d,%d),rxEnt(%ld)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlIntVer(pjRbCb(%d,%d),rxEnt(%d)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#endif

   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;

   /* 2. Xtract the last four bits irrescpective of whether you 
    * are going to do integrity verification or not */
   PJ_UNPK_MACI(rxEnt->mBuf, macI);

   /* 3. Process for Integrity Verification */
   if(pjRbCb->ueCb->secInfo.secAct == TRUE)
   {

      /* 3.1 Prepare the input Parameters */
      inParam.dir    =  PJ_SEC_DIR_UL;
      inParam.rbId   =  pjRbCb->rbId;
      inParam.count  =  rxEnt->count;

      /* 3.2. Add the SN to the buffer */
      sn  =  rxEnt->count % (1 << PJ_SRB_SN_LEN);
      ret = SAddPreMsg(sn, rxEnt->mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
               (_kwPBuf," pjUlmProcessSrb: SRemPreMsgMult Failed \n"));
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      /* 3.3 call deciphering hook */
      if( pjUtlIntVerReq(pjRbCb, inParam, rxEnt->mBuf, macI, &status) != ROK)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW110, (ErrVal)rxEnt->count,    \
               "pjUlmProcessDrbUm: Deciphering Failed \n");
         /* kw006.201 replaced macro with function */
         pjUlmSndPjuStaInd(pjRbCb, rxEnt);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(numIntgVrfFails);
         RETVALUE(RFAILED);
      }

      /* 3.4. Remove the SN from the buffer */
      ret = SRemPreMsg((Data *)&sn, rxEnt->mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
               (_kwPBuf," pjUlmProcessSrb: SRemPreMsgMult Failed \n"));
         PJ_FREE_BUF(rxEnt->mBuf);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

#ifdef PJ_SEC_ASYNC
      /* 3.5 wait for output before processing further */
      rxEnt->state   =  PJ_SENT_TO_INTVER;
      RETVALUE(ROK);
#else
#if 0
chetan
      /* 3.6 Post integrity verification updations */
      if(status != ROK)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW111, (ErrVal)rxEnt->count,
               "pjUlmProcessSrb: Integrity Failed \n");
         /* kw006.201 replaced macro with function */
         pjUlmSndPjuStaInd(pjRbCb, rxEnt);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(numIntgVrfFails);
         RETVALUE(RFAILED);
      }
#endif
#endif /* PJ_SEC_ASYNC */

   }

   /* 4. Update state and send for delivery */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   ret = pjUlmDeliverSrb(pjRbCb, rxEnt);

   RETVALUE(ret);
}/* end of pjUlmHdlIntVer */


/**
 *
 * @brief 
 *
 *        Function is called to handle decompression.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. Call decompression handler function.
 *    2. For sync, validate output and submit for delivery.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDeCmp
(
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
)
#else
PUBLIC S16 pjUlmHdlDeCmp(pjRbCb, rxEnt)
PjRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* !< Rx Entry */
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
   Buffer  *mBuf = NULLP;  /* Output Buffer */

   TRC2(pjUlmHdlDeCmp);

#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
          (_kwPBuf, "pjUlmHdlDeCmp(pjRbCb(%d,%d),rxEnt(%ld)) \n", \
            pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
          (_kwPBuf, "pjUlmHdlDeCmp(pjRbCb(%d,%d),rxEnt(%d)) \n", \
            pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#endif

   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;

   /* 2. process for decompression */
   if(pjRbCb->rohc.hdrCmpUsed == TRUE)
   {
      /* 2.1 call decompression hook */
      if(pjUtlDeCmpReq(pjRbCb,rxEnt->count, rxEnt->mBuf, &mBuf) != ROK)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW112, (ErrVal)rxEnt->count,\
               "pjUlmHdlDeCmp: Decomp Failed \n");
         /* kw006.201 replaced macro with function */
         pjUlmSndPjuStaInd(pjRbCb, rxEnt);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(numDecmpFails);
         RETVALUE(RFAILED);
      }

#ifdef PJ_CMP_ASYNC
      /* 2.2 Wait for output before processing further */
      rxEnt->state   =  PJ_SENT_TO_DCOMP;
      RETVALUE(ROK);
#else /* PJ_CMP_ASYNC */

      /* 2.3 Validate output of decomp */
      if(mBuf == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW113, (ErrVal)rxEnt->count,\
               "pjUlmProcessDrbUm: Decomp Failed \n");
         /* kw006.201 replaced macro with function */
         pjUlmSndPjuStaInd(pjRbCb, rxEnt);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(numDecmpFails);
         RETVALUE(RFAILED);
      }

      /* 2.4 Copy output buffer */
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf     =  mBuf;
#endif
   }

   /* 3. Return for discardable entries */
   if((rxEnt->discFlag == TRUE) ||
      (rxEnt->dupEntry == TRUE))
   {
      RETVALUE(ROK);
   }

   /* 4. Post decomp updations */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   PJ_DEC_OBD_COUNT(pjRbCb, rxEnt->count);
   /* kw006.201 ccpu00118629, replaced macro with function */
   pjUlmDeliverDrb(pjRbCb, rxEnt);

   RETVALUE(ROK);
}/* end of pjUlmHdlDeCmp */

/**
 *
 * @brief 
 *
 *        Function to perform updations and deliver the SDU to the upper layer. 
 *        It is called for SRBs .
 *
 * @b Description: 
 *
 *        This function performs the following functions,                    @n
 *        1. Call PjUiPjuDatInd to deliver the SDU to the upper layer.       @n
 *        2. In async mode, check if any subsequent SDUs can also be sent up.@n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEn     reception entry for the PDU
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmDeliverSrb
(
PjRbCb *pjRbCb,          /* !< PDCP Control Block */     
PjRxEnt *rxEnt           /* !< Recption Buffer Entry   */            
)
#else
PUBLIC S16 pjUlmDeliverSrb( pjRbCb, rxEnt)
PjRbCb *pjRbCb;          /* !< PDCP Control Block */                  
PjRxEnt *rxEnt;          /* !< Recption Buffer Entry   */             
#endif
{

#ifdef PJ_SEC_ASYNC
   U32      nxtRxCnt;     /* Count for looping through the entries */
   PjRxEnt  *tmpEnt;      /* Temp var for looping through the entries */
   /* kw006.201, ccpu00120574, fixed the warning */
   U32      count;        /* Count for looping through the entries */
#endif
   PjUlCb   *ulCb;        /* Pointer to Uplink Control Block */

   TRC2(pjUlmDeliverSrb);
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
          (_kwPBuf, "pjUlmDeliverSrb(pjRbCb(%d,%d),rxEnt(%ld)) \n", \
            pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
          (_kwPBuf, "pjUlmDeliverSrb(pjRbCb(%d,%d),rxEnt(%d)) \n", \
            pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#endif

   /* Post integrity verification updations */
#ifdef PJ_SEC_ASYNC
   /* kw006.201, ccpu00120574, fixed the warning */
   count          =  rxEnt->count;
#endif
   ulCb           =  &pjRbCb->ulCb;

   /* call the PJU Primitive to deliver it to upper layers */
   /* kw006.201 ccpu00118629, replaced macro with function */
   pjUlmSndPjuDatInd(pjRbCb, rxEnt);

   /* cleanup the entry */
   pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);

#ifdef PJ_SEC_ASYNC
   /* Search and see if any of the successive SDUs can 
    * also be sent to the upper layer */
   count++;
   nxtRxCnt  =  (ulCb->rxHfn << PJ_SRB_SN_LEN) | ulCb->nxtRxSn;

   /* Check till nxtRxCnt */
   while(count < nxtRxCnt) 
   {
      /* Get the next node */
      tmpEnt   =  (PjRxEnt *)pjDbmGetRxEnt(&ulCb->recBuf, count);
      count ++;

      if(tmpEnt == NULLP)
      {
         /* Such an entry does not exist, search for the next */
         continue;
      }
      if(tmpEnt->state != PJ_RDY_TO_SUBMIT)
      {
         /* Integrity verification not yet done so we have to wait */
         ulCb->nxtSubCnt =  tmpEnt->count;
         /* Cant send anymore messages up, break*/
         break;
      }
      else
      {
         /* call the PJU Primitive to deliver it to upper layers */
         /* kw006.201 ccpu00118629, replaced macro with function */
         pjUlmSndPjuDatInd(pjRbCb, tmpEnt);

         /* cleanup the entry */
         pjDbmDelRxEnt(&ulCb->recBuf, tmpEnt->count);
      }
   }
#endif /* PJ_SEC_ASYNC */

   RETVALUE(ROK);
}/* pjUlmDeliverSrb */


/**
 *
 * @brief 
 *
 *        Function to deliver the SDU to the upper layer. 
 *        It is called for DRB UM
 *
 * @b Description: 
 *
 *        This function performs the following functions.                  @n
 *        1. Deliver the SDU to the upper layer                            @n
 *        2. Clean up the hash list entry for this SDU.                    @n
 *        3. For asynchronous mode, it checks if any subsequent 
 *           messages can also be sent up.                                 @n
 *        4. Mark the next SDU to be submitted to the upper layers.        @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    reception entry for the PDU
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef MSPD
static MspdLogCaptureTrigger = 0;
#endif
#ifdef ANSI
PUBLIC S16 pjUlmDeliverDrbUm
(
PjRbCb *pjRbCb,        /* !< PDCP Control Block */                  
PjRxEnt *rxEnt         /* !< Recption Buffer Entry   */             
)
#else
PUBLIC S16 pjUlmDeliverDrbUm(pjRbCb, rxEnt)
PjRbCb *pjRbCb;        /* !< PDCP Control Block */                  
PjRxEnt *rxEnt;        /* !< Recption Buffer Entry   */             
#endif
{

   PjUlCb   *ulCb;     /* Uplink Cb Ptr */
   /* kw006.201, ccpu00120760 fixing the off board security/compression error 
    * with nxtSubCnt due to missing compile flag */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   U32      nxtRxCnt;  /* Count for looping through the entries */
   PjRxEnt  *tmpEnt;   /* Temp var for looping through the entries */
   U32      count;     /* count varaible */ 
#endif

   TRC2(pjUlmDeliverDrbUm)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmDeliverDrbUm(pjRbCb(%d,%d),rxEnt(%ld)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmDeliverDrbUm(pjRbCb(%d,%d),rxEnt(%d)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#endif

   /* Initialisations */
   ulCb  =  &pjRbCb->ulCb;

   /* Update state */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   if(rxEnt->count != ulCb->nxtSubCnt)
   {
      RETVALUE(ROK);
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
#ifndef PJ_CMP_ASYNC
   /* kw005.201 added support for L2 Measurement */
   PJ_UPD_L2_ULLOSS_PKT_STS(pjRbCb);
#endif
#ifdef MSPD
   {
      extern U32 ulrate_pju;
      MsgLen len;
      SFndLenMsg(rxEnt->mBuf, &len);
      ulrate_pju += len;
   }
#endif
   /* call the PJU Primitive to deliver it to upper layers */
   /* kw006.201 ccpu00118629, replaced macro with function */
   pjUlmSndPjuDatInd(pjRbCb, rxEnt);

   /* cleanup the entry */
   pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);

   /* kw006.201, ccpu00120760 fixing the off board security/compression error 
    * with nxtSubCnt due to missing compile flag */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))

   /* Search and see if any of the succeeding SNs can 
    * also be sent to the upper layer */
   nxtRxCnt  =  (ulCb->rxHfn << pjRbCb->snLen) | ulCb->nxtRxSn;

   /* Search till nextRxCnt */
   for(count = ulCb->nxtSubCnt + 1; count < nxtRxCnt; count++) 
   {
      /* Get the next node */
      tmpEnt   =  (PjRxEnt *)pjDbmGetRxEnt(&(ulCb->recBuf), count);

      if(tmpEnt == NULLP)
      {
         /* kw005.201 added support for L2 Measurement */
         PJ_UPD_L2_ULLOSS_STS(pjRbCb);
         /* Such an entry does not exist, search for the next */
         continue;
      }
      if(tmpEnt->state != PJ_RDY_TO_SUBMIT)
      {
         /* Cant send anymore messages up, break*/
         break;
      }
      else
      {
         /* kw005.201 added support for L2 Measurement */
         PJ_UPD_L2_ULLOSS_PKT_STS(pjRbCb);
         /* call the PJU Primitive to deliver it to upper layers */
         /* kw006.201 ccpu00118629, replaced macro with function */
         pjUlmSndPjuDatInd(pjRbCb, tmpEnt);

         /* cleanup the entry */
         pjDbmDelRxEnt(&ulCb->recBuf, tmpEnt->count);
      }
   }/* end of while */

   /* Update  nxtSubCnt */
   ulCb->nxtSubCnt =  count;
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
      /* start reestablishment procedures if last message has been processed */
      if((ulCb->transCmp == TRUE) && 
            (ulCb->obdCnt == 0))
      {
         /* It has received all messages from RLC and finished
          * processing them. Possibility only when the last last message
          * from deciphering fails. */
         PJ_ULM_DRBUM_REEST(pjRbCb);
      }
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

   RETVALUE(ROK);
}/* pjUlmDeliverDrbUm */

/**
 *
 * @brief 
 *
 *        Function to deliver the SDU to the upper layer. It is called for DRB AM
 *
 * @b Description: 
 *
 *        This function performs the following functions.                                  @n
 *        1. Delete duplicates and SDUs marked for discard.                                @n
 *        2. Deliver the SDU to the upper layer                                            @n
 *        3. Perform sequential delivery for SDUs received during reestablishment.         @n
 *        4. Clean up the hash list entry for this SDU.                                    @n
 *        5. For asynchronous mode, check if any subsequent messages can also be sent up.  @n
 *        6. Mark the next SDU to be submitted to the upper layers.                        @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmDeliverDrbAm
(
PjRbCb *pjRbCb,           /* !< PDCP Control Block */                  
PjRxEnt *rxEnt            /* !< Recption Buffer Entry   */             
)
#else
PUBLIC S16 pjUlmDeliverDrbAm(pjRbCb, rxEnt)
PjRbCb *pjRbCb;           /* !< PDCP Control Block */                  
PjRxEnt *rxEnt;           /* !< Recption Buffer Entry   */             
#endif
{
   PjUlCb  *ulCb;        /* RB uplink Control Block */
   U32      nxtSubCnt;   /* Delivery count */
   U32      subCnt;      /* Delivery count */
   U32      nxtRxCnt;    /* Next Rx count */
   S16      ret;         /* Return value */
   PjRxEnt *tmpEnt;      /* Temp Entry - loop counter */

   TRC2(pjUlmDeliverDrbAm)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmDeliverDrbAm(pjRbCb(%d,%d),rxEnt(%ld)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmDeliverDrbAm(pjRbCb(%d,%d),rxEnt(%d)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#endif

   /* Initialisations */
   ret         = ROK;
   ulCb        = &pjRbCb->ulCb;
   nxtSubCnt   = ulCb->lastSubCnt + 1;
   subCnt      = rxEnt->count;
   /* For reestablishment case */
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
      /* messages received before reestablishment was triggered need 
       * not go through this check */
      if(rxEnt->count >= ulCb->firstReEstCnt)
      {
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
         /* consider for delivering only if its equal to  
          * Last_Submitted_PDCP_RX_SN + 1  */
         if(nxtSubCnt != rxEnt->count)
         {
            /* done with trying to deliver, return */
            /* start reestablishment procedures if last message has been processed */
            if((ulCb->transCmp == TRUE)
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
                  && (ulCb->obdCnt == 0)
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
              )
            {
               /* It has received all messages from RLC and finished
                * processing them and delivered as much as possible */
               pjUlmReEstDrbAm(pjRbCb);
            }
            RETVALUE(ROK);
         }
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
      }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
   }
#ifdef MSPD
   {
      extern U32 ulrate_pju;
      MsgLen len;
      SFndLenMsg(rxEnt->mBuf, &len);
      ulrate_pju += len;
   }
#endif
   nxtRxCnt  =  (ulCb->rxHfn << pjRbCb->snLen) | ulCb->nxtRxSn;
   /* Try to deliver the messages up */
   for(nxtSubCnt = ulCb->lastSubCnt + 1; nxtSubCnt < nxtRxCnt; nxtSubCnt++)
   {
      tmpEnt   = (PjRxEnt *)pjDbmGetRxEnt(&ulCb->recBuf, nxtSubCnt); 
      
      if(tmpEnt == NULLP)
      {
         if(nxtSubCnt > subCnt)
         {
            /* PDUs with SN values greater than count should be delivered
             * consecutively only */
            break;
         }
         else
         {
            /* kw005.201 added support for L2 Measurement */
            PJ_UPD_L2_ULLOSS_STS(pjRbCb);
            /* PDUs with SN/Cnt values less that the rxEnt->count can be
             * delivered in ascending order, not necessary for consecutive
             * delivery. So search for next available entry  */
            continue;
         }
      }
      else if(tmpEnt->state != PJ_RDY_TO_SUBMIT)
      {
         /* PDU still under processing, return */
         RETVALUE(ROK);
      }

      /* kw005.201 added support for L2 Measurement */
      PJ_UPD_L2_ULLOSS_PKT_STS(pjRbCb);

      /* submit to upper layers and delete entry */
      /* kw006.201 ccpu00118629, replaced macro with function */
      pjUlmSndPjuDatInd(pjRbCb, tmpEnt);
      pjDbmDelRxEnt(&ulCb->recBuf, tmpEnt->count);

      /* do updations */
      ulCb->lastSubCnt   =  nxtSubCnt;
   }/* while(ulCb->recBuf.numEntries != 0) */

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   /* start reestablishment procedures if last message has been processed */
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
      if((ulCb->transCmp == TRUE) && 
         (ulCb->obdCnt == 0))
      {
         /* It has received all messages from RLC and finished
          * processing them. Possibility only when the last last message
          * from deciphering fails. */
         pjUlmReEstDrbAm(pjRbCb);
      }
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

   RETVALUE(ret);
}/* pjUlmDeliverDrbAm */

/*****************************************************************************
 *                      REESTABLISHMENT FUNCTIONS
 ****************************************************************************/

/**
 *
 * @brief 
 *
 *        Function to is called to reestablish the SRB. It is called as when the 
 *        reestablishment request  is received.
 *
 * @b Description: 
 *
 *        This function performes the following.      @n
 *        1. Reset NEXT_PDCP_RX_SN and RX_HFN         @n
 *        2. Deinitialise the hashlist.               @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmReEstSrb
(
PjRbCb *pjRbCb       /*!< PDCP control block  */
)
#else
PUBLIC S16 pjUlmReEstSrb(pjRbCb)
PjRbCb *pjRbCb;      /*!< PDCP control block  */
#endif
{
   TRC2(pjUlmReEstSrb);

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
          (_kwPBuf, "pjUlmReEstSrb(pjRbCb) \n"));

   /* Reset NEXT_PDCP_RX_SN and RX_HFN */
   pjRbCb->ulCb.nxtRxSn  =  0;
   pjRbCb->ulCb.rxHfn  =  0;

   /* Deinitialise the hashlist */
   pjDbmRxDelAll(&pjRbCb->ulCb.recBuf);

   RETVALUE(ROK);
}  /* pjUlmReEstSrb */


/**
 *
 * @brief 
 *
 *        Function to is called to reestablish the DRB .
 *
 * @b Description: 
 *
 *        This function is called after the last pdu is received from RLC
 *        as part of reestablishment procedures. This function performes the following.  @n
 *        1. Reset NEXT_PDCP_RX_SN and RX_HFN                                            @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmReEstDrbAm
(
PjRbCb *pjRbCb       /*!< PDCP control block  */
)
#else
PUBLIC S16 pjUlmReEstDrbAm(pjRbCb)
PjRbCb *pjRbCb;      /*!< PDCP control block  */
#endif
{

   U32               count;             /* Counter variable                 */
   U32               nxtRxCnt;          /* Count of the next Pdu to receive */
   U32               datFwdCnt = 0;     /* Counter variable for datFwInd    */
   S8                bitCnt;            /* Bit count                        */
   U8                bMapByte;          /* Temp byte for preparing bitmap   */
   Buffer           *bMapBuf;           /* Bitmap status report buffer      */
   U16               fms;               /* First Missing SN                 */
   PjuDatFwdIndInfo *datFwdInd = NULLP; /* Dat Fwd info                     */
   PjRxEnt          *tmpEnt    = NULLP; /* Temporary entity used in loop    */
   PjUlCb           *ulCb;              /* RB uplink Control Block          */
   PjSn              sn;                /* temp sn                          */

   TRC2(pjUlmReEstDrbAm);

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmReEstDrbAm(pjRbCb(%d,%d)) \n", \
                     pjRbCb->rbId, pjRbCb->rbType));

   /* Initialisations */
   ulCb  =  &pjRbCb->ulCb;

   /* Its DRB AM from now on */

   if(pjRbCb->state == PJ_STATE_REEST)
   {
      /* if status report generation not necessary, quit */
      if(ulCb->staRepReqd == FALSE)
      {
         /* Mark that this rb is done with reestablishment */
         pjUlmDecReEstCnt(pjRbCb);
         MSPD_DBG_RLC("[%d][%d]ReEst Done for DRB\n", pjRbCb->ueCb->key.ueId,
                      pjRbCb->rbId);
         RETVALUE(ROK);
      }
   }
   else if(pjRbCb->state == PJ_STATE_REEST_HO)
   {
      if(ulCb->recBuf.numEntries != 0)
      {
         /* Initialise the DatFwdInd structure */
         datFwdCnt   =  0;
         PJ_ALLOC(datFwdInd, sizeof(PjuDatFwdIndInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
         if(datFwdInd == NULLP)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR ),
                  (_kwPBuf, "pjUlmReEstDrbAm: Alloc Failed \n"));
            pjUlmDecReEstCnt(pjRbCb);
            MSPD_DBG_RLC("[%d][%d]ReEst failed for DRB\n",
                         pjRbCb->ueCb->key.ueId, pjRbCb->rbId);
			
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

         datFwdInd->dir       =  PJ_DIR_UL;
         datFwdInd->numSdus   =  ulCb->recBuf.numEntries;
         PJ_ALLOC(datFwdInd->datFwdInfo, 
               datFwdInd->numSdus * sizeof(PjuDatFwdInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
         if(datFwdInd->datFwdInfo == NULLP)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR ),
                  (_kwPBuf, "pjUlmReEstDrbAm: Alloc Failed \n"));
            pjUlmDecReEstCnt(pjRbCb);
            MSPD_DBG_RLC("[%d][%d]ReEst failed for DRB\n",
                         pjRbCb->ueCb->key.ueId, pjRbCb->rbId);			
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      } /* if(ulCb->recBuf.numEntries != 0) */
   }/* if(pjRbCb->state == ... */

   /* Initialisations for status report */
   bMapByte = 0;
   bitCnt   = PJ_BYTE_SIZE - 1;
   PJ_ALLOC_BUF(bMapBuf);
   fms      = (ulCb->lastSubCnt + 1) % (1 << pjRbCb->snLen); /*Fix for correct modulo with 4096*/
   nxtRxCnt  =  (ulCb->rxHfn << PJ_DRBAM_SN_LEN) | ulCb->nxtRxSn;
                                 
   /* Loop through and pack the status report and data fwd indication */
   /* This loop should not go through if ulCb->recBuf.numEntries == 0 */
   for(count = (ulCb->lastSubCnt + 2); count < nxtRxCnt; count ++)
   {
      tmpEnt   =  (PjRxEnt *)pjDbmGetRxEnt(&ulCb->recBuf, count);
      if(tmpEnt != NULLP)
      {
         /* Update the bitmap status report */
         bMapByte =  bMapByte | (0x01 << bitCnt);

         /* Update for datafwd indications if handover */
         if(pjRbCb->state == PJ_STATE_REEST_HO)
         {
            /* kw005.201 ccpu00118188 corrected the calculation of SN */
            sn = tmpEnt->count % (1 << pjRbCb->snLen);
            datFwdInd->datFwdInfo[datFwdCnt].sn   =  sn;
            SCpyMsgMsg(tmpEnt->mBuf, NULLD, NULLD,  
                  &datFwdInd->datFwdInfo[datFwdCnt].sdu);
            datFwdCnt++;
            PJ_STS_INC_GEN_CNT(numPktsFrwd);
         }
      }

      /* Update bitCnt and bMapByte */
      bitCnt --;
      if(bitCnt < 0)
      {
         SAddPstMsg(bMapByte, bMapBuf);
         bMapByte = 0;
         bitCnt = PJ_BYTE_SIZE - 1;
      }
   }/* for(count = (pjRbCb->lastSubCnt + 1)... */
   
   /* Add the last byte */
   if(bitCnt != PJ_BYTE_SIZE - 1)
   {
      SAddPstMsg(bMapByte, bMapBuf);
   }

   /* Send the status report */
   pjUlmBldStaRep(pjRbCb, fms, bMapBuf);

   /* Send the dat fwd indications */
   if((pjRbCb->state == PJ_STATE_REEST_HO)&&
      (ulCb->recBuf.numEntries != 0))
      
   {
      /* kw005.201 removed macro and written function to send
       * DatFwdInd mesage.
       * */
      pjUtlSndDatFwdInd(pjRbCb, datFwdInd);
   } 

   /* Mark that this rb is done with reestablishment */
   pjUlmDecReEstCnt(pjRbCb);
   MSPD_DBG_RLC("[%d][%d]ReEst Done for DRB\n", pjRbCb->ueCb->key.ueId,
                 pjRbCb->rbId);			
   RETVALUE(ROK);
}  /* pjUlmReEstDrbAm */


/**
 *
 * @brief 
 *
 *        Function is called to build and send the SDU Status report.
 *
 * @b Description: 
 *
 *        This function is called to build the SDU status report and send it to the 
 *        lower layer after reestablishment.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] fms      First Missing sequence number
 *  @param[in] staReq   Buffer containing status report bitmap
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmBldStaRep
(
PjRbCb   *pjRbCb,             /* !< PDCP Control Block */  
PjSn      fms,                /* !< First Missing SN */  
Buffer   *staRep              /* !< Status Report */       
)
#else
PUBLIC S16 pjUlmBldStaRep(pjRbCb, fms, staRep)
PjRbCb   *pjRbCb;             /* !< PDCP Control Block */   
PjSn      fms;                /* !< First Missing SN */     
Buffer   *staRep;             /* !< Status Report */        
#endif
{
   U8                packByte;         /* Temp Var for packing bytes */
   U32               count;            /* tem */
   MsgLen            len      = 0;     /* Message length */
   S16               ret      =  ROK;  /* Return value */
   MsgLen            cCnt     = 0;     /* counter variable */
   PjHoCfmInfo       *info    = NULLP; /* Ptr to sdu sta info */
   U8                *sduSta  = NULLP; /* Flat buf for bitmap */

   TRC2(pjUlmBldStaRep);

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmBldStaRep(pjRbCb(%d,%d)) \n", \
                     pjRbCb->rbId, pjRbCb->rbType));


   /* convert the buffer to a flat buffer and store it */
   SFndLenMsg(staRep, &len);

   if(pjRbCb->state == PJ_STATE_REEST_HO)
   {
      U16   fmsHfn;
      U32   fmsCnt;
      if(len != 0)
      {
         PJ_ALLOC(sduSta, len);
#if (ERRCLASS & ERRCLS_DEBUG)
         if(sduSta == NULLP)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR ),
                  (_kwPBuf, "pjUlmBldStaRep: Alloc Failed \n"));
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

         ret = SCpyMsgFix(staRep, 0, len, sduSta, &cCnt); 
#if (ERRCLASS & ERRCLS_DEBUG)
         if(ret != ROK)
         {
            PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR ),
                  (_kwPBuf, "pjUlmBldStaRep: Alloc Failed \n"));
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

         /* calc the num of bits in the last byte */
         packByte =  sduSta[len - 1];
         cCnt  =  8;
         while(!(packByte % 2))
         {
            packByte = packByte >> 1;
            cCnt --;
         }

         /* set len for sdusta */
         len = ((len - 1) * 8) + cCnt;

      }/* len != 0 */

      fmsHfn   =  (pjRbCb->ulCb.nxtRxSn >= fms)? pjRbCb->ulCb.rxHfn : (pjRbCb->ulCb.rxHfn - 1);
      PJ_GET_COUNT(fmsCnt, (pjRbCb->snLen), fms, fmsHfn);   

      if(!((pjRbCb->ulCb.nxtRxSn == 0) && (pjRbCb->ulCb.rxHfn == 0)))
      {
         /* Store it in ueCb, to be used while sending SduStaCfm */
         info              = &pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId];
         info->pres        = TRUE;
         info->rbId        = pjRbCb->rbId;
         info->dir        |= PJ_DIR_UL;
         info->ul.numBits  = (U8)len;
         info->ul.ulBitMap = sduSta;
         info->ul.count    =  fmsCnt;
      }
      PJ_FREE_BUF(staRep);
   }
   else /* Normal ReEstablishment */
   {

      /* Pack the 8 LSB  bits of fms directly  */
      /*ccpu00122053: Fix for PDCP status report construction*/
      /*packByte = fms % (1 << (PJ_DRBAM_SN_LEN - PJ_BYTE_SIZE));*/
      packByte = fms & 0x00ff;
      SAddPreMsg(packByte, staRep);

      /* pduType and control bit are zeros */
      packByte = fms >> PJ_BYTE_SIZE;
      SAddPreMsg(packByte, staRep);

      /* This will go to the RLC as a datReq. So we will get a confirm for it
       * we have to make sure that the count does not clash with the existing
       * counts. So pick a value that is least expected to clash. */
      count =  pjRbCb->dlCb.cfmExp - 1;

      ret   =  kwUimDatReq(pjRbCb, count, staRep);
   }
   
   RETVALUE(ret);

}/*pjUlmBldStaRep*/


/**
 *
 * @brief 
 *
 *        Function is called during handover to transfer the undeliverd SDUs
 *        at the target side.
 *
 * @b Description: 
 *
 *        This function performs the following.                           @n
 *        1. Calculate lastSubCnt.                                        @n
 *        2. Compute count for each SDU.                                  @n
 *        3. Create an rxEnt for each SDU and insert it into the hashlist.@n
 *                                               
 *  @param[in] pjRbCb      PDCP control block 
 *  @param[in] datFwdReq   Data Forward Request info
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDatFwdReq
(                                             
PjRbCb         *pjRbCb,                       /* !< PDCP ID */        
PjuDatFwdReqInfo   *datFwdReq                 /* !< DatFwdReq Info */
)
#else
PUBLIC S16 pjUlmHdlDatFwdReq(pjRbCb, datFwdReq)
PjRbCb         *pjRbCb;                       /* !< PDCP ID */        
PjuDatFwdReqInfo   *datFwdReq;                /* !< DatFwdReq Info */ 
#endif
{
   U32            datFwdCnt;         /* Temp value to store SDU count    */
   PjRxEnt       *rxEnt     = NULLP; /* Rx Entry pointer                 */
   PjuDatFwdInfo *info;              /* Loop var - pointer to DatFwdInfo */
   PjUlCb        *ulCb      = NULLP; /* UL Control block                 */
   U8             sduIndex;          /* loop index                       */

   TRC2(pjUlmHdlDatFwdReq);

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlDatFwdReq(pjRbCb(%d,%d), datFwdReq) \n", \
                     pjRbCb->rbId, pjRbCb->rbType));

   /* Initialisations */
   ulCb  =  &pjRbCb->ulCb;

   /* Loop through all SDUs */
   for( sduIndex = 0; sduIndex < datFwdReq->numSdus; sduIndex++)
   {
      info  =  &datFwdReq->datFwdInfo[sduIndex];
      PJ_STS_INC_GEN_CNT(numPktsRcvd);

      /* Compute the count from the SN value  */
      PJ_GET_DATFWD_CNT(pjRbCb, (info->sn), datFwdCnt); 
      /* PJ_ULM_UPD_RX_VAR(info->sn, pjRbCb); */

      /* Create and insert the entry in the buffer */
      PJ_ALLOC(rxEnt, sizeof(PjRxEnt));

#if (ERRCLASS & ERRCLS_DEBUG)
      if(rxEnt == NULLP)
      {
         PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR ),
          (_kwPBuf, "pjUlmHdlDatFwdReq: Failure in allocating an rxEnt\n"));
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      rxEnt->count     = datFwdCnt;
      rxEnt->state     = PJ_RDY_TO_SUBMIT;
      rxEnt->mBuf      = info->sdu;

      /* Insert the entry in the buffer */
      pjDbmInsRxEnt(&ulCb->recBuf, rxEnt, FALSE); 
   }

   RETVALUE(ROK);
}/*pjUlmHdlDatFwdReq*/

/**
 *
 * @brief 
 *
 *        Function is called when 
 *        1. The offboard timer expires.
 *        2. Deciphering returns. 
 *
 * @b Description: 
 *
 *    The function is called when the pdu is next in line for decompression
 *    It performs the following.                                           @n
 *    1. Submit the pdu for decompression.                                 @n
 *    2. Check if any successive messages can be sent to for decompression @n
 *    3. Update the nxtSubDeCmp value appropriately .                      @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Reception Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmSubmitForDecmp
(
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* Ptr To Rx Entry */
)
#else
PUBLIC S16 pjUlmSubmitForDecmp(pjRbCb, rxEnt)
PjRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* Ptr To Rx Entry */
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
#ifdef PJ_SEC_ASYNC
   U32     count;          /* count to go through the list and check */
   U32     nxtRxCnt;       /* count of Sdu to receive next */
   PjRxEnt *tmpEnt;        /* Temporary variable for looping */
#endif /* PJ_SEC_ASYNC */

   TRC2(pjUlmSubmitForDecmp);

#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmSubmitForDecmp(pjRbCb(%d,%d),rxEnt(%ld)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmSubmitForDecmp(pjRbCb(%d,%d),rxEnt(%d)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, rxEnt->count));
#endif

   /* 1. Initialisations */
   ulCb  =  &pjRbCb->ulCb;
   rxEnt->state = PJ_RDY_TO_DCOMP;

   /* 2. If decomp is disabled, start delivery procedures */
   if(pjRbCb->rohc.hdrCmpUsed != TRUE)
   {
      rxEnt->state   =  PJ_RDY_TO_SUBMIT;
      PJ_DEC_OBD_COUNT(pjRbCb, rxEnt->count);
      /* kw006.201 ccpu00118629, replaced macro with function */
      pjUlmDeliverDrb(pjRbCb, rxEnt);
      RETVALUE(ROK);
   }

   /* 3. If not the next in line for decompression, wait.. */
   if(ulCb->nxtSubDeCmp != rxEnt->count)
   {
      RETVALUE(ROK);
   }

   /* 4. Calling decompression hook */
   if(pjUlmHdlDeCmp(pjRbCb, rxEnt) != ROK)
   {
      RETVALUE(RFAILED);
   }

#ifdef PJ_SEC_ASYNC
   /* 5. cycle through the subsequent entries to see if any of them are also 
    * available for decompression */
   PJ_GET_COUNT(nxtRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));

   for(count =  ulCb->nxtSubDeCmp + 1; count < nxtRxCnt; count ++)
   {
      /* 5.1 Get the next node */
      tmpEnt   =  pjDbmGetRxEnt(&ulCb->recBuf, count);

      /* 5.2 If such an entry does not exist, search for the next */
      if(tmpEnt == NULLP)
      {
         continue;
      }

      /* 5.3 During post reestablishment phase, there might me some PDUs in 
       * the Rx Buffer that were received prior to reestablishment and 
       * have been fully processed but are not sent up because they are 
       * waiting for the arrival of a previous PDU. We can  safely skip 
       * such an entry */
      if(tmpEnt->state == PJ_RDY_TO_SUBMIT)
      {
         continue;
      }

      /* 5.4 Deciphering not yet done so we have to wait */
      if(tmpEnt->state != PJ_RDY_TO_DCOMP)
      {
         break;
      }

      /* 5.5 Send it to decompression */
      if(pjUlmHdlDeCmp(pjRbCb, tmpEnt) != ROK)
      {
         RETVALUE(RFAILED);
      }

   } /* while(!((sn == ulCb->nxtRxSn)... */

   /* 6. Update nxtSubDeCmp */
   ulCb->nxtSubDeCmp = count;

#endif /* PJ_SEC_ASYNC */

   RETVALUE(ROK);

}/* end of pjUlmSubmitForDecmp */

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
/**
 *
 * @brief 
 *
 *        Function is called when the offboard timer expires.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. If the SDU associated with the timer is not delivered, we 
 *       delete the entry .                                              @n
 *    2. A status indication is sent to the user with error cause.       @n
 *    3. In case of failure call pjUlmHdlErrUpdates to recalculate
 *       nxtSubCnt and if necessary nxtSubDeCmp.                         @n
 *    4. The timer is associated with the nextToSubmit SDU and restarted.@n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC Void pjUlmHdlObdTmrExpiry
(
PjRbCb     *pjRbCb         /* !< PDCP Control Block */  
)
#else
PUBLIC Void pjUlmHdlObdTmrExpiry(pjRbCb)
PjRbCb     *pjRbCb;        /* !< PDCP Control Block */  
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
   PjRxEnt *rxEnt;         /* Ptr To Rx Entry */
   U32      subCnt;        /* Count that was/will be submitted */

   TRC2(pjUlmHdlObdTmrExpiry);

   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlObdTmrExpiry(pjRbCb(%d,%d)) \n", \
                     pjRbCb->rbId, pjRbCb->rbType));

   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;
   PJ_STS_INC_GEN_CNT(numPdusDiscObdTmrExp);


   /* 2. For SDUs that have not yet been delivered */
   pjUlmGetSubCnt(pjRbCb, &subCnt);
   if(ulCb->obdPdu == subCnt)
   {
      /* 2.1 Send failure indication  and delete the entry */
      rxEnt =  (PjRxEnt *)pjDbmGetRxEnt(&(ulCb->recBuf), ulCb->obdPdu);
      if(rxEnt == NULLP)
      {
         if( ulCb->recBuf.numEntries == 0)
         {
            RETVOID;
         }
#if (ERRCLASS & ERRCLS_DEBUG)
         PJLOGERROR(ERRCLS_ADD_RES, EKW114, (ErrVal)0,       \
               "pjUlmHdlObdTmrExpiry: rxEnt Not Present \n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         RETVOID;
      }
      else
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW115, (ErrVal)ulCb->obdPdu,\
               "Obd Timer exipires without reply ");
         /* kw006.201 replaced macro with function */
         pjUlmSndPjuStaInd(pjRbCb, rxEnt);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
      }

      /* 2.2 Update nxtSubCnt */
      pjUlmHdlErrUpdates(pjRbCb, ulCb->obdPdu);

      /* 2.3 Restart timer if it has not been restarted already */
      if( ulCb->recBuf.numEntries != 0)
      {
         if((pjChkTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
         {
            pjUlmGetSubCnt(pjRbCb, &(ulCb->obdPdu));
            if(!PJ_DRBAM_ALL_PDU_RECVD(pjRbCb))
            {
               pjStartTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
            }
         }
      }
   }
   /* 3. For SDUs that have been delivered */
   else
   {
      if( ulCb->recBuf.numEntries != 0)
      {
         /* 3.1 Associate the nxtToSub with the timer */
         pjUlmGetSubCnt(pjRbCb, &(ulCb->obdPdu));
         if(!PJ_DRBAM_ALL_PDU_RECVD(pjRbCb))
         {
            pjStartTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
         }
      }
   }

   RETVOID;
}/* end of pjUlmHdlObdTmrExpiry */
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */


/**
 *
 * @brief 
 *
 *        Function is called when 
 *        1. The offboard timer expires 
 *        2. The library functions fail.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. Update nxtSubCnt if necessary.
 *    2. Update nxtSubDeCmp if necessary.
 *    3. Check if reestablishment has to be triggered.
 *                                               
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] errCnt   Count of the error PDU.
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmHdlErrUpdates
(
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
U32         errCnt         /* !< Count of PDU that raises the error */
)
#else
PUBLIC Void pjUlmHdlErrUpdates(pjRbCb, errCnt)
PjRbCb     *pjRbCb;        /* !< PDCP Control Block */  
U32         errCnt;        /* !< Count of PDU that raises the error */
#endif
{
#ifdef PJ_SEC_ASYNC
   U32      nxtToDeCmp;
#endif /* PJ_SEC_ASYNC */

#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   U32      nxtToSub;
   PjRxEnt *rxEnt = NULLP;   /* Ptr To Rx Entry */
   /* kw006.201, ccpu00120574, fixed the warning */
   PjUlCb  *ulCb;          /* ULM Control Block */
   U32      nxtRxCnt;
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */

   TRC2(pjUlmHdlErrUpdates);

#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlErrUpdates(pjRbCb(%d,%d),errCnt(%ld)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, errCnt));
#else
   PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_DETAIL ),
         (_kwPBuf, "pjUlmHdlErrUpdates(pjRbCb(%d,%d),errCnt(%d)) \n", \
          pjRbCb->rbId, pjRbCb->rbType, errCnt));
#endif

   /* kw006.201, ccpu00120574, fixed the warning */
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   /* 1. Initialistions */
   ulCb       =  &pjRbCb->ulCb;
#endif

   /* 2. Decrement obd counter */
   PJ_DEC_OBD_COUNT(pjRbCb, errCnt);

#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   /* kw006.201, ccpu00120574, fixed the warning */
   PJ_GET_COUNT(nxtRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));
   /* 3. For DRB AM , check if we can deliver anything  */
   if((pjRbCb->rbType == PJ_DRB) &&  (pjRbCb->mode == PJ_DRB_AM))
   {
      /* if any successive entries can be sent up , send it */
      for(nxtToSub  =  ulCb->lastSubCnt + 1; nxtToSub < nxtRxCnt; nxtToSub++)
      {
         if( (rxEnt = pjDbmGetRxEnt(&ulCb->recBuf, nxtToSub)) != NULLP)
         {
            /* As soon as we get an entry, we break */
            if(rxEnt->state == PJ_RDY_TO_SUBMIT)
            {
               pjUlmDeliverDrbAm(pjRbCb, rxEnt);
            }
            break;
         }

      }
      if(nxtToSub == nxtRxCnt)
      {
         /* This situation occurs, if this is the only pdu in the hashlist 
          * and obd fails for it. In this case, we have to trigger reestablishment
          * procedures */
         if(pjRbCb->state != PJ_STATE_NORMAL)
         {
            if((ulCb->transCmp == TRUE) && 
                  (ulCb->obdCnt == 0))
            {
               PJ_ULM_DRBUM_REEST(pjRbCb);
            }
         }
      }
   }
   /* 4. For SRB/DRB UM Update nxtSubCnt */
   else
   {
      if(errCnt ==  ulCb->nxtSubCnt)
      {
         for(nxtToSub  =  ulCb->nxtSubCnt + 1; nxtToSub < nxtRxCnt; nxtToSub++)
         {
            if( (rxEnt = pjDbmGetRxEnt(&ulCb->recBuf, nxtToSub)) != NULLP)
            {
               ulCb->nxtSubCnt = nxtToSub;
               break;
            }
         }
         if(nxtToSub == ulCb->nxtSubCnt )
         {
            /*kw006.201:Klocworks, Added NULL check for rxEnt*/
            if(rxEnt && (rxEnt->state == PJ_RDY_TO_SUBMIT) )
            {
               /* kw006.201 ccpu00118629, replaced macro with function */
               pjUlmDeliverPdu(pjRbCb, rxEnt);
            }
            else
            {
               PJDBGP((KW_PJ_DBGMASK_ULM | KW_DBGMASK_ERROR),
                      (_pjPBuf,"Pdu could not be delivered to upper layer\n"));
            }
         }
         else
         {
            /* Update nxtSubCnt to point to nxtRxCnt so that 
             * updations at pjUlmProcessDrbUm/Srb can happen properly */
            ulCb->nxtSubCnt   =  nxtRxCnt;

            /* Check if its necessary to reestablish */
            if(pjRbCb->state != PJ_STATE_NORMAL)
            {
               if((ulCb->transCmp == TRUE) && 
                     (ulCb->obdCnt == 0))
               {
                  PJ_ULM_DRBUM_REEST(pjRbCb);
               }
            }
         }
      }
   }

#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */

   /* Notes : Update nxSubCnt first and then nxtSubDeCmp because we dont want a case
    * where nxtSubDeCmp becomes greater that nxtSubCnt */
#ifdef PJ_SEC_ASYNC
   /* 5. Update nxtSubDeCmp */
   if((pjRbCb->rbType == PJ_DRB) && (errCnt == ulCb->nxtSubDeCmp))
   {
      nxtToDeCmp  =  ulCb->nxtSubDeCmp + 1;

      for(; nxtToDeCmp < nxtRxCnt; nxtToDeCmp++)
      {
         rxEnt = pjDbmGetRxEnt(&ulCb->recBuf, nxtToDeCmp);

         if(rxEnt == NULLP)
         {
            continue;
         }

         /* A pdu existing after reestablishment */
         if(rxEnt->state == PJ_RDY_TO_SUBMIT)
         {
            continue;
         }

         ulCb->nxtSubDeCmp = nxtToDeCmp;
         break;
      }

      if(nxtToDeCmp == ulCb->nxtSubDeCmp)
      {
         if(rxEnt && (rxEnt->state == PJ_RDY_TO_DCOMP))
         {
            pjUlmSubmitForDecmp(pjRbCb, rxEnt);
         }
      }
      else
      {
         /* Update nxtSubDeCmp to point to nxtRxCnt so that 
          * updations at pjUlmProcessDrbUm/Am can happen properly */
         ulCb->nxtSubDeCmp =  nxtRxCnt;

      }
   }
#endif /* PJ_SEC_ASYNC */

   RETVOID;
}/* end of pjUlmHdlErrUpdates */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))

/* kw006.201 ccpu00118629, replaced macro with function */
/**
 *
 * @brief 
*        Whether in normal or in reestablishment state, 
*        it is possible that the pdu  has returned from offboarding 
*        but has not been sent up. So check the state is 
*        at RDY_TO_SUBMIT. In normal case, immediately after reestablishment 
*        all the pdus in the recBuf can be at RDY_TO_SUBMIT state. 
*        In this case, the timer must not be restarted. For this to be detected, 
*        we set the count to lastSubCnt from the RBCB
 *
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] subPdu   sub pdu
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmGetDrbAmSubCnt
(
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
U32    *nxtSubCnt  /* !<Count that was/will be submitted */
)
#else
PUBLIC Void pjUlmGetDrbAmSubCnt(pjRbCb, subPdu)
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
U32    *nxtSubCnt  /*!< Count that was/will be submitted */
#endif
{
   U32 count    = pjRbCb->ulCb.lastSubCnt;
   U32 nxtRxCnt  =  (pjRbCb->ulCb.nxtRxSn << PJ_DRBAM_SN_LEN) 
                     | pjRbCb->ulCb.rxHfn;
   PjRxEnt *rxEnt = NULLP; 
   *nxtSubCnt      = count;

   TRC2(pjUlmGetDrbAmSubCnt);

   if(((pjRbCb->state != PJ_STATE_NORMAL) &&
       (pjRbCb->ulCb.obdCnt != 0)) ||
      ((pjRbCb->state == PJ_STATE_NORMAL) &&
       (pjRbCb->ulCb.recBuf.numEntries != 0)))
   {
      count++;
      for(;count < nxtRxCnt; count ++)
      {
         rxEnt   = (PjRxEnt *)pjDbmGetRxEnt(&pjRbCb->ulCb.recBuf, count);
         if((rxEnt != NULLP) && (rxEnt->state != PJ_RDY_TO_SUBMIT))
         {
            *nxtSubCnt  =  count;
            break;
         }
      }
   }
   RETVOID;
}/* End of of pjUlmGetDrbAmSubCnt */

/**
 *
 * @brief 
 *       This function is called to get the sub cnt
 *       from the RBCB
 *
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] subPdu   sub pdu
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmGetSubCnt
(
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
U32    *subPdu      /* Count that was/will be submitted */
)
#else
PUBLIC Void pjUlmGetSubCnt(pjRbCb, subPdu)
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
U32    *subPdu      /* Count that was/will be submitted */
#endif
{
   TRC2(pjUlmGetSubCnt);

   if((pjRbCb->rbType == PJ_DRB) && (pjRbCb->mode  == PJ_DRB_AM))
   {
      pjUlmGetDrbAmSubCnt(pjRbCb, subPdu);
   }
   else
   {
     *subPdu =  pjRbCb->ulCb.nxtSubCnt;
   }

   RETVOID;
}/* End of function pjUlmGetSubCnt */
#endif /* PJ_SEC_ASYNC */

/**
 *
 * @brief 
 *       Update state and call the respective 
 *       delivery function.
 *
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] rxEnt    Rx entry
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmDeliverPdu
(
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt *rxEnt    /* Ptr To Rx Entry */
)
#else
PUBLIC Void pjUlmDeliverPdu(pjRbCb, rxEnt)
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt *rxEnt;    /* Ptr To Rx Entry */
#endif
{
   TRC2(pjUlmDeliverPdu);

   if(pjRbCb->rbType == PJ_SRB)
   {
      pjUlmDeliverSrb(pjRbCb, rxEnt);
   }
   else
   {
      pjUlmDeliverDrb(pjRbCb, rxEnt);
   }
}/* End of function pjUlmDeliverPdu */

/**
 *
 * @brief 
 *       Update state and call the respective 
 *       delivery function.
 *
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] rxEnt    Rx entry
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmDeliverDrb
(
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt *rxEnt     /* Ptr To Rx Entry */
)
#else
PUBLIC Void pjUlmDeliverDrb(pjRbCb, rxEnt)
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt *rxEnt;    /* Ptr To Rx Entry */
#endif
{
   TRC2(pjUlmDeliverDrb);

   if(pjRbCb->mode == PJ_DRB_AM)
   {
      pjUlmDeliverDrbAm(pjRbCb, rxEnt);
   }
   else
   {
      pjUlmDeliverDrbUm(pjRbCb, rxEnt);
   }
}/* end of pjUlmDeliverDrb */

/**
 *
 * @brief 
 *
 *  @param[in] pjRbCb   PDCP control block.
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmDecReEstCnt
(
PjRbCb *pjRbCb    /* !< PDCP Control Block */  
)
#else
PUBLIC Void pjUlmDecReEstCnt(pjRbCb)
PjRbCb *pjRbCb;    /* !< PDCP Control Block */  
#endif
{
   TRC2(pjUlmDecReEstCnt);
   pjRbCb->ueCb->libInfo.numReEstDrbUl --;
   if(pjRbCb->ueCb->libInfo.numReEstDrbUl == 0)
   {
      if(pjRbCb->ueCb->libInfo.state == PJ_STATE_REEST)
      {
         pjUtlReEstReconfig(pjRbCb->ueCb, pjRbCb->rbType);
      }
      else if(pjRbCb->ueCb->libInfo.state == PJ_STATE_REEST_HO)
      {
         pjUtlReEstDl(pjRbCb->ueCb);
      }
   }
}/* end of function pjUlmDecReEstCnt */
#ifdef PJ_SEC_ASYNC 
/**
 *
 * @brief 
 *
 *  @param[in] pjRbCb   PDCP control block.
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmChkRdyToDeliver
(
PjRbCb *pjRbCb    /* !< PDCP Control Block */  
)
#else
PUBLIC Void pjUlmChkRdyToDeliver(pjRbCb)
PjRbCb *pjRbCb;    /* !< PDCP Control Block */  
#endif
{
   PjUlCb   *ulCb;
   PjRxEnt  *rxEnt;
   
   TRC2(pjUlmChkRdyToDeliver);
   ulCb  =  &pjRbCb->ulCb;
   if(ulCb->recBuf.numEntries == 0)
   {
      if((pjRbCb->rbType   ==  PJ_DRB)&&
         (pjRbCb->state != PJ_STATE_NORMAL))
      {
            if((ulCb->transCmp)  && 
               (ulCb->obdCnt == 0))
            {
               pjUlmReEstDrbAm(pjRbCb);
            }
      }
   }
   else
   {
     rxEnt  =  (PjRxEnt *)pjDbmGetRxEnt(&ulCb->recBuf, ulCb->nxtSubCnt);
     if( rxEnt && rxEnt->state == PJ_RDY_TO_SUBMIT)
     {
        if(pjRbCb->rbType == PJ_SRB)
        {
           pjUlmDeliverSrb(pjRbCb, rxEnt);
        }
        else
        {
           if(pjRbCb->mode   == PJ_DRB_UM)
           {
              pjUlmDeliverDrbUm(pjRbCb, rxEnt);
           }
           else
           {
              pjUlmDeliverDrbAm(pjRbCb, rxEnt);
           }
        }
     }
   }

   RETVOID;
}/* end of pjUlmChkRdyToDeliver */                                                          
#endif
/**
 *
 * @brief 
 *
 *  @param[in] pjRbCb   PDCP control block.
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmSndPjuDatInd
(
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt  *rxEnt    /* !<Ptr to Rx Entry     */
)
#else
PUBLIC Void pjUlmSndPjuDatInd(pjRbCb)
PjRbCb *pjRbCb;    /* !< PDCP Control Block */  
PjRxEnt  *rxEnt;   /* !<Ptr to Rx Entry     */
#endif
{
   CmLtePdcpId  *pdcpId = NULLP;
   Buffer       *mBuf;
   KwPjuSapCb   *pjuSap;
   
   TRC2(pjUlmSndPjuDatInd);

   if (pjRbCb->rbType == PJ_SRB)
      pjuSap   = &(kwCb.pjuSap[PJ_SRB_SAP]);
   else
      pjuSap   = &(kwCb.pjuSap[PJ_DRB_SAP]);
   PJ_ALLOC(pdcpId, sizeof(CmLtePdcpId));
   if(pdcpId != NULLP)
   {
      pdcpId->ueId   =  pjRbCb->ueCb->key.ueId;
      pdcpId->cellId =  pjRbCb->ueCb->key.cellId;
      pdcpId->rbType =  pjRbCb->rbType;
      pdcpId->rbId   =  pjRbCb->rbId;
      mBuf           =   rxEnt->mBuf;
      rxEnt->mBuf =  NULLP;
      /* If trace flag is enabled send the trace indication */
      if(kwCb.init.trc == TRUE)
      {
         /* Populate the trace params */
         pjLmmSendTrc(EVTPJUDATIND, mBuf);
      }
#if 1
    {
#ifdef MSPD
      U32 t = GetTIMETICK();
#endif
      PjUiPjuDatInd(&(pjuSap->pst), pjuSap->suId, 
             pdcpId, mBuf);
#ifdef MSPD_MLOG_NEW
	  MLogTask(PID_PDCP_DAT_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
    }
#else
   /* dropping the uplink data packets by not sending it to Application */
   if (pjRbCb->rbType != PJ_SRB)
   {
      SPutMsg(mBuf);
   }
   else
   {	   
      PjUiPjuDatInd(&(pjuSap->pst), pjuSap->suId, 
             pdcpId, mBuf);
   }   
#endif   
      
      PJ_STS_INC_PJU_CNT(pjRbCb->rbType, txSdus);
   }
}/* end of pjUlmSndPjuDatInd */

/**
 *
 * @brief 
 *
 *  @param[in] pjRbCb   PDCP control block.
    @param[in] PjRxEnt  *rxEnt  
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmSndPjuStaInd
(
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt  *rxEnt    /* !<Ptr to Rx Entry     */
)
#else
PUBLIC Void pjUlmSndPjuStaInd(pjRbCb)
PjRbCb *pjRbCb;    /* !< PDCP Control Block */  
PjRxEnt  *rxEnt;   /* !<Ptr to Rx Entry     */
#endif
{
   CmLtePdcpId  *pdcpId;
   Buffer       *mBuf = NULLP;
   PjuStaIndInfo  *staInd;
   KwPjuSapCb    *pjuSap;

   TRC2(pjUlmSndPjuStaInd)

   if (pjRbCb->rbType == PJ_SRB)
      pjuSap   = &(kwCb.pjuSap[PJ_SRB_SAP]);
   else
      pjuSap   = &(kwCb.pjuSap[PJ_DRB_SAP]);

   PJ_ALLOC(staInd, sizeof(PjuStaIndInfo));
   PJ_ALLOC(pdcpId, sizeof(CmLtePdcpId));
   if(staInd != NULLP && pdcpId != NULLP)
   {
      pdcpId->ueId   =  pjRbCb->ueCb->key.ueId;
      pdcpId->cellId =  pjRbCb->ueCb->key.cellId;
      pdcpId->rbId   =  pjRbCb->rbId;
      pdcpId->rbType =  pjRbCb->rbType;
      switch (rxEnt->state)
      {
         case PJ_RDY_TO_DCIPHER:
         case PJ_SENT_TO_DCIPHER:
         {
            staInd->cause  =  PJU_DCIP_FAILED;
            break;
         }
         case PJ_RDY_TO_INTVER:
         case PJ_SENT_TO_INTVER:
         {
            staInd->cause  =  PJU_INTVER_FAILED;
            if(pjRbCb->ulCb.discReqd == FALSE)
            {
               mBuf          =   rxEnt->mBuf;
               rxEnt->mBuf    =  NULLP;
            }
            break;
         }
         case PJ_RDY_TO_DCOMP:
         case PJ_SENT_TO_DCOMP:
         {
            staInd->cause  =  PJU_DCOMP_FAILED;
            if(pjRbCb->ulCb.discReqd == FALSE)
            {
               mBuf          =  rxEnt->mBuf;
               rxEnt->mBuf    =  NULLP;
            }
            break;
         }
         default:
            break;
      }
      PjUiPjuStaInd(&(pjuSap->pst), pjuSap->suId,
                    pdcpId, staInd, mBuf);
   }
}/* end of pjUlmSndPjuStaInd */
/*@}*/

/********************************************************************30**
  
         End of file:     gp_pj_ulm.c@@/main/1 - Fri Nov 13 14:13:50 2009
  
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
/main/1      ---      mn   1. LTERLC Release 2.1
/main/2     kw005.201 ap   1. Added support for L2 Measurment.
                           2. Removed macro and written function 
                             to send DatFwdInd message
/main/3    kw006.201  ap   1. Replaced macro with function.                    
                           2. ccpu00120574, fixed the warning.
                           3. ccpu00120760 fixing the off board 
                              security/compression error with 
                              nxtSubCnt due to missing compile flag.
*********************************************************************91*/

