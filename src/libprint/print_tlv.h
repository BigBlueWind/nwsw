/**
 * @file    print_tlv.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains all function definitions used to print Messages in TLV format.
 *
 * @author     : mkata
 * Date       : 10 - 05 - 2010
 * Description:
 */

#ifndef PRINT_TLV_H
#define PRINT_TLV_H

#include "cmn_types.h"
#include "all_cfg.h"
#include "enumstr.h"
#include "tlv_enc_dec.h"
#include "bsm_ueh_if.h"

extern EnumStrInfo c_EnumStrInfoTab[];

/* OAM Table Context */
extern OamTableContextForTlvPrint     g_PrintTlvOamTableContext;

/* Get OAM Table Context */
#define getOamTableTlvInfo()           &(g_PrintTlvOamTableContext)

#define tlvPrintGetOprCmdTableInfo(tableId)   &(g_PrintTlvOamTableContext.oprCmdTableInfo[tableId])
#define tlvPrintGetOprCfgTableInfo(tableId)   &(g_PrintTlvOamTableContext.oprCfgTableInfo[tableId])
#define tlvPrintGetStatusTableInfo(tableId)   &(g_PrintTlvOamTableContext.statusTableInfo[tableId])
#define tlvPrintGetPreCfgTableInfo(tableId)   &(g_PrintTlvOamTableContext.preCfgTableInfo[tableId])
/* TODO : Temporary solution */
int parseOamTableCfg(
  IN UINT8*          configFilePath,
  IO OamTableContext *oamCtx
  );
/* Print TLV messages initialisation function */
void printTlvInit(
  IO UINT8 *globalConfigFilePath
  );

void printTlv(
  IO TlvArray    *tlvArray
  );

void printTlvCmdRef(
  IN CmdRef *ref
  );
UINT32 printTlvTagInfo(
  IN TagInfo *info
  );

/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs GetObjReqMsg
 */
void printGetObjReqMsg(
  IO void    *data,
  IO UINT16  len
  );


/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs GetObjRspSuccMsg
 */
void printGetObjRspSuccMsg(
  IO void    *data,
  IO UINT16  len
  );
/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs GetObjRspFailMsg
 */
void printGetObjRspFailMsg(
  IO void    *data,
  IO UINT16  len
  );

/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs SetObjReqMsg
 */
void printSetObjReqMsg(
  IO void    *data,
  IO UINT16  len
  );


/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs SetObjRspSuccMsg
 */
void printSetObjRspSuccMsg(
  IO void    *data,
  IO UINT16  len
  );
/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs SetObjRspFailMsg
 */
void printSetObjRspFailMsg(
  IO void    *data,
  IO UINT16  len
  );
  
/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs NotiObjReqMsg
 */
void printNotiObjReqMsg(
  IO void    *data,
  IO UINT16  len
  );
  
inline OamTableType printTlvGetTableTypeByTableId(
  IN TableId       tableId
  );
OamTableInfoForTlvPrint* printTlvGetTablePtrByTableId(
  IN TableId      tableId
  );
int parseOamCfgForLibPrint(
  IN UINT8*                     configFilePath,
  IO OamTableContextForTlvPrint *oamCtx
  );
void getColNameAndData(
  IO TlvInfo  *objTlvInfo,
  IO UINT8    *columnName,
  IO UINT8    *data
  );

#endif /* PRINT_TLV_H */
