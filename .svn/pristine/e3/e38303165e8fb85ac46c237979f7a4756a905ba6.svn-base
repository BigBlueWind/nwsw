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
  
        Name:    RLC - Database module file
    
        Type:    C source file
  
        Desc:    Source code for Database Module functions such as, 

                 - kwDbmInit
                 - kwDbmDeInit
                 - kwDbmCreateRbCb
                 - kwDbmFetchRbCb 
                 - kwDbmFetchRbCbForUi
                 - kwDbmFetchRbCbForLi
                 - kwDbmUpdateRbCb
                 - kwDbmDelRbCb
                 - kwDbmDelSingleRb
                 - kwDbmDelAllRb
                 - kwDbmCreateUeCb
                 - kwDbmFetchUeCb
                 - kwDbmUpdateUeCb
                 - kwDbmDelUeCb
                 - kwDbmDelAllUe
                 - kwDbmDelMeasEvtCb
                 - kwDbmCreateCellCb
                 - kwDbmFetchCellCb
                 - kwDbmUpdateCellCb
                 - kwDbmDelCellCb
                 - kwDbmDelAllCell
                 - kwDbmShutdown
                 - kwDbmRemUeCbFromUeList
                 - kwDbmGetNextUeFromUeList
                 - kwDbmFreeRbCb

        File:    gp_dbm.c

        Sid:      gp_dbm.c@@/main/2 - Fri Nov 13 14:13:37 2009
 
        Prg:     nm
  
*********************************************************************21*/


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
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */
#ifdef KW_PDCP
#include "lpj.h"           /* LPJ defines */
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.h"            /* RLC defines */

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
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#ifdef KW_PDCP
#include "lpj.x"           /* LPJ defines */
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.x"


/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */
PUBLIC S16 kwDbmRemUeCbFromUeList ARGS ((KwUeCb *ueCb));
PRIVATE S16 kwDbmGetNextUeFromUeList ARGS ((KwUeCb *ueCb));
PRIVATE Void kwDbmFetchRbCbWL ARGS ((U8 rbId, U8 rbType, KwUeCb *ueCb, RbCb **rbCb));
PRIVATE Void kwDbmFreeRbCb ARGS ((RbCb **rbCb));

/** @file gp_dbm.c
@brief RLC Acknowledged Mode Module
**/


/**
 *
 * @brief Handler to initialize hash list
 *       
 *
 * @b Description
 *        This function is invoked by LMM to initialize the UeCb and CellCb
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmInit
(
)
#else
PUBLIC S16 kwDbmInit()
#endif
{
   S16      ret;              /* Return Value */

   TRC3(kwDbmInit)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwDbmInit()\n"));
#endif

   /* Initialize ueCb Hash List */
   ret = cmHashListInit(&(kwCb.ueLstCp), (U16) KW_UE_LIST_BUCKET_SIZE,
         (U16) 0, (Bool) FALSE, (U16) CM_HASH_KEYTYPE_DEF,
         KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb));
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmInit: cmHashListInit Failed for kwCb.ueLstCp.\n"));
#endif
      RETVALUE(ret);
   }

   /* Initialize cellCb Hash List */
   ret = cmHashListInit(&(kwCb.cellLstCp), (U16) KW_CELL_LIST_BUCKET_SIZE,
         (U16) 0, (Bool) FALSE, (U16) CM_HASH_KEYTYPE_DEF,
         kwCb.init.region, kwCb.init.pool);
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmInit: cmHashListInit Failed for kwCb.cellLstCp.\n"));
#endif
      RETVALUE(ret);
   }

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
   /* Initialize qcI Hash List */
   ret = cmHashListInit(&(kwCb.kwL2Cb.qciHlCp), (U16) KW_QCI_LIST_BUCKET_SIZE,
         (U16) 0, (Bool) TRUE, (U16) CM_HASH_KEYTYPE_DEF,
         kwCb.init.region, kwCb.init.pool);
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmInit: cmHashListInit Failed for kwCb.qciHlCp.\n"));
#endif
      RETVALUE(ret);
   }
   
   /* Initialize tbHlCp Hash List */
   ret = cmHashListInit(&(kwCb.kwL2Cb.tbHlCp), (U16) KW_TB_LIST_BUCKET_SIZE,
         (U16) 0, (Bool) FALSE, (U16) CM_HASH_KEYTYPE_DEF,
         kwCb.init.region, kwCb.init.pool);
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmInit: cmHashListInit Failed for kwCb.tbHlCp.\n"));
#endif
      RETVALUE(ret);
   }


#endif /* LTE_L2_MEAS */
   RETVALUE(ROK);
} /* kwDbmInit */


/**
 *
 * @brief Handler to De initialize hash list
 *       
 *
 * @b Description
 *        This function is invoked by LMM to De initialize the UeCb and CellCb
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmDeInit
(
)
#else
PUBLIC S16 kwDbmDeInit()
#endif
{
   S16      ret;                       /* Return Value */

   TRC3(kwDbmDeInit);

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwDbmDeInit()\n"));
#endif

   /* De Initialize ueCb Hash List */
   ret = cmHashListDeinit(&(kwCb.ueLstCp));
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDeInit: cmHashListDeinit Failed for kwCb.ueLstCp.\n"));
      RETVALUE(ret);
   }

   /* De Initialize cellCb Hash List */
   ret = cmHashListDeinit(&(kwCb.cellLstCp));
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDeInit: cmHashListDeinit Failed for kwCb.cellLstCp.\n"));
      RETVALUE(ret);
   }
/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
   /* De Initialize qciCb Hash List */
   ret = cmHashListDeinit(&(kwCb.kwL2Cb.qciHlCp));
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDeInit: cmHashListDeinit Failed for kwCb.qciHlCp.\n"));
      RETVALUE(ret);
   }
#endif /* LTE_L2_MEAS */
#ifdef KW_BG_UL_PROC
   /*Free the mBufUl used for UL reassembly*/
   ret = SPutMsg(kwCb.mBufUl);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDeInit: SPutMsg Failed for kwCb.mBufUl.\n"));
      RETVALUE(ret);
   }
#endif /* KW_BG_UL_PROC */
#ifdef KW_BG_DL_PROC
   /*Free the mBufDl used for DL Acked PDU deletion*/
   ret = SPutMsg(kwCb.mBufDl);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDeInit: SPutMsg Failed for kwCb.mBufDl.\n"));
      RETVALUE(ret);
   }
#endif /* KW_BG_DL_PROC */
   RETVALUE(ROK);
} /* kwDbmDeInit */


/**
 *
 * @brief Handler to fetch rbCb in the Upper Interface
 *       
 *
 * @b Description
 *        This function is invoked by CFG to fetch rbCb from the cellCb/ueCb in
 *        the upper interface (CKW/KWU).
 *
 *  @param[in]    rlcId    RLC Identifier 
 *  @param[out]   rbCb     RB Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmFetchRbCbForUi
(
CmLteRlcId     rlcId,      /* RLC Identifier */
KwRbCb         **rbCb       /* RB Cb */
)
#else
PUBLIC S16 kwDbmFetchRbCbForUi(rlcId, rbCb)
CmLteRlcId     rlcId;      /* RLC Identifier */
KwRbCb         **rbCb;      /* RB Cb */
#endif
{
   KwUeCb      *ueCb;                  /* UE Control Block */
   KwCellCb    *cellCb;                /* Cell Control Block */
   RbCb        **rbCbLst;              /* RB Control Block List */
   RbCb        *tRbCb;                 /* Local RB CB */
   S16         ret;                    /* Return Value */

   TRC3(kwDbmFetchRbCbForUi)

   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmFetchRbCbForUi(rlcId(ueId(%d), cellId(%d), rbId(%d)),\
   rbCb)\n", rlcId.ueId, rlcId.cellId, rlcId.rbId));

   ueCb = NULLP;
   cellCb = NULLP;
   rbCbLst = NULLP;
   ret = ROK;

   /* Validate the RBID in case of TM/UM/AM */
   KW_VALIDATE_RBID(rlcId.ueId, rlcId.rbId, rlcId.rbType, ret);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgReEstRb -- KW_VALIDATE_RBID Failed --\
               rbId(%d), cellId(%d), ueId(%d)\n",\
               rlcId.rbId, rlcId.cellId, rlcId.ueId));
      RETVALUE(ret);
   }

   /* Check for UE CB or CELL CB */
   if (rlcId.ueId == 0)
   {
      if (kwDbmFetchCellCb(rlcId.cellId, &cellCb) != ROK)
      {
         KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwDbmFetchRbCbForUi: kwDbmFetchCellCb Failed.\n"));
         RETVALUE(RFAILED);
      }

      rbCbLst = cellCb->rbCb;
      KW_DBM_FETCH_CELL_RBCB(rlcId.rbId, rbCbLst, tRbCb);
   }
   else
   {
      if (kwDbmFetchUeCb(rlcId.ueId, rlcId.cellId, &ueCb) != ROK)
      {
         KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwDbmFetchRbCbForUi: kwDbmFetchUeCb Failed.\n"));
         RETVALUE(RFAILED);
      }

      KW_DBM_FETCH_RBCB(rlcId.rbId, rlcId.rbType, ueCb, tRbCb);
   }


   if (tRbCb == NULLP)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmFetchRbCbForUi: kwDbmFetchRbCb Failed.\n"));
      RETVALUE(RFAILED);
   }

   *rbCb = &tRbCb->kwRbCb;

   RETVALUE(ROK);
} /* kwDbmFetchRbCbForUi */


/**
 *
 * @brief Handler to fetch rbCb in the Upper Interface
 *       
 *
 * @b Description
 *        This function is invoked by CFG to fetch rbCb from the cellCb/ueCb in
 *        the upper interface (CKW/KWU).
 *
 *  @param[in]    cellId   -  CELL Identifier 
 *  @param[in]    ueId     -  UE Identifier 
 *  @param[in]    lcId     -  Logical Channel Identifier
 *  @param[in]    dir      -  Direction DL/UL
 *  @param[out]   rbCb     -  RB Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmFetchRbCbForLi
(
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
CmLteLcId         lcId,       /* Logical channel instance */
U8                dir,        /* Direciotn DL/UL */
KwRbCb            **rbCb       /* RB Cb */
)
#else
PUBLIC S16 kwDbmFetchRbCbForLi(ueId, cellId, lcId, dir, rbCb)
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
CmLteLcId         lcId;       /* Logical channel instance */
U8                dir;        /* Direciotn DL/UL */
KwRbCb            **rbCb;      /* RB Cb */
#endif
{
   KwUeCb      *ueCb;                  /* UE Control Block */
   KwCellCb    *cellCb;                /* Cell Control Block */
   /* kw006.201, ccpu00120574, fixed the warning */

   TRC3(kwDbmFetchRbCbForUi)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmFetchRbCbForLi(rlcId(ueId(%d), cellId(%d)))\n",
            ueId, cellId));
#endif

   ueCb = NULLP;
   cellCb = NULLP;
   /* kw006.201, ccpu00120574, fixed the warning */

   /* Check for UE CB or CELL CB */
   if (ueId == 0)
   {
      if (kwDbmFetchCellCb(cellId, &cellCb) != ROK)
      {

#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwDbmFetchRbCbForLi: kwDbmFetchCellCb Failed.\n"));
#endif
         RETVALUE(RFAILED);
      }

      if (dir == CKW_CFG_DIR_UL)
      {
         *rbCb = cellCb->lCh[lcId - 1].ulRbCb;
      }
      else
      {
         *rbCb = cellCb->lCh[lcId - 1].dlRbCb;
      }
   }
   else
   {
      if (kwDbmFetchUeCb(ueId, cellId, &ueCb) != ROK)
      {
#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwDbmFetchRbCbForLi: kwDbmFetchUeCb Failed.\n"));
#endif
         RETVALUE(RFAILED);
      }

      if (dir == CKW_CFG_DIR_UL)
      {
         *rbCb = ueCb->lCh[lcId - 1].ulRbCb;
      }
      else
      {
         *rbCb = ueCb->lCh[lcId - 1].dlRbCb;
      }
   }

   if (*rbCb == NULLP)
   {

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmFetchRbCbForLi: RB CB is NULLP.\n"));
#endif
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* kwDbmFetchRbCbForLi */

/* [ccpu00127171] New function added to handle deletion of single RB */
/**
 *
 * @brief Handler to delete Single RbCb
 *       
 * @b Description
 *        This function is invoked by PDCP CFG to remove a single RbCb from 
 *        Ue/Cell hashlist 
 *
 *  @param[in] ueCb   - UE control block
 *  @param[in] rbCb   - RBCB to be deleted
 *
 *  @return  Void
 *
*/
#ifdef ANSI
PUBLIC Void kwDbmDelSingleRb
(
RbCb                    *rbCb
)
#else
PUBLIC Void kwDbmDelSingleRb(rbCb)
RbCb                    *rbCb;
#endif
{
   CmLteRbId rbId;
   U8        rbType;
   
   TRC3(kwDbmDelSingleRb)

   rbId = rbCb->kwRbCb.rlcId.rbId;
   rbType = rbCb->kwRbCb.rlcId.rbType;
   MSPD_DBG("[%d][%d][%d] Call UtlFreeRbCb to initiate RB delete\n", 
             rbCb->kwRbCb.rlcId.ueId, rbType, rbId);
   kwUtlFreeRbCb(&rbCb->kwRbCb);
}

/**
 *
 * @brief Handler to delete RbCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to remove RbCb from Ue/Cell hashlist 
 *
 *
 *  @param[in] rbCbLst   - Rb Cb List
 *  @param[in] numRbCb   - Number of rbCbs
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmDelAllRb
(
RbCb         **rbCbLst,              /* RB Cb list */
U8             numRbCb                 /* Number of rbCbs */
)
#else
PUBLIC S16 kwDbmDelAllRb(rbCbLst, numRbCb)
RbCb         **rbCbLst;              /* RB Cb list */
U8             numRbCb;                /* Number of rbCbs */
#endif
{
   U8          idx;                    /* Index */
   RbCb        *rbCb;                  /* RB Control Block */

   TRC3(kwDbmDelAllRb)

   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmDelAllRb(rbCbLst, numRbCb(%d))\n", numRbCb));
   for (idx = 0; idx < numRbCb; idx++)
   {
      rbCb = rbCbLst[idx];
      if (rbCb != NULLP)
      {
/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
         /* Check if measurement is running, if yes send an alarm to LM. */
         if(rbCb->kwRbCb.rbL2Cb.measOn)
         {
            KW_SEND_UEID_ALARM(rbCb->kwRbCb.ueCb->key.ueId, rbCb->kwRbCb.qci,
                          LKW_EVT_MEAS_HALT, LKW_CAUSE_RB_DELETION);
         }
         cmHashListDelete(&(kwCb.kwL2Cb.qciHlCp), (PTR)&rbCb->kwRbCb);
#endif /* LTE_L2_MEAS */
         /* Free the Buffers of RbCb */
         kwUtlFreeRbCb(&rbCb->kwRbCb);
         rbCbLst[idx] = NULLP;
      }
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to create an UeCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to create UeCb and insert into the Ue hashlist 
 *        of KwCb.
 *
 *
 *  @param[in] ueId     UE Identifier 
 *  @param[in] cellId   Cell Identifier 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmCreateUeCb
(
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
KwUeCb            **ueCb       /* UE Control Block */
)
#else
PUBLIC S16 kwDbmCreateUeCb(ueId, cellId, ueCb)
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
KwUeCb            **ueCb;     /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */
   KwUeCb   *tUeCb;           /* UE Control Block */

   TRC3(kwDbmCreateUeCb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmCreateUeCb(ueId(%d), cellId(%d))\n", ueId, cellId));
#endif
   MSPD_DBG_RLC("Entering kwDbmCreateUeCb\n");

   KW_ALLOC(*ueCb, sizeof(KwUeCb));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*ueCb == NULLP)
   {
      ret = RFAILED;
      KWLOGERROR(ERRCLS_ADD_RES, EKW023, (ErrVal) ret,
            "kwDbmCreateUeCb: UeCb allocation Failed.");
      RETVALUE(ret);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   MSPD_DBG_RLC("UeCb allocated\n");

   tUeCb = *ueCb;
   tUeCb->key.ueId = ueId;
   tUeCb->key.cellId = cellId;

   ret = cmHashListInsert(&(kwCb.ueLstCp), (PTR)tUeCb, (U8 *)&(tUeCb->key),
         (U16) sizeof(KwUeKey));
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmCreateUeCb: cmHashListInsert Failed for ueCb.\n"));
#endif
      RETVALUE(ret);
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   kwCb.genSts.numUe++;

#ifdef KW_PDCP
   (*ueCb)->libInfo.state =  PJ_STATE_NORMAL;
   cmInitTimers(&((*ueCb)->libInfo.obdTmr), 1);

#endif /* KW_PDCP */

   RETVALUE(ret);
} /* kwDbmCreateUeCb */

/**
 *
 * @brief Handler to Fetch an UeCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *        of KwCb.
 *
 *
 *  @param[in]    ueId     UE Identifier 
 *  @param[in]    cellId   Cell Identifier 
 *  @param[out]   ueCb     UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmFetchUeCb
(
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
KwUeCb            **ueCb       /* UE Control Block */
)
#else
PUBLIC S16 kwDbmFetchUeCb(ueId, cellId, ueCb)
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
KwUeCb            **ueCb;      /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */
   KwUeKey  key;              /* Key for UE Hash List */

   TRC3(kwDbmFetchUeCb)

   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmFetchUeCb(ueId(%d), cellId(%d))\n", ueId, cellId));
   KW_RDWR_LOCK(&kwCb.dbmLock);
   {
      key.ueId = ueId;
      key.cellId = cellId;

      ret = cmHashListFind(&(kwCb.ueLstCp), (U8 *)&(key), sizeof(KwUeKey),
            KW_DEF_SEQ_NUM, (PTR *) ueCb);
   }
   KW_RDWR_UNLOCK(&kwCb.dbmLock);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmFetchUeCb: cmHashListFind Failed for ueCb.\n"));
      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* kwDbmFetchUeCb */

/**
 *
 * @brief Handler to Fetch an UeCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *        of KwCb.
 *
 *
 *  @param[in]    ueId     UE Identifier 
 *  @param[in]    cellId   Cell Identifier 
 *  @param[out]   ueCb     UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PRIVATE S16 kwDbmFetchUeCbWL
(
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
KwUeCb            **ueCb       /* UE Control Block */
)
#else
PRIVATE S16 kwDbmFetchUeCbWL(ueId, cellId, ueCb)
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
KwUeCb            **ueCb;      /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */
   KwUeKey  key;              /* Key for UE Hash List */

   TRC3(kwDbmFetchUeCbWL)

   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmFetchUeCbWL(ueId(%d), cellId(%d))\n", ueId, cellId));

   MSPD_DBG_RLC("Entering kwDbmFetchUeCbWL\n");
   key.ueId = ueId;
   key.cellId = cellId;

   ret = cmHashListFind(&(kwCb.ueLstCp), (U8 *)&(key), sizeof(KwUeKey),
         KW_DEF_SEQ_NUM, (PTR *) ueCb);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmFetchUeCbWL: cmHashListFind Failed for ueCb.\n"));
      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* kwDbmFetchUeCbWL */

/**
 *
 * @brief Handler to remove UeCB from UeCb Hashlist
 *       
 * @b Description
 *        This function should be invoked for removing UeCb from Hashlist.
 *        It does so with a protection of lock.
 *
 *  @param[in] ueCb     UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmRemUeCbFromUeList
(
KwUeCb        *ueCb
)
#else
PUBLIC S16 kwDbmRemUeCbFromUeList(ueCb)
KwUeCb        *ueCb;
#endif
{
   S16        ret; 
   KW_RDWR_LOCK(&kwCb.dbmLock);
   {
      ret = cmHashListDelete(&(kwCb.ueLstCp), (PTR) ueCb);
   }
   KW_RDWR_UNLOCK(&kwCb.dbmLock);
   RETVALUE(ret);
}
/**
 *
 * @brief Handler to delete an UeCb
 *       
 * @b Description
 *        This function is invoked by PDCP CFG to delete all entries(PDCP
 *        and RLC) related with an UE.
 *
 *  @param[in] ueCb     UE Control Block
 *  @param[in] abrtFlag Abort Flag
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmDelUeCb
(
KwUeCb      *ueCb,       /* UE Identifier */
Bool        abortFlag    /* Abort Flag */
)
#else
PUBLIC S16 kwDbmDelUeCb(ueCb, abortFlag)
KwUeCb      *ueCb;       /* UE Identifier */
Bool        abortFlag;   /* Abort Flag */
#endif
{
   S16         ret = ROK;      /* Return Value */
   U8          idx;      /* Index */
#ifdef KW_PDCP
   Bool        isLogicalChAvailable = FALSE; /*Flag to ensure UeCb is deleted once*/
#endif /* KW_PDCP */

   TRC3(kwDbmDelUeCb)

   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmDelUeCb(ueId(%d), cellId(%d))\n",
            ueCb->key.ueId, ueCb->key.cellId));
#if 0
#if  (!defined(KW_PDCP) || !(defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))
   UNUSED(abortFlag);
#endif /* (!defined(KW_PDCP) || ! (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))*/

   /* Delete all logical channels */
   for (idx = 0; idx < KW_MAX_LCH_PER_UE; idx++)
   {
      /* kw005.201 fixed the error in the code */
      ueCb->lCh[idx].dlRbCb = NULLP;
      ueCb->lCh[idx].ulRbCb = NULLP;
   }

#ifdef KW_PDCP
   for ( idx = 0; idx< KW_MAX_SRB_PER_UE; idx++)
       pjCfgDelPjRBCb(ueCb, ueCb->srbCb[idx]);

   for ( idx = 0; idx< KW_MAX_DRB_PER_UE; idx++) 
       pjCfgDelPjRBCb(ueCb, ueCb->drbCb[idx]);

   /* Close the Integrity/Ciphering channels */
   if(ueCb->secInfo.secAct)
   {
      pjUtlIntClose(ueCb->secInfo.intCxtId); 
      pjUtlCipherClose(ueCb->secInfo.cpCxtId);
      pjUtlCipherClose(ueCb->secInfo.upCxtId);
   }

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

      PJ_FREE(ueCb->hoInfo->hoCfmInfo, (KW_MAX_DRB_PER_UE * sizeof(PjHoCfmInfo)));
      PJ_FREE(ueCb->hoInfo, sizeof(PjHoInfo));  
   }

#endif /* KW_PDCP */

   /* Delete all SRB RbCbs in UeCb */
   ret = kwDbmDelAllRb(ueCb->srbCb, KW_MAX_SRB_PER_UE);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDelUeCb: kwDbmDelAllRb Failed for ueCb.\n"));
   }

   /* Delete all DRB RbCbs in UeCb */
   ret = kwDbmDelAllRb(ueCb->drbCb, KW_MAX_DRB_PER_UE);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDelUeCb: kwDbmDelAllRb Failed for ueCb.\n"));
   }

#ifdef KW_PDCP
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   if(!abortFlag)
   {
      RETVALUE(ROK);
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */
#endif /* KW_PDCP */

   /* Delete ueCb entry from ueLstCp */
   ret = kwDbmRemUeCbFromUeList(ueCb);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
            "kwDbmDelUeCb: cmHashListDelete Failed for ueCb.\n"));
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   kwCb.genSts.numUe--;
   MSPD_DBG("[%d] Freeing UeCb pointer\n", ueCb->key.ueId);
   KW_FREE(ueCb, sizeof(KwUeCb));
#endif
   RETVALUE(ret);
} /* kwDbmDelUeCb */

/**
 *
 * @brief Handler to get next UE from UeList
 *
 * @b Description
 *        This function is invoked to get next UE from UeList.
 *        It does so using dbmLock protection.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PRIVATE S16 kwDbmGetNextUeFromUeList
(
KwUeCb        *ueCb
)
#else
PRIVATE S16 kwDbmGetNextUeFromUeList(ueCb)
KwUeCb        *ueCb;
#endif
{
   S16        ret;
   KW_RDWR_LOCK(&kwCb.dbmLock);
   {
      ret = cmHashListGetNext(&(kwCb.ueLstCp), (PTR) ueCb, (PTR *)&ueCb);
   }
   KW_RDWR_UNLOCK(&kwCb.dbmLock);
   RETVALUE(ret);
}
/**
 *
 * @brief Handler to delete all UeCbs
 *       
 *
 * @b Description
 *        This function is invoked by CFG to delete all UeCbs from the Ue
 *        hashlist of KwCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmDelAllUe
(
)
#else
PUBLIC S16 kwDbmDelAllUe()
#endif
{
   S16         ret;              /* Return Value */
   KwUeCb      *ueCb;            /* UE Control Block */

   TRC3(kwDbmDelAllUe)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmDelAllUe()\n"));
#endif

   ret = ROK;
   ueCb = NULLP;

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   ret = kwDbmGetNextUeFromUeList(ueCb);
   while((ret == ROK) && (ueCb != NULLP))
   {
      /* Delete ueCb */
      ret = kwDbmDelUeCb(ueCb, TRUE);
      if (ret != ROK)
      {
         KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwDbmDelAllUe: kwDbmDelUeCb Failed.\n"));
         RETVALUE(ret);
      }
      ueCb = NULLP;
      ret = kwDbmGetNextUeFromUeList(ueCb);
   }

   RETVALUE(ret);
} /* kwDbmDelAllUe */
/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS

/**
 *
 * @brief Handler to delete all MeasEvtCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to delete all MeasEvtCb from the KwCb
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmDelMeasEvtCb
(
)
#else
PUBLIC S16 kwDbmDelMeasEvtCb()
#endif
{
   U8             evtIdx;
   KwL2MeasEvtCb  *measEvtCb;

   TRC3(kwDbmDelMeasEvtCb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmDelMeasEvtCb()\n"));
#endif

   for(evtIdx = 0; evtIdx < LKW_MAX_L2MEAS; evtIdx++)
   {
      if(kwCb.kwL2Cb.kwL2EvtCb[evtIdx] != NULLP)
      {
         measEvtCb = kwCb.kwL2Cb.kwL2EvtCb[evtIdx];
         if(TRUE == kwChkTmr((PTR)measEvtCb, KW_EVT_L2_TMR))
         {
            kwStopTmr((PTR)measEvtCb, KW_EVT_L2_TMR);
         }
         kwCb.kwL2Cb.kwL2EvtCb[evtIdx] = NULLP;
         kwCb.kwL2Cb.kwNumMeas--;
         KW_FREE(measEvtCb, sizeof(KwL2MeasEvtCb));
      }
   }

   RETVALUE(ROK);
} /* kwDbmDelMeasEvtCb */
#endif /* LTE_L2_MEAS */

/**
 *
 * @brief Handler to create CellCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to create CellCb and insert into the Cell hashlist 
 *        of KwCb.
 *
 *
 *  @param[in] cellId   Cell Identifier 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmCreateCellCb
(
CmLteCellId       cellId,     /* Cell Identifier */
KwCellCb          **cellCb    /* UE Control Block */
)
#else
PUBLIC S16 kwDbmCreateCellCb(cellId, cellCb)
CmLteCellId       cellId;     /* Cell Identifier */
KwCellCb          **cellCb;   /* UE Control Block */
#endif
{
   S16         ret;           /* Return Value */
   KwCellCb    *tCellCb;      /* UE Control Block */

   TRC3(kwDbmCreateCellCb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmCreateCellCb(cellId(%d))\n", cellId));
#endif

   KW_ALLOC(*cellCb, sizeof(KwCellCb));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*cellCb == NULLP)
   {
      ret = RFAILED;
      KWLOGERROR(ERRCLS_ADD_RES, EKW024, (ErrVal) ret,
            "kwDbmCreateCellCb: CellCb allocation Failed.");
      RETVALUE(ret);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   tCellCb = *cellCb;
   tCellCb->cellId = cellId;

   ret = cmHashListInsert(&(kwCb.cellLstCp), (PTR) tCellCb,
         (U8 *)&(tCellCb->cellId), (U16) sizeof(CmLteCellId));
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmCreateCellCb: cmHashListInsert Failed for cellCb.\n"));
#endif
      RETVALUE(ret);
   }

   RETVALUE(ret);
} /* kwDbmCreateCellCb */


/**
 *
 * @brief Handler to Fetch an CellCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *        of KwCb.
 *
 *
 *  @param[in]    ueId     UE Identifier 
 *  @param[in]    cellId   Cell Identifier 
 *  @param[out]   cellCb     UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmFetchCellCb
(
CmLteCellId       cellId,     /* Cell Identifier */
KwCellCb          **cellCb     /* Cell Control Block */
)
#else
PUBLIC S16 kwDbmFetchCellCb(cellId, cellCb)
CmLteCellId       cellId;     /* Cell Identifier */
KwCellCb          **cellCb;    /* Cell Control Block */
#endif
{
   S16            ret;              /* Return Value */

   TRC3(kwDbmFetchCellCb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmFetchCellCb(cellId(%d))\n", cellId));
#endif

   ret = cmHashListFind(&(kwCb.cellLstCp), (U8 *)&(cellId),
         sizeof(CmLteCellId), KW_DEF_SEQ_NUM, (PTR*) cellCb);
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmFetchCellCb: cmHashListFind Failed for cellCb.\n"));
#endif
      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* kwDbmFetchCellCb */


/**
 *
 * @brief Handler to delete CellCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to delete CellCb from the Cell hashlist 
 *        of KwCb.
 *
 *
 *  @param[in] cellCb   Cell Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmDelCellCb
(
KwCellCb    *cellCb      /* Cell Control Block */
)
#else
PUBLIC S16 kwDbmDelCellCb(cellCb)
KwCellCb    *cellCb;     /* Cell Control Block */
#endif
{
   S16         ret;              /* Return Value */

   TRC3(kwDbmDelCellCb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmDelCellCb(cellId(%d))\n", cellCb->cellId));
#endif

   ret = ROK;

   /* Delete all rbCbs in cellCb */
   ret = kwDbmDelAllRb(cellCb->rbCb, KW_MAX_RB_PER_CELL);
   if (ret != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDelCellCb: kwDbmDelAllRb Failed for cellCb.\n"));
#endif
   }

   /* Delete cellCb entry in hash list cellLstCp */
   ret = cmHashListDelete(&(kwCb.cellLstCp), (PTR) cellCb);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwDbmDelCellCb: cmHashListDelete Failed for cellCb.\n"));
   }

   /* Deallocate cellCb */
   KW_FREE(cellCb, sizeof(KwCellCb));

   RETVALUE(ret);
} /* kwDbmDelCellCb */


/**
 *
 * @brief Handler to delete all UeCbs
 *       
 *
 * @b Description
 *        This function is invoked by CFG to delete all UeCbs from the Ue
 *        hashlist of KwCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmDelAllCell
(
)
#else
PUBLIC S16 kwDbmDelAllCell()
#endif
{
   S16         ret;              /* Return Value */
   KwCellCb    *cellCb;          /* Cell Control Block */

   TRC3(kwDbmDelAllCell)

   KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_BRIEF), (_kwPBuf,
            "kwDbmDelAllCell()\n"));

   ret = ROK;
   cellCb = NULLP;

   /* Until no more cellCb is ueLstCp hash list get and delete cellCb */
   while (cmHashListGetNext(&(kwCb.cellLstCp), (PTR) cellCb, (PTR *)&cellCb) == ROK)
   {
      /* Delete cellCb */
      ret = kwDbmDelCellCb(cellCb);
      if (ret != ROK)
      {
         KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwDbmDelAllCell: kwDbmDelCellCb Failed.\n"));
         RETVALUE(ret);
      }

      cellCb = NULLP;
   }

   RETVALUE(ret);
} /* kwDbmDelAllCell */


/**
 *
 * @brief Handler to delete all UeCbs and CellCbs
 *       
 *
 * @b Description
 *        This function is invoked by LMM to delete all UeCbs from the Ue
 *        hashlist of KwCb and cellCbs from the Cell hashlist of kwCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 kwDbmShutdown
(
)
#else
PUBLIC S16 kwDbmShutdown()
#endif
{
   S16 ret;

   TRC3(kwDbmDelAllCell)

   ret = kwDbmDelAllCell();
   /* kw006.201, ccpu00120574, fixed the warning */
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      KWLOGERROR(ERRCLS_DEBUG, EKW025, (ErrVal) ret,
               "kwDbmShutdown: kwDbmDelAllCell Failed.\n");
#endif /* ERRCLASS & ERRCLS_DEBUG */
   }
/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
   ret = kwDbmDelMeasEvtCb();
   /* kw006.201, ccpu00120574, fixed the warning */
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      KWLOGERROR(ERRCLS_DEBUG, EKWXXX, (ErrVal) ret,
               "kwDbmShutdown: kwDbmDelMeasEvtCb Failed.\n");
#endif /* ERRCLASS & ERRCLS_DEBUG */
   }
#endif /* LTE_L2_MEAS */
   ret = kwDbmDelAllUe();
   /* kw006.201, ccpu00120574, fixed the warning */
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      KWLOGERROR(ERRCLS_INT_PAR, EKW026, (ErrVal) ret,
               "kwDbmShutdown: kwDbmDelAllUe Failed.\n");
#endif /* ERRCLASS & ERRCLS_DEBUG */
   }
   ret = kwDbmDeInit();
   /* kw006.201, ccpu00120574, fixed the warning */
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      KWLOGERROR(ERRCLS_INT_PAR, EKW027, (ErrVal) ret,
               "kwDbmShutdown: kwDbmDeInit Failed.\n");
#endif /* ERRCLASS & ERRCLS_DEBUG */
   }

   RETVALUE(ROK);
} /* kwDbmShutdown */

#ifdef KW_PDCP
/**
 *
 * @brief Handler to create a RB control block.
 *       
 *
 * @b Description
 *        This function is called to create a RLC control block or PDCP 
 *        control block based on rb flag and update the pointers in RbCb.
 *        If the RbCb is already allocated, the rbId is updated in KwRbCb
 *        or PjRbCb based on rb. If the RbCb has not been allocated earlier,
 *        a new RbCb is created and the pointers are updated along with the
 *        rbIds.
 *
 *  @param[in] rbId      RB ID of the entity
 *  @param[in] rbCbLst   List of the RBs in the UeCb
 *  @param[in] rb        The RB to be created. This can be
 *                       PJ_CFG_PDCP when PDCP is created
 *                       or PJ_CFG_RLC when RLC is being
 *                       created. 
 *  
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC RbCb* kwDbmCreateRbCb
(
U8      rbId,
U8      rbType, 
KwUeCb  *ueCb, 
U8      rb
)
#else
PUBLIC RbCb* kwDbmCreateRbCb(rbId,rbType,ueCb,rb)
U8      rbId; 
U8      rbType;
KwUeCb  *ueCb; 
U8      rb;
#endif
{
   RbCb   *rbCb;
   RbCb   **rbCbLst;

   TRC2(kwDbmCreateRbCb)

   rbCb = NULLP;
   rbCbLst = NULLP;

   KW_DBM_FETCH_RBCB(rbId, rbType, ueCb, rbCb);

   if(rbCb == NULLP)
   {
      KW_ALLOC( rbCb, sizeof(RbCb) );
      if ( rbCb == NULLP )
      {
         PJLOGERROR(ERRCLS_DEBUG, EKW028, (ErrVal)0,
               "kwDbmCreateRbCb: PJ_ALLOC failed for rbCb.\n");
         RETVALUE(rbCb);
      }
      MSPD_DBG("RbCb (0x%x) created of size %d, ueId %d\n", rbCb, sizeof(RbCb), ueCb->key.ueId);
      /* kw006.201 ccpu00121338, numOfRb increments twice for a single *
       * DCCH/DTCH. So, moved it here where allocation happens.        */
      /* kw005.201 ccpu00117318, updating the statistics */   
      KW_LMM_RB_STS_INC();                                    
   }

   if ( rb == PJ_CFG_PDCP )
   {
      rbCb->pjRbCb.rbId   = rbId;
      rbCb->pjRbCb.rbType = rbType;
      rbCb->rbCfgStat    |= rb;
   }
   else
   {
      rbCb->kwRbCb.rlcId.rbId   = rbId;
      rbCb->kwRbCb.rlcId.rbType = rbType;
      rbCb->rbCfgStat          |= rb;
   }

   /* Update the RB Cfg state with PDCP */
   if(rbCb->rbCfgStat == PJ_CFG_BOTH)
   {
      rbCb->kwRbCb.pjRbCb = &(rbCb->pjRbCb);
      rbCb->pjRbCb.kwRbCb = &(rbCb->kwRbCb);

   }
  
   rbCbLst = ((rbType == CM_LTE_SRB)? ueCb->srbCb:ueCb->drbCb); 
   rbCbLst[rbId] = rbCb;
/*Optimization change for ccpu00111716 and ccpu00117290*/
#if ((defined KW_BG_DL_PROC) || (defined KW_BG_UL_PROC))
   /*RbCb pointer is stored in KwRbCb so as to free it after all DL/UL PDUs*
    *are freed.                                                            */
   rbCb->kwRbCb.rb = rbCb;
#endif /* ((defined KW_BG_DL_PROC) || (defined KW_BG_UL_PROC)) */
   
   RETVALUE(rbCb); 
   
} /* kwDbmCreateRbCb */
/**
 *
 * @brief Handler to fetch a  PDCP RB control block.
 *       
 *
 * @b Description
 *        This function is used to fetch the PDCP RB control block based on 
 *        the RB id. The pointer to PjRbCb is returned.
 *       
 *  @param[in]  ueCb    UE control block of the PDCP RB
 *  @param[in]  rbId    RB ID of the required PDCP entity.
 *  @param[out] pjRbCb  PDCP RB Control Block.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 kwDbmFetchPjRbCb
(
KwUeCb  *ueCb,
U8       rbId,
U8       rbType,
PjRbCb  **pjRbCb 
)
#else
PUBLIC S16 kwDbmFetchPjRbCb(ueCb,rbId,rbType,pjRbCb)
KwUeCb  *ueCb;      
U8       rbId;
U8       rbType;
PjRbCb  **pjRbCb;
#endif
{
   RbCb    *rbCb;

   TRC2(kwDbmFetchPjRbCb)

   *pjRbCb = NULLP;

   KW_DBM_FETCH_RBCB(rbId, rbType, ueCb, rbCb);
   if ( rbCb != NULLP )
   {
      if ( rbCb->rbCfgStat & PJ_CFG_PDCP )
      {
         *pjRbCb = &(rbCb->pjRbCb);
      }
   }

   RETVALUE(ROK);

} /* kwDbmFetchPjRbCb */

/**
 *
 * @brief Handler to free a RLC RB control block and related pointers.
 *
 * @b Description
 *        This function is used to free the RLC RB control block and
 *        related pointers depending upon the mode and also decrement
 *        RB stats.
 *
 *  @param[in]  rbCb    RB control block
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwDbmFreeRbCb
(
RbCb                   **rbCb
)
#else
PRIVATE Void kwDbmFreeRbCb(rbCb)
RbCb                   **rbCb;
#endif
{
   U16             winLen;
   CmLteRlcMode    mode;
   TRC2(kwDbmFreeRbCb)

   mode = (*rbCb)->kwRbCb.mode;
   if(CM_LTE_MODE_AM == mode)
   {
      winLen = KW_AM_WIN_SZ << 1;
      KW_FREE((*rbCb)->kwRbCb.m.am.amDl.txBuf, (winLen * sizeof(KwTx*)));
      KW_FREE((*rbCb)->kwRbCb.m.am.amUl.recBuf, (winLen * sizeof(KwAmRecBuf*)));
   }
   else if(CM_LTE_MODE_UM == mode)
   {
      winLen = (*rbCb)->kwRbCb.m.um.umUl.umWinSz << 1;
      KW_FREE((*rbCb)->kwRbCb.m.um.umUl.recBuf, (winLen * sizeof(KwUmRecBuf*)));
   }
   else if(CM_LTE_MODE_TM == mode)
   {
      /* NOP */
   }

   KW_FREE((*rbCb), sizeof(RbCb));
   KW_LMM_RB_STS_DEC();
}
/**
 *
 * @brief Handler to delete a PDCP/RLC RB control block.
 *       
 *
 * @b Description
 *        This function is used to delete the PDCP/RLC RB control block based 
 *        on RB id and the RB type (PDCP/RLC). If the corresponding RB is 
 *        present, the cfgStat flag is unset. If the cfgStat flag is zero,
 *        the RBCB is freed.
 *
 *  @param[in]  rbId    RB ID of the to be deleted RB
 *  @param[in]  rbCbLst RbCb list in UE CB
 *  @param[in]  rb      The RB to be deleted. This can be
 *                      PJ_CFG_PDCP when PDCP is created
 *                      or PJ_CFG_RLC when RLC is being
 *                      created.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 kwDbmDelRbCb
(
U8      rbId,
U8      rbType,
RbCb   **rbCb, 
U8      rb
)
#else
PUBLIC S16 kwDbmDelRbCb(rbId,rbType,rbCb,rb)
U8      rbId;
U8      rbType;
RbCb   **rbCb;
U8      rb;
#endif
{
   S16             ret;
   RbCb            *saveRbCb;
   CmLteRnti       ueId;
   CmLteRlcMode    mode;
 
   TRC2(kwDbmDelRbCb)
   ueId = (*rbCb)->kwRbCb.rlcId.ueId;
   mode = (*rbCb)->kwRbCb.mode;
   saveRbCb = *rbCb;
   ret   = RFAILED;

   MSPD_DBG_RLC("[%d][%d][%d]rbCfgStat flag %d\n", ueId, rbType, rbId, rb);
   if ( (*rbCb) != NULLP )
   {
      (*rbCb)->rbCfgStat &= ~(rb);
      if ( !((*rbCb)->rbCfgStat & (PJ_CFG_BOTH)) )
      {
         kwDbmFreeRbCb(rbCb);
         MSPD_DBG("[%d][%d][%d] Freed RbCb (0x%x) mode (%d), delete RB complete\n", 
                   ueId, rbType, rbId, saveRbCb, mode);
      }
      ret = ROK;
   }
   RETVALUE(ret);

} /* kwDbmDelRbCb */

#ifdef ANSI
PUBLIC S16 kwDbmFetchAndCreateUeCb
(
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
KwUeCb            **ueCb      /* UE Control Block */
)
#else
PUBLIC S16 kwDbmFetchAndCreateUeCb(ueId, cellId, ueCb)
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
KwUeCb            **ueCb;      /* UE Control Block */
#endif
{
   S16               ret;

   TRC3(kwDbmFetchAndCreateUeCb);

   /* Initialization */
   ret  = ROK;

   MSPD_DBG_RLC("Entering kwDbmFetchAndCreateUeCb\n");
   KW_RDWR_LOCK(&kwCb.dbmLock);
   {
      ret = kwDbmFetchUeCbWL(ueId, cellId, ueCb);

      if(ret != ROK)
      { 
         /* Ue Cb is not present, Create Ue Cb */
         ret = kwDbmCreateUeCb(ueId, cellId, ueCb);
         if (ret != ROK)
         {
            /* Ue Cb allocation failed  */   
            //PJ_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
	    //CPJ_CFG_CFM_NOK, CPJ_CFG_REAS_UE_CREAT_FAIL);

            PJDBGP((KW_PJ_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                  "pjCfgAddRb- kwDbmCreateUeCb Failed -- cellId(%d), \
                  ueId(%d)\n", cellId, ueId));
         } 
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dbmLock);
   RETVALUE(ret);
}

#ifdef ANSI
PRIVATE Void kwDbmFetchRbCb
(
U8                rbId,       /* UE Identifier */
U8                rbType,     /* Cell Identifier */
KwUeCb            *ueCb,      /* UE Control Block */
RbCb              **rbCb
)
#else
PRIVATE Void kwDbmFetchRbCb(rbId, rbType, ueCb, rbCb)
U8                rbId;       /* UE Identifier */
U8                rbType;     /* Cell Identifier */
KwUeCb            *ueCb;      /* UE Control Block */
RbCb              **rbCb;
#endif
{
   KW_RDWR_LOCK(&kwCb.dbmLock);
   {
      RbCb  **rbCbLst;
      rbCbLst = (rbType==CM_LTE_SRB)?ueCb->srbCb:ueCb->drbCb;
      if (rbCbLst[rbId] != NULLP)
      {
         (*rbCb) = rbCbLst[rbId];
      }
      else
      {
         (*rbCb) = NULLP;
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dbmLock);
}

#ifdef ANSI
PRIVATE Void kwDbmFetchRbCbWL
(
U8                rbId,       /* UE Identifier */
U8                rbType,     /* Cell Identifier */
KwUeCb            *ueCb,      /* UE Control Block */
RbCb              **rbCb
)
#else
PRIVATE Void kwDbmFetchRbCbWL(rbId, rbType, ueCb, rbCb)
U8                rbId;       /* UE Identifier */
U8                rbType;     /* Cell Identifier */
KwUeCb            *ueCb;      /* UE Control Block */
RbCb              **rbCb;
#endif
{
   RbCb  **rbCbLst;
   rbCbLst = (rbType==CM_LTE_SRB)?ueCb->srbCb:ueCb->drbCb;
   if (rbCbLst[rbId] != NULLP)
   {
      (*rbCb) = rbCbLst[rbId];
   }
   else
   {
      (*rbCb) = NULLP;
   }
}

#ifdef ANSI
PUBLIC S16 kwDbmFetchAndCreateRbCb
(
U8                rbId,
U8                rbType, 
KwUeCb            *ueCb,
RbCb              **rbCb,
U8                rb
)
#else
PUBLIC S16 kwDbmFetchAndCreateRbCb(rbId, rbType, ueCb, rbCb, rb)
U8                rbId; 
U8                rbType;
KwUeCb            *ueCb; 
RbCb              **rbCb;
U8                rb;
#endif
{
   S16        ret;

   ret = ROK;
   KW_RDWR_LOCK(&kwCb.dbmLock);
   {
      kwDbmFetchRbCbWL(rbId, rbType, ueCb, rbCb);

      if(((*rbCb) != NULLP)
           &&( (( (*rbCb)->rbCfgStat | rb) == rb) || 
               ((*rbCb)->rbCfgStat == PJ_CFG_BOTH) ))
      {
         /* Fill entCfm structure */
         //KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
         //         CKW_CFG_REAS_RB_PRSNT);
         ret = RFAILED;
      }
      else if((NULLP == *rbCb) || ((*rbCb)->rbCfgStat != PJ_CFG_BOTH))
      {
         (*rbCb) = kwDbmCreateRbCb(rbId, rbType, ueCb, rb);
         if ( (*rbCb) == NULLP)
         {
            /* Fill entCfm structure */
            //KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,
            //      CKW_CFG_REAS_RB_CREAT_FAIL);

            ret = RFAILED;
         }
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dbmLock);
   RETVALUE(ret);
}
#endif /* KW_PDCP */

/********************************************************************30**
  
         End of file:     gp_dbm.c@@/main/2 - Fri Nov 13 14:13:37 2009
  
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
/main/1      ---      gk     1. Initial release.
/main/2      ---      nm     1. LTERLC Release 2.1
/main/3      kw005.201 ap    1. Updating the statistics.
                             2. Added support for L2 Measurement.
/main/4      kw005.201 rd    3. Added code for deleting the KwRbCb entry
                                from the qciHlCp when all RBs are deleted
                                in function kwDbmDelAllRb.
                             4. Updated with important Avtecs comments.
/main/5      kw006.201 rd    1. ccpu00111716 and ccpu00117290, added code for
                                storing RbCb in KwRbCb in kwDbmCreateRbCb.
                       ap    2. ccpu00120574, fixed the warning.
                       rd    3. Freeing of mBufUl is done in kwDbmDeInit.
                       rd    4. ccpu00121338, numOfRb increments twice for a
                                single DCCH/DTCH.
*********************************************************************91*/
