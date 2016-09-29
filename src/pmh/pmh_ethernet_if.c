/**
 * @file    pmh_ethernet_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from BSM
 *
 * @author  Girish V
 * @date    12-05-2011
 * @details This file contains functions to process different messages received from BSM
 */

/* For Socket Creation */
#include<sys/types.h>
#include<sys/socket.h>

/* ifreq and ifconf structures are available  */
#include<net/if.h>
#include<net/if_arp.h>

/* ioctl requests */
#include<linux/sockios.h>

/* struct ethtool_cmd */
#include<linux/ethtool.h>

/* Constants */
#define IFHWRADDRLEN  18

#include "pmh.h"

/**
 * @param sa_data @a PhyAddress
 * @param phyAddr Dotted Format of PhyAddress
 * @brief This function creates the PhyAddress in user required format
 */
void print_eth_addrs(unsigned char* sa_data, unsigned char* phyAddr)
{
    snprintf(phyAddr, IFHWRADDRLEN, "%02x:%02x:%02x:%02x:%02x:%02x",sa_data[0], sa_data[1], sa_data[2], sa_data[3], sa_data[4], sa_data[5]);
}

/**
 *
 * @param ethCounters
 * @param interfaceName
 * @param index
 * @brief This function fills the Ethernet Counters Hardware Related Info.
 */
void pmhFillHardwareEthernetInfo(
  EthernetCounters *ethCounters,
  UINT8            *interfaceName,
  UINT32            index
  )
{
  struct ethtool_cmd edata;
  struct ifreq  ifrq;
  SINT32 sockfd;

  /*
   * create sockets for ioctls
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0)
  {
    logError("%s", "Could not create socket ...\n");
  }

  logInfo("Filling Hardware Info of Interface %s", interfaceName);

  memset(&ifrq, 0, sizeof(ifrq));
  memset(&ethCounters[index], 0, sizeof(EthernetCounters));

  /* Interface Name is used as index to fetch some values using ioctl calls */
  snprintf(ifrq.ifr_name, sizeof(ifrq.ifr_name), "%s", interfaceName);

  /*
   * Interface Index
   */
  if(ioctl(sockfd, SIOCGIFINDEX, &ifrq) < 0)
  {
    logError("%s", "ioctl SIOCGIFINDEX error ...\n");
  }
  ethCounters[index].ifIndex = ifrq.ifr_ifindex;

  /*
   * Interface Name
   */
  snprintf(ethCounters[index].ifDescr, sizeof(ethCounters[index].ifDescr), "%s", ifrq.ifr_name);

  /*
   * Interface Hardware Address
   */
  if(ioctl(sockfd, SIOCGIFHWADDR, &ifrq) < 0)
  {
    logError("%s", "ioctl SIOCGIFHWADDR error ...\n");
  }
  if( ifrq.ifr_hwaddr.sa_family NOTEQ ARPHRD_LOOPBACK)
  print_eth_addrs((unsigned char *)ifrq.ifr_hwaddr.sa_data, ethCounters[index].ifPhysAddress);

  /*
   * Interface Type
   */
  switch(ifrq.ifr_hwaddr.sa_family)
  {
    case ARPHRD_ETHER:
                         ethCounters[index].ifType = IFTYPE_ETHERNETCSMACD;  /* Ethernet CSMACD */
                         break;
    case ARPHRD_LOOPBACK:
                         ethCounters[index].ifType = IFTYPE_SOFTWARELOOPBACK;  /* Software Loopback  */
                         break;
    default:
                         ethCounters[index].ifType = IFTYPE_OTHER;  /* OTHERS */
  }

  /*
   * Interface MTU
   */
  if(ioctl(sockfd, SIOCGIFMTU, &ifrq) < 0)
  {
    logError("%s", "ioctl SIOCGIFMTU error ...\n");
  }
  ethCounters[index].ifMtu = ifrq.ifr_mtu;

  /*
   * ifOperStatus Description:
   * If ifAdminStatus is down(2) then ifOperStatus should be down(2).
   */
  if(ioctl(sockfd, SIOCGIFFLAGS, &ifrq) < 0)
  {
    logError("%s", "ioctl SIOCGIFFLAGS error ...\n");
  }
  if(ifrq.ifr_flags & IFF_UP)
  {
    ethCounters[index].ifAdminStatus = IFADMINSTATUS_ISUP;
    if(ifrq.ifr_flags & IFF_RUNNING)
      ethCounters[index].ifOperStatus = IFOPERSTATUS_ISUP;
    else
      ethCounters[index].ifOperStatus = IFOPERSTATUS_ISDOWN;
  }
  else
  {
    ethCounters[index].ifAdminStatus = IFADMINSTATUS_ISDOWN;
    ethCounters[index].ifOperStatus = IFOPERSTATUS_ISDOWN;
  }

  /*
   * Tx Queue Length
   */
  if(ioctl(sockfd, SIOCGIFTXQLEN, &ifrq) < 0)
  {
    logError("%s", "ioctl SIOCGIFTXQLEN error ...\n");
  }
  ethCounters[index].ifOutQLen = ifrq.ifr_qlen;

  /*
   * Interface Speed
   */
  edata.cmd = ETHTOOL_GSET;
  edata.speed = 0;
  ifrq.ifr_data = (char *)&edata;

  if(ethCounters[index].ifType NOTEQ IFTYPE_SOFTWARELOOPBACK)
  {
    if(ioctl(sockfd, SIOCETHTOOL, &ifrq) < 0)
    {
      logError("%s", "ioctl SIOCETHTOOL error ...\n");
    }

    if(edata.speed NOTEQ SPEED_10 && edata.speed NOTEQ SPEED_100
      && edata.speed NOTEQ SPEED_10000
      && edata.speed NOTEQ SPEED_2500
      && edata.speed NOTEQ SPEED_1000 )
    {
      if(ethCounters[index].ifType EQ IFTYPE_ETHERNETCSMACD)
        ethCounters[index].ifSpeed = IFSPEED_MBPS10;
      else
        ethCounters[index].ifSpeed = IFSPEED_UNDETECTABLE;
    }

    if(edata.speed EQ SPEED_10)
      ethCounters[index].ifSpeed = IFSPEED_MBPS10;
    else if(edata.speed EQ SPEED_100)
      ethCounters[index].ifSpeed = IFSPEED_MBPS100;
    else if(edata.speed EQ SPEED_1000)
      ethCounters[index].ifSpeed = IFSPEED_GBPS1;
    else if(edata.speed EQ SPEED_2500)
      ethCounters[index].ifSpeed = IFSPEED_GBPS2ANDHALF;
    else if(edata.speed EQ SPEED_10000)
      ethCounters[index].ifSpeed = IFSPEED_GBPS10;
  }
  else
  {
    ethCounters[index].ifSpeed = IFSPEED_MBPS10;
  }
  close(sockfd);
}


/**
 * @param ethCounters Ethernet counters
 * @brief This function fill the Ethernet Counters Structure.
 * */
void pmhFillEthernetCountersTable(
  IO EthernetCounters  *ethCounters
  )
{
  FILE   *filep = NULL;
  SINT8   line[256];
  SINT32  index = 0;
  SINT8   ifname_buf[IFNAMSIZ];
  EthernetHwInfoTable  *hwInfoEntry = NULL;

  /*
   * scan_line:
   *  [                   IN                              ]
   *   byte pkts errs drop fifo frame compressed multicast
   *  [                   OUT                           ]
   *   byte pkts errs drop fifo colls carrier compressed
   */
  UINT32       rec_pkt, rec_oct, rec_err, rec_drop, rec_mcast;
  UINT32       snd_pkt, snd_oct, snd_err, snd_drop, coll;
  const SINT8 *scan_line = "%u %u %u %u %*u %*u %*u %u %u %u %u %u %*u %u";


  if(!(filep = fopen("/proc/net/dev","r")))
  {
    BS_ERROR_FILE_OPEN_FAILED( "/proc/net/dev" );
  }

  /*
   * Read First and second lines for Headers
   */
  fgets(line, sizeof(line), filep);
  fgets(line, sizeof(line), filep);

  /*
   * Read Remaining lines for Interface Info
   */
  while(fgets(line, sizeof(line), filep))
  {
    SINT8 *ifstart = line;
    SINT8 *stats;

    if(line[strlen(line) - 1] EQ '\n')
       line[strlen(line) - 1] = '\0';

    while(*ifstart && *ifstart EQ ' ')
           ifstart++;

    if(!*ifstart || ((stats = strrchr(ifstart, ':')) EQ NULL))
    {
       logError("%s", "/proc/net/dev data format error\n");
    }

    *stats = 0;
    snprintf(ifname_buf, sizeof(ifname_buf), "%s", ifstart);
    *stats++ = ':';
    while(*stats EQ ' ')
           stats++;

    hwInfoEntry = getEthernetHwInfoEntry(index);
    /* Fill the Ethernet Counters Hardware related Info */
    if(strncmp(hwInfoEntry->interfaceName, ifname_buf, sizeof(ifname_buf)) || ( FALSE EQ hwInfoEntry->isHwInfoAlreadyRead ))
    {
      pmhFillHardwareEthernetInfo(ethCounters, ifname_buf, index);
      hwInfoEntry->isHwInfoAlreadyRead = TRUE;
      snprintf(hwInfoEntry->interfaceName, sizeof(hwInfoEntry->interfaceName), "%s", ifname_buf);
    }

    if(sscanf(stats, scan_line, &rec_oct, &rec_pkt, &rec_err, &rec_drop, &rec_mcast,
                                &snd_oct, &snd_pkt, &snd_err, &snd_drop, &coll) NOTEQ 10)
    {
      logError("/proc/net/dev data format error, line == |%s|", line);
    }

    rec_pkt -= rec_mcast;

    ethCounters[index].ifInOctets     = rec_oct;
    ethCounters[index].ifInUcastPkts  = rec_pkt;
    ethCounters[index].ifInNUcastPkts = rec_mcast;
    ethCounters[index].ifInDiscards   = rec_drop;
    ethCounters[index].ifInErrors     = rec_err;
    ethCounters[index].ifOutOctets    = snd_oct;
    ethCounters[index].ifOutUcastPkts = snd_pkt;
    ethCounters[index].ifOutDiscards  = snd_drop;
    ethCounters[index].ifOutErrors    = snd_err;

    /* Update last collected time in EthernetCounters table */
    getAndFillDateAndTimeStamp( MAX_SIZE_ETHERNETCOUNTERS_LASTCOLLECTEDAT, ethCounters[index].lastCollectedAt );

    index++;
  }
  fclose(filep);
}

/**
 * @param   isGetReqProcessing is GET_OBJ_REQ Processing
 * @param   pmhContext         PMH Context
 * @param   getReq             GetObjReq
 * @brief   This function fill the Ethernet Counters Structure.
 * */
PmhState pmhProcEthernetStatusReq(
  IN TBOOL          isGetReqProcessing,
  IO PmhContext    *pmhContext,
  IO GetObjReq     *getReq
  )
{
  GetObjRspSucc   rspTlv;

  EncBuf          encBuf;
  TableId         tableId;

  clearBytes( &rspTlv, sizeof(GetObjRspSucc));
  initEncBuf( &encBuf );

  EthernetCounters  *rowOfTable = (EthernetCounters*)getFirstRowOfTable(ETHERNETCOUNTERS);
  RowId rowId = 0;
  tableId = ETHERNETCOUNTERS;

  /* Update the Ethernet Counter status table with the current values */
  pmhFillEthernetCountersTable(rowOfTable);

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( TRUE EQ isGetReqProcessing )
  {
    /* Fill the RSP in TLV format */
    for(rowId = 0; rowId < MAX_ROWS_IN_ETHERNETCOUNTERS_TABLE; rowId++ )
    {
      if( 0 NOTEQ rowOfTable[rowId].ifIndex )
        getRowEthernetCountersTable( tableId, rowId+1, &rspTlv.tlvArray );
    }
    /* Fill CMD REF details */
     copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

    /* Encode and send the response */
    encGetObjRspSuccMsg( &rspTlv, &encBuf );
    createAndSendMsg( MSG_PMH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.ethernetCounter = TRUE;

  if( NULL NOTEQ pmhContext->fp )
  {
    /* This check of file exists is required, because operator can delete the
     * file using upload cmd with delete file after upload option */
    /* In this case PMH should create a new file with make a new entry in the pmLogFileStatusTable */
    if ( FALSE EQ checkIfCurrentStsLogFileExists( pmhContext->currPmLogFileName ) )
    {
      logInfo(" Current logging statistics file does not exists Filename:%s", pmhContext->currPmLogFileName );
      pmhClearPmLogFileStatusRow( pmhContext->currPmLogFileName );
      /* Open PM Log file to collect remaining counter values */
      if(TRUE EQ pmhOpenPmLogFile(pmhContext))
      {
        pmhAddEntryInPmLogFileStatusTable(pmhContext);
      }
      logInfo(" New file is created [filename:%s] "
          " and further will be done in this file", pmhContext->currPmLogFileName );
    }
    writeToFileEthernetCounterTable(rowOfTable, pmhContext->fp);
  }

  if(TRUE EQ pmhStsRspRcvdFromAllModules( &pmhContext->sendCounterReq, &pmhContext->rcvdCounterRsp ) )
  {
    doCounterCollectionProcCompletionSteps( pmhContext );
  }
  return pmhContext->pmhState;
}
