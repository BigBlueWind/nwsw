#ifndef _3GNWL_MSG_H_
#define _3GNWL_MSG_H_

#ifndef _WIN32
#include "supervisor.h"
#include "apimgr.h"
#include "cmgr.h"
#include "LtePhyL2Api.h"
#endif



#define NWL_MAX_PSC    (16) /*the maximum of searched cell*/

typedef struct
{
	U8 meaPeriod;
	U8 pscNum;
	U16 psc[MAX_PSC_LIST];
}NWL_CELLSRCHINFO;

U32 NWL_cellSearchReqCheck(U8* pIn);
U32 NWL_cellSearchReqPro(U8* pIn);
U32 NWL_cellSearchStopCheck(U8* pIn);


U32 NWL_BchConReqCheck(U8* pIn);
U32 NWL_BchConReqPro(U8* pIn);
U32 NWL_BchConStopCheck(U8 * pIn);

void NWL_GenerateAPIMsg(U32 msgId, U8 status,U32 MsgType);

#ifndef _WIN32
void   UMTSNwl_thread_SaveData(U8*);
void   UMTSNwl_SaveDataFun(LPVOID pParam);
MXRC   UMTS_Nwl_save_data_thread_init(void);
void   UMTS_Nwl_save_data_thread_kill(void);
U32 UmtsNwlMsgpro(U32 *inMsg);

APIHEC UmtsNWLHandler(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp);
UINT32 WCDMARadioSave(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);

MXRC lte_bs_phy_umtsnmm_msg_handler(LPVOID client_ctx, LPVOID msg);
MXRC lte_bs_phy_umtsnmm_init_api_thread(void);

#endif

#endif
