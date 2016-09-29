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

     Desc:     Data structures  

     File:     ve_rout.x

     Sid:      ve_rout.x@@/main/1 - Wed Dec  1 08:44:30 2010

     Prg:      rbhat

*********************************************************************21*/

#ifndef __VE_APP_X__
#define __VE_APP_X__


#ifdef __cplusplus
EXTERN "C" {
#endif

#ifdef VE_LNX_PCAP_PERF
/*******************************************************/
/*                eNodeB APP                           */
/*******************************************************/

/*
Packet filter component type identifier
Bits
8 7 6 5 4 3 2 1 
0 0 0 1 0 0 0 0   IPv4 remote address type
0 0 1 0 0 0 0 0   IPv6 remote address type
0 0 1 1 0 0 0 0   Protocol identifier/Next header type
0 1 0 0 0 0 0 0   Single local port type
0 1 0 0 0 0 0 1   Local port range type
0 1 0 1 0 0 0 0   Single remote port type 
0 1 0 1 0 0 0 1   Remote port range type
0 1 1 0 0 0 0 0   Security parameter index type
0 1 1 1 0 0 0 0   Type of service/Traffic class type
1 0 0 0 0 0 0 0   Flow label type

All other values are reserved.


For "IPv4 remote address type", the packet filter component value field shall be
encoded as a sequence of a four octet IPv4 address field and a four octet IPv4
address mask field. The IPv4 address field shall be transmitted first. */

/* IP address to MAC address mapping for all the external network 
   details */
typedef struct veAppRoutIP2MacMap
{
   U8            ipAddrStr[VE_APP_ROUT_IP_ADDR_LEN]; /* External network node IP 
                                                  Address string */
   U32           ipAddr;                       /*  External network node IP 
                                                  Address */
   U8            macAddr[VE_APP_ROUT_MAC_ADDR_LEN]; /* External network node
                                                  MAC Address */
}VeAppRoutIP2MacMap;

/* IP Address allocation control block */
typedef struct veAppNwAddrCfg
{
   CmInetNetAddr addr;      /* IP Address details */

  /* Need to check if the following can be avoided; As we will
   insert in to routCb once the next address is taken.
   The tracking can be based on global counter 
   if the insertion is failed, due to maximum entries, and the counter 
   gets reset, then we are exhausted to serve the UEs */

   Bool          assigned;  /* To track if the address is in use or not */
}VeAppNwAddrCfg;

/* IP Address allocation set to UEs */
/* Can add port too if required. */
typedef struct veAppUeRouteCb
{
   VeAppNwAddrCfg       nwUeAddr;      /* Network Address configured */
   CmLtePdcpId  *ueInfo;            /* pdcp Info*/

}VeAppUeRouteCb;


/* Main route control block for communicating with external network */
/* SV- Need to check for
1. MAC Address range
2. Ethernet Packet range
3. ARP Packet range 
*/

typedef struct veAppRouteCb
{
   TskInit    init;           /* Task initializat */
   pcap_t     *pcapHndl;      /* Libpcap Handler reference */

   U8         ethPkt[VE_APP_ROUT_MAX_ETH_PKT_LEN]; /* Total Ethernet Packet - used 
                                            everytime to send the data out
                                            to external network. Only the
                                            payload changes everytime and
                                            destination */

   U8         arpPkt[VE_APP_ROUT_MAX_ARP_PKT_LEN];     /* ARP Packet */

   S8         ethIntf[VE_APP_ROUT_PCAP_MAX_DEV_LEN];   /* Ethernet Interface */

   U8         localIPAddr[VE_APP_ROUT_IP_ADDR_LEN];  /* Self IP Address of eNB  */
   U8         localMACAddr[VE_APP_ROUT_MAC_ADDR_LEN];  /* Local MAC Address of eNB */


   U32        mapCnt;     /* Number of external network
                             nodes */
   VeAppRoutIP2MacMap ip2Mac[VE_APP_MAX_NW_NODES];  /* External network nodes(s)' 
                                                       IP - MAC
                                                       address mapping */

   U16        nmbUeAddr;      /* Number of UE network addresses configured */
   U16        nmbUeNwAddrCnt; /* Number of existing UEs in service
                                         to which IP Address has been allocated*/

   VeAppUeRouteCb    ueRouteCbLst[VE_MAX_UE];
                                    /* Set of route context control blocks
                                     having IP addresses allocated to UEs in
                                     service. This will be used exclusively
                                     by recieve task to check if destination 
                                     tunnel data packet to UE is intended one. */
}VeAppRouteCb;


/*******************************************************/
/* eNodeB APP Function declarations */
/*******************************************************/

/* eNB to Ue */
EXTERN S16 veAppRoutPerfStartPcap ARGS((Pst    *pst,   Buffer *mBuf,
                                    Mem    *sMem));
EXTERN S16 veAppRoutInit ARGS((Void));


EXTERN S16 veAppRoutAddUeRouteCb ARGS((CmLtePdcpId   *ueRbInfo,
                                   VeAppUeRouteCb  **newUeRouteCb));

EXTERN S16 veAppRoutDelUeRouteCb ARGS((CmLtePdcpId   *ueRbInfo));


EXTERN S16 veAppRoutSendArpReqPkt ARGS((U8  *ipAddr));


EXTERN Void veAppRoutHndlSndPkt ARGS((Buffer *mBuf));

EXTERN Void *veAppRoutPktReceiver ARGS((Void *arg));

#endif /* VE_LNX_PCAP_PERF */

/*******************************************************/
/* End of eNodeB APP Function declarations                 */
/*******************************************************/

/*******************************************************/
/*               End of eNB Route                          */
/*******************************************************/

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __VE_APP_X__ */

/********************************************************************30**
  
         End of file:     ve_rout.x@@/main/1 - Wed Dec  1 08:44:30 2010
  
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
