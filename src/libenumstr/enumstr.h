/**
 * @file    enumstr.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains Enum To String
 *
 * @author: Madhusudana Rao Kata
 * Date: 28-09-2010
 * Description: Draft version 
 */

#ifndef ENUMSTR_H
#define ENUMSTR_H

#include "basic_types.h"
#include "cmn_types.h"
#include "enumstr_oamtables.h"
#include "ueh_pmh_if.h"
/* Structure to display Enums into strings */
typedef UINT8* (*EnumStrHdlr)(IO UINT32  enumNum);

typedef struct s_EnumStrInfo
{
  EnumStrHdlr   enumStrHdlr;
  TableId       tableId;
  ColId         colId;
} EnumStrInfo;

typedef struct s_NotiEnumStrInfo
{
  EnumStrHdlr   enumStrHdlr;
  NotiId        notiId;
  ColId         colId;
} NotiEnumStrInfo;

/**
 * @details Converts the BsErrorValue ENUM value into strings
 * @param  enumNum   BsErrorValue ENUM value
 */
const UINT8* errorCodeStr( IN BsErrorValue enumNum);

char *getCounterIdStr( IN CounterId counterId );

char *getMeasurementStoppedCauseStr(MeasurementStopCause cause );

/* Converts Module Id Enum to String */
UINT8* enumModIdToStr(UINT32 enumNum);

/* Converts string to Module Id Enum */
ModuleId strModIdToEnum(UINT8* modIdStr);

#endif /* ENUMSTR_H */
