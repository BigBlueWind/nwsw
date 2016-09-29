//-------------------------------------------------------------------------------------------
/** @file bs_tx_MultiChannels.c
 *
 * @brief LTE BS Transmitter, Physical Channel processing from input
 *        codewords to output resource elements
 * @author Mindspeed Technologies
 * @version $Revision: 1.45 $
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

#ifdef _WIN32
#define WIN_BLD 1
#else
#define WIN_BLD 0
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#include "global.h"
extern void DmaFromExtDram(void *p_ext_addr, void *p_int_addr, unsigned size);
extern U8 *DataBuff_Bank1, *DataBuff_Bank2_Off;

#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif

//-------------------------------------------------------------------------------------------
/** @brief Symbol processing of TxMapper for all data channels
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   *pTxSys - Input structure containing all variables needed to run TxMapper for all
 *                     data channels for each symbol
 *  @param   *pIn    - Input structure containing all input stream from Scrambler for all data channels
 *  @param   *pOut   - Output structure containing all output streams from TxMapper for all data channels
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction|Length        |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |         |              |              |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pTxSys  |         |pInput   |              |pStructure    |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pIn     |         |pInput   |              |pStructure    |(0:8:0)     |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pOut    |         |pOutput  |              |pStructure    |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |tx_Mapper_MultiChan|
 *  ------------------------------------------
 *  |Input Stream        |pIn                |
 *  ------------------------------------------
 *  |Output Stream       |pOut               |
 *  ------------------------------------------
 *  |Config Parameters   |pTxSys             |
 *  ------------------------------------------
 *  |Status Parameters   |none               |
 *  ------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function will call TxMapper for all data channels present in each symbol of the subframe.  This
 *  will keep the number of TCBs constant regardless of the number of data channels present in the
 *  current subframe.  For more detailed explanation of tx_Mapper, please refer to:
 *
 *  - tx_Mapper()
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void tx_Mapper_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars,
                         TXMAPPERMULTICHANIN *pIn, TXMAPPERMULTICHANOUT *pOut, U32 isym)
{
    U32 ichan;
    U32 TotalSubc;
    U32 numChan;
    U32 numCtlCh;
    U32 pbIndex, currIndex;
    S16 *pTxOut;
    S32 i;

    numChan = pTxInPars->numChan;
    numCtlCh = pTxInPars->numCtrlChan;

    TotalSubc = 0;
    for (ichan = numCtlCh; ichan < numChan; ichan++)
    {
        pOut->Buf4M0symb[ichan] = 0;
        pOut->Buf4M1symb[ichan] = 0;
        currIndex = ichan * MAX_SYM_PER_SUBFRAME + isym;
        if (pTxInPars->M0symb[currIndex] > 0)
        {
            if (pTxInPars->length_b0[currIndex] != 0)
            {
                pbIndex = pTxInPars->pbIndex0[currIndex];
                pTxOut = (S16 *) &(pOut->TxMapperOutBuf0[2 * TotalSubc]);
                tx_Mapper((U8 *) &(pIn->TxPackedBitBuf0[pbIndex]), pTxInPars->Nsubc0[currIndex],
                    (S16)pTxInPars->Borrowed0[currIndex], pTxInPars->modType0[ichan], pTxOut);
                TotalSubc += pTxInPars->Nsubc0[currIndex];
                // Allocate 3 more zero subcarriers for STC
                for (i = 0; i < 6; i++)
                {
                    pOut->TxMapperOutBuf0[2 * TotalSubc + i] = 0;
                }
                TotalSubc += 3;
            }
        }
    }

    TotalSubc = 0;
    for (ichan = numCtlCh; ichan < numChan; ichan++)
    {
        currIndex = ichan * MAX_SYM_PER_SUBFRAME + isym;
        if ((pTxInPars->numCodeWords[ichan] == 2) && (pTxInPars->M1symb[currIndex] > 0))
        {
            if (pTxInPars->length_b1[currIndex] != 0)
            {
                pbIndex = pTxInPars->pbIndex1[currIndex];
                pTxOut = (S16 *) &(pOut->TxMapperOutBuf1[2 * TotalSubc]);
                tx_Mapper((U8 *) &(pIn->TxPackedBitBuf1[pbIndex]), pTxInPars->Nsubc1[currIndex],
                          (S16)pTxInPars->Borrowed1[currIndex], pTxInPars->modType1[ichan], pTxOut);
                TotalSubc += pTxInPars->Nsubc1[currIndex];
                // Allocate 3 more zero subcarriers for STC
                for (i = 0; i < 6; i++)
                {
                    pOut->TxMapperOutBuf1[2 * TotalSubc + i] = 0;
                }
                TotalSubc += 3;
            }
        }
    }
    return;
}

//----------------------------------------------------------

//-------------------------------------------------------------------------------------------
/** @brief Symbol processing of LayerMapper for all data channels
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   *pTxSys - Input structure containing all variables needed to run LayerMapper for all
 *                     data channels for each symbol
 *  @param   *pIn    - Input structure containing all input stream from TxMapper for all data channels
 *  @param   *pOut   - Output structure containing all output streams from LayerMapper for all data channels
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction|Length        |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |         |              |              |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pTxSys  |         |pInput   |              |pStructure    |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pIn     |         |pInput   |              |pStructure    |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pOut    |         |pOutput  |              |pStructure    |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------
 *  |Block Name          |LayerMapper_MultiChan|
 *  --------------------------------------------
 *  |Input Stream        |pIn                  |
 *  --------------------------------------------
 *  |Output Stream       |pOut                 |
 *  --------------------------------------------
 *  |Config Parameters   |pTxSys               |
 *  --------------------------------------------
 *  |Status Parameters   |none                 |
 *  --------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function will call LayerMapper for all data channels present in each symbol of the subframe.  This
 *  will keep the number of TCBs constant regardless of the number of data channels present in the
 *  current subframe.  For more detailed explanation of LayerMapper, please refer to:
 *
 *  - LayerMapper()
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void LayerMapper_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars, TXMAPPERMULTICHANOUT *pIn,
                           TXLAYERMAPPERMULTICHANOUT *pOut, U32 isym)
{
    U32 ichan, numChan, numCtlCh, currIndex;
    U8 transmode;
    S16 numTxLayers;
    //S16 Q;
    S32 M0symb, M1symb, alloc_length;
    S16 temp;
    S32 stc_alloc_length;
    S16 *px;
    S32 xIndex, nr, nc;
    S32 idxIQ0, idxIQ1, nIQ0, nIQ1;
    S16 *pd0IQ;
    S16 *pd1IQ;

#ifdef DEBUG_PRINT
    S32 i,j;
#endif

    numChan = pTxInPars->numChan;
    numCtlCh = pTxInPars->numCtrlChan;

    xIndex = 0;
    idxIQ0 = 0;
    idxIQ1 = 0;
    for (ichan = numCtlCh; ichan < numChan; ichan++)
    {
        currIndex = ichan * MAX_SYM_PER_SUBFRAME + isym;
        transmode = (U8) pTxInPars->transmissionMode[ichan];
        numTxLayers = (S16) pTxInPars->numTxLayers[ichan];
        M0symb = (S32) pTxInPars->M0symb[currIndex];
        M1symb = (S32) pTxInPars->M1symb[currIndex];
        pd0IQ = (S16 *) &(pIn->TxMapperOutBuf0[idxIQ0]);
        pd1IQ = (S16 *) &(pIn->TxMapperOutBuf1[idxIQ1]);

#ifdef DEBUG_PRINT
        for (i = 0; i < M0symb; i++)
        {
            fprintf(f_data_mod2_d0, "%f %f\n", pd0IQ[2 * i] / 16384.0,
                pd0IQ[2 * i + 1] / 16384.0);
        }
        for (i = 0; i < M1symb; i++)
        {
            fprintf(f_data_mod2_d1, "%f %f\n", pd1IQ[2 * i] / 16384.0,
                pd1IQ[2 * i + 1] / 16384.0);
        }
#endif
        temp = (S16) M0symb;
        if (M1symb > temp)
        {
            temp = (S16) M1symb;
        }
        alloc_length = temp * numTxLayers * 2;
        if (transmode != TXDIVERSITY) // Layer Mapping for non-STC
        {
            nIQ0 = (S32) (2 * pTxInPars->Nsubc0[currIndex]);
            nIQ1 = (S32) (2 * pTxInPars->Nsubc1[currIndex]);
            if (alloc_length != 0)
            {
                ////map 1 or 2 input codewords to up to 4 output layers
                ////////////////////////
                pOut->xIndex[ichan] = (U16) xIndex;
                ////////////////////////
                px = (S16 *) &(pOut->TxLayerMapperOutBuf[xIndex]);
                LayerMapper(pd0IQ, pd1IQ, M0symb, M1symb, numTxLayers, px, transmode, &nr, &nc);
                xIndex += alloc_length;
                idxIQ0 += nIQ0;
                idxIQ1 += nIQ1;
                idxIQ0 += 6; // 3 more zero subcarriers for STC
                idxIQ1 += 6;
#ifdef DEBUG_PRINT
                for ( j = 0; j < nr; j++)
                {
                    for ( i=0; i< nc; i++)
                    {
                        fprintf(f_data_mod3_x[j],"%f %f\n",
                            px[2*(i+j*nc)]/16384.0,
                            px[2*(i+j*nc)+1]/16384.0);
                    }
                }
#endif
            }
        }
        else
        {
            nIQ0 = (S32) (2 * pTxInPars->Nsubc0[currIndex]);
            M0symb = (S32) pTxInPars->M0symb[currIndex];
            stc_alloc_length = pTxInPars->Ntx_antennas * M0symb * 2;
            if (stc_alloc_length != 0)
            {
                ////////////////////////
                pOut->xIndex[ichan] = (U16) xIndex;
                ////////////////////////
                px = (S16 *) &(pOut->TxLayerMapperOutBuf[xIndex]);
                STC(pd0IQ, M0symb, pTxInPars->Ntx_antennas, px);
                xIndex += stc_alloc_length;
                idxIQ0 += nIQ0;
                idxIQ0 += 6; // 3 more zero subcarriers for STC
            }
        }
    }
}

//----------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief Symbol processing of Precoder for all data channels
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   *pTxSys - Input structure containing all variables needed to run Precoder for all
 *                     data channels for each symbol
 *  @param   *pIn    - Input structure containing all input stream from LayerMapper for all data channels
 *  @param   *pOut   - Output structure containing all output streams from Precoder for all data channels
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction|Length        |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |         |              |              |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pTxSys  |         |pInput   |              |pStructure    |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pIn     |         |pInput   |              |pStructure    |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pOut    |         |pOutput  |              |pStructure    |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------
 *  |Block Name          |Precoder_MultiChan   |
 *  --------------------------------------------
 *  |Input Stream        |pIn                  |
 *  --------------------------------------------
 *  |Output Stream       |pOut                 |
 *  --------------------------------------------
 *  |Config Parameters   |pTxSys               |
 *  --------------------------------------------
 *  |Status Parameters   |none                 |
 *  --------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function will call Precoder for all data channels present in each symbol of the subframe.  This
 *  will keep the number of TCBs constant regardless of the number of data channels present in the
 *  current subframe.  For more detailed explanation of Precoder, please refer to:
 *
 *  - precoder()
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void Precoder_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars, TXLAYERMAPPERMULTICHANOUT *pIn,
                        TXPRECODERMULTICHANOUT *pOut, U32 isym)
{
    PRECODERPARAM PrecoderIn;
    U32 ichan, numChan, numCtlCh;
    U32 transmissionMode, currIndex;
    S32 length_y;
    S16 temp;
    S32 M0symb, M1symb;
    S32 Mlayer_symb;
    S32 i, j;
    S32 yIndex;
    S16 *px;
    S16 *py;
    S32 stc_alloc_length;

    numChan = pTxInPars->numChan;
    numCtlCh = pTxInPars->numCtrlChan;
    yIndex = 0;

    for (ichan = numCtlCh; ichan < numChan; ichan++)
    {
        currIndex = ichan * MAX_SYM_PER_SUBFRAME + isym;
        //precode the complex constellation points, in layers, to antenna port resource element values
        Mlayer_symb = pTxInPars->ncol_x0[currIndex];
        transmissionMode = (U32) pTxInPars->transmissionMode[ichan];
        ///////////////////////////////////////////////////////////
        //Section below temporary added to accommodate running
        //on the host.  Will pass in this structure into TCB
        //so that we dont' have to copy entire PHY structure.
        ///////////////////////////////////////////////////////////

        PrecoderIn.Ntx_antennas = (U8) pTxInPars->Ntx_antennas;
        PrecoderIn.txSubFrameNumber = (U8) pTxInPars->subframeNumber;
        PrecoderIn.numCtrlSymbols = (U8) pTxInPars->numCtrlSymbols;

        PrecoderIn.transmissionMode = (U8) transmissionMode; // transmissionMode
        PrecoderIn.nLayers = (U8) pTxInPars->numTxLayers[ichan]; //nLayers;
        PrecoderIn.codeBookIdx = (U8) pTxInPars->codeBookIdx[ichan];
        PrecoderIn.cddType = (U8) pTxInPars->cddType[ichan];

        if (transmissionMode != TXDIVERSITY) // Layer Mapping for non-STC
        {
            M0symb = pTxInPars->M0symb[currIndex];
            M1symb = pTxInPars->M1symb[currIndex];

            temp = (S16) M0symb;
            if (M1symb > temp)
            {
                temp = (S16) M1symb;
            }
            if (temp != 0)
            {
                ////////////////////////
                i = pIn->xIndex[ichan];
                ////////////////////////
                px = (S16 *) &(pIn->TxLayerMapperOutBuf[i]);
                py = (S16 *) &(pOut->TxPrecoderOutBuf[yIndex]);

                precoder(PrecoderIn, px, py, &length_y, Mlayer_symb);
                ////////////////////////
                pOut->yIndex[ichan] = (U16) yIndex;
                ////////////////////////
                yIndex += 2 * length_y;
#ifdef DEBUG_PRINT
                for ( j = 0; j < (S32)PrecoderIn.nLayers; j++)
                {
                    for ( i=0; i<length_y; i++)
                    {
                        fprintf(f_data_mod4_y[j],"%f %f\n",
                            py[2*(i+j*length_y)]/16384.0,
                            py[2*(i+j*length_y)+1]/16384.0);
                    }
                }
#endif
            }
        } // if STC
        else
        {
            // move the data from pIn->x[] to pOut->y[]

            M0symb = pTxInPars->M0symb[currIndex]; // pChan->M0symb[isym];
            stc_alloc_length = pTxInPars->Ntx_antennas * M0symb * 2;
            if (stc_alloc_length != 0)
            {
                ////////////////////////
                j = pIn->xIndex[ichan];
                ////////////////////////
                py = (S16 *) &(pOut->TxPrecoderOutBuf[yIndex]);
                for (i = 0; i < stc_alloc_length; i++)
                {
                    py[i] = pIn->TxLayerMapperOutBuf[j + i];
                }
                ////////////////////////
                pOut->yIndex[ichan] = (U16) yIndex;
                ////////////////////////
                yIndex += stc_alloc_length;
            }
        }
    }
}

//----------------------------------------------------------

//-------------------------------------------------------------------------------------------
/** @brief Symbol processing of DLResourceElementMapper for all data channels
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   *pTxSys - Input structure containing all variables needed to run DLResourceElementMapper for all
 *                     data channels for each symbol
 *  @param   *pIn    - Input structure containing all input stream from Precoder for all data channels
 *  @param   *pOut   - Output structure containing all output streams from DLResourceElementMapper for all data channels
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction|Length        |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |         |              |              |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pTxSys  |         |pInput   |              |pStructure    |            |            |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pIn     |         |pInput   |              |pStructure    |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  |*pOut    |         |pOutput  |              |pStructure    |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |DLResourceElementMapper_MultiChan|
 *  --------------------------------------------------------
 *  |Input Stream        |pIn                              |
 *  --------------------------------------------------------
 *  |Output Stream       |pOut                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pTxSys                           |
 *  --------------------------------------------------------
 *  |Status Parameters   |none                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function will call DLResourceElementMapper for all data channels present in each symbol of the subframe.  This
 *  will keep the number of TCBs constant regardless of the number of data channels present in the
 *  current subframe.  For more detailed explanation of DLResourceElementMapper, please refer to:
 *
 *  - DLResourceElementMapper()
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void DLResourceElementMapper_MultiChan(TXSYSTEMINFO *pTxSys, DLTXINPUTPARAMS *pTxInPars, TXPRECODERMULTICHANOUT *pIn,
                                       DLRESELEMMAPPERMULTICHANOUT *pOut, U32 isym)
{
    S16 mi;
    U32 ichan, numChan, numCtlCh, currIndex;
    S16 ncol_y, yIndex;
    S16 scale_factor;
    S16 *pscIndex;
    S16 *pa;
    S16 *py;
    PTXSYSTEMINFOINNER pTxSysA;
    S16 ScaleRhob;

#ifdef DEBUG_PRINT
    S32 i;
    S32 numrows_a;
#endif

    pTxSysA = &(pTxSys->a);

    numChan = pTxInPars->numChan;
    numCtlCh = pTxInPars->numCtrlChan;
#ifdef DEBUG_PRINT
    numrows_a = pTxInPars->Ndl_rb * pTxInPars->Nrb_sc;
#endif

    ScaleRhob = RbRa_ratio[pTxInPars->Pb];
    if(pTxInPars->Ntx_antennas > 1)
        ScaleRhob = RbRa_ratio[4 + pTxInPars->Pb];

    for (mi = 0; mi < pTxInPars->Ntx_antennas; mi++)
    {
        pscIndex = pOut->scIndex;
        pa = pTxSysA->aSymbol[mi];
        for (ichan = numCtlCh; ichan < numChan; ichan++)
        {
            currIndex = ichan * MAX_SYM_PER_SUBFRAME + isym;
            ncol_y = (S16) pTxInPars->ncol_y0[currIndex];
            ////////////////////////
            yIndex = pIn->yIndex[ichan] + 2 * mi * ncol_y;
            ////////////////////////
            py = (S16 *) &(pIn->TxPrecoderOutBuf[yIndex]);
            scale_factor = pOut->scale_factor[ichan];
            if(pTxInPars->Ntx_antennas != 4 && pTxInPars->channelType[ichan] == PDSCH && (isym == 0 || isym == 4 || isym == 7 || isym == 11))
            {
              scale_factor = shl(mult(scale_factor, ScaleRhob), 1); //shift left to bring it back to Q14
            }
            else if(pTxInPars->Ntx_antennas == 4 && pTxInPars->channelType[ichan] == PDSCH && (isym == 0 || isym == 1 || isym == 4 || isym == 7 || isym == 8 || isym == 11))
            {
              scale_factor = shl(mult(scale_factor, ScaleRhob), 1); //shift left to bring it back to Q14
            }
            if (pTxInPars->M0symb[currIndex] > 0)
            {
#ifdef CEVA_INTRINSICS11
                DLResourceElementMapper_A(pa, py, pscIndex, ncol_y, scale_factor);
#else
                DLResourceElementMapper(pa, py, pscIndex, ncol_y, scale_factor);
#endif
                pscIndex = pscIndex + ncol_y;
            }
        }

#ifdef DEBUG_PRINT
        for (i = 0; i < numrows_a; i++)
        {
            fprintf(f_data_mod5_unscaled_a[mi],"%f %f\n",
                pa[2*i]/16384.0,
                pa[2*i+1]/16384.0);
        }
#endif
    }
}

//-------------------------------------------------------------------------------------------
/** @brief Groups several Downlink Tx Modulation functions into a single call per symbol
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   pVars   - Structure includes all variables need to run functions for all data channels in each symbol
 *  @param   *pIn    - Input structure containing all input stream from Scrambler for all data channels
 *  @param   pTxSys  - Input structure containing all information needed to run all functions for all data channels
 *                     in each symbol.
 *  @param   pSigOut - Final output from DLResourceElementMapper_MultiChan
 *  @param   syncP   - Input buffer containing Primary Syncronization signal
 *  @param   syncS   - Input buffer containing Secondary Syncronization signal
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------
 *  |Name    |Name in  |Direction|Length       |Type          |Data Format |Valid Range |Units |
 *  |        |Standard |         |             |              |            |            |      |
 *  --------------------------------------------------------------------------------------------
 *  |pVars   |         |pInput   |1            |pStructure    |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------
 *  |pIn     |         |pInput   |1            |pStructure    |(0:8:0)     |full range  |      |
 *  --------------------------------------------------------------------------------------------
 *  |pTxSys  |         |pInput   |1            |pStructure    |(1:15:0)    |full range  |      |
 *  --------------------------------------------------------------------------------------------
 *  |pSigOut |         |pOutput  |Ndl_rb*Nrb_sc|pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------
 *  |syncP   |         |pInput   |31           |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------
 *  |syncS   |         |pInput   |31           |pComplex      |(1:0:15)    |full range  |      |
 *  --------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------
 *  |Block Name          |tx_MultiChan_Modulation  |
 *  ------------------------------------------------
 *  |Input Stream        |pIn,syncP,syncS          |
 *  ------------------------------------------------
 *  |Output Stream       |pSigOut                  |
 *  ------------------------------------------------
 *  |Config Parameters   |pVars,pTxSys             |
 *  ------------------------------------------------
 *  |Status Parameters   |none                     |
 *  ------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function groups Downlink Resource Element Assignment, TxMapper, LayerMapper, Precoder,
 *  Downlink Pilots, Syncronization signals, and Downlink Resource Element Mapper into a single
 *  call per symbol.   This reduces the number of interrupts from Ceva to ARM and keeps the number
 *  TCBs constant regardless of the number of data channels present in the subframe.  For
 *  more detailed explanation of functions grouped here, please refer to:
 *
 *  -# DLResourceElementAssignment()
 *  -# tx_Mapper_MultiChan()
 *  -# LayerMapper_MultiChan()
 *  -# Precoder_MultiChan()
 *  -# DlPilot()
 *  -# BsTxSyncCH()
 *  -# DLResourceElementMapper_MultiChan()
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void tx_MultiChan_Modulation(PDLTXINPUTPARAMS pVars, PTXMODULATIONPARAMS pIn, TXSYSTEMINFO *pTxSys,
                             TXSYMBUFS *pTxSymBufs, TXSYMBUF *pTxSymBuf, S16 *pSigOut[], S16 *syncP, S16 *syncS, S32 isym)
{
    //U16 row_a;
    U32 j, length_pb;
    S16 Ndl_symb, slotNumber;
    PTXSYSTEMINFOINNER pTxSysA;
    U8 *b_0, *b_1;
    S32 bIndex0, bIndex1;
    S32 bitFirst;
    PTXSYMBUF pTxSymBufCurr = pTxSymBuf;

#ifdef _WIN32
    if((S16)isym < pVars->numCtrlSymbols)
    {
        return;
    }
#endif

    pTxSysA = &(pTxSys->a);
    if (pVars->numChan > 0)
    {
        StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DMA_IN);
        for (j = pVars->numCtrlChan; j < pVars->numChan; j++)
        {
            pIn->pOut_d->scale_factor[j] = pVars->phyChanScale_factor[j];
        }

        // DLRESELEMMAPPERMULTICHANOUT's scIndex
        if(pTxSymBufs->sizeLocRE)
        {
            MemCpyFromExt((void*) pIn->pOut_d->scIndex, (void*) pTxSymBufs->locRE,
                pTxSymBufs->sizeLocRE);
        }

        bIndex0 = bIndex1 = 0;
        pTxSymBufCurr = pTxSymBuf;
        for (j = pVars->numCtrlChan; j < pVars->numChan; j++)
        {
            b_0 = &(pIn->scrOut1[pIn->scrIndex1[j]]);
            length_pb = pTxSymBufCurr->size0;
            bitFirst = pTxSymBufCurr->bitFirst0;

            MemCpyFromExt((void*) &(pIn->pIn_a->TxPackedBitBuf0[bIndex0]),
                (void*) &b_0[bitFirst], length_pb);

            bIndex0 += length_pb;

            // TXMAPPERMULTICHANIN's TxPackedBitBuf1
            if (pVars->numCodeWords[j] == 2)
            {
                b_1 = &(pIn->scrOut2[pIn->scrIndex2[j]]);
                length_pb = pTxSymBufCurr->size1;
                bitFirst = pTxSymBufCurr->bitFirst1;

                MemCpyFromExt((void*) &(pIn->pIn_a->TxPackedBitBuf1[bIndex1]),
                    (void*) &b_1[bitFirst], length_pb);

                bIndex1 += length_pb;
            }
            pTxSymBufCurr += 1;
        }
        StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DMA_IN);

        StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_MAPPER);
        tx_Mapper_MultiChan(pTxSys, pVars, pIn->pIn_a, pIn->pOut_a, isym);
        StopAndStartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_MAPPER, SUBTASKID_TX_MULTICHAN_MODULATION_LAYERMAPPER);

        LayerMapper_MultiChan(pTxSys, pVars, pIn->pOut_a, pIn->pOut_b, isym);
        StopAndStartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_LAYERMAPPER, SUBTASKID_TX_MULTICHAN_MODULATION_PRECODER);

        Precoder_MultiChan(pTxSys, pVars, pIn->pOut_b, pIn->pOut_c, isym);
        StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_PRECODER);
    }

    StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_MEMSET);
    for (j = 0; j < pVars->Ntx_antennas; j++)
        memset(pTxSysA->aSymbol[j], 0, pVars->NumSubCars);
    StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_MEMSET);

    slotNumber = pVars->slotNumber;

    Ndl_symb = pVars->Ndl_symb;
    if (((isym >= 0) && (isym <= (Ndl_symb - 3))) || ((isym >= 7) && (isym <= (2 * Ndl_symb - 3))))
    {
        StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DLPILOT);
        DlPilot(pVars->Ncell_id, pVars->CPType, pVars->Ndl_rb, Ndl_symb, pVars->Nrb_sc,
                pVars->Ntx_antennas, pVars->pilotScaleFactor, (S16)isym, slotNumber, pTxSysA->aSymbol);
        StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DLPILOT);
    }

#if 0 // will enable when optimization is done
    if(pVars->PRS_EnableFlag)
    {
       if ( (isym == 3) || (isym == 5) || (isym == 6) || (isym == 8) || (isym == 9) || (isym == 10) || (isym == 12) || (isym == 13))
       {

            DlPilotPosition(pVars->Ncell_id, pVars->CPType, pVars->Ndl_rb, pVars->Ndl_rbPrs, Ndl_symb, pVars->Nrb_sc,
                            pVars->Ntx_antennas, pVars->PoSpilotScaleFactor, (S16)isym, slotNumber, pTxSysA->aSymbol);
       }
    }
#endif

    if ((slotNumber == 0) || (slotNumber == 10))
    {
        if ((isym == (Ndl_symb - 2)) || (isym == (Ndl_symb - 1)))
        {
            //Send Synch signals for FDD frame structure, only in slotNumbers 0 and 10
            StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_BSTXSYNCCH);
            BsTxSyncCH(pVars->cellSector, pVars->Ndl_rb, pVars->Nrb_sc, Ndl_symb,
                       pVars->primarySyncScaleFactor, pVars->secondarySyncScaleFactor,
                       pVars->CE_SyncTxAntennaNum, slotNumber, (S16)isym, pTxSysA->aSymbol, syncP, syncS);
            StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_BSTXSYNCCH);
        }
    }

    if (pVars->numChan > 0)
    {
        StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DLRESELEMMAPPER);
        DLResourceElementMapper_MultiChan(pTxSys, pVars, pIn->pOut_c, pIn->pOut_d, isym);
        StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DLRESELEMMAPPER);
    }
}

//-------------------------------------------------------------------------------------------
/** @brief Setting up pointers to Scrambler buffers for Symbol processing
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   isymstart     - First symbol to process
 *  @param   isymstop      - Last symbol to process
 *  @param   ichanStart    - First PDSCH
 *  @param   numChan       - total number of channels
 *  @param   Ntx_antennas  - number of Tx Ants
 *  @param   RBIndexMid6Lo - First resource block containing PBCH or Sync signals
 *  @param   RBIndexMid6Hi - Last resource block containing PBCH or Sync signals
 *  @param   pChan1        - Pointer to all Channel structures for current subframe
 *  @param   *index        - Array containing all chanIDs for PDSCH
 *  @param   *pResInfo     - Resource Block allocation for all PDSCH
 *  @param   *pTxSysIn     - Input structure to tx_multichannel_modulation.
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction |Length  |Type          |Data Format |Valid Range      |Units  |
 *  |              |Standard  |          |        |              |            |                 |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |isymstart     |          |input     |1       |unsigned int  |(0:32:0)    |[0:3]            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |isymstop      |          |input     |1       |unsigned int  |(0:32:0)    |[0:13]           |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |ichanStart    |          |input     |1       |unsigned int  |(0:32:0)    |[0:numChan]      |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |numChan       |          |input     |1       |unsigned int  |(0:32:0)    |[0:MAXCHSUBFRAME]|       |
 *  -----------------------------------------------------------------------------------------------------
 *  |Ntx_antennas  |          |input     |1       |unsigned short|(0:16:0)    |[1,2,4]          |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |RBIndexMid6Lo |          |input     |1       |short         |(1:15:0)    |[0,4,9,22,...    |       |
 *  |              |          |          |        |              |            | 34, 47]         |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |RBIndexMid6Hi |          |input     |1       |short         |(1:15:0)    |[5,10,15,...     |       |
 *  |              |          |          |        |              |            | 27, 40, 52]     |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pChan1        |          |pInput    |numChan |structure     |            |                 |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |*index        |          |pInput    |numChan |unsigned int  |(0:32:0)    |[0:32767]        |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |*pResInfo     |          |pInput    |numChan |structure     |            |                 |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |*pMem     |          |pInput    |buffer |unsigned char*     |            |                 |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |*pTxSysIn     |          |pOutput   |numChan |structure     |            |                 |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |SetupSymbDataInpBufs               |
 *  ----------------------------------------------------------
 *  |Input Stream        |none                               |
 *  ----------------------------------------------------------
 *  |Output Stream       |none                               |
 *  ----------------------------------------------------------
 *  |Config Parameters   |isymstart,isymstop,ichanStart      |
 *  |                    |numChan,Ntx_antennas,RBIndexMid6Lo |
 *  |                    |RBIndexMid6Hi,pChan1,index,pResInfo, pMem|
 *  ----------------------------------------------------------
 *  |Status Parameters   |pTxSysIn                           |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Determines the number of bytes from Scrambler output should be used per symbol.  This function
 *  prepares the pTxSys structure so that the function tx_multichannel_modulation can be run per
 *  symbol.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void SetupSymbDataInpBufs(U32 isymstart, U32 isymstop, U32 ichanStart, U32 numChan,
                          U16 Ntx_antennas,  TXSYMBUFS *pTxSymBufsOut, TXSYMBUF *pTxSymBufOut, 
                          TXSYMBUFS *pTxSymBufsOutCeva, TXSYMBUF *pTxSymBufCeva, 
                          DLTXINPUTPARAMS *pTxInPars)
{
    S32 nBits, bitFirst;
    U32 j;
    U32 bIndex0, bIndex1;
    S32 TotalEntries;
    U16 *locRE;
    U32 isym, isymSlot, nDlSymb;
    S32 nSymb;
    PTXSYMBUFS pTxSymBufs;
    PTXSYMBUF pTxSymBufCurr;
    S16 currNumRE;
    U32 currBit0, currBit1;
    S16 antenna_index;
    S16 antenna_index1;
    S16 subframeNumber;
    S16 cond1x, cond2x;
    S16 cond1x_ini, cond2x_ini;
    S16 cond3;
    S16 nRbSc;
    S16 *tab_ptrPdsch, *tab_ptrPbch;
    S16 *tab_ptrPdschCurr, *tab_ptrPbchCurr;
    U32 moduloNum, currIndex;

    S16 temp1;
    S16 Ndl_rb_Prs, Ndl_rb;
    S16 startRbPrs, endRbPrs;

    S16 const1, startRePrs , endRePrs;
    S16 vshift, flagPrs,  blksize_VRB = 0;
    S16 *tab_ptrPdschAll[4];
    S16 *tabPtrVRB = NULL, *tabPtrVRB1 = NULL, *tabPtrVRB2 = NULL;

#ifndef CEVA_INTRINSICS
    S32 k, n;
    S16 length_kIndexRange;
    S16 count;
    S16 k0, k0_start;
    S16 cond, cond1, cond2, cond4;
    RESELEMINFO *pCurrResInfo;
    S16 nPRB, NumRBs, nPRB_tilde;
    S32 row;
    S16 prevNumRE[MAXCHSUBFRAME];
    S16 Nlast0[MAXCHSUBFRAME];
    S16 Nlast1[MAXCHSUBFRAME];
    U32 prevBit0[MAXCHSUBFRAME];
    U32 prevBit1[MAXCHSUBFRAME];
#else
    U32 ptrCeva;
    S16* Nlast0;
    S16* Nlast1;
    S16* prevNumRE;
    U32* prevBit0;
    U32* prevBit1;
    PTXSYMBUFS pTxSymBufsCurr;
    PTXSYMBUF pTxSymBufCurrCeva;
#endif

#ifndef CEVA_INTRINSICS
    memset(prevNumRE, 0, MAXCHSUBFRAME * 2);
    memset(Nlast0, 0, MAXCHSUBFRAME * 2);
    memset(Nlast1, 0, MAXCHSUBFRAME * 2);
    memset(prevBit0, 0, MAXCHSUBFRAME * 4);
    memset(prevBit1, 0, MAXCHSUBFRAME * 4);
#else
    ptrCeva = (U32) pTxSymBufCeva;
    j = ptrCeva + numChan * sizeof(TXSYMBUF);
    ptrCeva = RUP32B(j);
    prevBit0 = (U32*)ptrCeva;
    j = numChan * sizeof(S16);
    ptrCeva += 2 * RUP32B(j);
    prevBit1 = (U32*)ptrCeva;
    ptrCeva += 2 * RUP32B(j);
    prevNumRE = (S16*)ptrCeva;
    ptrCeva += RUP32B(j);
    Nlast0 = (S16*)ptrCeva;
    ptrCeva += RUP32B(j);
    Nlast1 = (S16*)ptrCeva;
    memset(prevBit0, 0, 9 * RUP32B(j));
#endif

    nRbSc = (U8) pTxInPars->Nrb_sc;
    nDlSymb = (U8) pTxInPars->Ndl_symb;
    antenna_index = pTxInPars->Ntx_antennas >> 1;
    antenna_index1 = antenna_index * 12 * 7;
    subframeNumber = (U8) pTxInPars->subframeNumber;

    Ndl_rb_Prs = pTxInPars->Ndl_rbPrs;
    Ndl_rb = pTxInPars->Ndl_rb;

    tab_ptrPbch = NULL;
    switch (pTxInPars->Ncell_id % 3)
    {
        case 0:
            tab_ptrPbch = REinSubfrmSym_PBCH_ROT0;
            break;
        case 1:
            tab_ptrPbch = REinSubfrmSym_PBCH_ROT1;
            break;
        case 2:
            tab_ptrPbch = REinSubfrmSym_PBCH_ROT2;
            break;
    }

    moduloNum = 6; // 1 antenna
    if (Ntx_antennas != 1) // 2 or 4 antennas
    {
        moduloNum = 3;
    }
       
    tab_ptrPdschAll[0] = NULL;
    tab_ptrPdschAll[1] = NULL;
    tab_ptrPdschAll[2] = NULL;
    tab_ptrPdschAll[3] = NULL;
    switch (pTxInPars->Ncell_id % moduloNum)
       {
        case 0:
            tab_ptrPdschAll[0] = REinSubfrmSym_PDSCH_ROT0;
            tab_ptrPdschAll[1] = REinSubfrmSym_PDSCH_PRS_ROT0;
            tab_ptrPdschAll[2] = REinSubfrmSym_PDSCH_PRS_ROT0_Type1;
            tab_ptrPdschAll[3] = REinSubfrmSym_PDSCH_PRS_ROT0_Type2;
               
                break;
        case 1:
            tab_ptrPdschAll[0] = REinSubfrmSym_PDSCH_ROT1;
            tab_ptrPdschAll[1] = REinSubfrmSym_PDSCH_PRS_ROT1;
            tab_ptrPdschAll[2] = REinSubfrmSym_PDSCH_PRS_ROT1_Type1;
            tab_ptrPdschAll[3] = REinSubfrmSym_PDSCH_PRS_ROT1_Type2;
               
                break;
        case 2:
            tab_ptrPdschAll[0] = REinSubfrmSym_PDSCH_ROT2;
            tab_ptrPdschAll[1] = REinSubfrmSym_PDSCH_PRS_ROT2;
            tab_ptrPdschAll[2] = REinSubfrmSym_PDSCH_PRS_ROT2_Type1;
            tab_ptrPdschAll[3] = REinSubfrmSym_PDSCH_PRS_ROT2_Type2;
               
                break;
        case 3:
            tab_ptrPdschAll[0] = REinSubfrmSym_PDSCH_ROT3;
            tab_ptrPdschAll[1] = REinSubfrmSym_PDSCH_PRS_ROT3;
            tab_ptrPdschAll[2] = REinSubfrmSym_PDSCH_PRS_ROT3_Type1;
            tab_ptrPdschAll[3] = REinSubfrmSym_PDSCH_PRS_ROT3_Type2;
               
                break;
        case 4:
            tab_ptrPdschAll[0] = REinSubfrmSym_PDSCH_ROT4;
            tab_ptrPdschAll[1] = REinSubfrmSym_PDSCH_PRS_ROT4;
            tab_ptrPdschAll[2] = REinSubfrmSym_PDSCH_PRS_ROT4_Type1;
            tab_ptrPdschAll[3] = REinSubfrmSym_PDSCH_PRS_ROT4_Type2;
               
                break;
        case 5:
            tab_ptrPdschAll[0] = REinSubfrmSym_PDSCH_ROT5;
            tab_ptrPdschAll[1] = REinSubfrmSym_PDSCH_PRS_ROT5;
            tab_ptrPdschAll[2] = REinSubfrmSym_PDSCH_PRS_ROT5_Type1;
            tab_ptrPdschAll[3] = REinSubfrmSym_PDSCH_PRS_ROT5_Type2;
               
                break;
        }

    if(pTxInPars->DistributedExistFlag == 1) // Existence of Distributed channels, Initial pointers depends on BW
    {
        
       tabPtrVRB1 = NULL;
       tabPtrVRB2 = NULL;
       switch (Ndl_rb)
       {
           case 6:
                tabPtrVRB1 = nVRB_BW_1_4; // Slot0
                tabPtrVRB2 = &nVRB_BW_1_4[6]; // Slot1
                blksize_VRB =  6; // 12/2
                break;
            case 15:
                tabPtrVRB1 = nVRB_BW_3;
                tabPtrVRB2 = &nVRB_BW_3[14];
                blksize_VRB =  14; // 28/2
                break;
            case 25:
                tabPtrVRB1 = nVRB_BW_5;
                tabPtrVRB2 = &nVRB_BW_5[24];
                blksize_VRB =  24; // 48/2
                break;
            case 50:
                tabPtrVRB1 = nVRB_BW_10;
                tabPtrVRB2 = &nVRB_BW_10[92];
                blksize_VRB =  92; // 184/2
                break;
            case 75:
                tabPtrVRB1 = nVRB_BW_15;
                tabPtrVRB2 = &nVRB_BW_15[128];
                blksize_VRB =  128; // 256/2
                break;
            case 100:
                tabPtrVRB1 = nVRB_BW_20;
                tabPtrVRB2 = &nVRB_BW_20[192];
                blksize_VRB =  192; // 384/2
                break;
        }
    }
    
    vshift = pTxInPars->Ncell_id  % 6;
    cond3 = ((pTxInPars->RBIndexMid6Hi - pTxInPars->RBIndexMid6Lo) == 6);
    cond1x_ini = 0;
    cond2x_ini = 0;
    if(subframeNumber == 0)
        cond1x_ini = 1;

    if((subframeNumber == 0) ||(subframeNumber == 5))
        cond2x_ini = 1;

    for (isym = isymstart; isym < isymstop; isym++)
    {
        TotalEntries = 0;

        pTxSymBufCurr = (PTXSYMBUF) ((U8*)pTxSymBufOut + isym*RUP32B(MAXCHSUBFRAME*sizeof(TXSYMBUF)));

#ifdef CEVA_INTRINSICS
        pTxSymBufs = pTxSymBufsOutCeva;
        pTxSymBufCurrCeva = (PTXSYMBUF) ((U8*)pTxSymBufCeva);
#else
        pTxSymBufs = (PTXSYMBUFS)((U8*)pTxSymBufsOut + isym*RUP32B(sizeof(TXSYMBUFS)));
#endif
        locRE = pTxSymBufs->locRE;
        pTxSymBufs->sizeLocRE = 0;
        pTxSymBufs->sizeBuf0 = 0;
        pTxSymBufs->sizeBuf1 = 0;
        bIndex0 = 0;
        bIndex1 = 0;

        isymSlot = isym;
        tabPtrVRB = tabPtrVRB1;
        if (isym >= nDlSymb)
        {
            isymSlot = isym - nDlSymb;
            tabPtrVRB = tabPtrVRB2;
        }

        const1 = (6 - isymSlot + vshift) % 6;
        startRePrs = 6 * (Ndl_rb - Ndl_rb_Prs) + const1;
        endRePrs   = 6 * ( Ndl_rb + Ndl_rb_Prs -1) + const1;

        startRbPrs = startRePrs/12;
        endRbPrs = endRePrs/12;

        isymSlot = isymSlot * 12;
        tab_ptrPbchCurr = &tab_ptrPbch[isymSlot];
        
        //These 2 lines are done here temporary till assembly is upgraded
        tab_ptrPdsch = tab_ptrPdschAll[0];
        tab_ptrPdschCurr = &tab_ptrPdsch[isymSlot + antenna_index1];
    
        cond1x = cond1x_ini;
        cond2x = cond2x_ini;
        if(isym < nDlSymb || isym > (nDlSymb + 3) )
        {
            cond1x = 0;
        }
        if( isym < (nDlSymb - 2) || isym > (nDlSymb - 1) )
        {
            cond2x = 0;
        }

        for (j = ichanStart; j < numChan; j++)
        {
            currNumRE = 0;
            currIndex = j * MAX_SYM_PER_SUBFRAME + isym;
            pTxInPars->M0symb[currIndex] = 0;
            pTxInPars->M1symb[currIndex] = 0;

#ifdef CEVA_INTRINSICS
            if ((pTxInPars->channelType[j] == PDSCH))
            {
                SetupSymbDataInpBufs_part1_A(pTxInPars->RBIndexMid6Lo, pTxInPars->RBIndexMid6Hi,
                    pTxInPars->numberofEntries[j], nRbSc, cond1x, cond2x, cond3, &locRE,
                    &pTxInPars->reselmInfo[j][0], tab_ptrPdschCurr, &currNumRE,
                    NumREinSubfrmSym_PDSCH[antenna_index][isym]);
                pTxSymBufs->sizeLocRE += 2*currNumRE;
            }
            else if (((pTxInPars->channelType[j] == PBCH)) && ((isym >= 7) && (isym <= 10)))
            {
                SetupSymbDataInpBufs_part2_A(pTxInPars->RBIndexMid6Lo, pTxInPars->RBIndexMid6Hi,
                    pTxInPars->numberofEntries[j], nRbSc, cond1x, cond2x, cond3, &locRE,
                    &pTxInPars->reselmInfo[j][0], tab_ptrPbchCurr, &currNumRE,
                    NumREinSubfrmSym_PBCH[0][isym], NumREinSubfrmSym_PBCH[1][isym]);
                pTxSymBufs->sizeLocRE += 2*currNumRE;
            }
#else
            if ((pTxInPars->channelType[j] == PDSCH) || ((isym >= 7) && (isym <= 10)))
            {
                pCurrResInfo = &pTxInPars->reselmInfo[j][0];

                for (row = 0; row < pTxInPars->numberofEntries[j]; row++)
                {
                    nPRB = pCurrResInfo[row].startRes;  //resource block number
                    nPRB_tilde = nPRB;
                    if(pTxInPars->DistributedFlag[j] == 1)
                         nPRB = tabPtrVRB[nPRB + blksize_VRB * pTxInPars->NgapFlag[j]]; // to access entries of Ngap = 2;
                    NumRBs = pCurrResInfo[row].numRes;  //number of consecutive resource blocks
                                                        //in this assignment
                    k0_start = nPRB * nRbSc;
                    count = 0;

                    for (n = 0; n < NumRBs; n++)
                    {
                        cond = ((nPRB >= pTxInPars->RBIndexMid6Lo) && (nPRB <= pTxInPars->RBIndexMid6Hi));
                        cond4 = cond3 && ((nPRB == pTxInPars->RBIndexMid6Lo) || (nPRB == pTxInPars->RBIndexMid6Hi));

                        // Allow for Control, Broadcast and Synch Physical Channels
                        // before assigning ResourceElements to PDSCH
                        cond1 = (cond1x && cond);
                        cond2 = (cond2x && cond);

                        if ((cond1 || cond2 == 1) && cond4 == 1)
                        {
                            count++;
                        }
                        else
                        {
                            count = 0;
                        }

                        if (cond1 && (count != 1 || pTxInPars->channelType[j] == PBCH))
                        {
                            if (pTxInPars->channelType[j] == PBCH)
                            {
                                //
                                //PBCH (broadcast channel)
                                //
                                // Add the number of ResourceElements for this ResourceBlock and symbol to PBCH
                                k0 = k0_start;  //frequency index for first resource element in
                                                // the first resource block of this resource assignment ordered pair
                                if (nPRB == pTxInPars->RBIndexMid6Lo && cond3 == 1)
                                {
                                    k0 += nRbSc >> 1;
                                }

                                length_kIndexRange = NumREinSubfrmSym_PBCH[cond4][isym];
                                currNumRE += length_kIndexRange;
                                for (k = 0; k < length_kIndexRange; k++)
                                {
                                    *locRE++ = k0 + tab_ptrPbchCurr[k];
                                }
                            }
                        }
                        else if (cond2 && count != 1)
                        {
                            //synchronization channel
                            // So no k_l ResourceElement assignment to PDSCH
                        }
                        else
                        {
                            if (pTxInPars->channelType[j] == PDSCH)
                            {
                                // PDSCH
                                // So, assign k_l ResourceElements for this symbol to PDSCH
                                flagPrs = 1;
                                if(nPRB < startRbPrs || nPRB > endRbPrs || pTxInPars->PRS_EnableFlag == 0) // no PRS
                                {
                                    tab_ptrPdsch = tab_ptrPdschAll[0];
                                    length_kIndexRange = NumREinSubfrmSym_PDSCH[antenna_index][isym];
                                    flagPrs = 0;
                                }
                                else if (nPRB > startRbPrs && nPRB < endRbPrs) //  PRS
                                {
                                    tab_ptrPdsch = tab_ptrPdschAll[1];
                                    length_kIndexRange = NumREinSubfrmSym_PDSCH_PRS[antenna_index][isym];
                                    
                                }
                                else if (nPRB == startRbPrs)
                                {
                                    temp1 = startRePrs % 12;
                                    if(temp1 < 6)
                                    {
                                          tab_ptrPdsch = tab_ptrPdschAll[1];
                                          length_kIndexRange = NumREinSubfrmSym_PDSCH_PRS[antenna_index][isym];
                                          
                                    }
                                    else
                                    {
                                         tab_ptrPdsch = tab_ptrPdschAll[3];
                                         length_kIndexRange = NumREinSubfrmSym_PDSCH_PRS_Type1_2[antenna_index][isym];
                                    }
                                 
                                }
                                else // nPRB == endRbPrs
                                {
                                    temp1 = endRePrs % 12;
                                    if(temp1 < 6)
                                    {
                                          tab_ptrPdsch = tab_ptrPdschAll[2];
                                          length_kIndexRange = NumREinSubfrmSym_PDSCH_PRS_Type1_2[antenna_index][isym];
                                    }
                                    else
                                    {
                                         tab_ptrPdsch = tab_ptrPdschAll[1];
                                         length_kIndexRange = NumREinSubfrmSym_PDSCH_PRS[antenna_index][isym];
                                    }
                                }

                                tab_ptrPdschCurr = &tab_ptrPdsch[isym * 12 + 2 * antenna_index1]; // Need the 2 since we have all 14 symbols
                                if(flagPrs == 0)
                                   tab_ptrPdschCurr = &tab_ptrPdsch[isymSlot + antenna_index1]; // Unlike PRS, we need and have only 7 symbols

                                 
                                k0 = k0_start;  //frequency index for first resource element
                                                // in the first resource block of this resource assignment ordered pair
                                if (cond4 && (cond1 == 1 || cond2 == 1))
                                {
                                    if (nPRB == pTxInPars->RBIndexMid6Hi)
                                    {
                                        k0 += nRbSc >> 1;
                                    }
                                    length_kIndexRange = length_kIndexRange >> 1;
                                }

                                currNumRE += length_kIndexRange;
                                for (k = 0; k < length_kIndexRange; k++)
                                {
                                    *locRE++ = k0 + tab_ptrPdschCurr[k];
                                }
                            }
                        }
                           k0_start += nRbSc;
                        nPRB_tilde += 1; //increment resource block number
                        if(pTxInPars->DistributedFlag[j] == 1)
                        {
                           nPRB = tabPtrVRB[nPRB_tilde + blksize_VRB * pTxInPars->NgapFlag[j]]; // to access entries of Ngap = 2;
                        }
                        else
                        nPRB += 1; //increment resource block number
                    } //for n loop
                } // for row loop
                pTxSymBufs->sizeLocRE += 2*currNumRE;
            }
#endif
            pTxInPars->M0symb[currIndex] = currNumRE;
            pTxInPars->M1symb[currIndex] = 0;
            if (pTxInPars->numCodeWords[j] == 2)
            {
                pTxInPars->M1symb[currIndex] = currNumRE;
            }

            if (pTxInPars->transmissionMode[j] == TXDIVERSITY)
            {
                nSymb = (U32) (pTxInPars->M0symb[currIndex] & ((1 << (Ntx_antennas - 1)) - 1));
                if (Ntx_antennas == 2)
                {
                    if (nSymb != 0)
                    {
                        // Add extra zero IQ sample to make it even
                        pTxInPars->M0symb[currIndex] = pTxInPars->M0symb[currIndex] + 1;
                    }
                }
                else // 4 antennas
                {
                    if (nSymb != 0)
                    {
                        pTxInPars->M0symb[currIndex] = pTxInPars->M0symb[currIndex] + nSymb;
                    }
                }
            }

            /////////////////////////////////////////////////////////////////////////////////////////////////////////

            pTxInPars->nrow_x0[currIndex] = pTxInPars->numTxLayers[j];
            pTxInPars->ncol_x0[currIndex] = (U16) pTxInPars->M0symb[currIndex];
            if (2 * pTxInPars->numCodeWords[j] == pTxInPars->numTxLayers[j])
            {
                pTxInPars->ncol_x0[currIndex] = pTxInPars->ncol_x0[currIndex] >> 1;
            }

            /////////////////////////////////////////////////////////////////////////////////////////////////////////

            pTxInPars->ncol_y0[currIndex] = 0;
            //precode the complex constellation points, in layers, to antenna port resource element values
            if (pTxInPars->transmissionMode[j] != TXDIVERSITY) // Layer Mapping for non-STC
            {
                nSymb = pTxInPars->M0symb[currIndex];
                if (pTxInPars->M1symb[currIndex] > (U32)nSymb)
                {
                    nSymb = (S16) pTxInPars->M1symb[currIndex];
                }
                if (nSymb != 0)
                {
                    pTxInPars->ncol_y0[currIndex] = pTxInPars->ncol_x0[currIndex];
                }
            } // if STC
            else
            {
                if (pTxInPars->M0symb[currIndex] != 0)
                {
                    pTxInPars->ncol_y0[currIndex] = (U16) (pTxInPars->M0symb[currIndex]);
                }
            } // STC

            if (isym == 0)
            {
                currBit0 = 0;
                currBit1 = 0; // 2nd Cw
            }
            else
            {
                currBit0 = prevBit0[j] + prevNumRE[j];
                currBit1 = prevBit1[j] + prevNumRE[j];
            }
#ifdef CEVA_INTRINSICS
            pTxSymBufCurrCeva->bitFirst0 = 0;
            pTxSymBufCurrCeva->bitFirst1 = 0;
            pTxSymBufCurrCeva->size0 = 0;
            pTxSymBufCurrCeva->size1 = 0;
#else
            pTxSymBufCurr->bitFirst0 = 0;
            pTxSymBufCurr->bitFirst1 = 0;
            pTxSymBufCurr->size0 = 0;
            pTxSymBufCurr->size1 = 0;
#endif
            if (currNumRE > 0)
            {
                pTxInPars->length_b0[currIndex] = currNumRE * pTxInPars->Qm0[j];
                if (pTxInPars->modType0[j] == 3)
                {
                    nBits = 6;
                    pTxInPars->length_pb0[currIndex] = currNumRE;
                    bitFirst = currBit0;
                }
                else
                {
                    nBits = 8;
                    pTxInPars->length_pb0[currIndex] = (pTxInPars->length_b0[currIndex] + 7) >> 3;
                    bitFirst = (currBit0 * pTxInPars->Qm0[j]) >> 3;
                }

                if (Nlast0[j] != 0)
                    pTxInPars->length_pb0[currIndex] += 1;

#ifdef CEVA_INTRINSICS
                pTxSymBufCurrCeva->bitFirst0 = (U16)bitFirst;
                pTxSymBufCurrCeva->size0 = (U16)pTxInPars->length_pb0[currIndex];
#else
                pTxSymBufCurr->bitFirst0 = (U16)bitFirst;
                pTxSymBufCurr->size0 = (U16)pTxInPars->length_pb0[currIndex];
#endif
                pTxInPars->pbIndex0[currIndex] = bIndex0;
                bIndex0 += (S32) pTxInPars->length_pb0[currIndex];
                pTxSymBufs->sizeBuf0 += (U16)pTxInPars->length_pb0[currIndex];

                pTxInPars->Nsubc0[currIndex] = currNumRE;
                pTxInPars->Borrowed0[currIndex] = Nlast0[j];

                if (pTxInPars->modType0[j] != 3)
                {
#if 0
                    U32 shift = pTxInPars->Qm0[j] >> 1;

                    Nlast0[j] = (S16) ((pTxInPars->length_pb0[currIndex] * nBits
                    - pTxInPars->length_b0[currIndex]) >> shift) - Nlast0[j];

                    if (Nlast0[j] * pTxInPars->Qm0[j] >= 8)
                    {
                        Nlast0[j] -= (8 >> shift);
                    }
#else
                    U32 shift = pTxInPars->Qm0[j] >> 1;
                    S16 temp1, temp2;
                    temp1 = (S16) (pTxInPars->length_b0[currIndex] >> 3); // Num of bytes integer part
                    temp2 = (S16) (pTxInPars->length_b0[currIndex]) - (Nlast0[j]<< shift) - (temp1 << 3); //Number of bits from first byte of next symbol
                    if( temp2 < 0) // Adjust if negative
                    {
                       temp1 -= 1;
                       temp2 = (S16) (pTxInPars->length_b0[currIndex]) - (Nlast0[j]<< shift) - (temp1 << 3);
                    }
                    Nlast0[j] = 0;
                    if(temp2 != 0)
                    {
                        Nlast0[j] = (8 -  temp2) >> shift; //Number of bits to be borrowed from last byte of previous symbol in unit of RE scale for next time
                    }

#endif
                }
            }

            if (pTxInPars->numCodeWords[j] == 2 && currNumRE > 0)
            {
                pTxInPars->length_b1[currIndex] = currNumRE * pTxInPars->Qm1[j];
                if (pTxInPars->length_b1[currIndex] != 0)
                {
                    if (pTxInPars->modType1[j] == 3)
                    {
                        nBits = 6;
                        pTxInPars->length_pb1[currIndex] = currNumRE;
                        bitFirst = currBit1;
                    }
                    else
                    {
                        nBits = 8;
                        pTxInPars->length_pb1[currIndex] = (pTxInPars->length_b1[currIndex] + 7) >> 3;
                        bitFirst = (currBit1  * pTxInPars->Qm1[j]) >> 3;
                    }

                    if (Nlast1[j] != 0)
                        pTxInPars->length_pb1[currIndex] += 1;

#ifdef CEVA_INTRINSICS
                    pTxSymBufCurrCeva->bitFirst1 = (U16)bitFirst;
                    pTxSymBufCurrCeva->size1 = (U16)pTxInPars->length_pb1[currIndex];
#else
                    pTxSymBufCurr->bitFirst1 = (U16)bitFirst;
                    pTxSymBufCurr->size1 = (U16)pTxInPars->length_pb1[currIndex];
#endif
                    pTxInPars->pbIndex1[currIndex] = bIndex1;
                    bIndex1 += pTxInPars->length_pb1[currIndex];
                    pTxSymBufs->sizeBuf1 += (U16)pTxInPars->length_pb1[currIndex];

                    pTxInPars->Nsubc1[currIndex] = currNumRE;
                    pTxInPars->Borrowed1[currIndex] = Nlast1[j];

                    if (pTxInPars->modType1[j] != 3)
                    {
#if 0
                        U32 shift = pTxInPars->Qm1[j] >> 1;

                        Nlast1[j] = (S16) ((pTxInPars->length_pb1[currIndex] * nBits
                            - pTxInPars->length_b1[currIndex]) >> shift)  - Nlast1[j];

                        if (Nlast1[j] * pTxInPars->Qm1[j] >= 8)
                        {
                            Nlast1[j] -= (8 >> shift);
                        }
#else
                        U32 shift = pTxInPars->Qm1[j] >> 1;
                        S16 temp1, temp2;
                        temp1 = (S16) (pTxInPars->length_b1[currIndex] >> 3); // Num of bytes integer part
                        temp2 = (S16) (pTxInPars->length_b1[currIndex]) - (Nlast1[j]<< shift) - (temp1 << 3); //Number of bits from first byte of next symbol
                        if( temp2 < 0) // Adjust if negative
                        {
                           temp1 -= 1;
                           temp2 = (S16) (pTxInPars->length_b1[currIndex]) - (Nlast1[j]<< shift) - (temp1 << 3);
                        }
                        Nlast1[j] = 0;
                        if(temp2 != 0)
                        {
                            Nlast1[j] = (8 -  temp2) >> shift; //Number of bits to be borrowed from last byte of previous symbol in unit of RE scale for next time
                        }

#endif
                    }
                }
            }
            prevNumRE[j] = currNumRE;
            prevBit0[j] = currBit0;
            prevBit1[j] = currBit1;
            TotalEntries += pTxInPars->numberofEntries[j];
#ifdef CEVA_INTRINSICS
            pTxSymBufCurrCeva += 1;
#else
            pTxSymBufCurr += 1;
#endif
        }
#ifdef CEVA_INTRINSICS
        pTxSymBufsCurr = (PTXSYMBUFS)((U8*)pTxSymBufsOut + isym*RUP32B(sizeof(TXSYMBUFS)));
        MemCpyToExt((void*) pTxSymBufsCurr, (void*) pTxSymBufsOutCeva, RUP32B(sizeof(TXSYMBUFS)));
        MemCpyToExt((void*) pTxSymBufCurr, (void*) pTxSymBufCeva,
            RUP32B((numChan - ichanStart)*sizeof(TXSYMBUF)));
#endif
    }
    return;
}
