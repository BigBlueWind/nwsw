#ifndef _CELLLISTEN_BCH_MAIN_
#define _CELLLISTEN_BCH_MAIN_

#include "ltephydef.h"
#include "gputil_fx.h"
//#include "tcbscheduler.h"


#define THREAD_PATH_SEARCH                     (1)
#define THREAD_CELLLISTEN_DEMODULE     (2)
#define THREAD_CELLLISTEN_DECODE          (3)
#define THREAD_CELLLISTEN_DONE              (4)
#define THREAD_INVAILABLE                          (0xff)

typedef struct _CLBchThreadCtx
{
    U8 ProFlag;
    U8 SlotCnt;
    U16 PkIdx;
	
} T_CLBchThreadCtx;


typedef struct 
{
    //CellGrpPathInfo *CellGrpPathInfo;

    //CLDeModuTcbInfo *pDeModuInfo;

    T_CLBchThreadCtx *ThreadCtx;

    PMDMA_FDESC pMdma;
    HANDLE hThread;
    

}CLBCHCTXT;




//U32	CLBchReqMsgPro(void);


U32 	CLBchStopMsgPro(void);

void GetCLBchAntData(U8 *DataBuf, U32 DataSz);

void GetCLScrCode(U8 *DataBuf, U32 DataSz);
void StartDMA_DeModu(void);
MXRC UMTS_Nwl_BCH_decode_thread_init(void); 
void UMTS_Nwl_BCH_decode_thread_kill(void);
void Umtsnmm_BchTcbInit(void);
void Umtsnmm_BchTcbStop(void);
void CLBchInit(void);
void CLBchStartDMA(U16 Index, U16 Psc,U8 TtiNum);

#endif
