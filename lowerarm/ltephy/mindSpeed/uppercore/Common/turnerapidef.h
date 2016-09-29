#ifndef __TURNERAPI_DEF_
#define __TURNERAPI_DEF_

#include <windows.h>
#if !defined( UINT16 )
typedef unsigned short UINT16, *PUINT16;
#endif
 
#if !defined( UINT8 )
typedef unsigned char UINT8, *PUINT8;
#endif

#define C4K_API_LAST_PARAM	0xFFFF
#define C4K_API_UNKN_LENGTH	0xFFFFFFFF

#define C4K_API_COMMAND_SINGULARITY (12*1024)
#define C4K_API_MAX_COMMAND_SIZE	(16*1024)
	
#define C4K_API_LOAD_L_ARM		0x0001
#define C4K_API_LOAD_DSP		0x0002
#define C4K_API_SUPERV_READY	0xF800
#define C4K_API_DIAG_CONTEXT	0xF900
#define C4K_API_DIAG_CONTROL	0xF901
#define C4K_API_DIAG_INPUT		0xF902
#define C4K_API_DIAG_OUTPUT		0xF903
#define C4K_API_DIAG_RUN		0xF904
#define C4K_API_DIAG_RUNASYNC	0xF905
#define C4K_API_DIAG_DEBUG		0xF906

	
#define C4K_PARAM_HDR_SIZE      4
#define C4K_API_HDR_SIZE        (sizeof (C4kApiHeader))
#define C4K_API_EXT_HDR_SIZE	(sizeof (C4kExtApiHeader))
#define C4K_API_MAX_PAYLOAD	(C4K_API_COMMAND_SINGULARITY-C4K_API_HDR_SIZE)
	
typedef enum _API_TYPES_
{
	C4K_API_TYPE_RES        = 0,
	C4K_API_TYPE_SET	= 1,
	C4K_API_TYPE_SET_RESP   = 2,
	C4K_API_TYPE_QUERY	= 3,
	C4K_API_TYPE_QUERY_RESP = 4,
	C4K_API_TYPE_IND        = 5,
}ApiTypesID;

#define C4K_API_FLAGS_VERSION	(1<<28)
#define C4K_API_FLAGS_EXT_HDR	(1<<1)
#define C4K_API_ENCAP_FM        (1<<2)      // Encapsulation of foreign message
#define C4K_API_FLAGS_FIRST_BUF	(1<<3)

//destinations
#define	C4K_SUPERV_ID		0x0001
#define	C4K_HOST_ID			0x0002
#define	C4K_DSP0_ID			0x8002
#define	C4K_FP0_ID			0x800C
#define	C4K_FEC_ID			0x8016
#define	WiMAX_PHY_DEST_ID	0x8018
#define	LTE_PHY_DEST_ID		0x8019


#define C4KAPI_PID_ERRCODE		1
#define C4KAPI_PID_CONTEXT		3		
#define C4KAPI_PID_FILENAME		3		
#define C4KAPI_PID_CONTROL		4		
#define C4KAPI_PID_TASKID		5
#define C4KAPI_PID_RESID		6
#define C4KAPI_PID_SIZEID		7
#define C4KAPI_PID_OFFSID		8
#define C4KAPI_PID_OUTPUT		9
#define C4KAPI_PID_SET_OUT_SIZE	10
#define DIAG_SET_IODESC_PARAM_ID    11
#define C4KAPI_PID_DSP_CYCLES       12
#define C4KAPI_PID_DIAGTYPE         50

// Wimax PHY Parameters ID
#define PHY_SECTOR_PARAM_ID       (2)
#define PHY_BUF_SIZE_PARAM_ID     (3)
#define PHY_BUF_OFFS_PARAM_ID     (4)
#define PHY_DATA_PARAM_ID         (5)

// use apidefs.h for this
//#define PHY_LTE_FD_MSG 0x511b

#pragma pack ( push, 1 ) 

typedef struct _C4kApiHeader {
	UINT32	Control;
	UINT32	Length;
	UINT16	MsgType;
	UINT16	SrcInstID;
	UINT16	DstInstID;
	UINT16	MsgID;
}C4kApiHeader, * PC4kApiHeader;

typedef struct _C4kApiParam {
	UINT16  ParamID;
	UINT16  ParamLength; // the payload length (length of ParamData)
	UINT32  ParamData[1];
}C4kApiParam, * PC4kApiParam;

typedef struct _C4kExtApiHeader {
	C4kApiHeader h;
	UINT32		TotalSize;    // The total size of payload in bytes ( approx 2MB )
}C4kExtApiHeader, *PC4kExtApiHeader;

#pragma pack ( pop ) 

#define PARAM_ALIGN( X ) ((X+3)&~3)
	
#define ExtApiDataOffs(pApi)  ((PUINT8)pApi + C4K_API_EXT_HDR_SIZE)
	
#define FragExtApiDataSize(pApi) (((PC4kApiHeader)pApi)->Length - C4K_API_EXT_HDR_SIZE)

#endif //__TURNERAPI_DEF_
