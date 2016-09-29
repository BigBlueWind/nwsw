/**
 * @file    imc_module.h (Definition of module and properties)
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief All module related types and values are defined here.
 *
 * @author : Lakshmi Narayana Madala
 * Date   :
 * Description :
 */

#ifndef IMC_MODULE_H
#define IMC_MODULE_H

#include "basic_types.h"
#include "cmn_types.h"
#include "bs_modules.h"
#include "imc_timer.h"

typedef struct s_ModContext
{
  ModuleId   modId;       /** unique identifier of the module */
  String50   exeName;     /** executable name */
  FILE      *logFilePtr;  /** logfile */
  ErrChain   errChain;    /** errors encountered by functions are stored here */
  void      *exeContext;  /** Module Specific Context, depending on type of module */
}ModContext;

ModContext *getModuleContext(void);

TBOOL modInit(
  IN ModuleId  modId,
  IN UINT8    *exeName,
  IN UINT8    *rootDir,
  IN TimeSpec  tick
  );

void  modExit(
  IN ModuleId  modId,
  IN UINT8    *exeName
);


#endif /* IMC_MODULE_H */


