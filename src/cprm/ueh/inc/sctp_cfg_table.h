/**
 * @file    sctp_cfg_table.h
 * @brief This file
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author :           Lakshmi Narayana Madala
 * Date   :           06-Oct-2010
 * Change History :
 */

#include "basic_types.h"

#ifndef SCTP_CFG_TABLE_H
#define SCTP_CFG_TABLE_H
/* This file contains the EGTP configuration parameters that are used by EGTP stack
 *
 */
#if 0
typedef struct s_SctpCfgTable
{
   TBOOL     isSctpPwavConfig_;
   UINT16    maxNmbSctSaps;
   UINT16    maxNmbTSaps;
   UINT16    maxNmbAssoc;
   UINT16    maxNmbEndp;
   UINT16    maxNmbDstAddr;
   UINT16    maxNmbSrcAddr;
   UINT32    maxNmbTxChunks;
   UINT32    maxNmbRxChunks;
   UINT16    maxNmbInStrms;
   UINT16    maxNmbOutStrms;
   UINT32    initARwnd;
   UINT16    mtuInitial;
   UINT16    mtuMinInitial;
   UINT16    mtuMaxInitial;
   TBOOL     performMtu;
   UINT16    timeRes;
   UINT8     reConfig_MaxInitReTx;
   UINT8     reConfig_MaxAssocReTx;
   UINT8     reConfig_MaxPathReTx;
   TBOOL     reConfig_AltAcceptFlg;
   UINT16    reConfig_KeyTm;
   UINT16    reConfig_Alpha;
   UINT16    reConfig_Beta;
   UINT16    reConfig_TIntTmr;
   UINT8     reConfig_MaxBndRetry;
   UINT16    reConfig_MaxAckDelayTm;
   UINT16    reConfig_MaxAckDelayDg;
   UINT16    reConfig_RtoInitial;
   UINT16    reConfig_RtoMin;
   UINT16    reConfig_RtoMax;
   UINT16    reConfig_FreezeTm;
   UINT16    reConfig_BundleTime;
   UINT16    reConfig_CookieLife;
   UINT16    reConfig_HbeatIntervalTm;
   UINT16    reConfig_MaxBurst;
   UINT16    reConfig_MaxHbBurst;
   UINT16    reConfig_T5SdownGrdTm;
   TBOOL     reConfig_HandleInitFlg;
   TBOOL     reConfig_NegAbrtFlg;
   TBOOL     reConfig_HBeatEnable;
   UINT32    reConfig_FlcUpThr;
   UINT32    reConfig_FlcLowThr;
}SctpCfgTable;


void sctpConfigInit(
  IO SctpCfgTable  *sctpCfgTable
  );
TBOOL readSctpConfigTable(
  IN UINT8         *fileToRead,
  IO SctpCfgTable  *sctpCfgTable
  );
#endif
#endif /* SCTP_CFG_TABLE_H */
