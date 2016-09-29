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

     Name:     LTE-CL Layer 
  
     Type:     C souce file
  
     Desc:     SCL for Trillium to Silicon Rx Mapping. 

     File:     ys_rxmap.c 

     Sid:      ys_dft.c@@/main/1 - Thu Dec  2 02:27:13 2010

     Prg:      mraj

**********************************************************************/

#ifndef __YS_DFT_PARAMS_H__
#define __YS_DFT_PARAMS_H__

#define YS_REPEAT_CYCLE      1
#define YS_PERSIST_ENABLE    1

#define ACKNACKREPT          0

#define NOMPDSCHRSEPROFF     0

#define YS_CODING_DESC       TURBOCDR
#define YS_BLK_CODE_CAT      1
#define YS_CRC_SCRAMBLING    0
#define YS_SINGLE_MAPPING    1
#define YS_TX_PWR_CNTRL      0
#define YS_HALF_ITERATIONS  16

/* PUCCH Definitions */
#define YS_PUCCH_CRC_LEN     0
#define YS_PUCCH_COD_DES     0
#define YS_PUCCH_BLK_CAT     0
#define YS_PUCCH_MCS_TYP     0

#endif /* __YS_DFT_PARAMS_H__ */

/********************************************************************30**
  
         End of file:     ys_dft.c@@/main/1 - Thu Dec  2 02:27:13 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
 
**********************************************************************/
 
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sgm                   1. eNodeB 1.2 release
*********************************************************************91*/
