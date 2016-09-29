/**
 * @file    pmh_get_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different GET_REQ messages for statistics
 *
 * @author  Shweta Polepally
 * @date    6 April 2011
 * @details This file contains functions to handle get on all tables which are at PMH.
 */

#include "pmh.h"

/**
 * @param cmdRef    CmdRef
 * @param tagInfo   Tag Information
 * @param error     BsErrorValue
 * @details This function will send GET_OBJ_FAIL to BSM
 **/

void sendGetObjRspFailToBsm(
  IN CmdRef        *cmdRef,
  IN TagInfo       *tagInfo,
  IN BsErrorValue  *error
  )
{
  GetObjRspFail   rspTlv;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes( &rspTlv, sizeof(GetObjRspFail));

  /* Fill CmdRef, TagInfo and BsError */
  copyBytes( cmdRef, sizeof(CmdRef), &(rspTlv.cmdRef));
  copyBytes( tagInfo, sizeof(TagInfo), &(rspTlv.tagArray.tagInfo[0]));
  rspTlv.getErrRsp.errRspCode = *error;

  /* Encode */
  encGetObjRspFailMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_PMH_BSM_GET_OBJ_FAIL, (UINT8*)&encBuf.value, encBuf.len);
}


/**
 * @param transId      TransId
 * @param pmhContext   PMH Context information
 * @details This function will send Get Object response in TLV format to BSM Module
 **/
void sendGetObjRspSuccToBsm(
  IN UINT32         transId,
  IO PmhContext    *pmhContext
  )
{
  TagInfo         tagInfo;
  CmdRef          cmdRef;

  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));
  clearBytes( &tagInfo, sizeof(TagInfo));
  clearBytes( &cmdRef, sizeof(CmdRef));

  if(BS_NO_ERROR NOTEQ getCmdRefByTransIdAndDeallocTransId( transId, &cmdRef, &tagInfo, pmhContext))
  {
    return;
  }

  GenericTable  *tableFunc = getTableFunctions(tagInfo.tableId);

  /* Fill CMD REF details */
  copyBytes( &(cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* Fill Structure */
  if( NULL NOTEQ tableFunc->getStruct2TlvFunc )
  {
    tableFunc->getStruct2TlvFunc( &tagInfo, &rspTlv.tlvArray );
  }
  else
  {
    logInfo("No getStruct2TlvFunc Function defined for table %d", tagInfo.tableId );
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_PMH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param numOfElements Number of alarm elements.
 * @param getReq        GET Object request received.
 * @details This function will send the Active/History alarm table.
 **/
static void sendPmLogStatusTable(
  IN UINT32       numOfElements,
  IO GetObjReq   *getReq
  )
{
  RowId           rowId=0;
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* If entire table needs to be send, handling is done in a different way,
   * else the procedure remains same */
  GenericTable  *tableFunc = getTableFunctions(tagInfo->tableId);
  GetCmdType     cmdType   = getCmdType(tagInfo);

  if ( numOfElements NOTEQ 0 )
  {
    switch( cmdType )
    {
      case GET_TABLE:
        {
          for ( rowId=1; rowId <= numOfElements; rowId++ )
            getRowPmLogFileStatusTable( tagInfo->tableId, rowId, &rspTlv.tlvArray );
        }
        break;
      case GET_ROW:
        tableFunc->getRowFunc( tagInfo->tableId, tagInfo->rowId, &rspTlv.tlvArray );
        break;
      case GET_ELEMENT:
        tableFunc->getElementFunc( tagInfo->tableId, tagInfo->rowId, tagInfo->colId, &rspTlv.tlvArray );
        break;
      default:
        /* Error Condition */
        break;
    }
  }
  else
  {
    /* Fill One row atleast with values 0 . This is needed for SIH */
    getRowPmLogFileStatusTable( tagInfo->tableId, 1, &rspTlv.tlvArray ); /* 1 = rowId*/
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_PMH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @details This function updates PM Log File Status table with the lastest entries
 *          by reading it from the list stored in its context.
 **/
void updatePmLogFileStatusTable( void )
{
  PmLogFileStatusTable  *list       = getPmLogFileStatusTable();
  PmLogFileStatus       *rowOfTable = (PmLogFileStatus*)getFirstRowOfTable(PMLOGFILESTATUS);
  PmLogFileStatus       *entry = NULL;
  ListNode              *node = NULL;
  UINT32                numOfElements = list->numElemPresent;
  UINT32                indexCount = 0;

  struct stat fileCtx;
  clearBytes(&fileCtx, sizeof(struct stat));

  /* Empty Active alarm table and fill the updated values */
  /*clearPmLogFileStatusTable();*/

  UINT8 i = 0;
  for ( i =0; i < list->maxElem; i++ )
  {
    clearBytes(rowOfTable,sizeof(PmLogFileStatus));
    rowOfTable++;
  }
  rowOfTable = (PmLogFileStatus*)getFirstRowOfTable(PMLOGFILESTATUS);

  /* Reverse the list so that latest requested entries can be sent */
  node = list->first;

  /* Print the entire table posted to BSM */
  if (numOfElements EQ 0)
  {
    snprintf( (UINT8*)&rowOfTable->fileName, MAX_SIZE_PMLOGFILESTATUS_FILENAME + 1, "%s", "None");
    snprintf( (UINT8*)&rowOfTable->openedAt, MAX_SIZE_PMLOGFILESTATUS_OPENEDAT + 1, "%s", "00000000000000000");
    snprintf( (UINT8*)&rowOfTable->closedAt, MAX_SIZE_PMLOGFILESTATUS_CLOSEDAT + 1, "%s", "00000000000000000");
    logInfo("%s","No Contents in PM Log File table - TABLE EMPTY \n");
    return;
  }

  while(node)
  {
    entry = (PmLogFileStatus*)node->data;
    copyBytes( entry, sizeof(PmLogFileStatus), rowOfTable );
    indexCount++;
    rowOfTable->index = indexCount;
    if(0 EQ stat(rowOfTable->fileName, &fileCtx))
    {
      rowOfTable->fileSizeInBytes = fileCtx.st_size;
      clearBytes(&fileCtx, sizeof(struct stat));
    }
    getNextRowOfTable(rowOfTable);
    numOfElements--;
    if ( numOfElements EQ 0 )
      break;
    else
      node = node->next;
  }
}

/**
 * @param   getReq  GET Object request messag in TLV format.
 * @details This function will update and send the PM Log file status table information.
 **/
static void updateAndSendPmLogFileStatusTable(
  IO GetObjReq   *getReq
  )
{
  /* Copy current contents from list to the global variable of this status table */
  updatePmLogFileStatusTable();
  /* Sending table to OAM-IF module */
  sendPmLogStatusTable( getPmLogFileStatusTable()->numElemPresent, getReq );
}


/**
 * @param   getReq  Get Object Request message
 * @details This function will get the function pointer for getStruct2TlvFunc function of
 *          the respective Table, encode the information and send to the BSM.
 **/
void pmhGetAndSendData(
  IO GetObjReq   *getReq
  )
{
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  GenericTable  *tableFunc = getTableFunctions(tagInfo->tableId);

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* Fill Structure */
  if( NULL NOTEQ tableFunc->getStruct2TlvFunc )
  {
    tableFunc->getStruct2TlvFunc( tagInfo, &rspTlv.tlvArray );
  }
  else
  {
    BS_ERROR_NO_STRUCT2TLV_FUNCTION_DEFINED( tagInfo->tableId );  /* TODO: Get table name here in string */
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_PMH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);

}

void pmhGetAndSendL2Counters(
  IO GetObjReq  *getReq,
  IO PmhContext *pmhContext
  )
{
  UINT32 index = 0;
  UINT16 rowId = 0;
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;

  initEncBuf(&encBuf);
  clearBytes(&rspTlv, sizeof(GetObjRspSucc));

  copyBytes(&getReq->cmdRef, sizeof(CmdRef), &rspTlv.cmdRef);

  for(index = 0; index<MAX_ROWS_IN_L2COUNTERS_TABLE; index++)
  {
    if (TRUE EQ pmhContext->counterContext[index].l2CounterConfig.isEnabled )
    {
      fillTagLenUint32( L2COUNTERS, rowId+1, COL_L2COUNTERS_INDEX,           rowId+1, &(rspTlv.tlvArray) );
      fillTagLenUint32( L2COUNTERS, rowId+1, COL_L2COUNTERS_COUNTERID,       pmhContext->counterContext[index].l2CounterConfig.counterId, &(rspTlv.tlvArray) );
      fillTagLenUint32( L2COUNTERS, rowId+1, COL_L2COUNTERS_VALUE,           pmhContext->counterContext[index].value, &(rspTlv.tlvArray) );
      fillTagLenString( L2COUNTERS, rowId+1, COL_L2COUNTERS_LASTCOLLECTEDAT, pmhContext->counterContext[index].lastCollectedAt, MAX_SIZE_ACTIVEUESTATUS_ENTRYTIME, &(rspTlv.tlvArray) );
      rowId++;
    }
  }

  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );

  /* Create and send */
  createAndSendMsg(MSG_PMH_BSM_GET_OBJ_SUCC, (UINT8*) &encBuf.value, encBuf.len);

  return;
}
/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function Processes GET_OBJ_REQ message
 * */
PmhState pmhProcGetObjReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  GetObjReq       getObj;
  TableId         tableId;
  clearBytes( &getObj, sizeof(GetObjReq));

  decGetObjReqMsg( (UINT8*)msg->data, msg->len, &getObj );

  /* Only one TLV for get command, hence take from index 0 */
  tableId = getObj.tagArray.tagInfo[0].tableId;

  switch( tableId )
  {
    /* Get the data from status table */
    case MACGENCOUNTERS:
      createAndSendMacGenStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case MACUPLANECOUNTERS:
      pmhContext->rspRspFromMacRclAndMacPhyCounter          = 0;
      pmhContext->rspRspFromMacRclAndMacPhyCounterForGetCmd = 0;
      createAndSendMacUplaneStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case RLCGENCOUNTERS:
      createAndSendRlcGenStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case PDCPGENCOUNTERS:
      createAndSendPdcpGenStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case PDCPUPLANECOUNTERS:
      createAndSendPdcpUplaneStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case GTPGENCOUNTERS:
      createAndSendGtpGenStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case SCTPGENCOUNTERS:
      createAndSendSctpGenStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]),pmhContext);
      break;
    case RRCGENCOUNTERS:
      createAndSendRrcGenStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case RRCCELLCOUNTERS:
      createAndSendRrcCellStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext->cellId, pmhContext );
      break;
    case S1APGENCOUNTERS:
      createAndSendS1apGenStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext);
      break;
    case S1APPEERNODECOUNTERS:
      if(TRUE EQ pmhContext->isOpStartRcvd)
      {
        createAndSendS1apPeerNodeStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext->peerId, pmhContext );
      }
      else
      {
        /*TODO this code will be removed when crashing problem of cp.exe on
         * GET on S1APPEERNODECOUNTERS before first UNLOCK resolved
         * */
        GetObjRspSucc   rspTlv;
        EncBuf          encBuf;
        initEncBuf( &encBuf );
        clearBytes( &rspTlv, sizeof(GetObjRspSucc));
        copyBytes( &(getObj.cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));
        S1apPeerNodeCounters  *rowOfTable = (S1apPeerNodeCounters*)getFirstRowOfTable(S1APPEERNODECOUNTERS);

        getAndFillDateAndTimeStamp( MAX_SIZE_S1APPEERNODECOUNTERS_LASTCOLLECTEDAT,  rowOfTable->lastCollectedAt );
        getRowS1apPeerNodeCountersTable( S1APPEERNODECOUNTERS, 1, &rspTlv.tlvArray ); /* 1 = rowId*/
        encGetObjRspSuccMsg( &rspTlv, &encBuf );
        logInfo("%s", "GET on S1APPEERNODECOUNTERS is not forwarded to S1AP");
        createAndSendMsg( MSG_PMH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);

      }
      break;
    case RRMCOUNTERS:
      createAndSendUehRrmStatReq(&(getObj.cmdRef), &(getObj.tagArray.tagInfo[0]), pmhContext );
      break;
    case ETHERNETCOUNTERS:
      pmhProcEthernetStatusReq(TRUE, pmhContext, &getObj);
      break;
    case IPCOUNTERS:
      pmhProcIpStatusReq(TRUE, pmhContext, &getObj);
      break;
    case PMLOGFILESTATUS:
      updateAndSendPmLogFileStatusTable(&getObj);
      break;
    case L2COUNTERS:
      pmhGetAndSendL2Counters(&getObj, pmhContext);
      break;
    case COUNTERCOLLECTIONSTATUS:
      pmhGetAndSendData(&getObj);
      break;
    default:
    {
      BS_ERROR_UNKNOWN_TABLE_ID(tableId);
      break;
    }
  }
  return pmhContext->pmhState;
}
