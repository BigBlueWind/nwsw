/**
 * @file    deh_rh_fill_func.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains fills message structures sent to LK from BS for testing.
 *
 * @author        mkata, kgummad, m_sharma,spolepally
 * @date          27-05-2011
 * @details       Basic version
 *
 */

#include "deh_rh_fillFunc.h"
#include "deh.h"

/**
 * @param    msgId : Msg ID to be encoded in the buffer
 * @param    LkHdr : Header structure
 * @details  This function will fill the IPC header(dest & src mail box)
 *                                      + MsgId from Application data
 */
void fillHdr(
  IN MsgId   msgId,
  IO LkHdr * hdr
  )
{
  hdr->dstMailbox = LK_MAILBOX_ID;
  hdr->srcMailbox = BS_MAILBOX_ID;
  hdr->msgId      = msgId;
}

/*
 * @param    req : Pointer to BsLkHwInfoReq structure
 * @details  This function fills BsLkHwInfoReq structure.
 */
void fillDehRhHwInfoReq(
  IO BsLkHwInfoReq   *req
  )
{
  req->varDummy = 0x00000000;
}

/**
 * @param    req : Pointer to BsLkHwInfoReq structure
 * @details  This function fills BsLkFwInfoReq structure.
 */
void fillDehRhFwInfoReq(
  IO BsLkFwInfoReq  *req
  )
{
  req->varDummy = 0x00000000;
}

/**
 * @param    req : Pointer to BsLkHistoryInfoReadReq structure
 * @details  This function fills BsLkHistoryInfoReadReq structure.
 */
void fillDehRhHistoryInfoReadReq(
  IO BsLkHistoryInfoReadReq  *req
  )
{
  req->varDummy = 0x00;
}

/**
 * @details  This function fills BsLkHistoryInfoWriteReq structure.
 * @param    req : Pointer to BsLkHistoryInfoWriteReq structure
 */
void fillDehRhHistoryInfoWriteReq (
  IN LkHistoryWriteTable       *historyWriteReq,
  IO BsLkHistoryInfoWriteReq   *req
  )
{

  req->varDummy   = 0x00;
  req->varType    = historyWriteReq->Choice;    /* 0- Install Data , 1-Repair Date */
  req->date       = historyWriteReq->Date;
  req->repairCode = historyWriteReq->Code;
}

/**
 * @param    req : Pointer to BsLkOperationalModeInfoReq structure
 * @details  This function fills BsLkOperationalModeInfoReq structure.
 */
void fillDehRhOperationalModeInfoReq(
  IO BsLkOperationalModeInfoReq  *req
  )
{
  req->varDummy   = 0x00;
}


#if 0
//TODO : Remote fusing functionality is not supported in this release

void fillDehRhRemoteFusingReq()
{
}
#endif

void fillDehRhRemoteFusingCompleteAck(
  BsLkRemoteFusingCompleteAck *req
  )
{
  req->varDummy = 0x0000;
}

/**
 * @param    req : Pointer to BsLkResetReq structure
 * @details  This function fills BsLkResetReq structure
 */
void fillDehRhResetReq(
  IO BsLkResetReq  *req
  )
{
  //memset( req, 0, sizeof(BsLkResetReq));
  req->varDummy   = 0x00;
  req->sequenceNo = 0x00;
}

/**
 * @param    req : Pointer to BsLkInitializationAck structure
 * @details  This function fills BsLkInitializationAck structure.
 */
void fillDehRhInitializationAck(
  IO BsLkInitializationAck  *req
  )
{
  req->varDummy = 0;
  req->dataTickTime   = 0x20;
  req->systemId = 2;
  req->alarmAck = 0;
  req->dataDummy = 0;
}

/**
 * @param    req : Pointer to BsLkAlarmInfoReq structure
 * @details  This function fills BsLkAlarmInfoReq structure.
 */
void fillDehRhAlarmInfoReq(
  IN UINT32            seqNum,
  IO BsLkAlarmInfoReq  *req
  )
{
  req->varDummy    = 0;
  req->varSeqNum   = seqNum; /* Reserved */
}

/**
 * @param    req : Pointer to BsLkAlarmChangeAck structure
 * @details  This function fills BsLkAlarmChangeAck structure.
 */
void fillDehRhAlarmChangeAck(
  IN UINT16              seqNoReceivedInNoti,
  IO BsLkAlarmChangeAck  *req
  )
{
  req->varResult     = 0;   /* 0-OK , 1 - Error */
  req->varSeqNum     = seqNoReceivedInNoti;   /* Need to copy from AlarmChangeInfo message */
}

/**
 * @param    req : Pointer to BsLkStatusInfoReq structure
 * @details  This function fills BsLkStatusInfoReq structure.
 */
void fillDehRhStatusInfoReq(
  IN UINT32             *seqNum,
  IO BsLkStatusInfoReq  *req
 )
{
  req->varDummy     = 0;
  req->sequenceNo   = *seqNum;
}

/**
 * @param    txDelay : transmitter delay
 * @param    rxDelay : Receiver Delay
 * @details  This function fills BsLkDelaySettingReq structure.
 */
void fillDehRhDelaySettingReq(
  IO BsLkDelaySettingReq  *req,
  IO RhDefaultCfg   *rhConfig
  )
{
  if ( NULL EQ rhConfig )
  {
	rhConfig = getRhDefaultCfg();
  }

  req->varDummy     = 0;
  req->sequenceNo   = 0;
  req->dataDummy    = 0;
  req->txDelay      = rhConfig->txDelay;
  req->rxDelay      = rhConfig->rxDelay;
}

/**
 * @param    req : Pointer to BsLkTxOnOffReq structure
 * @param    path  Path A or Path B
 * @details  This function fills BsLkTxOnOffReq structure.
 */
void fillDehRhTxOnOffReq(
  IO BsLkTxOnOffReq  *req,
  IN RhPath          *path,
  IN OnOff           *txOnOff
  )
{
  req->varDummy     = 0;
  req->sequenceNo   = 0;
  req->faId         = RH_FA6;
  req->dataDummy1   = 0;
  req->path         = *path;   /* 0-PATH A, 1-PATH B*/
  req->txOnOrOff    = *txOnOff;   /* 0- OFF, 1- ON */
  req->dataDummy2   = 0;
  req->dataDummy3   = 0;
}

/**
 * @param    req : Pointer to BsLkEnableOrDisableReq structure
 * @details  This function fills BsLkEnableOrDisableReq structure.
 */
void fillDehRhEnableReqPathA(
  IO BsLkEnableOrDisableReq  *req
  )
{
  req->varDummy            = 0;
  req->path                = 0;   /* 0-PATH A, 1-PATH B*/
  req->control   = 2;   /* 0- Disable, 1- Enable */
  req->dataDummy           = 0;
}

/**
 * @param    req : Pointer to BsLkEnableOrDisableReq structure
 * @details  This function fills BsLkEnableOrDisableReq structure.
 */
void fillDehRhEnableReqPathB(
  IO BsLkEnableOrDisableReq  *req
  )
{
  req->varDummy            = 0;
  req->path                = 1;   /* 0-PATH A, 1-PATH B*/
  req->control   = 2;   /* 0- Disable, 1- Enable */
  req->dataDummy           = 0;
}

/**
 * @param    req : Pointer to BsLkEnableOrDisableReq structure
 * @details  This function fills BsLkEnableOrDisableReq structure.
 */
void fillDehRhDisableReqPathA(
  IO BsLkEnableOrDisableReq  *req
  )
{
  req->varDummy            = 0;
  req->path                = 0;   /* 0-PATH A, 1-PATH B*/
  req->control   = 1;   /* 1- User Disable, 2- User Enable */
  req->dataDummy           = 0;
}

/**
 * @param    req : Pointer to BsLkEnableOrDisableReq structure
 * @details  This function fills BsLkEnableOrDisableReq structure.
 */
void fillDehRhDisableReqPathB(
  IO BsLkEnableOrDisableReq  *req
  )
{
  req->varDummy            = 0;
  req->path                = 1;   /* 0-PATH A, 1-PATH B*/
  req->control             = 1;   /* 1- User Disable, 2- User Enable 3 - DU Enable */
  req->dataDummy           = 0;
}
/**
 * @param    req : Pointer to BsLkEnableOrDisableReq structure
 * @details  This function fills BsLkEnableOrDisableReq structure.
 */
void fillDehRhEnableOrDisableReqPath(
  IO BsLkEnableOrDisableReq  *req,
  UINT8 path,
  UINT8 control
  )
{
  req->varDummy            = 0;
  req->sequenceNo          = 0;
  req->path                = path;   /* 0-PATH A, 1-PATH B*/
  req->control             = control;   /* 1- Disable, 2- User Enable 3- DU Enable */
  req->dataDummy           = 0;
}
/**
 * @param    req : Pointer to BsLkDlUlRatioReq structure
 * @details  This function fills BsLkDlUlRatioReq structure.
 */
void fillDehRhDlUlRatioReq(
  IO BsLkDlUlRatioReq  *req
  )
{
  req->varDummy            = 0;
  req->dataDummy1          = 0;
  req->dataDummy2          = 0;
  req->dlRatio             = 26;  /* Range is from 26-35*/
  req->ulRatio             = 12;  /* Range is from 12-21*/
}

/**
 * @param    req : Pointer to BsLkDataPathReq structure
 * @details  This function fills BsLkDataPathReq structure by asking from stdin.
 */
void fillDehRhDataPathReq(
  IO  BsLkDataPathReq   *req,
  IO  RhDefaultCfg   *rhConfig
  )
{
  if ( NULL EQ rhConfig )
  {
	rhConfig = getRhDefaultCfg();
  }
  req->varDummy         = 0;
  req->iqMap[0]         = rhConfig->c1IQMap;
  req->iqMap[1]         = 0;
  req->iqMap[2]         = 0;
  req->iqMap[3]         = 0;
  req->dataDummy1       = 0;
  req->dataDummy2       = 0;
  req->dataDummy3       = 0;
  req->dataDummy4       = 0;
  req->dataDummy5       = 0;
  req->numberOfFa       = 1;
  req->dataDummy6       = 0;
  req->mappingOfFa[0]   = 0 ; /* Use FA0 for LTE */
  req->mappingOfFa[1]   = 0 ;
  req->mappingOfFa[2]   = 0 ;
  req->mappingOfFa[3]   = 0 ;
  req->mappingOfFa[4]   = 0 ;
  req->mappingOfFa[5]   = 0 ;
  req->mappingOfFa[6]   = 1 ;
  req->mappingOfFa[7]   = 0 ;
  req->mappingOfFa[8]   = 0 ;
  req->mappingOfFa[9]   = 0 ;
  req->mappingOfFa[10]  = 0 ;
  req->mappingOfFa[11]  = 0 ;
  req->mappingOfFa[12]  = 0 ;
  req->mappingOfFa[13]  = 0 ;
  req->mappingOfFa[14]  = 0 ;
  req->mappingOfFa[15]  = 0 ;
  req->reportPeriod     = 0;
  req->dataDummy7       = 0;
  req->dataDummy8       = 0;
  req->dataDummy9       = 0;
  req->pathAChipDelay   = 0;
  req->pathBChipDelay   = 0;
  req->pathCChipDelay   = 0;
  req->pathDChipDelay   = 0;
  req->dataDummy10      = 0;
}

/**
 * @param    req : Pointer to LkConfigTable structure
 * @details  This function fills BsLkFaControlReq structure.
 */
void fillDehRhFaControlReq(
  IO BsLkFaControlReq       *req,
  IO RhDefaultCfg   *rhConfig
  )
{
  if ( NULL EQ rhConfig )
  {
    rhConfig = getRhDefaultCfg();
  }
  req->varDummy            = 0;
  req->sequenceNo          = 0;
  req->faId                = RH_FA6;
  req->dataDummy1          = 0;
  req->dataDummy2          = 0;
  req->dataDummy3          = 0;
  req->txAttenPathA        = rhConfig->txAttenPathA; /* Range is from 0 to 300 */
  req->txAttenPathB        = rhConfig->txAttenPathB; /* Range is from 0 to 300 */
  req->dataDummy4          = 0;
  req->dataDummy5          = 0;
  req->txEarfcn            = rhConfig->txEarfcn; /* Range is from 0 to 65,535 */
  req->rxEarfcn            = rhConfig->rxEarfcn; /* Range is from 0 to 65,535 */
  req->txPathAOnOff        = 1; /* 0 - OFF, 1- ON */
  req->txPathBOnOff        = 1; /* 0 - OFF, 1- ON */
  req->txPathCOnOff        = 0; /* 0 - OFF, 1- ON */
  req->txPathDOnOff        = 0; /* 0 - OFF, 1- ON */
  req->rxPathAOnOff        = 1; /* 0 - OFF, 1- ON */
  req->rxPathBOnOff        = 1; /* 0 - OFF, 1- ON */
  req->rxPathCOnOff        = 0; /* 0 - OFF, 1- ON */
  req->rxPathDOnOff        = 0; /* 0 - OFF, 1- ON */
  req->dataDummy6          = 0;
  req->dataDummy7          = 0;
  req->dataDummy8          = 0;
  req->dataDummy9          = 0;
  req->dataDummy10         = 0xFFFF;
  req->dataDummy11         = 0xFFFF;
  req->dataDummy12         = 0;
}

/**
 * @param    req : Pointer to lkConfigReq structure
 * @details  This function fills BsLkAlarmThresholdSettingReq structure.
 */
void fillDehRhAlarmThresholdSettingReq(
  IO BsLkAlarmThresholdSettingReq   *req,
  IO RhDefaultCfg   *rhConfig
  )
{
  if ( NULL EQ rhConfig )
  {
	rhConfig = getRhDefaultCfg();
  }
  req->dummy1                   = 0;
  req->sequenceNo               = 0;
  req->dummy2                   = 0;
  req->vswrShutdownThreshold    = rhConfig->vswrShutdownAlarmThreshold;  /* 30 to 100 */
  req->overpowerAlarmThreshold  = rhConfig->overpowerAlarmThreshold; /* 0 to 54,00,00,000 */
  req->lowPowerThreshold        = rhConfig->lowpowerAlarmThreshold;  /* 0 to 54,00,00,000 */
}

/**
 * @param    fANum : FA Number
 * @param    loEvenOrOdd : gives information Carrier Is Even or Odd
 * @details  This function fills BsLkCarrierLOSettingReq structure.
 */
void fillDehRhCarrierLOSettingReq(
  IN RhFANum                   *fANum,
  IN LoSettings                *loEvenOrOdd,
  IO BsLkCarrierLOSettingReq   *req
  )
{
  req->varDummy           = 0;
  req->faNum              = *fANum;
  req->localOscillatorOddOrEven = *loEvenOrOdd; /* 1-ODD or 2-EVEN */
  req->dataDummy1         = 0;
  req->dataDummy2         = 0;
}

/**
 * @param    TimeTick : time tick coming from operator
 * @details  This function fills BsLkTimeSetReq structure.
 */
void fillDehRhTimeSetReq(
  IN TimeTick         *time,
  IO BsLkTimeSetReq   *req
  )
{
  UINT8   i;
  req->varDummy         = 0;
  req->timeTick         = *time;
  for ( i=0; i < 8; i++ )
    req->dataReserved[i] = 0;
}

/**
 * @param    data : MsgPrmDehRhSwInstallReq
 * @param    req  : BsLkSwInstallreq
 * @details  This function fills BsLkSwInstallreq structure.
 */
void fillDehRhSwInstallReq(
  IN MsgPrmDehRhSwInstallReq *data,
  IO BsLkSwInstallreq         *req
  )
{
  DehContext *dehContext = getDehContext();

  req->varDummy   = 0;
  req->sequenceNo = 0;
  getIpToHexValue((UINT8 *)&(dehContext->debugIpAddr), &(req->ipAddr));/*get the IP value in hex format*/
  snprintf(req->fileName, sizeof(String256), data->filename);
  snprintf(req->userName, sizeof(String50), "root");
  snprintf(req->password, sizeof(String50), "root123");
  req->port       = 21;
}

/**
 * @param    data : Contains RH side to activate
 * @param    req  : BsLkSwActivateReq
 * @details  This function fills BsLkSwActivateReq structure.
 */
void fillDehRhSwActivateReq(
  IN UINT8              side,
  IO BsLkSwActivateReq  *req
  )
{
  req->varDummy   = 0;
  req->sequenceNo = 0;
  req->activeSide = side;
}
