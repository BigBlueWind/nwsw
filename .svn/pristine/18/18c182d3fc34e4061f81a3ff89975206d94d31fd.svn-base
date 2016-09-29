/**
 * @file    s1ap_err.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @details
 * This file contains S1AP errors mapping with BS errors.
 *
 * @author Shweta Polepally
 *
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "ueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "s1ap.h"
#include "s1ap_cm.h"
#include "s1ap_wrap.h"
#include "s1ap_unwrap.h"
#include "s1ap_cmn_proc.h"
#include "s1ap_mgr.h"
#include "s1ap_sm.h"
#include "s1ap.h"
#include "bs_err.h"
#include "ueh_crm_sm.h"

#include "log.h"
#include "s1ap_err.h"


void s1apFillErrorValueByRadioNwCauseVal(
  IN EnumSztCauseRadioNw   causeVal,
  IN S1apMgrCb            *s1Cb,
  IO BsErrorValue         *error
  )
{
  switch( causeVal )
  {
     case SztCauseRadioNwunspecifiedEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_UNSPECIFIED;
       } break;
     case SztCauseRadioNwtx2relocoverall_expiryEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_TX2RELOCOVERALL_EXPIRY;
       } break;
     case SztCauseRadioNwsuccessful_handoverEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_SUCCESSFUL_HANDOVER;
       } break;
     case SztCauseRadioNwrelease_due_to_eutran_generated_reasonEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_RELEASE_DUE_TO_EUTRAN_GENERATED_REASON;
       } break;
     case SztCauseRadioNwhandover_cancelledEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_HANDOVER_CANCELLED;
       } break;
     case SztCauseRadioNwpartial_handoverEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_PARTIAL_HANDOVER;
       } break;
     case SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_HO_FAILURE_IN_TARGET_EPC_ENB_OR_TARGET_SYSTEM;
       } break;
     case SztCauseRadioNwho_target_not_allowedEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_HO_TARGET_NOT_ALLOWED;
       } break;
     case SztCauseRadioNwtS1relocoverall_expiryEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_TS1RELOCOVERALL_EXPIRY;
       } break;
     case SztCauseRadioNwtS1relocprep_expiryEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_TS1RELOCPREP_EXPIRY;
       } break;
     case SztCauseRadioNwcell_not_availableEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_CELL_NOT_AVAILABLE;
       } break;
     case SztCauseRadioNwunknown_targetIDEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_UNKNOWN_TARGETID;
       } break;
     case SztCauseRadioNwno_radio_resources_available_in_target_cellEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_NO_RADIO_RESOURCES_AVAILABLE_IN_TARGET_CELL;
       } break;
     case SztCauseRadioNwunknown_mme_ue_s1ap_idEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_UNKNOWN_MME_UE_S1AP_ID;
       } break;
     case SztCauseRadioNwunknown_enb_ue_s1ap_idEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_UNKNOWN_ENB_UE_S1AP_ID;
       } break;
     case SztCauseRadioNwunknown_pair_ue_s1ap_idEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_UNKNOWN_PAIR_UE_S1AP_ID;
       } break;
     case SztCauseRadioNwhandover_desirable_for_radio_reasonEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_HANDOVER_DESIRABLE_FOR_RADIO_REASON;
       } break;
     case SztCauseRadioNwtime_critical_handoverEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_TIME_CRITICAL_HANDOVER;
       } break;
     case SztCauseRadioNwresource_optimisation_handoverEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_RESOURCE_OPTIMISATION_HANDOVER;
       } break;
     case SztCauseRadioNwreduce_load_in_serving_cellEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_REDUCE_LOAD_IN_SERVING_CELL;
       } break;
     case SztCauseRadioNwuser_inactivityEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_USER_INACTIVITY;
       } break;
     case SztCauseRadioNwradio_connection_with_ue_lostEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_RADIO_CONNECTION_WITH_UE_LOST;
       } break;
     case SztCauseRadioNwload_balancing_tau_requiredEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_LOAD_BALANCING_TAU_REQUIRED;
       } break;
     case SztCauseRadioNwcs_fallback_triggeredEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_CS_FALLBACK_TRIGGERED;
       } break;
     case SztCauseRadioNwue_not_available_for_ps_serviceEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_UE_NOT_AVAILABLE_FOR_PS_SERVICE;
       } break;
     case SztCauseRadioNwradio_resources_not_availableEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_RADIO_RESOURCES_NOT_AVAILABLE;
       } break;
     case SztCauseRadioNwfailure_in_radio_interface_procedureEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_FAILURE_IN_RADIO_INTERFACE_PROCEDURE;
       } break;
     case SztCauseRadioNwinvalid_qos_combinationEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_INVALID_QOS_COMBINATION;
       } break;
     case SztCauseRadioNwinterrat_redirectionEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_INTERRAT_REDIRECTION;
       } break;
     case SztCauseRadioNwinteraction_with_other_procedureEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_INTERACTION_WITH_OTHER_PROCEDURE;
       } break;
     case SztCauseRadioNwunknown_E_RAB_IDEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_UNKNOWN_E_RAB_ID;
       } break;
     case SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_MULTIPLE_E_RAB_ID_INSTANCES;
       } break;
     case SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_ENCRYPTION_AND_OR_INTEGRITY_PROTECTION_ALGORITHMS_NOT_SUPPORTED;
       } break;
     case SztCauseRadioNws1_intra_system_handover_triggeredEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_S1_INTRA_SYSTEM_HANDOVER_TRIGGERED;
       } break;
     case SztCauseRadioNws1_inter_system_handover_triggeredEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_S1_INTER_SYSTEM_HANDOVER_TRIGGERED;
       } break;
     case SztCauseRadioNwx2_handover_triggeredEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_X2_HANDOVER_TRIGGERED;
       } break;
     case SztCauseRadioNwredirection_towards_1xRTTEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_REDIRECTION_TOWARDS_1XRTT;
       } break;
     case SztCauseRadioNwnot_supported_QCI_valueEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_NOT_SUPPORTED_QCI_VALUE;
       } break;
#if S1AP_REL851
     case SztCauseRadioNwinvalid_CSG_IdEnum:
       {
         *error = BS_ERR_S1AP_RADIONW_INVALID_CSG_ID;
       } break;
#endif
     default:
       logError( "RadioNw Error %d error not mapped with BS error. Check code ",  causeVal );
       break;
  }
}

void s1apFillErrorValueByTransPortCauseVal(
  IN EnumSztCauseTport   causeVal,
  IN S1apMgrCb           *s1Cb,
  IO BsErrorValue        *error
  )
{
  switch( causeVal )
  {
    case SztCauseTporttransport_resource_unavailableEnum:
      {
        *error = BS_ERR_S1AP_TPORT_TRANSPORT_RESOURCE_UNAVAILABLE;
      } break;
    case SztCauseTportunspecifiedEnum:
      {
        *error = BS_ERR_S1AP_TPORT_UNSPECIFIED;
      } break;
    default:
      logError( "TransPort Error %d error not mapped with BS error. Check code ",  causeVal );
      break;
  }
}

void s1apFillErrorValueByNasCauseVal(
  IN EnumSztCauseNas     causeVal,
  IN S1apMgrCb           *s1Cb,
  IO BsErrorValue        *error
  )
{
  switch( causeVal )
  {
    case SztCauseNasnormal_releaseEnum:
      {
        *error = BS_ERR_S1AP_NAS_NORMAL_RELEASE;
      } break;
    case SztCauseNasauthentication_failureEnum:
      {
        *error = BS_ERR_S1AP_NAS_AUTHENTICATION_FAILURE;
      } break;
    case SztCauseNasdetachEnum:
      {
        *error = BS_ERR_S1AP_NAS_DETACH;
      } break;
    case SztCauseNasunspecifiedEnum:
      {
        *error = BS_ERR_S1AP_NAS_UNSPECIFIED;
      } break;
#if S1AP_REL851
    case SztCauseNascsg_subscription_expiryEnum:
      {
        *error = BS_ERR_S1AP_NAS_CSG_SUBSCRIPTION_EXPIRY;
      } break;
#endif
    default:
      logError( "NAS Error %d error not mapped with BS error. Check code ",  causeVal );
      break;
  }
}

void s1apFillErrorValueByProtocolCauseVal(
  IN EnumSztCauseProt    causeVal,
  IN S1apMgrCb           *s1Cb,
  IO BsErrorValue        *error
  )
{
  switch( causeVal )
  {
    case SztCauseProttransfer_syntax_errorEnum:
      {
        *error = BS_ERR_S1AP_PROT_TRANSFER_SYNTAX_ERROR;
      } break;
    case SztCauseProtabstract_syntax_error_rejectEnum:
      {
        *error = BS_ERR_S1AP_PROT_ABSTRACT_SYNTAX_ERROR_REJECT;
      } break;
    case SztCauseProtabstract_syntax_error_ignore_and_notifyEnum:
      {
        *error = BS_ERR_S1AP_PROT_ABSTRACT_SYNTAX_ERROR_IGNORE_AND_NOTIFY;
      } break;
    case SztCauseProtmessage_not_compatible_with_receiver_stateEnum:
      {
        *error = BS_ERR_S1AP_PROT_MESSAGE_NOT_COMPATIBLE_WITH_RECEIVER_STATE;
      } break;
    case SztCauseProtsemantic_errorEnum:
      {
        *error = BS_ERR_S1AP_PROT_SEMANTIC_ERROR;
      } break;
    case SztCauseProtabstract_syntax_error_falsely_constructed_messageEnum:
      {
        *error = BS_ERR_S1AP_PROT_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE;
      } break;
    case SztCauseProtunspecifiedEnum:
      {
        *error = BS_ERR_S1AP_PROT_UNSPECIFIED;
      } break;
    default:
      logError( "Protocol Error %d error not mapped with BS error. Check code ",  causeVal );
      break;
  }
}

void s1apFillErrorValueByMiscCauseVal(
  IN EnumSztCauseMisc   causeVal,
  IN S1apMgrCb          *s1Cb,
  IO BsErrorValue       *error,
  IO UINT8              *errString
  )
{
  switch ( causeVal )
  {
    /* TODO - 23June2011-Shweta: Add the values for logging as and when required */
    case SztCauseMisccontrol_processing_overloadEnum:
    {
      logError( "%s", "Control Processing Overload Occurred" );
      *error = BS_ERR_S1AP_MISC_CONTROL_PROCESSING_OVERLOAD;
    } break;
    case SztCauseMiscnot_enough_user_plane_processing_resourcesEnum:
    {
      logError( "%s", "Not Enough User Plane Processing Resource" );
      *error = BS_ERR_S1AP_MISC_NOT_ENOUGH_USER_PLANE_PROCESSING_RESOURCES;
    } break;
    case SztCauseMischardware_failureEnum:
    {
      logError( "%s", "Hardware failure" );
      *error = BS_ERR_S1AP_MISC_HARDWARE_FAILURE;
    } break;
    case SztCauseMiscom_interventionEnum:
    {
      logError( "%s", "COM Intervention Occurred" );
      *error = BS_ERR_S1AP_MISC_OM_INTERVENTION;
    } break;
    case SztCauseMiscunspecifiedEnum:
    {
      logError( "%s", "S1AP Error Occurred, Error not specified" );
      *error = BS_ERR_S1AP_MISC_UNSPECIFIED;
    } break;
    /* Check the cause type, it it is unknown PLMN then break the SCTP association */
    case SztCauseMiscunknown_PLMNEnum:
    {
      S8  plmnIdStr[MAX_SIZE_PLMN_STRING];
      getPlmnIdsFromTac(  &(s1Cb->tacs[0]), plmnIdStr);
      logError("Received UNKNOWN PLMN ID :%s from MME", plmnIdStr );
      *error = BS_ERR_S1AP_MISC_UNKNOWN_PLMN;
    } break;
    default:
      logError( "Misc Error %d error not mapped with BS error. Check code ",  causeVal );
      break;
  }
}
