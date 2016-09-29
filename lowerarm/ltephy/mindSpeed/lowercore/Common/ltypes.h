/** @file types.h
 *
 * @brief This File defines the variable types to be used
 *        throughout the target device.
 *        Compile flags allow the target device to be changed.
 *        Possible targets are MSVC, ARM11, ARM_CORTEX_A9
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTYPES_H
#define LTYPES_H

#ifndef BASETYPES_H_

#include <stdio.h>

#ifndef _BOOL_
#define _BOOL_
typedef unsigned char BOOL;
#endif

#ifndef _C8_
#define _C8_
typedef const char C8;
#endif

#ifndef _U8_
#define _U8_
typedef unsigned char U8;
#endif

#ifndef _U16_
#define _U16_
typedef unsigned short U16;
#endif

#ifndef _U32_
#define _U32_
typedef unsigned long U32;
#endif
#ifdef ARM_TARGET
typedef unsigned long long U64;

#else
typedef unsigned __int64 U64;
#endif
typedef volatile unsigned char V8;
typedef volatile unsigned short V16;
typedef volatile unsigned long V32;

typedef signed char S8;
typedef signed short S16;
typedef signed long S32;
#ifdef ARM_TARGET
typedef signed long long int S64;
#else
typedef __int64 S64;
#endif

typedef void VOID;
typedef void *PVOID;

#endif /* BASETYPES_H_ */

#endif /*TYPES_H*/
