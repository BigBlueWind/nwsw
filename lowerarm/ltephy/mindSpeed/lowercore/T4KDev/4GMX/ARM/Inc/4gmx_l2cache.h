//-------------------------------------------------------------------------------------------
/** @file 4gmx_l2cache.h
 *
 * @brief ARM L2 PL310 control (based on ARM code)
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifndef _MX_ARM_L2_H_
#define _MX_ARM_L2_H_

#include "4gmx_types.h"
#include "4gmx_const.h"

#define L2X0_CACHE_ID			0x000
#define L2X0_CACHE_TYPE			0x004
#define L2X0_CTRL			    0x100
#define L2X0_AUX_CTRL			0x104
#define L2X0_TAG_LATENCY_CTRL	0x108
#define L2X0_DATA_LATENCY_CTRL	0x10C
#define L2X0_EVENT_CNT_CTRL		0x200
#define L2X0_EVENT_CNT1_CFG		0x204
#define L2X0_EVENT_CNT0_CFG		0x208
#define L2X0_EVENT_CNT1_VAL		0x20C
#define L2X0_EVENT_CNT0_VAL		0x210
#define L2X0_INTR_MASK			0x214
#define L2X0_MASKED_INTR_STAT	0x218
#define L2X0_RAW_INTR_STAT		0x21C
#define L2X0_INTR_CLEAR			0x220
#define L2X0_CACHE_SYNC			0x730
#define L2X0_INV_LINE_PA		0x770
#define L2X0_INV_WAY			0x77C
#define L2X0_CLEAN_LINE_PA		0x7B0
#define L2X0_CLEAN_LINE_IDX		0x7B8
#define L2X0_CLEAN_WAY			0x7BC
#define L2X0_CLEAN_INV_LINE_PA	0x7F0
#define L2X0_CLEAN_INV_LINE_IDX	0x7F8
#define L2X0_CLEAN_INV_WAY		0x7FC
#define L2X0_LOCKDOWN_WAY_D		0x900
#define L2X0_LOCKDOWN_WAY_I		0x904
#define L2X0_TEST_OPERATION		0xF00
#define L2X0_LINE_DATA			0xF10
#define L2X0_LINE_TAG			0xF30
#define L2X0_DEBUG_CTRL			0xF40

/* Interrupt bits */
#define L2X0_INTR_ECNTR                 0x01

/* Aux Control bits */
#define L2X0_AUX_CTRL_EMBUS             (0x01<<20)

/* Event Counter Control bits */
#define L2X0_EVENT_CONTROL_ENABLE       0x1
#define L2X0_EVENT_CONTROL_RESET_ALL    0x6

/* Event Counter Config bits */
#define L2X0_EVENT_CONFIG_DISABLED      0x0
#define L2X0_EVENT_CONFIG_CO            (0x1<<2)
#define L2X0_EVENT_CONFIG_DRHIT         (0x2<<2)
#define L2X0_EVENT_CONFIG_DRREQ         (0x3<<2)
#define L2X0_EVENT_CONFIG_DWHIT         (0x4<<2)
#define L2X0_EVENT_CONFIG_DWREQ         (0x5<<2)
#define L2X0_EVENT_CONFIG_DWTREQ        (0x6<<2)
#define L2X0_EVENT_CONFIG_IRHIT         (0x7<<2)
#define L2X0_EVENT_CONFIG_IRREQ         (0x8<<2)
#define L2X0_EVENT_CONFIG_WA            (0x9<<2)
#define L2X0_EVENT_INTERRUPT_ON_INC     0x1
#define L2X0_EVENT_INTERRUPT_ON_OVF     0x2
#define L2X0_EVENT_INTERRUPT_DISABLED   0x3

#if defined(MX_ARM_L2_CACHE_ENABLED)

void MxL2_init(UINT32 aux_val, UINT32 aux_mask);
void MxL2_disable (void);
BOOL MxL2IsEnabled (void);
BOOL MxL2IsWB (void);

void MxL2_invalidate(UINT32 start, UINT32 size);
void MxL2_clean(UINT32 start, UINT32 size);
void MxL2_flush(UINT32 start, UINT32 size);

void MxL2_inv_all(void);
void MxL2_clean_all(void);
void MxL2_flush_all(void);

#endif // _MX_ARM_L2_H_

#endif // defined(MX_ARM_L2_CACHE_ENABLED)

