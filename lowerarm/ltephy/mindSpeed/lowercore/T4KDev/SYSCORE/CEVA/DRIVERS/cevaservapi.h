//-------------------------------------------------------------------------------------------
/** @file cevaservapi.h
 *
 * @brief CEVA services, API used to control the same as in ARM primitives
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _CEVA_SERV_API_H_
#define _CEVA_SERV_API_H_

#ifndef CEVA_PROJ

#include "4gmx.h"
#include "hal.h"
#include "config.h"
#include "appprintf.h"

#else
#include "global.h"
#include "prototypes.h"
#endif

#ifndef _4GMX_TYPES_H_

typedef struct _MX_POOL_
{
    PUINT8      StoragePtr;     // The pointer to the storage where blocks are located
    UINT32      BlockNum;       // The number of blocks in storage 
    UINT32      BlockSize;      // The size of block in bytes

    PUINT32     FreePtr;        // The pointer to the storage with free object indexes  
    VUINT32     FreePut;        // PUT index used to put the new item to 'free' storage
    VUINT32     FreeGet;        // GET index used to get the new free item from 'free' storage

    PUINT32     UsedPtr;        // The pointer to the storage with 'Used' object indexes  
    VUINT32     UsedPut;        // PUT index used to put the new item to 'already used' storage
    VUINT32     UsedGet;        // GET index used to get the item from 'already used' storage

}MXPOOL, *PMXPOOL;

#define _NOP()  _dsp_asm("nop")

#endif

#ifndef CEVA_PROJ

void* CSAPoolAlloc(PMXPOOL pPool);
UINT32 CSAPoolFree(PMXPOOL pPool, void * pBlock);

#else

void* CSAPoolAlloc(PMXPOOL pPool) CEVAEXTERN;
UINT32 CSAPoolFree(PMXPOOL pPool, void * pBlock) CEVAEXTERN;

#endif

#endif // _CEVA_SERV_API_H_
#ifdef __cplusplus
}
#endif
