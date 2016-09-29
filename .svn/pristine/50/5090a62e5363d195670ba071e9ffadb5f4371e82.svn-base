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

        Name:    LTE RLC - Configuration Manager file

        Type:    C source file

        Desc:    It contains the following configuraiton primtives
                 for different actions
                    -- kwCfgValdtEntCfg
                    -- kwCfgFillRbCb
                    -- kwCfgRbInit
                    -- kwCfgAddRb
                    -- kwCfgReCfgRb
                    -- kwCfgDelRb
                    -- kwCfgReEstRb
                    -- kwCfgDelUe

        File:     gp_cfg.c

        Sid:      gp_cfg.c@@/main/2 - Fri Nov 13 14:13:36 2009

        Prg:      gk

*********************************************************************21*/
/** @file gp_cfg.c
@brief RLC Configuration Module
**/


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
#include "kw_err.h"        /* RLC error options */
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

#ifdef MSPD
EXTERN U32 ysMsAmmDelUeReqRcvd;
#endif

/** @details
 * This primitive Fills the configuration informtion (entCfg) received in RB Cb
 *
 * @param [out]   rbCb     -  RB Control Block
 * @param [out]   ueCb     -  UE Control Block
 * @param [in]    entCfg   -  RLC Entity configuration
 *
 * @return  ROK
 */
#ifdef ANSI
PRIVATE S16 kwCfgFillRbCb
(
KwRbCb            *rbCb,
KwUeCb            *ueCb,
CkwEntCfgInfo     *entCfg
)
#else
PRIVATE S16 kwCfgFillRbCb(rbCb, ueCb, entCfg)
KwRbCb            *rbCb;
KwUeCb            *ueCb;
CkwEntCfgInfo     *entCfg;
#endif
{
   S16            ret;
   U16            winLen;

   TRC3(kwCfgFillRbCb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
      "kwCfgFillRbCb(ueId(%d), cellId(%d), rbId(%d), rbType(%d))\n",\
      rbCb->rlcId.ueId, rbCb->rlcId.cellId, entCfg->rbId, entCfg->rbType));
#endif

   ret = ROK;

   /* Initialize according to entMode */
   switch (entCfg->entMode)
   {
      case CM_LTE_MODE_TM:
         {
            rbCb->lch[0].lChId  = entCfg->lCh[0].lChId;
            rbCb->lch[0].lChType = entCfg->lCh[0].type;

            rbCb->mode = entCfg->entMode;
            rbCb->dir = entCfg->dir;

            break;
         }

      case CM_LTE_MODE_UM:
         {
            /* If rbCb is already present then
             * Check for the direction already configured and verify the new
             * configuration requested is different from the previous.
             * If both directions are different then make
             * indx = 0 as Down Link and 
             * indx = 1 as Up Link
             */
            /* If rbId is same and same logical channel is going to be
             * used then what is the assumption to assign Dl and UL information
             */
            rbCb->lch[0].lChId  = entCfg->lCh[0].lChId;
            rbCb->lch[0].lChType = entCfg->lCh[0].type;

            rbCb->mode = entCfg->entMode;
            rbCb->dir = entCfg->dir;

            switch (rbCb->dir)
            {
               case KW_DIR_DL:
                  {
                     if (entCfg->m.umInfo.dl.snLen == KW_UM_CFG_SN_5)
                     {
                        rbCb->m.um.umDl.snLen = KW_UM_SN_5;
                     }
                     else
                     {
                        rbCb->m.um.umDl.snLen = KW_UM_SN_10;
                     }

                     ueCb->lCh[rbCb->lch[0].lChId - 1].dlRbCb = rbCb;

                     break;
                  }

               case KW_DIR_UL:
                  {
                     if (entCfg->m.umInfo.ul.snLen == KW_UM_CFG_SN_5)
                     {
                        rbCb->m.um.umUl.snLen = KW_UM_SN_5;
                     }
                     else
                     {
                        rbCb->m.um.umUl.snLen = KW_UM_SN_10;
                     }

                     rbCb->m.um.umUl.umWinSz = KW_POWER(2,
                           ((rbCb->m.um.umUl.snLen * KW_UM_CFG_SN_5) -1 ));

                     rbCb->m.um.umUl.reOrdTmrInt = 
                        entCfg->m.umInfo.ul.reOrdTmr;

                     cmInitTimers(&(rbCb->m.um.umUl.reOrdTmr), 1);

                     winLen = rbCb->m.um.umUl.umWinSz << 1;

                     KW_ALLOC(rbCb->m.um.umUl.recBuf, winLen *
                           sizeof(KwUmRecBuf*));

#if (ERRCLASS & ERRCLS_ADD_RES)
                     /* Sanity check */
                     if (rbCb->m.um.umUl.recBuf == NULLP)
                     {
                        ret = RFAILED;
                        KWLOGERROR(ERRCLS_ADD_RES, EKW019, (ErrVal) ret,
                              "kwCfgFillRbCb: UM RECBUF allocation Failed.");
                     }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

                     ueCb->lCh[rbCb->lch[0].lChId - 1].ulRbCb = rbCb; /*Rahul:lch[0] is changed to lch[1]*/
                     break;
                  }

               case KW_DIR_BOTH:
                  {
                     ueCb->lCh[rbCb->lch[0].lChId - 1].dlRbCb = rbCb;
                     ueCb->lCh[rbCb->lch[0].lChId - 1].ulRbCb = rbCb;

                     if (entCfg->m.umInfo.dl.snLen == KW_UM_CFG_SN_5)
                     {
                        rbCb->m.um.umDl.snLen = KW_UM_SN_5;
                     }
                     else
                     {
                        rbCb->m.um.umDl.snLen = KW_UM_SN_10;
                     }
                     /* kw005.201 fix for ccpu00119143 */
                     if(entCfg->m.umInfo.ul.snLen == KW_UM_CFG_SN_5)
                     {
                        rbCb->m.um.umUl.snLen = KW_UM_SN_5;
                     }
                     else
                     {
                        rbCb->m.um.umUl.snLen = KW_UM_SN_10;
                     }

                     rbCb->m.um.umUl.umWinSz = KW_POWER(2,
                           ((rbCb->m.um.umUl.snLen * KW_UM_CFG_SN_5) -1 ));

                     rbCb->m.um.umUl.reOrdTmrInt = 
                        entCfg->m.umInfo.ul.reOrdTmr;

                     cmInitTimers(&(rbCb->m.um.umUl.reOrdTmr), 1);

                     winLen = rbCb->m.um.umUl.umWinSz << 1;

                     KW_ALLOC(rbCb->m.um.umUl.recBuf, winLen *
                           sizeof(KwUmRecBuf*));

#if (ERRCLASS & ERRCLS_ADD_RES)
                     /* Sanity check */
                     if (rbCb->m.um.umUl.recBuf == NULLP)
                     {
                        ret = RFAILED;
                        KWLOGERROR(ERRCLS_ADD_RES, EKW020, (ErrVal) ret,
                              "kwCfgFillRbCb: UM RECBUF allocation Failed.");
                     }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

                     break;
                  }

               default:
                  {
                     ret = RFAILED;
                     KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                              "kwCfgFillRbCb Failed -- in UM configuraiton,\
                              for the invalid dir(%d)\n", rbCb->dir));
                  }
            }

            break;
         }

      case CM_LTE_MODE_AM:
         {
            /* Down Link Information */
            rbCb->lch[0].lChId  = entCfg->lCh[0].lChId;
            rbCb->lch[0].lChType = entCfg->lCh[0].type;

            rbCb->mode = entCfg->entMode;
            rbCb->dir = KW_DIR_BOTH;
            rbCb->m.am.amDl.pollPdu = entCfg->m.amInfo.dl.pollPdu;
            rbCb->m.am.amDl.pollByte = entCfg->m.amInfo.dl.pollByte;
            rbCb->m.am.amDl.maxRetx = entCfg->m.amInfo.dl.maxRetx;
            rbCb->m.am.amDl.pollRetxTmrInt = entCfg->m.amInfo.dl.pollRetxTmr;
            cmInitTimers(&(rbCb->m.am.amDl.pollRetxTmr), 1);
      
            rbCb->m.am.amDl.vtMs = (rbCb->m.am.amDl.vtA + 512) % 1024;

            /* Up Link Information */
            rbCb->lch[1].lChId  = entCfg->lCh[1].lChId;
            rbCb->lch[1].lChType = entCfg->lCh[1].type;

            rbCb->m.am.amUl.staProhTmrInt = entCfg->m.amInfo.ul.staProhTmr;
            rbCb->m.am.amUl.reOrdTmrInt = entCfg->m.amInfo.ul.reOrdTmr;
            cmInitTimers(&(rbCb->m.am.amUl.reOrdTmr), 1);
            cmInitTimers(&(rbCb->m.am.amUl.staProhTmr), 1);
            rbCb->m.am.amUl.vrMr = rbCb->m.am.amUl.vrR + KW_AM_WIN_SZ;

            ueCb->lCh[rbCb->lch[0].lChId - 1].dlRbCb = rbCb;
            ueCb->lCh[rbCb->lch[0].lChId - 1].ulRbCb = rbCb;

            winLen = KW_AM_WIN_SZ << 1;
            KW_ALLOC(rbCb->m.am.amDl.txBuf,
                  (winLen * sizeof(KwTx*)));
#if (ERRCLASS & ERRCLS_ADD_RES)
            if (rbCb->m.am.amDl.txBuf == NULLP)
            {
               ret = RFAILED;
               KWLOGERROR(ERRCLS_ADD_RES, EKW021, (ErrVal) ret,
                     "kwCfgFillRbCb: AM TXBUF allocation Failed.");
            }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

            KW_ALLOC(rbCb->m.am.amUl.recBuf,
                  (winLen * sizeof(KwAmRecBuf *)));
#if (ERRCLASS & ERRCLS_ADD_RES)
            if (rbCb->m.am.amUl.recBuf == NULLP)
            {
               KW_FREE(rbCb->m.am.amDl.txBuf, (winLen * sizeof(KwTx*)));
               ret = RFAILED;
               KWLOGERROR(ERRCLS_ADD_RES, EKW022, (ErrVal) ret,
                     "kwCfgFillRbCb: AM RECBUF allocation Failed.");
            }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

            break;
         }
      default:
         {
            KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
             "kwCfgFillRbCb Failed -- for the invalid mode(%d)\n",
             entCfg->entMode));
         }
   }
   
   /* kw005.201 removed numLc from statistics */

   RETVALUE(ret);
} /* kwCfgFillRbCb */


/** @details
 * This primitive Initializes the RB Cb
 *
 * @param [out]   rbCb     -  RB Control Block
 * @param [in]    ueCb     -  UE Control Block
 * @param [in]    entCfg   -  Entity Configuration
 *
 * @return  ROK
 */
#ifdef ANSI
PRIVATE S16 kwCfgUpdateRb
(
KwRbCb            *rbCb,
KwCellCb          *cellCb,
KwUeCb            *ueCb,
CkwEntCfgInfo     *entCfg
)
#else
PRIVATE S16 kwCfgUpdateRb(rbCb, cellCb, ueCb, entCfg)
KwRbCb            *rbCb;
KwCellCb          *cellCb;
KwUeCb            *ueCb;
CkwEntCfgInfo     *entCfg;
#endif
{
   TRC3(kwCfgUpdateRb)


   if (rbCb->mode != entCfg->entMode)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
       "kwCfgUpdateRb():Failed--rbId(%d), rbType(%d), cellId(%d), ueId(%d)\n",\
        rbCb->rlcId.rbId, rbCb->rlcId.rbType ,rbCb->rlcId.cellId, 
        rbCb->rlcId.ueId));
#endif
      RETVALUE(CKW_CFG_REAS_RB_MODE_MIS);
   }

   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
         {
            switch (rbCb->dir)
            {
               case KW_DIR_DL:
                  {
                     cellCb->lCh[rbCb->lch[0].lChId - 1].dlRbCb = NULLP;
                     break;
                  }
               case KW_DIR_UL:
                  {
                     cellCb->lCh[rbCb->lch[0].lChId - 1].ulRbCb = NULLP;
                     break;
                  }
            }

            rbCb->dir = entCfg->dir;
            rbCb->lch[0].lChId = entCfg->lCh[0].lChId;
            rbCb->lch[0].lChType = entCfg->lCh[0].type;

            switch (entCfg->dir)
            {
               case KW_DIR_DL:
                  {
                     cellCb->lCh[rbCb->lch[0].lChId - 1].dlRbCb = rbCb;
                     break;
                  }
               case KW_DIR_UL:
                  {
                     cellCb->lCh[rbCb->lch[0].lChId - 1].ulRbCb = rbCb;
                     break;
                  }
            }
            break;
         }

      case CM_LTE_MODE_UM:
         {
            if (entCfg->lCh[0].type == CM_LTE_LCH_DCCH)
            {
               RETVALUE(CKW_CFG_REAS_LCHTYPE_MIS);
            }

            /* If direciton is not both then re-configure
             * the corresponding rbCb.
             */
            if (rbCb->dir != KW_DIR_BOTH)
            {
               if (rbCb->dir == KW_DIR_DL)
               {
                  ueCb->lCh[rbCb->lch[0].lChId - 1].dlRbCb = NULLP;

                  if (entCfg->dir == KW_DIR_DL)
                  {
                     ueCb->lCh[entCfg->lCh[0].lChId - 1].dlRbCb = rbCb;
                  }
                  else
                  {
                     /* Free the Buffers of RbCb */
                     kwUtlFreeRbCb(rbCb);

                     if (kwCfgFillRbCb(rbCb, ueCb, entCfg) != ROK)
                     {
                        RETVALUE(CKW_CFG_REAS_RECFG_FAIL);
                     }
                  }
               }
               else
               {
                  ueCb->lCh[rbCb->lch[0].lChId - 1].ulRbCb = NULLP;

                  if (entCfg->dir == KW_DIR_DL)
                  {
                     /* Free the Buffers of RbCb */
                     kwUtlFreeRbCb(rbCb);

                     if (kwCfgFillRbCb(rbCb, ueCb, entCfg) != ROK)
                     {
                        RETVALUE(CKW_CFG_REAS_RECFG_FAIL);
                     }
                  }
                  else
                  {
                     ueCb->lCh[entCfg->lCh[0].lChId - 1].ulRbCb = rbCb;
                  }
               }

               rbCb->lch[0].lChId = entCfg->lCh[0].lChId;
               rbCb->lch[0].lChType = entCfg->lCh[0].type;
               rbCb->dir = entCfg->dir;
            }
            else
            {
               RETVALUE(CKW_CFG_REAS_RECFG_FAIL);
            }

            break;
         }

      case CM_LTE_MODE_AM:
         {
            /*We Won't modify the LCH ID */
            /*ueCb->lCh[rbCb->lch[0].lChId - 1].dlRbCb = NULLP;
            ueCb->lCh[rbCb->lch[1].lChId - 1].ulRbCb = NULLP;

            ueCb->lCh[entCfg->lCh[0].lChId - 1].dlRbCb = rbCb;
            ueCb->lCh[entCfg->lCh[0].lChId - 1].ulRbCb = rbCb; */

            /* Down Link */
            /*We Won't Change the LCH ID */
            /*rbCb->lch[0].lChId = entCfg->lCh[0].lChId;
            rbCb->lch[0].lChType = entCfg->lCh[0].type;*/
            rbCb->m.am.amDl.pollRetxTmrInt = entCfg->m.amInfo.dl.pollRetxTmr;
            rbCb->m.am.amDl.pollPdu = entCfg->m.amInfo.dl.pollPdu;
            rbCb->m.am.amDl.pollByte = entCfg->m.amInfo.dl.pollByte;
            rbCb->m.am.amDl.maxRetx = entCfg->m.amInfo.dl.maxRetx;

            /* Up Link */
	    /*We Won't Change the LCHI ID*/
            /*rbCb->lch[1].lChId = entCfg->lCh[0].lChId; 
            rbCb->lch[1].lChType = entCfg->lCh[0].type;*/

            rbCb->m.am.amUl.staProhTmrInt = entCfg->m.amInfo.ul.staProhTmr;
            rbCb->m.am.amUl.reOrdTmrInt = entCfg->m.amInfo.ul.reOrdTmr;

            break;
         }
   }

   RETVALUE(CKW_CFG_REAS_NONE);
} /* kwCfgUpdateRb */


/** @details
 * This primitive adds new RB in Ue/Cell Cb.
 *
 * - If UE ID is 0 then
 *   - Check for CELL CB is present
 *   - If yes, Check for RB ID
 *     - If RB ID is present Status Indication with reason
 *     - Else, Create New RB CB in CELL CB
 *   - If no Create New CELL CB and RB CB
 * - Else,
 *   - Check for UE CB is present
 *   - If yes Check for RB ID
 *     - If RB ID is present Status Indication with reason
 *     - Else, Create New RB CB in UE CB
 *   - If no Create New UE CB and RB CB
 * - Fill entity confirmation
 *
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation.
 *
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 kwCfgAddRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CkwEntCfgInfo     *entCfg,
CkwEntCfgCfmInfo  *entCfm
)
#else
PUBLIC S16 kwCfgAddRb(ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CkwEntCfgInfo     *entCfg;
CkwEntCfgCfmInfo  *entCfm;
#endif
{
   S16                  ret;           /* Return Value */
   KwUeCb               *ueCb;         /* UE Control Block */
   KwCellCb             *cellCb;       /* Cell Control Block */
   RbCb                 *rbCb;         /* RB Control Block */
   KwRbCb               *kwRbCb;       /* KW RB Control Block */
   U8                   rbId;          /* Rb Identifier */
   U8                   rbType;        /* RB type */
   U8                   reason;        /* Rb Identifier */

   TRC3(kwCfgAddRb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwCfgAddRb(ueId(%d), cellId(%d), cfgType(%d))\n",\
            ueId, cellId, entCfg->cfgType));
#endif

   ret = ROK;
   rbCb = NULLP;
   ueCb = NULLP;
   cellCb = NULLP;
   reason = 0;

   /* Get rbId */
   rbId = entCfg->rbId;
   rbType = entCfg->rbType;

   MSPD_DBG_RLC("Entering kwCfgAddRb\n");

   if (cellId == 0)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_CELL_UNKWN);

      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgAddRb(ueId(%d), cellId(%d), cfgType(%d))\n",\
               ueId, cellId, entCfg->cfgType));

      RETVALUE(RFAILED);
   }

   /* Validate the RBID in case of TM/UM/AM */
   KW_VALIDATE_RBID(ueId, rbId, rbType, ret);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_RB_UNKWN);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgAddRb -- KW_VALIDATE_RBID Failed --\
               rbId(%d), rbType(%d), cellId(%d), ueId(%d)\n",\
              rbId, rbType, cellId, ueId));
#endif

      RETVALUE(ret);
   }

   /* Process Adding new RB */
   if (ueId == 0)
   {
      if (((entCfg->lCh[0].type == CM_LTE_LCH_BCCH) ||
            (entCfg->lCh[0].type == CM_LTE_LCH_PCCH) ||
            (entCfg->lCh[0].type == CM_LTE_LCH_CCCH)) &&
           (entCfg->entMode == CM_LTE_MODE_TM))
      {
         /* Cell CB present */
         if ( kwDbmFetchCellCb(cellId, &cellCb) == ROK)
         {
            /* Get rbCb from cellCb->rbCb List */
            KW_DBM_FETCH_CELL_RBCB(rbId, cellCb->rbCb, rbCb);

            if (( rbCb != NULLP)
#ifdef KW_PDCP
              &&((rbCb->rbCfgStat | PJ_CFG_RLC) == PJ_CFG_RLC)
#endif /* KW_PDCP */
               )
            {
               /* Fill entCfm structure */
               KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
                     CKW_CFG_REAS_RB_PRSNT);

               KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                        "kwCfgAddRb -- kwDbmFetchRbCb - RB CB present --\
                        rbId(%d), cellId(%d)\n",\
                        rbId, rbCb->kwRbCb.rlcId.cellId));
               RETVALUE(RFAILED);
            }
         }
         else  /* Cell CB UNKNOWN */
         {
            /* Create CELL CB */
            if ( kwDbmCreateCellCb(cellId, &cellCb) != ROK)
            {
               /* Fill entCfm structure */
               KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
                     CKW_CFG_REAS_CELL_CREAT_FAIL);

               KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                        "kwCfgAddRb -- kwDbmCreateCellCb Failed -- cellId(%d)\
                        \n", cellId));
               RETVALUE(RFAILED);
            }
         }

         /* Validate LChId */
         KW_VALIDATE_LCHID(entCfg->lCh[0].lChId);

         /* Create RB CB */
         KW_DBM_CREATE_CELL_RBCB(rbId, cellCb->rbCb, rbCb);

         switch (entCfg->dir)
         {
            case KW_DIR_DL:
               {
                  cellCb->lCh[entCfg->lCh[0].lChId - 1].dlRbCb = &rbCb->kwRbCb;
                  break;
               }
            case KW_DIR_UL:
               {
                  cellCb->lCh[entCfg->lCh[0].lChId - 1].ulRbCb = &rbCb->kwRbCb;
                  break;
               }
            default:
               {
                  KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                           "kwCfgAddRb -- Wrong dir((%d)\n", entCfg->dir));
                  KW_FREE(rbCb, sizeof(RbCb));

                  /* Fill entCfm structure */
                  KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
                        CKW_CFG_REAS_INVALID_DIR);

                  RETVALUE(RFAILED);
               }
         }
      }
      else
      {
         if (entCfg->entMode != CM_LTE_MODE_TM)
         {
            reason = CKW_CFG_REAS_RB_MODE_MIS;
         }
         else
         {
            reason = CKW_CFG_REAS_LCHTYPE_MIS;
         }

         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK, reason);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if ((((entCfg->lCh[0].type == CM_LTE_LCH_DCCH) && (entCfg->entMode != CM_LTE_MODE_UM)) ||
            (entCfg->lCh[0].type == CM_LTE_LCH_DTCH)) &&
          (entCfg->entMode != CM_LTE_MODE_TM))
      {
         MSPD_DBG_RLC("Calling kwDbmFetchAndCreateUeCb\n");
         ret = kwDbmFetchAndCreateUeCb(ueId, cellId, &ueCb);
         if( ROK != ret)
         {
            RETVALUE(RFAILED);
         }

         /* Validate LChId for UM and AM modes */
         KW_VALIDATE_LCHID(entCfg->lCh[0].lChId);
         if (entCfg->entMode == CM_LTE_MODE_AM)
         {
            KW_VALIDATE_LCHID(entCfg->lCh[1].lChId);
         }
         /* Create RBCB */
         ret = kwDbmFetchAndCreateRbCb(rbId, rbType, ueCb, &rbCb, PJ_CFG_RLC);
         if( ROK != ret)
         {
            RETVALUE(ret);
         }

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
         rbCb->kwRbCb.qci = entCfg->qci;
         ret = cmHashListInsert(&(kwCb.kwL2Cb.qciHlCp), (PTR)&rbCb->kwRbCb, 
                 (U8 *)&(rbCb->kwRbCb.qci), (U16) sizeof(rbCb->kwRbCb.qci));
         if(ret != ROK)
         {
            KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
            "Failed adding rbCb in qciList(ueId(%d), cellId(%d), qci(%d))\n",\
            ueId, cellId, entCfg->qci));
            RETVALUE(RFAILED);
         }
         rbCb->kwRbCb.ueCb =  ueCb;
#endif /* LTE_L2_MEAS */
      }
      else
      {
         if (entCfg->entMode == CM_LTE_MODE_TM)
         {
            reason = CKW_CFG_REAS_RB_MODE_MIS;
         }
         else
         {
            reason = CKW_CFG_REAS_LCHTYPE_MIS;
         }

         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK, reason);
         RETVALUE(RFAILED);
      }
   }


   kwRbCb = &rbCb->kwRbCb;

   kwRbCb->rlcId.cellId = cellId;
   kwRbCb->rlcId.ueId   = ueId;
   kwRbCb->rlcId.rbType = rbType;


   /* Fill RB CB */
   if (kwCfgFillRbCb(kwRbCb, ueCb, entCfg) != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               CKW_CFG_REAS_RB_CREAT_FAIL);

      /* Delete RB CB created */
      KW_FREE(rbCb, sizeof(RbCb));

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgAddRb -- kwCfgFillRbCb Failed -- cellId(%d), \
               ueId(%d), rbId(%d), rbType(%d)\n", cellId, ueId, rbId, rbType));
#endif

      RETVALUE(RFAILED);
   }

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} /* kwCfgAddRb */


/** @details
 * This primitive reconfigures the existing RB in Ue/Cell Cb.
 *
 * - If UE ID is 0 then
 *   - Check for CELL CB is present
 *   - If yes, Check for RB ID
 *     - If RB ID is prenset Reconfigure the RB CB
 *     - Else, Status Indication with Reason
 *   - Else, Status Indication with Reason
 * - Else,
 *   - Check for UE CB is present
 *   - If yes, Check for RB ID
 *     - If RB ID is prenset Reconfigure the CELL CB
 *     - Else, Status Indication with Reason
 *   - Else, Status Indication with Reason
 * - Fill entity confirmation
 *
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 kwCfgReCfgRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CkwEntCfgInfo     *entCfg,
CkwEntCfgCfmInfo  *entCfm
)
#else
PUBLIC S16 kwCfgReCfgRb(ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CkwEntCfgInfo     *entCfg;
CkwEntCfgCfmInfo  *entCfm;
#endif
{
   S16                  ret;           /* Return value */
   RbCb                 *rbCb;         /* RB Control Block */
   KwRbCb               tRbCb;         /* KW RB Control Block */
   KwCellCb             *cellCb;       /* Cell Control Block */
   KwUeCb               *ueCb;         /* Ue Control Block */
   U8                   rbId;          /* RB Identifier */
   U8                   rbType;        /* RB Type */

   TRC3(kwCfgReCfgRb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwCfgReCfgRb(ueId(%d), cellId(%d), cfgType(%d))\n",\
            ueId, cellId, entCfg->cfgType));
#endif

   ret = ROK;
   rbCb = NULLP;
   cellCb = NULLP;
   ueCb = NULLP;
   rbId = entCfg->rbId;
   rbType = entCfg->rbType;

   KW_MEM_ZERO(&tRbCb, sizeof(KwRbCb));

   /* Validate the RBID in case of TM/UM/AM */
   KW_VALIDATE_RBID(ueId, rbId, rbType, ret);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_RB_UNKWN);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgReCfgRb -- KW_VALIDATE_RBID Failed --\
               rbId(%d), rbType(%d), cellId(%d), ueId(%d)\n",\
              rbId, rbType, cellId, ueId));
#endif
      RETVALUE(ret);
   }

   /* Check for UeCb or CellCb */
   if (ueId == 0)
   {
      /* Get cellCb */
      ret = kwDbmFetchCellCb(cellId, &cellCb);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               CKW_CFG_REAS_CELL_UNKWN);

#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                  "kwCfgReCfgRb -- kwDbmFetchCellCb Failed -- cellId(%d), \
                  rbId(%d)\n", cellId, rbId));
#endif
         RETVALUE(ret);
      }

      /* Get rbCb */
      KW_DBM_FETCH_CELL_RBCB(rbId, cellCb->rbCb, rbCb);

      if ( rbCb == NULLP)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               CKW_CFG_REAS_RB_UNKWN);

#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                  "kwCfgReCfgRb -- kwDbmFetchRbCb Failed -- cellId(%d), \
                  rbId(%d)\n", cellId, rbId));
#endif
         RETVALUE(ret);
      }

      /* Take backup of rbCb before updating.
       * Because in failure case restore original rbCb
       */
      cmMemcpy((U8 *)&tRbCb, (U8 *)&rbCb->kwRbCb, sizeof(KwRbCb));

      /* Update rbCb */
      /* kw006.201 Adding comment, ccpu00120058 */
      /* Since UeId is 0, so UeCb will be NULL and RB mode will be TM mode */
      ret = kwCfgUpdateRb(&rbCb->kwRbCb, cellCb, NULLP, entCfg);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               ret);

#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                  "kwCfgReCfgRb -- kwCfgUpdateRb Failed --\
                  rbId(%d), cellId(%d)\n",\
                  rbId, cellId));
#endif

         cmMemcpy((U8*)&rbCb->kwRbCb, (U8 *)&tRbCb, sizeof(KwRbCb));

         RETVALUE(ret);
      }
   }
   else
   {
      /* Get ueCb */
      ret = kwDbmFetchUeCb(ueId, cellId, &ueCb);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_UE_UNKWN);

#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwCfgReCfgRb -- kwDbmFetchUeCb Failed -- ueId(%d),\
            cellId(%d), rbId(%d)\n", ueId, cellId, rbId));
#endif
         RETVALUE(ret);
      }

      /* Get rbCb */
      KW_DBM_FETCH_RBCB(rbId, rbType, ueCb, rbCb);

      if ( rbCb == NULLP)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);

#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwCfgReCfgRb -- kwDbmFetchRbCb Failed -- ueId(%d),\
            cellId(%d), rbId(%d)\n", ueId, cellId, rbId));
#endif
         RETVALUE(ret);
      }

      /* Take backup of rbCb before updating.
       * Because in failure case restore original rbCb
       */
      cmMemcpy((U8 *)&tRbCb, (U8 *)&rbCb->kwRbCb, sizeof(KwRbCb));

      /* Update rbCb */
      /* kw006.201 Adding comment, ccpu00120058 */
      /*Since UeId is valid, no need for cellCb and RB mode will be AM/UM mode*/
      ret = kwCfgUpdateRb(&rbCb->kwRbCb, NULLP, ueCb, entCfg);
      if (ret != CKW_CFG_REAS_NONE)
      {
         /* Fill entCfm structure */
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               ret);

#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                  "kwCfgReCfgRb -- kwCfgUpdateRb Failed --\
                  rbId(%d), cellId(%d), ueId(%d)\n",\
                  rbId, cellId, ueId));
#endif
         cmMemcpy((U8*)&rbCb->kwRbCb, (U8 *)&tRbCb, sizeof(KwRbCb));

         RETVALUE(ret);
      }
   }

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} /* kwCfgReCfgRb */

/** @details
 * This primitive checks whether the RbCb is present in CellCb or UeCb.
 * If yes, then it resets the corresponding entry in CellCb/UeCb to NULL.
 * The actual freeing of RbCb pointer is done from PDCP context in core 1.
 *
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 kwCfgDelRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CkwEntCfgInfo     *entCfg,
CkwEntCfgCfmInfo  *entCfm
)
#else
PUBLIC S16 kwCfgDelRb(ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CkwEntCfgInfo     *entCfg;
CkwEntCfgCfmInfo  *entCfm;
#endif
{
   S16                  ret;           /* Return Value */
   CmLteRlcId           rlcId;         /* RLC Identifier */
   KwUeCb               *ueCb;         /* UE Control Block */
   KwCellCb             *cellCb;       /* UE Control Block */
   RbCb                 *rbCb;         /* RB Control Block */
   KwRbCb               *kwRbCb;       /* KW RB Control Block */
   U8                   rbId;          /* RB Identifier */
   U8                   rbType;        /* RB type */

   TRC3(kwCfgDelRb)

   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwCfgDelRb(ueId(%d), cellId(%d), cfgType(%d))\n",\
            ueId, cellId, entCfg->cfgType));
   MSPD_DBG("[%d][%d][%d] Received RB delete request in core 2\n", 
             ueId, entCfg->rbType, entCfg->rbId);

   ret = ROK;
   rbId = entCfg->rbId;
   rbType = entCfg->rbType;
   rbCb = NULLP;
   kwRbCb = NULLP;
   ueCb = NULLP;
   cellCb = NULLP;

   /* Validate the RBID in case of TM/UM/AM */
   KW_VALIDATE_RBID(ueId, rbId, rbType, ret);
   if (ret != ROK)
   {
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_RB_UNKWN);

      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgReEstRb -- KW_VALIDATE_RBID Failed --\
               rbId(%d), rbType(%d), cellId(%d), ueId(%d)\n",\
               rbId, rbType, cellId, ueId));

      RETVALUE(ret);
   }

   /* Get cellCb and delete rbCb from it */
   if (ueId == 0)
   {
      /* Get rbId */
      KW_MEM_ZERO(&rlcId, sizeof(CmLteRlcId));
      rlcId.ueId = ueId;
      rlcId.cellId = cellId;
      rlcId.rbId = entCfg->rbId;

      /* Get cellCb */
      ret = kwDbmFetchCellCb(cellId, &cellCb);
      if (ret != ROK)
      {
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               CKW_CFG_REAS_RB_UNKWN);

         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwCfgDelRb -- kwDbmFetchCellCb - RB CB not present --\
                  rbId(%d), cellId(%d)\n", rbId, cellId));

         RETVALUE(ret);
      }

      KW_DBM_FETCH_CELL_RBCB(rbId, cellCb->rbCb, rbCb);

      if ( rbCb == NULLP)
      {
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               CKW_CFG_REAS_RB_UNKWN);

         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwCfgDelRb -- kwDbmFetchRbCb - RB CB present --\
                  rbId(%d), cellId(%d)\n", rbId, cellId));

         RETVALUE(ret);
      }

      cellCb->rbCb[rbId] = NULLP;

      /* Assign NULLP to dlRbCb/ulRbCb.
       * Delete Hashlist allocated for it if any */
      kwRbCb = &rbCb->kwRbCb;
      switch (kwRbCb->dir)
      {
         case KW_DIR_DL:
            {
               cellCb->lCh[kwRbCb->lch[0].lChId].dlRbCb = NULLP;
               break;
            }
         case KW_DIR_UL:
            {
               cellCb->lCh[kwRbCb->lch[0].lChId].ulRbCb = NULLP;
               break;
            }
      }
   }
   else
   {
      ret = kwDbmFetchUeCb(ueId, cellId, &ueCb);
      if (ret != ROK)
      {
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               CKW_CFG_REAS_UE_UNKWN);

         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwCfgDelRb -- kwDbmFetchRbCb - RB CB present --\
                  rbId(%d), ueId(%d), cellId(%d)\n", rbId, ueId, cellId));

         RETVALUE(ret);
      }

      KW_DBM_FETCH_RBCB(rbId, rbType, ueCb, rbCb);

      if ( rbCb == NULLP)
      {
         KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
               CKW_CFG_REAS_RB_UNKWN);

         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwCfgDelRb -- kwDbmFetchRbCb - RB CB present --\
                  rbId(%d), ueId(%d), cellId(%d)\n", rbId, ueId, cellId));

         RETVALUE(ret);
      }

      kwRbCb = &rbCb->kwRbCb;
      /* Delete Logical channel corresponding to rbCb */
      switch (kwRbCb->mode)
      {
         case CM_LTE_MODE_UM:
            {
               switch (kwRbCb->dir)
               {
                  case KW_DIR_DL:
                     {
                        ueCb->lCh[kwRbCb->lch[0].lChId].dlRbCb = NULLP;
                        break;
                     }
                  case KW_DIR_UL:
                     {
                        ueCb->lCh[kwRbCb->lch[0].lChId].ulRbCb = NULLP;
                        break;
                     }
                  case KW_DIR_BOTH:
                     {
                        ueCb->lCh[kwRbCb->lch[0].lChId].dlRbCb = NULLP;
                        ueCb->lCh[kwRbCb->lch[0].lChId].ulRbCb = NULLP;
                        break;
                     }
               }
               break;
            }

         case CM_LTE_MODE_AM:
            {
               ueCb->lCh[kwRbCb->lch[0].lChId].dlRbCb = NULLP;
               ueCb->lCh[kwRbCb->lch[1].lChId].ulRbCb = NULLP;
		       kwAmmStopRbTmrs(kwRbCb);
		       kwUtlAddRbToPjDelRbLst(kwRbCb);
		       kwUtlPostPjDlRbSelfMsg();
               break;
            }

         default:
            {
               KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_NOK,\
                     CKW_CFG_REAS_UE_UNKWN);

               KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
                        "kwCfgDelRb -- RLC entity Mode is wrong --\
                        rbId(%d), ueId(%d), cellId(%d), mode(%d).\n",
                        rbId, ueId, cellId, kwRbCb->mode));

               RETVALUE(ret);
            }
      }
      if(rbType == CM_LTE_SRB)
      {
         ueCb->srbCb[rbId] = NULLP;
      }
      else
      {
         ueCb->drbCb[rbId] = NULLP;
      }

   }

   /* [ccpu00127171] The code for freeing the RbCb has been removed from here */

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

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, rbId, rbType, CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ret);
} /* kwCfgDelRb */

/** @details
 * This primitive re-establish the existing RB in Ue/Cell Cb.
 *
 * - If UE ID is 0 then
 *   - Check for CELL CB is present
 *   - If yes, Check for RB ID
 *     - If RB ID is prenset initialize the parameters of the RB CB
 *     - Else, Status Indication with Reason
 *   - Else, Status Indication with Reason
 * - Else,
 *   - Check for UE CB is present
 *   - If yes, Check for RB ID
 *     - If RB ID is prenset initialize the parameters of the RB CB
 *     - Else, Status Indication with Reason
 *   - Else, Status Indication with Reason
 * - Fill entity confirmation
 *
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 kwCfgReEstRb
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CkwEntCfgInfo     *entCfg,
CkwEntCfgCfmInfo  *entCfm
)
#else
PUBLIC S16 kwCfgReEstRb(ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CkwEntCfgInfo     *entCfg;
CkwEntCfgCfmInfo  *entCfm;
#endif
{
   S16                  ret;           /* Return Value */
   KwRbCb               *rbCb;         /* RB Control Block */
   CmLteRlcId           rlcId;         /* RLC Identifier */

   TRC3(kwCfgReEstRb)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwCfgReEstRb(ueId(%d), cellId(%d), cfgType(%d))\n",\
            ueId, cellId, entCfg->cfgType));
#endif

   /* Get rlcId */
   KW_MEM_ZERO(&rlcId, sizeof(CmLteRlcId));
   rlcId.ueId = ueId;
   rlcId.cellId = cellId;
   rlcId.rbId = entCfg->rbId;
   rlcId.rbType = entCfg->rbType;
   rbCb = NULLP;
   ret = ROK;

   /* Validate the RBID in case of TM/UM/AM */
   KW_VALIDATE_RBID(ueId, rlcId.rbId, rlcId.rbType, ret);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, rlcId.rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_RB_UNKWN);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgReEstRb -- KW_VALIDATE_RBID Failed --\
               rbId(%d), cellId(%d), ueId(%d)\n",\
               rlcId.rbId, rlcId.cellId, rlcId.ueId));
#endif

      RETVALUE(ret);
   }

   ret = kwDbmFetchRbCbForUi(rlcId, &rbCb);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, rlcId.rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_RB_UNKWN);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgReEstRb -- kwDbmFetchRbCbForUi Failed --\
               rbId(%d), rbType(%d), cellId(%d), ueId(%d)\n",\
               rlcId.rbId, rlcId.rbType, rlcId.cellId, rlcId.ueId));
#endif
      RETVALUE(ret);
   }

   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
         {
            ret = kwTmmReEstablish(rbCb);
            break;
         }

      case CM_LTE_MODE_UM:
         {
            ret = kwUmmReEstablish(rlcId, rbCb);
            break;
         }

      case CM_LTE_MODE_AM:
         {
            ret = kwAmmReEstablish(rbCb);
            break;
         }
   }
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, rlcId.rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_REEST_FAIL);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
            "kwCfgReEstRb Failed -- Mode(%d) rbId(%d), cellId(%d), ueId(%d)\n",\
             rbCb->mode, rlcId.rbId, rlcId.cellId, rlcId.ueId));
#endif

      RETVALUE(ret);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* Fill entCfm structure */
   KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,\
      CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} /* kwCfgReEstRb */

/** @details
 *    This is a dummy function. Entire UE delete processing will get done by
 *    PDCP CFG.
 *
 * @description
 *   Returns ROK if ueId is non-zero.
 *
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 kwCfgDelUe
(
CmLteRnti         ueId,
CmLteCellId       cellId,
CkwEntCfgInfo     *entCfg,
CkwEntCfgCfmInfo  *entCfm
)
#else
PUBLIC S16 kwCfgDelUe(ueId, cellId, entCfg, entCfm)
CmLteRnti         ueId;
CmLteCellId       cellId;
CkwEntCfgInfo     *entCfg;
CkwEntCfgCfmInfo  *entCfm;
#endif
{
   S16            ret;                 /* Return Value */
   KwUeCb         *ueCb;               /* UE Control Block */
#ifdef MSPD_MLOG_NEW
   U32            t, t2;
#endif
   U8             idx;
   TRC3(kwCfgDelUe)
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwCfgDelUe(ueId(%d), cellId(%d), cfgType(%d))\n",\
            ueId, cellId, entCfg->cfgType));
   MSPD_DBG("[%d] Received UE delete\n", ueId);

   ret = ROK;
   ueCb = NULLP;
#ifdef MSPD_MLOG_NEW
   t=GetTIMETICK();
#endif
   /* Check for ueId is present or not */
   if ( ueId == 0 )
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,\
          CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);

      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwCfgDelUe -- UeId is 0  -- cellId(%d)\n", cellId));

      RETVALUE(RFAILED);
   }
   /* Fetch Ue Cb */
   ret = kwDbmFetchUeCb(ueId, cellId, &ueCb);

   /* Ue Cb is not present  */
   if( ROK != ret || NULLP == ueCb)
   { 
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,\
      CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);

      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgDelUe -- kwDbmDelUeCb - Failed --\
               cellId(%d), ueId(%d)\n", cellId, ueId));
      MSPD_DBG("UeCb NOt Found\n");
      RETVALUE(ret);
   }

   /* MSPD_FIX: Removed fetching of UeCb since PDCP CFG will take care */
   /* during UE delete.                                                */

   /* All the deletion work will get done by PDCP in core 1. So, no need to */
   /* to call DBM delete from here. It will get called if PDCP is undefined */
#ifndef KW_PDCP
   ret = kwDbmDelUeCb(ueCb, FALSE);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,\
      CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);

      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgDelUe -- kwDbmDelUeCb - Failed --\
               cellId(%d), ueId(%d)\n", cellId, ueId));
      RETVALUE(RFAILED);
   }
#endif /* KW_PDCP */
   /* Delete all logical channels */
#ifdef MSPD
   ysMsAmmDelUeReqRcvd++;
#endif

   for (idx = 0; idx < KW_MAX_LCH_PER_UE; idx++)
   {
      /* kw005.201 fixed the error in the code */
      ueCb->lCh[idx].dlRbCb = NULLP;
      ueCb->lCh[idx].ulRbCb = NULLP;
   }

   for ( idx = 0; idx< KW_MAX_SRB_PER_UE; idx++)
   {
      if(ueCb->srbCb[idx] != NULLP)
      {
         kwAmmStopRbTmrs(&(ueCb->srbCb[idx]->kwRbCb));
         kwUtlAddRbToPjDelRbLst(&(ueCb->srbCb[idx]->kwRbCb));
         kwUtlPostPjDlRbSelfMsg();
         ueCb->srbCb[idx] = NULLP;
      }
   }
       

   for ( idx = 0; idx< KW_MAX_DRB_PER_UE; idx++)   
   {
      if(ueCb->drbCb[idx] != NULLP)
      {
         /* MS_FIX : Separate processing for Timer based on mode */	      
         if (ueCb->drbCb[idx]->kwRbCb.mode == CM_LTE_MODE_AM)
	     {		 
            kwAmmStopRbTmrs(&(ueCb->drbCb[idx]->kwRbCb));
	     }
	     else if (ueCb->drbCb[idx]->kwRbCb.mode == CM_LTE_MODE_UM)
	     {
       	    kwUmmStopRbTmrs(&(ueCb->drbCb[idx]->kwRbCb));
	     }
         kwUtlAddRbToPjDelRbLst(&(ueCb->drbCb[idx]->kwRbCb));
         kwUtlPostPjDlRbSelfMsg();
         ueCb->drbCb[idx] = NULLP;
      }
   }
#ifdef PDCP
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
            KW_FREE(ueCb->hoInfo->hoCfmInfo[idx].ul.ulBitMap,
                  (ueCb->hoInfo->hoCfmInfo[idx].ul.numBits % 8)?
                  ((ueCb->hoInfo->hoCfmInfo[idx].ul.numBits / 8) + 1):
                  (ueCb->hoInfo->hoCfmInfo[idx].ul.numBits / 8 ));
         }
      }

      KW_FREE(ueCb->hoInfo->hoCfmInfo, (KW_MAX_DRB_PER_UE * sizeof(PjHoCfmInfo)));
      KW_FREE(ueCb->hoInfo, sizeof(PjHoInfo));  
   }
#endif  
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
   
 
   KW_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,\
      CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_RLC_AM_RLC_UE_DEL, RESOURCE_LARM, t, t2=GetTIMETICK());
#endif
   RETVALUE(ROK);
} /* kwCfgDelUe */


/**
 * This primitive deletes the RBs in Ue Cb.
 *
 * - If CELL ID is 0 then
 *   - Status Indication with Reason
 * - Else,
 *   - Check for CELL CB is present
 *   - If yes, Delete all RB CB in CELL CB and Delete CELL CB also.
 *   - Else, Status Indication with Reason
 * - Fill entity confirmation
 *
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 kwCfgDelCell
(
CmLteCellId       cellId,
CkwEntCfgInfo     *entCfg,
CkwEntCfgCfmInfo  *entCfm
)
#else
PUBLIC S16 kwCfgDelCell(cellId, entCfg, entCfm)
CmLteCellId       cellId;
CkwEntCfgInfo     *entCfg;
CkwEntCfgCfmInfo  *entCfm;
#endif
{
   S16            ret;                 /* Return Value */
   KwCellCb       *cellCb;             /* UE Control Block */
   U8             rbId;                /* RB Identifier */

   TRC3(kwCfgDelCell)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,\
            "kwCfgDelCell( cellId(%d), cfgType(%d))\n",\
            cellId, entCfg->cfgType));
#endif

   ret = ROK;
   cellCb = NULLP;
   rbId = entCfg->rbId;

   /* Check for ueId is present or not */
   if ( cellId == 0 )
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_CELL_UNKWN);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
               "kwCfgDelCell Failed -- cellId is 0  -- cellId(%d)\n", cellId));
#endif

      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   ret = kwDbmFetchCellCb(cellId, &cellCb);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_CELL_UNKWN);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,
            "kwCfgDelCell -- kwDbmFetchCellCb - Failed -- cellId(%d)\n",
            cellId));
#endif
      RETVALUE(RFAILED);
   }

   /* Delete Ue Cb */
   ret = kwDbmDelCellCb(cellCb);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      KW_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_NOK,\
            CKW_CFG_REAS_CELL_UNKWN);

#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgDelCell -- kwDbmDelCellCb - Failed --\
               cellId(%d)\n", cellId));
#endif
      RETVALUE(RFAILED);
   }

   /* Fill entCfm structure */
  /* kw005.201 added support for L2 Measurement */         
   KW_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_OK,\
                       CKW_CFG_REAS_NONE);

   RETVALUE(ROK);
} /* kwCfgDelCell */

/**
 * This primitive changes the ueId of Ue Cb.
 *
 * - If oldUeId and newUeId are
 *   - Confirm the Status with Reason
 * - If UeId not present
 *   - Confirm the Status with Reason
 * - Create New UeCb
 * - Copy rbCbs from old UeCb to new UeCb
 * - Delete old UeCb
 * - Fill entity confirmation
 *
 * @param [in]    ueInfo      -  Old UE Information
 * @param [in]    newUeInfo   -  New UE Information
 * @param [out]   status      -  Status
 *
 * @return  ROK
 */
#ifdef ANSI
PUBLIC S16 kwCfgUeIdChng
(
CkwUeInfo *ueInfo,
CkwUeInfo *newUeInfo,
CmStatus  *status
)
#else
PUBLIC S16 kwCfgUeIdChng(ueInfo,newUeInfo,status)
CkwUeInfo *ueInfo;
CkwUeInfo *newUeInfo;
CmStatus  *status;
#endif
{
   KwUeCb *ueCb;
   S16    ret;
   U8     rbIdx;
   /*kw004.201 Adding of Missing Trace in LTE RLC PDCP*/
   TRC3(kwCfgUeIdChng)

   ueCb = NULLP;
   ret= ROK;

   MSPD_DBG("[%d] Received UeId Change request, new UeId %d\n", ueInfo->ueId, 
             newUeInfo->ueId);
   if ( (ueInfo->ueId == newUeInfo->ueId) && 
        (ueInfo->cellId == newUeInfo->cellId))
   {
      status->reason = CKW_CFG_REAS_SAME_UEID;
      status->status = CKW_CFG_CFM_NOK; 

      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgUeIdChng -- Failed -- cellId(%d), ueId(%d) --\
         newCellId(%d), newUeId(%d)\n", ueInfo->cellId, ueInfo->ueId,
         newUeInfo->cellId, newUeInfo->ueId));
      RETVALUE(RFAILED);
   } 
   
   ret = kwDbmFetchUeCb(newUeInfo->ueId, newUeInfo->cellId, &ueCb);
   if (ret == ROK)
   {
      /* RLC WORKAROUND For ReEst Start */
      if (ueCb->isUeIdChngDone == TRUE)
      {
         ueCb->isUeIdChngDone = FALSE;
         RETVALUE(ROK);
      } 
      /* RLC WORKAROUND For ReEst End */

      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgUeIdChng -- NewUeCb already Exists -- cellId(%d), \
               ueId(%d)\n", newUeInfo->cellId, newUeInfo->ueId));
      status->reason = CKW_CFG_REAS_UE_EXISTS;
      status->status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }
  
   ret = kwDbmFetchUeCb(ueInfo->ueId, ueInfo->cellId, &ueCb);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgUeIdChng -- kwDbmFetchUeCb Failed -- cellId(%d), \
               ueId(%d)\n", ueInfo->cellId, ueInfo->ueId));
      status->reason = CKW_CFG_REAS_UE_UNKWN;
      status->status = CKW_CFG_CFM_NOK;
      RETVALUE(RFAILED);
   }

   /* MS FIX: Added dbmLock for protecting ueLstCp */ 
   KW_RDWR_LOCK(&kwCb.dbmLock);
   {
      ret = cmHashListDelete(&(kwCb.ueLstCp), (PTR) ueCb);
      if (ret != ROK)
      {
         KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
               "kwCfgUeIdChng -- cmHashListDelete Failed -- cellId(%d), \
               ueId(%d)\n", ueInfo->cellId, ueInfo->ueId));
         status->reason = CKW_CFG_REAS_UE_CREAT_FAIL;
         status->status = CKW_CFG_CFM_NOK;
      }
      else
      {
         /* update the hash key with new values */ 
         ueCb->key.ueId = newUeInfo->ueId;
         ueCb->key.cellId = newUeInfo->cellId;

         ret = cmHashListInsert(&(kwCb.ueLstCp), (PTR)ueCb, (U8 *)&(ueCb->key),
               (U16) sizeof(KwUeKey));

         if (ret != ROK)
         {
            KWDBGP((KW_DBGMASK_CFG | KW_DBGMASK_ERROR), (_kwPBuf,\
                  "kwCfgUeIdChng -- cmHashListInsert Failed -- cellId(%d), \
                  ueId(%d)\n", newUeInfo->cellId, newUeInfo->ueId));
            status->reason = CKW_CFG_REAS_UE_CREAT_FAIL;
            status->status = CKW_CFG_CFM_NOK;
         }
      }
   }
   KW_RDWR_UNLOCK(&kwCb.dbmLock);
   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   /* RLC WORKAROUND For ReEst Start */ 
   ueCb->isUeIdChngDone = TRUE;
   for(rbIdx = 0;rbIdx < KW_MAX_SRB_PER_UE;rbIdx++)
   {
      if(ueCb->srbCb[rbIdx] != NULLP)
      {
         ueCb->srbCb[rbIdx]->kwRbCb.rlcId.ueId = ueCb->key.ueId;
      }
   }
   for(rbIdx = 0;rbIdx < KW_MAX_DRB_PER_UE;rbIdx++)
   {
      if(ueCb->drbCb[rbIdx] != NULLP)
      {
         ueCb->drbCb[rbIdx]->kwRbCb.rlcId.ueId = ueCb->key.ueId;
      }
   }
   /* RLC WORKAROUND For ReEst End */

   MSPD_DBG("[%d] UeId change is done\n", ueInfo->ueId);
   RETVALUE(ret);
} /* kwCfgUeIdChng */

/********************************************************************30**

         End of file:     gp_cfg.c@@/main/2 - Fri Nov 13 14:13:36 2009

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
/main/1      ---       gk        1. Initial release.
/main/2      ---       nm        1. LTERLC Release 2.1.
/main/3      kw004.201 av        1. Adding missing trace  
/main/4      kw005.201 ap        1. Removed numLc from statistics. 
                                 2. Added support for L2 Measurement.
                                 3. Fix for ccpu00119143. 
                       rd        4. Code updated with important Avtecs comments.
/main/5      kw006.201 rd        1. Added 2 comments in kwCfgReCfgRb.
                                 2. Added optimization change for ccpu00111726
                                    and ccpu00117290 in kwCfgDelRb.
                       ap        3. ccpu00120574, fixed the warning             
*********************************************************************91*/



