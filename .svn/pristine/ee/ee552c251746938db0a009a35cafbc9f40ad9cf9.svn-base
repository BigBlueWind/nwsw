/**
 * @file    tlv_enc.c
 * @brief TLV encoding functions.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * AutHor         : Shweta Polepally
 * Created Date   : 6th Aug 2010
 * Change History : Shweta 09 Sep 2010
 *                  > Changed the file name to tlvEec.c to tlv_enc.c (as per naming convention)
 */

#include "tlv_enc_dec.h"

#define encTagColId( len, encBuf )     putUint8 ( len,   encBuf );
#define encLen( len, encBuf )          putUint16( len,   encBuf );
#define encNumTags( value, encBuf )    putUint16( value, encBuf );
#define encNumTlvs( value, encBuf )    putUint16( value, encBuf );
#define encNotiId( value, encBuf )     putUint32( value, encBuf );
#define encErrRspCode( value, encBuf ) putUint32( value, encBuf );
#define encValue( data, len, encBuf )  putBytes ( data,  len, encBuf );

/* Command Encoding functions */
/*
 * @param    tag : TLV tag (TableId, rowId, colId )
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the tag value in the buffer.
 * */
static void encTag(
  IO TagInfo   *tag,
  IO EncBuf   *encBuf
  )
{
  putUint8( tag->tableId, encBuf );
  putUint16( tag->rowId,   encBuf );
  putUint8( tag->colId,   encBuf );
}

/*
 * @param    cmdRef : Command reference. (Origin, Cmd Reference Number, Instance Id)
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the command reference details in the buffer.
 * */
void encCmdRef(
  IO CmdRef    *cmdRef,
  IO EncBuf    *encBuf
  )
{
  putUint32(cmdRef->origin,     encBuf);
  putUint32(cmdRef->cmdRefNum,  encBuf);
  putUint32(cmdRef->instanceId, encBuf);
}

/*
 * @param    errRsp : Error Response structure
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the error response details in the buffer.
 * */
void encErrRsp(
  IO ErrRsp  *errRsp,
  IO EncBuf  *encBuf
  )
{
  encErrRspCode( errRsp->errRspCode, encBuf );
  encLen( sizeof(errRsp->errorInfo), encBuf );
  encValue( errRsp->errorInfo, sizeof(errRsp->errorInfo), encBuf );
  encTag( &(errRsp->failedTagInfo), encBuf );
}
/*
 * @param    msg :    Get Object Request message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the GetObjReq message in the buffer  in TLV format.
 * */
void encGetObjReqMsg(
  IO GetObjReq   *msg,
  IO EncBuf      *encBuf
  )
{
  NumTlvs numTlvIndex;

  encCmdRef( &(msg->cmdRef), encBuf ) ;
  encNumTags( msg->tagArray.numTags, encBuf );

  for ( numTlvIndex=0; numTlvIndex < msg->tagArray.numTags; numTlvIndex++ )
  {
    encTag( &(msg->tagArray.tagInfo[numTlvIndex]), encBuf );
  }
}

/*
 * @param    msg :    Get Object Response Success message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the GetObjRspSucc message in the buffer  in TLV format.
 * */
void encGetObjRspSuccMsg(
  IO GetObjRspSucc   *msg,
  IO EncBuf          *encBuf
  )
{
  NumTlvs numTlvIndex;

  encCmdRef( &(msg->cmdRef), encBuf ) ;
  encNumTlvs( msg->tlvArray.numTlvs, encBuf );

  for ( numTlvIndex=0; numTlvIndex < msg->tlvArray.numTlvs; numTlvIndex++ )
  {
    encTag( &(msg->tlvArray.tlvInfo[numTlvIndex].tag), encBuf );
    encLen( msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
    encValue( msg->tlvArray.tlvInfo[numTlvIndex].data, msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
  }
}

/*
 * @param    msg :    Get Object Response Failure message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the GetObjRspFail message in the buffer in TLV format.
 * */
void encGetObjRspFailMsg(
  IO GetObjRspFail   *msg,
  IO EncBuf          *encBuf
  )
{
  NumTlvs numTlvIndex;

  encCmdRef( &(msg->cmdRef), encBuf ) ;
  encNumTags( msg->tagArray.numTags, encBuf );

  for ( numTlvIndex=0; numTlvIndex < msg->tagArray.numTags; numTlvIndex++ )
  {
    encTag( &(msg->tagArray.tagInfo[numTlvIndex]), encBuf );
  }
  encErrRsp(&msg->getErrRsp, encBuf);
}
/*
 * @param    msg :    Set Object Request message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the SetObjReq message in the buffer in TLV format.
 * */
void encSetObjReqMsg(
  IO SetObjReq       *msg,
  IO EncBuf          *encBuf
  )
{
  NumTlvs numTlvIndex;

  encCmdRef( &(msg->cmdRef), encBuf ) ;
  encNumTlvs( msg->tlvArray.numTlvs, encBuf );

  for( numTlvIndex=0; numTlvIndex < msg->tlvArray.numTlvs; numTlvIndex++ )
  {
    encTag( &(msg->tlvArray.tlvInfo[numTlvIndex].tag), encBuf );
    encLen( msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
    encValue( msg->tlvArray.tlvInfo[numTlvIndex].data, msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
  }
}

/*
 * @param    msg :    Set Object Response Success message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the SetObjRspSucc message in the buffer in TLV format.
 * */
void encSetObjRspSuccMsg(
  IO SetObjRspSucc   *msg,
  IO EncBuf          *encBuf
  )
{
  NumTlvs numTlvIndex;

  encCmdRef( &(msg->cmdRef), encBuf ) ;
  encNumTlvs( msg->tlvArray.numTlvs, encBuf );

  for(numTlvIndex=0; numTlvIndex < msg->tlvArray.numTlvs; numTlvIndex++ )
  {
    encTag( &(msg->tlvArray.tlvInfo[numTlvIndex].tag), encBuf );
    encLen( msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
    encValue( msg->tlvArray.tlvInfo[numTlvIndex].data, msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
  }
}


/*
 * @param    msg :    Set Object Response Failure message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the SetObjRspFail message in the buffer in TLV format.
 * */
void encSetObjRspFailMsg(
  IO SetObjRspFail   *msg,
  IO EncBuf          *encBuf
  )
{
  NumTlvs numTlvIndex;

  encCmdRef( &(msg->cmdRef), encBuf ) ;
  encNumTlvs( msg->tlvArray.numTlvs, encBuf );

  for ( numTlvIndex=0; numTlvIndex < msg->tlvArray.numTlvs; numTlvIndex++ )
  {
    encTag( &(msg->tlvArray.tlvInfo[numTlvIndex].tag), encBuf );
    encLen( msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
    encValue( msg->tlvArray.tlvInfo[numTlvIndex].data, msg->tlvArray.tlvInfo[numTlvIndex].len, encBuf );
  }
  encErrRsp(&msg->setErrRsp, encBuf);
}


/*
 * @param    msg :    Set Object Response Success message structure.
 * @param    encBuf : Buffer in which data will be encoded.
 * @details  This function will encode the SetObjRspSucc message in the buffer in TLV format.
 * */
void encNotiObjMsg(
  IO NotiObj      *msg,
  IO EncBuf       *encBuf
  )
{
  NumTlvs  numTlvIndex;

  encNotiId( msg->notiId, encBuf );
  encNumTlvs( msg->numTlvs, encBuf );

  for(numTlvIndex=0; numTlvIndex < msg->numTlvs; numTlvIndex++ )
  {
    encTagColId( msg->notiTlvInfo[numTlvIndex].tagColId, encBuf );
    encLen( msg->notiTlvInfo[numTlvIndex].len, encBuf );
    encValue( msg->notiTlvInfo[numTlvIndex].data, msg->notiTlvInfo[numTlvIndex].len, encBuf );
  }
}
