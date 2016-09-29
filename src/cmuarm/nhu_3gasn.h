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

     Type:     Header file

     Desc:     Defines required by RRC

     File:     nhu_3gasn.h

     Sid:      nhu_3gasn.h@@/main/1 - Fri Dec 11 18:45:13 2009

     Prg:      chebli

**********************************************************************/

#ifndef __NHU3GASNH__
#define __NHU3GASNH__ 

#define NHUCHKUNPKPTR(func, val, ptr, mBuf) {\
   S16 ret;\
   if ((ret = func(val, mBuf, ptr)) != ROK)\
      RETVALUE(ret);\
}
#define Nhuid_Invalid 0xffffffff

#define SUPPORTFORRFC2507_NOTSUPP 0
#define SUPPORTFORRFC2507_SUPPORTED 1

#define TURBOSUPPORT_NOTSUPP 0
#define TURBOSUPPORT_SUPPORTED 1

#define MODESPECIFICINFO_FDD 0
#define MODESPECIFICINFO_TDD 1

#define SIMULTANEOUSSCCPCH_DPCH_RECEPTION_NOTSUPP 0
#define SIMULTANEOUSSCCPCH_DPCH_RECEPTION_SUPPORTED 1

#define OTHER_NOTSTORED 0
#define OTHER_STOREDWITHDIFFERENTVALUETAG 1
#define PREDEFINEDCONFIGSTATUSINFO_STOREDWITHVALUETAGSAMEASPREVIUS 0
#define PREDEFINEDCONFIGSTATUSINFO_OTHER 1
#define FDD_NOTSUPP 0
#define FDD_SUPPORTED 1
#define TDD384_RF_CAP_NOTSUPP 0
#define TDD384_RF_CAP_SUPPORTED 1
#define TDD128_RF_CAP_NOTSUPP 0
#define TDD128_RF_CAP_SUPPORTED 1
#define FDD_EDCH_SUPPORTED 0
#define FDD_EDCH_UNSUPPORTED 1

#define PREDEFINEDCONFIGSTATUSLST_ABSENT 0
#define PREDEFINEDCONFIGSTATUSLST_PRESENT 1
#define UE_SECINFO_ABSENT 0
#define UE_SECINFO_PRESENT 1
#define UE_CAPCONT_ABSENT 0
#define UE_CAPCONT_PRESENT 1
#define V390NONCRITICALEXTNS_ABSENT 0
#define V390NONCRITICALEXTNS_PRESENT 1
#define TDD_EDCH_SUPPORTED 0
#define TDD_EDCH_UNSUPPORTED 1
#define NhumaxPredefConfig 16
#define NhumaxFreqBandsFDD 8
#define NhumaxFreqBandsFDD_ext 15
#define NhumaxFreqBandsTDD 4
#define NhumaxFreqBandsGSM 16

#endif

/**********************************************************************
         End of file:     nhu_3gasn.h@@/main/1 - Fri Dec 11 18:45:13 2009
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---    ch        1. RRC LTE Initial Release.
*********************************************************************91*/
