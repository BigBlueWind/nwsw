/*****************************************************************************
 *                                                                            *
 *       Copyright 2010 by Mindspeed Technologies, Inc.                       *
 *       All Rights Reserved                                                  *
 *                                                                            *
 *       Mindspeed Technologies, Inc.                                         *
 *       4000 MACARTHUR BLVD - EAST TOWER                                     *
 *       NEWPORT BEACH, CA 92660                                              *
 *                                                                            *
 ******************************************************************************
 *       srdsdrv.c                                                            *
 *                                                                            *
 *      SerDes driver                                                        *
 *                                                                            *
 *****************************************************************************/

#include "srdsdrv.h"
#include "i2cdrv.h"
#include "devinfo.h"
#include "cpridrv.h"
#include "hal.h"
#include "mmumgr.h"

#define NEW_SERDES_PARAMS

void SysDelay(UINT32 ms)
{
    UINT32 time = GetTIMETICK();
    while ((GetTIMETICK() - time) < ms*150000);
}

void SerdesGemWriteReg(UINT32 SrdsAddr, UINT32 Val)
{
    PVUINT32 pSrdsReg;
    
    if (DevInfoIsT4Kx6() || DevInfoIsT4Kx7())
        pSrdsReg = (PUINT32)(GEMSRDS_BASEADDR + (SrdsAddr & 0xF000) + 4*(SrdsAddr & 0xFFF));
    else
        pSrdsReg = (PUINT32)(GEMSRDS_BASEADDR + (SrdsAddr & 0xFFFC) + ((SrdsAddr & 3) << 8));
    
    *pSrdsReg = Val;
    
    SysDelay(2);
}

UINT32 SerdesGemReadReg(UINT32 SrdsAddr)
{
    PVUINT32 pSrdsReg;
    
    if (DevInfoIsT4Kx6()||DevInfoIsT4Kx7())
        pSrdsReg = (PUINT32)(GEMSRDS_BASEADDR + (SrdsAddr & 0xF000) + 4*(SrdsAddr & 0xFFF));
    else
        pSrdsReg = (PUINT32)(GEMSRDS_BASEADDR + (SrdsAddr & 0xFFFC) + ((SrdsAddr & 3) << 8));
    
    return *pSrdsReg;
}

void SerdesWriteReg(UINT32 SrdsId, UINT32 SrdsAddr, UINT32 Val)
{
    PVUINT32 pSrdsReg;
    UINT32 SrdsBaseAddr;
    
    if (SrdsId == 0)
        SrdsBaseAddr = RAD_APB_SERDES0_4CH_BASEADDR;
    else if (SrdsId == 1)
        SrdsBaseAddr = RAD_APB_SERDES1_2CH_BASEADDR;
    else if (SrdsId == 2)
        SrdsBaseAddr = RAD_APB_SERDES2_4CH_BASEADDR;
    else
        return;
    
    pSrdsReg = (PUINT32)(SrdsBaseAddr + SrdsAddr);
    *pSrdsReg = Val;
    
    SysDelay(5);
}

UINT32 SerdesReadReg(UINT32 SrdsId, UINT32 SrdsAddr)
{
    PVUINT32 pSrdsReg;
    UINT32 SrdsBaseAddr;
    
    if (SrdsId == 0)
        SrdsBaseAddr = RAD_APB_SERDES0_4CH_BASEADDR;
    else if (SrdsId == 1)
        SrdsBaseAddr = RAD_APB_SERDES1_2CH_BASEADDR;
    else if (SrdsId == 2)
        SrdsBaseAddr = RAD_APB_SERDES2_4CH_BASEADDR;
    else
        return 0;
    
    pSrdsReg = (PUINT32)(SrdsBaseAddr + SrdsAddr);
    
    return *pSrdsReg;
}

void  SerdesGemInit(void)
{
    // reset GEM Serdes 0
    REG32(XP_SRDS_CNTRL) = 1;
    SysDelay(2);
//    REG32(GPIO_SRDS_LANE_CTRL_REG) = 0x0606; // set up 1/8 datarate
    REG32(GPIO_SRDS_PLL_CONF_REG) = 0x001F0027; // x 32, divide by 28 for PCS
    REG32(XP_SRDS_CNTRL) = 0;
    SysDelay(10);
    
    // Set clock control register to external
//    REG32(GPIO_SRDS_LANE_CTRL_REG) = 0x1010;
    // SGMII Serdes MGMT Register
    //  REG(0xFE070068)[1:0] = {lane1, lane0} 
    //  REG(0xFE070068)[5:4] = {MMD}
    REG32(GPIO_SRDS_MGMT_REG) = 0x13; // MMD addrress 1, select Lane 0 and 1

    // Release sgmii serdes reset
    //0xFE0C002C = 0x0

    // Pre-driver loopback
    // 0xFE1A8050 = 0x2
    // 0xFE1A8000 = 0x8006

    SerdesGemWriteReg(0xC000, 0x78DF); // set divider to 32 (0x78DF original)
    SerdesGemWriteReg(0xC001, 0xC440);
    SerdesGemWriteReg(0xC001, 0xC640);
    SerdesGemWriteReg(0x800D, 0x007E);
    SerdesGemWriteReg(0x800E, 0x0C83);
    SerdesGemWriteReg(0x800F, 0x0021);
    SerdesGemWriteReg(0x8010, 0x00ac);
    SerdesGemWriteReg(0x8011, 0x2121); // Duty Cycle control - TX clock select0
    SerdesGemWriteReg(0x8012, 0x2121); // TX Clock Select 1 0x2f00
    SerdesGemWriteReg(0x800C, 0x006f); // CTLE peak-pole
    SerdesGemWriteReg(0xC002, 0x262);
    SerdesGemWriteReg(0xC01E, 0x0340);//0xC2F0
    
    // try to remove 0x8001 reg setting
    REG32(GPIO_SRDS_MGMT_REG) = 0x33; // MMD addrress 3, select Lane 0 and/or 1
    SerdesGemWriteReg(0x8001, 0xB117); // Invert PCS RX CLK, 1/8 rate, adjust for RXn = 2
    SerdesGemWriteReg(0x8009, 0); // release PCS from reset
}

void  Serdes0Init(UINT32 SrdsRate, UINT32 SrdsMode)
{
    // reset Serdes 0
    REG32(RAD_SRDS0_CNTRL) = 0xF1;
    SysDelay(30);
    
    // 156.3 x 32 = 5g - line rate
    
    if (SrdsRate == SRDSRATE_2500)
    {
        // 5g / 20 = 250m - controller rate
        REG32(RAD_SRDS0_INIT_CFG_R0) = 0x000204DF;
        REG32(RAD_SRDS0_INIT_CFG_R1) = 0xAA800000;
        REG32(RAD_SRDS0_INIT_CFG_R2) = (SrdsMode<<18) | 0x0000002A; // 1/4 rate
    }
    else if (SrdsRate == SRDSRATE_5000)
    {
        // 5g / 10 = 500m - controller rate
        REG32(RAD_SRDS0_INIT_CFG_R0) = 0x0000025F;
        REG32(RAD_SRDS0_INIT_CFG_R1) = 0x55400000;
        REG32(RAD_SRDS0_INIT_CFG_R2) = (SrdsMode<<18) | 0x00000015; // 1/2 rate
    }
    else
    {
        uart_printf("%d not supported by Serdes 0 rate\n", SrdsRate);
        return;
     }
    
    SysDelay(10);

    //====================================
    // SERDES-0
    //====================================

    // Program SRDS0/SRIO0 clocks
    REG32(RAD_SRDS0_CONT_CLK_DIV_CNTRL) = 0x82;
    SysDelay(5);
    //!!! TxIf Clock derived from SERDES
    REG32(RAD_SRDS0_TXIF_CLK_DIV_CNTRL) = 0x82;
    SysDelay(5);
    
    REG32(RAD_SRDS0_CNTRL) = 0; // Release serdes0 reset
    SysDelay(5);

    // Serdes TX Config settings
    REG32(RAD_CFG_SRDS_MMD) = 0x1;
    REG32(RAD_CFG_SRDS_LANE) = 0xF;

    SysDelay(5);

    // if x6 or x7 specific code .
    if (!DevInfoIsT4Kx4())
    {
        SerdesWriteReg(0, 0xC004, 0xC440);
        SerdesWriteReg(0, 0xC008, 0x0362);
        SerdesWriteReg(0, 0xC004, 0xC640);
        SerdesWriteReg(0, 0x8034, 0x0046);// RX CTLE Control
        SerdesWriteReg(0, 0x8038, 0x0C83); // TX Config 1
        SerdesWriteReg(0, 0x803C, 0x0021); // TX Config 2
        SerdesWriteReg(0, 0x8040, 0x0086); // TX Pre-emphasis
        SerdesWriteReg(0, 0x8030, 0x006f); // CTLE peak-pole
        SerdesWriteReg(0, 0xC078, 0x1440); //Phase Interpolator
        SerdesWriteReg(0, 0x8004, 0x817F);
        
        if (SrdsRate == SRDSRATE_2500)
        {
            SerdesWriteReg(0, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(0, 0xC008, 0x04E3);
            SerdesWriteReg(0, 0x8000, 0x8002);
        }
        else if (SrdsRate == SRDSRATE_5000)
        {
            SerdesWriteReg(0, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(0, 0xC008, 0x04E3);
            SerdesWriteReg(0, 0x8000, 0x8000);
        }
        }
        else // X1 - X4
        {
            if (SrdsRate == SRDSRATE_2500)
            {
                // PLL clock ready assertion - section 3.2 of bringup doc
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC000) = 0x78DF; // 0x78DF
                MxDelayTicks(150000 * 3);
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC004) = 0xC440;
                MxDelayTicks(150000 * 3);
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC008) = 0x0362;
                MxDelayTicks(150000 * 3);
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC004) = 0xC640;
                MxDelayTicks(150000 * 3);
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8034) = 0x007E; // RX CTLE Control
                MxDelayTicks(150000 * 3);
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8038) = 0x0C83; // TX Config 1
                MxDelayTicks(150000 * 3);

                // Preemp 0xc1 default, 0x9b works better, also 0xac
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8040) = 0x00ac; // TX Pre-emphasis
                MxDelayTicks(150000 * 3);

                SerdesGemWriteReg(RAD_APB_SERDES0_4CH_BASEADDR+0x8044, 0x2828); // Duty Cycle control - TX clock select0
                MxDelayTicks(150000 * 3);
                SerdesGemWriteReg(RAD_APB_SERDES0_4CH_BASEADDR+0x8048, 0x2828); // TX Clock Select 1 0x2f00
                MxDelayTicks(150000 * 3);

                // CTLE peak-pole
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8030) = 0x00ff;
                MxDelayTicks(150000 * 3);

                // Serdes DLL settings - Need this for 3.125GHz
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC008) = 0x262;
                MxDelayTicks(150000 * 3);

                // Phase Interpolator
                REG32(RAD_APB_SERDES0_4CH_BASEADDR+0xC078) = 0xC2F0;
                MxDelayTicks(150000 * 3);
            }
            else if (SrdsRate == SRDSRATE_5000)
            {
            }
    }
    
    REG32(RAD_SRDS0_PCS_PRBS_RESET) = 0;
    SysDelay(5);
}

UINT32 Serdes1IsInited(UINT32 CpriRate, UINT32 nLbMode)
{
    if ((REG32(RAD_SRDS1_CNTRL) & 1) != 0)
        return 0;

    switch (CpriRate)
    {
        case CPRIRATE_1228:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 4 = 1228.8 - line rate
            // 4915.2 / 80 = 61.44 - controller rate
            if (REG32(RAD_SRDS1_TXIF_CLK_DIV_CNTRL) != 2)
                return 0;
            break;
        case CPRIRATE_2457:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 40 = 122.8 - controller rate
            if ((REG32(RAD_SRDS1_INIT_CFG_R0)&0x000009DF) != 0x000009DF)
                return 0;
            break;
        case CPRIRATE_3072:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 20 = 153.6 - controller rate
            if ((REG32(RAD_SRDS1_INIT_CFG_R0)&0x000004D3) != 0x000004D3)
                return 0;
            break;
        case CPRIRATE_4915:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 20 = 245.76 - controller rate
            if ((REG32(RAD_SRDS1_INIT_CFG_R0)&0x000004DF) != 0x000004DF)
                return 0;
            break;
        case CPRIRATE_6144:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 10 = 307.2 - controller rate
            if ((REG32(RAD_SRDS1_INIT_CFG_R0)&0x000004D3) != 0x000004D3)
                return 0;
            break;
    }

    if (nLbMode == 1)
    { // Serdes1 Internal loopback
        if ((REG32(0xFE908000) & (1<<14)) == 0)
            return 0;
    }
    else if (nLbMode == 2)
    { // Pre-driver loopback

        if ((REG32(0xFE908000)&0x0B01)  != 0x0B01)
            return 0;
    }

    return 1;
}

void Serdes1Init(UINT32 CpriRate, UINT32 SrdsMode, UINT32 loopback, UINT32 prbs)
{
    UINT32 prbs_err;
    UINT32 prbs_timer;
    UINT32 saved_pcs_mode;
    UINT32 saved_pcs_prbs_conf;
    UINT32 saved_pcs_reset;

    UINT32 val;
    
    // reset Serdes 1
    REG32(RAD_SRDS1_CNTRL) = 0x31;
    REG32(GPIO_SRDS_LANE_CTRL_REG) = 0x1010;
    
    switch (CpriRate)
    {
        case CPRIRATE_1228:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 4 = 1228.8 - line rate
            // 4915.2 / 80 = 61.44 - controller rate
            REG32(RAD_SRDS1_TXIF_CLK_DIV_CNTRL) = 2; // div 2
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000009DF; // mult 32, div 20
            REG32(RAD_SRDS1_INIT_CFG_R1) = (SrdsMode<<16) | 0xFF; // 1/8 rate
            break;
        case CPRIRATE_2457:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 40 = 122.8 - controller rate
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000009DF;
            REG32(RAD_SRDS1_INIT_CFG_R1) = (SrdsMode<<16) | 0xAA; // 1/4 rate
            break;
        case CPRIRATE_3072:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 20 = 153.6 - controller rate
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000004D3;
            REG32(RAD_SRDS1_INIT_CFG_R1) = (SrdsMode<<16) | 0x55; // 1/2 rate
            break;
        case CPRIRATE_4915:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 20 = 245.76 - controller rate
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000004DF;
            REG32(RAD_SRDS1_INIT_CFG_R1) = (SrdsMode<<16) | 0x55; // 1/2 rate
            break;
        case CPRIRATE_6144:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 10 = 307.2 - controller rate
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000004D3;
            REG32(RAD_SRDS1_INIT_CFG_R1) = (SrdsMode<<16) | 0x00; // full rate
            break;
    }
    // Release serdes1 reset RAD_SRDS1_CNTRL
    REG32(RAD_SRDS1_CNTRL) = 0;
    SysDelay(5);
    
    REG32(RAD_CFG_SRDS_MMD) = 0x1;
    REG32(RAD_CFG_SRDS_LANE) = 3;
    
    // Common initialization for all rates
    SerdesWriteReg(1, 0xC004, 0xC440);
    SerdesWriteReg(1, 0xC008, 0x0362);
    SerdesWriteReg(1, 0xC004, 0xC640);
    SerdesWriteReg(1, 0x8034, 0x0066); // RX CTLE Control
    SerdesWriteReg(1, 0x8038, 0x0C83); // TX Config 1
    SerdesWriteReg(1, 0x803C, 0x0021); // TX Config 2
    
    switch (CpriRate)
    {
        case SRDSRATE_1228:
            SerdesWriteReg(1, 0x8040, 0x00ac); // TX Pre-emphasis
            val = (SerdesReadReg(1, 0x8048) & 0xFFC0) | 0x21;
            SerdesWriteReg(1, 0x8048, val); // TX Clock Select 1
            SerdesWriteReg(1, 0x8030, 0x00ff); // CTLE peak-pole
            SerdesWriteReg(1, 0xC078, (DevInfoIsT4Kx4()) ? 0xC2F0:0x0340); // Phase Interpolator
            SerdesWriteReg(1, 0xC008, 0x262); // CTLE peak-pole
            SerdesWriteReg(1, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(1, 0xC008, 0x04E3);
            SerdesWriteReg(1, 0x8000, 0x8004); // divider
            break;
        case SRDSRATE_2457:
            SerdesWriteReg(1, 0x8040, 0x0086); // TX Pre-emphasis
            SerdesWriteReg(1, 0x8030, 0x008f); // CTLE peak-pole
            SerdesWriteReg(1, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(1, 0xC008, 0x04E3);
            SerdesWriteReg(1, 0xC078, 0x0340); //Phase Interpolator
            SerdesWriteReg(1, 0x8004, 0x817F);
            SerdesWriteReg(1, 0x8000, 0x8002); // divider
            break;
        case SRDSRATE_4915:
            SerdesWriteReg(1, 0x8040, 0x0086); // TX Pre-emphasis
            SerdesWriteReg(1, 0x8030, 0x008f); // CTLE peak-pole
            SerdesWriteReg(1, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(1, 0xC008, 0x04E3);
            SerdesWriteReg(1, 0xC078, 0x0340); //Phase Interpolator
            SerdesWriteReg(1, 0x8004, 0x817F);
            SerdesWriteReg(1, 0x8000, 0x8000); // divider
            break;
        case SRDSRATE_3072:
        case SRDSRATE_6144:
            SerdesWriteReg(1, 0x8040, 0x00ac); // TX Pre-emphasis
            SerdesWriteReg(1, 0x8030, 0x00ff); // CTLE peak-pole
            SerdesWriteReg(1, 0xC008, 0x0262); // CTLE peak-pole
            SerdesWriteReg(1, 0xC078, (DevInfoIsT4Kx4()) ? 0xC2F0:0x0340); // Phase Interpolator
            break;
    }

    if (loopback == 1)
    { // Serdes1 Internal loopback
        REG32(RAD_CFG_SRDS_MMD) = 0x3;   // mmd
        REG32(RAD_CFG_SRDS_LANE) = 3; // broadcast to 1 or 2 lanes
        REG32(0xFE908000) |= 1<<14;
    }
    else if (loopback == 2)
    { // Pre-driver loopback
        REG32(0xFE908050) = 0x2;
        REG32(0xFE908000) = 0x0B01;
    }

    if (prbs)
    { // Change SerDes1 to PRBS mode
        REG32(RAD_CFG_SRDS_MMD) = 3;
        // select lane0
        REG32(RAD_CFG_SRDS_LANE) = 1;

        saved_pcs_mode = REG32(RAD_SRDS1_PCS_MODE);
        saved_pcs_prbs_conf = REG32(RAD_SRDS1_PCS_PRBS_CONFIG);
        saved_pcs_reset = REG32(RAD_SRDS1_PCS_PRBS_RESET);

//      REG32(RAD_SRDS1_PCS_MODE) = 0x36CC; //clock pattern
        REG32(RAD_SRDS1_PCS_MODE) = 0x9CC;// | (1<<14); // PCS Loopback
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = 0xF;

        // pattern
//      REG32(RAD_APB_SERDES1_2CH_BASEADDR + 0xC01C) = 0x1F; // clock

        REG32(RAD_SRDS1_PCS_PRBS_RESET) = 0xFFCF;

        SysDelay(20);

        prbs_timer = (REG32(RAD_SRDS1_PCS_PRBS_TIMER1) << 16) | REG32(RAD_SRDS1_PCS_PRBS_TIMER2);
        prbs_err = (REG32(RAD_SRDS1_PCS_PRBS_ERRCNT0) << 16) | REG32(RAD_SRDS1_PCS_PRBS_ERRCNT1);
        uart_printf("Serdes1 Lane0: prbs_timer=%d  prbs_err=%d\r\n", prbs_timer, prbs_err);

        REG32(RAD_SRDS1_PCS_MODE) = saved_pcs_mode;
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = saved_pcs_prbs_conf;
        REG32(RAD_SRDS1_PCS_PRBS_RESET) = saved_pcs_reset;

        REG32(RAD_CFG_SRDS_MMD) = 3;
        // select lane1
        REG32(RAD_CFG_SRDS_LANE) = 2;

        saved_pcs_mode = REG32(RAD_SRDS1_PCS_MODE);
        saved_pcs_prbs_conf = REG32(RAD_SRDS1_PCS_PRBS_CONFIG);
        saved_pcs_reset = REG32(RAD_SRDS1_PCS_PRBS_RESET);

        REG32(RAD_SRDS1_PCS_MODE) = 0x9CC;
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = 0xF;
        REG32(RAD_SRDS1_PCS_PRBS_RESET) = 0xFFCF;

        SysDelay(20);

        prbs_timer = (REG32(RAD_SRDS1_PCS_PRBS_TIMER1) << 16) | REG32(RAD_SRDS1_PCS_PRBS_TIMER2);
        prbs_err = (REG32(RAD_SRDS1_PCS_PRBS_ERRCNT0) << 16) | REG32(RAD_SRDS1_PCS_PRBS_ERRCNT1);

//        uart_printf("Serdes1 Lane1: prbs_timer=%d  prbs_err=%d\r\n", prbs_timer, prbs_err);

        REG32(RAD_SRDS1_PCS_MODE) = saved_pcs_mode;
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = saved_pcs_prbs_conf;
        REG32(RAD_SRDS1_PCS_PRBS_RESET) = saved_pcs_reset;
    }
    
}

void Serdes2Init(UINT32 SrdsRate, UINT32 SrdsMode)
{
    UINT32 val;
    
    REG32(GP_CONFIG) &= ~(3 << 4);
    
    if (SrdsRate != SRDSRATE_2500 && SrdsRate != SRDSRATE_5000)
        REG32(GP_CONFIG) |= 1 << 4;
//        printf("GP_CONFIG=%x\n", REG32(GP_CONFIG));
    
    // reset Serdes 2
    REG32(RAD_SRDS2_CNTRL) = 0xF1;
    
    switch (SrdsRate)
    {
        case SRDSRATE_2500:
            // 5g / 20 = 250m - controller rate
            REG32(RAD_SRDS2_INIT_CFG_R0) = 0x000204DF;
            REG32(RAD_SRDS2_INIT_CFG_R1) = 0xAA800000;
            REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode<<18) | 0x0000002A; // 1/4 rate
            break;
        case SRDSRATE_5000:
            // 5g / 10 = 500m - controller rate
            REG32(RAD_SRDS2_INIT_CFG_R0) = 0x0000025F;
            REG32(RAD_SRDS2_INIT_CFG_R1) = 0x55400000;
            REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode<<18) | 0x00000015; // 1/2 rate
            break;
        case CPRIRATE_1228:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 4 = 1228.8 - line rate
            // 4915.2 / 80 = 61.44 - controller rate
            REG32(RAD_SRDS2_TXIF_CLK_DIV_CNTRL) = 2; // div 2
            REG32(RAD_SRDS2_INIT_CFG_R0) = 0x000019DF; // mult 32, div 20
            REG32(RAD_SRDS2_INIT_CFG_R1) = 0xFFC00000;
            REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode<<16) | 0x3F; // 1/8 rate
            break;
        case CPRIRATE_2457:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 40 = 122.8 - controller rate
            REG32(RAD_SRDS2_INIT_CFG_R0) = 0x000019DF;
            REG32(RAD_SRDS2_INIT_CFG_R1) = 0xAA800000;
            REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode<<16) | 0x2A; // 1/4 rate
            break;
        case CPRIRATE_3072:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 20 = 153.6 - controller rate
            REG32(RAD_SRDS2_INIT_CFG_R0) = 0x000014D3;
            REG32(RAD_SRDS2_INIT_CFG_R1) = 0x55400000;
            REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode<<16) | 0x55; // 1/2 rate
            break;
        case CPRIRATE_4915:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 20 = 245.76 - controller rate
            REG32(RAD_SRDS2_INIT_CFG_R0) = 0x000014DF;
            REG32(RAD_SRDS2_INIT_CFG_R1) = 0x55400000;
            REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode<<16) | 0x55; // 1/2 rate
            break;
        case CPRIRATE_6144:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 10 = 307.2 - controller rate
            REG32(RAD_SRDS2_INIT_CFG_R0) = 0x000014D3;
            REG32(RAD_SRDS2_INIT_CFG_R1) = 0x00000000;
            REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode<<16) | 0x00; // full rate
            break;
    }
    
    // Program SRDS2 clocks
//    REG32(RAD_SRDS2_CONT_CLK_DIV_CNTRL) = 0x82;

    //!!! TxIf Clock derived from SERDES
//    REG32(RAD_SRDS2_TXIF_CLK_DIV_CNTRL) = 0x82;

//    SysDelay(10);
    
    REG32(RAD_SRDS2_CNTRL) = 0; // Release serdes2 reset

    SysDelay(100);

    REG32(RAD_CFG_SRDS_MMD) = 0x1;
    REG32(RAD_CFG_SRDS_LANE) = 0xF;
    SysDelay(5);
    
    // Common initialization for all rates
    SerdesWriteReg(2, 0xC004, 0xC440);
    SerdesWriteReg(2, 0xC008, 0x0362);
    SerdesWriteReg(2, 0xC004, 0xC640);
    SerdesWriteReg(2, 0x8034, 0x0066); // RX CTLE Control
    SerdesWriteReg(2, 0x8038, 0x0C83); // TX Config 1
    SerdesWriteReg(2, 0x803C, 0x0021); // TX Config 2
    
    switch (SrdsRate)
    {
        case SRDSRATE_2500:
            SerdesWriteReg(2, 0x8040, 0x0086); // TX Pre-emphasis
            SerdesWriteReg(2, 0x8030, 0x006f); // CTLE peak-pole
            SerdesWriteReg(2, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(2, 0xC008, 0x04E3);
            SerdesWriteReg(2, 0xC078, 0x1440); //Phase Interpolator
            SerdesWriteReg(2, 0x8004, 0x817F);
            SerdesWriteReg(2, 0x8000, 0x8002);
            break;
        case SRDSRATE_5000:
            SerdesWriteReg(2, 0x8040, 0x0086); // TX Pre-emphasis
            SerdesWriteReg(2, 0x8030, 0x006f); // CTLE peak-pole
            SerdesWriteReg(2, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(2, 0xC008, 0x04E3);
            SerdesWriteReg(2, 0xC078, 0x1440); //Phase Interpolator
            SerdesWriteReg(2, 0x8004, 0x817F);
            SerdesWriteReg(2, 0x8000, 0x8000);
            break;
        case SRDSRATE_1228:
            SerdesWriteReg(2, 0x8040, 0x00ac); // TX Pre-emphasis
            val = (SerdesReadReg(2, 0x8048) & 0xFFC0) | 0x21;
            SerdesWriteReg(2, 0x8048, val); // TX Clock Select 1
            SerdesWriteReg(2, 0x8030, 0x00ff); // CTLE peak-pole
            SerdesWriteReg(2, 0xC078, (DevInfoIsT4Kx4()) ? 0xC2F0:0x0340); // Phase Interpolator
            SerdesWriteReg(2, 0xC008, 0x262); // CTLE peak-pole
            SerdesWriteReg(2, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(2, 0xC008, 0x04E3);
            SerdesWriteReg(2, 0x8000, 0x8004); // divider
            break;
        case SRDSRATE_2457:
            SerdesWriteReg(2, 0x8040, 0x0086); // TX Pre-emphasis
            SerdesWriteReg(2, 0x8030, 0x008f); // CTLE peak-pole
            SerdesWriteReg(2, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(2, 0xC008, 0x04E3);
            SerdesWriteReg(2, 0xC078, 0x0340); //Phase Interpolator
            SerdesWriteReg(2, 0x8004, 0x817F);
            SerdesWriteReg(2, 0x8000, 0x8002); // divider
            break;
        case SRDSRATE_4915:
            SerdesWriteReg(2, 0x8040, 0x0086); // TX Pre-emphasis
            SerdesWriteReg(2, 0x8030, 0x008f); // CTLE peak-pole
            SerdesWriteReg(2, 0xC074, 0x0010); // DLL bypass
            SerdesWriteReg(2, 0xC008, 0x04E3);
            SerdesWriteReg(2, 0xC078, 0x0340); //Phase Interpolator
            SerdesWriteReg(2, 0x8004, 0x817F);
            SerdesWriteReg(2, 0x8000, 0x8000); // divider
            break;
        case SRDSRATE_3072:
        case SRDSRATE_6144:
            SerdesWriteReg(2, 0x8040, 0x00ac); // TX Pre-emphasis
            SerdesWriteReg(2, 0x8030, 0x00ff); // CTLE peak-pole
            SerdesWriteReg(2, 0xC008, 0x0262); // CTLE peak-pole
            SerdesWriteReg(2, 0xC078, (DevInfoIsT4Kx4()) ? 0xC2F0:0x0340); // Phase Interpolator
            break;
    }
    
    REG32(RAD_SRDS2_PCS_PRBS_RESET) = 0;
    SysDelay(5);
}

UINT32 SfpWrite(UINT32 cmd, UINT32 addr, UINT8 Byte0, UINT8 Byte1)
{
    UINT32 rc;
    UINT32 status;
    
    // Start write command
    rc = I2CStartXmit();
    if (rc != MX_OK)
        return rc;
    
    status = REG32(I2C_STAT_REG);

    if (status == I2C_START_TRANSMIT || status == I2C_REPEAT_START_TRANSMIT)
    {
        REG32(I2C_DATA_REG) = cmd;
        REG32(I2C_CNTR_REG) &= ~I2C_IFLG;       
    }
    else
        return RC_SERDES_DEV_TIMEOUT;
    
    if (I2CWaitForIFlag() == 0)
        return RC_SERDES_DEV_TIMEOUT;
    
    //check what's returned, if acked then continue with dummy write
    status = REG32(I2C_STAT_REG);
    
    if (status != I2C_ADDRESS_W_ACK)
        return I2C_RC_SET_MASTER_ERROR;

    // send address
    rc = I2CXmitByte(addr);
    if (rc != MX_OK)
        return rc;

    //check what's returned, if acked then continue with dummy write
    status = REG32(I2C_STAT_REG);

    rc = I2CXmitByte(Byte0);
    if (rc != MX_OK)
        return rc;
    
    rc = I2CXmitByte(Byte1);
    if (rc != MX_OK)
        return rc;
    
    REG32(I2C_CNTR_REG) = I2C_STP;
    
    SysDelay(2); 
    
    return MX_OK;
}

UINT32 SrdsSfpCtrl(UINT32 SrdsId, UINT32 SfpEnableMask)
{
    UINT8 cmd, Byte0, Byte1;
    
    if (SrdsId == 1)
    {
        cmd = I2C_PCA_OPCODE | (I2CSRDS1SFP_0_1_ADDR << 1) | I2C_WRITE;
        Byte0 = (SfpEnableMask & 1) ? 0 : 2;
        Byte1 = (SfpEnableMask & 2) ? 0 : 2;
        if (SfpWrite(cmd, 6, ~2, ~2) != MX_OK)
        {
            uart_printf("Failed to access Serdes1 SFP via I2C\n");
        }
        else if (SfpWrite(cmd, 2, Byte0, Byte1) != MX_OK)
            uart_printf("Failed to access Serdes1 SFP via I2C\n");
    }
    else if (SrdsId == 2)
    {
        cmd = I2C_PCA_OPCODE | (I2CSRDS2SFP_0_1_ADDR << 1) | I2C_WRITE;
        Byte0 = (SfpEnableMask & 1) ? 0 : 2;
        Byte1 = (SfpEnableMask & 2) ? 0 : 2;
        if (SfpWrite(cmd, 6, ~2, ~2) != MX_OK)
        {
            uart_printf("Failed to access Serdes2 SFP_0_1 via I2C\n");
        }
        else if (SfpWrite(cmd, 2, Byte0, Byte1) != MX_OK)
            uart_printf("Failed to access Serdes1 SFP_0_1 via I2C\n");
        
        cmd = I2C_PCA_OPCODE | (I2CSRDS2SFP_2_3_ADDR << 1) | I2C_WRITE;
        Byte0 = (SfpEnableMask & 1) ? 0 : 2;
        Byte1 = (SfpEnableMask & 2) ? 0 : 2;
        if (SfpWrite(cmd, 6, ~2, ~2) != MX_OK)
        {
            uart_printf("Failed to access Serdes2 SFP_2_3 via I2C\n");
        }
        else if (SfpWrite(cmd, 2, Byte0, Byte1) != MX_OK)
            uart_printf("Failed to access Serdes1 SFP_2_3 via I2C\n");
    }
    else
        return RC_SRDES_HW_ID_ERROR;

    return MX_OK;
}

void SrdsMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (nCpuID == 0)
    {
        MmuMgrCfgMemAP(I2C_BASEADDR,            4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_CLKRST_BASEADDR, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_CONFIG_BASEADDR, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);

        MmuMgrCfgMemAP(RAD_APB_SERDES0_4CH_BASEADDR, 100*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_SERDES1_2CH_BASEADDR, 100*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_SERDES2_4CH_BASEADDR, 100*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}

