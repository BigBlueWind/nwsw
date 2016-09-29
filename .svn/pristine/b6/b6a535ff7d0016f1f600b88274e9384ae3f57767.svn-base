/**
 * @file    rh_enc.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains encoding functions for messages sent to LK from BS.
 *
 * @author    mkata, spolepally, aagrawal, kgummad, m_sharma
 * @date      27-05-2011
 * @details   Change History: Basic version
 *
 */

#include "rh_enc.h"

/**
 * @details  This function will encode header data.
 * @param    LkHdr : Header structure
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhHeader (
  IN LkHdr      *hdr,
  IO EncBuf     *buf
  )
{
  putUint16( hdr->dstMailbox, buf );
  putUint16( hdr->srcMailbox, buf );
  putUint32( hdr->msgId,      buf );
}

/**
 * @details  This function encodes DehRhHwInfoReq message
 * @param    BsLkHwInfoReq : REQ structure input structure
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhHwInfoReq(
  IN BsLkHwInfoReq   *req,
  IO EncBuf          *encBuf
  )
{
  putUint32( req->varDummy, encBuf );
}

/**
 * @details  This function encodes DehRhFwInfoReq message
 * @param    BsLkFwInfoReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhFwInfoReq(
  IN BsLkFwInfoReq   *req,
  IO EncBuf          *encBuf
  )
{
  putUint32( req->varDummy, encBuf );
}

/**
 * @details  This function encodes DehRhHistoryInfoReadReq message
 * @param    BsLkHistoryInfoReadReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhHistoryInfoReadReq(
  IN BsLkHistoryInfoReadReq   *req,
  IO EncBuf                   *encBuf
  )
{
  putUint32( req->varDummy,   encBuf );
}

/**
 * @details  This function encodes DehRhHistoryInfoWriteReq message
 * @param    BsLkHistoryInfoWriteReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhHistoryInfoWriteReq (
  IN BsLkHistoryInfoWriteReq   *req,
  IO EncBuf                    *encBuf
  )
{
  putUint16( req->varDummy,   encBuf );
  putUint16( req->varType,    encBuf );
  putUint32( req->date,       encBuf );
  putUint32( req->repairCode, encBuf );
}

/**
 * @details  This function encodes DehRhOperationalModeInfoReq message
 * @param    BsLkOperationalModeInfoReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhOperationalModeInfoReq(
  IN BsLkOperationalModeInfoReq   *req,
  IO EncBuf                       *encBuf
  )
{
  putUint32( req->varDummy,   encBuf );
}

/**
 * @details  This function encodes DehRhRemoteFusingReq message
 * @param    BsLkRemoteFusingReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhRemoteFusingReq(
  IN BsLkRemoteFusingReq   *req,
  IO EncBuf                *encBuf
  )
{
  if ( req->varFlag EQ FLAG_REMOTE_FUSING_START )  /* Start Req */
  {
    putUint16( req->reqMsg.startReq.varImageSize, encBuf );
  }
  else if ( req->varFlag EQ FLAG_REMOTE_FUSING_DATA ) /* Transmission Req*/
  {
    putUint16( req->reqMsg.transmissionReq.varDataNum, encBuf );
  }
  else if ( (req->varFlag EQ FLAG_REMOTE_FUSING_TERM_COPY_RESTART ) ||
            (req->varFlag EQ FLAG_REMOTE_FUSING_TERM_COPY ) ||
            (req->varFlag EQ FLAG_REMOTE_FUSING_TERM_ABORT ) )
  /* Termination Req */
  {
    putUint16( req->reqMsg.terminationReq.varImageMod, encBuf );
  }
  else /* Incase of Wrong flag for Fusion Info */
  {
    logError("Invalid Flag for Remote Fusing Req msg  FlagId=%d", req->varFlag );
  }
}

/**
 * @details  This function encodes DehRhRemoteFusingCompleteAck message
 * @param    BsLkRemoteFusingCompleteAck : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhRemoteFusingCompleteAck(
  IN BsLkRemoteFusingCompleteAck   *req,
  IO EncBuf                        *encBuf
  )
{
  putUint32( req->varDummy,encBuf );
}

/**
 * @details  This function encodes DehRhResetReq message
 * @param    BsLkResetReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhResetReq(
  IN BsLkResetReq   *req,
  IO EncBuf         *encBuf
  )
{
  putUint16( req->varDummy,   encBuf );
  putUint16( req->sequenceNo, encBuf );
}

/**
 * @details  This function encodes DehRhInitializationAck message
 * @param    BsLkInitializationAck : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhInitializationAck(
  IN BsLkInitializationAck   *req,
  IO EncBuf                  *encBuf
  )
{
  putUint32( req->varDummy,   encBuf );
  putUint32( req->dataTickTime,   encBuf );
  putUint8( req->systemId,   encBuf );
  putUint8( req->alarmAck,   encBuf );
  putUint16( req->dataDummy,   encBuf );

}

/**
 * @details  This function encodes DehRhAlarmInfoReq message
 * @param    BsLkAlarmInfoReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhAlarmInfoReq(
  IN BsLkAlarmInfoReq   *req,
  IO EncBuf             *encBuf
  )
{
  putUint16( req->varDummy,     encBuf );
  putUint16( req->varSeqNum,   encBuf );
}

/**
 * @details  This function encodes DehRhAlarmChangeAck message
 * @param    BsLkStatusInfoReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhAlarmChangeAck(
  IN BsLkAlarmChangeAck   *req,
  IO EncBuf               *encBuf
  )
{
  putUint16( req->varResult,   encBuf );
  putUint16( req->varSeqNum,   encBuf );
}

/**
 * @details  This function encodes DehRhStatusInfoReq message
 * @param    BsLkStatusInfoReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhStatusInfoReq(
  IN BsLkStatusInfoReq   *req,
  IO EncBuf             *encBuf
  )
{
  putUint16( req->varDummy,     encBuf );
  putUint16( req->sequenceNo,   encBuf );
}

/**
 * @details  This function encodes DehRhDelaySettingReq message
 * @param    BsLkDelaySettingReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhDelaySettingReq(
  IN BsLkDelaySettingReq   *req,
  IO EncBuf                *encBuf
  )
{
  putUint16( req->varDummy, encBuf );
  putUint16( req->sequenceNo, encBuf );
  putUint32( req->dataDummy,encBuf );
  putUint32( req->txDelay,  encBuf );
  putUint32( req->rxDelay,  encBuf );
}

/**
 * @details  This function encodes DehRhTxOnOffReq message
 * @param    BsLkTxOnOffReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.

 */
void encDehRhTxOnOffReq(
  IN BsLkTxOnOffReq   *req,
  IO EncBuf           *encBuf
  )
{
  putUint16( req->varDummy,     encBuf );
  putUint16( req->sequenceNo,   encBuf );
  putUint8( req->faId,          encBuf );
  putUint8(  req->dataDummy1,    encBuf );
  putUint8(  req->path,         encBuf );
  putUint8(  req->txOnOrOff,    encBuf );
  putUint8( req->dataDummy2,    encBuf );
  putUint16( req->dataDummy3,   encBuf );
}

/**
 * @details  This function encodes DehRhEnableOrDisableReq message
 * @param    BsLkEnableOrDisableReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhEnableOrDisableReq(
  IN BsLkEnableOrDisableReq   *req,
  IO EncBuf                   *encBuf
  )
{
  putUint16( req->varDummy,          encBuf );
  putUint16( req->sequenceNo,        encBuf );
  putUint8(  req->path,              encBuf );
  putUint8(  req->control,           encBuf );
  putUint16( req->dataDummy,         encBuf );
}

/**
 * @details  This function encodes DehRhDlUlRatioReq message
 * @param    BsLkDlUlRatioReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhDlUlRatioReq(
  IN BsLkDlUlRatioReq   *req,
  IO EncBuf             *encBuf
  )
{
  putUint32( req->varDummy,         encBuf );
  putUint8( req->dataDummy1,        encBuf );
  putUint8( req->dataDummy1,        encBuf );
  putUint8( req->dlRatio,           encBuf );
  putUint8( req->ulRatio,           encBuf );
}

/**
 * @details  This function encodes DehRhDataPathReq message
 * @param    BsLkDataPathReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhDataPathReq(
  IN BsLkDataPathReq   *req,
  IO EncBuf             *encBuf
  )
{
  UINT8 index = 0;
  putUint16( req->varDummy,   encBuf );
  putUint16( req->sequenceNo, encBuf );
  putUint8( req->iqMap[0],    encBuf );

  for(index = 1 ;index < MAX_CPRI_PORTS; index++ )
  {
    putUint8( req->iqMap[index],     encBuf );
  }

  putUint8( req->dataDummy1,     encBuf );
  putUint8( req->dataDummy2,     encBuf );
  putUint8( req->dataDummy3,     encBuf );
  putUint8( req->dataDummy4,     encBuf );
  putUint8( req->dataDummy5,     encBuf );
  putUint8( req->numberOfFa,     encBuf );
  putUint16( req->dataDummy6,    encBuf );
  putUint8( req->mappingOfFa[0], encBuf );

  for(index = 1 ;index < MAX_FAS; index++ )
  {
    putUint8( req->mappingOfFa[index],encBuf );
  }

  putUint16( req->reportPeriod, encBuf );
  putUint16( req->dataDummy7,   encBuf );
  putUint8( req->dataDummy8,    encBuf );
  putUint16( req->dataDummy9,   encBuf );
  putUint8( req->pathAChipDelay,  encBuf );
  putUint8( req->pathBChipDelay,  encBuf );
  putUint8( req->pathCChipDelay,  encBuf );
  putUint8( req->pathDChipDelay,  encBuf );
  putUint32( req->dataDummy10,    encBuf );
}

/**
 * @details  This function encodes DehRhFaControlReq message
 * @param    BsLkFaControlReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhFaControlReq(
  IN BsLkFaControlReq   *req,
  IO EncBuf             *encBuf
  )
{
  putUint16( req->varDummy,       encBuf );
  putUint16( req->sequenceNo,     encBuf );
  putUint8( req->faId,            encBuf );
  putUint8( req->dataDummy1,      encBuf );
  putUint8( req->dataDummy2,      encBuf );
  putUint8( req->dataDummy3,      encBuf );
  putUint16( req->txAttenPathA,   encBuf );
  putUint16( req->txAttenPathB,   encBuf );
  putUint16( req->dataDummy4,     encBuf );
  putUint16( req->dataDummy5,     encBuf );
  putUint32( req->txEarfcn,       encBuf );
  putUint32( req->rxEarfcn,       encBuf );
  putUint8( req->txPathAOnOff,    encBuf );
  putUint8( req->txPathBOnOff,    encBuf );
  putUint8( req->txPathCOnOff,    encBuf );
  putUint8( req->txPathDOnOff,    encBuf );
  putUint8( req->rxPathAOnOff,    encBuf );
  putUint8( req->rxPathBOnOff,    encBuf );
  putUint8( req->rxPathCOnOff,    encBuf );
  putUint8( req->rxPathDOnOff,    encBuf );
  putUint8( req->dataDummy6,      encBuf );
  putUint8( req->dataDummy7,      encBuf );
  putUint8( req->dataDummy8,      encBuf );
  putUint8( req->dataDummy9,      encBuf );
  putUint16( req->dataDummy10,     encBuf );
  putUint16( req->dataDummy11,     encBuf );
  putUint16( req->dataDummy12,     encBuf );

}

/**
 * @details  This function encodes DehRhAlarmThresholdSettingReq message
 * @param    BsLkAlarmThresholdSettingReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhAlarmThresholdSettingReq(
  IN BsLkAlarmThresholdSettingReq   *req,
  IO EncBuf                         *encBuf
  )
{
  putUint16( req->dummy1,                  encBuf );
  putUint16( req->sequenceNo,              encBuf );
  putUint16( req->dummy2,                  encBuf );
  putUint16( req->vswrShutdownThreshold,   encBuf );
  putUint32( req->overpowerAlarmThreshold, encBuf );
  putUint32( req->lowPowerThreshold,       encBuf );
}

/**
 * @details  This function encodes BsLkCarrierLOSettingReq message
 * @param    BsLkCarrierLOSettingReq :  REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhCarrierLOSettingReq(
  IN BsLkCarrierLOSettingReq   *req,
  IO EncBuf                    *encBuf
  )
{
  putUint32( req->varDummy,                encBuf );
  putUint8( req->faNum,                    encBuf );
  putUint8( req->localOscillatorOddOrEven, encBuf );
  putUint8( req->dataDummy1,               encBuf );
  putUint8( req->dataDummy2,               encBuf );
}

/**
 * @details  This function encodes DehRhTimeSetReq message
 * @param    BsLkTimeSetReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhTimeSetReq(
  IN BsLkTimeSetReq   *req,
  IO EncBuf           *encBuf
  )
{
  putUint32( req->varDummy,   encBuf );
  putUint32( req->timeTick,   encBuf );
}

/**
 * @details  This function encodes DehRhSwInstallReq message
 * @param    BsLkSwInstallreq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhSwInstallReq(
  IN BsLkSwInstallreq *req,
  IO EncBuf           *encBuf
  )
{
  putUint16(req->varDummy   , encBuf);
  putUint16(req->sequenceNo , encBuf);
  putUint32(req->ipAddr     , encBuf);
  putBytes((req->fileName)  , sizeof(String256) , encBuf);
  putBytes((req->userName)  , sizeof(String50)  , encBuf);
  putBytes((req->password)  , sizeof(String50)  , encBuf);
  putUint16(req->port       , encBuf);
}

/**
 * @details  This function encodes DehRhSwActivateReq message
 * @param    BsLkSwActivateReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhSwActivateReq(
  IN BsLkSwActivateReq *req,
  IO EncBuf           *encBuf
  )
{
  putUint16(req->varDummy   ,  encBuf);
  putUint16(req->sequenceNo ,  encBuf);
  putUint8(req->activeSide  ,  encBuf);
}
