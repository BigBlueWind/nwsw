//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL fpdiag.h
 *
 * @brief The FP diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
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

#ifndef _FP_DIAG_H_
#define _FP_DIAG_H_

#include "4gmx.h"
#include "fpdrv.h"
#include "appids.h"
#include "supervisor.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define RC_FPDIAG_OK                MX_OK
#define RC_FPDIAG_INIT_ERROR            DEF_USRRC(APP_MODULE, FPDIAG,1)
#define RC_FPDIAG_CTX_ALLOC_ERROR       DEF_USRRC(APP_MODULE, FPDIAG, 2)
#define RC_FPDIAG_UNKNOWN_API           DEF_USRRC(APP_MODULE, FPDIAG, 3)
#define RC_FPDIAG_UNSUP_TYPE            DEF_USRRC(APP_MODULE, FPDIAG, 4)
#define RC_FPDIAG_RUN_TIMEOUT           DEF_USRRC(APP_MODULE, FPDIAG, 5)
#define RC_FPDIAG_PARAM_ERROR           DEF_USRRC(APP_MODULE, FPDIAG, 6)

#define FPDIAG_NUM_FP         10



/******************************************************************************
*                The FP objects addresses                                    *
*******************************************************************************/

/******************************************************************************
*                The module types definition                                  *
*******************************************************************************/

typedef struct _FPDIAG_CTX_
{
    TCB         Tcb;
    HANDLE      hFpHeap;

} FPDiagCtx, *PFPDiagCtx;

typedef struct tIDFT_CTX
{
	UINT16  Pass;
	UINT16  Fail;
    UINT32  Total;
    UINT32  Avg;
    UINT32  Max;
    UINT32  Min;

}IDFTCTX, *PIDFTCTX;




typedef struct tFP_DIAG_FFT_IFFT_CTX
{
	UINT32 mode, cp1, cp2;
	UINT32 numReps;

	UINT32 fftInSize, fftOutSize;
	PUINT8 fftInRefPtr, fftOutRefPtr;

	UINT32 ifftInSize, ifftOutSize;
	PUINT8 ifftInRefPtr, ifftOutRefPtr;
} FPDIAG_FFT_IFFT_CTX, *PFPDIAG_FFT_IFFT_CTX;




/******************************************************************************
*                The API declaration                                          *
*******************************************************************************/

void FftCbDone(LPVOID pContext);
void IfftCbDone(LPVOID pContext, UINT16 CurrSym);
void IdftCallBack(PTCB pTcb);


MXRC  FPDiagInit (UINT32* pReportData, UINT32* pReportSize);
void FpDiagRunFftIfft(PFPDIAG_FFT_IFFT_CTX pFpDiagFftIfftCtx);
void FpDiagRunIndividualMaps(void);
APIHEC FPDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp);


extern unsigned fp_fft0128_indata_size;
extern unsigned short fp_fft0128_indata[];
extern unsigned fp_ifft0128_indata_size;
extern unsigned short fp_ifft0128_indata[];

extern unsigned fp_fft0128_outdata_size;
extern unsigned short fp_fft0128_outdata[];
extern unsigned fp_ifft0128_outdata_size;
extern unsigned short fp_ifft0128_outdata[];

extern unsigned fp_fft0256_indata_size;
extern unsigned short fp_fft0256_indata[];
extern unsigned fp_ifft0256_indata_size;
extern unsigned short fp_ifft0256_indata[];

extern unsigned fp_fft0256_outdata_size;
extern unsigned short fp_fft0256_outdata[];
extern unsigned fp_ifft0256_outdata_size;
extern unsigned short fp_ifft0256_outdata[];

extern unsigned fp_fft0512_indata_size;
extern unsigned short fp_fft0512_indata[];
extern unsigned fp_ifft0512_indata_size;
extern unsigned short fp_ifft0512_indata[];

extern unsigned fp_fft0512_outdata_size;
extern unsigned short fp_fft0512_outdata[];
extern unsigned fp_ifft0512_outdata_size;
extern unsigned short fp_ifft0512_outdata[];

extern unsigned fp_fft1024_indata_size;
extern unsigned short fp_fft1024_indata[];
extern unsigned fp_ifft1024_indata_size;
extern unsigned short fp_ifft1024_indata[];

extern unsigned fp_fft1024_outdata_size;
extern unsigned short fp_fft1024_outdata[];
extern unsigned fp_ifft1024_outdata_size;
extern unsigned short fp_ifft1024_outdata[];

extern unsigned fp_ifft2048_indata_size;
extern unsigned short fp_ifft2048_indata[];
extern unsigned fp_fft2048_indata_size;
extern unsigned short fp_fft2048_indata[];

extern unsigned fp_fft2048_outdata_size;
extern unsigned short fp_fft2048_outdata[];
extern unsigned fp_ifft2048_outdata_size;
extern unsigned short fp_ifft2048_outdata[];

extern unsigned fp_fft4096_outdata_size;
extern unsigned short fp_fft4096_outdata[];
extern unsigned fp_ifft4096_outdata_size;
extern unsigned short fp_ifft4096_outdata[];

#define MAX_FP_TIMEOUT      150000  // 1 ms

#define FP_IDFT_SIZE        1296 // 12, 24, 36, 48, 60, 72, 96, 108, 120, 144, 180, 192, 216, 240, 288, 300, 324, 360, 384, 432, 480, 540, 576, 600, 648, 720, 768, 864, 900, 960, 972, 1080, 1152, 1200, 1296
#define FP_IDFT_OUTREF      fp_idft1296_outref

#define NUM_REPS 5
#define RUN_FFT
#define RUN_IFFT
#define RUN_IDFT
#define DIAG_TEST_INDIVIDUAL_MAPS

#define RUN_IDFT_ALL_SIZES
#define RUN_IDFT_MULTI_USER_CASES
//#define DIAG_FFT_IFFT_0128
#define DIAG_FFT_IFFT_0256
#define DIAG_FFT_IFFT_0512
#define DIAG_FFT_IFFT_1024
#define DIAG_FFT_IFFT_2048

//#define PRINT_RESULTS
//#define DEBUGFPDIAG;

#endif /*_FP_DIAG_H_*/

#ifdef __cplusplus
}
#endif

