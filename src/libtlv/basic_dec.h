/**
 * @file    basic_dec.h (inter module communication)
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains the function definitions for decoding data from TLV
 *
 * @author : Lakshmi Narayan Madala
 * Date   :
 * Description :  Initial Coding Done - Lakshmi
 *                Moved all dec related functions from basic_types.h to here - Shweta/Raja
 *
 */

#ifndef BASIC_DEC_H
#define BASIC_DEC_H

#include "basic_types.h"

/* Structures */
/** TLV structure to store current information of TLV getting decoded */
typedef struct s_DecBuf
{
  UINT16       tag;
  UINT16       len;
  UINT16       index;
  UINT8       *value;
} DecBuf;

typedef DecBuf Tlv;

typedef struct s_DecPdu
{
  Length  len;
  UINT8  *value;
  UINT16  index;
} DecPdu;

#define initDecBuf( bufP, dataLen, dataVal ) \
                (bufP)->index = 0, (bufP)->len =dataLen, (bufP)->value = dataVal

#define getUint8(bufP)\
  *((bufP)->value + (bufP)->index), (bufP)->index +=1

#define getUint16(bufP)\
  ( (*((bufP)->value + (bufP)->index)) << 8 | *((bufP)->value + (bufP)->index + 1) ), (bufP)->index +=2

#define getUint32(bufP) \
  ( (*((bufP)->value + (bufP)->index)) << 24 |\
      *((bufP)->value + (bufP)->index + 1) << 16 |\
      *((bufP)->value + (bufP)->index + 2) << 8 |\
      *((bufP)->value + (bufP)->index + 3) ),\
       (bufP)->index +=4

#define getBytes(src, len, dest) \
        memcpy(dest, ((src)->value + (src)->index),(size_t) len), (src)->index +=len

#define getRemBytes( bufP, dest, datalen)\
     (datalen) = (size_t)((bufP)->len - (bufP)->index); memcpy(dest, ((bufP)->value + (bufP)->index), (datalen) ), (bufP)->index =datalen;

#define peekUint8(bufP)\
  *((bufP)->value + (bufP)->index)

#define peekUint16(bufP)\
  ( (*((bufP)->value + (bufP)->index)) << 8 | *((bufP)->value + (bufP)->index + 1) )

#define peekUint32(bufP) \
  ( (*((bufP)->value + (bufP)->index)) << 24 |\
      *((bufP)->value + (bufP)->index + 1) << 16 |\
      *((bufP)->value + (bufP)->index + 2) << 8 |\
      *((bufP)->value + (bufP)->index + 3) )

UINT32 getBits (
  UINT8 data[],
  UINT32 curBitIndex,
  UINT32 numBits2Read
  );
UINT8 getUint8FromDecBuf(
  DecBuf *bufP,
  UINT16 index
  );

UINT16 getUint16FromDecBuf(
  DecBuf *bufP,
  UINT16 index
  );

UINT32 getUint32FromDecBuf(
  DecBuf *bufP,
  UINT16 index
  );


#define getUint32FromArray( data ) ( (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]) );
#define getUint16FromArray( data ) ( (data[0] << 8) | (data[1]) );
#define getUint8FromArray( data )  ( data[0] );
#define getBytesFromArray(src, len, dest) \
                       memcpy(dest, src, len)

#endif



