/*
 * Userland Inter-Processor Interface library
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/firmware.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/irq.h>
#include <asm/cacheflush.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include "ipi.h"

#define CMD_LEN 		(16*1024)
#define CMD_INBOX_FIFO		256
#define CMD_OUTBOX_FIFO		256
#define FIFO_BLOCK_LEN		sizeof(u32)

//#define LOWARM_HDR_BASE		0x08000000
#define SHM_DESC_BASE_NEW      	0x3F000000
#define SHM_DESC_BASE_16M      	0x09000000
#define SHM_DESC_SIZE        	0x00100000

#define SHM_DESC_BASE      	shm_desc_base

#define SHM_PART_BASE		shm_part_base
#define SHM_PART_SIZE		(CMD_INBOX_FIFO*CMD_LEN + CMD_OUTBOX_FIFO*CMD_LEN)

#define SHM_QUEUE_BASE		shm_queue_base
#define SHM_QUEUE_SIZE		(CMD_INBOX_FIFO*FIFO_BLOCK_LEN + CMD_OUTBOX_FIFO*FIFO_BLOCK_LEN)


#define IPI_MAJOR       	243
#define IPI_DEVICE_NAME 	"lowarm"

/* Those 2 variables should be either global of platform specific */
#define IPI_TO_QUAD_ARM    (7)     /* QuadCore Interprocessor interrupt number  */
#define IPI_FROM_QUAD_ARM  (6)
#define IPI_TO_DUAL_ARM    (32+6)  /* DualCore Interprocessor interrupt number  */
#define IPI_FROM_DUAL_ARM  (32+7)

extern unsigned lowarm_hdr_base;
struct ipi_block_hdr_t {
        volatile struct ipi_block_hdr_t *next;
};

struct ipi_part_t {
        unsigned long   storage;		/* physically continous shared memory pool */
        /* physical address of next free block */
        volatile struct ipi_block_hdr_t* freeblock;
        volatile u32    blksize;		/* block length in bytes */
        u32             blkcount;		/* number of blocks in memory pool */
        volatile u32    lock;			/* interprocessor mutex  */
        u32             numalloc;
};

struct ipi_queue_t {
        unsigned long   storage;		/* physical address of fifo pool */
        volatile u16    get;			/* read index */
        volatile u16    put;			/* write index */
        u16             size;			/* fifo depth */
        u16             itemsize;		/* should be 4 bytes to carry index */
        volatile u32    lock;			/* interprocessor mutex */
};

struct ipi_mailbox_t {
        struct ipi_queue_t  	queue;		/* request fifo */
        struct ipi_part_t   	part;		/* shared memory pool descriptor */
};

/* This header can be passed to/from lower arm image */
struct lowarm_header_t {
        unsigned long 	magic;			/* lowarm arm magic 0x12345678  (from lower arm)*/
        unsigned long	version;		/* lower arm version number (from lower arm) 	*/
        unsigned long	shm_desc_base;		/* shared memory base address (from upper arm)	*/
        unsigned long   hbi_enable;		/* use hbi or not */
        unsigned long   status;
        unsigned long   diag;
	unsigned long   nommu;			/* use mmu on little arm or not */
};

struct api_hdr_t {
	u32 control;
	u32 length;
	u16 type;
	u16 srcID;
	u16 dstID;
	u16 msgID;
};


unsigned shm_part_base;
unsigned shm_queue_base;
unsigned shm_desc_base;

unsigned ipi_to_low_arm;
unsigned ipi_from_low_arm;

unsigned long* queue_storage = NULL;
unsigned long* part_storage = NULL;

static int maped_io = 0;
unsigned dual_core_iface = 0;
extern unsigned coreid;
extern unsigned diag;

unsigned ipi_sema_base = 0xFBF00000;

struct ipi_mailbox_t* mailbox_desc = NULL;
struct ipi_mailbox_t* lower_to_upper_mbox = NULL;
struct ipi_mailbox_t* upper_to_lower_mbox = NULL;

static DECLARE_WAIT_QUEUE_HEAD(ipi_wait_queue);

/* static unsigned char device_name[] = "lowarm"; */

#define __part_va(x)   ((struct ipi_block_hdr_t*) ((u32)(part_storage) + ((u32)(x) - SHM_PART_BASE)))
#define __queue_va(x)  ((u32*) ((u32)(queue_storage) + ((u32)(x) - SHM_QUEUE_BASE)))


static DEFINE_SPINLOCK(q_lock);

static unsigned long ipi_lock(volatile u32* lock) {
	unsigned oldval, res;
	unsigned long irq_flags;

	register unsigned old = 0;
	register unsigned new = 1;
	spin_lock_irqsave(&q_lock, irq_flags);
	do {
		if (dual_core_iface) {
			res = *lock;
		} else {
			__asm__ __volatile__("@ ipi_lock\n"
					"ldrex  %1, [%2]\n"
					"mov    %0, #0\n"
					"teq    %1, %3\n"
					"it     eq\n"
					"strexeq %0, %4, [%2]\n"
					: "=&r" (res), "=&r" (oldval)
					: "r" (lock), "Ir" (old), "r" (new)
					: "cc");
		}
	} while (res && atomic_backoff_delay());
	return irq_flags;
}

static void ipi_release(volatile u32* lock, unsigned long irq_flags) {
	unsigned oldval, res;

	if (dual_core_iface) {
		*lock = 0;
	} else {
		register unsigned old = 1;
		register unsigned new = 0;
		do {
			__asm__ __volatile__("@ ipi_release\n"
					"ldrex  %1, [%2]\n"
					"mov    %0, #0\n"
					"teq    %1, %3\n"
					"it     eq\n"
					"strexeq %0, %4, [%2]\n"
					: "=&r" (res), "=&r" (oldval)
					: "r" (lock), "Ir" (old), "r" (new)
					: "cc");
		} while (res && atomic_backoff_delay());
	}
	spin_unlock_irqrestore(&q_lock, irq_flags);
}

static void ipi_create_q(struct ipi_queue_t* q, void *storage, u32 blockcount) {
	q->put = 0;
	q->get = 0;
	q->size = blockcount;
	q->itemsize = FIFO_BLOCK_LEN;
	q->storage = (unsigned long) storage;
	if (dual_core_iface) {
		q->lock = ipi_sema_base;
		ipi_sema_base += sizeof(u32);
		*(volatile u32*) q->lock = 0;
	} else
		q->lock = 0;
}


static void ipi_create_p(struct ipi_part_t *p, void* storage, u32 blksize, u32 blkcnt)
{
/*        u32 i; */
        volatile struct ipi_block_hdr_t *block = (struct ipi_block_hdr_t *) storage;

        p->storage = (unsigned long) storage;
        p->blksize = blksize;
        p->blkcount = blkcnt;
        p->lock = 0;
        p->numalloc = 0;
        p->freeblock = block;
        while ( --blkcnt ) {
                __part_va(block)->next = (struct ipi_block_hdr_t *) (((unsigned char*) block) + blksize);
                block = __part_va(block)->next;
        }
        __part_va(block)->next = 0;	
	if ( dual_core_iface ) {
	    p->lock = ipi_sema_base;
	    ipi_sema_base += sizeof(u32);
	    *(volatile u32*) p->lock = 0;
	} else
	    p->lock = 0;
}


static void* ipi_alloc_part(struct ipi_part_t *p) {
	volatile struct ipi_block_hdr_t *block;
	unsigned long flags;

	if (dual_core_iface)
		flags = ipi_lock((volatile u32*) p->lock);
	else
		flags = ipi_lock(&p->lock);
	block = p->freeblock;
	if (block) {
		p->freeblock = __part_va(block)->next;
		p->numalloc++;
	}
	if (dual_core_iface)
		ipi_release((volatile u32*) p->lock, flags);
	else
		ipi_release(&p->lock, flags);
	return (void*) block;
}

static void ipi_free_part(struct ipi_part_t* p, struct ipi_block_hdr_t* block) {
	unsigned long flags;
	if (dual_core_iface)
		flags = ipi_lock((volatile u32*) p->lock);
	else
		flags = ipi_lock(&p->lock);
	__part_va(block)->next = p->freeblock;
	p->freeblock = block;
	p->numalloc--;
	if (dual_core_iface)
		ipi_release((volatile u32*) p->lock, flags);
	else
		ipi_release(&p->lock, flags);
}

static void ipi_signal_peer_cpu(void) {
	if (dual_core_iface)
		*(volatile u32*) COMCERTO_ARM_IRQ_SET |= 1 << (ipi_to_low_arm - 32);
	else {
		//	    gic_raise_softirq(cpumask_of_cpu(low_arm_cpu), ipi_to_low_arm);
		*(volatile unsigned*) 0xfff01f00 = 0x40006;
	}
}

static int ipi_enqueue(struct ipi_queue_t* q, unsigned long idx) {
	u32 put;
	u32 overflow = 0;
	unsigned long flags;

	if (dual_core_iface)
		flags = ipi_lock((volatile u32*) q->lock);
	else
		flags = ipi_lock(&q->lock);
	put = q->put + 1;
	if (put >= q->size)
		put = 0;
	if (put != q->get) {
		__queue_va(q->storage)[q->put] = idx;
		q->put = put;
	} else {
		printk(KERN_ERR "Queue overflow 0x%X\n", (unsigned int) q);
		overflow = 1;
	}
	if (dual_core_iface)
		ipi_release((volatile u32*) q->lock, flags);
	else
		ipi_release(&q->lock, flags);
	return (overflow) ? -1 : 0;
}


static unsigned long ipi_dequeue(struct ipi_queue_t* q) {
	u32 get;
	u32 idx = 0xffffffff;
	unsigned long flags;

	if (dual_core_iface)
		flags = ipi_lock((volatile u32*) q->lock);
	else
		flags = ipi_lock(&q->lock);
	get = q->get;
	if (q->put != get) {
		idx = __queue_va(q->storage)[get];
		if (++get >= q->size)
			get = 0;
		q->get = get;
	}
	if (dual_core_iface)
		ipi_release((volatile u32*)q->lock, flags);
	else
		ipi_release(&q->lock, flags);
	return idx;
}


static ssize_t ipi_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
	int index;
/*	int bytes_read = 0; */
	struct ipi_part_t *p = &lower_to_upper_mbox->part;
	struct ipi_queue_t *q = &lower_to_upper_mbox->queue;

	wait_event_interruptible(ipi_wait_queue, ((index = ipi_dequeue(q)) != -1) );

	if (index == -1) {
		return 0;
	}

	if (!maped_io) {

		/*
		 * For not mapped I/O driver copy full block from pool block to user buffer
		 */
		struct ipi_block_hdr_t* block = (struct ipi_block_hdr_t*) (p->storage + p->blksize * index);
		struct api_hdr_t *msg_hdr = (struct api_hdr_t *)__part_va(block);
		// Check for header version
		if ((msg_hdr->control & 0xF0000000) == 0x10000000) {
			if (length < msg_hdr->length + 16) {
				printk(KERN_ERR "IPI: insufficient buffer size for the read on the application side (%d < %d)\n",
					length, msg_hdr->length+16);
				BUG();
			}

			if (copy_to_user(buffer, (u32*) __part_va(block), msg_hdr->length + 16))
				printk(KERN_ERR "copy_to_user fails\n");
		}
		else {
			if (length < p->blksize) {
				printk(KERN_ERR "IPI: insufficient buffer size for the read on the application side (%d < %d)\n",
					length, p->blksize);
				BUG();
			}

			if (copy_to_user(buffer, (u32*) __part_va(block), p->blksize))
				printk(KERN_ERR "copy_to_user fails\n");
		}

		ipi_free_part(p, block);
	}
	return p->blksize;
}

/* ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *); */

static ssize_t ipi_write(struct file *filp, const char *buffer, size_t length, loff_t *offset) {
/*	int index;
	int bytes_read = 0, val;
	struct ipi_part_t *p = &lower_to_upper_mbox->part;
	struct ipi_queue_t *q = &lower_to_upper_mbox->queue;
*/
	u32* blk;
	int aidx;

	if (!maped_io) {
		/*
		 * For not mapped I/O, driver copies full block from user buffer to pool block
		 */
		blk = (u32*) ipi_alloc_part(&upper_to_lower_mbox->part);
		if (blk == NULL)
			return -ENOMEM;
		aidx = ((u32) blk - (u32) upper_to_lower_mbox->part.storage) / upper_to_lower_mbox->part.blksize;
		
		if (length > upper_to_lower_mbox->part.blksize) {
			printk(KERN_ERR "IPI: insufficient block size to support write from the application side (%d > %d)\n",
				length, upper_to_lower_mbox->part.blksize);
			BUG();
		}

		if (copy_from_user((void*) __part_va(blk), buffer, length))
			printk(KERN_ERR "copy_to_user fails\n");

		ipi_enqueue(&upper_to_lower_mbox->queue, aidx);
		ipi_signal_peer_cpu();
	}
	return length;
}

int ipi_open(struct inode* node, struct file *filp) {
	return 0;
}

int ipi_close(struct inode *node, struct file *filp) {
	return 0;
}

static ssize_t ipi_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned long index;
	struct ipi_queue_t *q = &lower_to_upper_mbox->queue;

	switch (cmd) {
	case ICPU_GET_INDEX:
		if (q->get == q->put) {
			if (wait_event_interruptible(ipi_wait_queue, q->get != q->put))
				return -ERESTARTSYS;
		}

		index = ipi_dequeue(q);
		if (index == -1)
			return -1;

		return put_user(index, (ulong __user *)arg);

	case ICPU_GET_BASE:
		if (!shm_desc_base)
			return -ENODEV;

		return put_user((ulong)shm_desc_base, (ulong __user *)arg);
    case ICPU_LARM_CRASH:
        // stop LARM
        *(volatile u32*) COMCERTO_ARM_IRQ_SET |= 1 << (32+5 - 32);
        printk(KERN_ERR "Abort LARM @ %u\n", *(volatile unsigned long *) 0xFE050004);

        return put_user(index, (ulong __user *)arg);
	}

	return -EOPNOTSUPP;
}

static unsigned int ipi_poll(struct file *file, struct poll_table_struct *wait)
{
	struct ipi_queue_t *q = &lower_to_upper_mbox->queue;

	poll_wait(file, &ipi_wait_queue, wait);
	if (q->get != q->put)
		return (POLLIN | POLLRDNORM);

	return 0;
}

static struct file_operations ipi_fops = {
        .open = ipi_open,
        .release = ipi_close,
        .read = ipi_read,
        .write = ipi_write,
	.ioctl = ipi_ioctl,
	.poll = ipi_poll,
};


static irqreturn_t lowarm_interrupt(int irq, void *context)
{
        wake_up_interruptible(&ipi_wait_queue);
        if ( dual_core_iface )
                *(volatile u32*) COMCERTO_ARM_IRQ_CLR |= 1 << (ipi_from_low_arm - 32);
        return IRQ_HANDLED;
}

static int tmp_dev;
static int __init ipi_device_init(void) {
	int major;
	int rc;

	printk("Create I-CPU character device %s (major %d)\n", IPI_DEVICE_NAME, IPI_MAJOR);
	major = register_chrdev(IPI_MAJOR, IPI_DEVICE_NAME, &ipi_fops);
	if (major < 0) {
		printk("Unable to register character device, ERR=%d\n", major);
		return major;
	}

	if (dual_core_iface) {
		*(volatile u32*) COMCERTO_ARM_IRQ_CLR |= 1 << (ipi_from_low_arm - 32);
	}

	if ((rc = request_irq(ipi_from_low_arm, lowarm_interrupt, IRQF_DISABLED
			| IRQF_SHARED, IPI_DEVICE_NAME, &tmp_dev))) {
		printk(KERN_ALERT "%s: error %d requesting IRQ %d\n", __FUNCTION__, rc,
				ipi_from_low_arm);
		return -ENODEV;
	}
	return 0;
}

static int ipi_dev_exit(void) {
	printk("Unplug I-CPU device\n");

	if (mailbox_desc) {
		iounmap(mailbox_desc);
		mailbox_desc = NULL;
	}

	if (queue_storage) {
		iounmap(queue_storage);
		queue_storage = NULL;
	}

	if (part_storage) {
		iounmap(part_storage);
		part_storage = NULL;
	}

	free_irq(ipi_from_low_arm, &tmp_dev);
	unregister_chrdev(IPI_MAJOR, IPI_DEVICE_NAME);
	return 0;
}

static unsigned ipi_shm_base(void) {
	unsigned shm_base;
	struct lowarm_header_t *hdr = NULL;

	hdr = ioremap_nocache(lowarm_hdr_base, sizeof(struct lowarm_header_t));
	if (!hdr) {
		printk("Unable to read L-ARM header\n");
		return 0;
	}
	if (hdr->magic != 0x12345678)
		shm_base = SHM_DESC_BASE_16M;
	else {
		hdr->shm_desc_base = SHM_DESC_BASE_NEW;
		shm_base = hdr->shm_desc_base;
	}
	hdr->hbi_enable = 0;
	hdr->diag = diag;
	iounmap(hdr);
	return (shm_base);
}

int ipi_init(void) {
	if (coreid > 3)
		dual_core_iface = 1;
	else
		dual_core_iface = 0;

	printk("I-CPU driver with optimized memcpy\n");

	printk("Init 4GMX I-CPU interface: %s %d\n",
			dual_core_iface ? "Dual-Core" : "Quad-Core",
			dual_core_iface ? coreid - 4 : coreid);
	shm_desc_base = ipi_shm_base();
	if (!shm_desc_base) {
		printk("Unable to access shared memory\n");
		return -1;
	}

	shm_part_base = shm_desc_base + SHM_DESC_SIZE;
	shm_queue_base = shm_part_base + SHM_PART_SIZE;
	mailbox_desc = (struct ipi_mailbox_t*) ioremap_nocache(shm_desc_base,
			SHM_DESC_SIZE);
	if (!mailbox_desc) {
		printk("Unable to map mailbox descriptors\n");
		return -1;
	}

	queue_storage = (void*) ioremap_nocache(shm_queue_base, SHM_QUEUE_SIZE);
	if (!queue_storage) {
		printk("Unable to map queue storage\n");
		return -1;
	}
	printk("I-CPU queue mapped to %x\n", (unsigned int) queue_storage);

	printk("Check I-CPU queue storage access @ %X ", shm_queue_base);
	writel(0xAA55AA55, queue_storage);
	printk(0xAA55AA55 == readl(queue_storage) ? "OK\n" : "ERROR\n");

	part_storage = (void*) ioremap_nocache(shm_part_base, SHM_PART_SIZE);
	if (!queue_storage) {
		printk("Unable to map partition storage\n");
		return -1;
	}
	printk("Partition mapped to %x\n", (unsigned int) part_storage);

	printk("Check I-CPU partition access @ 0x%X ", shm_part_base);
	writel(0xAA55AA55, part_storage);
	printk(0xAA55AA55 == readl(part_storage) ? "OK\n" : "ERROR\n");

	lower_to_upper_mbox = &mailbox_desc[0];
	upper_to_lower_mbox = &mailbox_desc[1];

	printk("I-CPU RX area @ %X\n", lower_to_upper_mbox - mailbox_desc
			+SHM_DESC_BASE);
	printk("I-CPU TX area @ %lX\n", (long) upper_to_lower_mbox
			- (long) mailbox_desc + SHM_DESC_BASE);

	ipi_create_q(&lower_to_upper_mbox->queue, (void*)shm_queue_base, CMD_INBOX_FIFO);
	ipi_create_q(&upper_to_lower_mbox->queue, 
		(void*) (shm_queue_base + FIFO_BLOCK_LEN * CMD_INBOX_FIFO), CMD_OUTBOX_FIFO);

	ipi_create_p(&lower_to_upper_mbox->part, (void*)shm_part_base, CMD_LEN,	CMD_INBOX_FIFO);
	ipi_create_p(&upper_to_lower_mbox->part, 
		(void*) (shm_part_base + CMD_LEN * CMD_INBOX_FIFO), CMD_LEN, CMD_OUTBOX_FIFO);

	if (dual_core_iface) {
		ipi_to_low_arm = IPI_TO_DUAL_ARM;
		ipi_from_low_arm = IPI_FROM_DUAL_ARM;
	} else {
		ipi_from_low_arm = IPI_TO_QUAD_ARM;
		ipi_to_low_arm = IPI_FROM_QUAD_ARM;
	}
	ipi_device_init();
	return 0;
}

int ipi_exit(void) {
	ipi_dev_exit();
	return 0;
}
