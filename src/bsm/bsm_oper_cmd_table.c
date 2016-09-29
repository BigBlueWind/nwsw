/**
 * @file    bsm_oper_cmd_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will handle all the set messages from operator command table.
 *
 * @author:  Lakshmi Narayana Madala / Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param    cmdRef  Command Ref
 * @details This function will fill and send COUNTER_CLEAR_REQ message to PMH
 */
static void createAndSendCounterClearReq(
  IO SetObjReq   *setObj
  )
{
  MsgBsmPmhCounterClearReq  clearReq;
  clearBytes( &clearReq, sizeof(MsgBsmPmhCounterClearReq) );
  if ( FALSE EQ isEnodeBLocked() )
  {
    BS_ERROR_ENODEB_NOT_LOCKED();
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
    return;
  }

  clearReq.cmdRef = setObj->cmdRef;
  createAndSendMsg( MSG_BSM_PMH_COUNTER_CLEAR_REQ, (UINT8*)&clearReq, sizeof(MsgBsmPmhCounterClearReq));
}

/**
 * @param    cmdRef  Command Ref
 * @details This function will fill and send DIAG_COUNTER_CMD message to PMH
 */
static void createAndSendDiagCounterCmd(
  IN CmdRef         *cmdRef,
  IN ENbCommand      eNbCommand
  )
{
  MsgBsmPmhDiagCounterCmd  cmd;
  clearBytes( &cmd, sizeof(MsgBsmPmhDiagCounterCmd) );

  cmd.cmdRef = *cmdRef;
  cmd.eNbCommand = eNbCommand;

  createAndSendMsg( MSG_BSM_PMH_DIAG_COUNTER_CMD, (UINT8*)&cmd, sizeof(MsgBsmPmhDiagCounterCmd));
}

/**
 * @param    msg         Message received from PMH
 * @param    bsmContext  BSM Context
 * @details This function will process COUNTER_CLEAR_SUCC message receieved from PMH
 */
BsmProceduralState bsmProcCounterClearSucc(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPmhBsmCounterClearSucc  *succ = (MsgPmhBsmCounterClearSucc*)msg->data;

  SetObjReq     setObjReq;
  clearBytes(&setObjReq, sizeof(SetObjReq));
  setObjReq.cmdRef = succ->cmdRef;
  fillTagLenUint32( ENBADMIN, 1, COL_ENBADMIN_ENBCOMMAND, ENBCOMMAND_RESETCOUNTERS, &setObjReq.tlvArray );
  bsmFillAndSendSetObjSucc(&setObjReq);

  return bsmContext->bsmProceduralState;
}

/**
 * @param    msg         Message received from PMH
 * @param    bsmContext  BSM Context
 * @details This function will process DIAG_COUNTER_CMD_CMPL message receieved from PMH
 */
BsmProceduralState bsmProcDiagCounterCmdCmpl(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPmhBsmDiagCounterCmdRsp  *succ = (MsgPmhBsmDiagCounterCmdRsp*)msg->data;

  SetObjReq     setObjReq;
  clearBytes(&setObjReq, sizeof(SetObjReq));
  setObjReq.cmdRef = succ->cmdRef;
  fillTagLenUint32( ENBADMIN, 1, COL_ENBADMIN_ENBCOMMAND, succ->eNbCommand, &setObjReq.tlvArray );
  bsmFillAndSendSetObjSucc(&setObjReq);

  return bsmContext->bsmProceduralState;
}

/**
 * @param    msg         Message received from PMH
 * @param    bsmContext  BSM Context
 * @details This function will process COUNTER_CLEAR_FAIL message receieved from PMH
 */
BsmProceduralState bsmProcCounterClearFail(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPmhBsmCounterClearFail  *fail = (MsgPmhBsmCounterClearFail*)msg->data;

  SetObjReq     setObjReq;
  clearBytes(&setObjReq, sizeof(SetObjReq));
  setObjReq.cmdRef = fail->cmdRef;
  fillTagLenUint32( ENBADMIN, 1, COL_ENBADMIN_ENBCOMMAND, ENBCOMMAND_RESETCOUNTERS, &setObjReq.tlvArray );
  bsmFillAndSendSetObjFail(&setObjReq, fail->bsError, NULL);

  return bsmContext->bsmProceduralState;
}

/**
 * @param msg          message sent by CIH/WIH/SIH
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @param bsmContext   BSM context information
 * @details This function will perform operation on EnodeB Admin table as per the SET request.
 *          1. Check if Lock is required to process this request
 *          2. Get the TLV value from the  setObj
 *          3. Take action on eNodeBAdmin Table
 */
void bsmPerformOperationOnEnbAdmin(
  IN Msg         *msg,
  IO SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  EnbAdmin    enbAdmin;

  GenericTable     *tableFunc = getTableFunctions(ENBADMIN);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, &enbAdmin );
  }

  /* Take Action */
  switch( enbAdmin.eNbCommand )
  {
    case ENBCOMMAND_LOCKENB:
         bsmDoLock( setObj, bsmContext );
         break;
    case ENBCOMMAND_UNLOCKENB:
         bsmDoUnLock( setObj, bsmContext  );
         break;
    case ENBCOMMAND_RESTARTENB:
         bsmDoRestart( setObj, bsmContext );
         break;
    case ENBCOMMAND_SAVECONFIG:
         bsmDoSaveConfig( setObj, bsmContext );
         break;
    case ENBCOMMAND_ROLLBACKCONFIG:
         bsmDoRollBackConfig( setObj, bsmContext );
         break;
    case ENBCOMMAND_RESETRADIOHEAD:
    case ENBCOMMAND_RESETGPS:
         createAndSendMsg( MSG_BSM_DEH_SET_OBJ_REQ, msg->data, msg->len );
         break;
    case ENBCOMMAND_REBOOTENB:
         bsmDoReboot( setObj, bsmContext );
         break;
    case ENBCOMMAND_RESETCOUNTERS:
         createAndSendCounterClearReq( setObj );
         break;
    case ENBCOMMAND_STARTDIAG:
         createAndSendDiagCounterCmd( &setObj->cmdRef, ENBCOMMAND_STARTDIAG );
         break;
    case ENBCOMMAND_STOPDIAG:
         createAndSendDiagCounterCmd( &setObj->cmdRef, ENBCOMMAND_STOPDIAG );
         break;
    case ENBCOMMAND_RUNBUILTINSELFTEST:
         bsmDoBist( msg , setObj, bsmContext  );
         break;
    case ENBCOMMAND_SETLONGPOST:
    	 bsmDoLongPost( msg, setObj, bsmContext  );
         break;
    case ENBCOMMAND_DELETEALARMHISTORY:
         bsmDeleteAlarmHistoryTable(setObj);
         break;
    case ENBCOMMAND_DELETEFILEEVENTHISTORY:
         createAndSendMsg( MSG_BSM_FTH_SET_OBJ_REQ, msg->data, msg->len );
         break;
    case ENBCOMMAND_DELETESWINSTALLHISTORY:
         createAndSendMsg( MSG_BSM_PRM_SET_OBJ_REQ, msg->data, msg->len );
         break;

    default:
         bsmFillAndSendSetObjFail(setObj, BS_ERR_UNKNOWN_ACTION_RECD, &(setObj->tlvArray.tlvInfo[0].tag));
         BS_ERROR_UNKNOWN_ACTION_RECD( enbAdmin.eNbCommand );
         break;
  }
}

/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @param bsmContext   BSM context information
 * @details This function will perform operation on Software Activation table as per the SET request.
 *          1. Check if Lock is required to process this request
 *          2. Get the TLV value from the setObj
 *          3. Take action on SwActivation Table
 *          Note: Sw Activation is done by PRM Module. BSM just performs the basic checks and relays the
 *                request to PRM.
 */
void bsmPerformOperationOnSwActivation(
  IO SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  EncBuf         encBuf;
  initEncBuf( &encBuf );
  encSetObjReqMsg( setObj, &encBuf );

  if ( FALSE EQ isEnodeBLocked() )  /* If EnodeB is not locked, Config install cannot be performed. */
  {
    BS_ERROR_ENODEB_NOT_LOCKED();
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
    return;
  }
  setBsmProceduralState( bsmContext, WAIT_SW_ACTIVATE_CMPL );
  createAndSendMsg( MSG_BSM_PRM_SW_ACTIVATE_REQ, (UINT8*)&encBuf.value, encBuf.len );
}

TBOOL isConfigInstall(
  IN FileName   filename
  )
{
  char*  peek = (char*)&filename[strlen(filename) - 1];
  String10     extension;
  clearBytes( extension, 10 );

  while (peek >= (char*)filename)
  {
    if (*peek EQ '.')
    {
    strcpy (extension, peek);
    break;
    }
    peek--;
  }
  if( 0 EQ strcmp(extension, ".cfg") )
  {
    return TRUE;
  }
  return FALSE;
}

void bsmPerformOperationOnCounterControl(
  IO SetObjReq   *setObj
  )
{
  CounterControl      ctrlReq;
  EncBuf         encBuf;
  initEncBuf( &encBuf );
  clearBytes( &ctrlReq, sizeof(CounterControl) );

  GenericTable     *tableFunc = getTableFunctions(COUNTERCONTROL);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, &ctrlReq );
  }

  if ( COUNTERCOMMAND_START EQ ctrlReq.counterCommand )
  {
    if ( FALSE EQ isEnodeBEnabled() )  /* If EnodeB is not Enabled, Measurement can not be started. */
    {
      /* TODO: need to update the macro */
      BS_ERROR_ENODEB_LOCKED();
      bsmFillAndSendSetObjFail( setObj, BS_ERR_ENB_NOT_ENABLED, &setObj->tlvArray.tlvInfo[0].tag );
      return;
    }
  }
  else if ( COUNTERCOMMAND_STOP EQ ctrlReq.counterCommand )
  {
    if ( TRUE EQ isEnodeBLocked() )  /* If EnodeB is locked then no measurement will be running */
    {
      BS_ERROR_ENODEB_LOCKED();
      bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_IS_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
      return;
    }
  }

  encSetObjReqMsg( setObj, &encBuf );
  createAndSendMsg( MSG_BSM_PMH_SET_OBJ_REQ, (UINT8*)&encBuf.value, encBuf.len );
}

void bsmPerformOperationOnSwInstall(
  IO SetObjReq   *setObj
  )
{
  SwInstall      install;
  EncBuf         encBuf;
  initEncBuf( &encBuf );
  clearBytes( &install, sizeof(SwInstall) );

  GenericTable     *tableFunc = getTableFunctions(SWINSTALL);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, &install );
  }

  if ( INSTALLATIONTYPE_L2L3CONFIGURATION EQ install.installationType || \
	   INSTALLATIONTYPE_L2L3PATCH EQ install.installationType )
  {
    if ( FALSE EQ isEnodeBLocked() )  /* If EnodeB is not locked, Config install cannot be performed. */
    {
      BS_ERROR_ENODEB_NOT_LOCKED();
      bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
      return;
    }
  }

  /* Here the received SET OBJECT is decoded as the */
  /* Installation Request is sent as TLV here - hence no need of decoding */
  encSetObjReqMsg( setObj, &encBuf );
  createAndSendMsg( MSG_BSM_PRM_SW_INSTALL_REQ, (UINT8*)&encBuf.value, encBuf.len );
}

