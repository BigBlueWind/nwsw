/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 17695 2009-07-21 12:22:18Z dts12 $
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

#include "fileUploadStatus.h"

/* function declarations */
Netsnmp_Node_Handler fileUploadStatusTable_handler;
Netsnmp_First_Data_Point  fileUploadStatusTable_get_first_data_point;
Netsnmp_Next_Data_Point   fileUploadStatusTable_get_next_data_point;

/* globals */
struct fileUploadStatusTable_entry  *fileUploadStatusTable_head = NULL;
static TBOOL g_sihFileUploadStatusGetReqProcessed = FALSE;
static UINT32 g_prevFileUploadStatusReqCount = 0;
static UINT32 g_fileUploadStatusFirstCall = 0;

/* free_fileUploadStatusTable */
/**
* @details 
*          This is the routine which frees the file upload status entry. 
*/
void free_fileUploadStatusTable(void)
{
  struct fileUploadStatusTable_entry * current = fileUploadStatusTable_head;

  if (current) 
  {
  for (current = fileUploadStatusTable_head->next ; current != NULL; current = current->next)    {
    SNMP_FREE( current );  
   }
   
  SNMP_FREE(fileUploadStatusTable_head);
  fileUploadStatusTable_head = NULL;
  }
  return;
}

/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS. 
* @param tlvArray contains pointer to TLV array.     
*/
TBOOL sendFileUploadStatusSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0;
  struct fileUploadStatusTable_entry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = FILE_UPLOAD_STATUS_MAX_COL;

  if (!totalNumCol)
      {
      logError("Invalid no. of columns in response",NULL);
      return FALSE;
      }

  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++) 
      {
      struct fileUploadStatusTable_entry *ptr=(struct fileUploadStatusTable_entry *) malloc(sizeof(struct fileUploadStatusTable_entry));

      clearBytes (ptr,sizeof(struct fileUploadStatusTable_entry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
        {
        if (colmn EQ COLUMN_UPINDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_FILEUPLOADCOMMANDRECEIVED)
           {
            ptr->fileUploadCommandReceived =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_UPSTSFILENAME)
           {
            clearBytes (ptr->fileName, FILE_UPLOAD_BUFFER_LEN_MAX);
            if (tlvArray->tlvInfo[colmn-1].len > 0)
            {
            copyBytes (tlvArray->tlvInfo[colmn-1].data, 
                       tlvArray->tlvInfo[colmn-1].len, ptr->fileName);
            }
            ptr->fileName_len = strlen(ptr->fileName); 
            continue;
           }
        if (colmn EQ COLUMN_UPLOADSTATUS)
           {
            ptr->uploadStatus = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_UPPERCENTAGECOMPLETE)
           {
            ptr->percentageComplete = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           fileUploadStatusTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (fileUploadStatusTable_head)
        return TRUE;
    else
        return FALSE;
}


/** Initializes the fileUploadStatus module */
void
init_fileUploadStatus(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_fileUploadStatusTable();
}

/*  # Determine the first/last column names */

/** Initialize the fileUploadStatusTable table by defining its contents and how it's structured */
void
initialize_table_fileUploadStatusTable(void)
{
    const oid fileUploadStatusTable_oid[] = {1,3,6,1,4,1,9344,501,FILEUPLOADSTATUS,1};
    const size_t fileUploadStatusTable_oid_len   = OID_LENGTH(fileUploadStatusTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("fileUploadStatus:init", "initializing table fileUploadStatusTable\n"));

    reg = netsnmp_create_handler_registration(
              "fileUploadStatusTable",     fileUploadStatusTable_handler,
              fileUploadStatusTable_oid, fileUploadStatusTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_INTEGER,  /* index: Index */
                           0);
    table_info->min_column = COLUMN_UPINDEX;
    table_info->max_column = COLUMN_UPPERCENTAGECOMPLETE;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = fileUploadStatusTable_get_first_data_point;
    iinfo->get_next_data_point  = fileUploadStatusTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct fileUploadStatusTable_entry *
fileUploadStatusTable_createEntry(
                 long  fileUploadCommand,
                ) {
    struct fileUploadStatusTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct fileUploadStatusTable_entry);
    if (!entry)
        return NULL;

    entry->fileUploadCommand = fileUploadCommand;
    entry->next = fileUploadStatusTable_head;
    fileUploadStatusTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
fileUploadStatusTable_removeEntry( struct fileUploadStatusTable_entry *entry ) {
    struct fileUploadStatusTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = fileUploadStatusTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        fileUploadStatusTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
fileUploadStatusTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;

    if (g_sihFileUploadStatusGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = FILEUPLOADSTATUS;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE; 
        sihTagInfo.colId = CONFIG_COL_ID_NONE;                

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (fileUploadStatusTable_head)
                {
		g_fileUploadStatusFirstCall = 1;
                g_sihFileUploadStatusGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }        

    *my_loop_context = fileUploadStatusTable_head;
    return fileUploadStatusTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
fileUploadStatusTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct fileUploadStatusTable_entry *entry = (struct fileUploadStatusTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->index );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the fileUploadStatusTable table */
int
fileUploadStatusTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct fileUploadStatusTable_entry          *table_entry = NULL;
    int loopCount = 0;

    DEBUGMSGTL(("fileUploadStatus:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0)
                {
		g_prevFileUploadStatusReqCount = 0;
                /*Processed all requests, rows and colmns */
                g_sihFileUploadStatusGetReqProcessed = FALSE;
                /* free the resources */
                free_fileUploadStatusTable();
                /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
                 * on single parameter -- 20/07/11 */
                loopCount--;
                continue;
                }

            /* perform anything here that you need to do before each
               request is processed. */           

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the traceConfigTable table 
               in question */                            

            table_entry = (struct fileUploadStatusTable_entry *)
                             netsnmp_extract_iterator_context(request);

            if ( table_entry == NULL) {
                if (reqinfo->mode == MODE_GET) {
                    netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
                    continue;
                }
                /* XXX: no row existed, if you support creation and this is a
                   set, start dealing with it here, else continue */
            }

            table_info  =     netsnmp_extract_table_info(request);
            /* table_info->colnum contains the column number requested */
            /* table_info->indexes contains a linked list of snmp variable
               bindings for the indexes of the table.  Values in the list
               have been set corresponding to the indexes of the
               request */
            if (table_info==NULL) {
                continue;
            } 
    
            switch (table_info->colnum) {
            case COLUMN_UPINDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->index);
                break;
            case COLUMN_FILEUPLOADCOMMANDRECEIVED:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->fileUploadCommandReceived);
                break;
            case COLUMN_UPSTSFILENAME:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
           
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->fileName,
                                          table_entry->fileName_len);
                break;
            case COLUMN_UPLOADSTATUS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->uploadStatus);
                break;
            case COLUMN_UPPERCENTAGECOMPLETE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->percentageComplete);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
	if(g_fileUploadStatusFirstCall EQ 1)
         {
            g_fileUploadStatusFirstCall = 0;
            g_prevFileUploadStatusReqCount = loopCount;
            // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
            // In the first call itself loopCount becomes one.
            // In General Case, the loopCount will be always > 1 because every table contains 
            // index column to fetch the row and remaining columns contain table attributes.
            if(loopCount EQ 1)
            {
               //Setting it to false in case of SNMP-GET request for single parameter.
               g_sihFileUploadStatusGetReqProcessed = FALSE;
               // Free all resources.
               free_fileUploadStatusTable();
            }
         }
         else if(loopCount > 1 || g_prevFileUploadStatusReqCount > 1)
         {
            //update g_prevReqCount with current loopCount for next call of Table_handler
            g_prevFileUploadStatusReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihFileUploadStatusGetReqProcessed = FALSE;
            // Free all resources.
            free_fileUploadStatusTable();
         }	
        break;

    }
    return SNMP_ERR_NOERROR;
}
