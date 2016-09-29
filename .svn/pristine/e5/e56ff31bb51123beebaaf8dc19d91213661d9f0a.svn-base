/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                   *
*    No part of this source code may be reproduced, transmitted, transcribed   *
*    or translated into any language without the written permission of         *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this  *
*    permission write to the attention of Legal counsel, Polaris Networks,     *
*    75 Robbins Road, MA 02421 United States.                                  *
*******************************************************************************/

#ifndef __PN_MEMORY_DEBUG_H__
#define __PN_MEMORY_DEBUG_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#define PN_MEMORY_DEBUG_API __declspec( dllexport )
#else
#define PN_MEMORY_DEBUG_API
#endif

#ifdef __cplusplus
extern "C" {
#endif 

#include "PW_PALProtocolStackShared.h"

PN_MEMORY_DEBUG_API
void* UTILMalloc(size_t uiSize);

PN_MEMORY_DEBUG_API
void* UTILRealloc(void* pvAllocatedMemory, size_t uiSize);

PN_MEMORY_DEBUG_API
void* UTILCalloc(size_t uiNumBlocks, size_t uiSize);

PN_MEMORY_DEBUG_API
char* UTILStrdup(const char* kpcStr);

PN_MEMORY_DEBUG_API
void  UTILFreeMem(void* pvAllocatedMemory);

PN_MEMORY_DEBUG_API
void UTILMemLeakDump();

PN_MEMORY_DEBUG_API
void UTILFreeAllMemoryAndCloseDb();

PN_MEMORY_DEBUG_API
void* UTILMemcpy(void* pvDst, const void* kpvSrc, size_t iSize);

PN_MEMORY_DEBUG_API
void* UTILMemset(void* pvDst, int iChar, size_t uiSize);

PN_MEMORY_DEBUG_API
int   UTILMemcmp(const void* kpvStr1, const void* kpvStr2, size_t uiSize);

PN_MEMORY_DEBUG_API
unsigned short UTILNtoHS(unsigned short usiNum);

PN_MEMORY_DEBUG_API
unsigned short UTILHtoNS(unsigned short usiNum);

PN_MEMORY_DEBUG_API
ubyte4  UTILNtoHL(ubyte4 uliNum);

PN_MEMORY_DEBUG_API
ubyte4  UTILHtoNL(ubyte4 uliNum);

PN_MEMORY_DEBUG_API
void UTILPrint(const char* kpcFmt, ...);

PN_MEMORY_DEBUG_API
void UTILPrintHexDump(const unsigned char* kpucData, int iLength);

PN_MEMORY_DEBUG_API
void UTILStrcpy(void* pvDst, const void* kpvSrc);

PN_MEMORY_DEBUG_API
size_t UTILStrlen(void* kpvSrc);

PN_MEMORY_DEBUG_API
int UTILStrcmp(const void* kpvStr1, const void* kpvStr2);

PN_MEMORY_DEBUG_API
void UTILEncodeByte( unsigned char *buf, int *pos, unsigned char c );

PN_MEMORY_DEBUG_API
unsigned char UTILDecodeByte( const unsigned char *buf, int *pos );

PN_MEMORY_DEBUG_API
void UTILEncodeByteString( unsigned char *buf, int *pos, unsigned char *src, int len );

PN_MEMORY_DEBUG_API
void UTILDecodeByteString( const unsigned char *buf, int *pos, unsigned char *dst, int len );

PN_MEMORY_DEBUG_API
void UTILEncodeHtoNShort( unsigned char *buf, int *pos, short s );

PN_MEMORY_DEBUG_API
short UTILDecodeNtoHShort( const unsigned char *buf, int *pos );

PN_MEMORY_DEBUG_API
void UTILEncodeHtoNLong( unsigned char *buf, int *pos, byte4 l );

PN_MEMORY_DEBUG_API
byte4 UTILDecodeNtoHLong( const unsigned char *buf, int *pos );

PN_MEMORY_DEBUG_API
void UTILEncodeHtoNLongLong( unsigned char *buf, int *pos, byte8 ll );

PN_MEMORY_DEBUG_API
byte8 UTILDecodeNtoHLongLong( const unsigned char *buf, int *pos );

#define UTILFree(p) {UTILFreeMem(p); p = NULL;}

#ifdef __cplusplus
       } /* extern "C" */
#endif


#endif /* __PN_MEMORY_DEBUG_H__ */
