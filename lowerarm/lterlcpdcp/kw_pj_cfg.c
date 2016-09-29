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

        Name:    LTE PDCP - Configuration Manager file

        Type:    C source file

        Desc:    It contains the following configuraiton primtives
                    -- pjCfgAddRb 
                    -- pjCfgReCfgRb 
                    -- pjCfgDelRb 
                    -- pjCfgReEstRb 
                    -- pjCfgDelUe 
                    -- pjCfgPrcSecCfg 
                    -- pjCfgUeIdChng 
                    -- pjCfgPrcCount 
                    -- pjCfgPrcSduSta 

        File:     gp_pj_cfg.c

        Sid:      gp_pj_cfg.c@@/main/1 - Fri Nov 13 14:13:43 2009

        Prg:      rbabu

*********************************************************************21*/
/** @file gp_pj_cfg.c
@brief LTE PDCP Configuration Module
**/


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

/* Proto types declarations for local functions */
 
PRIVATE S16 pjCfgFillPjRbCb ARGS ((
RbCb            *rbCb,
KwUeCb          *ueCb,
CpjCfgEnt       *cfgEnt,
CpjCfmEnt       *cfmEnt
));

PRIVATE S16 pjCfgUpdCompInfo  ARGS ((
PjRbCb               *rbCb,
KwUeCb               *ueCb,
CpjHdrCompCfg        *rohcCfg
));


/* Store the Profile ID into array index */
/* RRC is not sending the profile id directly. It will give
   the for all the profiles with True/False. PDCP will derive the profile
   based on this profile Id list */

U16  rohcProfId[CPJ_MAX_ROHC_PROF_LIST]=    { 0x0001, 0x0002, 0x0003, 0x0004,
                                              0x0006, 0x0101, 0x0102, 0x0103, 
                                              0x0104};


/** @addtogroup cfgdata */
/*@{*/
/** 
* @brief
* 
*   Hanlder to process the configuration recieved from the upper layer and update the pdcp
*   data base.
*
* @b Description: 
*     1. This function create a new  PDCP RB Cb. @n
*     2. Newly created PDCP RB Cb will be placed in Data base with the  configuration
*     informtion received.  @n
*     3. Return Success or Failure to the caller with reason and status
*     information. @n
 
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation.
 * 
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 pjCfgAddRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgAddRb (ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16               ret;
   KwUeCb            *ueCb;  /* UE Cb pointer */
   RbCb              *rbCb;  /* Rb Cb Pointer */

   TRC3(pjCfgAddRb);  

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;
   rbCb = NULLP;

   MSPD_DBG_RLC("Entering pjCfgAddRb\n");

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgAddRb- Received with -- cellId(%d), \
            ueId(%d)\n", cellId, ueId));

   /* Find Ue Cb exist or not*/
   ret = kwDbmFetchAndCreateUeCb(ueId, cellId, &ueCb);
   if( ROK != ret)
      {
         RETVALUE(ret);
   }

   /* Rb Id is allowed range or not */   
   KW_VALIDATE_RBID(ueId, entCfg->rbId, entCfg->rbType, ret)
   if ( ROK != ret)
   { 
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
         CPJ_CFG_REAS_INVALID_RBID);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
          "pjCfgAddRb - Invalid RB ID-- cellId(%d), \
          ueId(%d)\n", cellId, ueId));
      RETVALUE(ret);
   }


   /* Find Rb Cb already exist or not */   
   ret = kwDbmFetchAndCreateRbCb(entCfg->rbId, entCfg->rbType, ueCb, &rbCb,
         PJ_CFG_PDCP);
   if( ROK != ret)
   {
      RETVALUE(ret);
   }
   /* Copy the Recived information into RbCb */ 
   ret = pjCfgFillPjRbCb(rbCb, ueCb, entCfg , entCfm);

   /* Fill the Cfm status and Reason */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);


   RETVALUE(ret);
} /* pjCfgAddRb */

/**
* @brief 
*
*   Hanlder to update the PJRB with the received infromation during
*   configuration or re configuration. 
*
* @b Description: 
*     1. update the data base with the  new configuration informtion received.  @n
*     2. update the data base with the  new re-configuration informtion received.  @n
*     3. If required Initiliaze the tx/rx Buffers and Timers. @n
*     4. Return Success or Failure to the caller with reason and status
*     information. @n
*
 * @param [out]   rbCb     -  RB Control Block
 * @param [out]   entCfm   -  PDCP Entiry Confirmation 
 * @param [in]    ueCb     -  UE Control Block
 * @param [in]    entCfg   -  PDCP Entity Configuration
 *
 * @return  ROK
 */
#ifdef ANSI
PRIVATE S16 pjCfgFillPjRbCb
(
RbCb            *rbCb,
KwUeCb          *ueCb,
CpjCfgEnt       *cfgEnt,
CpjCfmEnt       *cfmEnt
)
#else
PRIVATE S16 pjCfgFillPjRbCb(rbCb, ueCb, cfgEnt, cfmEnt)
RbCb            *rbCb;
KwUeCb          *ueCb;
CpjCfgEnt       *cfgEnt;
CpjCfmEnt       *cfmEnt;
#endif
{
   S16            ret;
   S16            tmrVal;    /* Timer Value */
   PjRbCb         *pjRbCb;   /* PJRB Cb */
   Buffer         *bMapBuf;  /* Bitmap status report buffer      */
   MsgLen         bitMapLen; /* Bit Map length */
   /* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
   U8             lstIdx;
#endif
   TRC3(pjCfgFillPjRbCb);

   /* Initialization */
   ret     = ROK;
   pjRbCb  = NULLP;
   bMapBuf = NULLP;
   tmrVal  = 0; 
   bitMapLen = 0;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "pjCfgFillPjRbCb: Received ueId(%d), cellId(%d), rbId(%d), cfgType(%d)\n",
            rbCb->kwRbCb.rlcId.ueId, rbCb->kwRbCb.rlcId.cellId, cfgEnt->rbId, cfgEnt->cfgType));

   pjRbCb = &rbCb->pjRbCb;
   switch (cfgEnt->cfgType)
   {
      /* Cfg Type received as ADD RB */
      case CPJ_CFG_ADD:
         {
            pjRbCb->state  =   PJ_STATE_NORMAL;
            pjRbCb->rbId   =   cfgEnt->rbId;
            pjRbCb->rbType =   cfgEnt->rbType;
            pjRbCb->mode   =   cfgEnt->m.addCfg.rlcInfo.mode;
            pjRbCb->dir    =   cfgEnt->m.addCfg.rlcInfo.dir;
            pjRbCb->ueCb   =   ueCb;

            if(PJ_SRB == pjRbCb->rbType)
            {
               /* SN length is fixed for SRB */
               pjRbCb->snLen  = PJ_SRB_SN_LEN;
            }
            else
            {
               if (CM_LTE_MODE_UM == pjRbCb->mode)
                  /* SN length can be 7 bit or 12 bit in UM mode */
                  pjRbCb->snLen  =   cfgEnt->m.addCfg.rlcInfo.m.rlcUm.pdcpSNSize;
               else
               {
                  /* SN length is fixed for DRB in AM mode */
                  pjRbCb->snLen  =   PJ_DRBAM_SN_LEN;
               }

            }
            /* Initialize all off board  timers */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
            cmInitTimers(&pjRbCb->dlCb.obdTmr,PJ_RB_MAX_TIMERS); 
            cmInitTimers(&pjRbCb->ulCb.obdTmr,PJ_RB_MAX_TIMERS); 
#endif                  

            PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
                     "pjCfgAddRb- Received with -- Dir(%d),\
                     mode(%d)\n", pjRbCb->dir, pjRbCb->mode));

            if (KW_DIR_UL &  pjRbCb->dir)
            {
               pjRbCb->ulCb.discReqd   = cfgEnt->m.addCfg.discReqd;
               pjRbCb->ulCb.staRepReqd = cfgEnt->m.addCfg.rlcInfo.m.rlcAm.statusRepReqd;

               /*Initialization for Rx Buf */
               pjDbmBufInit(&pjRbCb->ulCb.recBuf,kwCb.pjGenCfg.nmbUlBins);
               pjRbCb->ulCb.lastSubCnt = 0xffffffff;
            }

            if (KW_DIR_DL &  pjRbCb->dir)
            {
               pjRbCb->dlCb.discTmrInt = cfgEnt->m.addCfg.discardTmr;
               pjRbCb->dlCb.cfmReqd    = cfgEnt->m.addCfg.cfmReqd;
               /* pjRbCb->dlCb.count      = 0xffffffff; */

               /*ccpu00121544: Initializing disc timer without any condition*/
               cmInitTimers(&pjRbCb->dlCb.discTmr,PJ_RB_MAX_TIMERS);
               /* MS_FIX for DISC TMR */
#ifndef MSPD
               /* Discard Timer is Enabled */
               if(pjRbCb->dlCb.discTmrInt > 0 )
               {
                  /* 1. The discTmr value is absolute value in ms units 
                     2. Store the DiscardTmr value in terms of Layer resolution
                     ie discTmrVal = dicTrmVal/(SS_TICKS*Layer Resolution) 
                     and round to next integer value.
                   */
                  tmrVal = pjRbCb->dlCb.discTmrInt % (SS_TICKS_SEC
                        * kwCb.genCfg.timeRes); 
                  pjRbCb->dlCb.discTmrInt = (S16)(pjRbCb->dlCb.discTmrInt / (SS_TICKS_SEC * kwCb.genCfg.timeRes));
                  if(tmrVal)
                     pjRbCb->dlCb.discTmrInt +=1; /* Round to next integer value if
                                                     required */          
               }
#endif
               /*Initialization for Tx Buf */
               pjDbmBufInit(&pjRbCb->dlCb.txBuf,kwCb.pjGenCfg.nmbDlBins);
            }

            /* Update the Compressor Informatio in Rb Cb */
            if(cfgEnt->rbType == CM_LTE_DRB)
            {
               ret=pjCfgUpdCompInfo(pjRbCb, ueCb,&cfgEnt->m.addCfg.hdrCompCfg);
/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
               pjRbCb->qci =  cfgEnt->m.addCfg.qci;
               for(lstIdx = 0; lstIdx <= PJ_MAX_L2MEAS_INRB; lstIdx++)
               {
                  pjRbCb->rbL2Cb.listNode[lstIdx].next = NULLP;
                  pjRbCb->rbL2Cb.listNode[lstIdx].prev = NULLP;
                  pjRbCb->rbL2Cb.listNode[lstIdx].node = (PTR) pjRbCb;
               }
#endif
            }

            break;
         }

         /* Cfg Type received as Modify RB */
      case CPJ_CFG_MODIFY:
         {
            /* Store the received Information */
            if(CPJ_RECFG_DISRQD & cfgEnt->m.modCfg.bitFlag)
               pjRbCb->ulCb.discReqd   = cfgEnt->m.modCfg.discReqd;

            if(CPJ_RECFG_CFMREQ & cfgEnt->m.modCfg.bitFlag)
               pjRbCb->dlCb.cfmReqd    = cfgEnt->m.modCfg.cfmReqd;

            /* Header Compression profile has been modified */

            if(TRUE == cfgEnt->m.modCfg.hdrCompCfg.hdrCompUsed)
            {
               /* Close All the channels */
               pjUtlCmpReset(pjRbCb); 
               pjCfgUpdCompInfo(pjRbCb, ueCb, &cfgEnt->m.modCfg.hdrCompCfg);
            }

            /* In case of Hand off scenario: At Target eNode-B, 
               Handoff information will send in re-cfg req */

            if(TRUE  & cfgEnt->m.modCfg.hoInfo.isHoPres)
            {
               /* Store the HO information  and call for Status Report
                * Generation also */
               if(CPJ_HO_DL & cfgEnt->m.modCfg.hoInfo.isHoPres)
               {
                  pjRbCb->dlCb.count = cfgEnt->m.modCfg.hoInfo.dlCount - 1;
                  PJ_GET_SN_HFN(cfgEnt->m.modCfg.hoInfo.dlCount, pjRbCb->snLen, 
                            pjRbCb->dlCb.nxtTxSn, pjRbCb->dlCb.txHfn);
                 pjRbCb->dlCb.nxtToSub = pjRbCb->dlCb.count + 1;
                 pjRbCb->dlCb.cfmExp   = pjRbCb->dlCb.count + 1;
#ifdef MSPD
                 MSPD_DBG("HODBG :: pdcp dl sn cfg :: nxtSn = %d hfn = %d nxtToSub = %d\n\n",\
                  pjRbCb->dlCb.nxtTxSn, pjRbCb->dlCb.txHfn, pjRbCb->dlCb.nxtToSub);
#endif
               }
                /* Down Up Lin Information present */
               if(CPJ_HO_UL & cfgEnt->m.modCfg.hoInfo.isHoPres)
               {
                  pjRbCb->ulCb.lastSubCnt = cfgEnt->m.modCfg.hoInfo.ulCount - 1;
                  PJ_GET_SN_HFN(cfgEnt->m.modCfg.hoInfo.ulCount , PJ_DRBAM_SN_LEN, \
                        (pjRbCb->ulCb.nxtRxSn), (pjRbCb->ulCb.rxHfn));
#ifdef MSPD
                 MSPD_DBG("HODBG :: pdcp ul sn cfg :: nxtRxSn = %d hfn = %d\n\n", \
                   pjRbCb->ulCb.nxtRxSn, pjRbCb->ulCb.rxHfn);
#endif

                  if (cfgEnt->m.modCfg.hoInfo.numBits)
                  {
                     PJ_ALLOC_BUF(bMapBuf);
                     bitMapLen = cfgEnt->m.modCfg.hoInfo.numBits % PJ_BYTE_SIZE;

                     if(bitMapLen)
                     { 
                        bitMapLen = cfgEnt->m.modCfg.hoInfo.numBits / PJ_BYTE_SIZE;
                        bitMapLen +=1;  /* Round to next Byte */
                     }
                     else
                     {
                        bitMapLen = cfgEnt->m.modCfg.hoInfo.numBits/PJ_BYTE_SIZE;
                     } 

                     SAddPstMsgMult(cfgEnt->m.modCfg.hoInfo.ulBitMap, bitMapLen, bMapBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
                     if (rbCb->rbCfgStat != PJ_CFG_BOTH)
                     {
                        PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf, \
                                 "pjCfgFillPjRbCb - rbCfgStat Invalid - rbId(%d) \
                                 \n", rbCb->pjRbCb.rbId));

                        PJLOGERROR(ERRCLS_ADD_RES, EKW049, (ErrVal)0,       \
                              "pjCfgFillPjRbCb: rbCfgStat Invalid\n");
                        RETVALUE(RFAILED);
                     }
                     else
                     {
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
                        /* Build and send the Status Report to RLC */
                        pjUlmBldStaRep(pjRbCb, cfgEnt->m.modCfg.hoInfo.ulCount, bMapBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
                     }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
                  }

                  /* DeAllocate the memory  and reset the num of bits */
                  PJ_FREE(cfgEnt->m.modCfg.hoInfo.ulBitMap,bitMapLen) 
                     cfgEnt->m.modCfg.hoInfo.numBits = 0;

               } 

            }

            break;
         }

         /* Un Expected cfg type */
      default:
         {
            PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
                     "pjCfgFillRbCb Failed -- for the invalid cfgType(%d)\n",
                     cfgEnt->cfgType));
         }
   }
   RETVALUE(ret);
} /* pjCfgFillPjRbCb */



/**
* @brief 
*
*   Hanlder to process the re-configuration for existing PJRB from the upper
*   layer.
*
* @b Description: 
*     1. This function fetch the PJRB from pdcp data base. @n
*     2. update the data base with the  new configuration informtion received.  @n
*     3. Return Success or Failure to the caller with reason and status
*     information. @n
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Confirmation.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgReCfgRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgReCfgRb (ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16       ret;
   KwUeCb   *ueCb;    /* Ue Cb */
   RbCb     *rbCb;    /* Rb Cb */

   TRC3(pjCfgReCfgRb);

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;
   rbCb = NULLP;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), 
         (_kwPBuf, "pjCfgReCfgRb - Received for -- cellId(%d), \
          ueId(%d)\n", cellId, ueId));

   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(ueId, cellId, &ueCb);

   /* Ue Cb is not present  */
   if( ROK != ret || NULLP == ueCb)
   { 
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbType, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_UE_UNKWN);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgReCfgRb - Ue is not present -- cellId(%d), \
               ueId(%d)\n", cellId, ueId));
      RETVALUE(ret);
   }

   /* Fetch Rb Cb */
   KW_DBM_FETCH_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if(NULLP == rbCb  || !(PJ_CFG_PDCP & rbCb->rbCfgStat))
   { 
      /* Fill entCfm structure */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_PDCPID_UNKWN);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgReCfgRb - RB Cb is not present -- cellId(%d), \
               ueId(%d), rbId(%d)\n", cellId, ueId, entCfg->rbId));
      RETVALUE(RFAILED);
   }

   /* Copy the received infromation in to Rb Cb */
   ret=pjCfgFillPjRbCb(rbCb, ueCb, entCfg , entCfm);

   /* Fill the Cfm status and Reason */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);


   RETVALUE(ret);
} /* pjCfgReCfgRb */


/**
* @brief
*  This function deletes the existing RB(PDCP and RLC) from  Ue Cb.
*
* @b Description:
*    1. Stop the timers and deallocate tx/rx Buffs for PJRB CB. @n
*    2. Delete the existing RB Cb(PDCP and RLC)  from Ue Cb. @n
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Confirmation.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgDelRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgDelRb (ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16      ret;
   KwUeCb   *ueCb;    /* Ue Cb */
   RbCb     *rbCb;    /* Rb Cb */

   TRC3(pjCfgDelRb);

   /* Initialization */
   ret = ROK;
   ueCb = NULLP;
   rbCb = NULLP;
#if 0
   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgDelRb- Received for -- cellId(%d), \
            ueId(%d)\n", cellId, ueId));
   MSPD_DBG("[%d][%d][%d] Received RB delete request in core 1\n", 
             ueId, entCfg->rbType, entCfg->rbId);

   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(ueId, cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      /* Ue Cb is not present, Send UE Unknown */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_UE_UNKWN);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgDelRb- UE Cb is not found -- cellId(%d), \
               ueId(%d)\n", cellId, ueId));
      RETVALUE(ret);
   }

   /* Fetch RB Cb */
   
   KW_DBM_FETCH_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

   /* RB Cb is not present   or not configured for PDCP */
   if(NULLP == rbCb || !(PJ_CFG_PDCP &rbCb->rbCfgStat))
   { 
      /* Fill entCfm structure with invalid pdcp Id */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_PDCPID);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgDelRb - RB Cb is not found -- cellId(%d), \
               ueId(%d), rbId(%d)\n", cellId, ueId, entCfg->rbId));
      RETVALUE(RFAILED);
   }
   /* Delete PDCP Related infromation from RB CB */
   ret= pjCfgDelPjRBCb(ueCb, rbCb); 

   if(ROK != ret)
   {
      /* Fill entCfm structure with invalid pdcp Id */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_PDCPID);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgDelRb - RB Cb is not found -- cellId(%d), \
               ueId(%d), rbId(%d)\n", cellId, ueId, entCfg->rbId));
      RETVALUE(ret);
   }
#endif
   /* Fill the Cfm status and Reason */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);


   RETVALUE(ret);
} /* pjCfgDelRb */




/**
*  @brief
*
*   This primitive is used to re-establish SRB2 and/or DRB'S during RRC connection
*   re-configuration procedure.
*  
*  @b Description:
*  1. This function will trigger re-establishment for SRB2 and/or DRB's
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Configuration cfm.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgReEstRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgReEstRb (ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16    ret;
   KwUeCb *ueCb;   /* Ue Cb */
   RbCb   *rbCb;   /* Rb Cb */

   TRC3(pjCfgReEstRb); 

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;
   rbCb = NULLP; 

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgReEstRb- Received for -- cellId(%d), \
            ueId(%d)\n", cellId, ueId));

   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(ueId, cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      /* Fill entCfm structure with Ue not present */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_UE_UNKWN);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgReEstRb- UE Cb is not present -- cellId(%d), \
               ueId(%d)\n", cellId, ueId));
      RETVALUE(ret);
   }

   /* Fetch Rb Cb */
   KW_DBM_FETCH_RBCB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if(NULLP == rbCb || !(PJ_CFG_PDCP & rbCb->rbCfgStat ))
   { 
      /* Fill entCfm structure with invalid PDCP Id*/
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_PDCPID_UNKWN);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgReEstRb-- RB CB is not present -- cellId(%d), \
               ueId(%d)\n", cellId, ueId));
      RETVALUE(RFAILED);
   }
   /* A Cfm with failure is sent if the re-establishment comes when RBs are not in
      re-est state */
   if (ueCb->libInfo.state != PJ_STATE_REEST )
   {
      /* Fill entCfm structure with invalid PDCP Id*/
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_INVALID_STATE);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgReEstRb-- RB CB is not present -- cellId(%d), \
               ueId(%d)\n", cellId, ueId));
      RETVALUE(RFAILED);
   }
   /* Caller will call Util function directly to trigger ReEst */

   RETVALUE(ret);
} /* pjCfgReEstRb */



/**
* @brief
* This primitive Delete Ue Cb from PDCP/RLC data base.
*
* @b Description:
*  1. This primitive Delete the Ue Cb from PDCP/RLC Data base.
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [in]    entCfg   -  Entity Configuration to be done.
* @param [out]   entCfm   -  Entity Confirmation.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgDelUe
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
)
#else
PUBLIC S16 pjCfgDelUe (ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CpjCfgEnt         *entCfg;
CpjCfmEnt         *entCfm;
#endif
{
   S16     ret;
   KwUeCb  *ueCb;  /* Ue Cb */
#ifdef MSPD_MLOG_NEW
   VOLATILE U32 t=0, t2=0;
#endif

#ifdef MSPD_MLOG_NEW
   U32            startTick, endTick;
#endif
   TRC3(pjCfgDelUe);
   /* Initialization */
   ueCb   = NULLP;
   ret    = ROK;
#if 0
   /* kw006.201, ccpu00120574, fixed the warning */

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgDelUe- Received for -- cellId(%d), \
            ueId(%d)\n", cellId, ueId));
   MSPD_DBG("[%d] Received UE delete\n", ueId);

   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(ueId, cellId, &ueCb);

   /* Ue Cb is not present  */
   if( ROK != ret || NULLP == ueCb)
   { 
      /* Fill entCfm structure with Ue unknown */
      PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,
            CPJ_CFG_REAS_UE_UNKWN);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
               "pjCfgDelUe- UE CB is not found -- cellId(%d), ueId(%d)\n",
               cellId, ueId));
      RETVALUE(ret);
   }
#ifdef MSPD
   ysMsAmmDelUeReqRcvd++;
#endif
#ifdef MSPD_MLOG_NEW
   startTick = GetTIMETICK();
#endif
#ifdef MSPD_MLOG_NEW   
   t = GetTIMETICK();
#endif

   /* Delete Ue Cb */
   ret=kwDbmDelUeCb(ueCb, FALSE);
#ifdef MSPD_MLOG_NEW
   endTick = GetTIMETICK();
   MLogTask(PID_PDCP_CFG_UE_DELETE, RESOURCE_LARM, startTick, endTick);
#endif

   /* Fill the Cfm */
   PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
         CPJ_CFG_REAS_NONE);

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_RLC_AM_RLC_UE_DEL, RESOURCE_LARM, t, t2=GetTIMETICK());
#endif
#endif  
   RETVALUE(ret);
} /* pjCfgDelUe */


/**
* @brief
* This primitive configure the security information of an Ue Cb.
*
* @b Description:
*  1. This primitive  configure the security information of an Ue Cb.
*  2. Initilize  the  Integrity/Ciphering Channels if required.
*
* @param [in]    secCfg   -  Entity Configuration to be done.
* @param [out]   secCfm   -  Entity Configuration Cfm done.
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgPrcSecCfg
(
CpjSecCfgReqInfo *secCfg,
CpjSecCfgCfmInfo *cfgCfm
)
#else
PUBLIC S16 pjCfgPrcSecCfg (secCfg, cfgCfm)
CpjSecCfgReqInfo  *secCfg;
CpjSecCfgCfmInfo  *cfgCfm;
#endif
{
   S16     ret;
   KwUeCb *ueCb;

   TRC3(pjCfgPrcSecCfg);

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgPrcSecCfg- Received for -- cellId(%d), \
            ueId(%d)\n", secCfg->cellId, secCfg->ueId));

   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(secCfg->ueId, secCfg->cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      /* Fill entCfm structure */
      PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
            CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_UE_UNKWN);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgPrcSecCfg- UE CB is not found -- cellId(%d), \
               ueId(%d)\n", secCfg->cellId, secCfg->ueId));
      RETVALUE(ret);
   }

   ueCb->secInfo.selSecAct= secCfg->secCfg.selSecAct;

   /* Store Integrity Information */
   ueCb->secInfo.intInfo.algoType=secCfg->secCfg.integInfo.algoType;
   cmMemcpy(&ueCb->secInfo.intInfo.intKey[0], &secCfg->secCfg.integInfo.intKey[0], CPJ_INTG_KEY_LEN);

   /* Store Ciphering Informaiton */
   ueCb->secInfo.cipherInfo.algoType=secCfg->secCfg.cipherInfo.algoType; 

   cmMemcpy(ueCb->secInfo.cipherInfo.cpKey,
         &secCfg->secCfg.cipherInfo.ctrlplnKey[0],CPJ_CIPHER_KEY_LEN);

   cmMemcpy(ueCb->secInfo.cipherInfo.upKey,
         &secCfg->secCfg.cipherInfo.usrplnKey[0],CPJ_CIPHER_KEY_LEN);

   /* Security Activation Done */
   ueCb->secInfo.secAct = TRUE;
   ueCb->secInfo.firstDl   =  TRUE;
   ueCb->secInfo.firstUl   =  TRUE;

   ueCb->secInfo.secCxt.cellId = secCfg->cellId;
   ueCb->secInfo.secCxt.ueId   = secCfg->ueId;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgPrcSecCfg- Security Paramters stored in UE CB are -- cellId(%d), \
            ueId(%d) Integrity Algo(%d), cipherAlgoType(%d) \n",\
            secCfg->cellId, secCfg->ueId, ueCb->secInfo.intInfo.algoType,
            ueCb->secInfo.cipherInfo.algoType));

   /* Open the Security Channels */

   if(ueCb->secInfo.secAct)
   {

#ifdef PJ_SEC_ASYNC
      ueCb->secInfo.secCxt.cfgTxId = ueCb->libInfo.nxtAvlbTxId;
#endif 

      ret = pjUtlIntInit(ueCb);

      /* Integrity Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_INTINIT_FAILED);

         RETVALUE(ret);

      }
      ret = pjUtlCpInit(ueCb);
      /* Ciphering Control Plane Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_CCIPHR_FAILED);

         RETVALUE(ret);

      }
      ret = pjUtlUpInit(ueCb);

      /* Ciphering User Plane Init failed */
      if(ROK != ret)
      {
         PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_UCIPHR_FAILED);

         RETVALUE(ret);
      }
   }

   /* Fill Cfg Cfm with Status OK */
   PJ_CFG_FILL_SECCFG_CFM(cfgCfm, secCfg->ueId,secCfg->cellId,secCfg->transId,\
         CPJ_CFG_CFM_OK,CPJ_CFG_REAS_NONE);

   RETVALUE(ROK);
} /* pjCfgPrcSecCfg */


/**
* @brief
* This primitive changes the ueId of existing Ue Cb.
* 
* @b Description:
*
*    1. This function delete the UeCb from hashList. @n
*    2. Update the Ue Cb with the new Keys. @n
*    3. Insert the UeCb in hash list with new Key.  @n
*
* @param [in]    ueInfo      -  Old UE Information
* @param [in]    newUeInfo   -  New UE Information
* @param [out]   status      -  Status
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgUeIdChng
(
CpjUeInfo *ueInfo,
CpjUeInfo *newUeInfo,
CmStatus  *status
)
#else
PUBLIC S16 pjCfgUeIdChng (ueInfo, newUeInfo, status)
CpjUeInfo *ueInfo;
CpjUeInfo *newUeInfo;
CmStatus  *status;
#endif
{
   S16      ret;
   CkwUeInfo ueKwInfo;     /* Ue Identity Details */ 
   CkwUeInfo newUeKwInfo;  /* New Identity Details */ 
   /* kw005.201 ccpu00117202, removed extra fetching of ueCb */

   TRC3(pjCfgUeIdChng); 

   /* Initialization */
   /* kw005.201 ccpu00117202, removed extra fetching of ueCb */
   ret   = ROK;
   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgUeIdChng- Recived for -- cellId(%d), \
            ueId(%d)\n", ueInfo->cellId, ueInfo->ueId));
   /* kw005.201 ccpu00117202, removed extra fetching of ueCb */
   /* Trigger Ue Id Change */
   ueKwInfo.cellId = ueInfo->cellId;
   ueKwInfo.ueId   = ueInfo->ueId;

   newUeKwInfo.cellId = newUeInfo->cellId;
   newUeKwInfo.ueId   = newUeInfo->ueId;


   ret = kwCfgUeIdChng(&ueKwInfo,&newUeKwInfo, status); 

   if(ret == ROK)
   {
      /* Fill entCfm structure */
      status->status =  CPJ_CFG_CFM_OK;
      status->reason =  CPJ_CFG_REAS_NONE;
   }
   RETVALUE(ret);
} /* pjCfgUeIdChng */


/**
* @brief
* This primitive reports the Count informatin of all RBs of an Ue Cb.
*
* @b Description:
* 1. This function reports the count information of all the RBs of an UE. 
*
* @param [in]    ueId     -  UE Identifier
* @param [in]    cellId   -  CELL Identifier
* @param [out]   countCfm -  Count Information
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgPrcCount
(
CpjCountReqInfo *countReq,
CpjCountCfmInfo       *countCfm
)
#else
PUBLIC S16 pjCfgPrcCount (countReq, countCfm)
CpjCountReqInfo *countReq;
CpjCountCfmInfo       *countCfm;
#endif
{
   S16     ret;
   KwUeCb  *ueCb;
   RbCb    *rbCb;
   U8      numRb;
   U8      idx;
   /* kw006.201, fix for ccpu00120654 */
   U32     tmpUlCount;

   TRC3(pjCfgPrcCount); 

   /* Initialization */
   ueCb   = NULLP;
   rbCb   = NULLP;
   ret    = ROK;
   numRb  = 0;
   idx  = 0;
   /* kw006.201, fix for ccpu00120654 */
   tmpUlCount = 0;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgPrcCount - Count Request Received for - cellId(%d), \
            ueId(%d)\n", countReq->cellId, countReq->ueId));

   /* Fetch  ue Cb */
   ret = kwDbmFetchUeCb(countReq->ueId, countReq->cellId, &ueCb);
   countCfm->ueId = countReq->ueId;
   countCfm->cellId = countReq->cellId;
   countCfm->transId = countReq->transId;

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      /* Fill entCfm structure */
      /* kw006.201, fix for ccpu00120654 */
      countCfm->status = RFAILED;
      countCfm->reason = CPJ_CFG_REAS_UE_UNKWN;

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgPrcCount - UE Cb is not found -- cellId(%d), \
               ueId(%d)\n", countReq->cellId, countReq->ueId));
      RETVALUE(ret);
   }

   /* Loop through all DRBs  */
   /* kw006.201, ccpu00120058, added boundary condition check for countInfo */
   for(numRb=0, idx=0; ((idx < KW_MAX_DRB_PER_UE) && (numRb < CPJ_MAX_DRB)); idx++)
   {
      rbCb = ueCb->drbCb[idx];
      /* RB is present and configured for PDCP Layer */
      if(NULLP != rbCb && (PJ_CFG_PDCP  & rbCb->rbCfgStat))
      {
         countCfm->countInfo[numRb].rbId= rbCb->pjRbCb.rbId; 
         countCfm->countInfo[numRb].dir = rbCb->pjRbCb.dir; 
         /* kw006.201, fix for ccpu00120654 */
         tmpUlCount= ( rbCb->pjRbCb.ulCb.rxHfn << rbCb->pjRbCb.snLen) | rbCb->pjRbCb.ulCb.nxtRxSn;
         if( tmpUlCount == 0)
         {
           countCfm->countInfo[numRb].ulCount= 0;
         }
         else
         {
           countCfm->countInfo[numRb].ulCount= tmpUlCount - 1;
         }
         countCfm->countInfo[numRb].dlCount= rbCb->pjRbCb.dlCb.count; 
         numRb++;
      }
   }

   /* Fill CountCfm with status and reason */
   countCfm->numRb  = numRb;
   if(numRb)
   {
      /* kw006.201, fix for ccpu00120654 */
      countCfm->status =ROK;
      countCfm->reason =CPJ_CFG_REAS_NONE;
   }
   else
   {
      /* kw006.201, fix for ccpu00120654 */
      countCfm->status =RFAILED;
      countCfm->reason =CPJ_CFG_REAS_NO_DRBS;

   }

   RETVALUE(ret);
} /* pjCfgPrcCount */

/**
* @brief
* This primitive reports SDU status information of all RBs of an Ue Cb.
*
* @b Description:
*  1. This primitive Trigger the Handoff Request procedure 
*
* @param [in]    ueId      -  UE Identifier
* @param [in]    cellId    -  CELL Identifier
* @param [out]   sduStaCfm -  SDU Status Information
* 
* @return  ROK
*
*/
#ifdef ANSI
PUBLIC S16 pjCfgPrcSduSta
(
CpjSduStaReqInfo     *staReq
)
#else
PUBLIC S16 pjCfgPrcSduSta (staReq)
CpjSduStaReqInfo     *staReq;
#endif
{
   S16      ret;
   KwUeCb  *ueCb;
   CpjSduStaCfmInfo  *cfmInfo;

   TRC3(pjCfgPrcSduSta);

   /* Initialization */
   ret  = ROK;
   ueCb = NULLP;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgPrcSduSta - Received for -- cellId(%d), \
            ueId(%d)\n", staReq->cellId, staReq->ueId));

   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(staReq->ueId, staReq->cellId, &ueCb);

   /* UeCb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      /* Perform processing for filling up cfm structures */
      PJ_ALLOC(cfmInfo, sizeof(CpjSduStaCfmInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
      if(cfmInfo == NULLP)
      {
         PJLOGERROR(ERRCLS_ADD_RES, EKW050, (ErrVal)0,       \
               "pjUtlSndSduStaCfm: Alloc Failed \n");
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      cfmInfo->ueId = staReq->ueId;
      cfmInfo->cellId = staReq->cellId;
      cfmInfo->transId = staReq->transId;
      cfmInfo->status = LCM_PRIM_NOK;
      cfmInfo->reason = CPJ_CFG_REAS_UE_UNKWN;

      /* Caller should return cfm with failure */
      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgPrcSduSta - UE Cb is not found -- cellId(%d), \
               ueId(%d)\n", staReq->cellId, staReq->ueId));

      /* Send Failure SduStaCfm */
      PjUiCpjSduStaCfm(&kwCb.cpjSap.pst, kwCb.cpjSap.suId, cfmInfo);

      RETVALUE(RFAILED);
   }

   /* Call Ho Start handler for this UE */
   pjUtlReEstHOStart(ueCb, staReq->transId);

   RETVALUE(ret);
} /* pjCfgPrcSduSta */

   

/**
 * @brief
 * This primitive to update the ROHC Comp details in PJRB  Cb.
 *
 * @b Description:
 *  1. This primitive to update  the ROHC comp detils in PJRB Cb.
 *
 * @param [in]    pjRbCb    -  RB Control Block  
 * @param [in]    ueCb      -  Ue Control Block 
 * @param [in]    hdrCompCfg-  Comp Cfg Information 
 * 
 * @return  ROK
 *
 */
#ifdef ANSI
PRIVATE S16 pjCfgUpdCompInfo 
(
PjRbCb               *rbCb,
KwUeCb               *ueCb,
CpjHdrCompCfg        *rohcCfg
)
#else
PRIVATE S16 pjCfgUpdCompInfo(rbCb, ueCb, rohcCfg)
PjRbCb               *rbCb;
KwUeCb               *ueCb;
CpjHdrCompCfg        *rohcCfg;
#endif
{
   U16   profId;
   U8    idx;
   S16   ret;

   TRC3(pjCfgUpdCompInfo);

   /* Initialization */   
   ret    = ROK;
   profId = 0;
   idx  = 0; 

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgUpdCompInfo - Received for -- cellId(%d), \
            RbId(%d)\n", rbCb->rbId, ueCb->key.ueId));

   /* Header Compresion information present */
   if(rohcCfg->hdrCompUsed)
   {
      /* Header Compression is enabled */ 
      rbCb->rohc.hdrCmpUsed = TRUE;

      /* User Can Configure multipel profiles. Maximum profile ID should be used */  
      for(; idx<CPJ_MAX_ROHC_PROF_LIST; idx++)
      {
         if((rohcCfg->rohcInfo.profLst[idx] )&&
               (profId < rohcProfId[idx]))
            profId = rohcProfId[idx];
      } 
   }
   else
   {
      /* Header Compression is disabled */
      rbCb->rohc.hdrCmpUsed = FALSE;
      profId = 0x0000;
   }

   /* Store the profile Id and MaxCid */

   rbCb->rohc.profId = profId;
   rbCb->rohc.maxCid = rohcCfg->rohcInfo.maxCid;

   /* Store the ue Details in Compressor Context */
   rbCb->cmpCxt.ueId   = ueCb->key.ueId;
   rbCb->cmpCxt.cellId = ueCb->key.cellId;
   rbCb->cmpCxt.rbId   = rbCb->rbId;
   rbCb->cmpCxt.rbType = rbCb->rbType;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgUpdCompInfo - Selected   -- profId(%d), \
            maxCid(%d)\n", rbCb->rohc.profId, rbCb->rohc.maxCid));


   /* Compress Init */
   if(rbCb->rohc.hdrCmpUsed)
   {
      ret=pjUtlCmpInit(rbCb); 
   }

   RETVALUE(ret);
} /* pjCfgUpdCompInfo */


/**
 * @brief
 * This primitive to delete RB from Ue Cb.
 *
 * @b Description:
 * 1. To delete the PJRB Cb from  UeCb.
 *
 * @param [in]    ueCb      -  Ue Control Block 
 * @param [in]    rbCb      -  Rb Control Block  
 * 
 * @return  ROK
 *
 */
#ifdef ANSI
PUBLIC S16  pjCfgDelPjRBCb 
(
RbCb                 *rbCb
)
#else
PUBLIC S16 pjCfgDelPjRBCb(rbCb)
RbCb                *rbCb;
#endif
{   
   S16  ret;
/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
   U8   evtIdx;
   U8   qciIdx;
   PjRbCb *pjRbCb;
#endif
   TRC3(pjCfgDelPjRBCb);

   /* Initialization */
   ret = ROK;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgDelPjRBCb- Recieved -- rbId() CfgState() \n"));

   /* [ccpu00127171] Fix for ERAB deletion */
   if( (NULLP != rbCb) /*&& (PJ_CFG_PDCP & rbCb->rbCfgStat )*/)
   {
      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
               "pjCfgDelPjRBCb- Received for -- rbId (%d), rbCb CfgState(%d)\n",
               rbCb->pjRbCb.rbId, rbCb->rbCfgStat));

      /* De refer all other pointers */
      rbCb->kwRbCb.pjRbCb = NULLP;
      rbCb->pjRbCb.kwRbCb = NULLP;
/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
      /* Seatch if any measurement is in progress for this qci */
      pjRbCb = &rbCb->pjRbCb;
      if(kwCb.pjL2Cb.measOn[pjRbCb->qci] !=  LPJ_L2MEAS_NONE)
      {
         PjL2MeasCb *measCb;
         for(evtIdx = 0; evtIdx < PJ_MAX_L2MEAS_EVT; evtIdx++)
         {
            if(kwCb.pjL2Cb.pjMeasEvtCb[evtIdx] == NULLP) 
            {
               continue;
            }
            measCb = &kwCb.pjL2Cb.pjMeasEvtCb[evtIdx]->measCb;
            for(qciIdx = 0; qciIdx < measCb->numQci; qciIdx++)
            {
               if(pjRbCb->qci == measCb->qci[qciIdx])
               {
                  break;
               }
            }
            if(qciIdx == measCb->numQci)
            {
               continue;
            }
            /* a mesurement running for that qci is found */
            switch(measCb->measType)
            {
               case LPJ_L2MEAS_DL_DELAY:
               {
                   PJ_RMV_RB_FRM_L2LIST(PJ_L2MEAS_DL_DELAY, measCb, pjRbCb);
                   /* TODO Clean up the TxEnt */
               }
               break;
               case LPJ_L2MEAS_DL_DISC:
               {
                   PJ_RMV_RB_FRM_L2LIST(PJ_L2MEAS_DL_DISC, measCb, pjRbCb);
               }
               break;
               case LPJ_L2MEAS_UL_LOSS:
               {
                   PJ_RMV_RB_FRM_L2LIST(PJ_L2MEAS_UL_LOSS, measCb, pjRbCb);
               }
               break;
               default:
                  RETVALUE(RFAILED);
            }
            pjRbCb->rbL2Cb.measOn &= ~measCb->measType;
            pjLmmSendAlarm(LCM_CATEGORY_INTERFACE, LPJ_EVT_MEAS_HALT,\
            LPJ_CAUSE_RB_DELETION, 0, pjRbCb->ueCb->key.ueId, pjRbCb->qci);
         }
      }
#endif
      rbCb->pjRbCb.ueCb   = NULLP;
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
      /* Stop off board timers */
      if(KW_DIR_DL & rbCb->pjRbCb.dir)
      {
         /*ccpu00121544: Stop the DL OBD timer only if it is running*/
         if(TRUE == (pjChkTmr((PTR)&rbCb->pjRbCb, PJ_EVT_DL_OBD_TMR)))
         {
            pjStopTmr((PTR)&rbCb->pjRbCb, PJ_EVT_DL_OBD_TMR); 
         }
      }

      if(KW_DIR_UL & rbCb->pjRbCb.dir)
      {
         /*ccpu00121544: Stop the UL OBD timer only if it is running*/
         if(TRUE == (pjChkTmr((PTR)&rbCb->pjRbCb, PJ_EVT_UL_OBD_TMR)))
         {
            pjStopTmr((PTR)&rbCb->pjRbCb, PJ_EVT_UL_OBD_TMR);
         }
      }
#endif
      /* Deinit Tx Buffers */
      if(KW_DIR_DL & rbCb->pjRbCb.dir)
      {
         pjDbmTxDeInit(&rbCb->pjRbCb.dlCb.txBuf);
         /* Stop discard timer */
         /*ccpu00121471: Stop the discard timer only if it is running*/
         if(TRUE == (pjChkTmr((PTR)&rbCb->pjRbCb, PJ_EVT_DISC_TMR)))
         {
            pjStopTmr((PTR)&rbCb->pjRbCb, PJ_EVT_DISC_TMR); 
         }
      }

      /* Deinit Rx Buffers */
      if(KW_DIR_UL & rbCb->pjRbCb.dir)
         pjDbmRxDeInit(&rbCb->pjRbCb.ulCb.recBuf);

      /* Close the Comp channels */  
      if((PJ_DRB == rbCb->pjRbCb.rbType) &&
         (rbCb->pjRbCb.rohc.hdrCmpUsed))
      {
            pjUtlCmpClose(rbCb->pjRbCb.cmpCxtId);
      }

#if 0
      /* Assign NULLP to rbCb in rbCbLst */
      if ( rbCb->rbCfgStat == PJ_CFG_PDCP)
      {
         /* No Rlc Cfg present for this RB so delete the RB */
         PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
                  "pjCfgDelPjRBCb- Deleting RB CB  -- , \
                  ueCb Id(%d) rbId(%d)\n", ueCb->key.ueId, rbCb->pjRbCb.rbId));

         /* Delete RB Cb  and set the NULP pointer in List */
         if ( rbCb->pjRbCb.rbType == CM_LTE_SRB )
            ueCb->srbCb[rbCb->pjRbCb.rbId] = NULLP;
         else
            ueCb->drbCb[rbCb->pjRbCb.rbId] = NULLP;
      }
#endif
      /* [ccpu00127171] Fix for ERAB deletion */
      rbCb->rbCfgStat &= ~(PJ_CFG_PDCP);
      kwDbmDelSingleRb(rbCb);

   } 
   RETVALUE(ret);
} /* pjCfgDelPjRBCb */


/**
 * @brief
 * Handler to process the re-establishment Request for SRB1 during
 * RRC Re-Establishment request.
 *
 * @b Description:
 * 1. This primitive is used to re-establish SRB1 only during RRC Re-establishment 
 * 2. New Security Algo will applied 
 *
 * @param [in]    reEstReq   -  Entity Configuration to be done.
 * @param [out]   reEstCfm   -  Entity Configuration cfm.
 * 
 * @return  ROK
 *
 */
#ifdef ANSI
PUBLIC S16 pjCfgReEstReq
(
CpjReEstReqInfo *reEstReq,
CpjReEstCfmInfo   *reEstCfm
)
#else
PUBLIC S16 pjCfgReEstReq (reEstReq, reEstCfm)
CpjReEstReqInfo *reEstReq;
CpjReEstCfmInfo   *reEstCfm;
#endif
{
   S16     ret;
   KwUeCb  *ueCb;
   RbCb    *rbCb;
   CmLtePdcpId *pdcpId;

   TRC3(pjCfgReEstReq);
 
   /* Initialization */
   ret = ROK;
   ueCb = NULLP;
   rbCb = NULLP;
   pdcpId   =  &reEstReq->pdcpId;

   PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "pjCfgReEstReq- Received for -- cellId(%d), \
            ueId(%d)\n", pdcpId->cellId, pdcpId->ueId));

   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(pdcpId->ueId, pdcpId->cellId, &ueCb);
   
   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      /* Fill entCfm structure */
      PJ_CFG_FILL_REESTREQ_CFM(reEstCfm, pdcpId->ueId,pdcpId->cellId,
            reEstReq->transId, CPJ_CFG_CFM_NOK);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgReEstReq- UE Cb is not present -- cellId(%d), \
               ueId(%d)\n", pdcpId->cellId, pdcpId->ueId));
      RETVALUE(ret);
   }

   /* Fetch Rb Cb */
   KW_DBM_FETCH_RBCB(pdcpId->rbId, pdcpId->rbType, ueCb, rbCb);

   /* Rb Cb is not present or Not configured for PDCP */
   if(NULLP == rbCb ||!(PJ_CFG_PDCP & rbCb->rbCfgStat))
   { 
      /* Fill entCfm structure */
      PJ_CFG_FILL_REESTREQ_CFM(reEstCfm, pdcpId->ueId,pdcpId->cellId,
            reEstReq->transId, CPJ_CFG_CFM_NOK);

      PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "pjCfgReEstReq-- RB CB is not found -- cellId(%d), \
               ueId(%d) rbId(%d)\n", pdcpId->cellId, pdcpId->ueId,
               pdcpId->rbId));
      RETVALUE(RFAILED);
   }

   /* Copy Security Information */ 

   cmMemcpy(&ueCb->secInfo.intInfo.intKey[0], &reEstReq->intKey[0], PJ_INTG_KEY_LEN);
   cmMemcpy(&ueCb->secInfo.cipherInfo.cpKey[0], &reEstReq->ctrlplnKey[0], PJ_CIPHER_KEY_LEN);
   cmMemcpy(&ueCb->secInfo.cipherInfo.upKey[0], &reEstReq->usrplnKey[0], PJ_CIPHER_KEY_LEN);

   /* Mark Security Action Done */
   ueCb->secInfo.secAct = TRUE;

   reEstCfm->transId = reEstReq->transId;
   reEstCfm->ueId    = reEstReq->pdcpId.ueId;
   reEstCfm->cellId  = reEstReq->pdcpId.cellId;

   /* Save the information */
#ifdef PJ_SEC_ASYNC
   pjUtlSaveCfmInfo(ueCb, PJ_REEST_ASYNC_CFM, TRUE, ENTPJ, 
                                          (PTR)reEstCfm, NULLP);
#else
   pjUtlSaveCfmInfo(ueCb, PJ_REEST_ASYNC_CFM, FALSE, ENTPJ, 
                                          (PTR)reEstCfm, NULLP);
#endif /* PJ_SEC_ASYNC */

   /* Call ReEst for SRB1 */
   pjUtlReEstSrb1(&rbCb->pjRbCb);

   PJ_FREE(reEstCfm, sizeof(CpjReEstCfmInfo));

   RETVALUE(ROK);
} /* pjCfgReEstReq */


/*@}*/

/********************************************************************30**

         End of file:     gp_pj_cfg.c@@/main/1 - Fri Nov 13 14:13:43 2009

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
/main/1      ---      rbabu    1. LTERLC Release 2.1
/main/2      kw004.201 ap      1. Sending  status as CPJ_CFG_CFM_OK 
                                  in deleting unknown UE
/main/3      kw005.201 ap      1. Removed extra fetching of ueCb in the 
                                  function pjCfgUeIdChng.
                               2. Added support for L2 Measurement.
/main/4      kw006.201 rd      1. Added boundary condition check for countInfo
                                  in pjCfgPrcCount.
                       ap      2. ccpu00120574, fixed the warning.
                               3. ccpu00120654, sending ROK or RFAILED in countCCfm
                                  as status.
*********************************************************************91*/
