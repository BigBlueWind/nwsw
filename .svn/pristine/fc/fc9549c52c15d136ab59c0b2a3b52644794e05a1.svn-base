/**
 * @file    prm_get_table.c
 * @brief This file is contains the code which will process the get req on all the tables
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : July 2010
 * Change History:
 *
 */

#include "prm.h"

/**
 * @param   getReq   Get Object Request Message received from BSM.
 * @details : This function Gets the requested information from the global data
 *            and sends to BSM.
 **/
static void prmGetAndSendTableData(
  IO GetObjReq   *getReq
  )
{
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  TagInfo       *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */
  GenericTable  *tableFunc       = getTableFunctions(tagInfo->tableId);

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* Fill Structure */
  if( NULL NOTEQ tableFunc->getStruct2TlvFunc )
  {
    tableFunc->getStruct2TlvFunc( tagInfo, &rspTlv.tlvArray );
  }
  else
  {
    logInfo("No getStruct2TlvFunc Function defined for table %d", tagInfo->tableId );
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_PRM_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param getReq     get object request
 * @param prmContext PRM context
 * @details This function fills GET_OBJ_SUCC for SwInstallHistory table
 */
static void prmGetAndSendSwInstallHistoryTableData(
  IO GetObjReq   *getReq,
  IO PrmContext  *prmContext
  )
{
  RowId           rowId=0;
  UINT8           index=0;
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  if(prmContext->noOfEntriesInSwInstallHistory EQ 0)
  {
    getRowSwInstallHistoryTable( tagInfo->tableId, 1, &(rspTlv.tlvArray) );
  }
  else if(prmContext->noOfEntriesInSwInstallHistory EQ MAX_ROWS_IN_SWINSTALLHISTORY_TABLE)
  {
    rowId = (prmContext->indexOfSwInstallHistory) % MAX_ROWS_IN_SWINSTALLHISTORY_TABLE;
    rowId++;
    for ( index=1; index <= MAX_ROWS_IN_SWINSTALLHISTORY_TABLE; index++ )
    {
      SwInstallHistory  *rowOfSwInstallStatus = (SwInstallHistory*)getRowOfTable(SWINSTALLHISTORY, rowId-1);
      rowOfSwInstallStatus->index = index ;
      getRowSwInstallHistoryTable( tagInfo->tableId, rowId, &rspTlv.tlvArray );
      rowId = rowId % MAX_ROWS_IN_SWINSTALLHISTORY_TABLE ;
      rowId++;
    }
  }
  else
  {
    for ( rowId=1; rowId <= prmContext->noOfEntriesInSwInstallHistory; rowId++ )
    {
      getRowSwInstallHistoryTable( tagInfo->tableId, rowId, &rspTlv.tlvArray );
    }
  }
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_PRM_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param   moduleStatus  module status table.
 * @details : This function will update memUsage field in module status table.
 **/
static void prmUpdateMemUsage(
  IO ModuleStatus   *moduleStatus
  )
{
  UINT8 *word = NULL;
  UINT8 *vmSize = NULL;

  UINT8 *delim1   = ":";
  UINT8 *delim2   = " ";

  FILE  *ptrProcFile     = NULL;

  FilePath  procFilePath;
  String50  lineInfo;

  clearBytes(&lineInfo, sizeof(String50));
  clearBytes(&procFilePath, sizeof(FilePath) );

  snprintf(procFilePath, sizeof(FilePath), "/proc/%d/status", moduleStatus->processOrTaskId);
  ptrProcFile = fopen(procFilePath, "r");
  if(ptrProcFile EQ NULL)
  {
    BS_ERROR_FILE_OPEN_FAILED(procFilePath);
    return;
  }

  while(NULL NOTEQ fgets(lineInfo, sizeof(String50), ptrProcFile))
  {
    word = strtok(lineInfo, delim1);
    /* Trying to match pattern "VmSize:     4564 kB" */
    if(0 EQ strcmp(word, "VmSize"))
    {
      word = strtok(NULL, delim1);
      vmSize = strtok(word, delim2);
      vmSize = strtok(NULL, delim2);
      moduleStatus->memUsage = atoi(vmSize);
      break;
    }
    clearBytes(&lineInfo, sizeof(String50));
  }
  fclose(ptrProcFile);
  return;
}

/**
 * @param   prmContext PRM module related Information.
 * @details : This function will update modules status table as per current status
 **/
void prmUpdateTableWithCurrentModuleStatus(
  IO PrmContext  *prmContext
  )
{
  UINT8 i;

  ModuleStatus *status = (ModuleStatus *)getFirstRowOfTable(MODULESTATUS);

  for (i=0 ; i< prmContext->numOfModules; i++)
  {
    /*  ProcessId will be 0 for Lowerarm modules - MAC, RLC and PDCP. 
     * Hence this check presents mis leading logs in PRM */
    if(0 NOTEQ status->processOrTaskId)
      prmUpdateMemUsage(status);
    getNextRowOfTable(status);
  }
}

/**
 * @param   msg        Message received from BSM.
 * @param   prmContext PRM module related Information.
 * @details : This function handles GET Request message from BSM.
 **/
void prmProcGetObjReqInAllStates(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  GetObjReq       getObj;
  GetObjRspSucc   sendRsp;
  EncBuf          encBuf;
  TableId         tableId;
  clearBytes( &getObj, sizeof(GetObjReq));
  clearBytes( &sendRsp, sizeof(GetObjRspSucc));
  initEncBuf( &encBuf );

  decGetObjReqMsg( (UINT8*)msg->data, msg->len, &getObj );

  copyBytes( &(getObj.cmdRef), sizeof(CmdRef), &(sendRsp.cmdRef));

  /* Only one TLV for get command, hence take from index 0 */
  tableId = getObj.tagArray.tagInfo[0].tableId;

  switch( tableId )
  {
    case PROCESSSTART:
    case MODULESTOP:
         prmGetAndSendTableData( &getObj );
         break;
    case MODULESTATUS:
         prmUpdateTableWithCurrentModuleStatus( prmContext );
         prmGetAndSendTableData( &getObj);
         break;
    case SWACTIVATIONSTATUS:
         prmGetAndSendTableData( &getObj );
         break;
    case SWINSTALLHISTORY:
         prmGetAndSendSwInstallHistoryTableData( &getObj, prmContext );
         break;
    case SWPACKAGESTATUS:
         prmGetAndSendTableData( &getObj );
         break;
  }
}
