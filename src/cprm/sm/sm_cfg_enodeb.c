/**
 * @file     sm_cfg_enodeb.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains Handlers for Configuration and Control Messages between SM and Protocol stacks
 * @author   Madhusudana Rao Kata
 * @date     01-Jun-2011
 * @details  ueh_sm_cfg.c is renamed to sm_cfg_enodeb.c @n
 */

#include "sm_includes.h"

/**
 * @param     event   Event is used to take respective action
 * @details   This function is used to configure and bind all the stack layers.
 */
PUBLIC Void smUehProcSm(
  U8 event
  )
{
  logFunc("%s","smUehProcSm");

  switch (smCb.smState)
  {
    case UEH_SM_STATE_INIT:
    {
      uehSmHdlTuclCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_TUCL_CFG_DONE:
    {
      uehSmHdlSctpCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_SCTP_CFG_DONE:
    {
      uehSmHdlS1apCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_S1AP_CFG_DONE:
    {
      uehSmHdlEgtpCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_EGTP_CFG_DONE:
    {
      uehSmHdlUdhCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_UDH_CFG_DONE:
    {
      uehSmHdlAppCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_APP_CFG_DONE:
    {
      uehSmHdlRrcCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_RRC_CFG_DONE:
    {
      uehSmHdlPdcpCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_PDCP_CFG_DONE:
    {
      uehSmHdlRlcCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_RLC_CFG_DONE:
    {
      uehSmHdlMacCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_MAC_CFG_DONE:
    {
      uehSmHdlCLCfgEvent(event);
      break;
    }
    case UEH_SM_STATE_CL_CFG_DONE:
    {
      /* Initialising Dbg Mask to Zero */
      //smCfgCb.smDbgMsk = 0;
      smCb.cfgCb.smDbgMsk = 0;
      uehSmHdlEnbDebugs();
      break;
    }
    case UEH_SM_STATE_ENB_DEBUGS_DONE:
    {
      uehSmHdlEnbAlarms(event);
      break;
    }
    case UEH_SM_STATE_ENB_ALRMS_DONE:
    {
      uehSmHdlBndS1apStack(event);
      break;
    }
    case UEH_SM_STATE_S1AP_STK_BND_DONE:
    {
      uehSmHdlBndEgtpStack(event);
      break;
    }
    case UEH_SM_STATE_EGTP_STK_BND_DONE:
    {
      uehSmHdlBndRrcStack(event);
      break;
    }
    case UEH_SM_STATE_RRC_STK_BND_DONE:
    {
      smCb.smState = UEH_SM_STATE_AWAIT_CELL_UP;
      uehSmHdlInitCellCfg(event);
      break;
    }

    case UEH_SM_STATE_CELL_UP:
      logError("%s","UEH_SM_STATE_CELL_UP");
      break;

    default:
      logInfo("%s","smUehProcSm : default");
      break;
  }
  RETVOID;
}

/**
 * @param     event   Event is used to take respective action on TUCL
 * @details   This function is used to configure TUCL in SM Init state.
 */
PUBLIC Void uehSmHdlTuclCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlTuclCfgEvent");

   if(event == EVTUEHTUCLCFG)
   {
      /* Start TUCL Configuration */
      uehSmHiCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }
   RETVOID;
}

/**
 * @param     event   Event is used to take respective action on receiving SCTP Config Succ
 * @details  This function is handles events in SCTP Config done state
 */
PUBLIC Void uehSmHdlSctpCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlSctpCfgEvent");

   if(event == EVTUEHSCTPCFG )
   {
      /* Start SCTP Configuration */
      uehSmSbCfg();
   }
   else
   {
      logError("%s", "Invalid event received");
   }
   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in S1AP Config done state.
 */
PUBLIC Void uehSmHdlS1apCfgEvent(
  U8 event
  )
{
  logFunc("%s","uehSmHdlSctpCfgEvent");

  if(event == EVTUEHS1APCFG )
  {
    /* Start S1AP Configuration */
    uehSmSzCfg();
  }
  else
  {
   logError("%s", "Invalid event received");
  }
  RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in EGTP Config done state.
 */
PUBLIC Void uehSmHdlEgtpCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlEgtpCfgEvent");

   if(event == EVTUEHEGTPCFG )
   {
      /* Start EGTP Configuration */
      uehSmEgCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }
   RETVOID;
}


/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in APP Config done state.
 */
PUBLIC Void uehSmHdlAppCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlAppCfgEvent");

   if(event == EVTUEHAPPCFG )
   {
      uehSmUehCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}


/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in RRC Config done state.
 */
PUBLIC Void uehSmHdlRrcCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlRrcCfgEvent");

   if(event == EVTUEHRRCCFG )
   {
      /* Start RRC Configuration */
      uehSmNhCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}


/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in UDH Config done state.
 */
PUBLIC Void uehSmHdlUdhCfgEvent
(
U8 event
)
{
   logFunc("%s","uehSmHdlUdhCfgEvent");

   if(event == EVTUEHUDHCFG )
   {
      /* Start UDH Configuration */
      uehSmUdhCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   } /* end of else part */

   RETVOID;
}


/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in PDCP Config done state.
 */
PUBLIC Void uehSmHdlPdcpCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlPdcpCfgEvent");

   if(event == EVTUEHPDCPCFG )
   {
      /* Start PDCP Configuration */
      uehSmPjCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }
   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in RLC Config done state.
 */
PUBLIC Void uehSmHdlRlcCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlRlcCfgEvent");

   if(event == EVTUEHRLCCFG )
   {
      /* Start RLC Configuration */
      uehSmKwCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in MAC Config done state.
 */
PUBLIC Void uehSmHdlMacCfgEvent(
  U8 event
  )
{
   logFunc("%s","uehSmHdlMacCfgEvent");

   if(event == EVTUEHMACCFG )
   {
      /* Start LTE MAC Configuration */
      uehSmRgCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles events in CL Config done state.
 */
PUBLIC Void uehSmHdlCLCfgEvent(
    U8 event
  )
{
   logFunc("%s","uehSmHdlCLCfgEvent");

   if(event == EVTUEHCLCFG )
   {
      /* Start LTE Convergence Layer Configuration */
      uehSmYsCfg();
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details    This function is used to Bind S1AP Stack with SM
 */
PUBLIC Void uehSmHdlBndS1apStack(
    U8 event
  )
{
   logFunc("%s","uehSmHdlBndS1apStack");

   if(event == EVTUEHS1APSTKBND)
   {
      /*Bind the SCTP lower SAP with TUCL */
      smBindSbToLSap(SB_HI_SPID);
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details    This function is used to Bind EGTP Stack with SM
 */
PUBLIC Void uehSmHdlBndEgtpStack(
    U8 event
  )
{
   logFunc("%s","uehSmHdlBndEgtpStack");

   if(event == EVTUEHEGTPSTKBND)
   {
      /*Bind the EGTP lower SAP with */
      smBindEgToHitSap(EG_HI_SPID);
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}
/**
 * @param     event   Event is used to take respective action
 * @details   This function is used to Bind RRC Stack with SM
 */
PUBLIC Void uehSmHdlBndRrcStack(
    U8 event
  )
{
   logFunc("%s","uehSmHdlBndRrcStack");

   if(event == EVTUEHRRCSTKBND)
   {
      /*Bind the MAC lower SAP with CL */
      smBindRgToTfuSap();
   }
   else
   {
     logError("%s", "Invalid event received");
   }

   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is used to enable alarms in Debug Config Done state,
 */
PUBLIC Void  uehSmHdlEnbAlarms(
    U8 event
  )
{
   Pst  uehPst;

   logFunc("%s","uehSmHdlEnbAlarms");

   SM_SET_ZERO(&uehPst, sizeof(Pst));

   if(event == EVTUEHENBALRMS)
   {
      /* Send Control request to enable alarms in required
       * layers */
      smBuildSzAlarmCntrl();
      smBuildNhAlarmCntrl();
      /* TODO Check why we need this in UEH  */
      smBuildUehAlarmCntrl();

      smCb.smState = UEH_SM_STATE_ENB_ALRMS_DONE;
      UEH_FILL_PST(uehPst, SM_UEH_PROC, ENTSM, ENTUEH, EVTUEHS1APSTKBND, UEH_POOL, SM_SELECTOR_LC);
      uehSmSelfPstMsg(&uehPst);
   }
   else
   {
      logError("Invalid event received: %d", event);
   }

   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is used to enable eNodeB Debugs
 */
PUBLIC Void  uehSmHdlEnbDebugs(
  )
{
   Pst lclPst;
   U8  nextEvent = 0;

   logFunc("%s","uehSmHdlEnbDebugs");

   SM_SET_ZERO(&lclPst, sizeof(Pst));

   logInfo("%s", "DEBUG enabled as per the configuration");

   /* Assuming after all Debug Cntrl Reqs. */
   smCb.smState = UEH_SM_STATE_ENB_DEBUGS_DONE;
   nextEvent = EVTUEHENBALRMS;
   UEH_FILL_PST(lclPst, SM_UEH_PROC, ENTSM, ENTUEH, nextEvent, UEH_POOL, SM_SELECTOR_LC);
   uehSmSelfPstMsg(&lclPst);

   RETVOID;
}

/**
 * @param     event   Event is used to take respective action
 * @details   This function is handles all events in done state, Send Control request to APP to indicate binding is complete
 */
PUBLIC Void  uehSmHdlInitCellCfg(
    U8 event
  )
{
   logFunc("%s","uehSmHdlInitCellCfg");

   if(event == EVTUEHCELLCFG)
   {
      /* Send Control request to APP to indicate binding is complete*/
      smBuildUehCellCntrl();
   }
   else
   {
     logError("%s","Invalid event received");
   }

   RETVOID;
}
