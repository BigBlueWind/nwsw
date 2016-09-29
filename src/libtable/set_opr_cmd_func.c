/**
 * @file    set_opr_cmd_func.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains SET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 2/8/2013 1:08:26 PM
 */

 
#include "set_oam_func.h"

/*******************************************************************/
/***************    OPERATOR COMMANDS TABLE SET FUNCTIONS   ********/
/*******************************************************************/

/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enbAdminstructure.
 * */
void tlv2StructEnbAdminTable(
  IO TlvArray           *tlvArray,
  IO EnbAdmin           *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_ENBADMIN_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_ENBADMIN_ENBCOMMAND:
        config->eNbCommand  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills fileUploadstructure.
 * */
void tlv2StructFileUploadTable(
  IO TlvArray           *tlvArray,
  IO FileUpload         *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_FILEUPLOAD_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEUPLOAD_FILEUPLOADCOMMAND:
        config->fileUploadCommand  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEUPLOAD_FILENAME:
        snprintf( config->fileName, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEUPLOAD_LOGINID:
        snprintf( config->loginId, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEUPLOAD_PASSWORD:
        snprintf( config->password, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEUPLOAD_DESTINATIONIP:
        snprintf( config->destinationIp, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEUPLOAD_PROTOCOLUSED:
        config->protocolUsed  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEUPLOAD_DESTINATIONPORT:
        config->destinationPort  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEUPLOAD_MAXUPLOADSPEED:
        config->maxUploadSpeed  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEUPLOAD_REMOTEFILENAME:
        snprintf( config->remoteFileName, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEUPLOAD_DELETEAFTERUPLOAD:
        config->deleteAfterUpload  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills fileDownloadstructure.
 * */
void tlv2StructFileDownloadTable(
  IO TlvArray           *tlvArray,
  IO FileDownload       *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_FILEDOWNLOAD_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEDOWNLOAD_FILEDOWNLOADCOMMAND:
        config->fileDownloadCommand  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEDOWNLOAD_FILENAME:
        snprintf( config->fileName, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEDOWNLOAD_LOGINID:
        snprintf( config->loginId, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEDOWNLOAD_PASSWORD:
        snprintf( config->password, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEDOWNLOAD_SOURCEIP:
        snprintf( config->sourceIp, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_FILEDOWNLOAD_PROTOCOLUSED:
        config->protocolUsed  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEDOWNLOAD_SOURCEPORT:
        config->sourcePort  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEDOWNLOAD_MAXDOWNLOADSPEED:
        config->maxDownloadSpeed  =  getUint32FromArray(tlv->data);
        break;
      case COL_FILEDOWNLOAD_SCHEDULEDATEANDTIME:
        snprintf( config->scheduleDateAndTime, (tlv->len+1), "%s", tlv->data ); 
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills swActivationstructure.
 * */
void tlv2StructSwActivationTable(
  IO TlvArray           *tlvArray,
  IO SwActivation       *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SWACTIVATION_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_SWACTIVATION_SWTYPE:
        config->swType  =  getUint32FromArray(tlv->data);
        break;
      case COL_SWACTIVATION_VERSION:
        snprintf( config->version, (tlv->len+1), "%s", tlv->data ); 
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills swInstallstructure.
 * */
void tlv2StructSwInstallTable(
  IO TlvArray           *tlvArray,
  IO SwInstall          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SWINSTALL_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_SWINSTALL_INSTALLATIONTYPE:
        config->installationType  =  getUint32FromArray(tlv->data);
        break;
      case COL_SWINSTALL_FILENAME:
        snprintf( config->filename, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_SWINSTALL_INSTALLER:
        snprintf( config->installer, (tlv->len+1), "%s", tlv->data ); 
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills traceControlstructure.
 * */
void tlv2StructTraceControlTable(
  IO TlvArray           *tlvArray,
  IO TraceControl       *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_TRACECONTROL_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_TRACECONTROL_TRACECOMMAND:
        config->traceCommand  =  getUint32FromArray(tlv->data);
        break;
      case COL_TRACECONTROL_TRACEDURATION:
        config->traceDuration  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills cellAdminstructure.
 * */
void tlv2StructCellAdminTable(
  IO TlvArray           *tlvArray,
  IO CellAdmin          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_CELLADMIN_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLADMIN_CELLID:
        config->cellId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLADMIN_CELLCOMMAND:
        config->cellCommand  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills counterControlstructure.
 * */
void tlv2StructCounterControlTable(
  IO TlvArray           *tlvArray,
  IO CounterControl     *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_COUNTERCONTROL_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_COUNTERCONTROL_PROFILEID:
        config->profileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_COUNTERCONTROL_COUNTERCOMMAND:
        config->counterCommand  =  getUint32FromArray(tlv->data);
        break;
      case COL_COUNTERCONTROL_MEASUREMENTDURATION:
        config->measurementDuration  =  getUint32FromArray(tlv->data);
        break;
      case COL_COUNTERCONTROL_REPORTINGDURATION:
        config->reportingDuration  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}

