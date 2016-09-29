/**
 * @file    tlv_enc_dec.h
 * @brief TLV encoding and decoding functions.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * AutHor         : Shweta Polepally
 * Created Date   : 6th Aug 2010
 * Change History : Shweta 09 Sep 2010
 *                  > Changed the file name to tlvEncDec.h to tlv_enc_dec.h (as per naming convention)
 */

/*
 * 6 Message structures are defined below:
 *
 * {T,L,V}  = {tag, len, value}
 * [n] denotes array.
 * {T,L,V}[n] denotes array of T,L,V 's are represent
 *
 * GetObjReq     ( {T}[n] )                      --
 * GetObjRspSucc ( {T,L,V}[n] )
 * GetObjRspFail ( {T}[n], ErrRspCode )
 *
 * SetObjReq     ( {T,L,V}[n] )
 * SetObjRspSucc ( {T,L,V}[n] )
 * SetObjRspFail ( {T,L,V}[n], ErrrspCode )
 * */

#ifndef TLV_ENC_DEC_H
#define TLV_ENC_DEC_H

#include "all_cmn.h"


#define MAX_TLV_TAGS              32
#define MAX_TLV_VALUE_IN_BYTES   256

typedef UINT16 NumTlvs;
typedef UINT16 NumTags;
typedef UINT16 TlvLen;

typedef struct s_TlvInfo
{
  TagInfo   tag;
  TlvLen    len;
  UINT8     data[MAX_TLV_VALUE_IN_BYTES];
} TlvInfo;

typedef BsErrorValue ErrRspCode;

typedef struct s_TagArray
{
  NumTags     numTags;
  TagInfo     tagInfo[MAX_TLV_TAGS];
} TagArray;

typedef struct s_TlvArray
{
  NumTags     numTlvs;
  TlvInfo     tlvInfo[MAX_TLV_ELEMENTS];
} TlvArray;

typedef struct s_GetObjReq
{
  CmdRef      cmdRef;
  TagArray    tagArray;
} GetObjReq;

typedef struct s_GetObjRspSucc
{
  CmdRef      cmdRef;
  TlvArray    tlvArray;
} GetObjRspSucc;

typedef struct s_ErrRsp
{
  ErrRspCode  errRspCode;
  UINT8       errorInfo[MAX_TLV_VALUE_IN_BYTES + 1 ];
  TagInfo     failedTagInfo;
} ErrRsp;

typedef struct s_GetObjRspFail
{
  CmdRef      cmdRef;
  TagArray    tagArray;
  ErrRsp      getErrRsp;
} GetObjRspFail;

typedef GetObjRspSucc SetObjReq;
typedef GetObjRspSucc SetObjRspSucc;

typedef struct s_SetObjRspFail
{
  CmdRef      cmdRef;
  TlvArray    tlvArray;
  ErrRsp      setErrRsp;
} SetObjRspFail;

typedef struct s_NotiTlvInfo
{
  ColId        tagColId;  /* Tag */
  TlvLen       len;       /* len */
  UINT8        data[MAX_TLV_VALUE_IN_BYTES];  /* Value */
} NotiTlvInfo;

typedef struct s_NotiObj
{
  NotiId      notiId;
  NumTlvs     numTlvs;
  NotiTlvInfo notiTlvInfo[MAX_TLV_ELEMENTS];
} NotiObj;

typedef enum e_GetCmdType
{
  GET_TABLE=0,
  GET_ROW,
  GET_ELEMENT,
  GET_NONE
} GetCmdType;

/* Function prototypes */
void encErrRsp(
  IO  ErrRsp  *errRsp,
  IO EncBuf   *encBuf
  );
void encCmdRef(
  IO CmdRef    *cmdRef,
  IO EncBuf    *encBuf
  );
void encGetObjReqMsg(
  IO GetObjReq   *msg,
  IO EncBuf      *encBuf
  );
void encGetObjRspSuccMsg(
  IO GetObjRspSucc   *msg,
  IO EncBuf          *encBuf
  );
void encGetObjRspFailMsg(
  IO GetObjRspFail   *msg,
  IO EncBuf          *encBuf
  );
void encSetObjReqMsg(
  IO SetObjReq       *msg,
  IO EncBuf          *encBuf
  );
void encSetObjRspSuccMsg(
  IO SetObjRspSucc   *msg,
  IO EncBuf          *encBuf
  );
void encSetObjRspFailMsg(
  IO SetObjRspFail   *msg,
  IO EncBuf          *encBuf
  );
void encNotiObjMsg(
  IO NotiObj      *msg,
  IO EncBuf       *encBuf
  );
void decErrRsp(
  IO DecBuf    *decBuf,
  IO ErrRsp    *errRsp
  );
void decGetObjReqMsg (
  IO UINT8          *msg2dec,
  IN Length          len,
  IO GetObjReq      *msg
  );
void decGetObjRspSuccMsg (
  IO UINT8           *msg2dec,
  IN Length           len,
  IO GetObjRspSucc   *msg
  );
void decGetObjRspFailMsg (
  IO UINT8          *msg2dec,
  IN Length          len,
  IO GetObjRspFail  *msg
  );
void decSetObjReqMsg (
  IO UINT8         *msg2dec,
  IN Length        len,
  IO SetObjReq     *msg
  );
void decSetObjRspSuccMsg (
  IO UINT8            *msg2dec,
  IN Length           len,
  IO SetObjRspSucc    *msg
  );
void decSetObjRspFailMsg (
  IO UINT8            *msg2dec,
  IN Length           len,
  IO SetObjRspFail    *msg
  );
void decNotiObjMsg(
  IO UINT8      *msg2dec,
  IN Length      len,
  IO NotiObj    *msg
  );

#endif /* TLV_ENC_DEC_H */
