/** @file intc.h
 *
 * @brief Interrupt controller (INTC) definitions and interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef INTC_H_
#define INTC_H_

#include "basetypes.h"
#include "resultcodes.h"
#include "portio.h"

//
// INT Status Masks
//

// STATUS_REGISTER_0
#define INTC_TIMERA             31
#define INTC_TIMERB             30
#define INTC_XDONE1             29
#define INTC_XDONE0             28
#define INTC_MDMA_DONE          27
#define INTC_SPI1               26
#define INTC_SPI                25
#define INTC_I2C                24
#define INTC_TIMERC             23
#define INTC_TIMERD             22
#define INTC_IDMA               21
#define INTC_SPU1               20
#define INTC_SPU0               19
#define INTC_HITXFUL            18
#define INTC_HIRXEMT            17
#define INTC_HIMBOX             16
#define INTC_TDM_TIMER          15
#define INTC_EDMA1TX            14
#define INTC_EDMA1RX            13
#define INTC_EMAC1              12
#define INTC_EDMA0TX            11
#define INTC_EDMA0RX            10
#define INTC_EMAC0              9
#define INTC_TDMA               8
#define INTC_PUDMATX            7
#define INTC_PUDMARX            6
#define INTC_PUI                5
#define ITTC_RESERVED0          4
#define INTC_PTP2               3   // DSP to Lower
#define INTC_PTP1               2   // Lower to Upper
#define INTC_PTP0               1   // Upper to Lower
#define INTC_STATUS1            0

// STATUS_REGISTER_1
#define INTC_SMC_ERR            (32+31)
#define INTC_ERRMDMA            (32+30)
#define INTC_ERRIDMA            (32+29)
#define INTC_ERRTDTX            (32+28)
#define INTC_ERRTDRX            (32+27)
#define INTC_TDMTSB             (32+26)
#define INTC_UART1              (32+25)
#define INTC_NTG                (32+24)
#define INTC_UEXP               (32+23)
#define INTC_G32                (32+22)
#define INTC_RESERVED1          (32+21)
#define INTC_RESERVED2          (32+20)
#define INTC_RESERVED3          (32+19)
#define INTC_RESERVED4          (32+18)
#define INTC_RESERVED5          (32+17)
#define INTC_RESERVED6          (32+16)
#define INTC_RESERVED7          (32+15)
#define INTC_RESERVED8          (32+14)
#define INTC_RESERVED9          (32+13)
#define INTC_RESERVED10         (32+12)
#define INTC_RESERVED11         (32+11)
#define INTC_ERRTDMA            (32+10)
#define INTC_UART0              (32+9)
#define INTC_GPIO7              (32+8)
#define INTC_GPIO6              (32+7)
#define INTC_GPIO5              (32+6)
#define INTC_GPIO4              (32+5)
#define INTC_GPIO3              (32+4)
#define INTC_GPIO2              (32+3)
#define INTC_GPIO1              (32+2)
#define INTC_GPIO0              (32+1)
#define INTC_STATUS0            (32+0)

//
// Priority registers
//
#define INTC_PRI_EMPTY          0x16        // No priority has been set

//
// IRQ Winner
//
#define INTC_NOWINNER           32

//
// Maximum possible number of IRQ sources
//
#define INTC_MAX_VEC            64

// TODO: Rewrite

void IntcEnableIrq(U32 cpuid, U32 vector);
void IntcDisableIrq(U32 cpuid, U32 vector);
BOOLEAN IntcIsIrqEnabled(U32 cpuid, U32 vector);
void IntcClearInt(U32 vector);
void IntcSetSelfclear(U32 vector);

RESULTCODE IntcSetPriority(U32 vector, U32 priority);

void IntcEnableLowArm(void);
void IntcDisableLowArm(void);

#endif  /*INTC_H_*/
