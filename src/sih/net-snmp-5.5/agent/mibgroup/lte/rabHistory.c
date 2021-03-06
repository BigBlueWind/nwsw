/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "bs_modules.h"
#include "all_cfg.h"
#include "all_cmn.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "syslog.h"
#include "rabHistory.h"

/* function declarations */
Netsnmp_Node_Handler rabHistoryTable_handler;
Netsnmp_First_Data_Point  rabHistoryTable_get_first_data_point;
Netsnmp_Next_Data_Point   rabHistoryTable_get_next_data_point;

/* globals */
struct rabHistoryTable_entry  *rabHistoryTable_head = NULL;
static TBOOL g_sihRabHistoryGetReqProcessed = FALSE;
static UINT32 g_prevRabHistoryReqCount = 0;
static UINT32 g_rabHistoryFirstCall = 0;

/* free_rabHistoryTable */
/**
* @details 
*          This is the routine which frees the rab history entry.
*/
void free_rabHistoryTable(void)
{
  rabHistoryTableEntry  * current = rabHistoryTable_head;
  rabHistoryTableEntry  * temp    = NULL;

  if(current)
  {
    while(current->next != NULL)
    {
        temp = current->next;
        SNMP_FREE( current );
        current = temp;
    }

    SNMP_FREE( current );
    rabHistoryTable_head = NULL;
  }
  return;
}


/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS.
* @param tlvArray contains pointer to TLV array.
*/
TBOOL sendRabHistorySnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0;
  rabHistoryTableEntry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = RAB_HISTORY_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++)
      {
      rabHistoryTableEntry *ptr=(rabHistoryTableEntry *)malloc(sizeof(rabHistoryTableEntry));

      clearBytes (ptr, sizeof(rabHistoryTableEntry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
        {
        if (colmn EQ COLUMN_RAB_HISTORY_INDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_INTERNALUEID)
           {
            ptr->internalUeId =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_RABIDENTITY)
           {
            ptr->rabIdentity =  getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_DRBIDENTITY)
           {
            ptr->drbIdentity =  getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_RABQCI)
           {
            ptr->rabQci =  getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_RABSETUPTIME)
           {
            clearBytes (ptr->rabSetupTime, RAB_HISTORY_TIME_MAX_BUF_LEN);
            if (tlvArray->tlvInfo[colmn-1].len > 0) 
                {    
                 copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->rabSetupTime);
                }
            ptr->rabSetupTime_len = strlen(ptr->rabSetupTime);
            continue;
           }
        if (colmn EQ COLUMN_RABRELEASETIME)
           {
            clearBytes (ptr->rabReleaseTime, RAB_HISTORY_TIME_MAX_BUF_LEN);
            if (tlvArray->tlvInfo[colmn-1].len > 0) 
                {    
                 copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->rabReleaseTime);
                }
            ptr->rabReleaseTime_len = strlen(ptr->rabReleaseTime);
            continue;
           }
        if (colmn EQ COLUMN_RABRELEASECAUSE)
           {
            ptr->rabReleaseCause = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_RAB_HISTORY_DLGTPTUNNELID)
           {
            ptr->dlGtpTunnelId = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_RAB_HISTORY_ULGTPTUNNELID)
           {
            ptr->ulGtpTunnelId = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_RAB_HISTORY_GTPKILOBYTESDL)
           {
            ptr->gtpKiloBytesDL = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_RAB_HISTORY_GTPKILOBYTESUL)
           {
            ptr->gtpKiloBytesUL = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           rabHistoryTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (rabHistoryTable_head)
        return TRUE;
    else
        return FALSE;
}


/** Initializes the rabHistory module */
void
init_rabHistory(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_rabHistoryTable();
}

/*   Determine the first/last column names */

/** Initialize the rabHistoryTable table by defining its contents and how it's structured */
void
initialize_table_rabHistoryTable(void)
{
    const oid rabHistoryTable_oid[] = {1,3,6,1,4,1,9344,501,RABHISTORY,1};
    const size_t rabHistoryTable_oid_len   = OID_LENGTH(rabHistoryTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("rabHistory:init", "initializing table rabHistoryTable\n"));

    reg = netsnmp_create_handler_registration(
              "rabHistoryTable",     rabHistoryTable_handler,
              rabHistoryTable_oid, rabHistoryTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: index */
                           0);
    table_info->min_column = COLUMN_RAB_HISTORY_INDEX;
    table_info->max_column = COLUMN_RAB_HISTORY_GTPKILOBYTESUL;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = rabHistoryTable_get_first_data_point;
    iinfo->get_next_data_point  = rabHistoryTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct rabHistoryTable_entry *
rabHistoryTable_createEntry(
                 u_long  index,
                ) {
    struct rabHistoryTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct rabHistoryTable_entry);
    if (!entry)
        return NULL;

    entry->index = index;
    entry->next = rabHistoryTable_head;
    rabHistoryTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
rabHistoryTable_removeEntry( struct rabHistoryTable_entry *entry ) {
    struct rabHistoryTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = rabHistoryTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        rabHistoryTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
rabHistoryTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;


    if (g_sihRabHistoryGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = RABHISTORY;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
        sihTagInfo.colId = CONFIG_COL_ID_NONE;

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (rabHistoryTable_head)
                {
		g_rabHistoryFirstCall = 1;
                g_sihRabHistoryGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }

    *my_loop_context = rabHistoryTable_head;
    return rabHistoryTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
rabHistoryTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct rabHistoryTable_entry *entry = (struct rabHistoryTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_UNSIGNED, entry->index );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the rabHistoryTable table */
int
rabHistoryTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct rabHistoryTable_entry          *table_entry = NULL;
    int loopCount = 0;

    DEBUGMSGTL(("rabHistory:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0) /*Processed all requests, rows and colmns */
                {
		g_prevRabHistoryReqCount = 0;
                g_sihRabHistoryGetReqProcessed = FALSE;
                /* free the resources */
                free_rabHistoryTable();
                /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
                 * on single parameter -- 20/07/11 */
                loopCount--;
                continue;
                }

            /* perform anything here that you need to do before each
               request is processed. */

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the rabHistoryTable table
               in question */

            table_entry = (struct rabHistoryTable_entry *)
                              netsnmp_extract_iterator_context(request);

            if ( table_entry == NULL) {
                if (reqinfo->mode == MODE_GET) {
                    netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
                    continue;
                }
                /* XXX: no row existed, if you support creation and this is a
                   set, start dealing with it here, else continue */
            }

            table_info  =     netsnmp_extract_table_info(      request);

            /* table_info->colnum contains the column number requested */
            /* table_info->indexes contains a linked list of snmp variable
               bindings for the indexes of the table.  Values in the list
               have been set corresponding to the indexes of the
               request */
            if (table_info==NULL) {
                continue;
            }
        
            switch (table_info->colnum) {
            case COLUMN_RAB_HISTORY_INDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->index);
                break;
            case COLUMN_INTERNALUEID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->internalUeId);
                break;
            case COLUMN_RABIDENTITY:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->rabIdentity);
                break;
            case COLUMN_DRBIDENTITY:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->drbIdentity);
                break;
            case COLUMN_RABQCI:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->rabQci);
                break;
            case COLUMN_RABSETUPTIME:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->rabSetupTime,
                                          table_entry->rabSetupTime_len);
                break;
            case COLUMN_RABRELEASETIME:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->rabReleaseTime,
                                          table_entry->rabReleaseTime_len);
                break;
            case COLUMN_RABRELEASECAUSE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->rabReleaseCause);
                break;
            case COLUMN_RAB_HISTORY_DLGTPTUNNELID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->dlGtpTunnelId);
                break;
            case COLUMN_RAB_HISTORY_ULGTPTUNNELID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->ulGtpTunnelId);
                break;
            case COLUMN_RAB_HISTORY_GTPKILOBYTESDL:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->gtpKiloBytesDL);
                break;
            case COLUMN_RAB_HISTORY_GTPKILOBYTESUL:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->gtpKiloBytesUL);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
	if(g_rabHistoryFirstCall EQ 1)
         {
            g_rabHistoryFirstCall = 0;
            g_prevRabHistoryReqCount = loopCount;
            // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
            // In the first call itself loopCount becomes one.
            // In General Case, the loopCount will be always > 1 because every table contains 
            // index column to fetch the row and remaining columns contain table attributes.
            if(loopCount EQ 1)
            {
               //Setting it to false in case of SNMP-GET request for single parameter.
               g_sihRabHistoryGetReqProcessed = FALSE;
               // Free all resources.
               free_rabHistoryTable();
            }
         }
         else if(loopCount > 1 || g_prevRabHistoryReqCount > 1)
         {
            //update g_prevReqCount with current loopCount for next call of Table_handler
            g_prevRabHistoryReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihRabHistoryGetReqProcessed = FALSE;
            // Free all resources.
            free_rabHistoryTable();
         }
        break;

    }
    return SNMP_ERR_NOERROR;
}
