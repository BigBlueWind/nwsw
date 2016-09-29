/**
 * @file    bsm_fth_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and FTH
 * @author :  Lakshmi Narayana Madala
 * Date   :
 * Change History:
 */

#ifndef BSM_FTH_MSG_PRINT_H
#define BSM_FTH_MSG_PRINT_H

#include "bsm_fth_if.h"
#include "tlv_enc_dec.h"

void printMsgFthBsmSwDownloadFail(
  void    *data,
  UINT16  len
  );
void printMsgFthBsmSwDownloadCmpInd(
  void    *data,
  UINT16  len
  );
void printMsgFthBsmSwUploadCmpInd(
  void    *data,
  UINT16  len
  );
void printMsgFthBsmSwUploadFail(
  void    *data,
  UINT16  len
  );
#endif //BSM_FTH_MSG_PRINT_H
