/** @file basetypes.h
 *
 * @brief Base types for C4K project
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef BASETYPES_H_
#define BASETYPES_H_

//
// Program ID
//
#define C4KFW                       "T4KFW: "

#ifndef __ENVDEPH__

#ifndef _U8_
#define _U8_
typedef unsigned char U8;
typedef volatile unsigned char V8;
#endif

#ifndef _U16_
#define _U16_
typedef unsigned short U16;
typedef volatile unsigned short V16;
#endif

#ifndef _U32_
#define _U32_
typedef unsigned int U32;
typedef volatile unsigned int V32;
#endif

#ifndef _UINT64_
#if defined (_MSC_VER)
#define _UINT64_
typedef unsigned __int64 U64;
#elif defined (__GNUC__)
#define _UINT64_
typedef unsigned long long U64;
#endif
#endif /* _UINT64_ */

#ifndef _S8_
#define _S8_
typedef signed char S8;
#endif

#ifndef _S16_
#define _S16_
typedef signed short S16;
#endif

#ifndef _S32_
#define _S32_
typedef signed int S32;
#endif

#ifndef _PTR_
#define _PTR_
typedef void* PTR;
#endif

#endif /* __ENVDEPH__ */

typedef U8* PBYTE;
#ifndef LOWERARM
#ifndef _HANDLE_
#define _HANDLE_ 
typedef void* HANDLE;
#endif
#endif /* LOWERARM */

typedef U8 BOOLEAN;

#ifdef _MSC_VER
#if _MSC_VER <= 1200
typedef unsigned long uintptr_t;
#endif
#endif /* _MSC_VER */

#ifndef _MSC_VER
#ifndef VOID
#define VOID void
#endif
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL 0
#endif

//
// Inline function support
//
#ifndef LOWERARM
#if defined (__CC_ARM)
#define inline                  __inline
#define INLINE                  __inline
#elif defined (_MSC_VER)
#define INLINE                  _inline
#elif defined (__GNUC__)
#define INLINE                  inline
#endif /* __CC_ARM */
#endif /* !LOWERARM */

//
// DLL or SO support
//
#if defined (USE_WINDOWS) || defined (WIN32)
#ifdef _USRDLL
#define EXPORT                  __declspec(dllexport)
#else
#define EXPORT
// __declspec(dllimport)
#endif
#else
#define EXPORT
#endif /* USE_WINDOWS */

//
// Round up to number of bits (U16, U32, U64)
//
#define RoundUp16(x)            (((x)+1)&(~1))
#define RoundUp32(x)            (((x)+3)&(~3))
#define RoundUp64(x)            (((x)+7)&(~7))

#define RoundUp(x,size)         (((x)+(size-1))&(~(size-1)))

#endif /* BASETYPES_H_ */
