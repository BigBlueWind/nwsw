/**
 * @file    ueh_sib.c
 * @brief This file contains the UEH System Information Broadcast module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh_eut.h"
#include "ueh.h" 
#include "lueh.h"
#include "lueh.x" 
#include "ueh_eut_x.h"
#include "s1ap_cmn_proc.h"
#include "ueh_sib.h"
#include "log.h"
                                           /* From 36.101 spec table 5.7.3-1 EUTRA Channel NUmbers */
                               /*EutraOprBand*/ /*FreqDlLow*/  /*FreqOffsetDl*/  /*FreqRangeDl*/   /*FreqUlLow*/  /*FreqOffsetUl*/  /*FreqRangeUl*/
 UehFreqBand uehFreqBand [33]={{     1,             2110,            0,               599,              1920,          18000,           18599},
                               {     2,             1930,            600,             1199,             1850,          18600,           19199},
                               {     3,             1805,            1200,            1949,             1710,          19200,           19949},
                               {     4,             2110,            1950,            2399,             710,           19950,           20399},
                               {     5,             869,             2400,            2649,             824,           20400,           20649}, 
                               {     6,             875,             2650,            2749,             830,           20650,           20749},
                               {     7,             2620,            2750,            3449,             2500,          20750,           21449}, 
                               {     8,             925,             3450,            3799,             880,           21450,           21799},
                               {     9,             1844,            3800,            4149,             1749,          21800,           22149},
                               {     10,            2110,            4150,            4749,             1710,          22150,           22749},
                               {     11,            1475,            4750,            4999,             1427,          22750,           22999},
                               {     12,            728,             5000,            5179,             698,           23000,           23179},
                               {     13,            746,             5180,            5279,             777,           23180,           23279},
                               {     14,            758,             5280,            5379,             788,           23280,           23379},  
                               {     17,            734,             5730,            5849,             704,           23730,           23849},
                               {     18,            860,             5850,            5999,             815,           23850,           23999},
                               {     19,            875,             6000,            6149,             830,           24000,           24149},
                               {     20,            791,             6150,            6449,             832,           24150,           24449},
                               {     21,            1495.9,          6450,            6599,             1447.9,        24450,           24599},
                               {     22,            3510,            6600,            7399,             3410,          24600,           25399},
                               {     23,            2180,            7500,            7699,             2000,          25500,           25699},
                               {     24,            1525,            7700,            8039,             1626.5,        25700,           26039},
                               {     25,            1930,            8040,            8689,             1850,          26040,           26689},
                               {     33,            1900,            36000,           36199,            1900,          36000,           36199},
                               {     34,            2010,            36200,           36349,            2010,          36200,           36349},
                               {     35,            1850,            36350,           36949,            1850,          36350,           36949},
                               {     36,            1930,            36950,           37549,            1930,          36950,           37549},
                               {     37,            1910,            37550,           37749,            1910,          37550,           37749},
                               {     38,            2570,            37750,           38249,            2570,          37750,           38249},
                               {     39,            1880,            38250,           38649,            1880,          38250,           38649},
                               {     40,            2300,            38650,           39649,            2300,          38650,           39649}};

/* Contains the Number of Periodicity in terms of SFN */
U16     schInfpPeriod[SCHEDULING_INFO_PERIOD_ARRAY_LEN] = { 8, 16, 32, 64, 128, 256, 512};
U16     FreqBandindex = 0;
U16     CalUlEarFcn = 0;

EnumNhuMasterInfoBlockdl_Bandwidth uehGetDlBwInDrbEnumForOamVal(
     DlBandwidth dlBw
    )
{
  switch(dlBw)
  {
    case DLBANDWIDTH_1DOT4MHZ:
      return NhuMasterInfoBlockdl_Bandwidthn6Enum;
    case DLBANDWIDTH_3MHZ:
      return NhuMasterInfoBlockdl_Bandwidthn15Enum;
    case DLBANDWIDTH_5MHZ:
      return NhuMasterInfoBlockdl_Bandwidthn25Enum;
    case DLBANDWIDTH_10MHZ:
      return NhuMasterInfoBlockdl_Bandwidthn50Enum;
    case DLBANDWIDTH_15MHZ:
      return NhuMasterInfoBlockdl_Bandwidthn75Enum;
    case DLBANDWIDTH_20MHZ:
      return NhuMasterInfoBlockdl_Bandwidthn100Enum;
  }
  return NhuMasterInfoBlockdl_Bandwidthn50Enum;
}

/*
*
*       Fun:   uehFillMibMsg
*
*       Desc:  Fills BCCH_BCH  message (DL BCCH BCH)
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
PUBLIC S16 uehFillMibMsg(
  Ptr                *evnt,
  NhuMasterInfoBlock *mib,
  NhuTimingInfo      *timingInfo
  )
{
  logFunc("%s", "uehFillMibMsg");

  UEH_FILL_TKN_UINT(mib->pres, PRSNT_NODEF);

  /*Fill DL-Bandwidth */
  UEH_FILL_TKN_UINT(mib->dl_Bandwidth, uehGetDlBwInDrbEnumForOamVal(uehCb.uehCfgCb.cellCfg.dlBandwidth) );

  /* PHICH-Config */
  UEH_FILL_TKN_UINT(mib->phich_Config.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(mib->phich_Config.phich_Duration, uehCb.uehCfgCb.phyCfg.phichDuration); /* uehDfltPhichDur */
  UEH_FILL_TKN_UINT(mib->phich_Config.phich_Resource, uehCb.uehCfgCb.phyCfg.phichResource);

  /* SFN - SystemFrameNumber */
  UEH_GET_MEM(evnt, sizeof(U8), &(mib->systemFrameNumber.val));
  mib->systemFrameNumber.pres = PRSNT_NODEF;
  mib->systemFrameNumber.len  = UEH_BYTE_LEN;

  /* sfn = nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.sfn;
   * sfn = ((sfn >> 2) & 0xFF);  
   * Note: RRC expecting 10 Bits for SFN
   * RRC internally taking care of encoding only MSB 8 bits of SFN
   */
  UEH_GET_SFN_MIB(mib->systemFrameNumber.val[0], timingInfo->sfn);

  /* Spare, 10 Bits it is not optional */
  UEH_GET_MEM(evnt, (2 * sizeof(U8)), &(mib->spare.val));
  mib->spare.pres = PRSNT_NODEF;
  mib->spare.len  = UEH_MIB_SPARE_LEN;

  RETVALUE(ROK);
} /* End of uehFillMibMsg */


/**
 * @details Fill the SI Scheduling information.
 * Group SIB's having same periodicity into one SI_Map Element,
 * Repeat the SIB's as per the given periodicity,
 * SIB2 is special, It must go with first SI, meaning the periodicity of SIB2 is
 * same as the periodicity of First SI_MAP
 * TODO Add support for other SIB's at a later Date
 **/
PUBLIC S16 uehFillSib1SchdInfoLst(
  Ptr                     *evnt,
  NhuSysInfoBlockTyp1     *sibTyp1
  )
{
  U8                           siIdx = 0;
  NhuSchedulingInfo            *schedInf = NULLP;
  UehCellCb                    *cellCb = NULLP;

  cellCb = uehGetCellCb();

  /* assigning no of sib count information */
  UEH_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, cellCb->numOfSis);
  UEH_GET_MEM(evnt, (sizeof(NhuSchedulingInfo) * sibTyp1->schedulingInfoLst.noComp.val), &(sibTyp1->schedulingInfoLst.member));

  /* Filling SIB2 information */
  schedInf = &(sibTyp1->schedulingInfoLst.member[siIdx]);
  UEH_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
  /* Zero means only SIB2 by default */
  UEH_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp, UEH_VAL_ZERO);
  UEH_FILL_TKN_UINT(schedInf->si_Periodicity, cellCb->siSchInfo[siIdx].siPrdcty);

  for(siIdx = 1; siIdx < sibTyp1->schedulingInfoLst.noComp.val; siIdx++)
  {
    schedInf = &(sibTyp1->schedulingInfoLst.member[siIdx]);
    UEH_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(schedInf->si_Periodicity, cellCb->siSchInfo[siIdx].siPrdcty);

    UEH_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp, 1);
    UEH_GET_MEM(evnt, (sizeof(NhuSIB_Typ) * schedInf->sib_MappingInfo.noComp.val), &(schedInf->sib_MappingInfo.member));
    UEH_FILL_TKN_UINT(schedInf->sib_MappingInfo.member[0], cellCb->siSchInfo[siIdx].sibMapInfo);
  }
  RETVALUE(ROK);
}
/**
 * @details Fill the Cell Selection Info
 **/
void uehFillSib1CellSelectionInfo(
    NhuSysInfoBlockTyp1     *sibTyp1
  )
{
  U16                          dfltqRxlevminoffset;

  /* Q-RxLevMin */
  UEH_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.pres, PRSNT_NODEF);

  /*rxLevMin can have a value from -60 to -13 */
  UEH_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMin, uehCb.uehCfgCb.sib1Cfg.qRxLevMin);

  /*Fill rxLevMinOffset only if the value is not 0 which is a special value,
   * This parameter corresponds to Qrxlevminoffset in 36.304. Actual value
   * Qrxlevminoffset = qRxLevMinOffset * 2 [dB]. If set to '0'(not defined), the (default) value of
   * 0 [dB] for Qrxlevminoffset is applied. Affects the minimum required Rx level in the cell.*/
  if( SPECIAL_VALUE_SIB1_QRXLEVEL_MIN_OFFSET != uehCb.uehCfgCb.sib1Cfg.qRxLevMinOffset )
  {
    dfltqRxlevminoffset = (U16) uehCb.uehCfgCb.sib1Cfg.qRxLevMinOffset;
    UEH_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMinOffset, dfltqRxlevminoffset);
  }
}

PUBLIC S16 uehFillSib1CellAccRelInfo(
  Ptr                    *evnt,
  NhuSysInfoBlockTyp1    *sibTyp1
  )
{
  UINT8                        cellId[MAX_SIZE_CELLID];
  U16                          tac = 0;
  U32                          allocSize = 0;
  U8                           idx;
  U8                           plmnIdx;
  NhuPLMN_IdentityLst          *plmnLst = NULLP;

  UEH_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.pres, PRSNT_NODEF);

  /* plmn-IdentityList, Presently we support only one PLMN */
  plmnLst = &(sibTyp1->cellAccessRelatedInfo.plmn_IdentityLst);
  UEH_FILL_TKN_UINT(plmnLst->noComp, UEH_VAL_ONE);

  allocSize = ((plmnLst->noComp.val) * (sizeof(NhuPLMN_IdentityInfo)));
  UEH_GET_MEM(evnt, allocSize, &(plmnLst->member));

  for(idx = 0; idx < (plmnLst->noComp.val); idx++)
  {
     UEH_FILL_TKN_UINT(plmnLst->member[idx].pres, PRSNT_NODEF);
     UEH_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.pres, PRSNT_NODEF);

     /* Allocating 3 instances of MCC digit, and fill the digits */
     UEH_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mcc.noComp, UEH_MCC_SIZE);
     UEH_GET_MEM(evnt,(UEH_MCC_SIZE * (sizeof(NhuMCC_MNC_Digit))), &(plmnLst->member[idx].plmn_Identity.mcc.member));
     for(plmnIdx = 0; plmnIdx < UEH_MCC_SIZE; plmnIdx++)
     {       
       UEH_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mcc.member[plmnIdx], uehCb.uehPlmnId.plmnMCC [plmnIdx]);
     }

     /* Allocating 2 instances of MNC digit */
     /* PLMN Change*/
     UEH_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mnc.noComp, uehCb.uehPlmnId.numOfMNCDigits);
     UEH_GET_MEM(evnt, (uehCb.uehPlmnId.numOfMNCDigits * (sizeof(NhuMCC_MNC_Digit))), &(plmnLst->member[idx].plmn_Identity.mnc.member));
     for(plmnIdx = 0; plmnIdx < uehCb.uehPlmnId.numOfMNCDigits; plmnIdx++)
     {
       UEH_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mnc.member[plmnIdx], uehCb.uehPlmnId.plmnMNC[plmnIdx]);
     }

     /* cellReservedForOperatorUse, Cell reserved for Operator use */
     UEH_FILL_TKN_UINT(plmnLst->member[idx].cellReservedForOperatorUse, uehCb.uehCfgCb.cellCfg.cellReservedForOperatorUse);
  }

  /* TrackingAreaCode */
  sibTyp1->cellAccessRelatedInfo.trackingAreaCode.pres = PRSNT_NODEF;
  sibTyp1->cellAccessRelatedInfo.trackingAreaCode.len = UEH_TRAC_LEN;
  UEH_GET_MEM(evnt, sizeof(U16), &(sibTyp1->cellAccessRelatedInfo.trackingAreaCode.val));
  tac = SWAP16(uehCb.uehCfgCb.cellCfg.tac);
  cmMemcpy((U8*)(sibTyp1->cellAccessRelatedInfo.trackingAreaCode.val),(U8*)& tac, sizeof(U16));

  /* CellIdentity  Bitstring of 28 bits len
   * The final value of Cell Identity is constructed by taking 20MSB from
   * eNBConfig->eNBIdentity and 8 LSB from CellId
   */
  s1apGetCellIdFromEnbIdAncActCellId( cellId, smCb.cfgCb.s1apCfgInfo.enodebId, uehCb.uehCfgCb.cellCfg.cellId );
  sibTyp1->cellAccessRelatedInfo.cellIdentity.pres = PRSNT_NODEF;
  sibTyp1->cellAccessRelatedInfo.cellIdentity.len = UEH_CELL_ID_LEN;
  UEH_GET_MEM(evnt, sizeof(U32), &(sibTyp1->cellAccessRelatedInfo.cellIdentity.val));
  cmMemcpy((U8 *)(sibTyp1->cellAccessRelatedInfo.cellIdentity.val), (U8 *)&cellId, sizeof(U32));

  /* CellBarred, This parameter defines whether the cell is barred  */
  /* Cell barring*/
  UEH_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.cellBarred, uehCb.uehCfgCb.cellBarred );

  /* IntraFreqReselection, Used to control cell re-selection to intraFrequency cells,
   * When the highest ranked cell is barred or treated as barred by UE */
  UEH_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.intraFreqReselection, uehCb.uehCfgCb.sib1Cfg.intraFreqReselection);

  /* csg-Indication, TODO Presently we don't support in Rel 1.0.0 */
  UEH_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.csg_Ind, FALSE);
  
  RETVALUE(ROK);

}

/*
*
*       Fun:   uehFillCmnSib1
*
*       Desc:  Fills comman fields of System Info Block Type1
*
*       Ret:
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
PUBLIC S16 uehFillCmnSib1(
  NhuDatReqSdus          *nhuDatReqEvntSdu
  )
{
  NhuBCCH_DL_SCH_Msg     *bcchSchMsg = NULLP;

  bcchSchMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
  UEH_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFOBLOCKTYP1);

  RETVALUE(ROK);
}
/*
*
*       Fun:   uehFillSib1Msg
*
*       Desc:  Fills System Info Block Type1 message (DL BCCH DLSCH)
*
*       Ret:   
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
PUBLIC S16 uehFillSib1Msg(
    Ptr                 *evnt,
    NhuSysInfoBlockTyp1 *sib1
  )
{
   U16 FreqOffset = 0;

   logFunc("%s", "uehFillSib1Msg");

   UEH_FILL_TKN_UINT(sib1->pres, PRSNT_NODEF);

   /* cellAccessRelatedInfo */
   if(ROK NOTEQ uehFillSib1CellAccRelInfo( evnt, sib1 ))
   {
     UEH_ERROR_MSG_FILLING_FAILED("SIB1 CELL ACCESS RELATED INFO" , CELL, uehCb.uehCfgCb.cellCfg.cellId);
     RETVALUE(RFAILED);
   }

   /* Fill CellSelection Info */
   uehFillSib1CellSelectionInfo( sib1 );

   /* P-Max
    * The parameter P-Max is used to limit the UE's uplink transmission power on a carrier
    * frequency and is used to calculate the parameter Pcompensation defined in TS 36.304.
    * Corresponds to parameter PEMAX_H in TS 36.101.
    */
   if( SPECIAL_VALUE_SIB1_PMAX NOTEQ uehCb.uehCfgCb.cellCfg.pMax )
   {
     UEH_FILL_TKN_UINT(sib1->p_Max, uehCb.uehCfgCb.cellCfg.pMax);
   }


   for (FreqBandindex = 0;FreqBandindex < 25;FreqBandindex ++ )
   {
     if (((int)uehCb.uehCfgCb.cellCfg.dlEarfcn - uehFreqBand[FreqBandindex].FreqRangeDl) <= 0)
     {
       /* FreqBandIndicator */
       UEH_FILL_TKN_UINT(sib1->freqBandIndicator, uehFreqBand[FreqBandindex].EutraOprBand);
       FreqOffset = uehCb.uehCfgCb.cellCfg.dlEarfcn - uehFreqBand[FreqBandindex].FreqOffsetDl;
       CalUlEarFcn = uehFreqBand[FreqBandindex].FreqOffsetUl + FreqOffset;
       if ((uehCb.uehCfgCb.cellCfg.ulEarfcn != 0) && (uehCb.uehCfgCb.cellCfg.ulEarfcn != CalUlEarFcn))
       {
         logInfo("Warning : The operator allocated UL EARFCN = %d is not same as the default one i.e calculated value from the UehFreqBand table = %d",uehCb.uehCfgCb.cellCfg.ulEarfcn, CalUlEarFcn);
       }
       break;
     }
   }
   /* SchedulingInfoList */
   if(ROK NOTEQ uehFillSib1SchdInfoLst( evnt, sib1 ))
   {
     UEH_ERROR_MSG_FILLING_FAILED("SIB1 Scheduling Info",CELL,uehCb.uehCfgCb.cellCfg.cellId);
     RETVALUE(RFAILED);
   }

   /* SI WindowLength, System information window length in ms */
   UEH_FILL_TKN_UINT(sib1->si_WindowLength, uehCb.uehCfgCb.sib1Cfg.siWindowLength);

   /* SystemInfoValueTag */
   UEH_FILL_TKN_UINT(sib1->systemInfoValueTag, smCb.cfgCb.uehConfig.systemInfoValueTag);//uehDfltSystemInfoValueTag);

   /* nonCriticalExtension */
   UEH_FILL_TKN_UINT(sib1->nonCriticalExtn.pres, PRSNT_NODEF);

  RETVALUE(ROK);
} /* End of uehFillSib1Msg */


PUBLIC S16 uehFillSib2FreqInfo(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  U16 ulBandwidth;
  UEH_FILL_TKN_UINT(sib2->freqInfo.pres, PRSNT_NODEF);

  /* ARFCN for uplink E-UTRA carrier frequency. The number is as per the band configured for the cell
   * using the table defined in 36.101. If absent, the (default) value determined from the default TX-RX frequency
   * separation defined in TS 36.101 [42, table 5.7.3-1] applies. Valid range : 23180 - 23279
   */
  if(uehCb.uehCfgCb.cellCfg.ulEarfcn NOTEQ 0)
  {
    UEH_FILL_TKN_UINT(sib2->freqInfo.ul_CarrierFreq, uehCb.uehCfgCb.cellCfg.ulEarfcn);
  }
  else
  {
    sib2->freqInfo.ul_CarrierFreq.pres = NOTPRSNT;
    logInfo("The UL EARFCN Value is calculated = %d and not broadcasted in SIB2 message",CalUlEarFcn);
  }

  /* UL Channel bandwidth. If for FDD this parameter is set to NotDefined, 
   * the uplink bandwidth is considered equal to the downlink bandwidth
   * and corresponding IE is not broadcast in SIB2 in the cell
   */
  if (ULBANDWIDTH_NOTDEFINED EQ uehCb.uehCfgCb.cellCfg.ulBandwidth)
  {
    ulBandwidth = UEH_CONVERT_UL_MHZ_TO_RB_ENUM(uehCb.uehCfgCb.cellCfg.dlBandwidth);
    sib2->freqInfo.ul_Bandwidth.pres = NOTPRSNT;
  }
  else
  {
    ulBandwidth = UEH_CONVERT_UL_MHZ_TO_RB_ENUM(uehCb.uehCfgCb.cellCfg.ulBandwidth);
    UEH_FILL_TKN_UINT(sib2->freqInfo.ul_Bandwidth, ulBandwidth);
  }

    /* Additional spectrum emmision */
    UEH_FILL_TKN_UINT(sib2->freqInfo.additionalSpectrumEmission, uehCb.uehCfgCb.sib2Cfg.additionalSpectrumEmission );

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2TimersAndConst(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  /*-Fill ue Timers And Constants -*/
  UEH_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.pres, PRSNT_NODEF);

  /* RRC Connection Request timer , Value ms0 corresponds with 0 ms, ms50 corresponds with 50 ms and so on
   */
  UEH_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t300, uehCb.uehCfgCb.sib2Cfg.t300);

  /* RRC Connection Reestablishment Request timer */
  UEH_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t301, uehCb.uehCfgCb.sib2Cfg.t301);

  /* Physical layer out of sync indication timer, started upon receiving N310 consecutive out-of-sync
   * indications from lower layers. Upon expiry, the UE moves to IDLE state and starts RRC Re-establishment.
   */
  UEH_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t310, uehCb.uehCfgCb.sib2Cfg.t310);

  /* Maximum number of consecutive 'out-of-sync' indications received from Physical layer before
   * T310 timer is started. n1 corresponds with 1, n2 corresponds with 2 and so on
   */
  UEH_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n310, uehCb.uehCfgCb.sib2Cfg.n310);

  /* T311 timer that is started when RRC Connection re-establishment procedure is triggered, and when this
   * timer expires, the UE moves to IDLE state .
   */
  UEH_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t311, uehCb.uehCfgCb.sib2Cfg.t311);

  /* Maximum number of consecutive 'in-sync' indications received from physical layer.
   * n1 corresponds with 1, n2 corresponds with 2 and so on
   */
  UEH_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n311, uehCb.uehCfgCb.sib2Cfg.n311);

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2AcBarringInfo(
    Ptr                 *evnt,
    NhuSysInfoBlockTyp2 *sib2
  )
{
  UINT8 barrForSpecialAc;

  /* IF any of the barring info is present that means Barring IE is PRESENT */
  if((uehCb.uehCfgCb.cellCfg.acBarringEmergency)||(uehCb.uehCfgCb.cellCfg.acBarrMoSigEnabled)||(uehCb.uehCfgCb.cellCfg.acBarrMoDataEnabled))
  {
    UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.pres, PRSNT_NODEF);

    /* Access class barring for AC 10. FALSE means access allowed. */
    UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.ac_BarringForEmergency, uehCb.uehCfgCb.cellCfg.acBarringEmergency);

    /* Access barring factor for mobile originated Signalling. */
    if(uehCb.uehCfgCb.cellCfg.acBarrMoSigEnabled)
    {
      UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.ac_BarringForMO_Signalling.pres,PRSNT_NODEF);

      /* If the random number drawn by the UE is
       * lower than this value, access is allowed. Otherwise the access is barred.
       * The values are interpreted in the range [0,1): p00 = 0, p05 = 0.05, p10 = 0.10,..,p95 = 0.95.
       */
      UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringFactor, uehCb.uehCfgCb.cellCfg.acBarrMoSigFactor);

      /* Mean access barring time for mobile originated signalling, in seconds */
      UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringTime, uehCb.uehCfgCb.cellCfg.acBarrMoSigTime);

      /* Access class barring for AC 11-15 for mobile originated signaling.
       * The first/ leftmost bit is for AC 11, the second bit is for AC 12, and so on.
       */
      sib2->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringForSpecialAC.pres = PRSNT_NODEF;
      sib2->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringForSpecialAC.len = MAX_SIZE_CELLCONFIG_ACBARRMOSIGSPECIALAC;
      UEH_GET_MEM(evnt, sizeof(UINT8), &(sib2->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringForSpecialAC.val));

      barrForSpecialAc  = (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoSigSpecialAC[0]) ) << 7;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoSigSpecialAC[1]) ) << 6;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoSigSpecialAC[2]) ) << 5;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoSigSpecialAC[3]) ) << 4;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoSigSpecialAC[4]) ) << 3;

      *(sib2->ac_BarringInfo.ac_BarringForMO_Signalling.ac_BarringForSpecialAC.val)= barrForSpecialAc;
    }
    /* Access barring factor for mobile originated data calls. */
    if(uehCb.uehCfgCb.cellCfg.acBarrMoDataEnabled)
    {
      UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.ac_BarringForMO_Data.pres,PRSNT_NODEF);

      /* If the random number drawn by the UE is lower than this value, access is allowed.
       * Otherwise the access is barred. The values are interpreted in the range [0,1):
       * p00 = 0, p05 = 0.05, p10 = 0.10, ... ,p95 = 0.95.
       */
      UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringFactor, uehCb.uehCfgCb.cellCfg.acBarrMoDataFactor);

      /* Mean access barring time for mobile originated data calls, in seconds */
      UEH_FILL_TKN_UINT(sib2->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringTime, uehCb.uehCfgCb.cellCfg.acBarrMoDataTime);

      /* Access class barring for AC 11-15 for mobile originated data calls.
       * The first/ leftmost bit is for AC 11, the second bit is for AC 12, and so on.
       */
      sib2->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringForSpecialAC.pres = PRSNT_NODEF;
      sib2->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringForSpecialAC.len = MAX_SIZE_CELLCONFIG_ACBARRMODATASPECIALAC;
      UEH_GET_MEM(evnt, sizeof(UINT8), &(sib2->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringForSpecialAC.val));

      barrForSpecialAc  = (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoDataSpecialAC[0]) ) << 7;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoDataSpecialAC[1]) ) << 6;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoDataSpecialAC[2]) ) << 5;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoDataSpecialAC[3]) ) << 4;
      barrForSpecialAc |= (CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.acBarrMoDataSpecialAC[4]) ) << 3;

      *(sib2->ac_BarringInfo.ac_BarringForMO_Data.ac_BarringForSpecialAC.val)= barrForSpecialAc;
    }
  }

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2RachCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuRACH_ConfigCmn             *rachCfg = NULLP;
  NhuRACH_ConfigCmnpreambleInfo *preAmbInf = NULLP;

  rachCfg = &(sib2->radioResourceConfigCmn.rach_Config);
  UEH_FILL_TKN_UINT(rachCfg->pres, PRSNT_NODEF);

  /** PreAmbInf **/
  preAmbInf = &(rachCfg->preambleInfo);
  UEH_FILL_TKN_UINT(preAmbInf->pres, PRSNT_NODEF);
  /* Number of non-dedicated random access preambles.Value is an integer.
   * Value n4 corresponds to 4, n8 corresponds to 8 and so on.
   */
  UEH_FILL_TKN_UINT(preAmbInf->numberOfRA_Preambles, uehCb.uehCfgCb.sib2Cfg.numRaPreambles);
  /* Size of the random access preambles group A.Value is an integer.
   * Value n4 corresponds to 4, n8 corresponds to 8 and so on
   */
  if(uehCb.uehCfgCb.sib2Cfg.numRaPreambles NOTEQ uehCb.uehCfgCb.sib2Cfg.sizeRAPreamblesGroupA)
  {
    UEH_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.sizeOfRA_PreamblesGroupA, uehCb.uehCfgCb.sib2Cfg.sizeRAPreamblesGroupA);
    /* Threshold for preamble selection. Value in bits. Value b56 corresponds to 56 bits,
     * b144 corresponds to 144 bits and so on
     */
    UEH_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messageSizeGroupA, uehCb.uehCfgCb.sib2Cfg.messageSizeGroupA);
    /* Threshold for preamble selection. Value in dB. Value minus infinity corresponds to infinity.
     * Value dB0 corresponds to 0 dB, dB5 corresponds to 5 dB and so on
     */
    UEH_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messagePowerOffsetGroupB, uehCb.uehCfgCb.sib2Cfg.messagePowerOffsetGroupB);
  }
  else
  {
    UEH_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.pres, NOTPRSNT);
    logInfo("%s","ENB Presently not supporting the Preamble GroupB configuration. Preamble GroupA Configuration is omitted which is used to derive the Preamble GroupB configuration."); 
  }

  /** Power Ramping **/
  /* Power ramping factor in dB. Value dB0 corresponds to 0 dB, dB2 corresponds to 2 dB and so on*/
  UEH_FILL_TKN_UINT(rachCfg->powerRampingParams.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(rachCfg->powerRampingParams.powerRampingStep, uehCb.uehCfgCb.sib2Cfg.powerRampingStep);
  /* Initial preamble power in dBm. Value minus120dBm corresponds to -120 dBm,
   * minus118dBm corresponds to -118 dBm and so on.
   */
  UEH_FILL_TKN_UINT(rachCfg->powerRampingParams.preambleInitReceivedTgtPower, uehCb.uehCfgCb.sib2Cfg.preambleInitRcvdTgtPower);

  /** Supervision **/
  UEH_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.pres, PRSNT_NODEF);
  /* Maximum number of preamble transmission in integer. Value n3 corresponds to 3, n4
   * corresponds to 4 and so on
   */
  UEH_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.preambleTransMax, uehCb.uehCfgCb.sib2Cfg.preambleTransMax);
  /* Duration of the RA response window in subframes. Value sf2 corresponds to 2 subframes, sf3
   * corresponds to 3 subframes and so on
   */
  UEH_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.ra_RspWindowSize, uehCb.uehCfgCb.sib2Cfg.raRspWindowSize);
  /* Timer for MAC contention resolution in subframes. Value sf8 corresponds to 8 subframes,
   * sf16 corresponds to 16 subframes and so on
   */
  UEH_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.mac_ContentionResolutionTimer, uehCb.uehCfgCb.sib2Cfg.macContResolutionTimer);
  /* Maximum number of Msg3 HARQ transmissions, used for contention based random access. */
  UEH_FILL_TKN_UINT(rachCfg->maxHARQ_Msg3Tx, uehCb.uehCfgCb.sib2Cfg.maxHarqMsg3Tx);

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2BcchCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuBCCH_Config *bcchCfg = NULLP;

  bcchCfg = &(sib2->radioResourceConfigCmn.bcch_Config);
  /* Actual modification period, expressed in number of radio frames= modificationPeriodCoeff * defaultPagingCycle.
   * n2 corresponds to value 2, n4 corresponds to value 4, n8 corresponds to value 8 and n16 corresponds to value 16.
   */
  UEH_FILL_TKN_UINT(bcchCfg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(bcchCfg->modificationPeriodCoeff, uehCb.uehCfgCb.sib2Cfg.modificationPeriodCoeff);

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2PcchCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuPCCH_Config *pcchCfg = NULLP;
  pcchCfg = &(sib2->radioResourceConfigCmn.pcch_Config);

  /* Default paging cycle, used to derive the parameter 'T' as defined in 36.304 which in turn derives the
   * paging frame .Value rf32 corresponds to 32 radio frames, rf64 corresponds to 64 radio frames and so on
   */
  UEH_FILL_TKN_UINT(pcchCfg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(pcchCfg->defaultPagCycle, uehCb.uehCfgCb.sib2Cfg.defaultPagCycle);
  /* nB is used as one of parameters to derive the Paging Frame and Paging Occasion. Value in multiples of
   * defaultPagingCycle ('T'). A value of fourT corresponds to 4 * defaultPagingCycle, a value of twoT corresponds
   * to 2 * defaultPagingCycle and so on.
   */
  UEH_FILL_TKN_UINT(pcchCfg->nB, uehCb.uehCfgCb.sib2Cfg.nB);/*uehDfltPCCHCfgNB*/

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2PrachCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuPRACH_ConfigSIB *prachCfg = NULLP;
  prachCfg = &(sib2->radioResourceConfigCmn.prach_Config);

  UEH_FILL_TKN_UINT(prachCfg->pres, PRSNT_NODEF);
  /* This parameter defines the RACH_ROOT_SEQUENCE as defined in 36.211. It is used to find the set of
   * 64 prach sequences in the cell.  This is found by including first, in the order of increasing cyclic shift,
   * all the available cyclic shifts of a root Zadoff-Chu sequence with the logical index RACH_ROOT_SEQUENCE
   */
  UEH_FILL_TKN_UINT(prachCfg->rootSequenceIdx, uehCb.uehCfgCb.sib2Cfg.rootSequenceIndex);


  UEH_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.pres, PRSNT_NODEF);
  /* PRACH configuration index. This is used determine the allowed RA preamble
   * format and subframes for RA transmission.
   */
  UEH_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_ConfigIdx, uehCb.uehCfgCb.sib2Cfg.prachConfigIndex);
  /* High-speed-flag.TRUE corresponds to Restricted set and FALSE to
   * Unrestricted set
   */
  UEH_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.highSpeedFlag, uehCb.uehCfgCb.sib2Cfg.highSpeedFlag);
  /* Ncs configuration index as defined in 36.211, used to derive the cyclic shift for the
   * SC sequence for RA preamble
   */
  UEH_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.zeroCorrelationZoneConfig, uehCb.uehCfgCb.sib2Cfg.zeroCorrelationZoneConfig);

  /* PRACH FrequencyOffset is the first physical resource block allocated to the PRACH opportunity.
   * The value of this parameter must be so that prachFreqOffset <= (UL RBs - 6).
   */
  UEH_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_FreqOffset, uehCb.uehCfgCb.phyCfg.prachFreqOffset);

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2PdschCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuPDSCH_ConfigCmn *pdschCfg = NULLP;
  pdschCfg = &(sib2->radioResourceConfigCmn.pdsch_Config);

  UEH_FILL_TKN_UINT(pdschCfg->pres, PRSNT_NODEF);
  /* Reference-signal power, which provides the downlink reference-signal EPRE. The actual value in dBm. */
  UEH_FILL_TKN_UINT(pdschCfg->referenceSignalPower, uehCb.uehCfgCb.phyCfg.referenceSignalPower);
  /* Ratio of EPRE of PDSCH-only REs  to EPRE of PDSCH+RS REs */
  UEH_FILL_TKN_UINT(pdschCfg->p_b, uehCb.uehCfgCb.phyCfg.cellPhyPb);//uehDfltPDSCHCfgPB);

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2PuschCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuPUSCH_ConfigCmn *puschCfg = NULLP;
  puschCfg = &(sib2->radioResourceConfigCmn.pusch_Config);

  UEH_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pres, PRSNT_NODEF);
  /* Number of sub-bands for PUSCH frequency hopping  */
  UEH_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.n_SB, uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingSubBands);
  /* PUSCH frequency hopping mode */
  UEH_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.hoppingMode, uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingMode);
  /* PUSCH frequency hopping offset, expressed in terms of Resource Blocks (RBs) */
  UEH_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pusch_HoppingOffset, uehCb.uehCfgCb.sib2Cfg.puschFreqHoppingOffset);

  /* 64 QAM Enabled information */
  UEH_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.enable64QAM, uehCb.uehCfgCb.sib2Cfg.enable64QAM );/*uehDfltEnable64QAM */

  UEH_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.pres, PRSNT_NODEF);
  /* Group hopping enabled on uplink  */
  UEH_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupHoppingEnabled, uehCb.uehCfgCb.sib2Cfg.ulRsGroupHoppingEnabled);
  /* Group assignment for generating the PUSCH RS sequence-shift pattern .
   * This is used if Group hopping is enabled in uplink
   */
  UEH_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH, uehCb.uehCfgCb.sib2Cfg.ulRsGroupPusch);
  /* Sequence hopping enabled on UL */
  UEH_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled, uehCb.uehCfgCb.sib2Cfg.ulRsSeqHoppingEnabled);
  /* Cyclic shift for uplink Demodulating Reference signals */
  UEH_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.cyclicShift, uehCb.uehCfgCb.sib2Cfg.ulRsCyclicShiftPusch);

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2PucchCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuPUCCH_ConfigCmn *pucchCfg = NULLP;
  pucchCfg = &(sib2->radioResourceConfigCmn.pucch_Config);

  UEH_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
  /* PUCCH delta-shift as defined in 36.211, used for PUCCH formats 1, 1a and 1b.
   * Value ds1 corresponds to value 1 ds2 to 2 etc.
   */
  UEH_FILL_TKN_UINT(pucchCfg->deltaPUCCH_Shift, uehCb.uehCfgCb.sib2Cfg.deltaPucchShift);
  /* Offset used to determine the ACK/NACK resource block in PDCCH */
  UEH_FILL_TKN_UINT(pucchCfg->n1PUCCH_AN, uehCb.uehCfgCb.sib2Cfg.n1PucchAn);

  /* TODO Get these values from OAM */
  UEH_FILL_TKN_UINT(pucchCfg->nRB_CQI, uehCb.uehCfgCb.sib2Cfg.nRbCqi); /*uehDfltPUCCHnRBCQI */
  UEH_FILL_TKN_UINT(pucchCfg->nCS_AN, uehCb.uehCfgCb.sib2Cfg.nCsAn); /* uehDfltPUCCHnCSAN */

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2UpLinkPwrCtrlCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuUlnkPowerCntrlCmn         *ulPwrCntrl = NULLP;
  
    /* Uplink Power Control Config */
  ulPwrCntrl = &(sib2->radioResourceConfigCmn.uplinkPowerCntrl);
  UEH_FILL_TKN_UINT(ulPwrCntrl->pres, PRSNT_NODEF);

  /* Corresponds to parameter Po_nominal_pusch as defined in 36.213, used in UE uplink power control.
   * This field is applicable for non-persistent scheduling, only. Unit is dBm and actual rangle is -126..24 dBm,
   * i.e. parameter value 0 corresponds to -126, value 1 corresponds to -125dBm, and so on.*/
  UEH_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUSCH, uehCb.uehCfgCb.sib2Cfg.poNominalPusch);
  /* Parameter: a as defined in 36.213, used for UE uplink power control. where al0 corresponds to 0,
   * al04 corresponds to value 0.4, al05 to 0.5, al06 to 0.6, al07 to 0.7, al08 to 0.8, al09 to 0.9 and
   * al1 corresponds to 1.
   */
  UEH_FILL_TKN_UINT(ulPwrCntrl->alpha, uehCb.uehCfgCb.phyCfg.alpha);
  /* Parameter PO_NOMINAL_ PUCCH P as defined in 36.213, used for UE uplink power control. */
  UEH_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUCCH, uehCb.uehCfgCb.phyCfg.poNominalPucch);


  UEH_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.pres, PRSNT_NODEF);
  /* deltaF_PUCCH (F) for the PUCCH format 1 relative to PUCCH format 1a as defined in 36.213. */
  UEH_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1, uehCb.uehCfgCb.sib2Cfg.deltaFPucchFormat1);
  /* deltaF_PUCCH (F) for the PUCCH format 1b relative to PUCCH format 1a as defined in 36.213,
   * used for UE uplink power control.
   */
  UEH_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1b, uehCb.uehCfgCb.sib2Cfg.deltaFPucchFormat1b);
  /* deltaF_PUCCH (F) for the PUCCH format 2 relative to PUCCH format 1a as defined in 36.213,
   * used for UE uplink power control.
   */
  UEH_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2, uehCb.uehCfgCb.sib2Cfg.deltaFPucchFormat2);
  /* deltaF_PUCCH (F) for the PUCCH format 2a relative to PUCCH format 1a as defined in 36.213,
   * used for UE uplink power control.
   */
  UEH_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2a, uehCb.uehCfgCb.sib2Cfg.deltaFPucchFormat2a);
  /* deltaF_PUCCH (F) for the PUCCH format 2b relative to PUCCH format 1a as defined in 36.213,
   * used for UE uplink power control.
   */
  UEH_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2b, uehCb.uehCfgCb.sib2Cfg.deltaFPucchFormat2b);
  /* deltaPreambleMessage3 as defined in 36.213, used to derive the PUSCH power (re)transmissions corresponding to
   * the random access response grant. Parameter actual range is {-1..6}, i.e. 0 means -1, 2 means 0, 3 means +1,
   * and so on. Actual value used by UE = Paramater value * 2 [dB].
   */
  UEH_FILL_TKN_UINT(ulPwrCntrl->deltaPreambleMsg3, uehCb.uehCfgCb.sib2Cfg.deltaPreambleMsg3);

  RETVALUE(ROK);
}

PUBLIC S16 uehFillSib2SoundingRefUpLnkCfg(
    NhuSysInfoBlockTyp2 *sib2
  )
{
  NhuSoundingRS_UL_ConfigCmn *srsCfg = NULLP;

  /* SoundingsRS UL Cfg */
  if (uehCb.uehCfgCb.phyCfg.srsEnabled)
  {
    srsCfg = &(sib2->radioResourceConfigCmn.soundingRS_UL_Config);
    UEH_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_SETUP);
    UEH_FILL_TKN_UINT(srsCfg->val.setup.pres, PRSNT_NODEF);
    /* SRS Bandwidth Configuration as defined in 36.211, 5.5.3.2 tables 1..4.
     * Actual configuration depends on UL bandwidth. bw0 corresponds to value 0, bw1 to value 1 and so on.
     */
    UEH_FILL_TKN_UINT(srsCfg->val.setup.srs_BandwidthConfig, uehCb.uehCfgCb.sib2Cfg.srsBandwidthConfig);
    /* Defines the cell specific sub frame configuration that corresponds to subframe configuration period
     * and the subframe offset for SRS transmission
     */
    UEH_FILL_TKN_UINT(srsCfg->val.setup.srs_SubfrmConfig, uehCb.uehCfgCb.sib2Cfg.srsSubfrmConfig);
    /* If the parameter SimultaneousAnSrsTx is set to FALSE, the UE shall not transmit SRS whenever
     * SRS transmission and PUCCH transmission carrying ACK/NACK and/or positive SR happen to coincide in
     * the same subframe.
     */
    UEH_FILL_TKN_UINT(srsCfg->val.setup.ackNackSRS_SimultaneousTransmission, uehCb.uehCfgCb.sib2Cfg.srsSimultaneousAnSrsTx);/*uehDfltAckNackSrsSimTrns*/
  }
  else
  {
    srsCfg = &(sib2->radioResourceConfigCmn.soundingRS_UL_Config);
    UEH_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_RELEASE);
  }
  RETVALUE(ROK);
}

/*
*
*       Fun:   uehFillCmnSib2
*
*       Desc:  Fills Comman fields of System Info message type2
*
*       Ret:   
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
PUBLIC S16 uehFillCmnSib2(
    Ptr                      *evnt,
    NhuBCCH_DL_SCH_Msg       *bcchSchMsg
  )
{
  NhuSysInfocriticalExtns  *critExt = NULLP;

  UEH_FILL_TKN_UINT(bcchSchMsg->pres,                               PRSNT_NODEF);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.choice,                     BCCH_DL_SCH_MSGTYP_C1);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice,              C1_SYSTEMINFO);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres, PRSNT_NODEF);

  critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
  UEH_FILL_TKN_UINT(critExt->choice,                                 CRITICALEXTNS_SYSTEMINFO_R8);
  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres,                 PRSNT_NODEF);
  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres, PRSNT_NODEF);
  /* TODO We are filling only SIB2 */
  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp, UEH_VAL_ONE);

  UEH_GET_MEM(evnt, ((critExt->val.systemInfo_r8.sib_TypAndInfo.noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember))),
       &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

  if(NULLP == (critExt->val.systemInfo_r8.sib_TypAndInfo.member))
    RETVALUE(RFAILED);

  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice, MEMBER_SIB2);
  RETVALUE(ROK);
}

/*
*
*       Fun:   uehFillSib2Msg
*
*       Desc:  Fills System Info message (DL BCCH DLSCH)
*
*       Ret:
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
PUBLIC S16 uehFillSib2Msg(
    Ptr                     *evnt,
    NhuSysInfoBlockTyp2     *sib2
  )
{
   logFunc("%s", "uehFillSib2Msg");

   UEH_FILL_TKN_UINT(sib2->pres, PRSNT_NODEF);

   /* Fill Access Barring Info */
   if( ROK NOTEQ uehFillSib2AcBarringInfo( evnt, sib2 ) )
   {
     RETVALUE(RFAILED);
   }

   /*-Fill Radio Resource Config Common -*/
   UEH_FILL_TKN_UINT(sib2->radioResourceConfigCmn.pres, PRSNT_NODEF);

   /* RACH cfg */
   uehFillSib2RachCfg(sib2);

   /* BCCH CFG */
   uehFillSib2BcchCfg(sib2);

   /* PCCH CFG */
   uehFillSib2PcchCfg(sib2);

   /* PRACH Cfg */
   uehFillSib2PrachCfg(sib2);

   /* PDSCH Cfg */
   uehFillSib2PdschCfg(sib2);

   /* PUSCH CFG */
   uehFillSib2PuschCfg(sib2);

   /* PUCCH Cfg */
   uehFillSib2PucchCfg(sib2);

   /* SoundingsRS UL Cfg */
   uehFillSib2SoundingRefUpLnkCfg(sib2);

   /* Uplink Power Control Config */
   uehFillSib2UpLinkPwrCtrlCfg(sib2);

   /* Uplink cyclic Prefix Length */
   UEH_FILL_TKN_UINT(sib2->radioResourceConfigCmn.ul_CyclicPrefixLength, smCb.cfgCb.uehConfig.ulCpLength);//uehDfltUlCyclicPrefixLength);

   /* Fill Timers and Constants */
   uehFillSib2TimersAndConst(sib2);

   /*-Fill Frequency Information -*/
   uehFillSib2FreqInfo(sib2);

  /* skipping MBSFN subframe config */

  /* TimeAlignmentTimer is cell-specific, TA timer used to control how long the UE is considered uplink time aligned.
  *  MAC must send TA to UE before expiry of this timer. Value in number of sub-frames.
  *  Value sf500 corresponds to 500 sub-frames, sf750 corresponds to 750 sub-frames and so on
  */
  UEH_FILL_TKN_UINT(sib2->timeAlignmentTimerCmn, uehCb.uehCfgCb.sib2Cfg.timeAlignmentTimer);

  RETVALUE(ROK);
} /* End of uehFillSib2Msg */

/*
*
*       Fun:   uehFillSib3Msg 
*
*       Desc:  Fills System Info message (DL BCCH DLSCH)
*
*       Ret:
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
PUBLIC S16 uehFillSib3Msg (
    NhuDatReqSdus *nhDatReqEvntSdu
  )
{
  NhuBCCH_DL_SCH_Msg        *bcchSchMsg = NULLP;
  NhuSysInfocriticalExtns   *critExt = NULLP;
  NhuSysInfoBlockTyp3       *sib3 = NULLP;

  logFunc("%s", "uehFillSib3Msg");

  bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);

  UEH_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres, PRSNT_NODEF);

  critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
  UEH_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres, PRSNT_NODEF);

  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp, UEH_VAL_ONE);

  UEH_GET_MEM(nhDatReqEvntSdu, (critExt->val.systemInfo_r8.sib_TypAndInfo.
        noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)),
      &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice, MEMBER_SIB3);
  /*-- For sib3 */
  sib3 = &(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].val.sib3);
  /* Code for Filling SIB3*/
  UEH_FILL_TKN_UINT(sib3->pres, PRSNT_NODEF);

  /*-Fill Cell Reselection Information Common -*/
  UEH_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.q_Hyst, uehCb.uehCfgCb.sib3Cfg.qHyst);

  /*-Fill Cell Reselection Serving Frequency Information -*/
  UEH_FILL_TKN_UINT(sib3->cellReselectionServingFreqInfo.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(sib3->cellReselectionServingFreqInfo.threshServingLow, uehCb.uehCfgCb.sib3Cfg.threshServingLow);
  UEH_FILL_TKN_UINT(sib3->cellReselectionServingFreqInfo.cellReselectionPriority, uehCb.uehCfgCb.sib3Cfg.cellReselectionPriority);

  /*-Fill Intra-Frequency Cell Reselection Information -*/
  UEH_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.q_RxLevMin, uehCb.uehCfgCb.sib3Cfg.qRxLevMin);
  if(SPECIAL_VALUE_SIB3_PMAX NOTEQ uehCb.uehCfgCb.sib3Cfg.pMax)
  {
    UEH_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.p_Max, uehCb.uehCfgCb.sib3Cfg.pMax);
  }
  if(SPECIAL_VALUE_SIB3_S_INTRA_SEARCH NOTEQ uehCb.uehCfgCb.sib3Cfg.sIntraSearch)
  {
    UEH_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.s_IntraSearch, uehCb.uehCfgCb.sib3Cfg.sIntraSearch);
  }
  if (ALLOWMEASBW_SAMEASOWNCELLDLBW NOTEQ uehCb.uehCfgCb.sib3Cfg.allowMeasBw)
  {
    UEH_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.allowedMeasBandwidth, uehCb.uehCfgCb.sib3Cfg.allowMeasBw);
  }
  UEH_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.presenceAntennaPort1, uehCb.uehCfgCb.sib3Cfg.presenceAntennaPort1 );
  UEH_FILL_BIT_STR(sib3->intraFreqCellReselectionInfo.neighCellConfig, 2, uehCb.uehCfgCb.sib3Cfg.neighCellConfig, nhDatReqEvntSdu);
  UEH_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.t_ReselectionEUTRA, uehCb.uehCfgCb.sib3Cfg.tReselectionEutra);

  RETVALUE(ROK);
}

#ifdef PWAV_SIB4
/*
*
*       Fun:   uehFillSib4Msg
*
*       Desc:  Fills System Info message SIB4
*
*       Ret:
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
PUBLIC S16 uehFillSib4Msg (
    NhuDatReqSdus *nhDatReqEvntSdu
  )
{
  U8                        idx = 0;
  NhuBCCH_DL_SCH_Msg        *bcchSchMsg = NULLP;
  NhuSysInfocriticalExtns   *critExt = NULLP;
  NhuSysInfoBlockTyp4       *sib4 = NULLP;

  logFunc("%s", "(uehFillSib4Msg");

  bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);

  UEH_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
  UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres, PRSNT_NODEF);

  critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
  UEH_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres, PRSNT_NODEF);

  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp, UEH_VAL_ONE);

  UEH_GET_MEM(nhDatReqEvntSdu, (critExt->val.systemInfo_r8.sib_TypAndInfo.noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)),
      &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

  UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice, MEMBER_SIB4);

  sib4 =&(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].val.sib4);
  UEH_FILL_TKN_UINT(sib4->pres, PRSNT_NODEF);

  /*-Fill Intra-Frequency Neighbouring Cell List-*/
  UEH_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.noComp, uehDfltS4MaxCellIntra);
  UEH_GET_MEM(nhDatReqEvntSdu, (sib4->intraFreqNeighCellLst.noComp.val) *(sizeof(NhuIntraFreqNeighCellInfo)), &(sib4->intraFreqNeighCellLst.member));
  for ( idx = 0; idx < sib4->intraFreqNeighCellLst.noComp.val; idx++)
  {
    UEH_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.member[idx].pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.member[idx].physCellId, uehCb.uehCfgCb.sib4Cfg.neighEutraPhyCellId);
    UEH_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.member[idx].q_OffsetCell, uehCb.uehCfgCb.sib4Cfg.intraFreqNeighCellQOffsetCell);
  }

  RETVALUE(ROK);
}
#endif /* PWAV_SIB4*/
#ifdef PWAV_SIB5
/*
*
*       Fun:   uehSibBldSIB5Msg
*
*       Desc:  Fills System Info message SIB5
*
*       Ret:
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 uehSibBldSIB5Msg(
    NhuDatReqSdus *nhDatReqEvntSdu
  )
#else
PUBLIC S16 uehSibBldSIB5Msg(nhDatReqEvntSdu, schedSFN, subframe)
NhuDatReqSdus *nhDatReqEvntSdu;
U16 schedSFN;
U8 subframe;
#endif
{
   NhuBCCH_DL_SCH_Msg *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns *critExt = NULLP;

   U8 idx1,idx2 = 0;

   NhuSysInfoBlockTyp5 *sib5 = NULLP;

   logFunc("%s", "uehSibBldSIB5Msg");

   bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);

   UEH_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
   UEH_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres,\
         PRSNT_NODEF);

   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
   UEH_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
   UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres,\
         PRSNT_NODEF);

   UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp,\
         UEH_VAL_ONE);

   UEH_GET_MEM(nhDatReqEvntSdu, (critExt->val.systemInfo_r8.sib_TypAndInfo.\
            noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)),
         &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

   UEH_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice,\
         MEMBER_SIB5);

   sib5 = &(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].
                              val.sib5);
   UEH_FILL_TKN_UINT(sib5->pres, PRSNT_NODEF);
   /*-Fill Inter-Frequency Carrier Frequency List -*/
   UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.noComp, 1);
   UEH_GET_MEM(nhDatReqEvntSdu, (sib5->interFreqCarrierFreqLst.\
            noComp.val)
         *(sizeof(NhuInterFreqCarrierFreqInfo)),
         &(sib5->interFreqCarrierFreqLst.member));
   for (idx1 = 0;
         idx1 < sib5->interFreqCarrierFreqLst.noComp.val; idx1++)
   {
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].pres,
            PRSNT_NODEF);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].dl_CarrierFreq,
            veDfltS5DlCarrierFreq);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].q_RxLevMin,
            veDfltS5QRxLevMin);
      /*UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].p_Max,
            veDfltInterFreqpMax); p-Max is not present */
      sib5->interFreqCarrierFreqLst.member[idx1].p_Max.pres = NOTPRSNT;
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].t_ReselectionEUTRA,
            veDfltS5InterFreqtReselection);
      /*UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].t_ReselectionEUTRA_SF.pres,
           NOTPRSNT);*/
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            threshX_High, veDfltS5ThreshXHigh);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            threshX_Low, veDfltS5ThreshXLow);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            allowedMeasBandwidth, veDfltS5AllowedMeasBandwidth);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            presenceAntennaPort1, veDfltS5PresenceAntennaPort1);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            cellReselectionPriority, veDfltS5CellReselectionPriority);
      UEH_FILL_BIT_STR(sib5->interFreqCarrierFreqLst.member[idx1].\
            neighCellConfig,2, veDfltS5NeighCellCfg, nhDatReqEvntSdu);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            q_OffsetFreq, veDfltS5QOffsetFreq);
      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqNeighCellLst.noComp, 1);
      UEH_GET_MEM(nhDatReqEvntSdu, (sib5->interFreqCarrierFreqLst.\
               member[idx1].interFreqNeighCellLst.noComp.val)
            *(sizeof(NhuInterFreqNeighCellInfo)),
            &(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqNeighCellLst.member));
      for (idx2 = 0;
            idx2 < sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqNeighCellLst.noComp.val;
            idx2++)
      {
         UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqNeighCellLst.member[idx2].pres,
               PRSNT_NODEF);
         UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].
               interFreqNeighCellLst.member[idx2].
               physCellId, 1);
         UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].
               interFreqNeighCellLst.member[idx2].q_OffsetCell,
               0);
      }

      UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqBlackCellLst.noComp, 1);
      UEH_GET_MEM(nhDatReqEvntSdu, (sib5->interFreqCarrierFreqLst.
               member[idx1].interFreqBlackCellLst.noComp.val)
            *(sizeof(NhuInterFreqBlackCellInfo)),
            &(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member));
      for (idx2 = 0;
            idx2 < sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqBlackCellLst.noComp.val; idx2++)
      {
         UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].pres, PRSNT_NODEF);
         UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.pres, 1);
         UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.start, 1);
         UEH_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.range, 1);
      }
   }

   RETVALUE(ROK);
}
#endif /* PWAV_SIB5 */

U8 getPoValFromIsAndNs(
  U8   is
  )
{
  U8  poActualVal = 0;

  poActualVal = poMappingValFromAsAndNs[((uehGetCellCb()->ns)/2)][is];
  return poActualVal;
}
void uehFillPagHdr
(
  NhuDatReqSdus   *pagMsg,
  U16              pagSfn,
  U8               is
)
{
  U16              cellId = 0;

  /* Cell ID */
  cellId = uehGetCellCb()->cellId;
  /* Fill HDR Info */
  uehFillNhuEvntHdr(&(pagMsg->hdr), cellId, UEH_VAL_ZERO, C2_PAGING);
  pagMsg->sdu.ueMsg.isMsgDed = FALSE;
  pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = NHU_MSG_DLPCCH;
  pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = PCCH_ID;
  pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.sfn = pagSfn;
  /* PO is reteived from ENUM */
  pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.subframe = getPoValFromIsAndNs(is);;
}

/*
*
*       Fun:   uehPagHdlTmrInd
*
*       Desc:  Function handler called every Tmr indication.
*
*       Ret:
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
PUBLIC S16 uehPagHdlTmrInd
(
 RgrTtiIndInfo *tmngInfo
)
{
  U8               tempIs = 0 ;
  U16              pagSfn = 0;
  Pst             *pst = NULLP;
  NhuDatReqSdus   *pagMsg[UEH_MAX_I_S];
  UehCellCb       *uehCellCb = uehGetCellCb();

  logFunc("%s", "uehPagHdlTmrInd");


  pagSfn = (tmngInfo->sfn + UEH_SFN_INCREMENT) % UEH_MAX_SFN_PAGING;

  /* No pending Paging Records in List */
  if(0 EQ uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn].count)
  {
    RETVALUE(ROK);
  }

  pst = &uehCb.nhuSap[0]->pst;
  pst->event   = EVTNHUDATREQ;
  /* For all the UEs in the Pending Paging List, compute
   * if there is any paging message to be scheduled
   */
  /* PO_0 , PO_4 , PO_5, PO_9  are present for FDD */
  for(tempIs = 0; tempIs < UEH_MAX_I_S; tempIs ++)
  {
    pagMsg[tempIs] = NULLP;
    if((uehCellCb->uehPagingRecordInfo.noOfRecInGroup[pagSfn][tempIs]) > 0)
    {
      /* Allocate memory for paging message */
      UEH_ALLOCEVNT_OR_RET(&pagMsg[tempIs], sizeof(NhuDatReqSdus));
      uehFillPagHdr(pagMsg[tempIs], pagSfn , tempIs);
    }
  }

  logInfo("uehPagHdlTmrInd SFN: %u",tmngInfo->sfn);

  /* Fill the NhuDatReqSdus paging message */
  uehFillRRCPagingMsg(pagMsg , pagSfn);
  for(tempIs = 0; tempIs < UEH_MAX_I_S; tempIs ++)
  {
    if(NULL NOTEQ pagMsg[tempIs])
    {
      /* Issue the primitive */
      if(ROK != UehLiNhuDatReq(pst, uehCb.nhuSap[0]->spId, pagMsg[tempIs]))
      {
        UEH_ERROR_SSI_EVENT_SEND_FAILED( pst, EVENTTYPE_OTHER, CELL, uehCellCb->cellId);   
        RETVALUE(RFAILED);
      } /* end of if statement */
      uehRecordUeEvnt( NULLP, EVENTTYPE_PAGING ,pagMsg[tempIs],FALSE);
    }
  }
  RETVALUE(ROK);
} /* uehPagHdlTmrInd */




#ifdef RGR_RRM_TICK
/*
*
*       Fun:   uehSibHdlTmrInd
*
*       Desc:  Function handler called every Tmr indication.
*
*       Ret:   
*
*       Notes: None
*
*       File:  ueh_sib.c
*
*/
PUBLIC S16 uehSibHdlTmrInd(
    RgrTtiIndInfo *tmngInfo
  )
{
  TRC2(uehSibHdlTmrInd);
#ifndef SI_NEW
  U8              siIdx = 0;
  U8              sibTyp = 0;
  U16             sfn = 0;
  UehCellCb       *cellCb = uehGetCellCb();
  CmLteTimingInfo tmnInf;

  /* Updating SFN */
  cellCb->sfn = tmngInfo->sfn;

  /*If system is not operational, dont send any data down and return from here*/
  if (!uehCb.systemOperational)
  {
    RETVALUE(ROK);
  }

  sfn = (tmngInfo->sfn + UEH_SFN_INCREMENT) % UEH_MAX_SFN;

  tmnInf.sfn = sfn;

  if (0 == (sfn % UEH_MIB_PERIOD))
  {
    tmnInf.subframe = UEH_MIB_SUBFRAME;
   // logInfo("uehSibHdlTmrInd: MIB Scheduled TransTm: (%u, %u)", sfn, tmnInf.subframe);
    uehSndCmnNhuDatReq(NHU_MSG_DLBCCHBCH, UEH_SYS_MIB, tmnInf);
    cellCb->numMibsTx++;
  }

  /* SIs Scheduling */
  for (siIdx = 0; siIdx < cellCb->numOfSis; siIdx++)
  {
    /* SFN mod T = FLOOR(x/10) */
    if(cellCb->siSchInfo[siIdx].siSchGap == (sfn % siPeriodicityVal[cellCb->siSchInfo[siIdx].siPrdcty]))
    {
      tmnInf.subframe = cellCb->siSchInfo[siIdx].siSchSubframe;

      sibTyp = (0xFF != cellCb->siSchInfo[siIdx].sibMapInfo) ? (cellCb->siSchInfo[siIdx].sibMapInfo + 3) : (UEH_SYS_SIB2);

    //  logInfo("uehSibHdlTmrInd: SI%u->SIB%u Scheduled TransTm: (%u, %u)", ++siSchCnt, sibTyp, tmnInf.sfn, tmnInf.subframe);
      uehSndCmnNhuDatReq(NHU_MSG_DLBCCHDLSCH, sibTyp, tmnInf);
    }
  }

  if (0 == (sfn % UEH_SIB1_PERIOD))
  {
    tmnInf.subframe = UEH_SIB1_SUBFRAME; /* SIB1 on subframe no. 5 */
   // logInfo("uehSibHdlTmrInd: SIB1 Scheduled TransTm: (%u, %u)", sfn, tmnInf.subframe);
    uehSndCmnNhuDatReq(NHU_MSG_DLBCCHDLSCH, UEH_SYS_SIB1, tmnInf);
  }
#endif /* SI_NEW */

  RETVALUE(ROK);
} /* uehSibHdlTmrInd */
#endif /* RGR_RRM_TICK */
