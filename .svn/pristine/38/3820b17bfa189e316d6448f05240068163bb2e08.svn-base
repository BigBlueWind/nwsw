/**
 *   C4000 firmware loader
 *
 *   Copyright (C) 2009 Mindspeed Technologies, Inc.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *
 **/
#include <mach/hardware.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/firmware.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>

#include <linux/device.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/mach-types.h>


#define ELFMAG0         0x7f            /* e_ident[EI_MAG0] */
#define ELFMAG1         'E'             /* e_ident[EI_MAG1] */
#define ELFMAG2         'L'             /* e_ident[EI_MAG2] */
#define ELFMAG3         'F'             /* e_ident[EI_MAG3] */
#define ELFMAG          "\177ELF"       /* magic */
#define SELFMAG         4               /* size of magic */
#define IS_ELF(ehdr) ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                      (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                      (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                      (ehdr).e_ident[EI_MAG3] == ELFMAG3)


#define LOW_ARM_CORE 	  	0
#define LOW_ARM_VBR	  	0xFFFF0000		/* reset physical address */
#define LOW_ARM_RESET_DELAY 	5000
#define MAX_RESET_CNT	  	200
#define INSTR_B			0xEA000000
#define INSTR_LDR_PC_18		0xE59FF018


#define SAFE_UNMAP(x) 	\
    if (x) {		\
	iounmap(x);	\
	x = NULL;	\
    }



extern volatile void (*asmp_core_reset)(void);
extern int icpu2_start(u32 context);


unsigned cpuid, coreid = LOW_ARM_CORE;
static unsigned noipi = 0;
static unsigned fileid = 0;
unsigned diag = 0;
unsigned dual_core = 1;
static DEFINE_SPINLOCK(reset_lock);
struct platform_device *low_arm_device;

volatile unsigned *reset_vect = NULL;
volatile unsigned char *lowarm_base = NULL;

volatile unsigned *core_ready = (volatile unsigned*) 0xfa002000;
volatile unsigned *core_boot = (volatile unsigned*) 0xfa002004;
volatile unsigned *reset_entry = (volatile unsigned*) 0xfa002008;
unsigned vbr = 0;
unsigned vbr_irq_flags = 0;
unsigned asmp_load = 1;

#define VBR_SIZE 256
unsigned char vbr_data[VBR_SIZE];
unsigned char vbr_linux_data[VBR_SIZE];

unsigned lowarm_hdr_base;

void ipi_init(void);
void ipi_exit(void);

static void wait_reset_complete(void *info)
{
        unsigned long irq_flags;
        if (cpuid != smp_processor_id()) {
                /* Hold core in lock until reset complete */
                spin_lock_irqsave(&reset_lock, irq_flags);
                spin_unlock_irqrestore(&reset_lock, irq_flags);
        }
}

static void set_exception_handler(unsigned vector, unsigned long handler)
{
        unsigned long addr;
        addr  = (handler - (vbr + vector + 8)) >> 2;
        if (addr & 0xFF000000UL) {
                reset_vect[0] = INSTR_LDR_PC_18;
                reset_vect[8] = handler;
        } else {
                *reset_vect = INSTR_B | addr;
        }
        smp_wmb();
}

unsigned long reset_core(unsigned int cpu)
{
        static int wait_loops;
        unsigned long vbr0, vbr8, irq_flags, reset_loops;

        /*
         * Some SMP cores can flush cache, raise IRQ and invalidate RESET vector 
         * We have to hold lock on all other cores and sync their cache before reset
         */
        cpuid = smp_processor_id();
        lock_kernel();

        /* lock any other symmetric cores */
        smp_call_function(wait_reset_complete, NULL, 0);
        spin_lock_irqsave(&reset_lock, irq_flags);

        *core_boot = 0;
        *core_ready = 0;

        /* save old value */
        smp_rmb();
        vbr0 = reset_vect[0];
        vbr8 = reset_vect[8];

        /* install reset handler */
        set_exception_handler(0, *reset_entry);

        /* reset the core */
        *(volatile u32*) COMCERTO_SYS_A9QP_RESET_0 |= (1 << (4 + cpu));
        wait_loops = 100000000;
        while (--wait_loops) {
                nop();
        }

        *(volatile u32*) COMCERTO_SYS_A9QP_RESET_0 &= ~(1 << (4 + cpu));

        /* and now waiting for reset */
        reset_loops = LOW_ARM_RESET_DELAY;
        while ((*core_ready != cpu) && --reset_loops) {
                nop();
        }

        /* restore old value */
        reset_vect[0] = vbr0;
        reset_vect[8] = vbr8;
        smp_wmb();

        spin_unlock_irqrestore(&reset_lock, irq_flags);
        unlock_kernel();

        return reset_loops;
}

int c4k_fw_reset_core(unsigned int cpu)
{
        static int reset_cnt = 0;
        unsigned long reset_loops = 0;

        *(volatile u32*) COMCERTO_ARM_IRQ_SET |= (1 << 4);
	mdelay(500);
	printk("%x written to %x\n", 1<<4, COMCERTO_ARM_IRQ_SET);
        printk("the core is running, send reset signal ");
        
        if (dual_core) {
	    *(volatile u32*) COMCERTO_SYS_A9QP_RESET_0 = 0xF0;
	    printk("done\n\n");
	    return 1;
	}

        if (cpu <= 3) {
                while(1) {
                        /*
                         * In case watchdog reset did not happen 
                         * during reset_loops, we have to retry reset again 
                         */
                        if ((reset_loops = reset_core(cpu)) != 0) {
                                break;
                        } else {
                                if (reset_cnt < MAX_RESET_CNT) {
                                        reset_cnt++;
                                        if( (reset_cnt % 4) == 0 ) {
                                                printk(".");
                                        }
                                } else {
                                        printk("failed core_ready=%X\n", *core_ready);
                                        reset_cnt = 0;
                                        break;
                                }
                        }
                }
        } else {		
		*(volatile u32*) COMCERTO_SYS_A9DP_RESET_0 = 0x30;
                printk("done\n\n");
	        udelay(1000);
        }
        return ( reset_loops );
}

int c4k_fw_cpu_up(unsigned int cpu, unsigned entry_phys)
{
        unsigned long timeout;
	unsigned result = 0;

        if (dual_core) {
        	int i;
        	u32 *header = ioremap_nocache(lowarm_hdr_base, 4096);
        	volatile u32 *version= &header[1];
        	volatile u32 *status = &header[4];

    		cpu = 0;
    		printk("Booting core #%d at address %x, fw version %u.%u\n", cpu, entry_phys, *version>>16, *version&0xFFFF);

		if (noipi)
			header[3] = 2;				/* if noipi parameter was passed - select ICPU2 */

                *(volatile u32*) COMCERTO_SYS_A9QP_RESET_0 = 0xE0;

		if (*version < 0x00020001) {			/* check for version 2.1 or lower */
			udelay(1000);
			printk("\nQuad Core[%d] has been started\n", cpu);
		} else {					/* version is high enough to try the status */
			for (i = 0; i < 1000; i++) {
				if (*status & (1 << 31))
					break;
				udelay(1);
			}

			if (*status & (1 << 31)) {
				printk("\nQuad Core[%d] is up and running\n", cpu);
			} else {
				printk("\nQuad Core[%d] - no start indication\n", cpu);
				result = -1;
			}
		}

		if (result == 0 && noipi) {
			/* wait until ICPU2 finished initializing */
			for (i = 0; i < 5000; i++) {
				if (header[2] != 0)
					break;
				udelay(1000);
			}
			if (header[2]) {
				printk("ICPU2: started in %ums\n", i);
				icpu2_start(header[2]);
			} else
				printk("ICPU2: not initialized\n");
		}

		iounmap(header);

		return result;
	}

        printk("booting core #%d at address %x\n", cpu, entry_phys);
        if (cpu <= 3) {
                /* asmp entry is waiting for non zero asmp_core_boot after WFI */
                if (asmp_load) {
                        *core_boot = entry_phys;
                        smp_wmb();

                        timeout = jiffies + (1 * HZ);
                        while (time_before(jiffies, timeout)) {
                                /*
                                   * asmp entry sets asmp_core_ready to zero 
                                   * once control is transfered to boot code  
                                   */
                                smp_rmb();
                                if (*core_ready == 0) {
                                        printk("\nAsymmetric CPU[%d] is up and running\n", cpu);
                                        break;
                                }
                                udelay(10);
                        }
                        if (*core_ready)
                                printk("Failed to reset low-arm core %x\n", *core_ready);
                } else {
                        volatile unsigned wait_loops;
                        unsigned char *sdata, *hdr;
                        unsigned long irq_flags;

                        hdr = ioremap_nocache(lowarm_hdr_base, 64);

                	/*
                         * Some SMP cores can flush cache, raise IRQ and invalidate RESET vector 
                         * We have to hold lock on all other cores and sync their cache before reset
                         */
                        cpuid = smp_processor_id();
                        lock_kernel();

                        /* lock any other symmetric cores */
                        smp_call_function(wait_reset_complete, NULL, 0);
                        spin_lock_irqsave(&reset_lock, irq_flags);

                        /* Either clear or load section if it fits in lowarm address range */
                        sdata = ioremap_nocache(vbr, VBR_SIZE);
                        memcpy(vbr_linux_data, sdata, VBR_SIZE);
                        memcpy(sdata, vbr_data, VBR_SIZE);

                        *(volatile u32*) COMCERTO_SYS_A9QP_RESET_0 |= (1 << (4 + cpu));
                        wait_loops = 4000000;
                        while (--wait_loops) {
                                nop();
                        }

                        *(volatile u32*) COMCERTO_SYS_A9QP_RESET_0 &= ~(1 << (4 + cpu));
			wait_loops = 4000000;
                        while ( (*(volatile unsigned*)(hdr + 16) & 1) == 0 ) {
                                nop(); wait_loops -= 1;
				if (wait_loops == 0)
					break;
                        }

			if (wait_loops)
                        	printk("\nThe core[%d] released from reset\n", cpu);
			else {
				printk("\nFailed to reset the core %d, check lowarm code\n", cpu);
				result = -1;
			}

                        /* Either clear or load section if it fits in lowarm address range */
                        memcpy (sdata, vbr_linux_data, VBR_SIZE);
                        iounmap(sdata);
                        spin_unlock_irqrestore(&reset_lock, irq_flags);
                        unlock_kernel();

			iounmap(hdr);
                }
        } else {
                cpu -= 4;
                if (entry_phys != 0 && entry_phys != 0xffff0000)
                        set_exception_handler(0, entry_phys);

                *(volatile u32*) COMCERTO_SYS_A9DP_RESET_0 &= ~( (( (cpu + 1)) << 4) | 1);
                udelay(1000);
                printk("\nDual Core[%d] is up and running\n", cpu);
        }
        return result;
}

static void memset8(char *dst, char byte, int len)
{
    while (len--)
	*dst++ = byte;
}

static void memcpy8(char *dst, char *src, int len)
{
    while (len--)
	*dst++ = *src++;
}

static unsigned firmware_load_elf(void *addr, int size)
{
	Elf32_Ehdr *ehdr = addr;
	Elf32_Shdr *shdr;
	unsigned char *strtab = 0;
	void *sdata;
	int i;

	if (!IS_ELF(*ehdr)) {
		printk(KERN_ERR "Not an ELF image\n");
		return -1;
	}

	if (ehdr->e_type != ET_EXEC) {
		printk(KERN_ERR "Not a 32-bit ELF image\n");
		return -1;
	}

	shdr = (Elf32_Shdr *) (addr + ehdr->e_shoff + (ehdr->e_shstrndx * sizeof (Elf32_Shdr)));
	if (shdr->sh_type != SHT_STRTAB) {
		printk(KERN_ERR "No string table found in image\n");
		return -1;
	}

	strtab = (unsigned char *) (addr + shdr->sh_offset);

	printk("\n=== Loading ELF image, kernel memory range: 0..0x%08lX\n", num_physpages*PAGE_SIZE-1);

	for (i = 0; i < ehdr->e_shnum; i++) {
		shdr = (Elf32_Shdr *) (addr + ehdr->e_shoff + (i * sizeof (Elf32_Shdr)));
		if (!(shdr->sh_flags & SHF_ALLOC) || shdr->sh_size == 0)
			continue;

		printk("0x%08X (%9u bytes) %s '%s' ...\n",
			shdr->sh_addr, shdr->sh_size,
			shdr->sh_type == SHT_NOBITS ? "clearing" : " loading",
			&strtab[shdr->sh_name]);

		if (!strcmp(&strtab[shdr->sh_name], "ROOT_RO"))
			lowarm_hdr_base = shdr->sh_addr;

		if (shdr->sh_addr != 0 && shdr->sh_addr < num_physpages*PAGE_SIZE) {
			printk("\tERROR: section base inside the kernel's memory, aborting\n");
			return -1;
		}

		/* zero address is for low exception vectors, use phys-to-virt instead of ioremap */
		if (shdr->sh_addr != 0)
			sdata = ioremap_nocache(shdr->sh_addr, shdr->sh_size);
		else
			sdata = phys_to_virt(0);

		if (!sdata) {
			printk("\tERROR: failed to map section\n");
			return -1;
		}

		if (shdr->sh_type == SHT_NOBITS)
			memset8(sdata, 0, shdr->sh_size);
		else
			memcpy8(sdata, addr + shdr->sh_offset, shdr->sh_size);

		if (sdata != phys_to_virt(0))
			iounmap(sdata);

		flush_cache_all();
	}

	printk("===\n\n");

	return ehdr->e_entry;
}

static int firmware_load(unsigned char *fw_name)
{
	int retval;
	const struct firmware *fw_entry;

	retval = request_firmware(&fw_entry, fw_name, &low_arm_device->dev);
	if (retval) {
		printk(KERN_ERR "Failed to load firmware %s\n", fw_name);
		return -1;
	}

        retval = firmware_load_elf((void*) fw_entry->data, fw_entry->size);

        release_firmware(fw_entry);

        return retval;
}


static int c4k_fw_map_entries(void)
{
        if (dual_core) {
                vbr = 0;        
        } else {
    	    if (coreid <= 3)
                vbr = 0;
    	    else
                vbr = 0xFFFF0000;
        }

        reset_vect = ioremap_nocache(vbr, 64);
        if (!reset_vect) {
                printk("can't map reset vector\n");
                goto errout;
        }

        return 0;
errout:
        SAFE_UNMAP(reset_vect);
        return -1;
}


static void __exit c4k_fw_load_exit(void)
{
        printk(KERN_INFO "c4k-loader: unplug low-arm core \n");
        SAFE_UNMAP(reset_vect);
        if (!noipi)
                ipi_exit();
        platform_device_unregister(low_arm_device);
}


static int __init c4k_fw_load_init(void)
{
	int tries = 0, result = -1;
        int entry;
	char file_name[80];

        if (c4k_fw_map_entries() < 0)
                return PTR_ERR(low_arm_device);

        printk(KERN_INFO "[%d] c4k-loader: plug low-arm core %d\n", smp_processor_id(), coreid);
        low_arm_device = platform_device_register_simple("low-arm",-1,NULL,0);
        if (IS_ERR(low_arm_device)) {
                printk(KERN_ERR
                       "%s:platform_device_register_simple "
                       "failed\n", __FUNCTION__);
                return PTR_ERR(low_arm_device);
        }

restart:
//	*(volatile u32*) COMCERTO_SPU_CEVA_RESET_0 |= 1;
        if (!c4k_fw_reset_core(coreid)) {
                printk("c4k-loader: reset failed\n");
                c4k_fw_load_exit();
                return -1;
        }
	if (fileid)
	    sprintf(file_name, "lowarm%d.elf",  fileid);
	else
	    sprintf(file_name, "lowarm.elf");	
        entry = firmware_load(file_name);
        if (entry != -1 && !noipi)
                ipi_init();
        if (entry != -1) {
                result = c4k_fw_cpu_up(coreid, entry);
                if (result && ++tries < 3) {
                	printk(KERN_ERR "Low ARM start failed, trying again...\n");
                	goto restart;
		}
	}

	if (entry == -1 || result)
		printk(KERN_ERR "Firmware load failed\n");

        return 0;
}


module_init(c4k_fw_load_init);
module_exit(c4k_fw_load_exit);
module_param(coreid, int, S_IRUGO);
module_param(noipi, int, S_IRUGO);
module_param(fileid, int, S_IRUGO);
module_param(diag, int, S_IRUGO);
module_param(dual_core, int, S_IRUGO);
MODULE_AUTHOR("Mindspeed Technologies, Inc.");
MODULE_DESCRIPTION("Mindspeed C4K firmware loader");
MODULE_LICENSE("GPL");
