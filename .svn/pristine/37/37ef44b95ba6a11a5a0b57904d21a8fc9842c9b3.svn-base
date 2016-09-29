/*file    ueh_cell.c
 * @brief This file contains the cell configuration for UEH module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. @author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"
/* RRM related includes */
#include "ueh.h" 
#include "lueh.h"
#include "lueh.x" 
#include "ueh_eut_x.h" 
#include "log.h"


/***********************************************************************
                    NHU related functions
************************************************************************/
const U8 siWinSizeFrmWinLen[] = {1, 2, 5, 10, 15, 20, 40};
const U32 siPeriodicityVal[] = {8, 16, 32, 64, 128, 256, 512};

const U8 rachNumOfRaPreambles[] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64};
const U8 rachSizeOfRaPreambleGrpA[] = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
const U16 rachMsgGrpSizeInBits[] = { 56, 144, 208, 256 };


/* Mapping of OAM Bandwidth Configuration to RRC, CTF, RGR */
PUBLIC Bw2RbMapping bw2RbMapping[] =
{
   /* OAM                  RRC_SPEC                                          NHU/ RGR          CL */
   {DLBANDWIDTH_1DOT4MHZ,  NhuSysInfoBlockTyp2freqInfoul_Bandwidthn6Enum,    NHU_SYS_BW_N6,    CTF_BW_RB_6},
   {DLBANDWIDTH_3MHZ,	   NhuSysInfoBlockTyp2freqInfoul_Bandwidthn15Enum,   NHU_SYS_BW_N15,   CTF_BW_RB_15},
   {DLBANDWIDTH_5MHZ,      NhuSysInfoBlockTyp2freqInfoul_Bandwidthn25Enum,   NHU_SYS_BW_N25,   CTF_BW_RB_25},
   {DLBANDWIDTH_10MHZ,     NhuSysInfoBlockTyp2freqInfoul_Bandwidthn50Enum,   NHU_SYS_BW_N50,   CTF_BW_RB_50},
   {DLBANDWIDTH_15MHZ,     NhuSysInfoBlockTyp2freqInfoul_Bandwidthn75Enum,   NHU_SYS_BW_N75,   CTF_BW_RB_75},
   {DLBANDWIDTH_20MHZ,     NhuSysInfoBlockTyp2freqInfoul_Bandwidthn100Enum,  NHU_SYS_BW_N100,  CTF_BW_RB_100}
};


/*
*
*       Fun:   uehFillCellRbCfg
*
*       Desc:  This function Cell RB Configuration 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_cell.c
*
*/
PUBLIC S16 uehFillCellRbCfg(
    NhuRbCfgList  **rbCfgList
  )
{
   NhuRbCfgList *lclRbCfgList = NULLP;

   logFunc("%s","Configuring Cell RBs");

   lclRbCfgList = *rbCfgList;

   /* Filling RB for BCCH (SIB1) to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[0].rbId.rbId =  BCCH_SIB1_ID;
      lclRbCfgList->rbCfgInfoList[0].rbCfgBitMsk = NHU_RB_CFG_DL;
      {
         lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRbBitMsk = (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);
         /* DL Logical Channel Mapping Info
          * Logical Channel ID
          * Logical Channel Type
          * Transport Channel Type
          */
         {
            lclRbCfgList->rbCfgInfoList[0].lcId.lcId = BCCH_SIB1_ID;
            lclRbCfgList->rbCfgInfoList[0].lcId.logChType = CM_LTE_LCH_BCCH;
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode,
          * BCCH is always on RLC TM */
         {
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_TM;
            uehFillDlRlcModeInfo(&(lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode), BCCH_SIB1_ID, 0);
         }
      }
   }

   /* Filling RB for PCCH to PCH  Mapping */
   {
      lclRbCfgList->rbCfgInfoList[1].rbId.rbId = PCCH_ID;
      lclRbCfgList->rbCfgInfoList[1].rbCfgBitMsk = NHU_RB_CFG_DL;
      {
         lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRbBitMsk = (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);
         /* DL Logical Channel Mapping Info
          * Logical Channel ID
          * Logical Channel Type
          * Transport Channel Type
          */
         {
            lclRbCfgList->rbCfgInfoList[1].lcId.lcId = PCCH_ID;
            lclRbCfgList->rbCfgInfoList[1].lcId.logChType = CM_LTE_LCH_PCCH;
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_PCH;
         }
         /* DL-RLC mode */
         {
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_TM;
            uehFillDlRlcModeInfo(&(lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode), PCCH_ID, 0);
         }
      }
   }

   /* Filling RB for BCCH to BCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[2].rbId.rbId =  BCCH_BCH_ID;
      lclRbCfgList->rbCfgInfoList[2].rbCfgBitMsk = NHU_RB_CFG_DL;
      {
         lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRbBitMsk = (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);
         /* DL Logical Channel Mapping Info
          * Logical Channel ID
          * Logical Channel Type
          * Transport Channel Type
          */
         {
            lclRbCfgList->rbCfgInfoList[2].lcId.lcId = BCCH_BCH_ID;
            lclRbCfgList->rbCfgInfoList[2].lcId.logChType = CM_LTE_LCH_BCCH;
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_BCH;
         }
         /* DL-RLC mode */
         {
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_TM;
            uehFillDlRlcModeInfo(&(lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode), BCCH_BCH_ID, 0);
         }
      }
   }

   /* Filling RB for CCCH to UL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[3].rbId.rbId =  CCCH_UL_ID;
      lclRbCfgList->rbCfgInfoList[3].rbCfgBitMsk = NHU_RB_CFG_UL;
      {
         lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRbBitMsk = (NHU_RB_CFG_LOGCH_MAP_UL | NHU_RB_CFG_RLC_MODE_UL);

         /* UL Logical Channel Mapping Info
          * Logical Channel ID
          * Logical Channel Type
          * Transport Channel Type
          * Fill Logical Channel Cfg
          */
         {
            lclRbCfgList->rbCfgInfoList[3].lcId.lcId = CCCH_UL_ID;
            lclRbCfgList->rbCfgInfoList[3].lcId.logChType = CM_LTE_LCH_CCCH;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap.lcCfgType          = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap.lcCfg.logChCfgpres = PRSNT_NODEF;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap.lcCfg.logChGrp     = UEH_SRB_LC_GRP;
 
            /* Manu, 12Oct: Bug Fix.
             * The following error was being logged:
             *  [1325376138:992884][ERR:./src/cprm/ueh/qos/ueh_bldmsg.c:2110:Unexpected rbId=5]
             * Root cause: The function below was being invoked.
             *  uehFillUlLogChCfg( &(lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap), CCCH_UL_ID);
             *  In this function, the default case was getting hit and the group of DRB was getting assigned.
             * Fix Done: Moved the code for filling the lcCfg here, removed function call.
             *  Fixing the function itself would have required more info to be passed to it ... more changes.
             */
         }
         /* ulRlcMode */
         {
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode.mode = NHU_RLC_MODE_TM;
            uehFillUlRlcModeInfo( &(lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode), CCCH_UL_ID);
         }
      }
   }

   /* Filling RB for CCCH to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[4].rbId.rbId =  CCCH_DL_ID;
      lclRbCfgList->rbCfgInfoList[4].rbCfgBitMsk = NHU_RB_CFG_DL;
      {
         lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRbBitMsk |= (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);
         /* DL Logical Channel Mapping Info
          * Logical Channel ID
          * Logical Channel Type
          * Transport Channel Type
          */
         {
            lclRbCfgList->rbCfgInfoList[4].lcId.lcId = CCCH_DL_ID;
            lclRbCfgList->rbCfgInfoList[4].lcId.logChType = CM_LTE_LCH_CCCH;
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_TM;
            uehFillDlRlcModeInfo(&(lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode), CCCH_DL_ID, 0);
         }
      }
   }

   /* Filling RB for BCCH (non-SIB1) to DL-SCH Mapping*/
   {
      lclRbCfgList->rbCfgInfoList[5].rbId.rbId =  BCCH_SIB_OTHERS_ID;
      lclRbCfgList->rbCfgInfoList[5].rbCfgBitMsk = NHU_RB_CFG_DL;
      {
         lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRbBitMsk = (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);
         /* DL Logical Channel Mapping Info
          * Logical Channel ID
          * Logical Channel Type
          * Transport Channel Type
          */
         {
            lclRbCfgList->rbCfgInfoList[5].lcId.lcId = BCCH_SIB_OTHERS_ID;
            lclRbCfgList->rbCfgInfoList[5].lcId.logChType = CM_LTE_LCH_BCCH;
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_TM;
            uehFillDlRlcModeInfo(&(lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode), BCCH_SIB_OTHERS_ID, 0);
         }
      }
   }

   RETVALUE(ROK);
} /* End of uehFillCellRbCfg */


/*
*
*       Fun:   uehSndNhuCellCfgReq 
*
*       Desc:  Trigger NHU Cell Cfg Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndNhuCellCfgReq(
    U16  cellId
  )
{
   Pst               *pst = NULLP;
   NhuCellCfgReqSdus *nhuCellCfgReqSdus = NULLP;
   NhuRbCfgList      *rbCfgList = NULLP;
   NhuRachCfg        *rachCfg = NULLP;

   logFunc("%s", "Sending CELL config Request to RRC");

   /* Alloc event structure */
   UEH_ALLOCEVNT_OR_RET(&nhuCellCfgReqSdus, sizeof(NhuCellCfgReqSdus));

   /* Copying the pst structure */
   pst = &(uehCb.nhuSap[0]->pst);
   pst->event = EVTNHUCELLCFGREQ;

   /* Filling Header */
   nhuCellCfgReqSdus->hdr.cellId        = cellId;
   nhuCellCfgReqSdus->hdr.ueId          = UEH_VAL_ZERO;
   nhuCellCfgReqSdus->hdr.transId       = UEH_VAL_ONE;

   nhuCellCfgReqSdus->sdu.cfgType       = NHU_CFGTYPE_SETUP;
   nhuCellCfgReqSdus->sdu.u.cfgInfo.duplexMode = ( ((uehGetCellCb())->duplexMode == NHU_MODE_FDD) ? NHU_MODE_FDD : NHU_MODE_TDD);

   /*Bandwidth in terms of radio bearers*/
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.dlSysBw = UEH_CONVERT_DL_MHZ_TO_RB(uehCb.uehCfgCb.cellCfg.dlBandwidth); 
   if ( ULBANDWIDTH_NOTDEFINED EQ uehCb.uehCfgCb.cellCfg.ulBandwidth)
   {
     //Same as DL Bandwidth
     nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.ulSysBw = UEH_CONVERT_DL_MHZ_TO_RB(uehCb.uehCfgCb.cellCfg.dlBandwidth); 
   }
   else
   {
     nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.ulSysBw = UEH_CONVERT_UL_MHZ_TO_RB(uehCb.uehCfgCb.cellCfg.ulBandwidth); 
   }

   /* Update the bitmask,
    * We are sending RB Info and MAC Connon config */
   nhuCellCfgReqSdus->sdu.u.cfgInfo.cellCfgBitMsk = (NHU_CELL_CFG_RB | NHU_CELL_CFG_MAC_CMN);

   /* 
    * Fill RB Configuration 
    */
   rbCfgList = &(nhuCellCfgReqSdus->sdu.u.cfgInfo.rbToAddCfgList);
   /* 5 cellRbs are configured by default */
   rbCfgList->numOfRbCfgInfo = UEH_CELL_CFG_DFLT_RBS;
   UEH_GET_MEM( nhuCellCfgReqSdus, (sizeof(NhuRbCfgInfo)*rbCfgList->numOfRbCfgInfo), &(rbCfgList->rbCfgInfoList));
   uehFillCellRbCfg(&rbCfgList);

   /* 
    * MAC common configuration 
    * Fill RACH Configuration 
    */
   rachCfg = &(nhuCellCfgReqSdus->sdu.u.cfgInfo.macCmnCfg.rachCfg);
   rachCfg->maxMsg3Tx =   uehCb.uehCfgCb.sib2Cfg.maxHarqMsg3Tx; /*uehDfltMaxHARQMsg3Tx*/

  RETVALUE(uehSndMsgOut(ENTNH, EVENTTYPE_RRCCELLCFG,NULLP, TRANSID_NONE, nhuCellCfgReqSdus));
   

  
} /* uehSndNhuCellCfgReq */


/***********************************************************************
                    CTF related functions
************************************************************************/
/*
*
*       Fun:   uehSndCtfCellCfgReq 
*
*       Desc:  Trigger Cell Cfg 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
#ifndef RRC_PHY_CFG_SUPP
PUBLIC S16 uehSndCtfCellCfgReq(
    U16  cellId
  )
{
   CtfCfgReqInfo    *cellCfgReq = NULLP;
   Pst              *pst = NULLP;
   CtfCfgTransId    transId;

   logFunc("%s", "CELL CFG REQ -> CTF");

   /* Copying the pst structure */
   pst = &uehCb.ctfSap[0]->pst;
   pst->event   = EVTCTFCFGREQ;

   UEH_ALLOC(&cellCfgReq, sizeof(CtfCfgReqInfo));

   cmMemset((U8 *)&transId, '\0', sizeof(CtfCfgTransId));

   cellCfgReq->cfgType                          = CTF_CONFIG;
   cellCfgReq->u.cfg.cfgElem                    = CTF_CELL_CFG;
   cellCfgReq->u.cfg.u.cellCfg.cellId           = cellId;
   cellCfgReq->u.cfg.u.cellCfg.cellIdGrpId      = uehCb.uehCfgCb.cellCfg.phyCellIdentityGroup;
   cellCfgReq->u.cfg.u.cellCfg.physCellIdPres   = TRUE;
   cellCfgReq->u.cfg.u.cellCfg.physCellId       = uehCb.uehCfgCb.cellCfg.phyLayerId;
   //logInfo("WARNING: cellCfg.physCellId hardcoded to %d", cellCfgReq->u.cfg.u.cellCfg.physCellId);

   /* Dl/Ul bandwidth in terms of Resource blocks (RB)
    * Each RB spans 12 subcarriers i.e. 0.5 ms slot duration,
    * Each sub carrier is 15 KHz, so, each RB takes up 180 KHz
    */
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.dlBw = GET_NUM_RB_FRM_CH_BW(uehCb.uehCfgCb.cellCfg.dlBandwidth); /* Convert bw in MHz to RB*/
   if ( ULBANDWIDTH_NOTDEFINED EQ uehCb.uehCfgCb.cellCfg.ulBandwidth)
   {
     //Same as DL Bandwidth
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.ulBw = GET_NUM_RB_FRM_CH_BW(uehCb.uehCfgCb.cellCfg.dlBandwidth); /* Convert bw in MHz to RB*/
   }
   else
   {
     cellCfgReq->u.cfg.u.cellCfg.bwCfg.ulBw = GET_NUM_RB_FRM_CH_BW(uehCb.uehCfgCb.cellCfg.ulBandwidth); /* Convert bw in MHz to RB*/
   }

   cellCfgReq->u.cfg.u.cellCfg.bwCfg.eUtraBand = (uehGetCellCb())->sysRelInfo.freqBandInd;

   /* Basic transmission scheme */
   cellCfgReq->u.cfg.u.cellCfg.txCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.txCfg.duplexMode = (((uehGetCellCb())->duplexMode == CTF_DUPMODE_FDD) ? CTF_DUPMODE_FDD : CTF_DUPMODE_TDD);
   cellCfgReq->u.cfg.u.cellCfg.txCfg.scSpacing  = smCb.cfgCb.uehConfig.scSpacing;
   cellCfgReq->u.cfg.u.cellCfg.txCfg.cycPfx     = smCb.cfgCb.uehConfig.cycPfxType;


   /* Antenna config */ 
   logInfo("WARNING: GLOBAL VARIABLE VALUE ASSUMED. Filling cellCfg.antennaCfg.antPortsCnt from smCb.cfgCb.uehConfig.numOfTxAnt (=%d)", \
                smCb.cfgCb.uehConfig.numOfTxAnt);
   if (smCb.cfgCb.uehConfig.numOfTxAnt EQ 1)
     cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = CTF_AP_CNT_1; /* Manu:2nd Aug was CTF_AP_CNT_2 earlier */
   else if (smCb.cfgCb.uehConfig.numOfTxAnt EQ 2) 
     cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = CTF_AP_CNT_2; /* Manu:2nd Aug was CTF_AP_CNT_2 earlier */
   else if (smCb.cfgCb.uehConfig.numOfTxAnt EQ 3) 
     cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = CTF_AP_CNT_4; /* Manu:2nd Aug was CTF_AP_CNT_2 earlier */
   else
   {
      UEH_ERROR_ELMNT_OUT_OF_RANGE("smCb.cfgCb.uehConfig.numOfTxAnt", smCb.cfgCb.uehConfig.numOfTxAnt, 1, 3);
      UEH_FREE(cellCfgReq, sizeof(CtfCfgReqInfo));
      RETVALUE(RFAILED);
   }
   /* PRACH config */
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.rootSequenceIndex       = uehCb.uehCfgCb.sib2Cfg.rootSequenceIndex;/*uehDfltRootSequenceIdx*/
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.prachCfgIndex           = uehCb.uehCfgCb.sib2Cfg.prachConfigIndex;/*uehDfltPrachCfgIdx*/
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.zeroCorrelationZoneCfg  = uehCb.uehCfgCb.sib2Cfg.zeroCorrelationZoneConfig; /*uehDfltZeroCorrelZoneCfg*/
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.highSpeedFlag           = uehCb.uehCfgCb.sib2Cfg.highSpeedFlag;/*uehDfltHighSpeedFlag*/
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.prachFreqOffset         = uehCb.uehCfgCb.phyCfg.prachFreqOffset; /*uehDfltPrachFreqOffset*/

   /* PDSCH config */
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.refSigPwr               =  uehCb.uehCfgCb.phyCfg.referenceSignalPower;/*uehDfltRefSignalPower*/
/* TODO: Need to be taken from Config file */
   cellCfgReq->u.cfg.u.cellCfg.priSigPwr= 65;
   cellCfgReq->u.cfg.u.cellCfg.secSigPwr= 65;
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.p_b                     =  uehCb.uehCfgCb.phyCfg.cellPhyPb;

   /* PUSCH config */
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.noOfsubBands   = uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingSubBands;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.hoppingMode    = uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingMode;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.hoppingOffset  = uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingOffset; /* uehDfltPuschHoppingOff*/
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.enable64QAM    = uehCb.uehCfgCb.sib2Cfg.enable64QAM;//uehDfltEnable64QAM;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.grpHopEnabled      = uehCb.uehCfgCb.sib2Cfg.ulRsGroupHoppingEnabled;/*uehDfltGroupHoppingEnabled*/
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.seqHopEnabled      = uehCb.uehCfgCb.sib2Cfg.ulRsSeqHoppingEnabled;/*uehDfltSeqHoppingEnabled*/
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.grpNum             = uehCb.uehCfgCb.sib2Cfg.ulRsGroupPusch; /*uehDfltGroupAssignPUSCH*/
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.cycShift           = uehCb.uehCfgCb.sib2Cfg.ulRsCyclicShiftPusch; /*uehDfltCyclicShift*/

   /* PHICH config */
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.pres = PRSNT_NODEF;
   /* Getting copied to ENUM only. Hence no convertion to actual value is required. */
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.duration = uehCb.uehCfgCb.phyCfg.phichDuration; /*uehDfltPhichDur*/
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.resource = uehCb.uehCfgCb.phyCfg.phichResource; /*uehDfltPhichRes*/

   /* PUCCH config */
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nRB   = uehCb.uehCfgCb.sib2Cfg.nRbCqi;/*uehDfltPUCCHnRBCQI*/
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nCS   = uehCb.uehCfgCb.sib2Cfg.nCsAn;/*uehDfltPUCCHnCSAN*/
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.n1PUCCH = uehCb.uehCfgCb.sib2Cfg.n1PucchAn;/*uehDfltNlPUCCHAn*/
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.deltaShift = uehCb.uehCfgCb.sib2Cfg.deltaPucchShift;/*uehDfltPUCCHDeltaShift*/
 /* 14th Sept: changes for 4GMX by CCPU-Reema */
/*   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nRB   = 1;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nCS   = 0;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.n1PUCCH = 1;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.deltaShift = CTF_PUCCH_DELTASHIFT1;
*/
   /* SRS UL config, setup case */
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.pres = NOTPRSNT;
#if 0
/* TODO: Need to take from Config file when srs supported */
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.srsBw = 0 ;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.sfCfg = 15;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.srsANSimultTx = 0;
#endif 
#ifdef LTE_TDD   
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.sfAssignment = uehDfltSubframeAssignment;   
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.spclSfPatterns = uehDfltSpecialSubfrmPatterns;
#else /* LTE_TDD */   
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.pres = NOTPRSNT;
#endif /* LTE_TDD */	

   /* ve004.102: added the remaining fields to avoid hard coding at CL */
   cellCfgReq->u.cfg.u.cellCfg.syncSigPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.syncSigPowOs = 6000;

   cellCfgReq->u.cfg.u.cellCfg.cfiPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.cfiPowOs = 6000;
   
   cellCfgReq->u.cfg.u.cellCfg.dciPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.dciPowOs = 6000;

   cellCfgReq->u.cfg.u.cellCfg.extWinMarginPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.extWinMargin = 5;

   cellCfgReq->u.cfg.u.cellCfg.pucchNoiseGammaPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pucchNoiseGamma = 0x2000;

   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio4Pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio4 = 0x24eb;

   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio0Pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio0 = 0x38cc;

   cellCfgReq->u.cfg.u.cellCfg.srsDopEstFactorPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.srsDopEstFactor = 0xffff;

   cellCfgReq->u.cfg.u.cellCfg.puschProbDtxAckPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.puschProbDtxAck = 22;

   cellCfgReq->u.cfg.u.cellCfg.pucchProbDtxAckPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pucchProbDtxAck = 25;

   cellCfgReq->u.cfg.u.cellCfg.txAntennaPortsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.txAntennaPorts = 1;

   cellCfgReq->u.cfg.u.cellCfg.rxAntennaPortsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.rxAntennaPorts = 1;

   cellCfgReq->u.cfg.u.cellCfg.phySyncModePres= PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.phySyncMode = 0x02;

   cellCfgReq->u.cfg.u.cellCfg.dataRepModePres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.dataRepMode = 0x00;

   cellCfgReq->u.cfg.u.cellCfg.rachSrRepModePres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.rachSrRepMode = 0 ;

   cellCfgReq->u.cfg.u.cellCfg.opMode  = 0; /* CPRI */
   cellCfgReq->u.cfg.u.cellCfg.counter = 0; /* 0: CPRI-CPRI mode , 2: RRH Mode */
   if (2 == smCb.cfgCb.uehConfig.numOfTxAnt)
   {
     cellCfgReq->u.cfg.u.cellCfg.period  = 5;
   }
   else
   {
     cellCfgReq->u.cfg.u.cellCfg.period  = 0;
   }

 RETVALUE( uehSndMsgOut(ENTTF /*ENTRG*/, EVENTTYPE_PHYCELLCONFIG, NULLP, &transId,  cellCfgReq));
 

} /* uehSndCtfCellCfgReq */
#endif

/***********************************************************************
                    RGR related functions
************************************************************************/


/*
*
*       Fun:   uehFillRgrDlHqCfg 
*
*       Desc:  Populating Dl Harq Cfg
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehFillRgrDlHqCfg(
    RgrDlHqCfg *cfg
  )
{
   logFunc("%s","uehFillRgrDlHqCfg");

   cfg->maxDlHqTx =  uehCb.uehCfgCb.schCfg.maxDlHarqTx;/*uehDfltMaxDlHqTx*/
   cfg->maxMsg4HqTx = uehCb.uehCfgCb.schCfg.maxMsg4HarqTx;/*uehDfltMaxMsg4HqTx*/

   RETVALUE(ROK);
} /* uehFillRgrDlHqCfg */ 

/*
*
*       Fun:   uehFillRgrRntiCfg 
*
*       Desc:  Populating RNTI Cfg 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
PUBLIC S16 uehFillRgrRntiCfg(
    RgrRntiCfg *cfg
  )
{
   logFunc("%s","uehFillRgrRntiCfg");

   cfg->startRnti = uehCb.uehCfgCb.cellCfg.startRnti;/*uehDfltStartRnti*/
   cfg->size =  (uehGetCellCb())->maxRntiPool;/* uehDfltMacRntisize = maxConnectedUes + 20(given for HO) */

   RETVALUE(ROK);
} /* uehFillRgrRntiCfg */ 

/*
*
*       Fun:   uehFillRgrCfiCfg 
* 
*       Desc:  Populating CFI Cfg
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
PUBLIC S16 uehFillRgrCfiCfg(
    RgrCfiCfg *cfg
  )
{
   logFunc("%s","uehFillRgrCfiCfg");

   cfg->cfi =  uehCb.uehCfgCb.schCfg.pcfichCfi;/*uehDfltCfiCfg*/

   RETVALUE(ROK);
} /* uehFillRgrCfiCfg */


/*
 *
 *      Fun:   uehFillRgrUlTrgCqiCfg
 *
 *      Desc:  Populate RgrUlTrgCqiCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrUlTrgCqiCfg(
    RgrUlTrgCqiCfg *cfg
  )
{
   logFunc("%s","uehFillRgrUlTrgCqiCfg");
   cfg->trgCqi = uehCb.uehCfgCb.cellCfg.initialUlCqi; /*uehDfltTrgUlCqi;*/
   logConfig("Init UL CQI: %d", cfg->trgCqi);
   RETVALUE(ROK);
} /* uehFillRgrUlTrgCqiCfg */

/*
 *
 *      Fun:   uehFillRgrDlCmnCodeRateCfg
 *
 *      Desc:  Populate RgrDlCmnCodeRateCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrDlCmnCodeRateCfg(
    RgrDlCmnCodeRateCfg *cfg
  )
{
   logFunc("%s","uehFillRgrDlCmnCodeRateCfg");

   cfg->bcchPchRaCodeRate = smCb.cfgCb.uehConfig.bcchPchRaCodeRate ;//uehDfltBcchPchRaCodeRate;
   cfg->pdcchCodeRate     = smCb.cfgCb.uehConfig.pdcchCodeRate ;//uehDfltPdcchCodeRate;
   cfg->ccchCqi           = smCb.cfgCb.uehConfig.dlCmnCodRateCcchCqi ;//uehDfltDlCmnCodRateCcchCqi;

   RETVALUE(ROK);
} /* uehFillRgrDlCmnCodeRateCfg */

/*
 *
 *      Fun:   uehFillRgrPuschSubBandCfg
 *
 *      Desc:  Populate RgrPuschSubBandCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrPuschSubBandCfg(
    RgrPuschSubBandCfg *cfg
  )
{

   logFunc("%s","uehFillRgrPuschSubBandCfg");

   cfg->subbandStart = smCb.cfgCb.uehConfig.puschSubBndStrt;
   cfg->numSubbands  = smCb.cfgCb.uehConfig.puschNoSubbnds;
   cfg->size         = smCb.cfgCb.uehConfig.puschSubBandSz;
   
   if(cfg->size > RGR_MAX_SUBBANDS)
   {
      UEH_ERROR_ELMNT_OUT_OF_RANGE( "Number of equal sized sub-bands ", cfg->size, 0, RGR_MAX_SUBBANDS );
      RETVALUE(RFAILED);
   }

   memset(&cfg->dmrs, smCb.cfgCb.uehConfig.puschSubBandDmrs, RGR_MAX_SUBBANDS);
#if 0
   U8 dmrsCnt = 0;
   for(dmrsCnt = 0; dmrsCnt < cfg->numSubbands; dmrsCnt++)
   {
      /* Sample values for the following */
      //cfg->dmrs[dmrsCnt] = smCb.cfgCb.uehConfig.puschSubBandDmrs + dmrsCnt ;//uehDfltPuschSubBandDmrs + dmrsCnt;
      cfg->dmrs[dmrsCnt] = 0;   /* Sept7th changed to 0 */
   }
#endif
   RETVALUE(ROK);
} /* uehFillRgrPuschSubBandCfg */


/*
 *
 *      Fun:   uehFillRgrUlCmnCodeRateCfg
 *
 *      Desc:  Populate RgrUlCmnCodeRateCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrUlCmnCodeRateCfg(
    RgrUlCmnCodeRateCfg *cfg
  )
{
   logFunc("%s","uehFillRgrUlCmnCodeRateCfg");

   cfg->ccchCqi = smCb.cfgCb.uehConfig.ulCmnCodRateCcchCqi;//uehDfltULCmnCodRateCcchCqi;
   RETVALUE(ROK);
} /* uehFillRgrUlCmnCodeRateCfg */

/*
 *
 *      Fun:   uehFillRgrDlfsCfg
 *
 *      Desc:  Populate RgrDlfsCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrDlfsCfg(
    RgrDlfsCfg *cfg
  )
{
   logFunc("%s","uehFillRgrDlfsCfg");

   cfg->isDlFreqSel  = smCb.cfgCb.uehConfig.isDlFreqSel;//uehDfltIsDlFreqSel;
   cfg->thresholdCqi = smCb.cfgCb.uehConfig.thresholdCqi;//uehDfltThresholdCqi;

   RETVALUE(ROK);
} /* uehFillRgrDlfsCfg */


/*
 *
 *      Fun:   uehFillRgrBwCfg
 *
 *      Desc:  Populate RgrBwCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrBwCfg(
    RgrBwCfg *cfg
  )
{
  logFunc("%s","uehFillRgrBwCfg");

  cfg->dlTotalBw = UEH_CONVERT_DL_MHZ_TO_RB(uehCb.uehCfgCb.cellCfg.dlBandwidth); 
  if (uehCb.uehCfgCb.cellCfg.ulBandwidth EQ ULBANDWIDTH_NOTDEFINED)
  {
    cfg->ulTotalBw = UEH_CONVERT_DL_MHZ_TO_RB(uehCb.uehCfgCb.cellCfg.dlBandwidth); 
  }
  else
  {
    cfg->ulTotalBw = UEH_CONVERT_UL_MHZ_TO_RB(uehCb.uehCfgCb.cellCfg.ulBandwidth); 
  }

  RETVALUE(ROK);
} /* uehFillRgrBwCfg */


/*
 *
 *      Fun:   uehFillRgrPhichCfg
 *
 *      Desc:  Populate RgrPhichCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrPhichCfg(
    RgrPhichCfg *cfg
  )
{
   logFunc("%s","uehFillRgrPhichCfg");

   cfg->ngEnum      = uehCb.uehCfgCb.phyCfg.phichResource; //uehDfltNgEnum;
   cfg->isDurExtend = uehCb.uehCfgCb.phyCfg.phichDuration; //uehDfltIsDurExt;

   RETVALUE(ROK);
} /* uehFillRgrPhichCfg */


/*
 *
 *      Fun:   uehFillRgrPucchCfg
 *
 *      Desc:  Populate RgrPucchCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrPucchCfg(
    RgrPucchCfg *cfg
  )
{
   logFunc("%s","uehFillRgrPucchCfg");

   cfg->resourceSize = uehCb.uehCfgCb.cellCfg.pucchResourceSize; /*uehDfltPUCCHResSz*/
   cfg->n1PucchAn    = uehCb.uehCfgCb.sib2Cfg.n1PucchAn;/*uehDfltN1PucchAn*/
   cfg->deltaShift   = uehCb.uehCfgCb.sib2Cfg.deltaPucchShift + 1; /* TODO: CTF expects 0 1 2, MAC expects as 1 2 3*/
   cfg->cyclicShift  = uehCb.uehCfgCb.sib2Cfg.ulRsCyclicShiftPusch; /*uehDfltPUCCHCycShift*/

   RETVALUE(ROK);
} /* uehFillRgrPucchCfg */


/*
 *
 *      Fun:   uehFillRgrSrsCfg
 *
 *      Desc:  Populate RgrSrsCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrSrsCfg(
    RgrSrsCfg *cfg
  )
{
   logFunc("%s","uehFillRgrSrsCfg");

   cfg->srsCfgPrdEnum  = smCb.cfgCb.uehConfig.srsCfgPrd;//uehDfltSrsCfgPrdEnum;
   cfg->srsBwEnum      = uehCb.uehCfgCb.sib2Cfg.srsBandwidthConfig; /*uehDfltSrsBwEnum*/
   cfg->srsSubFrameCfg = uehCb.uehCfgCb.sib2Cfg.srsSubfrmConfig; /*uehDfltSrsSubFrameCfg*/

   RETVALUE(ROK);
} /* uehFillRgrSrsCfg */


/*
 *
 *      Fun:   uehFillRgrSiCfg
 *
 *      Desc:  Populate RgrSiCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrSiCfg(
    RgrSiCfg  *cfg
  )
{
#ifdef SI_NEW
  U8           siIdx = 0;
  UehCellCb    *cellCb = NULLP;
#endif

  logFunc("%s","uehFillRgrSiCfg");

  cfg->siWinSize = siWinSizeFrmWinLen[uehCb.uehCfgCb.sib1Cfg.siWindowLength]; /*uehDfltSiWinSize*/
  /* Note: make sure that SiReTxCnt should be less than siWinSize. This check should be done by OAM Module-BSM */
  cfg->retxCnt = uehCb.uehCfgCb.cellCfg.siReTxCnt; /* uehDfltRetxCnt */

#ifdef SI_NEW
  cfg->modPrd = uehDfltModPrd;

  cellCb = uehGetCellCb();
  /* traversing in the loop to store the system information periodicity values */
  for(; siIdx < cellCb->numOfSis; siIdx++)
  {
    cfg->siPeriodicity[siIdx] = siPeriodicityVal[cellCb->siSchInfo[siIdx].siPrdcty];
  }
  cfg->numSi = cellCb->numOfSis;
#endif

  RETVALUE(ROK);
} /* uehFillRgrSiCfg */

/*
 *
 *      Fun:   uehFillRgrUlPwrCfg
 *
 *      Desc:  Populate RgrUlPwrCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrUlPwrCfg(
    RgrUlPwrCfg *cfg
  )
{
   logFunc("%s","uehFillRgrUlPwrCfg");

   cfg->p0NominalPusch             = uehCb.uehCfgCb.sib2Cfg.poNominalPusch; /* uehDfltP0NominalPusch */
   cfg->alpha                      = uehGetAlphaForOamVal(uehCb.uehCfgCb.phyCfg.alpha);//uehCb.uehCfgCb.phyCfg.alpha; //uehDfltUlPwrCfgAlpha;
   cfg->p0NominalPucch             = uehCb.uehCfgCb.phyCfg.poNominalPucch;/*uehDfltP0NominalPucch*/
   cfg->deltaPreambleMsg3          = uehCb.uehCfgCb.sib2Cfg.deltaPreambleMsg3;/*uehDfltDeltaPreambleMsg3*/
   cfg->pucchPwrFmt3.startTpcRnti  = smCb.cfgCb.uehConfig.pucchPwrFmt3RntiSt; //uehDfltPucchPwrFmt3RntiSt;
   cfg->pucchPwrFmt3.size          = smCb.cfgCb.uehConfig.pucchPwrFmt3RntiSz; //uehDfltPucchPwrFmt3RntiSz;
   cfg->pucchPwrFmt3a.startTpcRnti = smCb.cfgCb.uehConfig.pucchPwrFmt3aRntiSt; //uehDfltPucchPwrFmt3aRntiSt;
   cfg->pucchPwrFmt3a.size         = smCb.cfgCb.uehConfig.pucchPwrFmt3aRntiSz; //uehDfltPucchPwrFmt3aRntiSz;
   cfg->puschPwrFmt3.startTpcRnti  = smCb.cfgCb.uehConfig.puschPwrFmt3RntiSt; //uehDfltPuschPwrFmt3RntiSt;
   cfg->puschPwrFmt3.size          = smCb.cfgCb.uehConfig.puschPwrFmt3RntiSz; //uehDfltPuschPwrFmt3RntiSz;
   cfg->puschPwrFmt3a.startTpcRnti = smCb.cfgCb.uehConfig.puschPwrFmt3aRntiSt; //uehDfltPuschPwrFmt3aRntiSt;
   cfg->puschPwrFmt3a.size         = smCb.cfgCb.uehConfig.puschPwrFmt3aRntiSz; //uehDfltPuschPwrFmt3aRntiSz;

   RETVALUE(ROK);
} /* uehFillRgrUlPwrCfg */

/*
 *
 *      Fun:   uehFillRgrPuschCfg
 *
 *      Desc:  Populate RgrPuschCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrPuschCfg(
    RgrPuschCfg *cfg
  )
{
   logFunc("%s","uehFillRgrPuschCfg");

   cfg->numSubBands = uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingSubBands;/* uehDfltPuschCfgNoSubBands*/
   cfg->isIntraHop  = uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingMode ; /*uehDfltPuschCfgIsIntraHop, intra or inter sub-frm hopping*/
   cfg->hopOffst    = uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingOffset; /*uehDfltPuschCfgHopOffst*/

   RETVALUE(ROK);
} /* uehFillRgrPuschCfg */

/*
 *
 *      Fun:   uehFillRgrPreambleSetCfg
 *
 *      Desc:  Populate RgrPreambleSetCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrPreambleSetCfg(
    RgrPreambleSetCfg *cfg
  )
{
   logFunc("%s","uehFillRgrPreambleSetCfg");

   cfg->pres  = smCb.cfgCb.uehConfig.preambleSetCfgPres; //uehDfltPreambleSetCfgPres;
   cfg->start = smCb.cfgCb.uehConfig.preambleSetCfgStart; //uehDfltPreambleSetCfgStart;
   cfg->size  = smCb.cfgCb.uehConfig.preambleSetCfgSz; //uehDfltPreambleSetCfgSz;

   RETVALUE(ROK);
} /* uehFillRgrPreambleSetCfg */

#ifdef LTE_TDD
/*
 *
 *      Fun:   uehFillRgrTddPrachRscInfo
 *
 *      Desc:  Populate RgrTddPrachRscInfo Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrTddPrachRscInfo
(
RgrTddPrachRscInfo *cfg
)
{
   U8 idx = 0;

   logFunc("%s","uehFillRgrTddPrachRscInfo");

   cfg->numRsc = uehDfltPRACHNumRes;

   if(cfg->numRsc > RGR_TDD_MAX_FREQ_RSRC)
   {
      UEH_ERROR_ELMNT_OUT_OF_RANGE(" Number of PRACH resources", cfg->numRsc, 0   RGR_TDD_MAX_FREQ_RSRC);
      RETVALUE(RFAILED);
   }
   
   for(idx = 0; idx < cfg->numRsc; idx++)
   {
      /* Sample values */
      cfg->prachInfo[idx].freqIdx = idx;
      cfg->prachInfo[idx].sfn = idx + 1;
      cfg->prachInfo[idx].halfFrm = idx;
      cfg->prachInfo[idx].ulStartSfIdx = idx;
   }

   RETVALUE(ROK);
} /* uehFillRgrTddPrachRscInfo */

#endif /* LTE_TDD */

/*
 *
 *      Fun:   uehFillRgrRachCfg
 *
 *      Desc:  Populate RgrRachCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrRachCfg(
    RgrRachCfg *cfg
  )
{
   U8 idx = 0;

   logFunc("%s","uehFillRgrRachCfg");

   cfg->preambleFormat     = smCb.cfgCb.uehConfig.preambleFrmt ;//uehDfltPreambleFrmt;
   cfg->raWinSize          = smCb.cfgCb.uehConfig.raWinSz ;     //uehDfltRaWinSz;
   cfg->raOccasion.size    = smCb.cfgCb.uehConfig.raOccSz ;     //uehDfltRaOccSz;
   cfg->raOccasion.sfnEnum = smCb.cfgCb.uehConfig.raOccSfnEnum ;//uehDfltRaOccSfnEnum;
   
   if(cfg->raOccasion.size > RGR_MAX_SUBFRAME_NUM)
   {
      UEH_ERROR_ELMNT_OUT_OF_RANGE("Number of RA subframe numbers", cfg->raOccasion.size, 0, RGR_MAX_SUBFRAME_NUM);
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < cfg->raOccasion.size; idx++)
   {
      cfg->raOccasion.subFrameNum[idx] = idx + 1;
   }

   cfg->maxMsg3Tx          = uehCb.uehCfgCb.sib2Cfg.maxHarqMsg3Tx; /*uehDfltMaxMsg3Tx, Same as uehDfltMaxHARQMsg3Tx*/
   cfg->numRaPreamble      = rachNumOfRaPreambles[uehCb.uehCfgCb.sib2Cfg.numRaPreambles]; /*uehDfltNumRaPreamble*/
   cfg->sizeRaPreambleGrpA = rachSizeOfRaPreambleGrpA[uehCb.uehCfgCb.sib2Cfg.sizeRAPreamblesGroupA]; /*uehDfltSzRaPreambleGrpA*/
   cfg->msgSizeGrpA        = rachMsgGrpSizeInBits[uehCb.uehCfgCb.sib2Cfg.messageSizeGroupA]; /*uehDfltMsgSzGrpA*/
   cfg->prachResource      = smCb.cfgCb.uehConfig.prachResource;//uehDfltPrachResource;
   
#ifdef RGR_V1
   switch(uehCb.uehCfgCb.sib2Cfg.macContResolutionTimer)
   {
     case 0: /*sf8*/   cfg->contResTmr = 8;  break;
     case 1: /*sf16*/  cfg->contResTmr = 16; break;
     case 2: /*sf24*/  cfg->contResTmr = 24; break; 
     case 3: /*sf32*/  cfg->contResTmr = 32; break;
     case 4: /*sf40*/  cfg->contResTmr = 40; break;
     case 5: /*sf48*/  cfg->contResTmr = 48; break;
     case 6: /*sf56*/  cfg->contResTmr = 56; break;
     case 7: /*sf64*/  cfg->contResTmr = 64; break;
     default:
      UEH_ERROR_ELMNT_OUT_OF_RANGE("contResTmr", cfg->contResTmr, 0, 7);
      cfg->contResTmr = 0;
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
} /* uehFillRgrRachCfg */

/*
 *
 *      Fun:   uehFillRgrCellDlPfs
 *
 *      Desc:  Populate RgrCellDlPfs Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrCellDlPfs(
    RgrCellDlPfs *cfg
  )
{
   U8 idx = 0;

   logFunc("%s","uehFillRgrCellDlPfs");

   for(idx = 0; idx < RGR_PFS_CQI_PRIO_LVLS; idx++)
   {
      cfg->cqiPrios[idx]     = idx + 2; /* Sample value from acc. */
   }
   for(idx = 0; idx < RGR_PFS_FRAC_PRIO_LVLS; idx++)
   {
      cfg->fracPrios[idx]    = idx + 2; /* Sample value from acc. */
   }
   for(idx = 0; idx < RGR_PFS_SVC_PRIO_LVLS; idx++)
   {
      cfg->svcPrioPrios[idx] = idx + 2; /* Sample value from acc. */
   }

   RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehFillRgrCellUlPfs
 *
 *      Desc:  Populate RgrCellUlPfs Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrCellUlPfs(
    RgrCellUlPfs *cfg
  )
{
   U8 idx = 0;

   logFunc("%s","uehFillRgrCellUlPfs");

   for(idx = 0; idx < RGR_PFS_CQI_PRIO_LVLS; idx++)
   {
      cfg->cqiPrios[idx]  = idx + 2; /* Sample value from acc. */
   }
   for(idx = 0; idx < RGR_PFS_FRAC_PRIO_LVLS; idx++)
   {
      cfg->fracPrios[idx] = idx + 2; /* Sample value from acc. */
   }

   for(idx = 0; idx < 3; idx++)
   {
      cfg->lcgPrios[idx] = idx + 2; /* Sample value from acc. */
   }

   RETVALUE(ROK);
} /* uehFillRgrCellUlPfs */

/*
 *
 *      Fun:   uehFillRgrCmnLcCfg
 *
 *      Desc:  Populate RgrCellCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
PUBLIC S16 uehFillRgrCmnLcCfg(
    RgrCellCfg *cfg
  )
{
   logFunc("%s","uehFillRgrCmnLcCfg");
   S16 i = 0;


   /* DL, BCCH mapped to BCH */
   cfg->cmnLcCfg[i].lcId       = BCCH_BCH_ID;
   cfg->cmnLcCfg[i].lcType     = CM_LTE_LCH_BCCH;
   cfg->cmnLcCfg[i].dir        = RGR_DIR_TX;
   cfg->cmnLcCfg[i].dlTrchType = CM_LTE_TRCH_BCH;
   cfg->cmnLcCfg[i].ulTrchType = 0;
   i++;

   /* DL, BCCH mapped to DL-SCH */
   cfg->cmnLcCfg[i].lcId       = BCCH_SIB1_ID;
   cfg->cmnLcCfg[i].lcType     = CM_LTE_LCH_BCCH;
   cfg->cmnLcCfg[i].dir        = RGR_DIR_TX;
   cfg->cmnLcCfg[i].dlTrchType = CM_LTE_TRCH_DL_SCH;
   cfg->cmnLcCfg[i].ulTrchType = 0;
   i++;

   /* DL, PCCH mapped to PCH */
   cfg->cmnLcCfg[i].lcId       = PCCH_ID;
   cfg->cmnLcCfg[i].lcType     = CM_LTE_LCH_PCCH;
   cfg->cmnLcCfg[i].dir        = RGR_DIR_TX;
   cfg->cmnLcCfg[i].dlTrchType = CM_LTE_TRCH_PCH;
   cfg->cmnLcCfg[i].ulTrchType = 0;
   i++;
   
   /* DL, CCCH mapped to DL-SCH */
   cfg->cmnLcCfg[i].lcId       = CCCH_DL_ID;
   cfg->cmnLcCfg[i].lcType     = CM_LTE_LCH_CCCH;
   cfg->cmnLcCfg[i].dir        = RGR_DIR_TX;
   cfg->cmnLcCfg[i].dlTrchType = CM_LTE_TRCH_DL_SCH;
   cfg->cmnLcCfg[i].ulTrchType = 0;
   i++;

   /* UL, CCCH mapped to UL-SCH */
   cfg->cmnLcCfg[i].lcId       = CCCH_UL_ID;
   cfg->cmnLcCfg[i].lcType     = CM_LTE_LCH_CCCH;
   cfg->cmnLcCfg[i].dir        = RGR_DIR_RX;
   cfg->cmnLcCfg[i].dlTrchType = 0;
   cfg->cmnLcCfg[i].ulTrchType = CM_LTE_TRCH_UL_SCH;
   i++;

   /* TODO: What channel is this? Need to remove */
   cfg->cmnLcCfg[i].lcId       = BCCH_SIB_OTHERS_ID;
   cfg->cmnLcCfg[i].lcType     = CM_LTE_LCH_BCCH;
   cfg->cmnLcCfg[i].dir        = RGR_DIR_TX;
   cfg->cmnLcCfg[i].dlTrchType = CM_LTE_TRCH_DL_SCH;
   cfg->cmnLcCfg[i].ulTrchType = 0;
   i++;

   cfg->numCmnLcs = i;
   if(cfg->numCmnLcs > RGR_MAX_CMN_LC_PER_CELL)
   {
      UEH_ERROR_ELMNT_OUT_OF_RANGE("Common Channels", cfg->numCmnLcs, 0, RGR_MAX_CMN_LC_PER_CELL);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* uehFillRgrCmnLcCfg */

/*
*
*       Fun:   uehHdlRgrCellCfgReq 
*
*       Desc:  Trigger Cell Cfg 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehHdlRgrCellCfgReq(
    U16            cellId
  )
{
   Pst           *pst = NULLP;
   RgrCfgReqInfo *cfgReq = NULLP;
   RgrCellCfg    *cfg = NULLP;
   RgrCfgTransId transId;

   logFunc("%s", "Triggering CELL Configg to MAC(RGR)");

   /* Assign Pst */
   pst = &uehCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Trans Id */
   UEH_SET_ZERO(&transId , sizeof(RgrCfgTransId));

   /* Allocate RGR Cfg struct */
   UEH_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if( NULL EQ cfgReq )
   {
     UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RgrCfgReqInfo), "RgrCfgReqInfo");
     RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_CELL_CFG;

   cfg = &(cfgReq->u.cfgInfo.u.cellCfg);

   /* Populate RgrCellCfg */
   cfg->cellId  = cellId;

   /* MAC instance */
   cfg->macInst = smCb.cfgCb.uehConfig.macInst; //uehDfltMacInst;

   /* Maximum UEs that may be scheduled UL and DL */ 
   cfg->maxUePerUlSf = smCb.cfgCb.uehConfig.maxUePerUlSf; //uehDfltMaxUePerUlSf;
   cfg->maxUePerDlSf = smCb.cfgCb.uehConfig.maxUePerDlSf; //uehDfltMaxUePerDlSf;

#ifdef RGR_V1
   cfg->maxMsg3PerUlSf = smCb.cfgCb.uehConfig.maxMsg3PerUlSf; //uehMaxMsg3PerUlSf;
#endif
   /* Maximum uplink & downlink bandwidth */
   cfg->maxUlBwPerUe = smCb.cfgCb.uehConfig.maxUlBwPerUe; //uehDfltMaxUlBwPerUe;
   cfg->maxDlBwPerUe = smCb.cfgCb.uehConfig.maxDlBwPerUe; //uehDfltMaxDlBwPerUe;

   /* Maximum DL retransmissions B/W in RBs */
   cfg->maxDlRetxBw = smCb.cfgCb.uehConfig.maxDlRetxBw; //uehDfltMaxDlRetxBw;

   /* Number UE's per TTI for new transmissions */
   cfg->maxDlUeNewTxPerTti = smCb.cfgCb.uehConfig.maxDlUeNewTxPerTti; //uehDfltMaxDlUeNwTxPerTti;
   cfg->maxUlUeNewTxPerTti = smCb.cfgCb.uehConfig.maxUlUeNewTxPerTti; //uehDfltMaxUlUeNwTxPerTti;

   /* Number of Transmission antenna ports */
   cfg->numTxAntPorts =  smCb.cfgCb.uehConfig.numOfTxAnt; //uehDfltNumOfTxAnt; /* Moved from Operator config to Module Config*/

   /* Cyclic prefix */
   cfg->isCpUlExtend = smCb.cfgCb.uehConfig.isCyclicPrefixUlExt; //uehDfltIsCycPrefULExt;
   cfg->isCpDlExtend = 0; //smCb.cfgCb.uehConfig.isCyclicPrefixDlExt; //uehDfltIsCycPrefDLExt;

   /* Cell Midulation Scheme */
   cfg->cellModSchm = smCb.cfgCb.uehConfig.cellModSchm; //uehDfltCellModSchm;

   /* Cell Uplink transmission power */
   cfg->pMax = uehCb.uehCfgCb.cellCfg.pMax; /*uehDfltPMax */
   if (SCHEDULINGALGORITHM_ROUNDROBIN EQ uehCb.uehCfgCb.schCfg.schedulingAlgorithm)
   {
     /* DL scheduler to be used */
     cfg->dlSchdType = uehCb.uehCfgCb.schCfg.schedulingAlgorithm; //uehDfltDlSchdType;

     /* DL PFS Scheduler */
     uehFillRgrCellDlPfs(&cfg->dlSchInfo.cellDlPfs);

     /* UL scheduler to be used */
     cfg->ulSchdType = uehCb.uehCfgCb.schCfg.schedulingAlgorithm; //uehDfltUlSchdType;

     /* UL PFS Scheduler */
     uehFillRgrCellUlPfs(&cfg->ulSchInfo.cellUlPfs);
   }
   else
   {
     logError ("%s","Only Round Robin scheduler is supported, please set schedulerType as round robin");
     UEH_FREE(cfgReq, sizeof(RgrCfgReqInfo));
     RETVALUE(RFAILED);
   }

   /* DLFS scheduler to use */
   cfg->dlfsSchdType = SCHEDULINGALGORITHM_BASICSC1;//uehCb.uehCfgCb.schCfg.schedulingAlgorithm; //uehDfltDLFSSchdTyp;
   /* HARQ related configuration */
   uehFillRgrDlHqCfg(&cfg->dlHqCfg);

   /* Range of RNTIs to be managed by MAC */
   uehFillRgrRntiCfg(&cfg->macRnti);

   /* CFI for PDCCH */
   uehFillRgrCfiCfg(&cfg->cfiCfg);

   /* Target UL CQI */
   uehFillRgrUlTrgCqiCfg(&cfg->trgUlCqi);

   /* Coding rate for common DL channels */
   uehFillRgrDlCmnCodeRateCfg(&cfg->dlCmnCodeRate);

   /* UL sub-band information */
   if(ROK NOTEQ uehFillRgrPuschSubBandCfg(&cfg->puschSubBand))
   {
      UEH_ERROR_MSG_FILLING_FAILED("PUSCH Sub Band Config", CELL, cellId);
      UEH_FREE(cfgReq, sizeof(RgrCfgReqInfo));
      RETVALUE(RFAILED);
   }

   /* Coding rate for common UL channels */
   uehFillRgrUlCmnCodeRateCfg(&cfg->ulCmnCodeRate);

   /* Configuration for DLFS scheduler */
   uehFillRgrDlfsCfg(&cfg->dlfsCfg);

   /* Bandwidth configuration */ 
   uehFillRgrBwCfg(&cfg->bwCfg);

   /* PHICH configuration information */
   uehFillRgrPhichCfg(&cfg->phichCfg);

   /* PUCCH configuration information */
   uehFillRgrPucchCfg(&cfg->pucchCfg);

   /* SRS configuration information */ 
   uehFillRgrSrsCfg(&cfg->srsCfg);

   /* RACH configuration */
   if(ROK NOTEQ uehFillRgrRachCfg(&cfg->rachCfg))
   {
      UEH_ERROR_MSG_FILLING_FAILED("RACH configuration" , CELL, cellId);
      UEH_FREE(cfgReq, sizeof(RgrCfgReqInfo));
      RETVALUE(RFAILED);
   }

   /* SI configuration */
   uehFillRgrSiCfg(&cfg->siCfg);

   /* Cell-specific power configuration */ 
   uehFillRgrUlPwrCfg(&cfg->pwrCfg);

   /* Cell specific hopping configuration  */
   uehFillRgrPuschCfg(&cfg->puschCfg);

   /* PDCCH Order Preamble Id's */
   uehFillRgrPreambleSetCfg(&cfg->macPreambleSet);

   /* Configuration for common logical channels */
   if(ROK NOTEQ uehFillRgrCmnLcCfg(cfg))
   {
      UEH_ERROR_MSG_FILLING_FAILED("CCCH Information", CELL, cellId);
      UEH_FREE(cfgReq, sizeof(RgrCfgReqInfo));
      RETVALUE(RFAILED);
   }

#ifdef LTE_TDD
   /* UL-DL configuration index */
   cfg->ulDlCfgIdx = uehUlDlCfgIdx;
   /* Special Subframe configuration index */
   cfg->spclSfCfgIdx = uehSpclSfCfgIdx;
   /* PRACH information */
   if(ROK NOTEQ uehFillRgrTddPrachRscInfo(&cfg->prachRscInfo))
   {
      UEH_ERROR_MSG_FILLING_FAILED("PRACH Resource Information", CELL, cellId);
      UEH_FREE(cfgReq, sizeof(RgrCfgReqInfo));
      RETVALUE(RFAILED);
   }
#endif

#ifdef RGR_RRM_TICK
   cfg->rrmTtiIndPrd = 1;
#endif

   /* Fill appropriate TransId details 
    * here since lcId & crnti doesnt hold they are populated
    * as zero */
   UEH_SET_RGR_TRANS(transId.trans, RGR_CELL_CFG, UEH_VAL_ZERO, UEH_VAL_ZERO);
   RETVALUE( uehSndMsgOut (ENTRG, EVENTTYPE_SCHCELLCONFIG, NULLP, &transId, cfgReq));
   

   
} /* uehHdlRgrCellCfgReq */


#ifdef SI_NEW
/*
*
*       Fun:   uehSndRgrSiCfgReq
*
*       Desc:  Send Rgr Si Config Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndRgrSiCfgReq (
    U16            cellId
  )
{
  U8           sibIdx = 1;
  U8           siIdx = 1;
  UehCellCb    *cellCb  = uehGetCellCb();

  uehSndRgrSiMibCfgReq(cellId);
  uehSndRgrSiSib1CfgReq(cellId);
  uehSndRgrSiSib2CfgReq(siIdx, cellId);

  for(sibIdx = 1; sibIdx < cellCb->numOfSis; sibIdx++)
  {
    siIdx++;
    uehSndRgrSiMsg(sibIdx, siIdx, cellId);
  }
  RETVALUE(ROK);
} /* uehSndRgrSiCfgReq */

/*
*
*       Fun:   uehSndRgrSiMsg
*
*       Desc:  Send Rgr Si Config Request
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_cell.c
*
*/
PUBLIC S16 uehSndRgrSiMsg (
    U8      sibIdx,
    U8      siIdx,
    U16     cellId
    )
{
  switch(sibIdx)
  {
    case UEH_MOD_CFG_SIB3_IDX:
      uehSndRgrSiSib3CfgReq(siIdx,cellId);
      break;

    case UEH_MOD_CFG_SIB4_IDX:
      uehSndRgrSiSib4CfgReq(siIdx,cellId);
      break;

#ifdef PWAV_SIB5
    case UEH_MOD_CFG_SIB5_IDX:
     uehSndRgrSiSib5CfgReq(siIdx,cellId);
#endif
      break;

    default:
      logInfo("Invalid System information index: %u ", sibIdx);
      break;
  } /*uehSndRgrSiMsg*/

  RETVALUE(ROK);
}

/*
*
*       Fun:   uehSndRgrSiMibCfgReq
*
*       Desc:  Send Rgr Si Mib Config Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndRgrSiMibCfgReq( 
    U16            cellId
  )
{
  Pst             *sipst = NULLP;
  RgrSiCfgReqInfo *siCfgReq;
  NhuDatReqSdus   *nhuDatReqEvntSdu = NULLP;
  RgrCfgTransId   sitransId;
  S16 ret = RFAILED;  
#ifdef RRM_PRL_FLAG 
Buff *buff = NULL;
#endif
  sipst = &uehCb.rgrSap[0]->pst;
  sipst->event = EVTRGRSICFGREQ;

  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  UEH_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
  if(NULLP EQ siCfgReq) 
  {
    BS_ERROR_MEM_ALLOC_FAILED(sizeof(RgrSiCfgReqInfo), "RgrSiCfgReqInfo");
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }


  /* set Cfg type as 5(base)+offset. where offset is based on
   * type of SI being configured. base is 5 since 1 to 4 values
   * are already in use by RGR CFG for cell, ue, lch, lcg.
   */
  UEH_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_MIB), UEH_VAL_ZERO, UEH_VAL_ZERO);

  siCfgReq->cellId = cellId;
  siCfgReq->cfgType = RGR_SI_CFG_TYPE_MIB;

  /* Build MIB pdu */
  if(ROK NOTEQ uehFillSysInfMsg(nhuDatReqEvntSdu, NHU_MSG_DLBCCHBCH, UEH_SYS_MIB))
  {
    UEH_ERROR_MSG_FILLING_FAILED("MIB PDU", CELL, cellId);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  if(ROK NOTEQ uehEncSibPdu(nhuDatReqEvntSdu, &siCfgReq->pdu))
  {
    UEH_ERROR_MSG_ENCODING_FAILED(UEH_SYS_MIB);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
   ret = uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq);
#ifdef RRM_PRL_FLAG 
   if (islogApiEnabled())
   { 
    buff = GET_UEHCB_PRNT_LIB_BUFF();
    printNhUiNhuDatReq(nhuDatReqEvntSdu, buff);
    logAPI("RRM - RRC :MIB PDU %s", buff->startAddr);
    buff->numBytesWritten = 0;
   }
   if(ret != ROK)
   {
    RETVALUE(RFAILED);
   }

#else
  if(ROK NOTEQ uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq))
  {
    RETVALUE(RFAILED);
  }
#endif
  /* This memory is not needed now can be freed */
  UEH_FREEEVNT(nhuDatReqEvntSdu);
  RETVALUE(ret);
}


/*
*
*       Fun:   uehSndRgrSiSib1CfgReq
*
*       Desc:  Send Rgr Si Sib1 Config Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndRgrSiSib1CfgReq( 
    U16            cellId
  )
{
  Pst             *sipst = NULLP;
  RgrSiCfgReqInfo *siCfgReq;
  NhuDatReqSdus   *nhuDatReqEvntSdu = NULLP;
  RgrCfgTransId   sitransId;
  S16 ret = RFAILED;
#ifdef RRM_PRL_FLAG 
 Buff *buff = NULL;
#endif

  sipst = &uehCb.rgrSap[0]->pst;
  sipst->event = EVTRGRSICFGREQ;

  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  UEH_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
  if( NULL EQ siCfgReq)
  {
    BS_ERROR_MEM_ALLOC_FAILED(sizeof(RgrSiCfgReqInfo), "RgrSiCfgReqInfo for SIB1");
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
   /* set Cfg type as 5(base)+offset. where offset is based on
   * type of SI being configured. base is 5 since 1 to 4 values
   * are already in use by RGR CFG for cell, ue, lch, lcg. */
  UEH_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SIB1), UEH_VAL_ZERO, UEH_VAL_ZERO);
  siCfgReq->cellId = cellId;
  siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1;

  /* Build SIB1 pdu */
  if(ROK NOTEQ uehFillSysInfMsg(nhuDatReqEvntSdu, NHU_MSG_DLBCCHDLSCH, UEH_SYS_SIB1))
  {
    UEH_ERROR_MSG_FILLING_FAILED("SIB1 PDU" , CELL, cellId);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  if(ROK NOTEQ uehEncSibPdu(nhuDatReqEvntSdu, &siCfgReq->pdu))
  {
    UEH_ERROR_MSG_ENCODING_FAILED(UEH_SYS_SIB1);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
   ret = uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq);
#ifdef RRM_PRL_FLAG 
  if(islogApiEnabled())
  {  
   buff = GET_UEHCB_PRNT_LIB_BUFF();
   printNhUiNhuDatReq(nhuDatReqEvntSdu, buff);
   logAPI("RRM - RRC :SIB1 PDU %s", buff->startAddr);
   buff->numBytesWritten = 0;
  }

  if(ROK NOTEQ ret)
  {
    RETVALUE(RFAILED);
  }
#else
  if(ROK NOTEQ uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq))
  {
   RETVALUE(RFAILED);
  }
  
   

#endif
  /* Memory allocated can be freed now */
  UEH_FREEEVNT(nhuDatReqEvntSdu);
  RETVALUE(ret);
}


/*
*
*       Fun:   uehSndRgrSiSib2CfgReq
*
*       Desc:  Send Rgr Si Sib2 Config Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndRgrSiSib2CfgReq( 
    U8             siIdx,
    U16            cellId
  )
{
  Pst             *sipst = NULLP;
  RgrSiCfgReqInfo *siCfgReq;
  NhuDatReqSdus   *nhuDatReqEvntSdu = NULLP;
  RgrCfgTransId   sitransId;
  S16 ret = RFAILED;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  sipst = &uehCb.rgrSap[0]->pst;
  sipst->event = EVTRGRSICFGREQ;
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

  /* Configure SIB2 */
  /* Allocate RGR Cfg struct */
  UEH_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
  if( NULL EQ siCfgReq)
  {
    BS_ERROR_MEM_ALLOC_FAILED(sizeof(RgrSiCfgReqInfo), "RgrSiCfgReqInfo for SIB2");
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }

  /* set Cfg type as 5(base)+offset. where offset is based on
   * type of SI being configured. base is 5 since 1 to 4 values
   * are already in use by RGR CFG for cell, ue, lch, lcg. */
  UEH_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), UEH_VAL_ZERO, UEH_VAL_ZERO);
  siCfgReq->cellId = cellId;
  siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
  siCfgReq->siId = siIdx;

  /* Build SIB2 pdu */
  if(ROK NOTEQ uehFillSysInfMsg(nhuDatReqEvntSdu, NHU_MSG_DLBCCHDLSCH, UEH_SYS_SIB2))
  {
    UEH_ERROR_MSG_FILLING_FAILED("SIB2 PDU", CELL, cellId);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  if(ROK NOTEQ uehEncSibPdu(nhuDatReqEvntSdu, &siCfgReq->pdu))
  {
    UEH_ERROR_MSG_ENCODING_FAILED(UEH_SYS_SIB2);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
   ret = uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq);
#ifdef RRM_PRL_FLAG 
   
  if(islogApiEnabled())
   {     
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuDatReq(nhuDatReqEvntSdu, buff);
     logAPI("RRM - RRC :SIB2 PDU %s", buff->startAddr);
     buff->numBytesWritten = 0; 
   }
  if(ROK NOTEQ ret)
  {
    RETVALUE(RFAILED);
  }

#else 
  if(ROK NOTEQ uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq))
  {
    RETVALUE(RFAILED);
  }
  
#endif
  /* Free Allocated memory here not needed now */
  UEH_FREEEVNT(nhuDatReqEvntSdu);
  RETVALUE(ret); 

}

/*
*
*       Fun:   uehSndRgrSiSib3CfgReq
*
*       Desc:  Send Rgr Si Sib3 Config Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndRgrSiSib3CfgReq( 
	U8         	   siCfgIdx,
    U16            cellId
  )
{
  Pst             *sipst = NULLP;
  RgrSiCfgReqInfo *siCfgReq = NULLP;
  NhuDatReqSdus   *nhuDatReqEvntSdu = NULLP;
  RgrCfgTransId   sitransId;
 S16 ret = RFAILED;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  sipst = &uehCb.rgrSap[0]->pst;
  sipst->event = EVTRGRSICFGREQ;

  
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  /* Configure SIB3 */
  /* Allocate RGR Cfg struct */
  UEH_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
  if( NULL EQ siCfgReq)
  {
    BS_ERROR_MEM_ALLOC_FAILED(sizeof(RgrSiCfgReqInfo), "RgrSiCfgReqInfo for SIB3");
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }

   /* set Cfg type as 5(base)+offset. where offset is based on
   * type of SI being configured. base is 5 since 1 to 4 values
   * are already in use by RGR CFG for cell, ue, lch, lcg. */
  UEH_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), UEH_VAL_ZERO, UEH_VAL_ZERO);
  siCfgReq->cellId  = cellId;
  siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
  siCfgReq->siId    = siCfgIdx;

  /* Build SIB3 pdu */
  if(ROK NOTEQ uehFillSysInfMsg(nhuDatReqEvntSdu, NHU_MSG_DLBCCHDLSCH, UEH_SYS_SIB3))
  {
    UEH_ERROR_MSG_FILLING_FAILED("SIB3 PDU", CELL, cellId);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  if(ROK NOTEQ uehEncSibPdu(nhuDatReqEvntSdu, &siCfgReq->pdu))
  {
    UEH_ERROR_MSG_ENCODING_FAILED(UEH_SYS_SIB3);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  
ret = uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq);
#ifdef RRM_PRL_FLAG  

  if(islogApiEnabled())
   {   
   buff = GET_UEHCB_PRNT_LIB_BUFF();
   printNhUiNhuDatReq(nhuDatReqEvntSdu, buff);
   logAPI("RRM - RRC :SIB3 PDU %s", buff->startAddr);

     buff->numBytesWritten = 0;
   }
   if(ROK NOTEQ ret)
  {
    RETVALUE(RFAILED);
  }

 #else
  if(ROK NOTEQ uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq))
  {
    RETVALUE(RFAILED);
  }
  
#endif
  /* Free Allocated memory here not needed */
 
  UEH_FREEEVNT(nhuDatReqEvntSdu);

  RETVALUE(ret); 
}


/*
*
*       Fun:   uehSndRgrSiSib4CfgReq
*
*       Desc:  Send Rgr Si Sib4 Config Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndRgrSiSib4CfgReq( 
	U8             siCfgIdx,
    U16            cellId
  )
{
  Pst             *sipst = NULLP;
  RgrSiCfgReqInfo *siCfgReq = NULLP;
  NhuDatReqSdus   *nhuDatReqEvntSdu = NULLP;
  RgrCfgTransId   sitransId;
  S16 ret = RFAILED;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  sipst = &uehCb.rgrSap[0]->pst;
  sipst->event = EVTRGRSICFGREQ;
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  UEH_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
  if( NULL EQ siCfgReq)
  {
    BS_ERROR_MEM_ALLOC_FAILED(sizeof(RgrSiCfgReqInfo), "RgrSiCfgReqInfo for SIB4");
    UEH_FREEEVNT(nhuDatReqEvntSdu); 
    RETVALUE(RFAILED);
  }
    /* set Cfg type as 5(base)+offset. where offset is based on
   * type of SI being configured. base is 5 since 1 to 4 values
   * are already in use by RGR CFG for cell, ue, lch, lcg. */
  UEH_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), UEH_VAL_ZERO, UEH_VAL_ZERO);
  siCfgReq->cellId = cellId;
  siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
  siCfgReq->siId = siCfgIdx;

  /* Build SIB4 pdu */
  if(ROK NOTEQ uehFillSysInfMsg(nhuDatReqEvntSdu, NHU_MSG_DLBCCHDLSCH, UEH_SYS_SIB4))
  {
    UEH_ERROR_MSG_FILLING_FAILED("SIB4 PDU",CELL,cellId);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  if(ROK NOTEQ uehEncSibPdu(nhuDatReqEvntSdu, &siCfgReq->pdu))
  {
    UEH_ERROR_MSG_ENCODING_FAILED(UEH_SYS_SIB4);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  
  
  ret = uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq);
  #ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   {    
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuDatReq(nhuDatReqEvntSdu, buff);
     logAPI("RRM - RRC :SIB4 PDU %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
  if(ROK NOTEQ ret)
  {
    RETVALUE(RFAILED);
  }
  

  #else
  if(ROK NOTEQ uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq))
  {
   RETVALUE(RFAILED);
  }
  
#endif
  /* Free allocated memory here, not needed */
  UEH_FREEEVNT(nhuDatReqEvntSdu);
  
  RETVALUE(ret);
  

  
}


#ifdef PWAV_SIB5
/*
*
*       Fun:   uehSndRgrSiSib5CfgReq
*
*       Desc:  Send Rgr Si Sib5 Config Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_cell.c 
*
*/
PUBLIC S16 uehSndRgrSiSib5CfgReq( 
	U8             siCfgIdx,
    U16            cellId
  )
{
  Pst             *sipst = NULLP;
  RgrSiCfgReqInfo *siCfgReq = NULLP;
  NhuDatReqSdus   *nhuDatReqEvntSdu = NULLP;
  RgrCfgTransId   sitransId;
  S16 ret = RFAILED;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  sipst = &uehCb.rgrSap[0]->pst;
  sipst->event = EVTRGRSICFGREQ;
  
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));  
  UEH_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
  if( NULL EQ siCfgReq)
  {
    BS_ERROR_MEM_ALLOC_FAILED(sizeof(RgrSiCfgReqInfo), "RgrSiCfgReqInfo for SIB5");
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
    /* set Cfg type as 5(base)+offset. where offset is based on
   * type of SI being configured. base is 5 since 1 to 4 values
   * are already in use by RGR CFG for cell, ue, lch, lcg. */
  UEH_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), UEH_VAL_ZERO, UEH_VAL_ZERO);
  siCfgReq->cellId = cellId;
  siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
  siCfgReq->siId = siCfgIdx;


  if(ROK NOTEQ uehFillSysInfMsg(nhuDatReqEvntSdu, NHU_MSG_DLBCCHDLSCH, UEH_SYS_SIB5))
  {
    UEH_ERROR_MSG_FILLING_FAILED("SIB5 PDU",CELL,cellId);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  if(ROK NOTEQ uehEncSibPdu(nhuDatReqEvntSdu, &siCfgReq->pdu))
  {
    UEH_ERROR_MSG_ENCODING_FAILED(UEH_SYS_SIB5);
    UEH_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  
  ret = uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq);
  #ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   {   
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuDatReq(nhuDatReqEvntSdu, buff);
     logAPI("RRM - RRC :SIB5 PDU %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
   if(ROK NOTEQ ret)
  {
    RETVALUE(RFAILED);
  }

 #else
  if(ROK NOTEQ uehSndMsgOut (ENTRG, EVENTTYPE_SCHSICONFIG, NULLP, &sitransId, siCfgReq))
  {
   RETVALUE(RFAILED);
  }
  
#endif
  /* Free allocated memory here not needed */
  UEH_FREEEVNT(nhuDatReqEvntSdu);

  RETVALUE(ret);
  
}
#endif /* PWAV_SIB5 */
#endif /* SI_NEW */

/**********************************************************************

        Revision history:

**********************************************************************/


