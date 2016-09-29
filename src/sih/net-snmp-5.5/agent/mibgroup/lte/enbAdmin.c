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

#include "enbAdmin.h"

/* Handlers */
Netsnmp_Node_Handler enbAdminTable_handler;
Netsnmp_First_Data_Point  enbAdminTable_get_first_data_point;
Netsnmp_Next_Data_Point   enbAdminTable_get_next_data_point;

/* Globals */
struct enbAdminTable_entry  *enbAdminTable_head = NULL;
static TBOOL g_sihEnbAdminSetReqProcessed = FALSE;
static TBOOL g_sihEnbAdminGetReqProcessed = FALSE;
static UINT32 g_prevEnbAdminReqCount = 0;
static UINT32 g_enbAdminFirstCall = 0;

/* free_enbAdminTable */
/**
* @details 
*          This is the routine which frees the enb admin entry.
*/
void free_enbAdminTable(void)
{
  struct enbAdminTable_entry *current = enbAdminTable_head;

  if (current)
  {
  	for (current = enbAdminTable_head->next ; current != NULL; current = current->next) {
    		SNMP_FREE( current );
    	}

  	SNMP_FREE(enbAdminTable_head);
  	enbAdminTable_head = NULL;
  }
  return;
}

/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS.
* @param tlvArray contains pointer to TLV array.
*/
TBOOL sendEnbAdminSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0, pos=0;
  struct enbAdminTable_entry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = ENB_ADMIN_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++)
      {
      struct enbAdminTable_entry *ptr=(struct enbAdminTable_entry *)malloc(sizeof(struct enbAdminTable_entry));

      clearBytes (ptr, sizeof(struct enbAdminTable_entry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++, pos++)
        {
        if (colmn EQ COLUMN_ENB_ADMIN_INDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_ENBADMINCMD)
           {
            ptr->enbAdminCmd =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           enbAdminTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (enbAdminTable_head)
        return TRUE;
    else
        return FALSE;
}


/**
* @details 
*          This routine sends enodeb config set request to BSM, it uses SIH Set
*          Processor to create and send the message to BSM.
* @param sihTagInfo is used to have the tag information for set request.
*/
TBOOL sendEnbAdminSnmpSetRequest (
      TagInfo  sihTagInfo
      )
{
   TlvArray tlvArray;
   enbAdminTableEntry * setReqData = NULL;
   TBOOL status = FALSE;
   int colmn = 0, maxCols = 0;

   clearBytes(&tlvArray,sizeof(tlvArray));

   setReqData = enbAdminTable_head;

  /* max columns in row */
   maxCols = ENB_ADMIN_MAX_COL;

   /* Set the Tags */
   if (sihTagInfo.colId)
   {
    /* Tag for setting an element in table */
    tlvArray.tlvInfo[0].tag = sihTagInfo;
   }
   else
   {
   /* Tag for setting a row in table */
   for(colmn = 1 ; colmn <= maxCols ; colmn++)
      {
      tlvArray.tlvInfo[colmn-1].tag.tableId  = sihTagInfo.tableId;
      tlvArray.tlvInfo[colmn-1].tag.rowId    = sihTagInfo.rowId;
      tlvArray.tlvInfo[colmn-1].tag.colId    = colmn;
      }
    }

   /* Fill Data in TLV */
   for(colmn = 1; colmn <= maxCols ; colmn++)
      {
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) &&
           (colmn EQ COLUMN_ENB_ADMIN_INDEX)) ||
           (sihTagInfo.colId EQ COLUMN_ENB_ADMIN_INDEX))
           {
           putUint32InArray( setReqData->index, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->index);
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_ENB_ADMIN_INDEX)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) &&
           (colmn EQ COLUMN_ENBADMINCMD)) ||
           (sihTagInfo.colId EQ COLUMN_ENBADMINCMD))
           {
           putUint32InArray( setReqData->enbAdminCmd, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->enbAdminCmd);
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_ENBADMINCMD)
               break;
           }
       }

   /* Invoke SIH Set Processor */
   status = sihSetProcessor (sihTagInfo,  &tlvArray);
   if (status EQ TRUE)
     {
      g_sihEnbAdminSetReqProcessed = TRUE;
      g_sihEnbAdminGetReqProcessed = FALSE;
      /* free the resources */
      free_enbAdminTable();
     }
   return status;
}

/** Initializes the enbAdmin module */
void
init_enbAdmin(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_enbAdminTable();
}

/*  # Determine the first/last column names */

/** Initialize the enbAdminTable table by defining its contents and how it's structured */
void
initialize_table_enbAdminTable(void)
{
    const oid enbAdminTable_oid[] = {1,3,6,1,4,1,9344,501,ENBADMIN,1};
    const size_t enbAdminTable_oid_len   = OID_LENGTH(enbAdminTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("enbAdmin:init", "initializing table enbAdminTable\n"));

    reg = netsnmp_create_handler_registration(
              "enbAdminTable",     enbAdminTable_handler,
              enbAdminTable_oid, enbAdminTable_oid_len,
              HANDLER_CAN_RWRITE
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: index */
                           0);
    table_info->min_column = COLUMN_ENB_ADMIN_INDEX;
    table_info->max_column = COLUMN_ENBADMINCMD;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = enbAdminTable_get_first_data_point;
    iinfo->get_next_data_point  = enbAdminTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct enbAdminTable_entry *
enbAdminTable_createEntry(
                 u_long  index,
                ) {
    struct enbAdminTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct enbAdminTable_entry);
    if (!entry)
        return NULL;

    entry->index = index;
    entry->next = enbAdminTable_head;
    enbAdminTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
enbAdminTable_removeEntry( struct enbAdminTable_entry *entry ) {
    struct enbAdminTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = enbAdminTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        enbAdminTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
enbAdminTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;

    if (g_sihEnbAdminGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = ENBADMIN;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
        sihTagInfo.colId = CONFIG_COL_ID_NONE;

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (enbAdminTable_head)
                {
                g_enbAdminFirstCall = 1;
                g_sihEnbAdminGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }

    *my_loop_context = enbAdminTable_head;
    return enbAdminTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
enbAdminTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct enbAdminTable_entry *entry = (struct enbAdminTable_entry *)*my_loop_context;
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


/** handles requests for the enbAdminTable table */
int
enbAdminTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct enbAdminTable_entry          *table_entry = NULL;
    int loopCount = 0;
    int ret;
    TagInfo sihTagInfo;
    int noofColms = 0, rowIndex = 0, colId = 0;
    TBOOL status = FALSE;

    logInfo("enbAdmin:handler Processing request (%d)\n", reqinfo->mode);

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0)
            {
             g_prevEnbAdminReqCount = 0;
             g_sihEnbAdminGetReqProcessed = FALSE;

             /* free the resources */
             free_enbAdminTable();
             /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
              * on single parameter -- 20/07/11 */
             loopCount--;
             continue;
            }

            /* perform anything here that you need to do before each
               request is processed. */

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the enodebConfigTable table
               in question */

            table_entry = (struct enbAdminTable_entry *)
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
            case COLUMN_ENB_ADMIN_INDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->index);
                break;
            case COLUMN_ENBADMINCMD:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->enbAdminCmd);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
        if(g_enbAdminFirstCall EQ 1)
        {
           g_enbAdminFirstCall = 0;
           g_prevEnbAdminReqCount = loopCount;
           // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
           // In the first call itself loopCount becomes one.
           // In General Case, the loopCount will be always > 1 because every table contains
           // index column to fetch the row and remaining columns contain table attributes.
           if(loopCount EQ 1)
           {
             //Setting it to false in case of SNMP-GET request for single parameter.
             g_sihEnbAdminGetReqProcessed = FALSE;
             // Free all resources.
             free_enbAdminTable();
           }
        }
        else if(loopCount > 1 || g_prevEnbAdminReqCount > 1)
        {
           //update g_prevReqCount with current loopCount for next call of Table_handler
           g_prevEnbAdminReqCount = loopCount;
        }
        else if(loopCount EQ 1)
        {
           //Setting it to false in case of SNMP-GET request for single parameter.
           g_sihEnbAdminGetReqProcessed = FALSE;
           // Free all resources.
           free_enbAdminTable();
        }
        break;

        /*
         * Write-support
         */
    case MODE_SET_RESERVE1:
#ifndef PWAV_SIH
        for (request=requests; request; request=request->next) {
            table_entry = (struct enbAdminTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_INDEX:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_ENBADMINCMD:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            default:
                netsnmp_set_request_error( reqinfo, request,
                                           SNMP_ERR_NOTWRITABLE );
                return SNMP_ERR_NOERROR;
            }
        }
#endif /* PWAV_SIH */
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:

	g_sihEnbAdminSetReqProcessed = FALSE;

        for (request=requests; request; request=request->next) {
            table_entry = (struct enbAdminTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);

            if(table_entry == NULL){
                logInfo(" %s\n", "table_entry is NULL");
                if (reqinfo->mode == MODE_SET_ACTION) {
                    netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
                    continue;
                }
                /* XXX: no row existed, if you support creation and this is a
                   set, start dealing with it here, else continue */
            }

            if(table_info == NULL){
                logInfo(" %s\n", "table_info is NULL");
                continue;
            }
   
            colId = table_info->colnum;
            /* set the row index */
            rowIndex = requests->index;
 
            switch (table_info->colnum) {
            case COLUMN_ENB_ADMIN_INDEX:
                table_entry->index     = *request->requestvb->val.integer;
                break;
            case COLUMN_ENBADMINCMD:
                table_entry->enbAdminCmd     = *request->requestvb->val.integer;
                break;
            }
        }

        if ((request == NULL) && (g_sihEnbAdminSetReqProcessed EQ FALSE))
            /* Processed all requests and table entry is filled with
             * requested data. */
            {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = ENBADMIN;

            sihTagInfo.rowId = rowIndex;

            if (noofColms EQ 1)
                {
                sihTagInfo.colId = colId;
                }
            else
                {
                sihTagInfo.colId = CONFIG_COL_ID_NONE;
                }

            status = sendEnbAdminSnmpSetRequest (sihTagInfo);
            if (status EQ TRUE)
                {
                logInfo("%s","SET Succedded");
                }
            else
                netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_GENERR );
            }
        break;

    case MODE_SET_UNDO:
#ifndef PWAV_SIH
        for (request=requests; request; request=request->next) {
            table_entry = (struct enbAdminTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_INDEX:
                table_entry->index     = table_entry->old_index;
                table_entry->old_index = 0;
                break;
            case COLUMN_ENBADMINCMD:
                table_entry->enbAdminCmd     = table_entry->old_enbAdminCmd;
                table_entry->old_enbAdminCmd = 0;
                break;
            }
        }
#endif /* PWAV_SIH */
        break;

    case MODE_SET_COMMIT:
        break;
    }
    return SNMP_ERR_NOERROR;
}