#ifndef _RRC_MAC_INTERFACE_H_
#define _RRC_MAC_INTERFACE_H_

#include "basetypes.h"
#include "osal.h"


typedef struct _sUE_CtxDlConf_
{
    U32 UePriority;
    U8	Mod;
    U8  MaxRb;
    U32 CodRate;

}ueContextDlConf;

typedef struct _sUE_CtxUlConf_
{
    U32 UePriority;
    U8  Mod;
    U8  MaxRb;
    U32 CodRate;

}ueContextUlConf;

typedef struct _sLC_PARAMS_
{
    U16 TransportBlockSize;
    U16 MaxTransportBlocks;
    U32 GuaranteedBitRate;
    U32 MaxBitRate;
    U16 LCGroup;
    U16 Priority;
    U16 PrioritisedBitRate;
    U16 BucketSizeDuration;
}sLC_PARAMS;
static __inline unsigned short swap16(unsigned short x)
{
	return (x >> 8) | (x << 8);
}

RESULTCODE MacUeCreateParse(U32 UeId, U32* buf, U8 dir);
RESULTCODE MacUeConfigParse(U32 UeId, U32* buf, U8 dir);
RESULTCODE SystemCtrlParse(U8* InBuf, U32 InLen);


#endif /*_RRC_MAC_INTERFACE_H_*/
