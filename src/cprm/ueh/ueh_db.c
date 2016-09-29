/*
  * @file    ueh_db.c
  * @brief This file contains the functions for storing the Configuration received from
                   BSM to UEH global database  
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 06 Oct 2010 :
 *     a) Created initial version
 */
#include <string.h>
#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "lueh.x" 
#include "ueh_eut_x.h" 
#include "bsm_ueh_if.h"
#include "sfi.h"
#include "ueh.h"
#include "log.h"

/*
*
*       Fun:   uehStoreRcvdCellCfg
*       Desc:  This function stores the received cell configuration from BSM into
*                  the ueh global uehCfgCb
*       Ret:   ROK
*       Notes: None
*       File:  ueh_db.c
*/

PUBLIC S16 uehStoreRcvdCellCfg(
    Buffer   *mBuf
  )
{
  MsgLen        cCnt;
  UINT8         plmnIdx;
  UehCellCb *cellCb = uehGetCellCb();

  /*Save it in a Global to the global buffer*/
  SCpyMsgFix( mBuf, 0, sizeof(UehCfgCb), (Data*)&(uehCb.uehCfgCb) , &cCnt );
  /*BS-1799 Saving the received configuration from BSM which will be modified only when Oprator changes the value while ENB in unlock state*/
  SCpyMsgFix( mBuf, 0, sizeof(UehCfgCb), (Data*)&(uehCb.modiUehCfgCb) , &cCnt );

  for(plmnIdx = 0; plmnIdx < UEH_MCC_SIZE; plmnIdx++)
  {
    uehCb.uehPlmnId.plmnMCC[plmnIdx] = CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.plmnMCC[plmnIdx]);
  }
  /* PLMN Change*/
  uehCb.uehPlmnId.numOfMNCDigits = (U8)strlen (( const char *)(uehCb.uehCfgCb.cellCfg.plmnMNC)); 
  logInfo ("Received length of MNC from BSM = %d\n", uehCb.uehPlmnId.numOfMNCDigits);
  for(plmnIdx = 0; plmnIdx < uehCb.uehPlmnId.numOfMNCDigits; plmnIdx++)
  {
      uehCb.uehPlmnId.plmnMNC [plmnIdx] =  CONVERT_ASCII_TO_INT(uehCb.uehCfgCb.cellCfg.plmnMNC[plmnIdx]);  
  }
  uehCb.cfgValid = 1;
  /*Updated cellCb changes allowable parameters from BSM */
  logInfo ("cellCb parameters Before updation from BSM |cellId= %d\n|trackAreaCode =%d\n|maxUeSupp =%d\n", cellCb->cellId,cellCb->sysRelInfo.trackAreaCode,cellCb->maxUeSupp);
  cellCb->cellId                   = uehCb.uehCfgCb.cellCfg.cellId;
  cellCb->sysRelInfo.trackAreaCode = uehCb.uehCfgCb.cellCfg.tac;
  cellCb->maxUeSupp                = uehCb.uehCfgCb.cellCfg.maxConnectedUes;
  logInfo ("cellCb parameters After updation from BSM |cellId= %d\n|trackAreaCode =%d\n|maxUeSupp =%d\n", cellCb->cellId,cellCb->sysRelInfo.trackAreaCode,cellCb->maxUeSupp);
  /* plmn identity */
  for (plmnIdx = 0; plmnIdx < UEH_MCC_SIZE; plmnIdx++)
  {
     /* mcc */
     cellCb->sysRelInfo.mcc[plmnIdx].val = uehCb.uehPlmnId.plmnMCC[plmnIdx];
  }
  for(plmnIdx = 0; plmnIdx < uehCb.uehPlmnId.numOfMNCDigits; plmnIdx++)
  {  /* mnc */
     cellCb->sysRelInfo.mnc[plmnIdx].val = uehCb.uehPlmnId.plmnMNC [plmnIdx];
  }


RETVALUE(ROK);
}

