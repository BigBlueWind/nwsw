//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL const_ms.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------

#ifndef _CONST_H
#define _CONST_H

#ifdef _WIN32
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#endif
#include<limits.h>
#include<math.h>

/* Macro definitions */
#ifdef _WIN32
#ifdef CEVA_INTRINSICS
#define			myprintf(x)
#else
#define         myprintf		printf
#endif
#endif

#define         sqr(a)          ((a)*(a))
#define         cube(a)         ((a)*(a)*(a))

#define         MMAX(a,b)       ((a)>(b) ? (a) : (b))
#define         ABS(a)          (((a)<0) ? -(a) : (a))
#define         MMIN(a,b)        (((a)<(b)) ? (a) : (b))
#define         SIGN(a)         (((a)>=0) ? (1) : (-1))
//#define         ROUND(a)        (long)((a)>0 ? ((a)+0.5) : ((a)-0.5))
#define         AVG(a,b,c)        (short)(((a)+(b)+(c))/(3.0)+0.5)

/* misc. definitions */
#define         BOOLEAN         char
#define         FALSE           0
#define         TRUE            1
#define         MAXFLT          1.0e30
#define         MINFLT          1.0e-30
#define         EPSI            0.00000000000000001
#define         PI              3.14159265358979

#define         nPerRow 6

#endif /* _CONST_H */
