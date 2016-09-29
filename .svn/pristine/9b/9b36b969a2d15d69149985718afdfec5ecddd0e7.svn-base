/**
 * @file    ueh_crm.c
 * @brief This file contains the functions for handling 
 *        cell level rrm functions such as call admission control
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 25 Oct 2010 :
*     a) Created initial version
 */

 /* This function checks all conditions that a CPE can be added.
  * It should be called during cell setup procedure.
  * Returns: -1 in case of error
  */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "lueh.x" 
#include "ueh_eut_x.h" 
#include "ueh_crm.h"
#include "ueh_pucch_res_mgmt.h"
#include "ueh_srs_res_mgmt.h"
#include "ueh.h"
#include "ueh_crm_sm.h"
#include "ueh_cfg.h"
#include "pst_events.h"

#include "log.h"
#include "ueh_s1ap_cm.h"

#include "sfi.h"

UehCb uehCb;
/* Tunnel Variables */

S32 teidPool[UEH_MAX_TUNNEL_ID+1];
PUBLIC S16 uehInitSysInfSch(void);

/* This function returns cellCb for perticular cellId */
UehCellCb *uehGetCellCbByCellId(UINT8 cellId)
{
  return uehGetCellCb();
#if 0
  /* TODO Remove the fixed index */
  if(NULLP == uehCb.cellCb)
  {
    return (NULLP);
  }
  return (uehCb.cellCb[cellId - 1]); 
#endif
}

/** This function checks if UE can be admitted to the cell.
 *  If yes: 
 *     1. it allocates a Ue Context (UeCb) and assigns PUCCH resources to it.
 *     2. assigns PUCCH resources
 *     3. increments the number of UEs in the cell
 *     4. returns the allocated UeCb (ptr)
 *  On failure, it returns NULLP.
 */ 

UehUeCb *AdmissionRequest(
  U32     establishCause,
  U16     crnti
)
{
  UehUeCb   *ueCb     = NULLP; /** Allocated and returned by this function */
  UehCellCb *cellCb   = uehGetCellCb();
  S16        ueIdx    = -1;

  UECM_FUNC(crnti, AdmissionRequest, UECM, CRM, "Admission Request");

  if (! (UEH_CELL_STATE_AVAILABLE == getCellAvState(cellCb) && cellCb->isUnbarred) )
  {
    UEH_PROCEDURE_FAILED("Admit UE", UE, crnti, "Cell not available");
    RETVALUE(NULLP);
  }
  if((cellCb->numUeCfg >= cellCb->maxUeSupp) && (EMERGENY_CALL != establishCause) )
  {
    S8 errDetail[100];
    snprintf(errDetail, sizeof(errDetail), 
                "numUeCfg limit reached|minVal=0|maxVal=%d|curVal=%d",
                 cellCb->maxUeSupp, cellCb->numUeCfg);
    UEH_PROCEDURE_FAILED( "Admit UE", UE, crnti, errDetail );
    RETVALUE(NULLP);
  }
    // We do not allow any calls, including emergency calls, when numUeCfg is >= maxRntiPool
  if(cellCb->numUeCfg >= cellCb->maxRntiPool )
  {
    S8 errDetail[100];
    snprintf(errDetail, sizeof(errDetail), 
           "Emergency call|numUeCfg limit reached|minVal=0|maxVal=%ld|curVal=%d",cellCb->maxRntiPool,cellCb->numUeCfg);
    UEH_PROCEDURE_FAILED( "Admit UE", UE, crnti, errDetail );
    RETVALUE(NULLP);
  }

  if (ROK != uehCheckMmeOverload( establishCause, crnti ))
  {
    RETVALUE(NULLP);
  }

  ueIdx  = UEH_GET_UE_IDX(crnti);
  if(!isUeIndexValid(ueIdx, cellCb->maxRntiPool)) RETVALUE(NULLP);

  UEH_GET_UECB_FROM_UEIDX(ueCb, ueIdx);
  /* Need to clear bytes before uehCrmAddUe as it fills it */
  clearBytes(ueCb, sizeof(UehUeCb));
  if(ROK != uehCrmAddUe(crnti, &ueCb->acParams)) RETVALUE(NULLP);

  /* UE can be admitted. Initialize it */
  ueCb->crnti                    = crnti;
  ueCb->isUeCbAllocated          = TRUE;
  ueCb->enbUeInternalId          = allocInternalUeId();
  ueCb->ueState                  = UE_STATE_RRC_IDLE;
  ueCb->establishCause           = establishCause;
  /* TODO: Do parameters below need to be initialised ?*/
  ueCb->ueRadCap.isUeRadAccCap   = FALSE;
  ueCb->ueReleaseCauseVal        = NhuRlsCauseotherEnum;
  ueCb->activeIntProtAlgo        = ACTIVEINTPROTALGO_DISABLE;
  uehCrmAllocateSrs( ueCb );

  cellCb->numUeCfg++;

  logProc("iUeId=%d|AdmitUe|OK|crnti=%d|entryCause=%d|numUeCfg=%d", ueCb->enbUeInternalId, crnti, establishCause, cellCb->numUeCfg);
  RETVALUE(ueCb);
}

PUBLIC void CancelUe (
    UehUeCb* ueCb
    )
{

#if 0
  UehCellCb *cellCb = uehGetCellCb();

  UECM_FUNC(ueCb->crnti,CancelUe,UECM,CRM,"Cancel Ue");
  
  /* IF cell state is Unavailable All SRS, PUCCH resources are already freed
   * No need of separate clearing per UE Basis
   */
  if( UEH_CELL_STATE_UNAVAILABLE == cellCb->state )
  {
    return;
  }

  /* Free the PUCCH Resource for this UE */
  uehCrmDeleteUe(ueCb->crnti);

  /* Free the SRS Resource for this UE */
  uehCrmDeallocateSrs(uehCrmFindSfIndex(ueCb->ueSrsInfo.ueSrsCfgIndex),
                      ueCb->ueSrsInfo.ueTransmissionComb,
                      ueCb->ueSrsInfo.ueDedCyclicShift);
#endif

}

/* Admission Control Functionality for Initial Context Setup */
PUBLIC S16 initContxtAdmissionRequest(
    UehUeCb           *ueCb,
    UINT8             *causeType,
    UINT8             *causeVal
    )
{
  U8                  idx = 0;
  U8                  rbIdx = 0;
  S16                 ret = ROK;
  UehUeRABInfo        *rbInfo;
  char details[UEH_EVNT_DETAIL_MAX_LEN];

  rbInfo = &(ueCb->rbInfo);
  snprintf(details, UEH_EVNT_DETAIL_MAX_LEN, "NofRABs=%d", rbInfo->numOfDrbAdd);
  UECM_FUNC(ueCb->crnti,initContxtAdmissionRequest,UECM,CRM,details);

  if(MAX_RAB_PER_UE < rbInfo->numOfRbCfgd)
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE("Number of RBs Configured", rbInfo->numOfRbCfgd, 0, MAX_RAB_PER_UE);
    *causeType = CauseType_RadioNw;
    *causeVal = SztCauseRadioNwradio_resources_not_availableEnum;
    ret = RFAILED;
  }
  else
  {
    for(idx = 0; idx < rbInfo->numOfDrbAdd; idx++)
    {
      rbIdx = rbInfo->drbAddLstIndx[idx];
      if(5 > rbInfo->drb[rbIdx].qci)
      {
        UEH_ERROR_ELMNT_OUT_OF_RANGE("QCI Requested", rbInfo->drb[rbIdx].qci, 5, 7);
        *causeType = CauseType_RadioNw;
        *causeVal = SztCauseRadioNwnot_supported_QCI_valueEnum;
        ret = RFAILED;
      }
    }
  }

  return(ret);
}


PUBLIC S16 uehInitSysInfSch()
{
  U8           siIdx = 0;
  U8           siCnt = 0;
  U8           sibMap = 0;
  U8           n = 0; /*  n corresponds to the order of entry in the list of SI messages configured by schedulingInfoList in SIB1 */
  U8           w = 0; /*  si-WindowLength */
  U8           x = 0; /*  x = (n – 1)*w   */
  UehCellCb    *cellCb = NULLP;
 
  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED( CELL, NOT_APPLICABLE, EVT_BSM_CEM_OPSTART, ENT_BSM); 
    RETVALUE(RFAILED);
  }

  if(UEH_DISABLD_SIB == uehCb.uehCfgCb.sib1Cfg.sib2Periodicity)
  {
    UEH_PROCEDURE_FAILED("System Info scheduling", CELL, cellCb->cellId, "SIB2 periodicity is invalid");
    RETVALUE(RFAILED);
  }
  
  cellCb->siSchInfo[siCnt].siPrdcty = uehCb.uehCfgCb.sib1Cfg.sib2Periodicity;
  cellCb->siSchInfo[siCnt++].sibMapInfo = 0xFF; /* There is no mapping for SIB2 in SI so, sibMapInfo used for internal purpose */
  if(UEH_DISABLD_SIB != uehCb.uehCfgCb.sib1Cfg.sib3Periodicity)
  {
    cellCb->siSchInfo[siCnt].siPrdcty = uehCb.uehCfgCb.sib1Cfg.sib3Periodicity;
    cellCb->siSchInfo[siCnt++].sibMapInfo = sibMap++; /* sibMap = 0 corresponds to SIB3 */
  }
  if(UEH_DISABLD_SIB != uehCb.uehCfgCb.sib1Cfg.sib4Periodicity)
  {
    cellCb->siSchInfo[siCnt].siPrdcty = uehCb.uehCfgCb.sib1Cfg.sib4Periodicity;
    cellCb->siSchInfo[siCnt++].sibMapInfo = sibMap++; /* sibMap = 1 corresponds to SIB4 */
  }
#ifdef PWAV_SIB5
  if(UEH_DISABLD_SIB != uehCb.uehCfgCb.sib1Cfg.sib5Periodicity)
  {
    cellCb->siSchInfo[siCnt].siPrdcty = uehCb.uehCfgCb.sib1Cfg.sib5Periodicity;
    cellCb->siSchInfo[siCnt++].sibMapInfo = sibMap++; /* sibMap = 2 corresponds to SIB5 */
  }
#endif

  cellCb->numOfSis = siCnt;
#ifndef SI_NEW
  w = siWinSizeFrmWinLen[uehCb.uehCfgCb.sib1Cfg.siWindowLength];

  /* SI1 contains SIB2         corresponds siIdx = 0
   * SI2 contains SIB3/4/5 ... corresponds siIdx = 1
   * SI3 contains SIB4/5/6 ... corresponds siIdx = 2
   */
  for(siIdx = 0; siIdx < cellCb->numOfSis; siIdx++)
  {
    n++;
    x = (n - 1) * w;                                  /*   x = (n – 1) * w  */
    cellCb->siSchInfo[siIdx].siSchGap = x / 10;       /*   FLOOR(x / 10)    */
    cellCb->siSchInfo[siIdx].siSchSubframe = x % 10;  /* a = (x % 10        */
  }
#endif
#if 0
  for(siIdx = 0; siIdx < cellCb->numOfSis; siIdx++)
  {
    logInfo("SI%d -> siPrdcty: %d, sibMapInfo: %d, siSchGap: %d, siSubframe: %d", siIdx + 1, \
             cellCb->siSchInfo[siIdx].siPrdcty, cellCb->siSchInfo[siIdx].sibMapInfo, \
             cellCb->siSchInfo[siIdx].siSchGap, cellCb->siSchInfo[siIdx].siSchSubframe);
  }
#endif
  
  RETVALUE(ROK);
}

/** TunnelId related functions */
#define isTeIdValid(teid) (teid > 0) && (teid < UEH_MAX_TUNNEL_ID) ? TRUE : FALSE

void allocInitTeidPool(void)
{
  /** teidPool[i] = 0 means teId=i is not allocated */
  clearBytes(&teidPool[0], sizeof(teidPool));
}

void deallocTeidPool(void)
{
  //free(teidPool);
  clearBytes(&teidPool[0], sizeof(teidPool));
}

S32 allocateTeid(void)
{
  S32 teId;
  for (teId = 1; teId < UEH_MAX_TUNNEL_ID; teId++) /** teIds start from 1 */
  {
    if(teidPool[teId] EQ 0) /* 0 means not in use */
    {
      teidPool[teId] = 1; /* 1 means in use */  
      logInfo("Allocated TEID=%d",teId);
      return teId;
    }
  }
  logError("NO TEIDS AVAILABLE|capacity=%d", UEH_MAX_TUNNEL_ID);
  return -1;
}

void freeTeid( S32 teid )
{
  if(isTeIdValid(teid))
  {
    if(teidPool[teid] != 1)
    { /** does not seem to be in use. programming error */
      logError("call to deallocate teId=%d, which does not seem to be allocated (val=%d)", teid, teidPool[teid]);
    }
    teidPool[teid] = 0;
  }
  else
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE("tunnelId", teid, 1, UEH_MAX_TUNNEL_ID);
  }
}
