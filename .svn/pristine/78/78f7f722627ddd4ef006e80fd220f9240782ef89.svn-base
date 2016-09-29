/**
 * @file     sm_bsmsm_exms.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains Handlers for Messages coming from BSM to SM
 * @author   Madhusudana Rao Kata
 * @date     07-Jun-2011
 * @details  1. sm_sfi_exms.c is divided into sm_bsmsm_exms.c and sm_pmhsm_exms.c @n
 */

#include "sm.h"

/*Local functions*/
static TBOOL isConfigured;

/**
 * @param     srcEnt  Source Entity
 * @param     destEnt Destination Entity
 * @param     event   Event to be sent to OAM Frame work
 * @param     len     Length of the message to be sent to OAM Frame work
 * @param     *data   Data sent to OAM Frame work
 * @details   Process function is used to send events from SM to OAM Frame work
 */

PUBLIC S16 sendEventToOam (
  Ent      srcEnt,
  Ent      dstEnt,
  Event    event,
  MsgLen      len,
  U8      *data
  )
{
  Pst   pst;
  Buffer *mBuf = NULLP;

  if (SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf) != ROK)
  {
    logError (" SGetMsg() failed. Could not send event=%d, srcEnt=%d, dstEnt=%d", event, srcEnt, dstEnt);
    return RFAILED;
  }
  cmMemset((U8*)&pst, 0, sizeof(Pst));

  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = srcEnt;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);
  pst.dstEnt    = dstEnt;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);
  pst.event     = event;

  /* Add data to mBuf here.
   */
  if( (len > 0) && (NULL != data) )
  {
    SAddPstMsgMult((Data *)data, len, mBuf);
  }

  if(SPstTsk(&pst, mBuf) !=ROK)
  {
    logError ("FAILED to send event=%d srcEnt=%d, dstEnt=%d, len=%d",  event, srcEnt, dstEnt, len);
    return RFAILED;
  }
  else
  {
    logInfo ("Post: srcEnt=%d, dstEnt=%d, event=%d, len=%d", srcEnt, dstEnt, event, len);
    return ROK;
  }

}


/**
 * @param     event  Event to be sent to BSM
 * @param     len    Length of the message to be sent to BSM
 * @param     *data  Data sent to BSM
 * @details   Process function is used to send events from SM to BSM
 */
#define sendSmMsgToBsm(event, len, data) sendEventToOam( ENTSM, ENT_BSM, event, len, data )

/**
 * @param     mBuf  Buffer contains SCTP Initial Configuration coming from BSM
 * @details   Process function to SCTP initial configuration
 */
static void smProcSctpInitCfgReq(
  Buffer *mBuf
  )
{
  /* Get the data from mBuf into a flat buffer
   * SCTP config Request  */
  MsgLen                    cCnt;
  MsgBsmSctpInitConfigReq   sctpConfigReq;
  MsgSctpBsmInitConfigSucc  sctpConfigSucc;

  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmSctpInitConfigReq), (Data*)&sctpConfigReq , &cCnt );

  /*BS-1290 Changes*/
  cmInetAddr( (S8*)(sctpConfigReq.enodebIpAddr), &(smCb.cfgCb.enbIpAddr) );
  smCb.cfgCb.enbIpAddr = CM_INET_HTON_U32(smCb.cfgCb.enbIpAddr);
  smCb.sctpTransId = sctpConfigReq.transId;

  /* Send SCTP Config Success */
  sctpConfigSucc.transId = smCb.sctpTransId;
  sendSmMsgToBsm(EVT_SM_BSM_SCTP_CFG_SUCC, sizeof(sctpConfigSucc), (U8*)&sctpConfigSucc);
}

/**
 * @param     mBuf  Buffer contains EGTP Initial Configuration coming from BSM
 * @details   This function saves the received Message in String format into a 32 bit number Sends EGTP CFG success to BSM
 */
static void smProcEgtpInitCfgReq(
  Buffer *mBuf
  )
{
  /* Get the data from mBuf into a flat buffer EGTP config Request  */
  MsgLen                    cCnt;
  MsgBsmEgtpInitConfigReq   egtpConfigReq;
  MsgEgtpBsmInitConfigSucc  egtpConfigSucc;

  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmEgtpInitConfigReq), (Data*)&egtpConfigReq , &cCnt );

  /*BS-1290 Changes*/
  cmInetAddr( (S8*)(egtpConfigReq.enodebIpAddr), &(smCb.cfgCb.enbIpAddr) );
  smCb.cfgCb.enbIpAddr = CM_INET_HTON_U32(smCb.cfgCb.enbIpAddr);
  smCb.egtpTransId = egtpConfigReq.transId;

  /* Send EGTP Config Success */
  egtpConfigSucc.transId = smCb.egtpTransId;
  sendSmMsgToBsm(EVT_SM_BSM_EGTP_CFG_SUCC, sizeof(egtpConfigSucc), (U8*)&egtpConfigSucc);
}

/**
 * @param     mBuf  Buffer contains S1AP Initial Configuration coming from BSM
 * @details   This function saves the received S1AP configuration from BSM
 */
static void smProcS1apInitCfgReq(
  Buffer *mBuf
  )
{
  /* Get the data from mBuf into a flat buffer
   * The message is in PWAV Format, so skip the header and decode the remaining message to
   * S1AP config Request, Save the configuration
   * Save the enodeB IP Address received */
  MsgLen               cCnt;
  MsgBsmS1apInitConfigReq   s1apConfigReq;
  MsgS1apBsmInitConfigSucc  s1apConfigSucc;

  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmS1apInitConfigReq), (Data*)&s1apConfigReq , &cCnt );

  cmInetAddr( (S8*)(s1apConfigReq.enodebStaticIpAddr), &(smCb.cfgCb.enbIpAddr));
  smCb.cfgCb.enbIpAddr = CM_INET_HTON_U32(smCb.cfgCb.enbIpAddr);
  smCb.s1apTransId = s1apConfigReq.transId;

  /* Send S1AP Config Success */
  s1apConfigSucc.transId = smCb.s1apTransId;
  sendSmMsgToBsm(EVT_SM_BSM_S1AP_CFG_SUCC, sizeof(s1apConfigSucc), (U8*)&s1apConfigSucc);
}

/**
 * @param     mBuf  Buffer contains UEH Configuration coming from BSM
 * @details   This function saves the received UEH configuration received from BSM
 */
void saveUehCfgInSmCb(
  MsgBsmUehEnbConfigReq   *enbConfigReq,
  SmCfgCb                 *smCfgCallBlock
  )
{
  /* CELL Configuration  */
  smCfgCallBlock->cellId      = enbConfigReq->cellCfg.cellId;
  smCfgCallBlock->maxUeSupp   = enbConfigReq->cellCfg.maxConnectedUes;
  smCfgCallBlock->mcc[0].pres = TRUE;
  smCfgCallBlock->mcc[0].val  = enbConfigReq->cellCfg.plmnMCC[0];
  smCfgCallBlock->mcc[1].pres = TRUE;
  smCfgCallBlock->mcc[1].val  = enbConfigReq->cellCfg.plmnMCC[1];
  smCfgCallBlock->mcc[2].pres = TRUE;
  smCfgCallBlock->mcc[2].val  = enbConfigReq->cellCfg.plmnMCC[2];
  smCfgCallBlock->mnc[0].pres = TRUE;
  smCfgCallBlock->mnc[0].val  = enbConfigReq->cellCfg.plmnMNC[0];
  smCfgCallBlock->mnc[1].pres = TRUE;
  smCfgCallBlock->mnc[1].val  = enbConfigReq->cellCfg.plmnMNC[1];
  smCfgCallBlock->mnc[2].pres = TRUE;
  smCfgCallBlock->mnc[2].val  = enbConfigReq->cellCfg.plmnMNC[2];

  smCfgCallBlock->trackAreaCode = enbConfigReq->cellCfg.tac;
  smCfgCallBlock->freqBandInd = enbConfigReq->cellCfg.freqBandInd;
}
/**
 * @param     mBuf  Buffer contains CEM  Configuration coming from BSM
 * @details   This function saves the received CEM configuration received from BSM
 */
void cemProcEnbCfgReq(
  Buffer *mBuf,
  Pst    *pst
  )
{
  MsgBsmUehEnbConfigReq   enbConfigReq;
  MsgUehBsmEnbConfigSucc  enbCfgSucc;
  MsgLen                  cCnt;
  UehCellCb *cb         = uehGetCellCb();

  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmUehEnbConfigReq), (Data*)&enbConfigReq , &cCnt );

 /*Send the message to UEH*/
  smCb.enbTransId = enbConfigReq.transId;
  uehStoreRcvdCellCfg(mBuf);
  if( CELLBARRED_BARRED == enbConfigReq.cellBarred )
     cb->isUnbarred = FALSE; //crmUpdateCellState(UEH_CELL_STATE_BARRED);
  else 
     cb->isUnbarred = TRUE;

   enbCfgSucc.transId = enbConfigReq.transId;
   saveUehCfgInSmCb( &enbConfigReq, &(smCb.cfgCb));
  /* Now we can change the state of SM,
   * We can go ahead and configure all the stack layers.
   * Invoke the state machine only once, for the first time
   */
  if( FALSE == isConfigured)
  {
    smCb.smState =UEH_SM_STATE_INIT;
    smUehProcSm(EVTUEHTUCLCFG);
    isConfigured = TRUE;
  }
  else
  {
   sendCemMsgToBsm(EVT_CEM_BSM_CFG_SUCC, sizeof(enbCfgSucc), (U8*)&enbCfgSucc);
  }

}

/* Extract the message and invoke cell fsm */

void cemProcOpstopCmd(Buffer   *mBuf)
{

  MsgBsmModOpStopReq opStopReq;
  S16                n;
  UehCellCb         *cb=uehGetCellCb();

  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmModOpStopReq), (Data*)&opStopReq , &n );
  crmInvokeCellSm( cb, CELL_EVT_OP_STOP, &opStopReq );
  uehCb.lastRcvdSfn = -1;
}

//###############################################
/*@param        currentSfn This contains the current SFN of the cell
 *              modiPeriodTmr this contains modification period when new SIBs will be Tx
 *@details      This function calculate the SFN in which new SIBs will be Tx
  @return value SFN in which new SIBs will Tx
 *
*/
static UINT16 uehFillNewSibTxSfn (
  UINT16 currentsfn, 
  UINT16 modiPeriodTmr
  )
{
  UINT16 newSfn;
  /** if cellCb->sfn = C, cellCb->modiPeriodTmr = T 
  * then sfnNewSIBTx = ((C - (C % T)) + T) % 1024*/
  newSfn = (currentsfn - (currentsfn % modiPeriodTmr) + (modiPeriodTmr)) % UEH_MAX_SFN; 
  RETVALUE (newSfn);
}
/** BS-1799
 * @param     mBuf  Buffer contains Cell Barring information from BSM
 * @details   This function intiates the cell barring procedure. 
 *            Steps
 *            1) Updates the cell State to UEH_CELL_STATE_BARRED
 *            2) Updates the isSysInfoNeedToSend to TRUE to indicate system information is changed
 *            3) Fills MsgUehBsmCellBarSucc message and sends response to BSM
 * */
void cemProcCellBarReq(
  Buffer *mBuf
  )
{
  MsgBsmUehCellBarReq     cellBarReq;
  MsgUehBsmCellBarSucc    cellBarSucc;  
  MsgLen                  cCnt;
  UehCellCb               *cellCb = NULLP;
  
  clearBytes(&cellBarSucc, sizeof(MsgUehBsmCellBarSucc));
  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmUehCellBarReq), (Data*)&cellBarReq, &cCnt );
  
  logInfo ("Cell Bar request received from BSM for cell Id %d", cellBarReq.cellId);
  cellCb = uehGetCellCbByCellId(cellBarReq.cellId);
  if (cellCb EQ NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(CELL,  cellBarReq.cellId, EVT_BSM_CEM_CELL_BAR_REQ, ENT_BSM);
    return;
  }
  /*Assuming BSM do not send cell bar request if cell is already barred*/

  cellCb->isUnbarred            = FALSE;
  /*Indicates system info chenged used to set systemInfoModification IE of paging message*/
  cellCb->isSysInfoNeedToSend   = TRUE;
  uehCb.modiUehCfgCb.cellBarred = CELLBARRED_BARRED;
  //uehCb.uehCfgCb.cellBarred = CELLBARRED_BARRED; 

  /*Barring the cell and this change will be reflected in SIB1 after current modification beriod*/
  cellBarSucc.transId         = cellBarReq.transId;
  cellBarSucc.sfnReceived     = cellCb->sfn;
  cellBarSucc.sfnNewSIBTx     = uehFillNewSibTxSfn(cellCb->sfn, cellCb->modiPeriodTmr);

  logProc("Cell-Bar | Req Rx | cell %d | SFN %d | New sib will go in SFN %d", 
         cellBarReq.cellId,
         cellBarSucc.sfnReceived,
         cellBarSucc.sfnNewSIBTx);

  logInfo ("Cell-Bar | Rsp Tx to BSM for cell Id %d", cellBarReq.cellId);
  sendUehMsgToBsm(EVT_CEM_BSM_CELL_BAR_SUCC, sizeof (MsgUehBsmCellBarSucc), (UINT8 *)&cellBarSucc);
}

/** BS-1799
 * @param     mBuf  Buffer contains Cell UnBarring information from BSM
 * @details   This function intiates the cell unbarring procedure. 
 *            Steps
 *            1) Updates the cell State to UEH_CELL_STATE_AVAILABLE
 *            2) Updates the isSysInfoNeedToSend to TRUE to indicate system information is changed
 *            3) Fills MsgUehBsmCellUnbarSucc message and sends response to BSM
 */
void cemProcCellUnbarReq(
  Buffer *mBuf
  )
{
  MsgBsmUehCellUnbarReq     cellUnbarReq;
  MsgUehBsmCellUnbarSucc    cellUnbarSucc;
  MsgLen                    cCnt;
  UehCellCb                 *cellCb = NULLP;

  logFunc ("%s", "cemProcCellUnbarReq");
  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmUehCellBarReq), (Data*)&cellUnbarReq, &cCnt );

  logInfo ("BS-1799 Cell Unbar request received from BSM for cell Id %d", cellUnbarReq.cellId);
  cellCb = uehGetCellCbByCellId(cellUnbarReq.cellId);
  if (cellCb EQ NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(CELL,  cellUnbarReq.cellId, EVT_BSM_CEM_CELL_UNBAR_REQ, ENT_BSM);
    return;
  }
  /* Indicate system info change is needed. Affects systemInfoModification IE of paging message*/
  cellCb->isSysInfoNeedToSend   = TRUE;
  cellCb->isUnbarred            = TRUE;
  uehCb.modiUehCfgCb.cellBarred = CELLBARRED_NOTBARRED; 
  //uehCb.uehCfgCb.cellBarred     = CELLBARRED_NOTBARRED; 
  /*Constructing the cell UnBar Success message */
  cellUnbarSucc.transId         = cellUnbarReq.transId;
  cellUnbarSucc.sfnReceived     = cellCb->sfn;
  cellUnbarSucc.sfnNewSIBTx     = uehFillNewSibTxSfn(cellCb->sfn, cellCb->modiPeriodTmr);
  logProc("Cell Unbar| request rx in SFN %d | Will be sending to RRC in SFN %d", cellUnbarSucc.sfnReceived, cellUnbarSucc.sfnNewSIBTx);
  /*Barring the cell and this change will be reflected in SIB1 after current modification beriod*/
  logInfo ("Cell Unbar responce sent to BSM for cell Id %d", cellUnbarReq.cellId);
  sendUehMsgToBsm(EVT_CEM_BSM_CELL_UNBAR_SUCC, sizeof (MsgUehBsmCellUnbarSucc), (UINT8 *)&cellUnbarSucc);
}

/**
 * @param     mBuf contains SetLogCfg message structure
 * @details   This function processes the Log configurations received from BSM
 */
void smProcSetLogCfg(
  Buffer *mBuf
  )
{
  SetLogConfig updateLogCfg; //&((UpdModLogConfig *)mBuf)->logTableEntry;
  MsgLen    mesgLen=0;

  SCpyMsgFix( mBuf, 0, sizeof(SetLogConfig), (Data*)&updateLogCfg, &mesgLen);
  logMsgInParams( " Received ModuleId: %s \t logMask1=%d",getModuleName(updateLogCfg.moduleId), updateLogCfg.logMask1);
  switch (updateLogCfg.moduleId)
  {
    case MODID_EGTP:
      smBuildEgDbgCntrl( &updateLogCfg );
      break;
    case MODID_TUCL:
      smBuildHiDbgCntrl( &updateLogCfg );
      break;
    /* RLC and PDCP use same DBG-Mask for its logging */
    case MODID_RLC:
    case MODID_PDCP:
      smBuildKwDbgCntrl( &updateLogCfg );
      break;
    case MODID_RRC:
      smBuildNhDbgCntrl( &updateLogCfg );
      break;
    case MODID_SCTP:
      smBuildSbDbgCntrl( &updateLogCfg );
      break;
    case MODID_MAC:
      /* Log level for YS is same as for MAC */
      smBuildRgDbgCntrl( &updateLogCfg );
      smBuildYsDbgCntrl( &updateLogCfg );
      break;
    case MODID_S1AP:
      smBuildSzDbgCntrl( &updateLogCfg );
      break;
    case MODID_UEH:
      smBuildUehDbgCntrl( &updateLogCfg );
      break;
    case MODID_UDH:
      smBuildUdhDbgCntrl( &updateLogCfg );
      break;
    default:
      logError("smProcSetLogCfg: default: Unknown module ID received :%d", updateLogCfg.moduleId);
      return;
  }
}

/**
 * @param     pst   post structure contains source and destinations information
 * @param     mBuf  Buffer contains message coming from BSM
 * @details   Entry point to the messages received from BSM to SM
 */
PUBLIC S16 bsmSmActvTsk(
  Pst         *pst,
  Buffer      *mBuf
  )
{
   S16 ret;
   ret = ROK;
   switch(pst->event)
   {
      case EVT_BSM_SM_SCTP_CFG_REQ:
         smProcSctpInitCfgReq(mBuf);
         SPutMsg(mBuf);
         break;

      case EVT_BSM_SM_EGTP_CFG_REQ:
        smProcEgtpInitCfgReq(mBuf);
        SPutMsg(mBuf);
        break;

      case EVT_BSM_SM_S1AP_CFG_REQ:
        smProcS1apInitCfgReq(mBuf);
        SPutMsg(mBuf);
        break;


      case EVT_BSM_SM_SET_LOG_CFG:
        smProcSetLogCfg(mBuf);
        SPutMsg(mBuf);
        break;

      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("%s","Received invalid event\n");
         break;
   }
   RETVALUE(ret);
}

