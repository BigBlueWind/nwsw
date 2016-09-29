/**
 * @file    bsm_set_pre_cfg_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will handle all the set messages on Pre-Configuration table.
 *
 * @author:  Shweta Polepally
 * @date:    Nov 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   msgTraceEntry   Message trace entry
 * @details This function will set the tracing information as requested for the message.
 */
static void bsmProcSetMsgTraceTableInBsm(
  IN MsgTrace     *msgTraceEntry
  )
{
  MsgDesc   *msgDesc  = getMsgDesc(msgTraceEntry->msgId);
  msgDesc->msgTraceLevel     = msgTraceEntry->msgTraceLevel;
  msgDesc->msgTraceDirection = msgTraceEntry->msgTraceDirection;
}

/**
 * @param   modId           Module Id
 * @param   msgTraceEntry   Message trace entry
 * @details This function will send the SET_MSG_TRACE message to the module specified in modId
 *          field.
 */
static void bsmSendSetMsgTraceReqMsg(
  IN ModuleId      modId,
  IN MsgTrace     *msgTraceEntry
  )
{
  MsgId  msgId;

  switch( modId )
  {
    case MODID_BSM:
      bsmProcSetMsgTraceTableInBsm( msgTraceEntry );
      return;
    case MODID_FTH:
      msgId = MSG_BSM_FTH_SET_MSG_TRACE;
      break;
    case MODID_UEH:
      msgId = MSG_BSM_UEH_SET_MSG_TRACE;
      break;
    case MODID_PRM:
      msgId = MSG_BSM_PRM_SET_MSG_TRACE;
      break;
    case MODID_DEH:
      msgId = MSG_BSM_DEH_SET_MSG_TRACE;
      break;
    case MODID_LTH:
      msgId = MSG_BSM_LTH_SET_MSG_TRACE;
      break;
    case MODID_CIH1:
      msgId = MSG_BSM_CIH1_SET_MSG_TRACE;
      break;
    case MODID_CIH2:
      msgId = MSG_BSM_CIH2_SET_MSG_TRACE;
      break;
    case MODID_CIH3:
      msgId = MSG_BSM_CIH3_SET_MSG_TRACE;
      break;
 
    case MODID_WIH:
      msgId = MSG_BSM_WIH_SET_MSG_TRACE;
      break;
    case MODID_SIH:
      msgId = MSG_BSM_SIH_SET_MSG_TRACE;
      break;
    default:
      BS_ERROR_UNKNOWN_MOD_ID( modId );
      break;
  }
  /* Send the message to module */
  createAndSendMsg( msgId, (UINT8*)msgTraceEntry, sizeof(MsgTrace) );
}

/**
 * @param   setObj Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on Message Trace table as per the SET request.
 */
TBOOL bsmPerformOperationOnMsgTrace(
  IN SetObjReq   *setObj
  )
{
  UINT8          index=0;
  MsgTrace       *msgTraceEntry;
  TlvInfo        *tlv= NULL;
  UINT32          msgIdCount = 0;
  MsgId           msgIdArray[MSG_ID_MAX];
  clearBytes( &msgIdArray, MSG_ID_MAX );

  for ( index=0; index < setObj->tlvArray.numTlvs; index++ )
  {
    tlv = (TlvInfo*)&(setObj)->tlvArray.tlvInfo[index];
    if( tlv->tag.rowId > MSG_ID_MAX )
    {
      BS_ERROR_UNDEFINED_MSGTRACE_MSG_ID();
      bsmFillAndSendSetObjFail( setObj, BS_ERR_UNDEFINED_MSG_ID, &tlv->tag );
      return FALSE;
    }
    switch( tlv->tag.colId )
    {
      case COL_MSGTRACE_MSGID:
        {
          /* Store the MsgId and Silently ignore */
          msgIdArray[msgIdCount] = getUint32FromArray(tlv->data);
          msgIdCount++;
        }
        break;
      case COL_MSGTRACE_MSGTRACELEVEL:
        {
          msgTraceEntry = (MsgTrace*)getRowOfTable( MSGTRACE, (tlv->tag.rowId-1) );
          msgTraceEntry->msgTraceLevel = getUint32FromArray(tlv->data);
        }
        break;
      case COL_MSGTRACE_MSGTRACEDIRECTION:
        {
          msgTraceEntry = (MsgTrace*)getRowOfTable( MSGTRACE, (tlv->tag.rowId-1) );
          msgTraceEntry->msgTraceDirection  = getUint32FromArray(tlv->data);
        }
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        bsmFillAndSendSetObjFail( setObj, BS_ERR_INVALID_COLID_RCVD, &tlv->tag );
        return FALSE;
    }
  }
  logInfo("%s","MsgTrace Table has been updated");

  /* Send Success And Take action */
  bsmFillAndSendSetObjSucc( setObj );

  /* Take action - Send SET Trace Request messages to modules checking teh message ID */
  MsgDesc      *msgDesc;
  MsgId         msgId;
  BsErrorValue  bsErrorValue = BS_NO_ERROR;

  for ( index=0; index < msgIdCount; index++ )
  {
    msgId = msgIdArray[index];
    if( msgId > MSG_ID_MAX )
    {
      BS_ERROR_UNKNOWN_MSG( msgId );
      continue;
    }

    msgTraceEntry = (MsgTrace*)getRowOfTable( MSGTRACE, (msgId-1) );  /* Note : here msgid itself acts as rowId */
    msgDesc  = getMsgDesc( msgId );

    /* Check if the print function is not available for the msgId and traceLevel = withParams */
    if( ( msgTraceEntry->msgTraceLevel EQ MSGTRACELEVEL_MSGWITHPARAMS ) && ( NULL EQ msgDesc->printFunc ) )
    {
      bsErrorValue = BS_ERR_NO_PRINT_FUNCTION_DEFINED;
      /* Log the failure condition and
       * Dont send SET_FAIL
       * Continue for other messages */
      BS_ERROR_NO_PRINT_FUNCTION_DEFINED( getMsgName(msgId) );
      continue;
    }
    bsmSendSetMsgTraceReqMsg( msgDesc->src, msgTraceEntry );
    bsmSendSetMsgTraceReqMsg( msgDesc->dest, msgTraceEntry );
  }
  return TRUE;
}

/**
 * @param   setObj  Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on Alarm Thresholds as per the SET request.
 */
TBOOL bsmPerformOperationOnEventThresholds(
  IO SetObjReq   *setObj
  )
{
  GenericTable     *tableFunc = getTableFunctions(EVENTTHRESHOLDS);
  EventThresholds  *rowOfTable = (EventThresholds*)getFirstRowOfTable(EVENTTHRESHOLDS);

  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, rowOfTable );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "EVENTTHRESHOLDS" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}

/**
 * @param   setObj   Set Object Request sent by CIH/WIH/SIH
 * @details This function will perform operation on Factory Settings as per the SET request.
 */
TBOOL bsmPerformOperationOnEnbAddresses(
  IO SetObjReq   *setObj
  )
{
  GenericTable     *tableFunc = getTableFunctions(ENBADDRESSES);
  EnbAddresses     *rowOfTable =(EnbAddresses*) getFirstRowOfTable(ENBADDRESSES);

  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, rowOfTable );
    bsmFillAndSendSetObjSucc( setObj );
    return TRUE;
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "ENBADDRESSES" );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_INTERNAL_ERROR_OCCURED, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
}
