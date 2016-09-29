/**
 * @file    basic_enc.h (inter module communication)
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains the functions definitions for encoding data to TLV
 *
 * @author : Lakshmi Narayan Madala
 * Date   :
 * Description :
 */
#ifndef FRM_ENC_H
#define FRM_ENC_H

#include "cmn_types.h"
#include "bs_config.h"
#include "imc_err.h"

/* Macros */
#define POSITION_OF_LEN_FIELD_IN_MSG        2
#define MAX_SIZE_OF_ENCD_BUFF     SIZE_RECV_DATA

#define putUint32InArray( value, data )   \
  data[0] =  value >> 24;      \
  data[1] = ( value >> 16 ) & 0xFF;  \
  data[2] = ( value >> 8 ) & 0xFF;  \
  data[3] = ( value ) & 0xFF;   \

#define putUint16InArray( value, data )   \
  data[0] = ( value >> 8 ) & 0xFF;  \
  data[1] = ( value ) & 0xFF;   \

#define putUint8InArray( value, data )   \
  data[0] = ( value ) & 0xFF;   \

#define putBytesInArray( src, len, dest ) \
             copyBytes( src, len, dest );
/* Structures */

/* The structure of Encoded buffer
 * every encoding function accepting buffer as INOUT parameter updates
 * both len field and the value field. */
typedef struct s_EncBuf
{
  Length len;
  UINT8 value[MAX_SIZE_OF_ENCD_BUFF];
} EncBuf;

typedef struct s_LenInfo
{
  UINT16 bytesAtStart;
  UINT16 lenIndex;
} LenInfo;

/* Functions */
#define initEncBuf(encBuf) (encBuf)->len=0

#define setTlvMsgLen( buf ) putUint16AtIndex( POSITION_OF_LEN_FIELD_IN_MSG, (buf)->len, buf )

/**
* @def tlvEncTag( type, encBuf )
* @brief Encode the Tag Field
*/
#define tlvEncTag( type, encBuf ) putUint16( type, encBuf )

/**
* @def tlvEncLen( length, encBuf )
* @brief Encodes the Length field
*/
#define tlvEncLen( length, encBuf ) putUint16( length, encBuf )

inline Length getBufferIndex(
    IN EncBuf   *encBuf
    );
inline void tlvReserveLen(
    IO EncBuf      *encBuf,
    IO LenInfo     *lenInfo
    );
void tlvSetLengthField(
    IN LenInfo     *lenInfo,
    IO EncBuf      *encBuf
    );
inline void putUint16AtIndex(
    UINT16       index,
    UINT16       value,
    IO EncBuf    *encBuf
    );
inline void putUint32(
    UINT32       value,
    IO EncBuf    *encBuf
    );
inline void putUint16(
    UINT16       value,
    IO EncBuf    *encBuf
    );
inline void putUint8(
    UINT8        value,
    IO EncBuf    *encBuf
    );
inline void putBytes(
    IN UINT8     *src,
    IO UINT16     numBytes,
    IO EncBuf    *destBuf
    );
void tlvEncUint8(
    IN UINT16   tag,
    IN UINT8    value,
    IO EncBuf  *encBuf
    );
void tlvEncUint16(
    IN UINT16   tag,
    IN UINT16   value,
    IO EncBuf   *encBuf
    );
void tlvEncUint32(
    IN UINT16    tag,
    IN UINT32    value,
    IO EncBuf   *encBuf
    );
void tlvEncBuf(
    IN UINT16    tag,
    IN UINT16    len,
    IN UINT8    *value,
    IO EncBuf   *encBuf
    );

#define SIZEINBITS(A)    ( sizeof(A) * 8)
void putBits(
    UINT16    bitPosToInsert,
    UINT8     numBits2Write,
    UINT16    value,
    UINT8    *destBuf
    );
void encSetAriMsgLen(
    IN LenInfo     *lenInfo,
    IO EncBuf      *encBuf
    );
void encSetAriTlvLen(
    IN LenInfo     *lenInfo,
    IO EncBuf      *encBuf
    );
void  encAriBuffHdr(
    IO EncBuf   *encBuf,
    IN UINT16   transId,
    IN UINT16   srcModId,
    IN UINT16   dstModId
    );

#endif // FRM_ENC_H
