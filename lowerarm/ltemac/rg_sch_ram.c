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
  
     File:     rg_sch_ram.c
  
     Sid:      gk_sch_ram.c@@/main/2 - Sat Jul 30 02:21:48 2011
  
     Prg:     ps 
  
**********************************************************************/

/** @file rg_sch_ram.c
@brief This file has APIs to handle the random access procedure functionality for the scheduler.
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE */

#include "rg_env.h"        /* MAC Environment Defines */
#include "rgr.h"           /* RGR Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "lrg.h"           /* LRG Interface defines */
#include "rg_env.h"    /* Scheduler error defines */
#include "rg_sch_inf.h"        /* Scheduler defines */
#include "rg_sch_err.h"    /* Scheduler error defines */
#include "rg_sch.h"        /* Scheduler defines */
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* Timer */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE */

#include "rgr.x"           /* RGR Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* Scheduler includes */
#include "rg_sch_cmn.x"

/* local defines */
/* local typedefs */
 
PRIVATE S16 rgSCHRamContResCrnti   ARGS((RgSchCellCb *cell, RgSchUeCb *ue, 
                                      RgSchRaCb *raCb, RgSchErrInfo *err));
PRIVATE S16 rgSCHRamContResCcchsdu ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
#ifdef RGR_V1
/*rg009.201 - changes for CR timer implementation*/
PRIVATE Void rgSCHChkContResGrdTmrExp ARGS((RgSchCellCb        *cell));
PRIVATE Void rgSCHChkContResTmrExp ARGS((RgSchCellCb        *cell));
PRIVATE Void rgSCHRamProcContResExp ARGS((RgSchCellCb *cell, 
                                 RgSchRaCb  *raCb));
PRIVATE Void rgSCHRamProcContResGrdExp ARGS((RgSchCellCb *cell,
                                      RgSchRaCb  *raCb));
#endif
PRIVATE Void rgSCHRamIndRntiRls2Mac ARGS(( RgSchCellCb    *cell,
         RgSchRaCb      *raCb));
/* forward references */

/**
 * @brief Check configured preamble id not colliding with non dedicated or PDCCH
 * order preamble sets. When valid preamble id given check that C-RNTI given
 * in configuration is not amongst the C-RNTI'smanaged by scheduler
 *
 * @details
 *
 *     Function : rgSCHRamVldtUeCfg 
 *
 *     Processing Steps: Check configured preamble id not colliding with non dedicated or PDCCH
 *       order preamble sets. When valid preamble id given check that C-RNTI given
 *       in configuration is not amongst the C-RNTI'smanaged by scheduler
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamVldtUeCfg 
(
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
)
#else
PUBLIC S16 rgSCHRamVldtUeCfg(cell, ueCfg)
RgSchCellCb  *cell;
RgrUeCfg     *ueCfg;
#endif
{
   TRC2(rgSCHRamVldtUeCfg);
   if (ueCfg->dedPreambleId.pres == PRSNT_NODEF)
   {
      if ((ueCfg->dedPreambleId.val < cell->rachCfg.numRaPreamble) ||
          (ueCfg->dedPreambleId.val >= RGSCH_MAX_NUM_RA_PREAMBLE) ||
          ((ueCfg->dedPreambleId.val >= cell->macPreambleSet.start) && 
           (ueCfg->dedPreambleId.val <= cell->macPreambleSet.start +
                                       cell->macPreambleSet.size - 1)) ||
          ((ueCfg->crnti >= cell->rntiDb.rntiStart) && 
           (ueCfg->crnti < cell->rntiDb.rntiStart + cell->rntiDb.maxRntis-1)))
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief Handler for Random Access Request
 *
 * @details
 *
 *     Function : rgSCHRamProcRaReq
 *     
 *     -# Create a node for each TfuRaReqInfo element received
 *     -# Initialize the list with the above elements at the raRnti index 
 *        in the cell. 
 *     
 *           
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  CmLteRnti      raRnti
 *  @param[in]  RgTfuRaReqInd *raReqInd
 *  @param[out] RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamProcRaReq
(
RgSchCellCb       *cell,
CmLteRnti         raRnti,
TfuRachInfo       *raReqInd,
CmLteTimingInfo   timingInfo,
RgSchErrInfo      *err
)
#else
PUBLIC S16 rgSCHRamProcRaReq(cell, raRnti, raReqInd, timingInfo, err)
RgSchCellCb       *cell;
CmLteRnti         raRnti;
TfuRachInfo       *raReqInd;
CmLteTimingInfo   timingInfo;
RgSchErrInfo      *err;
#endif
{
   RgSchRaReqInfo *raReqInfo;
   U8             idx;
   U16            raIndex;
#ifdef DEBUGP
   Inst           inst = cell->instIdx;
#endif
#ifdef LTE_TDD
   U8             ulDlCfgIdx = cell->ulDlCfgIdx;
   U8             maxUlSubframes;
   U8             fid;
   U8             tid;
#endif

   TRC2(rgSCHRamProcRaReq)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamProcRaReq(): cellId = %u; "
            "raRnti = %u\n", cell->cellId, raRnti));

   /* allocate new raReqInfos and enqueue them */
   for (idx = 0; idx < raReqInd->numRaReqInfo; idx++)
   {
      if (raReqInd->raReqInfoArr[idx].rapId >= RGSCH_MAX_NUM_RA_PREAMBLE)
      {
         RGSCHDBGINFO(inst, (rgSchPBuf(inst), "rgSCHRamProcRaReq(): RaReq "
           "with invalid rapId=%u\n", raReqInd->raReqInfoArr[idx].rapId));
         /* Move on to the next raReq */
         continue;
      }
#ifdef LTE_L2_MEAS
      if (raReqInd->raReqInfoArr[idx].rapId < cell->rachCfg.sizeRaPreambleGrpA)
      {
         cell->raPrmbs.preamGrpA++;
      }
      else if (RGSCH_IS_DEDPRM(cell, raReqInd->raReqInfoArr[idx].rapId))
      {
         cell->raPrmbs.dedPream++;
      }
      else
      {
         cell->raPrmbs.preamGrpB++;
      }
#endif

      if((rgSCHUtlAllocSBuf(cell->instIdx, (Data **)(&raReqInfo), 
                        sizeof(RgSchRaReqInfo))) == RFAILED)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHRamProcRaReq(): Allocation"
                      " of RaReq failed\n"));
         MSPD_ERR("Allocation of RaReq failed\n");
	  err->errCause = RGSCHERR_RAM_MEM_EXHAUST;
         RETVALUE(RFAILED);
      }

      /* Insert the given raReqInfo */
      /* RACHO */
      raReqInfo->timingInfo = timingInfo;
      raReqInfo->raReq = raReqInd->raReqInfoArr[idx]; 
      raReqInfo->raReqLstEnt.next = NULLP;
      raReqInfo->raReqLstEnt.prev = NULLP;
      raReqInfo->raReqLstEnt.node = (PTR)raReqInfo;
#ifdef LTE_TDD
      maxUlSubframes = 
         rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
         /* UL subframes do not occupy all the subframes in a radio frame.
          * So RA Rnti index to be calculated based on actual UL subframe index. */
         /* Get the actual subframe number */
         tid = (raRnti-1)%RGSCH_NUM_SUB_FRAMES;
         /* Get the frequency index in the subframe */
         fid = ((raRnti-1) / RGSCH_NUM_SUB_FRAMES)* RGSCH_NUM_SUB_FRAMES;
         /* Get the index of RA RNTI in the array */
         raIndex = ((timingInfo.sfn % cell->raInfo.maxRaSize) \
                     * RGSCH_MAX_RA_RNTI_PER_SUBFRM * RGSCH_NUM_SUB_FRAMES) + \
                        tid + fid;
         /* rg001.201: 
          * Fixes for RACH handling in TDD: Removed deletion of queued RaReq */
#else
         raIndex = raRnti-1;
#endif
      /* RACHO: If dedicated preamble, then give preference by appending at front */
      if (RGSCH_IS_DEDPRM(cell, raReqInd->raReqInfoArr[idx].rapId))
      {
         cmLListFirst(&cell->raInfo.raReqLst[raIndex]);
         cmLListInsCrnt(&cell->raInfo.raReqLst[raIndex], &raReqInfo->raReqLstEnt);
      }
      else
      {
         cmLListAdd2Tail(&cell->raInfo.raReqLst[raIndex], &raReqInfo->raReqLstEnt);
      }
   }

   RETVALUE(ROK);
}  /* rgSCHRamProcRaReq */

/**
 * @brief Handler for Random Access control block creation
 *
 * @details
 *
 *     Function : rgSCHRamCreateRaCb
 *                Creates a raCb and gives the same to scheduler for its updation
 *     
 *
 *  @param[in]       RgSchCellCb       *cell 
 *  @param[in, out]  RgSchRaCb         **raCb 
 *  @param[out]      RgSchErrInfo         *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamCreateRaCb
(
RgSchCellCb       *cell,
RgSchRaCb         **raCb,
RgSchErrInfo      *err
)
#else
PUBLIC S16 rgSCHRamCreateRaCb(cell, raCb, err)
RgSchCellCb       *cell;
RgSchRaCb         **raCb;
RgSchErrInfo      *err;
#endif
{
   RgSchRntiLnk *rntiLnk;
   Inst         inst = cell->instIdx;

   TRC2(rgSCHRamCreateRaCb)

   if((rgSCHUtlAllocSBuf(inst, (Data **)(raCb),
                      sizeof(RgSchRaCb))) == RFAILED)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHRamCreateRaCb(): Allocation of "
           "RaCb failed\n"));
      MSPD_ERR("Allocation of RaCb failed\n");
      err->errCause = RGSCHERR_RAM_MEM_EXHAUST;
      RETVALUE(RFAILED);
   }

   rntiLnk = rgSCHDbmGetRnti(cell);
   if(rntiLnk != NULLP)
   {
      (*raCb)->rntiLnk = rntiLnk;
      (*raCb)->tmpCrnti = rntiLnk->rnti;
   }
   else
   {
      /* No rnti available! */
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHRamCreateRaCb(): Allocation of "
         "temporary RNTI failed\n"));
#ifdef MSPD
	  MSPD_ERR("Allocation of temporary RNTI Failed at MAC(CRNTI exhausted) \n");
#endif
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(raCb), sizeof(RgSchRaCb));
      err->errCause = RGSCHERR_RAM_RNTI_EXHAUST;
      RETVALUE(RFAILED);
   }

   /* Allocate and initialize the DL HARQ portion of the RACB */
   (*raCb)->dlHqE = rgSCHDhmHqEntInit(cell);
   if ((*raCb)->dlHqE == NULLP)
   {
      /* No memory available! */
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSCHRamCreateRaCb(): Creation of"
              " DL HARQ failed\n"));
      MSPD_ERR("Creation of DL HARQ failed\n");
	  /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(raCb), sizeof(RgSchRaCb));
      err->errCause = RGSCHERR_RAM_MEM_EXHAUST;
      RETVALUE(RFAILED);
   }

   (*raCb)->dlHqE->raCb = (*raCb);
   /* Initialize RaCb's contents */
   (*raCb)->timingInfo = cell->crntTime;
   (*raCb)->raState = RGSCH_RA_MSG3_PENDING;
   (*raCb)->msg3HqProcRef = &((*raCb)->msg3HqProc);
   (*raCb)->toDel = FALSE;
   (*raCb)->phr.pres = FALSE;
   /* This updates the Y table maintained in Racb used by */
   /* scheduler  when allocating PDCCH                  */
   rgSCHUtlUpdY((*raCb)->tmpCrnti, (*raCb)->y);

#ifdef MSPD
   MSPD_DBG("Creating RACB for UE(%d)\n", (*raCb)->tmpCrnti);
#endif
   /* Insert the created raCb into raCb list of cell */
   (*raCb)->raCbLnk.node = (PTR)(*raCb);
   cmLListAdd2Tail(&cell->raInfo.raCbLst, &(*raCb)->raCbLnk);
   
   RETVALUE(ROK);
}  /* rgSCHRamCreateRaCb */

/**
 * @brief Handler for Ue Configuration Request
 *
 * @details
 *
 *     Function : rgSCHRamRgrUeCfg
 *     
 *     This function handles the UE config received based on the state of the
 *     raCb.
 *     -# If raCb is in RGSCH_RA_MSG4_PENDING state, it shall update the harq 
 *        information to UeCb and update the references.
 *     -# If raCb is in RGSCH_RA_MSG4_DONE, then it shall free the raCb
 *     
 *           
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in,out] RgSchUeCb      *ue 
 *  @param[in,out] RgSchRaCb      *raCb 
 *  @param[out]    RgSchErrInfo   *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamRgrUeCfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchRaCb      *raCb,
RgSchErrInfo   *err
)
#else
PUBLIC S16 rgSCHRamRgrUeCfg(cell, ue, raCb, err)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchRaCb      *raCb;
RgSchErrInfo   *err;
#endif
{
   /*rg004.201: ccpu00105882: Releasing HARQ processes of old UE when ue
    *           reconfig with new crnti */
  /* U32 cnt; */
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHRamRgrUeCfg)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamRgrUeCfg(): cellId = %u; "
       "ueId = %u; tmpCrnti = %u\n", cell->cellId, ue->ueId, raCb->tmpCrnti));

   /* MS_FIX : set UE inactive in DL until UE is reinitialization completed */
   ue->dl.dlInactvMask |= RG_HQENT_INACTIVE;
   ue->ul.ulInactvMask |= RG_HQENT_INACTIVE;

   if(raCb->raState ==  RGSCH_RA_MSG4_PENDING)
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "rgSCHRamRgrUeCfg(): RaCb with "
            "rnti=%u is updated with Ue information\n", raCb->tmpCrnti));
      raCb->ue = ue;
      ue->rntiLnk = raCb->rntiLnk;
      /* Update UL Harq process information */
      rgSCHUhmUpdMsg3Hq(&(raCb->msg3HqProcRef), raCb->msg3HqProcId, ue);
#ifdef RG_UNUSED
      if (raCb->msg3HqProcRef->alloc)
      {
         rgSCHUtlUpdUlHqProc(cell, raCb->msg3HqProcRef, &(raCb->msg3HqProc));
      }
#endif      
   }
   else if(raCb->raState == RGSCH_RA_MSG4_DONE)
   {
      ue->rntiLnk = raCb->rntiLnk;
      rgSCHUhmUpdMsg3Hq(&(raCb->msg3HqProcRef), raCb->msg3HqProcId, ue);
#ifdef RG_UNUSED
      if (raCb->msg3HqProcRef->alloc)
      {
         rgSCHUtlUpdUlHqProc(cell, raCb->msg3HqProcRef, &(raCb->msg3HqProc));
      }
#endif      
      /* MS_FIX : syed Assign hqEnt to UE only if msg4 is done */
      rgSCHDhmAssgnUeHqEntFrmRaCb(ue, raCb);
   }
   else
   {
      err->errCause = RGSCHERR_RAM_NO_MSG3_RCVD;
      ue->dl.hqEnt = NULLP;
      raCb->dlHqE->ue = NULLP;
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHRamRgrUeCfg */


/**
 * @brief Handler for C-RNTI based contention resolution
 *
 * @details
 *
 *     Function : rgSCHRamContResCrnti
 *     
 *     This function shall be invoked once Msg3 indicates C-RNTI based
 *     contention resolution.This shall indicate the scheduler regarding
 *     C-RNTI based uplink grant.
 *     
 *           
 *  @param[in,out] RgSchCellCb *cell 
 *  @param[in,out] RgSchUeCb   *ue 
 *  @param[in,out] RgSchRaCb   *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHRamContResCrnti
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchRaCb    *raCb,
RgSchErrInfo *err
)
#else
PRIVATE S16 rgSCHRamContResCrnti(cell, ue, raCb, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchRaCb    *raCb;
RgSchErrInfo *err;
#endif
{
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TfuUlCqiRpt   ulCqiRpt;
   RgSchCmnCell  *cellSch= (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHRamContResCrnti)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamContResCrnti(): cellId = %u; "
            "tmpCrnti = %u\n", cell->cellId, raCb->tmpCrnti));

   /* MS_FIX: syed It is incorrect to copy over msg3HqProc to ueCb's 
    * UL harq proc. In case of Crnti based RACH, ueCb has valid context which
    * cannot be over written. It was leading to a crash. */ 

   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   rgSCHUtlRecMsg3Alloc(cell, ue, raCb);
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "rgSCHRamContResCrnti():Indicate c-rnti"
          " based uplink grant to scheduler for UeId = %u\n", ue->ueId));
   /* Indicate scheduler that the harq process is updated */
#ifdef RG_UNUSED
   if (raCb->msg3HqProcRef->alloc)
   {
      rgSCHUtlUpdUlHqProc(cell, raCb->msg3HqProcRef, &(raCb->msg3HqProc));
   }
#endif
   /* MS_FIX for ccpu00123908: Reset the UL CQI to the cell default value here */
   ulCqiRpt.isTxPort0 = TRUE;
   ulCqiRpt.numSubband = 0;
   ulCqiRpt.rnti = ue->rntiLnk->rnti;
   /* rg002.301:[ccpu00124018]-MOD- Avoiding hard coding of CQI and retriving from cell config*/
   ulCqiRpt.wideCqi = cellSch->ul.dfltUlCqi;
#ifdef MSPD
   MSPD_DBG("Sending default CQI to UE upon RACH\n");
#endif
   rgSCHUtlUlCqiInd(cell, ue, &ulCqiRpt);

   
   /* Invoke scheduler to indicate UL grant req for contention resolution */
   rgSCHUtlContResUlGrant(cell, ue, err);

   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   if (raCb->phr.pres == TRUE)
   {
      rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, err);
   }
   /* MS_FIX: syed Explicitly Inidcate MAC to release RNTI */
   rgSCHRamIndRntiRls2Mac(cell, raCb);
   /* No need of raCb any more */
   rgSCHRamDelRaCb(cell, raCb, TRUE);

   RETVALUE(ROK);
}  /* rgSCHRamContResCrnti */


/**
 * @brief Handler for CCCH SDU based contention resolution
 *
 * @details
 *
 *     Function : rgSCHRamContResCcchsdu
 *     
 *     This function shall be invoked once Msg3 indicates contention resolution
 *     based on CCCH sdu. This shall update the raCb state to 
 *     RGSCH_RA_MSG4_PENDING.
 *     
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHRamContResCcchsdu
(
RgSchCellCb *cell,
RgSchRaCb *raCb
)
#else
PRIVATE S16 rgSCHRamContResCcchsdu(cell, raCb)
RgSchCellCb *cell;
RgSchRaCb *raCb;
#endif
{
#ifdef RGR_V1 
   CmLteTimingInfo expTime;
#endif
   TRC2(rgSCHRamContResCcchsdu)
   if(raCb->raState != RGSCH_RA_MSG3_PENDING)
   {
#ifdef MSPD
      MSPD_DBG("[%d]RaCb in wrong State %d Drop Msg 3\n",raCb->rntiLnk->rnti, raCb->raState);
#endif /* MSPD */
      RETVALUE(ROK);
   }

   raCb->raState = RGSCH_RA_MSG4_PENDING;

   /*rg009.201 - CR timer implementation changes*/
#ifdef RGR_V1 
   if(cell->rachCfg.contResTmr) 
   {
      if(cell->rachCfg.contResTmr - RG_SCH_GRD_TMR)
      {
         RGSCHADDTOCRNTTIME(cell->crntTime, expTime,
               (cell->rachCfg.contResTmr - RG_SCH_GRD_TMR));
      }
      else
      {
         RGSCHADDTOCRNTTIME(cell->crntTime, expTime, 1);
      }
      raCb->expiryTime = expTime;
      raCb->contResTmrLnk.node = (PTR)(raCb);
      cmLListAdd2Tail(&(cell->contResGrdTmrLst), &(raCb->contResTmrLnk));
   }
#endif
   RETVALUE(ROK);
}  /* rgSCHRamContResCcchsdu */


/**
 * @brief Handler for Msg3
 *
 * @details
 *
 *     Function : rgSCHRamProcMsg3
 *     
 *     This function processes the received Msg3 and identifies the type of 
 *     contention resolution and act accordingly. 
 *     
 *           
 *  @param[in,out]  RgSchCellCb     *cell
 *  @param[in,out]  RgSchUeCb       *ue
 *  @param[in,out] RgSchRaCb        *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamProcMsg3
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgSchRaCb       *raCb,
RgInfUeDatInd   *pdu,
RgSchErrInfo    *err
)
#else
PUBLIC S16 rgSCHRamProcMsg3(cell, ue, raCb, pdu, err)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
RgSchRaCb       *raCb;
RgInfUeDatInd   *pdu;
RgSchErrInfo    *err;
#endif
{
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHRamProcMsg3)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamProcMsg3(): cellId = %u; "
      "tmpCrnti = %u\n", cell->cellId, raCb->tmpCrnti));

   /* Update raCb with PHR if received along with Msg3 */ 
   if (pdu->ceInfo.bitMask & RGSCH_PHR_CE_PRSNT)
   {
      /* PHR present */
      raCb->phr.pres = TRUE;
      raCb->phr.val = pdu->ceInfo.ces.phr;
   } 
   if (ue)
   {
      rgSCHRamContResCrnti(cell, ue, raCb, err);
   }
   else
   {
      /*rg009.201 - CR timer changes*/
      MSPD_DBG("<CRASH> CCCH based contention\n");
      rgSCHRamContResCcchsdu(cell, raCb);
   } 

   RETVALUE(ROK);
}  /* rgSCHRamProcMsg3 */


/**
 * @brief Handler for Updating Bo received in StaRsp
 *
 * @details
 *
 *     Function : rgSCHRamUpdtBo
 *     
 *     This function shall be invoked by RAM once it receives staRsp on CCCH
 *  
 *  @param[in]     RgSchCellCb       *cell         
 *  @param[in,out] RgSchRaCb         *raCb 
 *  @param[in]     RgRguCmnStaRsp *staRsp
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamUpdtBo
(
RgSchCellCb       *cell,
RgSchRaCb         *raCb,
RgInfCmnBoRpt     *staRsp
)
#else
PUBLIC S16 rgSCHRamUpdtBo(cell, raCb, staRsp)
RgSchCellCb       *cell;
RgSchRaCb         *raCb;
RgInfCmnBoRpt     *staRsp;
#endif
{
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHRamUpdtBo)

#ifndef ALIGN_64BIT
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamUpdtBo(): tmpCrnti = %u;"
                             " bo = %ld\n", raCb->tmpCrnti, (U32)staRsp->bo));
#else
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamUpdtBo(): tmpCrnti = %u; "
                              "bo = %d\n", raCb->tmpCrnti, (U32)staRsp->bo));
#endif

   /* Update Bo in RaCb */
   raCb->dlCcchInfo.bo = (U32)(staRsp->bo);

   /* add this to the "tobeSchdLst" */
   /* MSG4 MS_FIX  Start */   
   rgSCHRamAddToRaInfoSchdLst(cell, raCb);
   /* MSG4 MS_FIX  End */      
   
   RETVALUE(ROK);
} /* rgSCHRamUpdtBo */

/**
 * @brief Handler for Msg3 Feedback indication
 *
 * @details
 *
 *     Function : rgSCHRamMsg3DatInd
 *     
 *     This function shall be invoked by TOM once the transmission of Msg4 is
 *     ACKed/NACKed.
 *     This shall invoke UHM to set ACK for Msg3 reception.
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamMsg3DatInd
(
RgSchRaCb      *raCb
)
#else
PUBLIC S16 rgSCHRamMsg3DatInd(raCb)
RgSchRaCb      *raCb;
#endif
{
   TRC2(rgSCHRamMsg3DatInd)

   rgSCHUhmProcMsg3DatInd(raCb->msg3HqProcRef);

   RETVALUE(ROK);
}  /* rgSCHRamMsg3DatInd */

/**
 * @brief Handler for Msg3 Feedback indication
 *
 * @details
 *
 *     Function : rgSCHRamMsg3FailureInd
 *     
 *     This function shall be invoked by TOM once the transmission of Msg4 is
 *     ACKed/NACKed.
 *     This shall invoke UHM to set ACK for Msg3 reception.
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamMsg3FailureInd
(
RgSchRaCb      *raCb
)
#else
PUBLIC S16 rgSCHRamMsg3FailureInd(raCb)
RgSchRaCb      *raCb;
#endif
{
   TRC2(rgSCHRamMsg3FailureInd)

   rgSCHUhmProcMsg3Failure(raCb->msg3HqProcRef);

   RETVALUE(ROK);
}  /* rgSCHRamMsg3FailureInd */

/**
 * @brief Handler for Msg4 Feedback indication
 *
 * @details
 *
 *     Function : rgSCHRamMsg4FdbkInd
 *     
 *     This function shall be invoked by TOM once the transmission of Msg4 is
 *     ACKed/NACKed.
 *     This shall invoke UHM to set ACK for Msg3 reception.
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamMsg4FdbkInd
(
RgSchRaCb      *raCb
)
#else
PUBLIC S16 rgSCHRamMsg4FdbkInd(raCb)
RgSchRaCb      *raCb;
#endif
{
   TRC2(rgSCHRamMsg4FdbkInd)

   RETVALUE(ROK);
}  /* rgSCHRamMsg4FdbkInd */

/**
 * @brief Deletes raCb and releases rnti 
 *
 * @details
 *
 *     Function : rgSCHRamIndRntiRls2Mac 
 *       This function is the RNTI(transient UECB) is to be removed
 *       from MAC. It will post a release RNTI indication to MAC.
 *     
 *    
 *           
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in,out] RgSchRaCb      *raCb 
 *  @return  Void
 *      -# ROK 
 **/
#ifdef ANSI
PRIVATE Void rgSCHRamIndRntiRls2Mac 
(
RgSchCellCb    *cell,
RgSchRaCb      *raCb
)
#else
PRIVATE Void rgSCHRamIndRntiRls2Mac(cell, raCb)
RgSchCellCb    *cell;
RgSchRaCb      *raCb;
#endif
{
   Pst          pst;
   Inst         inst = cell->instIdx;
   RgInfRlsRnti rntiInfo;

   TRC2(rgSCHRamIndRntiRls2Mac)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamIndRntiRls2Mac(): cellId = %u; "
               "tmpCrnti = %u\n", cell->cellId, raCb->tmpCrnti));
#ifdef MSPD
   MSPD_DBG("RNTI Release for UE(%d)\n", raCb->tmpCrnti);
#endif
   /* Copy the info to rntiInfo */
   rntiInfo.cellId = cell->cellId;
   rntiInfo.rnti   = raCb->tmpCrnti;
   /* Invoke MAC to release the rnti */
   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[inst], cell->macInst);
   RgSchMacRlsRnti(&pst, &rntiInfo);
   RETVOID;
}

/**
 * @brief Handler for Msg4 state updation
 *
 * @details
 *
 *     Function : rgSCHRamMsg4Done
 *     
 *     This function shall be invoked by DHM once the transmission of Msg4 is
 *     done. This shall delete the raCb if there is a valid Ue or if this is to
 *     be deleted. If not this shall update the state of the raCb.
 *     
 *           
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in,out] RgSchRaCb      *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamMsg4Done
(
RgSchCellCb    *cell,
RgSchRaCb      *raCb
)
#else
PUBLIC S16 rgSCHRamMsg4Done(cell, raCb)
RgSchCellCb    *cell;
RgSchRaCb      *raCb;
#endif
{
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHRamMsg4Done)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamMsg4Done(): cellId = %u; "
        "tmpCrnti = %u\n", cell->cellId, raCb->tmpCrnti));

   if(raCb->ue != NULLP) 
   {
      /* MS_FIX : syed Let this funtion decide on releasing
       * hqP than the caller of this function otherwise sometimes it 
       * might lead to incorrec NDI setting. */	   
      rgSCHDhmRlsHqpTb(raCb->dlHqE->msg4Proc, 0, TRUE);
      /* MS_FIX : syed Assign hqEnt to UE only if msg4 is done */
      rgSCHDhmAssgnUeHqEntFrmRaCb(raCb->ue, raCb);
      /* Rnti shall not be released as Ue exists with this rnti */
      rgSCHRamDelRaCb(cell, raCb, FALSE);
   }
   else if(raCb->toDel == TRUE)
   {
      /* MS_FIX: syed Explicitly Inidcate MAC to release RNTI */
      rgSCHRamIndRntiRls2Mac(cell, raCb);
      /* Delete RACB and release RNTI */
      rgSCHRamDelRaCb(cell, raCb, TRUE);
   }
   else
   {
      raCb->raState = RGSCH_RA_MSG4_DONE;
      /* Release harq process as final feedback is received for Msg4. In other
       * cases, delRaCb will take care of releasing the harq process */
      /* rg001.301 -MOD- [ccpu00118350] : Correcting NDI manipulation of Harq */
      rgSCHDhmRlsHqpTb(raCb->dlHqE->msg4Proc, 0, TRUE);
   }

   RETVALUE(ROK);
}  /* rgSCHRamMsg4Done */


/**
 * @brief Handler for deletion
 *
 * @details
 *
 *     Function : rgSCHRamDelRaCb
 *     
 *     This function shall be invoked whenever a raCb needs to be deleted. 
 *    Invoked by both RAM and downlink scheduler 
 *           
 *  @param[in]     RgSchCellCb *cell 
 *  @param[in,out] RgSchRaCb   *raCb
 *  @param[in]     Bool        rlsRnti 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamDelRaCb
(
RgSchCellCb *cell,
RgSchRaCb   *raCb,
Bool        rlsRnti
)
#else
PUBLIC S16 rgSCHRamDelRaCb(cell, raCb, rlsRnti)
RgSchCellCb *cell;
RgSchRaCb   *raCb;
Bool        rlsRnti;
#endif
{
   Inst         inst = cell->instIdx;
   TRC2(rgSCHRamDelRaCb)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamDelRaCb(): cellId = %u; "
      "tmpCrnti = %u\n", cell->cellId, raCb->tmpCrnti));
#ifdef MSPD
   MSPD_DBG("Deleting RACB for UE(%d)\n", raCb->tmpCrnti);
#endif
   /* Delete from all the lists it is enqueued */
   cmLListDelFrm(&(cell->raInfo.raCbLst),&(raCb->raCbLnk));
   if (raCb->schdLnk.node == (PTR)raCb)
   {
      rgSCHRamRmvFrmRaInfoSchdLst(cell, raCb);
   }
#ifdef RGR_V1
   else if(raCb->contResTmrLnk.node != NULLP)
   {
      cmLListDelFrm(&cell->contResGrdTmrLst, &(raCb->contResTmrLnk));
      raCb->contResTmrLnk.node = NULLP;
   }
#endif

   if(rlsRnti == TRUE)
   {
      rgSCHDbmRlsRnti(cell, raCb->rntiLnk);
   }

   /* Check if msg4 Hq Proc has been released. If not, release it */
   if (raCb->dlHqE )
   {
      if (raCb->dlHqE->msg4Proc != NULLP)
      {
         /* MS_FIX: syed Remove the msg4Proc if it waiting in sf->tbs list for
	  * harq feedback */	      
         if ((raCb->dlHqE->msg4Proc->tbInfo[0].subFrm != NULLP) &&
             (raCb->dlHqE->msg4Proc->tbInfo[0].sfLnk.node != NULLP))
         {
#ifdef MSPD
            MSPD_ERR("RACH FAILURE!! msg4proc for UE(%d) removed from SF\n", 
		    raCb->tmpCrnti);
#endif
            cmLListDelFrm(&raCb->dlHqE->msg4Proc->tbInfo[0].subFrm->pdcchInfo.pdcchs,
                     &raCb->dlHqE->msg4Proc->tbInfo[0].pdcch->lnk);
            cmLListAdd2Tail(&cell->pdcchLst, &raCb->dlHqE->msg4Proc->tbInfo[0].pdcch->lnk);
            rgSCHUtlDlHqPTbRmvFrmTx(raCb->dlHqE->msg4Proc->tbInfo[0].subFrm, 
			            raCb->dlHqE->msg4Proc, 0, FALSE);
         }	      
         /* MS_FIX: syed Remove the msg4Proc from cell
	  * msg4Retx Queue. I have used CMN scheduler function
	  * directly. Please define a new API and call this
	  * function through that. */	      
         rgSCHCmnDlMsg4ProcRmvFrmRetx(cell, raCb->dlHqE->msg4Proc);	      
         /* rg001.301 -MOD- [ccpu00118350] : Correcting NDI manipulation of Harq */
         rgSCHDhmRlsHqpTb(raCb->dlHqE->msg4Proc, 0, TRUE);
      }

      /* Mark the raCb pointer in dlHqE to NULLP */
      raCb->dlHqE->raCb = NULLP;

      rgSCHDhmDelHqEnt(cell, &raCb->dlHqE);
   }
   /* MS_FIX: syed Adaptive Msg3 Retx crash. Remove the harqProc
    * from adaptive retx List. Free the alloc if it exists. */
   if (raCb->msg3HqProc.reTxLnk.node)
   {
#ifdef MSPD
      MSPD_DBG("Removing Msg3HqP from Adap Retx List\n");	      
#endif
      cmLListDelFrm(raCb->msg3HqProc.reTxAlloc.reTxLst, &raCb->msg3HqProc.reTxLnk); 
      raCb->msg3HqProc.reTxLnk.node = (PTR)NULLP;
   }
   if (raCb->msg3HqProcRef->alloc)
   {
      MSPD_ERR("<CRASH> MSG3 Alloc should have been released\n");	   
   }
   if (raCb->msg3HqProc.alloc)
   {
      /* MS_FIX: syed During GPR, please write an API instead of direct
       * call to cmn scheduler function */	   
      RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#ifdef MSPD
      MSPD_DBG("Freeing MSG3 UL Alloc\n");	      
#endif
      rgSCHCmnUlFreeAllocation(&cellUl->ulSfArr[raCb->msg3HqProc.ulSfIdx],
                                                raCb->msg3HqProc.alloc);
   }

   rgSCHUtlFreeSBuf(inst, (Data **)&raCb, sizeof(RgSchRaCb));

   RETVALUE(ROK);
}  /* rgSCHRamDelRaCb */


/**
 * @brief  TTI Handler for RAM module
 *
 * @details
 *
 *     Function : rgSCHRamTtiHndlr
 *     
 *     This function shall be invoked upon TtiInd by TOM
 *     This shall
 *         - remove RaReqs added to the queue for a raRnti for which PHY may
 *           give the requests in the next subframe
 *         - remove raCbs which are not yet processed once the 
 *           counter for raCb processing expires.
 *     
 *           
 *  @param[in,out] RgSchCellCb  *cell
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamTtiHndlr
(
RgSchCellCb  *cell
)
#else
PUBLIC S16 rgSCHRamTtiHndlr(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchRaCb   *raCb;
   U16         raSfn;
   U16         crntSfn;
   U16         dist;       /* Number of frames between raCb's creation and crnt
                              frame */
   U8          idx;
   /*rg003.201: maxCnt changed from U8 to U32 */
   U32         maxCnt;
   /* rg001.101: Corrected computation for RaRnti to be deleted */
#ifndef LTE_TDD
   S8          diff;        
   CmLteRnti   raRnti;
   RgSchRaReqInfo *raReqInfo;
#else
   /* rg001.201: Fixes for RACH handling: Added deletion of queued RaReq */
   CmLteTimingInfo      frm;
   U8                   raIdx;
#endif

   TRC2(rgSCHRamTtiHndlr)

   RGSCHDBGPRM(cell->instIdx, 
         (rgSchPBuf(cell->instIdx), "rgSCHRamTtiHndlr(): cellId = %u\n", 
         cell->cellId));
   crntSfn = cell->crntTime.sfn;
  
#ifdef RGR_V1
   /*rg009.201 - CR timer implementation changes*/
   /*Check if Contention resolution guard timer expiring in the TTI*/
   rgSCHChkContResGrdTmrExp(cell);
   /*Check if Contention resolution timer expiring in the TTI*/
   rgSCHChkContResTmrExp(cell);
#endif
#ifndef LTE_TDD
   /* Delete requests against the raRnti for which PHY may send the 
    * requests in the next subframe */
   /* rg001.101: Corrected computation for RaRnti to be deleted */
   /* rg002.201: The rarnti to be deleted is equal to the diff between the next 
    * subframe and the sum of preamble length, three subframe wait period 
    * for RARSP window start and the response window size. Preamble length is
    * considered as is because rarnti is directly used for indexing the array*/
   diff = ((cell->crntTime.subframe + 1) - 
            (RGSCH_RARSP_WAIT_PERIOD +
             rgRaPrmblToRaFrmTbl[cell->rachCfg.preambleFormat] + 
             cell->rachCfg.raWinSize));

   if (diff < 0)
   {
      diff += 10;
      if(diff < 0)
      {
         diff += 10;
      }
   }
   raRnti = diff;

   /* Flush the already existing raReqs against the given raRnti */
   RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), "rgSCHRamTtiHndlr(): Flushing the "
         "previously enqueued raReqs for raRnti = %u\n", raRnti));

   maxCnt = cell->raInfo.raReqLst[raRnti].count;
   for (idx = 0; idx < maxCnt; idx++)
   {
      raReqInfo = (RgSchRaReqInfo *)(cell->raInfo.raReqLst[raRnti].first->node);
      cmLListDelFrm(&(cell->raInfo.raReqLst[raRnti]),&(raReqInfo->raReqLstEnt));
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&raReqInfo,
            sizeof(RgSchRaReqInfo));
   }
#else
  /* rg001.201:
   * Fixes for RACH handling: Added deletion of queued RaReq */
   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
   if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][frm.subframe] !=
                     RG_SCH_TDD_UL_SUBFRAME)
   {
      raIdx = rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][frm.subframe]-1;
      rgSCHRamDelRaReq(cell, cell->crntTime, raIdx);
   }
#endif
   
   /*  remove raCbs which are not yet processed within RG_MAX_RA_PRC_FRM */
   if ((crntSfn % RG_MAX_RA_PRC_FRM) == 0)
   {
      maxCnt = cell->raInfo.raCbLst.count;
      for (idx = 0; idx < maxCnt; idx++)
      {
         raCb = (RgSchRaCb *)(cell->raInfo.raCbLst.first->node);
         /* Calculate number of frames between raCb's creation and crnt frame */
         raSfn = raCb->timingInfo.sfn;
         if (raSfn > crntSfn)
         {
            dist = crntSfn + (RGSCH_MAX_SFN - raSfn);
         }
         else
         {
            dist  = crntSfn - raSfn;
         }
         /* Delete RaCbs whose processing is not complete within 
          * "RG_MAX_RA_PRC_FRM" frames */
          /* raCb not to be deleted if msg4 is not completed */
          /* raCb should not be deleted(RNTI should not be released) if UE is present
	   * as it means the application still holds the RNTI. raCb will get deleted
	   * as part of UE deletion. raCb will anyway get deleted without releasing RNTI on success/failure of MSG4*/
	 
         if (dist >= RG_MAX_RA_PRC_FRM) 
	 {
            if ((raCb->dlHqE->msg4Proc == NULLP) && (raCb->dlHqE->ue == NULLP))
         {
            RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), "rgSCHRamTtiHndlr(): Deleting"
               " raCb with rnti = %u\n", raCb->tmpCrnti));
#ifdef PWAV
	   MSPD_DBG("Releasing RNTI:%d @ (%d, %d)\n", raCb->tmpCrnti, cell->crntTime.sfn, cell->crntTime.subframe);
#endif
            /* MS_FIX: syed Explicitly Inidcate MAC to release RNTI */
            rgSCHRamIndRntiRls2Mac(cell, raCb);
            /* Delete RACB and release RNTI */
            rgSCHRamDelRaCb(cell, raCb, TRUE);
         }
	 }
         else
         {
            break;
         }
      }
   }
   
   RETVALUE(ROK); 
}  /* rgSCHRamTtiHndlr */


/**
 * @brief Function for handling cell delete
 *
 * @details
 *
 *     Function : rgSCHRamFreeCell
 *     
 *     This function shall be invoked whenever a cell needs to be deleted.
 *     This shall remove raCbs and raReqs stored in cell.
 *     
 *           
 *  @param[in,out] RgSchCellCb  *cell
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamFreeCell
(
RgSchCellCb    *cell
)
#else
PUBLIC S16 rgSCHRamFreeCell(cell)
RgSchCellCb    *cell;
#endif
{
   RgSchRaReqInfo  *raReqInfo;
   RgSchRaCb       *raCb;
   U8              idx;
   U8              idx1;
   U8              raCbCnt;
   Inst            inst = cell->instIdx;
   U8              lstSz;
#ifdef LTE_TDD
   U8              maxUlSubframes;
   U8              maxDlSubframes;
#endif


   TRC2(rgSCHRamFreeCell)

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHRamFreeCell(): cellId = %u\n", 
         cell->cellId));

#ifdef LTE_TDD
   maxUlSubframes =
      rgSchTddNumUlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   maxDlSubframes =
      rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   lstSz = cell->raInfo.maxRaSize * RGSCH_MAX_RA_RNTI_PER_SUBFRM * \
                                                      maxUlSubframes;
#else
   lstSz = RGSCH_MAX_RA_RNTI;
#endif

   for (idx = 0; idx < lstSz; idx++)
   {
      /* Delete and free raReqs stored */
      for (idx1 = 0; idx1 < cell->raInfo.raReqLst[idx].count; idx1++)
      {
         raReqInfo = (RgSchRaReqInfo *)(cell->raInfo.raReqLst[idx].first->node);
         cmLListDelFrm(&(cell->raInfo.raReqLst[idx]),&(raReqInfo->raReqLstEnt));
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&raReqInfo, sizeof(RgSchRaReqInfo));
      }
   }

#ifdef LTE_TDD
   /* Delete the RACH response list*/
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, 
         (Data **)(&(cell->rachRspLst)), sizeof(RgSchTddRachRspLst) * \
                                    maxDlSubframes);
#endif

   /* Delete raCbs in the "to be scheduled" list */
   for (idx1 = 0; idx1 < cell->raInfo.toBeSchdLst.count; idx1++)
   {
      raCb = (RgSchRaCb *)(cell->raInfo.toBeSchdLst.first->node);
      /* MSG4 MS_FIX Start */
	  	  
      rgSCHRamRmvFrmRaInfoSchdLst(cell, raCb);
      /* MSG4 MS_FIX End */	  
   }

   raCbCnt =  cell->raInfo.raCbLst.count;

   /* Delete and free raCbs stored */ 
   for (idx1 = 0; idx1 < raCbCnt; idx1++)
   {
      raCb = (RgSchRaCb *)(cell->raInfo.raCbLst.first->node);
      /* MS_FIX: syed Explicitly Inidcate MAC to release RNTI */
      rgSCHRamIndRntiRls2Mac(cell, raCb);
      rgSCHRamDelRaCb(cell, raCb, TRUE);
   }

   RETVALUE(ROK); 
} /* rgSCHRamFreeCell */
/*rg009.201 - CR timer implementation changes*/
#ifdef RGR_V1
#ifdef ANSI
PRIVATE Void rgSCHRamProcContResExp
(
RgSchCellCb *cell,
RgSchRaCb  *raCb
)
#else
PRIVATE Void rgSCHRamProcContResExp (cell, raCb)
RgSchCellCb *cell;
RgSchRaCb  *raCb;
#endif
{
   TRC2(rgSCHRamProcContResExp);
   raCb->expiryTime.sfn = RGSCH_CONTRES_EXP;
   /*MSG4 MS_FIX*/
   if (raCb->ue)
   {
      /* UE exists and RNTI will be released as part of UE DEL */	   
      rgSCHRamDelRaCb(cell, raCb, FALSE);
   }
   else
   {
      /* Calling Release RNTI, which would perform Racb deletion
       * RNTI removal and RNTI release indication to MAC. */	   
      /* MS_FIX: syed Explicitly Inidcate MAC to release RNTI */
      rgSCHRamIndRntiRls2Mac(cell, raCb);
      /* Delete RACB and release RNTI */
      rgSCHRamDelRaCb(cell, raCb, TRUE);
   }
   RETVOID;
}

#ifdef ANSI
PRIVATE Void rgSCHRamProcContResGrdExp
(
RgSchCellCb *cell,
RgSchRaCb  *raCb
)
#else
PRIVATE Void rgSCHRamProcContResGrdExp (cell, raCb)
RgSchCellCb  *cell;
RgSchRaCb  *raCb;
#endif
{
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHRamProcContResGrdExp)

      RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHDhmProcContResGrdExp(): tmpCrnti = %u \n"
          , raCb->tmpCrnti));

/*Guard timer has expired, schedule only the contention REsolution CE with 
 * zero bo*/
   raCb->dlCcchInfo.bo = 0; 

   /* add this to the "tobeSchdLst" */
   raCb->schdLnk.node = (PTR)(raCb);
   
   cmLListDelFrm(&cell->contResGrdTmrLst, &(raCb->contResTmrLnk));
   raCb->contResTmrLnk.node = NULLP;

   /* MSG4 MS_FIX Start */
#ifdef MSPD
   MSPD_DBG("Con Res Grd Tmr exp [%d]\n", raCb->rntiLnk->rnti);   
#endif
   rgSCHRamAddToRaInfoSchdLst(cell, raCb);
   /* MSG4 MS_FIX End */    
   RETVOID;
   
}
/**
 * @brief Check the Contention Resolution Guard Timer Expiry. 
 *
 * @details
 *
 *     Function: rgSCHChkContResTmrExp 
 *
 *
 *     Invoked by: Scheduler 
 *  @param[in] RgSchCellCb        *cell 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void rgSCHChkContResTmrExp
(
RgSchCellCb        *cell
)
#else
PRIVATE Void rgSCHChkContResTmrExp(cell)
RgSchCellCb        *cell;
#endif
{
   CmLList         *chkLnk    = NULLP;
   RgSchRaCb       *raCb = NULLP;
   
   TRC2(rgSCHChkContResTmrExp)
      
   chkLnk = cmLListFirst(&(cell->contResTmrLst));
   
   for (; chkLnk; chkLnk = chkLnk->next)
   {
      raCb = (RgSchRaCb *)(chkLnk->node);
      
      if(RGSCH_TIMEINFO_SAME(raCb->expiryTime, cell->crntTime))
      {
         /*If timer expired, call the handler function*/
         rgSCHRamProcContResExp(cell, raCb);
      }
	  /*MS_FIX: Need to traverse till end of list as the entries may not be in ascending order*/
   /*   else
      {
    break;
      }*/
   }
}
/**
 * @brief Check the Contention Resolution Guard Timer Expiry. 
 *
 * @details
 *
 *     Function: rgSCHChkContResGrdTmrExp 
 *
 *
 *     Invoked by: Scheduler 
 *  @param[in] RgSchCellCb        *cell 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void rgSCHChkContResGrdTmrExp
(
RgSchCellCb        *cell
)
#else
PRIVATE Void rgSCHChkContResGrdTmrExp(cell)
RgSchCellCb        *cell;
#endif
{
   CmLList         *chkLnk    = NULLP;
   RgSchRaCb       *raCb = NULLP;
   
   TRC2(rgSCHChkContResGrdTmrExp)
      
   chkLnk = cmLListFirst(&(cell->contResGrdTmrLst));
   
   for (; chkLnk; chkLnk = chkLnk->next)
   {
      raCb = (RgSchRaCb *)(chkLnk->node);
      
      if(RGSCH_TIMEINFO_SAME(raCb->expiryTime, cell->crntTime))
      {
    /*If timer expired, call the handler function*/
    rgSCHRamProcContResGrdExp(cell, raCb);
      }
      else
      {
    break;
      }
   }
}
#endif
#ifdef LTE_TDD
/**
 * @brief Function for handling RACH Request deletion
 *
 * @details
 *
 *     Function : rgSCHRamDelRaReq
 *
 *     This function shall be invoked to delete the RACH Requests
 *     that is not scheduled within the RA window size.
 *
 *
 *  @param[in,out] RgSchCellCb      *cell
 *  @param[in]     CmLteTimingInfo  timingInfo
 *  @param[in]     U8               raIdx
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRamDelRaReq
(
RgSchCellCb          *cell,
CmLteTimingInfo      timingInfo,
U8                   raIdx
)
#else
PUBLIC S16 rgSCHRamDelRaReq(cell, timingInfo, raIdx)
RgSchCellCb          *cell;
CmLteTimingInfo      timingInfo;
U8                   raIdx;
#endif
{
   U8                   subfrmIdx;
   RgSchTddRachRspLst   *rachRsp;
   U16                   sfnIdx;
   S16                  calcSfn;
   U8                   subfrm;
   RgSchRaReqInfo       *raReqInfo;
   U8                   idx;
   U8                   i;
   U8                   raRntiIdx;
   CmLteRnti            raRnti;

   TRC2(rgSCHRamDelRaReq)

   RGSCHDBGPRM(cell->instIdx,\
         (rgSchPBuf(cell->instIdx), "rgSCHRamDelRaReq(): cellId = %u\n", cell->cellId));

   rachRsp = &cell->rachRspLst[raIdx];
   /* Get the SFN Index to be deleted */
   calcSfn = timingInfo.sfn - rachRsp->delInfo.sfnOffset;
   if(calcSfn < 0)
   {
      sfnIdx = (calcSfn + RGSCH_MAX_SFN) % cell->raInfo.maxRaSize;
   }
   else
   {
      sfnIdx = calcSfn;
   }

   /* Iterate through all the subframes to be delted in the SFN */
   for(subfrmIdx=0; subfrmIdx < rachRsp->delInfo.numSubfrms; subfrmIdx++)
   {
      subfrm = rachRsp->delInfo.subframe[subfrmIdx];
      /* Get the subframe Index to be deleted */
      /* rg001.201: Fixes for RACH handling in TDD: 
       * Corrected the computation of raRntiIdx
       * */
      raRntiIdx = ((sfnIdx % cell->raInfo.maxRaSize) * \
                     RGSCH_MAX_RA_RNTI_PER_SUBFRM * \
                     RGSCH_NUM_SUB_FRAMES) + subfrm;
      RGSCHDBGPRM(cell->instIdx, 
                  (rgSchPBuf(cell->instIdx), "rgSCHRamDelRaReq(): \
                     sfnIdx:subfrmIdx:raRntiIdx= %u:%u:%u\n",
                     sfnIdx, subfrmIdx, raRntiIdx));

      /* Iterate through all the RNTIs in the subframe */
      for(i=0; i < RGSCH_MAX_RA_RNTI_PER_SUBFRM; i++)
      {
         raRnti = raRntiIdx + (i*RGSCH_NUM_SUB_FRAMES);
         for (idx = 0; idx < cell->raInfo.raReqLst[raRnti].count; idx++)
         {
            raReqInfo = 
               (RgSchRaReqInfo *)(cell->raInfo.raReqLst[raRnti].first->node);
            cmLListDelFrm(&(cell->raInfo.raReqLst[raRnti]),
                                    &(raReqInfo->raReqLstEnt));
            /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
            rgSCHUtlFreeSBuf(cell->instIdx,
                              (Data **)&raReqInfo, sizeof(RgSchRaReqInfo));
         }
      }
   }

   RETVALUE(ROK);
}
#endif

/*MSG4 MS_FIX Start */
#ifdef ANSI
PUBLIC S16 rgSCHRamAddToRaInfoSchdLst
(
RgSchCellCb       *cell,
RgSchRaCb         *raCb
)
#else
PUBLIC S16 rgSCHRamAddToRaInfoSchdLst(cell, raCb)
RgSchCellCb       *cell;
RgSchRaCb         *raCb;
#endif
{
   CmLteTimingInfo expTime;

   TRC2(rgSCHRamAddToRaInfoSchdLst)

#ifdef MSPD
   MSPD_DBG("rgSCHRamAddToRaInfoSchdLst [%d] \n",raCb->rntiLnk->rnti);   	
#endif
   /*MS_FIX: This can be called even when guard timer is not expired. 
       * In this case CR timer expiry should be guard timer expiry time + Guard timer time*/
   RGSCHADDTOCRNTTIME(raCb->expiryTime, expTime, (RG_SCH_GRD_TMR));
   raCb->expiryTime = expTime;
   raCb->schdLnk.node = (PTR)(raCb);
   cmLListAdd2Tail(&(cell->raInfo.toBeSchdLst), &(raCb->schdLnk));
   raCb->contResTmrLnk.node = (PTR)(raCb);
   cmLListAdd2Tail(&(cell->contResTmrLst), &(raCb->contResTmrLnk));
   RETVALUE(ROK);
} /* rgSCHRamAddToRaInfoSchdLst */



#ifdef ANSI
PUBLIC S16 rgSCHRamRmvFrmRaInfoSchdLst
(
RgSchCellCb       *cell,
RgSchRaCb         *raCb
)
#else
PUBLIC S16 rgSCHRamRmvFrmRaInfoSchdLst(cell, raCb)
RgSchCellCb       *cell;
RgSchRaCb         *raCb;
#endif
{
   TRC2(rgSCHRamRmvFrmRaInfoSchdLst)

#ifdef MSPD
   MSPD_DBG("rgSCHRamRmvFrmRaInfoSchdLst [%d] \n",raCb->rntiLnk->rnti);
#endif
   cmLListDelFrm(&(cell->raInfo.toBeSchdLst), &(raCb->schdLnk));
   raCb->schdLnk.node = NULLP;   
   cmLListDelFrm(&(cell->contResTmrLst), &(raCb->contResTmrLnk));
   raCb->contResTmrLnk.node = NULLP;
   RETVALUE(ROK);
} /* rgSCHRamRmvFrmRaInfoSchdLst */

/*MSG4 MS_FIX End*/
/**********************************************************************
 
         End of file:     gk_sch_ram.c@@/main/2 - Sat Jul 30 02:21:48 2011
 
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
/main/1      ---       ps   1. Initial LTE MAC 2.1 release
             rg001.201 sd   1. Fixes for RACH handling in TDD for RaReq deletion
             rg002.201 sm   1. The rarnti to be deleted is equal to the diff 
                               between the next subframe and the sum of preamble
                               length, three subframe wait period for RARSP 
                               window start and the response window size. 
                               Preamble length is considered as is because 
                               rarnti is directly used for indexing the array.
             rg003.201 sm   1. declaration was corrected in rgSchRamTtiHdlr()
             rg004.201 sd   1. ccpu00105914: PHR handling for MSG3
                       ns   2. ccpu00105882: Releasing HARQ processes of old
                               UE when ue reconfig with new crnti.
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependency on MIMO compile-time flag
           rg009.201 rsharon  1. [ccpu00112372]:Changes for CR timer.
                              2. [ccpu00115186]: Added new function for Release
                              Rnti. Fixed Memory leak for clearing UE context
                              in MAC as part of RA cleanup.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
           rg001.301  nudupi 1. ccpu00118350:Correcting NDI manipulation of Harq.
*********************************************************************91*/
