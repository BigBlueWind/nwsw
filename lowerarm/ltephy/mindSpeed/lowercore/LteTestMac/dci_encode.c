/** @file dci_encode.c
 *
 * @brief Encode DCIs for different formats
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "resultcodes.h"
#include "dci.h"
#include "bs_static_config.h"
#include "MacUtils.h"

U8 ambiguous_sizes[] = {12, 14, 16 ,20, 24, 26, 32, 40, 44, 56};

U16 calc_riv(DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U8 type)
{
    U16 riv;
    U8 nt_dl_vrb=pBsStaticConfig->n_dl_vrb/pBsStaticConfig->n_step_rb;
    U8 Lt_crb = pDciInfo->l_crb/pBsStaticConfig->n_step_rb;
    U8 rbt_start_index = pDciInfo->start_rb_index/pBsStaticConfig->n_step_rb;

    switch (type)
    {
        case RIV_FOR_PUSCH: // format0
            if ((pDciInfo->l_crb - 1) <= (pBsStaticConfig->n_dl_rb >> 1))
                riv = pBsStaticConfig->n_dl_rb * (pDciInfo->l_crb - 1) + pDciInfo->start_rb_index;
            else
                riv = pBsStaticConfig->n_dl_rb * (pBsStaticConfig->n_dl_rb - pDciInfo->l_crb + 1) + (pBsStaticConfig->n_dl_rb - 1 - pDciInfo->start_rb_index);            
            break;
        
        case RIV_FOR_PDSCH: // format1A, format 1B, format1D, format2
            if ((pDciInfo->l_crb - 1) <= (pBsStaticConfig->n_ul_rb >> 1))
                riv = pBsStaticConfig->n_ul_rb * (pDciInfo->l_crb - 1) + pDciInfo->start_rb_index;
            else
                riv = pBsStaticConfig->n_ul_rb * (pBsStaticConfig->n_ul_rb - pDciInfo->l_crb + 1) + (pBsStaticConfig->n_ul_rb - 1 - pDciInfo->start_rb_index);                        
            break;
        
        case RIV_FOR_VRB: // format 1C
            if (Lt_crb==0)
            {
                Lt_crb++;
            }

            if ((Lt_crb - 1) <= (nt_dl_vrb >> 1))
                riv = nt_dl_vrb * (Lt_crb - 1) + rbt_start_index;
            else
                riv = nt_dl_vrb * (nt_dl_vrb - Lt_crb + 1) + (nt_dl_vrb - 1 - rbt_start_index);                                    
            break;
        
        default:
            riv=0;
            break;
    }    
    return riv;

}

void calc_dci_size(DCI_SIZE_t *pDCI_size, BS_STATIC_CONFIG *pBsStaticConfig)
{
    U8         i;
    U8         padding;
    U32        value;

    U8         n_ul_rb=pBsStaticConfig->n_ul_rb, n_dl_rb=pBsStaticConfig->n_dl_rb;

    U8         gap_1 = get_gap_1(n_dl_rb);
    U8         n_step_rb = get_n_step_rb(n_dl_rb);
    U8         rba_field_bit_size = MaxRBs2RBAFieldSize_F0(gap_1/n_step_rb);

    // RBA with virtual assignment
    U8         n_dl_vrb_gap1 = get_gap_1(pBsStaticConfig->n_dl_rb);

    U8 P = get_P(n_dl_rb);
    U8 n_antena_port= pBsStaticConfig->n_antenna_port;

//------------------------------------------------------------------------------    
    pDCI_size->format0  = 1 + 1 + Log2(n_ul_rb*(n_ul_rb+1)/2) + 5 + 1 + 2 + 3 + 1;
//                        |   |   |                             |   |   |   |   |--- CQI request 
//                        |   |   |                             |   |   |   |------- Cyclic shift for DM RS 
//                        |   |   |                             |   |   |----------- TPC command for scheduled PUSCH 
//                        |   |   |                             |   |--------------- New data indicator 
//                        |   |   |                             |------------------- Modulation and coding scheme and redundancy version 
//                        |   |   |------------------------------------------------- Resource block assignment and hopping resource allocation 
//                        |   |----------------------------------------------------- Frequency hopping flag 
//                        |--------------------------------------------------------- Flag for format0/format1A differentiation 

    if (pBsStaticConfig->tdd){
        pDCI_size->format0  += 2; // UL-index
            pDCI_size->format0  += 2; // dai
    }

//------------------------------------------------------------------------------------------------------------------------------------------------    
        pDCI_size->format1a = 1 + Log2(n_dl_rb*(n_dl_rb+1)/2) + 5 + 3 + 1 + 2 + 2; 
//                            |               |                 |   |   |   |   | 
//                            |               |                 |   |   |   |   |---- TPC command for PUCCH 
//                            |               |                 |   |   |   |-------- Redundancy version 
//                            |               |                 |   |   |------------ New data indicator 
//                            |               |                 |   |---------------- HARQ process number - 3 bits (FDD), 
//                            |               |                 |-------------------- Modulation and coding scheme
//                            |               |-------------------------------------- Resource block assignment
//                            |------------------------------------------------------ Localized/Distributed VRB assignment flag 

    if (pBsStaticConfig->tdd){
        pDCI_size->format1a  += 1; // HARQdiff (1bit)
    }


    /* When the format 1A CRC is scrambled with a RA-RNTI, P-RNTI, or SI-RNTI then the following fields among the fields above are reserved:
     *  - HARQ process number
     *  - Downlink Assignment Index (used for TDD only and is not present in FDD)
     */
    pDCI_size->format1a  += 2; // dai (2bit)

        
// !!! compare with format0 size !!!
    if (pDCI_size->format1a < pDCI_size->format0){
        pDCI_size->format1a = pDCI_size->format0;
    }

    if (pDCI_size->format0 < pDCI_size->format1a){
        pDCI_size->format0 = pDCI_size->format1a;
    }

    pDCI_size->format0  = format_padding (pDCI_size->format0, ambiguous_sizes, sizeof(ambiguous_sizes));
    pDCI_size->format1a = pDCI_size->format0;
    
//------------------------------------------------------------------------------------------------------------------------------------------------
    pDCI_size->format1 = (n_dl_rb+P-1)/P + 5 + 3 + 1 + 2 + 2 ; 
//                         |               |   |   |   |   |--- TPC command for PUCCH 
//                         |               |   |   |   |------- Redundancy version 
//                         |               |   |   |----------- New data indicator 
//                         |               |   |--------------- HARQ process number - 3 bits (FDD), 
//                         |               |------------------- Modulation and coding scheme
//                         |------------------------------------- Resource block assignment

    if (n_dl_rb>10){
        pDCI_size->format1 +=1; // Resource allocation header 
    }

    if (pBsStaticConfig->tdd){
        pDCI_size->format1  += 3; // Downlink Assignment Index (2bits) + HARQdiff (1bit)
    }

    if (pDCI_size->format1 == pDCI_size->format0){
        pDCI_size->format1 +=1;
    }

    do{
        padding= 0;

        for (i=0; i<sizeof(ambiguous_sizes); i++){
            if (pDCI_size->format1 == ambiguous_sizes[i]){
                padding =1;
                break;
            }    
        }

        if (padding){
            pDCI_size->format1+=1;
        }

        if (pDCI_size->format1 == pDCI_size->format0){
            pDCI_size->format1 +=1;

        }
        
    } while (padding);

//------------------------------------------------------------------------------------------------------------------------------------------------
        pDCI_size->format1b = 1 + Log2(n_dl_rb*(n_dl_rb+1)/2) + 5 + 3 + 1 + 2 + 2 + 1; 
//                            |               |                 |   |   |   |   |   |--- PMI confirmation for precoding 
//                            |               |                 |   |   |   |   |------- TPC command for PUCCH 
//                            |               |                 |   |   |   |----------- Redundancy version 
//                            |               |                 |   |   |--------------- New data indicator 
//                            |               |                 |   |------------------- HARQ process number - 3 bits (FDD), 
//                            |               |                 |----------------------- Modulation and coding scheme
//                            |               |----------------------------------------- Resource block assignment
//                            |--------------------------------------------------------- Localized/Distributed VRB assignment flag 


    if (pBsStaticConfig->tdd){
        pDCI_size->format1b  += 3; // Downlink Assignment Index (2bits) + HARQdiff (1bit)
    }

    if (n_antena_port == 1){
        pDCI_size->format1b  = 0;
    }
    else if (n_antena_port == 2){
        pDCI_size->format1b += 2;
    }
    else { // == 4
        pDCI_size->format1b += 4;
    }

//------------------------------------------------------------------------------------------------------------------------------------------------
    value = n_dl_vrb_gap1/n_step_rb;

    pDCI_size->format1c = 1+Log2((value)*(value+1)/2) +5;
//                       |  |                         |--- Modulation and coding scheme 
//                       |  |----------------------------- Resource block assignment 
//                       |-------------------------------- 1 bit indicates the gap value

//------------------------------------------------------------------------------------------------------------------------------------------------
        pDCI_size->format1d = 1 + Log2(n_dl_rb*(n_dl_rb+1)/2) + 5 + 3 + 1 + 2 + 2 + 1; 
//                             |                 |              |   |   |   |   |   |--- Downlink power offset 
//                             |                 |              |   |   |   |   |------- TPC command for PUCCH 
//                             |                 |              |   |   |   |----------- Redundancy version 
//                             |                 |              |   |   |--------------- New data indicator 
//                             |                 |              |   |------------------- HARQ process number - 3 bits (FDD), 
//                             |                 |              |----------------------- Modulation and coding scheme
//                             |                 |----------------------------------------- Resource block assignment
//                             |--------------------------------------------------------- Localized/Distributed VRB assignment flag 


    if (pBsStaticConfig->tdd){
        pDCI_size->format1d    += 3; // Downlink Assignment Index (2bits) + HARQdiff (1bit)
    }

    if (n_antena_port == 1){
        pDCI_size->format1d  = 0;
    }
    else if (n_antena_port == 2){
        pDCI_size->format1d += 2;
    }
    else { // == 4
        pDCI_size->format1d += 4;
    }
    
    pDCI_size->format1d = format_padding (pDCI_size->format1d, ambiguous_sizes, sizeof(ambiguous_sizes));

//------------------------------------------------------------------------------------------------------------------------------------------------
    pDCI_size->format2b = (n_dl_rb+P-1)/P + 2 + 3 + 1 + (5 + 1 + 2)*2;
//                         |                |   |   |    |   |   |-------- Redundancy version 
//                         |                |   |   |    |   |------------ New data indicator 
//                         |                |   |   |    |---------------- Modulation and coding scheme 
//                         |                |   |   |--------------------- Transport block to codeword swap flag 
//                         |                |   |------------------------- HARQ process number 
//                         |                |----------------------------- TPC command for PUCCH 
//                         |---------------------------------------------- Resource block assignment

    if (n_dl_rb>10){
        pDCI_size->format2b += 1; // Resource allocation header 

    }


    if (pBsStaticConfig->tdd&1){
        pDCI_size->format2b    += 3; // Downlink Assignment Index (2bits) + HARQdiff (1bit)
    }


//------------------------------------------------------------------------------------------------------------------------------------------------
    if (n_antena_port == 1){
        pDCI_size->format2a = 0;
    }
    else if (n_antena_port == 2){
        pDCI_size->format2a = pDCI_size->format2b+2;
    }
    else { // == 4
        pDCI_size->format2a = pDCI_size->format2b+4;
    }
    pDCI_size->format2a = format_padding (pDCI_size->format2a, ambiguous_sizes, sizeof(ambiguous_sizes));



//------------------------------------------------------------------------------------------------------------------------------------------------

    if (n_antena_port == 1){
        pDCI_size->format2 = 0;
    }
    else if (n_antena_port == 2){
        pDCI_size->format2 = pDCI_size->format2b+3;

    }
    else { // == 4
        pDCI_size->format2 = pDCI_size->format2b+6;
    }


    pDCI_size->format2  = format_padding (pDCI_size->format2, ambiguous_sizes, sizeof(ambiguous_sizes));
    pDCI_size->format2b = format_padding (pDCI_size->format2b, ambiguous_sizes, sizeof(ambiguous_sizes));


//------------------------------------------------------------------------------------------------------------------------------------------------
    pDCI_size->format3  = 0;

//------------------------------------------------------------------------------------------------------------------------------------------------
    pDCI_size->format3a = 0;
    
    return;
}

U8 dci_encode_0(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{
    U32 w32=0;
    U8  pos=1;         // size of FlagFormat0 always ==1
    U16 riv;

    pos += 1;// size of HoppingFormat0 always ==1
    w32 |= (pDciInfo->hopping&1)<<(32-pos);

    pos += pBsStaticConfig->size_rba_pusch;
    if (pDciInfo->hopping)
    {
        ; // For PUSCH hopping:
    }
    else
    {
        riv = calc_riv(pDciInfo, pBsStaticConfig, RIV_FOR_PUSCH);
        w32 |= (riv & ((1<<pBsStaticConfig->size_rba_pusch)-1))<<(32-pos);
    }

    // - Modulation and coding scheme and redundancy version - 5 bits
    pos += 5;// size of Mcs always ==5
    w32 |= (pDciInfo->mcs_rv&0x1f)<<(32-pos);

    // - New data indicator - 1 bit
    pos += 1; // sizeof ndi always ==1
    w32 |= (pDciInfo->ndi)<<(32-pos);

    // - TPC command for scheduled PUSCH - 2 bits as defined in section 5.1.1.1 of 36.213-930
    pos += 2; // sizeof TPC always ==2
    w32 |= (pDciInfo->tpc)<<(32-pos);

    // - Cyclic shift for DM RS - 3 bits as defined in section 5.5.2.1.1 of 36.211
    pos += 3; // sizeof cycle shift always ==3
    w32 |= (pDciInfo->cyclic_shift_dm_rs)<<(32-pos);

    if(pBsStaticConfig->tdd) // TDD
    {
        // - UL index - 2 bits as defined in sections 5.1.1.1, 7.2.1, 8 and 8.4 of 36.213-930 (this field is present only for TDD operation 
        // with uplink-downlink configuration 0)
        if (pBsStaticConfig->tdd_configuration==0) // configuration 0
        {
            pos += 2; // sizeof UL index always ==2
            w32 |= (pDciInfo->ul_index)<<(32-pos); // !!!
        }
    
        // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
        // with uplink-downlink configurations 1-6)
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            pos += 2; // sizeof dai always == 2
            w32 |= (pDciInfo->dai)<<(32-pos);  // !!!
        }

    }

    // - CQI request - 1 bit as defined in section 7.2.1 of [3] 
    pos += 1; // sizeof CQI request always ==1
    w32 |= (pDciInfo->cqi_request)<<(32-pos);


    *pSdu = SWAP((U32)w32);     // change byte-order    
    *index= pBsStaticConfig->DCI_size.format0;

    return MAC_SUCCESS;
}


U8 dci_encode_1(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{
    U32 w32[2]={0x00000000, 0x00000000};     //
    U8    pos1 = 63;

    /* If downlink bandwidth is less than or equal to 10 PRBs, there is no resource allocation 
     * header and resource allocation type 0 is assumed. 
     */
    if (pBsStaticConfig->n_dl_rb>10)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->rah,
                    &pos1,        // start_offset
                    1);        // size
    }

    put_bits((U32 *)w32, pDciInfo->rbg, &pos1, pBsStaticConfig->size_rbg_pusch); 

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].modulation_type,
                &pos1,
                5);

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos1,
                    4); 
    }
    else  // FDD
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos1,                 
                    3); 
    }
    
    // - New data indicator - 1 bit 
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].ndi,
                &pos1, 
                1);

    // - Redundancy version - 2 bits
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].redundancy_ver,
                &pos1,                             
                2);         

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213
    put_bits((U32 *)w32, 
                pDciInfo->tpc,
                &pos1, 
                2);             

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            put_bits((U32 *)w32, 
                        pDciInfo->dai,
                        &pos1,
                        2);
            pos1 += 2;           
        }
        pos1 -= 2;
    }

    SWAP64_2params(w32, pSdu);


    *index= pBsStaticConfig->DCI_size.format1;


    return MAC_SUCCESS;
}

U8 dci_encode_1a(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{
    U8  pos=63;                                // size of FlagFormat0 always ==1
    U32 w32[2]={0x80000000, 0x00000000};     // FlagFormat0 always ==1
    U16 riv;

    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // FlagFormat0 always ==1
        pos--;

        // 0 - indicates Localized 
        // w32 |= (0)<<(32-pos);
        pos--;

        /*  - For localized VRB:
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the resource allocation
         */
        riv = calc_riv(pDciInfo, pBsStaticConfig, RIV_FOR_PDSCH);    

        put_bits((U32 *)w32, 
                    riv,
                    &pos,                 
                    pBsStaticConfig->size_rba_pdsch);            // size    
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
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].modulation_type,
                &pos,                 
                5);            // size       

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD)
    if (pBsStaticConfig->tdd)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos,             
                    4);    
    }
    else  // FDD
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos,             
                    3);    
    }
    
    // - New data indicator - 1 bit
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].redundancy_ver,
                &pos,             
                2);

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213
    put_bits((U32 *)w32, 
                pDciInfo->tpc,
                &pos,             
                2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            put_bits((U32 *)w32, 
                        pDciInfo->dai,
                        &pos,             
                        2);
            pos += 2;
        }
    }
    pos -= 2; // sizeof dai always == 2

    SWAP64_2params(w32, pSdu);


    *index= pBsStaticConfig->DCI_size.format1a;


    return MAC_SUCCESS;
}

U8 dci_encode_1b(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{

    U32 w32[2]={0x00000000, 0x00000000}; 
    U8  pos=63;    
    U16 riv;

    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized
        pos--;

        /*  - For localized VRB:
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the resource allocation
         */
        riv = calc_riv(pDciInfo, pBsStaticConfig, RIV_FOR_PDSCH);    
        put_bits((U32 *)w32, 
                    riv,
                    &pos,             
                    pBsStaticConfig->size_rba_pdsch);        // size
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
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].modulation_type,
                &pos,             
                5);

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD)
    if (pBsStaticConfig->tdd)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos,             
                    4);
    }
    else  // FDD
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos,             
                    3);
    }

    
    // - New data indicator - 1 bit
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].redundancy_ver,
                &pos,             
                2);

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    put_bits((U32 *)w32, 
                pDciInfo->tpc,
                &pos,             
                2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
        // with uplink-downlink configurations 1-6)
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            put_bits((U32 *)w32, 
                        pDciInfo->dai,
                        &pos,             
                        2);
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    // - tpmi information for precoding - number of bits as specified in Table 5.3.3.1.3A-1
    if (pBsStaticConfig->n_antenna_port==2)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->tpmi,
                    &pos,             
                    2);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->tpmi,
                    &pos,             
                    4);
    }
        
    // - PMI confirmation for precoding - 1 bit as specified in Table 5.3.3.1.3A-2
    put_bits((U32 *)w32, 
                pDciInfo->tpmi_confirm,
                &pos,             
                1);

    SWAP64_2params(w32, pSdu);


    *index= pBsStaticConfig->DCI_size.format1b;


    return MAC_SUCCESS;
}


U8 dci_encode_1c(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{

    U32 w32=0x00000000; 
    U16    riv;
    U8  pos;            

    if (pDciInfo->scheduling_pdsch_codeword)
    {
        // 1 bit indicates the gap value, where value 0 indicates   and value 1 indicates
        pos = 1;
        w32 |= (pDciInfo->index_ngap)<<(32-pos);

        /*  - For localized VRB:
         * [log2(N_DL_VRBgap1/n_step_rb * (N_DL_VRBgap1/n_step_rb + 1)/2)] bits provide the resource allocation
         */
        pos += pBsStaticConfig->size_rba_vrb;
        riv = calc_riv(pDciInfo, pBsStaticConfig, RIV_FOR_VRB);

        w32 |= (riv&((1<<pBsStaticConfig->size_rba_vrb)-1))<<(32-pos);

        // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213
        pos += 5; // // size of Mcs always ==5        
        w32 |= (pDciInfo->tb_info[0].modulation_type)<<(32-pos);
    }
    else
    {
        pos = 8;
        /*     - Information for MCCH change notification - 8 bits as defined in section 5.8.1.3 of 36.331
             - Reserved information bits are added until the size is equal to that of format 1C used for very compact scheduling of one PDSCH codeword
         */
        w32 = (pDciInfo->notif_mcch)<<(32-pos);
    }

    *pSdu = SWAP((U32)w32);     // change byte-order
    *index= pBsStaticConfig->DCI_size.format1c;


    return MAC_SUCCESS;
}

U8 dci_encode_1d(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{
    U32 w32[2]={0x00000000, 0x00000000}; 
    U8  pos=63;    
    U16 riv;

    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized
        pos--;

        /*  - For localized VRB:
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the resource allocation
         */
        riv = calc_riv(pDciInfo, pBsStaticConfig, RIV_FOR_PDSCH);    
        put_bits((U32 *)w32, 
                    riv,
                    &pos,             
                    pBsStaticConfig->size_rba_pdsch);
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
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].modulation_type,
                &pos,             
                5);

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos,             
                    4);
    }
    else  // FDD
    {
        put_bits((U32 *)w32, 
                    pDciInfo->harq_process_id,
                    &pos,             
                    3);
    }

    
    // - New data indicator - 1 bit
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits
    put_bits((U32 *)w32, 
                pDciInfo->tb_info[0].redundancy_ver,
                &pos,             
                2);

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213
    put_bits((U32 *)w32, 
                pDciInfo->tpc,
                &pos,             
                2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
        // with uplink-downlink configurations 1-6)
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            put_bits((U32 *)w32, 
                        pDciInfo->dai,
                        &pos,             
                        2);
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    // - tpmi information for precoding - number of bits as specified in Table 5.3.3.1.3A-1
    if (pBsStaticConfig->n_antenna_port==2)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->tpmi,
                    &pos,             
                    2);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->tpmi,
                    &pos,             
                    4);
    }
        
    // - Downlink power offset - 1 bit as defined in section 7.1.5 of [3]
    put_bits((U32 *)w32, 
                pDciInfo->power_dl_offset,
                &pos,             
                1);

    SWAP64_2params(w32, pSdu);

    *index= pBsStaticConfig->DCI_size.format1d;


    return MAC_SUCCESS;
}

U8 dci_encode_2(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{
    U32 w32[2]={0x00000000, 0x00000000}; // FlagFormat0 always ==1
    U8  pos=63;            // size of Resource allocation header always ==1

    /* If downlink bandwidth is less than or equal to 10 PRBs, there is no resource allocation 
     * header and resource allocation type 0 is assumed. 
     */

    if (pBsStaticConfig->n_dl_rb>10)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->rah,
                    &pos,             
                    1);
    }

    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized 
        pDciInfo->rbg = calc_rbg(pDciInfo->l_crb, pDciInfo->start_rb_index, pBsStaticConfig->P, pBsStaticConfig->size_rbg_pusch);
        put_bits((U32 *)w32, 
                    pDciInfo->rbg,
                    &pos,             
                    pBsStaticConfig->size_rbg_pusch);        // size
    }
    else
    {
        /* 1 - For distributed VRB:
        */
    }

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tpc,
                &pos,             
                2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            put_bits((U32 *)w32,
                        pDciInfo->dai,
                        &pos,             
                        2);
            pos += 2;
        }
        pos -= 2; // sizeof dai always == 2
    }

    //  - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        put_bits((U32 *)w32,
                    pDciInfo->harq_process_id,
                    &pos,             
                    4);
    }
    else  // FDD
    {
        put_bits((U32 *)w32,
                    pDciInfo->harq_process_id,
                    &pos,             
                    3);
    }

    // - Transport block to codeword swap flag - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->swap_flag,
                &pos,             
                1);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].modulation_type,
                &pos,             
                5);

    // - New data indicator - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].redundancy_ver,
                &pos,             
                2);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].modulation_type,
                &pos,             
                5);

    // - New data indicator - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].redundancy_ver,
                &pos,             
                2);

    // - Precoding information - number of bits as specified in Table 5.3.3.1.5-3
    if (pBsStaticConfig->n_antenna_port==2)
    {
        put_bits((U32 *)w32,
                    pDciInfo->precoding_info,
                    &pos,             
                    3);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        put_bits((U32 *)w32,
                    pDciInfo->precoding_info,
                    &pos,             
                    6);
    }

    SWAP64_2params(w32, pSdu);

    *index= pBsStaticConfig->DCI_size.format2;

    return MAC_SUCCESS;
}

U8 dci_encode_2a(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{

    U32 w32[2]={0x00000000, 0x00000000}; // FlagFormat0 always ==1
    U8  pos=63;            // size of Resource allocation header always ==1

    /* If downlink bandwidth is less than or equal to 10 PRBs, there is no resource allocation 
     * header and resource allocation type 0 is assumed. 
     */
    if (pBsStaticConfig->n_dl_rb>10)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->rah,
                    &pos,             
                    1);
    }

    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized 
        put_bits((U32 *)w32, 
                    pDciInfo->rbg,
                    &pos,             
                    pBsStaticConfig->size_rbg_pusch);        // size
    }
    else
    {
        /* 1 - For distributed VRB:
        */
    }

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tpc,
                &pos,             
                2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            put_bits((U32 *)w32,
                        pDciInfo->dai,
                        &pos,             
                        2);
            pos += 2;

        }
        pos -= 2; // sizeof dai always == 2
    }

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        put_bits((U32 *)w32,
                    pDciInfo->harq_process_id,
                    &pos,             
                    4);
    }
    else  // FDD
    {
        put_bits((U32 *)w32,
                    pDciInfo->harq_process_id,
                    &pos,             
                    3);
    }

    // - Transport block to codeword swap flag - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->swap_flag,
                &pos,             
                1);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].modulation_type,
                &pos,             
                5);

    // - New data indicator - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].redundancy_ver,
                &pos,             
                2);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].modulation_type,
                &pos,             
                5);

    // - New data indicator - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].redundancy_ver,
                &pos,             
                2);

    // - Precoding information - number of bits as specified in Table 5.3.3.1.5-3
    if (pBsStaticConfig->n_antenna_port==2)
    {
        put_bits((U32 *)w32,
                    pDciInfo->precoding_info,
                    &pos,             
                    2);
    }
    else if (pBsStaticConfig->n_antenna_port==4)
    {
        put_bits((U32 *)w32,
                    pDciInfo->precoding_info,
                    &pos,             
                    4);
    }

    SWAP64_2params(w32, pSdu);

    *index= pBsStaticConfig->DCI_size.format2a;

    return MAC_SUCCESS;

}

U8 dci_encode_2b(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index)
{

    U32 w32[2]={0x00000000, 0x00000000}; // FlagFormat0 always ==1
    U8  pos=63;            // size of Resource allocation header always ==1

    /* If downlink bandwidth is less than or equal to 10 PRBs, there is no resource allocation 
     * header and resource allocation type 0 is assumed. 
     */
    if (pBsStaticConfig->n_dl_rb>10)
    {
        put_bits((U32 *)w32, 
                    pDciInfo->rah,
                    &pos,             
                    1);
    }

    if (pDciInfo->virtul_resrc_blk_type == 0)
    {
        // 0 - indicates Localized 
        put_bits((U32 *)w32, 
                    pDciInfo->rbg,
                    &pos,             
                    pBsStaticConfig->size_rbg_pusch);        // size
    }
    else
    {
        /* 1 - For distributed VRB:
        */
    }

    // - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tpc,
                &pos,             
                2);

    // - Downlink Assignment Index (dai) - 2 bits as defined in section 7.3 of 36.213-930 (this field is present only for TDD operation 
    // with uplink-downlink configurations 1-6)
    if(pBsStaticConfig->tdd) // TDD
    {
        if (pBsStaticConfig->tdd_configuration>0) // configurations 1-6
        {
            put_bits((U32 *)w32,
                        pDciInfo->dai,
                        &pos,             
                        2);
            pos += 2;

        }
        pos -= 2; // sizeof dai always == 2
    }

    // - HARQ process number - 3 bits (FDD), 4 bits (TDD) 
    if (pBsStaticConfig->tdd)
    {
        put_bits((U32 *)w32,
                    pDciInfo->harq_process_id,
                    &pos,             
                    4);
    }
    else  // FDD
    {
        put_bits((U32 *)w32,
                    pDciInfo->harq_process_id,
                    &pos,             
                    3);
    }

    // - Scrambling identity- 1 bit as defined in section 6.10.3.1 of 36.211
    put_bits((U32 *)w32,
                pDciInfo->scrambl_indentity,
                &pos,             
                1);

    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].modulation_type,
                &pos,             
                5);

    // - New data indicator - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[0].redundancy_ver,
                &pos,             
                2);


    // - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].modulation_type,
                &pos,             
                5);

    // - New data indicator - 1 bit 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].ndi,
                &pos,             
                1);
    
    // - Redundancy version - 2 bits 
    put_bits((U32 *)w32,
                pDciInfo->tb_info[1].redundancy_ver,
                &pos,             
                2);

    SWAP64_2params(w32, pSdu);

    *index= pBsStaticConfig->DCI_size.format2b;

    return MAC_SUCCESS;
}

