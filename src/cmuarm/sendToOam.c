/**
 * @file     SendToOam.c
 * @details  Copyright Powerwave Technologies, Inc., 2011
 * @brief    This file contains function to send event to OAM
 *
 * @author   Jyotirling Ugaji
 * @date:    22/12/2011
 * @details: Initial Version
 **/

#include "sendToOam.h"
#include "log.h"
#include "ueh_alias.h"



PUBLIC S16 sendEventToOam (
  Ent      srcEnt,
  Ent      dstEnt,
  Event    event,
  MsgLen     len,
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
  clearBytes((U8*)&pst, sizeof(Pst));

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
  logInfo ("-> srcEnt=%d, dstEnt=%d, event=%d, len=%d", srcEnt, dstEnt, event, len);
  return ROK;
}

