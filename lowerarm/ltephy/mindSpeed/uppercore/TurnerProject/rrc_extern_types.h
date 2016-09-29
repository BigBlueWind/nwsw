
#ifndef _EXT_TYPES_H_
#define _EXT_TYPES_H_

#include "basetypes.h"
#ifdef __cplusplus
extern "C"
{
#endif


typedef  enum _EstablishmentCause_
{
	emergency = 0,
	highPriorityAccess,
	mt_Access,
	mo_Signalling,
	mo_Data,
	spare3,
	spare2,
	spare1
} EstablishmentCause;

typedef struct {
	unsigned char mmec;
	unsigned long mTMSI;
}TsTMSI;
/*
typedef TsTMSI TInitialUEIdentity;

typedef union {
	TInitialUEIdentity Params;
	unsigned char Array[5];
}uTInitialUEIdentity;

typedef struct {
	uTInitialUEIdentity ueIdentity;
	unsigned char establishmentCause;
	unsigned char spare;
}TRRCConnectionRequestR8IEs;*/



typedef  enum _MAXHARQ_TX_
{
	n1, n2, n3, n4, n5, n6, n7, n8, 
		n10, n12, n16, n20, n24, n28, 
			HARQ_spare2, HARQ_spare1}	MAXHARQ_TX;



typedef enum _eBITRATE_ {kBps0, kBps8, kBps16, kBps32, kBps64, kBps128, 
												kBps256, infinity, BR_spare8, BR_spare7, BR_spare6,
												BR_spare5, BR_spare4, BR_spare3, BR_spare2, BR_spare1} eBitRate;

typedef struct _RLC_CFG_
{
	U32 ul_RLC;
	U32	dl_RLC;
} RLCCfg;


typedef struct _LC_CFG_
{
	U16	priority;
	eBitRate	BitRate;
	U16	bucketSizeDuration;
	U8	logicalChannelGroup;
} LCCFG;

typedef struct _SRB_ToAddModifyList_ 
{
	U8	srb_Identity;
	RLCCfg	rlc_cfg;
	LCCFG	lc_cfg;

} SRB_ToAddModifyList;

typedef struct _DRB_ToAddModifyList_
{
	U16	eps_BearerIdentity;
	U32 drb_Identity;
//	PDCPCfg pdcp_Cfg;
	RLCCfg	rlc_cfg;
	U16 logicalChannelIdentity;
	LCCFG	lc_cfg;
}DRB_ToAddModifyList;

typedef struct _RadioResourceConfigDedicated_
{
	SRB_ToAddModifyList srb_ToAddModifyList; // optional
	DRB_ToAddModifyList drb_ToAddModifyList; // optional
	//drb_ToReleaseList					DRB_ToReleaseList,
//	mac_MainConfig						MAC_MainConfiguration, // optional
	//sps_Configuration					SPS_Configuration, // NULL by default
//	physicalConfigDedicated				PhysicalConfigDedicated // NULL by default
}RadioResCfgDedicated;

typedef struct _RRCConnectionRequest_
{
	U8 ue_Identity[5];
	EstablishmentCause	establishment_cause;
	unsigned char bS_TMSI;	
} RRCConnectionRequest, * PRRCConnectionRequest;

typedef struct _RRCConnectionSetup_
{

	U32	rrc_transactionIdentifier;
	RadioResCfgDedicated rr_cfg;
	
} RRCConnectionSetup;


#ifdef __cplusplus
}
#endif
#endif /*_EXT_TYPES_H_ */

