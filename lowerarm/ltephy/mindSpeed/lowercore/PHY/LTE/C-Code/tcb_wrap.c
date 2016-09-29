//-------------------------------------------------------------------------------------------
/** @file tcb_wrap.c
 *
 * @brief TCB wrapper
 * @author Mindspeed Technologies
 * @version $Revision: 1.217 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "ext_phy.h"
#include "tc7000_lte.h"
#include "tc1700.h"
#include "math_ext32.h"
#include "prototypes.h"
#include "papr_rcf.h"
#include "gputil_fx.h"

#include "bs_rx_prach.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

#define USE_TCBQ 1

//Done to remove #ifdefs in USE_TCBQ
#if !defined (USE_TCBQ) || defined(_WIN32)
#define RexCreateTCBQ(a, b, c, d, e, f, g)     CreateTCB(a, b, c, d, e, f)
#endif

#ifndef _WIN32
#include "mdmadrv.h"

//#define CHECKALIGN

#ifdef CHECKALIGN
#define CHECKALIGN32(x, y, z) do {U32 a = ((U32)(x)+31)&~31; if (a!= (U32)(x)) phy_printf("CHECKALIGN32 - %s: %s not aligned 0x%x 0x%x %d bytes\n", (z), (y), (U32)(x), a, (a - (U32)(x)));} while (0)
#endif

#else

#define MxCacheClean(x,y)

#endif

#ifndef SCHEDULER
int CEVA_Proc(TCB* myTCB, int dspnum)
{
    return 0;
}
#endif

//----------------------------------------------------------------------------------------
// External Variables
// ----------------------
extern S16 PRACH_Tables[6720];

//----------------------------------------------------------------------------------------

#ifdef WIN32
#define REX_BLOCKING_CALL 0
#endif

//----------------------------------------------------------------------------------------
/** @brief GetTick   Returns value of clock for AXI Bus
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------
 *  |Name       |Name In  |Direction  |Length |Type          |Format  |Valid Range|Units |
 *  |           |Standard |           |       |              |        |           |      |
 *  --------------------------------------------------------------------------------------
 *  |return     |         |Output     |1      |real          |(1:31:0)|[0,2^32)   |      |
 *  --------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------
 *  |Block Name          |GetTick              |
 *  --------------------------------------------
 *  |Input Stream(s)     |                     |
 *  --------------------------------------------
 *  |Output Stream(s)    |                     |
 *  --------------------------------------------
 *  |Config Parameter(s) |                     |
 *  --------------------------------------------
 *  |Status Parameter(s) |return value         |
 *  --------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *  Returns value of clock for AXI Bus
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_tcb
 *
 *
 **/
//------------------------------------------------------------------------------------------
U32 GetTick(void)
{
    return *(volatile U32 *) 0xFE050004;
}

//----------------------------------------------------------------------------------------
/** @brief GetExecTicks   returns the number of cycles between the last call of GetTicks to
 *                        call of GetExecTicks.
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------
 *  |Name       |Name In  |Direction  |Length |Type          |Format  |Valid Range|Units |
 *  |           |Standard |           |       |              |        |           |      |
 *  --------------------------------------------------------------------------------------
 *  |start_tick |         |Input      |1      |real          |(1:31:0)|[0,2^32)   |      |
 *  --------------------------------------------------------------------------------------
 *  |return     |         |Output     |1      |real          |(1:31:0)|[0,2^32)   |      |
 *  --------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------
 *  |Block Name          |GetExecTicks         |
 *  --------------------------------------------
 *  |Input Stream(s)     |                     |
 *  --------------------------------------------
 *  |Output Stream(s)    |                     |
 *  --------------------------------------------
 *  |Config Parameter(s) |start_tick           |
 *  --------------------------------------------
 *  |Status Parameter(s) |return value         |
 *  --------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *  Returns the number of cycles between the last call of GetTicks to
 *  call of GetExecTicks.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 *
 * <6> References:
 *
 *
 * \ingroup group_lte_tcb
 *
 *
 **/
//------------------------------------------------------------------------------------------
U32 GetExecTicks(U32 start_tick)
{
    volatile U32 t;
    t = GET_TICKS;
    return (t - start_tick);
}
//------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// UpLink TCB Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// PUSCH TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WIN32
//Running on EVM
////////////////////////////////////////////////////////////////////////////////////////
/** @brief fft_tcb_hw TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                 |Scheduled on      |
 *  |                        |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_FFT              |RESOURCE_FLTP     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the FFT function.<BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//----------------------------------------------------------------------------------------
PTCB fft_tcb_hw(PTASKLIST pTskLst, PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend)
{

    PTCB pTCB = 0;
    S16 i;

    pTCB = CreateTCB(TASKID_RX_FFT, RESOURCE_FLTP, NULL, NULL, NULL, NULL);

    AddTCBToList(pTCB, pTskLst, NULL);
    if (pTaskListDependencyTo != NULL)
    {
        for (i = 0; i < numDepend; i++)
        {
            if (pTaskListDependencyTo[DependArray[i]] != NULL)
                AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
        }
    }

    return pTCB;

}
#else   //_WIN32
//Running with C-Code or Ceva Sim
////////////////////////////////////////////////////////////////////////////////////////
/** @brief fft_Win32 TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                 |Scheduled on      |
 *  |                        |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_FFT              |RESOURCE_FLTP     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the FFT function.<BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//----------------------------------------------------------------------------------------
void fft_Win32(S16 expo, S16 length,
                  S16 **adcbuf, S16* aDet,
                  S16 Nul_symb, S16 MO, S32 step_aDet, S16 L, S16 K,
                  S16 Ncp, S16 Ncp0)
{
    S16 islot, iSymInSubframe, isym, ncp_sym;
    S16 mo;
    S16 *adcMhcs;
    S16 *fft_out;
    S32 adcbufPtr = 0;
    S32 KK = K/2;
    S16 *mhcs;
    S16 expo_mat;

#ifdef DEBUG_PRINT
    double max = 0.0;
#endif

    // Memory Allocate
    adcMhcs = MemAlloc(2 * length * sizeof(S16), CRAM_MEM, NULL);
    fft_out = MemAlloc(2 * length * sizeof(S16), CRAM_MEM, NULL);
    mhcs    = MemAlloc(2 * length * sizeof(S16), CRAM_MEM, NULL);

    //Generate Half Cyclic Shift
    phy_printf("\nRunning GenHalfCyclicShift\n\n");
    GenHalfCyclicShift(mhcs, expo, length);

    for (islot = 0; islot <= 1; islot++)
    {
        // Comment by ArrayComm, aDet[] is filled from aDet[0] even for TDD UpPTS
        iSymInSubframe = islot * Nul_symb;
        for(isym = 0; isym < Nul_symb; isym++)
        {
            if (isym)
                ncp_sym = Ncp;
            else
                ncp_sym = Ncp0;

            adcbufPtr += ncp_sym;

            for(mo = 0; mo < MO; mo++)
            {
                Remove_HalfCyclicShift(&adcbuf[mo][2*adcbufPtr], mhcs, adcMhcs, length);

#ifdef DEBUG_PRINT
                phy_printf("Running FFT MO:[%d], isym: [%d], islot: [%d]\n", mo, isym, islot);
#endif
                fftcg_c_16t24d(adcMhcs, expo, fft_out, &expo_mat);

                SwapFFT_Out(fft_out, &aDet[iSymInSubframe*step_aDet + mo*step_aDet*L], KK, (S32)(2*length - K), &expo_mat);

#ifdef DEBUG_PRINT
                filePrint(fFFT_In, adcMhcs, length, "S16", "Complex", (double)(pow(2.0,10.0)), LOG_BS_FFTIN_FX);
                debugExpo[islot*Nul_symb + isym][mo][numDebugExpo] = expo_mat ;
                max = pow(2., (double)expo_mat);
                filePrint(fFFT_Out, fft_out, length, "S16", "Complex", (double)(pow(2.0,11.0))/(double)(max), LOG_BS_FFTOUT_FX);
                filePrint(fAdet, &aDet[iSymInSubframe * step_aDet + mo * step_aDet * L],
                                K, "S16", "Complex", (double)(pow(2.0,11.0))/(double)(max), LOG_BS_ADET_FX);
#endif
            }
            adcbufPtr += length;
            iSymInSubframe = iSymInSubframe + 1;
        }
    }

    // Memory Free
    MemFree(adcMhcs, CRAM_MEM);
    MemFree(fft_out, CRAM_MEM);
    MemFree(mhcs, CRAM_MEM);
}
#endif // _WIN32

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink Pilots TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                 |Scheduled on      |
 *  |                        |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_ULPILOT       |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Uplink  Pilots function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: ul_Pilot_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB ul_Pilot_fx_tcb(PTASKLIST pTskLst, S16 a[], ULRXINPUTPARAMS *pULPilotInput, U8 cvec_u[],
                     U8 PRSbits[], S16 bitRevTable[], S16 pUlPilots_QthRootTable[],
                     U32 *rs_grid, U32 stepsize_aDet, PTCB pTaskListDependencyTo,
                     U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32* pContext = 0;

    S16 K  = pULPilotInput->K;
    S16 L  = pULPilotInput->L;

    if (!RefillFlag)
    {

#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_ULPILOT, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL, TIME_RX_ULPILOT);
#else        //USECEVA
        pTCB = CreateTCB(TASKID_RX_ULPILOT, RESOURCE_LARM, NULL, 1 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_ULPILOT, &ul_Pilot_task);
#endif       //USECEVA

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = stepsize_aDet;

        CreateTCBIOBuf(pTCB, 7);
        FillTCBIOBuf(pTCB, (U32*) a,                      K * 2 * 2 * sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pULPilotInput,          sizeof(ULRXINPUTPARAMS), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) cvec_u,                 7 * NUM_SLOTS, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) PRSbits,                7 * NUM_SLOTS, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) bitRevTable,            256 * 2, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pUlPilots_QthRootTable, 220 * 30 * 2 * 2, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rs_grid,                L * MAXCHSUBFRAME * sizeof(U32), INOUT_BUF, NULL);

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);
    }
    else
    {
        pTCB = pTcbRefill;

        if (pULPilotInput->numPUSCH == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = stepsize_aDet;

            ReFillTCBIOBuf(pTCB, (U32*) a,                      K * 2 * 2 * sizeof(S16), OUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) pULPilotInput,          sizeof(ULRXINPUTPARAMS), IN_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) cvec_u,                 7 * NUM_SLOTS, IN_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) PRSbits,                7 * NUM_SLOTS, IN_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) bitRevTable,            256 * 2, IN_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) pUlPilots_QthRootTable, 220 * 30 * 2 * 2, IN_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) rs_grid,                L * MAXCHSUBFRAME * sizeof(U32), INOUT_BUF, 6);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    //generate pilots and add to the resource grid
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            ul_Pilot_task(pTCB);
    }

#endif  //SCHEDULER
    return pTCB;

}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Channel Estimation Part0 TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST      |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Channel Estimation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_part0_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB bs_rx_ChEstimator_part0_fx_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *aPilots,
                                    ULRXINPUTPARAMS *pChanEstPar, S32 stepsize_aDet,
                                    S16 *chEst, S16 *pScaleComputeErr_Tab, S32 Err[],
                                    S16 antenna, S16 slot,
                                    S16 *expo_mat, PTCB pTaskListDependencyTo[], S16 *DependArray,
                                    S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32* pContext = 0;
    S32 i = 0, numPusch;
    S16 MO, L, K;

    K = pChanEstPar->K;
    L = pChanEstPar->L;
    MO = pChanEstPar->MO;
    numPusch = pChanEstPar->numPUSCH;

    if (!RefillFlag)
    {

    //Construct TCB here
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_CHANEST_P0, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL, TIME_RX_CHANEST_P0);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_CHANEST_P0, RESOURCE_LARM, NULL, 3 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_CHANEST_P0, &bs_rx_ChEstimator_part0_fx_task);
#endif //USECEVA

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = stepsize_aDet;
        pContext[1] = antenna;
        pContext[2] = slot;

        CreateTCBIOBuf(pTCB, 8);
        FillTCBIOBuf(pTCB, (U32*) adet,                 stepsize_aDet * L * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rs_grid,              L * numPusch * sizeof(U32), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) aPilots,              K * 4 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pChanEstPar,          sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) chEst,                K * 4 * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pScaleComputeErr_Tab, 100 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) Err,                  8 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) expo_mat,             2 * sizeof(S16), INOUT_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] ChanEst_part0 to List\n", TASKID_RX_CHANEST_P0);
#endif
        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
            for (i = 0; i < numDepend; i++)
            {
                if (pTaskListDependencyTo[DependArray[i]] != NULL)
                    AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
            }
        }
    }
    else
    {
        S16 MO_1;
        pTCB = pTcbRefill;

        //MO_1 is a parameter for running c-code where number of RxAnt can change from
        //subframe to subframe for testing purposes.  On device, MO_1 will always remain
        //constant like the parameter MO.
        MO_1 = pChanEstPar->MO_1;
        if ((pChanEstPar->numPUSCH == 0) || (antenna >= MO_1))
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = stepsize_aDet;
            pContext[1] = antenna;
            pContext[2] = slot;

            ReFillTCBIOBuf(pTCB, (U32*) adet,                 stepsize_aDet * L * sizeof(S16), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) rs_grid,              L * numPusch * sizeof(U32), IN_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) aPilots,              K * 4 * sizeof(S16), IN_BUF, 2);

            ReFillTCBIOBuf(pTCB, (U32*) pChanEstPar,          sizeof(ULRXINPUTPARAMS), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) chEst,                K * 4 * sizeof(S16), INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) pScaleComputeErr_Tab, 100 * sizeof(S16), IN_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) Err,                  8 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, 6);

            ReFillTCBIOBuf(pTCB, (U32*) expo_mat,             2 * sizeof(S16), INOUT_BUF, 7);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            bs_rx_ChEstimator_part0_fx_task(pTCB);
    }

#endif  //SCHEDULER
    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Channel Estimation Part1 TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST      |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Channel Estimation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_part1_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB bs_rx_ChEstimator_part1_fx_tcb(PTASKLIST pTskLst,
                                    S16 *derotation, ULRXINPUTPARAMS *pChanEstPar, S32 stepsize_aDet,
                                    S16 *chEst,  S32 Err[],
                                    S16 antenna, S16 slot, S32 *snrPerRb_Pilot, S32 *snrPilot, S32 *noiseVar,
                                    S16 *mErrExpoTemp,  S32 *mErrTemp, S32 *rbPwrAvg_AntSlot, U8 *rbUsedFlag, S32 *rssi_AntSlot,
                                    PTCB pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32* pContext = 0;
    S16 MO, K;

    K = pChanEstPar->K;
    MO = pChanEstPar->MO;

    if (!RefillFlag)
    {

    //Construct TCB here
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_CHANEST_P1, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL, TIME_RX_CHANEST_P1);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_CHANEST_P1, RESOURCE_LARM, NULL, 3 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_CHANEST_P1, &bs_rx_ChEstimator_part1_fx_task);
#endif //USECEVA

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = stepsize_aDet;
        pContext[1] = antenna;
        pContext[2] = slot;

        CreateTCBIOBuf(pTCB, 12);
        FillTCBIOBuf(pTCB, (U32*) derotation,           2 * MAXSUBCARRIERS * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pChanEstPar,          sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) chEst,                K * 4 * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) Err,                  2 * 2 * MAXCHSUBFRAME *  sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) snrPerRb_Pilot,       100 * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) snrPilot,             4 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) noiseVar,             2 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) mErrExpoTemp, MAXCHSUBFRAME * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) mErrTemp,       2  * MAXCHSUBFRAME * sizeof(S32), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rbPwrAvg_AntSlot,     2 * MO * MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rbUsedFlag,           2 * MO * MAXRESOURCEBLOCKS * sizeof(U8), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rssi_AntSlot,         2 * MO * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] ChanEst_part1 to List\n", TASKID_RX_CHANEST_P1);
#endif
        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);
    }
    else
    {
        S16 MO_1;
        pTCB = pTcbRefill;

        //MO_1 is a parameter for running c-code where number of RxAnt can change from
        //subframe to subframe for testing purposes.  On device, MO_1 will always remain
        //constant like the parameter MO.
        MO_1 = pChanEstPar->MO_1;
        if ((pChanEstPar->numPUSCH == 0) || (antenna >= MO_1))
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = stepsize_aDet;
            pContext[1] = antenna;
            pContext[2] = slot;


            ReFillTCBIOBuf(pTCB, (U32*) derotation,          2 * MAXSUBCARRIERS * sizeof(S16), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) pChanEstPar,          sizeof(ULRXINPUTPARAMS), INOUT_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) chEst,                K * 4 * sizeof(S16), INOUT_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) Err,                  2 * 2 * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) snrPerRb_Pilot,       100 * sizeof(S32), INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) snrPilot,             4 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) noiseVar,             2 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, 6);
            ReFillTCBIOBuf(pTCB, (U32*) mErrExpoTemp, MAXCHSUBFRAME * sizeof(S16), IN_BUF, 7);
            ReFillTCBIOBuf(pTCB, (U32*) mErrTemp,       2  * MAXCHSUBFRAME * sizeof(S32), IN_BUF, 8);
            ReFillTCBIOBuf(pTCB, (U32*) rbPwrAvg_AntSlot,     2 * MO * MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, 9);
            ReFillTCBIOBuf(pTCB, (U32*) rbUsedFlag,           2 * MO * MAXRESOURCEBLOCKS * sizeof(U8), INOUT_BUF, 10);
            ReFillTCBIOBuf(pTCB, (U32*) rssi_AntSlot,         2 * MO * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 11);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            bs_rx_ChEstimator_part1_fx_task(pTCB);
    }
#endif //SCHEDULER
    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Err RunningAverage0 TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST      |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Channel Estimation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: RunningAverage() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB RunningAverage_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInParams, S32 *ErrAvgIn, S16 *ErrExpoPusch,
                        S16 *ErrExpo,  S32 *ErrAvgOut, S16 *mErrExpoTemp,  S32 *mErrTemp, S16 Ns, PTCB pTaskListDependencyTo[], S16 *DependArray,
                        S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32* pContext = 0;
    S32 i, numChan, L, MO;

    numChan = pRxInParams->numChan;
    L = pRxInParams->L;
    MO = pRxInParams->MO;

    if (!RefillFlag)
    {
    //Construct TCB here
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_RUNNING_AVG, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL, TIME_RX_RUNNING_AVG);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_RUNNING_AVG, RESOURCE_LARM, NULL, 1 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_RUNNING_AVG, &RunningAverage_task);
#endif

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = Ns;

        CreateTCBIOBuf(pTCB, 7);
        FillTCBIOBuf(pTCB, (U32*) pRxInParams,  sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrAvgIn,       8 * MO * MAXCHSUBFRAME * sizeof(S32), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrExpoPusch, MO * L * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrExpo, MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrAvgOut,       2 * 2  * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) mErrExpoTemp, MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) mErrTemp,       2  * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] RunningAverage to List\n", TASKID_RX_RUNNING_AVG);
#endif
        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
            for (i = 0; i < numDepend; i++)
            {
                if (pTaskListDependencyTo[DependArray[i]] != NULL)
                    AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
            }
        }
    }
    else
    {
        pTCB = pTcbRefill;

        if (pRxInParams->numChan == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = Ns;

            ReFillTCBIOBuf(pTCB, (U32*) pRxInParams,  sizeof(ULRXINPUTPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) ErrAvgIn,        8 * MO * numChan * sizeof(S32), IN_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) ErrExpoPusch, MO * L * sizeof(S16), IN_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) ErrExpo, 1 * sizeof(S16), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) ErrAvgOut,      2 * 2 * numChan * sizeof(S32), INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) mErrExpoTemp, MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) mErrTemp,       2  * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 6);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            RunningAverage_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx RunningAveragePUCCH TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST      |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx RunningAveragePUCCH function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: RunningAveragePUCCH() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB RunningAveragePUCCH_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInParams, S32 *ErrAvgIn, S16 *ErrExpoPucch,
                        S32 *ErrAvgOut,  S32 *ErrSub, S16 *ErrSubExpo, PTCB pTaskListDependencyTo[], S16 *DependArray,
                        S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    S32 i, numChan, L, MO;

    numChan = pRxInParams->numChan;
    L = pRxInParams->L;
    MO = pRxInParams->MO;

    if (!RefillFlag)
    {
    //Construct TCB here
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_RUNNING_AVG_PUCCH, RESOURCE_CEVA, NULL, 0 * sizeof(U32), NULL, NULL, TIME_RX_RUNNING_AVG_PUCCH);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_RUNNING_AVG_PUCCH, RESOURCE_LARM, NULL, 0 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_RUNNING_AVG_PUCCH, &RunningAveragePUCCH_task);
#endif
        CreateTCBIOBuf(pTCB, 6);
        FillTCBIOBuf(pTCB, (U32*) pRxInParams,  sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrAvgIn,       8 * MO * MAXCHSUBFRAME * sizeof(S32), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrExpoPucch, MO * L * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrAvgOut,      2 * 2  * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrSub,                2*2*2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrSubExpo,                2*2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] RunningAveragePUCCH to List\n", TASKID_RX_RUNNING_AVG_PUCCH);
#endif
        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
            for (i = 0; i < numDepend; i++)
            {
                if (pTaskListDependencyTo[DependArray[i]] != NULL)
                    AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
            }
        }
    }
    else
    {
        pTCB = pTcbRefill;

        if (pRxInParams->numChan == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass
            
            ReFillTCBIOBuf(pTCB, (U32*) pRxInParams,  sizeof(ULRXINPUTPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) ErrAvgIn,        8 * MO * numChan * sizeof(S32), IN_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) ErrExpoPucch, MO * L * sizeof(S16), IN_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) ErrAvgOut,      2 * 2 * numChan * sizeof(S32), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) ErrSub,                2*2*2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) ErrSubExpo,                2*2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 5);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            RunningAveragePUCCH_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Snr Subframe TCB Function.
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST      |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx SNR calculation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SnrSubframe() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB SnrSubframe_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInPar, S32 *snrPerRb, S32 *snrPilot,
                            PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend,
                            U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    S32 i, numChan, numInputs;

    numChan = pRxInPar->numChan;
    numInputs  = 3;

    if (!RefillFlag)
    {
        //Construct TCB here
        // IMPORTANT: WE need 1 Context for this because we are filling it with a timer used for PUCCH List execution time
#ifdef USECEVA
#ifdef WIN32
        numInputs = 3 + MAXCHSUBFRAME;
        pTCB = CreateTCB(TASKID_RX_SNRSUBFRAME, RESOURCE_CEVA, NULL, 2*sizeof(U32), NULL, NULL);
#else //CB
        pTCB = RexCreateTCBQ(TASKID_RX_SNRSUBFRAME, RESOURCE_CEVA, NULL, 2*sizeof(U32), (REXTCBCBDONE)SNRDone, NULL, TIME_RX_SNRSUBFRAME);
#endif // WIN32
#else //USECEVA
#ifdef WIN32
        numInputs = 3 + MAXCHSUBFRAME;
        pTCB = CreateTCB(TASKID_RX_SNRSUBFRAME, RESOURCE_LARM, NULL, 2*sizeof(U32), NULL, NULL);
#else //CB
        pTCB = CreateTCB(TASKID_RX_SNRSUBFRAME, RESOURCE_LARM, NULL, 2*sizeof(U32), SNRDone, NULL);
#endif
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_SNRSUBFRAME, &SnrSubframe_task);
#endif

        CreateTCBIOBuf(pTCB, numInputs);
        FillTCBIOBuf(pTCB, (U32*) pRxInPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) snrPerRb, 2 * 100 * pRxInPar->MO * sizeof(S32), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) snrPilot, 2 * 2 * pRxInPar->MO * MAXCHSUBFRAME * sizeof(S32), IN_BUF, NULL);

#ifdef _WIN32
        //For Ceva Sim
        for(i = 0; i < MAXCHSUBFRAME; i++)
            FillTCBIOBuf(pTCB, (U32*) pRxInPar->usedIndexVars[i], sizeof(USED_INDEX_VARS), INOUT_BUF, NULL);

        phy_printf("Adding [%3d] SnrSubframe to List\n", TASKID_RX_SNRSUBFRAME);
#endif
        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
            for (i = 0; i < numDepend; i++)
            {
                if (pTaskListDependencyTo[DependArray[i]] != NULL)
                    AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
            }
        }
    }
    else
    {
        pTCB = pTcbRefill;

        if (pRxInPar->numChan == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            ReFillTCBIOBuf(pTCB, (U32*) pRxInPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) snrPerRb, 2 * 100 * pRxInPar->MO * sizeof(S32), IN_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) snrPilot, 2 *2 * pRxInPar->MO * numChan * sizeof(S32), IN_BUF, 2);

#ifdef _WIN32
            //For Ceva Sim
            for(i = 0; i < numChan; i++)
                ReFillTCBIOBuf(pTCB, (U32*) pRxInPar->usedIndexVars[i], sizeof(USED_INDEX_VARS), INOUT_BUF, (i + 3));
#endif
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            SnrSubframe_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx RipRssiSubframe TCB Function.
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST      |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx RipRssi calculation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: RipRssiSubframe() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB RipRssiSubframe_tcb(PTASKLIST pTskLst, ULRXINPUTPARAMS *pRxInPar,S16 *aDet, S32 *rxInRbPwrAvg_AntSlot, U8 *rbUsedFlag,
                          S32 *rxInRIP, S32 step_aDet, S16 srsPilot, S16 *InitFlag, S16 *expo_mat, S32 *rssi_AntSlot,
                          PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32 *pContext = 0;
    S32 i;
    S16 MO, L, Nul_symb;


    MO = pRxInPar->MO;
    L = pRxInPar->L;
    Nul_symb = pRxInPar->Nul_symb;

    if (!RefillFlag)
    {
    //Construct TCB here
#ifdef USECEVA
#ifdef _WIN32
        pTCB = RexCreateTCBQ(TASKID_RX_RIPRSSISUBFRAME, RESOURCE_CEVA, NULL, 6 * sizeof(U32), NULL, NULL, TIME_RX_RIPRSSISUBFRAME);
#else
        pTCB = RexCreateTCBQ(TASKID_RX_RIPRSSISUBFRAME, RESOURCE_CEVA, NULL, 6 * sizeof(U32), (REXTCBCBDONE)RipRssiMeasDone, NULL, TIME_RX_RIPRSSISUBFRAME);
#endif
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_RIPRSSISUBFRAME, RESOURCE_LARM, NULL, 6 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_RIPRSSISUBFRAME, &RipRssiSubframe_task);
#endif

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = step_aDet;
        pContext[1] = srsPilot;
        pContext[2] = (U32)pRxInPar;

        CreateTCBIOBuf(pTCB, 8);
        FillTCBIOBuf(pTCB, (U32*) pRxInPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) aDet,                 MO * step_aDet * L * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rxInRbPwrAvg_AntSlot, 2 * MO * MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rbUsedFlag,           2 * MO * MAXRESOURCEBLOCKS * sizeof(U8), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rxInRIP,              MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) InitFlag,             sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) expo_mat,             2 * MO * Nul_symb * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rssi_AntSlot,         2 * MO * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
            for (i = 0; i < numDepend; i++)
            {
                if (pTaskListDependencyTo[DependArray[i]] != NULL)
                    AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
            }
        }
    }
    else
    {
        pTCB = pTcbRefill;
        if (pRxInPar->rxIntMeasFlag == 0 && pRxInPar->rssiMeasFlag == 0 && pRxInPar->rxTherNoiseFlag == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = step_aDet;
            pContext[1] = srsPilot;
            pContext[2] = (U32)pRxInPar;

            ReFillTCBIOBuf(pTCB, (U32*) pRxInPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) aDet,                 MO * step_aDet * L * sizeof(S16), INOUT_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) rxInRbPwrAvg_AntSlot, 2 * MO * MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) rbUsedFlag,           2 * MO * MAXRESOURCEBLOCKS * sizeof(U8), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) rxInRIP,              MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) InitFlag,             sizeof(S16), INOUT_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) expo_mat,             2 * MO * Nul_symb * sizeof(S16), INOUT_BUF, 6);
            ReFillTCBIOBuf(pTCB, (U32*) rssi_AntSlot,         2 * MO * MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 7);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            RipRssiSubframe_task(pTCB);
    }
#endif //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief rx_Multichan_DeModulation_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  -------------------------------------------------------
 *  |Task ID                           |Scheduled on      |
 *  |                                  |Hardware Resource |
 *  -------------------------------------------------------
 *  |TASKID_RX_MULTICHAN_DEMODULATION |RESOURCE_CEVA     |
 *  -------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Rx MultiChannel Demodulation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: rx_Multichan_DeModulation() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB rx_Multichan_DeModulation_tcb(PTASKLIST pTskLst, S16 *aDet, U32 *rs_grid, S16 *chEst_ul,
                                   S16 *derotation, S16 *reDet0, S16* expo_reEst_norm,
                                   S32 step_aDet, S32 step_reDet, S16 isym, ULRXINPUTPARAMS *pRxInParams,
                                   S16 srsPilot, S32 *noiseVar, PTCB pTaskListDependencyTo[],
                                   S16 *DependArray, S16 numDepend,
                                   U16 RefillFlag, PTCB pTcbRefill)
{
    S32 i, j = 0;
    PTCB pTCB = 0;
    U32 *pContext = 0;
    S32 numPusch, L, MO, MI, K;

    // Multichan Demodulation Task to consolidate all Ceva TCBs into 1 Single TCB
    numPusch = pRxInParams->numPUSCH;
    K = pRxInParams->K;
    L = pRxInParams->L;
    MO = pRxInParams->MO;
    MI = pRxInParams->MI;

    if (!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_MULTICHAN_DEMODULATION, RESOURCE_CEVA, NULL, 5 * sizeof(U32),
                         NULL, NULL, TIME_RX_MULTICHAN_DEMODULATION);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_MULTICHAN_DEMODULATION, RESOURCE_LARM, NULL, 5 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_MULTICHAN_DEMODULATION, &rx_Multichan_DeModulation_task);
#endif

        pContext = pTCB->ContextPtr;
        pContext[0] = isym;
        pContext[1] = step_aDet;
        pContext[2] = MO;
        pContext[3] = srsPilot;
        pContext[4] = step_reDet;

        pTCB->SubTaskTimer = 1; // Set to 1 so that sub tasks will be logged in

        CreateTCBIOBuf(pTCB, MO + 7);
        for (i = 0; i < MO; i++)
        {
            FillTCBIOBuf(pTCB, (U32*) (&aDet[step_aDet * L * i]),
                         step_aDet * L * sizeof(S16), IN_BUF, NULL);
        }

        FillTCBIOBuf(pTCB, (U32*) (rs_grid),         L * MAXCHSUBFRAME * sizeof(U32), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (chEst_ul),        2 * K * 2 * MO * MI * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (derotation),      2 * MAXSUBCARRIERS * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (reDet0),          step_reDet * (L - 2) * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (expo_reEst_norm), MAXCHSUBFRAME * L * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxInParams),     sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (noiseVar),        2 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, NULL);

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
           for (i = 0; i < numDepend; i++)
           {
               if (pTaskListDependencyTo[DependArray[i]] != NULL)
                   AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
           }
        }
    }
    else
    {
        pTCB = pTcbRefill;

        if (pRxInParams->numPUSCH == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = pTCB->ContextPtr;
            pContext[0] = isym;
            pContext[1] = step_aDet;
            pContext[2] = MO;
            pContext[3] = srsPilot;
            pContext[4] = step_reDet;

            pTCB->SubTaskTimer = 1; // Set to 1 so that sub tasks will be logged in

            j = 0;
            for (i = 0; i < MO; i++)
            {
                ReFillTCBIOBuf(pTCB, (U32*) (&aDet[step_aDet * L * i]),
                             step_aDet * L * sizeof(S16), IN_BUF, j++);
            }

           ReFillTCBIOBuf(pTCB, (U32*) (rs_grid),         L * numPusch * sizeof(U32), IN_BUF, j++);
           ReFillTCBIOBuf(pTCB, (U32*) (chEst_ul),        2 * K * 2 * MO * MI * sizeof(S16), IN_BUF, j++);
           ReFillTCBIOBuf(pTCB, (U32*) (derotation),      2 * MAXSUBCARRIERS * sizeof(S16), IN_BUF, j++);
           ReFillTCBIOBuf(pTCB, (U32*) (reDet0),          step_reDet * (L - 2) * sizeof(S16), INOUT_BUF, j++);
           ReFillTCBIOBuf(pTCB, (U32*) (expo_reEst_norm), MAXCHSUBFRAME * L * sizeof(S16), INOUT_BUF, j++);
           ReFillTCBIOBuf(pTCB, (U32*) (pRxInParams),     sizeof(ULRXINPUTPARAMS), INOUT_BUF, j++);
           ReFillTCBIOBuf(pTCB, (U32*) (noiseVar),        2 * MAXCHSUBFRAME * MO * sizeof(S32), INOUT_BUF, j++);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            rx_Multichan_DeModulation_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE iDFT DeSpreading TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_IDFT            |RESOURCE_FLTP     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE iDFT DeSpreading function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: IDFT() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB IDFT_tcb(PTASKLIST pTskLst, S16 *input, S16 *output, S16 isym, S16 srsPilot,
                 PMAP_SETUP_DESCRIPTOR pMapSetupDesc, S32 step_reDet, U16 *idftSize, S16 numPusch, U32 totalIdftSize,
                 PTCB *pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    S32 i = 0;

#ifndef _WIN32  // Device Mode

    if (!RefillFlag)
    {
        //Construct TCB here
        pTCB = CreateTCB(TASKID_RX_IDFT, RESOURCE_FLTP, NULL, 0, NULL, NULL);

        CreateTCBIOBuf(pTCB, 2);
        FillTCBIOBuf(pTCB, (U32*) pMapSetupDesc->pReadyListHead, sizeof(U32)*FPDRV_NUM_FP, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pMapSetupDesc->pReadyListTail, sizeof(U32)*FPDRV_NUM_FP, IN_BUF, NULL);

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
            for (i = 0; i < srsPilot; i++)
            {
                AddDependency(pTCB, 1, pTaskListDependencyTo[i]);
            }
        }
    }

#else // _WIN32 Ceva Sim Mode

    U32* pContext = 0, j = 0;

    if (!RefillFlag)
    {
        //Construct TCB here

        //For Ceva Sim mode, we don't have RESOURCE_FLTP on PC
        pTCB = CreateTCB(TASKID_RX_IDFT, RESOURCE_LARM, NULL, 4 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_IDFT, &IDFT_task);

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = numPusch;
        pContext[1] = step_reDet;
        pContext[2] = isym;
        pContext[3] = srsPilot;

        CreateTCBIOBuf(pTCB, 3);
        FillTCBIOBuf(pTCB, (U32*) input,  sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) output, sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) idftSize, numPusch * sizeof(U16), IN_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] iDFT to List\n", TASKID_RX_IDFT);
#endif

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo[0]);
    }
    else
    {
        pTCB = pTcbRefill;

        if (numPusch == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = numPusch;
            pContext[1] = step_reDet;
            pContext[2] = isym;
            pContext[3] = srsPilot;

            ReFillTCBIOBuf(pTCB, (U32*) input,  sizeof(S16*), IN_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) output, sizeof(S16*), OUT_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) idftSize, numPusch * sizeof(U16), IN_BUF, 2);
        }
    }
#endif // _WIN32

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            IDFT_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Demapper TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_DEMAPPER    |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Demapper function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: rx_Demapper_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB rx_Demapper_fx_tcb(PTASKLIST pTskLst, S16 reEst1[], S16 *expo_reEst, PULRXFECPARAMS pFecParams,
                        ULRXINPUTPARAMS *pRxInParams, S32 isym, S16 srsPilot, U32 step_reDet,
                        S8 *scrSeq, S32 *pDescrIndex, S8 *pMuxSoftDec,
                        PTCB pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32* pContext = 0;
    S32 numPusch, L, numVar;

    numPusch = pRxInParams->numPUSCH;
    L = pRxInParams->L;
    numVar = 7;

    if(!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_DEMAPPER, RESOURCE_CEVA, NULL, 3 * sizeof(S32), NULL, NULL, TIME_RX_DEMAPPER);
#else  //USECEVA
        pTCB = CreateTCB(TASKID_RX_DEMAPPER, RESOURCE_LARM, NULL, 3 * sizeof(S32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_DEMAPPER, &rx_Demapper_fx_task);
#endif  //USECEVA

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = isym;
        pContext[1] = srsPilot;
        pContext[2] = step_reDet;

#ifdef _WIN32
        CreateTCBIOBuf(pTCB, numVar + MAXCHSUBFRAME);
#else
        CreateTCBIOBuf(pTCB, numVar);
#endif
        FillTCBIOBuf(pTCB, (U32*) reEst1,      step_reDet * (L - 2) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) expo_reEst,  MAXCHSUBFRAME * L * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pRxInParams, sizeof(ULRXINPUTPARAMS), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pFecParams,  sizeof(ULRXFECPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) scrSeq,      MAX_INPUT_TRANSPORT_BYTE_SIZE * sizeof(S8), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pDescrIndex, numPusch * sizeof(S32), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pMuxSoftDec, MAX_MUXCTRL_SOFT_DEC * sizeof(S8), INOUT_BUF, NULL);

#ifdef _WIN32
        {
            S16 Qm, Msc;
            S32 i;
            S8 *bDetSoft;

            for(i = 0; i < MAXCHSUBFRAME; i++)
            {
                Msc      = pRxInParams->Mpusch_sc[i];
                Qm       = pRxInParams->Qm[i];
                bDetSoft = pFecParams->by[i];
                FillTCBIOBuf(pTCB, (U32*) bDetSoft, (L - 2)*Msc*Qm, OUT_BUF, NULL);
            }
            phy_printf("Adding [%3d] rx_Demapper to List\n", TASKID_RX_DEMAPPER);
        }
#endif

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);
    }
    else
    {
        pTCB = pTcbRefill;

        if (pRxInParams->numPUSCH == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext    = (U32*) pTCB->ContextPtr;
            pContext[0] = isym;
            pContext[1] = srsPilot;
            pContext[2] = step_reDet;

            ReFillTCBIOBuf(pTCB, (U32*) reEst1,      step_reDet * (L - 2) * sizeof(S16), IN_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) expo_reEst,  MAXCHSUBFRAME * L * sizeof(S16), IN_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) pRxInParams, sizeof(ULRXINPUTPARAMS), IN_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) pFecParams,  sizeof(ULRXFECPARAMS), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) scrSeq,      MAX_INPUT_TRANSPORT_BYTE_SIZE * sizeof(S8), IN_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) pDescrIndex, numPusch * sizeof(S32), IN_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) pMuxSoftDec, MAX_MUXCTRL_SOFT_DEC * sizeof(S8), INOUT_BUF, 6);

#ifdef _WIN32
            {
                S32 i;
                S16 Qm, Msc;
                S8 *bDetSoft;

                for(i = 0; i < numPusch; i++)
                {
                    Msc      = pRxInParams->Mpusch_sc[i];
                    Qm       = pRxInParams->Qm[i];
                    bDetSoft = pFecParams->by[i];
                    ReFillTCBIOBuf(pTCB, (U32*) bDetSoft, (L - 2)*Msc*Qm, OUT_BUF, numVar + i);
                }
            }
#endif
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            rx_Demapper_fx_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Multiplex Control and Data TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------
 *  |Task ID                    |Scheduled on      |
 *  |                           |Hardware Resource |
 *  ------------------------------------------------
 *  |TASKID_RX_FEC_MUX_RI_CQI_CNTL |RESOURCE_CEVA     |
 *  ------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Multiplex Control and Data function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_mux_RI_CQI_decoding() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB muxCntlRI_CQI_Dec_tcb(PTASKLIST pTskLst, PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec,
                                    PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32 numVar;
    S32 i;

    numVar = 4;
    if(!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_FEC_MUX_RI_CQI_CNTL, RESOURCE_CEVA, NULL, 1 * sizeof(S32), NULL, NULL, TIME_RX_FEC_MUX_RI_CQI_CNTL);
#else  //USECEVA
        pTCB = CreateTCB(TASKID_RX_FEC_MUX_RI_CQI_CNTL, RESOURCE_LARM, NULL, 1 * sizeof(S32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_FEC_MUX_RI_CQI_CNTL, &muxCntlRI_CQI_Dec_task);
#endif  //USECEVA

#ifdef _WIN32
        CreateTCBIOBuf(pTCB, numVar + 11);
#else
        CreateTCBIOBuf(pTCB, numVar);
#endif
        FillTCBIOBuf(pTCB, (U32*) pFecParams,     sizeof(ULRXFECPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pMuxSoftDec,    MAX_MUXCTRL_SOFT_DEC * sizeof(S8), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_X, 11 * sizeof(S8*), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) FHT_MASK,       128 * 32 * sizeof(S8), IN_BUF, NULL);
#ifdef _WIN32
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_2, 514, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_2, 514, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_3, 515, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_4, 388, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_5, 261, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_6, 166, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_7, 1495, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_8, 1492, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_9, 1481, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_10, 1486, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rm_dectab_32_11, 1423, IN_BUF, NULL);


#endif
        if (pTskLst)
            AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
           for (i = 0; i < numDepend; i++)
           {
               if (pTaskListDependencyTo[DependArray[i]] != NULL)
                   AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
           }
        }
    }
    else
    {
        pTCB = pTcbRefill;

        if (pFecParams->muxPUSCHCqiRi== 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            ReFillTCBIOBuf(pTCB, (U32*) pFecParams,     sizeof(ULRXFECPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) pMuxSoftDec,    MAX_MUXCTRL_SOFT_DEC * sizeof(S8), INOUT_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_X, 11 * sizeof(S8*), IN_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) FHT_MASK,       128 * 32 * sizeof(S8), IN_BUF, 3);
#ifdef _WIN32
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_2, 514, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_2, 514, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_3, 515, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_4, 388, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_5, 261, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_6, 166, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_7, 1495, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_8, 1492, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_9, 1481, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_10, 1486, IN_BUF, numVar++);
            ReFillTCBIOBuf(pTCB, (U32*) rm_dectab_32_11, 1423, IN_BUF, numVar++);
#endif
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            muxCntlRI_CQI_Dec_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Multiplex Control and Data TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ---------------------------------------------
 *  |Task ID                 |Scheduled on      |
 *  |                        |Hardware Resource |
 *  ---------------------------------------------
 *  |TASKID_RX_FEC_MUX_ACK_CNTL |RESOURCE_CEVA     |
 *  ---------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Multiplex Control and Data function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_mux_ACK_decoding() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB muxCntlACK_Dec_tcb(PTASKLIST pTskLst, PULRXFECPARAMS pFecParams, S8 *pMuxSoftDec, PTCB pTaskListDependencyTo[],
                                   S16 *DependArray, S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    S32 i;

    if(!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_FEC_MUX_ACK_CNTL, RESOURCE_CEVA, NULL, 1 * sizeof(S32), NULL, NULL, TIME_RX_FEC_MUX_ACK_CNTL);
#else  //USECEVA
        pTCB = CreateTCB(TASKID_RX_FEC_MUX_ACK_CNTL, RESOURCE_LARM, NULL, 1 * sizeof(S32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_FEC_MUX_ACK_CNTL, &muxCntlACK_Dec_task);
#endif  //USECEVA

        CreateTCBIOBuf(pTCB, 2);
        FillTCBIOBuf(pTCB, (U32*) pFecParams,     sizeof(ULRXFECPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pMuxSoftDec,    MAX_MUXCTRL_SOFT_DEC * sizeof(S8), INOUT_BUF, NULL);

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
           for (i = 0; i < numDepend; i++)
           {
               if (pTaskListDependencyTo[DependArray[i]] != NULL)
                   AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
           }
        }
#ifdef _WIN32
        phy_printf("Adding [%3d] TASKID_RX_FEC_MUX_ACK_CNTL to TaskList\n", TASKID_RX_FEC_MUX_ACK_CNTL);
#endif // _WIN32
    }
    else
    {
        pTCB = pTcbRefill;

        if (pFecParams->muxPUSCHAckNack == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            ReFillTCBIOBuf(pTCB, (U32*) pFecParams,     sizeof(ULRXFECPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) pMuxSoftDec,    MAX_MUXCTRL_SOFT_DEC * sizeof(S8), INOUT_BUF, 1);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            muxCntlACK_Dec_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// END PUSCH TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


#ifdef DEBUG_POWER_RX_SIGNAL
PTCB ul_signal_power_tcb(PTASKLIST pTskLst, S16 *adcbuf0, S16 *adcbuf1, ULRXINPUTPARAMS *pULPilotInput)
{
    PTCB pTCB = 0;
    U32* pContext = 0;

#ifdef USECEVA
    pTCB = RexCreateTCBQ(TASKID_RX_SIGNAL_POWER_CALC, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL, TIME_RX_SIGNAL_POWER_CALC);
#else //USECEVA

    pTCB = CreateTCB(TASKID_RX_SIGNAL_POWER_CALC, RESOURCE_LARM, NULL, 5 * sizeof(U32), NULL, NULL);
    pTCB->ResourceIndex = 0;
    RegARMTask(TASKID_RX_SIGNAL_POWER_CALC, &ul_signal_power_task);

#endif

    pContext = (U32*) pTCB->ContextPtr;
    pContext[0] = (U32)adcbuf0;
    pContext[1] = (U32)adcbuf1;
    pContext[2] = (U32)pULPilotInput;

    AddTCBToList(pTCB, pTskLst, NULL);
    return pTCB;
}
#endif



////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// PUCCH TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink PUCCH Pilots TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                 |Scheduled on      |
 *  |                        |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_ULPILOT_PUCCH |RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Uplink PUCCH Pilots function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: ul_Pilot_PUCCH() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB ul_Pilot_PUCCH_tcb(PTASKLIST pTskLst, S16 a[], U32 *rs_grid, S16 *r_alpha_uv,
                        S32 len_r_alpha_uv, S32 step_aDet, S16 *nOc, S16 *Sdescramb,
                        ULRXINPUTPARAMS *pULPilotInput,
                        PTCB pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32* pContext = 0;
    S16 MI, L;
    S16 Msc;

    MI = pULPilotInput->MI;
    Msc = pULPilotInput->Nrb_sc;
    L = pULPilotInput->L;

    if (!RefillFlag)
    {

#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_ULPILOT_PUCCH, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL, TIME_RX_ULPILOT_PUCCH);
#else //USECEVA

        pTCB = CreateTCB(TASKID_RX_ULPILOT_PUCCH, RESOURCE_LARM, NULL, 2 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_ULPILOT_PUCCH, &ul_Pilot_PUCCH_task);

#endif

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = len_r_alpha_uv;
        pContext[1] = step_aDet;

        CreateTCBIOBuf(pTCB, 6);
        FillTCBIOBuf(pTCB, (U32*) pULPilotInput, sizeof(ULRXINPUTPARAMS), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rs_grid, L * MAXCHSUBFRAME * sizeof(U32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) r_alpha_uv, len_r_alpha_uv * MAXCHSUBFRAME * sizeof(S16) * 2, OUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) a, 2 * 2 * Msc * 3 * MI * MAXCHSUBFRAME * sizeof(S16) * 2, OUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) nOc, NUM_SLOTS_SUBFRM * MAXCHSUBFRAME * sizeof(S16) * 2, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) Sdescramb, NUM_SLOTS_SUBFRM * MAXCHSUBFRAME * sizeof(S16) * 2, INOUT_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] PUCCH ul_pilots to List\n", TASKID_RX_ULPILOT_PUCCH);
#endif

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);
    }
    else
    {
        pTCB = pTcbRefill;

        if (pULPilotInput->numPUCCH == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = len_r_alpha_uv;
            pContext[1] = step_aDet;

            ReFillTCBIOBuf(pTCB, (U32*) pULPilotInput, sizeof(ULRXINPUTPARAMS), IN_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) rs_grid, L*MAXCHSUBFRAME*sizeof(U32), INOUT_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) r_alpha_uv, len_r_alpha_uv*MAXCHSUBFRAME*sizeof(S16) * 2, OUT_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) a, 2*2*Msc*3*MI*MAXCHSUBFRAME*sizeof(S16) * 2, OUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) nOc, NUM_SLOTS_SUBFRM*MAXCHSUBFRAME*sizeof(S16) * 2, INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) Sdescramb, NUM_SLOTS_SUBFRM*MAXCHSUBFRAME*sizeof(S16) * 2, INOUT_BUF, 5);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            ul_Pilot_PUCCH_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Channel Estimation for PUCCH_part0 TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST_PUCCH|RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Channel Estimation for PUCCH Part0 function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_PUCCH() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB bs_rx_ChEstimator_PUCCH_part0_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *aPilots,
                                 ULRXINPUTPARAMS *pChanEstPar, S16 slot, S16 mo,
                                 S16 *expo_ch_pucch, S32 step_aDet, S32 *pErrAvg,
                                 S16 *pScaleComputeErr_Tab, S16 *chEst, S32 *ErrSub, S16 *ErrSubExpo,
                                 PTCB pTaskListDependencyTo[], S16 *DependArray,
                                 S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    S16 L, MO, Msc;
    U32* pContext = 0;
    S32 i = 0, numPucch;

    L   = pChanEstPar->L;
    MO  = pChanEstPar->MO;
    Msc = pChanEstPar->Nrb_sc;
    numPucch = pChanEstPar->numPUCCH;

    if (!RefillFlag)
    {
        //Construct TCB here
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_CHANEST_PUCCH_P0, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL, TIME_RX_CHANEST_PUCCH_P0);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_CHANEST_PUCCH_P0, RESOURCE_LARM, NULL, 3 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_CHANEST_PUCCH_P0, &bs_rx_ChEstimator_PUCCH_part0_task);
#endif

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = slot;
        pContext[1] = mo;
        pContext[2] = step_aDet;


        CreateTCBIOBuf(pTCB, 10);
        FillTCBIOBuf(pTCB, (U32*) pChanEstPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) adet, step_aDet * L * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rs_grid, L * sizeof(U32) * MAXCHSUBFRAME, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) expo_ch_pucch, L * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) aPilots, 3 * Msc * 2 * 2 * MAXCHSUBFRAME * 2 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pErrAvg, 2*2*2*MO*sizeof(S32)*MAXCHSUBFRAME, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pScaleComputeErr_Tab, sizeof(S16) * 100, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) chEst,                2*2*PRB_12*MAX_PUCCH_PILOTSYMB_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrSub,                2*2*2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) ErrSubExpo,      2*2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] ChEstimator_PUCCH_P0 to List\n", TASKID_RX_CHANEST_PUCCH_P0);
#endif

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
            for (i = 0; i < numDepend; i++)
            {
                if (pTaskListDependencyTo[DependArray[i]] != NULL)
                    AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
            }
        }
    }
    else
    {
        S16 MO_1;
        pTCB = pTcbRefill;

        //MO_1 is a parameter for running c-code where number of RxAnt can change from
        //subframe to subframe for testing purposes.  On device, MO_1 will always remain
        //constant like the parameter MO.
        MO_1 = pChanEstPar->MO_1;
        if ((numPucch == 0) || (mo >= MO_1))
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass
            
            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = slot;
            pContext[1] = mo;
            pContext[2] = step_aDet;


            ReFillTCBIOBuf(pTCB, (U32*) pChanEstPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) adet, step_aDet * L * sizeof(S16), INOUT_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) rs_grid, L * sizeof(U32) * MAXCHSUBFRAME, IN_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) expo_ch_pucch, L * sizeof(S16), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) aPilots, 3 * Msc * 2 * 2 * numPucch * sizeof(S16) * 2, IN_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) (pErrAvg), 2*2*2*MO*sizeof(S32)*MAXCHSUBFRAME, INOUT_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) pScaleComputeErr_Tab, sizeof(S16) * 100, IN_BUF, 6);
            ReFillTCBIOBuf(pTCB, (U32*) chEst,                2*2*PRB_12*MAX_PUCCH_PILOTSYMB_PER_SUBF*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 7);
            ReFillTCBIOBuf(pTCB, (U32*) ErrSub,                2*2*2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 8);
            ReFillTCBIOBuf(pTCB, (U32*) ErrSubExpo,      2*2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 9);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            bs_rx_ChEstimator_PUCCH_part0_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Rx Channel Estimation for PUCCH TCB Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  --------------------------------------------
 *  |Task ID                |Scheduled on      |
 *  |                       |Hardware Resource |
 *  --------------------------------------------
 *  |TASKID_RX_CHANEST_PUCCH|RESOURCE_CEVA     |
 *  --------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the LTE Rx Channel Estimation for PUCCH function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_PUCCH() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB bs_rx_ChEstimator_PUCCH_tcb(PTASKLIST pTskLst, S16 *adet, U32 *rs_grid, S16 *chEst,
                                 ULRXINPUTPARAMS *pChanEstPar, S16 slot, S16 mo,
                                 S16 *expo_ch_pucch, S32 step_aDet, S16 *r_alpha_uv,
                                 S32 len_alpha_uv, S16 *Sdescramb, S16 *nOc,
                                 S16 srsPilotSym, S16 *pAvgReDet_pucchACK,
                                 S16 *pAvgReDet_pucchCQI, S16 *pChEst_pucch,
                                 S32 *pRxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                                 S32 *pRxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo,
                                 S32 *pAllPucchPwrAvg_AntSlot, S32 *pPucchPwrAvg, S32 *pRxInRbPwrAvg_AntSlot, U8 *pRbUsedFlag, S32 *pRxRssi_AntSlot,
                                 S32 Err[], PTCB pTaskListDependencyTo, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    S16 L, MO;
    U32* pContext = 0;

    L = pChanEstPar->L;
    MO = pChanEstPar->MO;

    if (!RefillFlag)
    {
        //Construct TCB here
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_CHANEST_PUCCH, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL, TIME_RX_CHANEST_PUCCH);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_CHANEST_PUCCH, RESOURCE_LARM, NULL, 5 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_CHANEST_PUCCH, &bs_rx_ChEstimator_PUCCH_task);
#endif

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = slot;
        pContext[1] = mo;
        pContext[2] = step_aDet;
        pContext[3] = srsPilotSym;
        pContext[4] = len_alpha_uv;

        CreateTCBIOBuf(pTCB, 21);
        FillTCBIOBuf(pTCB, (U32*) pChanEstPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) adet, step_aDet * L * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) rs_grid, L * sizeof(U32) * MAXCHSUBFRAME, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) expo_ch_pucch, L * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) chEst,                2*2*PRB_12*MAX_PUCCH_PILOTSYMB_PER_SUBF*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchCQI), 2*MAX_PUCCH_DATASYMB_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pAllPucchPwrAvg_AntSlot, 2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pPucchPwrAvg, 2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);

        FillTCBIOBuf(pTCB, (U32*) (Sdescramb),       NUM_SLOTS_SUBFRM * MAXCHSUBFRAME * sizeof(S16) * 2, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (r_alpha_uv),      len_alpha_uv * MAXCHSUBFRAME * sizeof(S16) * 2, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (nOc),             NUM_SLOTS_SUBFRM * MAXCHSUBFRAME * sizeof(S16) * 2, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchACK), 2*2*MO*MAXCHSUBFRAME * sizeof(S16) * 2, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pChEst_pucch), 2*2*MO*MAXCHSUBFRAME * sizeof(S16) * 2, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbPilot), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Pilot_expo), 2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbDataSym), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Data_expo), 2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*)  Err,               2 * 2 * MAXCHSUBFRAME *  sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxInRbPwrAvg_AntSlot), 2*MO*MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRbUsedFlag), 2*MO*MAXRESOURCEBLOCKS * sizeof(U8), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxRssi_AntSlot), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);

#ifdef _WIN32
        phy_printf("Adding [%3d] CHANEST_PUCCH to List\n", TASKID_RX_CHANEST_PUCCH);
#endif
        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);
    }
    else
    {
        S16 MO_1;
        pTCB = pTcbRefill;

        //MO_1 is a parameter for running c-code where number of RxAnt can change from
        //subframe to subframe for testing purposes.  On device, MO_1 will always remain
        //constant like the parameter MO.
        MO_1 = pChanEstPar->MO_1;
        if ((pChanEstPar->numPUCCH == 0) || (mo >= MO_1))
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = slot;
            pContext[1] = mo;
            pContext[2] = step_aDet;
            pContext[3] = srsPilotSym;
            pContext[4] = len_alpha_uv;

            ReFillTCBIOBuf(pTCB, (U32*) pChanEstPar, sizeof(ULRXINPUTPARAMS), INOUT_BUF, 0);
            ReFillTCBIOBuf(pTCB, (U32*) adet, step_aDet * L * sizeof(S16), INOUT_BUF, 1);
            ReFillTCBIOBuf(pTCB, (U32*) rs_grid, L * sizeof(U32) * MAXCHSUBFRAME, IN_BUF, 2);
            ReFillTCBIOBuf(pTCB, (U32*) expo_ch_pucch, L * sizeof(S16), INOUT_BUF, 3);
            ReFillTCBIOBuf(pTCB, (U32*) chEst,                2*2*PRB_12*MAX_PUCCH_PILOTSYMB_PER_SUBF*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchCQI), 2*MAX_PUCCH_DATASYMB_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 5);
            ReFillTCBIOBuf(pTCB, (U32*) pAllPucchPwrAvg_AntSlot, 2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 6);
            ReFillTCBIOBuf(pTCB, (U32*) pPucchPwrAvg, 2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 7);
            ReFillTCBIOBuf(pTCB, (U32*) (Sdescramb),       NUM_SLOTS_SUBFRM * MAXCHSUBFRAME * sizeof(S16) * 2, IN_BUF, 8);
            ReFillTCBIOBuf(pTCB, (U32*) (r_alpha_uv),      len_alpha_uv * MAXCHSUBFRAME * sizeof(S16) * 2, IN_BUF, 9);
            ReFillTCBIOBuf(pTCB, (U32*) (nOc),             NUM_SLOTS_SUBFRM * MAXCHSUBFRAME * sizeof(S16) * 2, IN_BUF, 10);
            ReFillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchACK), 2 * 2 * MO * MAXCHSUBFRAME * sizeof(S16) * 2, INOUT_BUF, 11);
            ReFillTCBIOBuf(pTCB, (U32*) (pChEst_pucch), 2 * 2 * MO * MAXCHSUBFRAME * sizeof(S16) * 2, INOUT_BUF, 12);
            ReFillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbPilot), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 13);
            ReFillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Pilot_expo), 2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 14);
            ReFillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbDataSym), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 15);
            ReFillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Data_expo), 2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, 16);
            ReFillTCBIOBuf(pTCB, (U32*) Err,                  2 * 2 * MAXCHSUBFRAME *  sizeof(S32), INOUT_BUF, 17);
            ReFillTCBIOBuf(pTCB, (U32*) (pRxInRbPwrAvg_AntSlot), 2*MO*MAXRESOURCEBLOCKS * sizeof(S32), INOUT_BUF, 18);
            ReFillTCBIOBuf(pTCB, (U32*) (pRbUsedFlag), 2*MO*MAXRESOURCEBLOCKS * sizeof(U8), INOUT_BUF, 19);
            ReFillTCBIOBuf(pTCB, (U32*) (pRxRssi_AntSlot), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, 20);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            bs_rx_ChEstimator_PUCCH_task(pTCB);
    }
#endif  //SCHEDULER
    return pTCB;
}
////////////////////////////////////////////////////////////////////////////////////////
/** @brief rx_Multichan_DeModulation_Pucch_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  -------------------------------------------------------------
 *  |Task ID                                 |Scheduled on      |
 *  |                                        |Hardware Resource |
 *  -------------------------------------------------------------
 *  |TASKID_RX_MULTICHAN_PUCCH_DEMODULATION |RESOURCE_CEVA     |
 *  -------------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Rx MultiChannel Demodulation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: rx_Multichan_DeModulation() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
PTCB rx_Multichan_DeModulation_Pucch_tcb(PTASKLIST pTskLst, S16 *pRxIn, S16 *expo_ch,
                                   ULRXINPUTPARAMS *pRxInputPars, ULRXSDUPARAMS *pUlRxSduParams,
                                   S16 *pucchChan, S16 numPucchRun,
                                   S8 *pDescrBuf, U32 *pDescrIndex,
                                   S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI,
                                   S32 *snrPerRb, S32 *snrPilot, S16 *pChEst_pucch,
                                   S32 *pRxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                                   S32 *pRxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo,
                                   S32 *pAllPucchPwrAvg_AntSlot, S32 *pPucchPwrAvg,
                                   S8  *prm_dectab_20_X, S16 *prm_dectab_20_Offset,
                                   PTCB pTaskListDependencyTo[], S16 *DependArray,
                                   S16 numDepend, U16 RefillFlag, PTCB pTcbRefill)
{
    S32 i = 0, j = 0;
    PTCB pTCB = 0;
    U32 *pContext = 0;
    S32 L, MO, step_aDet;

    // Multichan Demodulation Task to consolidate all Ceva TCBs into 1 Single TCB
    L = pRxInputPars->L;
    MO = pRxInputPars->MO;
    step_aDet = RUP32B(pRxInputPars->K);

    if (!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_MULTICHAN_PUCCH_DEMODULATION, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL, TIME_RX_MULTICHAN_PUCCH_DEMODULATION);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_MULTICHAN_PUCCH_DEMODULATION, RESOURCE_LARM, NULL, 2 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_MULTICHAN_PUCCH_DEMODULATION, &rx_Multichan_DeModulation_Pucch_task);
#endif

        pContext = pTCB->ContextPtr;
        pContext[0] = numPucchRun;
        pContext[1] = MO;

        pTCB->SubTaskTimer = 1; // Set to 1 so that sub tasks will be logged in

        CreateTCBIOBuf(pTCB, MO + 20);
        for (i = 0; i < MO; i++)
        {
            FillTCBIOBuf(pTCB, (U32*) (&pRxIn[step_aDet * L * i]),
                         step_aDet * L * sizeof(S16), IN_BUF, NULL);
        }

        FillTCBIOBuf(pTCB, (U32*) (expo_ch),         MO * L * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxInputPars),    sizeof(ULRXINPUTPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pUlRxSduParams),  sizeof(ULRXSDUPARAMS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pucchChan),       NUM_PUCCH_PER_TCB * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchCQI),       2*MAX_PUCCH_DATASYMB_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchACK),       2*2*2*MO*MAXCHSUBFRAME * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (snrPerRb),                 2*100*MO * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (snrPilot),                 2*4*2*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pChEst_pucch),             2*2*2*MO*MAXCHSUBFRAME * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbPilot),   2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Pilot_expo),       2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbDataSym), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Data_expo),        2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pAllPucchPwrAvg_AntSlot),  2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pPucchPwrAvg),             2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (prm_dectab_20_X),      9522 * sizeof(S8),  IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (prm_dectab_20_Offset), 11   * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (FHT_MASK),             4096 * sizeof(S8),  IN_BUF, NULL);

        FillTCBIOBuf(pTCB, (U32*) (pDescrBuf),   MAX_INPUT_TRANSPORT_BYTE_SIZE * sizeof(S8), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (pDescrIndex), MAXCHSUBFRAME * sizeof(U32), IN_BUF, NULL);
#ifdef _WIN32
        phy_printf("Adding [%3d] PUCCH Rx_MultiChan to List\n", TASKID_RX_MULTICHAN_PUCCH_DEMODULATION);
#endif

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
        {
           for (i = 0; i < numDepend; i++)
           {
               if (pTaskListDependencyTo[DependArray[i]] != NULL)
                   AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
           }
        }
    }
    else
    {
        pTCB = pTcbRefill;

        if (numPucchRun == 0)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = pTCB->ContextPtr;
            pContext[0] = numPucchRun;
            pContext[1] = MO;

            pTCB->SubTaskTimer = 1; // Set to 1 so that sub tasks will be logged in

            j = 0;
            for (i = 0; i < MO; i++)
            {
                ReFillTCBIOBuf(pTCB, (U32*) (&pRxIn[step_aDet * L * i]),
                             step_aDet * L * sizeof(S16), IN_BUF, j++);
            }
            ReFillTCBIOBuf(pTCB, (U32*) (expo_ch),         MO * L * sizeof(S16), IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pRxInputPars),    sizeof(ULRXINPUTPARAMS), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pUlRxSduParams),  sizeof(ULRXSDUPARAMS), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pucchChan),       numPucchRun * sizeof(S16), IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchCQI),       2*MAX_PUCCH_DATASYMB_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S16), IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pAvgReDet_pucchACK),       2*2*2*MO*MAXCHSUBFRAME * sizeof(S16), IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (snrPerRb),                 2*100*MO * sizeof(S32), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (snrPilot),                 2*4*2*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pChEst_pucch),             2*2*2*MO*MAXCHSUBFRAME * sizeof(S16), IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbPilot),   2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Pilot_expo),       2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pRxInPwrAvg_PerRbDataSym), 2*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pPwrAvg_Data_expo),        2*MO*MAXCHSUBFRAME * sizeof(S16), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pAllPucchPwrAvg_AntSlot),  2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pPucchPwrAvg),             2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME * sizeof(S32), INOUT_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (prm_dectab_20_X),      9522 * sizeof(S8),  IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (prm_dectab_20_Offset), 11   * sizeof(S16), IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (FHT_MASK),             4096 * sizeof(S8),  IN_BUF, j++);

            ReFillTCBIOBuf(pTCB, (U32*) (pDescrBuf),   MAX_INPUT_TRANSPORT_BYTE_SIZE * sizeof(S8), IN_BUF, j++);
            ReFillTCBIOBuf(pTCB, (U32*) (pDescrIndex), MAXCHSUBFRAME * sizeof(U32), IN_BUF, j++);
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            rx_Multichan_DeModulation_Pucch_task(pTCB);
    }
#endif  //SCHEDULER
    return pTCB;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// END PUCCH TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// PRACH TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------
/** @brief PRACH_Rx_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_RX_PRACH                  |RESOURCE_CEVA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Uplink Rx Random Access Channel function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: PRACH_Rx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB PRACH_Rx_tcb(PBS_RX pBsRx, PBS_RX_PRACH pBsRxPrach, PBS_RX_PRACHDet pBsRxPrachDet, S16 MO,
                  S16 **adcbufPrach, S32 length_adcPrach, S16 *pTablesInCRAM,
                  BS_PRACH *pBsPrachCram, PTASKLIST pTaskList, U16 RefillFlag, PTCB pTcbRefill, S16 *zczLUT)
{
    PTCB pTCB = 0;
    U32 *pContext;
    S16 i = 0;

    pContext = NULL;

    if (!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_PRACH, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL, TIME_RX_PRACH);
#else //USECEVA
        pTCB = CreateTCB(TASKID_RX_PRACH, RESOURCE_LARM, NULL, 3 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_PRACH, &bs_rx_prach_task);

#endif
        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = MO;
        pContext[1] = length_adcPrach;
        pContext[2] = PRACH_DETECTION_THRESHOLD;

        CreateTCBIOBuf(pTCB, 10);
        FillTCBIOBuf(pTCB, (PBS_RX) pBsRx,                  sizeof(BS_RX), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (PBS_RX_PRACH) pBsRxPrach,       sizeof(BS_RX_PRACH), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (PBS_RX_PRACHDet) pBsRxPrachDet, sizeof(BS_RX_PRACHDet), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (S16*) pTablesInCRAM,            6720 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (PBS_PRACH) pBsPrachCram,        sizeof(BS_PRACH), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (S16*) zczLUT,                   65 * sizeof(S16), IN_BUF, NULL);
        for (i = 0; i < MO; i++)
        {
            FillTCBIOBuf(pTCB, (S16*) adcbufPrach[i],       4 * length_adcPrach, IN_BUF, NULL);
        }

#ifdef _WIN32
        phy_printf("Adding [%3d] PRACH to List\n", TASKID_RX_PRACH);
#endif

        AddTCBToList(pTCB, pTaskList, NULL);
    }
    else
    {
        pTCB = pTcbRefill;
        pBsRxPrachDet->bs_prachDet_msDet.num = 0;

        if (pBsRxPrach->bs_prach_enable != 1)
            MarkTCBControl(pTCB, BYPASS_TCB | REX_BLOCKING_CALL, 0); //mark it as a bypass TCB
        else
        {
            //Need to unMark Bypass for running C-Code
            MarkTCBControl(pTCB, 0, 0); //unmark ByPass

            pContext = (U32*) pTCB->ContextPtr;
            pContext[0] = MO;
            pContext[1] = length_adcPrach;
            pContext[2] = PRACH_DETECTION_THRESHOLD;

            ReFillTCBIOBuf(pTCB, (PBS_RX) pBsRx,                  sizeof(BS_RX), IN_BUF, 0);
            ReFillTCBIOBuf(pTCB, (PBS_RX_PRACH) pBsRxPrach,       sizeof(BS_RX_PRACH), IN_BUF, 1);
            ReFillTCBIOBuf(pTCB, (PBS_RX_PRACHDet) pBsRxPrachDet, sizeof(BS_RX_PRACHDet), INOUT_BUF, 2);
            ReFillTCBIOBuf(pTCB, (S16*) pTablesInCRAM,            6720 * sizeof(S16), IN_BUF, 3);
            ReFillTCBIOBuf(pTCB, (PBS_PRACH) pBsPrachCram,        sizeof(BS_PRACH), INOUT_BUF, 4);
            ReFillTCBIOBuf(pTCB, (S16*) zczLUT,                 65 * sizeof(S16), IN_BUF, 5);
            for (i = 0; i < MO; i++)
            {
                ReFillTCBIOBuf(pTCB, (S16*) adcbufPrach[i],       4 * length_adcPrach, IN_BUF, 6+i);
            }
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        if ((pTCB->TaskControl & BYPASS_TCB) == 0)
            bs_rx_prach_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// END PRACH TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// SRS TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------
/** @brief SRSReceiverProcessing_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_RX_SRS                       |RESOURCE_CEVA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Uplink Rx SRS Processing function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SRSReceiverProcessing() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB SRSReceiverProcessing_tcb(PTASKLIST pTskLst, S16 df, U32 Fs, S16 Nrb_sc,
                               S16 NusedE, S16 Nfft, S16 srsBandwidthTab, S16 srsBandwidthConfig, 
                               S16 *psrsBandwidthConfigTable, S16 *aDet, S16 numSrs, S16 mo, S16 MO,
                               S16 K, S16 L, S16 Nul_rb, S16 srsPilotSymbol, PBS_RX_POINTERS pRxPtr, 
                               PBS_RX_CHAN_SRS pBsRxChanSrs, S32 stepsize_aDet,
                               S16 *pScaleComputeErr_Tab, S32 *pSnrPerRb,
                               PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend,
                               PTCB pTaskListDependencyTo1, U16 RefillFlag, PTCB pTcbRefill,
                               S16 **pTable_DFT,S16 frameNumber,S16 subframeNumber)
{
    PTCB pTCB = 0;
    PIOParams_SRS pContext = 0;

    if (!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_SRS, RESOURCE_CEVA, NULL, sizeof(IOParams_SRS), NULL, NULL, TIME_RX_SRS);
#else // USECEVA
        // Run this task on the ARM
        pTCB = CreateTCB(TASKID_RX_SRS, RESOURCE_LARM, NULL, sizeof(IOParams_SRS), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_SRS, &SRSReceiverProcessing_task);
#endif // USECEVA

        pContext = pTCB->ContextPtr;

        pContext->df = df;
        pContext->Fs = Fs;
        pContext->Nrb_sc = Nrb_sc;
        pContext->NusedE = NusedE;
        pContext->Nfft = Nfft;
        pContext->numSrs = numSrs;
        pContext->mo = MO;
        pContext->K = K;
        pContext->L = L;
        pContext->Nul_rb = Nul_rb;
        pContext->srsPilotSymbol = srsPilotSymbol;
        pContext->stepsize_aDet = stepsize_aDet;
        pContext->srsBandwidthTab = srsBandwidthTab;
        pContext->srsBandwidthConfig = srsBandwidthConfig;
        pContext->frameNumber = frameNumber;
        pContext->subframeNumber = subframeNumber;

        CreateTCBIOBuf(pTCB, 15);

        FillTCBIOBuf(pTCB, (U32*) pRxPtr->pSrsChEst, 4 * K * 2 * MO* numSrs, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pRxPtr->pRawSrsChEst, 2 * K * MO, OUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) aDet, stepsize_aDet * L * 4 , INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pRxPtr->pSrsPilots, 4 * K * numSrs, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsRxChanSrs, sizeof(BS_RX_CHAN_SRS), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pRxPtr->pLengthSrsPilots, numSrs * 2, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pScaleComputeErr_Tab, sizeof(S16) * 100, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) psrsBandwidthConfigTable, sizeof(S16) * 64 * 4, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pSnrPerRb, numSrs * Nul_rb * MO * sizeof(U32), INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pRxPtr->pSrsChMeas, numSrs * sizeof(BSRXCHMeas), OUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pTable_DFT[0], 2*9 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pTable_DFT[1], 2*384 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pTable_DFT[2], 2*384 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pTable_DFT[3], 2*384 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pTable_DFT[4], 72 * sizeof(S16), IN_BUF, NULL);

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo1 != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo1);
        {
            S16 i;
            if (pTaskListDependencyTo != NULL)
            {
                for (i = 0; i < numDepend; i++)
                {
                    if (pTaskListDependencyTo[DependArray[i]] != NULL)
                        AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
                }
            }
        }

#ifdef _WIN32
        phy_printf("Adding [%3d] TASKID_RX_SRS to TaskList\n", TASKID_RX_SRS);
#endif // _WIN32
    }
    else
    {
        pTCB = pTcbRefill;

        pContext = pTCB->ContextPtr;

        pContext->df = df;
        pContext->Fs = Fs;
        pContext->Nrb_sc = Nrb_sc;
        pContext->NusedE = NusedE;
        pContext->Nfft = Nfft;
        pContext->numSrs = numSrs;
        pContext->mo = MO;
        pContext->K = K;
        pContext->L = L;
        pContext->Nul_rb = Nul_rb;
        pContext->srsPilotSymbol = srsPilotSymbol;
        pContext->stepsize_aDet = stepsize_aDet;
        pContext->srsBandwidthTab = srsBandwidthTab;
        pContext->srsBandwidthConfig = srsBandwidthConfig;
        pContext->frameNumber = frameNumber;
        pContext->subframeNumber = subframeNumber;

        ReFillTCBIOBuf(pTCB, (U32*) pRxPtr->pSrsChEst, 4 * K * 2 * MO* numSrs, INOUT_BUF, 0);
        ReFillTCBIOBuf(pTCB, (U32*) pRxPtr->pRawSrsChEst, 2 * K * MO, OUT_BUF, 1);
        ReFillTCBIOBuf(pTCB, (U32*) aDet, stepsize_aDet * L * 4 , INOUT_BUF, 2);
        ReFillTCBIOBuf(pTCB, (U32*) pRxPtr->pSrsPilots, 4 * K * numSrs, IN_BUF, 3);
        ReFillTCBIOBuf(pTCB, (U32*) pBsRxChanSrs, sizeof(BS_RX_CHAN_SRS), INOUT_BUF, 4);
        ReFillTCBIOBuf(pTCB, (U32*) pRxPtr->pLengthSrsPilots, numSrs * 2, IN_BUF, 5);
        ReFillTCBIOBuf(pTCB, (U32*) pScaleComputeErr_Tab, sizeof(S16) * 100, IN_BUF, 6);
        ReFillTCBIOBuf(pTCB, (U32*) psrsBandwidthConfigTable, sizeof(S16) * 64 * 4, IN_BUF, 7);
        ReFillTCBIOBuf(pTCB, (U32*) pSnrPerRb, numSrs * Nul_rb * MO * sizeof(U32), INOUT_BUF, 8);
        ReFillTCBIOBuf(pTCB, (U32*) pRxPtr->pSrsChMeas, numSrs * sizeof(BSRXCHMeas), OUT_BUF, 9);
        ReFillTCBIOBuf(pTCB, (U32*) pTable_DFT[0], 2*9 * sizeof(S16), IN_BUF, 10);
        ReFillTCBIOBuf(pTCB, (U32*) pTable_DFT[1], 2*384 * sizeof(S16), IN_BUF, 11);
        ReFillTCBIOBuf(pTCB, (U32*) pTable_DFT[2], 2*384 * sizeof(S16), IN_BUF, 12);
        ReFillTCBIOBuf(pTCB, (U32*) pTable_DFT[3], 2*384 * sizeof(S16), IN_BUF, 13);
        ReFillTCBIOBuf(pTCB, (U32*) pTable_DFT[4], 72 * sizeof(S16), IN_BUF, 14);
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        SRSReceiverProcessing_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

//---------------------------------------------------------------------------------------------
/** @brief bs_rx_Pilot_srs_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_RX_SRS_PILOTS                       |RESOURCE_CEVA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Uplink Rx SRS Pilots function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_Pilot_srs() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB bs_rx_Pilot_srs_tcb(PTASKLIST pTskLst, S16 subframeNumber, S16 Nrb_sc, U32 Ncell_id,
                         S16 groupHoppingEnabled, S16 sequenceHoppingEnabled, S16 delta_ss, S16 *u, S16 *v, S16 *rSrs, S16 *m_srs, S16 *N,
                         S16 *nCs_srs, S16 *bandwidth, S16 numChan, S16 Nused, S16 UlPilots_QthRootTable[],
                         PTCB pTaskListDependencyTo[], S16 *DependArray, S16 numDepend,
                         U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    PIOParams_SRS_Pilots pContext = 0;
    if (!RefillFlag)
    {
#ifdef USECEVA
         pTCB = RexCreateTCBQ(TASKID_RX_SRS_PILOTS, RESOURCE_CEVA, NULL, sizeof(IOParams_SRS_Pilots), NULL, NULL, TIME_RX_SRS_PILOTS);
#else // USECEVA
        // Run this task on the ARM
        pTCB = CreateTCB(TASKID_RX_SRS_PILOTS, RESOURCE_LARM, NULL, sizeof(IOParams_SRS_Pilots), NULL,
                         NULL);
        RegARMTask(TASKID_RX_SRS_PILOTS, &bs_rx_Pilot_srs_task);
#endif // USECEVA

        pContext = pTCB->ContextPtr;

        pContext->subframeNumber = subframeNumber;
        pContext->Nrb_sc = Nrb_sc;
        pContext->Nused = Nused;
        pContext->groupHoppingEnabled = groupHoppingEnabled;
        pContext->sequenceHoppingEnabled = sequenceHoppingEnabled;
        pContext->Ncell_id = Ncell_id;
        pContext->numSrs = numChan;
        pContext->delta_ss = delta_ss;

        CreateTCBIOBuf(pTCB, 8);

        FillTCBIOBuf(pTCB, (U32*) rSrs, 1 * Nused *numChan*  sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) u, 2 * NUM_SLOTS, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) m_srs, 2 * 4  * sizeof(S16), IN_BUF, NULL); // 4 comes from the fact there are 4 bandwidth config //table 5.5.3.2-2
        FillTCBIOBuf(pTCB, (U32*) N, 2 * 4  * sizeof(S16), IN_BUF, NULL); //table 5.5.3.2-2
        FillTCBIOBuf(pTCB, (U32*) nCs_srs, 2 * numChan, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) bandwidth, 2 * numChan, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) v, 2 * NUM_SLOTS * numChan, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) UlPilots_QthRootTable, 220 * 30 * 2 * 2* sizeof(S16), IN_BUF, NULL);
        {
            S16 i;
            AddTCBToList(pTCB, pTskLst, NULL);
            if (pTaskListDependencyTo != NULL)
            {
                for (i = 0; i < numDepend; i++)
                {
                    if (pTaskListDependencyTo[DependArray[i]] != NULL)
                        AddDependency(pTCB, 1, pTaskListDependencyTo[DependArray[i]]);
                }
            }
        }

#ifdef _WIN32
        phy_printf("Adding [%3d] TASKID_RX_SRS_PILOTS to TaskList\n", TASKID_RX_SRS_PILOTS);
#endif // _WIN32
    }
    else
    {
        pTCB = pTcbRefill;

        pContext = pTCB->ContextPtr;

        pContext->subframeNumber = subframeNumber;
        pContext->Nrb_sc = Nrb_sc;
        pContext->Nused = Nused;
        pContext->groupHoppingEnabled = groupHoppingEnabled;
        pContext->sequenceHoppingEnabled = sequenceHoppingEnabled;
        pContext->Ncell_id = Ncell_id;
        pContext->numSrs = numChan;
        pContext->delta_ss = delta_ss;

        ReFillTCBIOBuf(pTCB, (U32*) rSrs, 1 * Nused *numChan*  sizeof(S16), OUT_BUF, 0);  // srs pilots
        ReFillTCBIOBuf(pTCB, (U32*) u, 2 * NUM_SLOTS, INOUT_BUF, 1);
        ReFillTCBIOBuf(pTCB, (U32*) m_srs, 2 * 4 *  sizeof(S16), IN_BUF, 2); // 4 comes from the fact there are 4 bandwidth config, table 5.5.3.2-2
        ReFillTCBIOBuf(pTCB, (U32*) N, 2 * 4 *  sizeof(S16), IN_BUF, 3); //table 5.5.3.2-2
        ReFillTCBIOBuf(pTCB, (U32*) nCs_srs, 2 * numChan, IN_BUF, 4);
        ReFillTCBIOBuf(pTCB, (U32*) bandwidth, 2 * numChan, IN_BUF, 5);
        ReFillTCBIOBuf(pTCB, (U32*) v, 2 * NUM_SLOTS * numChan , INOUT_BUF, 6);
        ReFillTCBIOBuf(pTCB, (U32*) UlPilots_QthRootTable, 220 * 30 * 2 * 2* sizeof(S16), IN_BUF, 7);
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        bs_rx_Pilot_srs_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}


//---------------------------------------------------------------------------------------------
/** @brief SRSSnrReporting_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_RX_SRS_SNR                       |RESOURCE_CEVA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Uplink Rx SRS SnrReporting function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SRSReceiverProcessing() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB SRSSnrReporting_tcb(PTASKLIST pTskLst, S16 MO, S16 numSrs, S16 Mrb,
                                 S16 *StartRb, S16 *EndRb, S32 *snrPerRb, U8 *rxV_snrPerRB,
                                 PTCB *pTaskDependency, U16 RefillFlag, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32 *pContext;
    int i;

    if (!RefillFlag)
    {
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_RX_SRS_SNR, RESOURCE_CEVA, NULL, sizeof(U32)*3, NULL, NULL, TIME_RX_SRS_SNR);
#else // USECEVA
        // Run this task on the ARM
        pTCB = CreateTCB(TASKID_RX_SRS_SNR, RESOURCE_LARM, NULL, sizeof(U32)*3, NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_RX_SRS_SNR, &SRSSnrReporting_task);

#endif // USECEVA
        pContext = pTCB->ContextPtr;

        pContext[0] = MO;
        pContext[1] = Mrb;
        pContext[2] = numSrs;

        CreateTCBIOBuf(pTCB, 4);

        FillTCBIOBuf(pTCB, (U32*) StartRb, 2 * numSrs , IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) EndRb, 2 *numSrs, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) snrPerRb, MAX_UL_SRS * Mrb * MO * sizeof(U32), INOUT_BUF, NULL); // 4 antennas, 100 SRS and 100 RB
        FillTCBIOBuf(pTCB, (U32*) rxV_snrPerRB, MAX_UL_SRS * Mrb * sizeof(U8), OUT_BUF, NULL); // 100 SRS and 100 RB

        AddTCBToList(pTCB, pTskLst, NULL);
        if (*pTaskDependency != NULL)
        {
            for (i = 0 ;i < MO; i++)
                AddDependency(pTCB, 1, pTaskDependency[i]);
        }

#ifdef _WIN32
        phy_printf("Adding [%3d] TASKID_RX_SRS_SNR to TaskList\n", TASKID_RX_SRS_SNR);
#endif // _WIN32

    }
    else
    {
        pTCB = pTcbRefill;

        pContext = pTCB->ContextPtr;
        pContext[0] = MO;
        pContext[1] = Mrb;
        pContext[2] = numSrs;

        ReFillTCBIOBuf(pTCB, (U32*) StartRb, 2 * numSrs , IN_BUF, 0);
        ReFillTCBIOBuf(pTCB, (U32*) EndRb, 2 *numSrs, IN_BUF, 1);
        ReFillTCBIOBuf(pTCB, (U32*) snrPerRb, MAX_UL_SRS * Mrb * MO * sizeof(U32), INOUT_BUF, 2); // 4 antennas, 100 SRS and 100 RB
        ReFillTCBIOBuf(pTCB, (U32*) rxV_snrPerRB, MAX_UL_SRS * Mrb * sizeof(U8), OUT_BUF, 3); // 100 SRS and 100 RB
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
        SRSSnrReporting_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// END SRS TCBS
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// END UpLink TCB Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////









////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// Downlink TCB Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/** @brief tx_MultiChan_Modulation_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_TX_MULTICHAN_MODULATION  |RESOURCE_CEVA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Downlink Tx MultiChannel Modulation function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: tx_MultiChan_Modulation() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB tx_MultiChan_Modulation_tcb(PTASKLIST pTskLst, PDLTXINPUTPARAMS pTxModVars,
                                 TXSYSTEMINFO *pTxSys, U32 *_totalsizescr,
                                 U32 *scrIndex[2], U8 *scrOut[2], TXSYMBUFS *pTxSymBufs,
                                 TXSYMBUF *pTxSymBuf, S16* sync[2], U32 isym,
                                 U16 RefillFlag, PTCB pTaskListDependencyTo, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32 *pContext = NULL;
    U32 numVars = 0;
    U32 len[10] = {0};
    U16 i = 0;

    U32 lenAlTxSys   = RUP32B(sizeof(TXSYSTEMINFO));
    U32 lenTxSymBufs = RUP32B(sizeof(TXSYMBUFS));
    U32 lenTxSymBuf  = RUP32B(sizeof(TXSYMBUF)*MAXCHSUBFRAME);

    U8* pTxSysLocal       = (U8*)pTxSys;
    U8* pTxSymBufsLocal   = (U8*)pTxSymBufs;
    U8* pTxSymBufLocal    = (U8*)pTxSymBuf;
    PTXSYSTEMINFO pTxSysTemp;

    if (!RefillFlag)
    {
        //Create TCB
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_TX_MULTICHAN_MODULATION, RESOURCE_CEVA, NULL,
                         1 * sizeof(U32), NULL, NULL, TIME_TX_MULTICHAN_MODULATION);
#else // USECEVA
        // Run Task on ARM
        pTCB = CreateTCB(TASKID_TX_MULTICHAN_MODULATION, RESOURCE_LARM, NULL, 1 * sizeof(U32), NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_TX_MULTICHAN_MODULATION,&tx_MultiChan_Modulation_task);
#endif
        pContext = (U32 *) pTCB->ContextPtr;
        pContext[0] = isym;

        //Set to Max during creation of TaskList
        len[0] = sizeof(TXSYSTEMINFO);
        len[1] = RUP32B(MAXCHSUBFRAME * sizeof(S16));
        len[2] = sizeof(TXSYMBUFS);
        len[3] = MAXCHSUBFRAME * sizeof(TXSYMBUF);
        len[4] = RUP32B(sizeof(U32) * MAXCHSUBFRAME);
        len[5] = MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT;
        len[6] = MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT;

        numVars = 10;
        CreateTCBIOBuf(pTCB, numVars + (pTxModVars->Ntx_antennas*2));

        pTCB->SubTaskTimer = 1; // Set to 1 so that sub tasks will be logged in

        FillTCBIOBuf(pTCB, pTxModVars, sizeof(DLTXINPUTPARAMS), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, pTxSysLocal, lenAlTxSys*8, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, sync[0], 62 * 2, IN_BUF, NULL);//3
        FillTCBIOBuf(pTCB, sync[1], 62 * 2, IN_BUF, NULL);//4
        FillTCBIOBuf(pTCB, pTxSymBufsLocal, lenTxSymBufs*8, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, pTxSymBufLocal, lenTxSymBuf*8, IN_BUF, NULL);

        FillTCBIOBuf(pTCB, scrIndex[0], len[4], IN_BUF, NULL);
        FillTCBIOBuf(pTCB, scrIndex[1], len[4], IN_BUF, NULL);
        FillTCBIOBuf(pTCB, scrOut[0], len[5], IN_BUF, NULL);
        FillTCBIOBuf(pTCB, scrOut[1], len[6], IN_BUF, NULL);

        pTxSysTemp = (PTXSYSTEMINFO)pTxSysLocal;
        for (i = 0; i < pTxModVars->Ntx_antennas; i++)
            FillTCBIOBuf(pTCB, (S16*) pTxSysTemp->a.aSymbol[i], pTxModVars->NumSubCars, INOUT_BUF, NULL);

        pTxSysLocal += lenAlTxSys*7;
        pTxSysTemp = (PTXSYSTEMINFO)pTxSysLocal;
        for (i = 0; i < pTxModVars->Ntx_antennas; i++)
            FillTCBIOBuf(pTCB, (S16*) pTxSysTemp->a.aSymbol[i], pTxModVars->NumSubCars, INOUT_BUF, NULL);

        pTCB->IOControlLen += 8; // Reserve extra space when creating Template

        if (pTskLst != NULL)
            AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);

#ifdef _WIN32
        phy_printf("Adding [%3d] TASKID_TX_MULTICHAN_MODULATION to TaskList\n", TASKID_TX_MULTICHAN_MODULATION);
#endif // _WIN32
    }
    else
    {
        //Update TCB
        pTCB = pTcbRefill;

        pContext = (U32 *) pTCB->ContextPtr;
        pContext[0] = isym;

        len[0] = sizeof(TXSYSTEMINFO);
        len[1] = RUP32B(pTxModVars->numChan * sizeof(S16));
        len[2] = sizeof(TXSYMBUFS);
        len[3] = pTxModVars->numChan * sizeof(TXSYMBUF);
        len[4] = RUP32B(sizeof(U32) * MAXCHSUBFRAME);
        len[5] = _totalsizescr[0];
        len[6] = _totalsizescr[1];

        numVars = 10;

        ReFillTCBIOBuf(pTCB, pTxModVars, sizeof(DLTXINPUTPARAMS), INOUT_BUF, 0);
        ReFillTCBIOBuf(pTCB, pTxSysLocal, lenAlTxSys*8, INOUT_BUF, 1);
        ReFillTCBIOBuf(pTCB, sync[0], 62 * 2, IN_BUF, 2);//3
        ReFillTCBIOBuf(pTCB, sync[1], 62 * 2, IN_BUF, 3);//4
        ReFillTCBIOBuf(pTCB, pTxSymBufsLocal, lenTxSymBufs*8, IN_BUF, 4);
        ReFillTCBIOBuf(pTCB, pTxSymBufLocal, lenTxSymBuf*8, IN_BUF, 5);
        ReFillTCBIOBuf(pTCB, scrIndex[0], len[4], IN_BUF, 6);
        ReFillTCBIOBuf(pTCB, scrIndex[1], len[4], IN_BUF, 7);
        ReFillTCBIOBuf(pTCB, scrOut[0], len[5], IN_BUF, 8);
        ReFillTCBIOBuf(pTCB, scrOut[1], len[6], IN_BUF, 9);

        pTxSysTemp = (PTXSYSTEMINFO)pTxSysLocal;
        for (i = 0; i < pTxModVars->Ntx_antennas; i++)
            ReFillTCBIOBuf(pTCB, (S16*) pTxSysTemp->a.aSymbol[i], pTxModVars->NumSubCars, INOUT_BUF, numVars++);

        pTxSysLocal += lenAlTxSys*7;
        pTxSysTemp = (PTXSYSTEMINFO)pTxSysLocal;
        for (i = 0; i < pTxModVars->Ntx_antennas; i++)
            ReFillTCBIOBuf(pTCB, (S16*) pTxSysTemp->a.aSymbol[i], pTxModVars->NumSubCars, INOUT_BUF, numVars++);
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
#ifdef _WIN32
        phy_printf("Executing [%3d] TASKID_TX_MULTICHAN_MODULATION\n", TASKID_TX_MULTICHAN_MODULATION);
#endif // _WIN32

        tx_MultiChan_Modulation_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief SetupSymbDataInpBufs_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_LOAD_SETUPSYMBDATAINPBUFS |RESOURCE_CEVA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Downlink Tx SetupSymbDataInpBufs function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SetupSymbDataInpBufs() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB SetupSymbDataInpBufs_tcb(PTASKLIST pTskLst, PLTEPHYDESC pPhy,
                          TXSYMBUFS *pTxSymBufs, TXSYMBUF *pTxSymBuf,
                          DLTXINPUTPARAMS *pTxInPars, U16 RefillFlag,
                          PTCB pTaskListDependencyTo, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32 len[4] = {0, 0, 0, 0};

    if (!RefillFlag)
    {
        //Create TCB
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_TX_SETUP_SYMB_BUFS, RESOURCE_CEVA, NULL, 0, NULL, NULL, TIME_TX_SETUP_SYMB_BUFS);
#else // USECEVA
        // Run Task on ARM
        pTCB = CreateTCB(TASKID_TX_SETUP_SYMB_BUFS, RESOURCE_LARM, NULL, 0, NULL, NULL);
        RegARMTask(TASKID_TX_SETUP_SYMB_BUFS,&SetupSymbDataInpBufs_task);
#endif

        len[0] = pPhy->pBsTx->Ndl_symb * 2 * RUP32B(sizeof(TXSYMBUFS));
        len[1] = pPhy->pBsTx->Ndl_symb * 2 * RUP32B( sizeof(TXSYMBUF)*MAXCHSUBFRAME );

        CreateTCBIOBuf(pTCB, 3);
        FillTCBIOBuf(pTCB, (TXSYMBUFS*) (pTxSymBufs), len[0], OUT_BUF, NULL);//0
        FillTCBIOBuf(pTCB, (TXSYMBUF*) (pTxSymBuf), len[1], OUT_BUF, NULL);//1
        FillTCBIOBuf(pTCB, (DLTXINPUTPARAMS*) (pTxInPars), sizeof(DLTXINPUTPARAMS), INOUT_BUF, NULL);//2

        if (pTskLst != NULL)
            AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);

#ifdef _WIN32
        phy_printf("Adding [%3d] TASKID_TX_SETUP_SYMB_BUFS to TaskList\n", TASKID_TX_SETUP_SYMB_BUFS);
#endif // _WIN32
    }
    else
    {
        //Update TCB
        pTCB = pTcbRefill;

        len[0] = pPhy->pBsTx->Ndl_symb * 2 * RUP32B(sizeof(TXSYMBUFS));
        len[1] = pPhy->pBsTx->Ndl_symb * 2 * RUP32B( sizeof(TXSYMBUF)*MAXCHSUBFRAME );

        ReFillTCBIOBuf(pTCB, (TXSYMBUFS*) (pTxSymBufs), len[0], OUT_BUF, 0);//0
        ReFillTCBIOBuf(pTCB, (TXSYMBUF*) (pTxSymBuf), len[1], OUT_BUF, 1);//1
        ReFillTCBIOBuf(pTCB, (DLTXINPUTPARAMS*) (pTxInPars), sizeof(DLTXINPUTPARAMS), INOUT_BUF, 2);//2
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
#ifdef _WIN32
        phy_printf("Executing [%3d] TASKID_TX_SETUP_SYMB_BUFS\n", TASKID_TX_SETUP_SYMB_BUFS);
#endif // _WIN32

        SetupSymbDataInpBufs_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}

#ifndef _WIN32    // Run on actual MAP
////////////////////////////////////////////////////////////////////////////////////////
/** @brief ifft_tcb_hw Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_TX_IFFT                      |RESOURCE_FLTP     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the iFFT function.<BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB ifft_tcb_hw(PTASKLIST pTskList, PTCB pTaskListDependencyTo)
{
    PTCB pTCB = 0;

    pTCB = CreateTCB(TASKID_TX_IFFT, RESOURCE_FLTP, NULL, NULL, NULL, NULL);

    if (pTskList)
        AddTCBToList(pTCB, pTskList, NULL);
    if (pTaskListDependencyTo != NULL)
        AddDependency(pTCB, 1, pTaskListDependencyTo);
    return pTCB;
}

////////////////////////////////////////////////////////////////////////////////////////
#else // _WIN32
////////////////////////////////////////////////////////////////////////////////////////

//Run on Ceva Simulator
////////////////////////////////////////////////////////////////////////////////////////
/** @brief ifft_tcb_sim Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_TX_IFFT                      |RESOURCE_FLTP     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the iFFT function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: ifftcg_c_16t24d() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB ifft_tcb_sim(PTASKLIST pTskLst, S16 *pRS_Grid, S16 **pIFFTOut, S16 slotNum, S16 fftExpo,
                  S16 num_sym, S16 numSubCar, S16 numTxAnt, S16 CPType, S16 Ncp0, S16 Ncp,
                  U16 RefillFlag, PTCB pTaskListDependencyTo, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    U32 stepsize_a, mi;
    U32 FFTLen = 1 << fftExpo;
    PIOParams_FFT pContext = NULL;
    U32 slotLen, Idx1;

    slotLen = 2*((FFTLen * num_sym) + (Ncp0 + Ncp * (num_sym-1)));
    stepsize_a = RUP32B(numSubCar);

#ifdef SCHEDULER

    // This function has 3 parts
    // Section B: When IFFT has to be run on Windows Environment
    // Section B1: When IFFT has to be run in C-Only Mode
    // Section B2: When IFFT has to be run in Ceva Simlator mode
    //                   In this mode, the IFFT will be run on the ARM
    //---------------------------------------------------------------------------
    // SECTION B:
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    // SECTION B2:
    //---------------------------------------------------------------------------
    if (!RefillFlag)
    {
        //Create TCB
        pTCB = CreateTCB(TASKID_TX_IFFT, RESOURCE_LARM, NULL, sizeof(IOParams_FFT),
            NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_TX_IFFT, &ifftcg_c_16t24d_task);

        pContext = (PIOParams_FFT) pTCB->ContextPtr;

        pContext->FFT_Order = fftExpo;
        pContext->slotNum = slotNum;
        pContext->num_sym = num_sym;
        pContext->Ncp = Ncp;
        pContext->Ncp0 = Ncp0;
        pContext->numSubCar = numSubCar;
        pContext->numTxAnt = numTxAnt;
        pContext->CPType = CPType;

        Idx1 = (slotLen)*slotNum;
        CreateTCBIOBuf(pTCB, (1 + numTxAnt));
        FillTCBIOBuf(pTCB, (U32*) (pRS_Grid), (U32) (2 * stepsize_a * num_sym * sizeof(S16)), IN_BUF, NULL);
        for(mi = 0; mi < (U32)numTxAnt; mi++)
        {
            FillTCBIOBuf(pTCB, (U32*) (&pIFFTOut[mi][Idx1]), (U32) ((slotLen) * sizeof(S16)), OUT_BUF, NULL);
        }

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);

        phy_printf("Adding [%3d] TASKID_TX_IFFT to TaskList\n", TASKID_TX_IFFT);
    }
    else
    {
        //Update TCB
        pTCB = pTcbRefill;

        pContext = (PIOParams_FFT) pTCB->ContextPtr;

        pContext->FFT_Order = fftExpo;
        pContext->slotNum = slotNum;
        pContext->num_sym = num_sym;
        pContext->Ncp = Ncp;
        pContext->Ncp0 = Ncp0;
        pContext->numSubCar = numSubCar;
        pContext->numTxAnt = numTxAnt;
        pContext->CPType = CPType;

        Idx1 = (slotLen/2)*slotNum;
        ReFillTCBIOBuf(pTCB, (U32*) (pRS_Grid), (U32) (2 * numSubCar * num_sym * sizeof(S16)), IN_BUF, 0);
        for(mi = 0; mi < (U32)numTxAnt; mi++)
        {
            ReFillTCBIOBuf(pTCB, (U32*) (&pIFFTOut[mi][Idx1]), (U32) ((slotLen/2) * sizeof(S16)), OUT_BUF, (1 + mi));
        }
    }
#else
    // Run C-Only Mode
    //---------------------------------------------------------------------------
    // SECTION B1:
    //---------------------------------------------------------------------------
    if (RefillFlag)
    {
        S32 Ncpsam, i, count_s = 0;
        S16 *pIfft_In, *pIfft_Out;
        S16 expo, *ptr_dac_samp;
        U32 Idx2 = 0, isym, col_a;

#ifdef DEBUG_PRINT
        S16 j;

        for(mi = 0; mi < (U32)numTxAnt; mi++)
        {
            fprintf(f_data_mod5a[mi],"%%SlotNum = %d\n", slotNum);
            fprintf(f_data_imod5a[mi],"%%SlotNum = %d\n", slotNum);
        }
#endif

        phy_printf("Executing Task [%3d] TASKID_TX_IFFT\n", TASKID_TX_IFFT);

        pIfft_In  = MemAlloc(FFTLen*2*sizeof(S16), CRAM_MEM, 0);
        pIfft_Out = MemAlloc(FFTLen*2*sizeof(S16), CRAM_MEM, 0);

        col_a = 2*num_sym;
        for(i = 0; i < (S32)num_sym; i++)
        {
            isym = slotNum*num_sym + i;

            Idx1 = stepsize_a*(isym << 1);
            Idx2 = stepsize_a*(col_a << 1);

            if (i == 0)
                Ncpsam = Ncp0;
            else
                Ncpsam = Ncp;

            for(mi = 0; mi < (U32)numTxAnt; mi++)
            {
                ptr_dac_samp = &pIFFTOut[mi][slotNum*slotLen + count_s];

                SetupIFFT_Bufs(pIfft_In, &pRS_Grid[Idx1 + Idx2*mi], numSubCar, (S16)col_a, (S16)isym, (S16)mi, (S16)FFTLen, (S16)Ncpsam);
#ifdef DEBUG_PRINT
                for (j = 0; j < numSubCar; j++)
               {
                 fprintf(f_data_mod5a[mi],"%f %f\n", pRS_Grid[Idx1 + Idx2*mi + 2*j]/8192.0, pRS_Grid[Idx1 + Idx2*mi + 2*j +1]/8192.0);
                 fprintf(f_data_imod5a[mi],"%hd %hd\n", pRS_Grid[Idx1 + Idx2*mi + 2*j], pRS_Grid[Idx1 + Idx2*mi + 2*j +1]);
               }
#endif
                ifftcg_c_16t24d(pIfft_In, fftExpo, pIfft_Out, &expo);
                AddCP(ptr_dac_samp, pIfft_Out, (S16)FFTLen, (S16)Ncpsam);
            }
            count_s += 2*(Ncpsam + FFTLen);
        }

        MemFree(pIfft_In, CRAM_MEM);
        MemFree(pIfft_Out, CRAM_MEM);
    }
#endif // SCHEDULER

    return pTCB;
}

#endif //_WIN32

////////////////////////////////////////////////////////////////////////////////////////
/** @brief phy_bs_tx_control_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_TX_DLCONTROL             |RESOURCE_CEVA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the Downlink Tx LteBsPhyDLControlProc function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: LteBsPhyDLControlProc() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
//-------------------------------------------------------------------------------------------
PTCB phy_bs_tx_control_tcb(PTASKLIST pTskLst, S16* rs_grid[], U8 *startPDCCH,
                           PBSDLINITMEMBUF pBsDlInitMemBuf, PIOParams_DlControl pDlCntrl_Pars,
                           U16 RefillFlag, PTCB pTaskListDependencyTo, PTCB pTcbRefill)
{
    PTCB pTCB = 0;
    S16 *StartCCECtl, *NumCCECtl;
    S32 stepsize_a, numTxAntennas;
    S32 numInputs = 0;
    S32 isym = 0, j = 0;

    pDlCntrl_Pars->startPDCCH = startPDCCH;

    StartCCECtl = pDlCntrl_Pars->StartCCECtl;
    NumCCECtl   = pDlCntrl_Pars->NumCCECtl;
    stepsize_a  = pDlCntrl_Pars->stepsize_a;
    numTxAntennas = pDlCntrl_Pars->numTxAntennas;

    if (!RefillFlag)
    {
        //Create TCB
#ifdef USECEVA
        pTCB = RexCreateTCBQ(TASKID_TX_DLCONTROL, RESOURCE_CEVA, NULL, 0, NULL, NULL, TIME_TX_DLCONTROL);
#else // USECEVA
        // Run this task on the ARM
        pTCB = CreateTCB(TASKID_TX_DLCONTROL, RESOURCE_LARM, NULL, 0, NULL, NULL);
        pTCB->ResourceIndex = 0;
        RegARMTask(TASKID_TX_DLCONTROL, &phy_bs_tx_control_task);
#endif // USECEVA
        numInputs = 15 + 3*numTxAntennas;
        CreateTCBIOBuf(pTCB, numInputs);
        FillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars, sizeof(IOParams_DlControl), IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) startPDCCH, MAX_PDCCH_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) StartCCECtl, sizeof(S16) * MAXCHSUBFRAME, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) NumCCECtl,   sizeof(S16) * MAXCHSUBFRAME, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pCfiMemBuffer, sizeof(S16)*PCFICH_INIT_MEM_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pCfiReMapBuffer, sizeof(S16)*PCFICH_INIT_RE_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pHIMemBuffer, sizeof(S16)*PHICH_INIT_MEM_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pHIREMapTable, sizeof(S16)*PHICH_INIT_RE_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pPDCCHPermuteTable, sizeof(S16)*PDCCH_INIT_PERMUTE_TABLE_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pPDCCHQuadrupletsLen, sizeof(S16)*PDCCH_INIT_NUM_QUADRUPLETS_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pPDCCHReMapTable, sizeof(S16)*PDCCH_INIT_RE_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pDLCellSpecificRefSeq, sizeof(S16)*DL_CRS_INIT_SYM_BUFFER_SIZE, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars->scrSeq, 10*800, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars->inputPDCCH, MAXCHSUBFRAME*128, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars->pdcchRNTI, sizeof(U16)*MAXCHSUBFRAME, IN_BUF, NULL);

        for(isym = 0; isym < 3; isym++)
        {
            for(j = 0; j < numTxAntennas; j++)
            {
                FillTCBIOBuf(pTCB, (U32*)(rs_grid[j + isym*numTxAntennas]), 2*stepsize_a*sizeof(S16), INOUT_BUF, NULL);
            }
        }

        AddTCBToList(pTCB, pTskLst, NULL);
        if (pTaskListDependencyTo != NULL)
            AddDependency(pTCB, 1, pTaskListDependencyTo);

#ifdef _WIN32
        phy_printf("Adding [%3d] TASKID_TX_DLCONTROL to TaskList\n", TASKID_TX_DLCONTROL);
#endif // _WIN32
    }
    else    //RefillFlag
    {
        //Updating TCB
        pTCB = pTcbRefill;
        numInputs = 15;
        ReFillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars, sizeof(IOParams_DlControl), IN_BUF, 0);
        ReFillTCBIOBuf(pTCB, (U32*) startPDCCH, MAX_PDCCH_BUFFER_SIZE, IN_BUF, 1);
        ReFillTCBIOBuf(pTCB, (U32*) StartCCECtl, sizeof(S16) * MAXCHSUBFRAME, IN_BUF, 2);
        ReFillTCBIOBuf(pTCB, (U32*) NumCCECtl,   sizeof(S16) * MAXCHSUBFRAME, IN_BUF, 3);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pCfiMemBuffer, sizeof(S16)*PCFICH_INIT_MEM_BUFFER_SIZE, IN_BUF, 4);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pCfiReMapBuffer, sizeof(S16)*PCFICH_INIT_RE_BUFFER_SIZE, IN_BUF, 5);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pHIMemBuffer, sizeof(S16)*PHICH_INIT_MEM_BUFFER_SIZE, IN_BUF, 6);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pHIREMapTable, sizeof(S16)*PHICH_INIT_RE_BUFFER_SIZE, IN_BUF, 7);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pPDCCHPermuteTable, sizeof(S16)*PDCCH_INIT_PERMUTE_TABLE_BUFFER_SIZE, IN_BUF, 8);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pPDCCHQuadrupletsLen, sizeof(S16)*PDCCH_INIT_NUM_QUADRUPLETS_BUFFER_SIZE, IN_BUF, 9);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pPDCCHReMapTable, sizeof(S16)*PDCCH_INIT_RE_BUFFER_SIZE, IN_BUF, 10);
        ReFillTCBIOBuf(pTCB, (U32*) pBsDlInitMemBuf->pDLCellSpecificRefSeq, sizeof(S16)*DL_CRS_INIT_SYM_BUFFER_SIZE, IN_BUF, 11);
        ReFillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars->scrSeq, 10*800, IN_BUF, 12);
        ReFillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars->inputPDCCH, MAXCHSUBFRAME*128, IN_BUF, 13);
        ReFillTCBIOBuf(pTCB, (U32*) pDlCntrl_Pars->pdcchRNTI, sizeof(U16)*MAXCHSUBFRAME, IN_BUF, 14);

        for(isym = 0; isym < pDlCntrl_Pars->numCtrlSymbols; isym++)
        {
            for(j = 0; j < numTxAntennas; j++)
            {
                ReFillTCBIOBuf(pTCB, (U32*)(rs_grid[j + isym*numTxAntennas]), 2*stepsize_a*sizeof(S16), INOUT_BUF, numInputs++);
            }
        }
    }

#ifndef SCHEDULER
    //////////////////////////////////////////////////
    //Run C-Code if SCHEDULER not defined
    //////////////////////////////////////////////////
    if (RefillFlag)
    {
#ifdef _WIN32
        phy_printf("Executing [%3d] TASKID_TX_DLCONTROL\n", TASKID_TX_DLCONTROL);
#endif // _WIN32

        phy_bs_tx_control_task(pTCB);
    }
#endif  //SCHEDULER

    return pTCB;
}



////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// End Downlink TCB Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 Function BuildFecTcb is used by the channel coding routines to communicate with
 the new FEC hardware drivers.
 ******************************************************************************/
#ifndef _WIN32
//----------------------------------------------------------------------------------------
/** @brief LTE function to generate Tx FEC hardware control structure.<BR>
 *  Function BuildFecTcb_TX is used to generate the structure required to
 *  control the Tx FEC hardware.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param      CodingType                  Type of encoding or decoding (0=CC, 1=CTC).
 *  @param      BlockSize                   Code block size.
 *  @param      CrcSelector                 Type of CRC.
 *  @param      FillerBits                  Number of initial filler bits in code block.
 *  @param      Modulation                  Number of modulation bits used (1,2,4,6).
 *  @param      CodingRate                  Coding rate.
 *  @param      HalfIterations              Number of decoder 1/2 iterations.
 *  @param      DynamicStopIteration        CTC decoder automatic stop select.
 *  @param      ScrambleEnable              Scrambling enable.
 *  @param      ScrambleMask                Scrambling mask.
 *  @param      SubpacketId                 Sub-packet ID for the itial transmission.
 *  @param      BpBlockInterleaving         Block interleaver bypass.
 *  @param      BpEncoder                   Encoder bypass.
 *  @param      BpDecoder                   Decoder bypass.
 *  @param      EnableCombine               HARQ soft-combining enable.
 *  @param      BpRateMatching              Rate matching bypass.
 *  @param      RateMatchingE               Rate matching E parameter.
 *  @param      RateMatchingk0              Rate matching k0 parameter.
 *  @param      RateMatchingNcb             Rate matching Ncb parameter.
 *  @param      BpChannelInterleaving       Channel interleaver bypass
 *  @param      CiSymbolNumber              Number of channel interleaver columns (12 or 14).
 *  @param      In[]                        Input buffer pointer.
 *  @param      InLen                       Input buffer length.
 *  @param      Out[]                       Output buffer pointer.
 *  @param      OutLen                      Output buffer length.
 *  @param      pDlCbCtx                    Callback function context pointer.
 *  @param      FecDlCbDone                 Callback function pointer.
 *  @return     None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name                 |Name In  |Direction |Length |Type       |Format    |Valid Range |Units|
 *  |                     |Standard |          |       |           |          |            |
 *  ----------------------------------------------------------------------------------------------
 *  |CodingType           |         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |BlockSize            |         |input     |1      |Real       |(0:16:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |CrcSelector          |         |input     |1      |Real       |(0:16:0)  |[0:2]       |     |
 *  ----------------------------------------------------------------------------------------------
 *  |FillerBits           |         |input     |1      |Real       |(0:16:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |Modulation           |         |input     |1      |Real       |(0:16:0)  |{1, 2, 4, 6}|     |
 *  ----------------------------------------------------------------------------------------------
 *  |CodingRate           |         |input     |1      |Real       |(0:16:0)  |[0:3]       |     |
 *  ----------------------------------------------------------------------------------------------
 *  |HalfIterations       |         |input     |1      |Real       |(0:16:0)  |[1:32]      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |DynamicStopIteration |         |input     |1      |Real       |(0:16:0)  |{0}         |     |
 *  ----------------------------------------------------------------------------------------------
 *  |ScrambleEnable       |         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |ScrambleMask         |         |input     |1      |Real       |(0:16:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |SubpacketId          |         |input     |1      |Real       |(0:16:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |BpBlockInterleaving  |         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |BpEncoder            |         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |BpDecoder            |         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |EnableCombine        |         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |BpRateMatching       |         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |RateMatchingE        |         |input     |1      |Real       |(0:32:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |RateMatchingk0       |         |input     |1      |Real       |(0:16:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |RateMatchingNcb      |         |input     |1      |Real       |(0:16:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |BpChannelInterleaving|         |input     |1      |Real       |(0:16:0)  |{0, 1}      |     |
 *  ----------------------------------------------------------------------------------------------
 *  |CiSymbolNumber       |         |input     |1      |Real       |(0:16:0)  |{12, 14}    |     |
 *  ----------------------------------------------------------------------------------------------
 *  |In                   |         |input     |InLen  |pReal Array|(0:8:0)   |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |InLen                |         |input     |1      |Real       |(0:32:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |Out                  |         |output    |OutLen |pReal Array|(0:8:0)   | full range |     |
 *  ----------------------------------------------------------------------------------------------
 *  |OutLen               |         |input     |1      |Real       |(0:32:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |pDlCbCtx             |         |input     |1      |Real       |(0:32:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  |FecDlCbDone          |         |input     |1      |Real       |(0:32:0)  |full range  |     |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------------------------------------------
 *  |Block Name          |BuildFecTcb_TX                                                         |
 *  ----------------------------------------------------------------------------------------------
 *  |Input Stream(s)     |In                                                                     |
 *  ----------------------------------------------------------------------------------------------
 *  |Output Stream(s)    |Out                                                                    |
 *  ----------------------------------------------------------------------------------------------
 *  |Config Parameter(s) |CodingType,BlockSize,CrcSelector,FillerBits,Modulation,CodingRate,     |
 *  |                    |HalfIterations,DynamicStopIteration,ScrambleEnable,ScrambleMask,       |
 *  |                    |SubpacketId,BpBlockInterleaving,BpEncoder,BpDecoder,EnableCombine,     |
 *  |                    |BpRateMatchingRateMatchingE,RateMatchingk0,RateMatchingNcb,            |
 *  |                    |BpChannelInterleaving,CiSymbolNumber,InLen,OutLen,pDlCbCtx,FecDlCbDone |
 *  ----------------------------------------------------------------------------------------------
 *  |Status Parameter(s) |None                                                                   |
 *  ----------------------------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function BuildFecTcb_TX is used to generate the structure required to
 *  control the Tx FEC hardware.
 *
 * \ingroup group_lte_tcb
 *
 **/
//----------------------------------------------------------------------------------------

PFEC_DL_FDESC BuildFecTcb_TX(U16 CodingType, // Type of encoding or decoding (0=CC, 1=CTC)
                             U16 BlockSize, // Code block size
                             U16 CrcSelector, // Type of CRC
                             U16 FillerBits, // Number of initial filler bits in code block
                             U16 Modulation, // Number of modulation bits used (1,2,4,6)
                             U16 CodingRate, // Coding rate
                             U16 HalfIterations, // Number of decoder 1/2 iterations
                             U16 DynamicStopIteration, // CTC decoder automatic stop select
                             U16 ScrambleEnable, // Scrambling enable
                             U16 ScrambleMask, // Scrambling mask
                             U16 SubpacketId, // Sub-packet ID for the itial transmission
                             U16 BpBlockInterleaving, // Block interleaver bypass
                             U16 BpEncoder, // Encoder bypass
                             U16 BpDecoder, // Decoder bypass
                             U16 EnableCombine, // HARQ soft-combining enable
                             U16 BpRateMatching, // Rate matching bypass
                             U32 RateMatchingE, // Rate matching E parameter
                             U16 RateMatchingk0, // Rate matching k0 parameter
                             U16 RateMatchingNcb, // Rate matching Ncb parameter
                             U16 BpChannelInterleaving, // Channel interleaver bypass
                             U16 CiSymbolNumber, // Number of channel interleaver columns (12 or 14)
                             U8 In[], // Input buffer pointer
                             U32 InLen, // Input buffer length
                             U8 Out[], // Output buffer pointer
                             U32 OutLen, // Output buffer length
                             U32 pDlCbCtx, // Pointer to callback context
                             void *FecDlCbDone) // Pointer to callback function

{
    PFEC_DL_FDESC pFecDL = FecDlAllocDesc(1);

    DLHWCTX hw_ctx =
    { 0 };
    _ASSERT_PTR (pFecDL);

    hw_ctx.CTC_CCn = CodingType;
    hw_ctx.CRC_Sel = CrcSelector;
    hw_ctx.Modulation = Modulation >> 1;
    hw_ctx.Rate_Index = CodingRate;
    hw_ctx.BlockSize = BlockSize; // block size in bits
    hw_ctx.Bypass_Encoder = BpEncoder;
    hw_ctx.Bypass_Interleaver = BpBlockInterleaving;
    hw_ctx.Harq_req = BpRateMatching; // bit 14 for LTE is BypassRateMatching
    hw_ctx.Filler = FillerBits;
    hw_ctx.LTERateMaching_K0 = RateMatchingk0 >> 1;

    FecDlSetData(pFecDL, In, InLen, Out, OutLen);

    if (CodingType == 0)
    { // CC
        hw_ctx.Harq_rep_end = ScrambleEnable & 1;
        hw_ctx.LTERateMaching_K0 = ScrambleMask & 0x3FFF;
        hw_ctx.Harq_spid = (ScrambleMask >> 14) & 3;
    }
    else
    {// CTC
        pFecDL->IoBdesc[1].BCtrl |= (RateMatchingNcb & 0x1FFF) << 18;
        hw_ctx.Ncb_14_13 = (RateMatchingNcb >> 13) & 3;
    }

    if (BpRateMatching)
    {
        pFecDL->IoBdesc[1].BCtrl |= ((pFecDL->IoBdesc[0].BCtrl & 0xFFFF) << 3) << 18;
    }
    else
    {
        hw_ctx.E_Param = RateMatchingE & 0x1FFFF;
    }

    FecDlSetHwCtx(pFecDL, hw_ctx);

    FecDlSetLastDesc(pFecDL, (void(*)(LPVOID)) FecDlCbDone, (LPVOID) pDlCbCtx);

    return pFecDL;

}

#endif  //_WIN32

////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// End FEC TCB Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////


