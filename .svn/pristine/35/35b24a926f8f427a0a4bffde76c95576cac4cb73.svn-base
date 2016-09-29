//-------------------------------------------------------------------------------------------
/** @file 4gmx_compiler.h
 *
 * @brief ARM dependent types definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_COMPILER_H_
#define _4GMX_COMPILER_H_

/* RVDS compiler */
#ifdef __CC_ARM

#define INLINE  __inline
#define IRQPROC  __irq
#define ASMPROC  __asm
#define _NOP()   __nop()

#ifdef __APCS_SWST
#define MXEHT
#else
#define MXEHT IRQPROC
#endif

#define MX_GET_RADDR	(__return_address() - 4)

#elif defined (__GNUC__)

#define INLINE  static inline
#define IRQPROC
#define ASMPROC
#define MXEHT
#define _NOP()

#define MX_GET_RADDR	__builtin_return_address(0)

#else /* __CC_ARM ^ __GNUC__ */

#define INLINE
#define IRQPROC
#define ASMPROC
#define _NOP()

#define MXEHT

#define MX_GET_RADDR	0

#endif

#endif /*_4GMX_COMPILER_H_*/
