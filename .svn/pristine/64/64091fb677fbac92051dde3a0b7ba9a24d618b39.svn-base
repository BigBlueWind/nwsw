//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL 4gmx_l2cache.c
 *
 * @brief ARM L2 PL310 control (based on ARM code)
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"
#include "4gmx_l2cache.h"
#include "4gmx_arm.h"

#if defined(MX_ARM_L2_CACHE_ENABLED)

#define CACHE_LINE_SIZE		32

#define readl(addr) MXREG32(addr)
#define writel(val, addr) MXREG32(addr) = (val)

INLINE BOOL MxIsL2Enabled (void)
{
    return (readl(MX_ARM_L2_BASE + L2X0_CTRL) & 1);
}

INLINE void cache_wait_always(UINT32 reg, UINT32 mask)
{
	/* wait for the operation to complete */
	while (readl(reg) & mask);
}

INLINE void cache_wait(UINT32 reg, UINT32 mask)
{
	/* cache operations are atomic */

    while ((MXREG32 (reg) & mask) != 0) _NOP();
}

#ifdef MX_L2_HW_SYNC_ENABLED

#define _MxL2_lock(sema)	MxiLockHwSema(sema)
#define _MxL2_unlock(sema)	MxiUnlockHwSema(sema)

#else

#define _MxL2_lock(sema)	
#define _MxL2_unlock(sema)	

#endif

#define block_end(start, end)		(end)

INLINE void cache_sync(void)
{
	writel(0, MX_ARM_L2_BASE + L2X0_CACHE_SYNC);
	//cache_wait(MX_ARM_L2_BASE + L2X0_CACHE_SYNC, 1);
}

INLINE void MxL2_clean_line(UINT32 addr)
{
	cache_wait(MX_ARM_L2_BASE + L2X0_CLEAN_LINE_PA, 1);
	writel(addr, MX_ARM_L2_BASE + L2X0_CLEAN_LINE_PA);
}

INLINE void MxL2_inv_line(UINT32 addr)
{
	cache_wait(MX_ARM_L2_BASE + L2X0_INV_LINE_PA, 1);
	writel(addr, MX_ARM_L2_BASE + L2X0_INV_LINE_PA);
}

#ifdef CONFIG_PL310_ERRATA_588369
static void debug_writel(UINT32 val)
{
         writel(val, MX_ARM_L2_BASE + L2X0_DEBUG_CTRL);
}

INLINE void MxL2_flush_line(UINT32 addr)
{
	/* Clean by PA followed by Invalidate by PA */
	cache_wait(MX_ARM_L2_BASE + L2X0_CLEAN_LINE_PA, 1);
	writel(addr, MX_ARM_L2_BASE + L2X0_CLEAN_LINE_PA);
	cache_wait(MX_ARM_L2_BASE + L2X0_INV_LINE_PA, 1);
	writel(addr, MX_ARM_L2_BASE + L2X0_INV_LINE_PA);
}
#else

/* Optimised out for non-errata case */
INLINE void debug_writel(UINT32 val)
{
}

INLINE void MxL2_flush_line(UINT32 addr)
{
	cache_wait(MX_ARM_L2_BASE + L2X0_CLEAN_INV_LINE_PA, 1);
	writel(addr, MX_ARM_L2_BASE + L2X0_CLEAN_INV_LINE_PA);
}
#endif

void MxL2_inv_all_ex(void)
{
	/* invalidate all ways */
	_MxL2_lock(MX_HW_SEMA_L2_SYNC);
	writel(0xffff, MX_ARM_L2_BASE + L2X0_INV_WAY);
	cache_wait_always(MX_ARM_L2_BASE + L2X0_INV_WAY, 0xffff);
	cache_sync();
	_MxL2_unlock(MX_HW_SEMA_L2_SYNC);
}

void MxL2_inv_all(void)
{
    if (!MxIsL2Enabled ())
        return;

	/* invalidate all ways */
	_MxL2_lock(MX_HW_SEMA_L2_SYNC);
	writel(0xffff, MX_ARM_L2_BASE + L2X0_INV_WAY);
	cache_wait_always(MX_ARM_L2_BASE + L2X0_INV_WAY, 0xffff);
	cache_sync();
	_MxL2_unlock(MX_HW_SEMA_L2_SYNC);
}

void MxL2_clean_all(void)
{
    if (!MxIsL2Enabled ())
        return;

	/* invalidate all ways */
	_MxL2_lock(MX_HW_SEMA_L2_SYNC);
	writel(0xffff, MX_ARM_L2_BASE + L2X0_CLEAN_WAY);
	cache_wait_always(MX_ARM_L2_BASE + L2X0_CLEAN_WAY, 0xffff);
	cache_sync();
	_MxL2_unlock(MX_HW_SEMA_L2_SYNC);
}

void MxL2_flush_all(void)
{
    if (!MxIsL2Enabled ())
        return;

	/* invalidate all ways */
	//_MxL2_lock(MX_HW_SEMA_L2_SYNC);
	writel(0xffff, MX_ARM_L2_BASE + L2X0_CLEAN_INV_WAY);
	cache_wait_always(MX_ARM_L2_BASE + L2X0_CLEAN_INV_WAY, 0xffff);
	cache_sync();
	//_MxL2_unlock(MX_HW_SEMA_L2_SYNC);
}

static void MxL2_inv_range(UINT32 start, UINT32 end)
{
	_MxL2_lock(MX_HW_SEMA_L2_SYNC);
    
	if (start & (CACHE_LINE_SIZE - 1)) {
		start &= ~(CACHE_LINE_SIZE - 1);
		debug_writel(0x03);
		MxL2_flush_line(start);
		debug_writel(0x00);
		start += CACHE_LINE_SIZE;
	}

	if (end & (CACHE_LINE_SIZE - 1)) {
		end &= ~(CACHE_LINE_SIZE - 1);
		debug_writel(0x03);
		MxL2_flush_line(end);
		debug_writel(0x00);
	}

	while (start < end) {
		UINT32 blk_end = block_end(start, end);

		while (start < blk_end) {
			MxL2_inv_line(start);
			start += CACHE_LINE_SIZE;
		}

		if (blk_end < end) {}
    }
	cache_wait(MX_ARM_L2_BASE + L2X0_INV_LINE_PA, 1);
	cache_sync();
    
	_MxL2_unlock(MX_HW_SEMA_L2_SYNC);
}

static void MxL2_clean_range(UINT32 start, UINT32 end)
{
	_MxL2_lock(MX_HW_SEMA_L2_SYNC);
    
	start &= ~(CACHE_LINE_SIZE - 1);
	while (start < end) {
		UINT32 blk_end = block_end(start, end);

		while (start < blk_end) {
			MxL2_clean_line(start);
			start += CACHE_LINE_SIZE;
		}

		if (blk_end < end) {}
	}
	cache_wait(MX_ARM_L2_BASE + L2X0_CLEAN_LINE_PA, 1);
	cache_sync();
    
	_MxL2_unlock(MX_HW_SEMA_L2_SYNC);
}

static void MxL2_flush_range(UINT32 start, UINT32 end)
{
	_MxL2_lock(MX_HW_SEMA_L2_SYNC);
    
	start &= ~(CACHE_LINE_SIZE - 1);
	while (start < end) {
		UINT32 blk_end = block_end(start, end);

		debug_writel(0x03);
		while (start < blk_end) {
			MxL2_flush_line(start);
			start += CACHE_LINE_SIZE;
		}
		debug_writel(0x00);

		if (blk_end < end) {}
	}
	cache_wait(MX_ARM_L2_BASE + L2X0_CLEAN_INV_LINE_PA, 1);
	cache_sync();
    
	_MxL2_unlock(MX_HW_SEMA_L2_SYNC);
}


void MxL2_clean(UINT32 start, UINT32 size)
{
    IRQTYPE irq = ARM_INT_disable ();

    if (MxIsL2Enabled ())
        MxL2_clean_range (start, start + size);

    ARM_INT_restore (irq);
}

void MxL2_flush(UINT32 start, UINT32 size)
{
    IRQTYPE irq = ARM_INT_disable ();

    if (MxIsL2Enabled ())
        MxL2_flush_range (start, start + size);

    ARM_INT_restore (irq);
}

void MxL2_invalidate(UINT32 start, UINT32 size)
{
    IRQTYPE irq = ARM_INT_disable ();

    if (MxIsL2Enabled ())
        MxL2_inv_range (start, start + size);

    ARM_INT_restore (irq);
}

BOOL MxL2IsEnabled (void)
{
    return MxIsL2Enabled();
}

BOOL MxL2IsWB (void)
{
    return !(MXREG32(MX_ARM_L2_BASE + L2X0_DEBUG_CTRL) & (1<<1));
}

void  MxL2_init(UINT32 aux_val, UINT32 aux_mask)
{
	UINT32 aux = 0;

	/*
	 * Check if MxL2 controller is already enabled.
	 * If you are booting from non-secure mode
	 * accessing the below registers will fault.
	 */
	if (!(readl(MX_ARM_L2_BASE + L2X0_CTRL) & 1)) {

		/* MxL2 controller is disabled */
		aux = readl(MX_ARM_L2_BASE + L2X0_AUX_CTRL);
		aux &= aux_mask;
		aux |= aux_val;
		writel(aux, MX_ARM_L2_BASE + L2X0_AUX_CTRL);

        // TAG RAM latency setup

        aux =   (MX_ARM_L2_TAG_RAM_WRITE << 8) |
                (MX_ARM_L2_TAG_RAM_READ  << 4) |
                (MX_ARM_L2_TAG_RAM_SETUP);

		writel(aux, MX_ARM_L2_BASE + L2X0_TAG_LATENCY_CTRL);

        // DATA RAM latency setup

        aux =   (MX_ARM_L2_DATA_RAM_WRITE << 8) |
                (MX_ARM_L2_DATA_RAM_READ  << 4) |
                (MX_ARM_L2_DATA_RAM_SETUP);

		writel(aux, MX_ARM_L2_BASE + L2X0_DATA_LATENCY_CTRL);

		MxL2_inv_all_ex();

#ifdef MX_ARM_L2_CACHE_WRITE_THROUGH_ENABLED
        aux = readl(MX_ARM_L2_BASE + L2X0_DEBUG_CTRL);
        aux |= (1 << 1);
        writel (aux, MX_ARM_L2_BASE + L2X0_DEBUG_CTRL);
#endif //MX_ARM_L2_CACHE_WRITE_THROUGH_ENABLED

		/* enable L2X0 */
		writel(1, MX_ARM_L2_BASE + L2X0_CTRL);
	}
}

void MxL2_disable (void)
{
    IRQTYPE irq = ARM_INT_disable ();

    if (MX_ARM_L2_BASE != 0)
    {
        //MxL2_inv_all();
        cache_sync();

        /* enable L2X0 */
        writel(0, MX_ARM_L2_BASE + L2X0_CTRL);

        //MxL2_inv_all();
    }
    ARM_INT_restore (irq);
}

#endif //(MX_ARM_L2_CACHE_ENABLED)
