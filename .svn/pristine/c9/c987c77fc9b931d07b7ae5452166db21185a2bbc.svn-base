/** @file tlvmsg.h
 *
 * @brief TLV support (network byte order)
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef TLVMSG_H_
#define TLVMSG_H_

#include "basetypes.h"

typedef U8* PTLVSTREAM;

//
// Read with automatic source pointer increment (sequential read)
//
U8 TlvReadU8Inc(PTLVSTREAM *pStream);
U16 TlvReadU16Inc(PTLVSTREAM *pStream);
U32 TlvReadU32Inc(PTLVSTREAM *pStream);

//
// Read without source pointer modification
//
U8 TlvReadU8(PTLVSTREAM pStream);
U16 TlvReadU16(PTLVSTREAM pStream);
U32 TlvReadU32(PTLVSTREAM pStream);

PTLVSTREAM TlvWriteU8(PTLVSTREAM stream, U8 data);
PTLVSTREAM TlvWriteU16(PTLVSTREAM stream, U16 data);
PTLVSTREAM TlvWriteU32(PTLVSTREAM stream, U32 data);
PTLVSTREAM TlvWriteTlv(PTLVSTREAM stream, int tag, int paramCount, ...);
// Insert nested TLV into parent TLV
PTLVSTREAM TlvAppendTlv(PTLVSTREAM parent, int tag, int paramCount, ...);
// Insert nested TLV into parent TLV with any nested level
PTLVSTREAM TlvNestedTlv(int tag, int paramCount, ...);

U16 TlvReadLen(PTLVSTREAM stream);
PTLVSTREAM TlvWriteLen(PTLVSTREAM stream, U16 len);

#endif /* TLVMSG_H_ */
