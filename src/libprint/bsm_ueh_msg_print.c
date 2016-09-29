/**
 * @file    bsm_ueh_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and UEH
 * @author :  Ritesh
 * Date   :
 * Change History:
 *
 */

#include "bsm_ueh_msg_print.h"
#include "print_cmn.h"
#include "print_tlv.h"

void printMsgBsmUehConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmUehEnbConfigReq *msg = (MsgBsmUehEnbConfigReq*)data;
  printMsgConfigReq(msg, len);
}

void printMsgUehBsmUeEventHistoryLogConfigFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UNUSED(len);

  MsgUehBsmUeEventHistoryLogConfigFail *msg = (MsgUehBsmUeEventHistoryLogConfigFail*)data;
  printCmdRef(&msg->cmdRef);
}

void printMsgUehBsmUeEventHistoryLogConfigSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UNUSED(len);

  MsgUehBsmUeEventHistoryLogConfigSucc *msg = (MsgUehBsmUeEventHistoryLogConfigSucc*)data;
  printCmdRef(&msg->cmdRef);
}

void printMsgBsmUehUeEventHistoryLogConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UNUSED(len);

  MsgBsmUehUeEventHistoryLogConfigReq *msg = (MsgBsmUehUeEventHistoryLogConfigReq*)data;
  logMsgParams("%s","{\n");
  logMsgParams( " ueEventHistoryIndex=%d\n cRnti=%d\n ueProcedureBitMap=%d\n ueEventMeasures=%d\n", \
     msg->ueEventHistoryLogConfig.ueEventHistoryIndex,msg->ueEventHistoryLogConfig.cRnti, \
         msg->ueEventHistoryLogConfig.ueProcedureBitMap, msg->ueEventHistoryLogConfig.ueEventMeasures);
  logMsgParams("%s","}\n");
}

void printMsgBsmUehModStateReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmUehModStateReq *msg = (MsgBsmUehModStateReq*)data;
  logMsgParams("%s","{\n");
  printTlvCmdRef(&msg->cmdRef);
  printTlvTagInfo(&msg->tagInfo);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgUehBsmModStateRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehBsmModStateRsp *msg = (MsgUehBsmModStateRsp*)data;
  logMsgParams("%s","{\n");
  printTlvCmdRef(&msg->cmdRef);
  printTlvTagInfo(&msg->tagInfo);
  logMsgParams(" numOfUesConnected = %d \n", msg->numOfUesConnected );
  logMsgParams("} Total length = %d\n", len );
}
void printMsgConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT8   i;
  MsgBsmUehEnbConfigReq *msg = (MsgBsmUehEnbConfigReq*) data;
  logMsgParams("%s","{\n");
  logMsgParams(" transId=%d\n"
      "*****ENODEBCFG*****\n"
      " enodebId=%d\n"
      " cellBarred=%d   0=CELLBARRED_BARRED, 1=CELLBARRED_NOTBARRED \n"

      " *****CELLCFG*****\n"
      " cellId=%d\n cellType=%d\n plmnMCC=%s\n plmnMNC=%s\n tac=%d\n freqBandInd=%d\n dlBandwidth=%d\n dlEarfcn=%d\n"
      " ulBandwidth =%d\n ulEarfcn=%d\n cellReservedForOperatorUse=%d\n maxConnectedUes=%d\n acBarringEmergency=%d\n"
      " acBarrMoSigEnabled=%d\n acBarrMoSigFactor=%d\n acBarrMoSigTime=%d\n acBarrMoSigSpecialAC=%s\n acBarrMoDataEnabled=%d\n"
      " acBarrMoDataFactor=%d\n acBarrMoDataTime=%d\n acBarrMoDataSpecialAC=%s\n startRnti=%d\n "
      " rsrpFilterCoefficient=%d\n schedulerProfileId=%d\n phyProfileId=%d\n physicalCellId=%d\n physicalCellGroupId=%d\n"
      " pMax=%d\n ueResponseTimer=%d\n rrcConnRejWaitTime=%d\n ueInactivityCheckInterval_secs=%d\n sysInfo1ProfileId=%d\n sysInfo2ProfileId=%d\n sysInfo3ProfileId=%d\n"

      "******SCHEDULERCFG******\n"
      " schedulerProfileId=%d\n maxDlHarqTx=%d\n maxUlHarqTx=%d\n maxMsg4HarqTx=%d\n pcfichCfi=%d\n periodicPhrTimer=%d\n"
      " prohibitPhrTimer=%d\n dlPathlossChange=%d\n periodicBsrTimer=%d\n retxBsrTimer=%d\n"

      " *********PHYCFG***********\n"
      " phyProfileId=%d\n phichResource=%d\n prachFreqOffset=%d\n totalTxPower=%d\n referenceSignalPower=%d\n cellPhyPb=%d\n srsEnabled=%d\n"
      " alpha=%d\n poNominalPucch=%d\n cellPhyPa=%d\n betaOffsetAckIndex=%d\n betaOffsetRiIndex=%d\n betaOffsetCqiIndex=%d\n"
      " poUePucch=%d\n poUePusch=%d\n pSrsOffset=%d\n"

      " *******SYSINFO1CFG**********\n"
      " sysInfo1ProfileId=%d\n intraFreqReselection=%d\n qRxLevMin=%d\n qRxLevMinOffset=%d\n "
      " sib2Periodicity=%d\n sib3Periodicity=%d\n sib4Periodicity=%d\n siWindowLength=%d\n"

      " *******SYSINFO2CFG********\n"
      " sysInfo2ProfileId=%d\n numRaPreambles=%d\n sizeRAPreamblesGroupA=%d\n messageSizeGroupA=%d\n messagePowerOffsetGroupB =%d\n"
      " powerRampingStep=%d\n preambleInitRcvdTgtPower=%d\n preambleTransMax=%d\n raRspWindowSize=%d\n macContResolutionTimer=%d\n"
      " maxHarqMsg3Tx=%d\n modificationPeriodCoeff=%d\n defaultPagCycle=%d\n nB=%d\n rootSequenceIndex=%d\n prachConfigIndex=%d\n"
      " highSpeedFlag=%d\n zeroCorrelationZoneConfig=%d\n puschFreqHoppingSubBands=%d\n puschFreqHoppingMode=%d\n puschFreqHoppingOffset=%d\n"
      " ulRsGroupHoppingEnabled=%d\n ulRsGroupPusch=%d\n ulRsSeqHoppingEnabled=%d\n ulRsCyclicShiftPusch=%d\n deltaPucchShift=%d\n n1PucchAn=%d\n"
      " srsBandwidthConfig=%d\n srsSubfrmConfig=%d\n srsSimultaneousAnSrsTx=%d\n poNominalPusch=%d\n deltaFPucchFormat1=%d\n"
      " deltaFPucchFormat1b=%d\n deltaFPucchFormat2=%d\n deltaFPucchFormat2a=%d\n deltaFPucchFormat2b=%d\n deltaPreambleMsg3=%d\n"
      " t300=%d\n t301=%d\n t310=%d\n n310=%d\n t311=%d\n n311=%d\n additionalSpectrumEmission=%d\n timeAlignmentTimer=%d\n"

      " *******SYSINFO3CFG********\n"
      " sysInfo3ProfileId=%d\n qHyst=%d\n threshServingLow=%d\n cellReselectionPriority=%d\n qRxLevMin=%d\n"
      " pMax=%d\n sIntraSearch=%d\n allowMeasBw=%d\n neighCellConfig=%d\n tReselectionEutra=%d\n presenceAntennaPort1=%d\n"

      " *******SYSINFO4CFG********\n"
      " neighEutraPhyCellId=%d\n intraFreqNeighCellQOffsetCell=%d\n",


       msg->transId,
       msg->enodebId,
       msg->cellBarred,
       msg->cellCfg.cellId,msg->cellCfg.cellType,msg->cellCfg.plmnMCC,msg->cellCfg.plmnMNC,msg->cellCfg.tac,
       msg->cellCfg.freqBandInd,msg->cellCfg.dlBandwidth,msg->cellCfg.dlEarfcn,msg->cellCfg.ulBandwidth,
       msg->cellCfg.ulEarfcn,msg->cellCfg.cellReservedForOperatorUse,msg->cellCfg.maxConnectedUes,
       msg->cellCfg.acBarringEmergency,msg->cellCfg.acBarrMoSigEnabled,msg->cellCfg.acBarrMoSigFactor,msg->cellCfg.acBarrMoSigTime,
       msg->cellCfg.acBarrMoSigSpecialAC,msg->cellCfg.acBarrMoDataEnabled,msg->cellCfg.acBarrMoDataFactor,msg->cellCfg.acBarrMoDataTime,
       msg->cellCfg.acBarrMoDataSpecialAC,msg->cellCfg.startRnti,msg->cellCfg.rsrpFilterCoefficient,
       msg->cellCfg.schedulerProfileId,msg->cellCfg.phyProfileId,msg->cellCfg.phyLayerId, msg->cellCfg.phyCellIdentityGroup,
       msg->cellCfg.pMax, msg->cellCfg.ueResponseTimer, msg->cellCfg.rrcConnRejWaitTime,msg->cellCfg.ueInactivityCheckIntervalSecs,
       msg->cellCfg.sysInfo1ProfileId, msg->cellCfg.sysInfo2ProfileId,msg->cellCfg.sysInfo3ProfileId,
       msg->schCfg.schedulerProfileId, msg->schCfg.maxDlHarqTx, msg->schCfg.maxUlHarqTx,msg->schCfg.maxMsg4HarqTx,
       msg->schCfg.pcfichCfi, msg->schCfg.periodicPhrTimer,msg->schCfg.prohibitPhrTimer, msg->schCfg.dlPathlossChange,
       msg->schCfg.periodicBsrTimer, msg->schCfg.retxBsrTimer,
       msg->phyCfg.phyProfileId,msg->phyCfg.phichResource,msg->phyCfg.prachFreqOffset,msg->phyCfg.totalTxPower,msg->phyCfg.referenceSignalPower,
       msg->phyCfg.cellPhyPb,msg->phyCfg.srsEnabled,msg->phyCfg.alpha,msg->phyCfg.poNominalPucch,msg->phyCfg.cellPhyPa,
       msg->phyCfg.betaOffsetAckIndex, msg->phyCfg.betaOffsetRiIndex,msg->phyCfg.betaOffsetCqiIndex,
       msg->phyCfg.poUePucch, msg->phyCfg.poUePusch, msg->phyCfg.pSrsOffset,
       msg->sib1Cfg.sysInfo1ProfileId,msg->sib1Cfg.intraFreqReselection, msg->sib1Cfg.qRxLevMin,msg->sib1Cfg.qRxLevMinOffset,
       msg->sib1Cfg.sib2Periodicity,msg->sib1Cfg.sib3Periodicity,msg->sib1Cfg.sib4Periodicity ,msg->sib1Cfg.siWindowLength,
       msg->sib2Cfg.sysInfo2ProfileId ,msg->sib2Cfg.numRaPreambles, msg->sib2Cfg.sizeRAPreamblesGroupA,msg->sib2Cfg.messageSizeGroupA,
       msg->sib2Cfg.messagePowerOffsetGroupB,msg->sib2Cfg.powerRampingStep, msg->sib2Cfg.preambleInitRcvdTgtPower,
       msg->sib2Cfg.preambleTransMax,msg->sib2Cfg.raRspWindowSize,msg->sib2Cfg.macContResolutionTimer,msg->sib2Cfg.maxHarqMsg3Tx,
       msg->sib2Cfg.modificationPeriodCoeff, msg->sib2Cfg.defaultPagCycle,msg->sib2Cfg.nB,msg->sib2Cfg.rootSequenceIndex,
       msg->sib2Cfg.prachConfigIndex,msg->sib2Cfg.highSpeedFlag,msg->sib2Cfg.zeroCorrelationZoneConfig,
       msg->sib2Cfg.puschFreqHoppingSubBands,msg->sib2Cfg.puschFreqHoppingMode,msg->sib2Cfg.puschFreqHoppingOffset,
       msg->sib2Cfg.ulRsGroupHoppingEnabled, msg->sib2Cfg.ulRsGroupPusch,msg->sib2Cfg.ulRsSeqHoppingEnabled,
       msg->sib2Cfg.ulRsCyclicShiftPusch,msg->sib2Cfg.deltaPucchShift,msg->sib2Cfg.n1PucchAn,msg->sib2Cfg.srsBandwidthConfig,
       msg->sib2Cfg.srsSubfrmConfig,msg->sib2Cfg.srsSimultaneousAnSrsTx,msg->sib2Cfg.poNominalPusch,msg->sib2Cfg.deltaFPucchFormat1,
       msg->sib2Cfg.deltaFPucchFormat1b,msg->sib2Cfg.deltaFPucchFormat2,msg->sib2Cfg.deltaFPucchFormat2a, msg->sib2Cfg.deltaFPucchFormat2b,
       msg->sib2Cfg.deltaPreambleMsg3,msg->sib2Cfg.t300,msg->sib2Cfg.t301,msg->sib2Cfg.t310,msg->sib2Cfg.n310,msg->sib2Cfg.t311,msg->sib2Cfg.n311,
       msg->sib2Cfg.additionalSpectrumEmission,msg->sib2Cfg.timeAlignmentTimer,
       msg->sib3Cfg.sysInfo3ProfileId,msg->sib3Cfg.qHyst, msg->sib3Cfg.threshServingLow,msg->sib3Cfg.cellReselectionPriority,
       msg->sib3Cfg.qRxLevMin,msg->sib3Cfg.pMax,msg->sib3Cfg.sIntraSearch,msg->sib3Cfg.allowMeasBw,msg->sib3Cfg.neighCellConfig,
       msg->sib3Cfg.tReselectionEutra,msg->sib3Cfg.presenceAntennaPort1,
       msg->sib4Cfg.neighEutraPhyCellId,msg->sib4Cfg.intraFreqNeighCellQOffsetCell);


  logMsgParams(

      "*******QOSCONFIG**********\n"
      " qosProfileName=%s\n amRlcTPollRetransmit=%d\n amRlcPollPDU=%d\n amRlcPollByte=%d\n amRlcMaxRetxThreshold=%d\n"
      " amRlcTReordering=%d\n amRlcTStatusProhibit=%d\n discardTimer=%d\n statusReportRequired=%d\n" ,

      msg->qosCfg.qosProfileName, msg->qosCfg.amRlcTPollRetransmit, msg->qosCfg.amRlcPollPDU, msg->qosCfg.amRlcPollByte,
      msg->qosCfg.amRlcMaxRetxThreshold, msg->qosCfg.amRlcTReordering, msg->qosCfg.amRlcTStatusProhibit, msg->qosCfg.discardTimer,
      msg->qosCfg.statusReportRequired );

  logMsgParams(
      "******* EUTRAHOPROFILECONFIG **********\n"
	  " hoProfileId=%d\n a3Offset=%d\n reportOnLeave=%d\n"
	  " a5Threshold1=%d\n a5Threshold2=%d\n hysteresis=%d\n"
	  " timeToTrigger=%d\n triggerQuantity=%d\n reportQuantity=%d\n"
	  " filterCoefficientRsrq=%d\n filterCoefficientRsrp=%d\n reportInterval=%d\n"
	  " reportAmount=%d\n maxReportCells=%d\n supportedEvents=%d\n sMeasure=%d\n",
	  msg->eutraHoProfileConfig.hoProfileId, msg->eutraHoProfileConfig.a3Offset, msg->eutraHoProfileConfig.reportOnLeave,
	  msg->eutraHoProfileConfig.a5Threshold1, msg->eutraHoProfileConfig.a5Threshold2, msg->eutraHoProfileConfig.hysteresis,
	  msg->eutraHoProfileConfig.timeToTrigger, msg->eutraHoProfileConfig.triggerQuantity, msg->eutraHoProfileConfig.reportQuantity,
	  msg->eutraHoProfileConfig.filterCoefficientRsrq, msg->eutraHoProfileConfig.filterCoefficientRsrp, msg->eutraHoProfileConfig.reportInterval,
	  msg->eutraHoProfileConfig.reportAmount, msg->eutraHoProfileConfig.maxReportCells, msg->eutraHoProfileConfig.supportedEvents, msg->eutraHoProfileConfig.sMeasure
		  );
  logMsgParams("\n numEutraNeighborCells=%d\n", msg->numEutraNeighborCells );
logMsgParams(
    "******* SECURITYCONFIG ******************\n"
  " uuCiphering=%d\n uuIntegrityProtection=%d\n",
  msg->securityConfig.uuCiphering,msg->securityConfig.uuIntegrityProtection);

  for(i = 0 ; i< msg->numEutraNeighborCells; i ++)
  {
    /* Print EUTRANEIGHCELLCONFIG tables */
	logMsgParams(
	  "******* EUTRANEIGHCELLCONFIG **********\n"
	  " neibourIndex=%d\n isUsed=%d\n neighEutraEnbId=%d\n"
	  " neighEutraCellId=%d\n neighEutraPhyCellId=%d\n neighCellMeasBandwidth=%s\n"
	  " neighEutraPlmnMnc=%s\n neighEutraTac=%d\n neighEutraEnbId=%d\n"
	  " cellIndividualOffset=%d\n intraFreqNeighCellQOffsetCell=%d\n",
	  msg->eutraNeighCellConfig[i].neibourIndex, msg->eutraNeighCellConfig[i].isUsed, msg->eutraNeighCellConfig[i].neighEutraEnbId,
	  msg->eutraNeighCellConfig[i].neighEutraCellId, msg->eutraNeighCellConfig[i].neighEutraPhyCellId, msg->eutraNeighCellConfig[i].neighEutraPlmnMcc,
	  msg->eutraNeighCellConfig[i].neighEutraPlmnMnc, msg->eutraNeighCellConfig[i].neighEutraTac, msg->eutraNeighCellConfig[i].neighCellMeasBandwidth,
	  msg->eutraNeighCellConfig[i].cellIndividualOffset, msg->eutraNeighCellConfig[i].intraFreqNeighCellQOffsetCell
	  );
  }
  logMsgParams("} Total length = %d\n", len );
}

void printMsgActiveUeStatusReq(
  IO void    *data,
  IO UINT16  len
  )
{
  MsgBsmUehActiveUeStatusReq *msg = (MsgBsmUehActiveUeStatusReq*) data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgActiveUeStatusRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT16    numElem = 0;
  MsgUehBsmActiveUeStatusRsp *msg = (MsgUehBsmActiveUeStatusRsp*) data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams(" numElem=%d\n", msg->numElem );
  for (numElem=0; numElem < msg->numElem; numElem++)
  {
    logMsgParams("Ue No: %d\n", numElem);
    logMsgParams(" index=%d\n internalUeIdentity=%d\n mmec=%d\n mTmsi=%d\n cRnti=%d\n s1apEnbUeId=%d\n"
    " s1apMmeUeId=%d\n entryTime=%s\n dlAmbr=%d\n ulAmbr=%d\n ueCategory=%d\n" 
    " ueAccessStratumRelease=%d\n activeIntProtAlgo=%d\n activeCiphAlgo=%d\n indexOfRab1InRabStatusTable=%d\n",
    msg->activeUeStatus[numElem].index, msg->activeUeStatus[numElem].internalUeIdentity, msg->activeUeStatus[numElem].mmec,msg->activeUeStatus[numElem].mTmsi,
    msg->activeUeStatus[numElem].cRnti, msg->activeUeStatus[numElem].s1apEnbUeId, msg->activeUeStatus[numElem].s1apMmeUeId,
    msg->activeUeStatus[numElem].entryTime, msg->activeUeStatus[numElem].dlAmbr, msg->activeUeStatus[numElem].ulAmbr,
    msg->activeUeStatus[numElem].ueCategory, msg->activeUeStatus[numElem].ueAccessStratumRelease, 
    msg->activeUeStatus[numElem].activeIntProtAlgo, msg->activeUeStatus[numElem].activeCiphAlgo, 
    msg->activeUeStatus[numElem].numERabs);
  }

  logMsgParams("} Total length = %d\n", len );
}

void printMsgBsmUehUeEventHistoryReq(
  IO void    *data,
  IO UINT16  len
  )
{
  MsgBsmUehUeEventHistoryReq *msg = (MsgBsmUehUeEventHistoryReq*) data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgUehBsmUeEventHistoryRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT16    i = 0;
  MsgUehBsmUeEventHistoryRsp *msg = (MsgUehBsmUeEventHistoryRsp*) data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams(" numElem=%d\n", msg->numElem );
  for (i=0; i < msg->numElem; i++)
  {
    logMsgParams("\nRow No: %d \n", i+1);
    logMsgParams(" index=%d\n mmec=%d\n mTmsi=%d\n cRnti=%d\n",
                   msg->ueEventHistory[i].index, msg->ueEventHistory[i].mmec, msg->ueEventHistory[i].mTmsi, msg->ueEventHistory[i].cRnti );
    logMsgParams(" eventType=%d\n eventDetail=%s\n eventTimestamp=%s\n",
                   msg->ueEventHistory[i].eventType, msg->ueEventHistory[i].eventDetail, msg->ueEventHistory[i].eventTimestamp );
  }
  logMsgParams("} Total length = %d\n", len );
}

void printMsgUehBsmUnknownEnbUeS1apId(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehBsmUnknownEnbUeS1apId *msg = (MsgUehBsmUnknownEnbUeS1apId*)data;
  logMsgParams( "{\n UE ID=%d\n", msg->ueId ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgBsmUehCellBarReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmUehCellBarReq *msg = (MsgBsmUehCellBarReq*)data;
  logMsgParams( "{\n TranseId=%d\n CellId=%d\n", msg->transId, msg->cellId ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgUehBsmCellBarSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehBsmCellBarSucc *msg = (MsgUehBsmCellBarSucc*)data;
  logMsgParams( "{\n TranseId=%d\n", msg->transId) ;
  logMsgParams( " sfnReceived=%d\n", msg->sfnReceived) ;
  logMsgParams( " sfnNewSIBTx=%d\n", msg->sfnNewSIBTx) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgBsmUehCellUnbarReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmUehCellUnbarReq *msg = (MsgBsmUehCellUnbarReq*)data;
  logMsgParams( "{\n TranseId=%d\n CellId=%d\n", msg->transId, msg->cellId ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgUehBsmCellUnbarSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehBsmCellUnbarSucc *msg = (MsgUehBsmCellUnbarSucc*)data;
  logMsgParams( "{\n TranseId=%d\n", msg->transId) ;
  logMsgParams( " sfnReceived=%d\n", msg->sfnReceived) ;
  logMsgParams( " sfnNewSIBTx=%d\n", msg->sfnNewSIBTx) ;
  logMsgParams("} Total length = %d\n", len );
}
