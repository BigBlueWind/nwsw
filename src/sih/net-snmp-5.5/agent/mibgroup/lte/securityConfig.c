#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "bs_modules.h"
#include "all_cfg.h"
#include "all_cmn.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "syslog.h"
#include "securityConfig.h"

/* function declarations */
Netsnmp_Node_Handler     securityConfigTable_handler;
Netsnmp_First_Data_Point securityConfigTable_get_first_data_point;
Netsnmp_Next_Data_Point  securityConfigTable_get_next_data_point;

/* Global Variables */
struct securityConfigTable_entry *securityConfigTable_head = NULL;
static TBOOL g_sihSecurityConfigGetReqProcessed = FALSE;
static TBOOL g_sihSecurityConfigSetReqProcessed = FALSE;
static UINT32 g_prevSecurityConfigReqCount = 0;
static UINT32 g_securityConfigFirstCall = 0;

/**
 * @details
 *          This is the routine which frees the securityConfig entry.
 */
void free_securityConfigTable(void)
{
  securityConfigTableEntry *current = securityConfigTable_head;

  if(current)
  {
    for (current = securityConfigTable_head->next; current != NULL; current = current->next)
    {
      SNMP_FREE( current );
    }

    SNMP_FREE(securityConfigTable_head);
    securityConfigTable_head = NULL;
  }
  return;
}

/**
 * @details
 *          This is the routine which handles get response from SIH Get Processor
 *          and Prepare the SNMP GET Response for EMS.
 * @param tlvArray containf pointer to TLV array.
 */

TBOOL sendSecurityConfigSnmpGetResponse (
  IO TlvArray *tlvArray
  )
{
  int totalNumRow, totalNumCol, i=0, colmn=0;
  securityConfigTableEntry *prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = SECURITYCONFIG_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  for (i=1;  i <= totalNumRow;  i++)
  {
    securityConfigTableEntry *ptr = (securityConfigTableEntry *)malloc(sizeof(securityConfigTableEntry));

    clearBytes(ptr, sizeof(securityConfigTableEntry));

    /* Fill Data in TLV */
    for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
    {
      if(colmn EQ SECURITYCONFIG_INDEX)
         {
	  ptr->index = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
          continue;                                                    
         }                                                            
      if(colmn EQ SECURITYCONFIG_UUCIPHERING)
      {
        ptr->uuCiphering = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ SECURITYCONFIG_UUINTEGRITYPROTECTION)
      {
        ptr->uuIntegrityProtection = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
    } /* Ending of InnerLoop */

  ptr->next = NULL;

    if(prevPtr == NULL)
    {
      securityConfigTable_head = prevPtr = ptr;
    }
    prevPtr->next = ptr;
    prevPtr = ptr;
  } /*Ending of OuterLoop */
  prevPtr->next = NULL;

  if(securityConfigTable_head)
    return TRUE;
  else
    return FALSE;
}
/**
 * @details
 *          This routine sends securityConfig set request to BSM, it uses SIH Set
 *          Processor to create and send the message to BSM.
 * @param sihTagInfo is used to have the tag information for set request.
 */
TBOOL sendSecurityConfigSnmpSetRequest (
      IO TagInfo sihTagInfo,
      IO TlvArray *tlvArray
      )
{
  securityConfigTableEntry *setReqData = NULL;
  TBOOL status = FALSE;
  int colmn = 0;

  setReqData = securityConfigTable_head;

  /* Fill Data in TLV */
   for(colmn = 0; colmn < tlvArray->numTlvs; colmn++)
  {
    if(tlvArray->tlvInfo[colmn].tag.colId EQ SECURITYCONFIG_INDEX)
    {
      putUint32InArray(setReqData->index, tlvArray->tlvInfo[colmn].data);
      tlvArray->tlvInfo[colmn].len = sizeof(setReqData->index);
    }
    if(tlvArray->tlvInfo[colmn].tag.colId EQ SECURITYCONFIG_UUCIPHERING)
    {
      putUint32InArray(setReqData->uuCiphering, tlvArray->tlvInfo[colmn].data);
      tlvArray->tlvInfo[colmn].len = sizeof(setReqData->uuCiphering);
    }
    if(tlvArray->tlvInfo[colmn].tag.colId EQ SECURITYCONFIG_UUINTEGRITYPROTECTION)
    {
      putUint32InArray(setReqData->uuIntegrityProtection, tlvArray->tlvInfo[colmn].data);
      tlvArray->tlvInfo[colmn].len = sizeof(setReqData->uuIntegrityProtection);
    }
  }

  /* Invoke SIH Set Processor */
  status = sihSetProcessor (sihTagInfo,  tlvArray);
  if(status EQ TRUE)
  {
    g_sihSecurityConfigSetReqProcessed = TRUE;
    g_sihSecurityConfigGetReqProcessed = FALSE;
    /* free the resources */
    free_securityConfigTable();
  }
  return status;
}

 /** Initializes the securityConfig module */
void 
init_securityConfig(void)
{
  /* here we initialize all the tables we're planning on supporting */
  initialize_table_securityConfigTable();
}

/** Initialize thesecurityConfigTable table by defining its contents and how it's structured */

void 
initialize_table_securityConfigTable(void)
{
  const oid securityConfigTable_oid[] = {1,3,6,1,4,1,9344,501,SECURITYCONFIG,1};
  const size_t securityConfigTable_oid_len = OID_LENGTH(securityConfigTable_oid);
  netsnmp_handler_registration    *reg;
  netsnmp_iterator_info           *iinfo;
  netsnmp_table_registration_info *table_info;

  DEBUGMSGTL(("securityConfig:init", "initializing table securityConfigTable\n"));

  reg = netsnmp_create_handler_registration(
            "securityConfigTable", securityConfigTable_handler,
            securityConfigTable_oid, securityConfigTable_oid_len,
            HANDLER_CAN_RWRITE
            );

  table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
  netsnmp_table_helper_add_indexes(table_info,
                          ASN_UNSIGNED, /* index: index */
                          0);
  table_info->min_column = SECURITYCONFIG_INDEX;
  table_info->max_column = SECURITYCONFIG_MAX_COL;

  iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
  iinfo->get_first_data_point = securityConfigTable_get_first_data_point;
  iinfo->get_next_data_point  = securityConfigTable_get_next_data_point;
  iinfo->table_reginfo        = table_info;

  netsnmp_register_table_iterator( reg, iinfo );

   /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH  
  /* create a new row in the (unsorted) table */                     
struct securityConfigTable_entry *
securityConfigTable_createEntry(                                        
                 unsigned int index,
                  )
{
      struct securityConfigTable_entry *entry;                            
                                                                     
      entry = SNMP_MALLOC_TYPEDEF(struct securityConfigTable_entry);      
      if (!entry)                                                    
          return NULL;                                               
                                                                     
      entry->index = index;                       
      entry->next = securityConfigTable_head;                             
      securityConfigTable_head = entry;                                   
      return entry;                                                  
}     

/* remove a row from the table */
void
securityConfigTable_removeEntry( struct securityConfigTable_entry *entry ) {
    struct securityConfigTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = securityConfigTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        securityConfigTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
securityConfigTable_get_first_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  TagInfo sihTagInfo;
  TBOOL status = FALSE;

  if(g_sihSecurityConfigGetReqProcessed EQ FALSE)
  {
    /* Set SIH Get Request Tag Info */
    sihTagInfo.tableId = SECURITYCONFIG;
    sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
    sihTagInfo.colId = CONFIG_COL_ID_NONE;

    status = sihGetProcessor (sihTagInfo);
    if(status EQ TRUE)
    {
      syslog(LOG_INFO,"Get Processor Succedded");
      if(securityConfigTable_head)
      {
        g_securityConfigFirstCall = 1;
        g_sihSecurityConfigGetReqProcessed = TRUE;
      }
    }
    else
    {
      syslog(LOG_INFO,"Get Processor Failed");
    }
  }

  *my_loop_context = securityConfigTable_head;
  return securityConfigTable_get_next_data_point(my_loop_context, my_data_context,
                                            put_index_data,  mydata );
}

netsnmp_variable_list *
securityConfigTable_get_next_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  struct securityConfigTable_entry *entry = (struct securityConfigTable_entry *)*my_loop_context;
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

/** handles requests for the securityConfigTable table */
int
securityConfigTable_handler(
    netsnmp_mib_handler            *handler,
    netsnmp_handler_registration   *reginfo,
    netsnmp_agent_request_info     *reqinfo,
    netsnmp_request_info           *requests)
{
  netsnmp_request_info       *request = NULL;
  netsnmp_table_request_info *table_info = NULL;
  struct securityConfigTable_entry  *table_entry = NULL;
  int loopCount = 0;

  int ret;
  TagInfo sihTagInfo;
  TlvArray tlvArray;
  int rowIndex = 0, colId = 0;
  TBOOL status = FALSE;
  DEBUGMSGTL(("securityConfig:handler", "Processing request (%d)\n", reqinfo->mode));

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
             g_prevSecurityConfigReqCount = 0;
             g_sihSecurityConfigGetReqProcessed = FALSE;

    /* free the resources */
             free_securityConfigTable();
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
              help return data for the columns of the securityConfigTable table
              in question. */

           table_entry = (struct securityConfigTable_entry *)
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
           if(table_info == NULL)
           {
             continue;
           }
           switch (table_info->colnum)
           {
             case SECURITYCONFIG_INDEX:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->index);
                  break;
             case SECURITYCONFIG_UUCIPHERING:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                              table_entry->uuCiphering);
                  break;
             case SECURITYCONFIG_UUINTEGRITYPROTECTION:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                              table_entry->uuIntegrityProtection);
                  break;
             default:
                  netsnmp_set_request_error(reqinfo, request,
                                            SNMP_NOSUCHOBJECT);
                  break;
           }
         }
         if(g_securityConfigFirstCall EQ 1)
       {
           g_securityConfigFirstCall = 0;
           g_prevSecurityConfigReqCount = loopCount;
           // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
           // In the first call itself loopCount becomes one.
           // In General Case, the loopCount will be always > 1 because every table contains
           // index column to fetch the row and remaining columns contain table attributes.
           if(loopCount EQ 1)
           {
             // Setting it to false in case of SNMP-GET request for single parameter.
             g_sihSecurityConfigGetReqProcessed = FALSE;
             // Free all Resources.
             free_securityConfigTable();
           }
         }
         else if(loopCount < 1 || g_prevSecurityConfigReqCount > 1)
         {
           // update g_prevReqCount with current loopCount for next call of Table_handler
           g_prevSecurityConfigReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
           //Setting it to false in case of SNMP-GET request for single parameter.
           g_sihSecurityConfigGetReqProcessed = FALSE;
           // Free all resources.
           free_securityConfigTable();
         }
         break;

    /*
     * Write-support
     */
    case MODE_SET_RESERVE1:
       //  break;
#ifndef PWAV_SIH

        for (request=requests; request; request=request->next) {
            table_entry = (struct securityConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case SECURITYCONFIG_INDEX: 
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SECURITYCONFIG_UUCIPHERING:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SECURITYCONFIG_UUINTEGRITYPROTECTION :
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
          g_sihSecurityConfigSetReqProcessed = FALSE;

        clearBytes(&sihTagInfo, sizeof(TagInfo));
        clearBytes(&tlvArray, sizeof(TlvArray));

          for(request=requests; request; request=request->next)
          {
            table_entry = (struct securityConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(request);

            if(table_entry == NULL)
            {
              logInfo(" %s\n","table_entry is NULL");
              if(reqinfo->mode == MODE_SET_ACTION)
              {
                netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
                continue;
              }
              /* XXX: no row existed, if you support creation and this is a
                 set, start dealing with it here, else continue */
            }

            if(table_info == NULL)
            {
              logInfo(" %s\n","table_info is NULL");
              continue;
            }

            colId = table_info->colnum;
            /* set the row index */
            rowIndex = requests->index;

            switch (table_info->colnum)
            {
              case SECURITYCONFIG_INDEX:
                   table_entry->index     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SECURITYCONFIG, rowIndex, SECURITYCONFIG_INDEX);
                   tlvArray.numTlvs++;
               break;
              case SECURITYCONFIG_UUCIPHERING:
                   table_entry->uuCiphering     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SECURITYCONFIG, rowIndex, SECURITYCONFIG_UUCIPHERING);
                   tlvArray.numTlvs++;
              break;
              case SECURITYCONFIG_UUINTEGRITYPROTECTION:
                   table_entry->uuIntegrityProtection     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SECURITYCONFIG, rowIndex, SECURITYCONFIG_UUINTEGRITYPROTECTION);
                   tlvArray.numTlvs++;
              break;
              default:
                   netsnmp_set_request_error(reqinfo, request,
                                             SNMP_NOSUCHOBJECT);
              break;
            }
          }

          /* Processed all requests and table entry is filled with
           * requested data.
           */
          if((request == NULL) && (g_sihSecurityConfigSetReqProcessed EQ FALSE))
          {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = SECURITYCONFIG;

            sihTagInfo.rowId = rowIndex;

            status = sendSecurityConfigSnmpSetRequest(sihTagInfo, &tlvArray);
            if(status EQ TRUE)
            {
              syslog (LOG_INFO, "SET Succedded");
            }
            else
            {
              netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_GENERR);
            }
          }
          break;

     case MODE_SET_UNDO:
#ifndef PWAV_SIH
        for (request=requests; request; request=request->next) {
            table_entry = (struct securityConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);

            switch (table_info->colnum) {
            case SECURITYCONFIG_INDEX:
                table_entry->index     = table_entry->old_index;
                table_entry->old_index = 0;
                break;
            case SECURITYCONFIG_UUCIPHERING:
                table_entry->uuCiphering     = table_entry->old_uuCiphering;
                table_entry->old_uuCiphering = 0;
                break;
            case SECURITYCONFIG_UUINTEGRITYPROTECTION:
                table_entry->uuIntegrityProtection     = table_entry->old_uuIntegrityProtection;
                table_entry->old_uuIntegrityProtection = 0;
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

