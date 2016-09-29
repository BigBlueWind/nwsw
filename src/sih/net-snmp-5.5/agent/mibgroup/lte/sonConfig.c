#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "bs_modules.h"
#include "all_cfg.h"
#include "all_cmn.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "syslog.h"
#include "sonConfig.h"

/* function declarations */
Netsnmp_Node_Handler     sonConfigTable_handler;
Netsnmp_First_Data_Point sonConfigTable_get_first_data_point;
Netsnmp_Next_Data_Point  sonConfigTable_get_next_data_point;

/* Global Variables */
struct sonConfigTable_entry *sonConfigTable_head = NULL;
static TBOOL g_sihSonConfigGetReqProcessed = FALSE;
static TBOOL g_sihSonConfigSetReqProcessed = FALSE;
static UINT32 g_prevSonConfigReqCount = 0;
static UINT32 g_sonConfigFirstCall = 0;

/**
 * @details
 *          This is the routine which frees the sonConfig entry.
 */
void free_sonConfigTable(void)
{
  sonConfigTableEntry *current = sonConfigTable_head;

  if(current)
  {
    for (current = sonConfigTable_head->next; current != NULL; current = current->next)
    {
      SNMP_FREE( current );
    }

    SNMP_FREE(sonConfigTable_head);
    sonConfigTable_head = NULL;
  }
  return;
}

/**
 * @details
 *          This is the routine which handles get response from SIH Get Processor
 *          and Prepare the SNMP GET Response for EMS.
 * @param tlvArray containf pointer to TLV array.
 */

TBOOL sendSonConfigSnmpGetResponse (
  IO TlvArray *tlvArray
  )
{
  int totalNumRow, totalNumCol, i=0, colmn=0;
  sonConfigTableEntry *prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = SONCONFIG_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  for (i=1;  i <= totalNumRow;  i++)
  {
    sonConfigTableEntry *ptr = (sonConfigTableEntry *)malloc(sizeof(sonConfigTableEntry));

    clearBytes(ptr, sizeof(sonConfigTableEntry));

    /* Fill Data in TLV */
    for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
    {
      if(colmn EQ SONCONFIG_SONPROFILEID)
         {
	  ptr->sonProfileId = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
          continue;                                                    
         }                                                            
      if(colmn EQ SONCONFIG_INTRAFREQHOTHRESHOLD)
      {
        ptr->intraFreqHoThreshold = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ SONCONFIG_ICICTHRESHOLD)
      {
        ptr->icicThreshold = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
      if(colmn EQ SONCONFIG_UELOADCHANGETHRESHOLD)
      {
        ptr->ueLoadChangeThreshold = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
        continue;
      }
    } /* Ending of InnerLoop */

  ptr->next = NULL;

    if(prevPtr == NULL)
    {
      sonConfigTable_head = prevPtr = ptr;
    }
    prevPtr->next = ptr;
    prevPtr = ptr;
  } /*Ending of OuterLoop */
  prevPtr->next = NULL;

  if(sonConfigTable_head)
    return TRUE;
  else
    return FALSE;
}
/**
 * @details
 *          This routine sends sonConfig set request to BSM, it uses SIH Set
 *          Processor to create and send the message to BSM.
 * @param sihTagInfo is used to have the tag information for set request.
 */
TBOOL sendSonConfigSnmpSetRequest (
      IO TagInfo sihTagInfo,
      IO TlvArray *tlvArray
      )
{
  sonConfigTableEntry *setReqData = NULL;
  TBOOL status = FALSE;
  int colmn = 0;

  setReqData = sonConfigTable_head;

  /* Fill Data in TLV */
   for(colmn = 0; colmn < tlvArray->numTlvs; colmn++)
   {
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SONCONFIG_SONPROFILEID)
     {
       putUint32InArray(setReqData->sonProfileId, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->sonProfileId);
     }
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SONCONFIG_INTRAFREQHOTHRESHOLD)
     {
       putUint32InArray(setReqData->intraFreqHoThreshold, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->intraFreqHoThreshold);
     }
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SONCONFIG_ICICTHRESHOLD)
     {
       putUint32InArray(setReqData->icicThreshold, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->icicThreshold);
     }
     if(tlvArray->tlvInfo[colmn].tag.colId EQ SONCONFIG_UELOADCHANGETHRESHOLD)
     {
       putUint32InArray(setReqData->ueLoadChangeThreshold, tlvArray->tlvInfo[colmn].data);
       tlvArray->tlvInfo[colmn].len = sizeof(setReqData->ueLoadChangeThreshold);
     }
  }

  /* Invoke SIH Set Processor */
  status = sihSetProcessor (sihTagInfo,  tlvArray);
  if(status EQ TRUE)
  {
    g_sihSonConfigSetReqProcessed = TRUE;
    g_sihSonConfigGetReqProcessed = FALSE;
    /* free the resources */
    free_sonConfigTable();
  }
  return status;
}

 /** Initializes the sonConfig module */
void 
init_sonConfig(void)
{
  /* here we initialize all the tables we're planning on supporting */
  initialize_table_sonConfigTable();
}

/** Initialize thesonConfigTable table by defining its contents and how it's structured */

void 
initialize_table_sonConfigTable(void)
{
  const oid sonConfigTable_oid[] = {1,3,6,1,4,1,9344,501,SONCONFIG,1};
  const size_t sonConfigTable_oid_len = OID_LENGTH(sonConfigTable_oid);
  netsnmp_handler_registration    *reg;
  netsnmp_iterator_info           *iinfo;
  netsnmp_table_registration_info *table_info;

  DEBUGMSGTL(("sonConfig:init", "initializing table sonConfigTable\n"));

  reg = netsnmp_create_handler_registration(
            "sonConfigTable", sonConfigTable_handler,
            sonConfigTable_oid, sonConfigTable_oid_len,
            HANDLER_CAN_RWRITE
            );

  table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
  netsnmp_table_helper_add_indexes(table_info,
                          ASN_UNSIGNED, /* index: sonProfileId */
                          0);
  table_info->min_column = SONCONFIG_SONPROFILEID;
  table_info->max_column = SONCONFIG_UELOADCHANGETHRESHOLD;

  iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
  iinfo->get_first_data_point = sonConfigTable_get_first_data_point;
  iinfo->get_next_data_point  = sonConfigTable_get_next_data_point;
  iinfo->table_reginfo        = table_info;

  netsnmp_register_table_iterator( reg, iinfo );

   /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH  
  /* create a new row in the (unsorted) table */                     
struct sonConfigTable_entry *
sonConfigTable_createEntry(                                        
                 unsigned int sonProfileId,
                  )
{
      struct sonConfigTable_entry *entry;                            
                                                                     
      entry = SNMP_MALLOC_TYPEDEF(struct sonConfigTable_entry);      
      if (!entry)                                                    
          return NULL;                                               
                                                                     
      entry->sonProfileId = sonProfileId;                       
      entry->next = sonConfigTable_head;                             
      sonConfigTable_head = entry;                                   
      return entry;                                                  
}     

/* remove a row from the table */
void
sonConfigTable_removeEntry( struct sonConfigTable_entry *entry ) {
    struct sonConfigTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = sonConfigTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        sonConfigTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
sonConfigTable_get_first_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  TagInfo sihTagInfo;
  TBOOL status = FALSE;

  if(g_sihSonConfigGetReqProcessed EQ FALSE)
  {
    /* Set SIH Get Request Tag Info */
    sihTagInfo.tableId = SONCONFIG;
    sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
    sihTagInfo.colId = CONFIG_COL_ID_NONE;

    status = sihGetProcessor (sihTagInfo);
    if(status EQ TRUE)
    {
      logInfo("%s","Get Processor Succedded");
      if(sonConfigTable_head)
      {
        g_sonConfigFirstCall = 1;
        g_sihSonConfigGetReqProcessed = TRUE;
      }
    }
    else
    {
      logError("%s","Get Processor Failed");
    }
  }

  *my_loop_context = sonConfigTable_head;
  return sonConfigTable_get_next_data_point(my_loop_context, my_data_context,
                                            put_index_data,  mydata );
}

netsnmp_variable_list *
sonConfigTable_get_next_data_point(void **my_loop_context,
                         void **my_data_context,
                         netsnmp_variable_list *put_index_data,
                         netsnmp_iterator_info *mydata)
{
  struct sonConfigTable_entry *entry = (struct sonConfigTable_entry *)*my_loop_context;
  netsnmp_variable_list *idx = put_index_data;

  if(entry) 
  {
    snmp_set_var_typed_integer( idx, ASN_UNSIGNED, entry->sonProfileId );
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

/** handles requests for the sonConfigTable table */
int
sonConfigTable_handler(
    netsnmp_mib_handler            *handler,
    netsnmp_handler_registration   *reginfo,
    netsnmp_agent_request_info     *reqinfo,
    netsnmp_request_info           *requests)
{
  netsnmp_request_info       *request = NULL;
  netsnmp_table_request_info *table_info = NULL;
  struct sonConfigTable_entry  *table_entry = NULL;
  int loopCount = 0;

  int ret;
  TagInfo sihTagInfo;
  TlvArray tlvArray;
  int rowIndex = 0, colId = 0;
  TBOOL status = FALSE;
  DEBUGMSGTL(("sonConfig:handler", "Processing request (%d)\n", reqinfo->mode));

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
             g_prevSonConfigReqCount = 0;
             g_sihSonConfigGetReqProcessed = FALSE;

    /* free the resources */
             free_sonConfigTable();
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
              help return data for the columns of the sonConfigTable table
              in question. */

           table_entry = (struct sonConfigTable_entry *)
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
             case SONCONFIG_SONPROFILEID:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->sonProfileId);
                  break;
             case SONCONFIG_INTRAFREQHOTHRESHOLD:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->intraFreqHoThreshold);
                  break;
             case SONCONFIG_ICICTHRESHOLD:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->icicThreshold);
                  break;
             case SONCONFIG_UELOADCHANGETHRESHOLD:
                  if(!table_entry )
                  {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                  }
                  snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                              table_entry->ueLoadChangeThreshold);
                  break;
             default:
                  netsnmp_set_request_error(reqinfo, request,
                                            SNMP_NOSUCHOBJECT);
                  break;
           }
         }
         if(g_sonConfigFirstCall EQ 1)
       {
           g_sonConfigFirstCall = 0;
           g_prevSonConfigReqCount = loopCount;
           // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
           // In the first call itself loopCount becomes one.
           // In General Case, the loopCount will be always > 1 because every table contains
           // index column to fetch the row and remaining columns contain table attributes.
           if(loopCount EQ 1)
           {
             // Setting it to false in case of SNMP-GET request for single parameter.
             g_sihSonConfigGetReqProcessed = FALSE;
             // Free all Resources.
             free_sonConfigTable();
           }
         }
         else if(loopCount < 1 || g_prevSonConfigReqCount > 1)
         {
           // update g_prevReqCount with current loopCount for next call of Table_handler
           g_prevSonConfigReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
           //Setting it to false in case of SNMP-GET request for single parameter.
           g_sihSonConfigGetReqProcessed = FALSE;
           // Free all resources.
           free_sonConfigTable();
         }
         break;

    /*
     * Write-support
     */
    case MODE_SET_RESERVE1:
       //  break;
#ifndef PWAV_SIH

        for (request=requests; request; request=request->next) {
            table_entry = (struct sonConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case SONCONFIG_SONPROFILEID: 
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SONCONFIG_INTRAFREQHOTHRESHOLD:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SONCONFIG_ICICTHRESHOLD :
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case SONCONFIG_ICICTHRESHOLD :
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
          g_sihSonConfigSetReqProcessed = FALSE;

        clearBytes(&sihTagInfo, sizeof(TagInfo));
        clearBytes(&tlvArray, sizeof(TlvArray));

          for(request=requests; request; request=request->next)
          {
            table_entry = (struct sonConfigTable_entry *)
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
              case SONCONFIG_SONPROFILEID:
                   table_entry->sonProfileId     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SONCONFIG, rowIndex, SONCONFIG_SONPROFILEID);
                   tlvArray.numTlvs++;
              break;
              case SONCONFIG_INTRAFREQHOTHRESHOLD:
                   table_entry->intraFreqHoThreshold     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SONCONFIG, rowIndex, SONCONFIG_INTRAFREQHOTHRESHOLD);
                   tlvArray.numTlvs++;
              break;
              case SONCONFIG_ICICTHRESHOLD:
                   table_entry->icicThreshold     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SONCONFIG, rowIndex, SONCONFIG_ICICTHRESHOLD);
                   tlvArray.numTlvs++;
              break;
              case SONCONFIG_UELOADCHANGETHRESHOLD:
                   table_entry->ueLoadChangeThreshold     = *request->requestvb->val.integer;
                   fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, SONCONFIG, rowIndex, SONCONFIG_UELOADCHANGETHRESHOLD);
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
          if((request == NULL) && (g_sihSonConfigSetReqProcessed EQ FALSE))
          {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = SONCONFIG;

            sihTagInfo.rowId = rowIndex;

            status = sendSonConfigSnmpSetRequest(sihTagInfo, &tlvArray);
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
            table_entry = (struct sonConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);

            switch (table_info->colnum) {
            case SONCONFIG_SONPROFILEID:
                table_entry->sonProfileId     = table_entry->old_sonProfileId;
                table_entry->old_sonProfileId = 0;
                break;
            case SONCONFIG_INTRAFREQHOTHRESHOLD:
                table_entry->intraFreqHoThreshold     = table_entry->old_intraFreqHoThreshold;
                table_entry->old_intraFreqHoThreshold = 0;
                break;
            case SONCONFIG_ICICTHRESHOLD:
                table_entry->icicThreshold     = table_entry->old_icicThreshold;
                table_entry->old_icicThreshold = 0;
                break;
            case SONCONFIG_UELOADCHANGETHRESHOLD:
                table_entry->ueLoadChangeThreshold     = table_entry->old_ueLoadChangeThreshold;
                table_entry->old_ueLoadChangeThreshold = 0;
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

