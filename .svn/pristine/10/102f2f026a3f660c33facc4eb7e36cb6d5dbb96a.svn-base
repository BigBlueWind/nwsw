//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ad9361radio.h
 *
 * @brief ADI9361 Radio control
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

#ifndef __AD9361RADIO_H__
#define __AD9361RADIO_H__

#include "4gmx.h"
#include "config.h"

#define RC_AD9361_ALLOC_DEV_ERROR       DEF_USRRC(APP_DRIVER, AD9361_DRV, 1)

#define SPIRD       1
#define SPIWR       2
#define DELAY       3
#define WCALDONE    4

#define BBPLL       0x10
#define RXCP        0x11
#define TXCP        0x12
#define RXFILTER    0x13
#define TXFILTER    0x14
#define RFDC        0x15
#define TXQUAD      0x16
#define BBDC        0x17

#define AD9361_RADIO_PROF_BAND4     4
#define AD9361_RADIO_PROF_BAND7     7
#define AD9361_RADIO_PROF_BAND13    13

#define RX_FREQ_INT1 0x231
#define RX_FREQ_INT2 0x232
#define RX_FREQ_FRAC1 0x233
#define RX_FREQ_FRAC2 0x234
#define RX_FREQ_FRAC3 0x235

#define TX_FREQ_INT1 0x271
#define TX_FREQ_INT2 0x272
#define TX_FREQ_FRAC1 0x273
#define TX_FREQ_FRAC2 0x274
#define TX_FREQ_FRAC3 0x275

#define DIV_REG 0x005

#define DEF_RFCLC 40.0

#define RX_VCO_OUTPUT_LEVEL_REG 	0x23A
#define RX_INIT_ALC_VAL_REG 		0x239
#define RX_VCO_BASIS_TCF_REG		0x242
#define RX_VCO_CAL_OFFSET_REG 		0x238
#define RX_VCO_CAL_REF_TCF_REG		0x245
#define RX_VCO_VARIATOR_REF_REG 	0x251
#define RX_VCO_VARIATOR_REF_TCF_REG	0x250
#define RX_SYNTH_LOOP_FILTER_R3		0x240
#define RX_SYNTH_LOOP_FILTER_R1_C3  0x23F
#define RX_SYNTH_LOOP_FILTER_C2_C1 	0x23E
#define RX_SYNTH_LOOP_FILTER_ICP	0x23B
#define TX_VCO_OUTPUT_LEVEL_REG		0x27A
#define TX_INIT_ALC_VAL_REG			0x279
#define TX_VCO_BASIS_TCF_REG		0x282
#define TX_VCO_CAL_OFFSET_REG		0x278
#define TX_VCO_CAL_REF_TCF_REG		0x285
#define TX_VCO_VARIATOR_REF_REG		0x291
#define TX_VCO_VARIATOR_REF_TCF_REG	0x290
#define TX_SYNTH_LOOP_FILTER_R3		0x280
#define TX_SYNTH_LOOP_FILTER_R1_C3	0x27F
#define TX_SYNTH_LOOP_FILTER_C2_C1	0x27E
#define TX_SYNTH_LOOP_FILTER_ICP	0x27B
#define RX_RF_PLL_LOCK_STATUS		0x247
#define TX_RF_PLL_LOCK_STATUS		0x287

typedef struct tagADRADINITPARAMS
{
    UINT32 nProfID;
    UINT32 NumTxAntennas;
    UINT32 NumRxAntennas;
    UINT32 loopback;
    UINT32 rxfreq;
    UINT32 txfreq;
} ADRADINITPARAMS, *PADRADINITPARAMS;

typedef struct tagADRADPROFILE
{
    UINT16    OpCode;
    UINT16    Addr;
    UINT16    Data;
} ADRADPROFILE, *PADRADPROFILE;

/** @brief Structure for script based programming of AD9361 */
typedef struct tagADRADPATCH
{
    UINT16    Addr;
    UINT16    Data;
} ADRADPATCH, *PADRADPATCH;

/** @brief This function initializes AD9361 radio module
            with selected profile configuration

    @param nProfID          [in] - the prof id
    @param NumTxAntennas    [in] - the number of TX antennas
    @param NumRxAntennas    [in] - the number of RX antennas
    @param loopback         [in] - loopback mode for ADI card

    @return [MXRC] an error code */

MXRC Ad9361RadioInit(UINT32 nProfID, UINT32 NumTxAntennas, UINT32 NumRxAntennas, UINT32 loopback, double rxfreq, double txfreq);
UINT32 Ad9361ReadReg(UINT32 spi_cs, UINT16 addr);
void Ad9361WriteReg(UINT32 spi_cs, UINT16 addr, UINT8 data);
UINT32 Ad9361GetRSSI(UINT32 ch);
MXRC Ad9361SetFreq(double rxfreq, double txfreq);


#endif

#ifdef __cplusplus
}
#endif

