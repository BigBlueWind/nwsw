//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL diagnostics.h
 *
 * @brief The root module of LowARM diagnostics, it contains:
*          - DspDiag
*          - FECDiag
*          - FPDiag
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _DIAGNOSTICS_H_
#define _DIAGNOSTICS_H_

#include "4gmx.h"
#include "appids.h"
#include "larmmsgid.h"
#include "supervisor.h"
#include "tcb.h"
#include "cmgr.h"


#define RC_DIAG_OK                  MX_OK
#define RC_DIAG_NOT_PARAM           DEF_USRRC(APP_MODULE, DIAGNOSTICS, 1)
#define RC_DIAG_ALLOC_BUF_ERROR     DEF_USRRC(APP_MODULE, DIAGNOSTICS, 2)
#define RC_DIAG_PARAM_ERROR         DEF_USRRC(APP_MODULE, DIAGNOSTICS, 3)
#define RC_DIAG_TABLE_INIT_ERROR    DEF_USRRC(APP_MODULE, DIAGNOSTICS, 5)

#define FPGA_FREQUENCY              25000000L       // 25 Mhz


#define DIAG_CONTEXT_PARAM_ID       3
#define DIAG_CONTROL_PARAM_ID       4
#define DIAG_TASK_PARAM_ID          5
#define DIAG_RES_PARAM_ID           6
#define DIAG_BUF_SIZE_PARAM_ID      7
#define DIAG_BUF_OFFS_PARAM_ID      8
#define DIAG_DATA_PARAM_ID          9
#define DIAG_SET_OUTPUT_SIZE        10
#define DIAG_SET_IODESC_PARAM_ID    11
#define DIAG_DSP_CYCLES             12
#define DIAG_LOG_CONTROL            13

#define DIAG_DATA_TYPE              50

#define IOBDESC_LEN_MASK            0x00FFFFFF
#define IOBDESC_CTRL_MASK           0xFF000000
#define IOBDESC_INP_MASK            0x40000000
#define IOBDESC_OUT_MASK            0x80000000
#define IOBDESC_IO_MASK             (IOBDESC_INP_MASK | IOBDESC_OUT_MASK)

#define IOBDESC_LEN(pIOBufDesc)     (pIOBufDesc->IOBufCtrl & IOBDESC_LEN_MASK)

struct _TCB_;

typedef void (* cbTCBDone)(struct tTCB *Tcb);

typedef struct tIOBUFDESC
{
    LPVOID IOBufPtr;
    UINT32 IOBufCtrl;
} IOBUFDESC, *PIOBUFDESC;

typedef enum DrvModMaskID
{
    DIAGNOSTICS_ID     = 0x0, // The general diagnostics functional
    ARMDIAG_ID         = 0x1, // The ARM diagnostics of the FPGA H/W components
    DSPDIAG_ID         = 0x2, // The Dsp diagnostic module
    FECDIAG_ID         = 0x3, // The FEC diagnostic module
    FPDIAG_ID          = 0x4, // The FP diagnostic module
    CPRIDIAG_ID        = 0x5, // The CPRI diagnostic module
    MDMADIAG_ID        = 0x6, // MDMA diagnostics
    RES_DIAG_ID        = 0x7, // 0x27-0x2F reserved for diagnostic modules
    SCH_DIAG_ID        = 0x8, // scheduler diagnostics
    I2C_DIAG_ID        = 0x9, // I2C diagnostics
    SPACC_DIAG_ID      = 0xA, // SPAcc diagnostics
    SPI_DIAG_ID        = 0xB, // SPI diagnostics
    PCIE_DIAG_ID       = 0xC, // PCIE diagnostics
    SW_DIAG_ID         = 0xD, // Software diagnostics 
} DrvModMaskIDs;
typedef struct tagDiagModule
{
    MXRC    (*Proc) (UINT32*, UINT32*);
    DrvModMaskIDs Id;
    char *  Name;

}DiagModule;

#define BEGIN_DIAG_TABLE(x) DiagModule x [] = {
#define END_DIAG_TABLE()    {NULL, DIAGNOSTICS_ID, NULL}};
#define REG_DIAG_MODULE(Proc, Id, name)     {Proc##Init, Id, name},

MXRC DiagSetContext (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi);
MXRC DiagSetControl (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi);
MXRC DiagSetInput   (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi);
MXRC DiagSetOutputSize (HANDLE hHeap, TCB * pTCB, UINT32 nSize);
MXRC DiagAllocOutput   (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi);
MXRC DiagGetContext (TCB * pTCB, ApiHeader * pApi);
MXRC DiagGetControl (TCB * pTCB, ApiHeader * pApi);
MXRC DiagGetInput   (TCB * pTCB, ApiHeader * pApi, ApiHeader * pResp);
MXRC DiagGetOutput  (TCB * pTCB, ApiHeader * pApi, ApiHeader * pResp);

UINT32 DiagHandler(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);


/** @brief The function initializes system diagnostics modules

    @return [MXRC] initialization error code */

MXRC  DiagInit (void);

#endif /*_DIAGNOSTICS_H_*/

#ifdef __cplusplus
}
#endif

