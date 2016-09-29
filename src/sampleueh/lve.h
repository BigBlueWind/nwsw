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

     Name:     LTE-eNodeB APP
  
     Type:    
  
     Desc:     

     Ret :     ROK - success
               RFAILED - failure

     File:     lve.h

     Sid:      lve.h@@/main/3 - Fri Mar  4 12:44:46 2011

     Prg:  

**********************************************************************/

#ifndef __LVEH__
#define __LVEH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Interface Events */
#define EVTLVECFGREQ    1       /* Config request */
#define EVTLVECFGCFM    2       /* Config Confirm */
#define EVTLVECNTRLREQ  3       /* Control Request */
#define EVTLVECNTRLCFM  4       /* Control Confirm */
#define EVTLVESTAIND    5       /* Status Indication */

#define EVEXXX 0
   


/**********************************************************************
 cause field values in cmAlarm
**********************************************************************/
#define LVE_CAUSE_NHU_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 1)  
#define LVE_CAUSE_CTF_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 2)  
#define LVE_CAUSE_RGR_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 3)  
#define LVE_CAUSE_SZT_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 4)  
#define LVE_CAUSE_EGT_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 5)  
#define LVE_CAUSE_PJU_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 6)  

#define LVE_EVENT_CELL_CFG_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 1)  
#define LVE_EVENT_CELL_CFG_FAILED        (LCM_CAUSE_LYR_SPECIFIC + 2)  
#define LVE_EVENT_S1CON_SET_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 3)  
#define LVE_EVENT_S1CON_SET_FAIL        (LCM_CAUSE_LYR_SPECIFIC + 4)  



   
/*----------------- State Of SAP ------------------*/

#define LVE_SAP_UNBOUND             1
#define LVE_SAP_BINDING             2
#define LVE_SAP_BOUND               3

#define ELVEXXX         0

#define STVEGEN            1

#define STVERGRSAP         2
#define STVENHUSAP         3
#define STVECTFSAP         4
#define STVESZTSAP         5
#define STVECELLCFG        6
#define STVES1CONSETUP     7


#define STVEDRBCFGDONE     99 /* DRB configuration is completed */

/* operations */
#define LVE_CELLCONFIG  1

#define LVE_USTA_DGNVAL_SAP  0
#define LVE_USTA_DGNVAL_CELLUEID  1

#ifdef EU_DAT_APP

#define STVEEGTSAP         8
#define STVEPJUSAP         9

#endif

#define LVE_CAUSE_CELL_UP  100
/* operations */
#define LVE_CELLCONFIG  1

/* lve_h_001.main_2: Added maximum file path */
#ifdef VE_SM_LOG_TO_FILE
#define LVE_MAX_FILE_PATH 256
#endif

#define ELVEXXX         0

#define ERRLVE          0

#define   ELVE001      (ERRLVE +    1)
#define   ELVE002      (ERRLVE +    2)
#define   ELVE003      (ERRLVE +    3)
#define   ELVE004      (ERRLVE +    4)
#define   ELVE005      (ERRLVE +    5)
#define   ELVE006      (ERRLVE +    6)
#define   ELVE007      (ERRLVE +    7)
#define   ELVE008      (ERRLVE +    8)
#define   ELVE009      (ERRLVE +    9)
#define   ELVE010      (ERRLVE +    10)
#define   ELVE011      (ERRLVE +    11)
#define   ELVE012      (ERRLVE +    12)
#define   ELVE013      (ERRLVE +    13)
#define   ELVE014      (ERRLVE +    14)
#define   ELVE015      (ERRLVE +    15)


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __LVEH__ */

/*********************************************************************

         End of file:     lve.h@@/main/3 - Fri Mar  4 12:44:46 2011

**********************************************************************/

/**********************************************************************

        Notes:

**********************************************************************/

/**********************************************************************

**********************************************************************/

/********************************************************************

        Revision history:

**********************************************************************/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/2      ---     aj           Initial release
/main/3      ---      lve_h_001.main_2  rk    1. Support for logging of console 
                                    prints to a file.
*********************************************************************91*/
