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

#include "logConfig.h"

Netsnmp_Node_Handler logConfigTable_handler;
Netsnmp_First_Data_Point  logConfigTable_get_first_data_point;
Netsnmp_Next_Data_Point   logConfigTable_get_next_data_point;

struct logConfigTable_entry  *logConfigTable_head = NULL;
static TBOOL g_sihLogConfigSetReqProcessed = FALSE;
static TBOOL g_sihLogConfigGetReqProcessed = FALSE;
static UINT32 g_prevLogConfigReqCount = 0;
static UINT32 g_logConfigFirstCall = 0;

/* free_logConfigTable */
/**
* @details 
*          This is the routine which frees the log config entry. 
*/
void free_logConfigTable(void)
{
  
  logConfigTableEntry  * current = logConfigTable_head;
  logConfigTableEntry  * temp = NULL;
  
  if(current)
  {
    while(current->next != NULL)
    {
	temp = current->next;
	SNMP_FREE( current );
	current = temp;
    }

    SNMP_FREE( current );
    logConfigTable_head = NULL;
  }
  return;
}


/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS. 
* @param tlvArray contains pointer to TLV array.     
*/
TBOOL sendLogConfigSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0, pos=0;
  logConfigTableEntry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = LOG_CONFIG_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  for (i=1;  i <= totalNumRow;  i++) 
      {
      logConfigTableEntry *ptr=(logConfigTableEntry *)malloc(sizeof(logConfigTableEntry));

      clearBytes(ptr,sizeof(logConfigTableEntry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++, pos++)
        {
       if (colmn EQ COLUMN_MODULEID)
           {
            ptr->moduleId = getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }        
       if (colmn EQ COLUMN_LOGMASK1)
           {
            ptr->logMask1 =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }        
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           logConfigTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (logConfigTable_head)
        return TRUE;
    else
        return FALSE;
}

/**
* @details 
*          This routine sends log config set request to BSM, it uses SIH Set 
*          Processor to create and send the message to BSM. 
* @param sihTagInfo is used to have the tag information for set request.
*/
TBOOL sendLogConfigSnmpSetRequest ( 
      TagInfo  sihTagInfo,
      struct logConfigTable_entry  *logConfig_entry
      )
{
   TlvArray tlvArray;
   logConfigTableEntry * setReqData = NULL;   
   TBOOL status = FALSE;
   int colmn = 0, maxCols = 0;
   unsigned char * ipAddr;
   struct in_addr ip;

   clearBytes(&tlvArray,sizeof(tlvArray));

   setReqData = logConfig_entry;

  /* max columns in row */
   maxCols = LOG_CONFIG_MAX_COL;

   /* Set the Tags */
   if (sihTagInfo.colId)
   {
    /* Tag for setting an element in table */
    tlvArray.tlvInfo[0].tag = sihTagInfo;
    logInfo("Preparing SET message with TLV details tableId=%d,rowId=%d, colId=%d", 
		tlvArray.tlvInfo[0].tag.tableId,
		tlvArray.tlvInfo[0].tag.rowId,
		tlvArray.tlvInfo[0].tag.colId);
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
    logInfo("Preparing SET message with TLV details tableId=%d,rowId=%d, colId=%d", 
		tlvArray.tlvInfo[colmn-1].tag.tableId,
		tlvArray.tlvInfo[colmn-1].tag.rowId,
		tlvArray.tlvInfo[colmn-1].tag.colId);
    }


   /* Fill Data in TLV */
   for(colmn = 1; colmn <= maxCols ; colmn++)
      {
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) &&
           (colmn EQ COLUMN_MODULEID)) || 
           (sihTagInfo.colId EQ COLUMN_MODULEID))
           {
           putUint32InArray( setReqData->moduleId, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->moduleId);     
           tlvArray.numTlvs++;           
           if (sihTagInfo.colId EQ COLUMN_MODULEID)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) &&
           (colmn EQ COLUMN_LOGMASK1)) || 
           (sihTagInfo.colId EQ COLUMN_LOGMASK1))
           {
           putUint32InArray( setReqData->logMask1, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->logMask1);
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_LOGMASK1)
               break;
           }
       }

   /* Invoke SIH Set Processor */
   status = sihSetProcessor (sihTagInfo,  &tlvArray);
   if (status EQ TRUE)
     {
      g_sihLogConfigSetReqProcessed = TRUE;
      g_sihLogConfigGetReqProcessed = FALSE;
      /* free the resources */      
      free_logConfigTable(); 
     }
   return status;
}


/**
* @details 
*          This is the routine for initializing logConfig module.
*/
void
init_logConfig(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_logConfigTable();
}

/* Determine the first/last column names

* Initialize the logConfigTable table by defining its contents and how it's structured */
void
initialize_table_logConfigTable(void)
{
    const oid logConfigTable_oid[] = {1,3,6,1,4,1,9344,501,LOGCONFIG,1};
    const size_t logConfigTable_oid_len   = OID_LENGTH(logConfigTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("logConfig:init", "initializing table logConfigTable\n"));

    reg = netsnmp_create_handler_registration(
              "logConfigTable",     logConfigTable_handler,
              logConfigTable_oid, logConfigTable_oid_len,
              HANDLER_CAN_RWRITE
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: moduleId */
                           0);
    table_info->min_column = COLUMN_MODULEID;
    table_info->max_column = LOG_CONFIG_MAX_COL;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = logConfigTable_get_first_data_point;
    iinfo->get_next_data_point  = logConfigTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH 
/* create a new row in the (unsorted) table */
struct logConfigTable_entry *
logConfigTable_createEntry(
                 unsigned int  moduleId,
                ) {
    struct logConfigTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct logConfigTable_entry);
    if (!entry)
        return NULL;

    entry->moduleId = moduleId;
    entry->next = logConfigTable_head;
    logConfigTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
logConfigTable_removeEntry( struct logConfigTable_entry *entry ) {
    struct logConfigTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = logConfigTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        logConfigTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
logConfigTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;

    if (g_sihLogConfigGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = LOGCONFIG;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE; 
        sihTagInfo.colId = CONFIG_COL_ID_NONE;                

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (logConfigTable_head)
                {
		g_logConfigFirstCall = 1;
                g_sihLogConfigGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }        
    
    *my_loop_context = logConfigTable_head;
    return logConfigTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
logConfigTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct logConfigTable_entry *entry = (struct logConfigTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_UNSIGNED, entry->moduleId );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the logConfigTable table */
int
logConfigTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct logConfigTable_entry          *table_entry = NULL;
    int loopCount = 0;
    int ret;
    TagInfo sihTagInfo;
    int noofColms = 0, rowIndex = 0, colId = 0;
    TBOOL status = FALSE;    


    DEBUGMSGTL(("logConfig:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0)
            {
	     g_prevLogConfigReqCount = 0;	
             g_sihLogConfigGetReqProcessed = FALSE;

             /* free the resources */
             free_logConfigTable(); 
             /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
              * on single parameter -- 20/07/11 */
             loopCount--;
             continue;
            }

            /* perform anything here that you need to do before each
               request is processed. */           

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the logConfigTable table 
               in question */    

            table_entry = (struct logConfigTable_entry *)
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
            case COLUMN_MODULEID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->moduleId);
                break;
            case COLUMN_LOGMASK1:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->logMask1);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
	if(g_logConfigFirstCall EQ 1)
         {
            g_logConfigFirstCall = 0;
            g_prevLogConfigReqCount = loopCount;
            // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
            // In the first call itself loopCount becomes one.
            // In General Case, the loopCount will be always > 1 because every table contains 
            // index column to fetch the row and remaining columns contain table attributes.
            if(loopCount EQ 1)
            {
               //Setting it to false in case of SNMP-GET request for single parameter.
               g_sihLogConfigGetReqProcessed = FALSE;
               // Free all resources.
               free_logConfigTable();
            }
         }
         else if(loopCount > 1 || g_prevLogConfigReqCount > 1)
         {
            //update g_prevReqCount with current loopCount for next call of Table_handler
            g_prevLogConfigReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihLogConfigGetReqProcessed = FALSE;
            // Free all resources.
            free_logConfigTable();
         }
        break;

        /*
         * Write-support
         */
    case MODE_SET_RESERVE1:
#ifndef PWAV_SIH      
        for (request=requests; request; request=request->next) {
            table_entry = (struct logConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_MODULEID:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_LOGMASK1:
	        /* or possibly 'netsnmp_check_vb_type_and_size' */
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
        g_sihLogConfigSetReqProcessed = FALSE; 
        
        for (request=requests; request; request=request->next) {
            table_entry = (struct logConfigTable_entry *)
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
            rowIndex = requests->requestvb->name_loc[OID_ROW_POSITION];

            switch (table_info->colnum) {
            case COLUMN_MODULEID:
                table_entry->moduleId     = *request->requestvb->val.integer;
                noofColms++;
                break;
            case COLUMN_LOGMASK1:
                table_entry->logMask1     = *request->requestvb->val.integer;
                noofColms++;
                break;
            }
        }

        if ((request == NULL) && (g_sihLogConfigSetReqProcessed EQ FALSE))
            /* Processed all requests and table entry is filled with
             * requested data. */
            {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = LOGCONFIG;

            sihTagInfo.rowId = rowIndex; 

            if (noofColms EQ 1)
                {
                sihTagInfo.colId = colId;                
                }
            else
                {
                sihTagInfo.colId = CONFIG_COL_ID_NONE;                
                }

            status = sendLogConfigSnmpSetRequest (sihTagInfo, table_entry);
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
            table_entry = (struct logConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_ENABLEINFOLOGS:
                table_entry->enableInfoLogs     = table_entry->old_enableInfoLogs;
                table_entry->old_enableInfoLogs = 0;
                break;
            case COLUMN_ENABLEERRORLOGS:
                table_entry->enableErrorLogs     = table_entry->old_enableErrorLogs;
                table_entry->old_enableErrorLogs = 0;
                break;
            case COLUMN_ENABLESTATECHANGELOGS:
                table_entry->enableStateChangeLogs     = table_entry->old_enableStateChangeLogs;
                table_entry->old_enableStateChangeLogs = 0;
                break;
            case COLUMN_ENABLETIMERLOGS:
                table_entry->enableTimerLogs     = table_entry->old_enableTimerLogs;
                table_entry->old_enableTimerLogs = 0;
                break;
            case COLUMN_ENABLEMODULESPECIFICLOG1:
                table_entry->enableModuleSpecificLog1     = table_entry->old_enableModuleSpecificLog1;
                table_entry->old_enableModuleSpecificLog1 = 0;
                break;
            case COLUMN_ENABLEMODULESPECIFICLOG2:
                table_entry->enableModuleSpecificLog2     = table_entry->old_enableModuleSpecificLog2;
                table_entry->old_enableModuleSpecificLog2 = 0;
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
