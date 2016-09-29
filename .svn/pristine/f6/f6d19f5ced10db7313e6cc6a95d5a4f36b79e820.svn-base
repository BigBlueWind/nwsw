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

     Desc:     C source code for Round Robin functions

     File:     rg_sch_rr.c

     Sid:      gk_sch_rr.c@@/main/2 - Sat Jul 30 02:21:48 2011

     Prg:     rr

**********************************************************************/

/** @file rg_sch_rr.c
@brief This module handles the round robin scheduler functionality
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rg_sch_rr.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"
#include "rg_sch_clist.x"        /* typedefs for RR Scheduler */
#include "rg_sch_rr.x"        /* typedefs for RR Scheduler */

/* local defines */
/* rg007.201 - Added missing declaration */
PRIVATE Void rgSCHRrRlsDlHqProc ARGS(( RgSchDlHqProcCb    *proc ));

/**
 * @brief Indication of DL CQI for the UE.
 *
 * @details
 *
 *     Function: rgSCHRrDlCqiInd
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDlCqiRpt        *dlCqiRpt
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlCqiInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt
)
#else
PUBLIC Void rgSCHRrDlCqiInd(cell, ue, dlCqiRpt)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDlCqiRpt        *dlCqiRpt;
#endif
{
   TRC2(rgSCHRrDlCqiInd);

   RETVOID;
}

/**
 * @brief Indication of activation of an UE after meas gap or ack/nack rep.
 *
 * @details
 *
 *     Function : rgSCHRrDlActvtUe
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlActvtUe
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrDlActvtUe(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

   TRC2(rgSCHRrDlActvtUe);
   /* Inserting the activated UE as current node in the list if it got skiped in inactive period */
   if((rrDlUe->ueTxLnk.node != NULLP) && (rrDlUe->isUeSkipped == TRUE)&&
      (rrDlCell->ueTxLst.crnt != &(rrDlUe->ueTxLnk)))
   {
      rgSCHRrCListDelFrm( &(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
      rgSCHRrCListInsrtAtCrnt( &(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));  
   }
   rrDlUe->isUeSkipped = FALSE;
   RETVOID;
}

/**
 * @brief Indication of Dl Refresh for the UE.
 *
 * @details
 *
 *     Function: rgSCHRrDlUeRefresh
 *     Purpose:  RR does nothing.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlUeRefresh
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHRrDlUeRefresh(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   TRC2(rgSCHRrDlUeRefresh);
   RETVOID;
}

/**
 * @brief Cell configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlCellCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information per cell
 *      - Initialize Tx and ReTx lists
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgrCellCfg          *cfg
 *  @param[in]  RgSchErrInfo        *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlCellCfg
(
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHRrDlCellCfg(cell, cfg, err)
RgSchCellCb          *cell;
RgrCellCfg           *cfg;
RgSchErrInfo         *err;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell;
   TRC2(rgSCHRrDlCellCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnCell->dl.schSpfc), (sizeof(RgSchRrDlCell))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), "Memory allocation FAILED"));
      MSPD_ERR("Memory allocation FAILED");
      RETVALUE(RFAILED);
   }

   rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);

   rgSCHRrCListInit(&(rrDlCell->ueTxLst));
   rgSCHRrCListInit(&(rrDlCell->ueRetxLst));

   RETVALUE(ROK);
}



/**
 * @brief Cell reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlCellRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellRecfg      *recfg
 *  @param[in]  RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlCellRecfg
(
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlCellRecfg(cell, recfg, err)
RgSchCellCb     *cell;
RgrCellRecfg    *recfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrDlCellRecfg);

   RETVALUE(ROK);
}


/**
 * @brief Cell deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlCellDel
 *
 *     Processing Steps:
 *      - Free Round Robin related information per cell.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlCellDel
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHRrDlCellDel(cell)
RgSchCellCb       *cell;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHRrDlCellDel);

   if(schCmnCell->dl.schSpfc)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(schCmnCell->dl.schSpfc)), (sizeof(RgSchRrDlCell)));
   }

   RETVOID;
}

/**
 * @brief Dl Harq Entity initialization for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeHqEntInit
 *
 *     Processing Steps:
 *      - Create Round Robin related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlUeHqEntInit
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC S16 rgSCHRrDlUeHqEntInit(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   TRC2(rgSCHRrDlUeHqEntInit);
   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(ue->dl.hqEnt->sch),
      (ue->dl.hqEnt->numHqPrcs * sizeof(RgSchRrDlHqProc))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), "Memory allocation FAILED"));
      MSPD_ERR("Memory allocation FAILED");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Dl Harq Entity deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeHqEntDeInit 
 *
 *     Processing Steps:
 *      - Free Round Robin related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchDlHqEnt     *hqE 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlUeHqEntDeInit 
(
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqE
)
#else
PUBLIC Void rgSCHRrDlUeHqEntDeInit(cell, hqE)
RgSchCellCb      *cell;
RgSchDlHqEnt     *hqE;
#endif
{
   TRC2(rgSCHRrDlUeHqEntDeInit);

   if(hqE->sch)
   {
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(hqE->sch)),
      (hqE->numHqPrcs * sizeof(RgSchRrDlHqProc)));
   }
   RETVOID;
}


/**
 * @brief UE configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information per UE.
 *      - Initialize signalling and data lists.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrUeCfg         *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlUeCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlUeCfg(cell, ue, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeCfg         *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe;
   /* rg003.201: Trace added */
   TRC2(rgSCHRrDlUeCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnUe->dl.schSpfc), (sizeof(RgSchRrDlUe))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), "Memory allocation FAILED"));
      MSPD_ERR( "Memory allocation FAILED");
      RETVALUE(RFAILED);
   }

   rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

   rgSCHRrCListInit(&(rrDlUe->sigLst));
   rgSCHRrCListInit(&(rrDlUe->dataLst));

   rrDlUe->isUeSkipped = FALSE;
   RETVALUE(ROK);
}

/**
 * @brief This function removes a HARQ process from the retx
 *
 * @details
 *
 *     Function: rgSCHRrDlProcRmvFrmRetx
 *     Purpose:  This function removes a HARQ process from retransmission
 *               queue. This may be performed when a HARQ ack is successful
 *               for a retransmission or during UE reset
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlHqProcCb  *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlProcRmvFrmRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PRIVATE Void rgSCHRrDlProcRmvFrmRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchRrDlHqProc *schRrDlHqProc =
      &((RgSchRrDlHqProc *)hqP->hqE->sch)[hqP->procId];
   TRC2(rgSCHRrDlProcRmvFrmRetx);

   if(schRrDlHqProc->retxLnk.node != NULLP)
   {
      rgSCHRrCListDelFrm(&(rrDlCell->ueRetxLst),
            &(schRrDlHqProc->retxLnk));
      schRrDlHqProc->retxLnk.node = NULLP;
   }

   RETVOID;
}

/**
 * @brief UE reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]   RgSchCellCb      *cell
 *  @param[in]   RgSchUeCb        *ue
 *  @param[in]   RgrUeRecfg       *recfg
 *  @param[in]   RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlUeRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlUeRecfg(cell, ue, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeRecfg       *recfg;
RgSchErrInfo     *err;
#endif
{
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchCmnDlUe *ueCmnDl  = RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHRrDlUeRecfg)
      UNUSED(recfg);
   UNUSED(err);
   /* Discarding TB2's context from scheduling Queues.
    * Since TB2 transmission needs signalling using
    * TM specific formats. And since during this transient
    * period of UE TM Recfg, SCH always uses Format 1A,
    * the TB2s are discarded. */
   if (ueCmnDl->mimoInfo.forceTD & RG_SCH_CMN_TD_TXMODE_RECFG)
   {
      /* If HqP is in retx queue only for TB2 retx scheduling
       * then remove the harp proc from retx Queue */

      /* If Hqp is in retx queue for retx allocation of
       * both TB1 and TB2, then reset TB2's state as ACKED */
      RgSchDlHqProcCb    *hqP;
      Pst                pst;
      RgInfRlsHqInfo     *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
      U8                 i;

      /* Prepare TB2 release information to be sent to MAC */
      rlsHqBufs->numUes = 0;
      for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
      {
         hqP = &ue->dl.hqEnt->procs[i];
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = ue->ueId;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = hqP->procId;
         if (hqP->tbInfo[1].state == HQ_TB_NACKED)
         {
            if (hqP->tbInfo[0].state != HQ_TB_NACKED)
            {
               /* Remove the HqP from retx Queue.
                  Release HqP.*/
               rgSCHRrDlProcRmvFrmRetx(cell, hqP);
            }
        /* rg001.301 - MOD -[ccpu00118350] : Correcting NDI manipulation of Harq */
            rgSCHDhmRlsHqpTb(hqP, 1, TRUE);
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[0] = 2;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 1;
         }
         rlsHqBufs->numUes++;
      }
      /* Send the hqProc list for MAC to clear TB1 contents */
      if (rlsHqBufs->numUes)
      {
         rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
         RgSchMacRlsHq (&pst, rlsHqBufs);
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief This function adds a HARQ process to the retx
 *
 * @details
 *
 *     Function: rgSCHRrDlProcAddToRetx
 *     Purpose:  This function adds a HARQ process to the retransmission
 *               queue. This may be performed when a HARQ nack is received
 *               and transmissions are not exceeded the max.
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlHqProcCb  *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlProcAddToRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHRrDlProcAddToRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchRrDlHqProc *schRrDlHqProc =
      &((RgSchRrDlHqProc *)hqP->hqE->sch)[hqP->procId];

   TRC2(rgSCHRrDlProcAddToRetx);


   rgSCHRrCListAdd2Tail(&(rrDlCell->ueRetxLst),
                     &(schRrDlHqProc->retxLnk));
   schRrDlHqProc->retxLnk.node = (PTR)hqP;

   RETVOID;
}

/* MS_WORKAROUND for ccpu00122892 : Temp fix for erroeneous RETX Harq release by rgSCHCmnDlAllocRetxRb */
#ifdef ANSI
PUBLIC Void rgSCHRrDlRmvProcFrmRetxLst
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHRrDlRmvProcFrmRetxLst(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   rgSCHRrDlProcRmvFrmRetx(cell, hqP);
   RETVOID;
}


/**
 * @brief UE delete for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeDel
 *
 *     Processing Steps:
 *     - Remove UE from retx queue if present
 *     - Remove UE from tx queue if present
 *     - Free Round Robin UE info
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlUeDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHRrDlUeDel(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8 i;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8 j;
   RgSchPdcch        *pdcchPrev = NULLP;
   TRC2(rgSCHRrDlUeDel);

   if(schCmnUe->dl.schSpfc == NULLP)
   {
      RETVOID;
   }

   if(rrDlUe->ueTxLnk.node != NULLP)
   {
      rgSCHRrCListDelFrm(&(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
      rrDlUe->ueTxLnk.node = NULLP; /* Added for avoiding dangling pointer */
   }

   /* MS_FIX : syed Assign hqEnt to UE only if msg4 is done */
   if (ue->dl.hqEnt)
   {
      /* remove all in use HARQ processes from the subframes */
      for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
      {
         RgSchDlHqProcCb  *hqP = &(ue->dl.hqEnt->procs[i]);
         rgSCHRrDlProcRmvFrmRetx(cell, hqP);
         for (j = 0; j < 2; j++)
         {
            if ((hqP->tbInfo[j].subFrm != NULLP) &&
                  (hqP->tbInfo[j].sfLnk.node != NULLP))
            { 
               /*
                * Since each TB can be assigned the same PDCCH structure incase of
                * Spatial multiplexing allocation of 2 TBs. Check if we have
                * already removed PDCCH as part of one TB, then no need to remove
                * as part of other TB processing 
                */
               if (hqP->tbInfo[j].pdcch != pdcchPrev)
               {
                  cmLListDelFrm(&hqP->tbInfo[j].subFrm->pdcchInfo.pdcchs,
                        &hqP->tbInfo[j].pdcch->lnk);
                  cmLListAdd2Tail(&cell->pdcchLst, &hqP->tbInfo[j].pdcch->lnk);
               }
               pdcchPrev = hqP->tbInfo[j].pdcch;
               /* rg001.301 - MOD -[ccpu00118350] : Correcting NDI manipulation of Harq */
               /* MS_FIX */
               rgSCHUtlDlHqPTbRmvFrmTx(hqP->tbInfo[j].subFrm, hqP, j, FALSE);
               rgSCHDhmRlsHqpTb(hqP, j, TRUE);
            }
         }
      }
   }

   rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(schCmnUe->dl.schSpfc)), (sizeof(RgSchRrDlUe)));

   RETVOID;
}

/**
 * @brief Dl LC configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlLcCfg
 *
 *     Processing Steps:
 *      - Create the round robin info
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @param[in]  RgrLchCfg        *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlLcCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlLcCfg(cell, ue, dlLc, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
RgrLchCfg        *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnDlSvc *schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
   TRC2(rgSCHRrDlLcCfg);
   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnDlLc->schSpfc), (sizeof(RgSchRrDlLc))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), "Memory allocation FAILED"));
      MSPD_ERR( "Memory allocation FAILED");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Dl LC re-configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlLcRecfg
 *
 *     Processing Steps:
 *      - RR does nothing
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @param[in]  RgrLchRecfg      *recfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlLcRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlLcRecfg(cell, ue, dlLc, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
RgrLchRecfg      *recfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrDlLcRecfg);

   RETVALUE(ROK);
}


/**
 * @brief Dl LC deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlLcDel
 *
 *     Processing Steps:
 *      - Free round robin info associated with the LC
 *      - Remove UE from tx queue if there are no LCs with
 *        outstanding BO
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlLcDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc
)
#else
PUBLIC Void rgSCHRrDlLcDel(cell, ue, dlLc)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchCmnDlSvc *schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
   RgSchRrDlLc *rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
   TRC2(rgSCHRrDlLcDel);

   if(schCmnDlLc->schSpfc == NULLP)
   {
      RETVOID;
   }

   if(rrDlLc->lnk.node)
   {
      if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
         (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
      {
         rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
      }
      else
      {
         rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
      }
      rrDlLc->lnk.node = NULLP; /* Added for avoiding dangling pointer */

      if((rrDlUe->sigLst.first == NULLP) &&
            (rrDlUe->dataLst.first == NULLP))
      {
         rgSCHRrCListDelFrm(&(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
         rrDlUe->ueTxLnk.node = NULLP;
      }
   }

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
   (Data**)(&(schCmnDlLc->schSpfc)), (sizeof(RgSchRrDlLc)));
   RETVOID;
}

/**
 * @brief BO update
 *
 * @details
 *
 *     Function : rgSCHRrDlDedBoUpd
 *
 *     Processing Steps:
 *     - If the UE is not part of transmission queue then
 *       append the UE to this queue if reported bo > 0.
 *       If reported bo is ZERO then remove the UE from
 *       transmission queue if there are no other LCs with
 *       outstanding bo.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchUeCb              *ue
 *  @param[in]  RgSchDlLcCb            *lc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlDedBoUpd
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *dlLc
)
#else
PUBLIC Void rgSCHRrDlDedBoUpd(cell, ue, dlLc)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgSchDlLcCb            *dlLc;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchCmnDlSvc *schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
   RgSchRrDlLc *rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
   TRC2(rgSCHRrDlDedBoUpd);

   if((dlLc->bo) && (rrDlLc->lnk.node == NULLP))
   {
      if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
         (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
      {
         rgSCHRrCListAdd2Tail(&(rrDlUe->sigLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = (PTR)dlLc;
      }
      else
      {
         rgSCHRrCListAdd2Tail(&(rrDlUe->dataLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = (PTR)dlLc;
      }

      if(rrDlUe->ueTxLnk.node == NULLP)
      {
         rgSCHRrCListAdd2Tail(&(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
         rrDlUe->ueTxLnk.node = (PTR)ue;
      }
      RETVOID;
   }

   if((dlLc->bo == 0) && (rrDlLc->lnk.node))
   {
      if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
         (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
      {
         rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = NULLP;
      }
      else
      {
         rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = NULLP;
      }

      if((rrDlUe->sigLst.first == NULLP) &&
            (rrDlUe->dataLst.first == NULLP))
      {
         rgSCHRrCListDelFrm(&(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
         rrDlUe->ueTxLnk.node = NULLP;
      }
      RETVOID;
   }

   /* Stack crash problem for TRACE5 Changes. added the return below */
  RETVOID;
}

/**
 * @brief Indication of DL UE reset
 *
 * @details
 *
 *     Function : rgSCHRrDlUeReset
 *
 *     Processing Steps:
 *     - Remove the UE from re-transmission list
 *     - Remove the UE from transmission list
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlUeReset
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrDlUeReset(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchRrCList *node;
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   U8 i;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   TRC2(rgSCHRrDlUeReset);

   while((node=rrDlUe->sigLst.first) != NULLP)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
      rrDlLc->lnk.node = NULLP;
      node = node->next;
   }
   rgSCHRrCListInit(&(rrDlUe->sigLst));

   while((node=rrDlUe->dataLst.first) != NULLP)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
      rrDlLc->lnk.node = NULLP;
      node = node->next;
   }
   rgSCHRrCListInit(&(rrDlUe->dataLst));

   /* remove all in use HARQ processes from the subframes */
   for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
   {
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      RgSchDlHqProcCb  *hqP = &(ue->dl.hqEnt->procs[i]);
      rgSCHRrDlProcRmvFrmRetx(cell, hqP);
      /*MS_WORKAROUND for ccpu00122892 */
      if ((hqP->tbInfo[0].subFrm != NULLP) &&
          (hqP->tbInfo[0].sfLnk.node != NULLP))
      {
         cmLListDelFrm(&hqP->tbInfo[0].subFrm->pdcchInfo.pdcchs,
                  &hqP->tbInfo[0].pdcch->lnk);
         cmLListAdd2Tail(&cell->pdcchLst, &hqP->tbInfo[0].pdcch->lnk);
         /* MS_FIX */
         rgSCHUtlDlHqPTbRmvFrmTx(hqP->tbInfo[0].subFrm, hqP, 0, FALSE);
      }
      if ((hqP->tbInfo[1].subFrm != NULLP) &&
               (hqP->tbInfo[1].sfLnk.node != NULLP))
         {
	 if (hqP->tbInfo[1].pdcch != hqP->tbInfo[0].pdcch)
	 {
            cmLListDelFrm(&hqP->tbInfo[1].subFrm->pdcchInfo.pdcchs,
                  &hqP->tbInfo[1].pdcch->lnk);
            cmLListAdd2Tail(&cell->pdcchLst, &hqP->tbInfo[1].pdcch->lnk);
         }
         /* MS_FIX */
         rgSCHUtlDlHqPTbRmvFrmTx(hqP->tbInfo[1].subFrm, hqP, 1, FALSE);
      }
   }

   if(rrDlUe->ueTxLnk.node != NULLP)
   {
      rgSCHRrCListDelFrm(&(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
      rrDlUe->ueTxLnk.node = NULLP;
   }
#ifdef MSPD
   MSPD_DBG("<RESET> DONE\n");
#endif

   RETVOID;
}



/**
 * @brief Scheduling of DL re-transmissions
 *
 * @details
 *
 *     Function : rgSCHRrDlRetx
 *
 *     Processing Steps:
 *     - Traverse the re-transmission list and do resource allocation
 *       for the UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlRetx
(
RgSchCellCb            *cell,
RgSchCmnDlRbAllocInfo  *allocInfo
)
#else
PRIVATE Void rgSCHRrDlRetx(cell, allocInfo)
RgSchCellCb            *cell;
RgSchCmnDlRbAllocInfo  *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchUeCb    *ue;
   RgSchDlRbAlloc *dlRbAlloc;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   RgSchDlHqProcCb *hqP;
   RgSchRrCList *node;
   U32 allocBo;
   /* MS_FIX: MUE_PERTTI_DL*/
   U32 remUe = allocInfo->dedAlloc.dedDlSf->remUeCnt;
   /*rg008.201 - Added for SPS support*/
#ifdef LTEMAC_SPS
   CmLteTimingInfo schdTime;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   TRC2(rgSCHRrDlRetx);

   if(rrDlCell->ueRetxLst.crnt == NULLP || !remUe)
   {
      RETVOID;
   }
/*rg008.201 - Added for SPS support*/
#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif
   /* loop till the crnt node */
   node = rrDlCell->ueRetxLst.crnt;
   do
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      dlRbAlloc = &(ue->dl.dlAllocCb);
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
      node = node->next;

#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
/*rg008.201 - Added for SPS support*/
      if((ue->dl.dlInactvMask)
#ifdef LTEMAC_SPS
            ||(RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
#endif
        )
      {
         continue;
      }

      if(schCmnUe->dl.proc != NULLP)
      {
         /*UE is already selected for re-tx*/
         continue;
      }
      /* Extra check: indicate if there is furtherScope for NewTx
       * addition for a HqProc. This information will
       * be utilized by common scheduler, in case of SM
       * UEs with only one of the TBs retransmitting and the
       * other TB can be used for clubbing new TX. */
      if ((rrDlUe->sigLst.first != NULLP) || (rrDlUe->dataLst.first != NULLP))
      {
         ue->dl.dlAllocCb.mimoAllocInfo.hasNewTxData = TRUE;
      }
      /* 3.1 MIMO : last parameter changed */
      if(rgSCHCmnDlAllocRetxRb(cell, allocInfo->dedAlloc.dedDlSf,
               ue, 0, &allocBo, hqP, allocInfo) != ROK)
      {
	 /* MS_FIX: syed For RETX let Fnlz alone change the crnt by means of 
	  * node deletions */
	 /* MS_FIX: syed Missing remUeCnt setting */
         rrDlCell->ueRetxLst.crnt = node->prev;
         break;
      }
      /* MS_FIX: syed removing unnecessary check*/

      if(allocBo == 0)
      {
         continue;
      }
      schCmnUe->dl.proc = hqP;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* Adding Ue into Retx list is moved to cmn Scheduler */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      --remUe;
      if(!remUe)
      {
         rrDlCell->ueRetxLst.crnt = node;
         break;
      }

   }while(node != rrDlCell->ueRetxLst.crnt);

   allocInfo->dedAlloc.dedDlSf->remUeCnt = remUe;
   RETVOID;
}

/**
 * @brief Scheduling of DL new-transmission for UE
 *
 * @details
 *
 *     Function : rgSCHRrDlSchdUe
 *
 *     Processing Steps:
 *     - Schedule DCCHs if outstanding bo exist for them.
 *     - Schedule DTCHs if outstanding bo exist for them.
 *     - Limit the LCs scheduling based on max TB size supported.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  RgSchDlSf           *sf
 *  @return  Void
 **/
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
#ifdef ANSI
PRIVATE S16 rgSCHRrDlSchdUe
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchDlSf               *sf,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE S16 rgSCHRrDlSchdUe(cell, ue, sf, allocInfo)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchDlSf               *sf;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   RgSchRrCList *node;
   U32 bo;
   U16 rlcHdrEstmt;
   U32 allocBo;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchCmnDlHqProc *cmnHqDl;
   S16  ret;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   rgSCHRrCListInit(&(rrDlUe->schedLcs));


   if((rrDlUe->sigLst.count) ||
         (rrDlUe->dataLst.count))
   {
      if(rrDlUe->schdTa == FALSE)
      {
         if(rgSCHDhmGetAvlHqProc(ue,
                  schCmnCell->dl.time, &(schCmnUe->dl.proc)) != ROK)
         {
            RETVALUE(ROK);
         }
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
         cmnHqDl = RG_SCH_CMN_GET_DL_HQP(schCmnUe->dl.proc);
         cmnHqDl->totBytes = 0;
      }
      else
      {
         /*rg002.301 - ADD -[ccpu00120941] Added the handling for fetching the
          * common HQ DL.*/
         cmnHqDl = RG_SCH_CMN_GET_DL_HQP(schCmnUe->dl.proc);
      }
   }
   else
   {
#ifdef MSPD
      MSPD_DBG("UE should have been removed from the queue\n");
#endif
      RETVALUE(ROK);
   }

   /* MS_FIX : syed Round Robin with in sigLst */
   node = rrDlUe->sigLst.crnt;
   while(node)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);

      RG_SCH_CMN_DL_GET_HDR_EST(dlLc, rlcHdrEstmt);
      bo= dlLc->bo+rlcHdrEstmt;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* rg003.301[ccpu00122038]-MOD- Check for return value */
      ret = rgSCHCmnDlAllocTxRb(cell, sf, ue, bo, &allocBo,schCmnUe->dl.proc,allocInfo);
      if((ret != ROK) || (allocBo == 0))
      {
         if (cmnHqDl->totBytes == 0)
         {
            /*rg002.301 - ADD -[ccpu00120941] Added the handling for freeing the
             * harq proc, removing UE from txUeLst and resetting outStndAlloc.*/
            RgSchCmnDlUe *ueDl  = RG_SCH_CMN_GET_DL_UE(ue);
            if(schCmnUe->dl.proc != NULLP)
            {
               /* MS_FIX [ccpu00126847]: Corrected the order of following if check to avoid crash  
			         in case of RB allocation failure */
               if(schCmnUe->dl.proc->reqLnk.node != (PTR)NULLP)
               {
                  cmLListDelFrm(&allocInfo->dedAlloc.txUeLst, &schCmnUe->dl.proc->reqLnk);
                  schCmnUe->dl.proc->reqLnk.node = (PTR)NULLP;
               }
               rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
               schCmnUe->dl.proc = NULLP;
            }
            
            /*Re-set the outstanding alloc information.*/
            ueDl->outStndAlloc = 0;
         }
         /* MS_FIX : syed Round Robin with in sigLst */
         rrDlUe->sigLst.crnt = node;
         /* rg003.301[ccpu00122038]-MOD- Return the value returned by DlAllocTxRb */
         RETVALUE(ret);
      }

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      rgSCHRrCListAdd2Tail(&(rrDlUe->schedLcs), &(rrDlLc->schdLnk));
      rrDlLc->schdLnk.node = (PTR)dlLc;
      rrDlLc->schdBo = allocBo;
      rrDlLc->rlcHdrEstmt = rlcHdrEstmt;
      /*rg002.301 - ADD -[ccpu00120941] Added the handling for updating the
       * totBtyes.*/
      cmnHqDl->totBytes += allocBo;

      /* MS_FIX : syed Round Robin with in sigLst */
      node = node->next;
      if(node == rrDlUe->sigLst.crnt)
      {
         break;
      }
   }

   node = rrDlUe->dataLst.crnt;
   while(node)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);

      RG_SCH_CMN_DL_GET_HDR_EST(dlLc, rlcHdrEstmt);
      bo= dlLc->bo+rlcHdrEstmt;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* rg003.301[ccpu00122038]-MOD- Check for return value */
      ret = rgSCHCmnDlAllocTxRb(cell, sf, ue, bo, &allocBo, schCmnUe->dl.proc,allocInfo);
      /* rg003.301[ccpu00122038]-DEL- Removing Release Harq code */
      /* rg003.301[ccpu00122038]-ADD- Check for return value */
      if((ret != ROK) || (allocBo == 0))
      {
         /* rg007.201 - Added missing fix for CR ccpu00110740
            Releasing the HARQ, if RB allocation fails */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
#ifdef MSPD
        MSPD_ERR("DL alloc failed: UE(%d) hqP(%d) txUeLstCnt(%d) reqLnk(%d) retxUeLstCnt(%d) txRetxLstCnt(%d) sfBw[%d:%d]\n",
	           ue->ueId, schCmnUe->dl.proc->procId, 
		   allocInfo->dedAlloc.txUeLst.count, 
		   schCmnUe->dl.proc->reqLnk.node, 
		   allocInfo->dedAlloc.retxUeLst.count,	
		   allocInfo->dedAlloc.txRetxUeLst.count,
		   sf->bw, sf->bwAssigned);
#endif
	 /* MS_FIX : syed Upon failure release HqP and UE from txUeLst.
	  * We use to just release hqP without removing it from txUeLst,
	  * which was leading to crash in Finalization. This piece of 
	  * code can be made as a function instead. Check other scheduler 
	  * implementation as well. */
         if (cmnHqDl->totBytes == 0)
         {
            /*rg002.301 - ADD -[ccpu00120941] Added the handling for freeing the
             * harq proc, removing UE from txUeLst and resetting outStndAlloc.*/
            RgSchCmnDlUe *ueDl  = RG_SCH_CMN_GET_DL_UE(ue);
            if(schCmnUe->dl.proc != NULLP)
            {
               /* MS_FIX [ccpu00126847]: Corrected the order of following if check to avoid crash  
			         in case of RB allocation failure */
               if(schCmnUe->dl.proc->reqLnk.node != (PTR)NULLP)
               {
                  cmLListDelFrm(&allocInfo->dedAlloc.txUeLst, &schCmnUe->dl.proc->reqLnk);
                  schCmnUe->dl.proc->reqLnk.node = (PTR)NULLP;
               }
               rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
               schCmnUe->dl.proc = NULLP;
            }
            
            /*Re-set the outstanding alloc information.*/
            ueDl->outStndAlloc = 0;
         }
         /* MS_FIX : syed Round Robin with in sigLst */
         rrDlUe->dataLst.crnt = node;
         /* rg003.301[ccpu00122038]-MOD- Return value */
         RETVALUE(ret);
      }
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      rgSCHRrCListAdd2Tail(&(rrDlUe->schedLcs), &(rrDlLc->schdLnk));
      rrDlLc->schdLnk.node = (PTR)dlLc;
      rrDlLc->schdBo = allocBo;
      rrDlLc->rlcHdrEstmt = rlcHdrEstmt;
      /*rg002.301 - ADD -[ccpu00120941] Added the handling for updating the
       * totBtyes.*/
      cmnHqDl->totBytes += allocBo;

      /* MS_FIX : syed Round Robin with in sigLst */
      node = node->next;
      if(node == rrDlUe->dataLst.crnt)
      {
         break;
      }
   }

   if((rrDlUe->schedLcs.count == 0) && (rrDlUe->schdTa == FALSE))
   {
      if(schCmnUe->dl.proc != NULLP)
      {
        /* rg001.301 - MOD -[ccpu00118350] : Correcting NDI manipulation of Harq */
         rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
         schCmnUe->dl.proc = NULLP;
      }
   }

   RETVALUE(ROK);
}
/* rg001.301 - MOD -[ccpu00118350] : Correcting NDI manipulation of Harq */
/**
 * @brief Release the DL HarqProc.
 *
 * @details
 *
 *     Function :  rgSCHRrRlsDlHqProc
 *
 *     Processing Steps:
 *               Toggle the NDI.
 *               Release the Hq Proc
 *
 *  @param[in]  RgSchCmnUe   *schCmnUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrRlsDlHqProc
(
RgSchDlHqProcCb    *proc
)
#else
PRIVATE Void rgSCHRrRlsDlHqProc(proc)
RgSchDlHqProcCb    *proc;
#endif
{
   TRC2(rgSCHRrRlsDlHqProc)
   rgSCHDhmRlsHqProc(proc);
   RETVOID;
}
/**
 * @brief Scheduling of TA for the UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlSchdTa
 *
 *     Processing Steps:
 *     - Traverse the ta list  and do resource allocation
 *       for the UEs.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @return  Void
 **/
 /* MS_FIX [ccpu00126870]: MUE_PERTTI_DL Limiting number of TA scheduling */
#ifdef ANSI
PRIVATE S16 rgSCHRrDlSchdTa
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE S16 rgSCHRrDlSchdTa(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchDlSf    *dlSf = allocInfo->dedAlloc.dedDlSf;
   RgSchUeCb    *ue;
   RgSchDlRbAlloc *dlRbAlloc;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   CmLList *taNode;
   U32 allocBo;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchCmnDlHqProc *cmnHqDl;
   /*rg008.201 - Added for SPS support*/
#ifdef LTEMAC_SPS
   CmLteTimingInfo schdTime;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif

   
   TRC2(rgSCHRrDlSchdTa);

#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif
      
   taNode = schCmnCell->dl.taLst.first;
   /* MS_FIX [ccpu00126870]: MUE_PERTTI_DL: Limiting number of TA scheduling */
   while(taNode && dlSf->remUeCnt)
   {
      ue = (RgSchUeCb *)(taNode->node);
      taNode = taNode->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      /*rg008.201 - Added for SPS support*/
      if((ue->dl.dlInactvMask)
#ifdef LTEMAC_SPS
            ||(RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
#endif
        )
      {
         continue;
      }
      dlRbAlloc = &(ue->dl.dlAllocCb);
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

      if(schCmnUe->dl.proc != NULLP)
      {
         continue;
      }

      if(rgSCHDhmGetAvlHqProc(ue,
               schCmnCell->dl.time, &(schCmnUe->dl.proc)) != ROK)
      {
         continue;
      }
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(schCmnUe->dl.proc);
      cmnHqDl->totBytes = 0;
      if(rgSCHCmnDlAllocTxRb(cell, dlSf, ue,
               RGSCH_TA_SIZE, &allocBo, schCmnUe->dl.proc,\
               allocInfo) != ROK)
      {
         /* TODO: the following 9 lines of code for
          * hqP release is reteated. Can be made as a
          * function */
         rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
         schCmnUe->dl.proc = NULLP;
         RETVALUE(RFAILED);
      }
      if(allocBo)
      {
         rrDlUe->schdTa = TRUE;
         /*rg002.301 - ADD -[ccpu00120941] Added the handling for updating the
          * totBtyes.*/
         cmnHqDl->totBytes += allocBo;
         /* MSPD_DBG("<MIMO> Scheduled TA for UE(%d)\n", ue->ueId); */
		 /* MS_FIX [ccpu00126870]: MUE_PERTTI_DL: Limiting number of TA scheduling. Please
          * check in remUeCnt handling in oher schedulers as well. */
         /* MS_FIX [ccpu00126870]: MUE_PERTTI_DL: Scheduling data and signalling bearers as well */
         rgSCHRrDlSchdUe(cell, ue, dlSf, allocInfo);
         /* MS_FIX [ccpu00126870]: MUE_PERTTI_DL: Limiting number of TA scheduling. Please
          * check in remUeCnt handling in oher schedulers as well. */
         (dlSf->remUeCnt)--;
      }      /* Add to UeLst is moved to common Scheduler */
      else
      {
         rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
         schCmnUe->dl.proc = NULLP;
         continue;
      }
   }       
	
   RETVALUE(ROK);
}

/**
 * @brief Scheduling of DL new transmissions
 *
 * @details
 *
 *     Function : rgSCHRrDlNewTx
 *
 *     Processing Steps:
 *     - Schedule TA for UEs
 *     - Schedule new transmissions for UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlNewTx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrDlNewTx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchDlSf    *dlSf = allocInfo->dedAlloc.dedDlSf;
   RgSchUeCb    *ue;
   RgSchDlRbAlloc *dlRbAlloc;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   RgSchRrCList *node;
   U8 numUes;
   /* MS_FIX: MUE_PERTTI_DL: cap off new tranmsisisons by total number of UEs per TTI */
   U8 excessUeCnt = 0;
   /*rg008.201 - Added for SPS support*/
#ifdef LTEMAC_SPS
   CmLteTimingInfo      schdTime;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   TRC2(rgSCHRrDlNewTx);
      
   if (schCmnCell->dl.maxUeNewTxPerTti < dlSf->remUeCnt)
   {
      excessUeCnt = dlSf->remUeCnt - schCmnCell->dl.maxUeNewTxPerTti;	   
      dlSf->remUeCnt = schCmnCell->dl.maxUeNewTxPerTti;
   }
   /* MS_FIX [ccpu00126870]: Consider TA as well for limiting number of UEs per TTI */
   /* MS_WORKAROUND: protection in MAC to avoid transmision if */
   /* retransmision is going out (for Wrong Sdu Size).  */
   if(rgSCHRrDlSchdTa(cell, allocInfo) != ROK)
   {
      dlSf->remUeCnt += excessUeCnt;
      RETVOID;
   }   
   
   /* MS_FIX [ccpu00126870]: Consider TA as well for limiting number of UEs per TTI */
   if ((!dlSf->remUeCnt) ||
       (!rrDlCell->ueTxLst.count))		   
   {
      dlSf->remUeCnt += excessUeCnt;
      RETVOID;
   }
   /*rg008.201 - Added for SPS support*/
#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif

   node = rrDlCell->ueTxLst.crnt;
   do
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif

      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

      /*rg008.201 - Added for SPS support*/
      if((ue->dl.dlInactvMask)
#ifdef LTEMAC_SPS
            ||(RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
#endif
        )
      {
         rrDlUe->isUeSkipped = TRUE;
         continue;
      }
      dlRbAlloc = &(ue->dl.dlAllocCb);

      /* MS_FIX [ccpu00126870]: Consider TA as well for limiting number of UEs per TTI */
         /* UE already selected for TA */
		 //MSPD_DBG("Skipping UE %d\n", ue->ueId);
	  	
      if (schCmnUe->dl.proc) 
      {
         continue;
      }

      if(rgSCHRrDlSchdUe(cell, ue, dlSf, allocInfo) != ROK)
      {
	 /* MS_FIX: syed If UE is partially considered this sf
	  * then move to next UE for the next sf. */
         if (schCmnUe->dl.proc != NULLP)
	 {		 
            rrDlCell->ueTxLst.crnt = node;
	 }
	 else
	 {		 
            rrDlCell->ueTxLst.crnt = node->prev;
	 }
	 /* MS_FIX: syed Missing remUeCnt setting */
         break;
      }

      if(rrDlUe->schedLcs.count == 0)
      {
         continue;
      }
      /* Adding UE into UeTxLst is done in cmn scheduler */
      /* MS_FIX [ccpu00126870]: Consider TA as well for limiting number of UEs per TTI */

      if(!(--dlSf->remUeCnt));
      {
         rrDlCell->ueTxLst.crnt = node;
         break;
      }

   }while(node != rrDlCell->ueTxLst.crnt);
   /* MS_FIX [ccpu00126870]: Updating the remUeCnt based on current allocation */
   dlSf->remUeCnt += excessUeCnt;
   RETVOID;
}

/**
 * @brief Round Robin related scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHRrDlSchd
 *
 *     Processing Steps:
 *     - Schedule the re-transmission UEs by invoking rgSCHRrDlRetx
 *     - Schedule the new transmission UEs by invoking rgSCHRrDlNewTx
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlSchd
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrDlSchd(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   TRC2(rgSCHRrDlSchd);

   rgSCHRrDlRetx(cell, allocInfo);
   rgSCHRrDlNewTx(cell, allocInfo);

   RETVOID;
}
/* 3.1 MIMO Alloc distribution functions being performed
 * TB wise */

/***********************************************************
 *
 * @brief Dl allocation finalization for the UE.
 *
 * @details
 *
 *     Function : rgSCHRrDlNewTxTbDstn
 *
 *     Processing Steps:
 *     - Finalize the UE allocation.
 *     - If TA scheduled then remove from TA list.
 *     - Update the schedule BO for the scheduled LCs.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  RgSchDlHqTbCb  *tbInfo,
 *  @param[in]  U32                    *effAlloc,
 *  @param[in]  CmLList              **node
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHRrDlNewTxTbDstn
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
RgSchRrCList        **node
)
#else
PRIVATE Void rgSCHRrDlNewTxTbDstn(cell, ue, tbInfo, effAlloc, node)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
RgSchRrCList        **node;
#endif
{
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   U32   bo;
   RgSchLchAllocInfo   lchSchdData;
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchRrCList *lnk = *node;
   RgSchRrCList *prev = NULLP;


   TRC2(rgSCHRrDlNewTxTbDstn);

   if(!tbInfo->schdTa.pres)
   {
      if(rrDlUe->schdTa == TRUE)
      {
         if(*effAlloc >= RGSCH_TA_SIZE)
         {
            *effAlloc -= RGSCH_TA_SIZE;
            rgSCHCmnRmvFrmTaLst(cell, ue);
            rgSCHDhmSchdTa(ue, tbInfo);
            rrDlUe->schdTa = FALSE;
         }
         else if(rrDlUe->schedLcs.first == NULLP)
         {
            RETVOID;
         }
      }
   }

   while((*node) && (*effAlloc > 0))
   {
      dlLc = (RgSchDlLcCb *)(*node)->node;
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      prev = *node;
      *node = (*node)->next;

      bo = RGSCH_MIN(rrDlLc->schdBo, *effAlloc);

      if(bo <= rrDlLc->rlcHdrEstmt)
      {

         break;
      }

      /* 3.1 MIMO updating the reqBytes field */
      rrDlLc->schdBo -= bo;
      (dlLc->bo <= bo-rrDlLc->rlcHdrEstmt)?\
        (dlLc->bo = 0):\
        (dlLc->bo -= bo-rrDlLc->rlcHdrEstmt);

      *effAlloc -= bo;

      /* Update DHM for this SVC */
      lchSchdData.schdData = bo;
      lchSchdData.lcId = dlLc->lcId;

      if(rgSCHDhmAddLcData(cell->instIdx, &lchSchdData,
            tbInfo) != ROK)
      {
         RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
                                 "Failed to update DHM"));
         MSPD_ERR("Failed to update DHM \n");
      }

      if(dlLc->bo == 0)
      {
         if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
            (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
         {
            rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
         }
         else
         {
            rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
         }
         rrDlLc->lnk.node = NULLP;
      }
      /* node is a circuler linked list. Starting address is stored in lnk,
       * when node reaches at the beining of CList,it should come out*/
      if (lnk == *node)
      {
         break;
      }
   }
   /* make sure node points to the svc not completely
    * satisfied.
    * make sure if not served completely then
    * the other TB allocation accomodates the same */
   *node = prev;
   RETVOID;
}


/***********************************************************
 *
 * @brief Dl allocation finalization for the UE.
 *
 * @details
 *
 *     Function : rgSCHRrDlSprTxTbDstn
 *
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given spare TB "tbInfo" allocation.
 *     spare TB allocation is as a result of 1 RETX TB allocation, when
 *     conditions are favourable for 2 TB spatial multiplexing.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  RgSchDlHqTbCb     *tbInfo,
 *  @param[in]  U32               *effAlloc,
 *  @param[in/out] RgSchRrCListCp *lcLst;
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHRrDlSprTxTbDstn
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
RgSchRrCListCp *lcLst
)
#else
PRIVATE Void rgSCHRrDlSprTxTbDstn(cell, ue, tbInfo, effAlloc, lcLst)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
RgSchRrCListCp *lcLst;
#endif
{
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   U32   bo;
   RgSchLchAllocInfo   lchSchdData;
   U32     effBo;


   TRC2(rgSCHRrDlSprTxTbDstn);

   MSPD_DBG("HIT IT for UE(%d) lstCnt(%d)\n", ue->ueId, lcLst->count);

   /* MS_FIX: syed List Corruption issue */
   while(lcLst->crnt)
   {
      if(*effAlloc == 0)
      {
         break;
      }
      dlLc = (RgSchDlLcCb *)(lcLst->crnt)->node;
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      effBo= dlLc->bo + rrDlLc->rlcHdrEstmt;
      bo = (effBo < *effAlloc) ?
            effBo : *effAlloc;

      MSPD_DBG("LC(%d) effAlloc(%d) bo(%d) effBo(%d) rlcHdrEst(%d)\n", 
                dlLc->lcId, *effAlloc, dlLc->bo, effBo, rrDlLc->rlcHdrEstmt);
      if(bo <= rrDlLc->rlcHdrEstmt)
      {
#ifdef MSPD
         MSPD_DBG("Cell Time [%d,%d] BO %d TbIdx %d TbSiz %d numLch %d\n", 
                  cell->crntTime.sfn,cell->crntTime.subframe,bo, 
                  tbInfo->tbIdx, tbInfo->tbSz, tbInfo->numLch);
#endif
         break;
      }
      if(bo > dlLc->bo)
      {
         lchSchdData.schdData = bo;
         dlLc->bo = 0;
      }
      else
      {
         lchSchdData.schdData = bo;
         dlLc->bo -= (bo - rrDlLc->rlcHdrEstmt);
      }

      *effAlloc -= bo;
      lchSchdData.lcId = dlLc->lcId;
      if(rgSCHDhmAddLcData(cell->instIdx, &lchSchdData,
            tbInfo) != ROK)
      {
         RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
                                 "Failed to update DHM"));
         MSPD_ERR("Failed to update DHM \n");
      }

      if(dlLc->bo == 0)
      {
         rgSCHRrCListDelFrm(lcLst, &(rrDlLc->lnk));
         rrDlLc->lnk.node = NULLP;
      }
   }
   RETVOID;
}

/**
 * @brief Dl allocation finalization for the UE.
 *
 * @details
 *
 *     Function : rgSCHRrDlUeAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize the UE allocation.
 *     - If TA scheduled then remove from TA list.
 *     - Update the schedule BO for the scheduled LCs.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHRrDlUeAllocFnlz
(
RgSchCellCb       *cell,
RgSchUeCb         *ue
)
#else
PRIVATE S16 rgSCHRrDlUeAllocFnlz(cell, ue)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
#endif
{
   RgSchCmnCell  *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe    *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe   *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchRrCList  *node;
   /* 3.1 MIMO Distribute data of each TB across services */
   U32                remTb1Bytes = ue->dl.dlAllocCb.tbInfo[0].bytesAlloc;
   U32                remTb2Bytes = ue->dl.dlAllocCb.tbInfo[1].bytesAlloc;
   U32                effAlloc = 0;
   /*ccpu00120365-ADD-added to code to check if second TB is utilized */
   U32                tb2Bytes = 0;

   TRC2(rgSCHRrDlUeAllocFnlz);



   /* 3.1 MIMO Consider the allocation of New TX TB for distribution */
   /* Handle schd services */
   node = rrDlUe->schedLcs.first;
   if (remTb1Bytes){
      effAlloc += remTb1Bytes;
      rgSCHRrDlNewTxTbDstn(cell, ue, &schCmnUe->dl.proc->tbInfo[0],\
                            &remTb1Bytes, &node);
      /* In the event that TB1 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 0,
           ue->dl.dlAllocCb.tbInfo[0].bytesAlloc - remTb1Bytes);

   }


   /*ccpu00120365-ADD-assigning value of remTb2Bytes before utilization */
   tb2Bytes = remTb2Bytes;

   /* Extra check for a non SM UE allocation */
   if (remTb2Bytes){
      effAlloc += remTb2Bytes;
      rgSCHRrDlNewTxTbDstn(cell, ue, &schCmnUe->dl.proc->tbInfo[1],\
                            &remTb2Bytes, &node);
      /* In the event that TB2 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 1,
           ue->dl.dlAllocCb.tbInfo[1].bytesAlloc - remTb2Bytes);

   }

   /* ccpu00120365-ADD-Disable the second TB as the second TB is not
    * utilized */
   if ( remTb2Bytes && ( tb2Bytes == remTb2Bytes) )
   {
      ue->dl.dlAllocCb.mimoAllocInfo.precIdxInfo = 0;
      ue->dl.dlAllocCb.mimoAllocInfo.numTxLyrs   = 1;
      ue->dl.dlAllocCb.tbInfo[1].schdlngForTb    = FALSE;
      ue->dl.dlAllocCb.tbInfo[1].isDisabled      = TRUE;
   }

   if (effAlloc == (remTb1Bytes + remTb2Bytes))
   {
      RETVALUE(RFAILED);
   }

   if((rrDlUe->sigLst.count == 0) &&
      (rrDlUe->dataLst.count == 0))
   {
      rgSCHRrCListDelFrm(&(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
      rrDlUe->ueTxLnk.node = NULLP;
   }
   RETVALUE(ROK);
}


/**
 * @brief Dl allocation finalization for re-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlRetxUesAllocFnlz
 *
 *     Processing Steps:
 *     - If allocation succeded then remove from re-transmission list
 *        LTEMAC_MIMO: For SM UEs consider harq state of both TBs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlRetxUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrDlRetxUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchUeCb       *ue;
   RgSchDlHqProcCb *hqP;
   RgSchDlRbAlloc  *dlRbAlloc;
   RgSchCmnUe      *schCmnUe;
   CmLList         *node;
   TRC2(rgSCHRrDlRetxUesAllocFnlz);

   node = allocInfo->dedAlloc.schdRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      dlRbAlloc = &(ue->dl.dlAllocCb);
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      node = node->next;
      rgSCHCmnFillHqPPdcch(cell, dlRbAlloc, hqP);
      /* Extra Check: Retain the hqProc in the RETX Queue if one/more
       * TBs of the HqProc are yet to be scheduled for RETX */
      if ((hqP->tbInfo[0].state != HQ_TB_NACKED) &&
            (hqP->tbInfo[1].state != HQ_TB_NACKED))
      {
         rgSCHRrDlProcRmvFrmRetx(cell, hqP);
      }
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   node = allocInfo->dedAlloc.nonSchdRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      node = node->next;
      /* MS_FIX: syed To update Retx Alloc failure counter.
       * Need to do the same in other schedulers as well. */
      MSPD_DBG("UE(%d) PROC(%d) FAILED\n", ue->ueId, hqP->procId);
      rgSCHDhmDlRetxAllocFail(ue, hqP);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/**
 * @brief Dl allocation finalizations for the new-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlUesAllocFnlz
 *
 *     Processing Steps:
 *     - If allocation succeded then link the harq in dl subframe.
 *     - else release the harq.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrDlUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchDlHqProcCb *hqP;
   RgSchCmnUe   *schCmnUe;
   CmLList      *node;
   TRC2(rgSCHRrDlUesAllocFnlz);

   node = allocInfo->dedAlloc.schdTxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      schCmnUe = (RgSchCmnUe *)(ue->sch);

      node = node->next;
      if(rgSCHRrDlUeAllocFnlz(cell, ue) == ROK)
      {
         rgSCHCmnFillHqPPdcch(cell, &(ue->dl.dlAllocCb), hqP);
      }
      else
      {
         rgSCHUtlPdcchPut(cell, &(allocInfo->dedAlloc.dedDlSf->pdcchInfo),
                     ue->dl.dlAllocCb.pdcch);
         /* rg001.301 - MOD -[ccpu00118350] : Correcting NDI manipulation 
	  * of Harq */
         rgSCHRrRlsDlHqProc(hqP);
         /* rg006.201:[ccpu00110358] Removed ndi toggling post release */
      }
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   node = allocInfo->dedAlloc.nonSchdTxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      node = node->next;

      MSPD_DBG("UE(%d) PROC(%d) FAILED\n", ue->ueId, hqP->procId);
      /* rg001.301 - MOD -[ccpu00118350] : Correcting NDI 
       * manipulation of Harq */
      rgSCHRrRlsDlHqProc(hqP);
      /* rg006.201:[ccpu00110358] Removed ndi toggling post release */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}
 
/* 3.1 Added new function to handle TX+RETX alloc fnlz'n */

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX+New TX allocations. The NewTx TB allocation
 *        is considered for distribution among LCs.
 *
 * @details
 *
 *     Function: rgSCHRrDlRetxNewTxAllocFnlz
 *     Purpose : 1. Reached here due to 1 RETX TB allocation for a
 *                  SM UE, which is capable to accomodate a newTX
 *                  in the other TB.
 *               2. Distribute NewTX TB allocation among the
 *                  SVCs present in lcsWithData list of UE.
 *
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlRetxNewTxAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHRrDlRetxNewTxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchDlHqProcCb *hqP;
   RgSchDlRbAlloc *dlRbAlloc;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   CmLList      *node;
   RgSchDlHqTbCb    *newTxTbInfo;
   U32              effAlloc;
   U32              remTbBytes;
   RgSchDlRbAlloc   *ueAllocInfo;
   RgSchCmnCell     *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell    *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);

   TRC2(rgSCHRrDlRetxNewTxAllocFnlz);

   node = allocInfo->dedAlloc.schdTxRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      dlRbAlloc = &(ue->dl.dlAllocCb);
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
      node = node->next;
      ueAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
      /* Index 0 of ueAllocInfo->tbInfo will always hold the
       * RETX TB and index 1 will hold the NewTX TB in case of
       * RETX+TX allocation. */
      newTxTbInfo = ueAllocInfo->tbInfo[1].tbCb;
      effAlloc = remTbBytes  =  ueAllocInfo->tbInfo[1].bytesAlloc;
      /* MS_FIX: syed List Corruption issue */
      rgSCHRrDlSprTxTbDstn(cell, ue, newTxTbInfo,\
            &remTbBytes, &rrDlUe->sigLst);
      rgSCHRrDlSprTxTbDstn(cell, ue, newTxTbInfo,\
            &remTbBytes, &rrDlUe->dataLst);

      /* MS_FIX : syed if UE has no new data to be scheduled as a result 
       * of rgSCHRrDlSprTxTbDstn, then remove UE from scheduling list */
      if((rrDlUe->sigLst.count == 0) &&
         (rrDlUe->dataLst.count == 0))
      {
         rgSCHRrCListDelFrm(&(rrDlCell->ueTxLst), &(rrDlUe->ueTxLnk));
         rrDlUe->ueTxLnk.node = NULLP;
      }
      /* Trying to reduce mcs of TX TB to increase reception quality at UE.
       * In case of RETX+TX allocation, TX TB allocation was irrespective
       * of actual requirement by UE, hence in case if consumption is
       * less than allocation, consider reducing the iMcs of this TX TB. */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 1, effAlloc - remTbBytes);
      rgSCHRrDlProcRmvFrmRetx(cell, hqP);

      rgSCHCmnFillHqPPdcch(cell, dlRbAlloc, hqP);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif

      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   node = allocInfo->dedAlloc.nonSchdTxRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      node = node->next;
      /* MS_FIX: syed To update Retx Alloc failure counter.
       * Need to do the same in other schedulers as well. */
      MSPD_DBG("UE(%d) PROC(%d) FAILED\n", ue->ueId, hqP->procId);
      rgSCHDhmDlRetxAllocFail(ue, hqP);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;

}

/**
 * @brief Dl allocations finalizations for the UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize dl allocations for re-transmission UEs by invoking
 *     rgSCHRrDlRetxUesAllocFnlz
 *     - Finalize dl allocations for new transmission UEs by invoking
 *     rgSCHRrDlUesAllocFnlz
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrDlAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   TRC2(rgSCHRrDlAllocFnlz);

   rgSCHRrDlRetxUesAllocFnlz(cell, allocInfo);
   rgSCHRrDlUesAllocFnlz(cell, allocInfo);
   /*3.1 MIMO new Function added to handle TX+RETX
    * harq process scheduling finalization */
   rgSCHRrDlRetxNewTxAllocFnlz(cell, allocInfo);

   RETVOID;
}

/**
 * @brief Indication of list of inactive UEs
 *
 * @details
 *
 *     Function: rgSCHRrDlInactvtUes
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  CmLListCp          *lst
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlInactvtUes
(
RgSchCellCb        *cell,
CmLListCp          *lst
)
#else
PUBLIC Void rgSCHRrDlInactvtUes(cell, lst)
RgSchCellCb        *cell;
CmLListCp          *lst;
#endif
{
   TRC2(rgSCHRrDlInactvtUes);
   RETVOID;
}


/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHRrDlInit
 *     Purpose:  This function initializes the DL RR SCH
 *
 *  @param[in]  RgDlSchdApis *rgSchDlApis
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlInit
(
RgDlSchdApis *rgSchDlApis
)
#else
PUBLIC Void rgSCHRrDlInit(rgSchDlApis)
RgDlSchdApis *rgSchDlApis;
#endif
{
   TRC2(rgSCHRrDlInit);

   /* Init the function pointers */
   rgSchDlApis->rgSCHRgrDlUeCfg       = rgSCHRrDlUeCfg;
   rgSchDlApis->rgSCHRgrDlUeRecfg     = rgSCHRrDlUeRecfg;
   rgSchDlApis->rgSCHFreeDlUe         = rgSCHRrDlUeDel;
   rgSchDlApis->rgSCHRgrDlCellCfg     = rgSCHRrDlCellCfg;
   rgSchDlApis->rgSCHRgrDlCellRecfg   = rgSCHRrDlCellRecfg;
   rgSchDlApis->rgSCHFreeDlCell       = rgSCHRrDlCellDel;
   rgSchDlApis->rgSCHRgrDlLcCfg       = rgSCHRrDlLcCfg;
   rgSchDlApis->rgSCHRgrDlLcRecfg     = rgSCHRrDlLcRecfg;
   rgSchDlApis->rgSCHFreeDlLc         = rgSCHRrDlLcDel;
   rgSchDlApis->rgSCHDlSched          = rgSCHRrDlSchd;
   rgSchDlApis->rgSCHDlDedBoUpd       = rgSCHRrDlDedBoUpd;
   rgSchDlApis->rgSCHDlProcAddToRetx  = rgSCHRrDlProcAddToRetx;
   rgSchDlApis->rgSCHDlAllocFnlz      = rgSCHRrDlAllocFnlz;
   rgSchDlApis->rgSCHDlCqiInd         = rgSCHRrDlCqiInd;
   rgSchDlApis->rgSCHDlUeRefresh      = rgSCHRrDlUeRefresh;
   rgSchDlApis->rgSCHDlActvtUe        = rgSCHRrDlActvtUe;
   rgSchDlApis->rgSCHDlUeReset        = rgSCHRrDlUeReset;
   rgSchDlApis->rgSCHDlInactvtUes     = rgSCHRrDlInactvtUes;
   rgSchDlApis->rgSCHDlUeHqEntInit    = rgSCHRrDlUeHqEntInit;
   rgSchDlApis->rgSCHDlUeHqEntDeInit  = rgSCHRrDlUeHqEntDeInit;

   RETVOID;
}

/**
 * @brief Indication of UL CQI for the UE
 *
 * @details
 *
 *     Function : rgSCHRrUlCqiInd
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  TfuUlCqiRpt         *ulCqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
)
#else
PUBLIC Void rgSCHRrUlCqiInd(cell, ue, ulCqiInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuUlCqiRpt          *ulCqiInfo;
#endif
{
   TRC2(rgSCHRrUlCqiInd);

   RETVOID;
}

/**
 * @brief Indication of activation of an UE after meas gap or ack/nack rep.
 *
 * @details
 *
 *     Function : rgSCHRrUlActvtUe
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlActvtUe
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrUlActvtUe(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlActvtUe);
   /* Inserting the activated UE as current node in the list if it got skiped in inactive period */
   if((rrUlUe->ueTxLnk.node != NULLP) && (rrUlUe->isUeSkipped == TRUE)&&
      (rrUlCell->ueTxLst.crnt != &(rrUlUe->ueTxLnk)))
   {
      rgSCHRrCListDelFrm( &(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
      rgSCHRrCListInsrtAtCrnt( &(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));  
   }
   /* Resetting the flag */
   rrUlUe->isUeSkipped = FALSE;
   RETVOID;
}

/**
 * @brief Indication of Ul Refresh for the UE.
 *
 * @details
 *
 *     Function: rgSCHRrUlUeRefresh
 *     Purpose:  RR does nothing.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlUeRefresh
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHRrUlUeRefresh(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   TRC2(rgSCHRrUlUeRefresh);

   RETVOID;
}

/**
 * @brief Cell configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlCellCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information per cell
 *      - Initialize ContRes and Tx lists
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgrCellCfg          *cfg
 *  @param[in]  RgSchErrInfo        *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlCellCfg
(
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHRrUlCellCfg(cell, cfg, err)
RgSchCellCb          *cell;
RgrCellCfg           *cfg;
RgSchErrInfo         *err;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell;
   TRC2(rgSCHRrUlCellCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnCell->ul.schSpfc), (sizeof(RgSchRrUlCell))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), "Memory allocation FAILED"));
      MSPD_ERR("Memory allocation FAILED \n");
	 RETVALUE(RFAILED);
   }

   rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);

   rgSCHRrCListInit(&(rrUlCell->contResLst));
   rgSCHRrCListInit(&(rrUlCell->ueTxLst));

   RETVALUE(ROK);
}

/**
 * @brief Cell reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlCellRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellRecfg      *recfg
 *  @param[in]  RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlCellRecfg
(
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo    *err
)
#else
PUBLIC S16 rgSCHRrUlCellRecfg(cell, recfg, err)
RgSchCellCb     *cell;
RgrCellRecfg    *recfg;
RgSchErrInfo    *err;
#endif
{
   TRC2(rgSCHRrUlCellRecfg);
   RETVALUE(ROK);
}


/**
 * @brief Cell deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlCellDel
 *
 *     Processing Steps:
 *      - Free Round Robin related information per cell.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlCellDel
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHRrUlCellDel(cell)
RgSchCellCb       *cell;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHRrUlCellDel);

   if(schCmnCell->ul.schSpfc)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(schCmnCell->ul.schSpfc)), (sizeof(RgSchRrUlCell)));
   }
   RETVOID;
}

/**
 * @brief UE configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlUeCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrUeCfg         *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlUeCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlUeCfg(cell, ue, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeCfg         *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe ;
   TRC2(rgSCHRrUlUeCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnUe->ul.schSpfc), (sizeof(RgSchRrUlUe))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), "Memory allocation FAILED"));
      MSPD_ERR("Memory allocation FAILED \n");
      RETVALUE(RFAILED);
   }
   rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
 
   /* Initialization */
   rrUlUe->isUeSkipped = FALSE;
   RETVALUE(ROK);
}

/**
 * @brief UE reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlUeRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]   RgSchCellCb      *cell
 *  @param[in]   RgSchUeCb        *ue
 *  @param[in]   RgrUeRecfg       *recfg
 *  @param[in]   RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlUeRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlUeRecfg(cell, ue, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeRecfg       *recfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrUlUeRecfg);

   RETVALUE(ROK);
}

/**
 * @brief UE delete for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlUeDel
 *
 *     Processing Steps:
 *     - Remove UE from contres list if present
 *     - Remove UE from tx list if present
 *     - Free Round Robin UE info
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlUeDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHRrUlUeDel(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlUeDel);

   if(schCmnUe->ul.schSpfc == NULLP)
   {
      RETVOID;
   }

   if(rrUlUe->ueTxLnk.node)
   {
      rgSCHRrCListDelFrm(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
      rrUlUe->ueTxLnk.node = NULLP; /* Added for avoiding dangling pointer */
   }
   if(rrUlUe->contResLnk.node)
   {
      rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
   (Data**)(&(schCmnUe->ul.schSpfc)), (sizeof(RgSchRrUlUe)));
   RETVOID;
}

/**
 * @brief Ul LCG configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlLcgCfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchUlLcgCb     *ulLcg
 *  @param[in]  RgrLcgCfg        *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlLcgCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg,
RgrLcgCfg        *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlLcgCfg(cell, ue, ulLcg, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *ulLcg;
RgrLcgCfg        *cfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrUlLcgCfg);

   RETVALUE(ROK);
}

/**
 * @brief Ul LC configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlLcgRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchUlLcgCb      *ulLcg
 *  @param[in]  RgrLcgRecfg      *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlLcgRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg,
RgrLcgRecfg      *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlLcgRecfg(cell, ue, ulLcg, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *ulLcg;
RgrLcgRecfg      *recfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrUlLcgRecfg);

   RETVALUE(ROK);
}

/**
 * @brief Check and Update UE scheduling status
 *
 * @details
 *
 *     Function : rgSCHRrUlUeChkAndUpdSchd
 *
 *     Processing Steps:
 *     - If the effective BSR > 0 and the UE is not already part of
 *       transmission queue then append the UE to the queue.
 *     - If the effective BSR is ZERO then remove the UE from the queue.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlUeChkAndUpdSchd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PRIVATE Void rgSCHRrUlUeChkAndUpdSchd(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlUeChkAndUpdSchd);

   if(schCmnUe->ul.effBsr)
   {
      if(rrUlUe->ueTxLnk.node == NULLP)
      {
         /* MS_FIX: syed remove from contResLst if UE already exists there. 
          * Without which we were scheduling twice for the same UE in a 
          * given TTI, which renders system unstable and finally to a crash
          * during UE DEL. Please check other schedulers for this. */
         if(rrUlUe->contResLnk.node != NULLP)
         {
            rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
            rrUlUe->contResLnk.node = NULLP;
         }
         rgSCHRrCListAdd2Tail(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
         rrUlUe->ueTxLnk.node = (PTR)ue;
         schCmnUeUlAlloc->reqLnk.node = NULLP;
      }
      else if(
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[0]) == FALSE) &&
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[1]) == FALSE) &&
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[2]) == FALSE) &&
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[3]) == FALSE)
             )
      {
         rgSCHRrCListDelFrm(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
         rrUlUe->ueTxLnk.node = NULLP;
      }
   }
   else
   {
      if((rrUlUe->ueTxLnk.node != NULLP) &&
            (rrUlUe->srRcvd == FALSE))
      {
         rgSCHRrCListDelFrm(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
         rrUlUe->ueTxLnk.node = NULLP;
      }
#ifdef RGR_V1
      /* rg006.201: [ccpu00112398] Added periodic BSR timer */
      if(schCmnUe->ul.totalBsr != 0)
      {
         if (ue->bsrTmr.tmrEvnt != TMR_NONE)
         {
            rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue);
         }
         if (ue->ul.bsrTmrCfg.isPrdBsrTmrPres)
         {
#ifdef MSPD
            MSPD_DBG("Starting timer for periodic BSR at [%d,%d]\n", cell->crntTime.sfn, cell->crntTime.subframe);
#endif
            rgSCHTmrStartTmr(cell, ue, RG_SCH_TMR_BSR,
                  ue->ul.bsrTmrCfg.prdBsrTmr);
         }
      }
#endif
   }

   RETVOID;
}


/**
 * @brief Ul LCG deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlLcgDel
 *     - RR does nothing.
 *
 *     Processing Steps:
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchUlLcgCb     *ulLcg
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlLcgDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg
)
#else
PUBLIC Void rgSCHRrUlLcgDel(cell, ue, ulLcg)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *ulLcg;
#endif
{
   TRC2(rgSCHRrUlLcgDel);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Short BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrUpdShortBsr
 *
 *     Processing Steps:
 *     - Check and update the UE scheduling status
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgSchLcgCb         *ulLcg
 *  @param[in]  U8                 bs
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUpdShortBsr
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *ulLcg,
U8                  bs
)
#else
PUBLIC Void rgSCHRrUpdShortBsr(cell, ue, ulLcg, bs)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchLcgCb          *ulLcg;
U8                  bs;
#endif
{
   TRC2(rgSCHRrUpdShortBsr);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Truncated BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrUpdTruncBsr
 *
 *     Processing Steps:
 *     - Check and update the UE scheduling status
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgSchLcgCb         *ulLcg
 *  @param[in]  U8                 bs
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUpdTruncBsr
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *ulLcg,
U8                  bs
)
#else
PUBLIC Void rgSCHRrUpdTruncBsr(cell, ue, ulLcg, bs)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchLcgCb          *ulLcg;
U8                  bs;
#endif
{
   TRC2(rgSCHRrUpdTruncBsr);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Long BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrUpdLongBsr
 *
 *     Processing Steps:
 *     - Check and update the UE scheduling status
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  U8                 *bsArr
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUpdLongBsr
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U8                  *bsArr
)
#else
PUBLIC Void rgSCHRrUpdLongBsr(cell, ue, bsArr)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
U8                  *bsArr;
#endif
{
   TRC2(rgSCHRrUpdLongBsr);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Indication of UL UE reset
 *
 * @details
 *
 *     Function : rgSCHRrUlUeReset
 *
 *     Processing Steps:
 *     - Remove the UE from contres list
 *     - Remove the UE from transmission list
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlUeReset
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrUlUeReset(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlUeReset);

   /* rg001.201 removed extra comments */
   /*Removing from re-transmission should
     be handled in CMN SCH
   rgSCHCmnUlUeDelAllocs(cell, ue);
   */

   rrUlUe->srRcvd = FALSE;

   if(rrUlUe->contResLnk.node != NULLP)
   {
      rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
      rrUlUe->contResLnk.node = NULLP;
      RETVOID;
   }

   if(rrUlUe->ueTxLnk.node != NULLP)
   {
      rgSCHRrCListDelFrm(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
      rrUlUe->ueTxLnk.node = NULLP;
      RETVOID;
   }

   /* Stack crash problem for TRACE5 Changes. added the return below */
  RETVOID;
}

/**
 * @brief SR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrSrInd
 *
 *     Processing Steps:
 *     - If UE is not part of transmission list then
 *       append the UE to the transmission list.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrSrInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHRrSrInd(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrSrInd);

   rrUlUe->srRcvd = TRUE;

   if(rrUlUe->ueTxLnk.node == NULLP)
   {
      /* MS_FIX: syed remove from contResLst if UE already exists there. 
       * Without which we were scheduling twice for the same UE in a 
       * given TTI, which renders system unstable and finally to a crash
       * during UE DEL. Please check other schedulers for this. */
      if(rrUlUe->contResLnk.node != NULLP)
      {
         rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
         rrUlUe->contResLnk.node = NULLP;
      }
      rgSCHRrCListAdd2Tail(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
      rrUlUe->ueTxLnk.node = (PTR)ue;
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   RETVOID;
}

/**
 * @brief Contention Resolution allocation request
 *
 * @details
 *
 *     Function : rgSCHRrContResUlGrant
 *
 *     Processing Steps:
 *     - Append the UE to the contention resolution queue.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrContResUlGrant
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHRrContResUlGrant(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrContResUlGrant);

   /* MS_FIX: syed Adding a check before adding to list. This
    * however did not create any issue. Just a precautionary measure.
    *  Please check other schedulers for this. */
   if (rrUlUe->contResLnk.node == NULLP)
   {
      rgSCHRrCListAdd2Tail(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
      rrUlUe->contResLnk.node = (PTR)ue;
      schCmnUeUlAlloc->reqLnk.node = NULLP;

      if(rrUlUe->ueTxLnk.node != NULLP)
      {
         rgSCHRrCListDelFrm(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
         rrUlUe->ueTxLnk.node = NULLP;
      }
   }
   RETVOID;
}

/**
 * @brief Scheduling of contention resolution UEs in the cell.
 *
 * @details
 *
 *     Function : rgSCHRrUlSchdContResUes
 *
 *     Processing Steps:
 *     - Traverse the contention resolution list and do resource
 *       allocation for the UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @param[in]  U8                     *remUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlSchdContResUes
(
RgSchCellCb            *cell,
RgSchCmnUlRbAllocInfo  *allocInfo,
U8                     *remUe
)
#else
PRIVATE Void rgSCHRrUlSchdContResUes(cell, allocInfo, remUe)
RgSchCellCb            *cell;
RgSchCmnUlRbAllocInfo  *allocInfo;
U8                     *remUe;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchUeCb  *ue;
   RgSchCmnUe *schCmnUe;
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc;
   RgSchRrCList *node;
#ifdef LTEMAC_HDFDD
   Bool                 ulAllowed = FALSE;
#endif
   TRC2(rgSCHRrUlSchdContResUes);
#ifndef MSPD
   if(rrUlCell->contResLst.crnt == NULLP)
   {
      RETVOID;
   }
#else
   /* MS_FIX:ccpu00120610 Fix to avaoid incorrect allocation */
   if(rrUlCell->contResLst.crnt == NULLP || !*remUe)
   {
      RETVOID;
   }
#endif

   node = rrUlCell->contResLst.crnt;
   do
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue,  &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      /* rg005.201 Use the macro to check if UE already picked for Retx */
      if (RG_SCH_CMN_IS_UL_UE_RETX(ue))
      {
         continue;
      }
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      schCmnUeUlAlloc->reqBytes = RG_RR_CONTRES_GRNT;

      cmLListAdd2Tail(&(allocInfo->contResLst),
                        &(schCmnUeUlAlloc->reqLnk));
      schCmnUeUlAlloc->reqLnk.node = (PTR)ue;
      (*remUe)--;
      if(*remUe == 0)
      {
         rrUlCell->contResLst.crnt = node;
         break;
      }

   }while(node != rrUlCell->contResLst.crnt);
   RETVOID;
}

/**
 * @brief Round Robin related scheduling of new-transmission for the UE
 *
 * @details
 *
 *     Function : rgSCHRrUlSchdUe
 *
 *     Processing Steps:
 *     - Schedule the UE and do resource allocation.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlSchdUe
(
RgSchCellCb       *cell,
RgSchUeCb         *ue
)
#else
PRIVATE Void rgSCHRrUlSchdUe(cell, ue)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
#endif
{
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlSchdUe);

   if(rrUlUe->srRcvd == TRUE)
   {
      if(schCmnUe->ul.effBsr == 0)
      {
         schCmnUeUlAlloc->reqBytes = RG_RR_SR_GRNT;
      }
      else
      {
         schCmnUeUlAlloc->reqBytes = schCmnUe->ul.effBsr;
      }
   }
   else
   {
      schCmnUeUlAlloc->reqBytes = schCmnUe->ul.effBsr;
   }
   RETVOID;
}

/**
 * @brief Round Robin related scheduling of new-transmission for the UEs
 *
 * @details
 *
 *     Function : rgSCHRrUlSchdUes
 *
 *     Processing Steps:
 *     - Traverse the new transmission list and do resource allocation
 *       for the UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @param[in]  U8                     remUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlSchdUes
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo,
U8                      remUe
)
#else
PRIVATE Void rgSCHRrUlSchdUes(cell, allocInfo, remUe)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
U8                      remUe;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchUeCb *ue;
   RgSchCmnUe *schCmnUe;
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc;
   RgSchRrCList *node;
   RgSchUlHqProcCb  *proc;
#ifdef LTEMAC_HDFDD
   Bool              ulAllowed = FALSE;
#endif
   TRC2(rgSCHRrUlSchdUes);
#ifndef MSPD
   if(rrUlCell->ueTxLst.crnt == NULLP)
   {
      RETVOID;
   }
#else
   /* MS_FIX:ccpu00120610 Fix to avaoid incorrect allocation */
   if(rrUlCell->ueTxLst.crnt == NULLP || !remUe)
   {
      RETVOID;
   }
#endif
   node = rrUlCell->ueTxLst.crnt;
   do
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue, &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      proc = &(ue->ul.hqEnt.hqProcCb[schCmnCell->ul.schdHqProcIdx]);
      /* rg005.201 Use the macro to check if UE already picked for Retx */
      /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
      if ((ue->ul.ulInactvMask) || RG_SCH_CMN_IS_UL_UE_RETX(ue) || RG_SCH_CMN_IS_SPS_SCHD(ue))
#else
      if((ue->ul.ulInactvMask) ||
         (RG_SCH_CMN_IS_UL_UE_RETX(ue)))
#endif
      {
         RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
#ifdef MSPD
         MSPD_DBG("Retransmitting  %d or inactive %d @ (%d,%d) remTx %d\n",
		 	 RG_SCH_CMN_IS_UL_UE_RETX(ue), ue->ul.ulInactvMask,
		 	 cell->crntTime.sfn, cell->crntTime.subframe, proc->remTx);
#endif
         rrUlUe->isUeSkipped = TRUE;
         continue;
      }
      rgSCHRrUlSchdUe(cell, ue);

      cmLListAdd2Tail(&(allocInfo->ueLst),
                        &(schCmnUeUlAlloc->reqLnk));
      schCmnUeUlAlloc->reqLnk.node = (PTR)ue;
      remUe--;
      if(remUe == 0)
      {
         /* MS_FIX [ccpu00126846]: Taking care of RR behaviour during finalization */
         /* rrUlCell->ueTxLst.crnt = node; */
         break;
      }

   }while(node != rrUlCell->ueTxLst.crnt);
   RETVOID;
}

/**
 * @brief Round Robin scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHRrUlNewTx
 *
 *     Processing Steps:
 *     - Schedule the contention resolution UEs by invoking
 *       rgSCHRrUlSchdContResUes
 *     - Schedule the new-transmission UEs by invoking
 *       rgSCHRrUlSchdUes
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlNewTx
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrUlNewTx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   U8 remUe = schCmnCell->ul.maxUeNewTxPerTti;
#ifdef MSPD
   /* MS_FIX ccpu00120610*/
   U8 remAllocs = schCmnCell->ul.maxAllocPerUlSf - *allocInfo->sf->allocCountRef;
#endif
   TRC2(rgSCHRrUlNewTx);
#ifdef  MSPD
   /* MS_FIX:ccpu00120610 consider remaining allocs in a subframe, because allocs may
    * have been consumed earlier as part of msg3 allocation.
    * Note: this needs to be carried over to other schedulers. */
   if (remUe > remAllocs)
   {
      remUe = remAllocs;
   }
   /* MS_WORKAROUND : Limiting one PUSCH per UL SF */
/*Removing the WA as fixed to control from configuration*/
#endif
   rgSCHRrUlSchdContResUes(cell, allocInfo, &remUe);
   rgSCHRrUlSchdUes(cell, allocInfo, remUe);
#ifdef  MSPD
   if (*allocInfo->sf->allocCountRef > schCmnCell->ul.maxAllocPerUlSf)
   {
       MSPD_DBG("More than expecte UEs per TTI allocated in UL exp(%u)"
                " allocated(%u)\n",
                schCmnCell->ul.maxAllocPerUlSf, *allocInfo->sf->allocCountRef);
   }
#endif
   RETVOID;
}

/**
 * @brief Round Robin scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHRrUlSchd
 *
 *     Processing Steps:
 *     - Schedule the UEs and do resource allocation.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlSchd
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrUlSchd(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   TRC2(rgSCHRrUlSchd);

   rgSCHRrUlNewTx(cell, allocInfo);

   RETVOID;
}

/**
 * @brief Ul allocation finalization for the contresolution UEs.
 *
 * @details
 *
 *     Function : rgSCHRrUlContResUesAllocFnlz
 *
 *     Processing Steps:
 *     - If the allocation succeded then remove from contres list.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlContResUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrUlContResUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchUeCb    *ue;
   RgSchCmnUe *schCmnUe;
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc;
   RgSchRrUlUe  *rrUlUe;
   CmLList      *node;
   TRC2(rgSCHRrUlContResUesAllocFnlz);

   node = allocInfo->schdContResLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
      node = node->next;
      rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
      rrUlUe->contResLnk.node = NULLP;
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   node = allocInfo->nonSchdContResLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   RETVOID;
}


/**
 * @brief Ul allocation finalization for the new-transmission UE.
 *
 * @details
 *
 *     Function : rgSCHRrUlUeAllocFnlz
 *
 *     Processing Steps:
 *     - If allocation succeded and effective BSR is ZERO
 *       then remove UE from the transmission list.
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef LTEMAC_DRX
#ifdef ANSI
PRIVATE Void rgSCHRrUlUeAllocFnlz
(
RgSchCellCb       *cell,
RgSchUeCb         *ue,
CmLListCp         *ulInactvLst
)
#else
PRIVATE Void rgSCHRrUlUeAllocFnlz(cell, ue, ulInactvLst)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
CmLListCp         *ulInactvLst;
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHRrUlUeAllocFnlz
(
RgSchCellCb       *cell,
RgSchUeCb         *ue
)
#else
PRIVATE Void rgSCHRrUlUeAllocFnlz(cell, ue)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
#endif
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
#ifdef LTEMAC_DRX
   RgSchDrxUeCb      *drxUe    = NULLP;
#endif
   TRC2(rgSCHRrUlUeAllocFnlz);

   if(rrUlUe->srRcvd == TRUE)
   {
#ifdef LTEMAC_DRX
      if (ue->isDrxEnabled)
      {
         drxUe = RG_SCH_DRX_GET_UE(ue);
         drxUe->drxUlInactvMask  |= RG_SCH_DRX_SR_BITMASK;
         if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
         {
            ue->ul.ulInactvMask |= RG_DRX_INACTIVE;
            /* Add to Ul inactive List */
            ue->ulDrxInactvLnk.node  = (PTR)ue;
            cmLListAdd2Tail(ulInactvLst,&(ue->ulDrxInactvLnk));
         }
         drxUe->srRcvd = FALSE;
      }
#endif
      rrUlUe->srRcvd = FALSE;
   }

   if(schCmnUe->ul.effBsr == 0)
   {
      rgSCHRrCListDelFrm(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
      rrUlUe->ueTxLnk.node = NULLP;
   }
   /* MS_FIX [ccpu00126846]: Taking care of RR behaviour during finalization to 
      avoid starvation of previously non-scheduled UEs */
   else
   {
      rgSCHRrCListDelFrm(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
	  rgSCHRrCListAdd2Tail(&(rrUlCell->ueTxLst), &(rrUlUe->ueTxLnk));
   }
   RETVOID;
}

/**
 * @brief Ul allocation finalization for the new-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrUlUesAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize the Ul allocations for the UEs by invoking
 *       rgSCHRrUlUeAllocFnlz
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrUlUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchCmnUe *schCmnUe;
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc;
   CmLList      *node;

#ifdef LTEMAC_DRX
   CmLListCp         ulInactvLst; /* list of UE's becoming UL-inactive */
#endif
   TRC2(rgSCHRrUlUesAllocFnlz);

#ifdef LTEMAC_DRX
   cmLListInit(&ulInactvLst);
#endif
   node = allocInfo->schdUeLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
#ifdef LTEMAC_DRX
      rgSCHRrUlUeAllocFnlz(cell, ue, &ulInactvLst);
#else
      rgSCHRrUlUeAllocFnlz(cell, ue);
#endif
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
#ifdef LTEMAC_DRX
   rgSCHRrUlInactvtUes(cell, &ulInactvLst);
#endif
   node = allocInfo->nonSchdUeLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   RETVOID;
}

/**
 * @brief Ul allocation finalization for the new-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrUlAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize the Ul allocations for the contres UEs by invoking
 *       rgSCHRrUlContResUesAllocFnlz
 *     - Finalize the Ul allocations for the new-transmission UEs by
 *       invoking rgSCHRrUlUesAllocFnlz
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrUlAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   TRC2(rgSCHRrUlAllocFnlz);

   rgSCHRrUlContResUesAllocFnlz(cell, allocInfo);
   rgSCHRrUlUesAllocFnlz(cell, allocInfo);

   RETVOID;
}

/**
 * @brief Indication of list of inactive UEs
 *
 * @details
 *
 *     Function: rgSCHRrUlInactvtUes
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  CmLListCp          *lst
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlInactvtUes
(
RgSchCellCb        *cell,
CmLListCp          *lst
)
#else
PUBLIC Void rgSCHRrUlInactvtUes(cell, lst)
RgSchCellCb        *cell;
CmLListCp          *lst;
#endif
{
   TRC2(rgSCHRrUlInactvtUes);

   RETVOID;
}


/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHRrUlInit
 *     Purpose:  This function initializes the UL RR SCH
 *
 *  @param[in]  RgDlSchdApis *rgSchDlApis
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlInit
(
RgUlSchdApis *rgSchUlApis
)
#else
PUBLIC Void rgSCHRrUlInit(rgSchUlApis)
RgUlSchdApis *rgSchUlApis;
#endif
{
   TRC2(rgSCHRrUlInit);

   /* Init the function pointers */
   rgSchUlApis->rgSCHRgrUlUeCfg     = rgSCHRrUlUeCfg;
   rgSchUlApis->rgSCHRgrUlUeRecfg   = rgSCHRrUlUeRecfg;
   rgSchUlApis->rgSCHFreeUlUe       = rgSCHRrUlUeDel;
   rgSchUlApis->rgSCHRgrUlCellCfg   = rgSCHRrUlCellCfg;
   rgSchUlApis->rgSCHRgrUlCellRecfg = rgSCHRrUlCellRecfg;
   rgSchUlApis->rgSCHFreeUlCell     = rgSCHRrUlCellDel;
   rgSchUlApis->rgSCHRgrUlLcgCfg    = rgSCHRrUlLcgCfg;
   rgSchUlApis->rgSCHRgrUlLcgRecfg  = rgSCHRrUlLcgRecfg;
   rgSchUlApis->rgSCHFreeUlLcg      = rgSCHRrUlLcgDel;
   rgSchUlApis->rgSCHUlSched        = rgSCHRrUlSchd;
   rgSchUlApis->rgSCHUpdBsrShort    = rgSCHRrUpdShortBsr;
   rgSchUlApis->rgSCHUpdBsrTrunc    = rgSCHRrUpdTruncBsr;
   rgSchUlApis->rgSCHUpdBsrLong     = rgSCHRrUpdLongBsr;
   rgSchUlApis->rgSCHContResUlGrant = rgSCHRrContResUlGrant;
   rgSchUlApis->rgSCHSrRcvd         = rgSCHRrSrInd;
   rgSchUlApis->rgSCHUlCqiInd       = rgSCHRrUlCqiInd;
   rgSchUlApis->rgSCHUlUeRefresh    = rgSCHRrUlUeRefresh;
   rgSchUlApis->rgSCHUlAllocFnlz    = rgSCHRrUlAllocFnlz;
   rgSchUlApis->rgSCHUlActvtUe      = rgSCHRrUlActvtUe;
   rgSchUlApis->rgSCHUlUeReset      = rgSCHRrUlUeReset;
   rgSchUlApis->rgSCHUlInactvtUes   = rgSCHRrUlInactvtUes;

   RETVOID;
}

/**********************************************************************

         End of file:     gk_sch_rr.c@@/main/2 - Sat Jul 30 02:21:48 2011

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
/main/1      ---     rr   1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments.
             rg003.201 ns 1. Trace added to functions.
             rg004.201 ms 1. DL DTX Handling
                          2. Functions and Datatypes starting
                             with rr are renamed as rgSchRr
           rg005.201 sm      1. Added Macro to check if UE is picked for Retx
           rg006.201 sd   1. ccpu00110358: Removed ndi toggling post release
                             of HARQ proc.
                          2. ccpu00112398: Added periodic BSR timer
           rg007.201   ap  1. Added support for MIMO
           rg008.201 sd   1. Removed dependency on MIMO compile-time flag
              rsharon 2. Added support for SPS.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
           rg001.301 nudupi  1. ccpu00118350:Correcting NDI manipulation of Harq.
           rg002.301  rntiwari 1.ccpu00120941: Added handling for releasing HARQ
                                 proc, deleting UE from txUeLst and resetting
                                 outStndAlloc.
*********************************************************************91*/
