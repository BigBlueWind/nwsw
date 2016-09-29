/** @file portio.h
 *
 * @brief Port I/O abstract
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef PORTIO_H_
#define PORTIO_H_

#include "basetypes.h"

#ifdef _MSC_VER
#define inline                      __inline
#endif /* _MSC_VER */

static inline U8 IORead8(PTR port)
{
    return *((V8*)(port));
}

static inline U8 IORead8A(U32 port)
{
    return *((V8*)(port));
}

static inline U16 IORead16(PTR port)
{
    return *((V16*)(port));
}

static inline U16 IORead16A(U32 port)
{
    return *((V16*)(port));
}

static inline U32 IORead32(PTR port)
{
    return *((V32*)(port));
}

static inline U32 IORead32A(U32 port)
{
    return *((V32*)(port));
}

static inline void IOWrite8(PTR port, U8 val)
{
    *((V8*)(port)) = val;
}

static inline void IOWrite8A(U32 port, U8 val)
{
    *((V8*)(port)) = val;
}

static inline void IOWrite16(PTR port, U16 val)
{
    *((V16*)(port)) = val;
}

static inline void IOWrite16A(U32 port, U16 val)
{
    *((V16*)(port)) = val;
}

static inline void IOWrite32(PTR port, U32 val)
{
    *((V32*)(port)) = val;
}

static inline void IOWrite32A(U32 port, U32 val)
{
    *((V32*)(port)) = val;
}

static inline void IOWriteOr8(PTR port, U8 val)
{
    *((V8*)(port)) |= val;
}

static inline void IOWriteOr8A(U32 port, U8 val)
{
    *((V8*)(port)) |= val;
}

static inline void IOWriteOr16(PTR port, U16 val)
{
    *((V16*)(port)) |= val;
}

static inline void IOWriteOr16A(U32 port, U16 val)
{
    *((V16*)(port)) |= val;
}

static inline void IOWriteOr32(PTR port, U32 val)
{
    *((V32*)(port)) |= val;
}

static inline void IOWriteOr32A(U32 port, U32 val)
{
    *((V32*)(port)) |= val;
}

// TODO: Invert mask in function ???
static inline void IOWriteAnd8(PTR port, U8 val)
{
    *((V8*)(port)) &= val;
}

// TODO: Invert mask in function ???
static inline void IOWriteAnd8A(U32 port, U8 val)
{
    *((V8*)(port)) &= val;
}

// TODO: Invert mask in function ???
static inline void IOWriteAnd16(PTR port, U16 val)
{
    *((V16*)(port)) &= val;
}

// TODO: Invert mask in function ???
static inline void IOWriteAnd16A(U32 port, U16 val)
{
    *((V16*)(port)) &= val;
}

// TODO: Invert mask in function ???
static inline void IOWriteAnd32(PTR port, U32 val)
{
    *((V32*)(port)) &= val;
}

// TODO: Invert mask in function ???
static inline void IOWriteAnd32A(U32 port, U32 val)
{
    *((V32*)(port)) &= val;
}

#endif  // PORTIO_H_
