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

    File: eg_dbs16.h

    Sid:      eg_dbs16.h@@/main/2 - Fri Feb  4 18:18:04 2011

    Prg: an

************************************************************************/

#ifdef EGTP_C

/* Message Codes   */

#define EG_S16_CTXACK_CMD_CODE         132       
#define EG_S16_CTXACK_NMB_IES          3         
#define EG_S16_CTXREQ_CMD_CODE         130       
#define EG_S16_CTXREQ_NMB_IES          12        
#define EG_S16_CTXRSP_CMD_CODE         131       
#define EG_S16_CTXRSP_NMB_IES          19        
#define EG_CTXRSP_PDN_CONN0_GRP_NMB_IES 12        
#define EG_CTXRSP_BEARER_CNTX00_GRP_NMB_IES 7         
#define EG_S16_FWDACCSCTXACK_CMD_CODE  138       
#define EG_S16_FWDACCSCTXACK_NMB_IES   2         
#define EG_S16_FWDACCSCTXNTF_CMD_CODE  137       
#define EG_S16_FWDACCSCTXNTF_NMB_IES   4         
#define EG_S16_FWDRELCMPACK_CMD_CODE   136       
#define EG_S16_FWDRELCMPACK_NMB_IES    3         
#define EG_S16_FWDRELCMPNTF_CMD_CODE   135       
#define EG_S16_FWDRELCMPNTF_NMB_IES    2         
#define EG_S16_FWDRELREQ_CMD_CODE      133       
#define EG_S16_FWDRELREQ_NMB_IES       28        
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 12        
#define EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES 8        
#define EG_S16_FWDRELRSP_CMD_CODE      134       
#define EG_S16_FWDRELRSP_NMB_IES       13        
#define EG_FWDRELRSP_BEARER_CNTX0_GRP_NMB_IES 8         
#define EG_FWDRELRSP_BEARER_CNTX1_GRP_NMB_IES 8         
#define EG_FWDRELRSP_BEARER_CNTX2_GRP_NMB_IES 8         
#define EG_S16_IDENTREQ_CMD_CODE       128       
#define EG_S16_IDENTREQ_NMB_IES        8         
#define EG_S16_IDENTRSP_CMD_CODE       129       
#define EG_S16_IDENTRSP_NMB_IES        10        
#define EG_S16_RANINFORLY_CMD_CODE     152       
#define EG_S16_RANINFORLY_NMB_IES      3         
#define EG_S16_RELCANCELREQ_CMD_CODE   139       
#define EG_S16_RELCANCELREQ_NMB_IES    3         
#define EG_S16_RELCANCELRSP_CMD_CODE   140       
#define EG_S16_RELCANCELRSP_NMB_IES    2         
#define EG_S16_SUSPNDACK_CMD_CODE      163       
#define EG_S16_SUSPNDACK_NMB_IES       2         
#define EG_S16_SUSPNDNOTFN_CMD_CODE    162       
#define EG_S16_SUSPNDNOTFN_NMB_IES     3         

#endif

/********************************************************************30**

         End of file:     eg_dbs16.h@@/main/2 - Fri Feb  4 18:18:04 2011

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
/main/2      ---      svenkat           1. Initial for eGTP 2.1 Release
*********************************************************************91*/

