/**
 * @file    pmh_ip_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from BSM
 *
 * @author  Girish V
 * @date    17-05-2011
 * @details This file contains functions to process different messages received from BSM
 */

/* Constants */
#define IP_EXT_HEADER   "IpExt:"

#include "pmh.h"

/**
 * @param ipCounters IP Counters
 * @details This function fill the IP Counters Structure.
 * */
void pmhFillIpCountersTable(
    IpCounters        *ipCounters
  )
{
  FILE   *filep1 = NULL;
  FILE   *filep2 = NULL;
  SINT8  line[256];
  SINT8  *stats;
  SINT8  *start = line;
  UINT32 len, scan_count;
  UINT32 index = 0;
  TBOOL retVal = 0;

  /*
   * scan_line1: /proc/net/snmp
   *   Ip: Forwarding DefaultTTL InReceives InHdrErrors InAddrErrors ForwDatagrams InUnknownProtos
   *       InDiscards InDelivers OutRequests OutDiscards OutNoRoutes ReasmTimeout ReasmReqds ReasmOKs
   *       ReasmFails FragOKs FragFails FragCreates
   *
   * scan_line2: /proc/net/netstat (kernel:2.6.32)
   *   IpExt: InNoRoutes InTruncatedPkts InMcastPkts OutMcastPkts InBcastPkts OutBcastPkts InOctets
   *          OutOctets InMcastOctets OutMcastOctets InBcastOctets OutBcastOctets
   *
   * scan_line2: /proc/net/netstat (kernel:2.6.28)
   *   IpExt: InNoRoutes InTruncatedPkts InMcastPkts OutMcastPkts InBcastPkts OutBcastPkts
   */

  UINT32 scan_vals1[19];
  UINT32 scan_vals2[12];

  memset(&ipCounters[0], 0, sizeof(IpCounters));
  if(!(filep1 = fopen("/proc/net/snmp","r")))
  {
    BS_ERROR_FILE_OPEN_FAILED( "/proc/net/snmp" );
  }

  if(!(filep2 = fopen("/proc/net/netstat","r")))
  {
    BS_ERROR_FILE_OPEN_FAILED( "/proc/net/netstat" );
  }

  /*
   * skip header of the /proc/net/snmp
   */
  fgets(line, sizeof(line), filep1);
  len = strlen(line);
  if(224 NOTEQ len)
  {
    fclose(filep1);
    logError("unexpected header length in /proc/net/snmp %d != 224\n", len);
  }

  /*
   * /proc/net/snmp provides the statistics for each systemstats.
   * Read in each line in turn, isolate the systemstats name
   * and retrieve the correspoding data structure.
   */
  start = fgets(line, sizeof(line), filep1);
  fclose(filep1);
  if(start)
  {
    len = strlen(line);
    if(line[len - 1] EQ '\n')
       line[len - 1] = '\0';
    while(*start && *start EQ ' ')
      start++;

    if((!*start) || ((stats = strrchr(start, ':')) EQ NULL ))
    {
      logError("systemstats data format error, line EQ |%s|\n", line);
    }

    logInfo("processing /proc/net/snmp -->  %s\n", start);

    *stats++  = 0; /*  NULL Terminated name */
    while(*stats EQ ' ')  /* skip spaces before stats */
      stats++;

    /*
     * Parse the line (i.e., starting from 'stats') to extract
     * the relevant statistics, and populate data structure accordingly.
     */
    memset(scan_vals1, 0, sizeof(scan_vals1));
    scan_count = sscanf(stats,
                        "%u %u %u %u %u %u %u %u %u %u"
                        "%u %u %u %u %u %u %u %u %u",
                         &scan_vals1[0],&scan_vals1[1],&scan_vals1[2],
                         &scan_vals1[3],&scan_vals1[4],&scan_vals1[5],
                         &scan_vals1[6],&scan_vals1[7],&scan_vals1[8],
                         &scan_vals1[9],&scan_vals1[10],&scan_vals1[11],
                         &scan_vals1[12],&scan_vals1[13],&scan_vals1[14],
                         &scan_vals1[15],&scan_vals1[16],&scan_vals1[17],
                         &scan_vals1[18]);
    logInfo("Read %d values\n", scan_count);

    if(scan_count NOTEQ 19)
    {
      logError("error scanning system stats data  (expected %d, got %d)\n", 19, scan_count);
    }

    ipCounters[index].ipSystemStatsIPVersion        =  IPSYSTEMSTATSIPVERSION_IPV4;
    ipCounters[index].ipSystemStatsInReceives       =  scan_vals1[2];
    ipCounters[index].ipSystemStatsInHdrErrors      =  scan_vals1[3];
    ipCounters[index].ipSystemStatsInAddrErrors     =  scan_vals1[4];
    ipCounters[index].ipSystemStatsOutForwDatagrams =  scan_vals1[5];
    ipCounters[index].ipSystemStatsInUnknownProtos  =  scan_vals1[6];
    ipCounters[index].ipSystemStatsInDiscards       =  scan_vals1[7];
    ipCounters[index].ipSystemStatsInDelivers       =  scan_vals1[8];
    ipCounters[index].ipSystemStatsOutRequests      =  scan_vals1[9];
    ipCounters[index].ipSystemStatsOutDiscards      =  scan_vals1[10];
    ipCounters[index].ipSystemStatsOutNoRoutes      =  scan_vals1[11];
    ipCounters[index].ipSystemStatsReasmReqds       =  scan_vals1[13];
    ipCounters[index].ipSystemStatsReasmOKs         =  scan_vals1[14];
    ipCounters[index].ipSystemStatsReasmFails       =  scan_vals1[15];
    ipCounters[index].ipSystemStatsOutFragOKs       =  scan_vals1[16];
    ipCounters[index].ipSystemStatsOutFragFails     =  scan_vals1[17];
    ipCounters[index].ipSystemStatsOutFragCreates   =  scan_vals1[18];

    /*
     * Filling the remaining dependent ipcounters columns with above values.
     * ipSystemStatsOutFragReqds can be computed from ipSystemStatsOutFragOKs + ipSystemStatsOutFragFails
     * ipSystemStatsOutTransmits can be computed from
     *            ipSystemStatsOutRequests + ipSystemStatsOutForwDatagrams + ipSystemStatsOutFragCreates
     *            - ipSystemStatsOutFragReqds - ipSystemStatsOutNoRoutes  - ipSystemStatsOutDiscards
     */
    ipCounters[index].ipSystemStatsOutFragReqds     =  scan_vals1[16] + scan_vals1[17];
    ipCounters[index].ipSystemStatsOutTransmits     =  scan_vals1[9] + scan_vals1[5] + scan_vals1[18] - scan_vals1[16] - scan_vals1[17] -                                                           scan_vals1[11] - scan_vals1[10];
  }


  /*
   * /proc/net/netstat provides the additional systemstats.
   */
  memset(line, 0, sizeof(line));
  while(fgets(line, sizeof(line), filep2))
  {
    if(strncmp(IP_EXT_HEADER, line, sizeof(IP_EXT_HEADER) - 1)  EQ 0)
    {
      /* Next line should include IPv4 additional statistics */
      if((fgets(line, sizeof(line), filep2)) EQ NULL)
      {
        retVal = FALSE;
        break;
      }
      if(strncmp(IP_EXT_HEADER, line, sizeof(IP_EXT_HEADER) - 1)  NOTEQ 0)
      {
        retVal = FALSE;
        break;
      }
      logInfo("processing /proc/net/netstat -->  %s", line);
      memset(scan_vals2, 0, sizeof(scan_vals2));
      scan_count = sscanf(line,
                          "%*s"  /* Ignore 'IpExt:' */
                          "%u %u %u %u %u %u %u %u %u %u %u %u",
                          &scan_vals2[0],&scan_vals2[1],&scan_vals2[2],
                          &scan_vals2[3],&scan_vals2[4],&scan_vals2[5],
                          &scan_vals2[6],&scan_vals2[7],&scan_vals2[8],
                          &scan_vals2[9],&scan_vals2[10],&scan_vals2[11]);
      logInfo("Read %d values\n", scan_count);
      if(scan_count < 12)
      {
        logInfo("Scanning additional system data"
               "(minimum expected %d, got %d)\n",
                 12, scan_count);
        scan_count = sscanf(line,
                                  "%*s"  /* Ignore 'IpExt:' */
                                  "%u %u %u %u %u %u",
                                  &scan_vals2[0],&scan_vals2[1],&scan_vals2[2],
                                  &scan_vals2[3],&scan_vals2[4],&scan_vals2[5]);
        logInfo("Read %d values\n", scan_count);
      }
      ipCounters[index].ipSystemStatsInNoRoutes        = scan_vals2[0];
      ipCounters[index].ipSystemStatsInTruncatedPkts   = scan_vals2[1];
      ipCounters[index].ipSystemStatsInMcastPkts       = scan_vals2[2];
      ipCounters[index].ipSystemStatsOutMcastPkts      = scan_vals2[3];
      ipCounters[index].ipSystemStatsInBcastPkts       = scan_vals2[4];
      ipCounters[index].ipSystemStatsOutBcastPkts      = scan_vals2[5];
      if(scan_count EQ 12)
      {
        ipCounters[index].ipSystemStatsInOctets          = scan_vals2[6];
        ipCounters[index].ipSystemStatsOutOctets         = scan_vals2[7];
        ipCounters[index].ipSystemStatsInMcastOctets     = scan_vals2[8];
        ipCounters[index].ipSystemStatsOutMcastOctets    = scan_vals2[9];
      }
    }
  }
  fclose(filep2);

  /*
   * Filling the remaining dependent ipcounters columns with above values.
   * ipSystemStatsInForwDatagrams can be computed from ipSystemStatsOutFragOKs + ipSystemStatsOutFragFails
   */
  ipCounters[index].ipSystemStatsInForwDatagrams = ipCounters[index].ipSystemStatsInNoRoutes + ipCounters[index].ipSystemStatsOutForwDatagrams;

  /* Update last collected time in IpCounters table */
  getAndFillDateAndTimeStamp( MAX_SIZE_IPCOUNTERS_LASTCOLLECTEDAT, ipCounters[index].lastCollectedAt );

  if(retVal EQ FALSE)
  {
    logError("%s", "/proc/net/netstat does not include additional statistics\n");
  }
}

/**
 * @param   isGetReqProcessing  Is GET_OBJ_REQ processing
 * @param   pmhContext PMH Context
 * @param   getObjReq GEtObjReq
 * @details This function fill the IP Counters Structure.
 * */
PmhState pmhProcIpStatusReq(
  IN TBOOL          isGetReqProcessing,
  IO PmhContext    *pmhContext,
  IO GetObjReq     *getObjReq
  )
{
  GetObjRspSucc   rspTlv;

  EncBuf          encBuf;
  TableId         tableId;

  clearBytes( &rspTlv, sizeof(GetObjRspSucc));
  initEncBuf( &encBuf );

  IpCounters  *rowOfTable = (IpCounters*)getFirstRowOfTable(IPCOUNTERS);
  RowId rowId = 0;
  tableId = IPCOUNTERS;

  /* Update the IP Counter status table with the current values */
  pmhFillIpCountersTable(rowOfTable);


  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( TRUE EQ isGetReqProcessing )
  {
    /* Fill the RSP in TLV format */
    for(rowId = 0; rowId < MAX_ROWS_IN_IPCOUNTERS_TABLE; rowId++ )
    {
      if( IPSYSTEMSTATSIPVERSION_UNKNOWN NOTEQ rowOfTable[rowId].ipSystemStatsIPVersion )
        getRowIpCountersTable( tableId, rowId+1, &rspTlv.tlvArray );
    }

    /* Fill CMD REF details */
    copyBytes( &(getObjReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

    /* Encode and send the response */
    encGetObjRspSuccMsg( &rspTlv, &encBuf );
    createAndSendMsg( MSG_PMH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
    return pmhContext->pmhState;
  }
  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.ipCounter = TRUE;

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
    writeToFileIpCounterTable(rowOfTable, pmhContext->fp);
  }

  if(TRUE EQ pmhStsRspRcvdFromAllModules( &pmhContext->sendCounterReq, &pmhContext->rcvdCounterRsp ) )
  {
    doCounterCollectionProcCompletionSteps( pmhContext );
  }
  return pmhContext->pmhState;
}
