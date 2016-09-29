/*
 * ICPU2 support user space library - header file
 *
 * Copyright (C) 2011 Mindspeed Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ICPU2_H
#define __ICPU2_H

#define ICPU2_IOCTL_WAIT	_IO('I', 1)
#define ICPU2_SYSFS_INFO_PATH	"/sys/class/l-arm/icpu2/info"

/* structure is shared between local and remote sides */
struct icpu2_queue {
	unsigned int	ptr_phys;	/* physical address pointer, can't be changed to virtual since structure is shared */
	unsigned int	put;
	unsigned int	get;
	unsigned int	num;		/* number of elements in the queue */
	unsigned int	stat_put;	/* put counter, incremented each time element is put into the queue */
	unsigned int	stat_get;
};

/* local structure, main purpose - to have mapped ::ptr */
struct icpu2_local_queue {
	unsigned int		*ptr;	/* local pointer, virtual address of queue array */
	struct icpu2_queue	*queue;	/* pointer to mapped queue structure */
};

/* main control structure, filled by remote side,
 * please note that RX/TX fields are 'crossed' - remote side uses TX where local side uses RX notations
 */
struct icpu2_context {
	unsigned int	version;
	unsigned int	status;		/* bit 0: 1 == the ICPU is initialized */
	unsigned int	rx_irq;		/* inter-ARM ID (0..7) of interrupt to notify local side */
	unsigned int	tx_irq;		/* inter-ARM ID (0..7) of interrupt to notify remote side */

	unsigned int	rx_storage;	/* pointer to the array where RX blocks are located, physical address */
	unsigned int	tx_storage;

	unsigned int	rx_blocks_num;	/* number of blocks in RX storage */
	unsigned int	rx_block_size;	/* size of block in bytes in RX storage */
	unsigned int	tx_blocks_num;
	unsigned int	tx_block_size;

	unsigned int	rx_queue_num;	/* number of TX queues, assumed to be 3, the same for RX below */
	unsigned int	tx_queue_num;

	struct icpu2_queue	queues[6]; 
};

/* structure to hold info about memory mapping, see icpu2_mmap(), icpu2_munmap() */
struct icpu2_mm {
	unsigned int	phys;
	void		*virt;
	unsigned int	length;
};

/* local control data structure */
struct icpu2 {
	int			driver_fd;	/* ICPU2 driver file descriptor, < 0 if not used */

	/* virtual pointers to all local queues */
	struct icpu2_local_queue	txq_free;
	struct icpu2_local_queue	txq_priority;
	struct icpu2_local_queue	txq;
	struct icpu2_local_queue	rxq_free;
	struct icpu2_local_queue	rxq_priority;
	struct icpu2_local_queue	rxq;

	unsigned int		rx_block_size;
	unsigned int		tx_block_size;
	unsigned int		irq_mask;	/* IRQ mask to be written into INTC register to notify remote side */

	struct icpu2_context	*context;	/* mapped context structure, shortcut to avoid typecasting of ::context_mm->virt */

	struct icpu2_mm		*context_mm;	/* context mapping info */
	struct icpu2_mm		*tx_storage_mm;
	struct icpu2_mm		*rx_storage_mm;
	struct icpu2_mm		*intc_mm;

	volatile unsigned int	*intc_arm_set;	/* INTC register to raise IRQ to notify remote side */
};


/*
 * ICPU2 init function
 *
 * Arguments:
 *   driver_node - may be NULL, in this case wait on IRQ is not supported
 * Returns:
 *   NULL in case of initialization error, pointer to local control structure in case of success
 */
struct icpu2	*icpu2_init(const char *driver_node, int cache);
/*
 * ICPU deinitialization
 *
 * Arguments:
 *  icpu2 - pointer to local control structure, frees icpu2 structure
 * Returns:
 *  0 - success
 */
int		icpu2_exit(struct icpu2 *icpu2);

/*
 * Memory mapping helper - wrapper over system mmap() call, memory mapped via this call must be
 * unmapped via munmap() call
 *
 * Arguments:
 *   phys_addr, length - physical address and length of memory region, address must be page aligned
 * Returns:
 *   NULL - failure, pointer to mapped area otherwise
 */
void		*icpu2_mmap_helper(unsigned int phys_addr, unsigned int length, int flag);
/*
 * Memory mapping helper - stored all mapping info in allocated icpu2_mm structure.
 *
 * Arguments:
 *   /see icpu2_mmap_helper()/
 * Returns:
 *   NULL - failure, pointer to allocated icpu2_mm structure with all fields filled
 */
struct icpu2_mm	*icpu2_mmap(unsigned int phys_addr, unsigned int length, int flag);
/*
 * Memory unmap helper, see icpu2_mmap(), destroys mapping and frees icpu2_mm structure
 *
 * Arguments:
 *   mm - structure with mapping info
 * Returns
 */
int		icpu2_munmap(struct icpu2_mm *mm);

/*
 * Wait for any non-empty RX queue.
 *
 * Arguments:
 *   icpu2 - local control structure
 *   wait_type:
 *	< 0 - negative specifies busy wait in user space (this will utilize 100% of CPU time and
 	function won't return until any RX queue is empty)
	0   - zero specifies to check queue statuses and exit immediately
 *	> 0 - positivie specifies blocking wait on IRQ, this requires kernel support and
 *	icpu2_init() must be called with proper driver node file name, note that function checks
 *	queue statuses anyway before calling driver, if queues are not empty - driver won't be called
 * Returns:
 *   < 0 - function failed
 *   0   - all RX queues are empty (can only be possible when called with wait_type == 0
 *   1   - RX high-priority queue is not empty
 *   2   - RX normal-priority queue is not empty
 *   3   - both queues are not empty
 */
int		icpu2_wait(struct icpu2 *icpu2, int wait_type);

/*
 * Take one block from RX queues
 *
 * Arguments:
 *   icpu2 - local control structure
 *   block - pointer to pointer to block to return, must be non-NULL
 * Returns:
 *   < 0 - function failed
 *   0   - queues were empty
 *   1   - block was taken from high-priority queue
 *   2   - block was taken from normal-priority queue
 */
int		icpu2_rx(struct icpu2 *icpu2, void **block);
/*
 * Free block, received previously
 *
 * Arguments:
 *   icpu2 - local control structure
 *   block - pointer to block to free, must be non-NULL
 * Returns:
 *   < 0 - function failed
 *   0   - success
 */
int		icpu2_rx_free(struct icpu2 *icpu2, void *block);

/*
 * Put one block to TX queue, raise IRQ to remote side. This function allocates tx block internally
 * and copies data from buffer provided by user. Block allocation is non-blocking - if no free blocks
 * are available function will fail.
 *
 * Arguments:
 *   icpu2    - local control structure
 *   buf      - pointer to data to transmit
 *   length   - data length in bytes
 *   priority - specifies normal priority (0) or high-priority queue (non-zero value)
 * Returns:
 *   < 0 - function failed, -ENOSPC specifies that function couldn't allocate block
 *   0   - success
 */
int		icpu2_tx(struct icpu2 *icpu2, void *buf, unsigned int length, int priority);
/*
 * Allocates one TX block. Can block if no free blocks are available. Block will be allocated in
 * non-cacheable memory thus there may be performance penalty if block is used for anything but
 * simple data filling.
 *
 * Arguments:
 *   icpu2 - local control structure
 *   block - pointer to pointer to allocated block
 *   wait  - whether to wait if no blocks are available (non-zero - do wait)
 * Returns:
 *   < 0 - function failed, -ENOSPC specifies that no free blocks are available
 *   0   - success
 */
int		icpu2_tx_alloc(struct icpu2 *icpu2, void **block, int wait);
/*
 * Put one block to TX queue, raise IRQ to remote side. This function uses block allocated previously
 * by icpu2_tx_alloc() call.
 *
 * Arguments:
 *   icpu2    - local control structure
 *   block    - pointer to block to transmit
 *   priority - specifies normal priority (0) or high-priority queue (non-zero value)
 * Returns:
 *   < 0 - function failed, -ENOSPC specifies that function couldn't allocate block
 *   0   - success
 */
int  icpu2_tx_allocated(struct icpu2 *icpu2, void *block, int priority);

/*
 * Returns physical address of the given virtual address. Address must be in icpu2_mm mapping.
 *
 * Arguments:
 *   mm   - memory mapping information
 *   virt - virtual address to translate
 */
static inline unsigned int icpu2_virt_to_phys(struct icpu2_mm *mm, void *virt)
{
	return mm->phys + (virt - mm->virt);
}
/*
 * Returns virtual address of the given physical address. Address must be in icpu2_mm mapping.
 *
 * Arguments:
 *   mm   - memory mapping information
 *   phys - virtual address to translate
 */
static inline void *icpu2_phys_to_virt(struct icpu2_mm *mm, unsigned int phys)
{
	return mm->virt + (phys - mm->phys);
}

RESULTCODE Icpu2Init(void);
void Icpu2RxTaskDirect(PTR data);
int Icpu2RxDirect(PTR data);
RESULTCODE Icpu2SendDirect(U32 size, PTR data);
PTR  Icpu2AllocBlock(void);
int Icpu2FreeBlock(PTR pMsg);

/*
 * Returns physical address of the given virtual address. Address must be in icpu2_mm mapping.
 *
 * Arguments:
 *   mm   - memory mapping information
 *   virt - virtual address to translate
 */
void icpu2_cache_clean(struct icpu2_mm *mm, void *virt, unsigned int size);
void icpu2_cache_invalidate(struct icpu2_mm *mm, void *virt, unsigned int size);

#define UARM_IOCTL_SHM_CREATE		_IOWR('U', 1, unsigned int*)
#define UARM_IOCTL_CACHE_CLEAN		_IOR ('U', 2, unsigned int*)
#define UARM_IOCTL_CACHE_INVALIDATE	_IOR ('U', 3, unsigned int*)
#define UARM_IOCTL_CACHE_FLUSH		_IOR ('U', 4, unsigned int*)


PTR  Icpu2GetPhys(PTR pAddr);
PTR  Icpu2GetVirt(PTR pAddr);

#endif
