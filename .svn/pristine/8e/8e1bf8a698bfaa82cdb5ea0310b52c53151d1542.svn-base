#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include <netinet/ether.h>


#include "lte_bb_bist.h"
#include "lte_bb_nor_util.h"


/* locals */
static SINT32           pingSockFd = -1;
static in_addr_t        bindAddr;
SINT32                  icmpseq=0;

/* locals */
SINT8 * cmdTable [] =
    {
    "h",
    "gm",

    "sm",
    "sb",
    "gb",
    };


unsigned short in_cksum(unsigned short * , SINT32 );
static SINT32 setupSocket (in_addr_t ifAddr)
{
    struct sockaddr_in  source;

   if (ifAddr != bindAddr)
   {
       /* source address changed. lets reopen the socket and bind to the
        * new ip address.
        */
       if (pingSockFd != -1)
       {
          close (pingSockFd);
          pingSockFd = BIST_ERROR;
       }
   }

   if (pingSockFd == -1)
   {
       if ((pingSockFd = socket (AF_INET, SOCK_RAW, PROTO_ICMP)) < 0)
       {
          perror("socket:");
          return BIST_ERROR;
       }
   }
   /* bind to the socket to using the interface IP address */
   if (ifAddr != bindAddr)
   {   
       source.sin_family = AF_INET;
       source.sin_addr.s_addr = ifAddr;
       source.sin_port = 0;

        if (bind (pingSockFd, (struct sockaddr *)&source,
                  sizeof (source)) == -1 )
        {
           perror("bind:");
           close (pingSockFd);
           pingSockFd = -1;
           return BIST_ERROR;
        }
   }

   bindAddr = ifAddr;
   return (0);
}

/*******************************************************************************
*
* doPing - send an ICMP echo request to given address and wait for reply
*
* RETURNS: 0 for success , -1 for failure
*
*/
int doPing
    (
    in_addr_t   ifAddr,     /* interface IP address */
    in_addr_t   destAddr,   /* destination IP address */
    FILE *  pFp             /* file pointer to write output */
    )
{
    struct icmp *       pPingPkt;
    char                buff[PING_SND_BUF_SZ];
    char                packet[PING_RCV_BUF_SZ];
    struct sockaddr_in  dest;
    struct sockaddr     from;
    struct timeval      timeout;
    time_t              stopTime;
    time_t              curTime;
    fd_set              readSet;
    struct iphdr *      ip;
    SINT32              ipHlen;
    SINT32              status;
    socklen_t           fromLen;
    SINT32              n;
    SINT32              result;

    memset(packet, 0, sizeof(packet));
    if (setupSocket (ifAddr) == -1)
        {
        /* there is no meaning in continuing if we are not able to setup
         * the socket.
         */
        return -1;
        }

    pPingPkt = (struct icmp *)buff;
    pPingPkt->icmp_type = ICMP_ECHO;
    pPingPkt->icmp_code = 0;
    pPingPkt->icmp_cksum = 0;
    pPingPkt->icmp_seq = icmpseq++;
    pPingPkt->icmp_id = getpid() & 0xFFFF;

    pPingPkt->icmp_cksum = in_cksum ((unsigned short *)pPingPkt,
                                     sizeof(buff));

    bzero((void *)&dest, sizeof(dest));
    dest.sin_addr.s_addr = destAddr;
    dest.sin_family = AF_INET;

    status = sendto (pingSockFd, buff, sizeof(buff), 0,
                     (struct sockaddr *)&dest, sizeof (struct sockaddr));

    stopTime = time (NULL) + PING_TIMEOUT + 1;
    while (stopTime > (curTime = time (NULL)))
    {
        FD_ZERO (&readSet);
        FD_SET (pingSockFd, &readSet);

        timeout.tv_sec = stopTime - curTime;
        timeout.tv_usec = 0;

        result = select (pingSockFd + 1, &readSet, NULL, NULL, &timeout);
        if (result == 0)
        {
            fprintf (stderr, "Timeout has occured \n");
            return BIST_ERROR;   /* timeout has occured */    
        }

        if (result < 0)
        {
            if (errno == EINTR)
                continue;
            fprintf (stderr, "select: %s\n", strerror (errno));
            return BIST_ERROR;
        }

        fromLen = sizeof (from);
        n = recvfrom (pingSockFd, packet, sizeof(packet), 0,
                      (struct sockaddr *) &from, &fromLen);
        if (n <= 0)
        {
            fprintf (stderr, "recvfrom: %s\n", strerror (errno));
            return BIST_ERROR;
        }

        ip = (struct iphdr *)packet;

        ipHlen = ip->ihl << 2;

        if (n < ipHlen + ICMP_MINLEN)
            continue;

        pPingPkt = (struct icmp *) (packet + ipHlen);

        if (pPingPkt->icmp_type == ICMP_ECHOREPLY &&
            pPingPkt->icmp_id == (getpid() & 0xFFFF))
        {
            BIST_DBG_LOG("matched icmp Echo reply \n");
            return 0;
        }
    } /* while */

    return BIST_ERROR;
}


/*
 * in_cksum --
 *  Checksum routine for Internet Protocol family headers (C Version)
 */
unsigned short in_cksum(unsigned short * addr, int len)
{
    SINT32          nleft = len;
    unsigned short *w = addr;
    SINT32          sum = 0;
    unsigned short  answer = 0;

    /*
    * Our algorithm is simple, using a 32 bit accumulator (sum), we add
    * sequential 16 bit words to it, and at the end, fold back all the
    * carry bits from the top 16 bits into the lower 16 bits.
    */
    while (nleft > 1)  
    {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if (nleft == 1) 
    {
        *(u_char *)(&answer) = *(u_char *)w ;
        sum += answer;
    }

    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
    sum += (sum >> 16);         /* add carry */
    answer = ~sum;              /* truncate to 16 bits */
    return(answer);

}

/*******************************************************************************
*
* EtherPotTest -  checks ethernet interface flags for given ethenet interface and 
                  send an ICMP echo request to given address and wait for reply
*
* RETURNS: 0 for success , -1 for failure
*
*/
EtherPotTest(char *pEthername)
{
   int sock = -1;
   struct ifreq ifr;
   u_int32_t  ifAddr;
   u_int32_t  ifAddrTest;
   char       ipAddr[16];

   
   if(pEthername == NULL)
      return BIST_ERROR;
  
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0)
   {
        BIST_DBG_LOG("Socket creation Failed"); 
        return BIST_ERROR;
   } 

   memset(&ifr, 0, sizeof(ifr));
   strncpy(ifr.ifr_name, pEthername, sizeof(ifr.ifr_name)-1);

   /* interface IP address */
   if (ioctl(sock, SIOCGIFADDR, &ifr) >= 0)
   {
       /* if addresses are not set ioctl returns error. So we ignore it */
       memcpy(&ifAddr, &SINADDROF(&ifr.ifr_addr), 
               sizeof(ifAddr));
       memset(ipAddr,0,16);
       strncpy(ipAddr,inet_ntoa(*(struct in_addr *)&ifAddr),sizeof(ipAddr)-1);
 
       BIST_DBG_LOG("ioctl sucess  [%x] [%s] \n",ifAddr,ipAddr);
   }
   else
   {
      BIST_DBG_LOG("Socket creation Failed"); 
      return BIST_ERROR;
   } 

    /* interface flags */
   if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
   {
      BIST_DBG_LOG("Getting interface flags  Failed"); 
      return BIST_ERROR;
   } 
   BIST_DBG_LOG("Interface flags are [%x]  UP Flags [%x ]RUNNING flag [%x] \n",ifr.ifr_flags,ifr.ifr_flags& IFF_UP,ifr.ifr_flags& IFF_RUNNING);

   if(ifr.ifr_flags& IFF_UP == 0)
   {
      BIST_DBG_LOG("Interface Not up ,exiting\n"); 
      return BIST_ERROR;
   }
 
   if (doPing (ifAddr, ifAddr, NULL) < 0)
   {
      BIST_DBG_LOG("Ping  failed for ip addess [%s]\n",ipAddr); 
      return BIST_ERROR;
   }
   else
   {
      return BIST_SUCCESS;
      BIST_DBG_LOG("Ping  Sucess for  ip addess [%s]\n",ipAddr); 
   }

}


/****************************************************************************
*  Function:    SetBistStatus(UINT8 status)
*
*  Description:
*
*  Prototype:   ReturnCode SetBistStatus(UINT8 status)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode SetBistStatus(UINT8 status)
{

   UINT16  offSet;
   UINT16  fileDescriptor;

   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return RWNORFLASHRETURNCODE_FAILURE;
   }

   /* caluculate the offset */
   offSet = STRUCT_OFFSET(NorData,bistStatus);

   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET))
   {
      return RWNORFLASHRETURNCODE_FAILURE;
   }

   if ((write(fileDescriptor,(char *)&status,sizeof(unsigned char))) < 0 )
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE;
   }

   close(fileDescriptor);
   return RWNORFLASHRETURNCODE_SUCCESS;
}

/****************************************************************************
*  Function:   GetBistStatus()
* 
*  Description:
*
*  Prototype:   GetBistStatus(void)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode GetBistStatus(unsigned char *pStatus)
{
   UINT16  offSet;
   UINT16  fileDescriptor;
   unsigned char Status;

   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return RWNORFLASHRETURNCODE_FAILURE;
   }

   /* caluculate the offset */
   offSet = STRUCT_OFFSET(NorData,bistStatus);

   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET))
   {
      return RWNORFLASHRETURNCODE_FAILURE;
   }

   if ((read(fileDescriptor,pStatus,sizeof(Status))) < 0 )
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE;
   }
   

   close(fileDescriptor);
   return RWNORFLASHRETURNCODE_SUCCESS;
}

/*******************************************************************************
*
* main - 
*
* This routine
*
* RETURNS: pointer to RESULT_SET or NULL
*/
int bistmain
    (
    int     argc,
    char ** ppArgv
    )
    {

    char * c = "h";
    int cmd = 0;

    if (argc > 1)
        c = ppArgv[1];

    while (cmd < sizeof(cmdTable))
        {
        if (strcmp(cmdTable[cmd], c) == 0)
            break;
        cmd++;
        }

    switch(cmd)
        {
        case 0: /* "h" */
            {
            /* help on db */
            printf("Usage: %s {gm | sm | sb | gb }\n",ppArgv[0]);
            printf("\t r   - reset \n\n");

            printf("\t gm  - get mac address ,Give 80 for eth0 ,81 for eth1 <port num >\n");
            printf("\t sm  - set mac address ,Give 80 for eth0 ,81 for eth1 <mac address > < port>\n");

            printf("\t sb  - Enable BIST \n");
            printf("\t gb  - Get BIST Status\n");


            return 0;
            }
        case 1: /* "gm" */
        {
             struct ether_addr mac_Address;
             int portNum; 
             char  * pData =NULL;
 
             if(atoi(ppArgv[2]) != 80 && atoi(ppArgv[2])!= 81)
             printf("Invalid port num");

             portNum=atoi(ppArgv[2]);
 
             if(getMacAddress(&mac_Address,portNum) == SETGETMACADDRESSRETURNCODE_SUCCESS)
             {
                 pData = ether_ntoa((struct ether_addr *)&mac_Address);

                 if(pData != NULL)
                 printf("Mac address is %s\n",pData);
             }
             return 0;
        }
        case 2: /* "sm" */
        {
            
             int portNum; 

             if(atoi(ppArgv[3]) != 80 && atoi(ppArgv[3])!=81)
             printf("Invalid port num");

             portNum=atoi(ppArgv[3]);

             if (ether_aton(ppArgv[2]) == NULL)
             {
               printf("Invalid mac address \n");
               return -1;
             }
             printf("port is %d and mac [%s] \n",portNum,ppArgv[2]); 

             if(setMacAddress(ppArgv[2],portNum) == SETGETMACADDRESSRETURNCODE_SUCCESS)
             {
                 printf("Mac address Set succesfully \n");
             }
            return 0;
        }     
        case 3: /* "sb" */
        {
            unsigned char status;

            if(ppArgv[2] <0)
               printf(" Invalid Status \n");

            status = atoi(ppArgv[2]);
     
            if(SetBistStatus(status) == RWNORFLASHRETURNCODE_SUCCESS)
            {
               printf(" Bist status Set suceesfully \n");
            }
            return 0;
        } 
        case 4:
       {
            unsigned char * pStatus;
            if(GetBistStatus(pStatus) == RWNORFLASHRETURNCODE_SUCCESS)
            {
               printf(" Bist status is [%x] \n",*pStatus);
            } 
            return 0;
       }

        }

    return 0;
    }

/****************************************************************************
*  Function:   bistTestStart()
* 
*  Description:
*
*  Prototype:   bistReturnCode bistTestStart (BistTestType bistTestType , EtherTestType etherTestType)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
BistReturnCode bistTestStart(BistTestType bistTestType , EtherTestType etherTestType) 
{
   UINT8 statusDebugPort;
   UINT8 statusBhPort;

    if (bistTestType > BISTTESTTYPE_ALL || etherTestType  > ETHERTESTYPE_ALL )
    {
       return BISTRETURNCODE_INVALID_PARAMETERS;
    }
 
   if(bistTestType == BISTTESTTYPE_BACKHAUL_ETHERNET_PORT || bistTestType == BISTTESTTYPE_DEBUG_ETHERNET_PORT || bistTestType == BISTTESTTYPE_ALL)
   {
       /* run ethernet test on backhaul ethernet port */
       if(bistTestType == BISTTESTTYPE_BACKHAUL_ETHERNET_PORT)
       {
          statusBhPort= EtherPotTest(BACKHUAL_ETHERNET_PORT);
       }
       if(statusBhPort == BIST_ERROR )
       {
          return BISTRETURNCODE_FAILURE;
       }
       else
       {
          return BISTRETURNCODE_SUCCESS;
       }

       /* run ethernet test on debug ethernet port */
       if(bistTestType == BISTTESTTYPE_DEBUG_ETHERNET_PORT )
       {
          statusDebugPort = EtherPotTest(DEBUG_ETHERNET_PORT);
       } 

       if(statusDebugPort == BIST_ERROR )
       {
          return BISTRETURNCODE_FAILURE;
       }
       else
       {
          return BISTRETURNCODE_SUCCESS;
       }


       /* run ethernet test on both ports */
       if(bistTestType == BISTTESTTYPE_ALL)
       {
          statusBhPort= EtherPotTest(BACKHUAL_ETHERNET_PORT);
          statusDebugPort= EtherPotTest(DEBUG_ETHERNET_PORT);

          if(statusBhPort == BIST_ERROR || statusDebugPort == BIST_ERROR )
          {
             return BISTRETURNCODE_FAILURE;
          }
          else
          {
             return BISTRETURNCODE_SUCCESS;
          }
          
       } 
   }   
}
