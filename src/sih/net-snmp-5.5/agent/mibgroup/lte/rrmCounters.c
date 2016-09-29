/* @file    rrmCounters.c */
/* @section Copyright Powerwave Technologies, Inc., 2010 */
/* This file auto generated on : 12/7/2012 6:50:37 PM*/

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "bs_modules.h"
#include "all_cfg.h"
#include "all_cmn.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "syslog.h"
#include "rrmCounters.h"

/* function declarations */
Netsnmp_Node_Handler rrmCountersTable_handler;
Netsnmp_First_Data_Point rrmCountersTable_get_first_data_point;
Netsnmp_Next_Data_Point rrmCountersTable_get_next_data_point;

/* Global Variables */
struct rrmCountersTable_entry *rrmCountersTable_head = NULL;
static TBOOL g_sihRrmCountersGetReqProcessed = FALSE;
static UINT32 g_prevRrmCountersReqCount = 0;
static UINT32 g_rrmCountersFirstCall = 0;

/**
 * @details
 *          This is the routine which frees the rrmCounters entry.
 */
void free_rrmCountersTable(void)
{
  struct rrmCountersTable_entry *current = rrmCountersTable_head;

  if(current)
  {
    for (current = rrmCountersTable_head->next; current != NULL; current = current->next)
    {
      	    SNMP_FREE( current );
    }
  }

  SNMP_FREE(rrmCountersTable_head);
  rrmCountersTable_head = NULL;

  return;
}

/**
 * @details
 *          This is the routine which handles get response from SIH Get Processor
 *          and Prepare the SNMP GET Response for EMS.
 * @param tlvArray containf pointer to TLV array.
 */
TBOOL sendRrmCountersSnmpGetResponse (
  IO TlvArray *tlvArray
  )
{
  int totalNumRow, totalNumCol, i=0, colmn=0;
  struct rrmCountersTable_entry *prevPtr = NULL;

  printTLV(tlvArray);

  if (tlvArray->numTlvs == 0)
  { return FALSE; }
  /* Total no. of Colmns */
  totalNumCol = RRMCOUNTERS_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++)
  {
    struct rrmCountersTable_entry *ptr = (struct rrmCountersTable_entry *)malloc(sizeof(struct rrmCountersTable_entry));

    clearBytes(ptr, sizeof(struct rrmCountersTable_entry));

    /* Fill Data in TLV */
    for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
    {
      if(colmn EQ RRMCOUNTERS_INDEX)
      {
        ptr->index = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUMRLFAILURES)
      {
        ptr->numRlFailures = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NS1SETUPREQ)
      {
        ptr->nS1SetupReq = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUMINCOMINGHOSUCCESSFUL)
      {
        ptr->numIncomingHoSuccessful = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUMINCOMINGHOFAILED)
      {
        ptr->numIncomingHoFailed = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUMINCOMINGHOREJECTED)
      {
        ptr->numIncomingHoRejected = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUMS1APREJECTED)
      {
        ptr->numS1apRejected = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCCONNREQ)
      {
        ptr->nRrcConnReq = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCCONNSETUP)
      {
        ptr->nRrcConnSetup = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCCONNSETUPCMPL)
      {
        ptr->nRrcConnSetupCmpl = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCCONNREL)
      {
        ptr->nRrcConnRel = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NDLINFOTRF)
      {
        ptr->nDlInfoTrf = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NULINFOTRF)
      {
        ptr->nUlInfoTrf = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NULNASTRFMSG)
      {
        ptr->nUlNasTrfMsg = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NDLNASTRFMSG)
      {
        ptr->nDlNasTrfMsg = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NINITCTXTSETREQ)
      {
        ptr->nInitCtxtSetReq = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NINITCTXTSETRSP)
      {
        ptr->nInitCtxtSetRsp = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NINITCTXTSETFAIL)
      {
        ptr->nInitCtxtSetFail = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCRECONFREQ)
      {
        ptr->nRrcReconfReq = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCRECONFCMPL)
      {
        ptr->nRrcReconfCmpl = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCREESTREQ)
      {
        ptr->nRrcReestReq = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUECAPENQ)
      {
        ptr->nUeCapEnq = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUECAPINFO)
      {
        ptr->nUeCapInfo = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUECAPINFOIND)
      {
        ptr->nUeCapInfoInd = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NSECMODECMD)
      {
        ptr->nSecModeCmd = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NSECMODECMPL)
      {
        ptr->nSecModeCmpl = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NSECMODEFAIL)
      {
        ptr->nSecModeFail = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUECTXTRELCMPL)
      {
        ptr->nUeCtxtRelCmpl = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUECTXTRELCMD)
      {
        ptr->nUeCtxtRelCmd = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUECTXTRELREQ)
      {
        ptr->nUeCtxtRelReq = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NUECTXTMODRESP)
      {
        ptr->nUeCtxtModResp = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NS1SETUPFAIL)
      {
        ptr->nS1SetupFail = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NPAGING)
      {
        ptr->nPaging = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NRRCCONNREJ)
      {
        ptr->nRrcConnRej = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NS1SETUPRSP)
      {
        ptr->nS1SetupRsp = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NS1RESETTX)
      {
        ptr->nS1ResetTx = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_NS1RESETRX)
      {
        ptr->nS1ResetRx = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ RRMCOUNTERS_LASTCOLLECTEDAT)
      {
        clearBytes(ptr->lastCollectedAt, RRMCOUNTERS_LASTCOLLECTEDAT_MAX_LEN);
        if(tlvArray->tlvInfo[colmn-1].len > 0)
        {
          copyBytes(tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->lastCollectedAt);
        }
        ptr->lastCollectedAt_len = strlen(ptr->lastCollectedAt);
        continue;
      }
    } /* Ending of InnerLoop */

    ptr->next = NULL;

    if(prevPtr == NULL)
    {
      rrmCountersTable_head = prevPtr = ptr;
    }
    prevPtr->next = ptr;
    prevPtr = ptr;

  } /*Ending of OuterLoop */
  prevPtr->next = NULL;

  if(rrmCountersTable_head)
    return TRUE;
  else
    return FALSE;
}
 /** Initializes the rrmCounters module */
void init_rrmCounters(void)
{
  /* here we initialize all the tables we're planning on supporting */
  initialize_table_rrmCountersTable();
}

/** Initialize the rrmCountersTable table by defining its contents and how it's structured */
void initialize_table_rrmCountersTable(void)
{
  const oid rrmCountersTable_oid[] = {1,3,6,1,4,1,9344,501,RRMCOUNTERS,1};
  const size_t rrmCountersTable_oid_len = OID_LENGTH(rrmCountersTable_oid);
  netsnmp_handler_registration    *reg;
  netsnmp_iterator_info           *iinfo;
  netsnmp_table_registration_info *table_info;

  DEBUGMSGTL(("rrmCounters:init", "initializing table rrmCountersTable\n"));

  reg = netsnmp_create_handler_registration(
            "rrmCountersTable", rrmCountersTable_handler,
            rrmCountersTable_oid, rrmCountersTable_oid_len,
            HANDLER_CAN_RONLY
            );

  table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
  netsnmp_table_helper_add_indexes(table_info,
                          ASN_UNSIGNED, /* index: index */
                          0);
  table_info->min_column = RRMCOUNTERS_INDEX;
  table_info->max_column = RRMCOUNTERS_MAX_COL;

  iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
  iinfo->get_first_data_point = rrmCountersTable_get_first_data_point;
  iinfo->get_next_data_point  = rrmCountersTable_get_next_data_point;
  iinfo->table_reginfo        = table_info;

  netsnmp_register_table_iterator( reg, iinfo );

  /* Initialise the contents of the table here */
}

#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct rrmCountersTable_entry *rrmCountersTable_createEntry(
  u_long index
  )
{
  struct rrmCountersTable_entry *entry;

  entry = SNMP_MALLOC_TYPEDEF(struct rrmCountersTable_entry);
  if (!entry)
    return NULL;

  entry->index = index;
  entry->next = rrmCountersTable_head;
  rrmCountersTable_head = entry;
  return entry;
}
/* remove a row from the table */
void rrmCountersTable_removeEntry( 
  struct rrmCountersTable_entry *entry
  )
{
  struct rrmCountersTable_entry *ptr, *prev;

  if (!entry)
    return;    /* Nothing to remove */

  for ( ptr  = rrmCountersTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) 
  {
          if ( ptr == entry )
            break;
  }
  if ( !ptr )
    return;    /* Can't find it */

  if ( prev == NULL )
    rrmCountersTable_head = ptr->next;
  else
    prev->next = ptr->next;

  SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
rrmCountersTable_get_first_data_point(void **my_loop_context,
                           void **my_data_context,
                           netsnmp_variable_list *put_index_data,
                           netsnmp_iterator_info *mydata)
{
  TagInfo sihTagInfo;
  TBOOL status = FALSE;

  if(g_sihRrmCountersGetReqProcessed EQ FALSE)
  {
    /* Set SIH Get Request Tag Info */
    sihTagInfo.tableId = RRMCOUNTERS;
    sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
    sihTagInfo.colId = CONFIG_COL_ID_NONE;

    status = sihGetProcessor (sihTagInfo);
    if(status EQ TRUE)
    {
      logInfo("%s","Get Processor Succedded");
      if(rrmCountersTable_head)
      {
        g_rrmCountersFirstCall = 1;
        g_sihRrmCountersGetReqProcessed = TRUE;
      }
    }
    else
      logError("%s","Get Processor Failed");
  }

  *my_loop_context = rrmCountersTable_head;
  return rrmCountersTable_get_next_data_point(my_loop_context, my_data_context,
                                              put_index_data,  mydata );
}

netsnmp_variable_list *
rrmCountersTable_get_next_data_point(void **my_loop_context,
                           void **my_data_context,
                           netsnmp_variable_list *put_index_data,
                           netsnmp_iterator_info *mydata)
{
  struct rrmCountersTable_entry *entry = (struct rrmCountersTable_entry *)*my_loop_context;
  netsnmp_variable_list *idx = put_index_data;

  if(entry) 
  {
    snmp_set_var_typed_integer( idx, ASN_UNSIGNED, entry->index );
    idx = idx->next_variable;
    *my_data_context = (void *)entry;
    *my_loop_context = (void *)entry->next;
    return put_index_data;
  }
  else
  {
    return NULL;
  }
}
/** handles requests for the timersConfigTable table */
int
rrmCountersTable_handler(
    netsnmp_mib_handler            *handler,
    netsnmp_handler_registration   *reginfo,
    netsnmp_agent_request_info     *reqinfo,
    netsnmp_request_info           *requests
  )
{
  netsnmp_request_info       *request = NULL;
  netsnmp_table_request_info *table_info = NULL;
  struct rrmCountersTable_entry  *table_entry = NULL;
  int loopCount = 0;

  DEBUGMSGTL(("rrmCounters:handler", "Processing request (%d)\n", reqinfo->mode));

  switch(reqinfo->mode)
  {
    /*
     * Read-support (also covers GetNext requests)
     */
    case MODE_GET:
         for (request=requests; request; request=request->next)
         {
           loopCount++;
           if(request->processed != 0)
           {
             g_prevRrmCountersReqCount = 0;
             g_sihRrmCountersGetReqProcessed = FALSE;

             /* free the resources */
             free_rrmCountersTable();
             /* Here loopCount is decremented, due to calling of free function two times
              * in SNMP-GETNEXT on single parameter -- 20/07/11
              */
             loopCount--;
             continue;
           }

           /* Perform anything here that you need to do before each
              request is processed. */

           /* The following extracts the my_data_context pointer set in
              the loop functions above.  You can then use the results to
              help return data for the columns of the rrmCountersTable table
              in question. */

           table_entry = (struct rrmCountersTable_entry *)
                          netsnmp_extract_iterator_context(request);

           if(table_entry == NULL)
           {
             if (reqinfo->mode == MODE_GET)
             {
               netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
               continue;
             }
             /* XXX: no row existed, if you support creation and this is a
                set, start dealing with it here, else continue */
           }

           table_info = netsnmp_extract_table_info(request);
           /* table_info->colnum contains the column number requested */
           /* table_info->indexes contains a linked list of snmp variable
              bindings for the indexes of the table.  Values in the list
              have been set corresponding to the indexes of the request */
           if (table_info==NULL) {
             continue;
           }
           switch (table_info->colnum)
           {
             case RRMCOUNTERS_INDEX:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->index);
                  break;
             case RRMCOUNTERS_NUMRLFAILURES:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->numRlFailures);
                  break;
             case RRMCOUNTERS_NS1SETUPREQ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nS1SetupReq);
                  break;
             case RRMCOUNTERS_NUMINCOMINGHOSUCCESSFUL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->numIncomingHoSuccessful);
                  break;
             case RRMCOUNTERS_NUMINCOMINGHOFAILED:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->numIncomingHoFailed);
                  break;
             case RRMCOUNTERS_NUMINCOMINGHOREJECTED:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->numIncomingHoRejected);
                  break;
             case RRMCOUNTERS_NUMS1APREJECTED:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->numS1apRejected);
                  break;
             case RRMCOUNTERS_NRRCCONNREQ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcConnReq);
                  break;
             case RRMCOUNTERS_NRRCCONNSETUP:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcConnSetup);
                  break;
             case RRMCOUNTERS_NRRCCONNSETUPCMPL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcConnSetupCmpl);
                  break;
             case RRMCOUNTERS_NRRCCONNREL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcConnRel);
                  break;
             case RRMCOUNTERS_NDLINFOTRF:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nDlInfoTrf);
                  break;
             case RRMCOUNTERS_NULINFOTRF:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUlInfoTrf);
                  break;
             case RRMCOUNTERS_NULNASTRFMSG:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUlNasTrfMsg);
                  break;
             case RRMCOUNTERS_NDLNASTRFMSG:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nDlNasTrfMsg);
                  break;
             case RRMCOUNTERS_NINITCTXTSETREQ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nInitCtxtSetReq);
                  break;
             case RRMCOUNTERS_NINITCTXTSETRSP:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nInitCtxtSetRsp);
                  break;
             case RRMCOUNTERS_NINITCTXTSETFAIL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nInitCtxtSetFail);
                  break;
             case RRMCOUNTERS_NRRCRECONFREQ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcReconfReq);
                  break;
             case RRMCOUNTERS_NRRCRECONFCMPL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcReconfCmpl);
                  break;
             case RRMCOUNTERS_NRRCREESTREQ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcReestReq);
                  break;
             case RRMCOUNTERS_NUECAPENQ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUeCapEnq);
                  break;
             case RRMCOUNTERS_NUECAPINFO:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUeCapInfo);
                  break;
             case RRMCOUNTERS_NUECAPINFOIND:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUeCapInfoInd);
                  break;
             case RRMCOUNTERS_NSECMODECMD:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nSecModeCmd);
                  break;
             case RRMCOUNTERS_NSECMODECMPL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nSecModeCmpl);
                  break;
             case RRMCOUNTERS_NSECMODEFAIL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nSecModeFail);
                  break;
             case RRMCOUNTERS_NUECTXTRELCMPL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUeCtxtRelCmpl);
                  break;
             case RRMCOUNTERS_NUECTXTRELCMD:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUeCtxtRelCmd);
                  break;
             case RRMCOUNTERS_NUECTXTRELREQ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUeCtxtRelReq);
                  break;
             case RRMCOUNTERS_NUECTXTMODRESP:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nUeCtxtModResp);
                  break;
             case RRMCOUNTERS_NS1SETUPFAIL:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nS1SetupFail);
                  break;
             case RRMCOUNTERS_NPAGING:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nPaging);
                  break;
             case RRMCOUNTERS_NRRCCONNREJ:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nRrcConnRej);
                  break;
             case RRMCOUNTERS_NS1SETUPRSP:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nS1SetupRsp);
                  break;
             case RRMCOUNTERS_NS1RESETTX:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nS1ResetTx);
                  break;
             case RRMCOUNTERS_NS1RESETRX:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                              table_entry->nS1ResetRx);
                  break;
             case RRMCOUNTERS_LASTCOLLECTEDAT:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                           table_entry->lastCollectedAt,
                                   table_entry->lastCollectedAt_len);
                  break;
             default:
                  netsnmp_set_request_error(reqinfo, request,
                                            SNMP_NOSUCHOBJECT);
                  break;
           }
         }
         if(g_rrmCountersFirstCall EQ 1)
         {
           g_rrmCountersFirstCall = 0;
           g_prevRrmCountersReqCount = loopCount;
           // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
           // In the first call itself loopCount becomes one.
           // In General Case, the loopCount will be always > 1 because every table contains
           // index column to fetch the row and remaining columns contain table attributes.
           if(loopCount EQ 1)
           {
             // Setting it to false in case of SNMP-GET request for single parameter.
             g_sihRrmCountersGetReqProcessed = FALSE;
             // Free all Resources.
             free_rrmCountersTable();
           }
         }
         else if(loopCount > 1 || g_prevRrmCountersReqCount > 1)
         {
           // update g_prevReqCount with current loopCount for next call of Table_handler
           g_prevRrmCountersReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
           //Setting it to false in case of SNMP-GET request for single parameter.
           g_sihRrmCountersGetReqProcessed = FALSE;
           // Free all resources.
           free_rrmCountersTable();
         }
         break;

  }
  return SNMP_ERR_NOERROR;
}
