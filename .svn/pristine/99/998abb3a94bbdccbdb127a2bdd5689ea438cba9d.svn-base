#include "ueh_cmn_sec.h"




PUBLIC S16 uehGetCiphAlgoBasedOnEnbCap(
    UehUeSecCfg  *secCfg
    )
{
  switch(secCfg->ciphAlgoSupported)
  {
    case EEA1_ALGO_SUPPORT:
      if (NHU_CIPHER_ALGO_EEA1 != uehCb.uehCfgCb.securityConfig.uuCiphering)
      RETVALUE(RFAILED); 

    case EEA2_ALGO_SUPPORT:
      if (NHU_CIPHER_ALGO_EEA2 != uehCb.uehCfgCb.securityConfig.uuCiphering)
      RETVALUE(RFAILED); 

    case EEA1_ALGO_SUPPORT + EEA2_ALGO_SUPPORT:
      break;

    default:
      secCfg->ciphAlgoSelected = NHU_CIPHER_ALGO_EEA0;
      RETVALUE(ROK); 
  }
  secCfg->ciphAlgoSelected = uehCb.uehCfgCb.securityConfig.uuCiphering;
  RETVALUE(ROK);
}

PUBLIC S16 uehGetIntAlgoBasedOnEnbCap(
    UehUeSecCfg  *secCfg
    )
{
  switch(secCfg->intgAlgoSupported)
  {
    case EIA1_ALGO_SUPPORT:
      if (NHU_INTG_ALGO_EIA1 != uehCb.uehCfgCb.securityConfig.uuIntegrityProtection)
      RETVALUE(RFAILED);

    case EIA2_ALGO_SUPPORT:
      if (NHU_INTG_ALGO_EIA2 != uehCb.uehCfgCb.securityConfig.uuIntegrityProtection)
      RETVALUE(RFAILED);

    case EIA1_ALGO_SUPPORT + EIA2_ALGO_SUPPORT:
      break;

    default:
      secCfg->intgAlgoSelected = 0 /*NHU_INTG_ALGO_EIA0*/;
      RETVALUE(ROK); 
  }
  secCfg->intgAlgoSelected = uehCb.uehCfgCb.securityConfig.uuIntegrityProtection;
  RETVALUE(ROK);
}

PUBLIC S16 uehExtEncIntKeysFromKeNB(
    UehUeSecCfg  *secCfg,
    SecurityKey  *securityKey,
    U16          cellId,
    U16          crnti
  )
{  
  if (RFAILED == uehGetCiphAlgoBasedOnEnbCap( secCfg ))
  {
    RETVALUE(RFAILED); 
  }
  if (RFAILED == uehGetIntAlgoBasedOnEnbCap( secCfg ))
  {
    RETVALUE(RFAILED); 
  }
  uehKDF(secCfg,(U8 *)securityKey->securityKey, CTF_KDF_TYPE1,crnti);
  
  RETVALUE(ROK);
}


/*
*
*       Fun:   uehFetchSecInfo
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehGetSecInfoFrmInitCtxSetupReq(
    UehUeCb             *ueCb,
    InitialContextSetup *initialContextSetup
  )
{
  UECM_FUNC(ueCb->crnti,uehGetSecInfoFrmInitCtxSetupReq,UECM,SEC,"-");

  ueCb->secCfg.ciphAlgoSupported = 0;
  ueCb->secCfg.intgAlgoSupported = 0;
  /* Check the Encryption Algorithms */
  if( initialContextSetup->ueSecurityCapability.encryptionAlgorithms[0] & EEA1_ALGO_SUPPORT )
  {
    ueCb->secCfg.ciphAlgoSupported |= EEA1_ALGO_SUPPORT;
  }
  if( initialContextSetup->ueSecurityCapability.encryptionAlgorithms[0] & EEA2_ALGO_SUPPORT )
  {
    ueCb->secCfg.ciphAlgoSupported |= EEA2_ALGO_SUPPORT;
  }

  /* Check the Integrity Algorithms */
  if( initialContextSetup->ueSecurityCapability.integrityAlgorithms[0] & EIA1_ALGO_SUPPORT )
  {
    ueCb->secCfg.intgAlgoSupported |= EIA1_ALGO_SUPPORT;
  }
  if( initialContextSetup->ueSecurityCapability.integrityAlgorithms[0] & EIA2_ALGO_SUPPORT )
  {
    ueCb->secCfg.intgAlgoSupported |= EIA2_ALGO_SUPPORT;
  }

  /* AS Per release8 , Integrity Algorithm is Mandatory */
  if(0 == (ueCb->secCfg.intgAlgoSupported & EIA2_ALGO_SUPPORT))
  {
    logError("uehGetSecInfoFromInitialCtxSetuReq: Integrity Algorith:%d not Supported by the UE ", EIA2_ALGO_SUPPORT);
    RETVALUE(RFAILED);
  }  
  /* Now Extract the Keys, KrrcInt, KrrcCiph, KupCiph  from KeNB */ 
  if (RFAILED == uehExtEncIntKeysFromKeNB(&ueCb->secCfg, &initialContextSetup->securityKey,ueCb->cellId,ueCb->crnti ))
  {
    uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwfailure_in_radio_interface_procedureEnum);
  }

  RETVALUE(ROK);
}
