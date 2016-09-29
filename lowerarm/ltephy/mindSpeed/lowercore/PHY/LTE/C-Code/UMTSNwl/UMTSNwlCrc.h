#ifndef _CELLSEARCH_CRC_
#define _CELLSEARCH_CRC_

#include "UMTSNwlCommMacro.h"

#if 0
U32 crc16_Calc(U8 *pIn, U32 usLen);
void CRC16( U8* pDataIn, U32 iLenIn, U32* pCRCOut) ; 
void bitverse(U32 *ulIn, U32 *ulOut);
#endif

U32 ProGetCRCOfTBBlock(U8 *pDataIn, U16 sTBSize, U8 byCRCLen);
U32 ConjBitToByte(U8 * pbitIn, U8 *pByteOut, U32 wBitLen);


U32 cal_crc16(U8 *ptr, U8 len);
U32 Bit_Calc_crc16(U8 *ptr,  U32 dLen);


#endif
