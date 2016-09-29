/**
 * @file    bsm_cmd_save_rollback_cfg.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to save config and rollback config procedure.
 *
 * @author:  Ramprasad Reddy Chinta / Shweta Polepally
 * @date:    Oct 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param enodebConfig   EnodeB Config table.
 * @details  This function reset EnodeB ID and Registration Accepted fields of Enodeb Config table.
 */
static void resetEnodeBIdAndRegAcceptedInEnodeBConfigTable(
  IO EnodebConfig   *enodebConfig
  )
{
  enodebConfig->enodebId                   = 0;
  enodebConfig->enodebRegistrationAccepted = FALSE;
}

/**
 * @param enodeBId         EnodeB ID Value.
 * @param enbRegAccepted   EnodeB Registration Accepted field.
 * @param enodebConfig     EnodeB Configuration table.
 * @details  This function reset EnodeB ID and Registration Accepted fields of Enodeb Config table.
 */
void revertBackEnodeBIdAndRegAcceptedInConfigTable(
  IN UINT32         enodeBId,
  IN TBOOL          enbRegAccepted,
  IO EnodebConfig   *enodebConfig
  )
{
  enodebConfig->enodebId                   = enodeBId;
  enodebConfig->enodebRegistrationAccepted = enbRegAccepted;
}

/**
 * @param   setObj     Set Object Request
 * @param   bsmContext BSM context.
 * @details This function will perform config save operation.
 */
void bsmDoSaveConfig(
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  EnodebConfig   *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  UINT32    currEnodebId          = enodebConfig->enodebId;
  TBOOL     currEnodebRegAccepted = enodebConfig->enodebRegistrationAccepted;

  resetEnodeBIdAndRegAcceptedInEnodeBConfigTable( enodebConfig );
  if( FALSE EQ bsmWriteRunningConfigToNextStartConfig( bsmContext->configDir ) )
  {
    revertBackEnodeBIdAndRegAcceptedInConfigTable( currEnodebId, currEnodebRegAccepted, enodebConfig );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_FILE_ACCESS, &setObj->tlvArray.tlvInfo[0].tag );
    return;
  }
  revertBackEnodeBIdAndRegAcceptedInConfigTable( currEnodebId, currEnodebRegAccepted, enodebConfig );
  bsmFillAndSendSetObjSucc(setObj);

  if ( OAMIF_SIH NOTEQ setObj->cmdRef.origin )
  {
    sendEventConfigurationSaved( setObj->cmdRef.origin );
  }
}

/**
 * @param   setObj     Set Object Request
 * @param   bsmContext BSM context.
 * @details This function will perform config roll back operation.
 */
void bsmDoRollBackConfig(
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  EnodebConfig   *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  UINT32    currEnodebId          = enodebConfig->enodebId;
  TBOOL     currEnodebRegAccepted = enodebConfig->enodebRegistrationAccepted;

  if ( FALSE EQ isEnodeBLocked() )
  {
    BS_ERROR_ENODEB_NOT_LOCKED();
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
    return;
  }

  if( FALSE EQ bsmLoadCfgFromFiles( bsmContext->configDir ) )
  {
    bsmFillAndSendSetObjFail( setObj, BS_ERR_FILE_ACCESS, NULL );
    return;
  }
  revertBackEnodeBIdAndRegAcceptedInConfigTable( currEnodebId, currEnodebRegAccepted, enodebConfig );
  bsmFillAndSendSetObjSucc(setObj);
  if ( OAMIF_SIH NOTEQ setObj->cmdRef.origin )
  {
    sendEventConfigurationRollBack( setObj->cmdRef.origin );
  }
}
