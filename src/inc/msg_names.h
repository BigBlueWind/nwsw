/**
 * @file    msg_names.h
 * @brief Defines the API to print message names
 * msg_names.c implements this method, msg_names.c is generated from shell script.
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @author     : Lakshmi Narayana MAdala
 * Date       :
 * Description:
 */

#ifndef MSG_NAMES_H
#define MSG_NAMES_H

#include "basic_types.h"
#include "msg_ids.h"

const UINT8* getMsgName(IN MsgId msgId);

#endif

