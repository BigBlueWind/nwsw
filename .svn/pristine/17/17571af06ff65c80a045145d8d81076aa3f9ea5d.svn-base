/**
 * @file    basic_dec.c (inter module communication)
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains the functions for encoding data
 *
 * @author : Lakshmi Narayan Madala
 *          Shweta Polepally
 * Date   :
 * Description : Initial coding done - Lakshmi Narayan Madala
 *               Added getBits       - Shweta.
 *               Moved all dec related functions from basic_types.h to here. - Shweta/Raja
 */

#include "basic_dec.h"

/**
* @param data The buffer holding the data
* @param curBitIndex The Index from where to get the bits
* @param numBits2Read Number of bits to be read from the buffer
* @details  This function gets the numBits2Read from the curBitIndex in the data
*/
UINT32 getBits (
  UINT8  *data,
  UINT32 curBitIndex,
  UINT32 numBits2Read
  )
{
  UINT8 lastByte, firstByte;
  UINT32 str      = 0;
  UINT32 bitsRead = 0;
  UINT32 bitsInFirstByte;
  UINT32 byteIndex;
  UINT32 startByte, endByte;
  UINT32 startBit, endBit;

  startByte = curBitIndex/8;
  startBit = curBitIndex%8;
  endByte = (curBitIndex + numBits2Read)/8;
  endBit = (curBitIndex + numBits2Read)%8;
  byteIndex = startByte;

  firstByte = 0;
  /* First byte */
  str = 0;

  firstByte |= ( *(data + byteIndex)) << startBit; /* remove bits in front */
  bitsInFirstByte = 8 - startBit;
  /* Is entire string present in this byte */
  if ((numBits2Read - bitsRead) <= bitsInFirstByte)
  {
    str = firstByte >> (8 - numBits2Read);
  }
  else /* the str spans multiple bytes */
  {
    str = firstByte >> (8 - bitsInFirstByte);
    bitsRead = bitsInFirstByte;
    byteIndex++; /* 2nd byte */
    while(byteIndex < endByte) /* complete octet belongs to the str */
    {
      str = str << 8;
      str |= (*(data + byteIndex));
      bitsRead +=8;
      byteIndex++;
    } /* you come out of this look in the last byte */
    lastByte = (*(data + byteIndex)) >> (8 - endBit);
    str = str << endBit | lastByte;
  }
  return str;
}

UINT8 getUint8FromDecBuf( DecBuf *bufP, UINT16 startIndex)
{
  (bufP)->index = 0;
  (bufP)->index = startIndex;
  return *((bufP)->value + (bufP)->index);
}

UINT16 getUint16FromDecBuf( DecBuf *bufP, UINT16 startIndex)
{
  (bufP)->index = 0;
  (bufP)->index = startIndex;
  return ( (*((bufP)->value + (bufP)->index)) << 8 | *((bufP)->value + (bufP)->index + 1) );
}


UINT32 getUint32FromDecBuf( DecBuf *bufP, UINT16 startIndex)
{
  (bufP)->index = 0;
  (bufP)->index = startIndex;
  return ( (*((bufP)->value + (bufP)->index)) << 24 |\
      *((bufP)->value + (bufP)->index + 1) << 16 |\
      *((bufP)->value + (bufP)->index + 2) << 8 |\
      *((bufP)->value + (bufP)->index + 3) );
}
