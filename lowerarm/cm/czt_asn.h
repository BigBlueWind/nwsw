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

     File:     czt_asn.h

     Sid:      czt_asn.h@@/main/2 - Thu Aug 25 17:55:45 2011

     Prg:      ad

**********************************************************************/
/*czt_asn_h_001.main_1 : This file is generated newly for X2AP 3.1 release. */

#ifndef __CZTASNH__
#define __CZTASNH__ 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CZUCHKUNPKPTR(func, val, ptr, mBuf) {\
   S16 ret;\
   if ((ret = func(val, mBuf, ptr)) != ROK)\
      RETVALUE(ret);\
}
#define Cztid_Invalid 0xffffffff
#define CztmaxPrivIEs 65535
#define CztmaxProtExtns 65535
#define CztmaxProtIEs 65535

#define PRIVIE_ID_LOCAL 0
#define PRIVIE_ID_GLOBAL 1
#define Cztid_handoverPrep 0
#define Cztid_handoverCancel 1
#define Cztid_loadInd 2
#define Cztid_errorInd 3
#define Cztid_snStatusTfr 4
#define Cztid_uECntxtRls 5
#define Cztid_x2Setup 6
#define Cztid_reset 7
#define Cztid_eNBConfigUpd 8
#define Cztid_resourceStatusRprtngInitiation 9
#define Cztid_resourceStatusRprtng 10
/* czt_asn_h_001.main_1 : X2AP 3.1 new messages */
#define Cztid_mobilitySettingsChange 11
#define Cztid_rLFInd 12
#define Cztid_handoverReport 13
#define Cztid_cellActvn 14
#define Cztid_privateMsg 15
#define CztmaxEARFCN 65535
#define CztmaxIntfs 16
#define CztmaxCellineNB 256
#define CztmaxnoofBrs 256
#define CztmaxNrOfErrs 256
#define CztmaxnoofPDCP_SN 16
#define CztmaxnoofEPLMNs 15
#define CztmaxnoofEPLMNsPlusOne 16
#define CztmaxnoofForbLACs 4096
#define CztmaxnoofForbTACs 4096
#define CztmaxnoofBPLMNs 6
#define CztmaxnoofNeighbours 512
#define CztmaxnoofPRBs 110
#define CztmaxPools 16
#define CztmaxnoofCells 16
#define CztmaxnoofMBSFN 8
#define Cztid_E_RABs_Admtd_Item 0
#define Cztid_E_RABs_Admtd_Lst 1
#define Cztid_E_RAB_Item 2
#define Cztid_E_RABs_NotAdmtd_Lst 3
#define Cztid_E_RABs_ToBeSetup_Item 4
#define Cztid_Cause 5
#define Cztid_CellInform 6
#define Cztid_CellInform_Item 7
#define Cztid_New_eNB_UE_X2AP_ID 9
#define Cztid_Old_eNB_UE_X2AP_ID 10
#define Cztid_TgetCell_ID 11
#define Cztid_TgeteNBtoSrc_eNBTprntCont 12
#define Cztid_TraceActvn 13
#define Cztid_UE_CntxtInform 14
#define Cztid_UE_HistoryInform 15
#define Cztid_UE_X2AP_ID 16
#define Cztid_CriticalityDiag 17
#define Cztid_E_RABs_SubjToStatusTfr_Lst 18
#define Cztid_E_RABs_SubjToStatusTfr_Item 19
#define Cztid_SrvdCells 20
#define Cztid_GlobalENB_ID 21
#define Cztid_TimeToWait 22
#define Cztid_GUMMEI_ID 23
#define Cztid_GUGroupIDLst 24
#define Cztid_SrvdCellsToAdd 25
#define Cztid_SrvdCellsToMdfy 26
#define Cztid_SrvdCellsToDelete 27
#define Cztid_Registration_Rqst 28
#define Cztid_CellToReport 29
#define Cztid_RprtngPeriodicity 30
#define Cztid_CellToReport_Item 31
#define Cztid_CellMeasurementResult 32
#define Cztid_CellMeasurementResult_Item 33
#define Cztid_GUGroupIDToAddLst 34
#define Cztid_GUGroupIDToDeleteLst 35
#define Cztid_SRVCCOperationPossible 36
#define Cztid_Measurement_ID 37
#define Cztid_ReportCharacteristics 38
#define Cztid_ENB1_Measurement_ID 39
#define Cztid_ENB2_Measurement_ID 40
#define Cztid_Number_of_Antennaports 41
#define Cztid_CompositeAvailableCapacityGroup 42
#define Cztid_ENB1_Cell_ID 43
#define Cztid_ENB2_Cell_ID 44
#define Cztid_ENB2_Proposed_Mobility_Params 45
#define Cztid_ENB1_Mobility_Params 46
#define Cztid_ENB2_Mobility_Params_Modification_Range 47
#define Cztid_FailCellPCI 48
#define Cztid_Re_establishmentCellECGI 49
#define Cztid_FailCellCRNTI 50
#define Cztid_ShortMAC_I 51
#define Cztid_SrcCellECGI 52
#define Cztid_FailCellECGI 53
#define Cztid_HovrReportTyp 54
#define Cztid_PRACH_Config 55
#define Cztid_MBSFN_Subframe_Info 56
#define Cztid_SrvdCellsToActivate 57
#define Cztid_ActivatedCellLst 58
#define Cztid_DeactivationInd 59
#define Cztid_UE_RLF_Report_Cont 60

#define CAUSE_RADIONW 0
#define CAUSE_TRANSPORT 1
/* czt_asn_h_001.main_2 :  Fix for the build warnings, all changes are guarded
 * under CZTV1 flag, for backward compatability */
#ifdef CZTV1
#define CZT_CAUSE_PROTOCOL 2
#define CZT_CAUSE_MISC 3
#else
#define CAUSE_PROTOCOL 2
#define CAUSE_MISC 3
#endif

#define EUTRA_MODE_INFO_FDD 0
#define EUTRA_MODE_INFO_TDD 1

#define ENB_ID_MACRO_ENB_ID 0
#define ENB_ID_HOME_ENB_ID 1

#define LASTVISITEDGERANCELLINFORM_UNDEFINED 0

#define LASTVISITEDCELL_ITEM_E_UTRAN_CELL 0
#define LASTVISITEDCELL_ITEM_UTRAN_CELL 1
#define LASTVISITEDCELL_ITEM_GERAN_CELL 2

#define SUBFRAMEALLOCN_ONEFRAME 0
#define SUBFRAMEALLOCN_FOURFRAMES 1

#define X2AP_PDU_INITIATINGMSG 0
#define X2AP_PDU_SUCCESSFULOUTCOME 1
#define X2AP_PDU_UNSUCCESSFULOUTCOME 2

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif/*_CZTASNH_*/

/**********************************************************************
        End of file:     czt_asn.h@@/main/2 - Thu Aug 25 17:55:45 2011
**********************************************************************/

/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       ad        1. LTE X2AP Initial Release.
/main/2      ---      czt_asn_h_001.main_1  pt  1. Updated for release of X2AP 3.1.
/main/3      ---      czt_asn_h_001.main_2  akaranth 1.Fix for the build warnings, all changes are guarded
                                                       under CZTV1 flag, for backward compatability. 
*********************************************************************91*/
