/** @file dci.h
 *
 * @brief Defines for DCIs
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _DCI_H
#define _DCI_H

#include "bs_static_config.h"

#define RIV_FOR_PUSCH        0
#define RIV_FOR_PDSCH        1
#define RIV_FOR_VRB          2

// TODO: Using only in "enbMacTestCfgMgr.h"
#define MAX_DCI_SIZE_BYTES   (8/sizeof(int))

typedef enum DCIFormatT
{
    DCI_FORMAT_HARDCODED_PAYLOAD = 0,
    DCI_FORMAT_0,     /* DCI format 0 is used for the scheduling of PUSCH */
    DCI_FORMAT_1,     /* DCI format 1 is used for the scheduling of one PDSCH codeword */
    DCI_FORMAT_1A,    /* DCI format 1A is used for the compact scheduling of one
                         PDSCH codeword and random access procedure initiated by a PDCCH order. */
    DCI_FORMAT_1B,    /* DCI format 1B is used for the compact scheduling of one
                         PDSCH codeword with precoding information. */
    DCI_FORMAT_1C,    /* DCI format 1C is used for very compact scheduling of one
                         PDSCH codeword. */
    DCI_FORMAT_1D,    /* DCI format 1D is used for the compact scheduling of one
                         PDSCH codeword with precoding and power offset information. */
    DCI_FORMAT_2,     /* DCI format 2 is used for scheduling PDSCH to UEs configured
                         in closed-loop spatial multiplexing mode */
    DCI_FORMAT_2A,    /* DCI format 2A is used for scheduling PDSCH to UEs
                         configured in open loop spatial multiplexing mode. */
    DCI_FORMAT_3,     /* DCI format 3 is used for the transmission of TPC commands
                         for PUCCH and PUSCH with 2-bit power adjustments. */
    DCI_FORMAT_3A,    /* DCI format 3A is used for the transmission of TPC
                         commands for PUCCH and PUSCH with single bit power adjustments. */
    DCI_FORMAT_RESERVED, 
    MAX_DCI_FORMAT
} DCI_FORMAT;

typedef struct TBInfoT
{
    U16 tbSize;
    U8 modulationType;
    U8 redundancyVer;
    U8 txIndicator;
    U8 mcs;

}TB_INFO;

typedef struct
{
    U8 modulation_type;
    U8 redundancy_ver;

    U8 cycle_shift;
    U8 cqi_request;
    U8 ndi;
}TB_INFO_new;


typedef struct DciDataInfoT // legacy struct - will be replaced with DCI_INFO
{
    U32 resrcAssgmntBitMap;
    TB_INFO tbInfo[2];
    DCI_FORMAT dciFormat;
    U16 rnti;
    U16 ueIndex;
    U8 cceIndex;
    U8 aggregationLvl;
    U8 numAntennaPort;
    U8 resrcAllocType;
    U8 rankIndicator;
    U8 transmissionMode;
    U8 numTB;
    U8 numLayer;
    U8 codeBookIndex;
    U8 tbSwapFlag;
    U8 ueCategory;
    U8 pA;
    U8 virtulResrcBlkType;
    U8 *pTbData;
    U8 tpc;
    U8 harqProcessId;
    U8 pmiConfirmation;
    U8 transmittedPMI;
    U8 cqiRequest;
    U8 startRBIndex;
    U8 numOfRBs;
    U8 RGBSize;
    U8 hopping;
    U8 Lcrb;
    U8 MaxRb;
}DCI_DATA_INFO;

typedef struct
{
    DCI_FORMAT  dci_format;

    TB_INFO_new tb_info[2];

    U8 virtul_resrc_blk_type;            //!< Localized/Distributed VRB assignment flag in DCI format 1A/1B/1D
    U8 tpc;                              //!< TPC Command Field in DCI format 0/1A/1B/1D/1/2A/2B/2/3
    U8 harq_process_id;                  //!< HARQ process number in DCI format 1/1A/1B/1D/2/2A/2B
    U8 start_rb_index;                   //!< first number of Rb for encode/decode riv
    U8 hopping;                          //!< Frequency hopping flag - 1 bit as defined in section 8.4 of 36.213-930
    U8 l_crb;                            //!< number of Rbs

    U32 rbg;                             //!< Resource block group

    U8  rah;                             //!< Resource allocation header (res alloc type 0 / type 1) - 1 bit, section 7.1.6 of 36.213-930
    U8  tpmi;                            //!< TPMI information for precoding (format1b, 1d)
    U8  tpmi_confirm;                    //!< PMI confirmation for precoding (format1b)
    U8  ul_index;                        //!< using in the TDD (configuration  0)
    U8  dai;                             //!< using in the TDD (configurations 1-6)
    U8  power_dl_offset;                 //!< Downlink power offset - 1 bit as defined in section 7.1.5 of 36.213-930
    U8  swap_flag;                       //!< Transport block to codeword swap flag - 1 bit
    U8  scrambl_indentity;               //!< Scrambling identity- 1 bit as defined in section 6.10.3.1 of [2]

    U8 scheduling_pdsch_codeword;        //!< DCI format 1C is used for very compact scheduling of one PDSCH codeword and notifying MCCH change 
    U8 index_ngap;                       //!< 1 bit indicates the gap value

    U8 notif_mcch;                       //!< Information for MCCH change notification - 8 bits as defined in section 5.8.1.3 of 36.331

    U8 cyclic_shift_dm_rs;               //!< Cyclic shift for DM RS – 3 bits as defined in section 5.5.2.1.1 of 36.211

    U8 precoding_info;                   //!< Precoding information

    U8 mcs_rv;                           //!< Modulation and coding scheme and redundancy version – 5 bits as defined in section 8.6 of 36.213-930 
    U8 cqi_request;                      //!< CQI request - 1 bit as defined in section 7.2.1 of 36.213-930
    U8 ndi;                              //!< New data indicator - 1 bit

} DCI_INFO, *PDCI_INFO;

/*
 * Interface Functions
 */


U8 dci_encode_0 (U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);
U8 dci_encode_1 (U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);
U8 dci_encode_1a(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);
U8 dci_encode_1b(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);
U8 dci_encode_1c(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);
U8 dci_encode_1d(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);


U8 dci_encode_2 (U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);
U8 dci_encode_2a(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);
U8 dci_encode_2b(U32 *pSdu, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U32 *index);

__inline U16 calc_riv(DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U8 type);

U8 dci_decode(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U8 payload_length);
void dci_decode_0 (U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);
void dci_decode_1 (U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);
void dci_decode_1a(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);
void dci_decode_1b(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);
void dci_decode_1c(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);
void dci_decode_1d(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);

void dci_decode_2(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);
void dci_decode_2a(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);
void dci_decode_2b(U32 *p, DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig);

void decode_format_0(U8 *p, U8 n_rbs, U8 *rb_start, U8 *L_crbs, U8 *CS_Dmrs);

void calc_nGPhich(BS_STATIC_CONFIG *pBsStaticConfig);

U16  calc_riv(DCI_INFO *pDciInfo, BS_STATIC_CONFIG *pBsStaticConfig, U8 type);
void calc_dci_size(DCI_SIZE_t *pDCI_size, BS_STATIC_CONFIG *pBsStaticConfig); 


#endif // _DCI_H

