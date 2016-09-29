/**
 * @file    module_names.h
 * @brief Defines the API to print module names in the logs
 * modu_names.c implements this method, mod_names.c is generated from shell script.
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @author     : Lakshmi Narayana MAdala
 * Date       :
 * Description:
 */

#ifndef MODULE_NAMES_H
#define MODULE_NAMES_H

#include "basic_types.h"
#include "bs_modules.h"

const UINT8* getModuleName(IN ModuleId modId);

#endif

