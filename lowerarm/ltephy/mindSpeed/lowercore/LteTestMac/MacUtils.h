#ifndef _MAC_UTILS_H
#define _MAC_UTILS_H


//#include "enbMacCore.h"

U8 mac_utils_bit_swp(U8 val);

U8 Log2(U32 value);

U8 MaxRBs2RBAFieldSize_F0(U32 max_rbs);

void decode_riv(U8 n_rbs, U16 riv, U8 *RB_start, U8 *L_crbs);

U32 extract_bits(U32 *w32,
                 U8  *pos,
                 U8  size);

void put_bits(U32 *w32, 
              U32 value,
              U8  *pos,
              U8  size);

U32 SWAP(U32 w32);

void SWAP64(U32 *w32);
void SWAP64_2params(U32 *i32, U32* o32);

U8 format_padding (U8 value, U8 *pambiguous_sizes, U8 length);

#endif //#ifndef _MAC_UTILS_H
