//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ad9361radio.c
 *
 * @brief ADI9361 Radio control
 * @author Mindspeed Technologies
 * @version $Revision: 1.25 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"
#include "ad9361radio.h"
#include "hal.h"
#include "spidrv.h"
#include "srdsdrv.h"
#include "appprintf.h"
#include "math.h"
#include "ad361lte10_b7.h" //2.6 Ghz
#include "ad361lte10_b1.h" //2.1 Ghz
#include "ad361lte10_b4.h" //band 4
#include "ad361lte10_b13.h"//band 13
#include "ad361lte10_Rev2_NPB_Test2_b7.h" // AD9361 revision 2 chip Band 7 (2.655 GHz transmit, 2.535 receive) (external oscillator)
#include "ad361lte10_Rev2_NPB_Test2_b7_nmm.h" // AD9361 revision 2 chip Band 7 (2.655 GHz transmit, 2.535 receive) (external oscillator)
#include "ad361lte10_Rev2_NPB_Test2_b13.h" // AD9361 revision 2 chip Band 13 (external oscillator)
#include "ad361lte10_b5_customer.h"

UINT32 Ad9361inited = 0;
UINT32 Adi9361InitState = 0;
UINT32 Adi9361Rev = 0;

UINT32 RF_CHANGE_REGS[] = 
{
	RX_VCO_OUTPUT_LEVEL_REG,
	RX_INIT_ALC_VAL_REG,
	RX_VCO_BASIS_TCF_REG,
	RX_VCO_CAL_OFFSET_REG,
	RX_VCO_CAL_REF_TCF_REG,
	RX_VCO_VARIATOR_REF_REG,
	RX_VCO_VARIATOR_REF_TCF_REG,
	RX_SYNTH_LOOP_FILTER_R3,
	RX_SYNTH_LOOP_FILTER_R1_C3,
	RX_SYNTH_LOOP_FILTER_C2_C1,
	RX_SYNTH_LOOP_FILTER_ICP,
	TX_VCO_OUTPUT_LEVEL_REG,
	TX_INIT_ALC_VAL_REG,
	TX_VCO_BASIS_TCF_REG,
	TX_VCO_CAL_OFFSET_REG,
	TX_VCO_CAL_REF_TCF_REG,
	TX_VCO_VARIATOR_REF_REG,
	TX_VCO_VARIATOR_REF_TCF_REG,
	TX_SYNTH_LOOP_FILTER_R3,
	TX_SYNTH_LOOP_FILTER_R1_C3,
	TX_SYNTH_LOOP_FILTER_C2_C1,
	TX_SYNTH_LOOP_FILTER_ICP,
	RX_FREQ_INT1,
	RX_FREQ_INT2,
	RX_FREQ_FRAC1,
	RX_FREQ_FRAC2,
	RX_FREQ_FRAC3,
	TX_FREQ_INT1,
	TX_FREQ_INT2,
	TX_FREQ_FRAC1,
	TX_FREQ_FRAC2,
	TX_FREQ_FRAC3,
	DIV_REG,
	0,0,0
};

ADRADPROFILE rfregval[36];

ADRADINITPARAMS Ad9361InitParams;

PADRADINITPARAMS Ad9361GetInitParamCtx(void)
{
    return &Ad9361InitParams;
}

typedef struct freq_reg_data_t
{
    UINT8 intr1;
    UINT8 intr2;
    UINT8 freqr1;
    UINT8 freqr2;
    UINT8 freqr3;
    UINT8 div;
}freq_reg_data;

UINT32 Ad9361CalcFreq(double freq, freq_reg_data *fd, double refclc)
{
    unsigned int integ, fract, i, dev;
    double integf, fractf;

    if(!fd)
        return 0;

    refclc = Adi9361Rev ? refclc * 2 : refclc;

    if(freq >= 3000 && freq < 6000)
        fd->div = 0;
    else if(freq >= 1500 && freq < 3000)
        fd->div = 1;
    else if(freq >= 750 && freq < 1500)
        fd->div = 2;
    else if(freq >= 375 && freq < 750)
        fd->div = 3;
    else if(freq >= 187 && freq < 375)
        fd->div = 4;
    else if(freq >= 93 && freq < 187)
        fd->div = 5;
    else if(freq >= 46 && freq < 93)
        fd->div = 6;
    else
        return 0;

    dev = 2;

    for(i = 0; i < fd->div; i++)
        dev *= 2;

    integf = (freq * dev) / refclc;

    integ = (int)(integf);

    fractf = 8388593 * (integf - (float)integ);
    fract = (int)(fractf);
	
    if(fractf - (double)(fract) > 0.5)
        fract++;

    _memcpy(&fd->intr1, &integ, 2);
    _memcpy(&fd->freqr1, &fract, 3);

    return 0;
}

UINT32 Ad9361FreqPatch(ADRADPROFILE * tab, UINT32 nElmNum, double rxfreq, double txfreq, double refclc)
{
    freq_reg_data rx, tx;
    unsigned int i;

    if(!tab)
        return 0;

    if(rxfreq)
    {
        memset(&rx, 0, sizeof(freq_reg_data));
        Ad9361CalcFreq(rxfreq, &rx, refclc);
    }

    if(txfreq)
    {
        memset(&tx, 0, sizeof(freq_reg_data));
        Ad9361CalcFreq(txfreq, &tx, refclc);
    }

    for(i = 0; i < nElmNum; i++)
    {
        if(rxfreq)
        {
            switch(tab[i].Addr)
            {
            case RX_FREQ_INT1:
                tab[i].Data = rx.intr1;
                break;
            case RX_FREQ_INT2:
                tab[i].Data = rx.intr2;
                    break;
            case RX_FREQ_FRAC1:
                tab[i].Data = rx.freqr1;
                break;
            case RX_FREQ_FRAC2:
                tab[i].Data = rx.freqr2;
                break;
            case  RX_FREQ_FRAC3:
                tab[i].Data = rx.freqr3;
                break;
            case DIV_REG:
                tab[i].Data = tab[i].Data & 0xF0 | rx.div;
                break;
            }

        }
        if(txfreq)
        {
            switch(tab[i].Addr)
            {
            case TX_FREQ_INT1:
                tab[i].Data = tx.intr1;
                break;
            case TX_FREQ_INT2:
                tab[i].Data = tx.intr2;
                break;
            case TX_FREQ_FRAC1:
                tab[i].Data = tx.freqr1;
                break;
            case TX_FREQ_FRAC2:
                tab[i].Data = tx.freqr2;
                break;
            case TX_FREQ_FRAC3:
                tab[i].Data = tx.freqr3;
                break;
            case DIV_REG:
                tab[i].Data = tab[i].Data & 0x0F | tx.div << 4;
                break;
            }
        }
    }

    return 0;
}


UINT32 Ad9361AddNumAntennasPatch(PADRADPATCH PatchTable,
                                 UINT32      CurrentIndex,
                                 UINT32      NumTxAntennas,
                                 UINT32      NumRxAntennas)
{
    //
    // Setup patch for number of TX antennas
    //
    PatchTable[CurrentIndex].Addr = 0x002; // TX antenna enable register
    if (NumTxAntennas == 1)
    {
        //
        // 1 Antenna requested, setup for AD9361 for one antenna (single bus TX/FX)
        // Bit 7:0 6:1
        //
        PatchTable[CurrentIndex].Data = 0x5E; // TX Enable First antenna, disable second antenna

    }
    else
    {
        //
        // Two antennas
        // Bit 7:1 6:1
        //
        PatchTable[CurrentIndex].Data = 0xDE; // TX Enable both antennas
    }
    CurrentIndex++;
    //
    // Setup RX antenna enable bits for 1 or 2 antennas
    //
    PatchTable[CurrentIndex].Addr = 0x003; // RX antenna enable register
    if (NumRxAntennas == 1)
    {
        PatchTable[CurrentIndex].Data = 0x5E;
    }
    else
    {
        PatchTable[CurrentIndex].Data = 0xDE;
    }
    CurrentIndex++;
    //
    // Set level versus pulse mode (level for single RX antenna, pulse for dual)
    //
    PatchTable[CurrentIndex].Addr = 0x010; // RX antenna enable register
    if (NumRxAntennas == 1)
    {
        PatchTable[CurrentIndex].Data = 0xC0; // RX Frame signal Level mode, bit 3 equals 0
    }
    else
    {
        PatchTable[CurrentIndex].Data = 0xC8; // RX Frame signal Level mode, bit 3 equals 1
    }
    CurrentIndex++;

    return CurrentIndex;
}

UINT16 Ad9361SearchPatchTable(PADRADPATCH PatchTable, UINT32 NumberOfEntries, UINT16 Address, UINT16 Value)
{
    UINT32 i;

    if (NumberOfEntries != 0)
    {
        for (i = 0; i < NumberOfEntries; i++)
        {
            if (PatchTable[i].Addr == Address)
            {
                Value = PatchTable[i].Data;
                break;
            }
        }
    }
    return Value;
}

UINT32 Ad9361IsRadioInited(void)
{
    return Ad9361inited;
}

UINT32 Ad9361RadioInitState(void)
{
    return Adi9361InitState;
}

MXRC Ad9361CallRadioInit(LPVOID pCtx)
{
    PADRADINITPARAMS pAdRadInitParams = Ad9361GetInitParamCtx();

    Ad9361RadioInit(pAdRadInitParams->nProfID, pAdRadInitParams->NumTxAntennas, pAdRadInitParams->NumRxAntennas,
                    pAdRadInitParams->loopback, pAdRadInitParams->rxfreq, pAdRadInitParams->txfreq);

    return MX_OK;
}

MXRC Ad9361IRadioScheduleInit(UINT32 nProfID, UINT32 NumTxAntennas, UINT32 NumRxAntennas, UINT32 loopback, UINT32 rxfreq, UINT32 txfreq)
{
    PADRADINITPARAMS pAdRadInitParams = Ad9361GetInitParamCtx();
    UINT32 cpuID = MxGetCpuID();
    HANDLE hAdiRadioInit = HNULL;
    UINT32 retVal = MX_OK;

    if(cpuID == 0)
    {
        retVal = Ad9361RadioInit(nProfID, NumTxAntennas, NumRxAntennas, loopback, rxfreq, txfreq);
    }
    else
    {
        pAdRadInitParams->nProfID = nProfID;
        pAdRadInitParams->NumTxAntennas = NumTxAntennas;
        pAdRadInitParams->NumRxAntennas = NumRxAntennas;
        pAdRadInitParams->loopback = loopback;
        pAdRadInitParams->rxfreq = rxfreq;
        pAdRadInitParams->txfreq = txfreq;

        retVal = MxCreateThreadEx(0, Ad9361CallRadioInit, NULL, NULL, 0, &hAdiRadioInit);

        if (FAILED(retVal))
        {
            uart_printf("[CPU: %d]: Error to create thread on core-0, rc=0x%x\r\n", cpuID, retVal);
            return retVal;
        }

        if (HANDLE_NOT_NULL (hAdiRadioInit))
        {
            uart_printf("[CPU: %d]: Scheduling ADI Radio init\r\n", cpuID);
            retVal = MxScheduleThread(hAdiRadioInit);

            if (FAILED(retVal))
            {
                MxRemoveThread(hAdiRadioInit);
                uart_printf("[CPU: %d]: Error to schedule thread on core-0, rc=0x%x\r\n", cpuID, retVal);
                return retVal;
            }
        }
        else
        {
        }

        // Since Radio Init has been scheduled, wait for it to be inited
        while(1)
        {
            if (Ad9361IsRadioInited())
            {
                retVal = Ad9361RadioInitState();
                break;
            }

            // Wait for some time (100 ms)
            MxDelayTicks(MS_TO_TICKS(100));
        }

        MxRemoveThread(hAdiRadioInit);
    }

    return retVal;
}


/** @brief This function initializes AD9361 radio module
            with selected profile configuration

    @param nProfID          [in] - the prof id
    @param NumTxAntennas    [in] - the number of TX antennas
    @param NumRxAntennas    [in] - the number of RX antennas
    @param loopback         [in] - loopback mode for ADI card

    @return [MXRC] an error code */

MXRC Ad9361RadioInit(UINT32 nProfID, UINT32 NumTxAntennas, UINT32 NumRxAntennas, UINT32 loopback, double rxfreq, double txfreq)
{
    UINT32 rc = MX_OK;
    UINT32 i, j;
    UINT8 val;
    UINT32 spi_cs = SPI_RAD0;
    SPICONFIG * pSpiConfig = NULL;
    UINT32 addr;

    ADRADPROFILE * Ad261Lte10Mhz;
    UINT32 nAdElmNum = 0;

    ADRADPATCH Ad9361PatchTable[8];
    UINT32 NumberOfPatchEntries=0;
    ADRADPROFILE *prfregval = &rfregval[0];

    memset(rfregval, 0, sizeof(rfregval));

    //
    // Setup patch table for programming multiple options
    //
    //
    // Build patch table based on registers
    //
    NumberOfPatchEntries += Ad9361AddNumAntennasPatch
                            (
                                Ad9361PatchTable,
                                NumberOfPatchEntries,
                                NumTxAntennas,       // Number of TX antennas
                                NumRxAntennas        // Number of RX antennas
                            );

    // Take Radio 0 out of reset

    uart_printf("Radio HW Reset via GPIO3\r\n");
    REG32(GPIO_OE_REG) |= GPIO_3;     // GPIO3 controls Radio module reset
    REG32(GPIO_OUTPUT_REG) &= ~GPIO_3; // set it low to reset
    MxDelayTicks(150000*2);
    REG32(GPIO_OUTPUT_REG) |= GPIO_3; // set it high to put it out of reset

    if (!SPIIsInit())
    {
        rc = SPIInit (SPI_DEF_DEV_NUM);

        if (FAILED(rc))
        {
            uart_printf ("SPI driver initialization error, rc=%x\r\n", rc);
            Adi9361InitState = rc;
            return rc;
        }
    }

    pSpiConfig = SPICreateDevCtx();

    if (pSpiConfig == NULL)
    {
        uart_printf ("SPI driver device allocation error\r\n");
        Adi9361InitState = RC_AD9361_ALLOC_DEV_ERROR;
        return RC_AD9361_ALLOC_DEV_ERROR;
    }

    pSpiConfig->frameSize       = SPI_FRAME_SIZE_8BITS;
    pSpiConfig->frameFormat     = SPI_MOTOROLA_FORMAT;
    pSpiConfig->clkMode         = SPI_CLOCK_MODE1;
    pSpiConfig->transferMode    = SPI_RX_TX_ENABLE;
    pSpiConfig->frqDivider      = SPI_DIVIDER_SPEED_1M;
    pSpiConfig->CS              = spi_cs;

    rc = SPISelectDevice(pSpiConfig);

    if (FAILED(rc))
    {
        SPIRemoveDevCtx(pSpiConfig);
        pSpiConfig = NULL;
        uart_printf ("SPI driver select device error, rc=%x\r\n", rc);

        Adi9361InitState = rc;
        return rc;
    }


    SPIDisableCS(SPIGetCtrlRegs (), spi_cs);

    // Setup GPIO for SPI
    REG32(GPIO_OE_REG)         |= GPIO_21;
    REG32(GPIO_OUTPUT_REG)     |= GPIO_21;
    REG32(GPIO_PIN_SELECT_REG) |= GPIO_21;

    Ad9361WriteReg(spi_cs, 0x3df, 0x01);
    //
    // Get the revision ID
    //
    val = Ad9361ReadReg(spi_cs, 0x37);
    uart_printf("AD9361 Revision Register:0x%02x\n",val);
    val &= 0x07; // Mask out all bits except D2 through D0

    if (val < 2)
    {
        switch (nProfID)
        {
            case 4:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)Ad261Lte10Mhz_Band4;
                nAdElmNum = sizeof(Ad261Lte10Mhz_Band4)/sizeof(Ad261Lte10Mhz_Band4[0]);
                uart_printf("AD9361 (band-4) device initialization...\r\n");
                break;
            }

            case 7:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)Ad261Lte10Mhz_2_6GHZ;
                nAdElmNum = sizeof(Ad261Lte10Mhz_2_6GHZ)/sizeof(Ad261Lte10Mhz_2_6GHZ[0]);
                uart_printf("AD9361 (band-7) device initialization...\r\n");
                break;
            }

            case 13:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)Ad261Lte10Mhz_Band13;
                nAdElmNum = sizeof(Ad261Lte10Mhz_Band13)/sizeof(Ad261Lte10Mhz_Band13[0]);
                uart_printf("AD9361 (band-13) device initialization...\r\n");
                break;
            }

            case 5:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)Ad261Lte10Mhz_b5;
                nAdElmNum = sizeof(Ad261Lte10Mhz_b5)/sizeof(Ad261Lte10Mhz_b5[0]);
                printf("AD9361 (band-5) device initialization...\r\n");
                uart_printf("AD9361 (band-5) device initialization...\r\n");
                break;
            }

            default:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)Ad261Lte10Mhz_2_1GHZ;
                nAdElmNum = sizeof(Ad261Lte10Mhz_2_1GHZ)/sizeof(Ad261Lte10Mhz_2_1GHZ[0]);
                uart_printf("AD9361 (band-1) device initialization...\r\n");
                break;
            }
        }
    }
    else
    {
        Adi9361Rev = 1;
		
        switch (nProfID)
        {
            case 7:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)&Ad261Rev2Lte10Mhz[0];
                nAdElmNum = sizeof(Ad261Rev2Lte10Mhz)/sizeof(Ad261Rev2Lte10Mhz[0]);
                uart_printf("Ad261Rev2 (band-7) device initialization...\r\n");
                break;
            }

            case 13:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)&Ad261Rev2_Band13_Lte10Mhz[0];
                nAdElmNum = sizeof(Ad261Rev2_Band13_Lte10Mhz)/sizeof(Ad261Rev2_Band13_Lte10Mhz[0]);
                uart_printf("Ad261Rev2 (band-13) device initialization...\r\n");
                break;
            }

            case 5:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)&Ad261Lte10Mhz_b5[0];
                nAdElmNum = sizeof(Ad261Lte10Mhz_b5)/sizeof(Ad261Lte10Mhz_b5[0]);
                printf("Ad261Rev2 (band-5) device initialization...\r\n");
                uart_printf("AD9361 (band-5) device initialization...\r\n");
                break;
            }

            case 107:
            {
                Ad261Lte10Mhz = (ADRADPROFILE *)&Ad261Rev2Lte10Mhz_nmm[0];
                nAdElmNum = sizeof(Ad261Rev2Lte10Mhz_nmm)/sizeof(Ad261Rev2Lte10Mhz_nmm[0]);
                uart_printf("Ad261Rev2 (band-7) NMM device initialization...\r\n");
                break;
            }

            default:
            {
                uart_printf("Ad261Rev2 (band-%d) not support!\r\n", nProfID);
                Adi9361InitState = rc;
                return rc;
            }
        }
    }

    if(rxfreq || txfreq)
    {
        Ad9361FreqPatch(Ad261Lte10Mhz, nAdElmNum, rxfreq, txfreq, DEF_RFCLC);
        uart_printf("patch table for rx freq %d and/or tx freq %d ...\r\n", (int)rxfreq, (int)txfreq);
    }

    uart_printf("Ad261%sLte10Mhz Profile Size=%d\r\n", (val < 2) ? "" : "rev2", nAdElmNum);

    for (i = 1; i < nAdElmNum; i++)
    {
        switch (Ad261Lte10Mhz[i].OpCode)
        {
            case SPIRD:
                val = Ad9361ReadReg(spi_cs, Ad261Lte10Mhz[i].Addr);
                break;
            case SPIWR:
                //Ad9361WriteReg(spi_cs, Ad261Lte10Mhz[i].Addr,Ad261Lte10Mhz[i].Data);

                //
                // Write operation, search patch table for
                // alternate value (if any)
                // And write out current data or patched
                // data if there is a match in the patch table
                //
                val = Ad9361SearchPatchTable(Ad9361PatchTable,
                                                NumberOfPatchEntries,
                                                Ad261Lte10Mhz[i].Addr,
                                                Ad261Lte10Mhz[i].Data);
				
                Ad9361WriteReg(spi_cs, Ad261Lte10Mhz[i].Addr, val);
                for(j = 0; j < (sizeof(RF_CHANGE_REGS) / sizeof(UINT32)); j++)
                {
                    if(Ad261Lte10Mhz[i].Addr == RF_CHANGE_REGS[j])
                    {
                        prfregval->Addr = Ad261Lte10Mhz[i].Addr;
                        prfregval->Data = val;
                        if(prfregval < (&rfregval[0] + sizeof(rfregval)))
                            prfregval++;
                        else
                            uart_printf("Ad9361 - rf reg val array to small\r\n");
                    }
                }

                break;
            case DELAY:
                SysDelay(Ad261Lte10Mhz[i].Data);
                break;
            case WCALDONE:
                SysDelay(Ad261Lte10Mhz[i].Data);
                break;
        }
    }

    for (addr = 0; addr < 512; addr++)
    {
        if ((addr % 16) == 0)
        {
            uart_printf("\r\n%03x: ", addr);
            //MxDelayTicks(150000*100);
        }
        val = Ad9361ReadReg(spi_cs, addr);
        uart_printf("%02x ", val);
    }
    uart_printf("\r\n");

    SPISelectDevice (NULL);
    SPIRemoveDevCtx(pSpiConfig);
    pSpiConfig = NULL;

    if (loopback)
    {
        Ad9361WriteReg(SPI_RAD0, 0x3f5, 0x01);

        //Initialize the device
        uart_printf("AD9361 driver initialization completed (loopback is enabled)\r\n");
    }
    else
    {
        Ad9361WriteReg(SPI_RAD0, 0x3f5, 0x00);
        uart_printf("AD9361 driver initialization completed (no loopback!)\r\n");
    }

    Ad9361inited = 1;
    Adi9361InitState = rc;

    return rc;
}

UINT32 Ad9361ReadReg(UINT32 spi_cs, UINT16 addr)
{
    UINT16 cw = addr;
    UINT32 val;

    SPIEnableCS( SPIGetCtrlRegs (), spi_cs);

    REG32(GPIO_OUTPUT_REG) &= ~GPIO_21;
    REG32(SPI_DR_REG) = cw >> 8;
    REG32(SPI_DR_REG) = cw & 0xFF;
    REG32(SPI_DR_REG) = 0;
    while (REG32(SPI_RXFLR_REG) < 3);
    val = REG32(SPI_DR_REG);
    val = REG32(SPI_DR_REG);
    val = REG32(SPI_DR_REG);
    REG32(GPIO_OUTPUT_REG) |= GPIO_21;

    SPIDisableCS(SPIGetCtrlRegs (), spi_cs);

    return val;
}

void Ad9361WriteReg(UINT32 spi_cs, UINT16 addr, UINT8 data)
{
    UINT16 cw = 0x8000 | addr;
    VUINT32 val;

    SPIEnableCS(SPIGetCtrlRegs (), spi_cs);
    REG32(GPIO_OUTPUT_REG) &= ~GPIO_21;
    REG32(SPI_DR_REG) = cw >> 8;
    REG32(SPI_DR_REG) = cw & 0xFF;
    REG32(SPI_DR_REG) = data;
    while (REG32(SPI_RXFLR_REG) < 3);
    val = REG32(SPI_DR_REG);
    val = REG32(SPI_DR_REG);
    val = REG32(SPI_DR_REG);
    REG32(GPIO_OUTPUT_REG) |= GPIO_21;

    SPIDisableCS(SPIGetCtrlRegs (), spi_cs);
}

UINT32 Ad9361GetRSSI(UINT32 ch)
{
    UINT32 rc = MX_OK;
    UINT32 res = 0;
    UINT32 spi_cs = SPI_RAD0;
    SPICONFIG * pSpiConfig = NULL;


    if(!Ad9361inited)
    {
        uart_printf ("Ad9361 not initialized!\r\n");
        return 0;
    }


    if (!SPIIsInit())
    {
        rc = SPIInit (SPI_DEF_DEV_NUM);

        if (FAILED(rc))
        {
            uart_printf ("SPI driver initialization error, rc=%x\r\n", rc);
            return rc;
        }
    }

    pSpiConfig = SPICreateDevCtx();

    if (pSpiConfig == NULL)
    {
        printf ("SPI driver device allocation error\r\n");
        return RC_AD9361_ALLOC_DEV_ERROR;
    }

    pSpiConfig->frameSize       = SPI_FRAME_SIZE_8BITS;
    pSpiConfig->frameFormat     = SPI_MOTOROLA_FORMAT;
    pSpiConfig->clkMode         = SPI_CLOCK_MODE1;
    pSpiConfig->transferMode    = SPI_RX_TX_ENABLE;
    pSpiConfig->frqDivider      = SPI_DIVIDER_SPEED_1M;
    pSpiConfig->CS              = spi_cs;

    rc = SPISelectDevice(pSpiConfig);

    if (FAILED(rc))
    {
        SPIRemoveDevCtx(pSpiConfig);
        pSpiConfig = NULL;
        printf ("SPI driver select device error, rc=%x\r\n", rc);

        return rc;
    }

    SPIDisableCS(SPIGetCtrlRegs (), spi_cs);

    res = Ad9361ReadReg(spi_cs, (!ch) ? 0x1A7 : 0x1A9);

    SPISelectDevice (NULL);
    SPIRemoveDevCtx(pSpiConfig);
    pSpiConfig = NULL;

    return res >> 1;
}

MXRC Ad9361SetFreq(double rxfreq, double txfreq)
{
    UINT32 rc = MX_OK;
    UINT32 i;
    UINT32 spi_cs = SPI_RAD0;
    SPICONFIG * pSpiConfig = NULL;

    UINT32 nAdElmNum = 0;

    if (!SPIIsInit())
    {
        rc = SPIInit (SPI_DEF_DEV_NUM);

        if (FAILED(rc))
        {
            uart_printf ("SPI driver initialization error, rc=%x\r\n", rc);
            Adi9361InitState = rc;
            return rc;
        }
    }

    pSpiConfig = SPICreateDevCtx();

    if (pSpiConfig == NULL)
    {
        uart_printf ("SPI driver device allocation error\r\n");
        Adi9361InitState = RC_AD9361_ALLOC_DEV_ERROR;
        return RC_AD9361_ALLOC_DEV_ERROR;
    }

    pSpiConfig->frameSize       = SPI_FRAME_SIZE_8BITS;
    pSpiConfig->frameFormat     = SPI_MOTOROLA_FORMAT;
    pSpiConfig->clkMode         = SPI_CLOCK_MODE1;
    pSpiConfig->transferMode    = SPI_RX_TX_ENABLE;
    pSpiConfig->frqDivider      = SPI_DIVIDER_SPEED_1M;
    pSpiConfig->CS              = spi_cs;

    rc = SPISelectDevice(pSpiConfig);

    if (FAILED(rc))
    {
        SPIRemoveDevCtx(pSpiConfig);
        pSpiConfig = NULL;
        uart_printf ("SPI driver select device error, rc=%x\r\n", rc);

        Adi9361InitState = rc;
        return rc;
    }


    SPIDisableCS(SPIGetCtrlRegs (), spi_cs);

    // Setup GPIO for SPI
    REG32(GPIO_OE_REG)         |= GPIO_21;
    REG32(GPIO_OUTPUT_REG)     |= GPIO_21;
    REG32(GPIO_PIN_SELECT_REG) |= GPIO_21;

    Ad9361WriteReg(spi_cs, 0x3df, 0x01);
    //
    // Get the revision ID
    //

    Adi9361Rev = 1;		

    nAdElmNum = sizeof(rfregval)/sizeof(rfregval[0]);

    if(rxfreq || txfreq)    
        Ad9361FreqPatch(rfregval, nAdElmNum, rxfreq, txfreq, DEF_RFCLC);            

    for (i = 1; i < nAdElmNum; i++)        	
        Ad9361WriteReg(spi_cs, rfregval[i].Addr, rfregval[i].Data);


    Ad9361ReadReg(spi_cs, RX_RF_PLL_LOCK_STATUS);
    Ad9361ReadReg(spi_cs, TX_RF_PLL_LOCK_STATUS);

    SPISelectDevice (NULL);
    SPIRemoveDevCtx(pSpiConfig);
    pSpiConfig = NULL;

    Ad9361WriteReg(SPI_RAD0, 0x3f5, 0x00);

    Ad9361inited = 1;
    Adi9361InitState = rc;

    return rc;
}


