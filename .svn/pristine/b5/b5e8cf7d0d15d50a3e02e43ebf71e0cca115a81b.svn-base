/** @file basetypes.h
 *
 * @brief Base types for C4K project
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
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

#if !defined(FAILURE)
#define FAILURE                     1
#endif

typedef unsigned char U8;
typedef unsigned short U16;
#ifndef _U32_
#define _U32_
typedef unsigned int U32;
#endif

#ifdef WIN32
typedef unsigned __int64 U64;
#else
#ifdef FDX_RT
typedef unsigned long long U64;
#endif /*FDX_RT*/
#endif /* WIN32*/

//#ifdef USE_LINUX
//typedef unsigned long long U64;
//#endif /* USE_LINUX */

typedef volatile unsigned char V8;
typedef volatile unsigned short V16;
typedef volatile unsigned int V32;

#ifndef __ENVDEPH__
#ifdef FDX_RT
typedef signed char S8;
#endif /*FDX_RT*/
typedef signed short S16;
#ifndef _S32_
#define _S32_
typedef signed int S32;
#endif

typedef void* PTR;
typedef U8* PBYTE;
#ifndef LOWERARM
typedef void* HANDLE;
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

// Standard function return types
#if !defined(RESULTCODE)
typedef U32 RESULTCODE;
#endif

#if !defined(RETURNVALUE)
typedef U32 RETURNVALUE;
#endif

//
// Round up to number of bits (U16, U32, U64)
//
#define RoundUp16(x)            (((x)+1)&(~1))
#define RoundUp32(x)            (((x)+3)&(~3))
#define RoundUp64(x)            (((x)+7)&(~7))

#define RoundUp(x,size)         (((x)+(size-1))&(~(size-1)))

#endif /* __ENVDEPH__ */
#endif /* BASETYPES_H_ */
