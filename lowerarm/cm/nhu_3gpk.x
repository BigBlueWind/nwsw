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

     Name:     LTE RRC Layer

     Type:     C include file

     Desc:     Defines required by RRC

     File:     nhu_3gpk.x 

     Sid:      nhu_3gpk.x@@/main/1 - Fri Dec 11 18:45:14 2009

     Prg:      chebli

**********************************************************************/

#ifndef __NHU3GPKX__
#define __NHU3GPKX__ 


EXTERN S16 cmPkNhuPDCP_CapsupportForRfc2507 ARGS ((NhuPDCP_CapsupportForRfc2507 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPDCP_Cap ARGS ((NhuPDCP_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRLC_Cap ARGS ((NhuRLC_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuTurboSupport ARGS ((NhuTurboSupport *param, Buffer *mBuf));
EXTERN S16 cmPkNhuDL_TransChCap ARGS ((NhuDL_TransChCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUL_TransChCapmodeSpecificInfotdd ARGS ((NhuUL_TransChCapmodeSpecificInfotdd *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUL_TransChCapmodeSpecificInfo ARGS ((NhuUL_TransChCapmodeSpecificInfo *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUL_TransChCap ARGS ((NhuUL_TransChCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuTportChannelCap ARGS ((NhuTportChannelCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CapfddRF_Cap ARGS ((NhuRF_CapfddRF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CaptddRF_Cap ARGS ((NhuRF_CaptddRF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_Cap ARGS ((NhuRF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuSimultaneousSCCPCH_DPCH_Receptionsupported ARGS ((NhuSimultaneousSCCPCH_DPCH_Receptionsupported *param, Buffer *mBuf));
EXTERN S16 cmPkNhuSimultaneousSCCPCH_DPCH_Reception ARGS ((NhuSimultaneousSCCPCH_DPCH_Reception *param, Buffer *mBuf));
EXTERN S16 cmPkNhuDL_PhysChCapFDD ARGS ((NhuDL_PhysChCapFDD *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUL_PhysChCapFDD ARGS ((NhuUL_PhysChCapFDD *param, Buffer *mBuf));
EXTERN S16 cmPkNhuDL_PhysChCapTDD ARGS ((NhuDL_PhysChCapTDD *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUL_PhysChCapTDD ARGS ((NhuUL_PhysChCapTDD *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCapfddPhysChCap ARGS ((NhuPhysicalChannelCapfddPhysChCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCaptddPhysChCap ARGS ((NhuPhysicalChannelCaptddPhysChCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap ARGS ((NhuPhysicalChannelCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuMultiRAT_Cap ARGS ((NhuMultiRAT_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_MultiModeRAT_Cap ARGS ((NhuUE_MultiModeRAT_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuSecCap ARGS ((NhuSecCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_Positioning_Cap ARGS ((NhuUE_Positioning_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuGSM_Measurements ARGS ((NhuGSM_Measurements *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCap ARGS ((NhuCompressedModeMeasCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuMeasurementCap ARGS ((NhuMeasurementCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCap ARGS ((NhuUE_RadioAccessCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabFDD ARGS ((NhuCompressedModeMeasCapabFDD *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabFDDLst ARGS ((NhuCompressedModeMeasCapabFDDLst *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabTDD ARGS ((NhuCompressedModeMeasCapabTDD *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabTDDLst ARGS ((NhuCompressedModeMeasCapabTDDLst *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabGSM ARGS ((NhuCompressedModeMeasCapabGSM *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabGSMLst ARGS ((NhuCompressedModeMeasCapabGSMLst *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabMC ARGS ((NhuCompressedModeMeasCapabMC *param, Buffer *mBuf));
EXTERN S16 cmPkNhuMeasurementCapExt ARGS ((NhuMeasurementCapExt *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDDfddRF_Cap ARGS ((NhuUE_RadioAccessCapabBandFDDfddRF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDD ARGS ((NhuUE_RadioAccessCapabBandFDD *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDDLst ARGS ((NhuUE_RadioAccessCapabBandFDDLst *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCap_v370ext ARGS ((NhuUE_RadioAccessCap_v370ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANUE_RadioAccessCapInfo ARGS ((NhuUTRANUE_RadioAccessCapInfo *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigStatusInfoother ARGS ((NhuPredefinedConfigStatusInfoother *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigStatusInfo ARGS ((NhuPredefinedConfigStatusInfo *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigStatusLst ARGS ((NhuPredefinedConfigStatusLst *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_SecInfo ARGS ((NhuUE_SecInfo *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_PositioningCapExt_v380 ARGS ((NhuUE_PositioningCapExt_v380 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCap_v380ext ARGS ((NhuUE_RadioAccessCap_v380ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuDL_PhysChCapFDD_v380ext ARGS ((NhuDL_PhysChCapFDD_v380ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v390ext_IEs ARGS ((NhuInterRATHovrInfo_v390ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_PositioningCapExt_v3a0 ARGS ((NhuUE_PositioningCapExt_v3a0 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCap_v3a0ext ARGS ((NhuUE_RadioAccessCap_v3a0ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v3a0ext_IEs ARGS ((NhuInterRATHovrInfo_v3a0ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v3d0ext_IEs ARGS ((NhuInterRATHovrInfo_v3d0ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_PositioningCapExt_v3g0 ARGS ((NhuUE_PositioningCapExt_v3g0 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCap_v3g0ext ARGS ((NhuUE_RadioAccessCap_v3g0ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v3g0ext_IEs ARGS ((NhuInterRATHovrInfo_v3g0ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v4b0ext_IEs ARGS ((NhuInterRATHovrInfo_v4b0ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v4d0ext_IEs ARGS ((NhuInterRATHovrInfo_v4d0ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigValueTagLst ARGS ((NhuPredefinedConfigValueTagLst *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigSetWithDifferentValueTag ARGS ((NhuPredefinedConfigSetWithDifferentValueTag *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigSetsWithDifferentValueTag ARGS ((NhuPredefinedConfigSetsWithDifferentValueTag *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigStatusLstVarSz ARGS ((NhuPredefinedConfigStatusLstVarSz *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPredefinedConfigStatusLstComp ARGS ((NhuPredefinedConfigStatusLstComp *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CapabBandLstFDDComp ARGS ((NhuRF_CapabBandLstFDDComp *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CapCompfdd ARGS ((NhuRF_CapCompfdd *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CapComptdd384_RF_Cap ARGS ((NhuRF_CapComptdd384_RF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CapComptdd128_RF_Cap ARGS ((NhuRF_CapComptdd128_RF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CapComp ARGS ((NhuRF_CapComp *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapComp ARGS ((NhuUE_RadioAccessCapComp *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v590ext_IEs ARGS ((NhuInterRATHovrInfo_v590ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_SecInfo2 ARGS ((NhuUE_SecInfo2 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_CapabBandLstFDDComp_ext ARGS ((NhuRF_CapabBandLstFDDComp_ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapComp_ext ARGS ((NhuUE_RadioAccessCapComp_ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupported ARGS ((NhuPhysicalChannelCap_edch_r6fdd_edchsupported *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_edch_r6fdd_edch ARGS ((NhuPhysicalChannelCap_edch_r6fdd_edch *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_edch_r6 ARGS ((NhuPhysicalChannelCap_edch_r6 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch ARGS ((NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapComp2 ARGS ((NhuUE_RadioAccessCapComp2 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v690ext_IEs ARGS ((NhuInterRATHovrInfo_v690ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v6b0ext_IEs ARGS ((NhuInterRATHovrInfo_v6b0ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst ARGS ((NhuUTRANInterRATHovrInfopredefinedConfigStatusLst *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfouE_SecInfo ARGS ((NhuUTRANInterRATHovrInfouE_SecInfo *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfoue_CapCont ARGS ((NhuUTRANInterRATHovrInfoue_CapCont *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtns ARGS ((NhuUTRANInterRATHovrInfov390NonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUTRANInterRATHovrInfo ARGS ((NhuUTRANInterRATHovrInfo *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabFDD2 ARGS ((NhuCompressedModeMeasCapabFDD2 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabFDDLst2 ARGS ((NhuCompressedModeMeasCapabFDDLst2 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuMeasurementCapExt2 ARGS ((NhuMeasurementCapExt2 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap ARGS ((NhuUE_RadioAccessCapabBandFDD2fddRF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDD2 ARGS ((NhuUE_RadioAccessCapabBandFDD2 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDDLst2 ARGS ((NhuUE_RadioAccessCapabBandFDDLst2 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabFDD_ext ARGS ((NhuCompressedModeMeasCapabFDD_ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuCompressedModeMeasCapabFDDLst_ext ARGS ((NhuCompressedModeMeasCapabFDDLst_ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDD_ext ARGS ((NhuUE_RadioAccessCapabBandFDD_ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCapabBandFDDLst_ext ARGS ((NhuUE_RadioAccessCapabBandFDDLst_ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCap_v650ext ARGS ((NhuUE_RadioAccessCap_v650ext *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_v690ext1_IEs ARGS ((NhuInterRATHovrInfo_v690ext1_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns ARGS ((NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns *param, Buffer *mBuf));
EXTERN S16 cmPkNhuInterRATHovrInfo_r3_add_ext_IEs ARGS ((NhuInterRATHovrInfo_r3_add_ext_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_Cap_r7fddRF_Cap ARGS ((NhuRF_Cap_r7fddRF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_Cap_r7tddRF_Cap ARGS ((NhuRF_Cap_r7tddRF_Cap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuRF_Cap_r7 ARGS ((NhuRF_Cap_r7 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuDL_PhysChCapTDD_768 ARGS ((NhuDL_PhysChCapTDD_768 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_r7fddPhysChCap ARGS ((NhuPhysicalChannelCap_r7fddPhysChCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_r7tddPhysChCap ARGS ((NhuPhysicalChannelCap_r7tddPhysChCap *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_r7tddPhysChCap_768 ARGS ((NhuPhysicalChannelCap_r7tddPhysChCap_768 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_r7 ARGS ((NhuPhysicalChannelCap_r7 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported ARGS ((NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edch ARGS ((NhuPhysicalChannelCap_tddedch_r7tdd_edch *param, Buffer *mBuf));
EXTERN S16 cmPkNhuPhysicalChannelCap_tddedch_r7 ARGS ((NhuPhysicalChannelCap_tddedch_r7 *param, Buffer *mBuf));
EXTERN S16 cmPkNhuUE_RadioAccessCap_v7xyext ARGS ((NhuUE_RadioAccessCap_v7xyext *param, Buffer *mBuf));
#endif

/**********************************************************************
         End of file:     nhu_3gpk.x@@/main/1 - Fri Dec 11 18:45:14 2009
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---    ch        1. RRC LTE Initial Release.
*********************************************************************91*/
