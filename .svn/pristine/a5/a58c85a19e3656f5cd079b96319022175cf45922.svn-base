/**
 * @file    tlv_dec.c
 * @brief TLV decoding functions.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * AutHor         : Shweta Polepally
 * Created Date   : 6th Aug 2010
 * Change History : Shweta 09 Sep 2010
 *                  > Changed the file name to tlvDec.c to tlv_dec.c (as per naming convention)
 */


#include "tlv_enc_dec.h"

/* Command Decoding functions */
#define decNumTlvElem( buf, numElem )  numElem = getUint16(buf);
#define decErrRspCode( buf, errCode )  errCode = getUint32(buf);

#define decTableId( buf, tableId )   tableId = getUint8(buf);
#define decRowId( buf, rowId )       rowId   = getUint16(buf);
#define decColId( buf, colId )       colId   = getUint8(buf);
#define decNotiId( buf, notiId )     notiId  = getUint32(buf);

/*
 * @param    decBuf  : This is pointer to the buffer from where data needs to be decoded.
 * @param    tag     : TLV Tag information structure.
 * @details  This function will decode TLV command reference information in TLV format.
 **/
static void getTag(
  IO DecBuf   *buf,
  IO TagInfo   *tag )
{
  decTableId( buf, tag->tableId );
  decRowId( buf, tag->rowId);
  decColId( buf, tag->colId);
}

#define getLen( buf, len )        len = getUint16(buf);
#define getVal( buf, len, val )   getBytes(buf, len, val);

/*
 * @param    decBuf  : This is pointer to the buffer from where data needs to be decoded.
 * @param    cmdRef  : Command Reference structure.
 * @details  This function will decode command Reference information in TLV format
 **/
static void decCmdRef(
  IO DecBuf    *decBuf,
  IO CmdRef    *cmdRef
  )
{
  cmdRef->origin     = getUint32(decBuf);
  cmdRef->cmdRefNum  = getUint32(decBuf);
  cmdRef->instanceId = getUint32(decBuf);
}

/*
 * @param    buf     : This is pointer to the buffer from where data needs to be decoded.
 * @param    errRsp  : Error Response structure
 * @details  This function will decode error response information in TLV format
 **/
void decErrRsp(
  IO DecBuf    *buf,
  IO ErrRsp    *errRsp
  )
{
  UINT16   lenOfErrInfo = 0 ;
  decErrRspCode(buf, errRsp->errRspCode );
  getLen( buf, lenOfErrInfo );
  getVal( buf, lenOfErrInfo, errRsp->errorInfo );
  getTag( buf, &(errRsp->failedTagInfo) );
}
/* Decoding messages - Get / Set messages */
/*
 * @param    msg2dec : This is pointer to the buffer from where data needs to be decoded.
 * @param    len     : Total bytes in the buffer.
 * @param    msg     : Get Object Request message structure.
 * @details  This function will decode the data in TLV format and fill the details
 *           in GetObjReq message structure.
 **/
void decGetObjReqMsg (
  IO UINT8          *msg2dec,
  IN Length          len,
  IO GetObjReq      *msg
  )
{
  DecBuf   buf = { .len = len, .value = msg2dec, .index = 0, .tag = 0 }; /*tag=not used*/
  NumTags  numtlvIndex;

  decCmdRef( &buf, &(msg->cmdRef) );
  decNumTlvElem( &buf, msg->tagArray.numTags );

  /* Get Tag Values here */
  for ( numtlvIndex =0; numtlvIndex < msg->tagArray.numTags; numtlvIndex++ )
  {
    getTag( &buf, &(msg->tagArray.tagInfo[numtlvIndex]));
  }
}

/*
 * @param    msg2dec : This is pointer to the buffer from where data needs to be decoded.
 * @param    len     : Total bytes in the buffer.
 * @param    msg     : Get Object Response Success message structure.
 * @details  This function will decode the data in TLV format and fill the details
 *           in GetObjRspSucc message structure.
 **/
void decGetObjRspSuccMsg (
  IO UINT8           *msg2dec,
  IN Length           len,
  IO GetObjRspSucc   *msg
  )
{
  DecBuf   buf = { .len = len, .value = msg2dec, .index = 0, .tag = 0 }; /*tag=not used*/
  NumTlvs  numtlvIndex;

  decCmdRef( &buf, &(msg->cmdRef) );
  decNumTlvElem( &buf, msg->tlvArray.numTlvs );

  /* Get Tag Values here */
  for (numtlvIndex =0; numtlvIndex < msg->tlvArray.numTlvs; numtlvIndex++)
  {
    getTag( &buf, &(msg->tlvArray.tlvInfo[numtlvIndex].tag) );
    getLen( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len );
    getVal( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len, msg->tlvArray.tlvInfo[numtlvIndex].data );
  }
}

/*
 * @param    msg2dec : This is pointer to the buffer from where data needs to be decoded.
 * @param    len     : Total bytes in the buffer.
 * @param    msg     : Get Object Response Failure message structure.
 * @details  This function will decode the data in TLV format and fill the details
 *           in GetObjRspFail message structure.
 **/
void decGetObjRspFailMsg (
  IO UINT8          *msg2dec,
  IN Length          len,
  IO GetObjRspFail  *msg
  )
{
  DecBuf   buf = { .len = len, .value = msg2dec, .index = 0, .tag = 0 }; /*tag=not used*/
  NumTags  numtlvIndex;

  decCmdRef( &buf, &(msg->cmdRef) );
  decNumTlvElem( &buf, msg->tagArray.numTags );

  /* Get Tag Values here */
  for (numtlvIndex =0; numtlvIndex < msg->tagArray.numTags; numtlvIndex++)
  {
    getTag( &buf, &(msg->tagArray.tagInfo[numtlvIndex]));
  }
  decErrRsp(&buf, &msg->getErrRsp);
}

/*
 * @param    msg2dec : This is pointer to the buffer from where data needs to be decoded.
 * @param    len     : Total bytes in the buffer.
 * @param    msg     : Set Object Request message structure.
 * @details  This function will decode the data in TLV format and fill the details
 *           in SetObjReq message structure.
 * */
void decSetObjReqMsg (
  IO UINT8         *msg2dec,
  IN Length        len,
  IO SetObjReq     *msg
  )
{
  DecBuf   buf = { .len = len, .value = msg2dec, .index = 0, .tag = 0 }; /*tag=not used*/
  NumTlvs  numtlvIndex;

  decCmdRef( &buf, &(msg->cmdRef) );
  decNumTlvElem( &buf, msg->tlvArray.numTlvs );

  /* Get Tag Values here */
  for ( numtlvIndex =0; numtlvIndex < msg->tlvArray.numTlvs; numtlvIndex++ )
  {
    getTag( &buf, &(msg->tlvArray.tlvInfo[numtlvIndex].tag) );
    getLen( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len );
    getVal( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len, msg->tlvArray.tlvInfo[numtlvIndex].data );
  }
}

/*
 * @param    msg2dec : This is pointer to the buffer from where data needs to be decoded.
 * @param    len     : Total bytes in the buffer.
 * @param    msg     : Set Object Response Success message structure.
 * @details  This function will decode the data in TLV format and fill the details
 *           in SetObjRspSucc message structure.
 * */
void decSetObjRspSuccMsg (
  IO UINT8            *msg2dec,
  IN Length           len,
  IO SetObjRspSucc    *msg
  )
{
  DecBuf   buf = { .len = len, .value = msg2dec, .index = 0, .tag = 0 }; /*tag=not used*/
  NumTlvs  numtlvIndex;

  decCmdRef( &buf, &(msg->cmdRef) );
  decNumTlvElem( &buf, msg->tlvArray.numTlvs );

  /* Get Tag Values here */
  for (numtlvIndex =0; numtlvIndex < msg->tlvArray.numTlvs; numtlvIndex++)
  {
    getTag( &buf, &(msg->tlvArray.tlvInfo[numtlvIndex].tag) );
    getLen( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len );
    getVal( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len, msg->tlvArray.tlvInfo[numtlvIndex].data );
  }
}

/*
 * @param    msg2dec : This is pointer to the buffer from where data needs to be decoded.
 * @param    len     : Total bytes in the buffer.
 * @param    msg     : Set Object Response Failure message structure.
 * @details  This function will decode the data in TLV format and fill the details
 *           in SetObjRspFail message structure.
 * */
void decSetObjRspFailMsg (
  IO UINT8            *msg2dec,
  IN Length           len,
  IO SetObjRspFail    *msg
  )
{
  DecBuf   buf = { .len = len, .value = msg2dec, .index = 0, .tag = 0 }; /*tag=not used*/
  NumTlvs  numtlvIndex;

  decCmdRef( &buf, &(msg->cmdRef) );
  decNumTlvElem( &buf, msg->tlvArray.numTlvs );

  /* Get Tag Values here */
  for ( numtlvIndex =0; numtlvIndex < msg->tlvArray.numTlvs; numtlvIndex++ )
  {
    getTag( &buf, &(msg->tlvArray.tlvInfo[numtlvIndex].tag) );
    getLen( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len );
    getVal( &buf, msg->tlvArray.tlvInfo[numtlvIndex].len, msg->tlvArray.tlvInfo[numtlvIndex].data );
  }
  decErrRsp(&buf, &msg->setErrRsp);
}

/*
 * @param    msg :    Set Object Response Success message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the SetObjRspSucc message in the buffer in TLV format.
 * */
void decNotiObjMsg(
  IO UINT8      *msg2dec,
  IN Length      len,
  IO NotiObj    *msg
  )
{

  DecBuf   buf = { .len = len, .value = msg2dec, .index = 0, .tag = 0 }; /*tag=not used*/
  NumTlvs  numtlvIndex;

  decNotiId( &buf, msg->notiId );
  decNumTlvElem( &buf, msg->numTlvs );

  /* Get Tag Values here */
  for ( numtlvIndex =0; numtlvIndex < msg->numTlvs; numtlvIndex++ )
  {
    decColId( &buf, msg->notiTlvInfo[numtlvIndex].tagColId );
    getLen( &buf, msg->notiTlvInfo[numtlvIndex].len );
    getVal( &buf, msg->notiTlvInfo[numtlvIndex].len , msg->notiTlvInfo[numtlvIndex].data );
  }
}
