/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
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
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/********************************************************************20**
  
     Name:     TCP UDP Convergence Layer acceptance tests
  
     Type:     C include file
  
     Desc:     Environment dependent defines required for TUCL
               acceptance tests.
  
     File:     hi_accen.h
  
     Sid:      hi_accen.h@@/main/5 - Mon Mar  3 20:08:53 2008
  
     Prg:      asa
  
*********************************************************************21*/

#ifndef __HIACCENH__
#define __HIACCENH__



/* Specify the IP Address of the machine in hex */
#define  HI_ACC_SERV_ADDR      0x7f000001

/* Any Class D address */
#define  HI_ACC_MCAST_IPV4ADDR 0xe0000129  /* "224.1.1.41" */

/* port number of the server */
#define  HI_ACC_SERV_PORT      12435

#ifdef IPV6_SUPPORTED
/* Specify the IPV6 loopback address */
#define  HI_ACC_SERV_V6ADDR      CM_INET_INADDR6_ANY 

/* All nodes multicast address */
#define  HI_ACC_MCAST_IPV6ADDR   "ff02::1"
#ifdef SS_LINUX
#define  HI_ACC_IPV6_MCAST_LOCINF    1
#else
#define  HI_ACC_IPV6_MCAST_LOCINF    0
#endif /* SS_LINUX */
#endif /* IPV6_SUPPORTED */

#ifdef HI_TLS
#define HI_ACC_TLSCLIENT_REMPORT       12457
#define HI_ACC_TLSCLIENT_CAFILE        "rootcert.pem"
#define HI_ACC_TLSCLIENT_CAPATH        NULL
#define HI_ACC_TLSCLIENT_CERTFILE      "client.pem"
#define HI_ACC_TLSCLIENT_PRIVKEYFILE   "client.pem"
#define HI_ACC_TLSCLIENT_CIPHERS       "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"

#define HI_ACC_TLSSERVER_LOCPORT       12458
#define HI_ACC_TLSSERVER_CAFILE        "rootcert.pem"
#define HI_ACC_TLSSERVER_CAPATH        NULL
#define HI_ACC_TLSSERVER_CERTFILE      "server.pem"
#define HI_ACC_TLSSERVER_PRIVKEYFILE   "server.pem"
#define HI_ACC_TLSSERVER_CIPHERS       "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
#endif /* HI_TLS */

#endif /* __HIACCENH__ */


/********************************************************************30**
 
         End of file:     hi_accen.h@@/main/5 - Mon Mar  3 20:08:53 2008

*********************************************************************31*/
 
/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/
/********************************************************************70**
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
 
*********************************************************************71*/
 
/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
    ver       pat     init                  description
----------- --------- ---- -----------------------------------------------
1.1         ---       asa  1. initial release.
/main/2     ---       cvp  1. changed the copyright header.
/main/3     ---       cvp  1. Changes for IPV6 support.
                           2. Changed the copyright header.
/main/3+    hi012.104 bdu  1. Define the MCAST6 interface as
                              1 on Linux.
/main/4      ---       kp   1. Updated for release 1.5.
/main/5      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
