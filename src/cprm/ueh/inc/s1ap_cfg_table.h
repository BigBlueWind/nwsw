/**
 * @file    s1ap_cfg_table.h
 * @brief This file
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author :           Lakshmi Narayana Madala
 * Date   :           06-Oct-2010
 * Change History :
 */

#include "basic_types.h"

#ifndef S1AP_CFG_TABLE_H
#define S1AP_CFG_TABLE_H

/* This file contains the S1AP configuration parameters that are used by S1AP stack
 *
 */
#if 0
typedef struct s_S1apCfgTable
{
   UINT16                             nmbSztSaps;
   UINT16                             nmbSctSaps;
   UINT32                             nmbPeer;
   UINT16                             timeRes;
   SINT16                             maxBndRetry;
   TBOOL                              bndTmCfgEnabled;
   UINT16                             bndTmCfgVal;
   UINT16                             s1apLocalPort;
   UINT16                             s1apGuardTimer;
   TBOOL                              assocCfg_tmrAssocRecov_enb;
   UINT16                             assocCfg_tmrAssocRecov_val;
   UINT16                             assocCfg_nmbRetry;
   UINT16                             maxS1SetupAllowed;
}S1apCfgTable;

void s1apConfigInit(
  IO S1apCfgTable  *s1apCfgTable
  );
TBOOL readS1apConfigTable(
  IN UINT8         *fileToRead,
  IO S1apCfgTable  *s1apCfgTable
  );

#endif
#endif /* S1AP_CFG_TABLE_H */
