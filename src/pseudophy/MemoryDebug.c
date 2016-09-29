/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                   *
*    No part of this source code may be reproduced, transmitted, transcribed   *
*    or translated into any language without the written permission of         *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this  *
*    permission write to the attention of Legal counsel, Polaris Networks,     *
*    75 Robbins Road, MA 02421 United States.                                  *
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef WIN32
#include <winsock2.h>
#define PRSIZE "lu"
#else
#include <arpa/inet.h>
#define PRSIZE "Zd"
#endif

#include "Constants.h"
#include "MemoryDebug.h"
#include "Logger.h"

#ifdef USE_VISUAL_LEAK_DETECTOR
#include "vld.h"
#endif

#ifdef WIN32
#pragma warning( disable : 4996 )
#endif

/* Module Name needs to be set for Logging */
#define __sModuleName__ "SLIB"

#define DEFAULT_MEM_DUMP_FILE_NAME "polaris-lte-emulator.mem"

PN_MEMORY_DEBUG_API
void UTILPrintHexDump(const unsigned char* kpucData, int iLength)
{
    int i;
    for (i=0; i<iLength; i++) 
    {
        printf("Data = %02x%c",kpucData[i], ((i+1)%16&&(i+1)!=iLength?' ': '\n'));
    }
}

void* UTILCalloc(size_t uiNumBlocks, size_t uiSize)
{
    void* pvAllocatedMemory = (void*)calloc(uiNumBlocks, uiSize);

    if (pvAllocatedMemory == NULL) 
    {
        return NULL;
    }
    return pvAllocatedMemory;
}

void* UTILMalloc(size_t uiSize)
{
    void* pvAllocatedMemory = (void*)malloc(uiSize);

    if (pvAllocatedMemory == NULL) 
    {
        return NULL;
    }
    UTILMemset(pvAllocatedMemory, '\0', uiSize);
    return pvAllocatedMemory;
}

void* UTILRealloc(void* pvAllocatedMemory, size_t uiSize)
{
    /* Allocate new memory */
    pvAllocatedMemory = (void*)realloc(pvAllocatedMemory, uiSize);

    if (pvAllocatedMemory == NULL)
    {
        return NULL;
    }
    return pvAllocatedMemory;
}

char* UTILStrdup(const char* kpcStr)
{
    char *pcDup = NULL;
    unsigned int nLen = 0;
    if (kpcStr != NULL)
    {
        nLen = (unsigned int)strlen(kpcStr);
        if (nLen > 0)
        {

            pcDup = UTILMalloc(nLen + 1);

            if(pcDup)
            {
                strcpy(pcDup, kpcStr);
            }
        }
    }

    return pcDup;
}

void UTILFreeMem(void* pvAllocatedMemory)
{
    if (pvAllocatedMemory != NULL)
    {
       free(pvAllocatedMemory);
       pvAllocatedMemory = NULL;
    }
}

void UTILMemLeakDump()
{
}

void UTILFreeAllMemoryAndCloseDb()
{
}

PN_MEMORY_DEBUG_API
void* UTILMemcpy(void* pvDst, const void* kpvSrc, size_t uiSize)
{
    return (memcpy(pvDst, kpvSrc, uiSize));

}

PN_MEMORY_DEBUG_API
void* UTILMemset(void* pvDst, int iChar, size_t uiSize)
{
#ifdef DEBUG
    printf("MEMORY_INFO: SET MEMORY DST = %p, SIZE = %d\n", pvDst, uiSize);
#endif
    return (memset(pvDst, iChar, uiSize));

}

PN_MEMORY_DEBUG_API
int UTILMemcmp(const void* kpvStr1, const void* kpvStr2, size_t uiSize)
{
    return (memcmp(kpvStr1, kpvStr2, uiSize));
}

unsigned short  UTILNtoHS(unsigned short usiNum)
{
    return (ntohs(usiNum));
}

PN_MEMORY_DEBUG_API
unsigned short UTILHtoNS(unsigned short usiNum)
{
    return (htons(usiNum));
}

PN_MEMORY_DEBUG_API
ubyte4 UTILNtoHL(ubyte4 uliNum)
{
    return (REVERSE_4_BYTES_INT(uliNum));
}

PN_MEMORY_DEBUG_API
ubyte4 UTILHtoNL(ubyte4 uliNum)
{
    return (REVERSE_4_BYTES_INT(uliNum));
}

PN_MEMORY_DEBUG_API
void UTILPrint(const char* kpcFmt, ...)
{
    va_list argp;
    char arr_cBuff[1000];
    memset(arr_cBuff, '\0', 1000);
    va_start(argp, kpcFmt);
    vsprintf(arr_cBuff, kpcFmt, argp);
    va_end(argp);
}

PN_MEMORY_DEBUG_API
void UTILStrcpy(void* pvDst, const void* kpvSrc)
{
    if(pvDst != NULL && kpvSrc != NULL)
        strcpy(pvDst, kpvSrc);
}

PN_MEMORY_DEBUG_API
size_t UTILStrlen(void* kpvSrc)
{
    if(kpvSrc != NULL)
		return strlen(kpvSrc);
	else
		return 0;
}

PN_MEMORY_DEBUG_API
int UTILStrcmp(const void* kpvStr1, const void* kpvStr2)
{
    if(kpvStr1 != NULL && kpvStr2 != NULL)
        return strcmp(kpvStr1, kpvStr2);
    else
		return -1;
}

PN_MEMORY_DEBUG_API
void UTILEncodeByte( unsigned char *buf, int *pos, unsigned char c )
{
   buf[*pos] = c;
   (*pos) = (*pos) + 1;
}

PN_MEMORY_DEBUG_API
unsigned char UTILDecodeByte( const unsigned char *buf, int *pos )
{
   return buf[(*pos)++];
}

PN_MEMORY_DEBUG_API
void UTILEncodeByteString( unsigned char *buf, int *pos, unsigned char *src, int len )
{
   memcpy( buf + *pos, src, len);
   *pos += len;
}

PN_MEMORY_DEBUG_API
void UTILDecodeByteString( const unsigned char *buf, int *pos, unsigned char *dst, int len )
{
   memcpy( dst, buf + *pos, len);
   *pos += len;
}

PN_MEMORY_DEBUG_API
void UTILEncodeHtoNShort( unsigned char *buf, int *pos, short s )
{
   short ns = htons(s);
   memcpy( buf + *pos, (unsigned char *)&ns, sizeof(short) );
   *pos += sizeof(short);
}

PN_MEMORY_DEBUG_API
short UTILDecodeNtoHShort( const unsigned char *buf, int *pos )
{
   short ns;
   memcpy( (unsigned char *)&ns, buf + *pos, sizeof(short) );
   *pos += sizeof(short);
   return ntohs(ns);
}

PN_MEMORY_DEBUG_API
void UTILEncodeHtoNLong( unsigned char *buf, int *pos, byte4 l )
{
   byte4 nl = REVERSE_4_BYTES_INT(l);
   memcpy( buf + *pos, (unsigned char *)&nl, sizeof(byte4) );
   *pos += sizeof(byte4);
}

PN_MEMORY_DEBUG_API
byte4 UTILDecodeNtoHLong( const unsigned char *buf, int *pos )
{
   byte4 nl;
   memcpy( (unsigned char *)&nl, buf + *pos, sizeof(byte4) );
   *pos += sizeof(byte4);
   return REVERSE_4_BYTES_INT(nl);
}

PN_MEMORY_DEBUG_API
void UTILEncodeHtoNLongLong( unsigned char *buf, int *pos, byte8 ll )
{
   byte4 nl = ( byte4 )(ll >> 32);
   nl = REVERSE_4_BYTES_INT(nl);
   memcpy( buf + *pos, (unsigned char *)&nl, sizeof(byte4) );
   *pos += sizeof(byte4);
   nl = ( byte4 )(ll & 0xFFFFFFFF);
   nl = REVERSE_4_BYTES_INT(nl);
   memcpy( buf + *pos, (unsigned char *)&nl, sizeof(byte4) );
   *pos += sizeof(byte4);
}

PN_MEMORY_DEBUG_API
byte8 UTILDecodeNtoHLongLong( const unsigned char *buf, int *pos )
{
   byte4 nl;
   byte8 nll;
   memcpy( (unsigned char *)&nl, buf + *pos, sizeof(byte4) );
   *pos += sizeof(byte4);
   nll = (byte8)ntohl(nl);
   memcpy( (unsigned char *)&nl, buf + *pos, sizeof(byte4) );
   *pos += sizeof(byte4);
   nll = (byte8 )( (nll << 32) | ( REVERSE_4_BYTES_INT(nl) ) );
   return nll;
}

