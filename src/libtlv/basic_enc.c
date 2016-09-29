/**
 * @file    basic_enc.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains the functions definitions for encoding data to TLV
 *
 * @author : Lakshmi Narayan Madala
 * Date   :
 * Description :
 *
 */

#include "basic_enc.h"

/**
 * @param index  The index to place the value
 * @param value  The value to be placed in the buffer
 * @param encBuf The buffer of type EncBuf where encoded data is placed
 * @details This function fills a UINT16 in Network Byte order at the given index in the Buffer
 */
inline void putUint16AtIndex(
  UINT16       startIndex,
  UINT16       value,
  IO EncBuf    *encBuf
  )
{
  encBuf->value[ startIndex ]     = (UINT8) ( value >> 8 );
  encBuf->value[ startIndex + 1 ] = (UINT8) ( value & 0xFF );
}

/**
 * @param value  The 32bit integer value to be placed in the buffer
 * @param encBuf The buffer of type EncBuf where encoded data is placed
 * @details This function converts the given UINT32 from the Host Byte order to Network Byte order
 *          Fills the data at the given index in the Buffer, Increments the length of Buffer by FOUR
 * @result encBuf encBuffer holding the network byte order of data
 */
inline void putUint32(
  UINT32       value,
  IO EncBuf    *encBuf
  )
{
  putUint8( (UINT8) ( value >> 24 ), encBuf);
  putUint8( (UINT8) ( ( value >> 16 ) & 0xFF), encBuf);
  putUint8( (UINT8) ( ( value >> 8 ) & 0xFF), encBuf);
  putUint8( (UINT8) ( ( value) & 0xFF), encBuf);
}

/**
 * @param value  The 16 bit integer value to be placed in the buffer
 * @param encBuf The buffer of type EncBuf where encoded data is placed
 * @details This function adds UINT16 in Network Byte order at the end of Buffer
 */
inline void putUint16(
  UINT16       value,
  IO EncBuf    *encBuf
  )
{
  putUint8( (UINT8)( value >> 8 ), encBuf);
  putUint8( (UINT8)( value & 0xFF), encBuf);
}

/**
 * @param value  The 8bit value to be placed in the buffer
 * @param encBuf The buffer of type EncBuf where encoded data is placed
 * @details Adds one byte at the present location of index, Increments the encBuffer by One
 */
inline void putUint8(
  UINT8        value,
  IO EncBuf    *encBuf
  )
{
  /* +1 to check if actual expected number bytes can be encoded. here its 1 byte, hence +1 */
  if ((encBuf->len+1) <= MAX_SIZE_OF_ENCD_BUFF)
  {
    encBuf->value[ (encBuf->len) ] = value;
    encBuf->len++;
  }
  else
  {
    /* buffer size is exceeded/encoding truncated */
    BS_ERROR_ENC_FAILED_BUFFER_OVERFLOW( MAX_SIZE_OF_ENCD_BUFF );
  }
}

/**
 * @param src      The source for copying
 * @param numBytes The number of bytes to be copied to buffer
 * @param destBuf   The buffer of type EncBuf where encoded data is placed
 * @details    This function copies the "numBytesToCopy" of bytes from src to the end Buffer
 */
inline void putBytes(
  IN UINT8     *src,
  IO UINT16     numBytes,
  IO EncBuf    *destBuf
  )
{
  UINT8 *vp = ( destBuf->value + destBuf->len );
  /* +numBytes to check if actual expected number bytes can be encoded. */
  if ((destBuf->len + numBytes) <= MAX_SIZE_OF_ENCD_BUFF)
  {
    copyBytes( src, numBytes, vp );
    destBuf->len += numBytes;
  }
  else
  {
    /* buffer size is exceeded/encoding truncated */
    BS_ERROR_ENC_FAILED_BUFFER_OVERFLOW( MAX_SIZE_OF_ENCD_BUFF );
  }
}

/**
 * @param   bitPosToInsert The bit position in the buffer from where to start insertion
 * @param   numBits2Write Number of bits to write into buffer
 * @param   value Value to be written to buffer
 * @param   destBuf Buffer holding the encoded data,
 * @details This is a generic function to put required number of bits in a buffer
 */
void putBits(
  UINT16    bitPosToInsert,
  UINT8     numBits2Write,
  UINT16    value,
  UINT8    *destBuf
  )
{
  UINT8 bitsToShift;
  UINT8 startByte;
  UINT8 startBit;
  UINT8 byte;
  UINT16 bitsWritten;
  UINT16 bitsRemaining;
  UINT16 remBitInFirstByte;
  UINT16 byteIndex;
  UINT8 tempStr;

  UINT16 str = value;

  startByte =  bitPosToInsert/8;
  startBit  =  bitPosToInsert%8;
  byteIndex = startByte;
  bitsWritten = 0;
  bitsRemaining = numBits2Write;

  remBitInFirstByte = 8 - startBit;
  if(remBitInFirstByte >= bitsRemaining ) /* can fit in this byte */
  {
    byte = destBuf[byteIndex]; /* this is the byte we need to change */
    bitsToShift = 8 - (startBit + bitsRemaining);
    destBuf[byteIndex] = byte | (str << bitsToShift);
    return;
  }
  else /* It needs to span multiple bytes */
  {
     tempStr = str >> (bitsRemaining - remBitInFirstByte);
     destBuf[byteIndex] |= tempStr;
     bitsRemaining -= remBitInFirstByte;
     bitsWritten += remBitInFirstByte;
     byteIndex++;
     while(bitsRemaining > 0)
     {
      if(bitsRemaining >= 8) /* we can fill this byte fully */
      {
         destBuf[byteIndex] = ( (str >> ( numBits2Write - (bitsWritten + 8)) ) & 0xFFFF );
         bitsRemaining -= 8;
         bitsWritten += 8;
         byteIndex++;
      }
      else
      { /* last byte */
         byte = destBuf[byteIndex]; /* this is the byte we need to change */
         bitsToShift = ( SIZEINBITS(str) - bitsRemaining)%8;
         destBuf[byteIndex] = byte | (str << bitsToShift);
         bitsRemaining=0;
      }
     }
  }
}

/**
 * @param encBuf The buffer of type EncBuf where encoded data is placed
 * @details This function returns the index at which the next byte will be written in encBuf
 */
inline Length getBufferIndex(
  IN EncBuf   *encBuf
  )
{
  return ( encBuf->len );
}

/**
 * @param   encBuf The buffer of type EncBuf where encoded data is placed
 * @param   lenInfo This structure holds length field location and length of data before filling
 * @details This functions fills the L part of the encBuffer as 0 and stores the length field information
 *          lenInfo Holds the length filed location in encBuffer, present length of encBuffer
 * @see asncEncTag, asncEncLength
 */
inline void tlvReserveLen(
  IO EncBuf      *encBuf,
  IO LenInfo     *lenInfo
  )
{
  //Save the encBuffer pointer to the Index
  lenInfo->lenIndex = getBufferIndex( encBuf );

  tlvEncLen( 0, encBuf );
  //Save the present length of encBuffer
  lenInfo->bytesAtStart = getBufferIndex( encBuf );
}

/**
 * @param lenInfo The structure holding the length field location, size of encBuffer before encoding
 * @param encBuf The encoded encBuffer where len field is updated
 * @details This function fills the length of encoded TLV in the encBuffer
 */
void tlvSetLengthField(
  IN LenInfo     *lenInfo,
  IO EncBuf      *encBuf
  )
{
  Length bytesEnc;
  bytesEnc = getBufferIndex(encBuf ) - (lenInfo->bytesAtStart);
  putUint16AtIndex( lenInfo->lenIndex, bytesEnc, encBuf );
}

/**
 * @param  tag The Tag value of TLV
 * @param  value The field that is to be encoded of type UINT8.
 * @param  encBuf Buffer holding the encoded data, Encoded data is added to the end of buffer
 * @details This is a generic function to encode all the TLV whose length is of ONE byte
 */
void tlvEncUint8(
  IN UINT16   tag,
  IN UINT8    value,
  IO EncBuf  *encBuf
  )
{
  tlvEncTag( tag, encBuf );
  tlvEncLen( 1, encBuf );
  putUint8( value, encBuf );
}

/**
 * @param   tag    The Tag value of TLV
 * @param   value  The field that is to be encoded of type UINT16.
 * @param   encBuf Buffer holding the encoded data, Encoded data is added to the end of buffer
 * @details This is a generic function to encode all the TLV whose length is of TWO bytes
 */
void tlvEncUint16(
  IN UINT16    tag,
  IN UINT16    value,
  IO EncBuf   *encBuf
  )
{
  tlvEncTag( tag, encBuf );
  tlvEncLen( 2, encBuf );
  putUint16( value, encBuf );
}

/**
 * @param   tag    The Tag value of TLV
 * @param   value  The field that is to be encoded of type UINT32.
 * @param   encBuf Buffer holding the encoded data, Encoded data is added to the end of buffer
 * @details This is a generic function to encode all the TLV whose length is of FOUR bytes
 */
void tlvEncUint32(
  IN UINT16    tag,
  IN UINT32    value,
  IO EncBuf   *encBuf
  )
{
  tlvEncTag( tag, encBuf );
  tlvEncLen( 4, encBuf );
  putUint32( value, encBuf );
}

/**
 * @param   tag    The Tag value of TLV
 * @param   len    Length of buffer for copy
 * @param   value  Buffer of type UINT8
 * @param   encBuf Buffer holding the encoded data, Encoded data is added to the end of buffer
 * @details This is a generic function to encode all the TLV whose length is of FOUR bytes
 */
void tlvEncBuf(
  IN UINT16    tag,
  IN UINT16    len,
  IN UINT8    *value,
  IO EncBuf   *encBuf
  )
{
  tlvEncTag( tag, encBuf );
  tlvEncLen( len, encBuf );
  putBytes( value, len, encBuf );
}

/* TODO Remove this code from IMCS Library
 * These functions are of used if aricent is to be used*/

void  encAriBuffHdr(
    IO EncBuf   *encBuf,
    IN UINT16   transId,
    IN UINT16   srcModId,
    IN UINT16   dstModId
    )
{
  putUint16( transId, encBuf);    /* Transaction Identifier */
  putUint16( srcModId, encBuf);          /* Source Module Id RRC */
  putUint16( dstModId, encBuf);          /* Dest Module Id PDCP */
}

void encSetAriTlvLen(
    IN LenInfo     *lenInfo,
    IO EncBuf      *encBuf
    )
{
  /* As per Aricent API,
   * Length of TLV = ( TAG field Length + Length field length + Value field Length )
   */
  Length bytesEnc;
  bytesEnc = getBufferIndex(encBuf ) - (lenInfo->bytesAtStart) + sizeof(UINT16) + sizeof(UINT16);
  putUint16AtIndex( lenInfo->lenIndex, bytesEnc, encBuf );
}

#define LEN_OF_ARI_BUF_HDR              10
void encSetAriMsgLen(
    IN LenInfo     *lenInfo,
    IO EncBuf      *encBuf
    )
{
  /* As per Aricent API, Length of Msg = (Len of Buff Header + TLV Lengths) */
  Length bytesEnc;
  bytesEnc = getBufferIndex(encBuf ) - (lenInfo->bytesAtStart) + LEN_OF_ARI_BUF_HDR;
  putUint16AtIndex( lenInfo->lenIndex, bytesEnc, encBuf );
}

