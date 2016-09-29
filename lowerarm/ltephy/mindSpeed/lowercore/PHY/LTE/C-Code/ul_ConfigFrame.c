//-------------------------------------------------------------------------------------------
/** @file ul_ConfigFrame.c
 *
 * @brief Setup Config parameters for Uplink at beginning of each radio frame. Common parameters for both ms Tx and bs Rx
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
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"

#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

#ifndef _WIN32
#include "mdmadrv.h"
#include "mlog.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Config Channel parameters for Uplink at beginning of each radio frame. Common parameters for both ms Tx and bs Rx
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param s      = INPUT, Base Station Structure info
 *  @param ul     = INPUT/OUTPUT, Base Station Receiver Structure info
 *  @param ulChan = INPUT/OUTPUT, Base Station Channel/Receiver Structure info
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name   |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |       |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |s      |         |pInput        |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ul     |         |pInput/pOutput|1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ulChan |         |pInput/pOutput|1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ul_ConfigFrame_Chan         |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |s,ul,ulChan                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |ul,ulChan                   |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Config Channel parameters for Uplink at beginning of each radio frame. <BR>
 * Common parameters for both ms Tx and bs Rx
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Uplink"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
#ifndef _WIN32
static U32 tprs = 0;
static U32 thop = 0;

static void GrpHopDone(LPVOID ctx)
{
    MLogTask(PID_RXLIST_GROUP_HOP, RESOURCE_SYS_AXI, thop, GET_TICKS);
    return;
}

static void PrsBitsDone(LPVOID ctx)
{
    MLogTask(PID_RXLIST_PRSBITS, RESOURCE_SYS_AXI, tprs, GET_TICKS);
    return;
}
#endif

void ul_ConfigFrame_Chan(BS *s, BS_RX *pBsRx)
{

#ifndef _WIN32
    PMDMA_FDESC pMdma;
    volatile U32 t;
#endif

    S16 temp;
    S16 fPUSCH_ss;
    S32 cLen;
    U32 cinit;
    S16 CE_NumBits;

    PBS_PUSCH pBsPusch;

    S32 length_cvec;

    //
    //Generate Pilot subcarrier random number sequence for each PUSCH [3GPP TS 36.211 v.8.5.0, Sec.5.5.2.1.1]
    //
    // (This sequence depends on the Cell ID and PUSCH id)
    //ch_ptr1 = ulChan;
    temp = divide_floor(s->Ncell_id, 30);

    //delta_ss = integer from set {0,1,...,29}, configured by higher layers
    //init pseudorandom seq generator at beginning of radio frame
    //   fPUSCH_ss = (s->Ncell_id + ch_ptr1->delta_ss) % 30;
    //no need to do (s->Ncell_id % 30), just use s->Ncell_id, they are the same
    fPUSCH_ss = (s->Ncell_id + pBsRx->pUlRxInputParams->delta_ss) % 30;

    pBsPusch = pBsRx->pBsPusch;

    //Need temp buffer of zeros to send through MDMA scrambler to obtain
    //the PsuedoRandomGen outputs
    memset(pBsPusch->cvec, 0, 7 * 20);
    cinit = temp;

    // Generate Random sequence for entire frame of length 8*7*20
#ifdef _WIN32
    if (pBsRx->pUlRxInputParams->groupHoppingEnabled == 1) //Group Hopping is enabled
    {
        U8 *cvec_u;

        cvec_u = MemAlloc(8*7*20,CRAM_MEM, 0);
        length_cvec = 8*7*20;
        PseudoRandomSeqGen(length_cvec, s->Ncell_id/30, cvec_u);

        PACKBB(cvec_u, pBsPusch->cvec_u, length_cvec, 0);

        MemFree(cvec_u,CRAM_MEM);
    }
#else
    if (pBsRx->pUlRxInputParams->groupHoppingEnabled == 1) //Group Hopping is enabled
    {
        tprs = GET_TICKS;
        length_cvec = 7 * 20;
        pMdma = MdmaAllocDesc (MDMA_HW_SYS, 1);
        _ASSERT_PTR (pMdma);
        MdmaSet4GSCR(pMdma, cinit, 1, 0, 0);
        MdmaSetData(pMdma, pBsPusch->cvec, length_cvec, pBsPusch->cvec_u, length_cvec);
        MdmaRunExQ (MDMA_HW_SYS, pMdma, GrpHopDone, NULL, 200);
    }
#endif

    //#if defined(IXIA_IOT)
    //    if (0)
    //#else
    //    if (1)
    //#endif
    //    {
    //       //!!!!!!Agilent bug: ADS 2008 Update 2 is done this way!
    //       CE_NumBits = 4; //=number of bits from random number generator used per slot
    //    }
    //    else
    //    {
    //!!!!!!As defined in standard...
    CE_NumBits = 8; //=number of bits from random number generator used per slot
    //    }

    cinit = (U32) (temp * 32 + fPUSCH_ss); // 32 is 2^5
    cLen = CE_NumBits * pBsRx->Nul_symb * (20 - 1) + CE_NumBits;
    //where, 20=number of slots per frame
#ifdef _WIN32
    {
        U8 *cvec_v;

        cvec_v = MemAlloc(cLen,CRAM_MEM, 0);
        PseudoRandomSeqGen(cLen, cinit, cvec_v); //First output bit in lsb

        PACKBB(cvec_v, pBsPusch->PRSbits, cLen, 0);
        MemFree(cvec_v,CRAM_MEM);
    }
#else
    tprs = GET_TICKS;
    length_cvec = cLen >> 3;
    pMdma = MdmaAllocDesc (MDMA_HW_SYS, 1);
    _ASSERT_PTR (pMdma);
    MdmaSet4GSCR(pMdma, cinit, 1, 0, 0);
    MdmaSetData(pMdma, pBsPusch->cvec, length_cvec, pBsPusch->PRSbits, length_cvec);
    MdmaRunExQ (MDMA_HW_SYS, pMdma, PrsBitsDone, NULL, 200);
#endif

}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Config Global parameters for Uplink PRACH at beginning of each radio frame. Common parameters for both ms Tx and bs Rx
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param s      = INPUT, Base Station Structure info
 *  @param ul     = INPUT/OUTPUT, Base Station Receiver Structure info
 *  @param subframeNum = INPUT, SubframeNumber
 *  @param ul_prach = INPUT/OUTPUT, PRACH structure info
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name    |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |        |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |s       |         |pInput        |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ul      |         |pInput/pOutput|1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ul_prach|         |pInput/pOutput|1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |subframe|         |pInput        |1       |Real          |(1:15:0)    |[0:9]       |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ul_ConfigFrame_PRACH_Globals|
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |s,ul,ul_prach,subframeNum   |
 *  ---------------------------------------------------
 *  |Status Parameters   |ul,ul_prach                 |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Config Global parameters for PRACH Uplink at beginning of each radio frame. <BR>
 * Common parameters for both ms Tx and bs Rx
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Uplink"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void ul_ConfigFrame_PRACH_Globals(BS *s, BS_RX *pBsRx, BS_RX_PRACH *ul_prach, S16 subframeNum)
{
    S16 confIndex, Nfft, Nrb_sc, Nul_rb;
    S16 frmChk = 0;
    S16 i = 0, len = 0, ilen = 0;

    //----------------------------------------------------------------------------------------------------------------
    //(4) Init PRACH Preamble for this radio frame [3GPP TS 36.211 v.8.7.0, Sec.5.7]
    //
    //Note:-
    // - Only calculate here the parameters that are common for all channels in the UL path, since individual
    //   channels will change on a subframe basis...
    // - So, the only values calculated and stored here are; ?????
    //preambleFormat = ul_prach.bs_prach_preambleFormat; //[3GPP 36.211 Rel-8, Table 5.7.1-2]
    if (ul_prach->bs_prach_enable == 1)
    {
        //init local variables, from config parameters in input structures
        confIndex = ul_prach->bs_prach_configurationIndex;
        Nfft = s->Nfft;
        Nrb_sc = s->Nrb_sc;
        Nul_rb = pBsRx->Nul_rb;

        //calculate variables, fixed for all subframes
        ul_prach->bs_prach_Nseq = 12 * Nfft; //IFFT length for PRACH preamble transmitter
        ul_prach->bs_prach_Ncp = 12 * 264 * Nfft / 2048; //Cyclic prefix length for PRACH preamble transmitter
        ul_prach->bs_prach_preambleFormat = CA_prachFrameStructureTable[confIndex]; //[3GPP 36.211 Rel-8, Table 5.7.1-2]
        if (ul_prach->bs_prach_preambleFormat <= 3)
        {
            ul_prach->bs_prach_preambleSeqLength = 839; //Nzc, Zadoff Chu sequence length
            ul_prach->bs_prach_subcSpacing = 1250; //Hz, Deltaf_ra = subcarrier frequency spacing for PRACH preamble
            ul_prach->bs_prach_subcOffset = 7; //phi = offset to first subcarrier used for PRACH preamble in the resource block
        }
        else //preambleFormat == 4
        {
            ul_prach->bs_prach_preambleSeqLength = 139; //Nzc, Zadoff Chu sequence length
            ul_prach->bs_prach_subcSpacing = 7500; //Hz, Deltaf_ra = subcarrier frequency spacing for PRACH preamble
            ul_prach->bs_prach_subcOffset = 2; //phi = offset to first subcarrier used for PRACH preamble in the resource block
        }

        ul_prach->bs_prach_subcSpacingRatio = s->df / ul_prach->bs_prach_subcSpacing; //K, ratio of subcarrier spacing for uplink data over subcarrier spacing for uplink prach preamble
        if (s->frameDuplexType == FDD)
            ul_prach->bs_prach_nRa_prb = ul_prach->bs_prach_frequencyOffset; //(RB#) = nRa_prbOffset
        else
            //s.frameDuplexType == CE_TDD
            printf("error in ul_ConfigFrame(): Error, PRACH for TDD is not supported yet\n");

        ul_prach->bs_prach_k0 = ul_prach->bs_prach_nRa_prb * Nrb_sc - Nul_rb * Nrb_sc / 2; //=base frequency index (as 15kHz index)

        //
        // Calculate subframes where PRACH Preamble is enabled
        // - [3GPP TS 36.211 Rel-8, Sec.5.7]
        //   - Check the subframe numbers where PRACH Preamble may be enabled, for this radio frame number
        //   - Store this state in an array for all subframes in this radio frame

        ul_prach->bs_prach_preambleSubframeEnable[subframeNum] = 0; //initialize PRACH preamble as disabled in this subframe

        // check current frame number against PRACH-Preamble-configured frame numbers
        frmChk = 0;
        // add 64 to skip over preamble to get frameNumber
        if (CA_prachFrameStructureTable[64 + confIndex] == CE_Any)
            frmChk = 1;
        else if (CA_prachFrameStructureTable[64 + confIndex] == CE_Even)
        {
            if ((pBsRx->frameNumber & 0x1) == 0) //if lsb=0, then the value is even
                frmChk = 1;
        }
        //else CA_prachFrameStructureTable(confIndex+CE_MI1).frameNumber == CE_NA
        //frmChk = 0;

        //check each subframe number against PRACH-Preamble-configured subframe numbers
        if (frmChk == 1)
        {
            //len = length(CA_prachFrameStructureTable(confIndex+CE_MI1).subframeNumber);
            len = 0;
            for (i = 0; i < 10; i++)
            {
                // add 128 to skip over preamble and frameNumber to get subframenumber
                // 10 since there are 10 entries per index
                if (CA_prachFrameStructureTable[128 + 10 * confIndex + i] != -1)
                    len++;
                else
                    break;
            }
            for (ilen = 0; ilen < len; ilen++)
            {
                if (subframeNum == CA_prachFrameStructureTable[128 + 10 * confIndex + ilen])
                {
                    //frameNumber and subframeNumber both match the configuration for PRACH Preamble transmission
                    ul_prach->bs_prach_preambleSubframeEnable[subframeNum] = 1; //enable the PRACH preamble in this subframe
                }
            }
        }
    }
    else
    {
        //ul.prach.enable == 0
        ul_prach->bs_prach_preambleSubframeEnable[subframeNum] = 0; //initialize PRACH preamble as disabled in this subframe
        ul_prach->bs_prach_preambleSeqLength = -1; //[];  %Nzc, Zadoff Chu sequence length
        ul_prach->bs_prach_subcSpacing = -1; //[]; %Hz, df_ra
    } //ul.prach.enable
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Config Global parameters for SRS Uplink at beginning of each radio frame. Common parameters for both ms Tx and bs Rx
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param s      = INPUT, Base Station Structure info
 *  @param ul     = INPUT/OUTPUT, Base Station Receiver Structure info
 *  @param ul_srs = INPUT/OUTPUT, SRS structure info
 *  @param subframeNum = INPUT, subframe Number
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name    |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |        |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |s       |         |pInput        |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ul      |         |pInput/pOutput|1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ul_srs  |         |pInput/pOutput|1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |subframe|         |pInput        |1       |Real          |(1:15:0)    |[0:9]       |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ul_ConfigFrame_SRS_Globals  |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |s,ul,ul_srs,subframe        |
 *  ---------------------------------------------------
 *  |Status Parameters   |ul,ul_srs          |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Config Global parameters for SRS Uplink at beginning of each radio frame. <BR>
 * Common parameters for both ms Tx and bs Rx
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Uplink"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void ul_ConfigFrame_SRS_Globals(BS *s, BS_RX *pBsRx, BS_SRS *ul_srs, S16 subframeNum)
{
    S16 srsSubframeConfig;
    S16 subframePeriod;
    S16 *subframeOffset;
    S16 subframeOffsetLength;

    S16 subframeNumber;

    S16 index;

    //----------------------------------------------------------------------------------------------------------------
    //(3) Generate SRS random number sequences for this radio frame [3GPP TS 36.211 v.8.7.0, Sec.5.5.3]
    //
    //Note:-
    // - Only calculate here the parameters that are common for all channels in the UL path, since individual
    //   channels will change on a subframe basis...
    // - So, the only values calculated and stored here are; u(ns)
    //
    ul_srs->srsBandwidthTab = 0;
    ul_srs->srsPilotSymbol = -1;
    if (ul_srs->srsEnable == 1)
    {
        //init local variables, from config parameters in input structures
        //  C_srs = ul->srs_bandwidthConfig;     //cell specific bandwidth config
        srsSubframeConfig = ul_srs->srsSubframeConfig;
        subframePeriod = CA_srsSubframeConfigTable[10 * srsSubframeConfig];
        subframeOffset = &CA_srsSubframeConfigTable[10 * srsSubframeConfig + 2];
        subframeOffsetLength = CA_srsSubframeConfigTable[10 * srsSubframeConfig + 1];

        // Store SRS symbol number within the subframe
        if (s->frameDuplexType == FDD)
        {
            ul_srs->srsPilotSymbol = 2 * pBsRx->Nul_symb - 1; //for FDD, SRS is always sent in the last symbol of the subframe
        }
        else //s->frameDuplexType == CE_TDD
        {
            printf("error in ul_ConfigFrame(): Error, SRS for TDD is not supported yet");
        }

        // Get Cell-specific bandwidth config table
        //  GGGG   ul->srs.bandwidthConfigTableRow = ul->srs.bandwidthConfigTable.config[C_srs];
        // Calculate subframes where SRS is enabled
        //- [3GPP TS 36.211 Rel-8, Sec.5.5.3.3]
        //  - Calculate the subframe number where SRS may be enabled.
        //  - Check this calculated number against the configured SRS subframes
        //  - If these two numbers match, enable the SRS in this subframe

        subframeNumber = subframeNum % subframePeriod; //subframe number where the SRS is enabled, if configured
        ul_srs->srsSubframeEnable[subframeNum] = 0; //initialize SRS as disabled in this subframe
        for (index = 0; index < subframeOffsetLength; index++)
        {
            if (subframeNumber == *(subframeOffset + index)) //check calculated subframe number against configured subframe numbers
            {
                ul_srs->srsSubframeEnable[subframeNum] = 1; //enable the SRS in this subframe
            }
        }

        //index_tab1 {0, 1, 2, 3} points to the part of table depending on Nul_Rb
        //index_tab2 {0, 1, 2, 3, 4, 5, 6, 7} points to row depending on Csrs
        if (pBsRx->Nul_rb > 40 && pBsRx->Nul_rb <= 60)
        {
            ul_srs->srsBandwidthTab = 1;
        }
        else if (pBsRx->Nul_rb > 60 && pBsRx->Nul_rb <= 80)
        {
            ul_srs->srsBandwidthTab = 2;
        }
        else if (pBsRx->Nul_rb > 80) //  && < 110
        {
            ul_srs->srsBandwidthTab = 3;
        }
    }
    else //ul.srs.enable == 0
    {
        ul_srs->srsSubframeEnable[subframeNum] = 0; //initialize SRS as disabled in this subframe
    } //ul.srs.enable == 1
}
// @brief ul_PrachPreamble_Config(): Generate Uplink PRACH Preamble sequence [3GPP TS 36.211 Rel-8, Sec.5.7]
//
//
// input parameters:-
//   @param ul_prach      = structure of ul.prach parameters, i.e. PRACH preamble parameters common to all uplink channels
//   @param ulChan_prach  = structure of ulChan.prach(ichan) parameters, i.e. PRACH preamble parameters specific to this uplink channel
//
// return values:-
//   @return u    = u'th root for Zadoff-Chu cazac sequence
//   @return Cv   = cyclic shift index within Zadoff-Chu sequence
//   @return vlen = number of cyclic shifts for each uRootIndex
//   @return Ncs  = number of samples of cyclic shift between each user sequence
//
// detailed description:-
//  @details Generate Uplink PRACH preamble [3GPP TS 36.211 Rel-8, Sec.5.7]
//

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Generate Uplink PRACH Preamble sequence for Uplink at beginning of each radio frame. Common parameters for both ms Tx and bs Rx
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param ul_prach = INPUT/OUTPUT, BS_RX_PRACH structure info
 *  @param ulChan_prach = INPUT/OUTPUT, BS_RX_PRACHDet structure info
 *  @param u = OUTPUT, SRS structure info
 *  @param Cv = OUTPUT, SRS structure info
 *  @param vLen = OUTPUT, SRS structure info
 *  @param Ncs = OUTPUT, SRS structure info
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ul_prach      |         |pInput        |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |ulChan_prach  |         |pInput        |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |u             |         |pOutput       |1       |pReal         |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |Cv            |         |pOutput       |1       |pReal         |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |vLen          |         |pOutput       |1       |pReal         |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |Ncs           |         |pOutput       |1       |pReal         |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ul_PrachPreamble_Config     |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |ul_prach,ulChan_prach       |
 *  |                    |u, Cv, vLen, Ncs            |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Generate Uplink PRACH preamble. <BR>
 * Common parameters for both ms Tx and bs Rx
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel-8, Sec.5.7, "Uplink"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void ul_PrachPreamble_Config(BS_RX_PRACH *ul_prach, BS_RX_PRACHDet *ulChan_prach, S16 *u, S16 *Cv,
                             S16 *vLen, S16 *Ncs)
{
    S16 *prachRootSeqNumberTable;
    //  S16 preambleFormat;
    S16 Nzc;
    S16 uRootIndex;
    S16 NcsConfig;
    S16 highSpeedFlag;
    S16 cyclicShift;
    S16 uOffset;
    S16 v;
    S16 uLogical;

    //printf("ul_PrachPreamble_Config\n");

    // Extract local variables from the input ul_prach and ulChan_prach data structures
    //    preambleFormat = ul_prach->bs_prach_preambleFormat; //[3GPP 36.211 Rel-8, Table 5.7.1-2]
    Nzc = ul_prach->bs_prach_preambleSeqLength; //Zadoff Chu preamble sequence length
    uRootIndex = ul_prach->bs_prach_rootSequenceIndex; //(0..837), RACH_ROOT_SEQUENCE, [3GPP 36.211, Sec.5.7.1]
    NcsConfig = ul_prach->bs_prach_cyclicShiftConfig; //BS cyclic shift configuration index
    highSpeedFlag = ul_prach->bs_prach_highSpeedFlag; //TRUE=1 uses Restricted Set, FALSE=0 uses Unrestricted Set, [3GPP 36.211, Table 5.7.2-2]
    cyclicShift = ulChan_prach->bs_prachDet_cyclicShift; //i.e. user dependent cyclic shift index, one of the 64 possible sequences for this cell, randomly selected by this mobile
    uOffset = ulChan_prach->bs_prachDet_uOffset; //i.e. used by BS only! Force uOffset value below, if this parameter !=0

    //Init derived parameters

    // - set Nzc depending on preambleFormat, [3GPP 36.211 Rel-8, Table 5.7.2-1]
    prachRootSeqNumberTable = CA_prachRootSeqNumberTable0_3; //point to table
    //    if(preambleFormat == 4)
    //    {
    //       prachRootSeqNumberTable = CA_prachRootSeqNumberTable4;  //point to table
    //    }


    //
    //Calculate u'th root and v'th cyclic shift value for the given MS configuration of cyclicShift [3GPP 36.211 Rel-8, Sec.5.7.2]
    //

    // - (1) calculate the number of cyclic shifts for each uRootIndex, for this cyclicShiftConfig
    *Ncs = CA_prachPreambleGenTable_Ncs[NcsConfig + 16 * highSpeedFlag];
    if (highSpeedFlag == CE_FALSE)
    {
        //Unrestricted cyclic shift set
        if (*Ncs == 0)
            *vLen = 1;
        else
            *vLen = divide_floor(Nzc, *Ncs); //floor(Nzc/Ncs);  %number of cyclic shifts for each uRootIndex, for this cyclicShiftConfig
    }
    else
    {
        //Restricted cyclic shift set
        printf("error in ms_tx_Preamble(): PhyError - Restricted Set ");
        printf("for cyclic shifts is not supported yet\n");
    }

    // - (2) calculate the values of u and v used by this MS.
    if (uOffset == 0)
        uOffset = divide_floor(cyclicShift, *vLen); //floor(cyclicShift/vLen);      %offset in u from uRootIndex, calculated from the MS cyclicShift in range 0:63

    uLogical = (uRootIndex + uOffset) % 838; //uLogical = uRootIndex + uOffset
    *u = prachRootSeqNumberTable[uLogical]; //u, physical sequence number

    v = cyclicShift % (*vLen); //= Cyclic shift to use with u'th root sequence

    // - (3) calculate Cv from v, for this MS config
    if (highSpeedFlag == CE_FALSE)
    {
        //Unrestricted cyclic shift set
        *Cv = v * (*Ncs);
    }
    else
    {
        //Restricted cyclic shift set
        printf("error in ms_tx_Preamble(): PhyError - Restricted ");
        printf("Set for cyclic shifts is not supported yet\n");
    }
    //---- end of ul_PrachPreamble_Config() function ----
}

