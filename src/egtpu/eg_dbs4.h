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


/***********************************************************************

    Name: Header file for defines for LM configuration

    Type: C Source

    Desc: 

    File: eg_dbs4.h

    Sid:      eg_dbs4.h@@/main/2 - Fri Feb  4 18:18:06 2011

    Prg: an

************************************************************************/

#ifdef EGTP_C

/* Message Codes   */

#define EG_S4_BRERESCMD_CMD_CODE       68        
#define EG_S4_BRERESCMD_NMB_IES        13        
#define EG_S4_BRERESFLRIND_CMD_CODE    69        
#define EG_S4_BRERESFLRIND_NMB_IES     5         
#define EG_S4_CHGNTFREQ_CMD_CODE       38        
#define EG_S4_CHGNTFREQ_NMB_IES        6         
#define EG_S4_CHGNTFRSP_CMD_CODE       39        
#define EG_S4_CHGNTFRSP_NMB_IES        4         
#define EG_S4_CRTBREREQ_CMD_CODE       95        
#define EG_S4_CRTBREREQ_NMB_IES        6         
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 7         
#define EG_S4_CRTBRERSP_CMD_CODE       96        
#define EG_S4_CRTBRERSP_NMB_IES        7         
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 6         
#define EG_S4_CRTINDDATFWDTUNREQ_CMD_CODE 166       
#define EG_S4_CRTINDDATFWDTUNREQ_NMB_IES 5         
#define EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_S4_CRTINDDATFWDTUNRSP_CMD_CODE 167       
#define EG_S4_CRTINDDATFWDTUNRSP_NMB_IES 5         
#define EG_CRTINDDATFWDTUNRSP_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_S4_CRTSESREQ_CMD_CODE       32        
#define EG_S4_CRTSESREQ_NMB_IES        23        
#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 6         
#define EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S4_CRTSESRSP_CMD_CODE       33        
#define EG_S4_CRTSESRSP_NMB_IES        13        
#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 8         
#define EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S4_DELBRECMD_CMD_CODE       66        
#define EG_S4_DELBRECMD_NMB_IES        2         
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 1         
#define EG_S4_DELBREFLRIND_CMD_CODE    67        
#define EG_S4_DELBREFLRIND_NMB_IES     4         
#define EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S4_DELBREREQ_CMD_CODE       99        
#define EG_S4_DELBREREQ_NMB_IES        6         
#define EG_S4_DELBRERSP_CMD_CODE       100       
#define EG_S4_DELBRERSP_NMB_IES        8         
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S4_DELINDDATFWDTUNREQ_CMD_CODE 168       
#define EG_S4_DELINDDATFWDTUNREQ_NMB_IES 1         
#define EG_S4_DELINDDATFWDTUNRSP_CMD_CODE 169       
#define EG_S4_DELINDDATFWDTUNRSP_NMB_IES 3         
#define EG_S4_DELSESREQ_CMD_CODE       36        
#define EG_S4_DELSESREQ_NMB_IES        7         
#define EG_S4_DELSESRSP_CMD_CODE       37        
#define EG_S4_DELSESRSP_NMB_IES        4         
#define EG_S4_DWLNKDATNOT_CMD_CODE     176       
#define EG_S4_DWLNKDATNOT_NMB_IES      2         
#define EG_S4_DWLNKDATNOTACK_CMD_CODE  177       
#define EG_S4_DWLNKDATNOTACK_NMB_IES   4         
#define EG_S4_DWLNKDATNOTFLRIND_CMD_CODE 70        
#define EG_S4_DWLNKDATNOTFLRIND_NMB_IES 2         
#define EG_S4_MODBRECMD_CMD_CODE       64        
#define EG_S4_MODBRECMD_NMB_IES        3         
#define EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S4_MODBREFLRIND_CMD_CODE    65        
#define EG_S4_MODBREFLRIND_NMB_IES     3         
#define EG_S4_MODBREREQ_CMD_CODE       34        
#define EG_S4_MODBREREQ_NMB_IES        10        
#define EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S4_MODBRERSP_CMD_CODE       35        
#define EG_S4_MODBRERSP_NMB_IES        12        
#define EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S4_RELACCBREREQ_CMD_CODE    170       
#define EG_S4_RELACCBREREQ_NMB_IES     2         
#define EG_S4_RELACCBRERSP_CMD_CODE    171       
#define EG_S4_RELACCBRERSP_NMB_IES     3         
#define EG_S4_STOPPAGINGIND_CMD_CODE   73        
#define EG_S4_STOPPAGINGIND_NMB_IES    1         
#define EG_S4_TRACESESSACTV_CMD_CODE   71        
#define EG_S4_TRACESESSACTV_NMB_IES    3         
#define EG_S4_TRACESESSDEACTV_CMD_CODE 72        
#define EG_S4_TRACESESSDEACTV_NMB_IES  1         
#define EG_S4_UPDBREREQ_CMD_CODE       97        
#define EG_S4_UPDBREREQ_NMB_IES        6         
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_S4_UPDBRERSP_CMD_CODE       98        
#define EG_S4_UPDBRERSP_NMB_IES        8         
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 4         

#endif

/********************************************************************30**

         End of file:     eg_dbs4.h@@/main/2 - Fri Feb  4 18:18:06 2011

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
/main/1      ---      an                1. Initial Release
/main/1      ---      kchaitanya        1. Initial for eGTP 1.2 Release
/main/2      ---      pmacharla       Initial Release of eGTP 2.1 release
*********************************************************************91*/

