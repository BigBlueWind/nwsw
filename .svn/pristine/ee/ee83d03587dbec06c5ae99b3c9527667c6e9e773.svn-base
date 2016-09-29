#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "bs_modules.h"
#include "all_cfg.h"
#include "all_cmn.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "syslog.h"
#include "ueEventHistoryLogConfig.h"

/* function declarations */
Netsnmp_Node_Handler     ueEventHistoryLogConfigTable_handler;
Netsnmp_First_Data_Point ueEventHistoryLogConfigTable_get_first_data_point;
Netsnmp_Next_Data_Point  ueEventHistoryLogConfigTable_get_next_data_point;

/* Global Variables */
struct ueEventHistoryLogConfigTable_entry *ueEventHistoryLogConfigTable_head = NULL;
static TBOOL g_sihUeEventHistoryLogConfigGetReqProcessed = FALSE;
static TBOOL g_sihUeEventHistoryLogConfigSetReqProcessed = FALSE;
static UINT32 g_prevUeEventHistoryLogConfigReqCount = 0;
static UINT32 g_ueEventHistoryLogConfigFirstCall = 0;

/**
 * @details
 *          This is the routine which frees the ueEventHistoryLogConfig entry.
 */
void free_ueEventHistoryLogConfigTable(void)
{
  ueEventHistoryLogConfigTableEntry *current = ueEventHistoryLogConfigTable_head;

  if(current)
  {
    for (current = ueEventHistoryLogConfigTable_head->next; current != NULL; current = current->next)
    {
      SNMP_FREE( current );
    }

    SNMP_FREE(ueEventHistoryLogConfigTable_head);
    ueEventHistoryLogConfigTable_head = NULL;
  }
  return;
}

/**
 * @details
 *          This is the routine which handles get response from SIH Get Processor
 *          and Prepare the SNMP GET Response for EMS.
 * @param tlvArray containf pointer to TLV array.
 */

TBOOL sendUeEventHistoryLogConfigSnmpGetResponse (
  IO TlvArray *tlvArray
  )
{
  int totalNumRow, totalNumCol, i=0, colmn=0;
  ueEventHistoryLogConfigTableEntry *prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = UEEVENTHISTORYLOGCONFIG_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  for (i=1;  i <= totalNumRow;  i++)
  {
    ueEventHistoryLogConfigTableEntry *ptr = (ueEventHistoryLogConfigTableEntry *)malloc(sizeof(ueEventHistoryLogConfigTableEntry));

    clearBytes(ptr, sizeof(ueEventHistoryLogConfigTableEntry));

    /* Fill Data in TLV */
    for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
    {
      if(colmn EQ UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX)
         {
	  ptr->ueEventHistoryIndex = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
          continue;                                                    
         }                                                            
      if(colmn EQ UEEVENTHISTORYLOGCONFIG_CRNTI)
      {
        ptr->cRnti = getUint16FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP)
      {
        ptr->ueProcedureBitMap = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES)
      {
        ptr->ueEventMeasures = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
    } /* Ending of InnerLoop */

  ptr->next = NULL;

    if(prevPtr == NULL)
    {
      ueEventHistoryLogConfigTable_head = prevPtr = ptr;
    }
    prevPtr->next = ptr;
    prevPtr = ptr;
  } /*Ending of OuterLoop */
  prevPtr->next = NULL;

  if(ueEventHistoryLogConfigTable_head)
    return TRUE;
  else
    return FALSE;
}
/**
 * @details
 *          This routine sends ueEventHistoryLogConfig set request to BSM, it uses SIH Set
 *          Processor to create and send the message to BSM.
 * @param sihTagInfo is used to have the tag information for set request.
 */
TBOOL sendUeEventHistoryLogConfigSnmpSetRequest (
      IO TagInfo sihTagInfo,
      IO TlvArray *tlvArray
      )
{
  ueEventHistoryLogConfigTableEntry *setReqData = NULL;
  TBOOL status = FALSE;
  int colmn = 0;

  setReqData = ueEventHistoryLogConfigTable_head;

  /* Fill Data in TLV */
   for(colmn = 0; colmn < tlvArray->numTlvs; colmn++)
   {
       if(tlvArray->tlvInfo[colmn].tag.colId EQ UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX)
       {
           putUint32InArray(setReqData->ueEventHistoryIndex, tlvArray->tlvInfo[colmn].data);
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->ueEventHistoryIndex);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ UEEVENTHISTORYLOGCONFIG_CRNTI)
       {
           putUint16InArray(setReqData->cRnti, tlvArray->tlvInfo[colmn].data);
          tlvArray->tlvInfo[colmn].len = sizeof(setReqData->cRnti);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP)
       {
           putUint8InArray(setReqData->ueProcedureBitMap, tlvArray->tlvInfo[colmn].data);
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->ueProcedureBitMap);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES)
       {
           putUint32InArray(setReqData->ueEventMeasures, tlvArray->tlvInfo[colmn].data);
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->ueEventMeasures);
       }
   }

  /* Invoke SIH Set Processor */
  status = sihSetProcessor (sihTagInfo, tlvArray);
  if(status EQ TRUE)
  {
    g_sihUeEventHistoryLogConfigSetReqProcessed = TRUE;
    g_sihUeEventHistoryLogConfigGetReqProcessed = FALSE;
    /* free the resources */
    free_ueEventHistoryLogConfigTable();
  }
  return status;
}

 /** Initializes the ueEventHistoryLogConfig module */
void 
init_ueEventHistoryLogConfig(void)
{
  /* here we initialize all the tables we're planning on supporting */
  initialize_table_ueEventHistoryLogConfigTable();
}

/** Initialize theueEventHistoryLogConfigTable table by defining its contents and how it's structured */

void 
initialize_table_ueEventHistoryLogConfigTable(void)
{
  const oid ueEventHistoryLogConfigTable_oid[] = {1,3,6,1,4,1,9344,501,UEEVENTHISTORYLOGCONFIG,1};
  const size_t ueEventHistoryLogConfigTable_oid_len = OID_LENGTH(ueEventHistoryLogConfigTable_oid);
  netsnmp_handler_registration    *reg;
  netsnmp_iterator_info           *iinfo;
  netsnmp_table_registration_info *table_info;

  DEBUGMSGTL(("ueEventHistoryLogConfig:init", "initializing table ueEventHistoryLogConfigTable\n"));

  reg = netsnmp_create_handler_registration(
            "ueEventHistoryLogConfigTable", ueEventHistoryLogConfigTable_handler,
            ueEventHistoryLogConfigTable_oid, ueEventHistoryLogConfigTable_oid_len,
            HANDLER_CAN_RWRITE
            );

  table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
  netsnmp_table_helper_add_indexes(table_info,
                          ASN_UNSIGNED, /* index: ueEventHistoryIndex */
                          0);
  table_info->min_column = UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX;
  table_info->max_column = UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES;

  iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
  iinfo->get_first_data_point = ueEventHistoryLogConfigTable_get_first_data_point;
  iinfo->get_next_data_point  = ueEventHistoryLogConfigTable_get_next_data_point;
  iinfo->table_reginfo        = table_info;

  netsnmp_register_table_iterator( reg, iinfo );

   /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH  
  /* create a new row in the (unsorted) table */                     
struct ueEventHistoryLogConfigTable_entry *
ueEventHistoryLogConfigTable_createEntry(                                        
                 unsigned int ueEventHistoryIndex,
                  )
{
      struct ueEventHistoryLogConfigTable_entry *entry;                            
                                                                     
      entry = SNMP_MALLOC_TYPEDEF(struct ueEventHistoryLogConfigTable_entry);      
      if (!entry)                                                    
          return NULL;                                               
                                                                     
      entry->ueEventHistoryIndex = ueEventHistoryIndex;                       
      entry->next = ueEventHistoryLogConfigTable_head;                             
      ueEventHistoryLogConfigTable_head = entry;                                   
      return entry;                                                  
}     

/* remove a row from the table */
void
ueEventHistoryLogConfigTable_removeEntry( struct ueEventHistoryLogConfigTable_entry *entry ) {
    struct ueEventHistoryLogConfigTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = ueEventHistoryLogConfigTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        ueEventHistoryLogConfigTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
ueEventHistoryLogConfigTable_get_first_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  TagInfo sihTagInfo;
  TBOOL status = FALSE;

  if(g_sihUeEventHistoryLogConfigGetReqProcessed EQ FALSE)
  {
    /* Set SIH Get Request Tag Info */
    sihTagInfo.tableId = UEEVENTHISTORYLOGCONFIG;
    sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
    sihTagInfo.colId = CONFIG_COL_ID_NONE;

    status = sihGetProcessor (sihTagInfo);
    if(status EQ TRUE)
    {
      logInfo("%s","Get Processor Succedded");
      if(ueEventHistoryLogConfigTable_head)
      {
        g_ueEventHistoryLogConfigFirstCall = 1;
        g_sihUeEventHistoryLogConfigGetReqProcessed = TRUE;
      }
    }
    else
    {
      logError("%s","Get Processor Failed");
    }
  }

  *my_loop_context = ueEventHistoryLogConfigTable_head;
  return ueEventHistoryLogConfigTable_get_next_data_point(my_loop_context, my_data_context,
                                            put_index_data,  mydata );
}

netsnmp_variable_list *
ueEventHistoryLogConfigTable_get_next_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  struct ueEventHistoryLogConfigTable_entry *entry = (struct ueEventHistoryLogConfigTable_entry *)*my_loop_context;
  netsnmp_variable_list *idx = put_index_data;

  if(entry) 
  {
    snmp_set_var_typed_integer( idx, ASN_UNSIGNED, entry->ueEventHistoryIndex );
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

/** handles requests for the ueEventHistoryLogConfigTable table */
int
ueEventHistoryLogConfigTable_handler(
    netsnmp_mib_handler            *handler,
    netsnmp_handler_registration   *reginfo,
    netsnmp_agent_request_info     *reqinfo,
    netsnmp_request_info           *requests)
{
  netsnmp_request_info       *request = NULL;
  netsnmp_table_request_info *table_info = NULL;
  struct ueEventHistoryLogConfigTable_entry  *table_entry = NULL;
  int loopCount = 0;

  int ret;
  TagInfo sihTagInfo;
  TlvArray tlvArray;
  int rowIndex = 0, colId = 0;
  TBOOL status = FALSE;
  DEBUGMSGTL(("ueEventHistoryLogConfig:handler", "Processing request (%d)\n", reqinfo->mode));

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
             g_prevUeEventHistoryLogConfigReqCount = 0;
             g_sihUeEventHistoryLogConfigGetReqProcessed = FALSE;

    /* free the resources */
             free_ueEventHistoryLogConfigTable();
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
              help return data for the columns of the ueEventHistoryLogConfigTable table
              in question. */

           table_entry = (struct ueEventHistoryLogConfigTable_entry *)
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
             case UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->ueEventHistoryIndex);
                  break;
             case UEEVENTHISTORYLOGCONFIG_CRNTI:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->cRnti);
                  break;
             case UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->ueProcedureBitMap);
                  break;
             case UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                              table_entry->ueEventMeasures);
                  break;
             default:
                  netsnmp_set_request_error(reqinfo, request,
                                            SNMP_NOSUCHOBJECT);
                  break;
           }
         }
         if(g_ueEventHistoryLogConfigFirstCall EQ 1)
       {
           g_ueEventHistoryLogConfigFirstCall = 0;
           g_prevUeEventHistoryLogConfigReqCount = loopCount;
           // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
           // In the first call itself loopCount becomes one.
           // In General Case, the loopCount will be always > 1 because every table contains
           // index column to fetch the row and remaining columns contain table attributes.
           if(loopCount EQ 1)
           {
             // Setting it to false in case of SNMP-GET request for single parameter.
             g_sihUeEventHistoryLogConfigGetReqProcessed = FALSE;
             // Free all Resources.
             free_ueEventHistoryLogConfigTable();
           }
         }
         else if(loopCount < 1 || g_prevUeEventHistoryLogConfigReqCount > 1)
         {
           // update g_prevReqCount with current loopCount for next call of Table_handler
           g_prevUeEventHistoryLogConfigReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
           //Setting it to false in case of SNMP-GET request for single parameter.
           g_sihUeEventHistoryLogConfigGetReqProcessed = FALSE;
           // Free all resources.
           free_ueEventHistoryLogConfigTable();
         }
         break;

    /*
     * Write-support
     */
    case MODE_SET_RESERVE1:
       //  break;
#ifndef PWAV_SIH

        for (request=requests; request; request=request->next) {
            table_entry = (struct ueEventHistoryLogConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX: 
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case UEEVENTHISTORYLOGCONFIG_CRNTI:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP :
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES :
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
          g_sihUeEventHistoryLogConfigSetReqProcessed = FALSE;

          clearBytes(&sihTagInfo, sizeof(TagInfo));
          clearBytes(&tlvArray, sizeof(TlvArray));

          for(request=requests; request; request=request->next)
          {
            table_entry = (struct ueEventHistoryLogConfigTable_entry *)
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
              case UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX:
                   table_entry->ueEventHistoryIndex     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, UEEVENTHISTORYLOGCONFIG, rowIndex, UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX);
                   tlvArray.numTlvs++;
                   break;
              case UEEVENTHISTORYLOGCONFIG_CRNTI:
                   table_entry->cRnti     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, UEEVENTHISTORYLOGCONFIG, rowIndex, UEEVENTHISTORYLOGCONFIG_CRNTI);
                   tlvArray.numTlvs++;
                   break;
              case UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP:
                   table_entry->ueProcedureBitMap     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, UEEVENTHISTORYLOGCONFIG, rowIndex, UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP);
                   tlvArray.numTlvs++;
                   break;
              case UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES:
                   table_entry->ueEventMeasures     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, UEEVENTHISTORYLOGCONFIG, rowIndex, UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES);
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
          if((request == NULL) && (g_sihUeEventHistoryLogConfigSetReqProcessed EQ FALSE))
          {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = UEEVENTHISTORYLOGCONFIG;

            sihTagInfo.rowId = rowIndex;

            status = sendUeEventHistoryLogConfigSnmpSetRequest(sihTagInfo, &tlvArray);
            if(status EQ TRUE)
            {
              logInfo("%s","SET Succedded");
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
            table_entry = (struct ueEventHistoryLogConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);

            switch (table_info->colnum) {
            case UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX:
                table_entry->ueEventHistoryIndex     = table_entry->old_ueEventHistoryIndex;
                table_entry->old_ueEventHistoryIndex = 0;
                break;
            case UEEVENTHISTORYLOGCONFIG_CRNTI:
                table_entry->cRnti     = table_entry->old_cRnti;
                table_entry->old_cRnti = 0;
                break;
            case UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP:
                table_entry->ueProcedureBitMap     = table_entry->old_ueProcedureBitMap;
                table_entry->old_ueProcedureBitMap = 0;
                break;
            case UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES:
                table_entry->ueEventMeasures     = table_entry->old_ueEventMeasures;
                table_entry->old_ueEventMeasures = 0;
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

