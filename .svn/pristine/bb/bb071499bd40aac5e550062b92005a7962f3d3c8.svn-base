/** @file ntg.h
 *
 * @brief Network Time Generator interfaces and definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef NTG_H_
#define NTG_H_

// Masks
#define NTG_MODE_HALF_EN        0x01        // Half enable
#define NTG_MODE_FRAME_CNT_RST  0x08        // Frame count reset
#define NTG_MODE_CLK_BIT_INV    0x10        // Clock bit inverted
#define NTG_MODE_CLK_BIT_DIR    0x20        // Clock bit direction
#define NTG_MODE_PULSE_FRM_DIR  0x40        // Pulse frame direction
#define NTG_MODE_REG_UPDATE     0x80        // Register update

// TODO: Rewrite

#if 0

INLINE void HalNtgFreqWrite(U64 FreqEst)
{
    IOWriteAnd32A(NTG_MODE, ~NTG_MODE_REG_UPDATE);
    IOWrite32A(NTG_FREQ_SET_INT, HIWORD32(FreqEst));
    IOWrite32A(NTG_FREQ_SET_FRA, LOWORD32(FreqEst));
    IOWriteOr32A(NTG_MODE, NTG_MODE_REG_UPDATE);
}

INLINE U64 HalNtgFreqRead(void)
{
    U64 u64temp;

    IOWriteAnd32A(NTG_MODE, ~NTG_MODE_REG_UPDATE);   // Suspend register updates
    u64temp = IORead32(NTG_FREQ_SET_INT);
    u64temp <<= 32;
    u64temp += IORead32(NTG_FREQ_SET_FRA);
    IOWriteOr32A(NTG_MODE, NTG_MODE_REG_UPDATE);     // Resume register updates

    return u64temp;
}

#endif  /* 0 */

#endif  /* INTG_H_ */
