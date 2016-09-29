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
  
     Desc:     C source code for scheduler 1
  
     File:     rg_sch_sc1.c
  
     Sid:      gk_sch_sc1.c@@/main/3 - Sat Jul 30 02:21:49 2011
  
     Prg:      apany
  
**********************************************************************/

/** @file rg_sch_sc1.c
@brief The scheduling functionality is implemented in this file.
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
#include "rg_sch_sc1.h"

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
#include "rg_sch_sc1.x"    /* typedefs for SC1 Scheduler */




#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Functions called from outside */
PRIVATE S16 rgSCHSc1RgrDlCellRecfg ARGS((
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
));

/*--------------------------*
 * DL SCHED STATIC declarations START
 *---------------------------*/
PRIVATE Void rgSCHSc1DlSvcAddToSchd ARGS((
RgSchCellCb                *cell,
RgSchDlLcCb                *svc
));
PRIVATE Void rgSCHSc1DlAdd2UeSchdSvcs ARGS((
RgSchUeCb          *ue,
RgSchDlLcCb        *svc
));
PRIVATE Void rgSCHSc1DlRmvUeFrmPrioQs ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
PRIVATE Void rgSCHSc1DlSuspendUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
PRIVATE Void rgSCHSc1DlInactvtUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
PRIVATE Void rgSCHSc1DlProcRmvFrmCellRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
PRIVATE Void rgSCHSc1DlProcRmvFrmUeRetx ARGS((
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *hqP
));
PRIVATE Void rgSCHSc1DlMngPrio0SvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
PRIVATE Void rgSCHSc1DlMngGbrSvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
PRIVATE Void rgSCHSc1DlMngAmbrSvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
PRIVATE Void rgSCHSc1DlMngSvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
PRIVATE Void rgSCHSc1DlUeAddToSchd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
PRIVATE Void rgSCHSc1DlTaCmd ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHSc1DlInitQueues ARGS((
RgSchSc1DlCell *cellDl
));
PRIVATE Void rgSCHSc1DlDeinitQueues ARGS((
RgSchSc1DlCell *cellDl
));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
PRIVATE Void rgSCHSc1DlAdd2UeLcsWithData ARGS((
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
PRIVATE Void rgSCHSc1DlRmFrmUeLcsWithData ARGS((
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
/*--------------------------*
 * UL SCHED STATIC declarations START
 *---------------------------*/
PRIVATE Void rgSCHSc1UlPosnUeInQ ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
PRIVATE Void rgSCHSc1UlSchdUeTxLst ARGS((
RgSchCellCb         *cell,
CmLListCp             *ueTxLst,
RgSchCmnUlRbAllocInfo *allocInfo,
U8                    *remUe
));

#ifdef __cplusplus
}
#endif /* __cplusplus */




/***************** SC1 DL SCHEDULER FUNCTION DEFNs START HERE ********/

/***********************************************************
 *
 *     Func : rgSCHSc1DlUeReset 
 *        
 *     Desc : Out of Meas Gap. Reposition the UEs Retx Hq Procs,
 *            and Svc in respective Prio Qs.
 *            
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlUeReset 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHSc1DlUeReset(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlUeReset);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlUeReset\n"));

   rgSCHSc1DlSuspendUe(cell, ue);

   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHSc1DlActvtUe
 *        
 *     Desc : Out of Meas Gap. Reposition the UEs Retx Hq Procs,
 *            and Svc in respective Prio Qs.
 *            
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlActvtUe 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHSc1DlActvtUe(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   RgSchSc1DlUe    *ueDl = RG_GET_SC1_UE_DL(ue);
   CmLListCp       *lst;
   CmLList         *node;
   RgSchDlHqProcCb *hqP;
   RgSchDlLcCb     *svc;
   U8              idx;
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlActvtUe);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlActvtUe\n"));

   /* Add UE's HqProcs From UERetxLst to CellRetxLst */
   lst = &ueDl->retxHqProcs;
   node = lst->first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      node = node->next;
      rgSCHSc1DlProcRmvFrmUeRetx(ue, hqP);
      rgSCHSc1DlProcAddToCellRetx(cell, hqP);
   }

   /* Iterate over all the Services if bo != 0 then add */
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      svc = ue->dl.lcCb[idx];
      if (svc == NULLP)
      {
         continue;
      }
      rgSCHSc1DlMngSvcPosn(cell, ue, svc);
   } 

   /* Add UE to AMBR Prio Q */
   if (ueDl->ambrSvc)
   {
      rgSCHSc1DlUeAddToSchd(cell, ue);
   }

   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHSc1DlUeRefresh
 *        
 *     Desc : Handle 'refresh' for Downlink
 *            (ie UE's downlink AMBR and downlink GBR LCGs are
 *            refreshed at this point)
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlUeRefresh
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHSc1DlUeRefresh(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   RgSchSc1DlUe   *ueDl = RG_GET_SC1_UE_DL(ue);
   RgSchCmnDlUe *ueCmnDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnDlSvc *svcCmn;
   RgSchSc1DlSvc  *svcSc1;
   CmLListCp   *lst;
   CmLList     *node;
   RgSchDlLcCb *svc;
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlUeRefresh);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlUeRefresh\n"));

   if (ueCmnDl->ambrCfgd)
   {
      ueDl->ambr = ueCmnDl->ambrCfgd;
   }
   else
   {
      ueDl->ambr = RG_SC1_MAX_DL_AMBR;
   }

   if (ueDl->ambrSvc != NULLP)
   {
      ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, ueDl->ambrSvc->bo);
      /* Update UEs position in the Queue */
      rgSCHSc1DlUeAddToSchd(cell, ue);
   }

   lst = &ueDl->gbrSvcs;
   node = lst->first;
   while (node != NULLP)
   {
      svc   = (RgSchDlLcCb *)node->node;
      svcSc1 = RG_GET_SC1_SVC_DL(svc);
      svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
      node = node->next;
      svcSc1->gbr   = svcCmn->gbr;
      svcSc1->mbr   = svcCmn->mbr;
      /* Update the SVC's positioning in the Queue */
      rgSCHSc1DlMngGbrSvcPosn(cell, ue, svc);
   }
   RETVOID;
}


/**
 * @brief This function removes a HARQ process from the retx
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcRmvFrmCellRetx
 *     Purpose:  This function removes a HARQ process from retransmission
 *               queue. This may be performed when a HARQ ack is successful
 *               for a retransmission or when the scheduling determines
 *               to throw out the process due to poor conditions
 *     
 *     Invoked by: LIM and Scheduler
 *     
 *  @param[in]  RgSchSc1Cb*      cell
 *  @param[in]  RgDlHqProc*   hqP  
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlProcRmvFrmCellRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PRIVATE Void rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchSc1DlCell       *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchCmnDlHqProc     *hqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlProcRmvFrmCellRetx);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlProcRmvFrmCellRetx\n"));

   if (hqProcDl->retxLnk.node != NULLP)
   {
      cmLListDelFrm(&cellDl->retxLst[((RgSchSc1DlHqProc *)\
                    (hqProcDl->schSpfc))->prio], &(hqProcDl->retxLnk));
      hqProcDl->retxLnk.node = NULLP;
   }
   RETVOID;
}


/**
 * @brief This function removes a HARQ process from the UE retx
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcRmvFrmUeRetx
 *     Purpose:  This function removes a HARQ process from UE retransmission
 *               queue. 
 *     
 *     Invoked by: LIM and Scheduler
 *     
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgDlHqProc*   hqP  
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlProcRmvFrmUeRetx
(
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *hqP
)
#else
PRIVATE Void rgSCHSc1DlProcRmvFrmUeRetx(ue, hqP)
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchSc1DlUe            *sc1Ue = RG_GET_SC1_UE_DL(ue);
   RgSchSc1DlHqProc     *hqProcDl = RG_GET_SC1_HQP_DL(hqP);
#ifdef DEBUGP
   Inst                 inst = ue->cell->instIdx;
#endif

   TRC2(rgSCHSc1DlProcRmvFrmUeRetx);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlProcRmvFrmUeRetx\n"));

   if (hqProcDl->retxLnkUe.node != NULLP)
   {
      cmLListDelFrm(&sc1Ue->retxHqProcs,
                  &(hqProcDl->retxLnkUe));
      hqProcDl->retxLnkUe.node = NULLP;
   }
   RETVOID;
}


/**
 * @brief This function adds a HARQ process for UEs retxLst
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcAddToUeRetx
 *     Purpose:  This function adds a HARQ process to UE retransmission
 *               queue. This is performed when UE is suspended due
 *               to measurement gap.
 *     
 *     Invoked by: HARQ feedback processing
 *     
 *  @param[in]  RgSchUeCb*       ue
 *  @param[in]  RgSchDlHqProc*   hqP  
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlProcAddToUeRetx
(
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *hqP
)
#else
PRIVATE Void rgSCHSc1DlProcAddToUeRetx(ue, hqP)
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchSc1DlUe            *sc1Ue = RG_GET_SC1_UE_DL(ue);
   RgSchSc1DlHqProc     *cmnHqDl = RG_GET_SC1_HQP_DL(hqP);
#ifdef DEBUGP
   Inst                 inst = ue->cell->instIdx;
#endif

   TRC2(rgSCHSc1DlProcAddToUeRetx);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlProcAddToUeRetx\n"));

   cmLListAdd2Tail(&sc1Ue->retxHqProcs, 
                    &(cmnHqDl->retxLnkUe));
   cmnHqDl->retxLnkUe.node = (PTR)hqP;
   RETVOID;
}


/**
 * @brief This function adds a HARQ process for retx
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcAddToCellRetx
 *     Purpose:  This function adds a HARQ process to retransmission
 *               queue. This may be performed when a HARQ ack is
 *               unsuccessful.
 *     
 *     Invoked by: HARQ feedback processing
 *     
 *  @param[in]  RgSchCellCb*     cell 
 *  @param[in]  RgSchDlHqProc*   hqP  
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlProcAddToCellRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHSc1DlProcAddToCellRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchSc1DlCell          *sc1CellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchCmnDlHqProc     *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlProcAddToCellRetx);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlProcAddToCellRetx\n"));

   if (!RG_SCH_CMN_DL_IS_UE_ACTIVE(hqP->hqE->ue))
   {
      rgSCHSc1DlProcAddToUeRetx(hqP->hqE->ue, hqP);
      RETVOID;
   }
   cmLListAdd2Tail(&sc1CellDl->retxLst[((RgSchSc1DlHqProc *)\
                  (cmnHqDl->schSpfc))->prio], &(cmnHqDl->retxLnk));
   cmnHqDl->retxLnk.node = (PTR)hqP;
   RETVOID;
}


/**
 * @brief This function implements DL RETRANSMISSION allocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlRetxAlloc
 *     Purpose:  This function implements downlink scheduler's
 *               retransmission allocation.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlRetxAlloc
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else
PRIVATE Void rgSCHSc1DlRetxAlloc(cell, subFrm, allocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif
{
   U8                   i;
   CmLListCp            *retxLst;
   CmLList              *node;
   RgSchDlHqProcCb      *hqP;
   RgSchSc1DlCell       *sc1CellDl;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchSc1DlUe         *sc1DlUe;
   RgSchCmnDlUe         *cmnUeDl;
   /*rg008.201 - Added support for SPS*/
#if (defined(LTEMAC_SPS) || (!defined(LTE_TDD)))
   CmLteTimingInfo      schdTime;
#endif
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   U32                  effBo;
   RgSchUeCb            *ue = NULLP;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   TRC2(rgSCHSc1DlRetxAlloc);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlRetxAlloc\n"));

   sc1CellDl = RG_GET_SC1_CELL_DL(cell);
   /*rg008.201 - Added for SPS support*/
#if (defined(LTEMAC_SPS) || (!defined(LTE_TDD)))
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif
   for (i = 0; i < RG_SCH_CMN_DL_PRIOS; i++)
   {
      retxLst = &sc1CellDl->retxLst[i];
      /* allocate bw for the retransmission..should be same are previous */
      /* If CQI gets worse, as we cannot find same TB size for another   */
      /* MCS, we just remove this from the retransmission queue          */
      node = retxLst->first;
      while (node != NULLP)
      {
         hqP = (RgSchDlHqProcCb *)node->node;
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
         node = node->next;
         ue = hqP->hqE->ue;

#ifndef LTE_TDD
         if((0 == schdTime.subframe) || (5 == schdTime.subframe))
         {
            Bool reTxAllw;
            rgSCHCmnChkRetxAllowDtx(cell, ue, hqP, &reTxAllw);
            if(FALSE == reTxAllw)
            {
               continue;
            }
         }
#endif
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
         /* This UE is already scheduled for transmission */
         cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
    /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
         if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
         {
            RGSCHDBGINFO(inst, (rgSchPBuf(inst), "\n r"
                     "Ue already scheduled by SPS module %d", ue->ueId));
            continue;
         }
#endif
         if (RG_SCH_CMN_IS_UE_SCHDLD(ue))
         {
            RGSCHDBGINFO(inst, (rgSchPBuf(inst), "\nrgSCHSc1DlRetxAlloc(): \
                     UE is transmitting."));
            continue;
         }
         effBo = 0;
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
         /* Extra check: indicate if there is furtherScope for NewTx
          * addition for a HqProc. This information will
          * be utilized by common scheduler, in case of SM
          * UEs with only one of the TBs retransmitting and the 
          * other TB can be used for clubbing new TX. */
         sc1DlUe = RG_GET_SC1_UE_DL(ue);
         if (sc1DlUe->lcsWithData.first != NULLP)
         {
            ue->dl.dlAllocCb.mimoAllocInfo.hasNewTxData = TRUE; 
         }
         /* 3.1 MIMO : last parameter changed */
         if (rgSCHCmnDlAllocRetxRb(cell, subFrm, ue, 0, &effBo, hqP, allocInfo) !=\
               ROK)
         {
            /* SF/RETX Bandwidth expired */
            RETVOID;
         }
         if (effBo == 0)
         {
            continue;
         }

         /* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
          * indefinite time in case of Retx */
         if ((hqP->tbInfo[0].state == HQ_TB_ACKED)
            && (hqP->tbInfo[1].state == HQ_TB_ACKED))
         {
            rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
            RETVOID;
         }

         cmnUeDl->proc = hqP;
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
         /* rg007.201 - Changes for MIMO feature addition */
         /* rg008.201 - Removed dependency on MIMO compile-time flag */
         /* 3.1 MIMO moving this call in cmn scheduler */
         /*rgSCHCmnDlRbInfoAddUeRetx(allocInfo, ue);*/
      }
   }
   RETVOID;
}

/* rg001.301 -MOD -[ccpu00118350] Correcting NDI manipulation of Harq */
/***********************************************************
 *
 *     Func : rgSCHSc1RlsHqProc
 *        
 *     Desc : Toggles the NDI and releases the harq proc.
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHSc1RlsHqProc
(
RgSchDlHqProcCb      *hqProc
)
#else
PRIVATE Void rgSCHSc1RlsHqProc(hqProc)
RgSchDlHqProcCb      *hqProc;
#endif
{
   TRC2(rgSCHSc1RlsHqProc)
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   rgSCHDhmRlsHqProc(hqProc);
   RETVOID;
}

/**
 * @brief This function implements dedicated logical channel data scheduling
 *
 * @details
 *
 *     Function: rgSCHSc1DlDedSvcAlloc 
 *     Purpose:  This function implements dedicated logical 
 *               channel data scheduling 
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchDlSf              *subFrm
 *  @param[in]  RgSchDlLcCb            *svc
 *  @param[in]  U32                    bo
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  S16 
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHSc1DlDedSvcAlloc
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchDlLcCb                *svc,
U32                        bo,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else
PRIVATE S16 rgSCHSc1DlDedSvcAlloc(cell, subFrm, svc, bo, allocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchDlLcCb                *svc;
U32                        bo;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif
{
   RgSchUeCb               *ue;
   RgSchDlHqProcCb         *proc;
   U16                     rlcHdrEstmt;
   U32                     effBo;
   RgSchCmnDlCell          *cmnCellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlSvc           *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchSc1DlSvc           *svcSc1;
   RgSchCmnDlUe            *ueDl;
   RgSchSc1DlHqProc        *sc1HqDl;
   RgSchCmnDlHqProc        *cmnHqDl;
#ifdef DEBUGP
   Inst                    inst = cell->instIdx;
#endif
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteTimingInfo      schdTime;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   S16                  ret;

   TRC2(rgSCHSc1DlDedSvcAlloc);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlDedSvcAlloc\n"));

   /* Get the UE to which this service belongs to */
   ue = svc->ue;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            RETVALUE(ROK);
         }
      }
#endif
   ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
   if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "\n r"
               "Ue already scheduled by SPS module %d", ue->ueId));
      RETVALUE(ROK);
   }
#endif
   /* rg002.101:ccpu00102106: correcting DL harq softbuffer limitation logic*/
   if (RG_SCH_CMN_IS_UE_SCHDLD(ue))
   {
      proc = (RgSchDlHqProcCb *)(ueDl->proc);
      /* This UE is selected for retransmission. Hence no further */
      /* scheduling may be done for this UE                       */
      if (RG_SCH_CMN_PROC_SLCTD_FOR_RETX(proc))
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "\nnrgSCHSc1DlDedSvcAlloc(): \
                  Ue retransmitting."));
         MSPD_ERR(" UE: %d retransmitting \n", ue->ueId);
         RETVALUE(ROK);
      }
      /* UE is scheduled for either other services or TA */
      sc1HqDl = RG_GET_SC1_HQP_DL(proc);
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);
      if (sc1HqDl->prio > svcCmn->prio)
      {
         sc1HqDl->prio = svcCmn->prio;
      }
   }
   else /* First consideration of this UE for scheduling */
   {
      if (rgSCHDhmGetAvlHqProc(ue, cmnCellDl->time, &proc) != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "\nnrgSCHSc1DlDedSvcAlloc(): \
                  No Hq Proc Avl."));
        MSPD_ERR(" No HARQ Proc available for UE: %d  \n", ue->ueId);
         RETVALUE(ROK);
      }
      sc1HqDl = RG_GET_SC1_HQP_DL(proc);
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      cmnHqDl->totBytes = 0;
      /* Initialize some of the parameters of the HQ proc */
      sc1HqDl->prio     = svcCmn->prio;
   }

   /* Including each SDU's header size */
   RG_SCH_CMN_DL_GET_HDR_EST(svc, rlcHdrEstmt);
   bo += rlcHdrEstmt;
   effBo = 0;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* rg003.301[ccpu00122038]-MOD- Check for return value */
   ret = rgSCHCmnDlAllocTxRb(cell, subFrm, ue, bo, &effBo, proc, allocInfo);
   if ((ret != ROK) || (effBo == 0))
   {
      /* If no allocations so far, meaning proc obtained now */
      if (cmnHqDl->totBytes == 0)
      {
      /* rg001.301 -MOD -[ccpu00118350] Correcting NDI manipulation of Harq */
         rgSCHSc1RlsHqProc(proc);
         /*rg002.301 - ADD -[ccpu00120941] Added the handling for removing
          * UE from txUeLst and resetting outStndAlloc.*/
         if(proc->reqLnk.node != (PTR)NULLP)
         {
            cmLListDelFrm(&allocInfo->dedAlloc.txUeLst, &proc->reqLnk);
            proc->reqLnk.node = (PTR)NULLP;
         }
         /*Re-set the outstanding alloc information.*/
         ueDl->outStndAlloc = 0;
      }
      /* rg003.301[ccpu00122038]-MOD- Return the value returned by DlAllocTxRb */
      RETVALUE(ret);
   }
   svcSc1 = RG_GET_SC1_SVC_DL(svc);
   svcSc1->hdrEstimate = rlcHdrEstmt;
   svcSc1->reqBytes = bo;
   ueDl->proc = proc;
   cmnHqDl->totBytes += effBo;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */

   rgSCHSc1DlAdd2UeSchdSvcs(ue, svc);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* 3.1 MIMO moving this call to cmn scheduler */
   /*rgSCHCmnDlRbInfoAddUeTx(allocInfo, ue); */
   RETVALUE(ROK);
}

/**
 * @brief This function adds a SVC to UE's schdSvcsLst. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlAdd2UeSchdSvcs
 *     Purpose:  This function adds a SVC to UE's schdSvcsLst. 
 *     
 *     Invoked by: Specific Scheduler 
 *     
 *  @param[out] RgSchUeCb          *ue
 *  @param[in]  RgSchDlLcCb        *svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlAdd2UeSchdSvcs
(
RgSchUeCb          *ue,
RgSchDlLcCb        *svc
)
#else
PRIVATE Void rgSCHSc1DlAdd2UeSchdSvcs(ue, svc)
RgSchUeCb          *ue;
RgSchDlLcCb        *svc;
#endif
{
   RgSchSc1DlSvc  *svcSc1 = RG_GET_SC1_SVC_DL(svc);
   RgSchSc1DlUe   *ueSc1 = RG_GET_SC1_UE_DL(ue);
#ifdef DEBUGP
   Inst        inst = ue->cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlAdd2UeSchdSvcs);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlAdd2UeSchdSvcs\n"));

   /* checking SVC's presence in this lst is unnecessary */
   cmLListAdd2Tail(&ueSc1->schdSvcs, &svcSc1->schdSvcLnk);
   svcSc1->schdSvcLnk.node = (PTR)svc;
   RETVOID;
}


/**
 * @brief This function performs new allocations for UEs
 *
 * @details
 *
 *     Function: rgSCHSc1DlDedTx
 *     Purpose:  This function implements scheduler for DL allocation for
 *               new transmissions of UEs.
 *               1. It performs across 9 priorities that it supports - 
 *                   This is from 3GPP specifications
 *               2. There are known number of GBR/MBR queues
 *               3. The first queue is highest priority queue and is 
 *                  satisfied completely prior to any other queues. This
 *                  queue is for RRC signalling.
 *               4. Futher GBR/MBR queues are satisfied for GBR and then MBR
 *               5. Subsequently all other queues are looked at for AMBR
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlDedTx
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else
PRIVATE Void rgSCHSc1DlDedTx(cell, subFrm, allocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchUeCb            *ue;
   RgSchDlLcCb          *svc;
   U8                   i;
   RgSchSc1DlSvc        *svcSc1;
   RgSchSc1DlUe         *ueDl;
   RgSchSc1DlCell       *sc1CellDl = RG_GET_SC1_CELL_DL(cell);
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   
   TRC2(rgSCHSc1DlDedTx);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlDedTx\n"));

   /* Process the first queue that is for RRC signalling and is of */
   /* highest priority.                                            */
   lst  = &sc1CellDl->prioLst[0];
   node = lst->first;
   while(node != NULLP)
   {
      /* Getting service instead of UE */
      svc   = (RgSchDlLcCb *)node->node;
      svcSc1 = RG_GET_SC1_SVC_DL(svc);
      node = node->next;
      if (rgSCHSc1DlDedSvcAlloc(cell, subFrm, svc, svcSc1->bo, allocInfo) != ROK) 
      {
         /* sf bw expired */
         RETVOID;
      }
   }

   /* Perform allocation for the GBR transmissions */
   for(i = RG_SCH_CMN_DL_GBR_PRIO_START; i <= RG_SCH_CMN_DL_GBR_PRIO_END; i++)
   {
      lst  = &sc1CellDl->prioLst[i];
      node = lst->first;
      while(node != NULLP)
      {
         /* Getting service instead of UE */
         svc   = (RgSchDlLcCb *)node->node;
         svcSc1 = RG_GET_SC1_SVC_DL(svc);
         node = node->next;
         if (rgSCHSc1DlDedSvcAlloc(cell, subFrm, svc, svcSc1->effMbr, allocInfo) != ROK) 
         {
            /* sf bw expired */
            RETVOID;
         }
      }
   }

   /* To implement AMBR svc scheduling */
   for(i = RG_SCH_CMN_DL_GBR_PRIO_END + 1; i < RG_SCH_CMN_DL_PRIOS; i++)
   {
      lst  = &sc1CellDl->prioLst[i];
      node = lst->first;
      while(node != NULLP)
      {
         ue = (RgSchUeCb *)node->node;
         ueDl = RG_GET_SC1_UE_DL(ue);
         node = node->next;
         /* Get the Curr ambr svc for which allocation is to be made */
         svc = ueDl->ambrSvc;
         if (rgSCHSc1DlDedSvcAlloc(cell, subFrm, svc, ueDl->effAmbr, allocInfo) != ROK) 
         {
            /* sf bw expired */
            RETVOID;
         }
      }
   }
   RETVOID;
}


/**
 * @brief This function implements scheduler DL allocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlDed
 *     Purpose:  This function implements scheduler for DL allocation for
 *               UEs.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlDed
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHSc1DlDed(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchDlSf            *subFrm = allocInfo->dedAlloc.dedDlSf;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1DlDed);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlDed\n"));

   rgSCHSc1DlRetxAlloc(cell, subFrm, allocInfo);

   /* Now perform the new UE selections */
   rgSCHSc1DlDedTx(cell, subFrm, allocInfo);

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}


/**
 * @brief This function adds a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcAddToSchd
 *     Purpose:  This function adds a service to the list of services
 *               based on the priority of the services.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSvcAddToSchd
(
RgSchCellCb                *cell,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlSvcAddToSchd(cell, svc)
RgSchCellCb                *cell;
RgSchDlLcCb                *svc;
#endif
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchDlLcCb          *lSvc;
   RgSchSc1DlSvc        *svcSc1;
   RgSchSc1DlSvc        *lSvcSc1;
   RgSchSc1DlCell       *sc1CellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchCmnDlSvc        *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlSvcAddToSchd);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSvcAddToSchd\n"));

   svcSc1 = RG_GET_SC1_SVC_DL(svc);
   /* The service is already in the scheduler */
   if (svcSc1->prioLnk.node != NULLP)
   {
      RETVOID;
   }

   /* If the priority = 0, it is the highest priority with infinite */
   /* allowance and the priority is time bound and hence just place */
   /* it at the end of the queue                                    */
   if (svcCmn->prio == 0)
   {
      lst  = &(sc1CellDl->prioLst[0]);
      cmLListAdd2Tail(lst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)svc;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* If a svc is put in to cell wide priority Qs
       * then add the same to UE's lcsWithData List */
      rgSCHSc1DlAdd2UeLcsWithData(svc->ue, svc);
      RETVOID;
   }

   /* Handle GBR services. We have them of next importance */
   /* check changed from .._START to  .._END */
   if (svcCmn->prio <= RG_SCH_CMN_DL_GBR_PRIO_END)
   {
      if (!RG_SC1_SVC_HAS_DATA(svc))
         RETVOID;
      lst  = &(sc1CellDl->prioLst[svcCmn->prio]);
      node = lst->first;
      while(node)
      {
         lSvc = (RgSchDlLcCb *)(node->node);
         lSvcSc1 = RG_GET_SC1_SVC_DL(lSvc);
         if (((svcSc1->effGbr > 0) && 
                  (lSvcSc1->effGbr <= svcSc1->effGbr)) ||
               ((lSvcSc1->effGbr == 0) && (svcSc1->effMbr > 0) && 
                (lSvcSc1->effMbr <= svcSc1->effMbr)))
         {
            break;
         }
         node = node->next;
      }
      if (node == NULLP)
      {
         /* We have come to the end of the queue. Let's place it */
         /* here irresepctive of effGbr or effMBr                */
         cmLListAdd2Tail(lst, &svcSc1->prioLnk);
         svcSc1->prioLnk.node = (PTR)svc;
      }
      else
      {
         lst->crnt = node;
         cmLListInsCrnt(lst, &svcSc1->prioLnk);
         svcSc1->prioLnk.node = (PTR)svc;
      }
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* If a svc is put in to cell wide priority Qs
       * then add the same to UE's lcsWithData List */
      rgSCHSc1DlAdd2UeLcsWithData(svc->ue, svc);
   }
   RETVOID;
}



/**
 * @brief This function removes a UE from scheduler Queue
 *
 * @details
 *
 *     Function: rgSCHSc1DlUeRmvFrmSchd
 *     Purpose:  This function removes a UE from the list of UEs
 *               based on the priority of the UEs Current AMBR SVC.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlUeRmvFrmSchd
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
PRIVATE Void rgSCHSc1DlUeRmvFrmSchd(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchSc1DlUe   *ueDl = RG_GET_SC1_UE_DL(ue);
   CmLListCp   *lst;
#ifdef DEBUGP
   Inst        inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlUeRmvFrmSchd);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlUeRmvFrmSchd\n"));

   lst  = &cellDl->prioLst[ueDl->prio];
   if (ueDl->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = (PTR)NULLP;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* If a svc is removed from cell wide priority Qs
       * then remove the same from UE's lcsWithData List */
      rgSCHSc1DlRmFrmUeLcsWithData(ue, ueDl->ambrSvc);
   }
   RETVOID;
}


/**
 * @brief This function removes a SVC from UEs AMBR LIST 
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcRmvFrmUeAmbrLst
 *     Purpose:  This function removes a SVC from UEs AMBR List.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSvcRmvFrmUeAmbrLst 
(
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlSvcRmvFrmUeAmbrLst(ue, svc)
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchSc1DlUe *ueDl = RG_GET_SC1_UE_DL(ue);
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(svc);
   CmLListCp            *lst;
#ifdef DEBUGP
   Inst                 inst = ue->cell->instIdx;
#endif

   TRC2(rgSCHSc1DlSvcRmvFrmUeAmbrLst);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSvcRmvFrmUeAmbrLst\n"));

   lst  = &ueDl->ambrLst;
   if (svcSc1->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)NULLP;
   }
   RETVOID;
}


/**
 * @brief This function adds a SVC to UEs AMBR LIST 
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcAddToUeAmbrLst
 *     Purpose:  This function adds a SVC to UEs AMBR List.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSvcAddToUeAmbrLst 
(
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlSvcAddToUeAmbrLst(ue, svc)
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchSc1DlUe            *ueDl = RG_GET_SC1_UE_DL(ue);
   CmLList              *node;
   RgSchDlLcCb          *lsvc;
   RgSchSc1DlSvc           *svcSc1 = RG_GET_SC1_SVC_DL(svc);
   RgSchCmnDlSvc        *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
#ifdef DEBUGP
   Inst                 inst = ue->cell->instIdx;
#endif

   TRC2(rgSCHSc1DlSvcAddToUeAmbrLst);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSvcAddToUeAmbrLst\n"));

   /* If svc already present in AMBR List return */
   if (svcSc1->prioLnk.node != NULLP)
      RETVOID;

   node = ueDl->ambrLst.first;
   while(node)
   {
      lsvc = (RgSchDlLcCb *)(node->node);
      if (((RgSchCmnDlSvc*)(lsvc->sch))->prio > svcCmn->prio)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      cmLListAdd2Tail(&ueDl->ambrLst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)svc;
   }
   else
   {
      ueDl->ambrLst.crnt = node;
      cmLListInsCrnt(&ueDl->ambrLst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)svc;
   }
   
   RETVOID;
}


/**
 * @brief This function removes a service from scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcRmvFrmSchd
 *     Purpose:  This function removes the SVC from the scheduler Qs.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSvcRmvFrmSchd
(
RgSchCellCb                *cell,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlSvcRmvFrmSchd(cell, svc)
RgSchCellCb                *cell;
RgSchDlLcCb                *svc;
#endif
{
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchSc1DlSvc *svcDl = RG_GET_SC1_SVC_DL(svc);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   CmLListCp    *lst;
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlSvcRmvFrmSchd);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSvcRmvFrmSchd\n"));

   lst = &(cellDl->prioLst[svcCmn->prio]);
   if (svcDl->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &svcDl->prioLnk);
      svcDl->prioLnk.node = NULLP;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* If a svc is removed from cell wide priority Qs
       * then remove the same from UE's lcsWithData List */
      rgSCHSc1DlRmFrmUeLcsWithData(svc->ue, svc);
   }
   RETVOID;
}


/**
 * @brief This function adds a service to scheduler for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcAdd
 *     Purpose:  This function is made available through a FP for 
 *               making scheduler aware of a service added to UE
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchUeCb*        ue   
 *  @param[in]  RgSchDlLcCb*      svc   
 *  @param[in]  CrgDlLchCfg*   qos
 *  @return                    Void 
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSvcAdd
(
RgSchUeCb               *ue,
RgSchDlLcCb             *svc,
RgrDlLchCfg             *cfg
)
#else
PRIVATE Void rgSCHSc1DlSvcAdd(ue, svc, cfg)
RgSchUeCb               *ue;
RgSchDlLcCb             *svc;
RgrDlLchCfg             *cfg;
#endif
{
   RgSchSc1DlUe  *ueDl = RG_GET_SC1_UE_DL(ue);
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(svc);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
#ifdef DEBUGP
   Inst         inst = ue->cell->instIdx;
#endif
   TRC2(rgSCHSc1DlSvcAdd);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSvcAdd\n"));

   UNUSED(cfg);

   if (RG_SCH_CMN_SVC_IS_GBR(svc))
   {
      svcSc1->gbr   = svcCmn->gbr;
      svcSc1->mbr   = svcCmn->mbr;
      cmLListAdd2Tail(&ueDl->gbrSvcs, &svcSc1->gbrLnk);
      svcSc1->gbrLnk.node = (PTR)svc;
   }
   RETVOID;
}


/**
 * @brief This function deletes a service from scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlLcRmv
 *     Purpose:  This function is made available through a FP for 
 *               making scheduler aware of a service being deleted from UE
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc   
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlLcRmv
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PUBLIC Void rgSCHSc1DlLcRmv(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchSc1DlUe  *ueDl;
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(svc);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlLcRmv);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlLcRmv\n"));

   if (svcSc1 == NULLP)
   {
      RETVOID;
   }
   ueDl = RG_GET_SC1_UE_DL(ue);

   if (svcCmn->prio == 0)
   {
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   else if (RG_SCH_CMN_SVC_IS_GBR(svc))
   {
      if (svcSc1->gbrLnk.node != NULLP)
      {
         cmLListDelFrm(&ueDl->gbrSvcs, &svcSc1->gbrLnk);
         svcSc1->gbrLnk.node = NULLP;
      }
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   else /* if AMBR service */
   {
      if (ueDl->ambrSvc == svc)
      {
         rgSCHSc1DlUeRmvFrmSchd(cell, ue);
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(ue, svc);
         ueDl->ambrSvc = NULLP;
         if (ueDl->ambrLst.first != NULLP)
         {
            ueDl->ambrSvc = (RgSchDlLcCb *)(ueDl->ambrLst.first->node);
            ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, svc->bo);
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
         }
      }
      else
      {
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(ue, svc);
      }
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, 
         (Data**)(&(svcSc1)), (sizeof(RgSchSc1DlSvc)));
   RETVOID;
}

/**
 * @brief This function is invoked as part of SVC reconfig 
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcMod
 *     Purpose:  This function is made available through a FP for 
 *               making scheduler aware of a service reconfiguration. 
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchDlLcCb*      svc   
 *  @param[in]  CrgLchRecfg*   recfg
 *  @return     Void 
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSvcMod
(
RgSchDlLcCb                *svc,
RgrLchRecfg             *recfg
)
#else
PRIVATE Void rgSCHSc1DlSvcMod(svc, recfg)
RgSchDlLcCb                *svc;
RgrLchRecfg             *recfg;
#endif
{ 
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(svc);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
#ifdef DEBUGP
   Inst         inst = svc->ue->cell->instIdx;
#endif
   TRC2(rgSCHSc1DlSvcMod);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSvcMod\n"));

   if (RG_SCH_CMN_SVC_IS_GBR(svc))
   {
      /* Convert the QOS to handle the refresh duration */
      svcSc1->gbr   = svcCmn->gbr;
      svcSc1->mbr   = svcCmn->mbr;
   }
   RETVOID;
}

/**
 * @brief This function adds UE to scheduler for an AMBR service
 *
 * @details
 *
 *     Function: rgSCHSc1DlUeAddToSchd
 *     Purpose:  This function adds a UE to scheduler for the AMBR
 *               service of highest priority.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchUeCb*        ue   
 *  @return                    Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlUeAddToSchd
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
PRIVATE Void rgSCHSc1DlUeAddToSchd(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchSc1DlUe   *ueDl = RG_GET_SC1_UE_DL(ue);
   RgSchSc1DlUe   *lueDl;
   CmLList        *node;
   CmLListCp      *lst;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1DlUeAddToSchd);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlUeAddToSchd\n"));

   ueDl->prio = ((RgSchCmnDlSvc *)(ueDl->ambrSvc->sch))->prio;
   lst  = &cellDl->prioLst[ueDl->prio]; 
   /* if UE already in list, remove and
    * readjust */
   if (ueDl->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = NULLP;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* If a svc is removed from cell wide priority Qs
       * then remove the same from UE's lcsWithData List */
      rgSCHSc1DlRmFrmUeLcsWithData(ue, ueDl->ambrSvc);
   }
   node = lst->first;
   while(node)
   {
      lueDl = RG_GET_SC1_UE_DL((RgSchUeCb *)(node->node));
      if (lueDl->effAmbr < ueDl->effAmbr)
         break;
      node = node->next;
   }
   if (node == NULLP)
   {
      cmLListAdd2Tail(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = (PTR)ue;
   }
   else
   {
      lst->crnt = node;
      cmLListInsCrnt(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = (PTR)ue;
   }
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* If a svc is put in to cell wide priority Qs
    * then add the same to UE's lcsWithData List */
   rgSCHSc1DlAdd2UeLcsWithData(ue, ueDl->ambrSvc);
   RETVOID;
}


/**
 * @brief This function implements managing BO for an ABMR service
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngAmbrSvcPosn
 *     Purpose:  This function should be called whenever there is a 
 *               change BO for a AMBR service.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlMngAmbrSvcPosn
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlMngAmbrSvcPosn(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchSc1DlUe    *ueDl = RG_GET_SC1_UE_DL(ue);
   RgSchSc1DlSvc   *svcSc1 = RG_GET_SC1_SVC_DL(svc);
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlMngAmbrSvcPosn);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlMngAmbrSvcPosn\n"));

   if (svcSc1->bo == 0)
   {
      if (ueDl->ambrSvc == svc)
      {
         rgSCHSc1DlUeRmvFrmSchd(cell, ue);
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(ue, svc);
         ueDl->ambrSvc = NULLP;
         if (ueDl->ambrLst.first != NULLP)
         {
            ueDl->ambrSvc = (RgSchDlLcCb *)(ueDl->ambrLst.first->node);
            ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, svcSc1->bo);
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
         }
      }
      else
      {
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(ue, svc);
      }
   }
   else /* svcSc1->bo != 0 */
   {
      if (svcSc1->prioLnk.node != NULLP)
      {
         if (svc == ueDl->ambrSvc)
         {
            ueDl->effAmbr = RGSCH_MIN(svcSc1->bo, ueDl->ambr);
            /* Update UE's position in the scheduler */
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
            else
            {
               rgSCHSc1DlUeRmvFrmSchd(cell, ue);
            }
         }
         RETVOID;
      }
      rgSCHSc1DlSvcAddToUeAmbrLst(ue, svc);
      /* Current ambr svc is always the first node of ambrLst.*/ 
      if (ueDl->ambrLst.first->node == (PTR)svc)
      {
         if(ueDl->ambrSvc != svc)
         {
            if(ueDl->ambrSvc)
            {
               rgSCHSc1DlUeRmvFrmSchd(cell, ue);
            }
            ueDl->ambrSvc = svc;
            ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, svcSc1->bo);
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
         }
      }
   }
   RETVOID;
}


/**
 * @brief This function updates the scheduler with service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlLcBoUpd
 *     Purpose:  This function should be called whenever there is a 
 *               change BO for a service.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlLcBoUpd
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PUBLIC Void rgSCHSc1DlLcBoUpd(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchSc1DlSvc   *svcSc1 = RG_GET_SC1_SVC_DL(svc);
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlLcBoUpd);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlLcBoUpd\n"));   

   if (svcSc1->bo == svc->bo)
   {
      RETVOID;
   }
   svcSc1->bo = svc->bo;
   if (!RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {
      RETVOID;
   }
   rgSCHSc1DlMngSvcPosn(cell, ue, svc);

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}


/**
 * @brief This function updates the scheduler with Prio0 service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngPrio0SvcPosn 
 *     Purpose:  This func shall be triggered whenever there is a
 *     change in the "Bo yet to be satisfied" field of the service.
 *     Appropriately positions the svc in its prio Q.
 *     Removes the SVC from the Q if BO is completely satisfied.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlMngPrio0SvcPosn
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlMngPrio0SvcPosn(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{ 
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(svc);
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1DlMngPrio0SvcPosn);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "\n"));

   /* In this priority, we just add or remove to the queue */
   if (svcSc1->bo > 0)
   {
      rgSCHSc1DlSvcAddToSchd(cell, svc);
   }
   else
   {
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   RETVOID;
}


/**
 * @brief This function updates the scheduler with GBR service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngGbrSvcPosn 
 *     Purpose:  This func shall be triggered whenever there is a
 *     change in the "Bo yet to be satisfied" field of the service.
 *     Appropriately positions the svc in its prio Q.
 *     Removes the SVC from the Q if BO is completely satisfied.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlMngGbrSvcPosn 
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlMngGbrSvcPosn(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{ 
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(svc);
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1DlMngGbrSvcPosn);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlMngGbrSvcPosn\n"));

   /* Handle a GBR service. */
   svcSc1->effGbr = RGSCH_MIN(svcSc1->bo, svcSc1->gbr);
   svcSc1->effMbr = RGSCH_MIN(svcSc1->bo, svcSc1->mbr);
   /* Adjust the SVC priority within the queue */
   rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   rgSCHSc1DlSvcAddToSchd(cell, svc);
   RETVOID;
}


/**
 * @brief This function updates the scheduler with service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngSvcPosn 
 *     Purpose:  This func shall be triggered whenever there is a
 *     change in the "Bo yet to be satisfied" field of the service.
 *     Appropriately positions the svc in its prio Q.
 *     Removes the SVC from the Q if BO is completely satisfied.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlMngSvcPosn 
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlMngSvcPosn(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{ 
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1DlMngSvcPosn);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "\n"));

   (cellDl->svcMngFunc[svcCmn->prio])(cell, ue, svc);
   RETVOID; 
}

/*--------------------------*
 * DL specific functions END
 *---------------------------*/



/**
 * @brief Scheduler processing on cell configuration
 *
 * @details
 *
 *     Function : rgSCHSc1RgrDlCellCfg
 *     
 *     This function does requisite initialisation 
 *     and setup for scheduler1 when a cell is
 *     configured
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrDlCellCfg
(
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
)
#else
PUBLIC S16 rgSCHSc1RgrDlCellCfg(cell, cellCfg, err)
RgSchCellCb   *cell;
RgrCellCfg    *cellCfg;
RgSchErrInfo  *err;
#endif
{
   S16         ret;
   RgSchSc1DlCell *cellDl;
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif

   TRC2(rgSCHSc1RgrDlCellCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1RgrDlCellCfg\n"));

   if((ret = rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&(((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc), \
      (sizeof(RgSchSc1DlCell)))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), \
      "Memory allocation FAILED"));
      MSPD_ERR( "Memory allocation FAILED \n");
      err->errCause = RGSCHERR_SCH_SC1_DL_CFG;
      RETVALUE(ret);
   }

   cellDl = RG_GET_SC1_CELL_DL(cell);
   /* Now perform downlink Queues related initializations  */
   rgSCHSc1DlInitQueues(cellDl);
   RETVALUE(ROK);
}  /* rgSCHSc1RgrDlCellCfg */

/***********************************************************
 *
 *     Func : rgSCHSc1DlDeinitQueues
 *        
 *     Desc : De-initialise downlink scheduler queues
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlDeinitQueues
(
RgSchSc1DlCell *cellDl
)
#else
PRIVATE Void rgSCHSc1DlDeinitQueues(cellDl)
RgSchSc1DlCell *cellDl;
#endif
{
   U8          i;
   TRC2(rgSCHSc1DlDeinitQueues);

   for (i = 0; i < RG_SC1_DL_NUM_Q; ++i)
   {
      cmLListInit(&cellDl->prioLst[i]);
      cmLListInit(&cellDl->retxLst[i]);
   } 
   RETVOID;
}


/**
 * @brief Scheduler processing for cell delete
 *
 * @details
 *
 *     Function : rgSCHSc1DlCellDel
 *     
 *     This functions de-initialises and frees memory
 *     taken up by scheduler1 for the entire cell.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlCellDel
(
RgSchCellCb  *cell
)
#else
PUBLIC Void rgSCHSc1DlCellDel(cell)
RgSchCellCb  *cell;
#endif
{
   TRC2(rgSCHSc1DlCellDel);

   if (((RgSchSc1DlCell *)((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc) \
       == NULLP)
   {
      RETVOID;
   }
      
   /* Perform the deinit for the DL scheduler */
   rgSCHSc1DlDeinitQueues(RG_GET_SC1_CELL_DL(cell));
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, 
      (Data**)(&(((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc)),
      (sizeof(RgSchSc1DlCell)));
   RETVOID;
}  /* rgSCHSc1DlCellDel */

/**
 * @brief UE initialisation for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrDlUeCfg
 *     
 *     This functions intialises UE specific scheduler
 *     information
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrDlUeCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrDlUeCfg(cell, ue, ueCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeCfg     *ueCfg;
RgSchErrInfo *err;
#endif
{
   RgSchCmnUe         *ueSchCmn = RG_SCH_CMN_GET_UE(ue);
   RgSchCmnDlUe       *ueCmnDl = RG_SCH_CMN_GET_DL_UE(ue);
   Inst               inst = cell->instIdx;
   RgSchSc1DlUe       *ueDl;

   TRC2(rgSCHSc1RgrDlUeCfg);

   if((rgSCHUtlAllocSBuf(inst, 
               (Data**)&(ueSchCmn->dl.schSpfc), (sizeof(RgSchSc1DlUe))) != ROK))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED"));
      MSPD_ERR( "Memory allocation FAILED \n");
      err->errCause = RGSCHERR_SCH_SC1_DL_CFG;
      RETVALUE(RFAILED);
   }
   ueDl = (RgSchSc1DlUe *)ueSchCmn->dl.schSpfc;
   if (ueCmnDl->ambrCfgd)
   {
      ueDl->ambr = ueCmnDl->ambrCfgd;
   }
   else
   {
      ueDl->ambr = RG_SC1_MAX_DL_AMBR;
   }
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   cmLListInit(&ueDl->lcsWithData);
   cmLListInit(&ueDl->gbrSvcs);
   cmLListInit(&ueDl->ambrLst);
   cmLListInit(&ueDl->schdSvcs);
   cmLListInit(&ueDl->retxHqProcs);
   RETVALUE(ROK);
}  /* rgSCHSc1RgrDlUeCfg */


/**
 * @brief Dl Harq Entity initialization for SC1
 *
 * @details
 *
 *     Function : rgSCHSc1DlUeHqEntInit
 *
 *     Processing Steps:
 *      - Create SC1 related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1DlUeHqEntInit
(
 RgSchCellCb      *cell,
 RgSchUeCb        *ue
 )
#else
PUBLIC S16 rgSCHSc1DlUeHqEntInit(cell, ue)
   RgSchCellCb      *cell;
   RgSchUeCb        *ue;
#endif
{
   RgSchSc1DlHqProc   *hqSpcSch;
   RgSchDlHqProcCb    *hqP;
   U8                 cnt;
   TRC2(rgSCHSc1DlUeHqEntInit);
   /* making use of hqE->sch for one shot allocation 
    * of RgSchSc1DlHqProc structures */
   if (rgSCHUtlAllocSBuf(cell->instIdx, 
            (Data**)&(ue->dl.hqEnt->sch),
            (ue->dl.hqEnt->numHqPrcs * sizeof(RgSchSc1DlHqProc))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), \
               "Memory allocation FAILED"));
      RETVALUE(RFAILED);
   }
   hqSpcSch = (RgSchSc1DlHqProc *)(ue->dl.hqEnt->sch);
   for(cnt = 0; cnt < ue->dl.hqEnt->numHqPrcs; cnt++)
   {
      hqP = &ue->dl.hqEnt->procs[cnt];
      ((RgSchCmnDlHqProc *)((hqP)->sch))->schSpfc = \
                                                    hqSpcSch++;
   }
   RETVALUE(ROK);
}

/**
 * @brief Dl Harq Entity deletion for Sc1
 *
 * @details
 *
 *     Function : rgSCHSc1DlUeHqEntDeInit 
 *
 *     Processing Steps:
 *      - Free SC1 related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchDlHqEnt     *hqE 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1DlUeHqEntDeInit 
(
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqE
)
#else
PUBLIC S16 rgSCHSc1DlUeHqEntDeInit(cell, hqE)
RgSchCellCb      *cell;
RgSchDlHqEnt     *hqE;
#endif
{
   TRC2(rgSCHSc1DlUeHqEntDeInit);

   if(hqE->sch)
   {
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(hqE->sch)),
      (hqE->numHqPrcs * sizeof(RgSchSc1DlHqProc)));
   }
   else
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/**
 * @brief UE reconfiguration for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrDlUeRecfg
 *     
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrDlUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrDlUeRecfg(cell, ue, ueRecfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
RgSchErrInfo *err;
#endif
{
   RgSchSc1DlUe *ueDl     = RG_GET_SC1_UE_DL(ue);
   RgSchCmnDlUe *ueCmnDl = RG_SCH_CMN_GET_DL_UE(ue);
#ifdef DEBUGP
   Inst        inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1RgrDlUeRecfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1RgrDlUeRecfg\n"));

   if (ueCmnDl->ambrCfgd)
   {
      ueDl->ambr = ueCmnDl->ambrCfgd;
   }
   else
   {
      ueDl->ambr = RG_SC1_MAX_DL_AMBR;
   }

   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
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
               rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
               rgSCHSc1DlProcRmvFrmUeRetx(ue, hqP);
            }
      /* rg001.301 -MOD -[ccpu00118350] Correcting NDI manipulation of Harq */
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
}  /* rgSCHSc1RgrDlUeRecfg */

/**
 * @brief Removes UEs context from Priority Qs.
 *
 * @details
 *
 *     Function : rgSCHSc1DlRmvUeFrmPrioQs
 *     
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlRmvUeFrmPrioQs
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PRIVATE Void rgSCHSc1DlRmvUeFrmPrioQs(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchSc1DlUe         *sc1Ue;
   RgSchDlLcCb          *svc;
   U32                  idx;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlRmvUeFrmPrioQs);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlRmvUeFrmPrioQs\n"));

   sc1Ue = RG_GET_SC1_UE_DL(ue);

   /* Remove UE From DL priority queues */
   if (sc1Ue->ambrSvc != NULLP)
   {
      rgSCHSc1DlUeRmvFrmSchd(cell, ue);
   }

   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      svc = ue->dl.lcCb[idx];
      if (svc == NULLP)
      {
         continue;
      }
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   
   RETVOID;
}  /* rgSCHSc1DlRmvUeFrmPrioQs */

/**
 * @brief Inactivate UE reason : measgap, acknaprept, poInactv.
 *
 * @details
 *
 *     Function : rgSCHSc1DlInactvtUe
 *     
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlInactvtUe
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PRIVATE Void rgSCHSc1DlInactvtUe(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchSc1DlCell       *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchDlHqProcCb      *hqP;
   RgSchCmnDlHqProc     *hqProcDl;
   U8                   i;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlInactvtUe);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlInactvtUe\n"));

   /* remove all in use HARQ processes from the subframes.
    * Store them in UEs hqProc Lst. Add back to cell's
    * retx lst when UE is activated again. */
   for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
   {
      hqP = &ue->dl.hqEnt->procs[i];
      hqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);
      /* Remove retx procs from cell's list and 
       * add them to UE's List */
      /*rg008.201 - Added support for SPS*/
      if(
#ifdef LTEMAC_SPS
!(RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP)) &&
#endif
            hqProcDl->retxLnk.node != NULLP)
      {
         cmLListDelFrm(&cellDl->retxLst[((RgSchSc1DlHqProc *)\
                       (hqProcDl->schSpfc))->prio], &(hqProcDl->retxLnk));
         hqProcDl->retxLnk.node = NULLP;
         rgSCHSc1DlProcAddToUeRetx(ue, hqP);
      }
   }

   rgSCHSc1DlRmvUeFrmPrioQs(cell, ue);

   RETVOID;
}  /* rgSCHSc1DlInactvtUe */


/**
 * @brief UE suspension.
 *
 * @details
 *
 *     Function : rgSCHSc1DlSuspendUe 
 *     
 *     Removes UE, its SVCs and its HqPs from CELL WIDE
 *     PrioQs and Retx Qs Respectively.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSuspendUe 
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PRIVATE Void rgSCHSc1DlSuspendUe(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchDlHqProcCb      *hqP;
   U8                   i;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchPdcch           *pdcchPrev = NULLP;
   U8                   j;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlSuspendUe);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSuspendUe\n"));

   /* remove all in use HARQ processes from the subframes.
    * Store them in UEs hqProc Lst. Add back to cell's
    * retx lst when UE is activated again. */
   for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
   {
      hqP = &ue->dl.hqEnt->procs[i];
      rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
      rgSCHSc1DlProcRmvFrmUeRetx(ue, hqP);
      /* Removing the Harq Proc from subframes tbs list */
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      for (j = 0; j < 2; j++)
      {
         if ((hqP->tbInfo[j].subFrm != NULLP) && 
               (hqP->tbInfo[j].sfLnk.node != NULLP))
         {
            cmLListDelFrm(&hqP->tbInfo[j].subFrm->tbs, &hqP->tbInfo[j].sfLnk);
            hqP->tbInfo[j].sfLnk.node = NULLP;
            /* Since each TB can be assigned the same PDCCH structure incase of 
             * Spatial multiplexing allocation of 2 TBs. Check if we have
             * already removed PDCCH as part of one TB, then no need to remove
             * as part of other TB processing */
            if (hqP->tbInfo[j].pdcch != pdcchPrev)
            {
               cmLListDelFrm(&hqP->tbInfo[j].subFrm->pdcchInfo.pdcchs, 
                     &hqP->tbInfo[j].pdcch->lnk);
               cmLListAdd2Tail(&cell->pdcchLst, &hqP->tbInfo[j].pdcch->lnk);
            }
            pdcchPrev = hqP->tbInfo[j].pdcch;
      /* rg001.301 -MOD -[ccpu00118350] Correcting NDI manipulation of Harq */
            rgSCHDhmRlsHqpTb(hqP, j, TRUE);
         }
      }
   }
   rgSCHSc1DlRmvUeFrmPrioQs(cell, ue);

   RETVOID;
}  /* rgSCHSc1DlSuspendUe */


/**
 * @brief UE deletion for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1DlUeDel
 *     
 *     This functions deletes all scheduler information
 *     pertaining to a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlUeDel
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHSc1DlUeDel(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlUeDel);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlUeDel\n"));

   if (((RgSchSc1DlUe *)((RgSchCmnUe *)((ue)->sch))->dl.schSpfc) == NULLP)
   {
      RETVOID;
   }
   if( ue->dl.hqEnt)
   {
      /* Remove UEs scheduler context */
      rgSCHSc1DlSuspendUe(cell, ue);

      /* Free all SC1 specific control blocks */
      if (ue->dl.hqEnt->sch != NULLP)
      {
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, 
               (Data**)(&(ue->dl.hqEnt->sch)), 
               (ue->dl.hqEnt->numHqPrcs * sizeof(RgSchSc1DlHqProc)));
      }
   }

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, 
         (Data**)(&(((RgSchCmnUe *)((ue)->sch))->dl.schSpfc)),
         (sizeof(RgSchSc1DlUe))); 

   RETVOID;
}  /* rgSCHSc1DlUeDel */

/**
 * @brief Scheduler invocation on Downlink logical channel addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcCfg
 *     
 *     This functions does required processing when a new
 *     (dedicated) logical channel is added. 
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchDlLcCb  *dlLc
 *  @param[int] RgrLchCfg    *lcCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrLcCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchCfg *lcCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrLcCfg(cell, ue, dlLc, lcCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchDlLcCb  *dlLc;
RgrLchCfg *lcCfg;
RgSchErrInfo *err;
#endif
{
   S16      ret;
#ifdef DEBUGP
   Inst     inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1RgrLcCfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1RgrLcCfg\n"));

   ret = rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&((RgSchCmnDlSvc *)((dlLc)->sch))->schSpfc, \
      (sizeof(RgSchSc1DlSvc)));
   if (ret != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "\nrgSCHSc1CrgLcCfg(): \
      SCH struct alloc failed."));
      err->errCause = RGSCHERR_SCH_SC1_DL_CFG;
      RETVALUE(ret);
   }

   rgSCHSc1DlSvcAdd(ue, dlLc, &lcCfg->dlInfo);
   RETVALUE(ROK);
}  /* rgSCHSc1RgrLcCfg */


/**
 * @brief Scheduler invocation on logical channel addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcRecfg
 *     
 *     This functions does required processing when an existing
 *     (dedicated) logical channel is reconfigured. Assumes lcg
 *     pointer in ulLc is set to the old value.
 *     Independent of whether new LCG is meant to be configured,
 *     the new LCG scheduler info is accessed and possibly modified.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchDlLcCb  *dlLc
 *  @param[int] RgrLchRecfg  *lcRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrLcRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchRecfg  *lcRecfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrLcRecfg(cell, ue, dlLc, lcRecfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchDlLcCb  *dlLc;
RgrLchRecfg  *lcRecfg;
RgSchErrInfo *err;
#endif
{
#ifdef DEBUGP
   Inst  inst = cell->instIdx;
#endif
   TRC2(rgSCHSc1RgrLcRecfg);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1RgrLcRecfg\n"));

   UNUSED(err);

   rgSCHSc1DlSvcMod(dlLc, lcRecfg);

   RETVALUE(ROK);
}  /* rgSCHSc1RgrLcRecfg */


/**
 * @brief This function handles the reconfiguration of cell 
 *
 * @details
 *
 *     Function: rgSCHSc1RgrDlCellRecfg
 *     Purpose:  Update the reconfiguration parameters.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHSc1RgrDlCellRecfg
(
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
)
#else
PRIVATE S16 rgSCHSc1RgrDlCellRecfg(cell, recfg, err)
RgSchCellCb             *cell;
RgrCellRecfg            *recfg;
RgSchErrInfo            *err;
#endif
{
   TRC2(rgSCHSc1DlUeReset);
   RETVALUE(ROK);
}



/**
 * @brief This function implements scheduler DL allocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlTaCmd
 *     Purpose:  This function implements scheduler for TA cmd alloc for
 *               UEs. The hq proc availed as part of this alloc can be used 
 *               by the UEs Dedicated CH transmission allocation.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*     cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlTaCmd
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHSc1DlTaCmd(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchDlHqProcCb      *proc;
   RgSchUeCb            *ue;
   U32                  effBo;
   RgSchCmnDlCell       *cellCmnDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlUe         *cmnUeDl; 
   RgSchSc1DlUe         *ueDl;
   RgSchCmnDlHqProc     *cmnHqDl;
   RgSchDlSf            *subFrm = allocInfo->dedAlloc.dedDlSf;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   TRC2(rgSCHSc1DlTaCmd);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlTaCmd\n"));

   lst  = &cellCmnDl->taLst;
   node = lst->first;
   while(node)
   {
      ue = (RgSchUeCb *)node->node;
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
      /* If Ue is inactive in DL then ignore */
      if (ue->dl.dlInactvMask)
      {
         continue;
      }
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
      ueDl = RG_GET_SC1_UE_DL(ue);

      if (rgSCHDhmGetAvlHqProc(ue, cellCmnDl->time, &proc) != ROK)
      {
         continue;    
      }
      /* Initialize some of the parameters of the HQ proc */
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);

      effBo = 0;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* 3.1 MIMO */
      cmnHqDl->totBytes = 0;
      rgSCHCmnDlAllocTxRb(cell, subFrm, ue, RGSCH_TA_SIZE, &effBo, proc, allocInfo);
      if (effBo == 0)
      {
         /* If no allocations so far, meaning proc obtained now */
         if (cmnHqDl->totBytes == 0)
         {
      /* rg001.301 -MOD -[ccpu00118350] Correcting NDI manipulation of Harq */
            rgSCHSc1RlsHqProc(proc);
            /*rg002.301 - ADD -[ccpu00120941] Added the handling for removing
             * UE from txUeLst and resetting outStndalloc.*/
            if(proc->reqLnk.node != (PTR)NULLP)
            {
               cmLListDelFrm(&allocInfo->dedAlloc.txUeLst, &proc->reqLnk);
               proc->reqLnk.node = (PTR)NULLP;
            }
            /*Re-set the outstanding alloc information.*/
            cmnUeDl->outStndAlloc = 0;
         }
         /* Avl BW could not satisfy even TA so break */
         break;
      }
      ueDl->taReqBytes = RGSCH_TA_SIZE;
      cmnUeDl->proc = proc;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      cmnHqDl->totBytes += effBo;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* 3.1 MIMO moving this call to cmn scheduler */
      /*rgSCHCmnDlRbInfoAddUeTx(allocInfo, ue);*/
   }
   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlHndlInActUes
 *     Purpose:  The list of inactive UEs present in inactvLst should
 *               be removed from the scheduling Qs.
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] CmLListCp   *inactvLst
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlHndlInActUes
(
RgSchCellCb           *cell,
CmLListCp             *inactvLst
)
#else
PUBLIC Void rgSCHSc1DlHndlInActUes(cell, inactvLst)
RgSchCellCb           *cell;
CmLListCp             *inactvLst;
#endif
{
   CmLList   *node;
   RgSchUeCb *ue;
#ifdef DEBUGP
   Inst      inst = cell->instIdx;
#endif

   TRC2(rgSCHSc1DlHndlInActUes);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlHndlInActUes\n"));

   node = inactvLst->first;
   while(node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      /* Suspend this UE from further scheduling
       * till it is activated again. */
      rgSCHSc1DlInactvtUe(cell, ue);
   }
   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlSched
 *     Purpose:  This function implements a DL scheduler for LTE. This is
 *               made available as a function pointer to be called
 *               at the time of TTI processing by the common Scheduler.
 *               UE's TA, Retx and New transmission are considered for
 *               scheduling here.
 *               The list of inactive UEs present in inactvLst should
 *               be removed from the scheduling Qs.
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo,
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlSched
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHSc1DlSched(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlSched);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlSched\n"));

   rgSCHSc1DlTaCmd(cell, allocInfo);

   rgSCHSc1DlDed(cell, allocInfo);

   RETVOID;
}


/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlInit
 *     Purpose:  This function initializes the following information
 *               1. Efficiency table
 *               2. CQI to table index - It is one row for upto 3 RBs
 *                  and another row for greater than 3 RBs
 *     
 *               currently extended prefix is compiled out.
 *     Invoked by: MAC intialization code..may be ActvInit
 *     
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlInit
(
RgDlSchdApis *rgSchDlApis
)
#else
PUBLIC Void rgSCHSc1DlInit(rgSchDlApis)
RgDlSchdApis *rgSchDlApis;
#endif
{
   TRC2(rgSCHSc1DlInit);
   /* Init the function pointers */
   rgSchDlApis->rgSCHRgrDlUeCfg        = rgSCHSc1RgrDlUeCfg;
   rgSchDlApis->rgSCHRgrDlUeRecfg      = rgSCHSc1RgrDlUeRecfg;
   rgSchDlApis->rgSCHFreeDlUe          = rgSCHSc1DlUeDel;
   rgSchDlApis->rgSCHRgrDlCellCfg      = rgSCHSc1RgrDlCellCfg;
   rgSchDlApis->rgSCHRgrDlCellRecfg    = rgSCHSc1RgrDlCellRecfg; 
   rgSchDlApis->rgSCHFreeDlCell        = rgSCHSc1DlCellDel;
   rgSchDlApis->rgSCHRgrDlLcCfg        = rgSCHSc1RgrLcCfg;
   rgSchDlApis->rgSCHRgrDlLcRecfg      = rgSCHSc1RgrLcRecfg;
   rgSchDlApis->rgSCHFreeDlLc          = rgSCHSc1DlLcRmv;
   rgSchDlApis->rgSCHDlSched           = rgSCHSc1DlSched;
   rgSchDlApis->rgSCHDlDedBoUpd        = rgSCHSc1DlLcBoUpd;
   rgSchDlApis->rgSCHDlProcAddToRetx   = rgSCHSc1DlProcAddToCellRetx;
   rgSchDlApis->rgSCHDlAllocFnlz       = rgSCHSc1DlAllocFnlz;
   rgSchDlApis->rgSCHDlCqiInd          = rgSCHSc1DlCqiInd;
   rgSchDlApis->rgSCHDlUeRefresh       = rgSCHSc1DlUeRefresh;
   rgSchDlApis->rgSCHDlUeReset         = rgSCHSc1DlUeReset;
   rgSchDlApis->rgSCHDlActvtUe         = rgSCHSc1DlActvtUe;
   rgSchDlApis->rgSCHDlInactvtUes      = rgSCHSc1DlHndlInActUes;
   rgSchDlApis->rgSCHDlUeHqEntInit     = rgSCHSc1DlUeHqEntInit;
   rgSchDlApis->rgSCHDlUeHqEntDeInit   = rgSCHSc1DlUeHqEntDeInit;

   RETVOID;
}




/***********************************************************
 *
 *     Func : rgSCHSc1DlInitQueues
 *        
 *     Desc : Initial downlink scheduler queues
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlInitQueues
(
RgSchSc1DlCell *cellDl
)
#else
PRIVATE Void rgSCHSc1DlInitQueues(cellDl)
RgSchSc1DlCell *cellDl;
#endif
{
   U8            i;
   TRC2(rgSCHSc1DlInitQueues);

   for (i = 0; i < RG_SC1_DL_NUM_Q; ++i)
   {
      cmLListInit(&cellDl->prioLst[i]);
      cmLListInit(&cellDl->retxLst[i]);
   }
   /* Set appropriate "manage svc positioning" function based on
    * svc priority as array index */
   /* for DCCH svcs */
   for (i = 0; i < RG_SCH_CMN_DL_GBR_PRIO_START; i++)
   {
      cellDl->svcMngFunc[i] = rgSCHSc1DlMngPrio0SvcPosn;
   }
   /* for GBR svcs */
   for (i = RG_SCH_CMN_DL_GBR_PRIO_START; i <= RG_SCH_CMN_DL_GBR_PRIO_END; i++)
   {
      cellDl->svcMngFunc[i] = rgSCHSc1DlMngGbrSvcPosn;
   }
   /* for Non-GBR svcs */
   for (i = RG_SCH_CMN_DL_GBR_PRIO_END+1; i <= RG_SCH_CMN_MAX_PRIO; i++)
   {
      cellDl->svcMngFunc[i] = rgSCHSc1DlMngAmbrSvcPosn;
   }
   RETVOID;
}




/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlRetxAllocFnlz 
 *     Purpose : Remove the Retx Hq Proc from the Cell's
 *     Retx list, if allocation is successful.
 *     Fill the HqProc and PDCCH and append it to the SubFrm.
 *     
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
PRIVATE Void rgSCHSc1DlRetxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHSc1DlRetxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   CmLList          *node;
/* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchUeCb        *ue;
   RgSchCmnDlUe     *ueDl;
   RgSchDlHqProcCb  *hqP;
/* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */

#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlRetxAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlRetxAllocFnlz\n"));

   node = allocInfo->dedAlloc.schdRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      /* Fill PDCCH and assign it to HqP */
      rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, hqP);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      /* Extra Check: Retain the hqProc in the RETX Queue if one/more
       * TBs of the HqProc are yet to be scheduled for RETX.
       * Note: Here we are not tracking at TB Level, the priority Q
       * to which it belongs to. The retx prio of transmission is still
       * being maintained at hqProc level, rather than at TB level */
      if ((hqP->tbInfo[0].state != HQ_TB_NACKED) && 
            (hqP->tbInfo[1].state != HQ_TB_NACKED)) {
         rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
      }
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/* 3.1 MIMO Alloc distribution functions being performed
 * TB wise */


/***********************************************************
 *
 *     Func : rgSCHSc1DlSprTxTbDstn 
 *        
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given spare TB "tbInfo" allocation.
 *     spare TB allocation is as a result of 1 RETX TB allocation, when
 *     conditions are favourable for 2 TB spatial multiplexing.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlSprTxTbDstn
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
CmLList        **node
)
#else
PRIVATE Void rgSCHSc1DlSprTxTbDstn(cell, ue, tbInfo, effAlloc, node)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
CmLList        **node;
#endif
{
   RgSchDlLcCb        *svc;
   RgSchSc1DlSvc      *svcSc1;
   RgSchSc1DlUe       *ueDl = RG_GET_SC1_UE_DL(ue);
   U32                bytes;
   RgSchLchAllocInfo  lchSchdData;
   U32                effBo;
   U32                rlcHdrEstmt;

   TRC2(rgSCHSc1DlSprTxTbDstn);

   while((*node) && (*effAlloc > 0))
   {
      svc = (RgSchDlLcCb *)(*node)->node;
      *node = (*node)->next;
      svcSc1 = RG_GET_SC1_SVC_DL(svc);
      
      RG_SCH_CMN_DL_GET_HDR_EST(svc, rlcHdrEstmt);
      /* Update the SVC QOS Param */
      if (RG_SCH_CMN_SVC_IS_GBR(svc))
      {
         effBo = svcSc1->effMbr + rlcHdrEstmt;
         bytes = RGSCH_MIN(*effAlloc, effBo);
         /* Determine How much BO is satisfied */
         if (bytes <= rlcHdrEstmt)
         {
            break; 
         }
         (svcSc1->bo <= bytes-rlcHdrEstmt)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-rlcHdrEstmt);
         svc->bo = svcSc1->bo;

         (svcSc1->gbr <= bytes)? (svcSc1->gbr = 0):
                                    (svcSc1->gbr -= bytes);
         (svcSc1->mbr <= bytes)? (svcSc1->mbr = 0):
                                    (svcSc1->mbr -= bytes);
      }
      else if(RG_SCH_CMN_SVC_IS_AMBR(svc))
      {
         effBo = ueDl->effAmbr + rlcHdrEstmt;
         bytes = RGSCH_MIN(*effAlloc, effBo);
         /* Determine How much BO is satisfied */
         if (bytes <= rlcHdrEstmt)
         {
            break; 
         }
         (svcSc1->bo <= bytes-rlcHdrEstmt)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-rlcHdrEstmt);

         (ueDl->ambr <= bytes)? (ueDl->ambr = 0):
                                       (ueDl->ambr -= bytes);
      }
      else /* Prio 0 SVC */
      {
         effBo = svcSc1->bo + rlcHdrEstmt;
         bytes = RGSCH_MIN(*effAlloc, effBo);
         /* Determine How much BO is satisfied */
         if (bytes <= rlcHdrEstmt)
         {
            break; 
         }
         (svcSc1->bo <= bytes-rlcHdrEstmt)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-rlcHdrEstmt);
      }
      /* Position the service accordingly */
      rgSCHSc1DlMngSvcPosn(cell, svc->ue, svc);
      /* Update effAlloc */
      *effAlloc -= bytes;

      /* Update DHM for this SVC */
      lchSchdData.lcId     = svc->lcId;
      lchSchdData.schdData = bytes;
      rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, tbInfo);
   }
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHSc1DlNewTxTbDstn 
 *        
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given TB "tbInfo" allocation.
 *     Assumption: TA is given higher priority in Alloc Distribution.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlNewTxTbDstn 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
CmLList        **node
)
#else
PRIVATE Void rgSCHSc1DlNewTxTbDstn(cell, ue, tbInfo, effAlloc, node)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
CmLList        **node;
#endif
{
   RgSchDlLcCb        *svc;
   RgSchSc1DlSvc      *svcSc1 = NULLP;
   RgSchSc1DlUe       *ueDl = RG_GET_SC1_UE_DL(ue);
   U32                bytes;
   RgSchLchAllocInfo  lchSchdData;
   CmLList            *prev = NULLP;

   TRC2(rgSCHSc1DlNewTxTbDstn);

   if (ueDl->taReqBytes)
   {
      if (ueDl->taReqBytes < *effAlloc)
      {
         /*TA satisfied, hence remove from TA Lst */
         rgSCHCmnRmvFrmTaLst(cell, ue);
         /* Indicate to DHM that TA has been scheduled */
         rgSCHDhmSchdTa(ue, tbInfo);
         *effAlloc -= ueDl->taReqBytes;
      }
      /* Reset the TA Req Bytes Field */
      ueDl->taReqBytes = 0;
   }
   while((*node) && (*effAlloc > 0))
   {
      svc = (RgSchDlLcCb *)(*node)->node;
      prev = *node;
      *node = (*node)->next;
      svcSc1 = RG_GET_SC1_SVC_DL(svc);
      if (*effAlloc > svcSc1->reqBytes)
      {
         bytes = svcSc1->reqBytes;
         if (bytes <= svcSc1->hdrEstimate)
         {
            break; 
         }
         /* 3.1 MIMO updating the reqBytes field */
         svcSc1->reqBytes = 0;
         svcSc1->bo = 0;
      }
      else
      {
         bytes = *effAlloc;
         if (bytes <= svcSc1->hdrEstimate)
         {
            break; 
         }
         /* 3.1 MIMO updating the reqBytes field */
         svcSc1->reqBytes -= bytes;
         (svcSc1->bo <= bytes-svcSc1->hdrEstimate)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-svcSc1->hdrEstimate);
      }
      svc->bo = svcSc1->bo;

      /* Update the SVC QOS Param */
      if (RG_SCH_CMN_SVC_IS_GBR(svc))
      {
         (svcSc1->gbr <= bytes)? (svcSc1->gbr = 0):
                                    (svcSc1->gbr -= bytes);
         (svcSc1->mbr <= bytes)? (svcSc1->mbr = 0):
                                    (svcSc1->mbr -= bytes);
      }
      else if(RG_SCH_CMN_SVC_IS_AMBR(svc))
      {
         (ueDl->ambr <= bytes)? (ueDl->ambr = 0):
                                       (ueDl->ambr -= bytes);
      }
      /* Position the service accordingly */
      rgSCHSc1DlMngSvcPosn(cell, svc->ue, svc);
      /* Update effAlloc */
      *effAlloc -= bytes;

      /* Update DHM for this SVC */
      lchSchdData.lcId     = svc->lcId;
      lchSchdData.schdData = bytes;
      rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, tbInfo);
   }
   /* If no more scheduled LCs for TB data distribution
    * then distribute the spare TB data among the LCs
    * of the UE with non-zero BO. This is effective for 
    * schedulers work on LC level priorities rather than
    * UE level. */
   if ((*node == NULLP) && (svcSc1) && (svcSc1->reqBytes == 0))
   {
      rgSCHSc1DlSprTxTbDstn(cell, ue, tbInfo, effAlloc,
                            &ueDl->lcsWithData.first);
      *node = NULLP;
      RETVOID;
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
 *     Func : rgSCHSc1DlNewTxUeFnlz 
 *        
 *     Desc : Perform allocation Distribution from scheduled TB
 *            among the list of services considered for scheduling.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlNewTxUeFnlz 
(
RgSchCellCb    *cell,
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb      *ue
)
#else
PRIVATE Void rgSCHSc1DlNewTxUeFnlz(cell, allocInfo, ue)
RgSchCellCb    *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchUeCb      *ue;
#endif
{
   CmLList            *node;
   RgSchSc1DlUe       *ueDl = RG_GET_SC1_UE_DL(ue);
   RgSchCmnDlUe       *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
   /* 3.1 MIMO Distribute data of each TB across services */
   U32                remTb1Bytes = ue->dl.dlAllocCb.tbInfo[0].bytesAlloc;
   U32                remTb2Bytes = ue->dl.dlAllocCb.tbInfo[1].bytesAlloc;
   U32                effAlloc = 0;
   /*ccpu00120365-ADD-added to code to check if second TB is utilized */
   U32                tb2Bytes = 0;

   TRC2(rgSCHSc1DlNewTxUeFnlz);


   /* 3.1 MIMO Consider the allocation of New TX TB for distribution */
   /* Handle schd services */
   node = ueDl->schdSvcs.first;
   if (remTb1Bytes){
      effAlloc += remTb1Bytes;
      rgSCHSc1DlNewTxTbDstn(cell, ue, &cmnUeDl->proc->tbInfo[0],\
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
      rgSCHSc1DlNewTxTbDstn(cell, ue, &cmnUeDl->proc->tbInfo[1],\
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
      /* Allocation such that Nothing could be satisfied */
      /* Return the grabbed PDCCH */
      rgSCHUtlPdcchPut(cell, &ue->dl.dlAllocCb.dlSf->pdcchInfo, 
            ue->dl.dlAllocCb.pdcch);
      rgSCHSc1RlsHqProc(cmnUeDl->proc);
      RETVOID;
   }

   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* Fill PDCCH and assign it to HqP */
   rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, cmnUeDl->proc);

   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        New TX allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlNewTxAllocFnlz 
 *     Purpose : Distribute the allocation among the Scheduled SVCs.
 *     Fill pdcch and HqP for UEs will allocations.
 *     Release HqP for UE with no allocation.
 *
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlNewTxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHSc1DlNewTxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   CmLList          *node;
   RgSchUeCb        *ue;
   RgSchCmnDlUe     *cmnUeDl;
   RgSchDlHqProcCb  *hqP;
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlNewTxAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlNewTxAllocFnlz\n"));

   node = allocInfo->dedAlloc.schdTxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);

      rgSCHSc1DlNewTxUeFnlz(cell, allocInfo, ue);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      /* reset the UE allocation Information */
      cmLListInit(&((RgSchSc1DlUe *)(cmnUeDl->schSpfc))->schdSvcs);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdTxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);

      /* Release HqProc */
      /* rg001.301 -MOD -[ccpu00118350] Correcting NDI manipulation of Harq */
      rgSCHSc1RlsHqProc(hqP);
      /* reset the UE allocation Information */
      cmLListInit(&((RgSchSc1DlUe *)(cmnUeDl->schSpfc))->schdSvcs);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/* 3.1 Added new function to handle TX+RETX alloc fnlz'n */

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX+New TX allocations. The NewTx TB allocation
 *        is considered for distribution among LCs.
 *
 * @details
 *
 *     Function: rgSCHSc1DlRetxNewTxAllocFnlz 
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
 *  @param[in]  RgSchCmnDlRbAllocInfo *cellAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlRetxNewTxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *cellAllocInfo
)
#else
PRIVATE Void rgSCHSc1DlRetxNewTxAllocFnlz(cell, cellAllocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *cellAllocInfo;
#endif
{
   CmLList          *node;
   RgSchUeCb        *ue;
   RgSchCmnDlUe     *ueDl;
   RgSchSc1DlUe     *sc1DlUe;
   RgSchDlHqProcCb  *hqP;
   RgSchDlHqTbCb    *newTxTbInfo;
   U32              effAlloc;
   U32              remTbBytes;
   RgSchDlRbAlloc   *ueAllocInfo; 
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlRetxNewTxAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlRetxNewTxAllocFnlz\n"));
   
   node = cellAllocInfo->dedAlloc.schdTxRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      sc1DlUe = RG_GET_SC1_UE_DL(ue);
      ueAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
      /* Index 0 of ueAllocInfo->tbInfo will always hold the 
       * RETX TB and index 1 will hold the NewTX TB in case of
       * RETX+TX allocation. */
      newTxTbInfo = ueAllocInfo->tbInfo[1].tbCb;
      effAlloc = remTbBytes  =  ueAllocInfo->tbInfo[1].bytesAlloc;
      rgSCHSc1DlSprTxTbDstn(cell, ue, newTxTbInfo,\
                     &remTbBytes, &(sc1DlUe->lcsWithData.first));
      /* Trying to reduce mcs of TX TB to increase reception quality at UE.
       * In case of RETX+TX allocation, TX TB allocation was irrespective
       * of actual requirement by UE, hence in case if consumption is 
       * less than allocation, consider reducing the iMcs of this TX TB. */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 1, effAlloc - remTbBytes);
      /* 3.1 MIMO Remove/Retain from/in cell RETX List */
      rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
      /* Fill PDCCH and assign it to HqP */
      rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, hqP);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxRetxUeLst and reset the UE allocation Info */
   node = cellAllocInfo->dedAlloc.nonSchdTxRetxUeLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
}


/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested. 
 *               1. Loop through scheduled TX and RETX lists.
 *                  Fill in the corresponding PDCCH and HqProcs.
 *                  In case of TX If actual Alloc < requested, then perform
 *                     an appropriate distribution among the schdSvcs.
 *                     If TA is satisfied, then remove UE from TA Lst.
 *               2. Loop through UnScheduled TX and RETX Lists.
 *                  Release grabbed HqProcs.
 *                  Put back SVCs from schdSvcsLst to their corresponding Qs.
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
PUBLIC Void rgSCHSc1DlAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHSc1DlAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1DlAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlAllocFnlz\n"));

   rgSCHSc1DlRetxAllocFnlz(cell, allocInfo);

   rgSCHSc1DlNewTxAllocFnlz(cell, allocInfo);

   /*3.1 MIMO new Function added to handle TX+RETX 
    * harq process scheduling finalization */
   rgSCHSc1DlRetxNewTxAllocFnlz(cell, allocInfo);
   RETVOID;
}



/**
 * @brief This function Updates the DL CQI for the UE. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlCqiInd 
 *     Purpose:  Updates the DL CQI for the UE
 *     
 *     Invoked by: Common Scheduler. SC1 does nothing.
 *     
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDlCqiRpt        *dlCqiRpt
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1DlCqiInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt
)
#else
PUBLIC Void rgSCHSc1DlCqiInd(cell, ue, dlCqiRpt)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDlCqiRpt        *dlCqiRpt;
#endif
{
   TRC2(rgSCHSc1DlCqiInd);
   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */

/**
 * @brief This function adds a service to UE's list of lcsWithData. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlAdd2UeLcsWithData
 *     Purpose:  1. This is to maintain a snapshot view of the 
 *               DL SVCs distributions among the cellwide priority
 *               queues. 
 *               2. This snapshot view is maintained in the order 
 *               of priority of the SVCs with in UE.
 *               3. Addition of SVC to a cellwide priority Queue
 *                  triggers this function.
 *     
 *     Invoked by: Functions of DL SC1 which add SVC or UE(for ambr svc)
 *                 to cellwide priority Queues.
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlAdd2UeLcsWithData
(
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlAdd2UeLcsWithData(ue, svc)
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchCmnDlSvc        *cmnDlSvc = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchSc1DlSvc        *sc1DlSvc = RG_GET_SC1_SVC_DL(svc);
   RgSchSc1DlUe         *sc1DlUe  = RG_GET_SC1_UE_DL(ue);
   RgSchCmnDlSvc        *cmnDlLstSvc;
#ifdef DEBUGP
   Inst                 inst = ue->cell->instIdx;
#endif

   TRC2(rgSCHSc1DlAdd2UeLcsWithData);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlAdd2UeLcsWithData\n"));
   
   lst  = &(sc1DlUe->lcsWithData);
   node = lst->first;
   while(node)
   {
      cmnDlLstSvc = RG_SCH_CMN_GET_DL_SVC(((RgSchDlLcCb *)(node->node)));
      if (cmnDlSvc->prio <= cmnDlLstSvc->prio)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      cmLListAdd2Tail(lst, &sc1DlSvc->lcWithDataLnk);
      sc1DlSvc->lcWithDataLnk.node = (PTR)svc;
   }
   else
   {
      lst->crnt = node;
      cmLListInsCrnt(lst, &sc1DlSvc->lcWithDataLnk);
      sc1DlSvc->lcWithDataLnk.node = (PTR)svc;
   }
   RETVOID;
}


/**
 * @brief This function adds a service to UE's list of lcsWithData. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlRmFrmUeLcsWithData
 *     Purpose:  1. This is to maintain a snapshot view of the 
 *               DL SVCs distributions among the cellwide priority
 *               queues. 
 *               2. This snapshot view is maintained in the order 
 *               of priority of the SVCs with in UE.
 *               3. Addition of SVC to a cellwide priority Queue
 *                  triggers this function.
 *     
 *     Invoked by: Functions of DL SC1 which add SVC or UE(for ambr svc)
 *                 to cellwide priority Queues.
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1DlRmFrmUeLcsWithData
(
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PRIVATE Void rgSCHSc1DlRmFrmUeLcsWithData(ue, svc)
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchSc1DlSvc        *sc1DlSvc = RG_GET_SC1_SVC_DL(svc);
   RgSchSc1DlUe         *sc1DlUe  = RG_GET_SC1_UE_DL(ue);
#ifdef DEBUGP
   Inst                 inst = ue->cell->instIdx;
#endif

   TRC2(rgSCHSc1DlRmFrmUeLcsWithData);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1DlRmFrmUeLcsWithData\n"));
   
   cmLListDelFrm(&(sc1DlUe->lcsWithData), &sc1DlSvc->lcWithDataLnk);
   sc1DlSvc->lcWithDataLnk.node = NULLP;
   RETVOID;
}
/***************** SC1 DL SCHEDULER FUNCTION DEFNs END HERE ****************/

/***************************************************************************/

/***************** SC1 UL SCHEDULER FUNCTION DEFNs START HERE **************/

/*--------------------------*
 * UL specific functions START
 *---------------------------*/


/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1UlInit
 *     Purpose:  This function initializes the following information
 *               1. Efficiency table
 *               2. CQI to table index - It is one row for upto 3 RBs
 *                  and another row for greater than 3 RBs
 *     
 *               currently extended prefix is compiled out.
 *     Invoked by: MAC intialization code..may be ActvInit
 *     
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlInit
(
RgUlSchdApis *rgSchUlApis
)
#else
PUBLIC Void rgSCHSc1UlInit(rgSchUlApis)
RgUlSchdApis *rgSchUlApis;
#endif
{
   TRC2(rgSCHSc1UlInit);
   /* Init the function pointers */
   rgSchUlApis->rgSCHRgrUlUeCfg     = rgSCHSc1RgrUlUeCfg;
   rgSchUlApis->rgSCHRgrUlUeRecfg   = rgSCHSc1RgrUlUeRecfg;
   rgSchUlApis->rgSCHFreeUlUe       = rgSCHSc1UlUeDel;
   rgSchUlApis->rgSCHRgrUlCellCfg   = rgSCHSc1RgrUlCellCfg;
   rgSchUlApis->rgSCHRgrUlCellRecfg = rgSCHSc1RgrUlCellRecfg; 
   rgSchUlApis->rgSCHFreeUlCell     = rgSCHSc1UlCellDel;
   rgSchUlApis->rgSCHRgrUlLcgCfg    = rgSCHSc1RgrLcgCfg;
   rgSchUlApis->rgSCHRgrUlLcgRecfg  = rgSCHSc1RgrLcgRecfg;
   rgSchUlApis->rgSCHFreeUlLcg      = rgSCHSc1LcgDel;
   rgSchUlApis->rgSCHUlSched        = rgSCHSc1UlSched;
   rgSchUlApis->rgSCHUpdBsrShort    = rgSCHSc1UpdBsrShort;
   rgSchUlApis->rgSCHUpdBsrTrunc    = rgSCHSc1UpdBsrTrunc;
   rgSchUlApis->rgSCHUpdBsrLong     = rgSCHSc1UpdBsrLong;
   rgSchUlApis->rgSCHContResUlGrant = rgSCHSc1ContResUlGrant;
   rgSchUlApis->rgSCHSrRcvd         = rgSCHSc1SrRcvd;
   rgSchUlApis->rgSCHUlCqiInd       = rgSCHSc1UlCqiInd;
   rgSchUlApis->rgSCHUlUeRefresh    = rgSCHSc1UlUeRefresh;
   rgSchUlApis->rgSCHUlAllocFnlz    = rgSCHSc1UlAllocFnlz;
   rgSchUlApis->rgSCHUlInactvtUes   = rgSCHSc1UlHndlInActUes;
   rgSchUlApis->rgSCHUlActvtUe      = rgSCHSc1UlActvtUe;
   rgSchUlApis->rgSCHUlUeReset      = rgSCHSc1UlUeReset;

   RETVOID;
}

/**
 * @brief UE initialisation for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrUlUeCfg
 *     
 *     This functions intialises UE specific scheduler
 *     information
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrUlUeCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrUlUeCfg(cell, ue, ueCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeCfg     *ueCfg;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHSc1RgrUlUeCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&(((RgSchCmnUe *)((ue)->sch))->ul.schSpfc), \
      (sizeof(RgSchSc1UlUe))) != ROK)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), \
      "Memory allocation FAILED"));
      err->errCause = RGSCHERR_SCH_SC1_UL_CFG;
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHSc1RgrUlUeCfg */

/**
 * @brief UE reconfiguration for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrUlUeRecfg
 *     
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrUlUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrUlUeRecfg(cell, ue, ueRecfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHSc1RgrUlUeRecfg);
   RETVALUE(ROK);
}  /* rgSCHSc1RgrUeRecfg */

/**
 * @brief UE deletion for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1UlUeDel
 *     
 *     This functions deletes all scheduler information
 *     pertaining to a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlUeDel
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHSc1UlUeDel(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchSc1UlCell *cellUl = RG_GET_SC1_CELL_UL(cell);
   RgSchSc1UlUe   *ueUl   = RG_GET_SC1_UE_UL(ue);
   
   TRC2(rgSCHSc1UlUeDel);

   if (ueUl == NULLP)
   {
      RETVOID;
   }
   if(ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = NULLP;
   }
   if(ueUl->contResLnk.node)
   {
      cmLListDelFrm(&(cellUl->contResLst), &(ueUl->contResLnk));
      ueUl->contResLnk.node = NULLP;
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(ueUl)), (sizeof(RgSchSc1UlUe)));

   RETVOID;
}  /* rgSCHSc1UlUeDel */

/**
 * @brief UE Reset for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1UlUeReset
 *     
 *     Remove this UE from all Scheduling Priority Qs
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlUeReset 
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHSc1UlUeReset(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchSc1UlCell *cellUl = RG_GET_SC1_CELL_UL(cell);
   RgSchSc1UlUe   *ueUl   = RG_GET_SC1_UE_UL(ue);
   
   TRC2(rgSCHSc1UlUeReset);

   if(ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = NULLP;
   }
   if(ueUl->contResLnk.node)
   {
      cmLListDelFrm(&(cellUl->contResLst), &(ueUl->contResLnk));
      ueUl->contResLnk.node = NULLP;
   }
   RETVOID;
}  /* rgSCHSc1UlUeReset */


/**
 * @brief Scheduler processing on cell configuration
 *
 * @details
 *
 *     Function : rgSCHSc1RgrUlCellCfg
 *     
 *     This function does requisite initialisation 
 *     and setup for scheduler1 when a cell is
 *     configured
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrUlCellCfg
(
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
)
#else
PUBLIC S16 rgSCHSc1RgrUlCellCfg(cell, cellCfg, err)
RgSchCellCb   *cell;
RgrCellCfg    *cellCfg;
RgSchErrInfo  *err;
#endif
{
   RgSchSc1UlCell     *cellUl; 

   TRC2(rgSCHSc1RgrUlCellCfg);

   if((rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&(((RgSchCmnCell*)((cell)->sc.sch))->ul.schSpfc), \
      (sizeof(RgSchSc1UlCell))) != ROK))
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx), \
      "Memory allocation FAILED"));
      err->errCause = RGSCHERR_SCH_SC1_UL_CFG;
      RETVALUE(RFAILED);
   }
   cellUl = RG_GET_SC1_CELL_UL(cell);
   cmLListInit(&cellUl->contResLst);
   cmLListInit(&cellUl->ueTxLst[0]);
   cmLListInit(&cellUl->ueTxLst[1]);

   RETVALUE(ROK);
}  /* rgSCHSc1RgrUlCellCfg */


/**
 * @brief This function handles the reconfiguration of cell 
 *
 * @details
 *
 *     Function: rgSCHSc1RgrUlCellRecfg
 *     Purpose:  Update the reconfiguration parameters.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrUlCellRecfg
(
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
)
#else
PUBLIC S16 rgSCHSc1RgrUlCellRecfg(cell, recfg, err)
RgSchCellCb             *cell;
RgrCellRecfg            *recfg;
RgSchErrInfo            *err;
#endif
{
   TRC2(rgSCHSc1RgrUlCellRecfg);
   RETVALUE(ROK);
}

/**
 * @brief Scheduler processing for cell delete
 *
 * @details
 *
 *     Function : rgSCHSc1UlCellDel
 *     
 *     This functions de-initialises and frees memory
 *     taken up by scheduler1 for the entire cell.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlCellDel
(
RgSchCellCb  *cell
)
#else
PUBLIC Void rgSCHSc1UlCellDel(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchSc1UlCell *cellUl = RG_GET_SC1_CELL_UL(cell);

   TRC2(rgSCHSc1UlCellDel);

   if (cellUl == NULLP)
   {
      RETVOID;
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cellUl)), (sizeof(RgSchSc1UlCell)));

   RETVOID;
}  /* rgSCHSc1UlCellDel */

/**
 * @brief Scheduler invocation on logical channel Group addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcgCfg
 *     
 *     This functions does required processing when a new
 *     (dedicated) logical channel is added. Assumes lcg
 *     pointer in ulLc is set.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb   *lcg
 *  @param[int] RgrLcgCfg    *lcgCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrLcgCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgCfg    *lcgCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrLcgCfg(cell, ue, lcg, lcgCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *lcg;
RgrLcgCfg    *lcgCfg;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHSc1RgrLcgCfg);
   RETVALUE(ROK);
}  /* rgSCHSc1RgrLcgCfg */

/**
 * @brief Scheduler invocation on logical channel addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcgRecfg
 *     
 *     This functions does required processing when an existing
 *     (dedicated) logical channel is reconfigured. Assumes lcg
 *     pointer in ulLc is set to the old value.
 *     Independent of whether new LCG is meant to be configured,
 *     the new LCG scheduler info is accessed and possibly modified.
 *
 *  @param[in]  RgSchCellCb  *cell,
 *  @param[in]  RgSchUeCb    *ue,
 *  @param[in]  RgSchLcgCb   *lcg,
 *  @param[in]  RgrLcgRecfg  *reCfg,
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHSc1RgrLcgRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgRecfg  *reCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHSc1RgrLcgRecfg(cell, ue, lcg, reCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *lcg;
RgrLcgRecfg  *reCfg;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHSc1RgrLcgRecfg);
   RETVALUE(ROK);
}  /* rgSCHSc1RgrLcgRecfg */

/***********************************************************
 *
 *     Func : rgSCHSc1LcgDel
 *        
 *     Desc : Scheduler handling for a (dedicated)
 *             uplink lcg being deleted
 *
 *     Ret  : 
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHSc1LcgDel
(
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchLcgCb    *lcg
)
#else
PUBLIC Void rgSCHSc1LcgDel(cell, ue, lcg)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
RgSchLcgCb    *lcg;
#endif
{
   TRC2(rgSCHSc1LcgDel);
   rgSCHSc1UlPosnUeInQ(cell, ue);
   RETVOID;
}

/**
 * @brief Perform alloction for this UE 
 *
 * @details
 *
 *     Function : rgSCHSc1UlSchdUe
 *
 *     Processing Steps: cater to as much as UE needs, with 
 *     a limitation on maxBits per scheduling instance(per TTI)
 *     per UE. Return failure, if UE is not considered 
 *     for scheduling (case, where it is already selected for a
 *     retransmission).
 *                   
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlSchdUe
(
RgSchUeCb         *ue
)
#else
PRIVATE Void rgSCHSc1UlSchdUe(ue)
RgSchUeCb         *ue;
#endif
{
   RgSchCmnUlUe      *cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchSc1UlUe      *ulUe    = RG_GET_SC1_UE_UL(ue);

   TRC2(rgSCHSc1UlSchdUe);

   if(ulUe->srRcvd == TRUE)
   {
      cmnUlUe->alloc.reqBytes = RGSCH_MAX(RG_SCH_CMN_UL_SR_BYTES, \
                                cmnUlUe->effBsr);
      RETVOID;
   }

   cmnUlUe->alloc.reqBytes = cmnUlUe->effBsr;

   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSchdForDataTrans
 *     Purpose:  Uplink Scheduling for UE data Transmission.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @param[in] U8                     remUe 
 *  @return Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlSchdForDataTrans
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo,
U8                    remUe
)
#else
PRIVATE Void rgSCHSc1UlSchdForDataTrans(cell, allocInfo, remUe)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
U8                    remUe;
#endif
{
   RgSchSc1UlCell    *sc1UlCell  = RG_GET_SC1_CELL_UL(cell);

   TRC2(rgSCHSc1UlSchdForDataTrans);

   if (remUe == 0)
   {
      RETVOID;
   }
   /* Allocate UEs with LCG0 data pending */
   rgSCHSc1UlSchdUeTxLst(cell, &sc1UlCell->ueTxLst[0], allocInfo, &remUe);

   if (remUe == 0)
   {
      RETVOID;
   }
   /* Allocate UEs with other LCGs data pending */
   rgSCHSc1UlSchdUeTxLst(cell, &sc1UlCell->ueTxLst[1], allocInfo, &remUe);

   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSchdUeTxLst
 *     Purpose:  Uplink Scheduling for UE data Transmission.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  CmLListCp             *ueTxLst
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @param[in] U8                     *remUe 
 *  @return Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlSchdUeTxLst
(
RgSchCellCb           *cell,
CmLListCp             *ueTxLst,
RgSchCmnUlRbAllocInfo *allocInfo,
U8                    *remUe
)
#else
PRIVATE Void rgSCHSc1UlSchdUeTxLst(cell, ueTxLst, allocInfo, remUe)
RgSchCellCb           *cell;
CmLListCp             *ueTxLst;
RgSchCmnUlRbAllocInfo *allocInfo;
U8                    *remUe;
#endif
{
   RgSchUeCb         *ue;
   CmLList           *node;
#ifdef LTEMAC_HDFDD
   Bool              ulAllowed = FALSE;
#endif

   TRC2(rgSCHSc1UlSchdUeTxLst);

   node = ueTxLst->first;
   while ((node) && (*remUe))
   {
      ue = (RgSchUeCb *)node->node;
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

      if (RG_SCH_CMN_IS_UL_UE_RETX(ue))
      {
         /* UE already scheduled in this subframe (for retx) 
          * OR is inactive for UL Transmission.*/
         continue;
      }
      /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
         else if (RG_SCH_CMN_IS_SPS_SCHD(ue))
         {
            /*-- Already Scheduled by SPS --*/
           continue;
         }
#endif

      rgSCHSc1UlSchdUe(ue);

      rgSCHCmnUlAdd2UeLst(allocInfo, ue);

      --(*remUe);
   }

   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSchdForContRes
 *     Purpose:  Uplink Scheduling for Contention Resolution.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @param[out] U8                    *remUe 
 *  @return Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlSchdForContRes
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo,
U8                    *remUe
)
#else
PRIVATE Void rgSCHSc1UlSchdForContRes(cell, allocInfo, remUe)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
U8                    *remUe;
#endif
{
   RgSchSc1UlCell    *sc1UlCell  = RG_GET_SC1_CELL_UL(cell);
   RgSchUeCb         *ue;
   CmLList           *node;
   RgSchCmnUlUe      *cmnUlUe;
#ifdef LTEMAC_HDFDD
   Bool                 ulAllowed = FALSE;
#endif

   TRC2(rgSCHSc1UlSchdForContRes);

   node = sc1UlCell->contResLst.first;
   while ((node) && (*remUe))
   {
      ue = (RgSchUeCb *)node->node;
      cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue);
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
      if (RG_SCH_CMN_IS_UL_UE_RETX(ue))
      {
         /* UE already scheduled in this subframe (for retx) 
          * OR is inactive for UL Transmission.*/
         continue;
      }
      cmnUlUe->alloc.reqBytes = RG_SCH_CMN_MAX_UL_CONTRES_GRNT;
      rgSCHCmnUlAdd2CntResLst(allocInfo, ue);
      --(*remUe);
      /* Node removal deferred to ULAllocFinalization */
   }

   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlNewTx
 *     Purpose:  Uplink Scheduling for New Transmissions.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @return Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlNewTx
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHSc1UlNewTx(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   U8               remUe = cellUl->maxUeNewTxPerTti;

   TRC2(rgSCHSc1UlNewTx);

   rgSCHSc1UlSchdForContRes(cell, allocInfo, &remUe);
   rgSCHSc1UlSchdForDataTrans(cell, allocInfo, remUe);
   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSched
 *     Purpose:  This function implements an UL scheduler for LTE. This is
 *               made available as a function pointer to be called
 *               at the time of TTI processing by the MAC.
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @return Void 
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlSched 
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHSc1UlSched(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
   TRC2(rgSCHSc1UlSched);
   rgSCHSc1UlNewTx(cell, allocInfo);
   RETVOID;
}

/**
 * @brief UEs Buffer Status Has changed so reposition it. 
 *
 * @details
 *
 *     Function : rgSCHSc1UlInsUeInQ 
 *     
 *     In UE in the list in Descending order of effBsr.
 *     
 *
 *  @param[in]  CmLListCp    *lst
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlInsUeInQ 
(
CmLListCp           *lst,
RgSchUeCb           *ue
)
#else
PRIVATE Void rgSCHSc1UlInsUeInQ(lst, ue)
CmLListCp           *lst;
RgSchUeCb           *ue;
#endif
{
   RgSchCmnUlUe    *cmnUeUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchSc1UlUe    *ueUl    = RG_GET_SC1_UE_UL(ue);
   RgSchUeCb       *lUe;
   RgSchCmnUlUe    *lcmnUeUl;
   CmLList         *node;

   TRC2(rgSCHSc1UlInsUeInQ);

   node = lst->first;
   while(node)
   {
      lUe = (RgSchUeCb *)(node->node);
      lcmnUeUl = RG_SCH_CMN_GET_UL_UE(lUe);
      if (lcmnUeUl->effBsr <= cmnUeUl->effBsr)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      /* We have come to the end of the queue, so Append */
      cmLListAdd2Tail(lst, &ueUl->txLnk);
      ueUl->txLnk.node = (PTR)ue;
   }
   else
   {
      lst->crnt = node;
      cmLListInsCrnt(lst, &ueUl->txLnk);
      ueUl->txLnk.node = (PTR)ue;
   }

   RETVOID;
}
/**
 * @brief UEs Buffer Status Has changed so reposition it. 
 *
 * @details
 *
 *     Function : rgSCHSc1UlPosnUeInQ
 *     
 *     -Ues bs value for its LCG has changed, due to either 
 *     allocation or BSR report OR the effUeBR, i.e the byteRate
 *     has changed due to some allocation, so add/reposition/remove
 *     it from Qs based on this new bs and/or effUeBR value.
 *     -If UE has non-zero lcg0 bs value, but the byteRate is 
 *     consumed totally, UE is still schedulable for this control data. 
 *     -If UE's LCG0 has pending bs then position this UE in 
 *     ueTxLst[0].
 *     -If Ue has pending BSR to be satisfied, but lcg0's BS
 *     is 0, then position it in ueTxLst[1].
 *     -In any of these 2 Qs, insertion is such that UEs are
 *     positioned in Descending order of their Pending BS.
 *     
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlPosnUeInQ 
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PRIVATE Void rgSCHSc1UlPosnUeInQ(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchSc1UlUe    *ueUl   = RG_GET_SC1_UE_UL(ue);
   RgSchCmnUlUe    *cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchSc1UlCell  *cellUl = RG_GET_SC1_CELL_UL(cell);
   RgSchCmnLcg     *cmnLcg0 = RG_SCH_CMN_GET_UL_LCG(&ue->ul.lcgArr[0]);
   CmLListCp       *lst;

   TRC2(rgSCHSc1UlPosnUeInQ);

   if (!RG_SCH_CMN_UL_IS_UE_ACTIVE(ue))
   {
      RETVOID;
   }
                     
   /* Remove the UE from its existing position */
   if (ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = (PTR)NULLP;
   }
   /* If UE has still bs left for scheduling 
    * then reposition it */
   if ((cmnUlUe->effBsr > 0) || (ueUl->srRcvd == TRUE))
   {
      /* Select the Queue where UE would be Placed */
      if (cmnLcg0->bs > 0)
      {
         lst = &cellUl->ueTxLst[0];
         ueUl->qId = 0;
      }
      else
      {
         lst = &cellUl->ueTxLst[1];
         ueUl->qId = 1;
      }
      /* Insert the UE in the Q */
      rgSCHSc1UlInsUeInQ(lst, ue); 
   }
#ifdef RGR_V1
   /* rg006.201: [ccpu00112398] Added periodic BSR timer */
   else if(cmnUlUe->totalBsr != 0)
   {
      if (ue->bsrTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue); 
      }
      if (ue->ul.bsrTmrCfg.isPrdBsrTmrPres)
      {
         rgSCHTmrStartTmr(cell, ue, RG_SCH_TMR_BSR, 
               ue->ul.bsrTmrCfg.prdBsrTmr);
      }
   }
#endif

   RETVOID;
}

/**
 * @brief Short BSR update
 *
 * @details
 *
 *     Function : rgSCHSc1UpdBsrShort
 *     
 *     This functions does requisite updates to handle short BSR reporting
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb   *lcg
 *  @param[in]  U8           bsr
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UpdBsrShort
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
U8           bsr
)
#else
PUBLIC Void rgSCHSc1UpdBsrShort(cell, ue, lcg, bsr)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *lcg;
U8           bsr;
#endif
{
   TRC2(rgSCHSc1UpdBsrShort);
   rgSCHSc1UlPosnUeInQ(cell, ue);
   RETVOID;
}  /* rgSCHSc1UpdBsrShort */

/**
 * @brief Truncated BSR update
 *
 * @details
 *
 *     Function : rgSCHSc1UpdBsrTrunc
 *     
 *     This functions does required updates to handle truncated BSR report
 *     
 *           
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb   *lcg
 *  @param[in]  U8           bsr
 *  @return  Void 
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UpdBsrTrunc
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
U8           bsr
)
#else
PUBLIC Void rgSCHSc1UpdBsrTrunc(cell, ue, lcg, bsr)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *lcg;
U8           bsr;
#endif
{
   TRC2(rgSCHSc1UpdBsrTrunc);
   rgSCHSc1UlPosnUeInQ(cell, ue);
   RETVOID;
}  /* rgSCHSc1UpdBsrTrunc */

/**
 * @brief Long BSR update
 *
 * @details
 *
 *     Function : rgSCHSc1UpdBsrLong
 *
 *     - Update UE's position within/across uplink scheduling queues
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8 bsArr[]
 *  @return  Void 
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UpdBsrLong
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           *bsArr
)
#else
PUBLIC Void rgSCHSc1UpdBsrLong(cell, ue, bsArr)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           *bsArr;
#endif
{
   TRC2(rgSCHSc1UpdBsrLong);
   rgSCHSc1UlPosnUeInQ(cell, ue);
   RETVOID;
}  /* rgSCHSc1UpdBsrLong */

/**
 * @brief UL grant for contention resolution
 *
 * @details
 *
 *     Function : rgSCHSc1ContResUlGrant
 *     
 *     Add UE to another queue specifically for CRNTI based contention
 *     resolution
 *     
 *           
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return Void 
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1ContResUlGrant
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHSc1ContResUlGrant(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchSc1UlUe    *ueUl   = RG_GET_SC1_UE_UL(ue);
   RgSchSc1UlCell  *cellUl = RG_GET_SC1_CELL_UL(cell);

   TRC2(rgSCHSc1ContResUlGrant);

   if (ueUl->contResLnk.node)
   {
      RETVOID;
   }

   /* Remove the UE from other Qs */
   if(ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = NULLP;
   }

   cmLListAdd2Tail(&cellUl->contResLst, &ueUl->contResLnk);
   ueUl->contResLnk.node = (PTR)ue;
   RETVOID;
}  /* rgSCHSc1ContResUlGrant */

/**
 * @brief SR reception handling
 *
 * @details
 *
 *     Function : rgSCHSc1SrRcvd
 *     Shift the UE with SrInd in to the lcgO queue.
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1SrRcvd
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHSc1SrRcvd(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchSc1UlUe    *ulUe    = RG_GET_SC1_UE_UL(ue);
   RgSchSc1UlCell  *ulCell  = RG_GET_SC1_CELL_UL(cell);

   TRC2(rgSCHSc1SrRcvd);

   ulUe->srRcvd = TRUE;

   if (ulUe->txLnk.node != NULLP)
   {
      if (ulUe->qId == 0)
      {
         /* Already present in lcg0 Q */
         RETVOID;
      }
      cmLListDelFrm(&(ulCell->ueTxLst[ulUe->qId]), &(ulUe->txLnk));
   }
   /* Adding the UE to the LCG0 list for SR IND */
   cmLListAdd2Tail(&ulCell->ueTxLst[0], &ulUe->txLnk);
   ulUe->txLnk.node = (PTR)ue;
   ulUe->qId = 0;

   RETVOID;
}  /* rgSCHSc1SrRcvd */

/**
 * @brief Indication of UL CQI
 *
 * @details
 *
 *     Function : rgSCHSc1UlCqiInd 
 *
 *     - Common Scheduler. SC1 does nothing. 
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  TfuUlCqiRpt         *ulCqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
)
#else
PUBLIC Void rgSCHSc1UlCqiInd(cell, ue, ulCqiInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuUlCqiRpt          *ulCqiInfo;
#endif
{
   TRC2(rgSCHSc1UlCqiInd);

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}


/***********************************************************
 *
 *     Func : rgSCHSc1UlUeRefresh
 *        
 *     Desc : Handle 'refresh' for uplink part of a UE
 *            (ie UE's uplink AMBR and uplink GBR LCGs are
 *            refreshed at this point)
 *
 *     Ret  : 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlUeRefresh
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHSc1UlUeRefresh(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   TRC2(rgSCHSc1UlUeRefresh);
   rgSCHSc1UlPosnUeInQ(cell, ue);
   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        UE data Trans Allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1UlDatTransAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               UE data Trans Allocations . 
 *     
 *     Invoked by: Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlDatTransAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHSc1UlDatTransAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
   RgSchSc1UlUe         *ueUl;
   RgSchUeCb         *ue;
   CmLList           *node;
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
#ifdef LTEMAC_DRX
   RgSchDrxUeCb      *drxUe    = NULLP;
   CmLListCp         ulInactvLst; /* list of UE's becoming UL-inactive */
#endif
   TRC2(rgSCHSc1UlDatTransAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1UlDatTransAllocFnlz\n"));

#ifdef LTEMAC_DRX
   cmLListInit(&ulInactvLst);
#endif
   node = allocInfo->schdUeLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
      ueUl = RG_GET_SC1_UE_UL(ue);

#ifdef LTEMAC_DRX
    if (ue->isDrxEnabled)
    {
      if(ueUl->srRcvd == TRUE)
      {
         drxUe = RG_SCH_DRX_GET_UE(ue);
         drxUe->drxUlInactvMask  |= RG_SCH_DRX_SR_BITMASK;

         if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
         {
            ue->ul.ulInactvMask |= RG_DRX_INACTIVE;
            /* Add to Ul inactive List */
            ue->ulDrxInactvLnk.node  = (PTR)ue;
            cmLListAdd2Tail(&ulInactvLst,&(ue->ulDrxInactvLnk));
         }
         drxUe->srRcvd = FALSE;
      }
    }
#endif
      /* Reset no matter */
      ueUl->srRcvd = FALSE;
      /* Reposition UE in Qs */
      rgSCHSc1UlPosnUeInQ(cell, ue);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(ue);
   }
#ifdef LTEMAC_DRX
   rgSCHSc1UlHndlInActUes(cell, &ulInactvLst);
#endif
   node = allocInfo->nonSchdUeLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(ue);
   }

   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        cont res Allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1UlContResAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               cont res Allocations . 
 *     
 *     Invoked by: Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSc1UlContResAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHSc1UlContResAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
   RgSchSc1UlCell    *sc1UlCell  = RG_GET_SC1_CELL_UL(cell);
   RgSchSc1UlUe      *ueUl;
   RgSchUeCb         *ue;
   CmLList           *node;
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1UlContResAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1UlContResAllocFnlz\n"));

   node = allocInfo->schdContResLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      ueUl = RG_GET_SC1_UE_UL(ue);

      /* Remove UE from Cont Res Q */ 
      cmLListDelFrm(&sc1UlCell->contResLst,
      &ueUl->contResLnk);
      ueUl->contResLnk.node = NULLP;
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(ue);
   }

   node = allocInfo->nonSchdContResLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(ue);
   }

   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested. 
 *
 * @details
 *
 *     Function: rgSCHSc1UlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested. 
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHSc1UlAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
#ifdef DEBUGP
   Inst        inst = cell->instIdx;  
#endif
   TRC2(rgSCHSc1UlAllocFnlz);
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHSc1UlAllocFnlz\n"));

   rgSCHSc1UlContResAllocFnlz(cell, allocInfo);
   rgSCHSc1UlDatTransAllocFnlz(cell, allocInfo);

   RETVOID;
}


/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlActvtUe 
 *     Purpose:  Put back the UE in to appropriate Qs.
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlActvtUe 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHSc1UlActvtUe(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   TRC2(rgSCHSc1UlActvtUe);

   rgSCHSc1UlPosnUeInQ(cell, ue);
   RETVOID;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlHndlInActUes
 *     Purpose:  The list of inactive UEs present in inactvLst should
 *               be removed from the scheduling Qs.
 *               But store the information pertaining to which Qs,
 *               were they belonging to. This information shall be used 
 *               to put them back in appropriate Qs when their Activation
 *               is initiated.
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] CmLListCp   *inactvLst
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHSc1UlHndlInActUes
(
RgSchCellCb           *cell,
CmLListCp             *inactvLst
)
#else
PUBLIC Void rgSCHSc1UlHndlInActUes(cell, inactvLst)
RgSchCellCb           *cell;
CmLListCp             *inactvLst;
#endif
{
   RgSchUeCb    *ue;
   RgSchSc1UlUe    *ulUe;
   RgSchSc1UlCell  *cellUl  = RG_GET_SC1_CELL_UL(cell);
   CmLList      *node = inactvLst->first;

   TRC2(rgSCHSc1UlHndlInActUes);

   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      ulUe = RG_GET_SC1_UE_UL(ue);
      if(ulUe->txLnk.node)
      {
         cmLListDelFrm(&(cellUl->ueTxLst[ulUe->qId]), &(ulUe->txLnk));
         /* This is required as lcg bs might change during
          * inactivity to activity. So we need to recompute 
          * its position. */
         ulUe->txLnk.node = NULLP;
      }
      /* Do not remove UE from contResLst */
   }
   RETVOID;
}


/**********************************************************************
 
         End of file:     gk_sch_sc1.c@@/main/3 - Sat Jul 30 02:21:49 2011
 
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
/main/2      ---        apany   1. LTE MAC 2.1 release
             rg004.201  ms      1. DL DTX Handling
             rg006.201  sd      1. ccpu00112398: Added periodic BSR timer
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependency on MIMO compile-time flag
               rsharon 2.Added support for SPS.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
           rg001.301  nudupi 1. ccpu00118350:Correcting NDI manipulation of Harq.
           rg002.301  rntiwari 1.ccpu00120941: Added handling for deleting UE
                                 from txUeLst and resetting outStndAlloc.
           rg003.301  hsingh   1. ccpu00120794-Freeing up the HARQ proc blocked 
                                  for indefinite time in case of Retx
*********************************************************************91*/
