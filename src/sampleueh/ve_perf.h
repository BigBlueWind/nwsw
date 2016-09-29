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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     Hash defines 

     File:     ve_perf.h

     Sid:      ve_perf.h@@/main/2 - Wed Dec  1 08:44:27 2010

     Prg:      rbabu

*********************************************************************21*/


/* This only for LTE eNB performance Measurement related */

#ifndef __VE_PERFH__
#define __VE_PERFH__

#define VE_PERF_NO_DRBS 1
#define VE_PERF_ERAB_ID 1
#define VE_PERF_ERAB_QCI 1
#define VE_PERF_MBR_DL   10000
#define VE_PERF_MBR_UL   2000
#define VE_PERF_GBR_DL   10000
#define VE_PERF_GBR_UL   2000

#define VE_PERF_AGGR_DL   10000
#define VE_PERF_AGGR_UL   2000

#define VE_POLL_BYTE_KB8000 8000

/* This value should be EVTEUDELEGTPTUNEL+1 */
/* But selecting bigger value */
#define VE_NOTIFY_SATRT_DATA   88

#define  VE_PERF_LOAD_TMR_VAL  2

#define  VE_PERF_TMR_EVNT_LOAD    1
#define  VE_PERF_MAX_TMR      1
#define  VE_PERF_MAX_UE       1

#define VE_PERF_PDCP_SAP_ID   2
#define VE_PERF_DRB_ID        1

#define VE_PERF_SDU_SIZE      1450
#define VE_PERF_NO_SDUS      10 

#endif


/********************************************************************30**
  
         End of file:     ve_perf.h@@/main/2 - Wed Dec  1 08:44:27 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
