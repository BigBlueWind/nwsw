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

     Name:     LTE-RLC Layer 
  
     Type:     C file
  
     Desc:     Source code for PDCP Utility Module

               This file contains following functions:

                  --PjLibObdCmpCfm
                  --PjLibObdIntProtCfm
                  --PjLibObdCipherCfm
                  --PjLibObdDecipherCfm
                  --PjLibObdIntVerCfm
                  --PjLibObdDecmpCfm
                  -- pjUtlCmpReq
                  -- pjUtlCipherReq
                  -- pjUtlIntProtReq
  
     File:     gp_pj_utl.c

     Sid:      gp_pj_utl.c@@/main/1 - Fri Nov 13 14:13:52 2009

     Prg:      nm 

**********************************************************************/
/** @file gp_pj_utl.c
@brief PDCP Utility Module 
*/

/* header (.h) include files */
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
#include "ckw.h"
#include "kwu.h"
#include "lkw.h"
#include "rgu.h"
#include "cpj.h"                /* CPJ defines */
#include "pju.h"                /* PJU defines */
#include "lpj.h"                /* LPJ defines */

#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_err.h"        /* Error defines */

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
#include "ckw.x"
#include "kwu.x"
#include "lkw.x"
#include "rgu.x"
#include "cpj.x"                /* CPJ */
#include "pju.x"                /* PJU */
#include "lpj.x"                /* LPJ */
#include "kw.x"
#include "kw_pj_lib.x"          /* LIB */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/** @addtogroup dldata */
/*@{*/

#ifdef PJ_CMP_ASYNC
/********************************************************************
 *    Input Handler from Offboarding unit                           *
 *******************************************************************/
/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        compression for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the RbCb.
 *
 *  @param[in]  pst      Post structure 
 *  @param[in]  cmpCxt   Context for ROHC
 *  @param[in]  ctxId    Context Id
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCmpInitCfm
(
Pst      *pst,                    /* post structure */
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PTR      ctxId                    /* Compression Context ID */
)
#else
PUBLIC S16 PjLibObdCmpInitCfm(pst, cmpCxt, ctxId)
Pst      *pst;                    /* post structure */
PjCmpCxt cmpCxt;                  /* Context for ROHC */
PTR      ctxId;                   /* Compression Context ID */
#endif
{                                 
   KwUeCb   *ueCb = NULLP;         /* UeCb control block */
   PjRbCb   *pjRbCb = NULLP;       /* PDCP RB control block */

   TRC3(PjLibObdCmpInitCfm);

   PJDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), 
        (_kwPBuf, "PjLibObdCmpInitCfm(pst, cmpCxt(%d,%d,%d,%d), ctxId(%ld))\n",\
           cmpCxt.ueId, cmpCxt.rbId, cmpCxt.rbType, cmpCxt.cfgTxId, ctxId));

   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   kwDbmFetchUeCb(cmpCxt.ueId, cmpCxt.cellId, &ueCb);
   if(ueCb  == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW116, (ErrVal)0,
            "PjLibObdCmpInitCfm: UeCb Nullp \n");
      RETVALUE(RFAILED);
   }

   kwDbmFetchPjRbCb(ueCb, cmpCxt.rbId, cmpCxt.rbType,&pjRbCb);
   if(pjRbCb  == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW117, (ErrVal)0,
            "PjLibObdCmpInitCfm: pjRbCb Nullp \n");
      RETVALUE(RFAILED);
   }

   pjRbCb->cmpCxtId  =  ctxId;

   /* Check if all confirms have been received */
   pjUtlHdlObdInitCfm(ueCb, cmpCxt.cfgTxId, PJ_CMP_INIT_CFM,  cmpCxt.rbId);

   RETVALUE(ROK);

} /* end of PjLibObdCmpInitCfm */
#endif /* PJ_SEC_ASYNC */

#ifdef PJ_SEC_ASYNC

/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        Integrity for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the UeCb.
 *
 *  @param[in]  pst        Post structure 
 *  @param[in]  secCxt     Context for Integrity protection/verification 
 *  @param[in]  ctxId      Integrity Context Id
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdIntInitCfm
(
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Integrity Context ID */
)
#else
PUBLIC S16 PjLibObdIntInitCfm(pst, secCxt, ctxId)
Pst      *pst;                    /* post structure */
PjSecCxt secCxt;                  /* Secutriy Context */
PTR      ctxId;                   /* Integrity Context ID */
#endif
{                                 
   KwUeCb *ueCb;                  /* Ue Control Block */

   TRC3(PjLibObdIntInitCfm)

   PJDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), 
        (_kwPBuf, "PjLibObdCmpInitCfm(pst, secCxt(%d,%d,%d), ctxId(%ld))\n",\
           secCxt.cellId, secCxt.ueId, secCxt.cfgTxId, ctxId));

   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }


   /* Fetch UeCb */
   kwDbmFetchUeCb(secCxt.ueId, secCxt.cellId, &ueCb);
   if(ueCb  == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW118, (ErrVal)0,
            "PjLibObdIntInitCfm: ueCb NULLP \n");
      RETVALUE(RFAILED);
   }

   /* Update context Id */
   ueCb->secInfo.intCxtId = ctxId;

   pjUtlHdlObdInitCfm(ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                                             PJ_LIB_INT_BIT_MASK);

   RETVALUE(ROK);

} /* end of PjLibObdIntInitCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        Ciphering Cp Key  for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the UeCb.
 *
 *  @param[in]  pst        Post structure 
 *  @param[in]  secCxt     Context for Ciphering 
 *  @param[in]  ctxId      Ciphering CP Context Id
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCpInitCfm
(
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
)
#else
PUBLIC S16 PjLibObdCpInitCfm(pst, secCxt, ctxId)
Pst      *pst;                    /* post structure */
PjSecCxt secCxt;                  /* Secutriy Context */
PTR      ctxId;                   /* Ciphering Context ID */
#endif
{
   KwUeCb *ueCb;                 /* Ue Control Block */

   TRC3(PjLibObdCpInitCfm);

   PJDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), 
          (_kwPBuf, "PjLibObdCpInitCfm(pst, secCxt(%d,%d,%d), ctxId(%ld))\n",\
           secCxt.cellId, secCxt.ueId, secCxt.cfgTxId, ctxId));

   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }


   kwDbmFetchUeCb(secCxt.ueId, secCxt.cellId, &ueCb);
   if(ueCb  == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW119, (ErrVal)0,
            "PjLibObdCpInitCfm: ueCb NULLP \n");
      RETVALUE(RFAILED);
   }

   ueCb->secInfo.cpCxtId =  ctxId;

   /* Update bit mask and check if all confirms have been received */
   pjUtlHdlObdInitCfm(ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                        (U8)PJ_LIB_CP_CIP_BIT_MASK);

   RETVALUE(ROK);

} /* end of PjLibObdCpInitCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        Ciphering Up Key  for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the UeCb.
 *
 *  @param[in]  pst        Post structure 
 *  @param[in]  secCxt     Context for Ciphering 
 *  @param[in]  ctxId      Ciphering UP Context Id
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdUpInitCfm
(
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
)
#else
PUBLIC S16 PjLibObdUpInitCfm(pst, secCxt, ctxId)
Pst      *pst;                    /* post structure */
PjSecCxt secCxt;                  /* Secutriy Context */
PTR      ctxId;                   /* Ciphering Context ID */
#endif
{
   KwUeCb *ueCb;                 /* Ue Control Block */

   TRC3(PjLibObdUpInitCfm);

   PJDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), 
          (_kwPBuf, "PjLibObdUpInitCfm(pst, secCxt(%d,%d,%d), ctxId(%ld))\n",\
           secCxt.cellId, secCxt.ueId, secCxt.cfgTxId, ctxId));

   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   kwDbmFetchUeCb(secCxt.ueId, secCxt.cellId, &ueCb);
   if(ueCb  == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW120, (ErrVal)0,
            "PjLibObdUpInitCfm: ueCb NULLP \n");
      RETVALUE(RFAILED);
   }

   ueCb->secInfo.upCxtId =  ctxId;

   /* Update bit mask and check if all confirms have been received */
   pjUtlHdlObdInitCfm(ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                        (U8)PJ_LIB_UP_CIP_BIT_MASK);

   RETVALUE(ROK);

} /* end of PjLibObdUpInitCfm */

#endif /* PJ_SEC_ASYNC */

#ifdef PJ_CMP_ASYNC
/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the compressed SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from compression unit and 
 *        sends the same for ciphering with relevant input paramenters.
 *
 *  @param[in]   pst          Post structure 
 *  @param[in]   cmpCxtId     Context Id for compression 
 *  @param[in]   libTrans     Transaction Id 
 *  @param[in]   opSdu        compressed SDU 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCmpCfm
(
Pst        *pst,        /* post structure             */
PTR         cmpCxtId,   /* Context Id for compression */
PjLibTrans  libTrans,   /* Transaction Id             */
Buffer     *opSdu       /* compressed SDU             */
)
#else
PUBLIC S16 PjLibObdCmpCfm(pst, cmpCxtId, libTrans, opSdu)
Pst        *pst;        /* post structure             */
PTR         cmpCxtId;   /* Context Id for compression */
PjLibTrans  libTrans;   /* Transaction Id             */
Buffer     *opSdu;      /* compressed SDU             */
#endif
{
   PjRbCb      *pjRbCb;
   Buffer      *ciphSdu;
   PjTxEnt     *txEnt;
   S16         ret;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC3(PjLibObdCmpCfm)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
          (_kwPBuf, "PjLibObdCmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%d))\n",\
           cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
         (_kwPBuf, "PjLibObdCmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%ld))\n",\
           cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#endif


   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }

   /* kw006.201 ccpu00120502 corrected the RB ID calculation */
   rbId =  libTrans.rbId;
   kwDbmFetchPjRbCb(libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   ciphSdu = NULLP;
   ret     = ROK;

   if ( pjRbCb == NULLP )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW121, (ErrVal)libTrans.count,
         "PjLibObdCmpCfm: pjRbCb is NULLP \n");
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(ret);
   }

   txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), libTrans.count);
   if ( txEnt == NULLP )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW122, (ErrVal)libTrans.count,
        "PjLibObdCmpCfm: txEnt is NULLP \n");
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   if ( opSdu == NULLP )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW123, (ErrVal)libTrans.count,
        "PjLibObdCmpCfm: opSdu is NULLP:Compression Failed \n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      txEnt->sdu = NULLP;
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(pjRbCb, txEnt, PJU_COMP_FAILED);
      }
      else
      {
          pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), txEnt->count);
      }
      kwCb.pjGenSts.numCmpFails++;
      RETVALUE(RFAILED);
   }

   txEnt->pdu    = opSdu;
   if ( pjRbCb->ueCb->secInfo.secAct )
   {
      ret = pjDlmHdlCiph(pjRbCb, txEnt);
      if ( ret != ROK )
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW124, (ErrVal)libTrans.count,
            "PjLibObdCmpCfm: pjUtlCipherReq failed \n");
         RETVALUE(RFAILED);
      }
#ifdef PJ_SEC_ASYNC
      RETVALUE(ret);
#endif
   }
   ret = pjDlmDeliverPdu(pjRbCb, txEnt);
   
   RETVALUE(ret);
} /* end of PjLibObdCmpCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the decompressed SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from decompression unit and 
 *        sends the SDU to upper layer in sequence. 
 *
 *  @param[in] pst         post structure
 *  @param[in] cmpCxtId    Context Id for decompression
 *  @param[in] libTrans    Transaction Id
 *  @param[in] opSdu       decompressed SDU
 *  @param[in] fbPkt       Feedback Pkt
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdDecmpCfm
(
Pst         *pst,                 /* post structure */
PTR         cmpCxtId,             /* Context Id for decompression */
PjLibTrans  libTrans,             /* Transaction Id */
Buffer      *opSdu,               /* decompressed SDU */
Buffer      *fbPkt                /* Feedback pkt */
)
#else
PUBLIC S16 PjLibObdDecmpCfm(pst, cmpCxtId, libTrans, opSdu, fbPkt)
Pst         *pst;                 /* post structure */
PTR         cmpCxtId;             /* Context Id for decompression */
PjLibTrans  libTrans;             /* Transaction Id */
Buffer      *opSdu;               /* decompressed SDU */
Buffer      *fbPkt;               /* feedback Pkt */
#endif
{
   PjRbCb   *pjRbCb;
   PjUlCb   *ulCb;
   PjRxEnt  *rxEnt;
   S16      ret;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC2(PjLibObdDecmpCfm);

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
      (_kwPBuf, "PjLibObdDecmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%d))\n",\
        cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
      (_kwPBuf, "PjLibObdDecmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%ld))\n",\
       cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#endif

   ret = ROK;
   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opSdu);
      PJ_FREE_BUF(fbPkt);
      RETVALUE(RFAILED);
   }

   /* 1. Initialisations */
   /* kw006.201 ccpu00120502 corrected the RB ID calculation */
   rbId =  libTrans.rbId;
   kwDbmFetchPjRbCb(libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if(pjRbCb == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW125, (ErrVal)libTrans.rbId,
            "PjLibObdDecmpCfm: PjRbCb is NULLP \n");
      RETVALUE(RFAILED);
   }
   ulCb  =  &pjRbCb->ulCb;

   /* 2. Send the feedback packet */
   if(fbPkt != NULLP)
   {
      PJ_SND_ROHC_FDB(pjRbCb, fbPkt, ret);
      if ( ret != ROK )
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW126, (ErrVal)libTrans.rbId,
            "PjLibObdDecmpCfm: Could not send feedback packet \n");
         PJ_FREE_BUF(opSdu);
         PJ_FREE_BUF(fbPkt);
         RETVALUE(RFAILED);
      }
   }

   /* 3. Get the rxEnt */
   rxEnt =  pjDbmGetRxEnt(&ulCb->recBuf, libTrans.count );
   if((rxEnt == NULLP) || (rxEnt->state != PJ_SENT_TO_DCOMP))
   {
      /* We  have got a duplicate . 
       * It is NULLP if the original has been sent up,
       * Or, the original is in another state 
       * (if both are in the same state, we accept the 
       * first output) */
      PJ_FREE_BUF(opSdu);
      RETVALUE(ROK);
   }

   /* 4. check decompression status */
   if(opSdu == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW127, (ErrVal)libTrans.rbId,
            "PjLibObdDecmpCfm: opSdu is NULLP \n");
      PJ_FREE_BUF(opSdu);
      /* kw006.201 replaced macro with function */
      pjUlmSndPjuStaInd(pjRbCb, rxEnt);
      PJ_STS_INC_GEN_CNT(numDecmpFails);
      pjUlmHdlErrUpdates(pjRbCb, rxEnt->count);
      pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
      RETVALUE(RFAILED);
   }

   /* 5. check for discardable entries */
   if(rxEnt->discFlag == TRUE)
   {
      pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
      PJ_FREE_BUF(opSdu);
      RETVALUE(ROK);
   }

   /* 6. Post decomp updations */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   PJ_FREE_BUF(rxEnt->mBuf);
   rxEnt->mBuf  =  opSdu;

   /* 7. Call the function to deliver the pdu*/
   PJ_DEC_OBD_COUNT(pjRbCb, rxEnt->count);
   /* kw006.201 ccpu00118629, replaced macro with function */
   pjUlmDeliverDrb(pjRbCb, rxEnt);

   RETVALUE(ROK);
} /* end of PjLibObdDecmpCfm */

#endif /* PJ_CMP_ASYNC */

#ifdef PJ_SEC_ASYNC 
/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the MAC-I from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output, MAC-I from Integration unit and 
 *        sends the data for ciphering with relevant input paramenters.
 *
 *  @param[in]   pst         Post structure 
 *  @param[in]   cxtId       Context Id for Integrity protection
 *  @param[in]   libTrans    Transaction Id 
 *  @param[in]   macI        Message authentication code 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdIntProtCfm
(
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Integrity protection*/
PjLibTrans libTrans,              /* Transaction Id */
U32      macI                     /* Message authentication code */
)
#else
PUBLIC S16 PjLibObdIntProtCfm(pst, cxtId, libTrans, macI)
Pst      *pst;                    /* post structure */
PTR      cxtId;                   /* Context Id for Integrity protection */
PjLibTrans libTrans;              /* Transaction Id */
U32      macI;                    /* Message authentication code */
#endif
{
   U8       hdr;
   PjRbCb   *pjRbCb;
   Buffer   *ciphSdu;
   S16      ret;
   U32      count;
   PjTxEnt  *txEnt;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC3(PjLibObdIntProtCfm)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
          (_kwPBuf, "PjLibObdIntProtCfm(pst, cxtId(%ld), libTrans(%d,%d,%d))\n",\
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
          (_kwPBuf, "PjLibObdIntProtCfm(pst, cxtId(%ld), libTrans(%d,%d,%ld))\n",\
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#endif

   count    = libTrans.count;
   ret      = ROK;
   txEnt    = NULLP;
   ciphSdu  = NULLP;

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }
   
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   kwDbmFetchPjRbCb(libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if ( pjRbCb == NULLP )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW128, (ErrVal)count,
         "PjLibObdIntProtCfm: pjRbCb is NULLP \n");
      RETVALUE(RFAILED);
   }
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      RETVALUE(ret);
   }

   txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
   if ( txEnt == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW129, (ErrVal)count,
         "PjLibObdIntProtCfm: txEnt is NULLP \n");
      RETVALUE(RFAILED);
   }


   PJ_PACK_MACI(txEnt->sdu, macI);
   
   ret = SRemPreMsg(&hdr, txEnt->sdu);
   if ( ret != ROK )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW130, (ErrVal)0,
         "PjLibObdIntProtCfm: SRemPreMsg for txEnt \n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      PJ_SND_CFM(pjRbCb, txEnt, PJ_REM_MSG_FAILED);
      pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), txEnt->count);
      RETVALUE(RFAILED);
   }
    
   txEnt->pdu = txEnt->sdu;

   if (pjRbCb->firstDlMsg == TRUE && pjRbCb->firstSn == txEnt->sn)
   {
      pjRbCb->firstDlMsg = FALSE; 
      txEnt->sdu = NULLP;
      ret = pjDlmDeliverPdu(pjRbCb, txEnt);
   }
   else
   {
      ret = pjDlmHdlCiph(pjRbCb, txEnt);
      if ( ret != ROK )
      {    
         PJLOGERROR(ERRCLS_ADD_RES, EKW131, (ErrVal)libTrans.count,
            "PjLibObdIntProtCfm: pjUtlCipherReq failed \n");
      } 
   }
   
   RETVALUE(ret);

} /* end of PjLibObdIntProtCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the ciphered SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from ciphering unit and 
 *        sends the PDU to lower layer in sequence.
 *
 *  @param[in]   pst              Post structure
 *  @param[in]   cxtId            Context Id for Ciphering 
 *  @param[in]   libTrans         Transaction Id 
 *  @param[in]   opSdu            ciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCipherCfm
(
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Ciphering */
PjLibTrans libTrans,              /* Transaction Id */
Buffer   *opSdu                   /* ciphered SDU */
)
#else
PUBLIC S16 PjLibObdCipherCfm(pst, cxtId, libTrans, opSdu)
Pst      *pst;                    /* post structure */
PTR      cxtId;                   /* Context Id for Integrity protection */
PjLibTrans libTrans;              /* Transaction Id */
Buffer   *opSdu;                  /* ciphered SDU */
#endif
{
   PjRbCb   *pjRbCb;
   S16      ret;
   PjTxEnt  *txEnt;
   U32      count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC3(PjLibObdCipherCfm)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
          (_kwPBuf, "PjLibObdCipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%d))\n",\
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
         (_kwPBuf, "PjLibObdCipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%ld))\n",\
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#endif

   count  = libTrans.count;
   ret    = ROK;

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }

   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   kwDbmFetchPjRbCb(libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if ( pjRbCb == NULLP )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW132, (ErrVal)count,
         "PjLibObdCipherCfm: pjRbCb is NULLP \n");
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(ret);
   }
   txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), count);
   if ( txEnt == NULLP )
   {
      /* Log an error and return */
      PJLOGERROR(ERRCLS_ADD_RES, EKW133, (ErrVal)0,
         "PjLibObdCipherCfm: txEnt is NULLP \n");
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   if ( opSdu == NULLP )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW134, (ErrVal)libTrans.count,
        "PjLibObdCipherCfm: opSdu is NULLP:Ciphering Failed \n");
      PJ_UPD_DL_VAR(pjRbCb, txEnt->count);
      if (!pjRbCb->rohc.hdrCmpUsed)
      {
         txEnt->sdu = NULLP;
      }
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(pjRbCb, txEnt, PJU_CIPHER_FAILED);
      }
      else
      {
         pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), txEnt->count);
      }
      kwCb.pjGenSts.numCiphFails++;
      RETVALUE(RFAILED);
   }

   if ( pjRbCb->rohc.hdrCmpUsed )
   {
      PJ_FREE_BUF(txEnt->pdu);
   }
   txEnt->pdu = opSdu;
   ret = pjDlmDeliverPdu(pjRbCb, txEnt);

   RETVALUE(ret);

} /* end of PjLibObdCipherCfm */
/*@}*/

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the deciphered SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from deciphering unit and 
 *        sends the same for Integrity verification or decompression
 *        based on the rbType.
 * 
 *  @param[in]  pst            Post structure 
 *  @param[in]  cxtId          Context Id for Integrity protection
 *  @param[in]  libTrans       Transaction Id 
 *  @param[in]  opPdu          Deciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdDecipherCfm
(
Pst         *pst,                 /* post structure */
PTR         cxtId,                /* Context Id for Integrity protection*/
PjLibTrans  libTrans,             /* Transaction Id */
Buffer      *opPdu                /* deciphered SDU */
)
#else
PUBLIC S16 PjLibObdDecipherCfm(pst, cxtId, libTrans, opPdu)
Pst         *pst;                 /* post structure */
PTR         cxtId;                /* Context Id for Integrity protection*/
PjLibTrans  libTrans;             /* Transaction Id */
Buffer      *opPdu;               /* deciphered SDU */
#endif
{
   PjUlCb     *ulCb;
   PjRxEnt    *rxEnt;
   PjRbCb     *pjRbCb;
   Buffer     *outPdu = NULLP;
   S16         ret   =  ROK;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC2(PjLibObdDecipherCfm);

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
      (_kwPBuf, "PjLibObdDecipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%d))\n",\
       cxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
      (_kwPBuf, "PjLibObdDecipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%ld))\n",\
       cxtId, libTrans.rbId, libTrans.rbType, libTrans.count));
#endif

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opPdu);
      RETVALUE(RFAILED);
   }

   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   /* 1. Initialisations */
   kwDbmFetchPjRbCb(libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if(pjRbCb == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW135, (ErrVal)libTrans.rbId,
            "PjLibObdDecipherCfm: PjRbCb is NULLP \n");
      PJ_FREE_BUF(opPdu);
      PJ_STS_INC_GEN_CNT(numDeciphFails);
      RETVALUE(RFAILED);
   }

   ulCb  =  &pjRbCb->ulCb;

   /* Validating rxEnt will be done later in code */
   rxEnt =  pjDbmGetRxEnt(&ulCb->recBuf, libTrans.count);

    /* 2. check decipher status */
   if( opPdu == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW136, (ErrVal)libTrans.rbId,
            "PjLibObdDecipherCfm: opPdu is NULLP \n");
      PJ_FREE_BUF(opPdu);
      PJ_STS_INC_GEN_CNT(numDecmpFails);
      if ( rxEnt != NULLP )
      {
         /* kw006.201 replaced macro with function */
         pjUlmSndPjuStaInd(pjRbCb, rxEnt);
         pjUlmHdlErrUpdates(pjRbCb, rxEnt->count);
         pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
      }
      RETVALUE(RFAILED);
   }


   /* 3. Check if it is SRB */
   if(pjRbCb->rbType == PJ_SRB)
   {
      /* 3.1 For reestablishment scenarios, discard */
      if(pjRbCb->state != PJ_STATE_NORMAL )
      {
         /* Discard the mBuf */
         PJ_FREE_BUF(opPdu);
         RETVALUE(ROK);
      }
      
      /* 3.2 Validate the rxEnt */
      if(rxEnt == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW137, (ErrVal)libTrans.rbId,
               "PjLibObdDecipherCfm: RxEnt is NULLP \n");
         PJ_FREE_BUF(opPdu);
         PJ_STS_INC_GEN_CNT(numDeciphFails);
         RETVALUE(RFAILED);
      }
         
      /* 3.3 Post deciphering updations */
      rxEnt->state   =  PJ_RDY_TO_INTVER;
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf  =  opPdu;

      /* 3.4 Send for IntVer. This will be ASYNC  */
      ret = pjUlmHdlIntVer(pjRbCb, rxEnt);

      RETVALUE(ret);
   }
   else  /*  DRB */
   {

      /* 4. Special case, 
       * Duplicate entries, can be sent out of order */
      if((rxEnt  == NULLP)||
         (rxEnt->discFlag == TRUE) ||
         (rxEnt->state != PJ_SENT_TO_DCIPHER))
      {
         /* 4.1 is decompression applicable */
         if(pjRbCb->rohc.hdrCmpUsed == TRUE)
         {

            /* 4.1.1 Calling decompression hook */
            if(pjUtlDeCmpReq(pjRbCb, libTrans.count, opPdu, &outPdu) != ROK)
            {
               RETVALUE(RFAILED);
            }

#ifdef PJ_CMP_ASYNC
            PJ_FREE_BUF(opPdu); 
            RETVALUE(ROK);
#endif /* PJ_CMP_ASYNC */
         }

         /* 4.1.2 discard output and exit */
         PJ_FREE_BUF(opPdu);
         PJ_FREE_BUF(outPdu);
         if((rxEnt != NULLP) &&
            (rxEnt->discFlag == TRUE))
         {
            pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count);
         }

         RETVALUE(ROK);
      }
      

      /* 5. Post deciphering updations */
      rxEnt->state   =  PJ_RDY_TO_SUBMIT;
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf  =  opPdu;

      /* 6. Submit it for decompression */
      pjUlmSubmitForDecmp(pjRbCb, rxEnt);

   }/* if(pjRbCb->rbType == PJ_DRB) */

   RETVALUE(ROK);

} /* end of PjLibObdDecipherCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the status for Integrity verification 
 *        from off-board entity.
 *
 * @b Description:
 *
 *        This function receives status, success or failure for Integrity
 *        verification and sends the data for deciphering with relevant input paramenters.
 *
 *  @param[in] pst        post structure 
 *  @param[in] cxtId      Context Id for Integrity protection
 *  @param[in] libTrans   Transaction Id 
 *  @param[in] status     Integrity verification status 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdIntVerCfm
(
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Integrity protection*/
PjLibTrans libTrans,              /* Transaction Id */
Status   status                   /* Integrity verification status */
)
#else
PUBLIC S16 PjLibObdIntVerCfm(pst, cxtId, libTrans, status)
Pst      *pst;                    /* post structure */
PTR      cxtId;                   /* Context Id for Integrity protection */
PjLibTrans libTrans;              /* Transaction Id */
Status   status;                  /* Integrity verification status */
#endif
{
   PjRbCb   *pjRbCb;
   PjUlCb   *ulCb;
   PjRxEnt  *rxEnt;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC2(PjLibObdIntVerCfm)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), (_kwPBuf, 
          "PjLibObdIntVerCfm(cxtId(%ld), libTrans(%d,%d,%d), status(%d))\n",\
          cxtId, libTrans.rbId, libTrans.rbType, libTrans.count, status));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), (_kwPBuf, 
          "PjLibObdIntVerCfm(cxtId(%ld), libTrans(%d,%d,%ld), status(%d))\n",\
          cxtId, libTrans.rbId, libTrans.rbType, libTrans.count, status));
#endif

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   /* Initialisations */
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   kwDbmFetchPjRbCb(libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if(pjRbCb == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW138, (ErrVal)libTrans.rbId,
            "PjLibObdIntVerCfm: PjRbCb is NULLP \n");
      PJ_STS_INC_GEN_CNT(numIntgVrfFails);
      RETVALUE(RFAILED);
   }
   ulCb  =  &pjRbCb->ulCb;

   /* For reestablishment cases, exit */
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
      RETVALUE(ROK);
   }

   /* Get the rxEnt */
   rxEnt =  pjDbmGetRxEnt(&ulCb->recBuf, libTrans.count );
   if(rxEnt == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW139, (ErrVal)libTrans.rbId,
            "PjLibObdIntVerCfm: RxEnt is NULLP \n");
      PJ_STS_INC_GEN_CNT(numIntgVrfFails);
      /* kw006.201 replaced macro with function */
      pjUtlPerfErrUpd(pjRbCb, libTrans.count);
      RETVALUE(RFAILED);
   }

   /* Validate status */
   if(status != ROK)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW140, (ErrVal)rxEnt->count,
            "PjLibObdIntVerCfm: Integrity Failed \n");
      /* kw006.201 replaced macro with function */
      pjUlmSndPjuStaInd(pjRbCb, rxEnt);
      PJ_STS_INC_GEN_CNT(numIntgVrfFails);
      /* kw006.201 replaced macro with function */
      pjUtlPerfErrUpd(pjRbCb, rxEnt->count);
/*      pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count); */
      RETVALUE(RFAILED);
   }

   /* Check if next in line for delivery */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   if(rxEnt->count != ulCb->nxtSubCnt)
   {
      RETVALUE(ROK);
   }

   /* Send for delivery */
   pjUlmDeliverSrb(pjRbCb, rxEnt);

   RETVALUE(ROK);

} /* end of PjLibObdIntVerCfm */
#endif /* PJ_SEC_ASYNC */


/********************************************************************
 *    Utility Handler for Sending to Offboarding unit               *
 *******************************************************************/

/**
 *
 * @brief 
 *
 *        Handler for opening the context with the compression unit (either 
 *        synchronous or asynchronous) for RB within an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the compression unit per RB per
 *        UE, by sending ROHC configuration elements using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *
 *  @param[in]  pjRbCb   PDCP RbCb
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCmpInit
(
PjRbCb   *pjRbCb                  /* PDCP RbCb */
)
#else
PUBLIC S16 pjUtlCmpInit(pjRbCb)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
#endif
{
   TRC3(pjUtlCmpInit)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCmpInit(pjRbCb(%d,%d))\n",\
                    pjRbCb->rbId, pjRbCb->rbType));
#ifdef PJ_CMP_ASYNC

   pjRbCb->ueCb->libInfo.tCmpInitBitMask |= (1 << (pjRbCb->rbId - 1));
   pjRbCb->ueCb->libInfo.tLibInitBitMask |= PJ_LIB_COMP_BIT_MASK;
   pjRbCb->cmpCxt.cfgTxId  =  pjRbCb->ueCb->libInfo.nxtAvlbTxId;

   PjLibObdCmpInitReq(&kwCb.pjGenCfg.obdPst.cmpPst, 
                                    pjRbCb->cmpCxt, pjRbCb->rohc);
#else
   pjLibCmpInitReq(pjRbCb->cmpCxt, pjRbCb->rohc, &(pjRbCb->cmpCxtId));
#endif  /* PJ_CMP_ASYNC */

   RETVALUE(ROK);

} /* end of pjUtlCmpInit */

/**
 *
 * @brief 
 *
 *        Handler for opening the context with the Integration unit (either 
 *        synchronous or asynchronous) for all SRBs of an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the security algo unit per UE
 *        for Integration protection/verification by sending the configured
 *        Integrity Info using synchronous / asynchronous function calls with 
 *        relevant input parameters.
 *
 *  @param[in]  pjRbCb     Pointer to RB control Block
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlIntInit
(
KwUeCb   *ueCb                   /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlIntInit(ueCb)
KwUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{
#ifndef PJ_SEC_ASYNC
   PTR   ctxId;
#else
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   TRC3(pjUtlIntInit)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlIntInit(ueCb(%d,%d))\n",\
                    ueCb->key.ueId, ueCb->key.cellId));

#ifdef PJ_SEC_ASYNC

   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
      {
         if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
               (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_REEST_ASYNC_CFM))
         {
            libInitBitMask =  &ueCb->libInfo.asyncCfm[txIdx]->libInitBitMask;
            break;
         }
      }
   }
   else if(ueCb->libInfo.state == PJ_STATE_NORMAL)
   {
      libInitBitMask =  &ueCb->libInfo.tLibInitBitMask;
   }

   if(libInitBitMask)
   {
      (*libInitBitMask) = (*libInitBitMask) | PJ_LIB_INT_BIT_MASK;
   }

   PjLibObdIntInitReq(&kwCb.pjGenCfg.obdPst.secPst, 
                           ueCb->secInfo.secCxt, ueCb->secInfo.intInfo);
#else
#ifndef TENB_AS_SECURITY
   pjLibIntInitReq(ueCb->secInfo.secCxt, ueCb->secInfo.intInfo,&(ctxId));
   ueCb->secInfo.intCxtId    =  ctxId;
#endif
#endif /* PJ_SEC_ASYNC */

   RETVALUE(ROK);

} /* end of pjUtlIntInit */

/**
 *
 * @brief 
 *
 *        Handler for opening the context with the Ciphering unit (either 
 *        synchronous or asynchronous) for SRBs of an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the security algo unit per UE
 *        for ciphering / deciphering by key by sending the configured
 *        Ciphering Info with control plane key using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *  @param[in]  pjRbCb     Pointer to RB control Block
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCpInit
(
KwUeCb   *ueCb                    /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlCpInit(ueCb)
KwUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{

#ifndef PJ_SEC_ASYNC
   PTR   ctxId;
#else
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   S16   ret = ROK;

   TRC3(pjUtlCpInit)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCpInit(ueCb(%d,%d))\n",\
                    ueCb->key.ueId, ueCb->key.cellId));
#ifdef PJ_SEC_ASYNC

   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
      {
         if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
               (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_REEST_ASYNC_CFM))
         {
            libInitBitMask =  &ueCb->libInfo.asyncCfm[txIdx]->libInitBitMask;
            break;
         }
      }
   }
   else if(ueCb->libInfo.state == PJ_STATE_NORMAL)
   {
      libInitBitMask =  &ueCb->libInfo.tLibInitBitMask;
   }

   if(libInitBitMask)
   {
      (*libInitBitMask) = (*libInitBitMask) | PJ_LIB_CP_CIP_BIT_MASK;
   }
   ret = PjLibObdCpInitReq(&kwCb.pjGenCfg.obdPst.secPst, ueCb->secInfo.secCxt, 
         ueCb->secInfo.cipherInfo.algoType, ueCb->secInfo.cipherInfo.cpKey);
#else
#ifndef TENB_AS_SECURITY
   ret = pjLibCpInitReq(ueCb->secInfo.secCxt, ueCb->secInfo.cipherInfo.algoType,
                  ueCb->secInfo.cipherInfo.cpKey, &ctxId);
   ueCb->secInfo.cpCxtId = ctxId;
#endif
#endif /* PJ_SEC_ASYNC */

   RETVALUE(ret);

} /* end of pjUtlCpInit */

/**
 *
 * @brief 
 *
 *        Handler for opening the context with the Ciphering unit (either 
 *        synchronous or asynchronous) for DRBs of an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the security algo unit per UE
 *        for ciphering / deciphering by sending the configured
 *        Ciphering Info with user plane key using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *  @param[in]  pjRbCb     Pointer to RB control Block
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlUpInit
(
KwUeCb   *ueCb                    /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlUpInit(ueCb)
KwUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{

#ifndef PJ_SEC_ASYNC
   PTR   ctxId;
#else
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   TRC3(pjUtlUpInit)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlUpInit(ueCb(%d,%d))\n",\
                    ueCb->key.ueId, ueCb->key.cellId));

#ifdef PJ_SEC_ASYNC

   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
      {
         if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
               (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_REEST_ASYNC_CFM))
         {
            libInitBitMask =  &ueCb->libInfo.asyncCfm[txIdx]->libInitBitMask;
            break;
         }
      }
   }
   else if(ueCb->libInfo.state == PJ_STATE_NORMAL)
   {
      libInitBitMask =  &ueCb->libInfo.tLibInitBitMask;
   }

   if(libInitBitMask)
      (*libInitBitMask) = (*libInitBitMask) | PJ_LIB_UP_CIP_BIT_MASK;

   PjLibObdUpInitReq(&kwCb.pjGenCfg.obdPst.secPst, ueCb->secInfo.secCxt, 
         ueCb->secInfo.cipherInfo.algoType, ueCb->secInfo.cipherInfo.upKey);
#else
#ifndef TENB_AS_SECURITY
   pjLibUpInitReq(ueCb->secInfo.secCxt, ueCb->secInfo.cipherInfo.algoType, 
                              ueCb->secInfo.cipherInfo.upKey, &ctxId);
   ueCb->secInfo.upCxtId = ctxId;
#endif
#endif /* PJ_SEC_ASYNC */

   RETVALUE(ROK);

} /* end of pjUtlUpInit */

/**
 *
 * @brief 
 *
 *        Handler for resetting the context with the compression unit (either 
 *        synchronous or asynchronous) for RB within an UE.
 *
 * @b Description:
 *
 *        This function resets the context with the compression unit per RB per
 *        UE by using synchronous / asynchronous function calls.
 *
 *
 *  @param[in] cmpCxtId     Context to be reset for compression 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCmpReset
(
PjRbCb      *pjRbCb                 /* Context to be reset for compression */
)
#else
PUBLIC S16 pjUtlCmpReset(pjRbCb)
PjRbCb      *pjRbCb;                 /* Context to be reset for compression */
#endif
{
   TRC3(pjUtlCmpReset)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCmpReset(pjRbCb(%d,%d))\n",\
                    pjRbCb->rbId, pjRbCb->rbType));

#ifdef PJ_CMP_ASYNC
   PjLibObdCmpResetReq(&kwCb.pjGenCfg.obdPst.cmpPst, pjRbCb->cmpCxtId);
#else
   pjLibCmpResetReq(pjRbCb->cmpCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlCmpReset */

/**
 *
 * @brief 
 *
 *        Handler for redirecing compression request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends compression request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb         PDCP RbCb 
 *  @param[in]   mBuf           SDU to be compressed 
 *  @param[in]   opSdu          Compressed SDU 
 *  @param[in]   count          COUNT - transaction Id 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCmpReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu,                 /* Compressed SDU */
U32      count                    /* COUNT - transaction Id */
)
#else
PUBLIC S16 pjUtlCmpReq(pjRbCb, mBuf, opSdu, count)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
Buffer   *mBuf;                   /* SDU to be compressed */
Buffer   **opSdu;                 /* Compressed SDU */
U32      count;                   /* COUNT - transaction Id */
#endif
{
#ifdef PJ_CMP_ASYNC
   PjLibTrans libTrans; /* Transaction Id for compression */
#endif
   S16        ret;

   ret = ROK;

   TRC3(pjUtlCmpReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCmpReq(pjRbCb(%d,%d), count(%d))\n",\
                    pjRbCb->rbId, pjRbCb->rbType, count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCmpReq(pjRbCb(%d,%d), count(%ld))\n",\
                    pjRbCb->rbId, pjRbCb->rbType, count));
#endif


#ifdef PJ_CMP_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = count;
   /* kw006.201 ccpu00120502 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   /* Send to the library */
   ret = PjLibObdCmpReq(&kwCb.pjGenCfg.obdPst.cmpPst, pjRbCb->cmpCxtId, 
                                                      libTrans, mBuf);  
#else
   ret = pjLibCmpReq(pjRbCb->cmpCxtId, mBuf, opSdu);
#endif

   RETVALUE(ret);

} /* end of pjUtlCmpReq */

/**
 *
 * @brief 
 *
 *        Handler for redirecing Integration request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends Intergrity protection request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb       PDCP RbCb 
 *  @param[in]   secInp       Input parameters for integrity 
 *  @param[in]   mBuf         SDU to be compressed 
 *  @param[out]  macI         Message authentication code for the SDU 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 pjUtlIntProtReq
(
PjRbCb   *pjRbCb,             /* PDCP RbCb */
PjSecInp secInp,              /* Input parameters for integrity */ 
Buffer   *mBuf,               /* SDU to be compressed */
U32      *macI                /* Message authentication code for the SDU */
)
#else
PUBLIC S16 pjUtlIntProtReq(pjRbCb, secInp, mBuf, macI)
PjRbCb   *pjRbCb;             /* PDCP RbCb */
PjSecInp secInp;              /* Input parameters for integrity */ 
Buffer   *mBuf;               /* SDU to be compressed */
U32      *macI;               /* Message authentication code for the SDU */
#endif
{
   Pst pst;
   S16 ret = ROK;
 
   PjLibTrans libTrans; /* Transaction Id for deciphering */

   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlIntProtReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlIntProtReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlIntProtReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#endif

   cxtId = pjRbCb->ueCb->secInfo.intCxtId;


   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;



#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   ret = PjLibObdIntProtReq(&kwCb.pjGenCfg.obdPst.secPst, cxtId, 
                                 secInp, libTrans, mBuf);  

#else

      ret = PjLibObdIntProtReq(&pst, cxtId, 
                                 secInp, libTrans, mBuf);  
#endif

   RETVALUE(ret);

} /* end of pjUtlIntProtReq */
#else
#ifdef ANSI
PUBLIC S16 pjUtlIntProtReq
(
PjRbCb   *pjRbCb,             /* PDCP RbCb */
PjSecInp secInp,              /* Input parameters for integrity */ 
Buffer   *mBuf,               /* SDU to be compressed */
U32      *macI                /* Message authentication code for the SDU */
)
#else
PUBLIC S16 pjUtlIntProtReq(pjRbCb, secInp, mBuf, macI)
PjRbCb   *pjRbCb;             /* PDCP RbCb */
PjSecInp secInp;              /* Input parameters for integrity */ 
Buffer   *mBuf;               /* SDU to be compressed */
U32      *macI;               /* Message authentication code for the SDU */
#endif
{
   S16 ret = ROK;
#ifdef PJ_SEC_ASYNC
   PjLibTrans libTrans; /* Transaction Id for deciphering */
#endif /* PJ_SEC_ASYNC */
   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlIntProtReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlIntProtReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlIntProtReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#endif

   cxtId = pjRbCb->ueCb->secInfo.intCxtId;

#ifdef PJ_SEC_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;


   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   ret = PjLibObdIntProtReq(&kwCb.pjGenCfg.obdPst.secPst, cxtId, 
                                 secInp, libTrans, mBuf);  

#else
   ret = pjLibIntProtReq(cxtId, secInp, mBuf, macI);
#endif

   RETVALUE(ret);

} /* end of pjUtlIntProtReq */
#endif

/**
 *
 * @brief 
 *
 *        Handler for redirecing ciphering request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends ciphering protection request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb           PDCP RbCb 
 *  @param[in]   secInp           Input parameters for deciphering 
 *  @param[in]   mBuf             Data to be deciphered 
 *  @param[in]   opSdu            Deciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 pjUtlCipherReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for deciphering */ 
Buffer   *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlCipherReq(pjRbCb, secInp, mBuf, opSdu)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                 /* Input parameters for deciphering */ 
Buffer   *mBuf;                   /* Data to be deciphered */
Buffer   **opSdu;                 /* Deciphered SDU */
#endif
{
   Pst pst;
   S16 ret = ROK;

   PjLibTrans libTrans; /* Transaction Id for deciphering */

   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlCipherReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlCipherReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlCipherReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#endif

   if (pjRbCb->rbType == PJ_SRB)
   {
      cxtId = pjRbCb->ueCb->secInfo.cpCxtId;
   }
   else
   {
      cxtId = pjRbCb->ueCb->secInfo.upCxtId;
   } 


   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   ret = PjLibObdCipherReq(&kwCb.pjGenCfg.obdPst.secPst, cxtId, 
                                             secInp, libTrans, mBuf);  

#else
      ret = PjLibObdCipherReq(&pst, cxtId, 
                                  secInp, libTrans, mBuf, opSdu);  
#endif

   RETVALUE(ret);

} /* end of pjUtlCipherReq */
#else
#ifdef ANSI
PUBLIC S16 pjUtlCipherReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for deciphering */ 
Buffer   *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlCipherReq(pjRbCb, secInp, mBuf, opSdu)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                 /* Input parameters for deciphering */ 
Buffer   *mBuf;                   /* Data to be deciphered */
Buffer   **opSdu;                 /* Deciphered SDU */
#endif
{
   S16 ret = ROK;
#ifdef PJ_SEC_ASYNC
   PjLibTrans libTrans; /* Transaction Id for deciphering */
#endif /* PJ_SEC_ASYNC */
   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlCipherReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlCipherReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlCipherReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#endif

   if (pjRbCb->rbType == PJ_SRB)
   {
      cxtId = pjRbCb->ueCb->secInfo.cpCxtId;
   }
   else
   {
      cxtId = pjRbCb->ueCb->secInfo.upCxtId;
   } 

#ifdef PJ_SEC_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   ret = PjLibObdCipherReq(&kwCb.pjGenCfg.obdPst.secPst, cxtId, 
                                             secInp, libTrans, mBuf);  

#else
   ret = pjLibCipherReq(cxtId, secInp, mBuf, opSdu);
#endif

   RETVALUE(ret);

} /* end of pjUtlCipherReq */
#endif
/**
 *
 * @brief 
 *
 *        Handler for redirecing decompression request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends decompression protection request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb         PDCP RbCb 
 *  @param[in]   mBuf           SDU to be compressed 
 *  @param[in]   opSdu          Compressed SDU 
 *  @param[in]   count          COUNT - transaction Id 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDeCmpReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
U32      count,                   /* COUNT - transaction Id */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Compressed SDU */
)
#else
PUBLIC S16 pjUtlDeCmpReq(pjRbCb, count, mBuf, opSdu)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
U32      count;                   /* COUNT - transaction Id */
Buffer   *mBuf;                   /* SDU to be compressed */
Buffer   **opSdu;                 /* Compressed SDU */
#endif
{
   S16 ret = ROK;
#ifdef PJ_CMP_ASYNC
   PjLibTrans libTrans; /* Transaction Id for compression */
#else
   Buffer *feedback  = NULLP;
#endif /* PJ_CMP_ASYNC */

   TRC3(pjUtlDeCmpReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlDeCmpReq(pjRbCb(%d,%d), count(%d))\n",\
                    pjRbCb->rbId, pjRbCb->rbType, count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlDeCmpReq(pjRbCb(%d,%d), count(%ld))\n",\
                    pjRbCb->rbId, pjRbCb->rbType, count));
#endif
#ifdef PJ_CMP_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = count;
   /* kw006.201 ccpu00120502 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  count;
      pjStartTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }

   ret = PjLibObdDecmpReq(&kwCb.pjGenCfg.obdPst.cmpPst, 
                                      pjRbCb->cmpCxtId, libTrans, mBuf);  
#else
   ret = pjLibDecmpReq(pjRbCb->cmpCxtId, mBuf, &feedback, opSdu);

   if(feedback != NULLP)
   {
      PJ_SND_ROHC_FDB(pjRbCb, feedback, ret);
   }
#endif

   RETVALUE(ret);

} /* end of pjUtlDeCmpReq */

/**
 *
 * @brief 
 *
 *        Handler for redirecing Integration request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends Intergrity verification request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb        PDCP RbCb 
 *  @param[in]   secInp        Input parameters for integrity 
 *  @param[in]   mBuf          SDU to be compressed 
 *  @param[out]  status        Integrity verification status 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 pjUtlIntVerReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      macI,                    /* MAC-I to be verified with */
Status   *status                  /* Integrity verification status */
)
#else
PUBLIC S16 pjUtlIntVerReq(pjRbCb, secInp, mBuf, macI, status)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                 /* Input parameters for integrity */ 
Buffer   *mBuf;                   /* SDU to be compressed */
U32      macI;                    /* MAC-I to be verified with */
Status   *status;                 /* Integrity verification status */
#endif
{

   S16 ret = ROK;
   Pst pst;
   PjLibTrans libTrans; /* Transaction Id for deciphering */
   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlIntVerReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
       "pjUtlIntVerReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d), macI(%d))\n",\
       pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir, macI));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
       "pjUtlIntVerReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d)), macI(%ld)\n",\
       pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir, macI));
#endif

   cxtId = pjRbCb->ueCb->secInfo.intCxtId;


   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }

   ret = PjLibObdIntVerReq(&kwCb.pjGenCfg.obdPst.secPst, cxtId, 
                                       secInp, libTrans, mBuf, macI);  
#else
   ret = PjLibObdIntVerReq(&pst, cxtId, 
                                       secInp, libTrans, mBuf, macI);  
#endif

   RETVALUE(ret);

} /* end of pjUtlIntVerReq */
#else
#ifdef ANSI
PUBLIC S16 pjUtlIntVerReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      macI,                    /* MAC-I to be verified with */
Status   *status                  /* Integrity verification status */
)
#else
PUBLIC S16 pjUtlIntVerReq(pjRbCb, secInp, mBuf, macI, status)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                 /* Input parameters for integrity */ 
Buffer   *mBuf;                   /* SDU to be compressed */
U32      macI;                    /* MAC-I to be verified with */
Status   *status;                 /* Integrity verification status */
#endif
{
   S16 ret = ROK;
#ifdef PJ_SEC_ASYNC
   PjLibTrans libTrans; /* Transaction Id for deciphering */
#endif /* PJ_SEC_ASYNC */
   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlIntVerReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
       "pjUtlIntVerReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d), macI(%d))\n",\
       pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir, macI));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
       "pjUtlIntVerReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d)), macI(%ld)\n",\
       pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir, macI));
#endif

   cxtId = pjRbCb->ueCb->secInfo.intCxtId;

#ifdef PJ_SEC_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }

   ret = PjLibObdIntVerReq(&kwCb.pjGenCfg.obdPst.secPst, cxtId, 
                                       secInp, libTrans, mBuf, macI);  
#else
   ret = pjLibIntVerReq(cxtId, secInp, mBuf, macI, status);
#endif

   RETVALUE(ret);

} /* end of pjUtlIntVerReq */
#endif
/**
 *
 * @brief 
 *
 *        Handler for redirecing deciphering request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends deciphering request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb           PDCP RbCb 
 *  @param[in]   secInp           Input parameters for deciphering 
 *  @param[in]   mBuf             Data to be deciphered 
 *  @param[in]   opSdu            Deciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 pjUtlDecipherReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp,                  /* Input parameters for deciphering */ 
Buffer   *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlDecipherReq(pjRbCb, secInp, mBuf, opSdu)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                  /* Input parameters for deciphering */ 
Buffer   *mBuf;                   /* Data to be deciphered */
Buffer   **opSdu;                 /* Deciphered SDU */
#endif
{
   Pst pst;
   S16 ret = ROK;

   PjLibTrans libTrans; /* Transaction Id for deciphering */

   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlDecipherReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlDecipherReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlDecipherReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#endif

   if (pjRbCb->rbType == PJ_SRB)
   {
      cxtId = pjRbCb->ueCb->secInfo.cpCxtId;
   }
   else
   {
      cxtId = pjRbCb->ueCb->secInfo.upCxtId;
   } 


   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }

   ret = PjLibObdDecipherReq(&kwCb.pjGenCfg.obdPst.secPst, 
                                 cxtId, secInp, libTrans, mBuf);  
#else
   ret = PjLibObdDecipherReq(&pst, 
                                 cxtId, secInp, libTrans, mBuf, opSdu);  
#endif

   RETVALUE(ret);

} /* end of pjUtlDecipherReq */
#else
#ifdef ANSI
PUBLIC S16 pjUtlDecipherReq
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp,                  /* Input parameters for deciphering */ 
Buffer   *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlDecipherReq(pjRbCb, secInp, mBuf, opSdu)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                  /* Input parameters for deciphering */ 
Buffer   *mBuf;                   /* Data to be deciphered */
Buffer   **opSdu;                 /* Deciphered SDU */
#endif
{
   S16 ret = ROK;
#ifdef PJ_SEC_ASYNC
   PjLibTrans libTrans; /* Transaction Id for deciphering */
#endif /* PJ_SEC_ASYNC */
   PTR        cxtId;    /* Context Identifier */

   TRC3(pjUtlDecipherReq)

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlDecipherReq(pjRbCb(%d,%d), secInp(count(%d), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
          "pjUtlDecipherReq(pjRbCb(%d,%d), secInp(count(%ld), dir(%d))\n",\
          pjRbCb->rbId, pjRbCb->rbType, secInp.count, secInp.dir));
#endif

   if (pjRbCb->rbType == PJ_SRB)
   {
      cxtId = pjRbCb->ueCb->secInfo.cpCxtId;
   }
   else
   {
      cxtId = pjRbCb->ueCb->secInfo.upCxtId;
   } 

#ifdef PJ_SEC_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }

   ret = PjLibObdDecipherReq(&kwCb.pjGenCfg.obdPst.secPst, 
                                 cxtId, secInp, libTrans, mBuf);  
#else
   ret = pjLibDecipherReq(cxtId, secInp, mBuf, opSdu);
#endif

   RETVALUE(ret);

} /* end of pjUtlDecipherReq */
#endif
/**
 *
 * @brief 
 *
 *        Handler for redirecing compression request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends compression request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb           PDCP RbCb
 *  @param[in]   feedback         ROHC Feedback
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlSndFeedback
(
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *feedback                /* ROHC Feedback */
)
#else
PUBLIC S16 pjUtlSndFeedback(pjRbCb, feedback)
PjRbCb   *pjRbCb;                 /* PDCP RbCb */
Buffer   *feedback;               /* Feedback */
#endif
{
#ifdef PJ_CMP_ASYNC
   PjLibTrans libTrans; /* Transaction Id for compression */
#endif /* PJ_SEC_ASYNC */

   TRC3(pjUtlSndFeedback)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCmpReq(pjRbCb(%d,%d))\n",\
                    pjRbCb->rbId, pjRbCb->rbType));

#ifdef PJ_CMP_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = pjRbCb->ulCb.lastSubCnt;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

   PjLibObdCmpFbReq(&kwCb.pjGenCfg.obdPst.cmpPst, pjRbCb->cmpCxtId, libTrans,
         feedback);
#else
   pjLibCmpFbReq(pjRbCb->cmpCxtId, feedback);
#endif

   RETVALUE(ROK);

} /* end of pjUtlSndFeedback */

/**
 *
 * @brief 
 *
 *        Handler for closing the context with the compression unit (either 
 *        synchronous or asynchronous) for RB within an UE.
 *
 * @b Description:
 *
 *        This function closes an existing context with the compression unit per RB per
 *        UE, either using synchronous / asynchronous function calls with relevant input parameters.
 *
 *
 *  @param[in] cmpCxtId         ROHC Context to be closed
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCmpClose
(
PTR      cmpCxtId                 /* ROHC Context to be closed */
)
#else
PUBLIC S16 pjUtlCmpClose(cmpCxtId)
PTR      cmpCxtId;                /* ROHC Context to be closed */
#endif
{
   TRC3(pjUtlCmpClose)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCmpClose(cmpCxtId(%ld))\n", cmpCxtId));

#ifndef PJ_CMP_ASYNC
   pjLibCmpCloseReq(cmpCxtId);
#else
   PjLibObdCmpCloseReq(&kwCb.pjGenCfg.obdPst.cmpPst, cmpCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlCmpClose */

/**
 *
 * @brief 
 *
 *        Handler for closing the context with the Integration unit (either 
 *        synchronous or asynchronous) for all RBs of an UE.
 *
 * @b Description:
 *
 *        This function closes an existing context with the security algo unit per UE
 *        for Integration protection/verification using synchronous / asynchronous function 
 *        calls with relevant input parameters.
 *
 *
 *  @param[in] intCxtId        Integration CxtId to be closed 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlIntClose
(
PTR      intCxtId                 /* Integration CxtId to be closed */
)
#else
PUBLIC S16 pjUtlIntClose(intCxtId)
PTR      intCxtId;                /* Integration CxtId to be closed */
#endif
{
   TRC3(pjUtlIntClose)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlIntClose(cmpCxtId(%ld))\n", intCxtId));

#ifndef PJ_SEC_ASYNC
#ifndef TENB_AS_SECURITY
   pjLibIntCloseReq(intCxtId);
#endif
#else
   PjLibObdIntCloseReq(&kwCb.pjGenCfg.obdPst.secPst, intCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlIntClose */

/**
 *
 * @brief 
 *
 *        Handler for closing the context with the Ciphering unit (either 
 *        synchronous or asynchronous) for SRBs of an UE.
 *
 * @b Description:
 *
 *        This function closes an existing context with the security algo unit per UE
 *        for ciphering / deciphering with control plane key using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *
 *  @param[in] cpCxtId          Context Id for Ciphering to be closed 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCipherClose
(
PTR      cpCxtId                  /* Context Id for Ciphering to be closed */
)
#else
PUBLIC S16 pjUtlCipherClose(cpCxtId)
PTR      cpCxtId;                 /* Context Id for Ciphering to be closed */
#endif
{
   TRC3(pjUtlCipherClose)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlCipherClose(cpCxtId(%ld))\n", cpCxtId));

#ifndef PJ_SEC_ASYNC
#ifndef TENB_AS_SECURITY
   pjLibCipherCloseReq(cpCxtId);
#endif
#else
   PjLibObdCipherCloseReq(&kwCb.pjGenCfg.obdPst.secPst, cpCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlCipherClose */

/**
 *
 * @brief 
 *
 *        Utility function that processes the CfgReq with type as
 *        REESTABLISH sent from RRC.
 *
 * @b Description:
 *
 *        This function 
 *        1. Marks the state of the RBs as PJ_STATE_REEST.
 *        2. Calculates the number of RBs undergoing reestablishment.
 *
 *  @param[in] ueCb     UE Control Block Pointer.
 *  @param[in] numEnt   Number of entities undergoing reestablishment
 *  @param[in] cfgEnt   List of entities configured.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlReEstSrb1
(
PjRbCb      *pjRbCb                    /* PDCP Control Block Pointer */
)
#else
PUBLIC S16 pjUtlReEstSrb1(pjRbCb)
PjRbCb      *pjRbCb;                   /* PDCP Control Block Pointer */
#endif
{
   KwUeCb   *ueCb;
   RbCb     *tRbCb;
   U8       rbId;

   TRC2(pjUtlReEstSrb1)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlReEstSrb1(pjRbCb(%d,%d))\n",\
                    pjRbCb->rbId, pjRbCb->rbType));

   /* Intitialisations */
   ueCb     = pjRbCb->ueCb;

   /* Check if this is the only RB to reestablish.If this is the only one,
    * we can start security reconfiguring from here itself*/
   ueCb->libInfo.state  =  PJ_STATE_REEST;
   pjRbCb->state  =  PJ_STATE_REEST;
   for(rbId =  2; rbId < KW_MAX_SRB_PER_UE; rbId ++)
   {
      KW_DBM_FETCH_RBCB(rbId, CM_LTE_SRB, ueCb, tRbCb);

      if(tRbCb == NULLP)
      {
         continue;
      }

      if((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP)
      {
         continue;
      }

      break;
   }
   for(rbId =  0; rbId < KW_MAX_DRB_PER_UE; rbId ++)
   {
      KW_DBM_FETCH_RBCB(rbId, CM_LTE_DRB, ueCb, tRbCb);

      if(tRbCb == NULLP)
      {
         continue;
      }

      if((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP)
      {
         continue;
      }

      break;
   }

   /* Perform uplink and downlink reestablishment
    * of SRB1 */
   pjDlmReEstSrb(pjRbCb);
   pjUlmReEstSrb(pjRbCb);

   RETVALUE(ROK);

} /* pjUtlReEstSrb1 */

/**
 *
 * @brief 
 *
 *        Utility function that processes the CfgReq with type as
 *        REESTABLISH sent from RRC.
 *
 * @b Description:
 *
 *        This function 
 *        1. Marks the state of the RBs as PJ_STATE_REEST.
 *        2. Calculates the number of RBs undergoing reestablishment.
 *
 *  @param[in] ueCb     UE Control Block Pointer.
 *  @param[in] numEnt   Number of entities undergoing reestablishment
 *  @param[in] cfgEnt   List of entities configured.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlReEstStart
(
KwUeCb         *ueCb,          /* UE Control Block Pointer */
CpjCfgReqInfo  *cfgReq         /* CfgReq for Reestablishment */
)
#else
PUBLIC S16 pjUtlReEstStart(ueCb, cfgReq)
KwUeCb         *ueCb;          /* UE Control Block Pointer */
CpjCfgReqInfo  *cfgReq;        /* CfgReq for Reestablishment */
#endif
{
   U8          entIndex;
   RbCb       *tRbCb;
   PjRbCb     *pjRbCb;
   U8          numEnt;     /* Number of entities undergoing reestablishment */
   CpjCfgEnt  *cfgEnt;     /* List of entities configured.                  */

   TRC2(pjUtlReEstStart);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlReEstStart(ueCb(%d,%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId));

   /* Initialisations */
   ueCb->libInfo.numReEstDrbDl = 0;
   /* kw006.201 fixed the error in initialization */
   ueCb->libInfo.numReEstDrbUl = 0;
   numEnt                      = cfgReq->numEnt;
   cfgEnt                      = cfgReq->cfgEnt;

   /* Update ueCb's state for reestablishment */
   ueCb->libInfo.state = PJ_STATE_REEST;

   /* Loop through all RBs and update states */
   for(entIndex = 0; entIndex < numEnt; entIndex ++)
   {

      /* 1. Continue if request is not to reestablish */
      if(cfgEnt[entIndex].cfgType != CPJ_CFG_REESTABLISH)
      {
         continue;
      }

      /* 2. Fetch the rbCb and intialise the states */
      KW_DBM_FETCH_RBCB(cfgEnt[entIndex].rbId,  \
                        cfgEnt[entIndex].rbType, ueCb, tRbCb);
      if(tRbCb == NULLP)
      {
         continue;
      }

      /* 3. Quit if PDCP not configured for this RB */
      if((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP)
      {
         continue;
      }

      /* 4. Update states */
      pjRbCb   =  &tRbCb->pjRbCb;
      pjRbCb->state  =  PJ_STATE_REEST;

      /* 5. Call the respective reestablishment functions for SRB */
      if(pjRbCb->rbType == PJ_SRB)
      {
         pjUlmReEstSrb(pjRbCb);
         pjDlmReEstSrb(pjRbCb);
      }
      else /* PJ_DRBs */
      {

         /* 6. Update reestablishment variables at UL and DL */
         if((pjRbCb->dir & KW_DIR_DL) == KW_DIR_DL)
         {
            ueCb->libInfo.numReEstDrbDl++;
         }

         if((pjRbCb->dir & KW_DIR_UL) == KW_DIR_UL)
         {
            ueCb->libInfo.numReEstDrbUl++;

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
            pjRbCb->ulCb.obdCnt   = 0;
            pjRbCb->ulCb.transCmp = FALSE;
            if(pjRbCb->mode == PJ_DRB_AM)
            {
               pjRbCb->ulCb.firstReEstCnt   =   pjRbCb->ulCb.lastSubCnt + 1;
            }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

         }

      }/* if(rbType == .... */

   }/* for(entIndex = 0; .. */

   /* if only SRBs have been reestablished proceed with reconfiguration */
   if((ueCb->libInfo.numReEstDrbDl == 0) && 
      (ueCb->libInfo.numReEstDrbUl == 0))
   {
      pjUtlReEstReconfig(ueCb, PJ_SRB);
   }

   RETVALUE(ROK);

} /* pjUtlReEstStart */

/**
 *
 * @brief 
 *
 *        Handler to start reestablishment processing
 *
 * @b Description:
 *
 *        This function 
 *        1. Marks the state of the RBs as PJ_STATE_REEST_HO.
 *        2. Calculates the number of RBs undergoing reestablishment.
 *
 *  @param[in] ueCb    Number of RBs undergoing reestablishment
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlReEstHOStart
(
KwUeCb      *ueCb,           /* Number of RBs undergoing reestablishment*/ 
U32         transId          /* Transaction id of Sdu Status cfm */
)
#else
PUBLIC S16 pjUtlReEstHOStart(ueCb, transId)
KwUeCb      *ueCb;           /* Number of RBs undergoing reestablishment*/ 
U32         transId;         /* Transaction id of Sdu Status cfm */
#endif
{
   U8       rbCnt;
   RbCb     **rbCbLst;
   RbCb     *tRbCb;
   PjRbCb   *pjRbCb;

   TRC2(pjUtlReEstHOStart);

#ifdef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlReEstHOStart(ueCb(%d,%d), transId(%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId, transId));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlReEstHOStart(ueCb(%d,%d), transId(%ld))\n",\
                ueCb->key.ueId, ueCb->key.cellId, transId));
#endif

   /* Initialisations */
   ueCb->libInfo.numReEstDrbDl = 0;
   /*ccpu00122056 Fix for initialization error*/
   ueCb->libInfo.numReEstDrbUl = 0;

   /* Update ueCb's state for reestablishment during handover */
   ueCb->libInfo.state = PJ_STATE_REEST_HO;

   /* Perform initialisations on the HO Info structure */
   PJ_ALLOC(ueCb->hoInfo, sizeof(PjHoInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ueCb->hoInfo == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW141, (ErrVal) RFAILED,
         "PjUiPjuCfgReq: cfgCfm allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   PJ_ALLOC(ueCb->hoInfo->hoCfmInfo, sizeof(PjHoCfmInfo) *  KW_MAX_DRB_PER_UE );
 
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ueCb->hoInfo->hoCfmInfo == NULLP )
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW142, (ErrVal) RFAILED,
         "PjUiPjuCfgReq: HO cfm info allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
 
   ueCb->hoInfo->transId =  transId;

   /* Initialise the reestablishment states for the RBs */
   rbCbLst      = ueCb->srbCb;
   for(rbCnt = 0; rbCnt < KW_MAX_SRB_PER_UE; rbCnt ++)
   {
      /* Check of rbCb available */
      if( (tRbCb = rbCbLst[rbCnt]) == NULLP)
      {
         continue;
      }

      /* check if PDCP not configured for this RB */
      if((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP)
      {
         continue;
      }

      /* Fetch the rbCb and intialise the states */
      pjRbCb   =  &tRbCb->pjRbCb;
      pjRbCb->state  =  PJ_STATE_REEST_HO;

      /*  Call the respective reestablishment functions for SRB */
      pjUlmReEstSrb(pjRbCb);
      pjDlmReEstSrb(pjRbCb);

   }/* for(rbCnt .. */ 

   /* Initialise the reestablishment states for the RBs */
   rbCbLst      = ueCb->drbCb;
   for(rbCnt = 0; rbCnt < KW_MAX_DRB_PER_UE; rbCnt ++)
   {
      /* Check of rbCb available */
      if( (tRbCb = rbCbLst[rbCnt]) == NULLP)
      {
         continue;
      }

      /* check if PDCP not configured for this RB */
      if((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP)
      {
         continue;
      }

      /* Fetch the rbCb and intialise the states */
      pjRbCb   =  &tRbCb->pjRbCb;
      pjRbCb->state  =  PJ_STATE_REEST_HO;

      /*  Update reestablishment variables at UL and DL */
      if((pjRbCb->dir & KW_DIR_DL) == KW_DIR_DL)
      {
         ueCb->libInfo.numReEstDrbDl++;
      }

      if((pjRbCb->dir & KW_DIR_UL) == KW_DIR_UL)
      {
         ueCb->libInfo.numReEstDrbUl++;

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
         pjRbCb->ulCb.obdCnt   = 0;
         pjRbCb->ulCb.transCmp = 0;
         if(pjRbCb->mode == PJ_DRB_AM)
         {
            pjRbCb->ulCb.firstReEstCnt   =   pjRbCb->ulCb.lastSubCnt + 1;
         }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

      }

   }/* for(rbCnt .. */ 

   /* If no DRBs to reestablish, send cfm */
   if((ueCb->libInfo.numReEstDrbDl == 0) &&
      (ueCb->libInfo.numReEstDrbUl == 0))
   {
      pjUtlSndSduStaCfm(ueCb);
   }

   RETVALUE(ROK);
} /* pjUtlReEstHOStart */

/**
 *
 * @brief 
 *
 *        Handler to apply the new security algorithms and 
 *        reset the compression unit.
 *
 * @b Description:
 *
 *        This function 
 *        1. Creates new contexts for Ciphering and Integrity.
 *        2. Closes the old contexts for Ciphering and Integrity.
 *        3. Resets the ROHC unit.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlReEstReconfig
(
KwUeCb       *ueCb,           /* UE Control Block */ 
U8           rbType  /* RB Type */
)
#else
PUBLIC S16 pjUtlReEstReconfig(ueCb, rbType)
KwUeCb       *ueCb;           /* UE Control Block */ 
U8           rbType;  /* RB Type */
#endif
{
   /* kw006.201, ccpu00120574, fixed the warning */
   U8       txIdx;
   U8       rbCnt;
   RbCb     *tRbCb;
   PjRbCb   *pjRbCb;
   PTR      tmpIntCxt;
   PTR      tmpCpCxt;
   PTR      tmpUpCxt;

   TRC2(pjUtlReEstReconfig);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlReEstReconfig(ueCb(%d,%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId));

   tRbCb = NULLP;
   pjRbCb = NULLP;
   /* kw006.201, ccpu00120574, fixed the warning */
   if (rbType != PJ_SRB)
   {
for(rbCnt = 0; rbCnt < KW_MAX_DRB_PER_UE; rbCnt ++)
      {
      if( (tRbCb = ueCb->drbCb[rbCnt]) == NULLP)
      {
         continue;
      }

      /* check if PDCP not configured for this RB */
      if((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP)
      {
         continue;
      }

      pjRbCb   =  &tRbCb->pjRbCb;

      /* Check if compression applicable */
      if(pjRbCb->rohc.hdrCmpUsed == FALSE)
      {
         continue;
      }

      pjUtlCmpReset(pjRbCb);

   }/* for(rbCnt .. */ 
}

   /* Update the secCtxt */
   for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
   {
      if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
             (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_CFG_REEST_ASYNC_CFM))
               {
                  ueCb->secInfo.secCxt.cfgTxId =  txIdx;
                  break;
               }
   }

   if(txIdx == PJ_MAX_ASYNC_CFM)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW143, (ErrVal)0, \
            "pjUtlReEstReconfig: Cant find  reEst Cfg Info \n");
      RETVALUE(RFAILED);
   }

   if(ueCb->secInfo.secAct == TRUE)
   {

      /* Initialisation is done first followed by closure since we
       * do not want the same ctxId to be returned for the new init.
       * In ASYNC case, we drop packets if they are returned from a
       * different ctxId. */

      tmpIntCxt = ueCb->secInfo.intCxtId;  
      tmpCpCxt  = ueCb->secInfo.cpCxtId;    
      tmpUpCxt  = ueCb->secInfo.upCxtId;    

      /* Initialise with securtiy algos */
      pjUtlCpInit(ueCb);
      pjUtlUpInit(ueCb);
      pjUtlIntInit(ueCb);

      /* Close the previous security algos */
      pjUtlIntClose(tmpIntCxt);
      pjUtlCipherClose(tmpCpCxt);
      pjUtlCipherClose(tmpUpCxt);

#ifdef PJ_SEC_ASYNC
      RETVALUE(ROK);
#endif /* PJ_SEC_ASYNC*/
   }

   /* Send ReEstCfm if we dont have to wait for init cfms*/

   /* Call DL Reestablishment */
   pjUtlReEstDl(ueCb);

   RETVALUE(ROK);

} /* pjUtlReEstReconfig */

/**
 *
 * @brief 
 *
 *        Called after the new security algorithms have been applied
 *        and header compression/decompression reset. It starts
 *        reestablishment procedures for downlink DRBs.
 *
 * @b Description:
 *
 *        This function 
 *        1. Creates new contexts for Ciphering and Integrity.
 *        2. Closes the old contexts for Ciphering and Integrity.
 *        3. Resets the ROHC unit.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlReEstDl
(
KwUeCb       *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlReEstDl(ueCb)
KwUeCb       *ueCb;           /* UE Control Block */ 
#endif
{
   U8       rbCnt;
   RbCb     *tRbCb;
   PjRbCb   *pjRbCb;

   TRC2(pjUtlReEstDl);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlReEstDl(ueCb(%d,%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId));

   for(rbCnt = 0; rbCnt < KW_MAX_DRB_PER_UE; rbCnt ++)
   {
      if( ((tRbCb = ueCb->drbCb[rbCnt]) == NULLP) ||
          ((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP))
      {
         continue;
      }

      /* Fetch the rbCb and intialise the states */
      pjRbCb   =  &tRbCb->pjRbCb;

      /*  Check if downlink is available  */
      if(((pjRbCb->dir & KW_DIR_DL) != KW_DIR_DL) ||
         (pjRbCb->state == PJ_STATE_NORMAL))
      {
         pjRbCb->state = PJ_STATE_NORMAL;
         continue;
      }

      /* There is nothing to do for SRBs and for DRB UMS
       * in HO state but to update state */
      if((pjRbCb->state == PJ_STATE_REEST_HO) &&
          (pjRbCb->mode == PJ_DRB_UM))
      {
         pjRbCb->state = PJ_STATE_NORMAL;
         continue;
      }

      if(pjRbCb->mode == PJ_DRB_UM)
      {
         pjDlmReEstDrbUm(pjRbCb);
         /* MS_FIX DL Stall Fix */		 
         pjRbCb->state = PJ_STATE_NORMAL;		 
      }
      else
      {
         if(pjRbCb->state == PJ_STATE_REEST)
         {
            /* MS_FIX DL Stall Fix */
            if(pjRbCb->ulCb.staRepReqd != TRUE)
            {
               pjDlmReEstDrbAm(pjRbCb);
               pjRbCb->state = PJ_STATE_NORMAL;
            }			   
         }
         else
         {
            pjDlmReEstHoDrbAm(pjRbCb);
            /* MS_FIX DL Stall Fix */			
            pjRbCb->state = PJ_STATE_NORMAL;
         }
      }


   }/* for(rbCb = 0; ..... */

   /* Send Cfms and update the state  only 
      when there is some cfgreq cfm is present for other RBs*/
   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      pjUtlSndReEstCfgCfm(ueCb);
   }
   else if(ueCb->libInfo.state == PJ_STATE_REEST_HO)
   {
      pjUtlSndSduStaCfm(ueCb);
   }
   ueCb->libInfo.state = PJ_STATE_NORMAL;

   RETVALUE(ROK);

} /* pjUtlReEstDl */

/**
 *
 * @brief 
 *
 *        Function to sent the REESTABLISHMENT COMPPLETE 
 *        to the RRC.
 *
 * @b Description:
 *
 *        This function 
 *        1. Sends a ReEstablishment Confirm for normal reestablishment.
 *        2. Sends a SDU status confirm for handover reestablishment.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlSndReEstCfgCfm
(
KwUeCb       *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlSndReEstCfgCfm(ueCb)
KwUeCb       *ueCb;           /* UE Control Block */ 
#endif
{
   U8             rbCnt;
   RbCb          *tRbCb;
   CpjCfgCfmInfo *cfgCfmInfo;
   U8             txIdx;
   PjAsyncCfm    *asyncCfm    = NULLP;

   TRC2(pjUtlSndReEstCfgCfm);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlSndReEstCfgCfm(ueCb(%d,%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId));

   ueCb->libInfo.state  =  PJ_STATE_NORMAL;

   /* Pick up the confirm info */
   for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
   {
      if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
         (ueCb->libInfo.asyncCfm[txIdx]->cfmType & PJ_CFG_REEST_ASYNC_CFM))
      {
         asyncCfm =  ueCb->libInfo.asyncCfm[txIdx];
         break;
      }
   }

   /* its a reestablishment with only srb1 configured */
   if(asyncCfm == NULLP)
   {
      RETVALUE(ROK);
   }

   for(rbCnt = 0; rbCnt < KW_MAX_SRB_PER_UE; rbCnt ++)
   {
      if( ((tRbCb = ueCb->srbCb[rbCnt]) == NULLP)    ||
          ((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP) ||
          (tRbCb->pjRbCb.state ==  PJ_STATE_NORMAL))
      {
         continue;
      }

      /* Fetch the rbCb and intialise the states */
      tRbCb->pjRbCb.state  =  PJ_STATE_NORMAL;
   } 

   PJ_ALLOC(cfgCfmInfo, sizeof(CpjCfgCfmInfo));
   if(cfgCfmInfo == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW144, (ErrVal)0, \
            "pjUtlSndReEstCfgCfm: Alloc Failed \n");
      RETVALUE(RFAILED);
   }

   pjUtlFillReEstCfgCfmInfo(cfgCfmInfo, asyncCfm);
   PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

   PjUiCpjCfgCfm(&kwCb.cpjSap.pst, kwCb.cpjSap.suId, cfgCfmInfo);         

   RETVALUE(ROK);
} /* pjUtlSndReEstCfgCfm */
/**
 *
 * @brief Handler to store the confirms while we wait for the init
 *        confirms from the off-board.
 *       
 *
 * @b Description
 *        This function is invoked during security config request,
 *        reestablishment request and config request if the libraries
 *        are off-boarded. This stores the entries in a confirm list
 *        while waiting for the confirm from the offboard.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/

#ifdef ANSI
PUBLIC S16 pjUtlSaveCfmInfo
(
KwUeCb      *ueCb,           /* Pointer to UeCb */ 
U8          cfmType,         /* Confirm type */
Bool        startTmr,        /* Start Timer */
U8          entity,          /* Confirm type */
PTR         cfmPtr,          /* Pointer to the structure for confirm */
PTR         cfgPtr           /* Pointer to the structure for request */
)
#else
PUBLIC S16 pjUtlSaveCfmInfo(ueCb, cfmType, startTmr, entity, cfmPtr, cfgPtr)
KwUeCb      *ueCb;           /* Pointer to UeCb */ 
U8          cfmType;         /* Confirm type */
Bool        startTmr;        /* Start Timer */
U8          entity;          /* Confirm type */
PTR         cfmPtr;          /* Pointer to the structure for confirm */
PTR         cfgPtr;          /* Pointer to the structure for request */
#endif
{
   S16               ret;        /* Return value            */
   CpjSecCfgCfmInfo  *secCfm;    /* Security confirm        */
   CpjReEstCfmInfo   *reEstCfm;  /* Reestablishment confirm */
   CpjCfgCfmInfo     *cfgCfm;    /* Config confirm          */
   U16               txId;       /* Transaction Id          */
   PjAsyncCfm        *tAsyncCfm; /* Pointer to the async Cfm*/
   CpjCfgReqInfo     *cfgReq;    /* Pointer to the config request */
   U8                cfmIdx;     /* index for loopoing */


   TRC3(pjUtlSaveCfmInfo)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
   "pjUtlSaveCfmInfo(ueCb(%d,%d), cfmType(%d), startTmr(%d), entity(%d))\n",\
   ueCb->key.ueId, ueCb->key.cellId, cfmType, startTmr, entity));

   /* Initialisations */
   ret      = ROK;
   secCfm   = NULLP;
   reEstCfm = NULLP;
   cfgCfm   = NULLP;
   cfgReq   = NULLP;
   txId     = ueCb->libInfo.nxtAvlbTxId;

   if(txId == PJ_MAX_ASYNC_CFM)
   {
      /* The entire list is full, have to return negative confirms */
      RETVALUE(RFAILED);
   }

   /* is entry already present */
   if(ueCb->libInfo.asyncCfm[txId] != NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Allocate the structure */
   PJ_ALLOC(ueCb->libInfo.asyncCfm[txId],  sizeof(PjAsyncCfm));
   if(ueCb->libInfo.asyncCfm[txId]== NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW145, (ErrVal)0,       \
            "pjUtlSaveCfmInfo: Alloc Failed \n");
      RETVALUE(RFAILED);
   }

   /* Initialise the structure */
   tAsyncCfm   =   ueCb->libInfo.asyncCfm[txId];
   tAsyncCfm->libInitBitMask   =   ueCb->libInfo.tLibInitBitMask;
   tAsyncCfm->cmpInitBitMask   =   ueCb->libInfo.tCmpInitBitMask;
   tAsyncCfm->entity           =   entity;
   tAsyncCfm->startTmr         =   startTmr;

   ueCb->libInfo.tLibInitBitMask =  0; 
   ueCb->libInfo.tCmpInitBitMask =  0; 
   
   /* Assign the values */
   if(cfmType  ==   PJ_SEC_ASYNC_CFM)
   {
      secCfm   =  (CpjSecCfgCfmInfo *)cfmPtr;
      tAsyncCfm->cfmType =  cfmType;
      tAsyncCfm->ueId    = secCfm->ueId;
      tAsyncCfm->cellId  = secCfm->cellId;
      tAsyncCfm->transId = secCfm->transId;
   }
   else if(cfmType  ==   PJ_REEST_ASYNC_CFM)
   {
      reEstCfm   =  (CpjReEstCfmInfo *)cfmPtr;
      tAsyncCfm->cfmType =  cfmType;
      tAsyncCfm->ueId    = reEstCfm->ueId;
      tAsyncCfm->cellId  = reEstCfm->cellId;
      tAsyncCfm->transId = reEstCfm->transId;
   }
   else if((cfmType  & PJ_CFG_ASYNC_CFM)  ||
           (cfmType  & PJ_CFG_REEST_ASYNC_CFM)||
           (cfmType  & PJ_CFG_UEDEL_ASYNC_CFM))
   {
      if(entity   == ENTPJ)
      {
         cfgCfm   =  (CpjCfgCfmInfo *)cfmPtr;
         cfgReq   =  (CpjCfgReqInfo *)cfgPtr;

         tAsyncCfm->cfmType = cfmType;
         tAsyncCfm->ueId    = cfgCfm->ueId;
         tAsyncCfm->cellId  = cfgCfm->cellId;
         tAsyncCfm->transId = cfgCfm->transId;
         tAsyncCfm->numEnt  = cfgReq->numEnt;

         for ( cfmIdx = 0; cfmIdx < cfgReq->numEnt; cfmIdx++ )
         {
            tAsyncCfm->cfmEnt[cfmIdx].status  = cfgCfm->cfmEnt[cfmIdx].status;
            tAsyncCfm->cfmEnt[cfmIdx].reason  = cfgCfm->cfmEnt[cfmIdx].reason;
            tAsyncCfm->cfmEnt[cfmIdx].rbId    = cfgCfm->cfmEnt[cfmIdx].rbId;
            tAsyncCfm->cfmEnt[cfmIdx].rbType  = cfgCfm->cfmEnt[cfmIdx].rbType;
            tAsyncCfm->cfmEnt[cfmIdx].cfgType = cfgReq->cfgEnt[cfmIdx].cfgType;
         }
      }
      else if(entity == ENTKW)
      {
         CkwCfgInfo     *kwCfg;
         CkwCfgCfmInfo  *kwCfgCfm;       /* Configuraiton Confirm */

         kwCfgCfm   =  (CkwCfgCfmInfo *)cfmPtr;
         kwCfg      =  (CkwCfgInfo *)cfgPtr;

         tAsyncCfm->cfmType = cfmType;
         tAsyncCfm->transId = kwCfgCfm->transId;
         tAsyncCfm->ueId    = kwCfgCfm->ueId;
         tAsyncCfm->cellId  = kwCfgCfm->cellId;
         tAsyncCfm->numEnt  = kwCfgCfm->numEnt;

         for ( cfmIdx = 0; cfmIdx < kwCfg->numEnt; cfmIdx++ )
         {
            tAsyncCfm->cfmEnt[cfmIdx].status  = 
                                 kwCfgCfm->entCfgCfm[cfmIdx].status.status;
            tAsyncCfm->cfmEnt[cfmIdx].reason  = 
                                 kwCfgCfm->entCfgCfm[cfmIdx].status.reason;
            tAsyncCfm->cfmEnt[cfmIdx].rbId    = 
                                 kwCfgCfm->entCfgCfm[cfmIdx].rbId;
            tAsyncCfm->cfmEnt[cfmIdx].rbType  = 
                                 kwCfgCfm->entCfgCfm[cfmIdx].rbType;
         }
      }
   }

   /* Start timer */
   /* Timer should not be started when SEC is SYNC for ReEst Req
    * and for the subsequent config req for REEST. The config req
    * after reest req can still start the timer in the case comp being
    * async and sec being sync if it also adds a RB and a compInit has
    * to be sent . 
    * */

   if(tAsyncCfm->startTmr  == TRUE)
   {
      if((pjChkTmr((PTR)ueCb, PJ_EVT_OBD_WAIT_TMR)) == FALSE)
      {
         pjStartTmr((PTR)ueCb, PJ_EVT_OBD_WAIT_TMR);
         ueCb->libInfo.crntTmrTxId  =  ueCb->libInfo.nxtAvlbTxId;
      }
   }

   /* update nxtAvlbTxId to the next empty slot */
   for(cfmIdx = 0; cfmIdx < PJ_MAX_ASYNC_CFM; cfmIdx++)
   {
      ueCb->libInfo.nxtAvlbTxId = 
         (ueCb->libInfo.nxtAvlbTxId + 1) % PJ_MAX_ASYNC_CFM;

      if(ueCb->libInfo.asyncCfm[ueCb->libInfo.nxtAvlbTxId] == NULLP)
      {
         break;
      }
   }

   /* Cant find an empty slot ? */
   if(cfmIdx == PJ_MAX_ASYNC_CFM)
   {
      ueCb->libInfo.nxtAvlbTxId = PJ_MAX_ASYNC_CFM;
   }

   RETVALUE(ret);
}/* pjUtlSaveCfmInfo */

/**
 *
 * @brief Handler to store update the bitmaks and send confirms if necessary.
 *       
 *
 * @b Description
 *        This function is invoked when receiving a InitCfm from Offboard.
 *        It updates the bit masks and checks if it is necesary to send a 
 *        confirm.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/

#ifdef ANSI
PUBLIC S16 pjUtlHdlObdInitCfm
(
KwUeCb      *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,           /* Transaction Index for UeCb */
U8          cfmType,         /* Confirm type */
U8          maskVal          /* mask value */
)
#else
PUBLIC S16 pjUtlHdlObdInitCfm(ueCb, txIdx, cfmType, maskVal)
KwUeCb      *ueCb;           /* Pointer to UeCb */ 
U16         txIdx;           /* Transaction Index for UeCb */
U8          cfmType;         /* Confirm type */
U8          maskVal;         /* mask value */
#endif
{

   CpjSecCfgCfmInfo *secCfgCfm;   /* Security config confirm */
   CpjReEstCfmInfo  *reEstCfm;    /* Reest config confirm */
   PjAsyncCfm       *asyncCfm;
   CpjCfgCfmInfo    *cfgCfm;
   U8               idx;

   TRC3(pjUtlHdlObdInitCfm)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf, 
      "pjUtlHdlObdInitCfm(ueCb(%d,%d), txIdx(%d), cfmType(%d), maskVal(%d))\n",\
      ueCb->key.ueId, ueCb->key.cellId, txIdx, cfmType, maskVal));

   /* Initialisations */
   secCfgCfm   =  NULLP;
   reEstCfm    =  NULLP;
   cfgCfm      =  NULLP;
   asyncCfm    =  NULLP;

   if((txIdx  == PJ_MAX_ASYNC_CFM) ||
      (ueCb->libInfo.asyncCfm[txIdx] == NULLP))
   {
      RETVALUE(RFAILED);
   }

   asyncCfm =  ueCb->libInfo.asyncCfm[txIdx];

   if(cfmType & PJ_SEC_INIT_CFM)
   {
      /* Update bit mask and check if all confirms have been received */
      asyncCfm->libInitBitMask ^= (maskVal);
      if(asyncCfm->libInitBitMask == 0)
      {
         PJ_CHK_RESTART_OBD_TIMER(ueCb, txIdx);
         if(ueCb->libInfo.state == PJ_STATE_NORMAL)
         {
            /* Send security config confirm */
            PJ_ALLOC(secCfgCfm, sizeof(CpjSecCfgCfmInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
            if (secCfgCfm == NULLP)
            {
               PJLOGERROR(ERRCLS_ADD_RES, EKW146, (ErrVal)0,
                     "PjLibObdIntInitCfm: Alloc Failed for secCfgCfm\n");
               RETVALUE(RFAILED);
            }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
            PJ_FILL_SEC_CFM_INFO(secCfgCfm, asyncCfm);

            PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

            PjUiCpjSecCfgCfm(&kwCb.cpjSap.pst, kwCb.cpjSap.suId, secCfgCfm);
         }
         else if(ueCb->libInfo.state == PJ_STATE_REEST)
         {
            /* Send ReEstCfm */
            PJ_ALLOC(reEstCfm, sizeof(CpjReEstCfmInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
            if (reEstCfm == NULLP)
            {
               PJLOGERROR(ERRCLS_ADD_RES, EKW147, (ErrVal)0,
                     "PjLibObdIntInitCfm: Alloc Failed for reEstCfm\n");
               RETVALUE(RFAILED);
            }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

            PJ_FILL_REEST_CFM_INFO(reEstCfm, asyncCfm, LCM_PRIM_OK);

            /* Some house keeping work */
            PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

            PjUiCpjReEstCfm(&kwCb.cpjSap.pst, kwCb.cpjSap.suId, reEstCfm);

            /* Call DL Reestablishment */
            pjUtlReEstDl(ueCb);
         }
      }
   }
   else if(cfmType  & PJ_CMP_INIT_CFM)
   {
      /* Check if all confirms have been received */
      asyncCfm->cmpInitBitMask &= ~(1 << (maskVal - 1));

      /* Update the status and reason for the received CmpInitCfm */
      for ( idx = 0; idx < asyncCfm->numEnt; idx++ )
      {
         if ( asyncCfm->cfmEnt[idx].rbId == maskVal)
         {
            asyncCfm->cfmEnt[idx].status = CPJ_CFG_CFM_OK;
            asyncCfm->cfmEnt[idx].reason = CPJ_CFG_REAS_NONE;
            break;
         }
      }

      /* Check if we can send confirm */
      if(asyncCfm->cmpInitBitMask == 0)
      {
         asyncCfm->libInitBitMask ^= (PJ_LIB_COMP_BIT_MASK);

         if((asyncCfm->libInitBitMask == 0) &&
            !(asyncCfm->cfmType & PJ_CFG_REEST_ASYNC_CFM))
         {

            /* Send config confirm */
            PJ_ALLOC(cfgCfm, sizeof(CpjCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
            if (cfgCfm == NULLP)
            {
               PJLOGERROR(ERRCLS_ADD_RES, EKW148, (ErrVal) RFAILED,
                     "PjUiPjuCfgReq: cfgCfm allocation Failed.");
               RETVALUE(RFAILED);
            }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
            cfgCfm->transId = asyncCfm->transId;
            cfgCfm->ueId    = asyncCfm->ueId;
            cfgCfm->cellId  = asyncCfm->cellId;
            cfgCfm->numEnt  = asyncCfm->numEnt; 
            for ( idx = 0; idx < asyncCfm->numEnt; idx++ )
            {
               cfgCfm->cfmEnt[idx].status = asyncCfm->cfmEnt[idx].status;  
               cfgCfm->cfmEnt[idx].reason = asyncCfm->cfmEnt[idx].reason;  
               cfgCfm->cfmEnt[idx].rbId   = asyncCfm->cfmEnt[idx].rbId;
               cfgCfm->cfmEnt[idx].rbType = asyncCfm->cfmEnt[idx].rbType;
            }

            /* Some house keeping work */
            PJ_CHK_RESTART_OBD_TIMER(ueCb, txIdx);
            PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

            /* Send confirmtion to the User */
            PjUiCpjCfgCfm(&(kwCb.cpjSap.pst), kwCb.cpjSap.suId, cfgCfm);
         }
      }
   }

   RETVALUE(ROK);
}/* pjUtlHdlObdInitCfm */

/**
 *
 * @brief Handler for init off-board timer expiry.
 *
 *
 * @b Description
 *        This function is called when the off-board timer expires for
 *        a Init Req of a channel (either for ciphering/integrity/compression).
 *        This function sends a confirm with failure for SecCfgReq if the
 *        state of the RB is normal. It sends a confirm with failure
 *        for config request(only for RBs with Re-establishment Req, a failure 
 *        is stored in the confirm) if the state of the RB is re-establishment.
 *
 *  @param[in] ueCb    UE control block.
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PUBLIC S16 pjUtlHdlInitObdTmrExp
(
KwUeCb *ueCb
)
#else
PUBLIC S16 pjUtlHdlInitObdTmrExp(ueCb)
KwUeCb *ueCb;
#endif
{
   S16               ret;           /* Return Value */
   CpjSecCfgCfmInfo  *secCfgCfm;    /* Security Cfg Confirm Info */
   CpjCfgCfmInfo     *cfgCfm;       /* Config Confirm Info */
   CpjReEstCfmInfo   *reEstCfm;     /* Reest config confirm */
   CkwCfgCfmInfo     *kwCfgCfm;
   PjLibInfo         *libInfo;      /* Off-board Info */
   PjAsyncCfm        *asyncCfm;     /* Async Cfm Info */
   KwCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */
   KwCkwSapCb        *ckwSap;       /* CKW SAP Control Block */
   U16               txIdx;         /* Tx Idx */
   U16               idx;           /* Index for looping */
   U16               cfgIdx;

   TRC3(pjUtlHdlInitObdTmrExp)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlHdlInitObdTmrExp(ueCb(%d,%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId));

   ret       = ROK;
   cpjSap    = &(kwCb.cpjSap);
   ckwSap    = &(kwCb.ckwSap);
   secCfgCfm = NULLP;
   libInfo   = &ueCb->libInfo;
   txIdx     = libInfo->crntTmrTxId;

   /* Restart the timer */
   PJ_CHK_RESTART_OBD_TIMER(ueCb, txIdx);

   /* Shouldnt happen, nevertheless, check */
   if( txIdx >= PJ_MAX_ASYNC_CFM || libInfo->asyncCfm[txIdx] == NULLP)
   {
      RETVALUE(ROK);
   }

   asyncCfm =  libInfo->asyncCfm[txIdx];

   /* Security confirm */
   if(asyncCfm->cfmType & PJ_SEC_ASYNC_CFM)
   {
      PJ_ALLOC(secCfgCfm, sizeof(CpjSecCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (secCfgCfm == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW149, (ErrVal) RFAILED,
               "pjUtlHdlInitObdTmrExp: PJ_ALLOC Failed for secCfgCfm.");
         RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      PJ_FILL_SEC_CFM_INFO(secCfgCfm, asyncCfm);
      secCfgCfm->status = CPJ_CFG_CFM_NOK;
      secCfgCfm->reason = CPJ_CFG_REAS_OBD_TIMEOUT;

      PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

      PjUiCpjSecCfgCfm(&(cpjSap->pst), cpjSap->suId, secCfgCfm);
   }

   /* Configuration confirms */
   else if ((asyncCfm->cfmType & PJ_CFG_ASYNC_CFM) ||
            (asyncCfm->cfmType & PJ_CFG_REEST_ASYNC_CFM))
   {
      PJ_FILL_TIMEOUT_CFG_CFM_INFO(cfgCfm, asyncCfm);

      PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

      PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
   }
   else if (asyncCfm->cfmType & PJ_REEST_ASYNC_CFM)
   {
      /* Send ReEstCfm */
      PJ_ALLOC(reEstCfm, sizeof(CpjReEstCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (reEstCfm == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW150, (ErrVal) RFAILED,
               "pjUtlHdlInitObdTmrExp: PJ_ALLOC Failed for reEstCfm.");
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      PJ_FILL_REEST_CFM_INFO(reEstCfm, asyncCfm, LCM_PRIM_NOK);

      PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

      PjUiCpjReEstCfm(&kwCb.cpjSap.pst, kwCb.cpjSap.suId, reEstCfm);

      /* Send the config confirm also if any exists */
      cfgIdx   =  txIdx + 1;

      for(idx = 0; idx < PJ_MAX_ASYNC_CFM; idx++, cfgIdx++)
      {
         cfgIdx %=   PJ_MAX_ASYNC_CFM;
         if((libInfo->asyncCfm[cfgIdx] != NULLP) &&
            (libInfo->asyncCfm[cfgIdx]->cfmType & PJ_CFG_REEST_ASYNC_CFM))
         {
            break;
         }
      }

      /* This scenario occurs when only SRB1 is configured */
      if(idx == PJ_MAX_ASYNC_CFM)
      {
         RETVALUE(ROK);
      }

      PJ_CHK_RESTART_OBD_TIMER(ueCb, cfgIdx);
      asyncCfm =  libInfo->asyncCfm[cfgIdx];

      PJ_FILL_TIMEOUT_CFG_CFM_INFO(cfgCfm, asyncCfm);

      PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);

      PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
   }
   else if(asyncCfm->cfmType & PJ_CFG_UEDEL_ASYNC_CFM)
   {
      /*ccpu00121544:Added a check before stopping the timer*/
      if((pjChkTmr((PTR)ueCb, PJ_EVT_OBD_WAIT_TMR)) == TRUE)
      {
         pjStopTmr((PTR)ueCb, PJ_EVT_OBD_WAIT_TMR);
      }
      /* Delete ueCb entry from ueLstCp */
      ret = cmHashListDelete(&(kwCb.ueLstCp), (PTR) ueCb);
      if (ret != ROK)
      {

#ifdef DEBUGP
         PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_ERROR), (_kwPBuf,
                  "pjUtlHdlInitObdTmrExp: cmHashListDelete Failed for ueCb.\n"));
#endif
      }

      if ( asyncCfm->entity == ENTPJ )
      {
         PJ_ALLOC(cfgCfm, sizeof(CpjCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (cfgCfm == NULLP)
         {
            PJLOGERROR(ERRCLS_ADD_RES, EKW151, (ErrVal) RFAILED,
                  "pjUtlHdlInitObdTmrExp: PJ_ALLOC Failed for cfgCfm.");
            RETVALUE(RFAILED);
         }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

         for ( idx = 0; idx < asyncCfm->numEnt; idx++)
         {
            cfgCfm->cfmEnt[idx].status = asyncCfm->cfmEnt[idx].status;
            cfgCfm->cfmEnt[idx].reason = asyncCfm->cfmEnt[idx].reason;
            cfgCfm->cfmEnt[idx].rbId   = asyncCfm->cfmEnt[idx].rbId;
            cfgCfm->cfmEnt[idx].rbType = asyncCfm->cfmEnt[idx].rbType;
         }
         cfgCfm->ueId    = asyncCfm->ueId;
         cfgCfm->cellId  = asyncCfm->cellId;
         cfgCfm->transId = asyncCfm->transId;
         cfgCfm->numEnt  = asyncCfm->numEnt;

         /* delete all confirms present there */
         for(idx = 0; idx < PJ_MAX_ASYNC_CFM; idx++)
         {
            if(libInfo->asyncCfm[idx] != NULLP)
            {
               PJ_FREE(ueCb->libInfo.asyncCfm[idx], sizeof(PjAsyncCfm)); 
               ueCb->libInfo.asyncCfm[idx] = NULLP; 
            }
         }

         KW_FREE(ueCb, sizeof(KwUeCb));
         PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
      }
      else if ( asyncCfm->entity == ENTKW )
      {
         PJ_ALLOC(kwCfgCfm, sizeof(CkwCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (kwCfgCfm == NULLP)
         {
            PJLOGERROR(ERRCLS_ADD_RES, EKW152, (ErrVal) RFAILED,
                  "pjUtlHdlInitObdTmrExp: PJ_ALLOC Failed for kwCfgCfm.");
            RETVALUE(RFAILED);
         }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
         for ( idx = 0; idx < asyncCfm->numEnt; idx++)
         {
            kwCfgCfm->entCfgCfm[idx].status.status = asyncCfm->cfmEnt[idx].status;
            kwCfgCfm->entCfgCfm[idx].status.reason = asyncCfm->cfmEnt[idx].reason;
            kwCfgCfm->entCfgCfm[idx].rbId          = asyncCfm->cfmEnt[idx].rbId;
            kwCfgCfm->entCfgCfm[idx].rbType        = asyncCfm->cfmEnt[idx].rbType;
         }
         kwCfgCfm->ueId    = asyncCfm->ueId;
         kwCfgCfm->cellId  = asyncCfm->cellId;
         kwCfgCfm->transId = asyncCfm->transId;
         kwCfgCfm->numEnt  = asyncCfm->numEnt;

         /* delete all confirms present there */
         for(idx = 0; idx < PJ_MAX_ASYNC_CFM; idx++)
         {
            if(libInfo->asyncCfm[idx] != NULLP)
            {
               PJ_FREE(ueCb->libInfo.asyncCfm[idx], sizeof(PjAsyncCfm)); 
               ueCb->libInfo.asyncCfm[idx] = NULLP; 
            }
         }

         KW_FREE(ueCb, sizeof(KwUeCb));
         KwUiCkwCfgCfm(&(ckwSap->pst), ckwSap->suId, kwCfgCfm);
      }
   }

   RETVALUE(ret);
} /* pjHdlUeDelWaitTmrExp */


/**
 *
 * @brief 
 *
 *        Function to sent the REESTABLISHMENT COMPPLETE 
 *        to the RRC.
 *
 * @b Description:
 *
 *        This function 
 *        1. Sends a ReEstablishment Confirm for normal reestablishment.
 *        2. Sends a SDU status confirm for handover reestablishment.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlSndSduStaCfm
(
KwUeCb       *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlSndSduStaCfm(ueCb)
KwUeCb       *ueCb;           /* UE Control Block */ 
#endif
{
   U8       rbCnt;
   RbCb     *tRbCb;
   CpjSduStaCfmInfo  *cfmInfo;

   TRC2(pjUtlSndSduStaCfm);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlSndSduStaCfm(ueCb(%d,%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId));

   ueCb->libInfo.state  =  PJ_STATE_NORMAL;
   for(rbCnt = 0; rbCnt < KW_MAX_SRB_PER_UE; rbCnt ++)
   {
      if( ((tRbCb = ueCb->srbCb[rbCnt]) == NULLP)    ||
          ((tRbCb->rbCfgStat &  PJ_CFG_PDCP) != PJ_CFG_PDCP) ||
          (tRbCb->pjRbCb.state ==  PJ_STATE_NORMAL))
      {
         continue;
      }

      /* Fetch the rbCb and intialise the states */
      tRbCb->pjRbCb.state  =  PJ_STATE_NORMAL;
   } 

   /* Perform processing for filling up cfm structures */
   PJ_ALLOC(cfmInfo, sizeof(CpjSduStaCfmInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(cfmInfo == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW153, (ErrVal)0,       \
            "pjUtlSndSduStaCfm: Alloc Failed \n");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   PJ_FILL_SDU_CFM_INFO(cfmInfo, ueCb)

   PjUiCpjSduStaCfm(&kwCb.cpjSap.pst, kwCb.cpjSap.suId, cfmInfo);

   RETVALUE(ROK);

} /* pjUtlSndSduStaCfm */

/**
 *
 * @brief Handler to clean up all the PDCP Control Blocks.
 *       
 *
 * @b Description
 *        This function is invoked by LMM to shutdown the
 *        layer. This cleans up recBuf and txBuf of all the
 *        PDCP RBs in all the UEs.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/

#ifdef ANSI
PUBLIC S16 pjUtlShutdown
(
)
#else
PUBLIC S16 pjUtlShutdown()
#endif
{
   S16         ret;              /* Return Value */
   KwUeCb      *ueCb;            /* UE Control Block */
   KwUeCb      *prevUeCb;        /* Previos UE Control Block */
   RbCb        *rbCb;            /* RB Control Block */
   RbCb        **rbCbLst;        /* RB Control Block List */
   PjSec       secInfo;          /* Security info of UE */
   U8          idx;              /* Index for RBs */
#ifdef LTE_L2_MEAS
   PjL2MeasEvtCb  *measEvtCb = NULLP;
#endif
   TRC3(pjUtlShutdown)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_BRIEF), 
          (_kwPBuf, "pjUtlShutdown()\n"));

   ret      = ROK;
   ueCb     = NULLP;
   prevUeCb = NULLP;
   rbCb     = NULLP;
   rbCbLst  = NULLP;
   idx      = 0;

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (cmHashListGetNext(&(kwCb.ueLstCp), (PTR) prevUeCb, (PTR *)&ueCb) == ROK)
   {
      if ( ueCb != NULLP )
      {
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
         if (ueCb->libInfo.obdTmr.tmrEvnt == PJ_EVT_OBD_TMR)
         {
            pjStopTmr((PTR)ueCb, PJ_EVT_OBD_TMR);
         }
#endif
         rbCbLst = ueCb->srbCb;
         for (idx = 0; idx < KW_MAX_SRB_PER_UE; idx++)
         {
             rbCb = rbCbLst[idx];
             if (rbCb != NULLP)
             {
               /* Free the Buffers of RbCb */
               if ( rbCb->rbCfgStat & PJ_CFG_PDCP )
               {
                  pjUtlFreeRb(&rbCb->pjRbCb);
               }
             }
         }
         rbCbLst = ueCb->drbCb;
         for (idx = 0; idx < KW_MAX_DRB_PER_UE; idx++)
         {
             rbCb = rbCbLst[idx];
             if (rbCb != NULLP)
             {
               /* Free the Buffers of RbCb */
               if ( rbCb->rbCfgStat & PJ_CFG_PDCP )
               {
                  pjUtlFreeRb(&rbCb->pjRbCb);
               }
             }
         }
         /* Close all the context info for offboarding */
         secInfo = ueCb->secInfo;
         pjUtlCipherClose(secInfo.cpCxtId);
         pjUtlCipherClose(secInfo.upCxtId);
         pjUtlIntClose(secInfo.intCxtId);

         /* Delete hoInfo if present */
         if (ueCb->hoInfo != NULLP)
         {
            for (idx = 0; idx < KW_MAX_DRB_PER_UE; idx++)
            {
               if (ueCb->hoInfo->hoCfmInfo[idx].pres == TRUE)
               {
                  PJ_FREE(ueCb->hoInfo->hoCfmInfo[idx].ul.ulBitMap,
                        (ueCb->hoInfo->hoCfmInfo[idx].ul.numBits % 8)?
                        ((ueCb->hoInfo->hoCfmInfo[idx].ul.numBits / 8) + 1):
                        (ueCb->hoInfo->hoCfmInfo[idx].ul.numBits / 8 ));
               }
            }

            PJ_FREE(ueCb->hoInfo->hoCfmInfo,                         \
                           (KW_MAX_DRB_PER_UE * sizeof(PjHoCfmInfo)));
            PJ_FREE(ueCb->hoInfo, sizeof(PjHoInfo));                 
         }
      }
      prevUeCb = ueCb;
   }
#ifdef LTE_L2_MEAS
   for(idx = 0; idx < PJ_MAX_L2MEAS_EVT; idx++)
   {
      if(kwCb.pjL2Cb.pjMeasEvtCb[idx] != NULLP)
      {
         measEvtCb = kwCb.pjL2Cb.pjMeasEvtCb[idx];
         /*ccpu00121544:Added a check before stopping the timer*/
         if((pjChkTmr((PTR)measEvtCb, PJ_EVT_L2_TMR)) == TRUE)
         {
            pjStopTmr((PTR)measEvtCb, PJ_EVT_L2_TMR);
         }
         kwCb.pjL2Cb.pjNumMeas--;
         PJ_FREE(measEvtCb, sizeof(PjL2MeasEvtCb));
         kwCb.pjL2Cb.pjMeasEvtCb[idx] = NULLP;
      }
   }
#endif

   kwCb.init.cfgDone &= ~PJ_LMM_GENCFG_DONE;
   RETVALUE(ret);
}

/**
 * 
 * @brief 
 *
 *        Handler to free a PDCP Control Block.
 *
 * @b Description:
 * 
 *        This function frees the txBuf and rxBuf in a PDCP control block.
 *        and closes the compression channel opened. It stops the timers 
 *        of this control block if they are running.
 *
 *  @param[in]  pst        Post structure 
 *  @param[in]  secCxt     Context for Integrity protection/verification 
 *  @param[in]  ctxId      Integrity Context Id
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC Void pjUtlFreeRb
(
PjRbCb *pjRbCb
)
#else
PUBLIC Void pjUtlFreeRb(pjRbCb)
PjRbCb *pjRbCb;
#endif
{
   TRC3(pjUtlFreeRb)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlFreeRb()\n"));

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   if (pjRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
   {
       pjStopTmr((PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }
   if (pjRbCb->ulCb.obdTmr.tmrEvnt == PJ_EVT_UL_OBD_TMR)
   {
       pjStopTmr((PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }
#endif
   if (pjRbCb->dlCb.discTmr.tmrEvnt == PJ_EVT_DISC_TMR)
   {
       pjStopTmr((PTR)pjRbCb, PJ_EVT_DISC_TMR);
   }
   pjDbmRxDeInit(&(pjRbCb->ulCb.recBuf));
   pjDbmTxDeInit(&(pjRbCb->dlCb.txBuf));
   
   pjUtlCmpClose(pjRbCb->cmpCxtId);
   /*kw004.201 Adding of Missing Trace in LTE RLC PDCP*/
   RETVOID;
}

/* kw005.201 added function to send data forward indication message */
/**
 * 
 * @brief 
 *
 *        Handler to send Data Fwd Indication
 *
 * @b Description:
 *      This function sends Data Fwd Indication message to PDCP user 
 *
 *  @param[in]  pjRbCb     PDCP RbCb
 *  @param[in]  datFwdInd  Data forward indication
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC S16 pjUtlSndDatFwdInd
(
PjRbCb           *pjRbCb,
PjuDatFwdIndInfo *datFwdInd
)
#else
PUBLIC S16 pjUtlSndDatFwdInd(pjRbCb, datFwdInd)
PjRbCb           *pjRbCb;
PjuDatFwdIndInfo *datFwdInd;
#endif
{
   CmLtePdcpId  *pdcpId;
   KwPjuSapCb   *pjuSap;

   TRC3(pjUtlSndDatFwdInd)
   
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlSndDatFwdInd()\n"));

   pjuSap   = &(kwCb.pjuSap[PJ_DRB_SAP]);

   PJ_ALLOC(pdcpId, sizeof(CmLtePdcpId));

   if(pdcpId != NULLP)
   {
      pdcpId->ueId   =  pjRbCb->ueCb->key.ueId;
      pdcpId->cellId =  pjRbCb->ueCb->key.cellId;
      pdcpId->rbId   =  pjRbCb->rbId;
      pdcpId->rbType =  pjRbCb->rbType;
      
      kwCb.pjGenSts.numPktsFrwd += datFwdInd->numSdus;
      /* If trace flag is enabled send the trace indication */
      if(kwCb.init.trc == TRUE)
      {                       
         /* Populate the trace params */
         pjLmmSendTrc(EVTPJUDATFWDIND, NULLP);
      }
      PjUiPjuDatFwdInd(&(pjuSap->pst), pjuSap->suId, pdcpId, datFwdInd);
   }

  RETVALUE(ROK);

}/* end of pjUtlSndDatFwdInd */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief Handler for L2 Measurement timer expiry.
 *
 *
 * @b Description
 *        This function is called when the l2 measurement timer expires. 
 *        This function sends a consolidates the mesaurements taken during
 *        this time and sends the confirm .
 *
 *  @param[in] measEvtCb    Measurement Event Control Block.
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PUBLIC S16 pjUtlHdlL2TmrExp
(
PjL2MeasEvtCb *measEvtCb
)
#else
PUBLIC S16 pjUtlHdlL2TmrExp(measEvtCb)
PjL2MeasEvtCb *measEvtCb;
#endif
{
   CmLList        *node;
   U32            idx;
   PjL2MeasCb     *measCb;
   PjL2MeasCfmEvt measCfmEvt;
   PjRbCb         *pjRbCb;
   PjTxEnt        *txEnt      = NULLP;
   U8             qciIdx;

   TRC3(pjUtlHdlL2TmrExp)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlHdlL2TmrExp(transId(%ld))\n", measEvtCb->transId));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlHdlL2TmrExp(transId(%d))\n", measEvtCb->transId));
#endif
   measCb = &measEvtCb->measCb;
   while((node = cmLListFirst(&measCb->rbList)))
   {
      node = cmLListDelFrm(&measCb->rbList, node);
      pjRbCb = (PjRbCb *) node->node;
      pjRbCb->rbL2Cb.measOn &= ~measCb->measType;

      /* Cleanup any pending buffers which were not acked */
      if(measCb->measType & LPJ_L2MEAS_DL_DELAY)
      {
         for(idx = pjRbCb->rbL2Cb.startCount; idx < pjRbCb->dlCb.count; idx++)
         {
            if((txEnt = pjDbmGetTxEnt( &(pjRbCb->dlCb.txBuf), idx)) == NULLP)
            {
               continue;
            }
            if(txEnt->state != PJ_PDU_SUBMITTED)
            {
               break;
            }
            pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), idx);
         }
         pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DELAY]   =  NULLP;
      }
      /* kw006.201 ccpu00120058, Removed the code for Active Ue.              *
       * Also changed PJ_L2MEAS_DL_DISC and PJ_L2MEAS_UL_LOSS in if condition *
       * to LPJ_L2MEAS_DL_DISC and LPJ_L2MEAS_UL_LOSS respectively.           */
      if(measCb->measType & LPJ_L2MEAS_DL_DISC )
      {
         pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DISC]   =  NULLP;
      }
      if(measCb->measType & LPJ_L2MEAS_UL_LOSS)
      {
         pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]   =  NULLP;
      }
    }/* End of while loop */
   /* Initialize measCfmEvt */
   cmMemset((U8 *) &measCfmEvt, 0, sizeof(PjL2MeasCfmEvt));

   measCfmEvt.transId = measEvtCb->transId;
   measCfmEvt.numCfm = 0;
   /* Fill the measCfmEvt structure */
   for(qciIdx = 0; qciIdx < measCb->numQci; qciIdx++)
   {
      kwCb.pjL2Cb.measOn[measCb->qci[qciIdx]] &= ~measCb->measType;
      measCfmEvt.measType = measCb->measType;
      measCfmEvt.measCfm[measCfmEvt.numCfm].qci = measCb->qci[qciIdx];
      measCfmEvt.status.status = LCM_PRIM_OK;
      measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
      if((measCb->measType &  LPJ_L2MEAS_DL_DELAY) &&
         (measCb->measData[measCb->qci[qciIdx]].dlDelay.numPkts > 0))
      {
         /* Packed delay is in ms */
         measCfmEvt.measCfm[measCfmEvt.numCfm].dlDelay = 
         (measCb->measData[measCb->qci[qciIdx]].dlDelay.val / 
         measCb->measData[measCb->qci[qciIdx]].dlDelay.numPkts);
      }
      if((measCb->measType & LPJ_L2MEAS_DL_DISC) &&
         (measCb->measData[measCb->qci[qciIdx]].dlDisc.numPkts > 0))
      {
         /* dlDisc = num of Pkts Discarded * 10^6 / total no of pkts. */
         measCfmEvt.measCfm[measCfmEvt.numCfm].dlDisc = 
         measCb->measData[measCb->qci[qciIdx]].dlDisc.val  * (1000000) /
         measCb->measData[measCb->qci[qciIdx]].dlDisc.numPkts;
      }
      if((measCb->measType &  LPJ_L2MEAS_UL_LOSS) &&
         (measCb->measData[measCb->qci[qciIdx]].ulLoss.numPkts > 0))
      {
         /* ul Loss  = num of Pkts lost * 10^6 / total no of pkts. */
         measCfmEvt.measCfm[measCfmEvt.numCfm].ulLoss  =
         measCb->measData[measCb->qci[qciIdx]].ulLoss.val  * (1000000) /
         measCb->measData[measCb->qci[qciIdx]].ulLoss.numPkts;
      }
      measCfmEvt.numCfm++;
   }

   kwCb.pjL2Cb.pjMeasEvtCb[measEvtCb->cbIdx] = NULLP;
   PJ_FREE(measEvtCb, sizeof(PjL2MeasEvtCb));
   kwCb.pjL2Cb.pjNumMeas--;

   /* Send Measurement confirmation to layer manager */
   PjMiLpjL2MeasCfm(&kwCb.pjGenCfg.lmPst,&measCfmEvt);

   RETVALUE(ROK);
} /* pjUtlHdlL2TmrExp */

/**
 *
 * @brief Handler for Sending Negative confirm .
 *
 *
 * @b Description
 *        This function is called when the l2 measurement cannot be started
 *        This function sends  negative confirm for all the requests
 *
 *  @param[in] measReqEvt    Measurement Req Structure
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PUBLIC S16 pjUtlSndL2MeasNCfm
(
PjL2MeasReqEvt  *measReqEvt,
PjL2MeasCfmEvt  *measCfmEvt
)
#else
PUBLIC S16 pjUtlSndL2MeasNCfm(measReqEvt, measCfmEvt)
PjL2MeasReqEvt  *measReqEvt;
PjL2MeasCfmEvt  *measCfmEvt;
#endif
{

   TRC3(pjUtlSndL2MeasNCfm)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlSndL2MeasNCfm(transId(%ld))\n", measReqEvt->transId));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlSndL2MeasNCfm(transId(%d))\n", measReqEvt->transId));
#endif
   PjMiLpjL2MeasCfm(&kwCb.pjGenCfg.lmPst, measCfmEvt);

   RETVALUE(ROK);
} /* pjUtlSndL2MeasNCfm */

/**
 * @brief  Validates the measurement request parameters. 
 *
 * @details
 *
 *     Function :pjUtlValidateL2Meas 
 *
 *  @param[in]  PjL2MeasReqEvt measReqEvt
 *  @param[out] PjL2MeasCfmEvt measCfmEvt
 **/
#ifdef ANSI
PUBLIC S16 pjUtlValidateL2Meas
(
PjL2MeasReqEvt  *measReqEvt,
PjL2MeasCfmEvt  *measCfmEvt
)
#else
PUBLIC S16 pjUtlValidateL2Meas(measReqEvt, measCfmEvt)
PjL2MeasReqEvt  *measReqEvt;
PjL2MeasCfmEvt  *measCfmEvt;
#endif
{
   U8      measType;
   S16     ret;
   U8      qciIdx;
   KwRbCb  *rbCb;

   TRC3(pjUtlValidateL2Meas)
   
   rbCb = NULLP;
   ret = ROK;
   measType = measReqEvt->measReq.measType;

   if((measType == 0) ||
       measType > 7)
   {
      measCfmEvt->numCfm = 1;
      measCfmEvt->measCfm[0].qci = measReqEvt->measReq.qci[0];
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_INVALID_MEASTYPE;
      RETVALUE(RFAILED);
   }
   if(kwCb.pjL2Cb.pjNumMeas >= PJ_MAX_L2MEAS_EVT)
   {
      measCfmEvt->numCfm = 1;
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_EXCEED_MEASREQ;
      RETVALUE(RFAILED);
   }
   if(measReqEvt->measReq.numQci > LPJ_MAX_QCI)
   {
      measCfmEvt->numCfm = 1;
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_EXCEED_NUMQCI;
      RETVALUE(RFAILED);
   }
   if(measReqEvt->measPeriod == 0)
   {
      measCfmEvt->numCfm = 1;
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_ZERO_MEASPERIOD;
      RETVALUE(RFAILED);
   }
   for(qciIdx = 0; qciIdx < measReqEvt->measReq.numQci; qciIdx++)
   {
      if(kwCb.pjL2Cb.measOn[measReqEvt->measReq.qci[qciIdx]] & 
            measReqEvt->measReq.measType)
      {
        /* Measurement is already ongoing */
        measCfmEvt->measCfm[measCfmEvt->numCfm].qci = qciIdx;
        measCfmEvt->measType = measType;
        measCfmEvt->numCfm++;
      }
   }
   if(measCfmEvt->numCfm > 0)
   {
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_MEAS_ALREADY_ENA;
      measCfmEvt->transId = measReqEvt->transId;
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* pjUtlValidateL2Meas */
#endif /* LTE_L2_MEAS */

#ifdef PJ_SEC_ASYNC 
/**
 * @brief
 *
 * @details
 *
 *     Function :pjUtlPerfErrUpd
 *
 *  @param[in]  PjRbCb *pjRbCb
 *  @param[out] U32    count
 **/
#ifdef ANSI
PUBLIC Void pjUtlPerfErrUpd
(
PjRbCb    *pjRbCb,
U32       count
)
#else
PUBLIC Void pjUtlPerfErrUpd(pjRbCb, count)
PjRbCb    *pjRbCb,
U32       count
#endif
{
   PjUlCb   *upCb;

   TRC2(pjUtlPerfErrUpd);

   upCb =  &pjRbCb->ulCb;
   if(upCb->recBuf.numEntries != 0)
   {
      pjUtlCalNxt2Sub(upCb);
   }
   pjUlmChkRdyToDeliver(pjRbCb);

   RETVOID;
}/* end of pjUtlPerfErrUpd */
#endif /*PJ_SEC_ASYNC */
/**
 * @brief
 *
 * @details
 *
 *     Function :pjUtlCalNxt2Sub
 *
 *  @param[in]  PjUlCb *upCb
 **/
#ifdef ANSI
PUBLIC Void pjUtlCalNxt2Sub
(
PjUlCb   *ulCb
)
#else
PUBLIC Void pjUtlCalNxt2Sub(ulCb)
PjUlCb   *ulCb;
#endif
{
   PjRxEnt  *rxEnt;
   U32  nxtToSub;
   
   TRC2(pjUtlCalNxt2Sub);
   nxtToSub = ulCb->nxtSubCnt;
   if(ulCb->recBuf.numEntries != 0)
   {
      nxtToSub ++;
      while( (rxEnt = (PjRxEnt *)pjDbmGetRxEnt(&ulCb->recBuf, nxtToSub)) 
            != NULLP)
      {
         ulCb->nxtSubCnt++;
      }
   }
   ulCb->nxtSubCnt  =  nxtToSub;

   RETVOID;
}/* end of pjUtlCalNxt2Sub */

/**
 * @brief
 *
 * @details
 *
 *     Function :pjUtlFillReEstCfgCfmInfo
 *
 *  @param[in]  PjUlCb *upCb
 **/
#ifdef ANSI
PUBLIC Void pjUtlFillReEstCfgCfmInfo
(
CpjCfgCfmInfo *cfmInfo,
PjAsyncCfm    *asyncCfm
)
#else
PUBLIC Void pjUtlFillReEstCfgCfmInfo(cfmInfo, asyncCfm)
#endif
{
   U8 idx;
   TRC2(pjUtlFillReEstCfgCfmInfo);
   for (idx = 0; idx < asyncCfm->numEnt; idx++)
   {
      if (asyncCfm->cfmEnt[idx].cfgType == CPJ_CFG_REESTABLISH)
      {
         cfmInfo->cfmEnt[idx].status = CPJ_CFG_CFM_OK;
         cfmInfo->cfmEnt[idx].reason = LCM_REASON_NOT_APPL;
      }
      else
      {
         cfmInfo->cfmEnt[idx].status = asyncCfm->cfmEnt[idx].status;
         cfmInfo->cfmEnt[idx].reason = asyncCfm->cfmEnt[idx].reason;
      }
      cfmInfo->cfmEnt[idx].rbId  = asyncCfm->cfmEnt[idx].rbId;
      cfmInfo->cfmEnt[idx].rbType= asyncCfm->cfmEnt[idx].rbType; /* ReEst Fix */
   }
   cfmInfo->ueId    = asyncCfm->ueId;
   cfmInfo->cellId  = asyncCfm->cellId;
   cfmInfo->transId = asyncCfm->transId;
   cfmInfo->numEnt  = asyncCfm->numEnt;
}/* end of pjUtlFillReEstCfgCfmInfo */


/**
 * @brief pjUtlSecUpdAndSndReestCfm
 *
 * @details
 *         Update Sec related info and Send ReestCfm to RRC
 *
 *     Function :pjUtlSecUpdAndSndReestCfm
 *
 *  @param[in]  PjUlCb *upCb
 **/
#ifdef ANSI
PUBLIC S16 pjUtlSecUpdAndSndReestCfm
(
   KwUeCb  *ueCb,
   PjRbCb  *pjRbCb
)
#else
PUBLIC S16 pjUtlSecUpdAndSndReestCfm (*ueCb, *pjRbCb)
KwUeCb *ueCb;
PjRbCb *pjRbCb;
#endif
{
   /* kw006.201, ccpu00120574, fixed the warning */
   U8       txIdx;
   U8       rbId;
   RbCb     *tRbCb;
   PTR      tmpIntCxt;
   PTR      tmpCpCxt;
   PTR      tmpUpCxt;
   Bool     onlySrb1;
   CpjReEstCfmInfo  *reEstCfm;    /* Reest config confirm */

   TRC2(pjUtlReEstReconfig);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), 
          (_kwPBuf, "pjUtlReEstReconfig(ueCb(%d,%d))\n",\
                ueCb->key.ueId, ueCb->key.cellId));

   tRbCb = NULLP;
   /* kw006.201, ccpu00120574, fixed the warning */

   /* Update the secCtxt */
   for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
   {
      if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
            (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_REEST_ASYNC_CFM))
      {
         ueCb->secInfo.secCxt.cfgTxId =  txIdx;
         break;
      }
   }
   if(txIdx == PJ_MAX_ASYNC_CFM)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW143, (ErrVal)0, \
            "pjUtlReEstReconfig: Cant find  reEst Cfg Info \n");
      RETVALUE(RFAILED);
   }

   if(ueCb->secInfo.secAct == TRUE)
   {

      /* Initialisation is done first followed by closure since we
       * do not want the same ctxId to be returned for the new init.
       * In ASYNC case, we drop packets if they are returned from a
       * different ctxId. */

      tmpIntCxt = ueCb->secInfo.intCxtId;  
      tmpCpCxt  = ueCb->secInfo.cpCxtId;    
      tmpUpCxt  = ueCb->secInfo.upCxtId;    
      /* Initialise with securtiy algos */
      pjUtlCpInit(ueCb);
      pjUtlUpInit(ueCb);
      pjUtlIntInit(ueCb);
      /* Close the previous security algos */
      pjUtlIntClose(tmpIntCxt);
      pjUtlCipherClose(tmpCpCxt);
      pjUtlCipherClose(tmpUpCxt);

#ifdef PJ_SEC_ASYNC
      RETVALUE(ROK);
#endif /* PJ_SEC_ASYNC*/
   }

   /* Change the UE state to normal only of SRB1 present */
   onlySrb1 = FALSE;
   KW_DBM_FETCH_RBCB(PJ_SRB2_ID, CM_LTE_SRB, ueCb, tRbCb);
   if(tRbCb != NULLP && (tRbCb->rbCfgStat &  PJ_CFG_PDCP) == PJ_CFG_PDCP)
   {
      onlySrb1  =  FALSE;
   }
   if ( onlySrb1  ==  FALSE)
   {
      for(rbId =  0; rbId < KW_MAX_DRB_PER_UE; rbId ++)
      {
         KW_DBM_FETCH_RBCB(rbId, CM_LTE_DRB, ueCb, tRbCb);

         if(tRbCb != NULLP &&
               (tRbCb->rbCfgStat &  PJ_CFG_PDCP) == PJ_CFG_PDCP)
         {
            onlySrb1  =  FALSE;
            break;
         }
      }
   }
   if (onlySrb1 == TRUE)
   {
      ueCb->libInfo.state = PJ_STATE_NORMAL;
   }
   pjRbCb->state = PJ_STATE_NORMAL;


   /* Send ReEstCfm if we dont have to wait for init cfms*/
   PJ_ALLOC(reEstCfm, sizeof(CpjReEstCfmInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(reEstCfm == NULLP)
   {
      PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_ERROR ),
            (_kwPBuf, "pjUtlReEstReconfig: Alloc Failed \n"));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   PJ_FILL_REEST_CFM_INFO(reEstCfm, (ueCb->libInfo.asyncCfm[txIdx]),\
         1); /*LCM_PRIM_OK);*/

   /* Some house keeping work */
   PJ_CLEAN_AND_UPD_ASYNCINFO(ueCb, txIdx);
   PjUiCpjReEstCfm(&kwCb.cpjSap.pst, kwCb.cpjSap.suId, reEstCfm);

   RETVALUE(ROK);

}/* pjUtlSecUpdAndSndReestCfm */
      
#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     gp_pj_utl.c@@/main/1 - Fri Nov 13 14:13:52 2009
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
/main/1      ---      nm   1. LTERLC Release 2.1
/main/2      kw004.201 av   1. Adding missing trace
/main/3      kw005.201 ap   1. Corrected the RB ID calculation.
                            2. Added support for L2 Measurement.
                            3. Added function to send the data forward 
                               indication instead of macro.
/main/4      kw006.201 rd   1. Removed Active Ue code and corrected names
                               of 2 macros in pjUtlHdlL2TmrExp.
                            2. Replace macro with function.
                            3. ccpu00120502 corrected the RB ID calculation.
                            4. ccpu00120574, fixed the warning.
*********************************************************************91*/

