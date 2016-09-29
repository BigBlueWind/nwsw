/**
 * @file    bsm_rrm_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains the messages that are exchanged between BSM and RRM
 *
 * @author  Amar Ramteke
 * @date    12-08-2010  Initial version
 *
 * @details BSM = Base Station Manager
 * @details RRM = Radio Resources Manager
 *
 */

#ifndef BSM_RRM_IF_H
#define BSM_RRM_IF_H

#include "cmn_types.h"
#include "bs_tables.h"
#include "log.h"


/* ################# MESSAGE STRUCTURES ############################# */


/*
 * Messages from BSM to RRM
 */

/* #################### PHY Cell Config ###################### */

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_RRM_MESSAGES
 *  @{
 */

typedef struct s_MsgBsmRrmCellConfigReq
{
  UINT32 cellConfig;	
}MsgBsmRrmCellConfigReq;

/*
 * Messages from RRM to BSM
 */

/* #################### PHY Cell Config ###################### */

typedef struct s_MsgBsmRrmCellConfigCfm
{
  TBOOL success;	
} MsgRrmBsmCellConfigCfm;

/** @}
 */

/** @}
 */

#endif /* BSM_RRM_IF_H */
