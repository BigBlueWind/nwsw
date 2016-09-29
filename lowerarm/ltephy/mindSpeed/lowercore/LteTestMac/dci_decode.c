/** @file dci_decode.c
 *
 * @brief Decode/Parse DCIs
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
* Project Includes
****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "resultcodes.h"
#include "dci.h"
#include "bs_static_config.h"
#include "MacUtils.h"


/****************************************************************************
Private Definitions
****************************************************************************/

U8 dci_decode(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U8 payload_length)
{

    if (payload_length == pBsStaticConfig->DCI_size.format0)
    {
// need to check bit7 in the *p for make decision for decode format0 or format1a
        if ((*p)&0x80)
        {
            dci_decode_1a(p, pDciInfo, pBsStaticConfig);
        }
        else
        {
            dci_decode_0 (p, pDciInfo, pBsStaticConfig);
        }
    }

    else if (payload_length == pBsStaticConfig->DCI_size.format1)
    {
        dci_decode_1(p, pDciInfo, pBsStaticConfig);
    }

    else if (payload_length == pBsStaticConfig->DCI_size.format1b)
    {
        dci_decode_1b(p, pDciInfo, pBsStaticConfig);
    }

    else if (payload_length == pBsStaticConfig->DCI_size.format1c)
    {
        dci_decode_1c(p, pDciInfo, pBsStaticConfig);
    }

    else if (payload_length == pBsStaticConfig->DCI_size.format1d)
    {
        dci_decode_1d(p, pDciInfo, pBsStaticConfig);
    }

    else if (payload_length == pBsStaticConfig->DCI_size.format2)
    {
        dci_decode_2(p, pDciInfo, pBsStaticConfig);
    }

    else if (payload_length == pBsStaticConfig->DCI_size.format2a)
    {
        dci_decode_2a(p, pDciInfo, pBsStaticConfig);
    }

    else if (payload_length == pBsStaticConfig->DCI_size.format2b)
    {
        dci_decode_2b(p, pDciInfo, pBsStaticConfig);
    }

    else
    {
        return MAC_FAILURE;
    }

    return MAC_SUCCESS;
}

void dci_decode_0 (U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8      pos=63;
    U16     riv;

    SWAP64(p);

    // FlagFormat0 always ==0
    pos--;

    pDciInfo->hopping = extract_bits(p, &pos, 1);

    if (pDciInfo->hopping == 0)
    {
        /* - For non-hopping PUSCH:
         * [log2(N_UL_RB * (N_UL_RB + 1)/2)] bits provide the resource allocation
         */
        riv = extract_bits(p, &pos, pBsStaticConfig->size_rba_pusch);
        decode_riv(pBsStaticConfig->n_ul_rb, riv, &pDciInfo->start_rb_index, &pDciInfo->l_crb);
    }
    else
    {
        /* - For PUSCH hopping
             N_Ul_hop-bits are used to obtain the value of nt_prb(i) as indicated in section 8.4 of 36.213-930 
             [log2(N_UL_RB * (N_UL_RB + 1)/2)]-N_Ul_hop bits provide the resource allocation in the UL subframe as defined in section 8.1 of 36.213-930
        */

    }

    // - Modulation and coding scheme and redundancy version – 5 bits as defined in section 8.6 of 36.213-930
    pDciInfo->mcs_rv = extract_bits(p, &pos, 5);

    // - New data indicator - 1 bit 
    pDciInfo->ndi = extract_bits(p, &pos, 1);

    //- TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    pDciInfo->tpc = extract_bits(p, &pos, 2);

    // - Cyclic shift for DM RS – 3 bits as defined in section 5.5.2.1.1 of 36.211
    pDciInfo->cyclic_shift_dm_rs = extract_bits(p, &pos, 3);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {

        if (pBsStaticConfig->tdd_configuration==0) // configurations 0
        {
            pDciInfo->ul_index = extract_bits(p, &pos, 2);
        }

        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2);
        }

    }

    // - CQI request - 1 bit as defined in section 7.2.1 of 36.213-930 
    pDciInfo->cqi_request = extract_bits(p, &pos, 1);

    SWAP64(p);

    return;
}

void dci_decode_1(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8    pos=63;

    SWAP64(p);


    if (pBsStaticConfig->n_dl_rb>10)
    {
        pDciInfo->rah = extract_bits(p, &pos, 1);        
    }

    pDciInfo->rbg = extract_bits(p, &pos, pBsStaticConfig->size_rbg_pusch);

    pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 4); 
    }
    else  // FDD
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 3);
    }
    
    // - New data indicator - 1 bit 
    pDciInfo->tb_info[0].ndi = extract_bits(p, &pos, 1);

    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[0].redundancy_ver = extract_bits(p, &pos, 2);

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    pDciInfo->tpc = extract_bits(p, &pos, 2);


    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2);
            pos += 2;
        }
        pos-=2;
    }

    SWAP64(p);

    return;
}

void dci_decode_1a(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8    pos=63;
    U16    riv;

    SWAP64(p);


    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // FlagFormat0 always ==1
        pos--;

        // 0 - indicates Localized 
        // w32 |= (0)<<(32-pos);
        pDciInfo->virtul_resrc_blk_type = 0;
        pos--;


        /*  - For localized VRB:
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the resource allocation
         */
        riv = extract_bits(p, &pos, pBsStaticConfig->size_rba_pdsch);
        decode_riv(pBsStaticConfig->n_dl_rb, riv, &pDciInfo->start_rb_index, &pDciInfo->l_crb);
    }
    else
    {
        /*    - For distributed VRB:
         * - If N_DL_RB < 50 or if the format 1A CRC is scrambled by
         * RA-RNTI, P-RNTI, or SI-RNTI
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the
         * resource allocation
         *
         * - else if N_DL_RB >= 50
         * - 1 bit, the MSB indicates the gap value, where value 0
         * indicates Ngap = Ngap_1 and value 1 indicates
         * Ngap = Ngap_2 - ([log2(N_DL_RB * (N_DL_RB + 1)/2)] - 1)
         * bits provide the resource allocation
         */
    }

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 4);
    }
    else  // FDD
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 3);
    }
    
    // - New data indicator - 1 bit 
    pDciInfo->tb_info[0].ndi = extract_bits(p, &pos, 1);

    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[0].redundancy_ver = extract_bits(p, &pos, 2);

    //- TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    pDciInfo->tpc = extract_bits(p, &pos, 2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2);
            pos += 2;
        }
    }
    pos -= 2; // sizeof dai always == 2


    SWAP64(p);

    return;
}

void dci_decode_1b(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8    pos=63;
    U16    riv;

    SWAP64(p);


    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized 
        pos--;

        /*  - For localized VRB:
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the resource allocation
         */
        riv = extract_bits(p, &pos, pBsStaticConfig->size_rba_pdsch);
        decode_riv(pBsStaticConfig->n_dl_rb, riv, &pDciInfo->start_rb_index, &pDciInfo->l_crb);
    }
    else
    {
        /*    - For distributed VRB:
         * - If N_DL_RB < 50 or if the format 1A CRC is scrambled by
         * RA-RNTI, P-RNTI, or SI-RNTI
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the
         * resource allocation
         *
         * - else if N_DL_RB >= 50
         * - 1 bit, the MSB indicates the gap value, where value 0
         * indicates Ngap = Ngap_1 and value 1 indicates
         * Ngap = Ngap_2 - ([log2(N_DL_RB * (N_DL_RB + 1)/2)] - 1)
         * bits provide the resource allocation
         */
    }

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 4);
    }
    else  // FDD
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 3);
    }

    
    // - New data indicator - 1 bit 
    pDciInfo->tb_info[0].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[0].redundancy_ver = extract_bits(p, &pos, 2);

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    pDciInfo->tpc = extract_bits(p, &pos, 2);                                    

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
        // with uplink-downlink configurations 1-6)
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2); 
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    // - tpmi information for precoding - number of bits as specified in Table 5.3.3.1.3A-1
    if (pBsStaticConfig->n_antenna_port==2)
    {
        pDciInfo->tpmi = extract_bits(p, &pos, 2);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        pDciInfo->tpmi = extract_bits(p, &pos, 4);
    }
        
    // - PMI confirmation for precoding - 1 bit as specified in Table 5.3.3.1.3A-2
    pDciInfo->tpmi_confirm = extract_bits(p, &pos, 1);

    SWAP64(p);


    return;
}

void dci_decode_1c(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8      pos=63;
    U8      startRBtIndex, Ltcrb;
    U16     riv;

    SWAP64(p);


    if (pDciInfo->scheduling_pdsch_codeword)
    {
        // 1 bit indicates the gap value
        pDciInfo->index_ngap = extract_bits(p, &pos, 1);

        /*  - For localized VRB:
         * [log2(N_DL_VRBgap1/n_step_rb * (N_DL_VRBgap1/n_step_rb + 1)/2)] bits provide the resource allocation
         */
        riv = extract_bits(p, &pos, pBsStaticConfig->size_rba_vrb);        
        decode_riv(pBsStaticConfig->n_dl_rb/pBsStaticConfig->n_step_rb, riv, &startRBtIndex, &Ltcrb);

        pDciInfo->l_crb = Ltcrb*pBsStaticConfig->n_step_rb;
        pDciInfo->start_rb_index = startRBtIndex*pBsStaticConfig->n_step_rb;

        // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
        pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);
    }
    else
    {
        pos = 8;
        /*   - Information for MCCH change notification - 8 bits as defined in section 5.8.1.3 of 36.331
             - Reserved information bits are added until the size is equal to that of format 1C used 
               for very compact scheduling of one PDSCH codeword
         */
        pDciInfo->notif_mcch = p[0]>>(32-pos);
    }

    SWAP64(p);


    return;
}


void dci_decode_1d(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8    pos=63;
    U16   riv;

    SWAP64(p);


    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized 
        pos--;

        /*  - For localized VRB:
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the resource allocation
         */
        riv = extract_bits(p, &pos, pBsStaticConfig->size_rba_pdsch);
        decode_riv(pBsStaticConfig->n_dl_rb, riv, &pDciInfo->start_rb_index, &pDciInfo->l_crb);
    }
    else
    {
        /*    - For distributed VRB:
         * - If N_DL_RB < 50 or if the format 1A CRC is scrambled by
         * RA-RNTI, P-RNTI, or SI-RNTI
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the
         * resource allocation
         *
         * - else if N_DL_RB >= 50
         * - 1 bit, the MSB indicates the gap value, where value 0
         * indicates Ngap = Ngap_1 and value 1 indicates
         * Ngap = Ngap_2 - ([log2(N_DL_RB * (N_DL_RB + 1)/2)] - 1)
         * bits provide the resource allocation
         */
    }

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD)
    if (pBsStaticConfig->tdd)
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 4);
    }
    else  // FDD
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 3);
    }

    // - New data indicator - 1 bit 
    pDciInfo->tb_info[0].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[0].redundancy_ver = extract_bits(p, &pos, 2);

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213
    pDciInfo->tpc = extract_bits(p, &pos, 2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
        // with uplink-downlink configurations 1-6)
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2);
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    // - tpmi information for precoding - number of bits as specified in Table 5.3.3.1.3A-1
    if (pBsStaticConfig->n_antenna_port==2)
    {
        pDciInfo->tpmi = extract_bits(p, &pos, 2);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        pDciInfo->tpmi = extract_bits(p, &pos, 4);
    }
        
    // - Downlink power offset - 1 bit as defined in section 7.1.5 of 36.213-930
    pDciInfo->power_dl_offset = extract_bits(p, &pos, 1);

    SWAP64(p);

    return;
}

void dci_decode_2(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8    pos=63;

    SWAP64(p);


    /* If downlink bandwidth is less than or equal to 10 PRBs, there is no resource allocation 
     * header and resource allocation type 0 is assumed. 
     */
    if (pBsStaticConfig->n_dl_rb>10)
    {
        pDciInfo->rah = extract_bits(p, &pos, 1);
    }

    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized 
        pDciInfo->rbg = extract_bits(p, &pos, pBsStaticConfig->size_rbg_pusch);
    }
    else
    {
        /* 1 - For distributed VRB:
        */
    }

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    pDciInfo->tpc = extract_bits(p, &pos, 2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2);
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 4);
    }
    else  // FDD
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 3);
    }

    // - Transport block to codeword swap flag - 1 bit 
    pDciInfo->swap_flag = extract_bits(p, &pos, 1);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);

    // - New data indicator - 1 bit
    pDciInfo->tb_info[0].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[0].redundancy_ver = extract_bits(p, &pos, 2);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    pDciInfo->tb_info[1].modulation_type = extract_bits(p, &pos, 5);

    // - New data indicator - 1 bit
    pDciInfo->tb_info[1].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[1].redundancy_ver = extract_bits(p, &pos, 2);



    // - Precoding information - number of bits as specified in Table 5.3.3.1.5-3
    if (pBsStaticConfig->n_antenna_port==2)
    {
        pDciInfo->precoding_info = extract_bits(p, &pos, 3);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        pDciInfo->precoding_info = extract_bits(p, &pos, 6);
    }

    SWAP64(p);


    return;
}

void dci_decode_2a(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8    pos=63;

    SWAP64(p);


    /* If downlink bandwidth is less than or equal to 10 PRBs, there is no resource allocation 
     * header and resource allocation type 0 is assumed. 
     */
    if (pBsStaticConfig->n_dl_rb>10)
    {
        pDciInfo->rah = extract_bits(p, &pos, 1);
    }


    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized 
        pDciInfo->rbg = extract_bits(p, &pos, pBsStaticConfig->size_rbg_pusch);
    }
    else
    {
        /* 1 - For distributed VRB:
        */
    }

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213
    pDciInfo->tpc = extract_bits(p, &pos, 2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2);
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD)
    if (pBsStaticConfig->tdd)
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 4);
    }
    else  // FDD
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 3);
    }

    // - Transport block to codeword swap flag - 1 bit
    pDciInfo->swap_flag = extract_bits(p, &pos, 1);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213
    pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);

    // - New data indicator - 1 bit
    pDciInfo->tb_info[0].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits
    pDciInfo->tb_info[0].redundancy_ver = extract_bits(p, &pos, 2);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213
    pDciInfo->tb_info[1].modulation_type = extract_bits(p, &pos, 5);

    // - New data indicator - 1 bit
    pDciInfo->tb_info[1].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits
    pDciInfo->tb_info[1].redundancy_ver = extract_bits(p, &pos, 2);

    // - Precoding information - number of bits as specified in Table 5.3.3.1.5-3
    if (pBsStaticConfig->n_antenna_port==2)
    {
        pDciInfo->precoding_info = extract_bits(p, &pos, 2);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        pDciInfo->precoding_info = extract_bits(p, &pos, 4);
    }

    SWAP64(p);


    return;
}

void dci_decode_2b(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8    pos=63;

    p[0] = SWAP((U32)p[0]);
    p[1] = SWAP((U32)p[1]);

    /* If downlink bandwidth is less than or equal to 10 PRBs, there is no resource allocation 
     * header and resource allocation type 0 is assumed. 
     */
    if (pBsStaticConfig->n_dl_rb>10)
    {
        pDciInfo->rah = extract_bits(p, &pos, 1);
    }


    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized
        pDciInfo->rbg = extract_bits(p, &pos, pBsStaticConfig->size_rbg_pusch);
    }
    else
    {
        /* 1 - For distributed VRB:
        */
    }

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213
    pDciInfo->tpc = extract_bits(p, &pos, 2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pDciInfo->dai = extract_bits(p, &pos, 2);
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 4);
    }
    else  // FDD
    {
        pDciInfo->harq_process_id = extract_bits(p, &pos, 3);
    }

    // - Transport block to codeword swap flag - 1 bit
    pDciInfo->scrambl_indentity = extract_bits(p, &pos, 1);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    pDciInfo->tb_info[0].modulation_type = extract_bits(p, &pos, 5);

    // - New data indicator - 1 bit
    pDciInfo->tb_info[0].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[0].redundancy_ver = extract_bits(p, &pos, 2);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    pDciInfo->tb_info[1].modulation_type = extract_bits(p, &pos, 5);

    // - New data indicator - 1 bit
    pDciInfo->tb_info[1].ndi = extract_bits(p, &pos, 1);
    
    // - Redundancy version - 2 bits 
    pDciInfo->tb_info[1].redundancy_ver = extract_bits(p, &pos, 2);

    SWAP64(p);


    return;
}




