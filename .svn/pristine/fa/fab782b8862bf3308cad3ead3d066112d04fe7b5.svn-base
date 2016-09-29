/**
 * @file    cmn_types.h
 * @brief This file contains the common structures that are shared across multiple modules
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 *
 * @author : Lakshmi Narayana Madala
 * Date   :
 * Description : Initial Coding
 *
 */

#ifndef CMN_TYPES_H
#define CMN_TYPES_H

#include "bs_tables.h"
#include "basic_types.h"
#include "bs_err.h"
#include "cmn_larm_uarm.h"

typedef UINT32 NumEntry;

typedef enum s_FthProto
{
  FTP,
  SCP,
  SFTP
}FthProto;

/** These are the different methods available for transport of messages across executables. */
typedef enum e_ImcMethod {
  UDP_SOCK = 1,
  UNIX_SOCK
} ImcMethod;

typedef String50   TableName;

typedef String50   NotiMsgName;

typedef enum e_DataType
{
  BS_TYPE_SINT8,
  BS_TYPE_SINT16,
  BS_TYPE_SINT32,
  BS_TYPE_UINT8,
  BS_TYPE_UINT16,
  BS_TYPE_UINT32,
  BS_TYPE_STRING,
  BS_TYPE_BOOL,
  BS_TYPE_MODID,
  BS_TYPE_MSGID,
  BS_TYPE_ENUM,
  BS_TYPE_IPV4ADDRESS,
  BS_TYPE_MACADDRESS,
  BS_TYPE_FILENAME,
  BS_TYPE_LOCALTIME,
  BS_TYPE_OFFSETTIME,
  BS_TYPE_SWVERSION,
  BS_TYPE_MCC,
  BS_TYPE_MNC,
  BS_TYPE_LATITUDE,
  BS_TYPE_LONGITUDE,
  BS_TYPE_ENODEBNAME,
  BS_TYPE_PROFILENAME,
  BS_TYPE_COUNTERID
} DataType;

typedef enum e_OamTableType
{
  TABLE_UNKNOWNTYPE, /* to be used for errors */
  TABLE_OPERCMD,
  TABLE_OPERCONFIG,
  TABLE_STATUS,
  TABLE_PRECONFIG
} OamTableType;

typedef enum e_Attributes
{
  READ_ONLY,
  READ_WRITE,
  WRITE_WHEN_LOCK_ONLY
} Attributes;

typedef enum e_ParamType
{
  PARAM_OPTIONAL,
  PARAM_MANDATORY
} ParamType;

typedef enum e_Persistent
{
  PERSISTENT,
  NON_PERSISTENT
} Persistent;

typedef String50    ColName;
typedef String50    SpecialValue;
typedef String50    DataTypeStr;
typedef String50    AttributeStr;
typedef String1kb   Description;


#define MAX_TABLE_NAME 50
#define MAX_COL_NAME   50

#define MAX_ELEMENTS_IN_TABLE           150  /* This need to be removed and should be taken from
                                                bs_tables.h file. As of now, we dont have
                                                this information */

typedef struct s_OamColInfo
{
  ColName       colName;
  UINT32        colId;
  DataType      dataType;
  UINT16        length;
  UINT32        colMinVal;
  UINT32        colMaxVal;
  Description   desc;
  Attributes    attributes;
  Persistent    persistent;
  TBOOL         isSpecialValuePresent;
  SpecialValue  specialValue;
  ParamType     paramtype;
} OamColInfo;

typedef struct s_OamTableInfo
{
  TableName      tableName;
  UINT32         noOfColumns;
  UINT32         noOfRows;
  OamTableType   oamTableType;
  OamTableNames  oamTableNames;
  Description    desc;
  OamColInfo     colInfo[MAX_ELEMENTS_IN_TABLE];
}OamTableInfo;

typedef struct s_OamTableContext
{
  UINT32        noOfvalidTables;
  OamTableInfo  oprCmdTableInfo[MAX_NUM_OPR_CMD_TABLES];
  OamTableInfo  oprCfgTableInfo[MAX_NUM_OPR_CFG_TABLES];
  OamTableInfo  statusTableInfo[MAX_NUM_STATUS_TABLES];
  OamTableInfo  preCfgTableInfo[MAX_NUM_PRE_CFG_TABLES];
} OamTableContext;



/****************************************************/
typedef struct s_OamColInfoForTlvPrint
{
  ColName       colName;
  DataType      dataType;
} OamColInfoForTlvPrint;

typedef struct s_OamTableInfoForTlvPrint
{
  TableName                 tableName;
  OamColInfoForTlvPrint     colInfo[MAX_ELEMENTS_IN_TABLE];
}OamTableInfoForTlvPrint;

typedef struct s_OamTableContextForTlvPrint
{
  UINT32                   noOfvalidTables;
  OamTableInfoForTlvPrint  oprCmdTableInfo[MAX_NUM_OPR_CMD_TABLES];
  OamTableInfoForTlvPrint  oprCfgTableInfo[MAX_NUM_OPR_CFG_TABLES];
  OamTableInfoForTlvPrint  statusTableInfo[MAX_NUM_STATUS_TABLES];
  OamTableInfoForTlvPrint  preCfgTableInfo[MAX_NUM_PRE_CFG_TABLES];
} OamTableContextForTlvPrint;


/* Structures used for referencing CIH/WIH/SIH <-> OIH <-> BSM <-> Any module */

typedef enum { OAMIF_CIH = 1 /* CLI */, OAMIF_WIH /* Web Craft */, OAMIF_SIH /* SNMP */} OamIfType;
typedef UINT32 CmdRefNum;
typedef UINT32 InstanceId;
typedef UINT16 RowId;
typedef UINT8  ColId;
typedef UINT8  TableId;
typedef UINT32 NotiId;
typedef UINT32 ColBitMap;

/*
 * Error chaining data structure
 */
#define MAX_NO_ERR  50
#define MAX_ERR_LEN 300
typedef struct s_ErrChain
{
  UINT8 buf[MAX_NO_ERR][MAX_ERR_LEN];
  Index index;
}__attribute__((__packed__))ErrChain;

/*  Reference is used to distinguish Messages coming from CLI, WEB and SNMP at OIH   */
typedef struct s_CmdRef
{
  OamIfType     origin;    /* CLI: 0, WEB: 1, SNMP: 2 */
  CmdRefNum  cmdRefNum; /* Normal sequence Number will be incremented for each message */
  InstanceId instanceId;/* CLI/SNMP/WEB Instance */
}__attribute__((__packed__))CmdRef;

typedef struct s_ProcFail
{
  BsErrorValue error;
  String256    failReason;
} __attribute__((__packed__))ProcFail;

/* Error Description */
typedef struct s_ErrorDesc
{
  BsErrorValue errorNum;
  String256    failReason;
} __attribute__((__packed__))ErrorDesc;

typedef ErrorDesc OpStartFail;

/* Enode disable reason */
typedef enum e_EnodebDisableReason
{
  USER_INITAITED_LOCK,
  MOD_DOWN_OCCURED,
  S1AP_LINK_DOWN,
  DEVICE_DOWN,
  UEH_OPSTART_FAIL,
  S1AP_OPSTART_FAIL,
  PMH_OPSTART_FAIL,
  DEH_OPSTART_FAIL,
  CRITICAL_ALARM_GENERATED
} EnbDisableReason;

typedef struct s_MsgBsmModOpStopReq
{
  EnbDisableReason enbDisableReason;
} MsgBsmModOpStopReq;

/* Exit Cmd Message common across all modules */
typedef enum e_ModExitCause
{
  MODEXITCAUSE_RESTART = 1 ,
  MODEXITCAUSE_REBOOT,
  MODEXITCAUSE_SWACT,
  MODEXITCAUSE_L2L3_PATCH_INSTALL,
  MODEXITCAUSE_L2L3_PATCH_INSTALL_FAILED
} ModExitCause;

typedef struct s_MsgPrmModExitCmd
{
  ModExitCause modExitCause;
} MsgPrmModExitCmd;

typedef struct s_EnbConfigSucc
{
  UINT16    transId; /* transaction ID */
} EnbConfigSucc;

typedef struct s_EnbConfigFail
{
  UINT16        transId; /* transaction ID */
  ErrorDesc     errorDesc; /* includes error code and error string from UEH */
} EnbConfigFail;

typedef struct s_TagInfo
{
  TableId tableId;
  RowId   rowId;
  ColId   colId;
} TagInfo;

#endif
