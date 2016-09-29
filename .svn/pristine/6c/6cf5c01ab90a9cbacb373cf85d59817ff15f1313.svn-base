//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL fpdiag.c
 *
 * @brief Filter Processor diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.46 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "appprintf.h"
#include "diagnostics.h"
#include "fpdiag.h"
#include "heaps.h"
#include "larmmsgid.h"
#include "fp_idft_data.h"
#include "systimers.h"
#include "devinfo.h"

unsigned fp_fft_indata_size;
unsigned short *fp_fft_indata;
unsigned fp_ifft_indata_size;
unsigned short *fp_ifft_indata;

VUINT32 gFpDiagCount = 0;
VUINT32 IdftDone, IfftDone, FftDone;
VUINT32 FpDiagOk;



//--------------------------------------------------------------
// Multi-User IDFT Scenarios
//----------------------

unsigned short idft_multiuser_testcase1[] = {
12, 24
};

unsigned short idft_multiuser_testcase2[] = {
12, 48, 324, 192, 72
};

unsigned short idft_multiuser_testcase3[] = {
216, 240, 288, 300
};

unsigned short idft_multiuser_testcase4[] = {
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
};

unsigned short idft_multiuser_testcase5[] = {
108, 96, 72, 60, 48, 36, 24, 12
};

unsigned short idft_multiuser_testcase6[] = {
216, 216, 300, 300
};


#define NUM_IDFT_MULTI_USER_CASES 3
unsigned short *idft_multiuser_testcases[] = {
    idft_multiuser_testcase1,
//    idft_multiuser_testcase2,
    idft_multiuser_testcase3,
//    idft_multiuser_testcase4,
//    idft_multiuser_testcase5,
    idft_multiuser_testcase6,
};

unsigned short idft_multiuser_testcase_sizes[] = {
    2, 4, 4
};

//unsigned short idft_multiuser_testcase_sizes[] = {
//    2, 5, 4, 100, 8, 4
//};

//--------------------------------------------------------------

#ifdef PRINT_RESULTS
#define myuart_printf(...) do {                                                   \
    char string[512];                                                           \
    sprintf(string, __VA_ARGS__);                                               \
    uart_printf("%s", string); \
} while (0);
#else
#define myuart_printf(...)
#endif

TCB fpDiagTcb;


void FftCbDone(LPVOID pContext)
{
    FftDone |= 1<<(((PTCB)pContext)->isym);

    return;
}

void IfftCallBack(LPVOID pContext)
{
    IfftDone |= 1<<(((PTCB)pContext)->isym);

    return;
}

void IdftCallBack(PTCB pTcb)
{
    IdftDone = 1;
    return;
}




MXRC FPDiagInit(UINT32* pReportData, UINT32* pReportSize)
{
    MXRC rc = MX_OK;
    PFPDiagCtx pCtx;
	UINT32 olddev = 0;

    FPDIAG_FFT_IFFT_CTX FpDiagFftIfftCtx;

    UINT32 i, numOutput, outputIndex = 0;
    PUINT8 pBuf;

    // Variables to MapIdftGenerateTables
    UINT16 pIdftSizeArr[1];
    UINT32 ArrSize;

    PTCB pTcb;
    PUINT8 *pIdftInput, *pIdftOutput;
    UINT32 IdftSize, IdftSizeIndex;

    VUINT32 start_tick, exec_tick, numTcbSend;
    PUINT8 fp_outref;

#ifdef RUN_IDFT_ALL_SIZES
    UINT16 IdftSizes[35] = {12, 24, 36, 48, 60, 72, 96, 108, 120, 144,
        180, 192, 216, 240, 288, 300, 324, 360, 384, 432, 480, 540, 576,
        600, 648, 720, 768, 864, 900, 960, 972, 1080, 1152, 1200, 1296};
    UINT16 sizes, numIdftRun = 35;
    IDFTCTX *idftCtx;
#else
    UINT32 numRun;
#endif

    void * pUartBuf = NULL;
    UINT32 nLen = 10*1024;

	olddev = uart_printf_get_dev();

    #ifndef DIAG_FP_PRINT_ALWAYS
    pUartBuf = MxHeapAlloc (AppHeap, nLen);
    if (pUartBuf != NULL)
    {	    
        uart_printf_select_dev(PRINT_DEV_BUF, (UINT32)pUartBuf, nLen);
    }
    #endif

    MapDrvReset();
    FpDiagOk = 1;


#ifdef DIAG_TEST_INDIVIDUAL_MAPS

    //------------------------------------------------------------------------------------------------------------------
    // Test Individual MAPs
    //----------------

    FpDiagRunIndividualMaps();
    uart_printf("\n");

#endif

#ifdef DIAG_FFT_IFFT_0128

    //------------------------------------------------------------------------------------------------------------------
    // 128 Point FFT / IFFT
    //-------------------

#ifdef PRINT_RESULTS
    MxDelayTicks(TICKS_TO_MS(1000));
#endif

    myuart_printf("\n---------------------------\n");
    myuart_printf(  "Testing 128 Pt. FFT / IFFT\n");
    myuart_printf(  "---------------------------\n\n");

    FpDiagFftIfftCtx.mode = FPMODE_FFT0128;
    FpDiagFftIfftCtx.numReps = NUM_REPS;
    FpDiagFftIfftCtx.cp1 = 10;
    FpDiagFftIfftCtx.cp2 = 9;

    FpDiagFftIfftCtx.fftInSize = fp_fft0128_indata_size >> 2;
    FpDiagFftIfftCtx.fftOutSize = 72;
    FpDiagFftIfftCtx.fftInRefPtr = (PUINT8)fp_fft0128_indata;
    FpDiagFftIfftCtx.fftOutRefPtr = (PUINT8)fp_fft0128_outdata;

    FpDiagFftIfftCtx.ifftInSize = fp_ifft0128_indata_size >> 2;
    FpDiagFftIfftCtx.ifftOutSize = 128;
    FpDiagFftIfftCtx.ifftInRefPtr = (PUINT8)fp_ifft0128_indata;
    FpDiagFftIfftCtx.ifftOutRefPtr = (PUINT8)fp_ifft0128_outdata;


    FpDiagRunFftIfft(&FpDiagFftIfftCtx);

#endif

#ifdef DIAG_FFT_IFFT_0256

    //------------------------------------------------------------------------------------------------------------------
    // 256 Point FFT / IFFT
    //-------------------

#ifdef PRINT_RESULTS
    MxDelayTicks(TICKS_TO_MS(1000));
#endif

    myuart_printf("\n---------------------------\n");
    myuart_printf(  "Testing 256 Pt. FFT / IFFT\n");
    myuart_printf(  "---------------------------\n\n");

    FpDiagFftIfftCtx.mode = FPMODE_FFT0256;
    FpDiagFftIfftCtx.numReps = NUM_REPS;
    FpDiagFftIfftCtx.cp1 = 20;
    FpDiagFftIfftCtx.cp2 = 18;

    FpDiagFftIfftCtx.fftInSize = fp_fft0256_indata_size >> 2;
    FpDiagFftIfftCtx.fftOutSize = 180;
    FpDiagFftIfftCtx.fftInRefPtr = (PUINT8)fp_fft0256_indata;
    FpDiagFftIfftCtx.fftOutRefPtr = (PUINT8)fp_fft0256_outdata;

    FpDiagFftIfftCtx.ifftInSize = fp_ifft0256_indata_size >> 2;
    FpDiagFftIfftCtx.ifftOutSize = 256;
    FpDiagFftIfftCtx.ifftInRefPtr = (PUINT8)fp_ifft0256_indata;
    FpDiagFftIfftCtx.ifftOutRefPtr = (PUINT8)fp_ifft0256_outdata;


    FpDiagRunFftIfft(&FpDiagFftIfftCtx);

#endif


#ifdef DIAG_FFT_IFFT_0512

    //------------------------------------------------------------------------------------------------------------------
    // 512 Point FFT / IFFT
    //-------------------

#ifdef PRINT_RESULTS
    MxDelayTicks(TICKS_TO_MS(1000));
#endif

    myuart_printf("\n---------------------------\n");
    myuart_printf(  "Testing 512 Pt. FFT / IFFT\n");
    myuart_printf(  "---------------------------\n\n");

    FpDiagFftIfftCtx.mode = FPMODE_FFT0512;
    FpDiagFftIfftCtx.numReps = NUM_REPS;
    FpDiagFftIfftCtx.cp1 = 40;
    FpDiagFftIfftCtx.cp2 = 36;

    FpDiagFftIfftCtx.fftInSize = fp_fft0512_indata_size >> 2;
    FpDiagFftIfftCtx.fftOutSize = 300;
    FpDiagFftIfftCtx.fftInRefPtr = (PUINT8)fp_fft0512_indata;
    FpDiagFftIfftCtx.fftOutRefPtr = (PUINT8)fp_fft0512_outdata;

    FpDiagFftIfftCtx.ifftInSize = fp_ifft0512_indata_size >> 2;
    FpDiagFftIfftCtx.ifftOutSize = 512;
    FpDiagFftIfftCtx.ifftInRefPtr = (PUINT8)fp_ifft0512_indata;
    FpDiagFftIfftCtx.ifftOutRefPtr = (PUINT8)fp_ifft0512_outdata;


    FpDiagRunFftIfft(&FpDiagFftIfftCtx);

#endif


#ifdef DIAG_FFT_IFFT_1024

    //------------------------------------------------------------------------------------------------------------------
    // 1024 Point FFT / IFFT
    //-------------------

#ifdef PRINT_RESULTS
    MxDelayTicks(TICKS_TO_MS(1000));
#endif

    myuart_printf("\n----------------------------\n");
    myuart_printf(  "Testing 1024 Pt. FFT / IFFT\n");
    myuart_printf(  "----------------------------\n\n");

    FpDiagFftIfftCtx.mode = FPMODE_FFT1024;
    FpDiagFftIfftCtx.numReps = NUM_REPS;
    FpDiagFftIfftCtx.cp1 = 80;
    FpDiagFftIfftCtx.cp2 = 72;

    FpDiagFftIfftCtx.fftInSize = fp_fft1024_indata_size >> 2;
    FpDiagFftIfftCtx.fftOutSize = 600;
    FpDiagFftIfftCtx.fftInRefPtr = (PUINT8)fp_fft1024_indata;
    FpDiagFftIfftCtx.fftOutRefPtr = (PUINT8)fp_fft1024_outdata;

    FpDiagFftIfftCtx.ifftInSize = fp_ifft1024_indata_size >> 2;
    FpDiagFftIfftCtx.ifftOutSize = 1024;
    FpDiagFftIfftCtx.ifftInRefPtr = (PUINT8)fp_ifft1024_indata;
    FpDiagFftIfftCtx.ifftOutRefPtr = (PUINT8)fp_ifft1024_outdata;


    FpDiagRunFftIfft(&FpDiagFftIfftCtx);

#endif

#ifdef DIAG_FFT_IFFT_2048

    //------------------------------------------------------------------------------------------------------------------
    // 2048 Point FFT / IFFT
    //-------------------

#ifdef PRINT_RESULTS
    MxDelayTicks(TICKS_TO_MS(1000));
#endif

    myuart_printf("\n----------------------------\n");
    myuart_printf(  "Testing 2048 Pt. FFT / IFFT\n");
    myuart_printf(  "----------------------------\n\n");

    FpDiagFftIfftCtx.mode = FPMODE_FFT2048;
    FpDiagFftIfftCtx.numReps = NUM_REPS;
    FpDiagFftIfftCtx.cp1 = 160;
    FpDiagFftIfftCtx.cp2 = 144;

    FpDiagFftIfftCtx.fftInSize = fp_fft2048_indata_size >> 2;
    FpDiagFftIfftCtx.fftOutSize = 1200;
    FpDiagFftIfftCtx.fftInRefPtr = (PUINT8)fp_fft2048_indata;
    FpDiagFftIfftCtx.fftOutRefPtr = (PUINT8)fp_fft2048_outdata;

    FpDiagFftIfftCtx.ifftInSize = fp_ifft2048_indata_size >> 2;
    FpDiagFftIfftCtx.ifftOutSize = 2048;
    FpDiagFftIfftCtx.ifftInRefPtr = (PUINT8)fp_ifft2048_indata;
    FpDiagFftIfftCtx.ifftOutRefPtr = (PUINT8)fp_ifft2048_outdata;


    FpDiagRunFftIfft(&FpDiagFftIfftCtx);

#endif

    //------------------------------------------------------------------------------------------------------------------

#ifdef RUN_IDFT

#ifdef PRINT_RESULTS
     MxDelayTicks(TICKS_TO_MS(1000));
#endif

    myuart_printf("\n----------------------------\n");
    myuart_printf("Testing IDFT\n");
    myuart_printf("----------------------------\n\n");

    MapIdftDrvSetCallback((MAPDONECB)IdftCallBack, NULL);

    // Malloc TCBs
    pIdftInput = MxHeapAlloc(DspHeap, IDFT_DRV_QSIZE*sizeof(PUINT8));
    pIdftOutput = MxHeapAlloc(DspHeap, IDFT_DRV_QSIZE*sizeof(PUINT8));
    for (i = 0; i < IDFT_DRV_QSIZE;  i++)
    {
        // Input
        pIdftInput[i] = MxHeapAlloc(DspHeap, 5188); // allocate max size buffer
        _ASSERT_PTR(pIdftInput[i]);
        memcpy(pIdftInput[i], fp_idft_indata, 5184);

        //Output
        pIdftOutput[i] = MxHeapAlloc(DspHeap, 5188); // allocate max size buffer
        _ASSERT_PTR(pIdftOutput[i]);
        memset(pIdftOutput[i], 0xBC, 5188);
    }

    // TCB
    pTcb = &fpDiagTcb;
    _ASSERT_PTR(pTcb);

#ifdef RUN_IDFT_ALL_SIZES
    idftCtx = MxHeapAlloc(DspHeap, sizeof(IDFTCTX)*numIdftRun);
    for (sizes = 0; sizes < numIdftRun; sizes++){
        idftCtx[sizes].Pass = idftCtx[sizes].Fail = idftCtx[sizes].Avg = idftCtx[sizes].Total = 0;
        idftCtx[sizes].Max = 0;
        idftCtx[sizes].Min = 100000;
        }

    for (sizes = 0; sizes < numIdftRun; sizes++){
        IdftSize = IdftSizes[sizes];

    myuart_printf("Running IDFT Size: %d\n", IdftSize);

#else // RUN_IDFT_ALL_SIZES
    for (numRun = 0; numRun < 1; numRun++){
        IdftSize = FP_IDFT_SIZE;                        // IDFT size
#endif // RUN_IDFT_ALL_SIZES

    numTcbSend = IDFT_DRV_QSIZE;
    pIdftSizeArr[0] = IdftSize;
    ArrSize = 1;
    MapIdftGenerateTables(pIdftSizeArr, ArrSize);

    // Set TCB
    pTcb->ExecTicks = 0;

    // Inputs / Outputs
    for (i = 0; i < numTcbSend;  i++)
    {
        memset(pIdftOutput[i], 0xBC, 5188);
        MapIdftDrvSetInBuf(pIdftInput[i], i, (2*IdftSize*sizeof(short) + 8));                   // 8 for Exponent (Copying PHY)
        MapIdftDrvSetOutBuf(pIdftOutput[i], i, (2*IdftSize*sizeof(short) + 8));                 // 8 for Exponent
    }

    // Run IDFT
    IdftSizeIndex = fp_idft_size_index_map[IdftSize];
    fp_outref = (PUINT8)fp_idft_outrefs[IdftSizeIndex];

    MapIdftQueue(pTcb, 0xFFF);

    start_tick = GetTIMETICK();

    IdftDone = 0;
    MapIdftUseQueueFromTcb(0);
    MapIdftDrvRun(pTcb);
    while(IdftDone != 1);

    exec_tick = GetTIMETICK() - start_tick;

    for (i = 0; i < numTcbSend;  i++)
    {
        if (memcmp(pIdftOutput[i], fp_outref, 2*sizeof(short)*IdftSize) == 0)
        {
#ifndef RUN_IDFT_ALL_SIZES

            myuart_printf("FP Diag: %d IDFT BIST passed. ExecClocks=%d", IdftSize, pTcb->ExecTicks);
            myuart_printf(" Expo = %d\r\n", (short) (*((PUINT8)pIOControl[1].IOBufPtr+(4*IdftSize))));

#else // RUN_IDFT_ALL_SIZES
            idftCtx[sizes].Pass++;
#endif // RUN_IDFT_ALL_SIZES
        }
        else
        {
#ifndef RUN_IDFT_ALL_SIZES
            pBuf = pIdftOutput[i];
            myuart_printf("FP Diag: %d IDFT BIST failed. ExecClocks=%d\r\n", IdftSize, pTcb->ExecTicks);
            for (j = 0; j < 16; j++)
            {
                myuart_printf("%02x ", pBuf[j]);
            }
            myuart_printf("\n");
            FpDiagOk = 0;

#else // RUN_IDFT_ALL_SIZES
            idftCtx[sizes].Fail++;
            FpDiagOk = 0;
#endif // RUN_IDFT_ALL_SIZES
        }
    }

#ifdef RUN_IDFT_ALL_SIZES
    if (idftCtx[sizes].Max < pTcb->ExecTicks)
    {
        idftCtx[sizes].Max = pTcb->ExecTicks;
    }

    if (idftCtx[sizes].Min > pTcb->ExecTicks)
    {
        idftCtx[sizes].Min = pTcb->ExecTicks;
    }

    idftCtx[sizes].Total = exec_tick;
    idftCtx[sizes].Avg = exec_tick/numTcbSend;

#endif // RUN_IDFT_ALL_SIZES

#ifndef RUN_IDFT_ALL_SIZES

    myuart_printf("\nTotal time to run all tests: %d\n", exec_tick);
    myuart_printf("Avg time to run all tests: %d\n", exec_tick/numTcbSend);
    myuart_printf("----------------------------\n\n");
    }

#else // RUN_IDFT_ALL_SIZES

    }

    // Print results
    //-----------

#ifdef PRINT_RESULTS
    MxDelayTicks(TICKS_TO_MS(1000));
#endif

    myuart_printf("\nNumber of Times Run: %d\n", numTcbSend);
    myuart_printf("Size  Pass  Fail    Avg    Tot     Min  Max\n\n");
    for (sizes = 0; sizes < numIdftRun; sizes++)
    {
        myuart_printf("%4d  %2d    %2d    %5d  %6d  %5d  %d\n",IdftSizes[sizes],
        idftCtx[sizes].Pass, idftCtx[sizes].Fail,
        idftCtx[sizes].Avg, idftCtx[sizes].Total, idftCtx[sizes].Min, idftCtx[sizes].Max);
    }
    myuart_printf("\n");

    MxHeapFree(DspHeap, idftCtx);

#endif // RUN_IDFT_ALL_SIZES

    for (i = 0; i < IDFT_DRV_QSIZE;  i++)
    {
        MxHeapFree(DspHeap, pIdftInput[i]);
        MxHeapFree(DspHeap, pIdftOutput[i]);
    }
    MxHeapFree(DspHeap, pIdftInput);
    MxHeapFree(DspHeap, pIdftOutput);





#ifdef RUN_IDFT_MULTI_USER_CASES

#ifdef PRINT_RESULTS
    MxDelayTicks(TICKS_TO_MS(1000));
#endif
    MapIdftDrvSetCallback((MAPDONECB)IdftCallBack, NULL);

    // Malloc
    pIdftInput = MxHeapAlloc(DspHeap, IDFT_DRV_QSIZE*sizeof(PUINT8));
    pIdftOutput = MxHeapAlloc(DspHeap, IDFT_DRV_QSIZE*sizeof(PUINT8));
    for (i = 0; i < IDFT_DRV_QSIZE;  i++)
    {
        // Input
        pIdftInput[i] = MxHeapAlloc(DspHeap, 6000); // allocate max size buffer
        _ASSERT_PTR(pIdftInput[i]);

        //Output
        pIdftOutput[i] = MxHeapAlloc(DspHeap, 6000); // allocate max size buffer
        _ASSERT_PTR(pIdftOutput[i]);
    }

    // TCB
    pTcb = &fpDiagTcb;
    _ASSERT_PTR(pTcb);

    // Init
    idftCtx = MxHeapAlloc(DspHeap, sizeof(IDFTCTX)*NUM_IDFT_MULTI_USER_CASES);
    for (sizes = 0; sizes < NUM_IDFT_MULTI_USER_CASES; sizes++)
    {
        idftCtx[sizes].Pass = idftCtx[sizes].Fail = idftCtx[sizes].Avg = idftCtx[sizes].Total = 0;
        idftCtx[sizes].Max = 0;
        idftCtx[sizes].Min = 100000;
    }

    for (sizes = 0; sizes < NUM_IDFT_MULTI_USER_CASES; sizes++)
    {
        myuart_printf("Running Multi-User TestCase %d\n", sizes+1);

        // Generate Descriptors
        MapIdftGenerateTables(idft_multiuser_testcases[sizes], idft_multiuser_testcase_sizes[sizes]);

        numTcbSend = IDFT_DRV_QSIZE;

        // Copy Input
        for (i = 0; i < numTcbSend;  i++)
        {
            outputIndex = 0;
            for (numOutput = 0; numOutput < idft_multiuser_testcase_sizes[sizes]; numOutput++)
            {
                IdftSize = idft_multiuser_testcases[sizes][numOutput];
                pBuf = (PUINT8) &pIdftInput[i][outputIndex];
                outputIndex += (IdftSize+2) << 2;     // 8 for Exponent (Copying PHY)
                memcpy(pBuf, fp_idft_indata, IdftSize << 2);
            }
        }

        // Set TCBs
        pTcb->ExecTicks = 0;

        // Input / Output
        for (i = 0; i < numTcbSend;  i++)
        {
            MapIdftDrvSetInBuf(pIdftInput[i], i, outputIndex);
            MapIdftDrvSetOutBuf(pIdftOutput[i], i, outputIndex);
        }

        // Run IDFT
        MapIdftUseQueueFromTcb(0);
        MapIdftQueue(pTcb, 0xFFF);
        start_tick = GetTIMETICK();

        IdftDone = 0;
        MapIdftDrvRun(pTcb);
        while(IdftDone != 1);

        exec_tick = GetTIMETICK() - start_tick;

        for (i = 0; i < numTcbSend;  i++)
        {
            outputIndex = 0;
            for (numOutput = 0; numOutput < idft_multiuser_testcase_sizes[sizes]; numOutput++)
            {
                IdftSize = idft_multiuser_testcases[sizes][numOutput];
                IdftSizeIndex = fp_idft_size_index_map[IdftSize];
                fp_outref = (PUINT8)fp_idft_outrefs[IdftSizeIndex];
                pBuf = (PUINT8) &pIdftOutput[i][outputIndex];
                outputIndex += (IdftSize+2) << 2;     // 8 for Exponent (Copying PHY)
                if (memcmp(pBuf, fp_outref, 2*sizeof(short)*IdftSize) == 0)
                {
                    idftCtx[sizes].Pass++;
                }
                else
                {
                    idftCtx[sizes].Fail++;
                    FpDiagOk = 0;
                }
            }
        }

        if (idftCtx[sizes].Max < pTcb->ExecTicks)
        {
            idftCtx[sizes].Max = pTcb->ExecTicks;
        }

        if (idftCtx[sizes].Min > pTcb->ExecTicks)
        {
            idftCtx[sizes].Min = pTcb->ExecTicks;
        }

        idftCtx[sizes].Total = exec_tick;
        idftCtx[sizes].Avg = exec_tick/numTcbSend;
    }

    // Print results
    //-----------
    myuart_printf("\nNumber of Times Run: %d\n", numTcbSend);
    myuart_printf("Case  NumIdft  Pass  Fail    Avg    Tot     Min  Max\n\n");
    for (sizes = 0; sizes < NUM_IDFT_MULTI_USER_CASES; sizes++)
    {
        myuart_printf("%4d   %4d   %4d   %4d  %5d  %6d  %5d  %d\n",sizes+1, idft_multiuser_testcase_sizes[sizes],
        idftCtx[sizes].Pass, idftCtx[sizes].Fail,
        idftCtx[sizes].Avg, idftCtx[sizes].Total, idftCtx[sizes].Min, idftCtx[sizes].Max);
    }
    myuart_printf("\r\n");

    // Clean Up
    //---------
    MxHeapFree(DspHeap, idftCtx);
    for (i = 0; i < IDFT_DRV_QSIZE; i++)
    {
        MxHeapFree(DspHeap, pIdftInput[i]);
        MxHeapFree(DspHeap, pIdftOutput[i]);
    }
    MxHeapFree(DspHeap, pIdftInput);
    MxHeapFree(DspHeap, pIdftOutput);


#endif // RUN_IDFT_MULTI_USER_CASES



#endif // RUN_IDFT


    uart_printf("\r\n                ~~~~~~~~\n");
    uart_printf("FP diagnostics: | %s |\n",(FpDiagOk?"PASS":"FAIL"));
    uart_printf("                ~~~~~~~~\n");

    MapDrvInit(FPMODE_FFT1024, 0);

    pCtx = (PFPDiagCtx)MxHeapAlloc(AppHeap, sizeof (FPDiagCtx));

    if (pCtx == NULL)
        return RC_FPDIAG_CTX_ALLOC_ERROR;

    memset(pCtx, 0, sizeof (FPDiagCtx));

    pCtx->hFpHeap = DspHeap;

    for (i = 0; i < DevInfoGetNumber(DEVID_FP); i++)
    {
        rc = SvsrRegClient(IID_FP_DIAG_0 + i, FPDiagApiHandler, pCtx);
    }

    if (pUartBuf != NULL)
    {
        if (!FpDiagOk)
        {
            uart_printf_print_buf();
        }

        uart_printf_select_dev(olddev, 0, 0);
        MxHeapFree(AppHeap, pUartBuf);
    }

    return rc;
}

APIHEC FPDiagApiHandler(LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    PFPDiagCtx pCtx = (PFPDiagCtx)pData;
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    MXRC rc = MX_OK;
    ApiParam * pContextID;
    ApiParam * pControlParam;
    ApiParam * pTaskParam;
    UINT32 dbg_blk_sel;
    LPVOID data_buf;
    UINT32 data_buf_size;
    UINT32 dbg_blk_offset;
    UINT32 apb_offset;
    UINT32 apb_reg_val;
    UINT32 nCycles;

    if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        pResp->Type = API_TYPE_CFG_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_DEBUG:
                {
                    pContextID = ApiGetParamByID(pApi, DIAG_LOG_CONTROL);
                    if (pContextID)
                    {
                        if (pContextID->ParamLength != 4)
                        {
                            rc = RC_FPDIAG_PARAM_ERROR;
                            break;
                        }
                        pMapDrvCtx->DbgLogEna = pContextID->ParamData[0];
                    }
                    pContextID = ApiGetParamByID(pApi, DIAG_CONTEXT_PARAM_ID);
                    if (pContextID)
                    {
                        dbg_blk_sel = pContextID->ParamData[0];
                        dbg_blk_offset = pContextID->ParamData[1];
                        data_buf_size = pContextID->ParamLength - 2*sizeof(pContextID->ParamData[0]);
                        data_buf = &pContextID->ParamData[2];
                        printf("FPDiagApiHandler: DBGCFG: dbg_blk_sel=0x%x data_buf_size=0x%x\r\n", dbg_blk_sel, data_buf_size);
                        MapDbgUploadDram(0, FALSE, dbg_blk_sel, dbg_blk_offset, data_buf, data_buf_size);
                    }
                    pContextID = ApiGetParamByID(pApi, DIAG_CONTROL_PARAM_ID);
                    if (pContextID)
                    {
                        if (pContextID->ParamLength != 4 && pContextID->ParamLength != 8 && pContextID->ParamLength != 12)
                        {
                            rc = RC_FPDIAG_PARAM_ERROR;
                            break;
                        }
                        nCycles = pContextID->ParamData[0];
                        pContextID->ParamLength -= 4;
                        if (pContextID->ParamLength)
                        {
                            printf("FPDiagApiHandler: DBGCFG: MapDbgSetDebugTrigger 0x%x\r\n", pContextID->ParamData[1]);
                            if (pContextID->ParamData[1])
                                MapDbgSetDebugTrigger(0, pContextID->ParamData[1]);
                            pContextID->ParamLength -= 4;
                        }
                        if (pContextID->ParamLength)
                        {
                            printf("FPDiagApiHandler: DBGCFG: MapDbgSetPcTrigger 0x%x\r\n", pContextID->ParamData[2]);
                            MapDbgSetPcTrigger(0, pContextID->ParamData[2]);
                            pContextID->ParamLength -= 4;
                        }

                        printf("FPDiagApiHandler: DBGRUN: nCycles=0x%x\r\n", nCycles);
                        if (MapDbgRun(0, nCycles) == 0)
                            rc = RC_FPDIAG_RUN_TIMEOUT;
                        else
                        {
                            data_buf_size = 4*128*sizeof(UINT32);
                            data_buf = MxHeapAlloc(DspHeap, data_buf_size);
                            if(!data_buf) {
                                rc = RC_FPDIAG_INIT_ERROR;
                                break;
                             }
                            data_buf_size = 128*sizeof(UINT32);

                            dbg_blk_sel = 0x18;
                            MapDbgUnloadDram(0, FALSE, dbg_blk_sel, 0, (UINT32 *)data_buf, data_buf_size);

                            dbg_blk_sel = 0x19;
                            MapDbgUnloadDram(0, FALSE, dbg_blk_sel, 0, (UINT32 *)data_buf+128, data_buf_size);

                            dbg_blk_sel = 0x1A;
                            MapDbgUnloadDram(0, FALSE, dbg_blk_sel, 0, (UINT32 *)data_buf+256, data_buf_size);

                            dbg_blk_sel = 0x1B;
                            MapDbgUnloadDram(0, FALSE, dbg_blk_sel, 0, (UINT32 *)data_buf+384, data_buf_size);

                            data_buf_size = 4*128*sizeof(UINT32);
                            ApiAddParam(pResp, NULL, DIAG_CONTROL_PARAM_ID, data_buf, data_buf_size);
                            MxHeapFree(DspHeap, data_buf);
                        }
                    }
                    break;
                }

            case DIAG_RUN:
                {
                    pTaskParam = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
                    if (pTaskParam)
                    {
                        pCtx->Tcb.TaskID = pTaskParam->ParamData[0];
                        //MapIdftQueue(&pCtx->Tcb);
                        //MapIdftDrvRun(&pCtx->Tcb);
                    }
                    break;
                }

            case DIAG_CONTEXT:
                {
                    pContextID = ApiGetParamByID(pApi, DIAG_CONTEXT_PARAM_ID);
                    if (pContextID)
                    {
                        data_buf_size = pContextID->ParamLength;
                        data_buf = &pContextID->ParamData[0];
                        printf("FPDiagApiHandler: CTX: data_buf_size=0x%x\r\n", data_buf_size);
                        MapUploadPram(0, data_buf, data_buf_size);
                    }
                    else
                        rc = RC_FPDIAG_PARAM_ERROR;
                    break;
                }

            case DIAG_CONTROL:
                {
                    pControlParam = ApiGetParamByID(pApi, DIAG_CONTROL_PARAM_ID);
                    if (pControlParam)
                    {
                        apb_offset = pControlParam->ParamData[0];
                        apb_reg_val = pControlParam->ParamData[1];
                        printf("FPDiagApiHandler: DIAG_CONTROL CFG: apb_offset=0x%x apb_reg_val=0x%x\r\n", apb_offset, apb_reg_val);
                        if (apb_offset < 0x110)
                        {
                            MapRegWrite(0, apb_offset, apb_reg_val);
                        }
                        else
                            rc = RC_FPDIAG_PARAM_ERROR;
                    }
                    else
                        rc = RC_FPDIAG_PARAM_ERROR;
                    break;
                }

            case DIAG_INPUT:
                {
                    rc = DiagSetInput(pCtx->hFpHeap, &pCtx->Tcb, pApi);
                    break;
                }

            default:
                {
                    rc = RC_FPDIAG_UNKNOWN_API;
                    break;
                }
        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {
        pResp->Type = API_TYPE_QRY_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_DEBUG:
                {
                    pContextID = ApiGetParamByID(pApi, DIAG_CONTEXT_PARAM_ID);
                    if (pContextID)
                    {
                        dbg_blk_sel = pContextID->ParamData[0];
                        if (dbg_blk_sel < 0x08)
                        {
                            data_buf_size = 2048*sizeof(UINT16);
                        }
                        else if (dbg_blk_sel < 0x10)
                        {
                            data_buf_size = 2048*3;
                        }
                        else if (dbg_blk_sel < 0x18)
                        {
                            data_buf_size = 2048*sizeof(UINT32);
                        }
                        else
                        {
                            data_buf_size = 128*sizeof(UINT32);
                        }
                        printf("FPDiagApiHandler: DBGQRY: dbg_blk_sel=0x%x data_buf_size=0x%x\r\n", dbg_blk_sel, data_buf_size);
                        data_buf = MxHeapAlloc(DspHeap, data_buf_size);
                        if(!data_buf) {rc =RC_FPDIAG_INIT_ERROR; break;}
                        MapDbgUnloadDram(0, FALSE, dbg_blk_sel, 0, data_buf, data_buf_size);
                        ApiAddParam(pResp, NULL, DIAG_CONTEXT_PARAM_ID, data_buf, data_buf_size);
                        MxHeapFree(DspHeap, data_buf);
                    }
                    else
                        rc = RC_FPDIAG_PARAM_ERROR;
                    break;
                }

            case DIAG_CONTEXT:
                {
                    pContextID = ApiGetParamByID(pApi, DIAG_CONTEXT_PARAM_ID);
                    if (pContextID)
                    {
                        data_buf_size = 20*512;
                        printf("FPDiagApiHandler: PRAM: data_buf_size=0x%x\r\n", data_buf_size);
                        data_buf = MxHeapAlloc(DspHeap, data_buf_size);
                        if(!data_buf) { rc=RC_FPDIAG_INIT_ERROR; break;}
                        MapUnloadPram(0, data_buf, data_buf_size);
                        ApiAddParam(pResp, NULL, DIAG_CONTEXT_PARAM_ID, data_buf, data_buf_size);
                        MxHeapFree(DspHeap, data_buf);
                    }
                    else
                        rc = RC_FPDIAG_PARAM_ERROR;
                    break;
                }

            case DIAG_CONTROL:
                {
                    pControlParam = ApiGetParamByID(pApi, DIAG_CONTROL_PARAM_ID);
                    if (pControlParam)
                    {
                        apb_offset = pControlParam->ParamData[0];
                        printf("FPDiagApiHandler: DIAG_CONTROL QRY: apb_offset=0x%x\r\n", apb_offset);
                        if (apb_offset <= 0x110)
                        {
                            UINT32 resp_buf;

                            resp_buf = MapRegRead(0, apb_offset);
                            printf("FPDiagApiHandler: DIAG_CONTROL QRY: apb_value=0x%x\r\n", resp_buf);
                            ApiAddParam(pResp, NULL, DIAG_CONTROL_PARAM_ID, &resp_buf, sizeof(resp_buf));
                        }
                        else
                            rc = RC_FPDIAG_PARAM_ERROR;
                    }
                    else
                        rc = RC_FPDIAG_PARAM_ERROR;
                    break;
                }

            case DIAG_INPUT:
                {
                    rc = DiagGetInput(&pCtx->Tcb, pApi, pResp);
                    break;
                }

            case DIAG_OUTPUT:
                {
                    rc = DiagGetOutput(&pCtx->Tcb, pApi, pResp);
                    break;
                }

            default:
                {
                    rc = RC_FPDIAG_UNKNOWN_API;
                    break;
                }
        }
    }
    else
    {
        rc = RC_FPDIAG_UNSUP_TYPE;
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}




















void FpDiagRunFftIfft(PFPDIAG_FFT_IFFT_CTX pCtx)
{
    UINT8 FftPass, FftFail, IfftPass, IfftFail;
    UINT32 i, j;
    UINT32 numAnt;
    UINT32 _size, ticks, symNum;
    UINT32 cpSize, outSize, inSize;
    UINT32 roundFftOutLen, roundIfftInLen;
    PUINT32 p;
    PUINT8 pRef, pOut;
    PUINT32 pInpBuf, pOutBuf;
    BOOL bOk;
    TCB  Tcb1, Tcb2;

    UINT16 trap = 0;

    FftPass = FftFail = IfftPass = IfftFail = 0;

    roundFftOutLen = ROUND((pCtx->fftOutSize + 2), 8);
    roundIfftInLen = ROUND(pCtx->ifftInSize, 32);

    MapDrvInit(pCtx->mode, 0);

    while(trap);

    for (j = 0; j < pCtx->numReps; ++j)
    {

#ifdef RUN_FFT
        // FFT
        //-----

        numAnt = FFT_DRV_QSIZE / 14;

        // Malloc Input / Output Buffers
        inSize = (pCtx->fftInSize << 2);
        _size = (FFT_DRV_QSIZE * inSize) + ((2*numAnt*pCtx->cp1 + 12*numAnt*pCtx->cp2) << 2);
        pInpBuf = MxHeapAlloc(DspHeap, _size);
        _ASSERT_PTR(pInpBuf);
        memset(pInpBuf, 0x42, _size);

        _size = (FFT_DRV_QSIZE * (roundFftOutLen << 2));
        pOutBuf = MxHeapAlloc(DspHeap, _size);
        _ASSERT_PTR(pOutBuf);
        memset(pOutBuf, 0x42, _size);

        for (i = 0, p = pInpBuf; i < FFT_DRV_QSIZE; i++, p += pCtx->fftInSize)
        {
            symNum = i%14;
            p += ((symNum == 0 || symNum == 7) ? pCtx->cp1 : pCtx->cp2);
            memcpy(p, pCtx->fftInRefPtr, inSize);
        }

        // Call FFT functions
        trap = 0;
        //uart_printf("Halt FFT Diag All FP\n");
        while(trap);
        MapFftDrvSetInpBuf(pInpBuf, FFT_DRV_QSIZE, 0);
        MapFftDrvSetOutBuf(pOutBuf, FFT_DRV_QSIZE, 0);
        MapFftDrvSetNumAntennas(numAnt);
        MapFftDrvSetCallback(FftCbDone, NULL);

        // Dispatch
        ticks = SysTimerGetTick();
        FftDone = 0;
        Tcb1.isym = 0;
        MapFftDrvPreRun(&Tcb1);
        MapFftDrvRun(&Tcb1);
        Tcb2.isym = 1;
        MapFftDrvPreRun(&Tcb2);
        MapFftDrvRun(&Tcb2);

        // Wait till done
        while (FftDone != 0x3);
        ticks = SysTimerGetTick() - ticks;

        // Compare outputs
        bOk = TRUE;
        pRef = (PUINT8)pCtx->fftOutRefPtr;
        pOut = (PUINT8)pOutBuf;
        outSize = (pCtx->fftOutSize << 1);
        for (i = 0; i < FFT_DRV_QSIZE; i++)
        {
            if (memcmp(pRef + (inSize - outSize), pOut, outSize) ||
            memcmp(pRef, pOut + outSize, outSize))
            {
#ifndef DEBUGFPDIAG
                bOk = FALSE;
                break;
#else
                {
                    UINT32 ii;
                    PUINT16 pa, pb;

                    uart_printf("FFT: isym = %d\n1st\n", i);
                    pa = (PUINT16)(pRef + (inSize - outSize));
                    pb = (PUINT16)pOut;
                    for (ii = 0; ii < outSize/2; ii++)
                        if (*(pa+ii)!= *(pb+ii))
                            {
                            uart_printf("%d 0x%04x 0x%04x\n", ii, *(pa+ii), *(pb+ii));
                            break;
                            }

                    uart_printf("\n2nd\n", i);
                    pa = (PUINT16)pRef;
                    pb = (PUINT16)(pOut + outSize);
                    for (ii = 0; ii < outSize/2; ii++)
                        if (*(pa+ii)!= *(pb+ii))
                            {
                            uart_printf("%d 0x%04x 0x%04x\n", ii, *(pa+ii), *(pb+ii));
                            break;
                            }

                    uart_printf("\n\n");
                    MxDelayTicks(15000000);
                }

                bOk = FALSE;
                break;
#endif
            }
            pOut += (roundFftOutLen << 2);    //  Next symbol
        }

        pOut = (PUINT8)pOutBuf;
        myuart_printf("FFT %u %s: ExecTicks=%u, Exp=%d\n",
                     (1 << pCtx->mode), (bOk ? "passed" : "failed"),
                     ticks, *(PUINT16)(pOut + (pCtx->fftOutSize << 2)));

        FftPass += bOk ? 1:0;
        FftFail += bOk ? 0:1;
        if (bOk == FALSE)
        FpDiagOk = 0;


        // Free allocated memory
        MxHeapFree(DspHeap, pInpBuf);
        MxHeapFree(DspHeap, pOutBuf);
#endif

#ifdef RUN_IFFT
        // IFFT
        //-----

        // Malloc Input / Output Buffers
        _size = IFFT_DRV_QSIZE * (roundIfftInLen << 2);
        pInpBuf = MxHeapAlloc(DspHeap, _size);
        _ASSERT_PTR(pInpBuf);

        outSize = (pCtx->ifftOutSize << 2);
        numAnt = IFFT_DRV_QSIZE / 14;
        _size = (IFFT_DRV_QSIZE * outSize) + (((2*numAnt * pCtx->cp1) + (12*numAnt * pCtx->cp2)) << 2);

        pOutBuf = MxHeapAlloc(DspHeap, _size);
        _ASSERT_PTR(pOutBuf);
        memset(pOutBuf, 0x42, _size);

        inSize = (pCtx->ifftInSize << 2);
        for (i = 0; i < IFFT_DRV_QSIZE; i++)
        {
            memcpy(pInpBuf + (i*roundIfftInLen), pCtx->ifftInRefPtr, inSize);
        }


        // Call IFFT functions
        trap = 0;
        //uart_printf("Halt IFFT Diag All FP\n");
        while(trap);
        MapIfftDrvSetInpBuf(pInpBuf, IFFT_DRV_QSIZE, 0);
        MapIfftDrvSetOutBuf(pOutBuf, IFFT_DRV_QSIZE, 0);
        MapIfftDrvSetNumAntennas(numAnt);
        MapIfftDrvSetCallback(IfftCallBack, NULL);

        // Dispatch
        IfftDone = 0;
        ticks = SysTimerGetTick();
        Tcb1.isym = 0;
        MapIfftDrvPreRun(&Tcb1);
        MapIfftDrvRun(&Tcb1);
        Tcb2.isym = 1;
        MapIfftDrvPreRun(&Tcb2);
        MapIfftDrvRun(&Tcb2);

        // Wait till done
        while (IfftDone != 0x3);
        ticks = SysTimerGetTick() - ticks;

        trap = 0;
        while(trap);

        // Compare outputs
        bOk = TRUE;
        pRef = (PUINT8)pCtx->ifftOutRefPtr;
        pOut = (PUINT8)pOutBuf;
        outSize = (pCtx->ifftOutSize << 2);
        for (i = 0; i < IFFT_DRV_QSIZE; i++)
        {
            symNum = i%14;
            cpSize = (((symNum == 0 || symNum == 7) ? pCtx->cp1 : pCtx->cp2) << 2);
            if (memcmp(pRef, pOut + cpSize, outSize) ||
                memcmp(pRef + (outSize - cpSize), pOut, cpSize))
            {
#ifndef DEBUGFPDIAG
                bOk = FALSE;
                break;
#else
                {
                    UINT32 ii;
                    PUINT16 pa, pb;

                    uart_printf("IFFT: isym = %d\n1st\n", i);
                    pa = (PUINT16)pRef;
                    pb = (PUINT16)(pOut + cpSize);
                    for (ii = 0; ii < outSize/2; ii++)
                        if (*(pa+ii)!= *(pb+ii))
                        {
                            uart_printf("%d 0x%04x 0x%04x\n", ii/2, *(pa+ii), *(pb+ii));
                            break;
                        }

                    uart_printf("\n2nd\n", i);
                    pa = (PUINT16)(pRef + (outSize - cpSize));
                    pb = (PUINT16)pOut;
                    for (ii = 0; ii < cpSize/2; ii++)
                        if (*(pa+ii)!= *(pb+ii))
                        {
                            uart_printf("%d 0x%04x 0x%04x\n", ii/2, *(pa+ii), *(pb+ii));
                            break;
                        }

                    uart_printf("\n\n");
                    MxDelayTicks(15000000);
                }
                bOk = FALSE;
                //break;
#endif
            }
            pOut += (outSize + cpSize);
        }

        myuart_printf("IFFT %u %s: ExecTicks=%u\n", (1 << pCtx->mode), bOk ? "passed" : "failed", ticks);

        if (bOk == FALSE)
        FpDiagOk = 0;
        IfftPass += bOk ? 1:0;
        IfftFail += bOk ? 0:1;


        // Free allocated memory
        MxHeapFree(DspHeap, pInpBuf);
        MxHeapFree(DspHeap, pOutBuf);
#endif
    }

    myuart_printf("\nTotal FFT Tests run: %d, Pass: %d, Fail: %d\n", FftPass+FftFail, FftPass, FftFail);
    myuart_printf(  "Total IFFT Tests run: %d, Pass: %d, Fail: %d\n", IfftPass+IfftFail, IfftPass, IfftFail);


    return;
}


void FpDiagRunIndividualMaps(void)
{
    UINT32 i, j;
    UINT16 badMaps, goodMaps;
    UINT32 _size, ticks, symNum, fpNum;
    UINT32 cpSize, outSize, inSize;
    UINT32 roundFftOutLen, roundIfftInLen;
    PUINT32 p;
    PUINT8 pRef, pOut;
    MXRC rc;

    // Variables to MapIdftGenerateTables
    UINT16 pIdftSizeArr[1];
    UINT32 ArrSize;

    PUINT8 *pIdftInput, *pIdftOutput;
    UINT32 IdftSizeIndex;

    PUINT32 pInpBuf, pOutBuf;
    BOOL bOk;

    PFPDIAG_FFT_IFFT_CTX pCtx;
    FPDIAG_FFT_IFFT_CTX FpDiagFftIfftCtx;

    PTCB pTcb;
    TCB  Tcb;
    UINT32 IdftSize;
    PUINT8 fp_outref;
    UINT16 idftMask, ifftMask, fftMask;

    UINT16 trap = 0;
    while(trap);

    // Testing 1024 Pt FFT/IFFT and 1200 Pt IDFT on MAPs
    //------------------------------------------
    pCtx = &FpDiagFftIfftCtx;

    FpDiagFftIfftCtx.mode = FPMODE_FFT1024;
    FpDiagFftIfftCtx.numReps = 1;
    FpDiagFftIfftCtx.cp1 = 80;
    FpDiagFftIfftCtx.cp2 = 72;

    FpDiagFftIfftCtx.fftInSize = fp_fft1024_indata_size >> 2;
    FpDiagFftIfftCtx.fftOutSize = 600;
    FpDiagFftIfftCtx.fftInRefPtr = (PUINT8)fp_fft1024_indata;
    FpDiagFftIfftCtx.fftOutRefPtr = (PUINT8)fp_fft1024_outdata;

    FpDiagFftIfftCtx.ifftInSize = fp_ifft1024_indata_size >> 2;
    FpDiagFftIfftCtx.ifftOutSize = 1024;
    FpDiagFftIfftCtx.ifftInRefPtr = (PUINT8)fp_ifft1024_indata;
    FpDiagFftIfftCtx.ifftOutRefPtr = (PUINT8)fp_ifft1024_outdata;

    roundFftOutLen = ROUND((pCtx->fftOutSize + 2), 8);
    roundIfftInLen = ROUND(pCtx->ifftInSize, 32);
    idftMask = ifftMask = fftMask = 0;
    badMaps = goodMaps = 0;

    uart_printf ("Init FP, mode:%d ... ", pCtx->mode);
    rc = MapDrvInit(pCtx->mode, 0);
    uart_printf("rc=%x\r\n", rc);

    if (FAILED(rc))
    {
        uart_printf ("FpDiagRunIndividualMaps is Failed due to MAP initialization\r\n");
        return;
    }

    // Testing 12 Pt IDFT on MAPs
    //------------------------
    MapIdftDrvSetCallback((MAPDONECB)IdftCallBack, NULL);
    IdftSize = 12;             // IDFT size to test

    // Malloc TCBs
    pIdftInput = MxHeapAlloc(DspHeap, IDFT_DRV_QSIZE*sizeof(PUINT8));
    pIdftOutput = MxHeapAlloc(DspHeap, IDFT_DRV_QSIZE*sizeof(PUINT8));

    for (i = 0; i < 1;  i++)
    {
        // Input
        pIdftInput[i] = MxHeapAlloc(DspHeap, 5188); // allocate max size buffer
        _ASSERT_PTR(pIdftInput[i]);
        memcpy(pIdftInput[i], fp_idft_indata, 5184);

        //Output
        pIdftOutput[i] = MxHeapAlloc(DspHeap, 5188); // allocate max size buffer
        _ASSERT_PTR(pIdftOutput[i]);
        memset(pIdftOutput[i], 0xBC, 5188);
    }

    // TCB
    pTcb = &fpDiagTcb;
    _ASSERT_PTR(pTcb);

    // Run
    for (fpNum = DevInfoGetValue(CONSTID_IDFT_FP_ID_START); fpNum <= DevInfoGetValue(CONSTID_IDFT_FP_ID_STOP); fpNum++)
    {
        IdftSizeIndex = fp_idft_size_index_map[IdftSize];
        fp_outref = (PUINT8)fp_idft_outrefs[IdftSizeIndex];

        MapIdftDrvSetFpNum(fpNum);
        uart_printf("Testing %4u pt IDFT on FP #%d: ", IdftSize, fpNum);

        {
            UINT32 flag1 = 0;
            while(flag1);
        }

        pIdftSizeArr[0] = IdftSize;
        ArrSize = 1;
        MapIdftGenerateTables(pIdftSizeArr, ArrSize);

        pTcb->ExecTicks = 0;
        MapIdftDrvSetInBuf(pIdftInput[0], 0, (2*IdftSize*sizeof(short) + 8));
        MapIdftDrvSetOutBuf(pIdftOutput[0], 0, (2*IdftSize*sizeof(short) + 8));
        MapIdftUseQueueFromTcb(0);
        MapIdftQueue(pTcb, 0x1);
        ticks = SysTimerGetTick();

        IdftDone = 0;
        MapIdftDrvRun(pTcb);
        while((IdftDone != 1)&&((SysTimerGetTick() - ticks) < MAX_FP_TIMEOUT));

        ticks = SysTimerGetTick() - ticks;

        if (IdftDone == 1)
        {
            // Completed running the test successfully

            bOk = TRUE;
            for (i = 0; i < 1;  i++)
            {
                if (memcmp(pIdftOutput[i], fp_outref, 2*sizeof(short)*IdftSize) != 0)
                {
                    bOk = FALSE;
                    break;
                }
            }
            uart_printf("%s: ExecTicks=%u\n",
                           (bOk ? "passed" : "failed"), ticks);
            idftMask |= (1<<fpNum);
            goodMaps++;
        }
        else
        {
            // Time out which means device might not be working correctly
            uart_printf("DEVICE TIMEOUT. MAP NOT RESPONDING!!!\n");
            badMaps++;
        }
    }

    for (i = 0; i < 1; i++)
    {
        MxHeapFree(DspHeap, pIdftInput[i]);
        MxHeapFree(DspHeap, pIdftOutput[i]);
    }
    MxHeapFree(DspHeap, pIdftInput);
    MxHeapFree(DspHeap, pIdftOutput);

    // Testing 1024 Pt FFT/IFFT on MAPs
    //----------------------------

    for (fpNum = DevInfoGetValue(CONSTID_FFT_FP_ID_START); fpNum <= DevInfoGetValue(CONSTID_FFT_FP_ID_STOP) ; fpNum++)
    {
        for (j = 0; j < pCtx->numReps; ++j)
        {

#ifdef RUN_FFT
            // FFT
            //-----

            // Malloc Input / Output Buffers
            inSize = (pCtx->fftInSize << 2);
            _size = (inSize) + (pCtx->cp1 << 2);
            pInpBuf = MxHeapAlloc(DspHeap, _size);
            _ASSERT_PTR(pInpBuf);
            memset(pInpBuf, 0x42, _size);

            _size = (roundFftOutLen << 2);
            pOutBuf = MxHeapAlloc(DspHeap, _size);
            _ASSERT_PTR(pOutBuf);
            memset(pOutBuf, 0x42, _size);

            for (i = 0, p = pInpBuf; i < 1; i++, p += pCtx->fftInSize)
            {
                symNum = i%14;
                p += ((symNum == 0 || symNum == 7) ? pCtx->cp1 : pCtx->cp2);
                memcpy(p, pCtx->fftInRefPtr, inSize);
            }

            // Call FFT functions
            trap = 0;

            while(trap);
            MapFftDrvSetInpBuf(pInpBuf, 1, 0);
            MapFftDrvSetOutBuf(pOutBuf, 1, 0);
            MapFftDrvSetFpNum(fpNum);
            MapFftDrvSetNumSym(1);
            MapFftDrvSetNumAntennas(1);
            MapFftDrvSetCallback(FftCbDone, NULL);

            // Dispatch
            FftDone = 0;
            uart_printf("Testing %u pt  FFT on FP #%d: ", (1 << pCtx->mode), fpNum);

            ticks = SysTimerGetTick();
            Tcb.isym = 0;
            MapFftDrvPreRun(&Tcb);
            MapFftDrvRun(&Tcb);
            while ((!FftDone)&&((SysTimerGetTick() - ticks) < MAX_FP_TIMEOUT));
            ticks = SysTimerGetTick() - ticks;

            if (FftDone == 1)
            {
                // Completed running the test successfully

                // Compare outputs
                bOk = TRUE;
                pRef = (PUINT8)pCtx->fftOutRefPtr;
                pOut = (PUINT8)pOutBuf;
                outSize = (pCtx->fftOutSize << 1);
                for (i = 0; i < 1; i++)
                {
                    if (memcmp(pRef + (inSize - outSize), pOut, outSize) ||
                    memcmp(pRef, pOut + outSize, outSize))
                    {
                        bOk = FALSE;
                        break;
                    }
                    pOut += (roundFftOutLen << 2);    //  Next symbol
                }

                pOut = (PUINT8)pOutBuf;
                uart_printf("%s: ExecTicks=%u\n",
                           (bOk ? "passed" : "failed"), ticks);

                fftMask |= (1<<fpNum);
                goodMaps++;
            }
            else
            {
                // Time out which means device might not be working correctly
                uart_printf("DEVICE TIMEOUT. MAP NOT RESPONDING!!!\n");
                badMaps++;
            }

            // Free allocated memory
            MxHeapFree(DspHeap, pInpBuf);
            MxHeapFree(DspHeap, pOutBuf);
#endif // RUN_FFT


#ifdef RUN_IFFT
            // IFFT
            //-----

            // Malloc Input / Output Buffers
            _size = 1 * (roundIfftInLen << 2);
            pInpBuf = MxHeapAlloc(DspHeap, _size);
            _ASSERT_PTR(pInpBuf);

            outSize = (pCtx->ifftOutSize << 2);
            _size = (1 * outSize) + (pCtx->cp1  << 2);

            pOutBuf = MxHeapAlloc(DspHeap, _size);
            _ASSERT_PTR(pOutBuf);
            memset(pOutBuf, 0x42, _size);

            inSize = (pCtx->ifftInSize << 2);
            for (i = 0; i < 1; i++)
            {
                memcpy(pInpBuf + (i*roundIfftInLen), pCtx->ifftInRefPtr, inSize);
            }


            // Call IFFT functions
            MapIfftDrvSetInpBuf(pInpBuf, 1, 0);
            MapIfftDrvSetOutBuf(pOutBuf, 1, 0);
            MapIfftDrvSetNumSym(1);
            MapIfftDrvSetNumAntennas(1);
            MapIfftDrvSetFpNum(fpNum);
            MapIfftDrvSetCallback(IfftCallBack, NULL);

            // Dispatch
            IfftDone = 0;
            uart_printf("Testing %u pt IFFT on FP #%d: ", (1 << pCtx->mode), fpNum);

            ticks = SysTimerGetTick();
            Tcb.isym = 0;
            MapIfftDrvPreRun(&Tcb);
            MapIfftDrvRun(&Tcb);
            while((!IfftDone)&&((SysTimerGetTick() - ticks) < MAX_FP_TIMEOUT));
            ticks = SysTimerGetTick() - ticks;

            if (IfftDone == 1)
            {
                // Completed running the test successfully

                // Compare outputs
                bOk = TRUE;
                pRef = (PUINT8)pCtx->ifftOutRefPtr;
                pOut = (PUINT8)pOutBuf;
                outSize = (pCtx->ifftOutSize << 2);
                for (i = 0; i < 1; i++)
                {
                    symNum = i%14;
                    cpSize = (((symNum == 0 || symNum == 7) ? pCtx->cp1 : pCtx->cp2) << 2);
                    if (memcmp(pRef, pOut + cpSize, outSize) ||
                        memcmp(pRef + (outSize - cpSize), pOut, cpSize))
                    {
                        bOk = FALSE;
                        break;
                    }
                    pOut += (outSize + cpSize);
                }

                uart_printf("%s: ExecTicks=%u\n",
                           (bOk ? "passed" : "failed"), ticks);
                ifftMask |= (1<<fpNum);
            }
            else
            {
                // Time out which means device might not be working correctly
                uart_printf("DEVICE TIMEOUT. MAP NOT RESPONDING!!!\n");
            }

            // Free allocated memory
            MxHeapFree(DspHeap, pInpBuf);
            MxHeapFree(DspHeap, pOutBuf);
#endif // RUN_IFFT
        }
    }


    uart_printf("\n");
    uart_printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    uart_printf("| Number of GOOD MAPS: %2d |\n", goodMaps);
    uart_printf("| Number of BAD MAPS:  %2d |\n", badMaps);
    uart_printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    MapIdftDrvSetMapMask(idftMask);
    MapIfftDrvSetMapMask(ifftMask);
    MapFftDrvSetMapMask(fftMask);

    uart_printf("\n");

    return;
}


