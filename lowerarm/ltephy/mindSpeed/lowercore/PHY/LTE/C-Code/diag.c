//-------------------------------------------------------------------------------------------
/** @file diag.c
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.39 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "Debug.h"

//#define BYTSWP 1
#undef BYTSWP

////////////////////////////////////////////
//Flags below for enabling debug files to
//debugging eNodeB when running SQV test application
////////////////////////////////////////////
//#define DEBUG_RXVECTOR   1     //logs RxVector
////////////////////////////////////////////

////////////////////////////////////////////
//Open diagnostic files for Uplink Rx
////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   signal[] DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *file DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |signal[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*file         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void LogRxVector_Main(PULSUBFRDESC ptr)
{
#ifdef DEBUG_RXVECTOR

    FILE *fRxVector;

    fRxVector = fopen("Bs_RxVector.txt","at");

    fprintf(fRxVector,"frameNumber: %d\n", ptr->frameNumber);
    fprintf(fRxVector,"subframeNumber: %d\n", ptr->subframeNumber);
    fprintf(fRxVector,"numberofChannelDescriptors: %d\n", ptr->numberofChannelDescriptors);
    fprintf(fRxVector,"numberOfCtrlChannelDescriptors: %d\n", ptr->numberOfCtrlChannelDescriptors);
    fprintf(fRxVector,"numberSrsinSf: %d\n", ptr->numberSrsinSf);
    fprintf(fRxVector,"delta_ss: %d\n", ptr->ulSfrCtrl.groupAssignmentPUSCH);
    fprintf(fRxVector,"groupHoppingEnabled: %d\n", ptr->ulSfrCtrl.groupHoppingEnabled);
    fprintf(fRxVector,"sequenceHoppingEnabled: %d\n\n", ptr->ulSfrCtrl.sequenceHoppingEnabled);

    fclose(fRxVector);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   signal[] DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *file DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |signal[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*file         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void LogRxVector_Pusch(PULSUBFRDESC ptr, S32 index, ULRXFECPARAMS *pRxFecParams)
{
#ifdef DEBUG_RXVECTOR

    FILE *fRxVector;
    PULCHANDESC pInt = NULL;

    fRxVector = fopen("Bs_RxVector.txt","at");

    pInt = &ptr->ulCh[index];

    fprintf(fRxVector,"PUSCH #%d!!!!\n", index);
    fprintf(fRxVector,"harqInfo.nDi: %d\n", ptr->ulCh[index].ulSubChInfo.harqInfo.nDi);
    fprintf(fRxVector,"harqInfo.rV: %d\n",  ptr->ulCh[index].ulSubChInfo.harqInfo.rV);
    fprintf(fRxVector,"physChanType: %d\n",  pInt->channelType);
    fprintf(fRxVector,"txpowerControl: %d\n",  pInt->txpowerControl);
    fprintf(fRxVector,"scrinitValueinput: %d\n",  pInt->ulSubChInfo.scrInfo.scrinitValueinput);
    fprintf(fRxVector,"Qm: %d\n",  pInt->ulSubChInfo.mcinfo.modulationType);
    fprintf(fRxVector,"mcsType: %d\n",  pInt->ulSubChInfo.mcinfo.mcsType);
    fprintf(fRxVector,"G: %d\n",  pRxFecParams->G[index]);
    fprintf(fRxVector,"numBytes: %d\n",  pRxFecParams->numBytes[index]);
    fprintf(fRxVector,"n2_DMRS: %d\n",  pInt->ulSubChInfo.puschDed.nDMRS2);
    fprintf(fRxVector,"startRes: %d\n",  pInt->ulSubChInfo.mapInfo.reselmInfo[0].startRes);
    fprintf(fRxVector,"numRes: %d\n",  pInt->ulSubChInfo.mapInfo.reselmInfo[0].numRes);
    fprintf(fRxVector,"nACK: %d\n", pInt->ulSubChInfo.puschDed.nACK);
    fprintf(fRxVector,"nr1CQI: %d\n", pInt->ulSubChInfo.puschDed.nr1CQI);
    fprintf(fRxVector,"nrg1CQI: %d\n", pInt->ulSubChInfo.puschDed.nrg1CQI);
    fprintf(fRxVector,"nRI: %d\n", pInt->ulSubChInfo.puschDed.nRI);
    fprintf(fRxVector,"nsymi: %d\n", pInt->ulSubChInfo.puschDed.nsymi);
    fprintf(fRxVector,"nrbi: %d\n", pInt->ulSubChInfo.puschDed.nrbi);
    fprintf(fRxVector,"betaOffsetACKIndex: %d\n", pInt->ulSubChInfo.puschDed.betaOffsetACKIndex);
    fprintf(fRxVector,"betaOffsetCQIIndex: %d\n", pInt->ulSubChInfo.puschDed.betaOffsetCQIIndex);
    fprintf(fRxVector,"betaOffsetRIIndex: %d\n\n", pInt->ulSubChInfo.puschDed.betaOffsetRIIndex);

    fclose(fRxVector);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   signal[] DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *file DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |signal[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*file         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void LogRxVector_Pucch(PULSUBFRDESC ptr, S32 index, S16 startRB)
{
#ifdef DEBUG_RXVECTOR

    FILE *fRxVector;
    PULCTRLCHDESC pInt1 = NULL;

    fRxVector = fopen("Bs_RxVector.txt","at");
    pInt1  = (PULCTRLCHDESC) ((U32) ptr + ptr->offsetULCtrlChannels);
    pInt1 += index;

    fprintf(fRxVector,"PUCCH #%d!!!!\n", index);
    fprintf(fRxVector,"physChanType: %d\n",  pInt1->channelType);
    fprintf(fRxVector,"txpowerControl: %d\n",  pInt1->txpowerControl);
    fprintf(fRxVector,"scrinitValueinput: %d\n", pInt1->scrmblerInitValue);
    fprintf(fRxVector,"Qm: %d\n",  pInt1->modulationType);
    fprintf(fRxVector,"formatType: %d\n", pInt1->pucchDedCtl.formatType);
    fprintf(fRxVector,"startRB: %d\n", startRB);
    fprintf(fRxVector,"deltaPUCCHShift: %d\n", ptr->ulSfrCtrl.deltaPUCCHShift);
    fprintf(fRxVector,"n1 or harqPucchIndex: %d\n", pInt1->pucchDedCtl.harqPucchIndex); 
    fprintf(fRxVector,"n1 or srPUCCHResourceIndex: %d\n", pInt1->pucchDedCtl.srPUCCHResourceIndex); 
    fprintf(fRxVector,"n2 or cqiPUCCHResourceIndex: %d\n", pInt1->pucchDedCtl.cqiPUCCHResourceIndex);
    fprintf(fRxVector,"dlCqiPmiSizeBits: %d\n", pInt1->pucchDedCtl.dlCqiPmiSizeBits);
    fprintf(fRxVector,"harqSizebits: %d\n", pInt1->pucchDedCtl.harqSizebits);
    fprintf(fRxVector,"N1_cs: %d\n", ptr->ulSfrCtrl.nCSAn);
    fprintf(fRxVector,"N2_rb: %d\n", ptr->ulSfrCtrl.nRBCQI);
    fprintf(fRxVector,"simSRHarq: %d\n", pInt1->pucchDedCtl.simSRHarq);
    fprintf(fRxVector,"msPucchSrSel: %d\n\n", pInt1->pucchDedCtl.msPucchSrSel);

    fclose(fRxVector);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   signal[] DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *file DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |signal[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*file         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void LogRxVector_Srs(PULSUBFRDESC ptr, S32 index)
{
#ifdef DEBUG_RXVECTOR

    FILE *fRxVector;
    PSRSDED psrsInfo = NULL;

    fRxVector = fopen("Bs_RxVector.txt","at");

    psrsInfo  = (PSRSDED) ((U32) ptr + ptr->offsetsrsInfo);
    psrsInfo += index;

    fprintf(fRxVector,"SRS #%d!!!!\n", index);
    fprintf(fRxVector,"enable: %d\n", psrsInfo[0].enableDisable);
    fprintf(fRxVector,"cyclicShift: %d\n", psrsInfo[0].cyclicShift);
    fprintf(fRxVector,"srsBandwidth: %d\n", psrsInfo[0].srsBandwidth);
    fprintf(fRxVector,"srsHoppingBandwidth: %d\n", psrsInfo[0].srsHoppingBandwidth);
    fprintf(fRxVector,"freqDomainPosition: %d\n", psrsInfo[0].freqDomainPosition);
    fprintf(fRxVector,"duration: %d\n", psrsInfo[0].duration);
    fprintf(fRxVector,"srsConfigIndex: %d\n", psrsInfo[0].srsConfigIndex);
    fprintf(fRxVector,"transmissionComb: %d\n\n", psrsInfo[0].transmissionComb);

    fclose(fRxVector);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   signal[] DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *file DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |signal[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*file         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void LogRxVector_Prach(PULSUBFRDESC ptr, S32 index)
{
#ifdef DEBUG_RXVECTOR

    FILE *fRxVector;
    PRACHCTRL prachCtrl = NULL;

    fRxVector = fopen("Bs_RxVector.txt","at");

    prachCtrl  = (PRACHCTRL) ((U32) ptr + ptr->offsetRachCtrlStruct);
    prachCtrl += index;

    fprintf(fRxVector,"PRACH #%d!!!!\n", index);
    fprintf(fRxVector,"prachEnable: %d\n", prachCtrl->prachEnable);
    fprintf(fRxVector,"prachConfigIndex: %d\n", prachCtrl->prachConfigIndex);
    fprintf(fRxVector,"rootSequenceIndex: %d\n", prachCtrl->rootSequenceIndex);
    fprintf(fRxVector,"highSpeedFlag: %d\n", prachCtrl->highSpeedFlag);
    fprintf(fRxVector,"zeroCorrelationZoneConfig: %d\n", prachCtrl->zeroCorrelationZoneConfig);
    fprintf(fRxVector,"prachFreqOffset: %d\n", prachCtrl->prachFreqOffset);
    fprintf(fRxVector,"prachPreambleNumber: %d\n\n", prachCtrl->prachPreambleNumber);

    fclose(fRxVector);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   numchan DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |numchan       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void diagnostic_file_open(U32 numAnt)
{
    FILE *fTempFile = NULL;

#ifdef _WIN32
    //delete all files that will be appended to with ("at") as the test application runs
    fTempFile = fopen("rx_derotation.txt","wt");
    fclose(fTempFile);
    fTempFile = fopen("mmseNoiseVarAvg.txt","wt");
    fclose(fTempFile);
    fTempFile = fopen("rx_ChanEst_pucch.txt","wt");
    fclose(fTempFile);
    fTempFile = fopen("rx_DemapperIn_pucch.txt","wt");
    fclose(fTempFile);
    fTempFile  = fopen("TcbIOBuf.txt","wt");    //use of this file will be: open - append - close with each instance
    fclose(fTempFile);

    //Add file headers
    fTempFile  = fopen("rx_DemapperIn_pucch.txt","at");
    fprintf(fTempFile, "%%dEstAck[0] dEstAck[1],  chanPow,   noisePowSubf noiseVarTimeAvg berLoopNoiseVar, noisePwrAvg, allPucchPwrAvg, rxInPwrAvg, chanDetected\n");
    fclose(fTempFile);
#endif /* _WIN32 */

#ifdef DEBUG_PRINT

    //DL TX
    f_data_codeword0 = fopen("codeword0.txt","w");
    f_data_codeword1 = fopen("codeword1.txt","w");

    f_data_feca0 = fopen("feca0.txt","w");
    f_data_feca1 = fopen("feca1.txt","w");
    f_data_fecb0 = fopen("fecb0.txt","w");
    f_data_fecb1 = fopen("fecb1.txt","w");

    f_data_fecc0 = fopen("fecc0.txt","w");
    f_data_fecc1 = fopen("fecc1.txt","w");
    f_data_fecd0 = fopen("fecd0.txt","w");
    f_data_fecd1 = fopen("fecd1.txt","w");
    f_data_fecf0 = fopen("fecf0.txt","w");
    f_data_fecf1 = fopen("fecf1.txt","w");
    f_data_mod0_b0  = fopen("mod0_b0.txt","w");
    f_data_mod0_b1  = fopen("mod0_b1.txt","w");
    f_data_mod1_b_0 = fopen("mod1_b_0.txt","w");
    f_data_mod1_b_1 = fopen("mod1_b_1.txt","w");

    f_data_cfi_mod0_b0  = fopen("cfi_mod0_b0.txt","w");
    f_data_cfi_mod1_b_0 = fopen("cfi_mod1_b_0.txt","w");
    f_data_ctl_mod0_b0  = fopen("ctl_mod0_b0.txt","w");
    f_data_ctl_mod1_b_0 = fopen("ctl_mod1_b_0.txt","w");

    f_data_mod2_d0 = fopen("mod2_d0.txt","w");
    f_data_mod2_d1 = fopen("mod2_d1.txt","w");

    f_data_mod3_x[0] = fopen("mod3_x0.txt","w");
    f_data_mod4_y[0] = fopen("mod4_y0.txt","w");
    f_data_mod5_unscaled_a[0] = fopen("mod5_unscaled_a_0.txt","w");
    f_data_mod5a[0]  = fopen("mod5a_0.txt","w");
    f_data_imod5a[0] = fopen("imod5a_0.txt","w");
    f_data_mod5c_ctl_a[0]  = fopen("mod5c_ctl_a0.txt","w");
    f_data_mod5d_bcst_a[0] = fopen("mod5d_bcst_a0.txt","w");
    f_data_mod5f_a[0]    = fopen("mod5f_a0.txt","w");
    f_data_mod_bfps_a[0] = fopen("mod_bfps_a0.txt","w");
    if (numAnt >= 2)
    {
        f_data_mod3_x[1] = fopen("mod3_x1.txt","w");
        f_data_mod4_y[1] = fopen("mod4_y1.txt","w");
        f_data_mod5_unscaled_a[1] = fopen("mod5_unscaled_a_1.txt","w");
        f_data_mod5a[1]  = fopen("mod5a_1.txt","w");
        f_data_imod5a[1] = fopen("imod5a_1.txt","w");
        f_data_mod5c_ctl_a[1]  = fopen("mod5c_ctl_a1.txt","w");
        f_data_mod5d_bcst_a[1] = fopen("mod5d_bcst_a1.txt","w");
        f_data_mod5f_a[1]    = fopen("mod5f_a1.txt","w");
        f_data_mod_bfps_a[1] = fopen("mod_bfps_a1.txt","w");
        if (numAnt > 2)
        {
            f_data_mod3_x[2] = fopen("mod3_x2.txt","w");
            f_data_mod3_x[3] = fopen("mod3_x3.txt","w");

            f_data_mod4_y[2] = fopen("mod4_y2.txt","w");
            f_data_mod4_y[3] = fopen("mod4_y3.txt","w");

            f_data_mod5_unscaled_a[2] = fopen("mod5_unscaled_a_2.txt","w");
            f_data_mod5_unscaled_a[3] = fopen("mod5_unscaled_a_3.txt","w");

            f_data_mod5a[2] = fopen("mod5a_2.txt","w");
            f_data_mod5a[3] = fopen("mod5a_3.txt","w");

            f_data_imod5a[2] = fopen("imod5a_2.txt","w");
            f_data_imod5a[3] = fopen("imod5a_3.txt","w");

            f_data_mod5c_ctl_a[2] = fopen("mod5c_ctl_a2.txt","w");
            f_data_mod5c_ctl_a[3] = fopen("mod5c_ctl_a3.txt","w");

            f_data_mod5d_bcst_a[2] = fopen("mod5d_bcst_a2.txt","w");
            f_data_mod5d_bcst_a[3] = fopen("mod5d_bcst_a3.txt","w");

            f_data_mod5f_a[2] = fopen("mod5f_a2.txt","w");
            f_data_mod5f_a[3] = fopen("mod5f_a3.txt","w");

            f_data_mod_bfps_a[2] = fopen("mod_bfps_a2.txt","w");
            f_data_mod_bfps_a[3] = fopen("mod_bfps_a3.txt","w");
        }
    }

    f_data_k_l     = fopen("k_l.txt","w");
    f_data_k_l_dec = fopen("k_l_dec.txt","w");

    f_data_mod5e_sync_a = fopen("mod5e_sync_a.txt","w");
    f_data_mod6_s       = fopen("mod6_s.txt","w");
    f_data_i16_mod6_s   = fopen("i16_mod6_s.txt","w");
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   signal[] DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *file DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |signal[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*file         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
short my_fread(short signal[], short type, short length, FILE *file)
{
    short nr;

    nr = fread(signal, type, length, file);
#ifdef BYTSWP
    for( i=0; i< length; i++)
    signal[i] = (signal[i] >> 8 & 0x00ff)^(signal[i] << 8);
#endif

    return (nr);
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   signal[] DOXYGEN_TO_DO
 *  @param   type DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *file DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |signal[]      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |type          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |length        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*file         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
short my_fwrite(short signal[], short type, short length, FILE *file)
{
    short nw;
#ifdef BYTSWP
    short tmp_vec[MAX_OUTPUT_BLK_SIZE];
#endif

#ifdef BYTSWP
    for( i=0; i< length; i++)
    tmp_vec[i] = (signal[i] >> 8 & 0x00ff)^(signal[i] << 8);

    nw=fwrite( tmp_vec, type, length, file);
#else
    nw = fwrite(signal, type, length, file);
#endif
    return (nw);
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   numChan DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |numChan       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void diagnostic_file_close(U32 numAnt)
{
    U32 i = 0;

    fclose(f_data1);

#ifdef DEBUG_PRINT
    fclose(f_data_codeword0);
    fclose(f_data_codeword1);

    fclose(f_data_feca0);
    fclose(f_data_feca1);
    fclose(f_data_fecb0);
    fclose(f_data_fecb1);

    fclose(f_data_fecc0);
    fclose(f_data_fecc1);
    fclose(f_data_fecd0);
    fclose(f_data_fecd1);
    fclose(f_data_fecf0);
    fclose(f_data_fecf1);
    fclose(f_data_mod0_b0);
    fclose(f_data_mod0_b1);
    fclose(f_data_mod1_b_0);
    fclose(f_data_mod1_b_1);

    fclose(f_data_cfi_mod0_b0);
    fclose(f_data_cfi_mod1_b_0);
    fclose(f_data_ctl_mod0_b0);
    fclose(f_data_ctl_mod1_b_0);

    fclose(f_data_mod2_d0);
    fclose(f_data_mod2_d1);

    fclose(f_data_k_l);
    fclose(f_data_k_l_dec);

    for (i= 0; i < numAnt; i ++)
    {
        fclose(f_data_mod3_x[i]);
        fclose(f_data_mod4_y[i]);

        fclose(f_data_mod5_unscaled_a[i]);
        fclose(f_data_mod5a[i]);
        fclose(f_data_imod5a[i]);
        fclose(f_data_mod5c_ctl_a[i]);
        fclose(f_data_mod5d_bcst_a[i]);
        fclose(f_data_mod5f_a[i]);
        fclose(f_data_mod_bfps_a[i]); // too many open files error if not close files.
    }

    fclose(f_data_mod5e_sync_a);
    fclose(f_data_mod6_s);
    fclose(f_data_i16_mod6_s);

    fclose(f_data_demod_data_b_all);
    fclose(f_data_demod_data_b_0);

    fclose(f_data_demod1_u0_b);

#endif
}

