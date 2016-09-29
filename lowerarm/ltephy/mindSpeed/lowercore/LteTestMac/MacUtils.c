
#include "resultcodes.h"
#include "MacUtils.h"

/****************************************************************************
Private Definitions
****************************************************************************/
//#define _USE_REV_INTRINSIC


U8 mac_utils_bit_swp_table[] = {
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 
    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 
    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 
    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 
    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 
    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 
    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 
    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 
    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 
    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 
    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff};

U8 mac_utils_bit_swp(U8 val)
{
    return (mac_utils_bit_swp_table[val]);
}


U32 SWAP(U32 w32)
{

#ifdef _USE_REV_INTRINSIC
    w32 = __rev(w32);
#else
     // convert BE<->LE
     w32 =  ((w32 & 0x000000ff) << 24) +
            ((w32 & 0x0000ff00) << 8)  +
            ((w32 & 0x00ff0000) >> 8)  +
            ((w32 & 0xff000000) >> 24);
#endif
    return w32;
}

void SWAP64(U32 *w32)
{
    w32[0] = SWAP((U32)w32[0]);
    w32[1] = SWAP((U32)w32[1]);

    return;
}

void SWAP64_2params(U32 *i32, U32* o32)
{
    o32[0] = SWAP((U32)i32[0]);
    o32[1] = SWAP((U32)i32[1]);

    return;
}



U8 Log2(U32 value)
{
    U8 num_bits = 1;
    if(value)
        while ( (U32)((1 << num_bits) - 1) < (U32)(value - 1) ) ++num_bits;
    return num_bits;
}

U8 format_padding (U8 value, U8 *pambiguous_sizes, U8 length)
{
    U8        padding;
    U8        i;
    do{

        padding = 0;
        for (i = 0; i < length; i++){
            if (value == pambiguous_sizes[i]){
                padding = 1;
                break;
            }    
        }

        if (padding){
            value += 1;
        }
        
    } while (padding);

    return (value);

}

U8 MaxRBs2RBAFieldSize_F0(U32 max_rbs)
{
     U32 max_value = max_rbs * (max_rbs + 1) / 2;
     return Log2(max_value);
}

void decode_riv(U8 n_rbs, U16 riv, U8 *rb_start, U8 *L_crbs)
{
    U8    l_crbs, rb_start1;

    l_crbs = (riv) / n_rbs + 1;
    rb_start1 = riv % n_rbs;

//    if (((n_rbs*(l_crbs-1)+rb_start)!=riv) || ((l_crbs -1) > (n_rbs>>1)) || ((l_crbs + rb_start) > (n_rbs)))
    if ( (l_crbs + rb_start1) > (n_rbs) )
    {
         l_crbs = n_rbs - l_crbs + 2;
         rb_start1 = n_rbs - rb_start1 - 1;
    }

    *rb_start = rb_start1;
    *L_crbs = l_crbs;

    return;
}

void put_bits(U32 *w32, 
              U32 value,
              U8  *pos,
              U8  size)
{
    U8 start_offset = *pos;
    if ((start_offset-size+1)>=32)
    {
        start_offset -=32;
        *w32 = (*w32 & ~(((1<<size)-1)<<(start_offset-size +1 ))) | ((value << (start_offset-size+1)) & ((1<<size)-1)<<(start_offset-size +1));
    }
    else
    {
        // need to put into second dw
        if (start_offset < 32)
        {
            w32++;
            *w32 = (*w32 & (~((1<<size)-1)<<(start_offset-size+1))) | ((value << (start_offset-size+1)) & ((1<<size)-1)<<(start_offset-size+1));
        }
        // need to put into diferent dw
        else
        {
            *w32 = (*w32 & (~((1<<(start_offset-32+1))-1))) | ((value >> (size-start_offset+32-1)) & ((1<<(start_offset-32+1))-1));

            w32++;
//            *w32 = (*w32 & (~(((1<<(size-start_offset+32-1))-1))<<(start_offset-size))) | (value << (start_offset-size+1));
            *w32 = (*w32 & (((1<<(start_offset-size+1)) -1 ))) | ((value << (start_offset-size+1)) & (~((1 << (start_offset-size+1)) - 1)));

        }
    }

    *pos = *pos - size;
    return;
}


U32 extract_bits(U32 *w32,
                 U8  *pos,
                 U8  size)
{
    U32 out, out1;
    U8 start_offset = *pos;

    if ((start_offset-size+1)>=32)
    {
        start_offset -=32;
        out = (*w32>>(start_offset-size+1)) & ((1<<size)-1);
    }
    else
    {
        out = 0;
        if (start_offset < 32)
        {
            w32++;
            out = (*w32>>(start_offset-size+1)) & ((1<<size)-1);        
        }
        else
        {
            out1 = (*w32<<(size-(start_offset-32)-1));
            w32++;
            out  = (*w32>>(start_offset-size+1));// & ((1<<(start_offset-32)-1));
            out  |= out1;
            out  &= ((1<<size)-1);
        }
    }

    *pos = *pos - size;

    return out;
}
