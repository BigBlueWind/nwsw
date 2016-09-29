/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/
/**********************************************************************

     Name:     LTE-X2AP Layer

     Type:     C include file

     Desc:     Structures, variables and typedefs required by the LTE-X2AP
               upper interface.

     File:     czt_asn.x

     Sid:      czt_asn.x@@/main/2 - Thu Aug 25 17:58:13 2011

     Prg:     ad

**********************************************************************/

/* czt_asn_x_001.main_1 : This file is generated newly for X2AP 3.1 release. */

#ifndef __CZTASNX__
#define __CZTASNX__ 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Element ----- Criticality ------- */
typedef enum enumCztCriticality
{
        CztCriticalityrejectEnum,
        CztCriticalityignoreEnum,
        CztCriticalitynotifyEnum
} EnumCztCriticality;
typedef TknU32 CztCriticality;

/* Element ----- Presence ------- */
typedef enum enumCztPresence
{
        CztPresenceoptionalEnum,
        CztPresenceconditionalEnum,
        CztPresencemandatoryEnum
} EnumCztPresence;
typedef TknU32 CztPresence;

/* Element ----- local ------- */
typedef TknU32 CztPrivIE_IDlocal;
typedef TknU32 Cztglobal; /*czt_asn_x_001.main_1 : X2AP 3.1 */

typedef struct x2apPrivIE_ID
{
        TknU8 choice;
        union {
                CztPrivIE_IDlocal local;
                Cztglobal global; 
        } val;
} CztPrivIE_ID;

/* Element ----- ProcedureCode ------- */
typedef TknU32 CztProcedureCode;

/* Element ----- ProtocolIE-ID ------- */
typedef TknU32 CztProtIE_ID;

/* Element ----- TriggeringMessage ------- */
typedef enum enumCztTrgMsg
{
        CztTrgMsginitiating_messageEnum,
        CztTrgMsgsuccessful_outcomeEnum,
        CztTrgMsgunsuccessful_outcomeEnum
} EnumCztTrgMsg;
typedef TknU32 CztTrgMsg;

/* Element ---------- Key-eNodeB-Star ----------- */
typedef TknStrBSXL CztKey_eNodeB_Star;
/* Element ----- NextHopChainingCount ------- */
typedef TknU32 CztNextHopChainingCount;

typedef struct x2ap_ExtnAS_SecurInform_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnAS_SecurInform_ExtIEsCls;
typedef struct x2apProtExtnField_AS_SecurInform_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnAS_SecurInform_ExtIEsCls extensionValue;
} CztProtExtnField_AS_SecurInform_ExtIEs;

typedef struct x2apProtExtnCont_AS_SecurInform_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_AS_SecurInform_ExtIEs *member;
}CztProtExtnCont_AS_SecurInform_ExtIEs;

typedef struct x2apAS_SecurInform
{
        TknPres pres;
        CztKey_eNodeB_Star key_eNodeB_star;
        CztNextHopChainingCount nextHopChainingCount;
        CztProtExtnCont_AS_SecurInform_ExtIEs iE_Extns;
} CztAS_SecurInform;

/* Element ----- PriorityLevel ------- */
typedef TknU32 CztPriorityLvl;

/* Element ----- Pre-emptionCapability ------- */
typedef enum enumCztPre_emptionCapblty
{
        CztPre_emptionCapbltyshall_not_trigger_pre_emptionEnum,
        CztPre_emptionCapbltymay_trigger_pre_emptionEnum
} EnumCztPre_emptionCapblty;
typedef TknU32 CztPre_emptionCapblty;

/* Element ----- Pre-emptionVulnerability ------- */
typedef enum enumCztPre_emptionVulnerability
{
        CztPre_emptionVulnerabilitynot_pre_emptableEnum,
        CztPre_emptionVulnerabilitypre_emptableEnum
} EnumCztPre_emptionVulnerability;
typedef TknU32 CztPre_emptionVulnerability;

typedef struct x2ap_ExtnAllocnAndRetentionPriority_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnAllocnAndRetentionPriority_ExtIEsCls;
typedef struct x2apProtExtnField_AllocnAndRetentionPriority_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnAllocnAndRetentionPriority_ExtIEsCls extensionValue;
} CztProtExtnField_AllocnAndRetentionPriority_ExtIEs;

typedef struct x2apProtExtnCont_AllocnAndRetentionPriority_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_AllocnAndRetentionPriority_ExtIEs *member;
}CztProtExtnCont_AllocnAndRetentionPriority_ExtIEs;

typedef struct x2apAllocnAndRetentionPriority
{
        TknPres pres;
        CztPriorityLvl priorityLvl;
        CztPre_emptionCapblty pre_emptionCapblty;
        CztPre_emptionVulnerability pre_emptionVulnerability;
        CztProtExtnCont_AllocnAndRetentionPriority_ExtIEs iE_Extns;
} CztAllocnAndRetentionPriority;

/* Element ----- BitRate ------- */
typedef TknStrOSXL CztBitRate;

/* Element ---------- PLMN-Identity ----------- */
typedef TknStrOSXL CztPLMN_Identity;
typedef struct x2apBroadcastPLMNs_Item
{
        TknU16 noComp;
        CztPLMN_Identity *member;
}CztBroadcastPLMNs_Item;

/* Element ----- CapacityValue ------- */
typedef TknU32 CztCapacityValue;

/* Element ----- CellCapacityClassValue ------- */
typedef TknU32 CztCellCapacityClassValue;

/* Element ----- CauseRadioNetwork ------- */
typedef enum enumCztCauseRadioNw
{
        CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum,
        CztCauseRadioNwtime_critical_handoverEnum,
        CztCauseRadioNwresource_optimisation_handoverEnum,
        CztCauseRadioNwreduce_load_in_serving_cellEnum,
        CztCauseRadioNwpartial_handoverEnum,
        CztCauseRadioNwunknown_new_eNB_UE_X2AP_IDEnum,
        CztCauseRadioNwunknown_old_eNB_UE_X2AP_IDEnum,
        CztCauseRadioNwunknown_pair_of_UE_X2AP_IDEnum,
        CztCauseRadioNwho_target_not_allowedEnum,
        CztCauseRadioNwtx2relocoverall_expiryEnum,
        CztCauseRadioNwtrelocprep_expiryEnum,
        CztCauseRadioNwcell_not_availableEnum,
        CztCauseRadioNwno_radio_resources_available_in_target_cellEnum,
        CztCauseRadioNwinvalid_MME_GroupIDEnum,
        CztCauseRadioNwunknown_MME_CodeEnum,
        CztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum,
        CztCauseRadioNwreportCharacteristicsEmptyEnum,
        CztCauseRadioNwnoReportPeriodicityEnum,
        CztCauseRadioNwexistingMeasurementIDEnum,
        CztCauseRadioNwunknown_eNB_Measurement_IDEnum,
        CztCauseRadioNwmeasurement_temporarily_not_availableEnum,
        CztCauseRadioNwunspecifiedEnum,
        CztCauseRadioNwload_balancingEnum,
        CztCauseRadioNwhandover_optimisationEnum,
        CztCauseRadioNwvalue_out_of_allowed_rangeEnum,
        CztCauseRadioNwmultiple_E_RAB_ID_instancesEnum,
        CztCauseRadioNwswitch_off_ongoingEnum,
        CztCauseRadioNwnot_supported_QCI_valueEnum
} EnumCztCauseRadioNw;
typedef TknU32 CztCauseRadioNw;

/* Element ----- CauseTransport ------- */
typedef enum enumCztCauseTport
{
        CztCauseTporttransport_resource_unavailableEnum,
        CztCauseTportunspecifiedEnum
} EnumCztCauseTport;
typedef TknU32 CztCauseTport;

/* Element ----- CauseProtocol ------- */
typedef enum enumCztCauseProt
{
        CztCauseProttransfer_syntax_errorEnum,
        CztCauseProtabstract_syntax_error_rejectEnum,
        CztCauseProtabstract_syntax_error_ignore_and_notifyEnum,
        CztCauseProtmessage_not_compatible_with_receiver_stateEnum,
        CztCauseProtsemantic_errorEnum,
        CztCauseProtunspecifiedEnum,
        CztCauseProtabstract_syntax_error_falsely_constructed_messageEnum
} EnumCztCauseProt;
typedef TknU32 CztCauseProt;

/* Element ----- CauseMisc ------- */
typedef enum enumCztCauseMisc
{
        CztCauseMisccontrol_processing_overloadEnum,
        CztCauseMischardware_failureEnum,
        CztCauseMiscom_interventionEnum,
        CztCauseMiscnot_enough_user_plane_processing_resourcesEnum,
        CztCauseMiscunspecifiedEnum
} EnumCztCauseMisc;
typedef TknU32 CztCauseMisc;

typedef struct x2apCause
{
        TknU8 choice;
        union {
                CztCauseRadioNw radioNw;
                CztCauseTport transport;
                CztCauseProt protocol;
                CztCauseMisc misc;
        } val;
} CztCause;

/* Element ----- Cell-Size ------- */
typedef enum enumCztCell_Size
{
        CztCell_SizeverysmallEnum,
        CztCell_SizesmallEnum,
        CztCell_SizemediumEnum,
        CztCell_SizelargeEnum
} EnumCztCell_Size;
typedef TknU32 CztCell_Size;

typedef struct x2ap_ExtnCellTyp_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCellTyp_ExtIEsCls;
typedef struct x2apProtExtnField_CellTyp_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCellTyp_ExtIEsCls extensionValue;
} CztProtExtnField_CellTyp_ExtIEs;

typedef struct x2apProtExtnCont_CellTyp_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CellTyp_ExtIEs *member;
}CztProtExtnCont_CellTyp_ExtIEs;

typedef struct x2apCellTyp
{
        TknPres pres;
        CztCell_Size cell_Size;
        CztProtExtnCont_CellTyp_ExtIEs iE_Extns;
} CztCellTyp;

typedef struct x2ap_ExtnCompositeAvailableCapacity_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCompositeAvailableCapacity_ExtIEsCls;
typedef struct x2apProtExtnField_CompositeAvailableCapacity_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCompositeAvailableCapacity_ExtIEsCls extensionValue;
} CztProtExtnField_CompositeAvailableCapacity_ExtIEs;

typedef struct x2apProtExtnCont_CompositeAvailableCapacity_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CompositeAvailableCapacity_ExtIEs *member;
}CztProtExtnCont_CompositeAvailableCapacity_ExtIEs;

typedef struct x2apCompositeAvailableCapacity
{
        TknPres pres;
        CztCellCapacityClassValue cellCapacityClassValue;
        CztCapacityValue capacityValue;
        CztProtExtnCont_CompositeAvailableCapacity_ExtIEs iE_Extns;
} CztCompositeAvailableCapacity;

typedef struct x2ap_ExtnCompositeAvailableCapacityGroup_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCompositeAvailableCapacityGroup_ExtIEsCls;
typedef struct x2apProtExtnField_CompositeAvailableCapacityGroup_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCompositeAvailableCapacityGroup_ExtIEsCls extensionValue;
} CztProtExtnField_CompositeAvailableCapacityGroup_ExtIEs;

typedef struct x2apProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CompositeAvailableCapacityGroup_ExtIEs *member;
}CztProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs;

typedef struct x2apCompositeAvailableCapacityGroup
{
        TknPres pres;
        CztCompositeAvailableCapacity dL_CompositeAvailableCapacity;
        CztCompositeAvailableCapacity uL_CompositeAvailableCapacity;
        CztProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs iE_Extns;
} CztCompositeAvailableCapacityGroup;

/* Element ----- PDCP-SN ------- */
typedef TknU32 CztPDCP_SN;

/* Element ----- HFN ------- */
typedef TknU32 CztHFN;

typedef struct x2ap_ExtnCOUNTvalue_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCOUNTvalue_ExtIEsCls;
typedef struct x2apProtExtnField_COUNTvalue_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCOUNTvalue_ExtIEsCls extensionValue;
} CztProtExtnField_COUNTvalue_ExtIEs;

typedef struct x2apProtExtnCont_COUNTvalue_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_COUNTvalue_ExtIEs *member;
}CztProtExtnCont_COUNTvalue_ExtIEs;

typedef struct x2apCOUNTvalue
{
        TknPres pres;
        CztPDCP_SN pDCP_SN;
        CztHFN hFN;
        CztProtExtnCont_COUNTvalue_ExtIEs iE_Extns;
} CztCOUNTvalue;

/* Element ----- TypeOfError ------- */
typedef enum enumCztTypOfErr
{
        CztTypOfErrnot_understoodEnum,
        CztTypOfErrmissingEnum
} EnumCztTypOfErr;
typedef TknU32 CztTypOfErr;

typedef struct x2ap_ExtnCriticalityDiag_IE_Lst_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCriticalityDiag_IE_Lst_ExtIEsCls;
typedef struct x2apProtExtnField_CriticalityDiag_IE_Lst_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCriticalityDiag_IE_Lst_ExtIEsCls extensionValue;
} CztProtExtnField_CriticalityDiag_IE_Lst_ExtIEs;

typedef struct x2apProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CriticalityDiag_IE_Lst_ExtIEs *member;
}CztProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs;

typedef struct x2apCriticalityDiag_IE_LstMember
{
        TknPres pres;
        CztCriticality iECriticality;
        CztProtIE_ID iE_ID;
        CztTypOfErr typeOfErr;
        CztProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs iE_Extns;
} CztCriticalityDiag_IE_LstMember;

typedef struct x2apCriticalityDiag_IE_Lst
{
        TknU16 noComp;
        CztCriticalityDiag_IE_LstMember *member;
}CztCriticalityDiag_IE_Lst;

typedef struct x2ap_ExtnCriticalityDiag_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCriticalityDiag_ExtIEsCls;
typedef struct x2apProtExtnField_CriticalityDiag_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCriticalityDiag_ExtIEsCls extensionValue;
} CztProtExtnField_CriticalityDiag_ExtIEs;

typedef struct x2apProtExtnCont_CriticalityDiag_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CriticalityDiag_ExtIEs *member;
}CztProtExtnCont_CriticalityDiag_ExtIEs;

typedef struct x2apCriticalityDiag
{
        TknPres pres;
        CztProcedureCode procedureCode;
        CztTrgMsg triggeringMsg;
        CztCriticality procedureCriticality;
        CztCriticalityDiag_IE_Lst iEsCriticalityDiag;
        CztProtExtnCont_CriticalityDiag_ExtIEs iE_Extns;
} CztCriticalityDiag;

/* Element ---------- CRNTI ----------- */
typedef TknBStr32 CztCRNTI;
/* Element ----- CyclicPrefixDL ------- */
typedef enum enumCztCyclicPrefixDL
{
        CztCyclicPrefixDLnormalEnum,
        CztCyclicPrefixDLextendedEnum
} EnumCztCyclicPrefixDL;
typedef TknU32 CztCyclicPrefixDL;

/* Element ----- CyclicPrefixUL ------- */
typedef enum enumCztCyclicPrefixUL
{
        CztCyclicPrefixULnormalEnum,
        CztCyclicPrefixULextendedEnum
} EnumCztCyclicPrefixUL;
typedef TknU32 CztCyclicPrefixUL;

/* Element ----- DeactivationIndication ------- */
typedef enum enumCztDeactivationInd
{
        CztDeactivationInddeactivatedEnum
} EnumCztDeactivationInd;
typedef TknU32 CztDeactivationInd;

/* Element ----- DL-Forwarding ------- */
typedef enum enumCztDL_Fwding
{
        CztDL_FwdingdL_forwardingProposedEnum
} EnumCztDL_Fwding;
typedef TknU32 CztDL_Fwding;

/* Element ----- EARFCN ------- */
typedef TknU32 CztEARFCN;

/* Element ----- Transmission-Bandwidth ------- */
typedef enum enumCztTransmission_Bandwidth
{
        CztTransmission_Bandwidthbw6Enum,
        CztTransmission_Bandwidthbw15Enum,
        CztTransmission_Bandwidthbw25Enum,
        CztTransmission_Bandwidthbw50Enum,
        CztTransmission_Bandwidthbw75Enum,
        CztTransmission_Bandwidthbw100Enum
} EnumCztTransmission_Bandwidth;
typedef TknU32 CztTransmission_Bandwidth;

typedef struct x2ap_ExtnFDD_Info_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnFDD_Info_ExtIEsCls;
typedef struct x2apProtExtnField_FDD_Info_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnFDD_Info_ExtIEsCls extensionValue;
} CztProtExtnField_FDD_Info_ExtIEs;

typedef struct x2apProtExtnCont_FDD_Info_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_FDD_Info_ExtIEs *member;
}CztProtExtnCont_FDD_Info_ExtIEs;

typedef struct x2apFDD_Info
{
        TknPres pres;
        CztEARFCN uL_EARFCN;
        CztEARFCN dL_EARFCN;
        CztTransmission_Bandwidth uL_Transmission_Bandwidth;
        CztTransmission_Bandwidth dL_Transmission_Bandwidth;
        CztProtExtnCont_FDD_Info_ExtIEs iE_Extns;
} CztFDD_Info;

/* Element ----- SubframeAssignment ------- */
typedef enum enumCztSubframeAssignment
{
        CztSubframeAssignmentsa0Enum,
        CztSubframeAssignmentsa1Enum,
        CztSubframeAssignmentsa2Enum,
        CztSubframeAssignmentsa3Enum,
        CztSubframeAssignmentsa4Enum,
        CztSubframeAssignmentsa5Enum,
        CztSubframeAssignmentsa6Enum
} EnumCztSubframeAssignment;
typedef TknU32 CztSubframeAssignment;

/* Element ----- SpecialSubframePatterns ------- */
typedef enum enumCztSpecialSubframePatterns
{
        CztSpecialSubframePatternsssp0Enum,
        CztSpecialSubframePatternsssp1Enum,
        CztSpecialSubframePatternsssp2Enum,
        CztSpecialSubframePatternsssp3Enum,
        CztSpecialSubframePatternsssp4Enum,
        CztSpecialSubframePatternsssp5Enum,
        CztSpecialSubframePatternsssp6Enum,
        CztSpecialSubframePatternsssp7Enum,
        CztSpecialSubframePatternsssp8Enum
} EnumCztSpecialSubframePatterns;
typedef TknU32 CztSpecialSubframePatterns;

typedef struct x2ap_ExtnSpecialSubframe_Info_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnSpecialSubframe_Info_ExtIEsCls;
typedef struct x2apProtExtnField_SpecialSubframe_Info_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnSpecialSubframe_Info_ExtIEsCls extensionValue;
} CztProtExtnField_SpecialSubframe_Info_ExtIEs;

typedef struct x2apProtExtnCont_SpecialSubframe_Info_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_SpecialSubframe_Info_ExtIEs *member;
}CztProtExtnCont_SpecialSubframe_Info_ExtIEs;

typedef struct x2apSpecialSubframe_Info
{
        TknPres pres;
        CztSpecialSubframePatterns specialSubframePatterns;
        CztCyclicPrefixDL cyclicPrefixDL;
        CztCyclicPrefixUL cyclicPrefixUL;
        CztProtExtnCont_SpecialSubframe_Info_ExtIEs iE_Extns;
} CztSpecialSubframe_Info;

typedef struct x2ap_ExtnTDD_Info_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnTDD_Info_ExtIEsCls;
typedef struct x2apProtExtnField_TDD_Info_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnTDD_Info_ExtIEsCls extensionValue;
} CztProtExtnField_TDD_Info_ExtIEs;

typedef struct x2apProtExtnCont_TDD_Info_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_TDD_Info_ExtIEs *member;
}CztProtExtnCont_TDD_Info_ExtIEs;

typedef struct x2apTDD_Info
{
        TknPres pres;
        CztEARFCN eARFCN;
        CztTransmission_Bandwidth transmission_Bandwidth;
        CztSubframeAssignment subframeAssignment;
        CztSpecialSubframe_Info specialSubframe_Info;
        CztProtExtnCont_TDD_Info_ExtIEs iE_Extns;
} CztTDD_Info;

typedef struct x2apEUTRA_Mode_Info
{
        TknU8 choice;
        union {
                CztFDD_Info fDD;
                CztTDD_Info tDD;
        } val;
} CztEUTRA_Mode_Info;

/* Element ---------- EUTRANCellIdentifier ----------- */
typedef TknBStr32 CztEUTRANCellIdentifier;
typedef struct x2ap_ExtnECGI_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnECGI_ExtIEsCls;
typedef struct x2apProtExtnField_ECGI_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnECGI_ExtIEsCls extensionValue;
} CztProtExtnField_ECGI_ExtIEs;

typedef struct x2apProtExtnCont_ECGI_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_ECGI_ExtIEs *member;
}CztProtExtnCont_ECGI_ExtIEs;

typedef struct x2apECGI
{
        TknPres pres;
        CztPLMN_Identity pLMN_Identity;
        CztEUTRANCellIdentifier eUTRANcellIdentifier;
        CztProtExtnCont_ECGI_ExtIEs iE_Extns;
} CztECGI;

/* Element ---------- macro-eNB-ID ----------- */
typedef TknBStr32 CztENB_IDmacro_eNB_ID;
/* Element ---------- home-eNB-ID ----------- */
typedef TknBStr32 CztENB_IDhome_eNB_ID;
typedef struct x2apENB_ID
{
        TknU8 choice;
        union {
                CztENB_IDmacro_eNB_ID macro_eNB_ID;
                CztENB_IDhome_eNB_ID home_eNB_ID;
        } val;
} CztENB_ID;

/* Element ---------- EncryptionAlgorithms ----------- */
typedef TknBStr32 CztEncryptionAlgorithms;
typedef struct x2apEPLMNs
{
        TknU16 noComp;
        CztPLMN_Identity *member;
}CztEPLMNs;

/* Element ----- E-RAB-ID ------- */
typedef TknU32 CztE_RAB_ID;

/* Element ----- QCI ------- */
typedef TknU32 CztQCI;

typedef struct x2ap_ExtnGBR_QosInform_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnGBR_QosInform_ExtIEsCls;
typedef struct x2apProtExtnField_GBR_QosInform_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnGBR_QosInform_ExtIEsCls extensionValue;
} CztProtExtnField_GBR_QosInform_ExtIEs;

typedef struct x2apProtExtnCont_GBR_QosInform_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_GBR_QosInform_ExtIEs *member;
}CztProtExtnCont_GBR_QosInform_ExtIEs;

typedef struct x2apGBR_QosInform
{
        TknPres pres;
        CztBitRate e_RAB_MaxBitrateDL;
        CztBitRate e_RAB_MaxBitrateUL;
        CztBitRate e_RAB_GuaranteedBitrateDL;
        CztBitRate e_RAB_GuaranteedBitrateUL;
        CztProtExtnCont_GBR_QosInform_ExtIEs iE_Extns;
} CztGBR_QosInform;

typedef struct x2ap_ExtnE_RAB_Lvl_QoS_Params_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnE_RAB_Lvl_QoS_Params_ExtIEsCls;
typedef struct x2apProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnE_RAB_Lvl_QoS_Params_ExtIEsCls extensionValue;
} CztProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEs;

typedef struct x2apProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEs *member;
}CztProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs;

typedef struct x2apE_RAB_Lvl_QoS_Params
{
        TknPres pres;
        CztQCI qCI;
        CztAllocnAndRetentionPriority allocationAndRetentionPriority;
        CztGBR_QosInform gbrQosInform;
        CztProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs iE_Extns;
} CztE_RAB_Lvl_QoS_Params;

typedef struct x2ap_ExtnE_RAB_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnE_RAB_Item_ExtIEsCls;
typedef struct x2apProtExtnField_E_RAB_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnE_RAB_Item_ExtIEsCls extensionValue;
} CztProtExtnField_E_RAB_Item_ExtIEs;

typedef struct x2apProtExtnCont_E_RAB_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_E_RAB_Item_ExtIEs *member;
}CztProtExtnCont_E_RAB_Item_ExtIEs;

typedef struct x2apE_RAB_Item
{
        TknPres pres;
        CztE_RAB_ID e_RAB_ID;
        CztCause cause;
        CztProtExtnCont_E_RAB_Item_ExtIEs iE_Extns;
} CztE_RAB_Item;

typedef struct x2ap_ValueE_RAB_ItemIEsCls{
        union {
                CztE_RAB_Item cztE_RAB_Item;
        } u;
} Czt_ValueE_RAB_ItemIEsCls;
typedef struct x2apProtIE_Field_E_RAB_ItemIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueE_RAB_ItemIEsCls value;
} CztProtIE_Field_E_RAB_ItemIEs;

/* Element ------------- ProtocolIE-Single-Container ------------ */
typedef CztProtIE_Field_E_RAB_ItemIEs CztProtIE_Single_Cont_E_RAB_ItemIEs;
typedef struct x2apE_RAB_Lst
{
        TknU16 noComp;
        CztProtIE_Single_Cont_E_RAB_ItemIEs *member;
}CztE_RAB_Lst;

/* Element ---------- EUTRANTraceID ----------- */
typedef TknStrOSXL CztEUTRANTraceID;
/* Element ----- EventType ------- */
typedef enum enumCztEventTyp
{
        CztEventTypchange_of_serving_cellEnum
} EnumCztEventTyp;
typedef TknU32 CztEventTyp;

/* Element ----- ForbiddenInterRATs ------- */
typedef enum enumCztForbiddenInterRATs
{
        CztForbiddenInterRATsallEnum,
        CztForbiddenInterRATsgeranEnum,
        CztForbiddenInterRATsutranEnum,
        CztForbiddenInterRATscdma2000Enum,
        CztForbiddenInterRATsgeranandutranEnum,
        CztForbiddenInterRATscdma2000andutranEnum
} EnumCztForbiddenInterRATs;
typedef TknU32 CztForbiddenInterRATs;

/* Element ---------- TAC ----------- */
typedef TknStr4 CztTAC;
typedef struct x2apForbiddenTACs
{
        TknU16 noComp;
        CztTAC *member;
}CztForbiddenTACs;

typedef struct x2ap_ExtnForbiddenTAs_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnForbiddenTAs_Item_ExtIEsCls;
typedef struct x2apProtExtnField_ForbiddenTAs_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnForbiddenTAs_Item_ExtIEsCls extensionValue;
} CztProtExtnField_ForbiddenTAs_Item_ExtIEs;

typedef struct x2apProtExtnCont_ForbiddenTAs_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_ForbiddenTAs_Item_ExtIEs *member;
}CztProtExtnCont_ForbiddenTAs_Item_ExtIEs;

typedef struct x2apForbiddenTAs_Item
{
        TknPres pres;
        CztPLMN_Identity pLMN_Identity;
        CztForbiddenTACs forbiddenTACs;
        CztProtExtnCont_ForbiddenTAs_Item_ExtIEs iE_Extns;
} CztForbiddenTAs_Item;

typedef struct x2apForbiddenTAs
{
        TknU16 noComp;
        CztForbiddenTAs_Item *member;
}CztForbiddenTAs;

/* Element ---------- LAC ----------- */
typedef TknStr4 CztLAC;
typedef struct x2apForbiddenLACs
{
        TknU16 noComp;
        CztLAC *member;
}CztForbiddenLACs;

typedef struct x2ap_ExtnForbiddenLAs_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnForbiddenLAs_Item_ExtIEsCls;
typedef struct x2apProtExtnField_ForbiddenLAs_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnForbiddenLAs_Item_ExtIEsCls extensionValue;
} CztProtExtnField_ForbiddenLAs_Item_ExtIEs;

typedef struct x2apProtExtnCont_ForbiddenLAs_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_ForbiddenLAs_Item_ExtIEs *member;
}CztProtExtnCont_ForbiddenLAs_Item_ExtIEs;

typedef struct x2apForbiddenLAs_Item
{
        TknPres pres;
        CztPLMN_Identity pLMN_Identity;
        CztForbiddenLACs forbiddenLACs;
        CztProtExtnCont_ForbiddenLAs_Item_ExtIEs iE_Extns;
} CztForbiddenLAs_Item;

typedef struct x2apForbiddenLAs
{
        TknU16 noComp;
        CztForbiddenLAs_Item *member;
}CztForbiddenLAs;

/* Element ---------- Fourframes ----------- */
typedef TknBStr32 CztFourframes;
typedef struct x2ap_ExtnGlobalENB_ID_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnGlobalENB_ID_ExtIEsCls;
typedef struct x2apProtExtnField_GlobalENB_ID_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnGlobalENB_ID_ExtIEsCls extensionValue;
} CztProtExtnField_GlobalENB_ID_ExtIEs;

typedef struct x2apProtExtnCont_GlobalENB_ID_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_GlobalENB_ID_ExtIEs *member;
}CztProtExtnCont_GlobalENB_ID_ExtIEs;

typedef struct x2apGlobalENB_ID
{
        TknPres pres;
        CztPLMN_Identity pLMN_Identity;
        CztENB_ID eNB_ID;
        CztProtExtnCont_GlobalENB_ID_ExtIEs iE_Extns;
} CztGlobalENB_ID;

/* Element ---------- TransportLayerAddress ----------- */
typedef TknStrBSXL CztTportLyrAddr;
/* Element ---------- GTP-TEI ----------- */
typedef TknStrOSXL CztGTP_TEI;
typedef struct x2ap_ExtnGTPtunnelEndpoint_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnGTPtunnelEndpoint_ExtIEsCls;
typedef struct x2apProtExtnField_GTPtunnelEndpoint_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnGTPtunnelEndpoint_ExtIEsCls extensionValue;
} CztProtExtnField_GTPtunnelEndpoint_ExtIEs;

typedef struct x2apProtExtnCont_GTPtunnelEndpoint_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_GTPtunnelEndpoint_ExtIEs *member;
}CztProtExtnCont_GTPtunnelEndpoint_ExtIEs;

typedef struct x2apGTPtunnelEndpoint
{
        TknPres pres;
        CztTportLyrAddr transportLyrAddr;
        CztGTP_TEI gTP_TEID;
        CztProtExtnCont_GTPtunnelEndpoint_ExtIEs iE_Extns;
} CztGTPtunnelEndpoint;

/* Element ---------- MME-Group-ID ----------- */
typedef TknStr4 CztMME_Group_ID;
typedef struct x2ap_ExtnGU_Group_ID_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnGU_Group_ID_ExtIEsCls;
typedef struct x2apProtExtnField_GU_Group_ID_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnGU_Group_ID_ExtIEsCls extensionValue;
} CztProtExtnField_GU_Group_ID_ExtIEs;

typedef struct x2apProtExtnCont_GU_Group_ID_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_GU_Group_ID_ExtIEs *member;
}CztProtExtnCont_GU_Group_ID_ExtIEs;

typedef struct x2apGU_Group_ID
{
        TknPres pres;
        CztPLMN_Identity pLMN_Identity;
        CztMME_Group_ID mME_Group_ID;
        CztProtExtnCont_GU_Group_ID_ExtIEs iE_Extns;
} CztGU_Group_ID;

typedef struct x2apGUGroupIDLst
{
        TknU16 noComp;
        CztGU_Group_ID *member;
}CztGUGroupIDLst;

/* Element ---------- MME-Code ----------- */
typedef TknStr4 CztMME_Code;
typedef struct x2ap_ExtnGUMMEI_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnGUMMEI_ExtIEsCls;
typedef struct x2apProtExtnField_GUMMEI_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnGUMMEI_ExtIEsCls extensionValue;
} CztProtExtnField_GUMMEI_ExtIEs;

typedef struct x2apProtExtnCont_GUMMEI_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_GUMMEI_ExtIEs *member;
}CztProtExtnCont_GUMMEI_ExtIEs;

typedef struct x2apGUMMEI
{
        TknPres pres;
        CztGU_Group_ID gU_Group_ID;
        CztMME_Code mMME_Code; /* czt_asn_x_001.main_1 :  X2AP 3.1 */
        CztProtExtnCont_GUMMEI_ExtIEs iE_Extns;
} CztGUMMEI;

/* Element ----- HandoverReportType ------- */
typedef enum enumCztHovrReportTyp
{
        CztHovrReportTyphoTooEarlyEnum,
        CztHovrReportTyphoToWrongCellEnum
} EnumCztHovrReportTyp;
typedef TknU32 CztHovrReportTyp;

typedef struct x2ap_ExtnHovrRestrnLst_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnHovrRestrnLst_ExtIEsCls;
typedef struct x2apProtExtnField_HovrRestrnLst_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnHovrRestrnLst_ExtIEsCls extensionValue;
} CztProtExtnField_HovrRestrnLst_ExtIEs;

typedef struct x2apProtExtnCont_HovrRestrnLst_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_HovrRestrnLst_ExtIEs *member;
}CztProtExtnCont_HovrRestrnLst_ExtIEs;

typedef struct x2apHovrRestrnLst
{
        TknPres pres;
        CztPLMN_Identity servingPLMN;
        CztEPLMNs equivalentPLMNs;
        CztForbiddenTAs forbiddenTAs;
        CztForbiddenLAs forbiddenLAs;
        CztForbiddenInterRATs forbiddenInterRATs;
        CztProtExtnCont_HovrRestrnLst_ExtIEs iE_Extns;
} CztHovrRestrnLst;

/* Element ----- LoadIndicator ------- */
typedef enum enumCztLoadIndicator
{
        CztLoadIndicatorlowLoadEnum,
        CztLoadIndicatormediumLoadEnum,
        CztLoadIndicatorhighLoadEnum,
        CztLoadIndicatoroverLoadEnum
} EnumCztLoadIndicator;
typedef TknU32 CztLoadIndicator;

typedef struct x2ap_ExtnHWLoadIndicator_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnHWLoadIndicator_ExtIEsCls;
typedef struct x2apProtExtnField_HWLoadIndicator_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnHWLoadIndicator_ExtIEsCls extensionValue;
} CztProtExtnField_HWLoadIndicator_ExtIEs;

typedef struct x2apProtExtnCont_HWLoadIndicator_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_HWLoadIndicator_ExtIEs *member;
}CztProtExtnCont_HWLoadIndicator_ExtIEs;

typedef struct x2apHWLoadIndicator
{
        TknPres pres;
        CztLoadIndicator dLHWLoadIndicator;
        CztLoadIndicator uLHWLoadIndicator;
        CztProtExtnCont_HWLoadIndicator_ExtIEs iE_Extns;
} CztHWLoadIndicator;

/* Element ---------- IntegrityProtectionAlgorithms ----------- */
typedef TknBStr32 CztIntegrityProtectionAlgorithms;
/* Element ---------- InterfacesToTrace ----------- */
typedef TknBStr32 CztIntfsToTrace;
/* Element ----- Time-UE-StayedInCell ------- */
typedef TknU32 CztTime_UE_StayedInCell;

typedef struct x2ap_ExtnLastVisitedEUTRANCellInform_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnLastVisitedEUTRANCellInform_ExtIEsCls;
typedef struct x2apProtExtnField_LastVisitedEUTRANCellInform_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnLastVisitedEUTRANCellInform_ExtIEsCls extensionValue;
} CztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs;

typedef struct x2apProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs *member;
}CztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs;

typedef struct x2apLastVisitedEUTRANCellInform
{
        TknPres pres;
        CztECGI global_Cell_ID;
        CztCellTyp cellTyp;
        CztTime_UE_StayedInCell time_UE_StayedInCell;
        CztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs iE_Extns;
} CztLastVisitedEUTRANCellInform;

/* Element ---------- LastVisitedUTRANCellInformation ----------- */
typedef TknStrOSXL CztLastVisitedUTRANCellInform;
typedef struct x2apLastVisitedGERANCellInform
{
        TknU8 choice;
/* czt_asn_x_001.main_1 :  X2AP 3.1 */
} CztLastVisitedGERANCellInform;

typedef struct x2apLastVisitedCell_Item
{
        TknU8 choice;
        union {
                CztLastVisitedEUTRANCellInform e_UTRAN_Cell;
                CztLastVisitedUTRANCellInform uTRAN_Cell;
                CztLastVisitedGERANCellInform gERAN_Cell;
        } val;
} CztLastVisitedCell_Item;

/* Element ----- ReportArea ------- */
typedef enum enumCztReportArea
{
        CztReportAreaecgiEnum
} EnumCztReportArea;
typedef TknU32 CztReportArea;

typedef struct x2ap_ExtnLocRprtngInform_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnLocRprtngInform_ExtIEsCls;
typedef struct x2apProtExtnField_LocRprtngInform_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnLocRprtngInform_ExtIEsCls extensionValue;
} CztProtExtnField_LocRprtngInform_ExtIEs;

typedef struct x2apProtExtnCont_LocRprtngInform_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_LocRprtngInform_ExtIEs *member;
}CztProtExtnCont_LocRprtngInform_ExtIEs;

typedef struct x2apLocRprtngInform
{
        TknPres pres;
        CztEventTyp eventTyp;
        CztReportArea reportArea;
        CztProtExtnCont_LocRprtngInform_ExtIEs iE_Extns;
} CztLocRprtngInform;

/* Element ----- Measurement-ID ------- */
typedef TknU32 CztMeasurement_ID;

/* Element ----- RadioframeAllocationPeriod ------- */
typedef enum enumCztRadioframeAllocnPeriod
{
        CztRadioframeAllocnPeriodn1Enum,
        CztRadioframeAllocnPeriodn2Enum,
        CztRadioframeAllocnPeriodn4Enum,
        CztRadioframeAllocnPeriodn8Enum,
        CztRadioframeAllocnPeriodn16Enum,
        CztRadioframeAllocnPeriodn32Enum
} EnumCztRadioframeAllocnPeriod;
typedef TknU32 CztRadioframeAllocnPeriod;

/* Element ----- RadioframeAllocationOffset ------- */
typedef TknU32 CztRadioframeAllocnOffset;

/* Element ---------- Oneframe ----------- */
typedef TknBStr32 CztOneframe;
typedef struct x2apSubframeAllocn
{
        TknU8 choice;
        union {
                CztOneframe oneframe;
                CztFourframes fourframes;
        } val;
} CztSubframeAllocn;

typedef struct x2ap_ExtnMBSFN_Subframe_Info_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnMBSFN_Subframe_Info_ExtIEsCls;
typedef struct x2apProtExtnField_MBSFN_Subframe_Info_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnMBSFN_Subframe_Info_ExtIEsCls extensionValue;
} CztProtExtnField_MBSFN_Subframe_Info_ExtIEs;

typedef struct x2apProtExtnCont_MBSFN_Subframe_Info_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_MBSFN_Subframe_Info_ExtIEs *member;
}CztProtExtnCont_MBSFN_Subframe_Info_ExtIEs;

typedef struct x2apMBSFN_Subframe_Info
{
        TknPres pres;
        CztRadioframeAllocnPeriod radioframeAllocnPeriod;
        CztRadioframeAllocnOffset radioframeAllocnOffset;
        CztSubframeAllocn subframeAllocn;
        CztProtExtnCont_MBSFN_Subframe_Info_ExtIEs iE_Extns;
} CztMBSFN_Subframe_Info;

typedef struct x2apMBSFN_Subframe_Infolist
{
        TknU16 noComp;
        CztMBSFN_Subframe_Info *member;
}CztMBSFN_Subframe_Infolist;

/* Element ----- handoverTriggerChangeLowerLimit ------- */
typedef TknS32 CztMobilityParamsModificationRangehandoverTriggerChangeLowerLimit;

/* Element ----- handoverTriggerChangeUpperLimit ------- */
typedef TknS32 CztMobilityParamsModificationRangehandoverTriggerChangeUpperLimit;

typedef struct x2apMobilityParamsModificationRange
{
        TknPres pres;
        CztMobilityParamsModificationRangehandoverTriggerChangeLowerLimit handoverTriggerChangeLowerLimit;
        CztMobilityParamsModificationRangehandoverTriggerChangeUpperLimit handoverTriggerChangeUpperLimit;
} CztMobilityParamsModificationRange;

/* Element ----- handoverTriggerChange ------- */
typedef TknU32 CztMobilityParamsInformhandoverTriggerChange; /* X2AP 3.1 */

typedef struct x2apMobilityParamsInform
{
        TknPres pres;
        CztMobilityParamsInformhandoverTriggerChange handoverTriggerChange;
} CztMobilityParamsInform;

/* Element ----- PCI ------- */
typedef TknU32 CztPCI;

typedef struct x2ap_ExtnNeighbour_Inform_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnNeighbour_Inform_ExtIEsCls;
typedef struct x2apProtExtnField_Neighbour_Inform_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnNeighbour_Inform_ExtIEsCls extensionValue;
} CztProtExtnField_Neighbour_Inform_ExtIEs;

typedef struct x2apProtExtnCont_Neighbour_Inform_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_Neighbour_Inform_ExtIEs *member;
}CztProtExtnCont_Neighbour_Inform_ExtIEs;

typedef struct x2apNeighbour_InformMember
{
        TknPres pres;
        CztECGI eCGI;
        CztPCI pCI;
        CztEARFCN eARFCN;
        CztProtExtnCont_Neighbour_Inform_ExtIEs iE_Extns;
} CztNeighbour_InformMember;

typedef struct x2apNeighbour_Inform
{
        TknU16 noComp;
        CztNeighbour_InformMember *member;
}CztNeighbour_Inform;

/* Element ----- Number-of-Antennaports ------- */
typedef enum enumCztNumber_of_Antennaports
{
        CztNumber_of_Antennaportsan1Enum,
        CztNumber_of_Antennaportsan2Enum,
        CztNumber_of_Antennaportsan4Enum
} EnumCztNumber_of_Antennaports;
typedef TknU32 CztNumber_of_Antennaports;

/* Element ----- rootSequenceIndex ------- */
typedef TknU32 CztPRACH_ConfigrootSequenceIdx;

/* Element ----- zeroCorrelationIndex ------- */
typedef TknU32 CztPRACH_ConfigzeroCorrelationIdx;

/* Element ----- highSpeedFlag ------- */
typedef TknU8 CztPRACH_ConfighighSpeedFlag;

/* Element ----- prach-FreqOffset ------- */
typedef TknU32 CztPRACH_Configprach_FreqOffset;

/* Element ----- prach-ConfigIndex ------- */
typedef TknU32 CztPRACH_Configprach_ConfigIdx;

typedef struct x2ap_ExtnPRACH_Config_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnPRACH_Config_ExtIEsCls;
typedef struct x2apProtExtnField_PRACH_Config_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnPRACH_Config_ExtIEsCls extensionValue;
} CztProtExtnField_PRACH_Config_ExtIEs;

typedef struct x2apProtExtnCont_PRACH_Config_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_PRACH_Config_ExtIEs *member;
}CztProtExtnCont_PRACH_Config_ExtIEs;

typedef struct x2apPRACH_Config
{
        TknPres pres;
        CztPRACH_ConfigrootSequenceIdx rootSequenceIdx;
        CztPRACH_ConfigzeroCorrelationIdx zeroCorrelationIdx;
        CztPRACH_ConfighighSpeedFlag highSpeedFlag;
        CztPRACH_Configprach_FreqOffset prach_FreqOffset;
        CztPRACH_Configprach_ConfigIdx prach_ConfigIdx;
        CztProtExtnCont_PRACH_Config_ExtIEs iE_Extns;
} CztPRACH_Config;

/* Element ---------- ReceiveStatusofULPDCPSDUs ----------- */
typedef TknStrBSXL CztReceiveStatusofULPDCPSDUs;
/* Element ----- Registration-Request ------- */
typedef enum enumCztRegistration_Rqst
{
        CztRegistration_RqststartEnum,
        CztRegistration_RqststopEnum
} EnumCztRegistration_Rqst;
typedef TknU32 CztRegistration_Rqst;

/* Element ----- RNTP-Threshold ------- */
typedef enum enumCztRNTP_Threshold
{
        CztRNTP_ThresholdminusInfinityEnum,
        CztRNTP_ThresholdminusElevenEnum,
        CztRNTP_ThresholdminusTenEnum,
        CztRNTP_ThresholdminusNineEnum,
        CztRNTP_ThresholdminusEightEnum,
        CztRNTP_ThresholdminusSevenEnum,
        CztRNTP_ThresholdminusSixEnum,
        CztRNTP_ThresholdminusFiveEnum,
        CztRNTP_ThresholdminusFourEnum,
        CztRNTP_ThresholdminusThreeEnum,
        CztRNTP_ThresholdminusTwoEnum,
        CztRNTP_ThresholdminusOneEnum,
        CztRNTP_ThresholdzeroEnum,
        CztRNTP_ThresholdoneEnum,
        CztRNTP_ThresholdtwoEnum,
        CztRNTP_ThresholdthreeEnum
} EnumCztRNTP_Threshold;
typedef TknU32 CztRNTP_Threshold;

/* Element ---------- rNTP-PerPRB ----------- */
typedef TknStrBSXL CztRelativeNarrowbandTxPowerrNTP_PerPRB;
/* Element ----- numberOfCellSpecificAntennaPorts ------- */
typedef enum enumCztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts
{
        CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsoneEnum,
        CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortstwoEnum,
        CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsfourEnum
} EnumCztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts;
typedef TknU32 CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts;

/* Element ----- p-B ------- */
typedef TknU32 CztRelativeNarrowbandTxPowerp_B;

/* Element ----- pDCCH-InterferenceImpact ------- */
typedef TknU32 CztRelativeNarrowbandTxPowerpDCCH_InterferenceImpact;

typedef struct x2ap_ExtnRelativeNarrowbandTxPower_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnRelativeNarrowbandTxPower_ExtIEsCls;
typedef struct x2apProtExtnField_RelativeNarrowbandTxPower_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnRelativeNarrowbandTxPower_ExtIEsCls extensionValue;
} CztProtExtnField_RelativeNarrowbandTxPower_ExtIEs;

typedef struct x2apProtExtnCont_RelativeNarrowbandTxPower_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_RelativeNarrowbandTxPower_ExtIEs *member;
}CztProtExtnCont_RelativeNarrowbandTxPower_ExtIEs;

typedef struct x2apRelativeNarrowbandTxPower
{
        TknPres pres;
        CztRelativeNarrowbandTxPowerrNTP_PerPRB rNTP_PerPRB;
        CztRNTP_Threshold rNTP_Threshold;
        CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts numberOfCellSpecificAntennaPorts;
        CztRelativeNarrowbandTxPowerp_B p_B;
        CztRelativeNarrowbandTxPowerpDCCH_InterferenceImpact pDCCH_InterferenceImpact;
        CztProtExtnCont_RelativeNarrowbandTxPower_ExtIEs iE_Extns;
} CztRelativeNarrowbandTxPower;

/* Element ---------- ReportCharacteristics ----------- */
typedef TknBStr32 CztReportCharacteristics;
/* Element ---------- RRC-Context ----------- */
typedef TknStrOSXL CztRRC_Cntxt;
/* Element ----- DL-GBR-PRB-usage ------- */
typedef TknU32 CztDL_GBR_PRB_usage;

/* Element ----- UL-GBR-PRB-usage ------- */
typedef TknU32 CztUL_GBR_PRB_usage;

/* Element ----- DL-non-GBR-PRB-usage ------- */
typedef TknU32 CztDL_non_GBR_PRB_usage;

/* Element ----- UL-non-GBR-PRB-usage ------- */
typedef TknU32 CztUL_non_GBR_PRB_usage;

/* Element ----- DL-Total-PRB-usage ------- */
typedef TknU32 CztDL_Total_PRB_usage;

/* Element ----- UL-Total-PRB-usage ------- */
typedef TknU32 CztUL_Total_PRB_usage;

typedef struct x2ap_ExtnRadioResStatus_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnRadioResStatus_ExtIEsCls;
typedef struct x2apProtExtnField_RadioResStatus_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnRadioResStatus_ExtIEsCls extensionValue;
} CztProtExtnField_RadioResStatus_ExtIEs;

typedef struct x2apProtExtnCont_RadioResStatus_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_RadioResStatus_ExtIEs *member;
}CztProtExtnCont_RadioResStatus_ExtIEs;

typedef struct x2apRadioResStatus
{
        TknPres pres;
        CztDL_GBR_PRB_usage dL_GBR_PRB_usage;
        CztUL_GBR_PRB_usage uL_GBR_PRB_usage;
        CztDL_non_GBR_PRB_usage dL_non_GBR_PRB_usage;
        CztUL_non_GBR_PRB_usage uL_non_GBR_PRB_usage;
        CztDL_Total_PRB_usage dL_Total_PRB_usage;
        CztUL_Total_PRB_usage uL_Total_PRB_usage;
        CztProtExtnCont_RadioResStatus_ExtIEs iE_Extns;
} CztRadioResStatus;

typedef struct x2ap_ExtnS1TNLLoadIndicator_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnS1TNLLoadIndicator_ExtIEsCls;
typedef struct x2apProtExtnField_S1TNLLoadIndicator_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnS1TNLLoadIndicator_ExtIEsCls extensionValue;
} CztProtExtnField_S1TNLLoadIndicator_ExtIEs;

typedef struct x2apProtExtnCont_S1TNLLoadIndicator_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_S1TNLLoadIndicator_ExtIEs *member;
}CztProtExtnCont_S1TNLLoadIndicator_ExtIEs;

typedef struct x2apS1TNLLoadIndicator
{
        TknPres pres;
        CztLoadIndicator dLS1TNLLoadIndicator;
        CztLoadIndicator uLS1TNLLoadIndicator;
        CztProtExtnCont_S1TNLLoadIndicator_ExtIEs iE_Extns;
} CztS1TNLLoadIndicator;

typedef struct x2ap_ExtnSrvdCell_Inform_ExtIEsCls{
        union {
                CztNumber_of_Antennaports cztNumber_of_Antennaports;
                CztPRACH_Config cztPRACH_Config;
                CztMBSFN_Subframe_Infolist cztMBSFN_Subframe_Infolist;
        } u;
} Czt_ExtnSrvdCell_Inform_ExtIEsCls;
typedef struct x2apProtExtnField_SrvdCell_Inform_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnSrvdCell_Inform_ExtIEsCls extensionValue;
} CztProtExtnField_SrvdCell_Inform_ExtIEs;

typedef struct x2apProtExtnCont_SrvdCell_Inform_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_SrvdCell_Inform_ExtIEs *member;
}CztProtExtnCont_SrvdCell_Inform_ExtIEs;

typedef struct x2apSrvdCell_Inform
{
        TknPres pres;
        CztPCI pCI;
        CztECGI cellId;
        CztTAC tAC;
        CztBroadcastPLMNs_Item broadcastPLMNs;
        CztEUTRA_Mode_Info eUTRA_Mode_Info;
        CztProtExtnCont_SrvdCell_Inform_ExtIEs iE_Extns;
} CztSrvdCell_Inform;

typedef struct x2ap_ExtnSrvdCell_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnSrvdCell_ExtIEsCls;
typedef struct x2apProtExtnField_SrvdCell_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnSrvdCell_ExtIEsCls extensionValue;
} CztProtExtnField_SrvdCell_ExtIEs;

typedef struct x2apProtExtnCont_SrvdCell_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_SrvdCell_ExtIEs *member;
}CztProtExtnCont_SrvdCell_ExtIEs;

typedef struct x2apSrvdCellsMember
{
        TknPres pres;
        CztSrvdCell_Inform servedCellInfo;
        CztNeighbour_Inform neighbour_Info;
        CztProtExtnCont_SrvdCell_ExtIEs iE_Extns;
} CztSrvdCellsMember;

typedef struct x2apSrvdCells
{
        TknU16 noComp;
        CztSrvdCellsMember *member;
}CztSrvdCells;

/* Element ---------- ShortMAC-I ----------- */
typedef TknBStr32 CztShortMAC_I;
/* Element ----- SRVCCOperationPossible ------- */
typedef enum enumCztSRVCCOperationPossible
{
        CztSRVCCOperationPossiblepossibleEnum
} EnumCztSRVCCOperationPossible;
typedef TknU32 CztSRVCCOperationPossible;

/* Element ----- SubscriberProfileIDforRFP ------- */
typedef TknU32 CztSubscriberProfileIDforRFP;

/* Element ---------- TargeteNBtoSource-eNBTransparentContainer ----------- */
typedef TknStrOSXL CztTgeteNBtoSrc_eNBTprntCont;
/* Element ----- TimeToWait ------- */
typedef enum enumCztTimeToWait
{
        CztTimeToWaitv1sEnum,
        CztTimeToWaitv2sEnum,
        CztTimeToWaitv5sEnum,
        CztTimeToWaitv10sEnum,
        CztTimeToWaitv20sEnum,
        CztTimeToWaitv60sEnum
} EnumCztTimeToWait;
typedef TknU32 CztTimeToWait;

/* Element ----- TraceDepth ------- */
typedef enum enumCztTraceDepth
{
        CztTraceDepthminimumEnum,
        CztTraceDepthmediumEnum,
        CztTraceDepthmaximumEnum,
        CztTraceDepthminimumWithoutVendorSpecificExtnEnum,
        CztTraceDepthmediumWithoutVendorSpecificExtnEnum,
        CztTraceDepthmaximumWithoutVendorSpecificExtnEnum
} EnumCztTraceDepth;
typedef TknU32 CztTraceDepth;

/* Element ---------- TraceCollectionEntityIPAddress ----------- */
typedef TknStrBSXL CztTraceCollectionEntityIPAddr;
typedef struct x2ap_ExtnTraceActvn_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnTraceActvn_ExtIEsCls;
typedef struct x2apProtExtnField_TraceActvn_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnTraceActvn_ExtIEsCls extensionValue;
} CztProtExtnField_TraceActvn_ExtIEs;

typedef struct x2apProtExtnCont_TraceActvn_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_TraceActvn_ExtIEs *member;
}CztProtExtnCont_TraceActvn_ExtIEs;

typedef struct x2apTraceActvn
{
        TknPres pres;
        CztEUTRANTraceID eUTRANTraceID;
        CztIntfsToTrace interfacesToTrace;
        CztTraceDepth traceDepth;
        CztTraceCollectionEntityIPAddr traceCollectionEntityIPAddr;
        CztProtExtnCont_TraceActvn_ExtIEs iE_Extns;
} CztTraceActvn;

typedef struct x2apUE_HistoryInform
{
        TknU16 noComp;
        CztLastVisitedCell_Item *member;
}CztUE_HistoryInform;

/* Element ----- UE-S1AP-ID ------- */
typedef TknU32 CztUE_S1AP_ID;

/* Element ----- UE-X2AP-ID ------- */
typedef TknU32 CztUE_X2AP_ID;

typedef struct x2ap_ExtnUEAgg_MaxBitrate_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnUEAgg_MaxBitrate_ExtIEsCls;
typedef struct x2apProtExtnField_UEAgg_MaxBitrate_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnUEAgg_MaxBitrate_ExtIEsCls extensionValue;
} CztProtExtnField_UEAgg_MaxBitrate_ExtIEs;

typedef struct x2apProtExtnCont_UEAgg_MaxBitrate_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_UEAgg_MaxBitrate_ExtIEs *member;
}CztProtExtnCont_UEAgg_MaxBitrate_ExtIEs;

typedef struct x2apUEAggMaxBitRate
{
        TknPres pres;
        CztBitRate uEaggregateMaxBitRateDlnk;
        CztBitRate uEaggregateMaxBitRateUlnk;
        CztProtExtnCont_UEAgg_MaxBitrate_ExtIEs iE_Extns;
} CztUEAggMaxBitRate;

typedef struct x2ap_ExtnUESecurCapabilities_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnUESecurCapabilities_ExtIEsCls;
typedef struct x2apProtExtnField_UESecurCapabilities_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnUESecurCapabilities_ExtIEsCls extensionValue;
} CztProtExtnField_UESecurCapabilities_ExtIEs;

typedef struct x2apProtExtnCont_UESecurCapabilities_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_UESecurCapabilities_ExtIEs *member;
}CztProtExtnCont_UESecurCapabilities_ExtIEs;

typedef struct x2apUESecurCapabilities
{
        TknPres pres;
        CztEncryptionAlgorithms encryptionAlgorithms;
        CztIntegrityProtectionAlgorithms integrityProtectionAlgorithms;
        CztProtExtnCont_UESecurCapabilities_ExtIEs iE_Extns;
} CztUESecurCapabilities;

/* Element ----- UL-InterferenceOverloadIndication-Item ------- */
typedef enum enumCztUL_InterferenceOverloadInd_Item
{
        CztUL_InterferenceOverloadInd_Itemhigh_interferenceEnum,
        CztUL_InterferenceOverloadInd_Itemmedium_interferenceEnum,
        CztUL_InterferenceOverloadInd_Itemlow_interferenceEnum
} EnumCztUL_InterferenceOverloadInd_Item;
typedef TknU32 CztUL_InterferenceOverloadInd_Item;

typedef struct x2apUL_InterferenceOverloadInd
{
        TknU16 noComp;
        CztUL_InterferenceOverloadInd_Item *member;
}CztUL_InterferenceOverloadInd;

/* Element ---------- UL-HighInterferenceIndication ----------- */
typedef TknStrBSXL CztUL_HighInterferenceInd;
typedef struct x2ap_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsCls;
typedef struct x2apProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsCls extensionValue;
} CztProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEs;

typedef struct x2apProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEs *member;
}CztProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs;

typedef struct x2apUL_HighInterferenceIndInfo_Item
{
        TknPres pres;
        CztECGI target_Cell_ID;
        CztUL_HighInterferenceInd ul_interferenceindication;
        CztProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs iE_Extns;
} CztUL_HighInterferenceIndInfo_Item;

typedef struct x2apUL_HighInterferenceIndInfo
{
        TknU16 noComp;
        CztUL_HighInterferenceIndInfo_Item *member;
}CztUL_HighInterferenceIndInfo;

/* Element ---------- UE-RLF-Report-Container ----------- */
typedef TknStrOSXL CztUE_RLF_Report_Cont;
typedef struct x2ap_ExtnE_RABs_ToBeSetup_ItemExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnE_RABs_ToBeSetup_ItemExtIEsCls;
typedef struct x2apProtExtnField_E_RABs_ToBeSetup_ItemExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnE_RABs_ToBeSetup_ItemExtIEsCls extensionValue;
} CztProtExtnField_E_RABs_ToBeSetup_ItemExtIEs;

typedef struct x2apProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs
{
        TknU16 noComp;
        CztProtExtnField_E_RABs_ToBeSetup_ItemExtIEs *member;
}CztProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs;

typedef struct x2apE_RABs_ToBeSetup_Item
{
        TknPres pres;
        CztE_RAB_ID e_RAB_ID;
        CztE_RAB_Lvl_QoS_Params e_RAB_Lvl_QoS_Params;
        CztDL_Fwding dL_Fwding;
        CztGTPtunnelEndpoint uL_GTPtunnelEndpoint;
        CztProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs iE_Extns;
} CztE_RABs_ToBeSetup_Item;

typedef struct x2ap_ValueE_RABs_ToBeSetup_ItemIEsCls{
        union {
                CztE_RABs_ToBeSetup_Item cztE_RABs_ToBeSetup_Item;
        } u;
} Czt_ValueE_RABs_ToBeSetup_ItemIEsCls;
typedef struct x2apProtIE_Field_E_RABs_ToBeSetup_ItemIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueE_RABs_ToBeSetup_ItemIEsCls value;
} CztProtIE_Field_E_RABs_ToBeSetup_ItemIEs;

/* Element ------------- ProtocolIE-Single-Container ------------ */
typedef CztProtIE_Field_E_RABs_ToBeSetup_ItemIEs CztProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs;
typedef struct x2apE_RABs_ToBeSetup_Lst
{
        TknU16 noComp;
        CztProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs *member;
}CztE_RABs_ToBeSetup_Lst;

typedef struct x2ap_ExtnUE_CntxtInform_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnUE_CntxtInform_ExtIEsCls;
typedef struct x2apProtExtnField_UE_CntxtInform_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnUE_CntxtInform_ExtIEsCls extensionValue;
} CztProtExtnField_UE_CntxtInform_ExtIEs;

typedef struct x2apProtExtnCont_UE_CntxtInform_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_UE_CntxtInform_ExtIEs *member;
}CztProtExtnCont_UE_CntxtInform_ExtIEs;

typedef struct x2apUE_CntxtInform
{
        TknPres pres;
        CztUE_S1AP_ID mME_UE_S1AP_ID;
        CztUESecurCapabilities uESecurCapabilities;
        CztAS_SecurInform aS_SecurInform;
        CztUEAggMaxBitRate uEaggregateMaxBitRate;
        CztSubscriberProfileIDforRFP subscriberProfileIDforRFP;
        CztE_RABs_ToBeSetup_Lst e_RABs_ToBeSetup_Lst;
        CztRRC_Cntxt rRC_Cntxt;
        CztHovrRestrnLst handoverRestrnLst;
        CztLocRprtngInform locationRprtngInform;
        CztProtExtnCont_UE_CntxtInform_ExtIEs iE_Extns;
} CztUE_CntxtInform;

typedef struct x2ap_ValueHovrRqst_IEsCls{
        union {
                CztUE_X2AP_ID cztUE_X2AP_ID;
                CztCause cztCause;
                CztECGI cztECGI;
                CztGUMMEI cztGUMMEI;
                CztUE_CntxtInform cztUE_CntxtInform;
                CztUE_HistoryInform cztUE_HistoryInform;
                CztTraceActvn cztTraceActvn;
                CztSRVCCOperationPossible cztSRVCCOperationPossible;
        } u;
} Czt_ValueHovrRqst_IEsCls;
typedef struct x2apProtIE_Field_HovrRqst_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueHovrRqst_IEsCls value;
} CztProtIE_Field_HovrRqst_IEs;

typedef struct x2apProtIE_Cont_HovrRqst_IEs
{
        TknU16 noComp;
        CztProtIE_Field_HovrRqst_IEs *member;
}CztProtIE_Cont_HovrRqst_IEs;

typedef struct x2apHovrRqst
{
        TknPres pres;
        CztProtIE_Cont_HovrRqst_IEs protocolIEs;
} CztHovrRqst;

typedef struct x2ap_ExtnE_RABs_Admtd_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnE_RABs_Admtd_Item_ExtIEsCls;
typedef struct x2apProtExtnField_E_RABs_Admtd_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnE_RABs_Admtd_Item_ExtIEsCls extensionValue;
} CztProtExtnField_E_RABs_Admtd_Item_ExtIEs;

typedef struct x2apProtExtnCont_E_RABs_Admtd_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_E_RABs_Admtd_Item_ExtIEs *member;
}CztProtExtnCont_E_RABs_Admtd_Item_ExtIEs;

typedef struct x2apE_RABs_Admtd_Item
{
        TknPres pres;
        CztE_RAB_ID e_RAB_ID;
        CztGTPtunnelEndpoint uL_GTP_TunnelEndpoint;
        CztGTPtunnelEndpoint dL_GTP_TunnelEndpoint;
        CztProtExtnCont_E_RABs_Admtd_Item_ExtIEs iE_Extns;
} CztE_RABs_Admtd_Item;

typedef struct x2ap_ValueE_RABs_Admtd_ItemIEsCls{
        union {
                CztE_RABs_Admtd_Item cztE_RABs_Admtd_Item;
        } u;
} Czt_ValueE_RABs_Admtd_ItemIEsCls;
typedef struct x2apProtIE_Field_E_RABs_Admtd_ItemIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueE_RABs_Admtd_ItemIEsCls value;
} CztProtIE_Field_E_RABs_Admtd_ItemIEs;

/* Element ------------- ProtocolIE-Single-Container ------------ */
typedef CztProtIE_Field_E_RABs_Admtd_ItemIEs CztProtIE_Single_Cont_E_RABs_Admtd_ItemIEs;
typedef struct x2apE_RABs_Admtd_Lst
{
        TknU16 noComp;
        CztProtIE_Single_Cont_E_RABs_Admtd_ItemIEs *member;
}CztE_RABs_Admtd_Lst;

typedef struct x2ap_ValueHovrRqstAckg_IEsCls{
        union {
                CztUE_X2AP_ID cztUE_X2AP_ID_Old;
/* czt_asn_x_001.main_1 : X2AP 3.1 : ccpu00117267 */
                CztUE_X2AP_ID cztUE_X2AP_ID_New; 
                CztE_RABs_Admtd_Lst cztE_RABs_Admtd_Lst;
                CztE_RAB_Lst cztE_RAB_Lst;
                CztTgeteNBtoSrc_eNBTprntCont cztTgeteNBtoSrc_eNBTprntCont;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueHovrRqstAckg_IEsCls;
typedef struct x2apProtIE_Field_HovrRqstAckg_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueHovrRqstAckg_IEsCls value;
} CztProtIE_Field_HovrRqstAckg_IEs;

typedef struct x2apProtIE_Cont_HovrRqstAckg_IEs
{
        TknU16 noComp;
        CztProtIE_Field_HovrRqstAckg_IEs *member;
}CztProtIE_Cont_HovrRqstAckg_IEs;

typedef struct x2apHovrRqstAckg
{
        TknPres pres;
        CztProtIE_Cont_HovrRqstAckg_IEs protocolIEs;
} CztHovrRqstAckg;

typedef struct x2ap_ValueHovrPrepFail_IEsCls{
        union {
                CztUE_X2AP_ID cztUE_X2AP_ID;
                CztCause cztCause;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueHovrPrepFail_IEsCls;
typedef struct x2apProtIE_Field_HovrPrepFail_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueHovrPrepFail_IEsCls value;
} CztProtIE_Field_HovrPrepFail_IEs;

typedef struct x2apProtIE_Cont_HovrPrepFail_IEs
{
        TknU16 noComp;
        CztProtIE_Field_HovrPrepFail_IEs *member;
}CztProtIE_Cont_HovrPrepFail_IEs;

typedef struct x2apHovrPrepFail
{
        TknPres pres;
        CztProtIE_Cont_HovrPrepFail_IEs protocolIEs;
} CztHovrPrepFail;

typedef struct x2ap_ValueHovrReport_IEsCls{
        union {
                CztHovrReportTyp cztHovrReportTyp;
                CztCause cztCause;
                CztECGI cztSrcCellECGI;
                CztECGI cztFailCellECGI;
                CztECGI cztreEstCellECGI; /* X2AP 3.1 */
        } u;
} Czt_ValueHovrReport_IEsCls;
typedef struct x2apProtIE_Field_HovrReport_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueHovrReport_IEsCls value;
} CztProtIE_Field_HovrReport_IEs;

typedef struct x2apProtIE_Cont_HovrReport_IEs
{
        TknU16 noComp;
        CztProtIE_Field_HovrReport_IEs *member;
}CztProtIE_Cont_HovrReport_IEs;

typedef struct x2apHovrReport
{
        TknPres pres;
        CztProtIE_Cont_HovrReport_IEs protocolIEs;
} CztHovrReport;

typedef struct x2ap_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsCls;
typedef struct x2apProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsCls extensionValue;
} CztProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs;

typedef struct x2apProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs
{
        TknU16 noComp;
        CztProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs *member;
}CztProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs;

typedef struct x2apE_RABs_SubjToStatusTfr_Item
{
        TknPres pres;
        CztE_RAB_ID e_RAB_ID;
        CztReceiveStatusofULPDCPSDUs receiveStatusofULPDCPSDUs;
        CztCOUNTvalue uL_COUNTvalue;
        CztCOUNTvalue dL_COUNTvalue;
        CztProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs iE_Extns;
} CztE_RABs_SubjToStatusTfr_Item;

typedef struct x2ap_ValueE_RABs_SubjToStatusTfr_ItemIEsCls{
        union {
                CztE_RABs_SubjToStatusTfr_Item cztE_RABs_SubjToStatusTfr_Item;
        } u;
} Czt_ValueE_RABs_SubjToStatusTfr_ItemIEsCls;
typedef struct x2apProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueE_RABs_SubjToStatusTfr_ItemIEsCls value;
} CztProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs;

/* Element ------------- ProtocolIE-Single-Container ------------ */
typedef CztProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs CztProtIE_Single_Cont_E_RABs_SubjToStatusTfr_ItemIEs;
typedef struct x2apE_RABs_SubjToStatusTfr_Lst
{
        TknU16 noComp;
        CztProtIE_Single_Cont_E_RABs_SubjToStatusTfr_ItemIEs *member;
}CztE_RABs_SubjToStatusTfr_Lst;

typedef struct x2ap_ValueSNStatusTfr_IEsCls{
        union {
                CztUE_X2AP_ID cztUE_X2AP_ID_Old;
               /* czt_asn_x_001.main_1 */
                CztUE_X2AP_ID cztUE_X2AP_ID_New;  /*X2AP 3.1: ccpu00117267*/
                CztE_RABs_SubjToStatusTfr_Lst cztE_RABs_SubjToStatusTfr_Lst;
        } u;
} Czt_ValueSNStatusTfr_IEsCls;
typedef struct x2apProtIE_Field_SNStatusTfr_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueSNStatusTfr_IEsCls value;
} CztProtIE_Field_SNStatusTfr_IEs;

typedef struct x2apProtIE_Cont_SNStatusTfr_IEs
{
        TknU16 noComp;
        CztProtIE_Field_SNStatusTfr_IEs *member;
}CztProtIE_Cont_SNStatusTfr_IEs;

typedef struct x2apSNStatusTfr
{
        TknPres pres;
        CztProtIE_Cont_SNStatusTfr_IEs protocolIEs;
} CztSNStatusTfr;

typedef struct x2ap_ValueUECntxtRls_IEsCls{
        union {
                CztUE_X2AP_ID cztUE_X2AP_ID_Old;
                /* czt_asn_x_001.main_1 */
                CztUE_X2AP_ID cztUE_X2AP_ID_New;  /* X2AP 3.1 : ccpu00117267*/
        } u;
} Czt_ValueUECntxtRls_IEsCls;
typedef struct x2apProtIE_Field_UECntxtRls_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueUECntxtRls_IEsCls value;
} CztProtIE_Field_UECntxtRls_IEs;

typedef struct x2apProtIE_Cont_UECntxtRls_IEs
{
        TknU16 noComp;
        CztProtIE_Field_UECntxtRls_IEs *member;
}CztProtIE_Cont_UECntxtRls_IEs;

typedef struct x2apUECntxtRls
{
        TknPres pres;
        CztProtIE_Cont_UECntxtRls_IEs protocolIEs;
} CztUECntxtRls;

typedef struct x2ap_ValueHovrCancel_IEsCls{
        union {
                CztUE_X2AP_ID cztUE_X2AP_ID_Old;
                /* czt_asn_x_001.main_1 */
                CztUE_X2AP_ID cztUE_X2AP_ID_New;  /*X2AP 3.1 : ccpu00117267*/
                CztCause cztCause;
        } u;
} Czt_ValueHovrCancel_IEsCls;
typedef struct x2apProtIE_Field_HovrCancel_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueHovrCancel_IEsCls value;
} CztProtIE_Field_HovrCancel_IEs;

typedef struct x2apProtIE_Cont_HovrCancel_IEs
{
        TknU16 noComp;
        CztProtIE_Field_HovrCancel_IEs *member;
}CztProtIE_Cont_HovrCancel_IEs;

typedef struct x2apHovrCancel
{
        TknPres pres;
        CztProtIE_Cont_HovrCancel_IEs protocolIEs;
} CztHovrCancel;

typedef struct x2ap_ValueErrInd_IEsCls{
        union {
                CztUE_X2AP_ID cztUE_X2AP_ID_Old;
                /* czt_asn_x_001.main_1 */
                CztUE_X2AP_ID cztUE_X2AP_ID_New;  /* X2AP 3.1 : ccpu00117267*/
                CztCause cztCause;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueErrInd_IEsCls;
typedef struct x2apProtIE_Field_ErrInd_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueErrInd_IEsCls value;
} CztProtIE_Field_ErrInd_IEs;

typedef struct x2apProtIE_Cont_ErrInd_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ErrInd_IEs *member;
}CztProtIE_Cont_ErrInd_IEs;

typedef struct x2apErrInd
{
        TknPres pres;
        CztProtIE_Cont_ErrInd_IEs protocolIEs;
} CztErrInd;

typedef struct x2ap_ValueResetRqst_IEsCls{
        union {
                CztCause cztCause;
        } u;
} Czt_ValueResetRqst_IEsCls;
typedef struct x2apProtIE_Field_ResetRqst_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueResetRqst_IEsCls value;
} CztProtIE_Field_ResetRqst_IEs;

typedef struct x2apProtIE_Cont_ResetRqst_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ResetRqst_IEs *member;
}CztProtIE_Cont_ResetRqst_IEs;

typedef struct x2apResetRqst
{
        TknPres pres;
        CztProtIE_Cont_ResetRqst_IEs protocolIEs;
} CztResetRqst;

typedef struct x2ap_ValueResetResp_IEsCls{
        union {
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueResetResp_IEsCls;
typedef struct x2apProtIE_Field_ResetResp_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueResetResp_IEsCls value;
} CztProtIE_Field_ResetResp_IEs;

typedef struct x2apProtIE_Cont_ResetResp_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ResetResp_IEs *member;
}CztProtIE_Cont_ResetResp_IEs;

typedef struct x2apResetResp
{
        TknPres pres;
        CztProtIE_Cont_ResetResp_IEs protocolIEs;
} CztResetResp;

typedef struct x2ap_ValueX2SetupRqst_IEsCls{
        union {
                CztGlobalENB_ID cztGlobalENB_ID;
                CztSrvdCells cztSrvdCells;
                CztGUGroupIDLst cztGUGroupIDLst;
        } u;
} Czt_ValueX2SetupRqst_IEsCls;
typedef struct x2apProtIE_Field_X2SetupRqst_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueX2SetupRqst_IEsCls value;
} CztProtIE_Field_X2SetupRqst_IEs;

typedef struct x2apProtIE_Cont_X2SetupRqst_IEs
{
        TknU16 noComp;
        CztProtIE_Field_X2SetupRqst_IEs *member;
}CztProtIE_Cont_X2SetupRqst_IEs;

typedef struct x2apX2SetupRqst
{
        TknPres pres;
        CztProtIE_Cont_X2SetupRqst_IEs protocolIEs;
} CztX2SetupRqst;

typedef struct x2ap_ValueX2SetupResp_IEsCls{
        union {
                CztGlobalENB_ID cztGlobalENB_ID;
                CztSrvdCells cztSrvdCells;
                CztGUGroupIDLst cztGUGroupIDLst;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueX2SetupResp_IEsCls;
typedef struct x2apProtIE_Field_X2SetupResp_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueX2SetupResp_IEsCls value;
} CztProtIE_Field_X2SetupResp_IEs;

typedef struct x2apProtIE_Cont_X2SetupResp_IEs
{
        TknU16 noComp;
        CztProtIE_Field_X2SetupResp_IEs *member;
}CztProtIE_Cont_X2SetupResp_IEs;

typedef struct x2apX2SetupResp
{
        TknPres pres;
        CztProtIE_Cont_X2SetupResp_IEs protocolIEs;
} CztX2SetupResp;

typedef struct x2ap_ValueX2SetupFail_IEsCls{
        union {
                CztCause cztCause;
                CztTimeToWait cztTimeToWait;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueX2SetupFail_IEsCls;
typedef struct x2apProtIE_Field_X2SetupFail_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueX2SetupFail_IEsCls value;
} CztProtIE_Field_X2SetupFail_IEs;

typedef struct x2apProtIE_Cont_X2SetupFail_IEs
{
        TknU16 noComp;
        CztProtIE_Field_X2SetupFail_IEs *member;
}CztProtIE_Cont_X2SetupFail_IEs;

typedef struct x2apX2SetupFail
{
        TknPres pres;
        CztProtIE_Cont_X2SetupFail_IEs protocolIEs;
} CztX2SetupFail;

typedef struct x2ap_ExtnCellInform_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCellInform_Item_ExtIEsCls;
typedef struct x2apProtExtnField_CellInform_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCellInform_Item_ExtIEsCls extensionValue;
} CztProtExtnField_CellInform_Item_ExtIEs;

typedef struct x2apProtExtnCont_CellInform_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CellInform_Item_ExtIEs *member;
}CztProtExtnCont_CellInform_Item_ExtIEs;

typedef struct x2apCellInform_Item
{
        TknPres pres;
        CztECGI cell_ID;
        CztUL_InterferenceOverloadInd ul_InterferenceOverloadInd;
        CztUL_HighInterferenceIndInfo ul_HighInterferenceIndInfo;
        CztRelativeNarrowbandTxPower relativeNarrowbandTxPower;
        CztProtExtnCont_CellInform_Item_ExtIEs iE_Extns;
} CztCellInform_Item;

typedef struct x2ap_ValueCellInform_ItemIEsCls{
        union {
                CztCellInform_Item cztCellInform_Item;
        } u;
} Czt_ValueCellInform_ItemIEsCls;
typedef struct x2apProtIE_Field_CellInform_ItemIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueCellInform_ItemIEsCls value;
} CztProtIE_Field_CellInform_ItemIEs;

/* Element ------------- ProtocolIE-Single-Container ------------ */
typedef CztProtIE_Field_CellInform_ItemIEs CztProtIE_Single_Cont_CellInform_ItemIEs;
typedef struct x2apCellInform_Lst
{
        TknU16 noComp;
        CztProtIE_Single_Cont_CellInform_ItemIEs *member;
}CztCellInform_Lst;

typedef struct x2ap_ValueLoadInform_IEsCls{
        union {
                CztCellInform_Lst cztCellInform_Lst;
        } u;
} Czt_ValueLoadInform_IEsCls;
typedef struct x2apProtIE_Field_LoadInform_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueLoadInform_IEsCls value;
} CztProtIE_Field_LoadInform_IEs;

typedef struct x2apProtIE_Cont_LoadInform_IEs
{
        TknU16 noComp;
        CztProtIE_Field_LoadInform_IEs *member;
}CztProtIE_Cont_LoadInform_IEs;

typedef struct x2apLoadInform
{
        TknPres pres;
        CztProtIE_Cont_LoadInform_IEs protocolIEs;
} CztLoadInform;

typedef struct x2ap_ExtnSrvdCellsToMdfy_Item_ExtIEsCls{
        union {
                CztDeactivationInd cztDeactivationInd;
        } u;
} Czt_ExtnSrvdCellsToMdfy_Item_ExtIEsCls;
typedef struct x2apProtExtnField_SrvdCellsToMdfy_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnSrvdCellsToMdfy_Item_ExtIEsCls extensionValue;
} CztProtExtnField_SrvdCellsToMdfy_Item_ExtIEs;

typedef struct x2apProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_SrvdCellsToMdfy_Item_ExtIEs *member;
}CztProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs;

typedef struct x2apSrvdCellsToMdfy_Item
{
        TknPres pres;
        CztECGI old_ecgi;
        CztSrvdCell_Inform servedCellInfo;
        CztNeighbour_Inform neighbour_Info;
        CztProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs iE_Extns;
} CztSrvdCellsToMdfy_Item;

typedef struct x2apSrvdCellsToMdfy
{
        TknU16 noComp;
        CztSrvdCellsToMdfy_Item *member;
}CztSrvdCellsToMdfy;

typedef struct x2apOld_ECGIs
{
        TknU16 noComp;
        CztECGI *member;
}CztOld_ECGIs;

typedef struct x2ap_ValueENBConfigUpd_IEsCls{
        union {
                CztSrvdCells cztSrvdCells;
                CztSrvdCellsToMdfy cztSrvdCellsToMdfy;
                CztOld_ECGIs cztOld_ECGIs;
                CztGUGroupIDLst cztGUGroupIDLst;
                CztGUGroupIDLst cztGUGroupIDLst2; /* X2AP 3.1 */
        } u;
} Czt_ValueENBConfigUpd_IEsCls;
typedef struct x2apProtIE_Field_ENBConfigUpd_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueENBConfigUpd_IEsCls value;
} CztProtIE_Field_ENBConfigUpd_IEs;

typedef struct x2apProtIE_Cont_ENBConfigUpd_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ENBConfigUpd_IEs *member;
}CztProtIE_Cont_ENBConfigUpd_IEs;

typedef struct x2apENBConfigUpd
{
        TknPres pres;
        CztProtIE_Cont_ENBConfigUpd_IEs protocolIEs;
} CztENBConfigUpd;

typedef struct x2ap_ValueENBConfigUpdAckg_IEsCls{
        union {
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueENBConfigUpdAckg_IEsCls;
typedef struct x2apProtIE_Field_ENBConfigUpdAckg_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueENBConfigUpdAckg_IEsCls value;
} CztProtIE_Field_ENBConfigUpdAckg_IEs;

typedef struct x2apProtIE_Cont_ENBConfigUpdAckg_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ENBConfigUpdAckg_IEs *member;
}CztProtIE_Cont_ENBConfigUpdAckg_IEs;

typedef struct x2apENBConfigUpdAckg
{
        TknPres pres;
        CztProtIE_Cont_ENBConfigUpdAckg_IEs protocolIEs;
} CztENBConfigUpdAckg;

typedef struct x2ap_ValueENBConfigUpdFail_IEsCls{
        union {
                CztCause cztCause;
                CztTimeToWait cztTimeToWait;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueENBConfigUpdFail_IEsCls;
typedef struct x2apProtIE_Field_ENBConfigUpdFail_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueENBConfigUpdFail_IEsCls value;
} CztProtIE_Field_ENBConfigUpdFail_IEs;

typedef struct x2apProtIE_Cont_ENBConfigUpdFail_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ENBConfigUpdFail_IEs *member;
}CztProtIE_Cont_ENBConfigUpdFail_IEs;

typedef struct x2apENBConfigUpdFail
{
        TknPres pres;
        CztProtIE_Cont_ENBConfigUpdFail_IEs protocolIEs;
} CztENBConfigUpdFail;

typedef struct x2ap_ExtnCellToReport_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnCellToReport_Item_ExtIEsCls;
typedef struct x2apProtExtnField_CellToReport_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCellToReport_Item_ExtIEsCls extensionValue;
} CztProtExtnField_CellToReport_Item_ExtIEs;

typedef struct x2apProtExtnCont_CellToReport_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CellToReport_Item_ExtIEs *member;
}CztProtExtnCont_CellToReport_Item_ExtIEs;

typedef struct x2apCellToReport_Item
{
        TknPres pres;
        CztECGI cell_ID;
        CztProtExtnCont_CellToReport_Item_ExtIEs iE_Extns;
} CztCellToReport_Item;

typedef struct x2ap_ValueCellToReport_ItemIEsCls{
        union {
                CztCellToReport_Item cztCellToReport_Item;
        } u;
} Czt_ValueCellToReport_ItemIEsCls;
typedef struct x2apProtIE_Field_CellToReport_ItemIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueCellToReport_ItemIEsCls value;
} CztProtIE_Field_CellToReport_ItemIEs;

/* Element ------------- ProtocolIE-Single-Container ------------ */
typedef CztProtIE_Field_CellToReport_ItemIEs CztProtIE_Single_Cont_CellToReport_ItemIEs;
typedef struct x2apCellToReport_Lst
{
        TknU16 noComp;
        CztProtIE_Single_Cont_CellToReport_ItemIEs *member;
}CztCellToReport_Lst;

/* Element ----- ReportingPeriodicity ------- */
typedef enum enumCztRprtngPeriodicity
{
        CztRprtngPeriodicityone_thousand_msEnum,
        CztRprtngPeriodicitytwo_thousand_msEnum,
        CztRprtngPeriodicityfive_thousand_msEnum,
        CztRprtngPeriodicityten_thousand_msEnum
} EnumCztRprtngPeriodicity;
typedef TknU32 CztRprtngPeriodicity;

typedef struct x2ap_ValueResStatusRqst_IEsCls{
        union {
                CztMeasurement_ID cztMeasurement_ID;
                CztMeasurement_ID cztMeasurement_ID2;  /* X2AP 3.1*/
                CztRegistration_Rqst cztRegistration_Rqst;
                CztReportCharacteristics cztReportCharacteristics;
                CztCellToReport_Lst cztCellToReport_Lst;
                CztRprtngPeriodicity cztRprtngPeriodicity;
        } u;
} Czt_ValueResStatusRqst_IEsCls;
typedef struct x2apProtIE_Field_ResStatusRqst_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueResStatusRqst_IEsCls value;
} CztProtIE_Field_ResStatusRqst_IEs;

typedef struct x2apProtIE_Cont_ResStatusRqst_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ResStatusRqst_IEs *member;
}CztProtIE_Cont_ResStatusRqst_IEs;

typedef struct x2apResStatusRqst
{
        TknPres pres;
        CztProtIE_Cont_ResStatusRqst_IEs protocolIEs;
} CztResStatusRqst;

typedef struct x2ap_ValueResStatusResp_IEsCls{
        union {
                CztMeasurement_ID cztMeasurement_ID;
                CztMeasurement_ID cztMeasurement_ID2; /* X2AP 3.1*/
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueResStatusResp_IEsCls;
typedef struct x2apProtIE_Field_ResStatusResp_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueResStatusResp_IEsCls value;
} CztProtIE_Field_ResStatusResp_IEs;

typedef struct x2apProtIE_Cont_ResStatusResp_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ResStatusResp_IEs *member;
}CztProtIE_Cont_ResStatusResp_IEs;

typedef struct x2apResStatusResp
{
        TknPres pres;
        CztProtIE_Cont_ResStatusResp_IEs protocolIEs;
} CztResStatusResp;

typedef struct x2ap_ValueResStatusFail_IEsCls{
        union {
                CztMeasurement_ID cztMeasurement_ID;
                CztMeasurement_ID cztMeasurement_ID2; /* X2AP 3.1 */
                CztCause cztCause;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueResStatusFail_IEsCls;
typedef struct x2apProtIE_Field_ResStatusFail_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueResStatusFail_IEsCls value;
} CztProtIE_Field_ResStatusFail_IEs;

typedef struct x2apProtIE_Cont_ResStatusFail_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ResStatusFail_IEs *member;
}CztProtIE_Cont_ResStatusFail_IEs;

typedef struct x2apResStatusFail
{
        TknPres pres;
        CztProtIE_Cont_ResStatusFail_IEs protocolIEs;
} CztResStatusFail;

typedef struct x2ap_ExtnCellMeasurementResult_Item_ExtIEsCls{
        union {
                CztCompositeAvailableCapacityGroup cztCompositeAvailableCapacityGroup;
        } u;
} Czt_ExtnCellMeasurementResult_Item_ExtIEsCls;
typedef struct x2apProtExtnField_CellMeasurementResult_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnCellMeasurementResult_Item_ExtIEsCls extensionValue;
} CztProtExtnField_CellMeasurementResult_Item_ExtIEs;

typedef struct x2apProtExtnCont_CellMeasurementResult_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_CellMeasurementResult_Item_ExtIEs *member;
}CztProtExtnCont_CellMeasurementResult_Item_ExtIEs;

typedef struct x2apCellMeasurementResult_Item
{
        TknPres pres;
        CztECGI cell_ID;
        CztHWLoadIndicator hWOverLoadIndicator;
        CztS1TNLLoadIndicator s1TNLOverLoadIndicator;
        CztRadioResStatus radioResStatus;
        CztProtExtnCont_CellMeasurementResult_Item_ExtIEs iE_Extns;
} CztCellMeasurementResult_Item;

typedef struct x2ap_ValueCellMeasurementResult_ItemIEsCls{
        union {
                CztCellMeasurementResult_Item cztCellMeasurementResult_Item;
        } u;
} Czt_ValueCellMeasurementResult_ItemIEsCls;
typedef struct x2apProtIE_Field_CellMeasurementResult_ItemIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueCellMeasurementResult_ItemIEsCls value;
} CztProtIE_Field_CellMeasurementResult_ItemIEs;

/* Element ------------- ProtocolIE-Single-Container ------------ */
typedef CztProtIE_Field_CellMeasurementResult_ItemIEs CztProtIE_Single_Cont_CellMeasurementResult_ItemIEs;
typedef struct x2apCellMeasurementResult_Lst
{
        TknU16 noComp;
        CztProtIE_Single_Cont_CellMeasurementResult_ItemIEs *member;
}CztCellMeasurementResult_Lst;

typedef struct x2ap_ValueResStatusUpd_IEsCls{
        union {
                CztMeasurement_ID cztMeasurement_ID;
                CztMeasurement_ID cztMeasurement_ID2;  /* X2AP 3.1*/
                CztCellMeasurementResult_Lst cztCellMeasurementResult_Lst;
        } u;
} Czt_ValueResStatusUpd_IEsCls;
typedef struct x2apProtIE_Field_ResStatusUpd_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueResStatusUpd_IEsCls value;
} CztProtIE_Field_ResStatusUpd_IEs;

typedef struct x2apProtIE_Cont_ResStatusUpd_IEs
{
        TknU16 noComp;
        CztProtIE_Field_ResStatusUpd_IEs *member;
}CztProtIE_Cont_ResStatusUpd_IEs;

typedef struct x2apResStatusUpd
{
        TknPres pres;
        CztProtIE_Cont_ResStatusUpd_IEs protocolIEs;
} CztResStatusUpd;

typedef struct x2ap_ValuePrivMsg_IEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ValuePrivMsg_IEsCls;
typedef struct x2apPrivIE_Field_PrivMsg_IEs
{
        TknPres pres;
        CztPrivIE_ID id;
        CztCriticality criticality;
        Czt_ValuePrivMsg_IEsCls value;
} CztPrivIE_Field_PrivMsg_IEs;

typedef struct x2apPrivIE_Cont_PrivMsg_IEs
{
        TknU16 noComp;
        CztPrivIE_Field_PrivMsg_IEs *member;
}CztPrivIE_Cont_PrivMsg_IEs;

typedef struct x2apPrivMsg
{
        TknPres pres;
        CztPrivIE_Cont_PrivMsg_IEs privateIEs;
} CztPrivMsg;

typedef struct x2ap_ValueMobilityChangeRqst_IEsCls{
        union {
                CztECGI czteNB1ECGI;
                CztECGI czteNB2ECGI; /* X2AP 3.1 */
                CztMobilityParamsInform czteNB1MobilityParamsInform;
                CztMobilityParamsInform czteNB2MobilityParamsInform; /* X2AP 3.1  */
                CztCause cztCause;
        } u;
} Czt_ValueMobilityChangeRqst_IEsCls;
typedef struct x2apProtIE_Field_MobilityChangeRqst_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueMobilityChangeRqst_IEsCls value;
} CztProtIE_Field_MobilityChangeRqst_IEs;

typedef struct x2apProtIE_Cont_MobilityChangeRqst_IEs
{
        TknU16 noComp;
        CztProtIE_Field_MobilityChangeRqst_IEs *member;
}CztProtIE_Cont_MobilityChangeRqst_IEs;

typedef struct x2apMobilityChangeRqst
{
        TknPres pres;
        CztProtIE_Cont_MobilityChangeRqst_IEs protocolIEs;
} CztMobilityChangeRqst;

typedef struct x2ap_ValueMobilityChangeAckg_IEsCls{
        union {
                CztECGI czteNB1ECGI;
                CztECGI czteNB2ECGI;  /* X2AP 3.1 */
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueMobilityChangeAckg_IEsCls;
typedef struct x2apProtIE_Field_MobilityChangeAckg_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueMobilityChangeAckg_IEsCls value;
} CztProtIE_Field_MobilityChangeAckg_IEs;

typedef struct x2apProtIE_Cont_MobilityChangeAckg_IEs
{
        TknU16 noComp;
        CztProtIE_Field_MobilityChangeAckg_IEs *member;
}CztProtIE_Cont_MobilityChangeAckg_IEs;

typedef struct x2apMobilityChangeAckg
{
        TknPres pres;
        CztProtIE_Cont_MobilityChangeAckg_IEs protocolIEs;
} CztMobilityChangeAckg;

typedef struct x2ap_ValueMobilityChangeFail_IEsCls{
        union {
                CztECGI czteNB1ECGI;
                CztECGI czteNB2ECGI;  /* X2AP 3.1*/
                CztCause cztCause;
                CztMobilityParamsModificationRange cztMobilityParamsModificationRange;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueMobilityChangeFail_IEsCls;
typedef struct x2apProtIE_Field_MobilityChangeFail_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueMobilityChangeFail_IEsCls value;
} CztProtIE_Field_MobilityChangeFail_IEs;

typedef struct x2apProtIE_Cont_MobilityChangeFail_IEs
{
        TknU16 noComp;
        CztProtIE_Field_MobilityChangeFail_IEs *member;
}CztProtIE_Cont_MobilityChangeFail_IEs;

typedef struct x2apMobilityChangeFail
{
        TknPres pres;
        CztProtIE_Cont_MobilityChangeFail_IEs protocolIEs;
} CztMobilityChangeFail;

typedef struct x2ap_ValueRLFInd_IEsCls{
        union {
                CztPCI cztPCI;
                CztECGI cztECGI;
                CztCRNTI cztCRNTI;
                CztShortMAC_I cztShortMAC_I;
                CztUE_RLF_Report_Cont cztUE_RLF_Report_Cont;
        } u;
} Czt_ValueRLFInd_IEsCls;
typedef struct x2apProtIE_Field_RLFInd_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueRLFInd_IEsCls value;
} CztProtIE_Field_RLFInd_IEs;

typedef struct x2apProtIE_Cont_RLFInd_IEs
{
        TknU16 noComp;
        CztProtIE_Field_RLFInd_IEs *member;
}CztProtIE_Cont_RLFInd_IEs;

typedef struct x2apRLFInd
{
        TknPres pres;
        CztProtIE_Cont_RLFInd_IEs protocolIEs;
} CztRLFInd;

typedef struct x2ap_ExtnSrvdCellsToActivate_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnSrvdCellsToActivate_Item_ExtIEsCls;
typedef struct x2apProtExtnField_SrvdCellsToActivate_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnSrvdCellsToActivate_Item_ExtIEsCls extensionValue;
} CztProtExtnField_SrvdCellsToActivate_Item_ExtIEs;

typedef struct x2apProtExtnCont_SrvdCellsToActivate_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_SrvdCellsToActivate_Item_ExtIEs *member;
}CztProtExtnCont_SrvdCellsToActivate_Item_ExtIEs;

typedef struct x2apSrvdCellsToActivate_Item
{
        TknPres pres;
        CztECGI ecgi;
        CztProtExtnCont_SrvdCellsToActivate_Item_ExtIEs iE_Extns;
} CztSrvdCellsToActivate_Item;

typedef struct x2apSrvdCellsToActivate
{
        TknU16 noComp;
        CztSrvdCellsToActivate_Item *member;
}CztSrvdCellsToActivate;

typedef struct x2ap_ValueCellActvnRqst_IEsCls{
        union {
                CztSrvdCellsToActivate cztSrvdCellsToActivate;
        } u;
} Czt_ValueCellActvnRqst_IEsCls;
typedef struct x2apProtIE_Field_CellActvnRqst_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueCellActvnRqst_IEsCls value;
} CztProtIE_Field_CellActvnRqst_IEs;

typedef struct x2apProtIE_Cont_CellActvnRqst_IEs
{
        TknU16 noComp;
        CztProtIE_Field_CellActvnRqst_IEs *member;
}CztProtIE_Cont_CellActvnRqst_IEs;

typedef struct x2apCellActvnRqst
{
        TknPres pres;
        CztProtIE_Cont_CellActvnRqst_IEs protocolIEs;
} CztCellActvnRqst;

typedef struct x2ap_ExtnActivatedCellLst_Item_ExtIEsCls{
        union {
                TknU8 unused;
        } u;
} Czt_ExtnActivatedCellLst_Item_ExtIEsCls;
typedef struct x2apProtExtnField_ActivatedCellLst_Item_ExtIEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ExtnActivatedCellLst_Item_ExtIEsCls extensionValue;
} CztProtExtnField_ActivatedCellLst_Item_ExtIEs;

typedef struct x2apProtExtnCont_ActivatedCellLst_Item_ExtIEs
{
        TknU16 noComp;
        CztProtExtnField_ActivatedCellLst_Item_ExtIEs *member;
}CztProtExtnCont_ActivatedCellLst_Item_ExtIEs;

typedef struct x2apActivatedCellLst_Item
{
        TknPres pres;
        CztECGI ecgi;
        CztProtExtnCont_ActivatedCellLst_Item_ExtIEs iE_Extns;
} CztActivatedCellLst_Item;

typedef struct x2apActivatedCellLst
{
        TknU16 noComp;
        CztActivatedCellLst_Item *member;
}CztActivatedCellLst;

typedef struct x2ap_ValueCellActvnResp_IEsCls{
        union {
                CztActivatedCellLst cztActivatedCellLst;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueCellActvnResp_IEsCls;
typedef struct x2apProtIE_Field_CellActvnResp_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueCellActvnResp_IEsCls value;
} CztProtIE_Field_CellActvnResp_IEs;

typedef struct x2apProtIE_Cont_CellActvnResp_IEs
{
        TknU16 noComp;
        CztProtIE_Field_CellActvnResp_IEs *member;
}CztProtIE_Cont_CellActvnResp_IEs;

typedef struct x2apCellActvnResp
{
        TknPres pres;
        CztProtIE_Cont_CellActvnResp_IEs protocolIEs;
} CztCellActvnResp;

typedef struct x2ap_ValueCellActvnFail_IEsCls{
        union {
                CztCause cztCause;
                CztCriticalityDiag cztCriticalityDiag;
        } u;
} Czt_ValueCellActvnFail_IEsCls;
typedef struct x2apProtIE_Field_CellActvnFail_IEs
{
        TknPres pres;
        CztProtIE_ID id;
        CztCriticality criticality;
        Czt_ValueCellActvnFail_IEsCls value;
} CztProtIE_Field_CellActvnFail_IEs;

typedef struct x2apProtIE_Cont_CellActvnFail_IEs
{
        TknU16 noComp;
        CztProtIE_Field_CellActvnFail_IEs *member;
}CztProtIE_Cont_CellActvnFail_IEs;

typedef struct x2apCellActvnFail
{
        TknPres pres;
        CztProtIE_Cont_CellActvnFail_IEs protocolIEs;
} CztCellActvnFail;

typedef struct x2ap_InitiatingMsgCls{
        union {
                CztHovrRqst cztHovrRqst;
                CztResetRqst cztResetRqst;
                CztX2SetupRqst cztX2SetupRqst;
                CztResStatusRqst cztResStatusRqst;
                CztENBConfigUpd cztENBConfigUpd;
                CztMobilityChangeRqst cztMobilityChangeRqst;
                CztCellActvnRqst cztCellActvnRqst;
                CztSNStatusTfr cztSNStatusTfr;
                CztUECntxtRls cztUECntxtRls;
                CztHovrCancel cztHovrCancel;
                CztErrInd cztErrInd;
                CztResStatusUpd cztResStatusUpd;
                CztLoadInform cztLoadInform;
                CztPrivMsg cztPrivMsg;
                CztRLFInd cztRLFInd;
                CztHovrReport cztHovrReport;
        } u;
} Czt_InitiatingMsgCls;
typedef struct x2apInitiatingMsg
{
        TknPres pres;
        CztProcedureCode procedureCode;
        CztCriticality criticality;
        Czt_InitiatingMsgCls value;
} CztInitiatingMsg;

typedef struct x2ap_SuccessfulOutcomeCls{
        union {
                CztHovrRqstAckg cztHovrRqstAckg;
                CztResetResp cztResetResp;
                CztX2SetupResp cztX2SetupResp;
                CztResStatusResp cztResStatusResp;
                CztENBConfigUpdAckg cztENBConfigUpdAckg;
                CztMobilityChangeAckg cztMobilityChangeAckg;
                CztCellActvnResp cztCellActvnResp;
        } u;
} Czt_SuccessfulOutcomeCls;
typedef struct x2apSuccessfulOutcome
{
        TknPres pres;
        CztProcedureCode procedureCode;
        CztCriticality criticality;
        Czt_SuccessfulOutcomeCls value;
} CztSuccessfulOutcome;

typedef struct x2ap_UnsuccessfulOutcomeCls{
        union {
                CztHovrPrepFail cztHovrPrepFail;
                CztX2SetupFail cztX2SetupFail;
                CztResStatusFail cztResStatusFail;
                CztENBConfigUpdFail cztENBConfigUpdFail;
                CztMobilityChangeFail cztMobilityChangeFail;
                CztCellActvnFail cztCellActvnFail;
        } u;
} Czt_UnsuccessfulOutcomeCls;
typedef struct x2apUnsuccessfulOutcome
{
        TknPres pres;
        CztProcedureCode procedureCode;
        CztCriticality criticality;
        Czt_UnsuccessfulOutcomeCls value;
} CztUnsuccessfulOutcome;

typedef struct x2apX2AP_PDU
{
        TknU8 choice;
        union {
                CztInitiatingMsg initiatingMsg;
                CztSuccessfulOutcome successfulOutcome;
                CztUnsuccessfulOutcome unsuccessfulOutcome;
        } val;
} CztX2AP_PDU;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

/**********************************************************************
        End of file:     czt_asn.x@@/main/2 - Thu Aug 25 17:58:13 2011
**********************************************************************/

/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       ad        1. LTE X2AP Initial Release.
/main/2      ---      czt_asn_x_001.main_1  pt    1. Updated for release of X2AP 3.1.
*********************************************************************91*/
