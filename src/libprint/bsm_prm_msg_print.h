/**
 * @file    bsm_prm_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and PRM
 * @author : Ramprasad Reddy Chinta
 * Date   :
 * Change History:
 */

#ifndef BSM_PRM_MSG_PRINT_H_
#define BSM_PRM_MSG_PRINT_H_

#include "all_cmn.h"
#include "bsm_prm_if.h"

void printMsgPrmBsmAllModulesUpInd(
    void    *data,
    UINT16  len
    );
void printMsgModUpInd(
    void    *data,
    UINT16  len
    );
void printMsgModDownInd(
    void    *data,
    UINT16  len
    );
void printMsgPrmBsmSwInstallCmpl(
  void    *data,
  UINT16  len
  );
void printMsgPrmBsmSwInstallFail(
  void    *data,
  UINT16  len
  );
void printMsgPrmBsmConfigInstallReq(
  void    *data,
  UINT16  len
  );
void PrintMsgBsmPrmConfigInstallSucc(
  void    *data,
  UINT16  len
  );
void PrintMsgBsmPrmConfigInstallFail(
  void    *data,
  UINT16  len
  );
void printMsgPrmBrmSwActSucc(
    void    *data,
    UINT16  len
    );
void printMsgPrmBrmSwActFail(
    void    *data,
    UINT16  len
    );
#endif /* BSM_PRM_MSG_PRINT_H_ */
