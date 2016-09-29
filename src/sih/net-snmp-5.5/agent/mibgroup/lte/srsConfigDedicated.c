#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "bs_modules.h"
#include "all_cfg.h"
#include "all_cmn.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "syslog.h"
#include "srsConfigDedicated.h"

/* function declarations */
Netsnmp_Node_Handler     srsConfigDedicatedTable_handler;
Netsnmp_First_Data_Point srsConfigDedicatedTable_get_first_data_point;
Netsnmp_Next_Data_Point  srsConfigDedicatedTable_get_next_data_point;

/* Global Variables */
struct srsConfigDedicatedTable_entry *srsConfigDedicatedTable_head = NULL;
static TBOOL g_sihSrsConfigDedicatedGetReqProcessed = FALSE;
static TBOOL g_sihSrsConfigDedicatedSetReqProcessed = FALSE;
static UINT32 g_prevSrsConfigDedicatedReqCount = 0;
static UINT32 g_srsConfigDedicatedFirstCall = 0;

/**
 * @details
 *          This is the routine which frees the srsConfigDedicated entry.
 */
void free_srsConfigDedicatedTable(void)
{
  srsConfigDedicatedTableEntry *current = srsConfigDedicatedTable_head;

  if(current)
  {
    for (current = srsConfigDedicatedTable_head->next; current != NULL; current = current->next)
    {
      SNMP_FREE( current );
    }

    SNMP_FREE(srsConfigDedicatedTable_head);
    srsConfigDedicatedTable_head = NULL;
  }
  return;
}

/**
 * @details
 *          This is the routine which handles get response from SIH Get Processor
 *          and Prepare the SNMP GET Response for EMS.
 * @param tlvArray containf pointer to TLV array.
 */

TBOOL sendSrsConfigDedicatedSnmpGetResponse (
  IO TlvArray *tlvArray
  )
{
  int totalNumRow, totalNumCol, i=0, colmn=0;
  srsConfigDedicatedTableEntry *prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = SRSCONFIGDEDICATED_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  for (i=1;  i <= totalNumRow;  i++)
  {
    srsConfigDedicatedTableEntry *ptr = (srsConfigDedicatedTableEntry *)malloc(sizeof(srsConfigDedicatedTableEntry));

    clearBytes(ptr, sizeof(srsConfigDedicatedTableEntry));

    /* Fill Data in TLV */
    for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
    {
      if(colmn EQ SRSCONFIGDEDICATED_SRSBANDWIDTH)
         {
	  ptr->srsBandwidth = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
          continue;                                                    
         }                                                            
      if(colmn EQ SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH)
      {
        ptr->srsHoppingBandWidth = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ SRSCONFIGDEDICATED_FREQDOMAINPOSITION)
      {
        ptr->frequecyDomainPosition = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ SRSCONFIGDEDICATED_DURATION)
      {
        ptr->duration = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
    } /* Ending of InnerLoop */

  ptr->next = NULL;

    if(prevPtr == NULL)
    {
      srsConfigDedicatedTable_head = prevPtr = ptr;
    }
    prevPtr->next = ptr;
    prevPtr = ptr;
  } /*Ending of OuterLoop */
  prevPtr->next = NULL;

  if(srsConfigDedicatedTable_head)
    return TRUE;
  else
    return FALSE;
}
/**
 * @details
 *          This routine sends srsConfigDedicated set request to BSM, it uses SIH Set
 *          Processor to create and send the message to BSM.
 * @param sihTagInfo is used to have the tag information for set request.
 */
TBOOL sendSrsConfigDedicatedSnmpSetRequest (
      IO TagInfo   sihTagInfo,
      IO TlvArray *tlvArray
      )
{
  srsConfigDedicatedTableEntry *setReqData = NULL;
  TBOOL status = FALSE;
  int colmn = 0;

  setReqData = srsConfigDedicatedTable_head;

  /* Fill Data in TLV */
   for(colmn = 0; colmn < tlvArray->numTlvs; colmn++)
   {
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SRSCONFIGDEDICATED_SRSBANDWIDTH)
     {
       putUint32InArray(setReqData->srsBandwidth, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->srsBandwidth);
     }
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH)
     {
       putUint32InArray(setReqData->srsHoppingBandWidth, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->srsHoppingBandWidth);
     }
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SRSCONFIGDEDICATED_FREQDOMAINPOSITION)
     {
       putUint8InArray(setReqData->frequecyDomainPosition, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->frequecyDomainPosition);
     }
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SRSCONFIGDEDICATED_DURATION)
     {
       putUint8InArray(setReqData->duration, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->duration);
     }
  }

  /* Invoke SIH Set Processor */
  status = sihSetProcessor (sihTagInfo,  tlvArray);
  if(status EQ TRUE)
  {
    g_sihSrsConfigDedicatedSetReqProcessed = TRUE;
    g_sihSrsConfigDedicatedGetReqProcessed = FALSE;
    /* free the resources */
    free_srsConfigDedicatedTable();
  }
  return status;
}

 /** Initializes the srsConfigDedicated module */
void 
init_srsConfigDedicated(void)
{
  /* here we initialize all the tables we're planning on supporting */
  initialize_table_srsConfigDedicatedTable();
}

/** Initialize thesrsConfigDedicatedTable table by defining its contents and how it's structured */

void 
initialize_table_srsConfigDedicatedTable(void)
{
  const oid srsConfigDedicatedTable_oid[] = {1,3,6,1,4,1,9344,501,SRSCONFIGDEDICATED,1};
  const size_t srsConfigDedicatedTable_oid_len = OID_LENGTH(srsConfigDedicatedTable_oid);
  netsnmp_handler_registration    *reg;
  netsnmp_iterator_info           *iinfo;
  netsnmp_table_registration_info *table_info;

  DEBUGMSGTL(("srsConfigDedicated:init", "initializing table srsConfigDedicatedTable\n"));

  reg = netsnmp_create_handler_registration(
            "srsConfigDedicatedTable", srsConfigDedicatedTable_handler,
            srsConfigDedicatedTable_oid, srsConfigDedicatedTable_oid_len,
            HANDLER_CAN_RWRITE
            );

  table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
  netsnmp_table_helper_add_indexes(table_info,
                          ASN_UNSIGNED, /* index: srsBandwidth */
                          0);
  table_info->min_column = SRSCONFIGDEDICATED_SRSBANDWIDTH;
  table_info->max_column = SRSCONFIGDEDICATED_DURATION;

  iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
  iinfo->get_first_data_point = srsConfigDedicatedTable_get_first_data_point;
  iinfo->get_next_data_point  = srsConfigDedicatedTable_get_next_data_point;
  iinfo->table_reginfo        = table_info;

  netsnmp_register_table_iterator( reg, iinfo );

   /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH  
  /* create a new row in the (unsorted) table */                     
struct srsConfigDedicatedTable_entry *
srsConfigDedicatedTable_createEntry(                                        
                 unsigned int srsBandwidth,
                  )
{
      struct srsConfigDedicatedTable_entry *entry;                            
                                                                     
      entry = SNMP_MALLOC_TYPEDEF(struct srsConfigDedicatedTable_entry);      
      if (!entry)                                                    
          return NULL;                                               
                                                                     
      entry->srsBandwidth = srsBandwidth;                       
      entry->next = srsConfigDedicatedTable_head;                             
      srsConfigDedicatedTable_head = entry;                                   
      return entry;                                                  
}     

/* remove a row from the table */
void
srsConfigDedicatedTable_removeEntry( struct srsConfigDedicatedTable_entry *entry ) {
    struct srsConfigDedicatedTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = srsConfigDedicatedTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        srsConfigDedicatedTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
srsConfigDedicatedTable_get_first_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  TagInfo sihTagInfo;
  TBOOL status = FALSE;

  if(g_sihSrsConfigDedicatedGetReqProcessed EQ FALSE)
  {
    /* Set SIH Get Request Tag Info */
    sihTagInfo.tableId = SRSCONFIGDEDICATED;
    sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
    sihTagInfo.colId = CONFIG_COL_ID_NONE;

    status = sihGetProcessor (sihTagInfo);
    if(status EQ TRUE)
    {
      syslog(LOG_INFO,"Get Processor Succedded");
      if(srsConfigDedicatedTable_head)
      {
        g_srsConfigDedicatedFirstCall = 1;
        g_sihSrsConfigDedicatedGetReqProcessed = TRUE;
      }
    }
    else
    {
      syslog(LOG_INFO,"Get Processor Failed");
    }
  }

  *my_loop_context = srsConfigDedicatedTable_head;
  return srsConfigDedicatedTable_get_next_data_point(my_loop_context, my_data_context,
                                            put_index_data,  mydata );
}

netsnmp_variable_list *
srsConfigDedicatedTable_get_next_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  struct srsConfigDedicatedTable_entry *entry = (struct srsConfigDedicatedTable_entry *)*my_loop_context;
  netsnmp_variable_list *idx = put_index_data;

  if(entry) 
  {
    snmp_set_var_typed_integer( idx, ASN_UNSIGNED, entry->srsBandwidth );
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

/** handles requests for the srsConfigDedicatedTable table */
int
srsConfigDedicatedTable_handler(
    netsnmp_mib_handler            *handler,
    netsnmp_handler_registration   *reginfo,
    netsnmp_agent_request_info     *reqinfo,
    netsnmp_request_info           *requests)
{
  netsnmp_request_info       *request = NULL;
  netsnmp_table_request_info *table_info = NULL;
  struct srsConfigDedicatedTable_entry  *table_entry = NULL;
  int loopCount = 0;

  int ret;
  TagInfo sihTagInfo;
  TlvArray tlvArray;
  int rowIndex = 0, colId = 0;
  TBOOL status = FALSE;
  DEBUGMSGTL(("srsConfigDedicated:handler", "Processing request (%d)\n", reqinfo->mode));

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
             g_prevSrsConfigDedicatedReqCount = 0;
             g_sihSrsConfigDedicatedGetReqProcessed = FALSE;

    /* free the resources */
             free_srsConfigDedicatedTable();
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
              help return data for the columns of the srsConfigDedicatedTable table
              in question. */

           table_entry = (struct srsConfigDedicatedTable_entry *)
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
             case SRSCONFIGDEDICATED_SRSBANDWIDTH:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->srsBandwidth);
                  break;
             case SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                              table_entry->srsHoppingBandWidth);
                  break;
             case SRSCONFIGDEDICATED_FREQDOMAINPOSITION:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->frequecyDomainPosition);
                  break;
             case SRSCONFIGDEDICATED_DURATION:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->duration);
                  break;
             default:
                  netsnmp_set_request_error(reqinfo, request,
                                            SNMP_NOSUCHOBJECT);
                  break;
           }
         }
         if(g_srsConfigDedicatedFirstCall EQ 1)
       {
           g_srsConfigDedicatedFirstCall = 0;
           g_prevSrsConfigDedicatedReqCount = loopCount;
           // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
           // In the first call itself loopCount becomes one.
           // In General Case, the loopCount will be always > 1 because every table contains
           // index column to fetch the row and remaining columns contain table attributes.
           if(loopCount EQ 1)
           {
             // Setting it to false in case of SNMP-GET request for single parameter.
             g_sihSrsConfigDedicatedGetReqProcessed = FALSE;
             // Free all Resources.
             free_srsConfigDedicatedTable();
           }
         }
         else if(loopCount < 1 || g_prevSrsConfigDedicatedReqCount > 1)
         {
           // update g_prevReqCount with current loopCount for next call of Table_handler
           g_prevSrsConfigDedicatedReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
           //Setting it to false in case of SNMP-GET request for single parameter.
           g_sihSrsConfigDedicatedGetReqProcessed = FALSE;
           // Free all resources.
           free_srsConfigDedicatedTable();
         }
         break;

    /*
     * Write-support
     */
    case MODE_SET_RESERVE1:
       //  break;
#ifndef PWAV_SIH

        for (request=requests; request; request=request->next) {
            table_entry = (struct srsConfigDedicatedTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case SRSCONFIGDEDICATED_SRSBANDWIDTH: 
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SRSCONFIGDEDICATED_FREQDOMAINPOSITION :
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SRSCONFIGDEDICATED_FREQDOMAINPOSITION :
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
          g_sihSrsConfigDedicatedSetReqProcessed = FALSE;

        clearBytes(&sihTagInfo, sizeof(TagInfo));
        clearBytes(&tlvArray, sizeof(TlvArray));

          for(request=requests; request; request=request->next)
          {
            table_entry = (struct srsConfigDedicatedTable_entry *)
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
              case SRSCONFIGDEDICATED_SRSBANDWIDTH:
                   table_entry->srsBandwidth     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SRSCONFIGDEDICATED, rowIndex, SRSCONFIGDEDICATED_SRSBANDWIDTH);
                   tlvArray.numTlvs++;
                   break;
              case SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH:
                   table_entry->srsHoppingBandWidth     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SRSCONFIGDEDICATED, rowIndex, SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH);
                   tlvArray.numTlvs++;
                   break;
              case SRSCONFIGDEDICATED_FREQDOMAINPOSITION:
                   table_entry->frequecyDomainPosition     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SRSCONFIGDEDICATED, rowIndex, SRSCONFIGDEDICATED_FREQDOMAINPOSITION);
                   tlvArray.numTlvs++;
                   break;
              case SRSCONFIGDEDICATED_DURATION:
                   table_entry->duration     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SRSCONFIGDEDICATED, rowIndex, SRSCONFIGDEDICATED_DURATION);
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
          if((request == NULL) && (g_sihSrsConfigDedicatedSetReqProcessed EQ FALSE))
          {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = SRSCONFIGDEDICATED;

            sihTagInfo.rowId = rowIndex;

            status = sendSrsConfigDedicatedSnmpSetRequest(sihTagInfo, &tlvArray);
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
            table_entry = (struct srsConfigDedicatedTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);

            switch (table_info->colnum) {
            case SRSCONFIGDEDICATED_SRSBANDWIDTH:
                table_entry->srsBandwidth     = table_entry->old_srsBandwidth;
                table_entry->old_srsBandwidth = 0;
                break;
            case SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH:
                table_entry->srsHoppingBandWidth     = table_entry->old_srsHoppingBandWidth;
                table_entry->old_srsHoppingBandWidth = 0;
                break;
            case SRSCONFIGDEDICATED_FREQDOMAINPOSITION:
                table_entry->frequecyDomainPosition     = table_entry->old_frequecyDomainPosition;
                table_entry->old_frequecyDomainPosition = 0;
                break;
            case SRSCONFIGDEDICATED_DURATION:
                table_entry->duration     = table_entry->old_duration;
                table_entry->old_duration = 0;
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

