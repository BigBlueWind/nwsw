//-------------------------------------------------------------------------------------------
/** @file spaccdrv.h
 *
 * @brief SPAcc driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __SPACCDRV_H__
#define __SPACCDRV_H__

#include "config.h"
#include "appids.h"
#include "apimgr.h"

#define SPACC_DRIVER_VERSION   "1.0.1"

#define ENABLED  1
#define DISABLED	 0


#define SPACC_RC_OK                   MX_OK
#define SPACC_RC_SUCCESS                   SPACC_RC_OK

#define SPACC_RC_UNKNOWN_API        DEF_USRRC(APP_DRIVER, SPACC_DRV, 9)
#define SPACC_RC_UNKNOWN_TYPE        DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xA)
#define SPACC_RC_INVALID_ALLOC        DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xB)
#define SPACC_RC_INVALID_PARAM        DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xE)

#define SPACC_RC_CRYPTO_FAILED        DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xC0)
#define SPACC_RC_CRYPTO_INPROGRESS        DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xC1)
#define SPACC_RC_CRYPTO_INVALID_HANDLE                 DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xC2)
#define SPACC_RC_CRYPTO_INVALID_ALG           DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xC7)
#define SPACC_RC_CRYPTO_INVALID_BLOCK_ALIGNMENT       DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xCC)
#define SPACC_RC_CRYPTO_INVALID_MODE        DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xCD)
#define SPACC_RC_CRYPTO_AUTHENTICATION_FAILED  DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xCF)

#define SPACC_RC_CRYPTO_MEMORY_ERROR      DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xD1)
#define SPACC_RC_CRYPTO_INIT_ERROR      DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xD6)
#define SPACC_RC_CRYPTO_OPEN_ERROR        DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xD7)
#define SPACC_RC_CRYPTO_COMPARE_ERROR       DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xD8)
#define SPACC_RC_CRYPTO_COMPARE_ERROR2      DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xD9)
#define SPACC_RC_CRYPTO_TIMEOUT             DEF_USRRC(APP_DRIVER, SPACC_DRV, 0xDA)

#define TRNG_MEMORY_BASE_OFFSET			0x18000
#define SPACC_GLOBAL_MEM_SIZE           64*1024

#define CRYPTO_DES_CBC			1
#define CRYPTO_3DES_CBC			2
#define CRYPTO_BLF_CBC			3
#define CRYPTO_CAST_CBC			4
#define CRYPTO_SKIPJACK_CBC		5
#define CRYPTO_MD5_HMAC			6
#define CRYPTO_SHA1_HMAC		7
#define CRYPTO_RIPEMD160_HMAC	8
#define CRYPTO_MD5_KPDK			9
#define CRYPTO_SHA1_KPDK		10
#define CRYPTO_RIJNDAEL128_CBC	11
#define CRYPTO_AES_CBC			11
#define CRYPTO_ARC4				12
#define CRYPTO_MD5				13
#define CRYPTO_SHA1				14
#define CRYPTO_NULL_HMAC		15
#define CRYPTO_NULL_CBC			16
#define CRYPTO_DEFLATE_COMP		17
#define CRYPTO_SHA2_256_HMAC	18
#define CRYPTO_SHA2_384_HMAC	19
#define CRYPTO_SHA2_512_HMAC	20
#define CRYPTO_CAMELLIA_CBC		21
#define CRYPTO_SHA2_256			22
#define CRYPTO_SHA2_384			23
#define CRYPTO_SHA2_512			24
#define CRYPTO_RIPEMD160		25

/* max size of a packet in bytes */
#define ELP_PACKET_SIZE					2048

/* number of DDT entries (gets x2 for in/out), totsize = X * 2 * 8 bytes */
#define SINGLE_DDT_SIZE					(10*4)

#define SPACC_DATA_SIZE_MAX				2048

/* max DDT particle size */
#ifndef SPACC_MAX_PARTICLE_SIZE
#define SPACC_MAX_PARTICLE_SIZE			65536
#endif

#define SPACC_LOOP_WAIT					100000

/* Avoid using NULL addresses, which indicate the end of a DDT table. */
#define SPACC_MASTER_OFFSET				128

#define SPACC_MIN_CHUNK_SIZE        1024
#define SPACC_MAX_CHUNK_SIZE        16384

/* Priorities */
#define SPACC_SW_ID_PRIO_HI         0
#define SPACC_SW_ID_PRIO_MED        1
#define SPACC_SW_ID_PRIO_LOW        2

/********** Context Offsets **********/

#define SPACC_CTX_CIPH_KEY     0x04000
#define SPACC_CTX_HASH_KEY     0x08000
#define SPACC_CTX_RC4_CTX      0x20000

/******** Sub-Context Offsets ********/

#define SPACC_CTX_AES_KEY      0x00
#define SPACC_CTX_AES_IV       0x20

#define SPACC_CTX_DES_KEY      0x08
#define SPACC_CTX_DES_IV       0x00

#define SPACC_CTX_RC4_KEY      0x00

#define SPACC_RC4_DMA_CTRL     0x00
#define SPACC_RC4_DMA_STAT     0x04
#define SPACC_RC4_DMA_CTX      0x200

#define SPACC_RC4_CTX_IDX_O    0
#define SPACC_RC4_CTX_IDX_W    8
#define _SPACC_RC4_IMPORT      31
#define SPACC_RC4_IMPORT      (1 << _SPACC_RC4_IMPORT)

#define SPACC_SNOW3G_KEY		0x0
#define SPACC_SNOW3G_IV			0x10

/******** Context Page Sizes ???? *********/

//#define SPACC_CTX_CIPH_PAGE_64
#define SPACC_ALG_SHA1

#ifdef SPACC_CTX_CIPH_PAGE_64
#   define SPACC_CTX_CIPH_PAGE      64
#else
/* Cipher content page size:  this is 128 if F8 is present, 64 otherwise */
#   define SPACC_CTX_CIPH_PAGE      128
#endif

#ifndef SPACC_CTX_HASH_PAGE

#if   defined (SPACC_ALG_SHA384) || defined (SPACC_ALG_SHA512)
#   define SPACC_CTX_HASH_PAGE      128
#elif defined (SPACC_ALG_MD5)    || defined (SPACC_ALG_SHA1)   || \
      defined (SPACC_ALG_SHA224) || defined (SPACC_ALG_SHA256)
#   define SPACC_CTX_HASH_PAGE      64
#else
#   error "At least one hash algorithm must be defined."
#endif

#endif

#define SPACC_CTX_RC4_PAGE          512

/************ SPAcc Flags ************/

/* Enables bus mastering rather than use evaluation board memory */
#define SPACC_FLAG_BUS_MASTER        1

/* Manage evaluation board's memory internally, using virtual DDTs */
#define SPACC_FLAG_AUTO_MEM          2

/* Flags to handle various core configurations */
#define SPACC_FLAG_NODES             256
#define SPACC_FLAG_NOAES             512
#define SPACC_FLAG_NORC4             1024
#define SPACC_FLAG_NOMD5             2048
#define SPACC_FLAG_NOSHA1            4096
#define SPACC_FLAG_NOSHA224          8192
#define SPACC_FLAG_NOSHA256          16384
#define SPACC_FLAG_NOSHA384          32768
#define SPACC_FLAG_NOSHA512          65536

/*************************************/

#define MASK_INTEGER(value, offset, width) ( \
   (((1 << (width)) - 1) & (value)) << (offset) \
)

#define UNMASK_INTEGER(value, offset, width) ( \
   ((value) >> (offset)) & ((1 << (width)) - 1) \
)

/******* Hash/Cipher Bitmasks ********/

#define SPACC_PROC_NULL     -1

#define SPACC_PROC_DES      1
#define SPACC_PROC_AES      2
#define SPACC_PROC_RC4      4

#define SPACC_PROC_ECB      0
#define SPACC_PROC_CBC      (1U<<8)
#define SPACC_PROC_CTR      (2U<<8)
#define SPACC_PROC_CCM      (3U<<8)
#define SPACC_PROC_GCM      (5U<<8)
#define SPACC_PROC_F8       (9U<<8)

#define SPACC_PROC_MD5      65536
#define SPACC_PROC_SHA1     131072
#define SPACC_PROC_SHA224   262144
#define SPACC_PROC_SHA256   524288
#define SPACC_PROC_SHA384   1048576
#define SPACC_PROC_SHA512   2097152

#define SPACC_PROC_RAW      0
#define SPACC_PROC_SSLMAC   16777216
#define SPACC_PROC_HMAC     33554432

#define SPACC_PROC_C_MASK   (0x000000ff & ~SPACC_PROC_ENCRYPT & \
                                          ~SPACC_PROC_EXPAND)
#define SPACC_PROC_CM_MASK  0x0000ff00
#define SPACC_PROC_H_MASK   0x00ff0000
#define SPACC_PROC_HM_MASK  0xff000000

/* Used internally, do not set on process words sent to the API */
#define SPACC_PROC_EXPAND   64
#define SPACC_PROC_ENCRYPT  128

#define SPACC_CTX_C_MASK    0x0000ffff
#define SPACC_CTX_H_MASK    0xffff0000

/***********************************************************************************/
/***********************************************************************************/

#define CTRL_SET_CIPH_ALG(a)    ((a) << _SPACC_CTRL_CIPH_ALG)
#define CTRL_SET_CIPH_MODE(a)   ((a) << _SPACC_CTRL_CIPH_MODE)
#define CTRL_SET_HASH_ALG(a)    ((a) << _SPACC_CTRL_HASH_ALG)
#define CTRL_SET_HASH_MODE(a)   ((a) << _SPACC_CTRL_HASH_MODE)
#define CTRL_SET_ENCRYPT        (1U << _SPACC_CTRL_ENCRYPT)
#define CTRL_SET_AAD_COPY       (1U << _SPACC_CTRL_AAD_COPY)
#define CTRL_SET_ICV_PT         (1U << _SPACC_CTRL_ICV_PT)
#define CTRL_SET_ICV_ENC        (1U << _SPACC_CTRL_ICV_ENC)
#define CTRL_SET_ICV_APPEND     (1U << _SPACC_CTRL_ICV_APPEND)
#define CTRL_SET_KEY_EXP        (1U << _SPACC_CTRL_KEY_EXP)
#define CTRL_SET_MSG_BEGIN      (1U << _SPACC_CTRL_MSG_BEGIN)
#define CTRL_SET_MSG_END        (1U << _SPACC_CTRL_MSG_END)
#define CTRL_SET_CTX_IDX(a)     ((a) << _SPACC_CTRL_CTX_IDX)

#define AUX_INFO_SET_DIR(a)     ((a) << _SPACC_AUX_INFO_DIR)
#define AUX_INFO_SET_BIT_ALIGN(a) ((a) << _SPACC_AUX_INFO_BIT_ALIGN)

/**************************************************************************/
/*  PKA   PKA     PKA     PKA     PKA     PKA     PKA     PKA     PKA     */
/**************************************************************************/
#define CRYPTO_PKA_ENDIANESS_LITTLE    1
#define CRYPTO_PKA_ENDIANESS_BIG       2

#define CLUE_BASE	0		// 0x20000
// 32 bits register
#define CLUE_CTRL       CLUE_BASE + 0x0000
#define CLUE_ENTRY      CLUE_BASE + 0x0004
#define CLUE_ADDR       CLUE_BASE + 0x0008	//
#define CLUE_OPCODE		CLUE_BASE + 0x000C	//
#define CLUE_STACK      CLUE_BASE + 0x0010
#define CLUE_CONF_REG   CLUE_BASE + 0x001C
#define CLUE_STATUS     CLUE_BASE + 0x0020
#define CLUE_FLAGS      CLUE_BASE + 0x0024
#define CLUE_WATCHDOG   CLUE_BASE + 0x0028
#define CLUE_CYCLE_SINCE_GO   CLUE_BASE + 0x002C
#define CLUE_INST_SINCE_GO    CLUE_BASE + 0x0014
#define CLUE_INDEX_I    CLUE_BASE + 0x0030
#define CLUE_INDEX_J    CLUE_BASE + 0x0034
#define CLUE_INDEX_K    CLUE_BASE + 0x0038
#define CLUE_INDEX_L    CLUE_BASE + 0x003C
#define CLUE_INT_EN     CLUE_BASE + 0x0040
#define CLUE_JUMP_PROB  CLUE_BASE + 0x0044
#define CLUE_LFSR_SEED  CLUE_BASE + 0x0048

#define CLUE_OPERAND_RA   CLUE_BASE + 0x0400
#define CLUE_OPERAND_RB   CLUE_BASE + 0x0800
#define CLUE_OPERAND_RC   CLUE_BASE + 0x0C00
#define CLUE_OPERAND_RD   CLUE_BASE + 0x1000
#define CLUE_SEQUENCE     CLUE_BASE + 0x2000

// INT EN REGISTER BITS in 0x0040
#define CLUE_INT_BIT_IRQ_EN       30
#define CLUE_CTRL_BIT_IRQ_EN      CLUE_INT_BIT_IRQ_EN

// CONTROL REGISTERS definition in 0x0000
#define CLUE_CTRL_BIT_GO              31
#define CLUE_CTRL_BIT_STOP_ROST       27
//#define CLUE_CTRL_BIT_ENDIAN_SWAP     26
#define CLUE_CTRL_BIT_M521_MODE       16
#define CLUE_CTRL_BIT_BASE_RADIX       8
#define CLUE_CTRL_BIT_PARTIAL_RADIX    0

#define CLUE_CTRL_GO               (UINT32)(1UL<<CLUE_CTRL_BIT_GO)
#define CLUE_CTRL_IRQ_EN           (UINT32)(1UL<<CLUE_CTRL_BIT_IRQ_EN)
#define CLUE_CTRL_STOP_ROST        (UINT32)(1UL<<CLUE_CTRL_BIT_STOP_ROST)
//#define CLUE_CTRL_ENDIAN_SWAP      (UINT32)(1UL<<CLUE_CTRL_BIT_ENDIAN_SWAP)
#define CLUE_CTRL_BASE_RADIX(s)    (UINT32)((s)<<CLUE_CTRL_BIT_BASE_RADIX)
#define CLUE_CTRL_PARTIAL_RADIX(s) (UINT32)((s)<<CLUE_CTRL_BIT_PARTIAL_RADIX)
#define CLUE_CTRL_M521_MODE        (UINT32)(9UL << CLUE_CTRL_BIT_M521_MODE)

// CONTROL REGISTERS definition in 0x0004
#define CLUE_ENTRY_BIT_NEXT_SEQ_INDEX   0
#define CLUE_ENTRY_NEXT_SEQ_INDEX(s)   (UINT32)((s)<<CLUE_ENTRY_BIT_NEXT_SEQ_INDEX)

// CONTROL REGISTERS definition in 0x0008	// ????
#define CLUE_ADDR_BIT_NEXT_SEQ_ADDR      0
#define CLUE_ADDR_NEXT_SEQ_ADDR(s)      (UINT32)((s)<<CLUE_ADDR_BIT_NEXT_SEQ_ADDR)

// CONTROL REGISTERS definition in 0x000C	// ????
#define CLUE_OPCODE_BIT_LAST_OPCODE      0
#define CLUE_OPCODE_LAST_OPCODE(s)      (UINT32)((s)<<CLUE_OPCODE_BIT_LAST_OPCODE)

// CONTROL REGISTERS definition in 0x0010	//
#define CLUE_STACK_BIT_SLOTS_USED      0
#define CLUE_STACK_SLOTS_USED(s)      (UINT32)((s)<<CLUE_STACK_BIT_SLOTS_USED)

#define CLUE_REASON_NORMAL        0
#define CLUE_REASON_INVALID_OP    1
#define CLUE_REASON_FSTACK_UFLOW  2
#define CLUE_REASON_FSTACK_OFLOW  3
#define CLUE_REASON_WATCHDOG      4
#define CLUE_REASON_STOP_RQST     5
#define CLUE_REASON_PSTACK_UFLOW  6
#define CLUE_REASON_PSTACK_OFLOW  7
#define CLUE_REASON_MEM_PORT_COL  8
#define CLUE_REASON_INVALID_PNT_FAULT 67
#define CLUE_REASON_INVALID_KEY_FAULT 66
#define CLUE_REASON_NO_KEY_FAULT      65
#define CLUE_REASON_NO_EXPONENT_FAULT 64

// CONTROL REGISTERS definition in 0x0020
#define CLUE_STATUS_BIT_BUSY          31
#define CLUE_STATUS_BIT_IRQ           CLUE_INT_BIT_IRQ_EN
#define CLUE_STATUS_BIT_ZERO          28
#define CLUE_STATUS_BIT_STOP_REASON   16
#define CLUE_STATUS_BUSY              (UINT32)(1UL<<CLUE_STATUS_BIT_BUSY)
#define CLUE_STATUS_IRQ               (UINT32)(1UL<<CLUE_STATUS_BIT_IRQ)
#define CLUE_STATUS_ZERO              (UINT32)(1UL<<CLUE_STATUS_BIT_ZERO)
#define CLUE_STATUS_STOP_REASON(s)    (UINT32)((s)<<CLUE_STATUS_BIT_STOP_REASON)

// CONTROL REGISTERS definition in 0x0024
#define CLUE_FLAGS_BIT_F3           7
#define CLUE_FLAGS_BIT_F2           6
#define CLUE_FLAGS_BIT_F1           5
#define CLUE_FLAGS_BIT_F0           4
#define CLUE_FLAGS_BIT_CARRY        3
#define CLUE_FLAGS_BIT_BORROW       2
#define CLUE_FLAGS_BIT_MEMBIT       1
#define CLUE_FLAGS_BIT_ZERO         0
#define CLUE_FLAGS_F3               (UINT32)(1UL<<CLUE_FLAGS_BIT_F3)
#define CLUE_FLAGS_F2               (UINT32)(1UL<<CLUE_FLAGS_BIT_F2)
#define CLUE_FLAGS_F1               (UINT32)(1UL<<CLUE_FLAGS_BIT_F1)
#define CLUE_FLAGS_F0               (UINT32)(1UL<<CLUE_FLAGS_BIT_F0)
#define CLUE_FLAGS_CARRY            (UINT32)(1UL<<CLUE_FLAGS_BIT_CARRY)
#define CLUE_FLAGS_BORROW           (UINT32)(1UL<<CLUE_FLAGS_BIT_BORROW)
#define CLUE_FLAGS_MEMBIT           (UINT32)(1UL<<CLUE_FLAGS_BIT_MEMBIT)
#define CLUE_FLAGS_ZERO             (UINT32)(1UL<<CLUE_FLAGS_BIT_ZERO)

// CONTROL REGISTERS definition in 0x0028
#define CLUE_WATCHDOG_BIT_CYCLES_UNTIL_HALT   0 //default hFFFF FFFF
#define CLUE_WATCHDOG_CYCLES_UNTIL_HALT(s)   (UINT32)((s)<<CLUE_WATCHDOG_BIT_CYCLES_UNTIL_HALT)

// CONTROL REGISTERS definition in 0x002C
#define CLUE_CYCLE_SINCE_GO_BIT_      0
#define CLUE_CYCLE_SINCE_GO_(s)      (UINT32)((s)<<CLUE_CYCLE_SINCE_GO_BIT_)

// CONTROL REGISTERS definition in 0x0030
#define CLUE_INDEX_I_BIT_         0  //0-15
#define CLUE_INDEX_I_(s)         (UINT32)((s)<< CLUE_INDEX_I_BIT_)
// CONTROL REGISTERS definition in 0x0034
#define CLUE_INDEX_J_BIT_         0  //0-15
#define CLUE_INDEX_J_(s)         (UINT32)((s)<< CLUE_INDEX_J_BIT_)
// CONTROL REGISTERS definition in 0x0038
#define CLUE_INDEX_K_BIT_         0  //0-15
#define CLUE_INDEX_K_(s)         (UINT32)((s)<< CLUE_INDEX_K_BIT_)
// CONTROL REGISTERS definition in 0x003C
#define CLUE_INDEX_L_BIT_         0  //0-15
#define CLUE_INDEX_L_(s)         (UINT32)((s)<< CLUE_INDEX_L_BIT_)

#define CLUE_CTRL_REG(mmap)       (UINT32 *)(mmap + CLUE_CTRL)
#define CLUE_PROG_IDX_REG(mmap)   (UINT32 *)(mmap + CLUE_ENTRY)
#define CLUE_STATUS_REG(mmap)     (UINT32 *)(mmap + CLUE_STATUS)

#define CLUE_CODE_MEMORY_START(mmap)   (UINT32 *)(mmap + CLUE_SEQUENCE)
#define CLUE_REGION_A_START(mmap)      (UINT32 *)(mmap + CLUE_OPERAND_RA)
#define CLUE_REGION_B_START(mmap)      (UINT32 *)(mmap + CLUE_OPERAND_RB)
#define CLUE_REGION_C_START(mmap)      (UINT32 *)(mmap + CLUE_OPERAND_RC)
#define CLUE_REGION_D_START(mmap)      (UINT32 *)(mmap + CLUE_OPERAND_RD)
#define CLUE_REGION_A_PAGE(mmap,size,idx)   (UINT32 *)(mmap + CLUE_OPERAND_RA + size*idx)
#define CLUE_REGION_B_PAGE(mmap,size,idx)   (UINT32 *)(mmap + CLUE_OPERAND_RB + size*idx)
#define CLUE_REGION_C_PAGE(mmap,size,idx)   (UINT32 *)(mmap + CLUE_OPERAND_RC + size*idx)
#define CLUE_REGION_D_PAGE(mmap,size,idx)   (UINT32 *)(mmap + CLUE_OPERAND_RD + (UINT32)size*idx)

#define CLUE_A_PAGE_SIZE   512
#define CLUE_B_PAGE_SIZE   512
#define CLUE_C_PAGE_SIZE   512
#define CLUE_D_PAGE_SIZE   1024

#define CLUE_SEQUENCE_SIZE   4096

// for CLUE RSA, 32 bit memory access only!!!
// Basic 32 bit memory access macros
// 32 memory write byte operation
#define CLUE_WRITE_UINT(mem,val)             (*(UINT32 *)(mem)=(UINT32)(val))
// 32 memory read byte operation
#define CLUE_READ_UINT(mem)                  *(volatile UINT32 *)(mem)

// Read registers' values macro
#define CLUE_READ_CTRL(mmap)      CLUE_READ_UINT((UINT32)mmap+CLUE_CTRL)
#define CLUE_READ_STATUS(mmap)    CLUE_READ_UINT((UINT32)mmap+CLUE_STATUS)
#define CLUE_READ_ENTRY(mmap)     CLUE_READ_UINT((UINT32)mmap+CLUE_ENTRY)
#define CLUE_READ_FLAGS(mmap)     CLUE_READ_UINT((UINT32)mmap+CLUE_FLAGS)
#define CLUE_READ_REASON(mmap)    ((CLUE_READ_UINT((UINT32)mmap+CLUE_STATUS) >> CLUE_STATUS_BIT_STOP_REASON) & 0xFF)
#define CLUE_READ_STACK(mmap)     CLUE_READ_UINT((UINT32)mmap+CLUE_STACK)
#define CLUE_READ_CYCLES(mmap)    CLUE_READ_UINT((UINT32)mmap+CLUE_CYCLE_SINCE_GO)
#define CLUE_READ_INSTS(mmap)     CLUE_READ_UINT((UINT32)mmap+CLUE_INST_SINCE_GO)

#define CLUE_READ_I(mmap)  CLUE_READ_UINT((UINT32)mmap+CLUE_INDEX_I)
#define CLUE_READ_J(mmap)  CLUE_READ_UINT((UINT32)mmap+CLUE_INDEX_J)
#define CLUE_READ_K(mmap)  CLUE_READ_UINT((UINT32)mmap+CLUE_INDEX_K)
#define CLUE_READ_L(mmap)  CLUE_READ_UINT((UINT32)mmap+CLUE_INDEX_L)

#define CLUE_CLEAR_REGISTER(mmap)\
   CLUE_WRITE_UINT(((UINT32)mmap+CLUE_CTRL),0);\
   CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0)

#define CLUE_SET_STACK(mmap,val) CLUE_WRITE_UINT(((UINT32)mmap+CLUE_STACK),val)
#define CLUE_CLEAR_STACK(mmap)   CLUE_WRITE_UINT(((UINT32)mmap+CLUE_STACK),0)
#define CLUE_STOP_REQUEST(mmap)  CLUE_WRITE_UINT(((UINT32)mmap+CLUE_CTRL),0x08000000)

#define CLUE_CLEAR_FLAG(mmap)          CLUE_WRITE_UINT(((UINT32)mmap+CLUE_FLAGS),0x0)
#define CLUE_CLEAR_CARRY(mmap)         CLUE_WRITE_UINT(((UINT32)mmap+CLUE_FLAGS),CLUE_READ_FLAGS(mmap)&~CLUE_FLAGS_CARRY)
#define CLUE_SET_ENDIAN_SWAP(mmap)     CLUE_WRITE_UINT(((UINT32)mmap+CLUE_CTRL),CLUE_READ_CTRL(mmap)|CLUE_CTRL_ENDIAN_SWAP)
#define CLUE_CLEAR_ENDIAN_SWAP(mmap)   CLUE_WRITE_UINT(((UINT32)mmap+CLUE_CTRL),CLUE_READ_CTRL(mmap)&~CLUE_CTRL_ENDIAN_SWAP)

#define CLUE_CALC_MP(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x1c)
#define CLUE_R_INV(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x1e)
#define CLUE_R_SQR(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x23)
#define CLUE_DIV_MODP(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x18)
#define CLUE_MOD_ADD(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x14)
#define CLUE_MOD_DIV(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x18)
#define CLUE_MOD_EXP(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0xe)
#define CLUE_MOD_INV(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x18)
#define CLUE_MOD_MULT(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x12)
#define CLUE_MOD_SUB(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x16)
#define CLUE_M_RESIDUE(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x10)
#define CLUE_POINT_ADD(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0xa)
#define CLUE_POINT_DOUBLE(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x8)
#define CLUE_POINT_MULT(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x0)
#define CLUE_POINT_VERIFY(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0xc)
#define CLUE_MOD_RED(mmap)		CLUE_WRITE_UINT(((UINT32)mmap+CLUE_ENTRY),0x1a)

// Acknowledge interrupt macro
#define CLUE_INT_ACK(mmap)      CLUE_WRITE_UINT(((UINT32)mmap+CLUE_STATUS),CLUE_READ_STATUS(mmap)|CLUE_STATUS_IRQ)

// Load control registes and start an operation
// This is NO IRQ MODE , clear flag first CLUE_CLEAR_FLAG(mmap);
#define CLUE_START_OPERATION(mmap, s) CLUE_WRITE_UINT(((UINT32)mmap+CLUE_CTRL),CLUE_CTRL_PARTIAL_RADIX(s)|CLUE_CTRL_GO)

// This is IRQ MODE; Interrupt will be generated upon completion
//#define CLUE_START_OPERATION_INT(mmap, s) CLUE_WRITE_UINT(((UINT32)mmap+CLUE_CTRL),CLUE_CTRL_IRQ_EN|CLUE_CTRL_BASE_RADIX(s)|CLUE_CTRL_GO)
#define CLUE_START_OPERATION_INT(mmap, s) CLUE_WRITE_UINT(((UINT32)mmap+CLUE_CTRL),CLUE_CTRL_BASE_RADIX(s)|CLUE_CTRL_GO)

// Wait loop expire counter
#define CLUE_LOOP_WAIT      10000000

//#endif

//////////////////////////////////////////////////////////////////////////////

#define OP_ENCRYPT						0
#define OP_DECRYPT						1

#define SPACC_CRYPTO_OPERATION			1
#define SPACC_HASH_OPERATION			2

#define SPACC_AADCOPY_FLAG        0x80000000

#define CRYPTO_AES_KEY_LENGTH_128		16
#define CRYPTO_AES_KEY_LENGTH_192		24
#define CRYPTO_AES_KEY_LENGTH_256		32

#define CRYPTO_AES_KEY_LENGTH_MIN			CRYPTO_AES_KEY_LENGTH_128
#define CRYPTO_AES_KEY_LENGTH_MAX			CRYPTO_AES_KEY_LENGTH_256

#define CRYPTO_3DES_KEY_LENGTH				24
#define CRYPTO_3DES_KEY_LENGTH_MIN			CRYPTO_3DES_KEY_LENGTH
#define CRYPTO_3DES_KEY_LENGTH_MAX			CRYPTO_3DES_KEY_LENGTH

#define CRYPTO_DES_KEY_LENGTH            8
#define CRYPTO_DES_KEY_LENGTH_MIN CRYPTO_DES_KEY_LENGTH
#define CRYPTO_DES_KEY_LENGTH_MAX CRYPTO_DES_KEY_LENGTH

#define CRYPTO_DES_IV_LENGTH            8
#define CRYPTO_DES_MIN_BLOCK_SIZE      8
#define CRYPTO_AES_IV_LENGTH            16
#define CRYPTO_AES_MIN_BLOCK_SIZE      16
#define CRYPTO_HASH_SHA1_SIZE            20
#define CRYPTO_HASH_SHA224_SIZE            28
#define CRYPTO_HASH_SHA256_SIZE            32
#define CRYPTO_HASH_SHA384_SIZE            48
#define CRYPTO_HASH_SHA512_SIZE            64
#define CRYPTO_HASH_MD5_SIZE            16

#define CRYPTO_MAC_XCBC_SIZE           16
#define CRYPTO_MAC_CMAC_SIZE           16
#define CRYPTO_MAC_KASUMIF9_SIZE           8
#define CRYPTO_MAC_SNOW3GUIA2_SIZE         4

#define CRYPTO_MODULE_SPACC	0x0001
//#define CRYPTO_MODULE_TRNG	0x0010
//#define CRYPTO_MODULE_PKA	0x0100

// Each module have the same number of contexts
#ifndef CRYPTO_CONTEXTS_MAX
#define CRYPTO_CONTEXTS_MAX    254
#endif

#define SPACC_DRV_IRQ_ENABLED


// EDN-0228 version 2.6
typedef struct tagELPIRQEN
{
    VUINT32 IrqEnCmdEn : 1; // bit 0
    VUINT32 rsv0 : 3; // bit 1~3
    VUINT32 IrqEnStatEn : 1; // bit 4
    VUINT32 rsv1 : 26; // bit 5~30
    VUINT32 IrqEnGlblEn : 1; // bit 31
} ELPIRQEN;

typedef struct _ELP_IRQ_STAT_
{
    VUINT32 IrqStatCmd : 1; // bit 0
    VUINT32 rsv0 : 3; // bit 1~3
    VUINT32 IrqStatStat : 1; // bit 4
    VUINT32 rsv1 : 27; // bit 5~31
} ELPIRQSTAT;

typedef struct _ELP_IRQ_CTL_
{
    VUINT32 IrqEnCmdCnt : 8; // bit 0~7
    VUINT32 rsv0 : 8; // bit 8~15
    VUINT32 IrqEnStatCnt : 9; // bit 16~24
    VUINT32 rsv1 : 7; // bit 25~31
} ELPIRQCTL;

typedef struct _ELP_FIFO_STAT_
{
    VUINT32 FifoStatCmdCnt : 9; // bit 0~8
    VUINT32 rsv0 : 6; // bit 9~14
    VUINT32 FifoStatCmdFull : 1; // bit 15
    VUINT32 FifoStatStatCnt : 9; // bit 16~24
    VUINT32 rsv1 : 6; // bit 9~14
    VUINT32 FifoStatStatEmpty : 1; // bit 31
} ELPFIFOSTAT;

typedef struct _ELP_OFFSET_
{
    VUINT32 OffsetSrc : 16; // bit 0~15
    VUINT32 OffsetDst : 16; // bit 16~31
} ELPOFFSET;

typedef struct _ELP_SW_CTL_
{
    VUINT32 SwId : 8; // bit 0~7
    VUINT32 rsv0 : 22; // bit 8~29
    VUINT32 Priority : 2; // bit 30~31
} ELPSWCTL;

//typedef struct _ELP_CTL_
typedef union _ELP_CTL_
{
    struct
    {
        VUINT32 CiphAlg : 3; // bit 0~2
        VUINT32 rsv0 : 1; // bit 3
        VUINT32 HashAlg : 4; // bit 4~7
        VUINT32 CiphMode : 4; // bit 8~11
        VUINT32 HashMode : 2; // bit 12~13
        VUINT32 MsgBegin : 1; // bit 14
        VUINT32 MsgEnd : 1; // bit 15
        VUINT32 CtxIdx : 8; // bit 16~23
        VUINT32 Encrypt : 1; // bit 24
        VUINT32 AadCopy : 1; // bit 25
        VUINT32 IcvPt : 1; // bit 26
        VUINT32 IcvEnc : 1; // bit 27
        VUINT32 IcvAppend : 1; // bit 28
        VUINT32 KeyExp : 1; // bit 29
        VUINT32 rsv1 : 1; // bit 30
        VUINT32 SecKey : 1; // bit 31
    } bit;

    VUINT32 word;
} ELPCTRL;

typedef struct _ELP_AUX_INFO_
{
    VUINT32 InfoDir : 1; // bit 0
    VUINT32 BitAlign : 3; // bit 1~3
} ELPAUXINFO;

typedef struct _ELP_STATUS_
{
    VUINT32 SwId : 8; // bit 0~7
    VUINT32 rsv : 16; // bit 8~23
    VUINT32 RetCode : 2; // bit 24~25
} ELPSTATUS;

typedef union _ELP_KEY_SZ_
{
    struct
    {
        VUINT32 Size : 7; // bit 0~6
        VUINT32 rsv0 : 1; // bit 7
        VUINT32 CtxIdx : 8; // bit 8~15
        VUINT32 rsv1 : 15; // bit 16~30
        VUINT32 Cipher : 1; // bit 31
    } bit;

    VUINT32 word;
} ELPKEYSZ;

typedef struct tagELPSPACCREGS
{
    volatile ELPIRQEN SpaccRegIrqEn; // 0x0
    volatile ELPIRQSTAT SpaccRegIrqStat; // 0x4
    volatile ELPIRQCTL SpaccRegIrqCtrl; // 0x8
    volatile ELPFIFOSTAT SpaccRegFifoStat; // 0xc
    VUINT32 SpaccRegSdmaBurstSz; // 0x10
    VUINT32 SpaccRegRsv0; // 0x14
    VUINT32 SpaccRegRsv1; // 0x18
    VUINT32 SpaccRegRsv2; // 0x1c
    VUINT32 SpaccRegSrcPtr; // 0x20
    VUINT32 SpaccRegDstPtr; // 0x24
    volatile ELPOFFSET SpaccRegOffset; // 0x28
    VUINT32 SpaccRegAadLen; // 0x2c
    VUINT32 SpaccRegProcLen; // 0x30
    VUINT32 SpaccRegIcvLen; // 0x34
    VUINT32 SpaccRegIcvOffset; // 0x38
    volatile ELPSWCTL SpaccRegSwCtl; // 0x3c
    volatile ELPCTRL SpaccRegCtrl; // 0x40
    VUINT32 SpaccRegRsv3; // 0x44
    VUINT32 SpaccRegRsv4; // 0x48
    volatile ELPAUXINFO SpaccRegAuxInfo; // 0x4c
    VUINT32 SpaccRegStatPop; // 0x50
    volatile ELPSTATUS SpaccRegStatus; // 0x54
    VUINT32 SpaccRegRsvArray[42]; // 0x58~0xfc
    volatile ELPKEYSZ SpaccRegKeySz; // 0x100
} ELPSPACCREGS, *PELPSPACCREGS;

enum ecipher
{
    C_NULL = 0,
    C_DES = 1,
    C_AES = 2,
    C_RC4 = 3,
    C_MULTI2 = 4,
    C_KASUMI = 5,
    C_SNOW3G_UEA2 = 6,
    C_MAX
};

enum eciphermode
{
    CM_ECB = 0,
    CM_CBC = 1,
    CM_CTR = 2,
    CM_CCM = 3,
    CM_GCM = 5,
    CM_OFB = 7,
    CM_CFB = 8,
    CM_F8 = 9,

    CM_MAX
};

enum ehash
{
    H_NULL = 0,
    H_MD5 = 1,
    H_SHA1 = 2,
    H_SHA224 = 3,
    H_SHA256 = 4,
    H_SHA384 = 5,
    H_SHA512 = 6,
    H_XCBC = 7,
    H_CMAC = 8,
    H_KF9 = 9,
    H_SNOW3G_UIA2 = 10,

    H_MAX
};

enum ehashmode
{
    HM_RAW = 0,
    HM_SSLMAC = 1,
    HM_HMAC = 2,

    HM_MAX
};

enum eicvmode
{
    IM_ICV_HASH = 0, /* HASH of plaintext */
    IM_ICV_HASH_ENCRYPT = 1, /* HASH the plaintext and encrypt the lot */
    IM_ICV_ENCRYPT_HASH = 2, /* HASH the ciphertext */
    IM_ICV_IGNORE = 3,

    IM_MAX
};

enum eicvpos
{

    IP_ICV_OFFSET = 0,
    IP_ICV_APPEND = 1,
    IP_ICV_IGNORE = 2,

    IP_MAX
};

enum spacc_ret_code
{
    SPACC_OK = 0,
    SPACC_ICVFAIL,
    SPACC_MEMERR,
    SPACC_BLOCKERR,
};

enum crypto_modes
{
    CRYPTO_MODE_NULL,
    CRYPTO_MODE_RC4_40,
    CRYPTO_MODE_RC4_128,
    CRYPTO_MODE_AES_ECB,        // 3
    CRYPTO_MODE_AES_CBC,        // 4
    CRYPTO_MODE_AES_CTR,        // 5
    CRYPTO_MODE_AES_CCM,        // 6
    CRYPTO_MODE_AES_GCM,        // 7
    CRYPTO_MODE_AES_F8,         // 8
    CRYPTO_MODE_MULTI2_ECB,     // 9
    CRYPTO_MODE_MULTI2_CBC,     //10
    CRYPTO_MODE_MULTI2_OFB,     // 11
    CRYPTO_MODE_MULTI2_CFB,     // 12
    CRYPTO_MODE_3DES_CBC,       // 13
    CRYPTO_MODE_3DES_ECB,       // 14
    CRYPTO_MODE_DES_CBC,        // 15
    CRYPTO_MODE_DES_ECB,        // 16
    CRYPTO_MODE_KASUMI_ECB,     // 17
    CRYPTO_MODE_KASUMI_F8,      // 18
    CRYPTO_MODE_SNOW3G_UEA2,    // 19

    CRYPTO_MODE_HASH_SHA1,      // 20
    CRYPTO_MODE_HMAC_SHA1,      // 21
    CRYPTO_MODE_HASH_MD5,       // 22
    CRYPTO_MODE_HMAC_MD5,       // 23
    CRYPTO_MODE_HASH_SHA224,    // 24
    CRYPTO_MODE_HMAC_SHA224,    // 25
    CRYPTO_MODE_HASH_SHA256,    // 26
    CRYPTO_MODE_HMAC_SHA256,    // 27
    CRYPTO_MODE_HASH_SHA384,    // 28
    CRYPTO_MODE_HMAC_SHA384,    // 29
    CRYPTO_MODE_HASH_SHA512,    // 30
    CRYPTO_MODE_HMAC_SHA512,    // 31

    CRYPTO_MODE_MAC_XCBC,       // 32
    CRYPTO_MODE_MAC_CMAC,       // 33
    CRYPTO_MODE_MAC_KASUMI_F9,  // 34
    CRYPTO_MODE_MAC_SNOW3G_UIA2,// 35

    CRYPTO_MODE_SSLMAC_MD5,
    CRYPTO_MODE_SSLMAC_SHA1,
};

enum
{
    SPACC_DEQUEUE_MAP = 0,
    SPACC_DEQUEUE_UNMAP
};

typedef struct tagDDT_DESCR
{
    void *map;
    void *buf;
    UINT32 len;
} DDT_DESCR, *PDDT_DESCR;

typedef struct tagDDT_ENTRY
{
    UINT32 ptr;
    UINT32 len;
} DDT_ENTRY, *PDDT_ENTRY;

typedef struct descDDT
{
    UINT8 *ptr;
    UINT32 len;
} DESC_DDT, *PDESC_DDT;

typedef struct tagDDTList
{
    struct tagDDTList *next;
    DESC_DDT ddt;
}DDT_LST, *PDDT_LST;

#define MAX_DDT_ENTRIES 20

typedef struct tagCRYPTO_VECTOR_DATA
{
    int enc_mode;
    int hash_mode;
    int keysize;
    int hmac_keysize;

    int iv_offset;
    int iv_size;

    int icv_mode;
    int icv_offset;
    int icv_size;
    int icvpos; // this is set interally it's the flag of how the ICV offset is loaded

    int salt_size;
    int flags;
    VUINT32 errors;
    int fail_mode;
    int outsplit[10];
    int insplit[10];

    int pre_aad_size;
    int post_aad_size;
    int pt_size;
    int ct_size;

    int auxinfo_dir;
    int auxinfo_bit_align;
    unsigned long seed;

    unsigned char key[128];
    unsigned char hmac_key[128];
    unsigned char iv[128];
    unsigned char saltkey[128];

    unsigned char *pre_aad;
    unsigned char *post_aad;
    unsigned char *pt;
    unsigned char *ct;
    int error_field, error_mangle, error_offset;    
    PDDT_LST  ptLst;
    PDDT_LST  ctLst;
    UINT32 dir;             // direction: encrypt/decrypt
    void (*cbReqComp)(void *);
    volatile unsigned int execTicks;
    int handle;
    volatile unsigned int done; 
    unsigned int asynchMode;
} CRYPTO_VECTOR_DATA, *PCRYPTO_VECTOR_DATA;

//  crypto module context descriptor
typedef struct tagCRYPTOCTX
{
    SINT32 taken;
    UINT32 enc_mode; // Encription Algorith mode
    UINT32 hash_mode; // HASH Algorith mode
    UINT32 hmac_keysz; // HMAC Key size
    UINT32 icv_len;
    UINT32 icv_offset;
    UINT32 op; // Operation
    ELPCTRL ctrl; // CTRL shadoe register
    UINT32 first_use; // indicates that context just has been initialized/taken
    // and this is the first use
    SINT32 curr_handle; // handle currently is been processed by the hardware
    UINT32 *ctx; // crypto module context pointer
    UINT32 ctx_offset; // crypto module context OFFSET
    UINT32 out_size; // output data size for non-blocking operation
    UINT32 aad_sz; // size of AAD for the latest packet
    ELPKEYSZ hkey_sz, ckey_sz;
    UINT32 job_id, job_done, job_err, pkt_sz;
    UINT32 *ciph_key, *hash_key, *rc4_key;
    UINT32 auxinfo_dir, auxinfo_bit_align; // Direction and bit alignment parameters for the AUX_INFO reg

    //  struct elp_operand	op_src;	/* source operand */
    //  struct elp_operand	op_dst; 	/* destination operand */
    //  struct cryptop *crp;

    void *ddt_map, *sddt_map; // physical (mapped) DDT address
    DDT_ENTRY *ddt, *sddt;
    DDT_DESCR ddt_desc[MAX_DDT_ENTRIES]; // assosiated user bugffers
    DDT_DESCR sddt_desc[MAX_DDT_ENTRIES]; // assosiated user bugffers
    UINT32 ddt_len; //  total length of the chain
    int ddt_idx; // next free ddt index
    UINT32 sddt_len; //  total length of the chain
    int sddt_idx; // next free ddt index
    void *in_buf, *in_map; // real (physical) and virtual address of the input buffer
    int in_sz;
    void *out_buf, *out_map; // real (physical) and virtual address of the input buffer
    int out_sz;
    PCRYPTO_VECTOR_DATA pVecData;
} CRYPTOCTX, *PCRYPTOCTX;


#define DDT_ENTRY_SIZE (sizeof(DDT_ENTRY)*MAX_DDT_ENTRIES)
#define MAX_SPACC_DIAG_TIMEOUT      150*1000*100  // 100 ms
typedef struct tagSPACCDRVCTX
{
    volatile PELPSPACCREGS  pSpaccRegs;
    UINT32                  cpuID;
    PCRYPTOCTX              mctx;
    UINT32                  irqModeEnabled;
    VUINT32                 popped;
    BOOL                    bInited;
    volatile UINT32         lock;
} SPACCDRVCTX, *PSPACCDRVCTX;

enum
{
    FAILURE_FLAG = 0,
    MULTI2_FLAG = 1,
    AUXINFO_FLAG = 2,
};

enum
{
    ERROR_KEY = 0,
    ERROR_IV = 1,
    ERROR_SALT_KEY = 2,
    ERROR_ICV_KEY = 3,
    ERROR_INPUT = 4,
};


typedef struct tSPACC_DIAG_CTX
{
    SINGLELIST  hL;
    CRYPTO_VECTOR_DATA vec; // vector data
    UINT32  dir;            // direction: encrypt/decrypt
    UINT32  offset;         // buffer offset
    UINT32  mode;           // data mode: 0, 1, 2

} SPACC_DIAG_CTX, *PSPACC_DIAG_CTX;

enum spacc_diag_modes
{
    SPACC_DIAG_AES_ECB_ENC = 0, 
    SPACC_DIAG_AES_CTR_ENC,
    SPACC_DIAG_AES_CMAC_HASH,
    SPACC_DIAG_SNOW3G_UEA2_ENC,
    SPACC_DIAG_SNOW3G_UIA2_HASH,
    SPACC_DIAG_HMAC_SHA256_HASH,
	
    SPACC_DIAG_MAX,
};

typedef struct sSPACC_DIAG_MODES_DESC
{
    enum spacc_diag_modes mode;
    UINT8 text[20];
    UINT32 numTestSets;
}SPACC_DIAG_MODES_DESC, *PSPACC_DIAG_MODES_DESC;

extern const UINT8 snow3guea2_uia2hash744[];
extern UINT8 key_aes_ecb[][16];
extern UINT8 pt_aes_ecb[][16];
extern UINT8 ct_aes_ecb[][16];

#define SPACC_DIAG_AES_ECB_NUM_TEST_SETS    1

extern UINT8 key_aes_ctr[][16];
extern UINT8 count_aes_ctr[][4];
extern UINT8 bearer_aes_ctr[];
extern UINT8 direction_aes_ctr[];
extern UINT32 length_aes_ctr[];
extern UINT8 pt_aes_ctr[][1024];
extern UINT8 ct_aes_ctr[][1024];

#define SPACC_DIAG_AES_CTR_NUM_TEST_SETS    1

extern UINT8 key_aes_cmac[][16];
extern UINT8 count_aes_cmac[][4];
extern UINT8 bearer_aes_cmac[];
extern UINT8 direction_aes_cmac[];
extern UINT32 length_aes_cmac[];
extern UINT8 pt_aes_cmac[][1024];
extern UINT8 ct_aes_cmac[][4];

#define SPACC_DIAG_AES_CMAC_NUM_TEST_SETS    5

extern UINT8 count_uea2[][4];
extern UINT8 bearer_uea2[];
extern UINT32 length_uea2[];
extern UINT8 direction_uea2[];
extern UINT8 key_snow3g_uea2[][16];
extern UINT8 pt_snow3g_uea2[][32];
extern UINT8 ct_snow3g_uea2[][32];

#define SPACC_DIAG_UEA2_NUM_TEST_SETS    1

extern UINT8 key_snow3g_uia2[][16];
extern UINT8 count_uia2[][4];
extern UINT8 fresh_uia2[][4];
extern UINT32 direction_uia2[];
extern UINT32 length_uia2[];
extern UINT8 ct_snow3g_uia2[][4];
extern UINT8 pt_snow3g_uia2[][4096];

#define SPACC_DIAG_UIA2_NUM_TEST_SETS    7

extern UINT8 key_hmac_sha256[][4];
extern UINT8 pt_hmac_sha256[][16];
extern UINT8 ct_hmac_sha256[][32];
extern UINT32 length_hmac_sha256[];

#define SPACC_DIAG_HMAC_SHA256_NUM_TEST_SETS    1
/******************************************************************************
 *                The API declaration                                          *
 *******************************************************************************/

MXRC SPAccDrvInit(void);
MXRC SPAccDrvShutdown(void);
MXRC SPAccDiagInit(UINT32* pReportData, UINT32* pReportSize);
UINT32 SPAccDiagTest(UINT32 mode, UINT32 nRepeats, UINT32 *len, 
                            UINT32 *avgSpeed, UINT32 *maxSpeed, UINT32 *ticks,
                            char *cmd, UINT32 isAsynchMode, char *rslt);
MXRC SPAccDrvRun(PCRYPTO_VECTOR_DATA inVecData);
void SPAccDiagDoneAPICb(void *p);
void SPAccDrvIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam);
MXRC SPAccDrvRunBlocking(PCRYPTO_VECTOR_DATA inVecData);
PCRYPTO_VECTOR_DATA SPAccDiagCreateCryptVec(UINT32 mode, UINT32 testSet);
MXRC SPAccDiagDeleteCryptVec(PCRYPTO_VECTOR_DATA inVecData);
void SPAccDiagPrintHelp(void);
PSPACC_DIAG_CTX SPACCDiagGetCtxById(UINT32 ctxId);
void SPAccDiagDoneCb(void *p);

MXRC spacc_open(SINT32 type, SINT32 mode);
MXRC spacc_close(SINT32 handle);
MXRC spacc_release_ddt(SINT32 handle);
MXRC spacc_add_ddt(SINT32 handle, UINT8 *data, UINT32 len);
MXRC spacc_release_dst_ddt(SINT32 handle);
MXRC spacc_add_dst_ddt(SINT32 handle, UINT8 *data, UINT32 len);
MXRC spacc_set_context(SINT32 handle, SINT32 op, UINT8 * key, SINT32 ksz, UINT8 * iv, SINT32 ivsz);
MXRC spacc_get_context(SINT32 handle, UINT8 * iv, SINT32 ivsize, SINT32 * psize);
MXRC spacc_set_operation(SINT32 handle, SINT32 op, UINT32 prot, UINT32 icvpos, UINT32 icvoff, UINT32 icvsz);
MXRC spacc_set_auxinfo(SINT32 handle, UINT32 direction, UINT32 bitsize);
MXRC spacc_packet_partial_ddt(SINT32 handle, UINT32 pay_sz, UINT32 aad_sz, UINT32 prio, UINT32 chunk_size, UINT32 *outsz);
MXRC spacc_packet_enqueue_ddt(PSPACCDRVCTX pElpCtx, SINT32 handle, UINT32 proc_sz, UINT32 aad_offset, UINT32 aad_sz, UINT32 prio);
MXRC spacc_packet_enqueue(PSPACCDRVCTX pElpCtx, SINT32 handle, UINT8 *in_buf, UINT32 in_sz, UINT8 *out_buf, UINT32 out_sz, UINT32 aad_sz, UINT32 prio);
MXRC spacc_packet_dequeue(PSPACCDRVCTX pElpCtx, SINT32 handle, UINT32 * size, int demap);
MXRC spacc_status(SINT32 handle, UINT8 * buf, SINT32 * size);
UINT32 spacc_get_version(void);
void spacc_enable_int(void);
void spacc_disable_int(void);
CRYPTOCTX *context_lookup(SINT32 handle);
PCRYPTOCTX spacc_get_context_done(PSPACCDRVCTX pElpCtx);
MXRC spacc_packet_dequeue_irq(PSPACCDRVCTX pElpCtx, PCRYPTOCTX ctx, UINT32 * size, int demap);
void SPAccMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif /* __SPACCDRV_H__ */

#ifdef __cplusplus
}
#endif

