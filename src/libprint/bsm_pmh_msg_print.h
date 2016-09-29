/**
 * @file    bsm_pmh_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and PMH
 * @author :  Ramprasad Reddy
 * Date   :
 * Change History:
 */

#ifndef BSM_PMH_MSG_PRINT_H_
#define BSM_PMH_MSG_PRINT_H_

#include "log.h"
#include "bsm_pmh_if.h"
#include "print_cmn.h"
#include "enumstr.h"
#include "print_cmn.h"


void printMsgBsmPmhConfigReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgBsmPmhCntrInputConfigReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printCntrFileAvailableMsg(
    IO  void    *data,
    IO  UINT16  len
    );
void printCntrCollectionFailMsg(
    IO  void    *data,
    IO  UINT16  len
    );
void printCntrFileUploadedMsg(
    IO  void    *data,
    IO  UINT16  len
    );
void printMsgCounterClearReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgCounterClearSucc(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgCounterClearFail(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgBsmPmhDiagCounterCmd(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgPmhBsmDiagCounterCmdRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgPmhBsmMeasurementCompleteInd(
    IO  void    *data,
    IO  UINT16  len
    );

#endif /* BSM_PMH_MSG_PRINT_H_ */
