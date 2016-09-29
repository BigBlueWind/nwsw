/**
 * @file    tput_tester_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains the prototypes of functions for the
 *          messages that are exchanged between COORD and NODE A and B
 * @author         :  Shweta Polepally
 * Date           :  01 July 2010
 * Change History :
 *
 */


#ifndef TPUT_TESTER_MSG_PRINT_H
#define TPUT_TESTER_MSG_PRINT_H

//#include "coord.h"
//#include "node.h"
#include "cmn_types.h"

void printMsgCoordNodeXSessionCreateReq(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgNodeXCoordSessionCreateSucc(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgNodeXCoordSessionCreateFail(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgCoordNodeXStartDataTx(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgNodeXCoordDataTxRxComplete(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgCoordNodeXSessionInfoReq(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgNodeXCoordSessionInfoRsp(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgCoordNodeXSessionDelReq(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgNodeXCoordSessionDelSucc(
    IO  void    *data,
    IO  UINT16  len
  );
void printMsgNodeXCoordSessionDelFail(
    IO  void    *data,
    IO  UINT16  len
  );
#endif
