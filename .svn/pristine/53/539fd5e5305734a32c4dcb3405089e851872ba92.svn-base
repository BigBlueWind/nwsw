//-------------------------------------------------------------------------------------------
/** @file typedef.h
 *
 * @brief Type Definitions Header File
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

//-------------------------------------------------------------------------------------------

//*****************************************************************************
//**********          BASIC NUMERICAL DATA TYPE DEFINITIONS          **********
//*****************************************************************************

// Version: 3.4             Revision date: march 24, 2009

//*****************************************************************************

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

// TODO: Move all base definitions to a single location
#ifndef BASETYPES_H_

#ifndef __ENVDEPH__

//***** Define unsigned 8-bit byte type *****
#ifndef _U8_
#define _U8_
   /** @brief Data Type: Unsigned 8-bits
   *
   *         <b>Data Prototype:</b> <BR/>
   *         typedef unsigned char   U8;
   *
   *         <b>Description:</b><br/>
   *         Unsigned 8-bits
   *
   */
   typedef unsigned char	    U8;
   typedef volatile unsigned char   V8;
#endif

//***** Define signed 8-bit byte type *****
#ifndef _S8_
#define _S8_
   /** @brief Data Type: Signed 8-bits
   *
   *         <b>Data Prototype:</b> <BR/>
   *         typedef signed char   S8;
   *
   *         <b>Description:</b><br/>
   *         Signed 8-bits
   *
   */
    typedef signed char         S8;
#endif

//***** Define unsigned 16-bit word type *****
#ifndef _U16_
#define _U16_
   /** @brief Data Type: Unsigned 16-bits
   *
   *         <b>Data Prototype:</b> <BR/>
   *         typedef unsigned short int   U16;
   *
   *         <b>Description:</b><br/>
   *         Unsigned 16-bits
   *
   */
    typedef unsigned short int  U16;
    typedef volatile unsigned short int V16;
#endif

//***** Define signed 16-bit word type *****
#ifndef _S16_
#define _S16_
   /** @brief Data Type: Signed 16-bits
   *
   *         <b>Data Prototype:</b> <BR/>
   *         typedef signed short int   S16;
   *
   *         <b>Description:</b><br/>
   *         Signed 16-bits
   *
   */
    typedef signed short int    S16;
#endif

//***** Define unsigned 32-bit word type *****
#ifndef _U32_
#define _U32_
   /** @brief Data Type: Unsigned 32-bits
   *
   *         <b>Data Prototype:</b> <BR/>
   *         typedef unsigned long int   U32;
   *
   *         <b>Description:</b><br/>
   *         Unsigned 32-bits
   *
   */
    typedef unsigned long int   U32;
    typedef volatile unsigned long int V32;
#endif

//***** Define signed 32-bit word type *****
#ifndef _S32_
#define _S32_
   /** @brief Data Type: Signed 32-bits
   *
   *         <b>Data Prototype:</b> <BR/>
   *         typedef signed long int   S32;
   *
   *         <b>Description:</b><br/>
   *         Signed 32-bits
   *
   */
    typedef signed long int     S32;
#endif

#endif /* __ENVDEPH__ */

#endif /* BASETYPES_H_ */

#ifndef CEVA_INTRINSICS
//***** Define signed 40-bit accumulator type *****
#ifndef _S40_
#define _S40_
   /** @brief Data Type: Signed 40-bits
   *
   *         <b>Data Prototype:</b> <BR/>
   *         typedef double   S40;
   *
   *         <b>Description:</b><br/>
   *         Signed 40-bits
   *
   */
    typedef double              S40;
#endif

#ifndef __ENVDEPH__

#ifndef _UINT64_
#define _UINT64_
#if defined (_MSC_VER)
typedef unsigned __int64 U64, UINT64, *PUINT64;
#elif defined (__GNUC__)
typedef unsigned long long U64, UINT64, *PUINT64;
#elif defined (__CC_ARM)
typedef unsigned long long U64, UINT64, *PUINT64;
#endif
#endif /* _UINT64_ */

#ifndef _S64_
#define _S64_
#if defined (_MSC_VER)
typedef signed __int64 S64, *PS64;
#elif defined (__GNUC__)
typedef signed long long S64;
#elif defined (__CC_ARM)
typedef signed long long S64;
#endif
#endif /* _UINT64_ */

#endif /* __ENVDEPH__ */

#endif

typedef unsigned int BIT_FIELD;

//*****************************************************************************
//**********          BASIC OP NUMERICAL RANGE DEFINITIONS           **********
//*****************************************************************************

//***** Define long integer sign bit *****
#ifndef  LW_SIGN
    #define LW_SIGN     (long)0x80000000
#endif

//***** Define long integer minimum value *****
#ifndef  LW_MIN
    #define LW_MIN      (long)0x80000000
#endif

//***** Define long integer maximum value *****
#ifndef  LW_MAX
    #define LW_MAX      (long)0x7fffffff
#endif

//***** Define short integer sign bit *****
#ifndef  SW_SIGN
    #define SW_SIGN     (short)0x8000         
#endif

//***** Define short integer minimum value *****
#ifndef  SW_MIN
    #define SW_MIN      (short)0x8000       
#endif

//***** Define short integer maximum value *****
#ifndef SW_MAXV
    #define SW_MAXV      (short)0x7fff
#endif

//*****************************************************************************
//**********     BASIC OP OVERFLOW STATUS INDICATOR DEFINITIONS      **********
//*****************************************************************************

extern int giOverflow;
extern int giOldOverflow;

#endif /* TYPEDEF_H_ */
