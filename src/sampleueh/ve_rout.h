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

     Name:    LTE enodeB App(PCAP) Defines 

     Type:    C Include file

     Desc:    This file contains the eNodeB APP defines code

     File:    ve_pcap.h

     Sid:      ve_rout.h@@/main/1 - Wed Dec  1 08:44:29 2010

     Prg:     sv 
*********************************************************************21*/

#ifndef __VE_ROUTH__
#define __VE_ROUTH__

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#ifdef VE_LNX_PCAP_PERF

#define VE_APP_ROUT_IP_ADDR_LEN             4
#define VE_APP_ROUT_MAX_ETH_PKT_LEN         1600
#define VE_APP_ROUT_MAX_ARP_PKT_LEN         60
#define VE_APP_ROUT_MAC_ADDR_LEN            6
#define VE_APP_ROUT_MIN_ETH_PKT_SIZE        64
#define VE_APP_ROUT_MAX_DATA_SIZE           1500

#define VE_APP_ROUT_PCAP_BUF_SIZE           65536
#define VE_APP_ROUT_PCAP_TIMEOUT            10    /* In milli second */
#define VE_APP_ROUT_PCAP_MAX_DEV_LEN        16
#define VE_APP_PRIMARY_GW_ID                0
#define VE_APP_MAX_NW_NODES                 32

#define VE_ROUT_RX_THRD_CORE                0 /* Receive threda core num */

/* Changing the value of maximum no of UEs to support performance setup */
#define VE_APP_PRIMARY_GW_IDX 0

#define VE_APP_CHRGNG_ID 1

#define VE_APP_CMD1 "netstat -rn > gateway.txt"
#define VE_APP_CMD2 "rm -f gateway.txt"
#define VE_APP_FILENAME "gateway.txt"
#define VE_APP_DEFAULT "0.0.0.0"

#endif /* VE_LNX_PCAP_PERF */

#ifdef __cplusplus
}
#endif
#endif /* __VE_ROUTH__ */
/**********************************************************************
  
         End of file:     ve_rout.h@@/main/1 - Wed Dec  1 08:44:29 2010
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
