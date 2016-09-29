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


/********************************************************************20**
  
        Name:    Common LTE
    
        Type:    C include file
  
        Desc:    This file Contains the Data structures for Common LTE
 
        File:    cm_lte.h

        Sid:      cm_lte.h@@/main/2 - Fri Nov 13 14:09:17 2009
  
        Prg:     chakrapani
  
*********************************************************************21*/

#ifndef __CM_LTE_H__
#define __CM_LTE_H__

#ifdef __cplusplus
EXTERN "C" {
#endif  /*__cplusplus*/

/** @file cm_lte.h
    @brief CM_LTE Interface File (cm_lte.h)
*/
 
#define CM_LTE_MAX_LOGCH_PER_RB 2   /*!< Maximum Logical Channel per RB */

/* Mode of RLC entity */
#define CM_LTE_MODE_TM        1     /*!< TM mode */
#define CM_LTE_MODE_UM        2     /*!< UM mode */
#define CM_LTE_MODE_AM        3     /*!< AM mode */

/* Logical Channel Type */
#define CM_LTE_LCH_BCCH      1      /*!< BCCH Logical Channel */
#define CM_LTE_LCH_PCCH      2      /*!< PCCH Logical Channel */
#define CM_LTE_LCH_CCCH      3      /*!< CCCH Logical Channel */
#define CM_LTE_LCH_DTCH      4      /*!< DTCH Logical Channel */
#define CM_LTE_LCH_DCCH      5      /*!< DCCH Logical Channel */

/* Transport Channel Type */
#define CM_LTE_TRCH_BCH      1      /*!< BCH Transport Channel */ 
#define CM_LTE_TRCH_PCH      2      /*!< PCH Transport Channel */
#define CM_LTE_TRCH_DL_SCH   3      /*!< DL-SCH Transport Channel */
#define CM_LTE_TRCH_RACH     4      /*!< RACH Transport Channel */
#define CM_LTE_TRCH_UL_SCH   5      /*!< UL-SCH Transport Channel */

#define CM_LTE_DIR_UL        1      /*!< Uplink Direction */
#define CM_LTE_DIR_DL        2      /*!< Downlink Direction */

#define CM_LTE_SRB           0      /*!< Signalling Radio Bearer */
#define CM_LTE_DRB           1      /*!< Data Radio Bearer */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CM_LTE_H__ */

/********************************************************************30**
  
         End of file:     cm_lte.h@@/main/2 - Fri Nov 13 14:09:17 2009
  
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************90**

     ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      cp   1. initial release.
*********************************************************************91*/
