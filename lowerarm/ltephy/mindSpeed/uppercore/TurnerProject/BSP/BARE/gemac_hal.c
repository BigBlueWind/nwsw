/** @file gemac_hal.c
 *
 * @brief GEMAC HAL
 * @author Cadence Design Foundry
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * COPYRIGHT(c) 2003 Cadence Design Foundry
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "gemac_hal.h"

/* Some functions to set/reset and get specific bits in the MAC registers
 * Note that all functions operate on a read-modify-write basis
 */

/**
 * Function to start transmission on the specified device.  The parameter to
 * this function is simply a pointer to the GEM_DEVICE structure.
 * This function should be called after the relevant queues and data has been
 * set up, however it will check if the number of queue elements is zero first.
 * Note that this function will also enable tx even if it was previously not set
 *
 * Return value:
 *  0   :   OK
 *  -1  :   Transmit queue not valid.
 */
RESULTCODE GemStartTx (GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_TX_START | GEM_TX_EN;
    return SUCCESS;
}

/**
 * Halt transmission after current frame has completed.  This is accomplished
 * simply by writing to the GEM_TX_HALT bit in the network control register,
 * which should cause the MAC to complete its current frame then stop.
 *
 * There is no return value for this function.
 */
void GemStopTx(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_TX_HALT;
}

/**
 * Abort transmission immediately WITHOUT waiting for completion of any current
 * frame.
 * Note that after this operation, the transmit buffer descriptor will be reset
 * to point to the first buffer in the descriptor list!
 *
 * There is no return value.
 */
void GemAbortTx(GEM_DEVICE *mac)
{
    mac->registers->net_control &= (~GEM_TX_EN);
}

/**
 * Obtains status of transmission circuitry, whether it is transmitting or idle.
 *
 * Return value:
 *  0   :   Transmitter is idle.
 *  1   :   Transmitter active.
 */
int GemTransmitting(GEM_DEVICE *mac)
{
    return ((mac->registers->tx_status & GEM_TX_GO) == GEM_TX_GO);
}

/**
 * Enable the receive circuitry.  This should be performed only after the
 * buffers and the descriptor queues have been set up, otherwise unpredictable
 * results may occur.
 *
 * Return value:
 *  0   :   OK
 *  -1  :   Receive queue not valid.
 */
RESULTCODE GemEnableRx(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_RX_EN;
    return SUCCESS;
}

/**
 * Disable the receive circuitry.  This will stop reception of any frame
 * immediately, note that the descriptor pointer is not changed.
 *
 * There is no return value for this function.
 */
void GemDisableRx(GEM_DEVICE *mac)
{
    mac->registers->net_control &= (~GEM_RX_EN);
}

/**
 * Obtain the status of the receive circuitry, whether it is enabled.
 *
 * Return value:
 *  0   :   Receive circuitry disabled.
 *  -1  :   Receive circuits enabled.
 */
int GemReceiveOn(GEM_DEVICE *mac)
{
    return ((mac->registers->net_control & GEM_RX_EN) == GEM_RX_EN);
}

/**
 * Set the loopback mode of the MAC.  This can be either no loopback for normal
 * operation, local loopback through MAC internal loopback module or PHY
 * loopback for external loopback through a PHY.  This asserts the external loop
 * pin.
 * The function parameters are a pointer to the device and an enumerated type
 * specifying the type of loopback required.
 *
 * Note: if an invalid loopback mode is specified, loop back will be disabled.
 *
 * @param mac Pointer to the device structure
 * @param gem_loop Loopback mode
 */
void GemSetLoop(GEM_DEVICE *mac, MAC_LOOP gem_loop)
{
    switch (gem_loop)
    {
    case LB_LOCAL:
        mac->registers->net_control &= (~GEM_LB_PHY);
        mac->registers->net_control |= (GEM_LB_MAC);
        break;
    case LB_EXT:
        mac->registers->net_control &= (~GEM_LB_MAC);
        mac->registers->net_control |= (GEM_LB_PHY);
        break;
    default:
        mac->registers->net_control &= (~(GEM_LB_MAC | GEM_LB_PHY));
    }
}

/**
 * Get the loopback mode of the MAC.  This can be either no loopback for normal
 * operation, local loopback through MAC internal loopback module or PHY
 * loopback for external loopback through a PHY.  This asserts the external loop
 * pin.
 * The function parameters are a pointer to the device.
 *
 * Return value:
 *  LB_LOCAL    :   Local loop back active.
 *  LB_EXT      :   External loop back active.
 *  LB_NONE     :   Loop back disabled (unknown).
 */
MAC_LOOP GemGetLoop(GEM_DEVICE *mac)
{
    U32 lb_mode = mac->registers->net_control & (GEM_LB_PHY | GEM_LB_MAC);

    switch (lb_mode)
    {
    case GEM_LB_MAC:
        return LB_LOCAL;
    case GEM_LB_PHY:
        return LB_EXT;
    default:
        return LB_NONE;
    }
}

/**
 * Activate the Management interface.  This is required to program the PHY
 * registers through the MDIO port.
 *
 * There is no return value for this function.
 */
void GemEnableMdio(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_MDIO_EN;
}

/**
 * Disable the Management interface.  In this state, the MDIO is placed in a
 * high impedance state and the MDC clock is driven low.
 *
 * There is no return value for this function.
 */
void GemDisableMdio(GEM_DEVICE *mac)
{
    mac->registers->net_control &= ~GEM_MDIO_EN;
}

/**
 * Enable writing to the statistic registers.  This is for debug purposes only
 * and should not be active during normal operation.
 *
 * There is no return value for this function.
 */
void GemStatsWrEn(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_STATS_WR_EN;
}

/**
 * Disable writing to the statistic registers.  Under normal operation this is
 * not necessary as the writing to statistics registers should be off by default
 *
 * There is no return value for this function.
 */
void GemStatsWrOff(GEM_DEVICE *mac)
{
    mac->registers->net_control &= (~GEM_STATS_WR_EN);
}

/**
 * Increment all the statistic registers by 1.  This is for debug purposes only.
 * Note that the statistic registers are automatically cleared on read!
 *
 * No return value.
 */
void GemStatsInc(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_STATS_INC;
}

/**
 * Clear all the statistic registers.
 *
 * No return value.
 */
void GemStatsClr(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_STATS_CLR;
}

/**
 * Enable pause frame reception.  With this enabled, if a valid pause frame is
 * received, transmission will halt for the specified time after the current
 * frame has completed transmission.
 *
 * There is no return value for this function.
 */
void GemEnablePauseRx(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_RX_PAUSE_EN;
}

/**
 * Disable pause frame reception.  Incoming pause frames are ignored.
 *
 * There is no return value for this function.
 */
void GemDisablePauseRx(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_RX_PAUSE_EN);
}

/**
 * Enable copy of received pause frame.
 *
 * There is no return value for this function.
 */
void GemEnablePauseCpy(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_RX_NO_PAUSE);
}

/**
 * Disable copy of received pause frame.
 *
 * There is no return value for this function.
 */
void GemDisablePauseCpy(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_RX_NO_PAUSE;
}

/**
 * Send a pause frame with zero quantum.
 *
 * There is no return value for this function.
 */
void GemSend0qPause(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_TX_0Q_PAUSE;
}

/**
 * Send a normal pause frame.
 *
 * There is no return value for this function.
 */
void GemSendPause(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_TX_PAUSE;
}

/**
 * Set transmit pause quantum.
 *
 * There is no return value for this function.
 */
void GemSetTxPauseQ(GEM_DEVICE *mac, U32 gem_pause)
{
    mac->registers->tx_pause_quant = gem_pause;
}

/**
 * Read transmit pause quantum.
 *
 * Return values:
 * 0-65535: transmit pause quantum.
 */
U32 GemGetTxPauseQ(GEM_DEVICE *mac)
{
    return mac->registers->tx_pause_quant;
}

/**
 * Set retry test bit, this is used for debug purposes only to speed up testing.
 * This should not be enabled for normal operation.
 *
 * There is no return value for this function.
 */
void GemEnRetryTest(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_RETRY_TEST;
}

/**
 * Disable retry test bit.  For normal operation this bit should not be set.
 *
 * There is no return value for this function.
 */
void GemDisRetryTest(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_RETRY_TEST);
}

/**
 * Enable external address match via the eam pin, which when active will copy
 * the current frame to memory.
 *
 * There is no return value for this function.
 */
void GemEnableEam(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_EAM_EN;
}

/**
 * Disable external address match capability.  The MAC will ignore the status of
 * the eam pin.
 *
 * There is no return value for this function.
 */
void GemDisableEam(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_EAM_EN);
}

/**
 * Enable storing of the receive frame check sequence into memory.
 *
 * There is no return value for this function.
 */
void GemEnableFcsRx(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_RX_NO_FCS);
}

/**
 * Disable storing of the receive frame check sequence into memory.  The last 4
 * bytes from the incoming frame will be checked for valid CRC, however this
 * will not be stored into memory.  The frame length will be updated
 * accordingly.
 *
 * There is no return value for this function.
 */
void GemDisableFcsRx(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_RX_NO_FCS;
}

/**
 * Enable reception of long frames up to 1536 bytes in length.
 * These are not standard IEEE 802.3 frames.
 *
 * There is no return value for this function.
 */
void GemEnable1536Rx(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_FRAME_1536;
}

/**
 * Disable reception of frames greater than 1518 bytes in length.
 * This is normal operation mode for the MAC for compatibility with IEEE 802.3
 *
 * There is no return value for this function.
 */
void GemDisable1536Rx(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_FRAME_1536);
}

/**
 * Enable reception of unicast hashed frames.  The frame will be received when
 * the 6 bit hash function of the frame's destination address points a bit that
 * is set in the 64-bit hash register and is signalled as a unicast frame.
 *
 * There is no return value for this function.
 */
void GemEnableUnicast(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_UNICAST_EN;
}

/**
 * Disable reception of unicast hashed frames.
 *
 * There is no return value for this function.
 */
void GemDisableUnicast(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_UNICAST_EN);
}

/**
 * Enable reception of multicast hashed frames.  The frame will be received when
 * the 6 bit hash function of the frame's destination address points a bit that
 * is set in the 64-bit hash register and is signalled as a multicast frame.
 *
 * There is no return value for this function.
 */
void GemEnableMulticast(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_MULTICAST_EN;
}

/**
 * Disable reception of multicast hashed frames.
 *
 * There is no return value for this function.
 */
void GemDisableMulticast(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_MULTICAST_EN);
}

/**
 * Allow reception of broadcast frames (frames with address set to all 1's)
 * This is normal operating mode for the MAC.
 *
 * There is no return value for this function.
 */
void GemAllowBroadcast(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_NO_BROADCAST);
}

/**
 * Do not allow reception of broadcast frames, such frames will be ignored.
 *
 * There is no return value for this function.
 */
void GemNoBroadcast(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_NO_BROADCAST;
}

/**
 * Enable copy all frames.  In this mode, the MAC will copy all valid received
 * frames to memory regardless of the destination address.
 *
 * There is no return value for this function.
 */
void GemEnableCopyAll(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_COPY_ALL;
}

/**
 * Do not copy all frames.  Normal operating mode for the MAC, frames will only
 * be copied to memory if it matches one of the specific or hash addresses.
 *
 * There is no return value for this function.
 */
void GemDisableCopyAll(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_COPY_ALL);
}

/**
 * Set MAC into full duplex mode.  The crs and col signals will be ignored in
 * this mode.
 *
 * There is no return value for this function.
 */
void GemFullDuplex(GEM_DEVICE *mac)
{
    // mac->registers->net_config |= GEM_FULL_DUPLEX;
    *mac->config |= GEM_CONF_DUPLEX_GEM_FULL;
}

/**
 * Set MAC into half duplex mode.  The crs and col signals are used to detect
 * collisions and perform deference where necessary.
 *
 * There is no return value for this function.
 */
void GemHalfDuplex(GEM_DEVICE *mac)
{
    // mac->registers->net_config &= (~GEM_FULL_DUPLEX);
    *mac->config &= ~GEM_CONF_DUPLEX_GEM_FULL;
}

/**
 * Set the operating speed of the MAC, for 10 and 100Mb modes, this has no
 * effect on the MAC functionality, but simply asserts an external speed pin
 * accordingly.
 * For 1G modes, this will set the MAC into the appropriate operating mode by
 * switching to either the GMII or TBI interface depending on required mode.
 *
 * There is no return value for this function.
 */
void GemSetSpeed(GEM_DEVICE *mac, MAC_SPEED gem_speed)
{
    switch (gem_speed)
    {
    case SPEED_10M:
        *mac->config = (*(mac->config) & ~GEM_CONF_SPEED_MASK)
            | GEM_CONF_SPEED_SEL_GEM | GEM_CONF_SPEED_GEM_10M;
        // mac->registers->net_config &= (~GEM_SPEED_100);
        // mac->registers->net_config &= (~GEM_GIG_MODE);
        mac->registers->net_config &= (~GEM_PCS_SEL);
        break;
    case SPEED_100M:
        *mac->config = (*(mac->config) & ~GEM_CONF_SPEED_MASK)
            | GEM_CONF_SPEED_SEL_GEM | GEM_CONF_SPEED_GEM_100M;
        // mac->registers->net_config |= GEM_SPEED_100;
        // mac->registers->net_config &= (~GEM_GIG_MODE);
        mac->registers->net_config &= (~GEM_PCS_SEL);
        break;
    case SPEED_1000M:
        *mac->config = (*(mac->config) & ~GEM_CONF_SPEED_MASK)
            | GEM_CONF_SPEED_SEL_GEM | GEM_CONF_SPEED_GEM_1G;
        // mac->registers->net_config &= (~GEM_SPEED_100);
        // mac->registers->net_config |= GEM_GIG_MODE;
        mac->registers->net_config &= (~GEM_PCS_SEL);
        break;
    case SPEED_1000M_PCS:
        *mac->config = (*(mac->config) & ~GEM_CONF_SPEED_MASK)
            | GEM_CONF_SPEED_SEL_GEM | GEM_CONF_SPEED_GEM_1G;
        // mac->registers->net_config &= (~GEM_SPEED_100);
        // mac->registers->net_config |= GEM_GIG_MODE;
        mac->registers->net_config |= GEM_PCS_SEL;
        break;
    default:
        *mac->config = (*(mac->config) & ~GEM_CONF_SPEED_MASK)
            | GEM_CONF_SPEED_SEL_GEM | GEM_CONF_SPEED_GEM_100M;
        // mac->registers->net_config |= GEM_SPEED_100;
        // mac->registers->net_config &= (~GEM_GIG_MODE);
        mac->registers->net_config &= (~GEM_PCS_SEL);
    }
}

/**
 * Get the operating speed of the MAC, currently this has no functional effect
 * on the MAC.
 *
 * This function returns an enumerated value cast into an int.  This is for
 * backward compatibility with the macb driver.
 *
 * Return value:
 *  SPEED_10M   :   MAC in 10Mb/s mode.
 *  SPEED_100M  :   MAC in 100Mb/s mode.
 *  SPEED_1000M :   MAC in 1G mode with GMII interface.
 *  SPEED_1000M_PCS :   MAC in 1G mode with PCS interface.
 */
MAC_SPEED GemGetSpeed(GEM_DEVICE *mac)
{

    if ( *(mac->config) & GEM_CONF_SPEED_SEL_GEM)
    {
        if ( *(mac->config) & GEM_CONF_SPEED_GEM_1G)
        {
            if (mac->registers->net_config & GEM_PCS_SEL)
                return (SPEED_1000M_PCS);
            else
                return (SPEED_1000M);
        }
        else
        {
            if ( *(mac->config) & GEM_CONF_SPEED_GEM_100M)
                return (SPEED_100M);
            else
                return (SPEED_10M);
        }
    }
    else
    {
        if ( *(mac->config) & GEM_CONF_SPEED_PHY_1G)
        {
            if (mac->registers->net_config & GEM_PCS_SEL)
                return (SPEED_1000M_PCS);
            else
                return (SPEED_1000M);
        }
        else
        {
            if ( *(mac->config) & GEM_CONF_SPEED_PHY_100M)
                return (SPEED_100M);
            else
                return (SPEED_10M);
        }
    }
}

/**
 * Set AMBA AHB bus width.
 * AMBA_AHB_32:
 * AMBA_AHB_64:
 * AMBA_AHB_128:
 * There is no return value for this function.
 */
void GemSetAhbWidth(GEM_DEVICE *mac, MAC_AHB_WIDTH gem_buswidth)
{
    switch(gem_buswidth)
    {
    case AMBA_AHB_128:
        mac->registers->net_config |= GEM_AHB_WIDTH1;
        break;
    case AMBA_AHB_64:
        mac->registers->net_config &= (~GEM_AHB_WIDTH1);
        mac->registers->net_config |= GEM_AHB_WIDTH0;
        break;
    case AMBA_AHB_32:
    default:
        mac->registers->net_config &= ((~GEM_AHB_WIDTH1) & (~GEM_AHB_WIDTH0));
    }
}

/**
 * Get AMBA AHB bus width.
 * It returns one of the following values:
 * AMBA_AHB_32, AMBA_AHB_64, AMBA_AHB_128,
 */
MAC_AHB_WIDTH GemGetAhbWidth(GEM_DEVICE *mac)
{
    if (mac->registers->net_config & GEM_AHB_WIDTH1)
    {
        return AMBA_AHB_128;
    }
    else
    {
        if (mac->registers->net_config & GEM_AHB_WIDTH0)
        {
            return AMBA_AHB_64;
        }
        else
        {
            return AMBA_AHB_32;
        }
    }
}

/**
 * Set MDC clock division.
 * MDC_DIV_8:
 * MDC_DIV_16:
 * MDC_DIV_32:
 * MDC_DIV_48:
 * MDC_DIV_64:
 * MDC_DIV_96:
 * MDC_DIV_128:
 * MDC_DIV_224:
 * There is no return value for this function.
 */
void GemSetMdcDiv(GEM_DEVICE *mac, MAC_MDC_DIV gem_mdcdiv)
{
    switch(gem_mdcdiv)
    {
    case MDC_DIV_8:
        mac->registers->net_config &=
        ((~GEM_MDC_DIV2) & (~GEM_MDC_DIV1) & (~GEM_MDC_DIV0));
        break;
    case MDC_DIV_16:
        mac->registers->net_config &= ((~GEM_MDC_DIV2) & (~GEM_MDC_DIV1));
        mac->registers->net_config |= GEM_MDC_DIV0;
        break;
    case MDC_DIV_32:
        mac->registers->net_config &= ((~GEM_MDC_DIV2) & (~GEM_MDC_DIV0));
        mac->registers->net_config |= GEM_MDC_DIV1;
        break;
    case MDC_DIV_48:
        mac->registers->net_config |= (GEM_MDC_DIV1 | GEM_MDC_DIV0);
        mac->registers->net_config &= (~GEM_MDC_DIV2);
        break;
    case MDC_DIV_64:
        mac->registers->net_config &= ((~GEM_MDC_DIV1) & (~GEM_MDC_DIV0));
        mac->registers->net_config |= GEM_MDC_DIV2;
        break;
    case MDC_DIV_96:
        mac->registers->net_config |= (GEM_MDC_DIV2 | GEM_MDC_DIV0);
        mac->registers->net_config &= (~GEM_MDC_DIV1);
        break;
    case MDC_DIV_128:
        mac->registers->net_config |= (GEM_MDC_DIV2 | GEM_MDC_DIV1);
        mac->registers->net_config &= (~GEM_MDC_DIV0);
        break;
    case MDC_DIV_224:
        mac->registers->net_config |=
        (GEM_MDC_DIV2 | GEM_MDC_DIV1 |GEM_MDC_DIV0);
        break;
    }
}

/**
 * Get MDC clock division.
 * It returns one of this values:
 * MDC_DIV_8, MDC_DIV_16, MDC_DIV_32, MDC_DIV_48, MDC_DIV_64, MDC_DIV_96,
 * MDC_DIV_128, MDC_DIV_224.
 */
MAC_MDC_DIV GemGetMdcDiv(GEM_DEVICE *mac)
{
    if (mac->registers->net_config & GEM_MDC_DIV2)
    {
        if (mac->registers->net_config & GEM_MDC_DIV1)
        {
            if (mac->registers->net_config & GEM_MDC_DIV0)
            {
                return MDC_DIV_224;
            }
            else
            {
                return MDC_DIV_128;
            }
        }
        else
        {
            if (mac->registers->net_config & GEM_MDC_DIV0)
            {
                return MDC_DIV_96;
            }
            else
            {
                return MDC_DIV_64;
            }
        }
    }
    else
    {
        if (mac->registers->net_config & GEM_MDC_DIV1)
        {
            if (mac->registers->net_config & GEM_MDC_DIV0)
            {
                return MDC_DIV_48;
            }
            else
            {
                return MDC_DIV_32;
            }
        }
        else
        {
            if (mac->registers->net_config & GEM_MDC_DIV0)
            {
                return MDC_DIV_16;
            }
            else
            {
                return MDC_DIV_8;
            }
        }
    }
}

/**
 * Enable to read snapshot values of statistic registers.
 *
 * There is no return value for this function.
 */
void GemEnableRdSnap(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_READ_SNAP;
}

/**
 * Disable to read snapshot values of statistic registers.
 *
 * There is no return value for this function.
 */
void GemDisableRdSnap(GEM_DEVICE *mac)
{
    mac->registers->net_control &= (~GEM_READ_SNAP);
}

/**
 * Take snapshot of statistic registers. Writing a one will record the current
 * value of all statistics registers in the snapshot registers and clear the
 * statistics registers.
 *
 * There is no return value for this function.
 */
void GemTakeSnap(GEM_DEVICE *mac)
{
    mac->registers->net_control |= GEM_TAKE_SNAP;
}

/**
 * Get the current link status as returned by the PHY
 *
 * Return value:
 *  0   :   Link is down.
 *  1   :   Link active.
 */
int GemLinkStatus(GEM_DEVICE *mac)
{
    return ((mac->registers->net_status & GEM_LINK_STATUS) == GEM_LINK_STATUS);
}

/**
 * Check if the PHY management logic is idle, i.e. completed management
 * operation.
 *
 * Return value:
 *  0   :   PHY management not-idle.
 *  1   :   PHY management completed.
 */
int GemPhyManIdle(GEM_DEVICE *mac)
{
    return ((mac->registers->net_status & GEM_PHY_IDLE) == GEM_PHY_IDLE);
}

/**
 * Get the value of the transmit status register.
 * The return value is an unsigned 32-bit integer containing the contents of the
 * register.  This should be masked appropriately to obtain the relevant status.
 *
 * Return value:
 * Returns current value of transmit status register.
 */
U32 GemGetTxStat(GEM_DEVICE *mac)
{
    return (mac->registers->tx_status);
}

/**
 * Reset the specified bits of the transmit status register.
 *
 * @param mac Pointer to device structure
 * @param rst_status Status to reset
 */
void GemResetTxStat(GEM_DEVICE *mac, U32 rst_status)
{
    mac->registers->tx_status |= rst_status;
}

/**
 * Get the value of the receive status register.
 * The return value is an unsigned 32-bit integer containing the contents of the
 * register.  This should be masked appropriately to obtain the relevant status.
 *
 * Returns current receive status.
 */
U32 GemGetRxStat(GEM_DEVICE *mac)
{
    return (mac->registers->rx_status);
}

/**
 * Reset the specified bits of the receive status register.
 *
 * @param mac Pointer to device structure
 * @param rst_status Status to reset
 * There is no return value.
 */
void GemResetRxStat(GEM_DEVICE *mac, U32 rst_status)
{
    mac->registers->rx_status |= rst_status;
}

/**
 * Enable jumbo frames to be accepted.
 *
 * There is no return value.
 */
void GemEnableRxJmb(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_RX_JUMBO;
}

/**
 * Disable jumbo frames to be accepted.
 *
 * There is no return value.
 */
void GemDisableRxJmb(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_RX_JUMBO);
}

/**
 * Enable only VLAN frames to be accepted, all other frames will be discarded.
 *
 * There is no return value.
 */
void GemEnableVlanOnly(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_VLAN_ONLY;
}

/**
 * Disable VLAN frame only mode. All frames will be accepted.
 *
 * There is no return value.
 */
void GemDisableVlanOnly(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_VLAN_ONLY);
}

/**
 * Read the interrupt status register.
 * This returns an unsigned 32-bit integer with the current interrupt status,
 * this should be masked appropriately to get the required status.
 * Note that the interrupt status register is automatically reset on read, so
 * the returned value should be stored if further processing required.
 *
 * Returns the current interrupt status.
 */
U32 GemGetIrqStat(GEM_DEVICE *mac)
{
    return (mac->registers->irq_status);
}

/**
 * Set specified bits in the interrupt status register.
 * This can be used for debug purposes to manually activate an interrupt.
 *
 * There is no return value.
 */
void GemSetIrqStat(GEM_DEVICE *mac, U32 irq_status)
{
    mac->registers->irq_status |= irq_status;
}

/**
 * Enable specified interrupts.
 * The specified interrupt bits are enabled by unmasking them.
 * Note that this appends to the existing interrupt enable list.
 *
 * There is no return value.
 */
void GemEnableIrq(GEM_DEVICE *mac, U32 irq_en)
{
    mac->registers->irq_enable |= irq_en;
}

/**
 * Disable specified interrupts.
 * The specified interrupts are masked out so that they do not generate an
 * interrupt.
 * Note that this appends to the existing interrupt mask list.
 *
 * There is no return value.
 */
void GemMaskIrq(GEM_DEVICE *mac, U32 irq_mask)
{
    mac->registers->irq_disable |= irq_mask;
}

/**
 * Obtain the current interrupt mask value.
 * The return value indicates which interrupts are currently masked out i.e. do
 * not generate an interrupt.
 *
 * Returns the interrupt mask status.
 */
U32 GemGetIrqMask(GEM_DEVICE *mac)
{
    return (mac->registers->irq_mask);
}

/**
 * Write to the PHY management registers.
 * This function simply sets off the PHY management operation, completion will
 * be indicated by an interrupt.
 * The input parameters are the PHY address, register address, and the 16-bit
 * data to be written.
 * Note that the MDIO enable register must be on.
 *
 * Return value:
 *  0   :   OK
 *  -1  :   Invalid input range.
 */
RESULTCODE GemPhyManWr(GEM_DEVICE *mac, U8 phy_addr, U8 reg_addr, U32 data)
{
    U32 write_data;

    if ( (phy_addr> 0x1F) || (reg_addr> 0x1F) || (data> 0xFFFF) )
    {
        return FAILURE;
    }

    write_data = 0x50020000;
    write_data |= ( (phy_addr << 23) | (reg_addr << 18) | data );
    mac->registers->phy_man = write_data;

    return SUCCESS;
}

/**
 * Perform PHY management register read operation.
 * This function simply sets off the PHY management operation, completion will
 * be indicated by an interrupt.
 * The input parameters are the PHY address and the register address to be read
 *
 * Return value:
 *  0   :   OK
 *  -1  :   Invalid input range.
 */
RESULTCODE GemPhyManRd(GEM_DEVICE *mac, U8 phy_addr, U8 reg_addr)
{
    U32 write_data;

    if ( (phy_addr> 0x1F) || (reg_addr> 0x1F) )
    {
        return FAILURE;
    }

    write_data = 0x60020000;
    write_data |= ( (phy_addr << 23) | (reg_addr << 18) );
    mac->registers->phy_man = write_data;

    return SUCCESS;
}

/**
 * Read the data section of the PHY management register.  After a read operation
 * the data from the PHY will be stored here.
 *
 * Return value is the lower 16-bits of the PHY management register.
 */
U32 GemPhyManData(GEM_DEVICE *mac)
{
    return (mac->registers->phy_man & 0xFFFF);
}

/**
 * Read the pause time register.
 *
 * Returns the current value in the pause time register which will
 * decrement when the MAC has gone into pause mode.
 */
U32 GemPauseTime(GEM_DEVICE *mac)
{
    return (mac->registers->pause_time);
}

/**
 * Set the id-check registers of the MAC.
 * These registers are used to check the type-id field of the incoming frames,
 * if matched, the appropriate status bit will be set in word 1 of the receive
 * descriptor for that frame.
 * The input parameter is truncated to 16-bits.
 *
 * There is no return value.
 */
void GemSetIdCheck1(GEM_DEVICE *mac, U32 id_check)
{
    mac->registers->id_check1 = (id_check & 0xFFFF);
}
void GemSetIdCheck2(GEM_DEVICE *mac, U32 id_check)
{
    mac->registers->id_check2 = (id_check & 0xFFFF);
}
void GemSetIdCheck3(GEM_DEVICE *mac, U32 id_check)
{
    mac->registers->id_check3 = (id_check & 0xFFFF);
}
void GemSetIdCheck4(GEM_DEVICE *mac, U32 id_check)
{
    mac->registers->id_check4 = (id_check & 0xFFFF);
}

/**
 * Get the value of the id-check registers in the MAC.
 *
 * Return value:
 *  Value of ID check register.
 */
U32 GemGetIdCheck1(GEM_DEVICE *mac)
{
    return (mac->registers->id_check1);
}
U32 GemGetIdCheck2(GEM_DEVICE *mac)
{
    return (mac->registers->id_check2);
}
U32 GemGetIdCheck3(GEM_DEVICE *mac)
{
    return (mac->registers->id_check3);
}
U32 GemGetIdCheck4(GEM_DEVICE *mac)
{
    return (mac->registers->id_check4);
}

/**
 * Set the hash register of the MAC.
 * This register is used for matching unicast and multicast frames.
 * The parameter of this function should be a pointer to type MAC_ADDR as
 * defined in the header file.
 *
 * There is no return value.
 */
void GemSetHash(GEM_DEVICE *mac, MAC_ADDR *hash_addr)
{
    mac->registers->hash_addr = *hash_addr;
}

/**
 * Get the current value of the hash registers of the MAC.
 *
 * This function returns a value of type MAC_ADDR
 */
MAC_ADDR GemGetHash(GEM_DEVICE *mac)
{
    return (mac->registers->hash_addr);
}

/**
 * Setup all the specific address registers for the MAC.
 * These registers are matched against incoming frames to determine whether the
 * frame should be copied to memory.
 * The input parameter to this function should be a pointer to type SPEC_ADDR
 * as defined in the header file.
 *
 * There is no return value.
 */
void GemSetAddress(GEM_DEVICE *mac, SPEC_ADDR *spec_addr)
{
    mac->registers->address = *spec_addr;
}

/**
 * Get the current set of specific match addresses for the MAC.
 * Note that a pointer is not returned as this would give direct access to the
 * MAC address space which may cause unpredictable behaviour if wrongly used.
 *
 * Return type is of type SPEC_ADDR as defined in the header file.
 */
SPEC_ADDR GemGetAddress(GEM_DEVICE *mac)
{
    return (mac->registers->address);
}

/**
 * Set specific local addresses of the MAC.
 * Rather than setting up all four specific addresses, this function sets them
 * up individually.  The input parameter should be a pointer to type MAC_ADDR.
 *
 * There are no return values.
 */
void GemSetLAddr1(GEM_DEVICE *mac, MAC_ADDR *address)
{
    mac->registers->address.one = *address;
}
void GemSetLAddr2(GEM_DEVICE *mac, MAC_ADDR *address)
{
    mac->registers->address.two = *address;
}
void GemSetLAddr3(GEM_DEVICE *mac, MAC_ADDR *address)
{
    mac->registers->address.three = *address;
}
void GemSetLAddr4(GEM_DEVICE *mac, MAC_ADDR *address)
{
    mac->registers->address.four = *address;
}

/**
 * Get specific local addresses of the MAC.
 * This allows returning of a single specific address stored in the MAC.
 *
 * Return value if of type MAC_ADDR as defined in the header file.
 */
MAC_ADDR GemGetLAddr1(GEM_DEVICE *mac)
{
    return (mac->registers->address.one);
}
MAC_ADDR GemGetLAddr2(GEM_DEVICE *mac)
{
    return (mac->registers->address.two);
}
MAC_ADDR GemGetLAddr3(GEM_DEVICE *mac)
{
    return (mac->registers->address.three);
}
MAC_ADDR GemGetLAddr4(GEM_DEVICE *mac)
{
    return (mac->registers->address.four);
}

/**
 * Obtain the statistics registers structure.
 * type GEM_STATS as defined in the header file.
 * Note that this operation is relatively slow as it is copying all the
 * statistics register values rather than providing a pointer reference to them.
 * Note also that the statistics registers will all be automatically cleared
 * after this operation.
 *
 * Returns the entire statistics register block for the MAC in a structure of
 * type GEM_STATS
 */
GEM_STATS GemGetStats(GEM_DEVICE *mac)
{
    return (mac->registers->stats);
}

/**
 * Set the values of the statistics registers.
 * This is for debug only and allows reading and writing to the statistic
 * registers to verify functionality.
 *
 * There is no return value.
 */
void GemSetStats(GEM_DEVICE *mac, GEM_STATS *stats)
{
    mac->registers->stats = *stats;
}

/**
 * Reset the MAC device to its default value and load up the MAC address stored
 * in the header file into specific address 1.  The MAC will be held in
 * quiescent state.
 * This function should be called to initialise and check the device prior to
 * setting up the buffer queues and enabling the MAC.  If it is called mid way
 * through operation, the MAC is reset to default value and any pending frames
 * will be lost.
 * Note that the values in the GEM_DEVICE software structure are not reset, only
 * the MAC registers are reset.  This is to allow, if necessary to recover the
 * buffers and reload them into the MAC, however prior to doing this, they
 * should be cleared first.
 *
 * Return value:
 *  0   :   OK
 *  -1  :   Error in write/read check on initialisation.
 */
RESULTCODE GemReset(GEM_DEVICE *mac)
{
    MAC_ADDR zero_address =
    {0x00000000, 0x00000000};
    MAC_ADDR enet_address = zero_address;

    int stats_length;
    int loop_i;

    stats_length = sizeof(GEM_STATS)/4;

    /* Write to registers and set default values. */
    mac->registers->net_control = GEM_STATS_CLR | GEM_MDIO_ENABLED;
    mac->registers->net_config = GEM_DEF_DUPLEX;
    GemSetLoop(mac, GEM_DEF_LOOP);
    GemSetMdcDiv(mac, GEM_DEF_PCLK_DIV);
    GemSetSpeed (mac, GEM_DEF_SPEED);
    GemSetAhbWidth(mac, GEM_DEF_AHB_WIDTH);

    mac->registers->tx_status = 0xFFFFFFFF;
    mac->registers->rx_qptr = 0x00000000;
    mac->registers->tx_qptr = 0x00000000;
    mac->registers->rx_status = 0xFFFFFFFF;
    mac->registers->irq_disable = 0xFFFFFFFF;
    mac->registers->irq_status = 0x00000000;
    mac->registers->phy_man = 0x00000000;
    mac->registers->hash_addr = zero_address;
    mac->registers->address.one = enet_address;
    mac->registers->address.two = zero_address;
    mac->registers->address.three = zero_address;
    mac->registers->address.four = zero_address;
    mac->registers->id_check1 = 0x00000000;
    mac->registers->id_check2 = 0x00000000;
    mac->registers->id_check3 = 0x00000000;
    mac->registers->id_check4 = 0x00000000;

    /* Now read back values and return if not correct. */
    if ( (mac->registers->id_check1 != 0x00000000) ||
        (mac->registers->id_check2 != 0x00000000) ||
        (mac->registers->id_check3 != 0x00000000) ||
        (mac->registers->id_check4 != 0x00000000) ||
        (mac->registers->address.four.bottom != zero_address.bottom) ||
        (mac->registers->address.four.top != zero_address.top) ||
        (mac->registers->address.three.bottom != zero_address.bottom) ||
        (mac->registers->address.three.top != zero_address.top) ||
        (mac->registers->address.two.bottom != zero_address.bottom) ||
        (mac->registers->address.two.top != zero_address.top) ||
        (mac->registers->address.one.bottom != enet_address.bottom) ||
        (mac->registers->address.one.top != enet_address.top) ||
        (mac->registers->hash_addr.bottom != zero_address.bottom) ||
        (mac->registers->hash_addr.top != zero_address.top) ||
        (mac->registers->phy_man != 0x00000000) ||
        (mac->registers->irq_status != 0x00000000) ||
        (mac->registers->irq_mask != 0x0003FFFF) ||
        (mac->registers->rx_status != 0x00000000) ||
        (mac->registers->tx_qptr != 0x00000000) ||
        (mac->registers->rx_qptr != 0x00000000) ||
        (mac->registers->tx_status != 0x00000000))
    {
        return FAILURE;
    }
    else if (((mac->registers->net_control & GEM_MDIO_EN) !=
            (0x00000000 | GEM_MDIO_ENABLED)) ||
            ((mac->registers->net_config & GEM_FULL_DUPLEX) !=
            (0x00000000 | GEM_DEF_DUPLEX)) ||
            (GemGetLoop(mac) != GEM_DEF_LOOP) ||
            (GemGetMdcDiv(mac) != GEM_DEF_PCLK_DIV) ||
            (GemGetAhbWidth(mac) != GEM_DEF_AHB_WIDTH))
    {
        return FAILURE;
    }
    else
    {
        for (loop_i = 0; loop_i < stats_length; loop_i++)
        {
            if ( *(((U32 *)&mac->registers->stats) + loop_i) != 0x00000000)
            {
                return FAILURE;
            }
        }
    }
    return SUCCESS;
}

/**
 * Enable length field checking feature.
 * The length field check feature automatically discards frames that has a frame
 * length smaller than that reported in the length field of the header.
 *
 * Note that in accordance with the IEEE spec, frames that are longer than that
 * reported in length field is still accepted as a valid frame.
 *
 * This function has no return value.
 */
void GemEnableLenCheck(GEM_DEVICE *mac)
{
    mac->registers->net_config |= GEM_RX_LEN_CHK;
}

/**
 * Disable length field checking feature.
 *
 * This function has no return value.
 */
void GemDisableLenCheck(GEM_DEVICE *mac)
{
    mac->registers->net_config &= (~GEM_RX_LEN_CHK);
}

/**
 * Convert standard byte style ethernet address to format compatible with MAC.
 *
 * Input    :   Pointer to beginning of 6 byte address.
 *              Pointer to MAC_ADDR structure.
 * Return values:
 *  0   :   OK
 *  -1  :   Invalid inputs.
 */
RESULTCODE GemEnetAddrByteMac(U8 * enet_byte_addr, MAC_ADDR *enet_addr)
{
    if ((enet_byte_addr == NULL) || (enet_addr == NULL))
    {
        return FAILURE;
    }

    enet_addr->bottom = enet_byte_addr[0] | (enet_byte_addr[1] << 8)
        | (enet_byte_addr[2] << 16) | (enet_byte_addr[3] << 24);
    enet_addr->top = enet_byte_addr[4] | (enet_byte_addr[5] << 8);

    return SUCCESS;
}

/**
 * Convert MAC type ethernet address to standard byte style ethernet address.
 *
 * Input    :   Pointer to beginning of free space for 6 byte address.
 *              Pointer to MAC_ADDR structure.
 * Return values:
 *  0   :   OK
 *  -1  :   Invalid inputs.
 */
RESULTCODE GemEnetAddrMacByte(U8 * enet_byte_addr, MAC_ADDR *enet_addr)
{
    if ((enet_byte_addr == NULL) || (enet_addr == NULL))
    {
        return FAILURE;
    }

    enet_byte_addr[0] = enet_addr->bottom & 0xFF;
    enet_byte_addr[1] = (enet_addr->bottom >> 8) & 0xFF;
    enet_byte_addr[2] = (enet_addr->bottom >> 16) & 0xFF;
    enet_byte_addr[3] = (enet_addr->bottom >> 24) & 0xFF;

    enet_byte_addr[4] = enet_addr->top & 0xFF;
    enet_byte_addr[5] = (enet_addr->top >> 8) & 0xFF;

    return SUCCESS;
}
