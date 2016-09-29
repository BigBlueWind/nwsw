/**
 * @file    get_opr_cmd_func.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains GET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 2/8/2013 1:08:25 PM
 */

 
#include "get_oam_func.h"

/*******************************************************************/
/***************    OPERATOR COMMANDS TABLE GET FUNCTIONS   ********/
/*******************************************************************/

/* GetData OprCommands */
EnbAdmin*                 getDataEnbAdminTable( IN RowId rowId )
{
  return   &(g_EnbAdmin[rowId]);
}
 
FileUpload*               getDataFileUploadTable( IN RowId rowId )
{
  return   &(g_FileUpload[rowId]);
}
 
FileDownload*             getDataFileDownloadTable( IN RowId rowId )
{
  return   &(g_FileDownload[rowId]);
}
 
SwActivation*             getDataSwActivationTable( IN RowId rowId )
{
  return   &(g_SwActivation[rowId]);
}
 
SwInstall*                getDataSwInstallTable( IN RowId rowId )
{
  return   &(g_SwInstall[rowId]);
}
 
TraceControl*             getDataTraceControlTable( IN RowId rowId )
{
  return   &(g_TraceControl[rowId]);
}
 
CellAdmin*                getDataCellAdminTable( IN RowId rowId )
{
  return   &(g_CellAdmin[rowId]);
}
 
CounterControl*           getDataCounterControlTable( IN RowId rowId )
{
  return   &(g_CounterControl[rowId]);
}
 
/* GetRow OprCommands */
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowEnbAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_ENBADMIN_TABLE; colId++)
  {
     getElementEnbAdminTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUpload the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowFileUploadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_FILEUPLOAD_TABLE; colId++)
  {
     getElementFileUploadTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileDownload the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowFileDownloadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_FILEDOWNLOAD_TABLE; colId++)
  {
     getElementFileDownloadTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swActivation the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowSwActivationTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_SWACTIVATION_TABLE; colId++)
  {
     getElementSwActivationTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swInstall the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowSwInstallTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_SWINSTALL_TABLE; colId++)
  {
     getElementSwInstallTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowTraceControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_TRACECONTROL_TABLE; colId++)
  {
     getElementTraceControlTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill cellAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowCellAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_CELLADMIN_TABLE; colId++)
  {
     getElementCellAdminTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill counterControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void getRowCounterControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IO TlvArray      *tlvArray
  ) 
{
  ColId   colId=0;
  for (colId=FIRST_COL; colId <= MAX_COLS_IN_COUNTERCONTROL_TABLE; colId++)
  {
     getElementCounterControlTable( tableId, rowId-1, colId, tlvArray);
  }
}
 
/* GetElement OprCommands */
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementEnbAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  EnbAdmin  *config = getData(rowId);

  switch(colId)
  {
    case COL_ENBADMIN_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_ENBADMIN_ENBCOMMAND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->eNbCommand, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUpload the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementFileUploadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  FileUpload  *config = getData(rowId);

  switch(colId)
  {
    case COL_FILEUPLOAD_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_FILEUPLOAD_FILEUPLOADCOMMAND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fileUploadCommand, tlvArray );
      break;
    case COL_FILEUPLOAD_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->fileName, strlen(config->fileName), tlvArray );
      break;
    case COL_FILEUPLOAD_LOGINID:
      fillTagLenString( tableId, (rowId+1), colId, config->loginId, strlen(config->loginId), tlvArray );
      break;
    case COL_FILEUPLOAD_PASSWORD:
      fillTagLenString( tableId, (rowId+1), colId, config->password, strlen(config->password), tlvArray );
      break;
    case COL_FILEUPLOAD_DESTINATIONIP:
      fillTagLenString( tableId, (rowId+1), colId, config->destinationIp, strlen(config->destinationIp), tlvArray );
      break;
    case COL_FILEUPLOAD_PROTOCOLUSED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->protocolUsed, tlvArray );
      break;
    case COL_FILEUPLOAD_DESTINATIONPORT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->destinationPort, tlvArray );
      break;
    case COL_FILEUPLOAD_MAXUPLOADSPEED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->maxUploadSpeed, tlvArray );
      break;
    case COL_FILEUPLOAD_REMOTEFILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->remoteFileName, strlen(config->remoteFileName), tlvArray );
      break;
    case COL_FILEUPLOAD_DELETEAFTERUPLOAD:
      fillTagLenUint8( tableId, (rowId+1), colId, config->deleteAfterUpload, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileDownload the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementFileDownloadTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  FileDownload  *config = getData(rowId);

  switch(colId)
  {
    case COL_FILEDOWNLOAD_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_FILEDOWNLOAD_FILEDOWNLOADCOMMAND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->fileDownloadCommand, tlvArray );
      break;
    case COL_FILEDOWNLOAD_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->fileName, strlen(config->fileName), tlvArray );
      break;
    case COL_FILEDOWNLOAD_LOGINID:
      fillTagLenString( tableId, (rowId+1), colId, config->loginId, strlen(config->loginId), tlvArray );
      break;
    case COL_FILEDOWNLOAD_PASSWORD:
      fillTagLenString( tableId, (rowId+1), colId, config->password, strlen(config->password), tlvArray );
      break;
    case COL_FILEDOWNLOAD_SOURCEIP:
      fillTagLenString( tableId, (rowId+1), colId, config->sourceIp, strlen(config->sourceIp), tlvArray );
      break;
    case COL_FILEDOWNLOAD_PROTOCOLUSED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->protocolUsed, tlvArray );
      break;
    case COL_FILEDOWNLOAD_SOURCEPORT:
      fillTagLenUint32( tableId, (rowId+1), colId, config->sourcePort, tlvArray );
      break;
    case COL_FILEDOWNLOAD_MAXDOWNLOADSPEED:
      fillTagLenUint32( tableId, (rowId+1), colId, config->maxDownloadSpeed, tlvArray );
      break;
    case COL_FILEDOWNLOAD_SCHEDULEDATEANDTIME:
      fillTagLenString( tableId, (rowId+1), colId, config->scheduleDateAndTime, strlen(config->scheduleDateAndTime), tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swActivation the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementSwActivationTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  SwActivation  *config = getData(rowId);

  switch(colId)
  {
    case COL_SWACTIVATION_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_SWACTIVATION_SWTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->swType, tlvArray );
      break;
    case COL_SWACTIVATION_VERSION:
      fillTagLenString( tableId, (rowId+1), colId, config->version, strlen(config->version), tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swInstall the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementSwInstallTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  SwInstall  *config = getData(rowId);

  switch(colId)
  {
    case COL_SWINSTALL_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_SWINSTALL_INSTALLATIONTYPE:
      fillTagLenUint32( tableId, (rowId+1), colId, config->installationType, tlvArray );
      break;
    case COL_SWINSTALL_FILENAME:
      fillTagLenString( tableId, (rowId+1), colId, config->filename, strlen(config->filename), tlvArray );
      break;
    case COL_SWINSTALL_INSTALLER:
      fillTagLenString( tableId, (rowId+1), colId, config->installer, strlen(config->installer), tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementTraceControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  TraceControl  *config = getData(rowId);

  switch(colId)
  {
    case COL_TRACECONTROL_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_TRACECONTROL_TRACECOMMAND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceCommand, tlvArray );
      break;
    case COL_TRACECONTROL_TRACEDURATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->traceDuration, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill cellAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementCellAdminTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  CellAdmin  *config = getData(rowId);

  switch(colId)
  {
    case COL_CELLADMIN_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_CELLADMIN_CELLID:
      fillTagLenUint8( tableId, (rowId+1), colId, config->cellId, tlvArray );
      break;
    case COL_CELLADMIN_CELLCOMMAND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->cellCommand, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/**
 * @param tableId    Table ID
 * @param rowId      Row ID
 * @param colId      Column ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill counterControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void getElementCounterControlTable(
  IN TableId        tableId,
  IN RowId          rowId,
  IN ColId          colId,
  IO TlvArray      *tlvArray
  ) 
{
  DataHdlr      getData = g_OprCmdTable[tableId-MIN_OPR_CMD_TABLEID].getData;
  CounterControl  *config = getData(rowId);

  switch(colId)
  {
    case COL_COUNTERCONTROL_INDEX:
      fillTagLenUint32( tableId, (rowId+1), colId, config->index, tlvArray );
      break;
    case COL_COUNTERCONTROL_PROFILEID:
      fillTagLenUint32( tableId, (rowId+1), colId, config->profileId, tlvArray );
      break;
    case COL_COUNTERCONTROL_COUNTERCOMMAND:
      fillTagLenUint32( tableId, (rowId+1), colId, config->counterCommand, tlvArray );
      break;
    case COL_COUNTERCONTROL_MEASUREMENTDURATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->measurementDuration, tlvArray );
      break;
    case COL_COUNTERCONTROL_REPORTINGDURATION:
      fillTagLenUint32( tableId, (rowId+1), colId, config->reportingDuration, tlvArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, (rowId+1), colId );
      break;
  }
}
/* GetTable OprCommands */
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableEnbAdminTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_ENBADMIN_TABLE; rowId++)
  {
     getRowEnbAdminTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUpload the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableFileUploadTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_FILEUPLOAD_TABLE; rowId++)
  {
     getRowFileUploadTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileDownload the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableFileDownloadTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_FILEDOWNLOAD_TABLE; rowId++)
  {
     getRowFileDownloadTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swActivation the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableSwActivationTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_SWACTIVATION_TABLE; rowId++)
  {
     getRowSwActivationTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swInstall the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableSwInstallTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_SWINSTALL_TABLE; rowId++)
  {
     getRowSwInstallTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableTraceControlTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_TRACECONTROL_TABLE; rowId++)
  {
     getRowTraceControlTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill cellAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableCellAdminTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_CELLADMIN_TABLE; rowId++)
  {
     getRowCellAdminTable( tableId, rowId, tlvArray);
  }
}
 
/**
 * @param tableId    Table ID
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill counterControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void getTableCounterControlTable(
  IN TableId        tableId,
  IO TlvArray      *tlvArray
  ) 
{
  RowId   rowId=0;
  for (rowId=FIRST_ROW; rowId <= MAX_ROWS_IN_COUNTERCONTROL_TABLE; rowId++)
  {
     getRowCounterControlTable( tableId, rowId, tlvArray);
  }
}
 
/* struct2Tlv OprCommands */
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill enbAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvEnbAdminTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileUpload the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvFileUploadTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill fileDownload the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvFileDownloadTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swActivation the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvSwActivationTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill swInstall the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvSwInstallTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill traceControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvTraceControlTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill cellAdmin the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvCellAdminTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/**
 * @param tagInfo      TagInfo
 * @param tlvArray     Array of TLV in which Response will be filled.
 * @section This function will get and fill counterControl the tableId.RowId.Colid information and fill the response TLV.
 * */
void struct2TlvCounterControlTable(
  IO TagInfo       *tagInfo,
  IO TlvArray      *tlvArray
  ) 
{
  GetCmdType      cmdType;
  cmdType = getCmdType(tagInfo);
  
  switch( cmdType )
  {
    case GET_TABLE:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getTableFunc( tagInfo->tableId, tlvArray );
      break;
    case GET_ROW:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getRowFunc( tagInfo->tableId, tagInfo->rowId, tlvArray );
      break;
    case GET_ELEMENT:
      g_OprCmdTable[tagInfo->tableId-MIN_OPR_CMD_TABLEID].getElementFunc( tagInfo->tableId, tagInfo->rowId - 1, tagInfo->colId, tlvArray );
      break;
    default:
      /* Error Condition */
      break;
  }
}
/* Extern Global Variables OprCommands */

