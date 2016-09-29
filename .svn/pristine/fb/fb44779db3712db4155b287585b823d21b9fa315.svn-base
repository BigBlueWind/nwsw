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

     Name:    Trillium LTE eNB APP -  PCAP Router Module

     Type:    C Source file

     Desc:    This file contains the Ethernet Packet routing controller source code

     File:    ve_rout.c

     Sid:      ve_rout.c@@/main/1 - Wed Dec  1 08:44:29 2010

     Prg:     sv 
*********************************************************************21*/
#ifdef VE_LNX_PCAP_PERF

/* header include files (.h) */
#include "envopt.h"        /* environment options */

#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"        /* common DNS library defines */
#include "cm_lte.h"       /*  Common UMTS includes */

/*#include "leg.h"    */        /* defines and macros for EGTP */
/*#include "egt.h"   */         /* defines and macros for EGTP */

/*#include "lav.h" */
#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#include "ve_eut.h"
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif
#include "lrg.h"           /* MAC RRM control Interface */
#include "ve_perf.h"

/* RRM related includes */
#include "ve.h"
#include "ve_sm_init.h"
#include "lve.h"

#include "ve_rout.h"

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "cm_dns.x"        /* common DNS library defines */
#include "cm_lte.x"       /*  Common UMTS includes */

#include "leg.x"           /* layer management typedefs for EGTP */
/*#include "egt.x"*/           /* typedefs for EGTP */

/* #include "lav.x" */
#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "ve_eut.x"
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
/* RRM related includes */
#include "lve.x"
#include "ve.x"
#include "ve_sm_init.x"
#include "ve_dflt.h"

#include "ve_rout.x"


VeAppRouteCb veAppRoutCb;
CmInetNetAddr veAppRoutexGwCfg;

/* NOTE:  PCAP Implementation at eNB to test Uu performence, It is currently implemented,
   by considering single UE and Here Packet filtering with respect to TFT is not added.
   to Use This SetUp, in make file lnx_perf should have one more define "VE_LNX_PCAP_PERF"
   corresponding to this, at ueSim  we need to enable both  UESIM_ENABLE_PCAP  and 
   ENB_PERF_MEAS flags */



/* NOTE: Following values are reverse of actual values for direct
   memcpy and comparision purpose as this software is written for 
   x86 platform (little endian), actual values are in comments */
/* Let us see how best we can make it platform independent */
typedef enum veAppEthType
{
   VE_APP_ETH_TYPE_IP  = 0x0008,  /* 0x0800 */
   VE_APP_ETH_TYPE_ARP = 0x0608  /* 0x0806 */ 
}VeAppEthType;

typedef enum veAppARPType
{
   VE_APP_ARP_REQ     =  0x0100, /* 0x0001 */
   VE_APP_ARP_RSP     =  0x0200 /* 0x0002 */
}VeAppARPType;

typedef enum veAppIPProtType
{
   VE_APP_IP_ICMP =   0x01, /* 1 */
   VE_APP_IP_TCP  =   0x06, /* 6 */
   VE_APP_IP_UDP  =   0x11  /* 17 */
}VeAppIPProtType;

typedef enum veAppICMPType
{
   VE_APP_ICMP_REDIR   =  0x05
}VeAppICMPType;


PRIVATE U16 arp_hardware_type_ethernet =  0x0100; /* 0x0001 */
PRIVATE U8  arp_hardware_size          =  0x06;
PRIVATE U8  arp_protocol_size          =  0x04;


/* Function Prototypes */


PRIVATE S16 veAppRoutGetGatewayAddr ARGS((Void));


PRIVATE S16 veAppRoutGetNwParam ARGS((U32 selfIp));
PRIVATE S16 veAppRoutInitUeRouteCb ARGS((CmLtePdcpId      *dataTunnel,
                                     VeAppUeRouteCb  *newUeRouteCb));

/* Declaration of pcap_sendpacket() which is not present in pcap.h */
EXTERN int pcap_sendpacket(pcap_t *hdl, CONSTANT U8* buf, int len);

PRIVATE S16 veAppRoutInitAdaptor ARGS((Void));
PRIVATE S16 veAppRoutInitEthPkt ARGS((Void));
PRIVATE S16 veAppRoutInitArpPkt ARGS((Void));
PRIVATE Void veAppRoutSendGratArpReqPkt ARGS((U8  *ipAddr));


PRIVATE Void veAppRoutSendEthPkt ARGS((U8  *ethPkt,
                                   U32 len));


PRIVATE Void veAppRoutSendArpRspPkt ARGS((CONSTANT U8  *arpReqPkt,
                                      U32          len));

PRIVATE S16 veAppRoutFndDstBearerOnAddrPort ARGS((U32               ipAddr,
                                              U16               port,
                                              CmLtePdcpId      **ueTunlCb));

PRIVATE S16 veAppRoutFndDstBearerOnAddrProtType ARGS((U32                ipAddr,
                                                  CONSTANT U8        protoType,
                                                  CmLtePdcpId       **ueTunlCb));

PRIVATE Void veAppRoutToUe ARGS((CONSTANT U8  *ipPkt,
                                  U32           len,
                                  CONSTANT U8   protoType));

PRIVATE U8 * veAppRoutFindMACAddress ARGS((U32 ipAddr));

PRIVATE Void veAppRoutBuildEthPkt ARGS((U8   *ipPkt, 
                                    U32   len));


PRIVATE U16 veAppRoutCalcIPChecksum ARGS((U8  *ipPkt, 
                                      U32 ipHdrLen));

PRIVATE Void veAppRoutFragmentIPPkt ARGS((U8  *ipPkt, 
                                      U32  len));

PRIVATE S16 veAppRoutUpdUeRouteCbPort ARGS((U32  ipAddr,
                                        U16  port,
                                        U8   protoType));

PRIVATE Void veAppRoutRcvdPktHndlr ARGS((u_char                   *args,
                                     CONSTANT struct pcap_pkthdr *hdr,
                                     CONSTANT u_char             *pktData));

PRIVATE Void veAppRoutHndlARPPkt ARGS((CONSTANT U8  *arpPkt,
                                   U32          bytesRcvd));

PRIVATE Void veAppRoutHndlARPRequest ARGS((CONSTANT U8  *arpPkt,
                                       U32          bytesRcvd));

PRIVATE Void veAppRoutHndlARPResponse ARGS((CONSTANT U8  *arpPkt,
                                        U32          bytesRcvd));

PRIVATE S16 veAppRoutFndDstBearerOnAddr ARGS((U32      ipAddr));


PRIVATE Void veAppRoutHndlIPPkt ARGS((CONSTANT U8  *ipPkt,
                                  U32          bytesRcvd));


/*
 *
 *       Fun:   veAppRoutGetGatewayAddr
 *
 *       Desc:  This function gets the Gateway IP address
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 veAppRoutGetGatewayAddr
(
Void
)
#else
PRIVATE S16 veAppRoutGetGatewayAddr (Void)
#endif
{
   struct in_addr addr;
   S8             line[255];
   S8             *ptr = NULLP;
   FILE           *fp = NULLP;
   U8             idx;
   S16            ret;
   S8             ethIntf[8];

   TRC2(veAppRoutGetGatewayAddr)
   
   /* Copy the ethernet interface name upto ":"(in case of virtual interface)
      to ethIntf */
   strncpy(ethIntf, veAppRoutCb.ethIntf, sizeof(ethIntf));
   for (idx = 0; idx < sizeof(ethIntf); idx++)
   {
      if (ethIntf[idx] == ':')
      {
         ethIntf[idx] = '\0';
      }
   }
   
   /* Execute 'netstat -rn' command and redirect output 
      to 'gateway.txt' file temporarily. This file will be deleted after
      extracting the Gateway IP address */
   ret = system(VE_APP_CMD1);
   if (ret != 0)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Failed to execute command:%s, error(%d)\n",
                            VE_APP_CMD1, ret));

      RETVALUE(RFAILED);
   }

   /* Open 'gateway.txt' file in read mode */
   fp = fopen(VE_APP_FILENAME, "rb");
   if (!fp)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Failed to open file:%s\n", VE_APP_FILENAME));
      perror("fopen");
      system(VE_APP_CMD2);
      RETVALUE(RFAILED);
   }

   /*
    Dirty parsing of 'netstat -rn' output, as there is no ioctl command
    to get the Gateway IP address. The following code parses the output
    of following format to get the Gateway IP address. 

Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
172.25.0.0      0.0.0.0         255.255.0.0     U         0 0          0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U         0 0          0 eth0
0.0.0.0         172.25.0.254    0.0.0.0         UG        0 0          0 eth0
0.0.0.0         172.26.0.254    0.0.0.0         UG        0 0          0 eth1

    NOTE: If there are multiple default gateways for same interface
          first one shall be used.
    */
   while (fgets(line, 255, fp))
   {
      if ((ptr = strstr(line, VE_APP_DEFAULT)))
      {
         /* See sample output above, the default Gateway entry 
            contains falgs 'UG' and interface name */
         if (strstr(line, "UG") && strstr(line, ethIntf))
            break;
      }
   }

   if (!ptr)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Failed to get Gateway address\n"));
      fclose(fp);
      system(VE_APP_CMD2);
      RETVALUE(RFAILED);
   }

   /* Skip default address */
   ptr = ptr + strlen(VE_APP_DEFAULT);
   
   /* Skip tabs and(or) spaces */
   idx = 0;
   while (!isdigit(ptr[idx]))
   {
      idx++;
   }
   ptr = ptr + idx;
   
   /* Get the length of the Gateway IP address string */
   idx = 0;
   while (isdigit(ptr[idx]))
   {
      idx++;
      if (ptr[idx] == '.')
         idx++;
   }

   /* Make sure to add null character at the end of 
      Gateway IP address string */
   ptr[idx] = '\0';

   /* Convert the Gateway IP address string to U32 format */
   inet_aton(ptr, &addr);
   veAppRoutexGwCfg.u.ipv4NetAddr = htonl(addr.s_addr);

   /* Close 'gateway.txt' file pointer and delete the file */
   fclose(fp);
   system(VE_APP_CMD2);

   RETVALUE(ROK);
}/* End of veAppRoutGetGatewayAddr */


/*
 *
 *       Fun:   veAppRoutGetNwParam
 *
 *       Desc:  This function gets the Network paramters Ethernet Interface,
 *              MAC address and gateway IP address
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 veAppRoutGetNwParam
(
U32 selfIp
)
#else
PRIVATE S16 veAppRoutGetNwParam (selfIp)
U32 selfIp;
#endif
{
   struct ifreq       *ifr;
   struct ifconf      ifc;
   struct in_addr     *addr;
   struct sockaddr_in sa;
   S32                sockfd;
   S32                ifclen;
   S16                ret;
   U8                 found = FALSE;

   TRC2(veAppRoutGetNwParam)

   /* Open a socket for ioctl */
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, "Failed to open socket\n"));
      perror("socket");
      close(sockfd);
      RETVALUE(RFAILED);
   }

   /* Get the length of configuration info of all interfaces */
   cmMemset((U8 *)&ifc, 0, sizeof(struct ifconf));
   if (ioctl(sockfd, SIOCGIFCONF, &ifc))
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                           "Failed to get length of config info of all \
                            interfaces\n"));
      perror("ioctl SIOCGIFCONF");
      close(sockfd);
      RETVALUE(RFAILED);
   }
   ifclen = ifc.ifc_len;

   /* Get the configuration info of all interfaces */
   cmMemset((U8 *)&ifc, 0, sizeof(struct ifconf));

   VE_ALLOC(&ifc.ifc_req, ifclen);

   if (NULLP == ifc.ifc_req)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                "Failed to allocate memory of size(%ld) for ifc.ifc_req\n",
                 ifclen));
      close(sockfd);
      RETVALUE(RFAILED);
   }

   ifc.ifc_len = ifclen;
   if (ioctl(sockfd, SIOCGIFCONF, &ifc))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                           "Failed to get config info of all interfaces\n"));

      perror("ioctl SIOCGIFCONF");
      VE_FREE(ifc.ifc_req, ifclen);

      close(sockfd);
      RETVALUE(RFAILED);
   }

   /* Get the ethernet interface name and address matching the selfIp from
      all configured interfaces */
   for (ifr = ifc.ifc_req; 
        (ifr && ((char *)ifr < (char *) ifc.ifc_req + ifc.ifc_len)); ++ifr)
   {
      addr = ((struct in_addr *) &ifr->ifr_addr.sa_data[sizeof(sa.sin_port)]);

      if (htonl(addr->s_addr) == selfIp)
      {
         /* Found the interface matching selfIp, get other details */
         strcpy(veAppRoutCb.ethIntf, ifr->ifr_name);

         /* Get Ethernet (Hardware) Address */
         if (ioctl(sockfd, SIOCGIFHWADDR, ifr)) 
         {
            VE_DBG_ERROR((VE_PRNT_BUF,
                                  "Failed to get Ethernet address\n"));
            perror("ioctl SIOCGIFHWADDR");

            VE_FREE(ifc.ifc_req, ifclen);
            close(sockfd);
            RETVALUE(RFAILED);
         }
         cmMemcpy(veAppRoutCb.localMACAddr, (U8 *)ifr->ifr_addr.sa_data, 
                  VE_APP_ROUT_MAC_ADDR_LEN);
         found = TRUE;
         break;
      }
   }

   VE_FREE(ifc.ifc_req, ifclen);
   close(sockfd);

   if (FALSE == found)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Failed to get network parameteres\n"));
      RETVALUE(RFAILED);
   }
   /* We now configure external network node's address */
   /* Check the primary external network node's address*/
   if((CM_INET_IPV4ADDR_TYPE == veAppRoutexGwCfg.type) &&
      (0 == veAppRoutexGwCfg.u.ipv4NetAddr))
   { 
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                     "External network details not configured, trying default \
                      Gateway...\n"));
#endif
   /* The following is optional to retrieve the default gateway's IP 
      Address */
      ret = veAppRoutGetGatewayAddr();

      if (ret != ROK)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,
                               "Failed to get Gateway IP address\n"));

         RETVALUE(RFAILED);
      }
   }
   
   RETVALUE(ROK);
}/* End of veAppRoutGetNwParam */
/*
*
*       Fun:   veAppRoutPerfStartPcap 
*
*       Desc:  Start Data Traffic 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rout.c 
*
*/
#ifdef ANSI
PUBLIC S16 veAppRoutPerfStartPcap 
(              
Pst *pst,
Buffer *mBuf,
Mem *sMem   
)              
#else  
PUBLIC S16 veAppRoutPerfStartPcap( pst, mBuf, sMem)
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif         
{
   S16 ret = ROK;
   CmLtePdcpId *pdcpId;
   VeAppUeRouteCb  *newUeRouteCb;
               
   TRC2(veAppRoutPerfStartPcap);
                                                                                       
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
            \n ***** Received Event from Relay to start Data ****\n"));
#endif

   VE_ALLOC(&pdcpId,sizeof(CmLtePdcpId));
   
   /* Unpk CellId, Ue Id details and mark the state to Ready to send Data */
   cmUnpkLtePdcpId(pdcpId, mBuf);

   ret = veAppRoutAddUeRouteCb(pdcpId,&newUeRouteCb );
   if( ret != ROK)
   {
     VE_FREE(pdcpId,sizeof(CmLtePdcpId)); 
   }
   printf(" Signalling is Completed: Ready for Data Traffic\n");
   if(veAppRoutCb.nmbUeNwAddrCnt == VE_MAX_UE)
   {
#ifdef DEBUGP
      /* List is full */
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
               \n  *****  All UEs are busy ****\n"));
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }                                                                      
   SPutMsg(mBuf);

   RETVALUE(ROK);

}/* veAppRoutPerfStartPcap */


/*
 *
 *       Fun:   veAppRoutInit
 *
 *       Desc:  This function initializes the routing related params
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PUBLIC S16 veAppRoutInit
(
)
#else
PUBLIC S16 veAppRoutInit ()
#endif
{
   VeAppRouteCb *routeCb = NULLP;
   VeAppUeRouteCb *ueRouteCb;
   pthread_t pcapTid;
   CmInetIpAddr ueIPAddr, selfIP;
   U8   idx, routeIdx = 0;
   U8  primaryMac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
   U8  *addr;
   S16  ret;
#ifdef LTE_LNX_AFFINITY
   cpu_set_t cpuSet;
#endif


   TRC2(veAppRoutInit)
    
   /* Configure the external gatway address */
   veAppRoutexGwCfg.type = CM_INET_IPV4ADDR_TYPE;
   veAppRoutexGwCfg.u.ipv4NetAddr = smCfgCb.dfltExGwIpAddr;

   cmMemset((U8 *)&veAppRoutCb, 0, (sizeof(VeAppRouteCb))); 
    
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                       "Initialising Pcap Routing Parameters\n"));
#endif
   routeCb = &veAppRoutCb; 

   /* Initialize all UE routing control blocks */
   routeCb->nmbUeAddr = VE_MAX_UE;

   /* Start Address */
   ueIPAddr = smCfgCb.ueStartIpAddr;

   /* Initialize the set of Network addresses to assign to UE */
   for (routeIdx = 0; routeIdx < routeCb->nmbUeAddr; routeIdx++)
   {
      ueRouteCb = &routeCb->ueRouteCbLst[routeIdx];
      printf(" routeIdx= %d\n",routeIdx); 
      cmMemset((U8 *)ueRouteCb, 0, (sizeof(VeAppUeRouteCb))); /* Assigned FALSE */
      ueRouteCb->nwUeAddr.addr.type = CM_INET_IPV4ADDR_TYPE;
      ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr = ueIPAddr;
      ueIPAddr++;
   }

   /* Get Network Parameters using self assigned IP Address */
   ret = veAppRoutGetNwParam(smCfgCb.enbIpAddr);
   if (ROK != ret)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                           "Failed to get eNodeB network parameters\n"));
      RETVALUE(RFAILED);
   }

   selfIP = smCfgCb.enbIpAddr;

   /* Save the U32 local s IP address in byte array form
      to save time in future */
   for (idx = 0; idx < VE_APP_ROUT_IP_ADDR_LEN; idx++)
   {
      routeCb->localIPAddr[idx] = selfIP >> (8 * (3 - idx)); 
   }
   
  /* We have one address of external network node
     configured. This will change if redirected during
     run time. */
 
   routeCb->mapCnt = 1; 

   /* Initialize IP to MAC Mapping Table */
   cmMemset((U8 *)&routeCb->ip2Mac, 0, 
                  sizeof(VeAppRoutIP2MacMap) * (VE_APP_MAX_NW_NODES));

   /* Save the U32 external node's IP addresses in byte array form
      to save time in future */
   for(routeIdx = 0; routeIdx < routeCb->mapCnt; routeIdx++)
   {
      routeCb->ip2Mac[routeIdx].ipAddr = veAppRoutexGwCfg.u.ipv4NetAddr;
      for (idx = 0; idx < VE_APP_ROUT_IP_ADDR_LEN; idx++)
      {
         routeCb->ip2Mac[routeIdx].ipAddrStr[idx] = 
               veAppRoutexGwCfg.u.ipv4NetAddr >> (8 * (3 - idx));
      }
      addr = routeCb->ip2Mac[routeIdx].ipAddrStr;

#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, 
                   "Gateway IP address : %d.%d.%d.%d\n", 
                   addr[0], addr[1], addr[2], addr[3]));

      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                 "Gateway IP address in hex : %x\n",
                 routeCb->ip2Mac[routeIdx].ipAddr));
#endif
      cmMemcpy((U8 *)routeCb->ip2Mac[routeIdx].macAddr, primaryMac, 
            VE_APP_ROUT_MAC_ADDR_LEN);
    
   }

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                        "eNodeB Network Configuration:\n"));

   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, 
                        "---------------------------\n"));
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                        "Ethernet Interface : %s\n", routeCb->ethIntf));
   addr = routeCb->localMACAddr;
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
               "Ethernet Address : %x:%x:%x:%x:%x:%x\n", addr[0], addr[1],
               addr[2], addr[3], addr[4], addr[5]));

   addr = routeCb->localIPAddr;
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, 
               "Local IP address : %d.%d.%d.%d\n", 
               addr[0], addr[1], addr[2], addr[3]));

   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                        "Local IP address in hex : %x\n", selfIP));

  
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "---------------------------\n"));

#endif

   /* Initialize Ethernet packet */
   ret = veAppRoutInitEthPkt();
   if (ret != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Failed to initialise Ethernet Packet\n"));
      RETVALUE(RFAILED);
   }

   /* Initialize ARP packet */
   ret = veAppRoutInitArpPkt();
   if (ret != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            "Failed to initialise ARP Packet\n"));
      RETVALUE(RFAILED);
   }

   /* Open the adaptor for capturing packets - LibPcap */
   ret = veAppRoutInitAdaptor();
   if (ret != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                           "veAppRoutInitAdaptor failed\n"));
      RETVALUE(RFAILED);
   }

   /* Create the thread for capturing the packets at eNB App */
   if (pthread_create(&pcapTid, NULLP, veAppRoutPktReceiver, NULLP))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                "veAppRoutInit: veAppRoutPktReceiver thread creation failed\n"));
      RETVALUE(RFAILED);
   }
   /* Following is applicable only for Linux machine (Calpella)
      from Intel. Core number used here can be changed
      accordingly, as expected on the machine of interest.
      This portion of code can be disabled if not required. */

#ifdef LTE_LNX_AFFINITY
   /* Initialize the cpu mask */
   CPU_ZERO(&cpuSet);

   /* Set the cpu mask */
   CPU_SET(VE_ROUT_RX_THRD_CORE, &cpuSet);

   /* Set thread affinity for linux */
   if (pthread_setaffinity_np(pcapTid, sizeof(cpuSet), &cpuSet) < 0)
   {
      printf("Set affinity failed for TTI generator \n");
      exit(0);
   }
#endif

   /* Now send the ARP Request to primary external network node */

   ret = veAppRoutSendArpReqPkt(veAppRoutCb.ip2Mac[VE_APP_PRIMARY_GW_ID].ipAddrStr);
   if (ROK != ret)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                           "veAppRoutSendArpReqPkt failed \n"));
      RETVALUE (ret);
   }

   RETVALUE(ROK);
}/* End of veAppRoutInit */



/*
 *
 *       Fun:  veAppRoutAddUeRouteCb 
 *
 *       Desc:  This function adds a new UE route control block
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  ve_route.c
 *
 */
#ifdef ANSI
PUBLIC S16 veAppRoutAddUeRouteCb
(
CmLtePdcpId        *pdcpCb,
VeAppUeRouteCb  **newUeRouteCb
)
#else
PUBLIC S16 veAppRoutAddUeRouteCb(pdcpCb, newUeRouteCb)
CmLtePdcpId        *pdcpCb;
VeAppUeRouteCb  **newUeRouteCb;
#endif
{
   VeAppRouteCb *routeCb = &veAppRoutCb;
   U16  routeIdx = 0;

   U8   addr[VE_APP_ROUT_IP_ADDR_LEN];
   U32  ipAddr;

   S16  ret = RFAILED;

   TRC2(veAppRoutAddUeRouteCb)
      
   if ((routeCb->nmbUeNwAddrCnt > routeCb->nmbUeAddr))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                    "Failed to add new UE route control block,\
                     reached max UE configured set %d\n",
                     routeCb->nmbUeAddr));
      RETVALUE(RFAILED);
   }

   /* Right first get the RouteCb based on 
      destination IP Address to assign from the configured ones */

   if (!routeCb->ueRouteCbLst[routeCb->nmbUeNwAddrCnt].nwUeAddr.assigned) 
   {
      *newUeRouteCb = &routeCb->ueRouteCbLst[routeCb->nmbUeNwAddrCnt];

      /* Right assign a new control block */
      ret = veAppRoutInitUeRouteCb(pdcpCb, *newUeRouteCb);

   }
   else /* Somehow we have missed out the counter - next one to assign to UE */
   {
      /* Loop upto latest counter Nw Addresses assigned to set of UEs */
      for (routeIdx = 0; routeIdx < routeCb->nmbUeAddr; routeIdx++)
      {
         /* Check if an IP address is free to allocate */
         if (!routeCb->ueRouteCbLst[routeIdx].nwUeAddr.assigned) 
         {
            /* Right assign a new control block */
            *newUeRouteCb = &routeCb->ueRouteCbLst[routeIdx];
            ret = veAppRoutInitUeRouteCb(pdcpCb, *newUeRouteCb);
            break;
         }
      }
   }
   if(ROK == ret)
   { 
      /* Send Gratuitous ARP request for this IP address, so that the external
         nodes update their ARP tables. */
      ipAddr = ((*newUeRouteCb)->nwUeAddr.addr.u.ipv4NetAddr);

      addr[0] = (U8) ((ipAddr >> 24) & 0xff);
      addr[1] = (U8) ((ipAddr >> 16) & 0xff);
      addr[2] = (U8) ((ipAddr >> 8) & 0xff);
      addr[3] = (U8) (ipAddr & 0xff);

      printf("eNodeB: Sending Gratuitous ARP request for %d:%d:%d:%d \n", \
              addr[0], addr[1], addr[2], addr[3]);

      veAppRoutSendGratArpReqPkt(addr);
   }
   RETVALUE(ret);

}


/*
 *
 *       Fun:   veAppRoutInitUeRouteCb
 *
 *       Desc:  This function initializes a new UE route control block
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  ve_route.c
 *
 */
#ifdef ANSI
PRIVATE  S16 veAppRoutInitUeRouteCb
(
CmLtePdcpId        *pdcpCb,
VeAppUeRouteCb  *newUeRouteCb
)
#else
PRIVATE S16 veAppRoutInitUeRouteCb(pdcpCb, newUeRouteCb)
CmLtePdcpId        *pdcpCb;
VeAppUeRouteCb  *newUeRouteCb;
#endif
{

   TRC2(veAppRoutInitUeRouteCb)

   newUeRouteCb->nwUeAddr.assigned = TRUE;
    
   newUeRouteCb->ueInfo = pdcpCb;
   /* packet filter Init can be added here */ 
   veAppRoutCb.nmbUeNwAddrCnt++; /* Right finally increment the counter */

   RETVALUE(ROK);
}



/*
 *
 *       Fun:   veAppRoutDelUeRouteCb
 *
 *       Desc:  This function deletes an existing UE route control block
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  ve_route.c
 *
 */
#ifdef ANSI
PUBLIC S16 veAppRoutDelUeRouteCb
(
CmLtePdcpId  *pdcpCb
)
#else
PUBLIC S16 veAppRoutDelUeRouteCb(pdcpCb)
CmLtePdcpId  *pdcpCb;
#endif
{
   VeAppRouteCb *routeCb = &veAppRoutCb;
   VeAppUeRouteCb *ueRouteCb = NULLP;
   U16  routeIdx;

   TRC2(veAppRoutDelUeRouteCb)
      
   /* Loop upto latest counter  Nw Addresses assigned to set of UEs */
   for (routeIdx = 0; routeIdx < routeCb->nmbUeAddr; routeIdx++)
   {
      ueRouteCb = &routeCb->ueRouteCbLst[routeIdx];  
      
      if(pdcpCb == ueRouteCb->ueInfo)
      {
            routeCb->nmbUeAddr--;
            RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);

}


/*
 *
 *       Fun:   veAppRoutInitEthPkt
 *
 *       Desc:  This function initializes the Ethernet Packet
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 veAppRoutInitEthPkt
(
Void
)
#else
PRIVATE S16 veAppRoutInitEthPkt (Void)
#endif
{
   VeAppEthType ethType = VE_APP_ETH_TYPE_IP;
   VeAppRouteCb *routeCb = &veAppRoutCb;

   TRC2(veAppRoutInitEthPkt)

   /* Set the destination MAC address to broadcast address (all 0xff) */
   cmMemset(routeCb->ethPkt, 0xff, VE_APP_ROUT_MAC_ADDR_LEN);

   /* Set the source MAC address to local MAC address */
   cmMemcpy(routeCb->ethPkt + VE_APP_ROUT_MAC_ADDR_LEN, 
            routeCb->localMACAddr, VE_APP_ROUT_MAC_ADDR_LEN);

   /* Set the type of Ethernet packet to IP packet (0x0800) */
   cmMemcpy(routeCb->ethPkt + (VE_APP_ROUT_MAC_ADDR_LEN * 2), 
            (U8 *)&ethType, 2);

   RETVALUE(ROK);

}/* End of veAppRoutInitEthPkt */

/*
 *
 *       Fun:   veAppRoutInitArpPkt
 *
 *       Desc:  This function initializes the Ethernet Packet
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 veAppRoutInitArpPkt
(
Void
)
#else
PRIVATE S16 veAppRoutInitArpPkt (Void)
#endif
{
   VeAppEthType ethType = VE_APP_ETH_TYPE_ARP;
   VeAppEthType ethProtocolType = VE_APP_ETH_TYPE_IP;
   VeAppARPType arpType = VE_APP_ARP_REQ;
   VeAppRouteCb *routeCb = &veAppRoutCb;
   U8 idx;

   TRC2(veAppRoutInitArpPkt)

   /* First Fill Ethenet Header */
   /* Set the destination MAC address to broadcast address and 
      set the source MAC address to local MAC address */
   cmMemcpy(routeCb->arpPkt, routeCb->ethPkt, VE_APP_ROUT_MAC_ADDR_LEN * 2);
   idx = VE_APP_ROUT_MAC_ADDR_LEN * 2;

   /* Set the type of Ethernet packet to ARP (0x0806) */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&ethType, 2);

   /* Fill ARP packet (Ethernet payload) */
   idx += 2;
   /* Set the hardware type to Ethernet (0x0001) */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&arp_hardware_type_ethernet, 2);
   idx += 2;

   /* Set the ethernet protocol type to IP (0x0800) */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&ethProtocolType, 2);
   idx += 2;

   /* Set hardware size to 6 for Ethernet */
   cmMemcpy(routeCb->arpPkt + idx, &arp_hardware_size, 1);
   idx += 1;

   /* Set protocol size to 4 for IP */
   cmMemcpy(routeCb->arpPkt + idx, &arp_protocol_size, 1);
   idx += 1;

   /* Set operation to ARP request (0x0001) */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&arpType, 2);
   idx += 2;

   /* Set the Sender Hardware Address to local MAC address */
   cmMemcpy(routeCb->arpPkt + idx, routeCb->localMACAddr,
            VE_APP_ROUT_MAC_ADDR_LEN);

   idx += VE_APP_ROUT_MAC_ADDR_LEN;

   /* Set the Sender Protocol Address to local IP address */
   cmMemcpy(routeCb->arpPkt + idx, routeCb->localIPAddr, 
            VE_APP_ROUT_IP_ADDR_LEN);

   idx += VE_APP_ROUT_IP_ADDR_LEN;

   /* Initialise the Target Hardware Address and 
      Target Protocol Adress to 0xff */
   cmMemset(routeCb->arpPkt + idx, 0xff, 
            VE_APP_ROUT_MAC_ADDR_LEN + VE_APP_ROUT_IP_ADDR_LEN);

   RETVALUE(ROK);

}/* End of veAppRoutInitArpPkt */

/*
 *
 *       Fun:   veAppRoutSendEthPkt
 *
 *       Desc:  This function sends out Ethernet packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutSendEthPkt
(
U8    *ethPkt,
U32    len
)
#else
PRIVATE Void veAppRoutSendEthPkt (ethPkt, len)
U8    *ethPkt;
U32    len;
#endif
{
   S16 ret;

   TRC2(veAppRoutSendEthPkt)

   /* Send out Ethernet packet using PCAP library function */
   ret = pcap_sendpacket(veAppRoutCb.pcapHndl, ethPkt, (S32)len);
   if (0 != ret)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                      "Failed to send Ethernet packet through the \
                       interface - %d\n", ret));
   }

   RETVOID;
}/* End of veAppRoutSendEthPkt */

/*
 *
 *       Fun:   veAppRoutSendArpReqPkt
 *
 *       Desc:  This function sends the ARP request packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PUBLIC S16 veAppRoutSendArpReqPkt
(
U8  *ipAddr
)
#else
PUBLIC S16 veAppRoutSendArpReqPkt (ipAddr)
U8  *ipAddr;
#endif
{
   U16 idx = 38; /* Index of ARP packet at which destination IP address
                    has to be added */

   TRC2(veAppRoutSendArpReqPkt)

   /* Take the default ARP packet and add the destination IP address */
   cmMemcpy(veAppRoutCb.arpPkt + 38, ipAddr, VE_APP_ROUT_IP_ADDR_LEN);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, 
              "Sending ARP request packet for IP address:%d.%d.%d.%d\n",
              ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]));
#endif
   /* Send out ARP request packet */
   veAppRoutSendEthPkt(veAppRoutCb.arpPkt, idx + 4); 

   RETVALUE(ROK);

}/* End of veAppRoutSendArpReqPkt */

/*
 *
 *       Fun:   veAppRoutSendGratArpReqPkt
 *
 *       Desc:  This function sends the Gratuitous ARP request packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutSendGratArpReqPkt
(
U8  *ipAddr
)
#else
PRIVATE Void veAppRoutSendGratArpReqPkt (ipAddr)
U8  *ipAddr;
#endif
{
   U8  gratArpPkt[VE_APP_ROUT_MAX_ARP_PKT_LEN]; /* Gratuitous ARP */
   U16 idx = 38; /* Index of ARP packet at which destination IP address
                    has to be added */
   printf("veAppRoutSendGratArpReqPkt:1\n");
   TRC2(veAppRoutSendGratArpReqPkt)

   /* Copy ARP request packet to gratuitous ARP request packet */
   cmMemcpy(gratArpPkt, veAppRoutCb.arpPkt, VE_APP_ROUT_MAX_ARP_PKT_LEN);

   /* Gratuitous ARP request packet should have same source and destination
      protocol address. Also destination MAC address should be broadcast
      address */
   cmMemcpy(gratArpPkt + 28, ipAddr, VE_APP_ROUT_IP_ADDR_LEN);
   cmMemcpy(gratArpPkt + 38, ipAddr, VE_APP_ROUT_IP_ADDR_LEN);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, 
              "Sending Gratuitous ARP request for IP address:%d.%d.%d.%d\n",
               ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]));
#endif
   /* Send out ARP request packet */
   veAppRoutSendEthPkt(gratArpPkt, idx + 4); 

   RETVOID;
}/* End of veAppRoutSendGratArpReqPkt */

/*
 *
 *       Fun:   veAppRoutSendArpRspPkt
 *
 *       Desc:  This function sends the ARP response packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutSendArpRspPkt
(
CONSTANT U8  *arpReqPkt,
U32          len
)
#else
PRIVATE Void veAppRoutSendArpRspPkt (arpReqPkt, len)
CONSTANT U8  *arpReqPkt;
U32          len;
#endif
{
   U8 arpRspPkt[VE_APP_ROUT_MIN_ETH_PKT_SIZE]; /* ARP Response packet */

   VeAppEthType ethType = VE_APP_ETH_TYPE_ARP;
   VeAppARPType arpType = VE_APP_ARP_RSP;

   TRC2(veAppRoutSendArpRspPkt)

   U8 idx = 0;

   /* Fill Ethernet header */
   /* Collect sender MAC address from ARP request packet and 
      add it as destination MAC address in the Ethernet header */
   cmMemcpy(arpRspPkt, arpReqPkt + 8, VE_APP_ROUT_MAC_ADDR_LEN);
   idx += VE_APP_ROUT_MAC_ADDR_LEN;

   /* Add local MAC address as source MAC address */
   cmMemcpy(arpRspPkt + idx, veAppRoutCb.localMACAddr, 
            VE_APP_ROUT_MAC_ADDR_LEN);

   idx += VE_APP_ROUT_MAC_ADDR_LEN;

   /* Set type of Ethernet packet to ARP packet (0x0806) */
   cmMemcpy(arpRspPkt + idx, (U8 *)&ethType, 2);
   idx += 2;

   /* Construct ARP response packet (Ethernet payload) */
   /* First 6 bytes are same for ARP request and response, hence
      copy from ARP request packet */
   cmMemcpy(arpRspPkt + idx, arpReqPkt, 6);
   idx += 6;

   /* Fill the ARP response code */
   cmMemcpy(arpRspPkt + idx, (U8 *)&arpType, 2);
   idx += 2;

   /* Fill local MAC address as the sender MAC address */
   cmMemcpy(arpRspPkt + idx, veAppRoutCb.localMACAddr,
            VE_APP_ROUT_MAC_ADDR_LEN);

   idx += VE_APP_ROUT_MAC_ADDR_LEN;

   /* Collect target IP address from ARP request packet and 
      add it as sender IP address */
   /* NOTE: We should not add local IP address as sender IP address because
      we are only responding to an ARP request directed at an IP address
      allocated to the UE */
   cmMemcpy(arpRspPkt + idx, arpReqPkt + 24, VE_APP_ROUT_IP_ADDR_LEN);
   idx += VE_APP_ROUT_IP_ADDR_LEN;

   /* Collect sender IP address and MAC address from ARP request packet 
      and add them as target IP and MAC addresses */
   cmMemcpy(arpRspPkt + idx, arpReqPkt + 8, 10);
   idx += 10;

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, 
              "Sending ARP response packet to IP:%d.%d.%d.%d\n",
              arpRspPkt[38], arpRspPkt[39], arpRspPkt[40], arpRspPkt[41]));
#endif
   /* Send out ARP response packet */
   veAppRoutSendEthPkt(arpRspPkt, idx);

   RETVOID;
}/* End of veAppRoutSendArpRspPkt */


#ifdef ANSI
PRIVATE S16 veAppRoutFndDstBearerOnAddrPort
(
U32              ipAddr,
U16              port,
CmLtePdcpId      **pdcpIdInfo
)
#else
PRIVATE S16 veAppRoutFndDstBearerOnAddrPort(ipAddr, port, pdcpIdInfo)
U32              ipAddr;
U16              port;
CmLtePdcpId      **pdcpIdInfo;
#endif
{
   VeAppRouteCb *routeCb = &veAppRoutCb;
   VeAppUeRouteCb *ueRouteCb = NULLP;
   U16 routeIdx;

   /* since pcap at eNodeB not Standard Appn way, 
             is Packet filtering concept is removed */
   TRC2(veAppRoutFndDstBearerOnAddrPort)

   /* Right first get the RouteCb based on 
      destination IP Address */

   /* Loop upto latest counter Nw Addresses assigned to set of UEs */
   for (routeIdx = 0; routeIdx < routeCb->nmbUeNwAddrCnt; routeIdx++)
   {
      if (ipAddr == routeCb->ueRouteCbLst[routeIdx].nwUeAddr.addr.u.ipv4NetAddr)
      {
         /* Right Match found */
         ueRouteCb = &routeCb->ueRouteCbLst[routeIdx];  
         *pdcpIdInfo = ueRouteCb->ueInfo;
         RETVALUE(ROK);
      }/* End of UeRouteCb Match */
   } /* End of UeRoutCb Loop */

   RETVALUE(RFAILED);

}/* End of veAppRoutFndDstBearerOnAddrPort */


#ifdef ANSI
PRIVATE S16 veAppRoutFndDstBearerOnAddrProtType
(
U32                ipAddr,
CONSTANT U8        protoType,
CmLtePdcpId       **pdcpIdInfo
)
#else
PRIVATE S16 veAppRoutFndDstBearerOnAddrProtType(ipAddr, protoType, pdcpIdInfo)
U32                ipAddr;
CONSTANT U8        protoType;
CmLtePdcpId       **pdcpIdInfo;
#endif
{
   VeAppRouteCb *routeCb = &veAppRoutCb;
   VeAppUeRouteCb *ueRouteCb = NULLP;
   U16 routeIdx;
   /* since pcap at eNodeB not Standard Appn way, 
             is Packet filtering concept is removed */
   TRC2(veAppRoutFndDstBearerOnAddrProtType)

   /* Right first get the RouteCb based on 
      destination IP Address */

   /* Check if we can optimize the search in next phase */
   /* Loop upto latest counter  Nw Addresses assigned to set of UEs */
   for (routeIdx = 0; routeIdx < routeCb->nmbUeNwAddrCnt; routeIdx++)
   {
      if (ipAddr == routeCb->ueRouteCbLst[routeIdx].nwUeAddr.addr.u.ipv4NetAddr)
      {
         /* Right Match found */
         ueRouteCb = &routeCb->ueRouteCbLst[routeIdx];  

         *pdcpIdInfo = ueRouteCb->ueInfo; 
          RETVALUE(ROK);

      }/* End of UeRouteCb Match */
   } /* End of UeRoutCb Loop */

   RETVALUE(RFAILED);

}/* End of veAppRoutFndDstBearerOnAddr */


/*
 *
 *       Fun:   veAppRoutToUe
 *
 *       Desc:  This function sends the IP packet to UeSim through Uu Stack
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutToUe
(
CONSTANT U8  *ipPkt,
U32           len,
CONSTANT U8   protoType         
)
#else
PRIVATE Void veAppRoutToUe (ipPkt, len, protoType)
CONSTANT U8  *ipPkt;
U32           len;
CONSTANT U8   protoType; 
#endif
{
   CmLtePdcpId  *pdcpIdInfo = NULLP; 
   CmLtePdcpId *pdcpId; 
   VeLiSapCb   *lSapCb = NULLP;
   Buffer *mBuf = NULLP;
   U16    dstPort = 0; 
   S16    ret;
   U32    dstIPAddr;
   U32    srcIPAddr;

   TRC2(veAppRoutToUe)

   /* Find GTP-U tunnel end point details based on the destination IP address */
   dstIPAddr = (ipPkt[16] << 24) + (ipPkt[17] << 16) +
               (ipPkt[18] << 8 ) + ipPkt[19];
   
   srcIPAddr = (ipPkt[12] << 24) + (ipPkt[13] << 16) +
               (ipPkt[14] << 8 ) + ipPkt[15];
   /* Distinguish based on protocol type and port */
   /* No Transport protocol, but still IP is used. Refer to list of protocols
      above */
   if(srcIPAddr == smCfgCb.dfltExGwIpAddr)
   {
   if(VE_APP_IP_TCP != protoType && VE_APP_IP_UDP != protoType)
   {
      ret = veAppRoutFndDstBearerOnAddrProtType(dstIPAddr, protoType, &pdcpIdInfo);
   }
   else /* TCP/UDP should have the port number */
   {
      /* Peek the port */
      dstPort = (ipPkt[22] << 8 ) + ipPkt[23];
      ret = veAppRoutFndDstBearerOnAddrPort(dstIPAddr, dstPort, &pdcpIdInfo);
   }

   if (ROK != ret)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                "Can not find destination UE tunnel mapping to the \
                 IP address:%d.%d.%d.%d\n",
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      /* Failed to find the destination UE tunnel mapping to the dstIPAddr */
      RETVOID;
   }

   /* Allocate Buffer for posting to PDCP  */
   /*ret = SGetMsg(veAppRoutCb.init.region, 3, &mBuf);*/
   ret = SGetMsg(veEuCb.mem.region, 3, &mBuf);
   if (ROK != ret)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            "Failed to allocate mBuf - %d\n", ret));
      RETVOID;
   }
   /* Add the received IP Pkt buffer to to mBuf */
   SAddPstMsgMult((Data *)ipPkt, (MsgLen)len, mBuf);

   ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool,
         (Data **)&pdcpId, sizeof(CmLtePdcpId)); 
   if (ret != ROK)
   {
      /* Log error */
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            "Failed to allocate pdcpId - %d\n", ret));
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   lSapCb = veEuCb.lsapLst[VE_PERF_PDCP_SAP_ID];     
   pdcpId->cellId =  pdcpIdInfo->cellId;
   pdcpId->ueId   =  pdcpIdInfo->ueId;
   pdcpId->rbId   =  pdcpIdInfo->rbId; 
   pdcpId->rbType =  CM_LTE_DRB;

   VeLiPjuDatReq(&lSapCb->pst, lSapCb->spId, pdcpId,
               veEuCb.sduCntr++, mBuf);
   }
   if (veEuCb.sduCntr > 500000)
   {
      veEuCb.sduCntr = 1;
#ifndef SS_SEUM_CAVIUM
   /* SRegInfoShow(0, &memInfo);*/
#endif
   }
   
   RETVOID;
}/* End of veAppRoutToUe */

/*
 *
 *       Fun:   veAppRoutPktReceiver
 *
 *       Desc:  This function receives packets from network interface
 *
 *       Ret:   NULLP
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PUBLIC Void *veAppRoutPktReceiver
(
Void *arg
)
#else
PUBLIC Void *veAppRoutPktReceiver (arg)
Void *arg;
#endif
{
   /* Added trace macro */
   TRC2(veAppRoutPktReceiver)
   
   /* Read packets from Ethernet interface and invoke veAppRoutRcvdPktHndlr
      callback function for every packet read */
   pcap_loop(veAppRoutCb.pcapHndl, -1, veAppRoutRcvdPktHndlr, NULLP);

   RETVALUE(NULLP);
}/* End of veAppRoutPktReceiver */



/*
 *
 *       Fun:   veAppRoutRcvdPktHndlr
 *
 *       Desc:  This is the callback function of pcap_loop
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutRcvdPktHndlr
(
u_char                   *args,
CONSTANT struct pcap_pkthdr *hdr,
CONSTANT u_char             *pktData
)
#else
PRIVATE Void veAppRoutRcvdPktHndlr (args, hdr, pktData)
u_char                   *args;
CONSTANT struct pcap_pkthdr *hdr;
CONSTANT u_char             *pktData;
#endif
{
   U16 pktType;
   U32 bytesRcvd;

   TRC2(veAppRoutRcvdPktHndlr)

   bytesRcvd = hdr->caplen;

   /* Find out the type of Ethernet packet (bytes 12-13 in Ethernet header),
      handle IP and ARP packets */
   pktType = (pktData[13] << 8) + pktData[12];
   if (pktType == VE_APP_ETH_TYPE_IP)
   {
      /* Pass the IP packet, exclude Ethernet header 
         portion of first 14 bytes */
      veAppRoutHndlIPPkt(pktData + 14, bytesRcvd - 14);
   }
   else if (pktType == VE_APP_ETH_TYPE_ARP)
   {
      /* Pass the ARP packet, exclude Ethernet header 
         portion of first 14 bytes */
      veAppRoutHndlARPPkt(pktData + 14, bytesRcvd - 14);
   }
   else
   {
      /* Unknown packet type received */
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Unknown packet type received - %d\n", pktType));
   }

   RETVOID;
}/* End of veAppRoutRcvdPktHndlr */



/*
 *
 *       Fun:   veAppRoutHndlARPPkt
 *
 *       Desc:  This function handles the received ARP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutHndlARPPkt
(
CONSTANT U8  *arpPkt,
U32          bytesRcvd
)
#else
PRIVATE Void veAppRoutHndlARPPkt (arpPkt, bytesRcvd)
CONSTANT U8  *arpPkt;
U32          bytesRcvd;
#endif
{
   U16 opcode;

   TRC2(veAppRoutHndlARPPkt)

   /* Operation fiels in the ARP packet indicates whether its an
      ARP request or ARP response, handle accordingly */
   opcode = (arpPkt[7] << 8) + arpPkt[6];
   if (opcode == VE_APP_ARP_REQ)
   {
      /* Handle ARP Request */
      veAppRoutHndlARPRequest(arpPkt, bytesRcvd);
   }
   else if (opcode == VE_APP_ARP_RSP)
   {
      /* Handle ARP Response */
      veAppRoutHndlARPResponse(arpPkt, bytesRcvd);
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Unknown ARP Operation - %d\n", opcode));
   }

   RETVOID;
}/* End of veAppRoutHndlARPPkt */


/*
 *
 *       Fun:   veAppRoutHndlARPRequest
 *
 *       Desc:  This function handles the ARP request packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutHndlARPRequest
(
CONSTANT U8  *arpPkt,
U32          bytesRcvd
)
#else
PRIVATE Void veAppRoutHndlARPRequest (arpPkt, bytesRcvd)
CONSTANT U8  *arpPkt;
U32          bytesRcvd;
#endif
{
   U32 targetIPAddr;
   S16 ret;

   TRC2(veAppRoutHndlARPRequest)

   targetIPAddr = (arpPkt[24] << 24) + (arpPkt[25] << 16) +
                  (arpPkt[26] << 8) + arpPkt[27];

   /* Check the target IP address. If the target IP is one of our IP addresses
      in the veAppRoutUeCb, send ARP response with local MAC address.
      Otherwise simply ignore the ARP request packet */
   ret = veAppRoutFndDstBearerOnAddr(targetIPAddr);
   if (ROK != ret)
   {
#ifdef DEBUGP
      /* printf("Received ARP request for IP address:%d.%d.%d.%d Out of \
                      range\n", arpPkt[24], arpPkt[25], 
                      arpPkt[26], arpPkt[27])); */
#endif
      /* Not our IP address, just return */
      RETVOID;
   }

   /* We should further check if the ARP request has come from our own MAC
      address. We should respond only if it is NOT from our MAC address */
   ret = cmMemcmp(arpPkt + 8, veAppRoutCb.localMACAddr,
                  VE_APP_ROUT_MAC_ADDR_LEN);

   if (ROK != ret)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
              "Received ARP request for IP address:%d.%d.%d.%d\n", 
              arpPkt[24], arpPkt[25], arpPkt[26], arpPkt[27]));
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                          "ARP request is for one of our IP addresses\n"));
#endif
      /* ARP request is not from our MAC address, send ARP response */
      veAppRoutSendArpRspPkt(arpPkt, bytesRcvd);
   }

   RETVOID;
}/* End of veAppRoutHndlARPRequest */

/*
 *
 *       Fun:   veAppRoutHndlARPResponse
 *
 *       Desc:  This function handles the ARP response packet
 *
 *       Ret:   None
 *
 *       Notes: IP address conflict is not handled in this release
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutHndlARPResponse
(
CONSTANT U8  *arpPkt,
U32          bytesRcvd
)
#else
PRIVATE Void veAppRoutHndlARPResponse (arpPkt, bytesRcvd)
CONSTANT U8  *arpPkt;
U32          bytesRcvd;
#endif
{
   VeAppRouteCb *routeCb = &veAppRoutCb;
   U32 dstIPAddr;
   U8  primaryMac[VE_APP_MAX_NW_NODES] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
   S16 ret;

   TRC2(veAppRoutHndlARPResponse)

#ifdef DEBUGP
       /*  printf("Received ARP response for:%d.%d.%d.%d\n", arpPkt[14],
               arpPkt[15], arpPkt[16], arpPkt[17]); */
#endif
   /* If we receive ARP Response, more than we support, simply return */
   if (routeCb->mapCnt >= VE_APP_MAX_NW_NODES)
   {
      RETVOID;
   }

   dstIPAddr = (arpPkt[14] << 24) + (arpPkt[15] << 16) + 
               (arpPkt[16] << 8) + arpPkt[17];

   if (dstIPAddr != veAppRoutexGwCfg.u.ipv4NetAddr)
   {
      RETVOID;
   }
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                          "Received ARP response for Gateway IP address\n"));
#endif
      ret = cmMemcmp(routeCb->ip2Mac[VE_APP_PRIMARY_GW_IDX].macAddr, 
                     primaryMac,
                        VE_APP_ROUT_MAC_ADDR_LEN);
      if(ret == ROK) /* If No MAC is set */
      {
         /* Just update the MAC Address for primary destination 
            external network node */ 
         cmMemcpy(routeCb->ip2Mac[VE_APP_PRIMARY_GW_IDX].macAddr, arpPkt + 8,
                     VE_APP_ROUT_MAC_ADDR_LEN);
      }

   RETVOID;

}/* End of veAppRoutHndlARPResponse */


/*
 *
 *       Fun:   veAppRoutFndDstBearerOnAddr
 *
 *       Desc:  This function finds an UE associated data bearer based on IP Address
 *              assigned to UE
 *
 *       Ret:   
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */

#ifdef ANSI
PRIVATE S16 veAppRoutFndDstBearerOnAddr
(
U32              ipAddr
)
#else
PRIVATE S16 veAppRoutFndDstBearerOnAddr(ipAddr)
U32              ipAddr;
#endif
{
   VeAppRouteCb *routeCb = &veAppRoutCb;
   VeAppUeRouteCb *ueRouteCb = NULLP;
   U16 routeIdx;

   TRC2(veAppRoutFndDstBearerOnAddr)

   /* Right first get the RouteCb based on 
      destination IP Address */

   /* Check if we can optimize the search in next phase */
   /* Loop upto latest counter Nw Addresses assigned to set of UEs */
   for (routeIdx = 0; routeIdx < routeCb->nmbUeNwAddrCnt; routeIdx++)
   {
      if (ipAddr == routeCb->ueRouteCbLst[routeIdx].nwUeAddr.addr.u.ipv4NetAddr)
      {
         /* Right Match found */
         ueRouteCb = &routeCb->ueRouteCbLst[routeIdx];  
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);

}
/*
 *
 *       Fun:   veAppRoutHndlIPPkt
 *
 *       Desc:  This function handles the received IP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutHndlIPPkt
(
CONSTANT U8  *ipPkt,
U32           bytesRcvd
)
#else
PRIVATE Void veAppRoutHndlIPPkt (ipPkt, bytesRcvd)
CONSTANT U8  *ipPkt;
U32           bytesRcvd;
#endif
{
   U8  protoType;

   TRC2(veAppRoutHndlIPPkt)

#ifdef DEBUGP
/*   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
               "Received IP packet for : %d.%d.%d.%d\n", ipPkt[16],
               ipPkt[17], ipPkt[18], ipPkt[19])); */
#endif
   /* Get the protocol type from the IP header */
   protoType = ipPkt[9];
#ifdef DEBUGP
/*    VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending IP packet to Pdcp\n"));*/
#endif
      /* Send the IP packet to Ue */
      veAppRoutToUe(ipPkt, bytesRcvd, protoType);

   RETVOID;
}/* End of veAppRoutHndlIPPkt */

/*
 *
 *       Fun:   veAppRoutInitAdaptor
 *
 *       Desc:  This function opens Ethernet adaptor for capturing packets
 *              and creates a thread for capturing packets
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 veAppRoutInitAdaptor
(
Void
)
#else
PRIVATE S16 veAppRoutInitAdaptor (Void)
#endif
{
   S8   errBuf[PCAP_ERRBUF_SIZE];

   TRC2(veAppRoutInitAdaptor)

   /* Open the Ethernet Adaptor with LibPCAP  */
   veAppRoutCb.pcapHndl = pcap_open_live(veAppRoutCb.ethIntf, 
                                        VE_APP_ROUT_PCAP_BUF_SIZE,
                                        TRUE, VE_APP_ROUT_PCAP_TIMEOUT, errBuf);
   if (NULLP == veAppRoutCb.pcapHndl)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                            "Error opening network adaptor\n"));
      RETVALUE(RFAILED);
   }
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                    "Initialised Ethernet interface for capturing packets\n"));
#endif
   RETVALUE(ROK);
}/* End of veAppRoutInitAdaptor */


/*
 *
 *       Fun:   veAppRoutBuildEthPkt
 *
 *       Desc:  This function encapsulate IP packet in to Ethernet packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutBuildEthPkt
(
U8   *ipPkt, 
U32   len
)
#else
PRIVATE Void veAppRoutBuildEthPkt (ipPkt, len)
U8   *ipPkt; 
U32   len;
#endif
{
   U32  dstIPAddr;
   U8  *dstMACAddr;
   VeAppRouteCb *routeCb = &veAppRoutCb;

   TRC2(veAppRoutBuildEthPkt)
   
   /* Copy IP packet into Ethernet payload */
   cmMemcpy(routeCb->ethPkt + 14, ipPkt, len);

   /* Find out the destination MAC address using destination IP address */
   dstIPAddr = (routeCb->ethPkt[30] << 24) + (routeCb->ethPkt[31] << 16) +
               (routeCb->ethPkt[32] << 8) + routeCb->ethPkt[33];

   /* Search the MAC map for destination MAC address */
   dstMACAddr = veAppRoutFindMACAddress(dstIPAddr);

   /* Copy the destination MAC address */
   cmMemcpy(routeCb->ethPkt, dstMACAddr, VE_APP_ROUT_MAC_ADDR_LEN);

   RETVOID;
}/* End of veAppRoutBuildEthPkt */

/*
 *
 *       Fun:   veAppRoutCalcIPChecksum
 *
 *       Desc:  This function calculates the 16 bit IP header checksum
 *
 *       Ret:   U16, checksum
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE U16 veAppRoutCalcIPChecksum
(
U8   *ipPkt, 
U32   ipHdrLen
)
#else
PRIVATE U16 veAppRoutCalcIPChecksum (ipPkt, ipHdrLen)
U8   *ipPkt; 
U32   ipHdrLen;
#endif
{
   U16 word;
   U16 idx;
   U32 chksum = 0;

   TRC2(veAppRoutCalcIPChecksum)

   /* Make 16 bit words out of every two adjacent 8 bit octets in
      the packet and add them up */
   for (idx = 0; idx < ipHdrLen; idx += 2)
   {
      word = (((U16)ipPkt[idx] << 8) & 0xFF00) + (ipPkt[idx + 1] & 0xFF);
      chksum += (U32)word;
   }

   /* Take only 16 bits out of the 32 bit checksum */
   while (chksum >> 16)
   {
      chksum = (chksum & 0xFFFF) + (chksum >> 16);
   }

   /* One's complement the result */
   chksum = ~chksum;

   /* Return the 16 bit checksum */
   RETVALUE((U16)chksum);
}/* End of veAppRoutCalcIPChecksum */

/*
 *
 *       Fun:   veAppRoutFragmentIPPkt
 *
 *       Desc:  This function fragments the IP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void veAppRoutFragmentIPPkt
(
U8  *ipPkt, 
U32  len
)
#else
PRIVATE Void veAppRoutFragmentIPPkt (ipPkt, len)
U8  *ipPkt; 
U32  len;
#endif
{
   U8  *ipHdr;
   U32 ipHdrLen;
   U32 bytesSent;
   U8  ipPacket[VE_APP_ROUT_MAX_DATA_SIZE];
   U32 bytesRemaining;
   U32 maxIPDatSize;
   U32 fragOffset;
   U32 totLen;
   U32 dataSent;
   U16 ipChksum;

   TRC2(veAppRoutFragmentIPPkt)
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, 
                       "Fragmenting IP packet of length - %ld\n", len));
#endif
   /* Initialise variables */
   ipHdr = ipPkt; /* Copy IP header pointer */
   ipHdrLen = (ipPkt[0] & 0xF) * 4;
   bytesSent = ipHdrLen;
   bytesRemaining = len - bytesSent;
   maxIPDatSize = VE_APP_ROUT_MAX_DATA_SIZE - ipHdrLen;
   fragOffset = 0;

   /* Loop for fragmenting the IP packets */
   while (bytesRemaining > 0)
   {
      /* Copy the original IP header to IP fragment and modify 
         the header to reflect fragmentation */
      cmMemcpy(ipPacket, ipHdr, ipHdrLen); 

      /* Set more flag in the IP header if this is not the last IP fragment */
      if (bytesRemaining > maxIPDatSize)
      {
         ipPacket[6] |= (0x1 << 5);
      }
      
      /* Set fragment offset */
      ipPacket[6] |= (U8)((fragOffset >> 8) & 0x1F);
      ipPacket[7] = (U8)(fragOffset & 0xFF);

      if (bytesRemaining > maxIPDatSize)
      {
         totLen = VE_APP_ROUT_MAX_DATA_SIZE;
      }
      else
      {
         totLen = bytesRemaining + ipHdrLen;
      }

      dataSent = totLen - ipHdrLen;
      if (bytesRemaining > maxIPDatSize)
      {
         U32 bytesToSend;

         /* If there are more fragments, send multiple of 8 octets as data */
         bytesToSend = dataSent % 8;
         dataSent -= bytesToSend;
         totLen -= bytesToSend;
      }

      /* Change total length */
      ipPacket[2] = (U8)((totLen >> 8) & 0xFF);
      ipPacket[3] = (U8)(totLen & 0xFF);

      /* Calculate the IP header checksum */
      ipChksum = veAppRoutCalcIPChecksum(ipPacket, ipHdrLen);

      /* Copy new IP header checksum */
      ipPacket[10] = (ipChksum >> 8) & 0xFF;
      ipPacket[11] = ipChksum & 0xFF;

      /* Copy the data to IP packet */
      cmMemcpy(ipPacket + ipHdrLen, ipHdr + bytesSent, dataSent);

      /* Encapsulate IP packet in an Ethernet packet */
      veAppRoutBuildEthPkt(ipPacket, totLen);

#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,
                "Sending IP fragment from %d.%d.%d.%d to %d.%d.%d.%d \
                 of length = %d\n", ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15],
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19], totLen));
#endif
      /* Send out Ethernet packet */
      veAppRoutSendEthPkt(veAppRoutCb.ethPkt, totLen + 14);
      
      /* Update variables */
      bytesSent += dataSent;
      bytesRemaining -= dataSent;
      fragOffset += ((dataSent + 7) / 8);
   }

   RETVOID;
}/* End of veAppRoutFragmentIPPkt */

/*
 *
 *       Fun:   veAppRoutHndlSndPkt
 *
 *       Desc:  This function handles the IP packets received from Pdcp
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PUBLIC Void veAppRoutHndlSndPkt
(
Buffer *mBuf
)
#else
PUBLIC Void veAppRoutHndlSndPkt (mBuf)
Buffer *mBuf;
#endif
{
   U8     ipPkt[VE_APP_ROUT_MAX_ETH_PKT_LEN]; 
   MsgLen len;
   MsgLen cnt;
   U32    srcIPAddr;
   U16    srcPort = 0;
   U8     protoType;
   S16    ret;

   TRC2(veAppRoutHndlSndPkt)

   /* Find the length of the received IP packet */
   SFndLenMsg(mBuf, &len);

   if(len > VE_APP_ROUT_MAX_ETH_PKT_LEN)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                     "Failed to send the Packet as it is exceeding,\
                     Maximum Ethernet Packet Size Size of Packet-%d\n",len));
      RETVOID;
   }

   /* Convert the mBuf to U8 array */
   SCpyMsgFix(mBuf, 0, len, ipPkt, &cnt);

   /* Get the source port */
   srcPort = (ipPkt[20] << 8 ) | ipPkt[21];

   /* Get the source address */
   srcIPAddr = (ipPkt[12] << 24) + (ipPkt[13] << 16) +
               (ipPkt[14] << 8) + ipPkt[15];

   /* Get the protocol type */
   protoType = ipPkt[9];
   ret = veAppRoutUpdUeRouteCbPort(srcIPAddr, srcPort, protoType);
   if(ROK != ret)
   {
    /* We didnt get the matching UE IP which was assigned earlier to forward */
      VE_DBG_ERROR((VE_PRNT_BUF,
                      "Failed to get the intended IP Address match assigned,\
                      to this UE. Received IP from UE %d.%d.%d.%d, Port(%d)\n",
                      ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15], srcPort));
      RETVOID;
   }
   
   /* Check if this port falls in the range of the ports which was initially
       assigned as part of packet filter cb. */
   /* If within range just send the packet to external network, else
      update our packet filter control block */
   
   /* Fragment the IP packet if its length is greater than the 
      Maximum data size */
   if (cnt < VE_APP_ROUT_MAX_DATA_SIZE)
   {
      /* Encapsulate the IP packet in an Ethernet packet */
      veAppRoutBuildEthPkt(ipPkt, cnt);
            
      VE_DBG_ERROR((VE_PRNT_BUF,
                 "Sending IP packet from %d.%d.%d.%d to %d.%d.%d.%d\n",
                 ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15],
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      /* Send out Ethernet packet */
      veAppRoutSendEthPkt(veAppRoutCb.ethPkt, cnt + 14);
   }
   else
   {
      /* Fragment IP packet */
      veAppRoutFragmentIPPkt(ipPkt, cnt);
   }

   RETVOID;
}/* End of veAppRoutHndlSndPkt */



/*
 *
 *       Fun:  veAppRoutUpdUeRouteCbPort  
 *
 *       Desc: This function updates a routing control block 
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 veAppRoutUpdUeRouteCbPort
(
U32      ipAddr,
U16      port,
U8       protoType
)
#else
PRIVATE S16 veAppRoutUpdUeRouteCbPort(ipAddr, port, protoType)
U32      ipAddr;
U16      port;
U8       protoType;
#endif
{
   VeAppRouteCb *routeCb = &veAppRoutCb;
   VeAppUeRouteCb *ueRouteCb = NULLP;
   U16 routeIdx;

   TRC2(veAppRoutUpdUeRouteCbPort)

   /* Right first get the RouteCb based on 
      destination IP Address */

   /* Loop upto latest counter Nw Addresses assigned to set of UEs */
   for (routeIdx = 0; routeIdx < routeCb->nmbUeNwAddrCnt; routeIdx++)
   {
      if (ipAddr == routeCb->ueRouteCbLst[routeIdx].nwUeAddr.addr.u.ipv4NetAddr)
      {
         /* Right Match found */
         ueRouteCb = &routeCb->ueRouteCbLst[routeIdx];  

         /* Packet filter concept is removed */

         /* Next match on packet filter context */
         /* Now the criterion is based on:
            Destination/remote port + Port Range which we serve to UEs */
         /* Remote Port and Remote Port range is a packet component within
            a packet filter context */

         RETVALUE(ROK);
      }/* End of UeRouteCb Match */
   } /* End of UeRoutCb Loop */

   RETVALUE(RFAILED);

}/* End of veAppRoutUpdUeRouteCbPort */



/*
 *
 *       Fun:   veAppRoutFindMACAddress
 *
 *       Desc:  This function finds the MAC address corresponding to an
 *              IP address from the IP->MAC map
 *
 *       Ret:   Pointer to a MAC address (U8 *)
 *
 *       Notes: none
 *
 *       File:  ve_rout.c
 *
 */
#ifdef ANSI
PRIVATE U8 * veAppRoutFindMACAddress
(
U32 ipAddr
)
#else
PRIVATE U8 * veAppRoutFindMACAddress (ipAddr)
U32 ipAddr;
#endif
{
   U8    idx;

   TRC2(veAppRoutFindMACAddress)

   /* search for MAC address mapped to the ipAddr */
   for (idx = 0; idx < veAppRoutCb.mapCnt; idx++)
   {
      if (ipAddr == veAppRoutCb.ip2Mac[idx].ipAddr)
      {
         RETVALUE(veAppRoutCb.ip2Mac[idx].macAddr); 
      }
   }

   /* Retrun Gateway MAC address if no MAC address is found in the map */
   RETVALUE(veAppRoutCb.ip2Mac[VE_APP_PRIMARY_GW_IDX].macAddr);

}/* End of veAppRoutFindMACAddress */
#endif /* VE_LNX_PCAP_PERF */ 
/**********************************************************************
  
         End of file:     ve_rout.c@@/main/1 - Wed Dec  1 08:44:29 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/


