/**
 * @file    generic_table.h
 * @brief This file contains all generic table structure used for all possible tables in
 *           eNodeB
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Shweta Polepally
 * Date           : 08-Oct-2010
 * Change History :
 */


#ifndef GENERIC_TABLE_H
#define GENERIC_TABLE_H

/* common file for .h */

#include "bs_err.h"
#include "tlv_enc_dec.h"
#include "fill_tlv_func.h"
#include "all_cfg.h"

#define FIRST_ROW   1
#define FIRST_COL   1
#define SINGLE_ROW_TABLE 1

typedef void* (*DataHdlr)( IN RowId rowId );
typedef void (*GetTableHdlr)( IN TableId tableId, IO TlvArray  *tlvArray );
typedef void (*GetRowHdlr)( IN TableId tableId, IN RowId rowId, IO TlvArray  *tlvArray );
typedef void (*GetElementHdlr)( IN TableId tableId, IN RowId rowId, IN ColId  colId, IO TlvArray  *tlvArray );
typedef void (*Tlv2StructFuncHdlr)(IO TlvArray  *tlvArray, IO void  *config );
typedef void (*Struct2tlvFuncHdlr)(IO void *config,  IO TlvArray  *tlvArray );
typedef void (*PrintFuncHdlr)(void);
typedef void (*EmptyTableHdlr)(void);
typedef void (*CreateTableHdlr)(void);
typedef void (*DeleteTableHdlr)(void);
typedef NumEntry (*ReadFromFileFuncHdlr)( IN UINT8 *fileToRead, IO void *cfgTable, IO UINT8 *error);
typedef void (*WriteToFileFuncHdlr)( IO void *cfgTable, IO UINT8 *fileToWrite, IO UINT8 *error, IO FILE *fp);
typedef void (*WriteAllowedFuncHdlr)(void);
typedef void (*NumOfRowsHdlr)(void);
typedef void (*MaxNumOfRowsHdlr)(void);
typedef void (*InsertRowHdlr)(void);
typedef void (*UpdateRowHdlr)(void);
typedef void (*DeleteRowHdlr)(void);
typedef void (*InsertElementHdlr)(void);
typedef void (*DeleteElementHdlr)(void);
typedef void (*IsElementPresentHdlr)(void);
typedef void (*IsRowPresentHdlr)(void);

typedef struct s_GenericTable
{
  DataHdlr             getData;                  /* values stored in the table */
  GetTableHdlr         getTableFunc;              /* values stored in the table */
  GetRowHdlr           getRowFunc;                /* returns pointer to the specified row */
  GetElementHdlr       getElementFunc;         /* returns pointer to the element */
  Struct2tlvFuncHdlr   getStruct2TlvFunc;         /* GET Func: function that converts structure to tlv */
  Tlv2StructFuncHdlr   getTlv2StructFunc;         /* SET Func: function that converts tlv to structure format */
  PrintFuncHdlr        getPrintFunc;              /* for logging/printing table */
  EmptyTableHdlr       getEmptyTableFunc;         /* call this function to empty the table */
  CreateTableHdlr      getCreateTableFunc;
  DeleteTableHdlr      getDeleteTableFunc;
  ReadFromFileFuncHdlr getReadFromFileFunc;       /* reads the table data from file into structure */
  WriteToFileFuncHdlr  getWriteToFileFunc;        /* writes data in structure to file */
  WriteAllowedFuncHdlr getWriteAllowedFunc;       /* check if write is allowed -- all consistency checks */
  NumOfRowsHdlr        getNumOfRows;              /* returns the number of rows present in table */
  MaxNumOfRowsHdlr     getMaxNumOfRows;           /* returns the Max number of rows of the table */
  InsertRowHdlr        getInsertRowFunc;
  UpdateRowHdlr        getUpdateRowFunc;          /* change an existing row */
  DeleteRowHdlr        getDeleteRowFunc;          /* delete a row from table */
  InsertElementHdlr    getInsertElementFunc;      /* adds an element */
  DeleteElementHdlr    getDeleteElementFunc;      /* removes an element */
  IsElementPresentHdlr getIsElementPresentFunc;   /* check if it is present inside */
  IsRowPresentHdlr     getIsRowPresentFunc;       /* checks if row is present */
} GenericTable;

extern GenericTable  g_OprCmdTable[MAX_NUM_OPR_CMD_TABLES];
extern GenericTable  g_OprCfgTable[MAX_NUM_OPR_CFG_TABLES];
extern GenericTable  g_StatusTable[MAX_NUM_STATUS_TABLES];
extern GenericTable  g_PreCfgTable[MAX_NUM_PRE_CFG_TABLES];

void emptyAllTables(void);
void fillGenericFunctionTable(void);

OamTableType  getTableTypeByTableId( TableId tableId );
GenericTable *getTableFunctions( IN TableId tableId );
DataHdlr     *getFirstRowOfTable( IN TableId tableId );
DataHdlr     *getRowOfTable( IN TableId tableId, IN RowId rowId );

#define getNextRowOfTable(ptrToTable) ptrToTable++;

#endif /* GENERIC_TABLE_H_ */
