/**
 * @file   tlv_fill_func.c
 * @brief
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author :           Shweta Polepally
 * Date   :           Aug 2010
 * Change History :
 */


#include "tlv_enc_dec.h"

GetCmdType getCmdType(
  IO TagInfo     *tag
  )
{
  if( (tag->rowId EQ 0) && (tag->colId EQ 0 ))  /* e.g. 3.0.0*/
    return GET_TABLE;
  else if( (tag->rowId NOTEQ 0) && (tag->colId EQ 0 ))  /* e.g. 3.1.0*/
    return GET_ROW;
  else if( (tag->rowId NOTEQ 0) && (tag->colId NOTEQ 0 ))
    return GET_ELEMENT;
  else
    return GET_NONE;
}

/** find 'tag' in 'array' and return pointer to it if found **/
TlvInfo* findTlvInArray(
  IN TagInfo   *tag,
  IO TlvArray  *tlvArray
  )
{
  UINT8 i=0;
  for (i=0; i < tlvArray->numTlvs; i++ )
  {
    if(tlvArray->tlvInfo[i].tag.tableId EQ tag->tableId &&
       tlvArray->tlvInfo[i].tag.rowId EQ tag->rowId     &&
       tlvArray->tlvInfo[i].tag.colId EQ tag->colId) /* found */
      {
        return &(tlvArray->tlvInfo[i]);
      }
  }
  return NULL;
}

static void fillTagInfo(
  IN TableId    tableId,
  IN RowId      rowId,
  IN ColId      colId,
  IO TagInfo    *fillTlvInfo
  )
{
  fillTlvInfo->tableId = tableId;
  fillTlvInfo->rowId   = rowId;
  fillTlvInfo->colId   = colId;
}

#define fillLenInfo( srcLen, destLen )  destLen = srcLen;

void fillTagLenString(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT8          *value,
  IN Length          len,
  IO TlvArray       *tlvRspArray
  )
{
  fillTagInfo( tableId, rowId, colId , &(tlvRspArray->tlvInfo[tlvRspArray->numTlvs].tag) );
  fillLenInfo( len, tlvRspArray->tlvInfo[tlvRspArray->numTlvs].len );
  putBytesInArray( value, len, &(tlvRspArray->tlvInfo[tlvRspArray->numTlvs].data) );
  tlvRspArray->numTlvs++;
}

void fillTagLenUint32(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT32          value,
  IO TlvArray       *tlvRspArray
  )
{
  fillTagInfo( tableId, rowId, colId , &(tlvRspArray->tlvInfo[tlvRspArray->numTlvs].tag) );
  fillLenInfo( 4, tlvRspArray->tlvInfo[tlvRspArray->numTlvs].len );
  putUint32InArray( value, tlvRspArray->tlvInfo[tlvRspArray->numTlvs].data );
  tlvRspArray->numTlvs++;
}

void fillTagLenUint16(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT16          value,
  IO TlvArray       *tlvRspArray
  )
{
  fillTagInfo( tableId, rowId, colId , &(tlvRspArray->tlvInfo[tlvRspArray->numTlvs].tag) );
  fillLenInfo( 2, tlvRspArray->tlvInfo[tlvRspArray->numTlvs].len );
  putUint16InArray( value, tlvRspArray->tlvInfo[tlvRspArray->numTlvs].data );
  tlvRspArray->numTlvs++;
}

void fillTagLenUint8(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT8           value,
  IO TlvArray       *tlvRspArray
  )
{
  fillTagInfo( tableId, rowId, colId , &(tlvRspArray->tlvInfo[tlvRspArray->numTlvs].tag) );
  fillLenInfo( 1, tlvRspArray->tlvInfo[tlvRspArray->numTlvs].len );
  putUint8InArray( value, tlvRspArray->tlvInfo[tlvRspArray->numTlvs].data );
  tlvRspArray->numTlvs++;
}

/* Fill functions for NOTI messages */
void fillNotiTagLenString(
  IN ColId           colId,
  IN UINT8          *value,
  IN Length          len,
  IO NotiObj        *notiObj
  )
{
  notiObj->notiTlvInfo[notiObj->numTlvs].tagColId = colId;
  fillLenInfo( len, notiObj->notiTlvInfo[notiObj->numTlvs].len );
  putBytesInArray( value, len, &(notiObj->notiTlvInfo[notiObj->numTlvs].data) );
  notiObj->numTlvs++;
}

void fillNotiTagLenUint32(
  IN ColId           colId,
  IN UINT32          value,
  IO NotiObj        *notiObj
  )
{
  notiObj->notiTlvInfo[notiObj->numTlvs].tagColId = colId;
  fillLenInfo( 4, notiObj->notiTlvInfo[notiObj->numTlvs].len );
  putUint32InArray( value, notiObj->notiTlvInfo[notiObj->numTlvs].data );
  notiObj->numTlvs++;
}

void fillNotiTagLenUint16(
  IN ColId           colId,
  IN UINT16          value,
  IO NotiObj        *notiObj
  )
{
  notiObj->notiTlvInfo[notiObj->numTlvs].tagColId = colId;
  fillLenInfo( 2, notiObj->notiTlvInfo[notiObj->numTlvs].len );
  putUint16InArray( value, notiObj->notiTlvInfo[notiObj->numTlvs].data );
  notiObj->numTlvs++;
}

void fillNotiTagLenUint8(
  IN ColId           colId,
  IN UINT8           value,
  IO NotiObj        *notiObj
  )
{
  notiObj->notiTlvInfo[notiObj->numTlvs].tagColId = colId;
  fillLenInfo( 1, notiObj->notiTlvInfo[notiObj->numTlvs].len );
  putUint8InArray( value, notiObj->notiTlvInfo[notiObj->numTlvs].data );
  notiObj->numTlvs++;
}
