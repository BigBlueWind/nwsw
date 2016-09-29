/**
 * @file    ueh_udh_if.h
 * @brief This file contains UEH-UDH Internal Intf. hash defines
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Madhu K, 05May2011 :
 *     a) Created initial version
 */
#ifndef  UEH_UDH_IF_H
#define  UEH_UDH_IF_H

#include "ccpu_cmn.h"

#include "ueh.h"
#include "ludh.h"


PUBLIC S16 UehLiUdhBndReq
(
  Pst    *pst,
  SuId   suId,
  SpId   spId
);

PUBLIC S16 UehLiUdhAddRabReq
(
  Pst               *pst,
  SuId              suId,
  UehUdhAddRabReq   *addRabReq   /* UDH Add RAB structure */
);

PUBLIC S16 UehLiUdhDelRabReq
(
  Pst               *pst,        /* post structure */
  SuId              suId,        /* UDH serivce user SAP ID */
  UehUdhDelRabReq   *delRabReq   /* UDH Del RAB structure */
);

PUBLIC S16 UehLiUdhDelUeReq
(
  Pst               *pst,        /* post structure */
  SuId              suId,        /* UDH serivce user SAP ID */
  UehUdhDelUeReq    *delUeReq
);

PUBLIC S16 UehLiEgtEguLclTnlMgmtReq 
(
  Pst          *pst,
  SuId         suId,
  EgtUEvnt     *egtUEvnt
);

void createAndSendUdhUehDelUeCfm(
  UdhUehDelUeCfm  *udhUehDelUeCfm
  );
void createAndSendUdhUehAddRabCfm(
    UdhUehAddRabCfm  *udhUehAddRabCfm
  );

PUBLIC S16 udhProcUehUdhAddRabReq(
  Pst              *pst,
  SuId              suId,
  UehUdhAddRabReq  *uehUdhAddRabReq
  );

void createAndSendUdhUehDelRabCfm(
    UdhUehDelRabCfm  *udhUehDelRabCfm
  );
PUBLIC S16 udhProcUehUdhDelRabReq(
  Pst              *pst,
  SuId              suId,
  UehUdhDelRabReq  *uehUdhDelRabReq
  );



void createAndSendUdhUehBndCfm(
  Pst    *pst,
  SuId    suId,
  U8      status
  );
PUBLIC S16 udhProcUehUdhBndReq
(
  Pst       *pst,
  SuId      suId,
  SpId      spId
);
PUBLIC S16 udhUehInactiveUesInd
(
  Pst                 *pst,              /* post structure */
  SpId                *spId,             /* service Provider id */
  UdhUehUeInactiveInd *udhUehInactiveUeInd
);

void createAndSendUdhUehUeInactiveInd(
  UdhUehUeInactiveInd *udhUehUeInactiveInd
  );


PUBLIC S16 udhProcUehUdhDelUeReq(
  Pst              *pst,
  SuId              suId,
  UehUdhDelUeReq   *uehUdhDelUeReq
  );

PUBLIC S16 SmMiLudhBndCfm
(
  Pst          *pst,
  CmSapBindCfm *cmSapBindCfm,
  U8           *status
);

PUBLIC S16 SmMiLudhCfgCfm(
  Pst     *pst,
  SpId    spId,
  U8      status
  );
PUBLIC Void udhSendLmAlarm
(
U16 category,
U16 event,
U16 cause
);
PUBLIC Void smBuildUdhDbgCntrl(LogConfig *logConfig);
#endif /* UEH_UDH_IF_H */


