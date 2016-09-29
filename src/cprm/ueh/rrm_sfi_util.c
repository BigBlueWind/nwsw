/**
 * @file    rrm_sfi_util.c
 * @brief This file is the starting point for User Equipment Handler (UEH) module and contains
 * UE Manager Initialization Functions.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
 *     a) Created initial version from sample application
 */

/* RRM Related Includes */
#include "ueh.h"

/** GLOBALS **/
PUBLIC    SmCb smCb;
//PUBLIC    SmCfgCb smCfgCb;
PUBLIC FILE *callTrcFile;
/* end globals */

PUBLIC S16 regAndAttachTsk(
  const char* entName,            /* entity name */
  Ent ent,                        /* entity */
  Inst inst,                      /* instance */
  Ttype type,                     /* task type */
  Prior prior,                    /* task priority */
  PAIFS16 initTsk,                /* initialization function */
  ActvTsk actvTsk,                /* activation function */
  SSTskId sTskId                 /* System Task Id to which the TAPA Task it to be attached */
  )
{
  if (SRegTTsk(ent, inst, type, prior, initTsk, actvTsk) != ROK)
  {
     logError("SRegTTsk() failed for entity=%s(%d)", entName, ent);
     RETVALUE(RFAILED);
  }

  if (SAttachTTsk(ent, inst, sTskId)!= ROK)
  {
    logError("SAttachTTsk() failed for %s(%d)", entName, ent);
    RETVALUE(RFAILED);
  }

  logInfo("Registration of TAPA Task %s(%d) to system task %d OK", entName, ent, sTskId);
  RETVALUE(ROK);;
}
