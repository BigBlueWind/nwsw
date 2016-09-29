/**
 * @file    bsm_cell_bar_unbar.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to Cell BAR/UNBAR procedures.
 *
 * @author:  Ankur Agrawal
 * @date:    28-May-2012
 * @details: Initial Version
 **/
#include "bsm.h"


/**
 * @param   barUnbarInfo  cell BarUnbarCause
 * @details This function will update the cellStatus->info field as per cell Bar-Unbar cause
 */
void bsmUpdateCellBarUnbarInfo(
  IN UINT8   *barUnbarInfo
  )
{
  UINT8 *cellOperationalStateInfo = NULL;
  UINT8 *cellBarUnbarInfo         = NULL;
  UINT8 *cellOverloadInfo         = NULL;
  UINT8 *delim                    = "|";

  UINT8  info[MAX_SIZE_CELLSTATUS_INFO + 1 ];

  CellStatus   *cellStatusTable = (CellStatus*)getFirstRowOfTable(CELLSTATUS);

  copyBytes(cellStatusTable->info, sizeof(cellStatusTable->info),info);

  cellOperationalStateInfo = strtok( (char*)info, delim );
  cellBarUnbarInfo         = strtok( NULL, delim );
  cellOverloadInfo         = strtok( NULL, delim );

  snprintf(cellStatusTable->info, MAX_SIZE_CELLSTATUS_INFO +1 , "%s|%s|%s",
           cellOperationalStateInfo, barUnbarInfo, cellOverloadInfo);
}

/**
 * @param   operationalStateInfo  cell Operational state info
 * @details This function will update the cellStatus->info field as per operationalStateInfo
 */
void bsmUpdateCellOperationalStateInfo(
  IN UINT8   *operationalStateInfo
  )
{
  UINT8 *cellOperationalStateInfo = NULL;
  UINT8 *cellBarUnbarInfo         = NULL;
  UINT8 *cellOverloadInfo         = NULL;
  UINT8 *delim                    = "|";

  UINT8  info[MAX_SIZE_CELLSTATUS_INFO + 1 ];

  CellStatus   *cellStatusTable = (CellStatus*)getFirstRowOfTable(CELLSTATUS);

  copyBytes(cellStatusTable->info, sizeof(cellStatusTable->info),info);

  cellOperationalStateInfo = strtok( (char*)info, delim );
  cellBarUnbarInfo         = strtok( NULL, delim );
  cellOverloadInfo         = strtok( NULL, delim );

  snprintf(cellStatusTable->info, MAX_SIZE_CELLSTATUS_INFO +1 , "%s|%s|%s",
           operationalStateInfo, cellBarUnbarInfo, cellOverloadInfo);
}

/**
 * @param   setObj     Set Object request in TLV format.
 * @param   bsmContext BSM Context
 * @details This function will take appropriate actions as the commands received
 */
void bsmPerformOperationOnCellAdminTable(
  IO SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  CellAdmin    cellAdmin;
  CellConfig   *cellConfigTable = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  CellStatus   *cellStatusTable = (CellStatus*)getFirstRowOfTable(CELLSTATUS);

  GenericTable     *tableFunc = getTableFunctions(CELLADMIN);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, &cellAdmin );
  }

  /* SET on single element is not supported because both CellCommd and CellId are require to BAR a cell */
  if(1 EQ setObj->tlvArray.numTlvs)
  {
    bsmFillAndSendSetObjFail( setObj, BS_ERR_SET_ON_SINGLE_PARAMETER_UNSUPPORTED, &(setObj->tlvArray.tlvInfo[0].tag) );
    return;
  }

  /* CellId should be same as persent in CellConfig table */
  if(cellAdmin.cellId NOTEQ cellConfigTable->cellId)
  {
    bsmFillAndSendSetObjFail( setObj, BS_ERR_CELL_ID_NOT_FOUND, &(setObj->tlvArray.tlvInfo[0].tag) );
    return;
  }

  /* If two continuous same command given then no need to take any action */
  if(cellAdmin.cellCommand EQ bsmContext->cellCmdRcvd)
  {
    if(CELLCOMMAND_BAR EQ cellAdmin.cellCommand)
      logInfo("%s","No Action is required as Cell is already Barred");
    else if(CELLCOMMAND_UNBAR EQ cellAdmin.cellCommand)
      logInfo("%s","No Action is required as Cell is already Unbarred");
    bsmFillAndSendSetObjSucc( setObj );
    return;
  }

  bsmContext->cellCmdRcvd = cellAdmin.cellCommand;

  /* Update CellStatus table */
  if(CELLCOMMAND_BAR EQ cellAdmin.cellCommand)
  {
    bsmUpdateCellBarUnbarInfo("Operator initiated Barred ");
    cellStatusTable->isBarred = TRUE;
  }
  else
  {
    bsmUpdateCellBarUnbarInfo("Operator initiated Unbarred ");
    cellStatusTable->isBarred = FALSE;
  }

  /* IF ENB IS UNLOCKED THEN ONLY ACTION WILL TAKEN */
  if(FALSE EQ isEnodeBLocked())
  {
    /* Take Action */
    switch( cellAdmin.cellCommand )
    {
      case CELLCOMMAND_BAR:
          { 
            MsgBsmUehCellBarReq barReq;
            clearBytes(&barReq, sizeof(MsgBsmUehCellBarReq));
            barReq.cellId = cellAdmin.cellId;
            createAndSendMsg(MSG_BSM_CEM_CELL_BAR_REQ, (UINT8 *)&barReq, sizeof(MsgBsmUehCellBarReq));
          break;
          }
      case CELLCOMMAND_UNBAR:
          {
            MsgBsmUehCellUnbarReq unbarReq;
            clearBytes(&unbarReq, sizeof(MsgBsmUehCellUnbarReq));
            unbarReq.cellId = cellAdmin.cellId;
            createAndSendMsg(MSG_BSM_CEM_CELL_UNBAR_REQ, (UINT8 *)&unbarReq, sizeof(MsgBsmUehCellUnbarReq));
            break;
          }
      default:
          BS_ERROR_UNKNOWN_ACTION_RECD( cellAdmin.cellCommand );
          bsmFillAndSendSetObjFail( setObj, BS_ERR_UNKNOWN_ACTION_RECD, &(setObj->tlvArray.tlvInfo[0].tag) );
          return;
          break;
    }
  }

  bsmFillAndSendSetObjSucc( setObj );
  return;
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles MSG_CEM_BSM_CELL_BAR_SUCC message received from CEM.
 */
inline BsmProceduralState bsmProcCellBarSuccInAllStates(
  IN Msg        *msg,
  IO BsmContext *bsmContext
  )
{
  UNUSED(msg);
  logInfo("%s", "No action is required at BSM");
  return (bsmContext->bsmProceduralState);
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles MSG_CEM_BSM_CELL_UNBAR_SUCC message received from CEM.
 */
inline BsmProceduralState bsmProcCellUnBarSuccInAllStates(
  IN Msg        *msg,
  IO BsmContext *bsmContext
  )
{
  UNUSED(msg);
  logInfo("%s", "No action is required at BSM");
  return (bsmContext->bsmProceduralState);
}
