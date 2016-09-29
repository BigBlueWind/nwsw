/*
 * ueh_cmn_sec.h
 *
 * This file contains Security common Procedures.
 * @author: lmadala
 *
 *
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh.h"
#include "lueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "s1ap_cm.h"

#ifndef UEH_SMN_SEC_H
#define UEH_SMN_SEC_H

#define KEY_DERIVATION_STRING_LENGTH            7
#define SIZE_EXTRACT_KEY_FROM_HMAC              32

#define EEA1_ALGO_SUPPORT                       0x80
#define EEA2_ALGO_SUPPORT                       0x40

#define EIA1_ALGO_SUPPORT                       0x80
#define EIA2_ALGO_SUPPORT                       0x40

#define ALGO_DISTINGUISHER_RRC_ENC              0x03
#define ALGO_DISTINGUISHER_RRC_INT              0x04
#define ALGO_DISTINGUISHER_UP_ENC               0x05

#define RRC_EIA1						1
#define RRC_EIA2						2

#define RRC_EEA0						0
#define RRC_EEA1						1
#define RRC_EEA2						2


PUBLIC S16 uehGetCiphAlgoBasedOnEnbCap(
    UehUeSecCfg  *secCfg
    );
PUBLIC S16 uehGetIntAlgoBasedOnEnbCap(
    UehUeSecCfg  *secCfg
    );
PUBLIC S16 uehExtKey(
    UehUeSecCfg  *secCfg,
    SecurityKey  *securityKey,
    UINT8         type,
    U16           cellId,
    U32           crnti
  );

PUBLIC S16 uehKDF
(
  UehUeSecCfg  *secCfg,
  U8  *secKey,
  U32 SecKeyType,
  U16 crnti
);
PUBLIC S16 uehExtEncIntKeysFromKeNB(
    UehUeSecCfg  *secCfg,
    SecurityKey  *securityKey,
    U16           cellId,
    U16           crnti           
  );
PUBLIC S16 uehGetSecInfoFrmInitCtxSetupReq(
    UehUeCb             *ueCb,
    InitialContextSetup *initialContextSetup
  );

#endif /* UEH_SMN_SEC_H */
