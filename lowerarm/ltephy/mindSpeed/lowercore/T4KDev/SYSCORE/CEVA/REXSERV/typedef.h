/** @file typedef.h
 *
 * @brief Type Definitions Header File
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

//*****************************************************************************
//**********          BASIC NUMERICAL DATA TYPE DEFINITIONS          **********
//*****************************************************************************

// Version: 3.4             Revision date: march 24, 2009

//*****************************************************************************

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

// TODO: Move all base definitions to a single location
#ifndef BASETYPES_H_

//***** Define unsigned 8-bit byte type *****
#ifndef U8
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
#endif

//***** Define signed 8-bit byte type *****
#ifndef S8
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
#ifndef U16
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
#endif

//***** Define signed 16-bit word type *****
#ifndef S16
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
#ifndef U32
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
#endif

//***** Define signed 32-bit word type *****
#ifndef S32
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

#endif /* BASETYPES_H_ */

#ifndef CEVA_INTRINSICS
//***** Define signed 40-bit accumulator type *****
#ifndef S40
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
