/**
 * @file    imc_util.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains prototypes of utility functions
 *
 * @author :   Lakshmi Narayana Madala
 * Date   :
 * Description
 *
 */

#ifndef IMC_UTIL_H
#define IMC_UTIL_H

#include "all_cmn.h"
#include "imc_cfg.h"
#include <assert.h>

/** File Functions */
UINT8 *getBasename (
    IN UINT8 *name
    );

TBOOL setIpAddressOfEth(
  IN UINT8     *eth,
  IN UINT8     *ipAddr,
  IO UINT8     *ipNetMask
  );

TBOOL getIpAddressOfEth(
  IN UINT8     *eth,
  IO UINT8     *ipAddr,
  IO UINT8     *ipNetMask
  );

/** String Functions */
void setString100(
    IO String100 *str,
    IN UINT8 *val
    );

void appendString100(
    IO String100 *str,
    IN UINT8 *val
    );

void reverseString(
    UINT8 *src,
    UINT8 *dst
    );

/** Misc */
#define convertIntToAscii(i) ((i) + 48)

Msg *recvMsgAtMod(
    IN ModuleId modId,
    IN MsgId timeoutMsgId
    );

Msg *recvMsgAtMod2(
    //IN ModuleId modId,
    IN MsgId    timeoutMsgId,
    IN UINT32   timeoutMsgWaitMsec
    );

TBOOL addModulo(
    IN UINT16 received,
    IN UINT16 expected,
    IN UINT16 modulo,
    IO UINT16 *result
    );

TBOOL subtractModulo(
    IN UINT16 received,
    IN UINT16 expected,
    IN UINT16 modulo,
    IO UINT16 *result
    );

UINT8 parseCmdString(
  IO UINT8           *inputStr,
  IO DataFromFileStr tokens[]
  );
void initImcContext(
  IN UINT8*   configFilePath
);
void constructFullPath(
  IN UINT8   *configFilePath,
  IN UINT8   *filename,
  IO UINT8    fullFilePath[]
  );
void itoa(
    IO SINT8 number,
    IO SINT8 str[]
    );
void reverse(
    IO SINT8 str[]
    );
void imcReplaceByte(
    IO String50 str,
    IN SINT8     from ,
    IN SINT8     to
    );
void getLapsedTime(
  IN struct timeval  *first,
  IO struct timeval  *second,
  IO struct timeval  *lapsed
);
void readImcCfgFile(
  IN UINT8       *fileToRead,
  IO ImcContext  *imcContext,
  IO UINT8       *error);
#endif // IMC_UTIL
