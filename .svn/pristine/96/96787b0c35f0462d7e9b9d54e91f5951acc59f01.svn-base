//-------------------------------------------------------------------------------------------
/** @file spaccdrv.c
 *
 * @brief SPAcc driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.20 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "spaccdrv.h"
#include "heaps.h"
#include "diagnostics.h"
#include "systimers.h"
#include "appprintf.h"
#include "supervisor.h"
#include "mmumgr.h"
#ifdef SPACC_DRV_ENABLED

SPACCDRVCTX gSPAccDrvCtx = {0};
PSPACCDRVCTX pSPAccDrvCtx = NULL;
HANDLE hSpaccDiagList;

__align (32) DDT_ENTRY NCNBVAR ddtentry[MAX_DDT_ENTRIES];
__align (32) DDT_ENTRY NCNBVAR sddtentry[MAX_DDT_ENTRIES];


PUINT32 pGlobalMem = NULL;
static UINT32 hostId;

SPACC_DIAG_CTX g_SpaccDiagCtx;

#define MODE_ENCRYPT 0
#define MODE_HASH    1

const int context_modes[2][32] =
{
{ CRYPTO_MODE_NULL, CRYPTO_MODE_RC4_40, // 1
  CRYPTO_MODE_RC4_128, CRYPTO_MODE_AES_ECB, // 3
  CRYPTO_MODE_AES_CBC, CRYPTO_MODE_AES_CTR, CRYPTO_MODE_AES_CCM, CRYPTO_MODE_AES_GCM,
  CRYPTO_MODE_AES_F8, CRYPTO_MODE_MULTI2_ECB, CRYPTO_MODE_MULTI2_CBC, CRYPTO_MODE_MULTI2_OFB,
  CRYPTO_MODE_MULTI2_CFB, CRYPTO_MODE_3DES_CBC, // 13
  CRYPTO_MODE_3DES_ECB, // 14
  CRYPTO_MODE_DES_CBC, // 15
  CRYPTO_MODE_DES_ECB, // 16
  CRYPTO_MODE_KASUMI_ECB, CRYPTO_MODE_KASUMI_F8, CRYPTO_MODE_SNOW3G_UEA2 },
{ CRYPTO_MODE_NULL, CRYPTO_MODE_HASH_SHA1, CRYPTO_MODE_HMAC_SHA1, CRYPTO_MODE_HASH_MD5,
  CRYPTO_MODE_HMAC_MD5, CRYPTO_MODE_HASH_SHA224, CRYPTO_MODE_HMAC_SHA224,
  CRYPTO_MODE_HASH_SHA256, CRYPTO_MODE_HMAC_SHA256, CRYPTO_MODE_HASH_SHA384,
  CRYPTO_MODE_HMAC_SHA384, CRYPTO_MODE_HASH_SHA512, CRYPTO_MODE_HMAC_SHA512,
  CRYPTO_MODE_MAC_XCBC, CRYPTO_MODE_MAC_CMAC, CRYPTO_MODE_MAC_KASUMI_F9,
  CRYPTO_MODE_SSLMAC_MD5, CRYPTO_MODE_SSLMAC_SHA1, CRYPTO_MODE_MAC_SNOW3G_UIA2,
}

};

enum
{
    DIR_ENC=0,
    DIR_DEC
};

#ifdef DIAG_SPACC_ENABLED

SPACC_DIAG_MODES_DESC mode_desc[] =
{
    {SPACC_DIAG_AES_ECB_ENC,        "aesecb_enc",       SPACC_DIAG_AES_ECB_NUM_TEST_SETS},
    {SPACC_DIAG_AES_CTR_ENC,        "aesctr_enc",       SPACC_DIAG_AES_CTR_NUM_TEST_SETS},        
    {SPACC_DIAG_AES_CMAC_HASH,      "aescmac_hash",     SPACC_DIAG_AES_CMAC_NUM_TEST_SETS},            
    {SPACC_DIAG_SNOW3G_UEA2_ENC,    "snow3guea2_enc",   SPACC_DIAG_UEA2_NUM_TEST_SETS},            
    {SPACC_DIAG_SNOW3G_UIA2_HASH,   "snow3guia2_hash",  SPACC_DIAG_UIA2_NUM_TEST_SETS},        
    {SPACC_DIAG_HMAC_SHA256_HASH,   "hmacsha256_hash",  SPACC_DIAG_HMAC_SHA256_NUM_TEST_SETS},  	
};

   unsigned char spaccCmdText[] =   "USAGE: spacc -m {mode} -r {numRepeat} -s {testSet} -a -f\r\n"
                                    "-r: set number of repeats\r\n"
                                    "{numRepeat} - the number of repeats\r\n" 
                                    "1 - by default\r\n\r\n"
                                    "-s: set number of test sets\r\n"
                                    "{testSet} - the number of test sets\r\n" 
                                    "0 - by default\r\n\r\n"
                                    "-a: set asynchronous mode of SPAcc driver\r\n"
                                    "sync mode - by default\r\n\r\n"
                                    "-f: use full possible modes and testSets\r\n\r\n"
                                    "-m: set mode for testing\r\n\r\n"
                                    "-c: set mask of cores where will be run test sets (1 by default)\r\n\r\n"                                    
                                    "{mode} :\r\n";

void SPAccDiagPrintHelp(void)
{
   UINT32 i = 0;
    
    uart_printf("%s", spaccCmdText);

    for (i = 0; i < SPACC_DIAG_MAX; ++i)
        uart_printf("%d --> %s number of test sets: %d \r\n", i, mode_desc[i].text, mode_desc[i].numTestSets);

    uart_printf("\r\n\r\n");
}
   

int getNum(int length, UINT8* pInput)
{
    unsigned int temp, i;
    char c;
    temp = 0;
    if (length == 0)
        return 0;
    for (i = 0; i < length; i+=1)
    {
        c = *pInput++;
        temp |= (c & 0xFF) << 8*(length - 1 - i);
    }
    return temp;
}

void getChars(UINT8* buffer, int length, UINT8* pInput)
{
    int i;
    for (i = 0; i < length; i+= 1)
    {
        char c = *pInput++;
        buffer[i] = c & 0xFF;
    }
}

int getSplits(int * splits, UINT8* pInput)
{
    int numofsplits = 0;
    int i = 0;

    numofsplits = *pInput++;
    for (i = 0; i < numofsplits; i += 1)
    {
        splits[i] = getNum(4, pInput);
        pInput += 4;
        if (splits[i] == -1)
        {
            return -1;
        }
    }
    return (numofsplits*4 + 1);
}

int free_test_vector(PSPACCDRVCTX pCtx, CRYPTO_VECTOR_DATA *vector)
{
    if (vector->ct)
        MxHeapFree(NcnbHeap, (PUINT32)vector->ct);
    if (vector->pt)
        MxHeapFree(NcnbHeap,(PUINT32)vector->pt);
    if (vector->post_aad)
        MxHeapFree(NcnbHeap,(PUINT32)vector->post_aad);
    if (vector->pre_aad)
        MxHeapFree(NcnbHeap,(PUINT32)vector->pre_aad);

    return 0;
}

int parse_test_vector(UINT8 *pInput)
{
    int y=0;
    CRYPTO_VECTOR_DATA *vector;
    int tmp;
    //UINT8 *pInput;

    vector = &g_SpaccDiagCtx.vec;
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->seed = getNum(4, pInput);
    //uart_printf("seed:%x \n", vector->seed);
    pInput += 4;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->flags = getNum(1, pInput++);
    //uart_printf("flag: %x\n", vector->flags);
    tmp = vector->flags;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->errors = getNum(1, pInput++);
    //uart_printf("errors: %x\n", vector->errors);

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->error_field = getNum(1, pInput++);
    //uart_printf("error_field: %x\n", vector->error_field);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->error_mangle = getNum(1, pInput++);
    //uart_printf("error_mangle: %x\n", vector->error_mangle);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->error_offset = getNum(4, pInput);
    //uart_printf("error_field: %x error_mangle: %x error_offset: %x\n", vector->error_field, vector->error_mangle, vector->error_offset);
    pInput += 4;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->enc_mode = getNum(1, pInput++);
    //uart_printf("enc_mode: %x\n", vector->enc_mode);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->hash_mode = getNum(1, pInput++);
    //uart_printf("hash_mode: %x\n", vector->hash_mode);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->icv_mode = getNum(1, pInput++);
    //uart_printf("icv_mode: %x\n", vector->icv_mode);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->iv_offset = getNum(4, pInput);
    //uart_printf("iv_offset: %x\n", vector->iv_offset);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    pInput += 4;

    if (vector->iv_offset == 0xFFFFFFFF)
    {
        vector->iv_offset = 0;
    }
    else
    {
        //vector->iv_offset |= 0x80000000;   // if we are using it we must enable the high bit
    }

//    if ((vector->flags >> AUXINFO_FLAG) & 0x01) {
    if (tmp & 0x04)
    {
        vector->auxinfo_bit_align = getNum(1, pInput++);
        vector->auxinfo_dir = getNum(1, pInput++);
    }
    else
    {
        vector->auxinfo_bit_align = 8;
        vector->auxinfo_dir = 0;
    }

    //uart_printf("auxinfo_bit_align: %x auxinfo_dir: %x\n", vector->auxinfo_bit_align, vector->auxinfo_dir);

    //if ((vector->flags >> FAILURE_FLAG) & 0x01) {
    if (tmp & 0x01)
    {
        vector->fail_mode = 1;
    }
    else
    {
        vector->fail_mode = 0;
    }
    //uart_printf("fail_mode: %x \n", vector->fail_mode);


    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->keysize = getNum(1, pInput++);
    //uart_printf("keysize: %x\n", vector->keysize);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    getChars(vector->key, vector->keysize, pInput);
    pInput += vector->keysize;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->hmac_keysize = getNum(1, pInput++);
    //uart_printf("hmac_keysize: %x\n", vector->hmac_keysize);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    getChars(vector->hmac_key, vector->hmac_keysize, pInput);
    pInput += vector->hmac_keysize;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->iv_size = getNum(1, pInput++);
    //uart_printf("iv_size: %x\n", vector->iv_size);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    getChars(vector->iv, vector->iv_size, pInput);
    pInput += vector->iv_size;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->salt_size = getNum(1, pInput++);
    //uart_printf("salt_size: %x\n", vector->salt_size);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    getChars(vector->saltkey, vector->salt_size, pInput);
    pInput += vector->salt_size;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->pre_aad_size = getNum(4, pInput);
    //uart_printf("pre_aad_size: %x\n", vector->pre_aad_size);
    pInput += 4;
    if (vector->pre_aad_size)
    {
        vector->pre_aad = (unsigned char*)MxHeapAlloc(NcnbHeap, vector->pre_aad_size); //kmalloc( vector->pre_aad_size, GFP_ATOMIC);
        //uart_printf("pInput:%x \n", (UINT32)pInput);
        getChars(vector->pre_aad, vector->pre_aad_size, pInput);
        pInput += vector->pre_aad_size;
    }

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->post_aad_size = getNum(4, pInput);
    //uart_printf("post_aad_size: %x\n", vector->post_aad_size);
    pInput += 4;
    if (vector->post_aad_size)
    {
        //uart_printf("pInput:%x \n", (UINT32)pInput);
        vector->post_aad = (unsigned char*)MxHeapAlloc(NcnbHeap,vector->post_aad_size); //kmalloc( vector->post_aad_size, GFP_ATOMIC);
        getChars(vector->post_aad, vector->post_aad_size, pInput);
        pInput += vector->post_aad_size;
    }

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->pt_size = getNum(4, pInput);
    //uart_printf("pt_size: %x\n", vector->pt_size);
    pInput += 4;
    vector->pt = (unsigned char*)MxHeapAlloc(NcnbHeap,vector->pt_size); //kmalloc( vector->data_size_in, GFP_ATOMIC);
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    getChars(vector->pt, vector->pt_size, pInput);
    pInput += vector->pt_size;

    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->ct_size = getNum(4, pInput);
    //uart_printf("ct_size: %x\n", vector->ct_size);
    pInput += 4;
    //uart_printf("pInput:%x \n", (UINT32)pInput);
    vector->ct = (unsigned char*)MxHeapAlloc(NcnbHeap,vector->ct_size); //kmalloc( vector->data_size_out, GFP_ATOMIC);
    getChars(vector->ct, vector->ct_size, pInput);
    pInput += vector->ct_size;

    vector->icv_size = vector->ct_size - vector->pt_size;
    vector->icv_offset = vector->pt_size;
    //uart_printf("icv_size: %x icv_offset: %x\n", vector->icv_size, vector->icv_offset);

    //elp_fclose(in);
    return y;
}

MXRC SPAccDiagInit(UINT32* pReportData, UINT32* pReportSize)
{
    PSPACCDRVCTX pCtx = pSPAccDrvCtx;
    unsigned char *OUTCT;
    UINT32 OUTCTsize;
    int handle, err = 0;
    UINT32 y = 0, timer = 0;

    extern const unsigned char snow3guea2_uia2hash744[];

    if ((pCtx == NULL) || !pCtx->bInited)
    {
        y = SPAccDrvInit();

        if (y)
            return y;
    }
    memset(&g_SpaccDiagCtx, 0, sizeof(g_SpaccDiagCtx));
    g_SpaccDiagCtx.dir = OP_DECRYPT;

    err = parse_test_vector((PUINT8)snow3guea2_uia2hash744);

    if (err != 0)
        return err;

    if ((handle = spacc_open(context_modes[MODE_ENCRYPT][g_SpaccDiagCtx.vec.enc_mode],
                            context_modes[MODE_HASH][g_SpaccDiagCtx.vec.hash_mode])) < 0)
    {
        y = SPACC_RC_CRYPTO_OPEN_ERROR;
        return y;
    }

   g_SpaccDiagCtx.vec.handle = handle;
   ((PCRYPTOCTX)(context_lookup(handle)))->pVecData = &g_SpaccDiagCtx.vec;

    OUTCT = (unsigned char*)MxHeapAlloc(NcnbHeap,g_SpaccDiagCtx.vec.ct_size);
     if (OUTCT == NULL)
    {
        _ASSERT_PTR(OUTCT);       
        return SPACC_RC_INVALID_ALLOC;
    }

    memset(OUTCT, 0, g_SpaccDiagCtx.vec.ct_size);

    // take the salt as the first part of the key for F8 mode
    if (context_modes[MODE_ENCRYPT][g_SpaccDiagCtx.vec.enc_mode] == CRYPTO_MODE_AES_F8)
    {
        unsigned char tmp[128];

        memcpy(tmp, g_SpaccDiagCtx.vec.saltkey, g_SpaccDiagCtx.vec.keysize);
        memcpy(tmp + g_SpaccDiagCtx.vec.keysize, g_SpaccDiagCtx.vec.key, g_SpaccDiagCtx.vec.keysize);
        memcpy(g_SpaccDiagCtx.vec.key, tmp, g_SpaccDiagCtx.vec.keysize<<1);
    }

    if ((g_SpaccDiagCtx.vec.enc_mode > 0) &&
        (err = spacc_set_context(handle, SPACC_CRYPTO_OPERATION,
                    g_SpaccDiagCtx.vec.key, g_SpaccDiagCtx.vec.keysize, g_SpaccDiagCtx.vec.iv, g_SpaccDiagCtx.vec.iv_size))
        != SPACC_RC_OK)
    {
        y = err;
        return y;
    }

    if ((g_SpaccDiagCtx.vec.hash_mode > 0) &&
        (err = spacc_set_context(handle, SPACC_HASH_OPERATION,
                    g_SpaccDiagCtx.vec.hmac_key, g_SpaccDiagCtx.vec.hmac_keysize, 0, 0))
        != SPACC_RC_OK)
    {
        y = err;
        return y;
    }

    if (g_SpaccDiagCtx.vec.icv_offset == -1)
    {
        g_SpaccDiagCtx.vec.icvpos = IP_ICV_APPEND;
    }
    else
    {
        g_SpaccDiagCtx.vec.icvpos = IP_ICV_OFFSET;
    }

    err = spacc_set_operation(
        handle,
        g_SpaccDiagCtx.dir==DIR_ENC ? OP_ENCRYPT : OP_DECRYPT,
        g_SpaccDiagCtx.vec.icv_mode, g_SpaccDiagCtx.vec.icvpos,
        g_SpaccDiagCtx.dir == DIR_ENC ? g_SpaccDiagCtx.vec.icv_offset
            : (g_SpaccDiagCtx.vec.pre_aad_size + g_SpaccDiagCtx.vec.pt_size + g_SpaccDiagCtx.vec.post_aad_size),
        g_SpaccDiagCtx.vec.icv_size);
    if (err != SPACC_RC_OK)
    {
        y = err;
        return err;
    }

    // add sources
    if (g_SpaccDiagCtx.vec.pre_aad)
    {
        if ((err = spacc_add_ddt(handle, g_SpaccDiagCtx.vec.pre_aad, g_SpaccDiagCtx.vec.pre_aad_size))
            != SPACC_RC_OK)
        {
            y = err;
            return err;
        }
    }

    if (g_SpaccDiagCtx.dir == DIR_ENC)
    {
        if ((err = spacc_add_ddt(handle, g_SpaccDiagCtx.vec.pt, g_SpaccDiagCtx.vec.pt_size))
            != SPACC_RC_OK)
        {
            y = err;
            return err;
        }
    }
    else
    {
        // we add pt size because the ICV must go after the post aad
        if ((err = spacc_add_ddt(handle, g_SpaccDiagCtx.vec.ct, g_SpaccDiagCtx.vec.pt_size))
            != SPACC_RC_OK)
        {
            y = err;
            return err;
        }
    }

    if (g_SpaccDiagCtx.vec.post_aad)
    {
        if ((err = spacc_add_ddt(handle, g_SpaccDiagCtx.vec.post_aad, g_SpaccDiagCtx.vec.post_aad_size))
            != SPACC_RC_OK)
        {
            y = err;
            return err;
        }
    }

    // in decrypt mode we stick the icv after all this nonsense
    if (g_SpaccDiagCtx.dir == DIR_DEC && g_SpaccDiagCtx.vec.ct_size != g_SpaccDiagCtx.vec.pt_size)
    {
        if ((err = spacc_add_ddt(handle,
                        g_SpaccDiagCtx.vec.ct + g_SpaccDiagCtx.vec.pt_size,
                        g_SpaccDiagCtx.vec.ct_size - g_SpaccDiagCtx.vec.pt_size))
            != SPACC_RC_OK)
        {
            y = err;
            return err;
        }
    }

    // add destinations
    if ((err = spacc_add_dst_ddt(handle,
                    OUTCT,
                    g_SpaccDiagCtx.dir == DIR_ENC ? g_SpaccDiagCtx.vec.ct_size : g_SpaccDiagCtx.vec.pt_size))
        != SPACC_RC_OK)
    {
        y = err;
        return y;
    }

    // only set the auxiliary register if this is Kasumi F8/F9 or Snow3G UIA2 mode
    if (context_modes[MODE_HASH][g_SpaccDiagCtx.vec.hash_mode] == CRYPTO_MODE_MAC_KASUMI_F9
        || context_modes[MODE_ENCRYPT][g_SpaccDiagCtx.vec.enc_mode] == CRYPTO_MODE_KASUMI_F8
        || context_modes[MODE_ENCRYPT][g_SpaccDiagCtx.vec.enc_mode] == CRYPTO_MODE_SNOW3G_UEA2
        || context_modes[MODE_HASH][g_SpaccDiagCtx.vec.hash_mode] == CRYPTO_MODE_MAC_SNOW3G_UIA2)
    {
        if ((err = spacc_set_auxinfo(handle, g_SpaccDiagCtx.vec.auxinfo_dir, g_SpaccDiagCtx.vec.auxinfo_bit_align))
            != SPACC_RC_OK)
        {
            y = err;
            return y;
        }
    }

    if ((err = spacc_packet_enqueue_ddt(pCtx, handle, 0, 0, g_SpaccDiagCtx.vec.pre_aad_size, SPACC_SW_ID_PRIO_HI))
        != SPACC_RC_OK)
    {
        y = err;
        return y;
    }

    OUTCTsize = g_SpaccDiagCtx.vec.ct_size;

    timer = SysTimerGetTick();    

    while (((err = spacc_packet_dequeue(pCtx, handle, &OUTCTsize, SPACC_DEQUEUE_UNMAP))
        == SPACC_RC_CRYPTO_INPROGRESS) && ((SysTimerGetTick() - timer) < MAX_SPACC_DIAG_TIMEOUT))
    {
    }

    if (err != SPACC_RC_OK)
    {
        y = err;
        return y;
    }

    y = 0;

    // zero out unused bits in the last byte returned of the stream cipher for SNOW3G and Kasumi
    if (g_SpaccDiagCtx.vec.auxinfo_bit_align != 8
        && (g_SpaccDiagCtx.vec.enc_mode == CRYPTO_MODE_SNOW3G_UEA2
            || g_SpaccDiagCtx.vec.hash_mode == CRYPTO_MODE_MAC_KASUMI_F9
            || g_SpaccDiagCtx.vec.hash_mode == CRYPTO_MODE_MAC_SNOW3G_UIA2)
        && g_SpaccDiagCtx.vec.post_aad_size == 0)
    {
        if (g_SpaccDiagCtx.dir == DIR_ENC)
        {
            OUTCT[g_SpaccDiagCtx.vec.ct_size - g_SpaccDiagCtx.vec.icv_size - 1] &= 0xFF << (8 - g_SpaccDiagCtx.vec.auxinfo_bit_align);
        }
        else
        {
            OUTCT[g_SpaccDiagCtx.vec.pt_size - 1] &= 0xFF << (8 - g_SpaccDiagCtx.vec.auxinfo_bit_align);
        }
    }

    if (g_SpaccDiagCtx.dir == DIR_ENC)
    {
        if (memcmp(OUTCT, g_SpaccDiagCtx.vec.ct, g_SpaccDiagCtx.vec.ct_size))
        {
            y = SPACC_RC_CRYPTO_COMPARE_ERROR;
        }
    }
    else
    {
        if (memcmp(OUTCT, g_SpaccDiagCtx.vec.pt, g_SpaccDiagCtx.vec.pt_size))
        {
            y = SPACC_RC_CRYPTO_COMPARE_ERROR2;
        }
    }

    if (handle >= 0)
    {
        spacc_close(handle);
    }

    // free the allocated buffers
    MxHeapFree(NcnbHeap,(PUINT32)OUTCT);

    free_test_vector(pCtx, &g_SpaccDiagCtx.vec);

    return y;
}

MXRC SPAccDiagSetContext(PApiHeader pApi)
{
    PApiParam pParam = ApiGetParamByID(pApi, DIAG_CONTEXT_PARAM_ID);
    UINT32 len;
    PSPACC_DIAG_CTX pDiagCtx = NULL;

    if (pParam == NULL)
        return SPACC_RC_INVALID_PARAM;

    len = ApiGetParamDataSize(pParam);

    if (len != sizeof(pDiagCtx->vec) + sizeof(pDiagCtx->dir))
        return SPACC_RC_INVALID_PARAM;

    pDiagCtx = (PSPACC_DIAG_CTX) MxHeapAlloc (NcnbHeap, sizeof(SPACC_DIAG_CTX));
    if (!pDiagCtx)
    {
        uart_printf("SPAccDiagSetContext: pElpCtx allocation error\r\n");
        _ASSERT_PTR(pDiagCtx);
    }

    memcpy(&pDiagCtx->dir, pParam->ParamData, sizeof(pDiagCtx->dir));     // direction: enc/dec
    memcpy(&pDiagCtx->vec, pParam->ParamData + 1, sizeof(pDiagCtx->vec)); // vector data

	if (pDiagCtx->vec.pt_size)
	    pDiagCtx->vec.pt = (unsigned char*)MxHeapAlloc(NcnbHeap,pDiagCtx->vec.pt_size);
    
	if (pDiagCtx->vec.ct_size)	
		pDiagCtx->vec.ct = (unsigned char*)MxHeapAlloc(NcnbHeap,pDiagCtx->vec.ct_size);

    if (pDiagCtx->vec.pre_aad_size)
    {
        pDiagCtx->vec.pre_aad = (unsigned char*)MxHeapAlloc(NcnbHeap,pDiagCtx->vec.pre_aad_size);
    }
    if (pDiagCtx->vec.post_aad_size)
    {
        pDiagCtx->vec.post_aad = (unsigned char*)MxHeapAlloc(NcnbHeap,pDiagCtx->vec.post_aad_size);
    }

    MxAddListElm(hSpaccDiagList, pDiagCtx);
        
    return SPACC_RC_OK;
}

MXRC SPAccDiagSetInput(PApiHeader pApi)
{
    PApiParam pParam = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
    UINT32 ctxId = 0;
    PSPACC_DIAG_CTX pDiagCtx = NULL;

    if (pParam == NULL)
        return SPACC_RC_INVALID_PARAM;    

    ctxId = pParam->ParamData[0];
    
    pDiagCtx = SPACCDiagGetCtxById(ctxId);
	if 	(!pDiagCtx)
	{
		_ASSERT_PTR(pDiagCtx);
	}

    pParam  = ApiGetParamByID(pApi, DIAG_DATA_PARAM_ID);
    
//    if (API_IS_EXTENDED(pApi))
    {
//        PExtApiHeader pExtApiHdr = (PExtApiHeader)pApi;
//        UINT32 Length = pApi->Length - (sizeof(ExtApiHeader) - sizeof(ApiHeader));
//        PUINT8 pDst = NULL, pSrc = GetExtApiPayload(pExtApiHdr);
        
        UINT32 Length = ApiGetParamDataSize(pParam);
        PUINT8 pDst = NULL, pSrc =(PUINT8) &pParam->ParamData[0];

    	pDiagCtx->offset = 0;

//        if (pDiagCtx->offset == 0)
        {
            pDiagCtx->mode = (*pSrc << 24) | (*(pSrc + 1) << 16) | (*(pSrc + 2) << 8) | *(pSrc + 3);
            pSrc += sizeof(UINT32);
            Length -= sizeof(UINT32);
            switch (pDiagCtx->mode)
            {
            case 0: // input
                pDst = (pDiagCtx->dir == DIR_ENC ? pDiagCtx->vec.pt : pDiagCtx->vec.ct) + pDiagCtx->offset;
                break;
            case 1: // pre aad
                pDst = pDiagCtx->vec.pre_aad + pDiagCtx->offset;
                break;
            case 2: // post aad
                pDst = pDiagCtx->vec.post_aad + pDiagCtx->offset;
                break;
            default:
                return SPACC_RC_INVALID_PARAM;
            }
        }

        _memcpy(pDst, pSrc, Length);
    }

    return SPACC_RC_OK;
}


MXRC SPAccDiagRun(PApiHeader pApi, PUINT32 pCycleNum)
{
    MXRC err;
    PApiParam pParam = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
    UINT32 ctxId = 0;
    PSPACC_DIAG_CTX pDiagCtx = NULL;

    if (pParam == NULL)
        return SPACC_RC_INVALID_PARAM;    

    ctxId = pParam->ParamData[0];

    pDiagCtx = SPACCDiagGetCtxById(ctxId);
    if (!pDiagCtx)
    {   
        return SPACC_RC_INVALID_PARAM;
    }    

    err = SPAccDrvRunBlocking(&pDiagCtx->vec);

    return err;
}

MXRC SPAccDiagRunAsync(PApiHeader pApi)
{
    MXRC err;
    PApiParam pParam = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
    UINT32 ctxId = 0;
    PSPACC_DIAG_CTX pDiagCtx = NULL;

    if (pParam == NULL)
        return SPACC_RC_INVALID_PARAM;    

    ctxId = pParam->ParamData[0];

    pDiagCtx = SPACCDiagGetCtxById(ctxId);
    if (!pDiagCtx)
    {   
        return SPACC_RC_INVALID_PARAM;
    }

    hostId  = pApi->SrcInstID;

    pDiagCtx->vec.asynchMode = 1;
    pDiagCtx->vec.cbReqComp = SPAccDiagDoneAPICb;

    err = SPAccDrvRun(&pDiagCtx->vec);

    return err;
}

MXRC SPAccDiagGetOutput(PApiHeader pApi, PApiHeader pResp)
{
    PApiParam pParam;
    UINT32 nSize, nOffs;
    MXRC rc = SPACC_RC_OK;
    UINT32 ctxId = 0;
    PSPACC_DIAG_CTX pDiagCtx = NULL;
    UINT32 nOutSize;
    PUINT8 pOut;

    pParam = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID); // size

    if (pParam == NULL)
    {
        return SPACC_RC_INVALID_PARAM;
    }

    ctxId = pParam->ParamData[0];

    pDiagCtx = SPACCDiagGetCtxById(ctxId);  
    if (!pDiagCtx)
    {   
        return SPACC_RC_INVALID_PARAM;
    }      
    
    nOutSize = (pDiagCtx->dir == DIR_ENC ? pDiagCtx->vec.ct_size : pDiagCtx->vec.pt_size);
    pOut = (pDiagCtx->dir == DIR_ENC ? pDiagCtx->vec.ct : pDiagCtx->vec.pt);

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID); // size

    if (pParam == NULL)
    {
        return SPACC_RC_INVALID_PARAM;
    }

    nSize = pParam->ParamData[0];

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);

    if (pParam == NULL)
    {
        return SPACC_RC_INVALID_PARAM;
    }

    nOffs = pParam->ParamData[0];

    if (nSize + nOffs > nOutSize)
    {
        uart_printf("SPACCDIAG: GetOutputParamError !\r\n");
        uart_printf("OFFS:%d, SIZE:%d, TOTAL:%d\r\n", nOffs, nSize, nOutSize);

        if (nOffs >= nOutSize)
        {
            return SPACC_RC_INVALID_PARAM;
        }

        nSize = nOutSize - nOffs;
    }

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID, pOut + nOffs, nSize);


    return rc;
}

MXRC SPAccDiagGetContext(PApiHeader pApi, PApiHeader pResp)
{
    UINT32 ctxId = 0;
    PSPACC_DIAG_CTX pDiagCtx = NULL;
    PApiParam pParam;    

    pParam = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);

    if (pParam == NULL)
    {
        return SPACC_RC_INVALID_PARAM;
    }

    ctxId = pParam->ParamData[0];
 
    pDiagCtx = SPACCDiagGetCtxById(ctxId);  
    if (!pDiagCtx)
    {   
        return SPACC_RC_INVALID_PARAM;
    }  

    ApiAddParam(pResp, NULL, DIAG_CONTEXT_PARAM_ID, (LPVOID)((PUINT32)&pDiagCtx->vec - 1), sizeof(pDiagCtx->vec) + 4); // добавлем old_dir к передаче.

    // reverse order
    if (pDiagCtx->vec.post_aad)
        MxHeapFree(NcnbHeap,(PUINT32)pDiagCtx->vec.post_aad);
    if (pDiagCtx->vec.pre_aad)
        MxHeapFree(NcnbHeap,(PUINT32)pDiagCtx->vec.pre_aad);
    MxHeapFree(NcnbHeap,(PUINT32)pDiagCtx->vec.ct);
    MxHeapFree(NcnbHeap,(PUINT32)pDiagCtx->vec.pt);

    
    MxDelListElm(hSpaccDiagList, pDiagCtx);

    MxHeapFree(NcnbHeap,pDiagCtx);

    return SPACC_RC_OK;
}

APIHEC SPAccDiagApiHandler(LPVOID pCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = SPACC_RC_OK;
    UINT32 nCycles;

    if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        pResp->Type = API_TYPE_CFG_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_RUN:
                rc = SPAccDiagRun(pApi, &nCycles);
                break;
            case DIAG_RUNASYNC:
                rc = SPAccDiagRunAsync(pApi);
                break;
            case DIAG_CONTEXT:
                rc = SPAccDiagSetContext(pApi);
                break;
            case DIAG_CONTROL:
                break;
            case DIAG_INPUT:
                rc = SPAccDiagSetInput(pApi);
                break;
            default:
                rc = SPACC_RC_UNKNOWN_API;
                break;
        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {
        pResp->Type = API_TYPE_QRY_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_CONTEXT:
                rc = SPAccDiagGetContext(pApi,pResp);
                break;
            case DIAG_CONTROL:
                break;
            case DIAG_INPUT:
                break;
            case DIAG_OUTPUT:
                rc = SPAccDiagGetOutput(pApi, pResp);
                break;
            default:
                rc = SPACC_RC_UNKNOWN_API;
                break;
        }

        if (rc == SPACC_RC_OK)
            return APIHEC_OK;
    }
    else
    {
        rc = SPACC_RC_UNKNOWN_TYPE;
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}

PSPACC_DIAG_CTX SPACCDiagGetCtxById(UINT32 ctxId)
{
    UINT32 count = 0;
    PSPACC_DIAG_CTX pElm = NULL;

    count = MxGetListElmCount(hSpaccDiagList);

    pElm = (PSPACC_DIAG_CTX) MxGetListFirstElm(hSpaccDiagList);

    while(count --)
    {
        if (pElm->vec.seed == ctxId)
            return pElm;

        pElm = MxGetListNextElm(hSpaccDiagList, pElm);
    }

    return NULL;
}
void SPAccDiagDoneAPICb(void *p)
{
    MXRC rc = SPACC_RC_OK;
    PCRYPTO_VECTOR_DATA pVecData = (PCRYPTO_VECTOR_DATA) p;
    ApiHeader * pApi = SvsrAllocICApi();
    pVecData->done = 1;

    pApi = ApiCreate(pApi,
                     API_DEF_FLAGS,
                     0x8025,
                     hostId,
                     API_TYPE_IND,
                     DIAG_RUNASYNC);

   
    ApiAddParam(pApi, NULL, DIAG_TASK_PARAM_ID, &pVecData->seed, sizeof(pVecData->seed));

    ApiAddErrorCode(pApi, pVecData->errors);
    
    // to send created API command to the host
    rc = SvsrSendICApi(pApi);
    if (rc != MX_OK)
    {
        uart_printf("SPAccDiagDoneAPICb: pApi = 0x%x\r\n", pApi);
        _ASSERT_RC(rc);    
    }
    
}

UINT32 SPAccDiagTest(UINT32 mode, UINT32 nRepeats, UINT32 *len, 
                            UINT32 *avgSpeed, UINT32 *maxSpeed, UINT32 *ticks,
                            char *cmd, UINT32 isAsynchMode, char *rslt)
{
    UINT32 timer = 0;
    UINT32 y = 0;
    PDDT_LST pPt = NULL;
    DDT_LST *currDDT;
    UINT32 cmpLen,currCmpLen;
    UINT8 *pDataOutRef;
    UINT32 speed = 0;
    UINT32 testSet = *len;
    PCRYPTO_VECTOR_DATA pVec = NULL;

    *ticks = 0;
    *maxSpeed = 0;
    *avgSpeed = 0;
    
    while (nRepeats--)
    {
        
        pVec = SPAccDiagCreateCryptVec(mode, testSet);

        if (pVec == NULL)
        {
            uart_printf("SPAccDiagTest: can't allocate memory for PCRYPTO_VECTOR_DATA \r\n");
            _ASSERT(0);
        }

        pVec->asynchMode = isAsynchMode;
    
        switch (mode)
        {

            case SPACC_DIAG_AES_ECB_ENC:
                memcpy (cmd, mode_desc[mode].text, sizeof(mode_desc[mode].text));

                cmpLen = sizeof(ct_aes_ecb[testSet]);
                pDataOutRef = ct_aes_ecb[testSet];
                break;

            case SPACC_DIAG_AES_CTR_ENC:
                memcpy (cmd, mode_desc[mode].text, sizeof(mode_desc[mode].text));

                cmpLen = pPt->ddt.len;
                pDataOutRef = ct_aes_ctr[testSet];
                break;

            case SPACC_DIAG_AES_CMAC_HASH:
                memcpy (cmd, mode_desc[mode].text, sizeof(mode_desc[mode].text));

                cmpLen = sizeof(ct_aes_cmac[testSet]);
                pDataOutRef = ct_aes_cmac[testSet];
                break;

                case SPACC_DIAG_SNOW3G_UEA2_ENC:
                memcpy (cmd, mode_desc[mode].text, sizeof(mode_desc[mode].text));

                cmpLen = length_uea2[testSet]>> 3;
                pDataOutRef = ct_snow3g_uea2[testSet];
                break;
                
             case SPACC_DIAG_SNOW3G_UIA2_HASH:
                memcpy (cmd, mode_desc[mode].text, sizeof(mode_desc[mode].text));
    
                cmpLen = sizeof(ct_snow3g_uia2[testSet]);
                pDataOutRef = ct_snow3g_uia2[testSet];
             break;

             case SPACC_DIAG_HMAC_SHA256_HASH:
                memcpy (cmd, mode_desc[mode].text, sizeof(mode_desc[mode].text));
    
                cmpLen = sizeof(ct_hmac_sha256[testSet]);
                pDataOutRef = ct_hmac_sha256[testSet];
             break;
                
               default: 
                    continue;

                
        }


        if (!pVec->asynchMode)
        {

            y = SPAccDrvRunBlocking(pVec);
           
       }else
       {
            pVec->cbReqComp = SPAccDiagDoneCb;

            SPAccDrvRun(pVec);

            timer = SysTimerGetTick();
            while ( (!pVec->done) && ((SysTimerGetTick() - timer) < MAX_SPACC_DIAG_TIMEOUT) );

            if (!pVec->done)
            {
                uart_printf("SPAccDiagTest:CPU #%d crypto task wasn't done!\r\n", MxGetCpuID());
            }
        }

        if (pVec->errors)
        {
           sprintf(rslt, "fail[rc=0x%x]", pVec->errors);
           *ticks = 0;
           *avgSpeed = 0;
           *maxSpeed = 0;
        }else 
        {
            currDDT = (pVec->dir == OP_ENCRYPT)? pVec->ctLst : pVec->ptLst;

            do
            {
                if (currDDT->ddt.len <= cmpLen)
                {
                    currCmpLen = currDDT->ddt.len;
                    cmpLen -= currDDT->ddt.len;
                }
                else 
                    currCmpLen = cmpLen;

                if (memcmp(currDDT->ddt.ptr, pDataOutRef, currCmpLen))
                {
                    sprintf(rslt, "fail[rc=0x%x]", SPACC_RC_CRYPTO_COMPARE_ERROR);

                    y = SPACC_RC_CRYPTO_COMPARE_ERROR;
                }
                else
                {
                    sprintf(rslt, "OK");
                }

                pDataOutRef += currCmpLen;

                currDDT = currDDT->next;
                
            }while(currDDT);


            *len = 0;
            currDDT = (pVec->dir == OP_ENCRYPT)?pVec->ptLst: pVec->ctLst;
            while(currDDT)
            {
                *len += currDDT->ddt.len;                
                currDDT = currDDT->next;
            }

            speed  = (*len*8*150)/(pVec->execTicks);
  
            *avgSpeed = (*avgSpeed == 0) ? speed : (*avgSpeed + speed )/ 2;
            *maxSpeed = (*maxSpeed < speed) ? speed : *maxSpeed;
            *ticks += pVec->execTicks;
            
        }

            SPAccDiagDeleteCryptVec(pVec);

            if (y == SPACC_RC_CRYPTO_COMPARE_ERROR)
                break;

        }
        
    return y;

}

PCRYPTO_VECTOR_DATA SPAccDiagCreateCryptVec(UINT32 mode, UINT32 testSet)
{
    PDDT_LST pPt = NULL, pCt = NULL;
    PCRYPTO_VECTOR_DATA pVec = NULL;

    pVec = (PCRYPTO_VECTOR_DATA) MxHeapAlloc (NcnbHeap, sizeof(CRYPTO_VECTOR_DATA));

    if (pVec == NULL)
    {
        uart_printf("sSPAccDiagCreateCryptVec: can't allocate memory for PCRYPTO_VECTOR_DATA \r\n");
        _ASSERT(0);
    }
    memset(pVec, 0x00, sizeof(CRYPTO_VECTOR_DATA));    

    pVec->ptLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));
    pPt = pVec->ptLst;
    _ASSERT_PTR(pPt); 

    memset (pVec->ptLst, 0, sizeof(DDT_LST));

    pVec->ctLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));            
    pCt = pVec->ctLst;
    _ASSERT_PTR(pCt);                        

    memset (pVec->ctLst, 0, sizeof(DDT_LST));
    
    switch (mode)
    {
        case SPACC_DIAG_AES_ECB_ENC:
            pVec->dir = OP_ENCRYPT;
            pVec->enc_mode = CRYPTO_MODE_AES_ECB;         
            pVec->hash_mode = NULL;

            pVec->keysize = sizeof(key_aes_ecb[testSet]);
            memcpy(pVec->key, key_aes_ecb[testSet], pVec->keysize);

            pPt->next = NULL;
            pPt->ddt.len = sizeof(pt_aes_ecb[testSet]);
            pPt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pPt->ddt.len);
            _ASSERT_PTR(pPt->ddt.ptr);            
            memcpy (pPt->ddt.ptr, (unsigned char *) pt_aes_ecb[testSet], pPt->ddt.len);            

            pCt->next = NULL;
            pCt->ddt.len = pPt->ddt.len;
            pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
            _ASSERT_PTR(pCt->ddt.ptr);
            break;

        case SPACC_DIAG_AES_CTR_ENC:
            pVec->dir = OP_ENCRYPT;
            pVec->enc_mode = CRYPTO_MODE_AES_CTR;         
            pVec->hash_mode = NULL;

            pVec->keysize = sizeof(key_aes_ctr[testSet]);
            memcpy(pVec->key, key_aes_ctr[testSet], pVec->keysize);

            REG32(&pVec->iv[0]) = REG32(count_aes_ctr[testSet]);
            REG32(&pVec->iv[4]) = (bearer_aes_ctr[testSet]<< 3) | (direction_aes_ctr[testSet]<< 2);        
            memset(&pVec->iv[8], 0x00, 8);
            pVec->iv_size = 16;

            pPt->next = NULL;
            pPt->ddt.len = length_aes_ctr[testSet] % 8 ? length_aes_ctr[testSet]/8 +1: length_aes_ctr[testSet]/8;
            pPt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pPt->ddt.len);
            _ASSERT_PTR(pPt->ddt.ptr);            
            memcpy (pPt->ddt.ptr, (unsigned char *) pt_aes_ctr[testSet], pPt->ddt.len);                  
            

            pCt->next = NULL;
            pCt->ddt.len = pPt->ddt.len;
            pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
            _ASSERT_PTR(pCt->ddt.ptr);
            break;

        case SPACC_DIAG_AES_CMAC_HASH:
            pVec->dir = OP_ENCRYPT;
            pVec->enc_mode = NULL;
            pVec->hash_mode = CRYPTO_MODE_MAC_CMAC;

            pVec->hmac_keysize = sizeof(key_aes_cmac[testSet]);
            memcpy(&pVec->hmac_key[0], &key_aes_cmac[testSet], pVec->hmac_keysize);

            pVec->icv_mode = IM_ICV_IGNORE;
            pVec->icvpos = IP_ICV_IGNORE;

            pPt->ddt.len = 8;
            pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
            _ASSERT_PTR(pPt->ddt.ptr);
            memset(pPt->ddt.ptr, 0x00, pPt->ddt.len);
            REG32(&pPt->ddt.ptr[0]) = REG32(&count_aes_cmac[testSet][0]);
            pPt->ddt.ptr[4] = (bearer_aes_cmac[testSet]<< 3) | (direction_aes_cmac[testSet]<< 2); 

            pCt->ddt.len = pPt->ddt.len;

            pPt->next = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));;

            pPt = pPt->next;
            pPt->next = NULL;
            pPt->ddt.len = length_aes_cmac[testSet]% 8 ? length_aes_cmac[testSet]/8 +1: length_aes_cmac[testSet]/8;
            pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
            _ASSERT_PTR(pPt->ddt.ptr);
            memcpy(pPt->ddt.ptr,(UINT8 *) pt_aes_cmac[testSet], pPt->ddt.len);

            pCt->next = NULL;
            pCt->ddt.len += pPt->ddt.len;
            pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
            _ASSERT_PTR(pCt->ddt.ptr);

            pPt = pVec->ptLst;
        break;

        case SPACC_DIAG_SNOW3G_UEA2_ENC:
            pVec->dir = OP_ENCRYPT;
            pVec->enc_mode = CRYPTO_MODE_SNOW3G_UEA2;
            pVec->hash_mode = NULL;

            REG32(&pVec->key[12]) = REG32(&key_snow3g_uea2[testSet][0]);
            REG32(&pVec->key[8]) = REG32(&key_snow3g_uea2[testSet][4]);
            REG32(&pVec->key[4]) = REG32(&key_snow3g_uea2[testSet][8]);
            REG32(&pVec->key[0]) = REG32(&key_snow3g_uea2[testSet][12]);

            REG32(&pVec->key[20]) = REG32(&count_uea2[testSet][0]);
            pVec->key[16] = (bearer_uea2[testSet]<< 3) | (direction_uea2[testSet]<< 2);
            REG32(&pVec->key[28]) = REG32(&count_uea2[testSet][0]);
            pVec->key[24] = (bearer_uea2[testSet]<< 3) | (direction_uea2[testSet]<< 2);        

            pVec->keysize = 32;

            pVec->icv_mode = IM_ICV_IGNORE;
            pVec->icvpos = IP_ICV_IGNORE;

            pPt->next = NULL;
            pPt->ddt.len = length_uea2[testSet]% 8 ? length_uea2[testSet]/8 +1: length_uea2[testSet]/8;
            pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
            _ASSERT_PTR(pPt->ddt.ptr);
            memcpy(pPt->ddt.ptr,(UINT8 *) pt_snow3g_uea2[testSet], pPt->ddt.len);

            pCt->next = NULL;
            pCt->ddt.len = pPt->ddt.len;
            pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
            _ASSERT_PTR(pCt->ddt.ptr);
            break;

        case SPACC_DIAG_SNOW3G_UIA2_HASH:
            pVec->dir = OP_ENCRYPT;
            pVec->enc_mode = NULL;
            pVec->hash_mode = CRYPTO_MODE_MAC_SNOW3G_UIA2;

            pVec->auxinfo_bit_align =  length_uia2[testSet]& 0x7;

            REG32(&pVec->hmac_key[12]) = REG32(&key_snow3g_uia2[testSet][0]);
            REG32(&pVec->hmac_key[8]) = REG32(&key_snow3g_uia2[testSet][4]);
            REG32(&pVec->hmac_key[4]) = REG32(&key_snow3g_uia2[testSet][8]);
            REG32(&pVec->hmac_key[0]) = REG32(&key_snow3g_uia2[testSet][12]);

            REG32(&pVec->hmac_key[20]) = REG32(&count_uia2[testSet][0]);
            pVec->hmac_key[20] = (count_uia2[testSet][0] ^ direction_uia2[testSet] << 7);

            REG32(&pVec->hmac_key[16]) = REG32(&fresh_uia2[testSet][0]);
            pVec->hmac_key[18] = (pVec->hmac_key[18] ^ direction_uia2[testSet] << 7); 
            REG32(&pVec->hmac_key[28]) = REG32(&count_uia2[testSet][0]);
            REG32(&pVec->hmac_key[24]) = REG32(&fresh_uia2[testSet][0]);

            pVec->hmac_keysize= 32;

            pVec->icv_mode = IM_ICV_IGNORE;
            pVec->icvpos = IP_ICV_IGNORE;

            pPt->next = NULL;
            pPt->ddt.len = length_uia2[testSet]% 8 ? length_uia2[testSet]/8 +1: length_uia2[testSet]/8;
            pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
            _ASSERT_PTR(pPt->ddt.ptr);
            memcpy(pPt->ddt.ptr,(UINT8 *) pt_snow3g_uia2[testSet], pPt->ddt.len);
            
            pCt->next = NULL;
            pCt->ddt.len = pPt->ddt.len;
            pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
            _ASSERT_PTR(pCt->ddt.ptr);
         break;

        case SPACC_DIAG_HMAC_SHA256_HASH:
            pVec->dir = OP_ENCRYPT;
            pVec->enc_mode = NULL;
            pVec->hash_mode = CRYPTO_MODE_HMAC_SHA256;

            pVec->hmac_keysize = sizeof(key_hmac_sha256[testSet]);
            memcpy(pVec->hmac_key, &key_hmac_sha256[testSet][0], pVec->hmac_keysize);

            pVec->icv_mode = IM_ICV_IGNORE;
            pVec->icvpos = IP_ICV_IGNORE;

            pPt->next = NULL;
            pPt->ddt.len = length_hmac_sha256[testSet];
            pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
            _ASSERT_PTR(pPt->ddt.ptr);
            memcpy(pPt->ddt.ptr,(UINT8 *) pt_hmac_sha256[testSet], pPt->ddt.len);
            
            pCt->next = NULL;
            if (pPt->ddt.len < 32)
                pCt->ddt.len = 32;
            else
                pCt->ddt.len = pPt->ddt.len;
            
            pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
            _ASSERT_PTR(pCt->ddt.ptr);
         break;


       default:
        break;
        }

        return pVec;

}

MXRC SPAccDiagDeleteCryptVec(PCRYPTO_VECTOR_DATA inVecData)
{
    PDDT_LST delDDT = NULL, nextDDT = NULL;

    if (inVecData)
    {   
        delDDT = inVecData->ptLst;

        while(delDDT)
        {
            if (delDDT->ddt.ptr != NULL)
                HeapFreeByPtr((PUINT32)delDDT->ddt.ptr);
            
            nextDDT = delDDT->next;

            HeapFreeByPtr(delDDT);
            
            delDDT  = nextDDT;
        }

        delDDT = inVecData->ctLst;

        while(delDDT)
        {
            if (delDDT->ddt.ptr != NULL)
                HeapFreeByPtr((PUINT32)delDDT->ddt.ptr);
            
            nextDDT = delDDT->next;

            HeapFreeByPtr(delDDT);
            
            delDDT  = nextDDT;
        }

        HeapFreeByPtr(inVecData);
    }

    return MX_OK;
    
}

UINT32 SPAccDiagGetNumTestSets(UINT32 mode)
{
    return mode_desc[mode].numTestSets;
}

UINT32 SPAccDiagGetNumModes(void)
{
    return SPACC_DIAG_MAX;
}

void SPAccDiagDoneCb(void *p)
{
    PCRYPTO_VECTOR_DATA pVecData = (PCRYPTO_VECTOR_DATA) p;

    pVecData->done = 1;
}



#endif

// Initialize crypto context for all implemented
// hardware crypto modules.
// mmap:  start of the crypto hardware physical memory

MXRC SPAccDrvInit(void)
{
    int x;
    volatile PELPSPACCREGS pSpaccRegs;
    MXRC rc = SPACC_OK;

    //uart_printf("SPAcc driver init ... ");

    pSPAccDrvCtx = &gSPAccDrvCtx;

    if (!pSPAccDrvCtx->bInited )
    {
        pSPAccDrvCtx->pSpaccRegs = (PELPSPACCREGS)SPACC_BASEADDR;

        pSpaccRegs = pSPAccDrvCtx->pSpaccRegs;

        REG32(CFG_SYS_CFG_2) = 1; // SPAcc and ESPAH Little Endian

        // Initialize all global structures

        pSPAccDrvCtx->mctx = (CRYPTOCTX *)MxHeapAlloc(AppHeap, sizeof(CRYPTOCTX)*CRYPTO_CONTEXTS_MAX);
        if(pSPAccDrvCtx->mctx == NULL)
        {
            rc = SPACC_RC_INVALID_ALLOC;
            uart_printf("FAIL: pSPAccDrvCtx->mctx allocation error, rc = 0x%x\r\n", rc);
            return rc;
        }

        memset((unsigned char *)pSPAccDrvCtx->mctx, 0, sizeof(CRYPTOCTX)*CRYPTO_CONTEXTS_MAX);

        if (pSpaccRegs->SpaccRegFifoStat.FifoStatStatEmpty == 0)
            return SPACC_RC_CRYPTO_INIT_ERROR;

        for (x = 0; x < CRYPTO_CONTEXTS_MAX; x++)
        {
            pSPAccDrvCtx->mctx[x].ciph_key = (PUINT32)(SPACC_BASEADDR + SPACC_CTX_CIPH_KEY + x * SPACC_CTX_CIPH_PAGE);
            pSPAccDrvCtx->mctx[x].hash_key = (PUINT32)(SPACC_BASEADDR + SPACC_CTX_HASH_KEY + x * SPACC_CTX_HASH_PAGE);
            pSPAccDrvCtx->mctx[x].rc4_key = (PUINT32)(SPACC_BASEADDR + SPACC_CTX_RC4_CTX + x * SPACC_CTX_RC4_PAGE);
            pSPAccDrvCtx->mctx[x].job_id = 0xFFFFFFFF;
            pSPAccDrvCtx->mctx[x].job_err = SPACC_RC_CRYPTO_INPROGRESS;
            pSPAccDrvCtx->mctx[x].taken = 0;

            if (x < 1)
            {
                pSPAccDrvCtx->mctx[x].taken = 1;
            }

            pSPAccDrvCtx->mctx[x].ddt = pSPAccDrvCtx->mctx[x].ddt_map = (DDT_ENTRY *)&ddtentry[0];
            pSPAccDrvCtx->mctx[x].sddt = pSPAccDrvCtx->mctx[x].sddt_map = (DDT_ENTRY *)&sddtentry[0];

        }

        pSPAccDrvCtx->cpuID = MxGetCpuID();

#ifdef DIAG_SPACC_ENABLED
        rc = SvsrRegClient(IID_SPACC_DIAG, SPAccDiagApiHandler, NULL);
        rc = MxCreateList (SINGLE_LINKED_LIST, "SpaccDiagList", HNULL, &hSpaccDiagList);
#endif
#ifdef SPACC_DRV_IRQ_ENABLED
        pSPAccDrvCtx->pSpaccRegs->SpaccRegIrqEn.IrqEnGlblEn = 1;
        pSPAccDrvCtx->pSpaccRegs->SpaccRegIrqEn.IrqEnStatEn = 1;
        pSPAccDrvCtx->pSpaccRegs->SpaccRegIrqCtrl.IrqEnStatCnt = 1;
        MxSetIntHandler(HAL_IRQ_SPAcc, INT_TYPE_IRQ, PRI_SPAC, SPAccDrvIrqHandler, pSPAccDrvCtx);
        MxEnableInt(HAL_IRQ_SPAcc);
        pSPAccDrvCtx->irqModeEnabled = 1;
#endif
        pSPAccDrvCtx->bInited = TRUE;
    }

    //uart_printf((rc == SPACC_OK)?"OK \r\n":"FAIL, rc = 0x%x\r\n", rc);
    return rc;
}

MXRC SPAccDrvShutdown(void)
{
    if(pSPAccDrvCtx == NULL)
        return SPACC_RC_OK;

    if (MxGetCpuID() != pSPAccDrvCtx->cpuID)
        return MX_OK;

    if (pSPAccDrvCtx->bInited == 0)
        return MX_OK;

   if(pSPAccDrvCtx->mctx)
    {
        MxHeapFree(AppHeap, pSPAccDrvCtx->mctx);
        pSPAccDrvCtx->mctx = NULL;
    }

    pSPAccDrvCtx->bInited = 0;

    memset(pSPAccDrvCtx, 0x00, sizeof(SPACCDRVCTX));

    return SPACC_RC_OK;
}

void SPAccDrvIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    PSPACCDRVCTX pElpCtx = (PSPACCDRVCTX) pParam;
    PCRYPTOCTX  cryptoCtx = NULL;
    UINT32 err = 0, OUTCTsize = 0;
    
    MxGetLock(&pElpCtx->lock);

    pElpCtx->pSpaccRegs->SpaccRegIrqStat.IrqStatStat= 1;
    *pIntClearReg = nIrqID;

    if (!pElpCtx->irqModeEnabled)
    {
        uart_printf("SPAccDrvIrqHandler error: irq generation is wrong\r\n");
        return;
    }

    while (!pElpCtx->pSpaccRegs->SpaccRegFifoStat.FifoStatStatEmpty)
    {

        cryptoCtx =(PCRYPTOCTX) spacc_get_context_done(pElpCtx);

        if (!cryptoCtx)
        {
            uart_printf("SPAccDrvIrqHandler: cryptoCtx is NULL\r\n");        
            continue;
        }

        if (cryptoCtx->pVecData->asynchMode)
        {
            if ((err = spacc_packet_dequeue_irq(pElpCtx, cryptoCtx, &OUTCTsize, SPACC_DEQUEUE_UNMAP))  
                != SPACC_RC_OK)
            {
                uart_printf("SPAccDrvIrqHandler: Attention the task wasn't done err= %x\r\n", err);
            }

            cryptoCtx->pVecData->execTicks = SysTimerGetTick() - cryptoCtx->pVecData->execTicks;

            cryptoCtx->pVecData->errors = err;

            spacc_close(cryptoCtx->pVecData->handle);

            if (cryptoCtx->pVecData->cbReqComp)
                cryptoCtx->pVecData->cbReqComp(cryptoCtx->pVecData);
        }else
        {
            pElpCtx->popped = 1;    // just for diagnostic
        }

     }

    MxReleaseLock(&pElpCtx->lock);   

    

}


MXRC SPAccDrvRun(PCRYPTO_VECTOR_DATA inVecData)
{

    PSPACCDRVCTX pCtx = pSPAccDrvCtx;
    int handle, err = 0;
    UINT32 y = 0;
    DDT_LST *srcDDT = NULL, *dstDDT = NULL, *currDDT = NULL, lSrcDDT, lDstDDT;
    PCRYPTOCTX cryptoCtx= NULL;
    IRQTYPE irq;

    if (!pSPAccDrvCtx->irqModeEnabled && inVecData->asynchMode)
    {
        uart_printf("SPAccDrvRun:Error non-blcoking mode isn't enabled\r\n");
        
        return SPACC_RC_CRYPTO_FAILED;
    }

    irq = ARM_INT_disable();
    MxGetLock(&pCtx->lock);
   
    if (inVecData->dir == DIR_ENC)
    {
        if (inVecData->pt && (inVecData->pt_size > 0))
        {
            lSrcDDT.next = NULL;
            lSrcDDT.ddt.ptr = inVecData->pt;
            lSrcDDT.ddt.len =  inVecData->pt_size;
            srcDDT = &lSrcDDT;
        }else
        {
            srcDDT = inVecData->ptLst;
        }

        if (inVecData->ct && (inVecData->ct_size > 0))
        {
            lDstDDT.next = NULL;
            lDstDDT.ddt.ptr = inVecData->ct;
            lDstDDT.ddt.len =  inVecData->ct_size;        
            dstDDT = &lDstDDT;
        }else
        {
            dstDDT = inVecData->ctLst;
        }
        
    }
    else if (inVecData->dir == DIR_DEC)
    {
        if (inVecData->pt && (inVecData->pt_size > 0))
        {
            lDstDDT.next = NULL;
            lDstDDT.ddt.ptr = inVecData->pt;
            lDstDDT.ddt.len =  inVecData->pt_size;        
            dstDDT = &lDstDDT;            
        }else
        {
            dstDDT = inVecData->ptLst;
        }

        if (inVecData->ct && (inVecData->ct_size > 0))
        {
            lSrcDDT.next = NULL;
            lSrcDDT.ddt.ptr = inVecData->ct;
            lSrcDDT.ddt.len =  inVecData->ct_size;
            srcDDT = &lSrcDDT;            
        }else
        {
            srcDDT = inVecData->ctLst;
        }
    
    }

   if ((handle = spacc_open(inVecData->enc_mode,
                            inVecData->hash_mode)) >= CRYPTO_CONTEXTS_MAX)
    {
        y = handle;
        MxReleaseLock(&pCtx->lock);
        ARM_INT_restore(irq);                
        return y;
    }

   inVecData->handle = handle;
   cryptoCtx = context_lookup(handle);
   cryptoCtx->pVecData = inVecData;

    // take the salt as the first part of the key for F8 mode
    if (inVecData->enc_mode == CRYPTO_MODE_AES_F8)
    {
        unsigned char tmp[128];

        memcpy(tmp, inVecData->saltkey, inVecData->keysize);
        memcpy(tmp + inVecData->keysize, inVecData->key, inVecData->keysize);
        memcpy(inVecData->key, tmp, inVecData->keysize << 1);
    }

    if ((inVecData->enc_mode > 0) &&
        (err = spacc_set_context(handle, SPACC_CRYPTO_OPERATION,
                    inVecData->key, inVecData->keysize, inVecData->iv, inVecData->iv_size))
        != SPACC_RC_OK)
    {
        y = err;
        MxReleaseLock(&pCtx->lock);        
        ARM_INT_restore(irq);                
        return y;
    }

    if ((inVecData->hash_mode > 0) &&
        (err = spacc_set_context(handle, SPACC_HASH_OPERATION,
                    inVecData->hmac_key, inVecData->hmac_keysize, 0, 0))
        != SPACC_RC_OK)
    {
        y = err;
        MxReleaseLock(&pCtx->lock);        
        ARM_INT_restore(irq);                
        return y;
    }

    if (inVecData->icv_offset == -1)
    {
        inVecData->icvpos = IP_ICV_APPEND;
    }
    else
    {
        inVecData->icvpos = IP_ICV_OFFSET;
    }

    err = spacc_set_operation(
        handle,
        inVecData->dir,
        inVecData->icv_mode, inVecData->icvpos,
        inVecData->dir == DIR_ENC ? inVecData->icv_offset
            : (inVecData->pre_aad_size + inVecData->pt_size + inVecData->post_aad_size),
        inVecData->icv_size);
    if (err != SPACC_RC_OK)
    {
        y = err;
        MxReleaseLock(&pCtx->lock);        
        ARM_INT_restore(irq);                
        return err;
    }

    // add sources
    if (inVecData->pre_aad)
    {
        if ((err = spacc_add_ddt(handle, inVecData->pre_aad, inVecData->pre_aad_size))
            != SPACC_RC_OK)
        {
            y = err;

            MxReleaseLock(&pCtx->lock);            
            ARM_INT_restore(irq);                    
            return err;
        }
    }

   currDDT = srcDDT;
     
    do 
    {
        if ((err = spacc_add_ddt(handle, currDDT->ddt.ptr, currDDT->ddt.len))
            != SPACC_RC_OK)
        {
            y = err;
            MxReleaseLock(&pCtx->lock);            
            ARM_INT_restore(irq);                                
            return err;
        }
        currDDT = currDDT->next;
    }while(currDDT);

    if (inVecData->post_aad)
    {
        if ((err = spacc_add_ddt(handle, inVecData->post_aad, inVecData->post_aad_size))
            != SPACC_RC_OK)
        {
            y = err;

            MxReleaseLock(&pCtx->lock);            
            ARM_INT_restore(irq);                                
            return err;
        }
    }

    currDDT = dstDDT;

    do 
    {
        // add destinations
        if ((err = spacc_add_dst_ddt(handle,
                        currDDT->ddt.ptr,
                        currDDT->ddt.len))
            != SPACC_RC_OK)
        {
            y = err;

            MxReleaseLock(&pCtx->lock);            
            ARM_INT_restore(irq);                                
            return y;
        }

        currDDT = currDDT->next;
        
    }while(currDDT);

    // only set the auxiliary register if this is Kasumi F8/F9 or Snow3G UIA2 mode
    if (inVecData->hash_mode == CRYPTO_MODE_MAC_KASUMI_F9
        || inVecData->enc_mode == CRYPTO_MODE_KASUMI_F8
        || inVecData->enc_mode == CRYPTO_MODE_SNOW3G_UEA2
        || inVecData->hash_mode == CRYPTO_MODE_MAC_SNOW3G_UIA2)
    {
        if ((err = spacc_set_auxinfo(handle, inVecData->auxinfo_dir, inVecData->auxinfo_bit_align))
            != SPACC_RC_OK)
        {
            y = err;

            MxReleaseLock(&pCtx->lock);            
            ARM_INT_restore(irq);                                
            return y;
        }
    }

    inVecData->execTicks = SysTimerGetTick();

    if ((err = spacc_packet_enqueue_ddt(pCtx, handle, 0, 0, inVecData->pre_aad_size, SPACC_SW_ID_PRIO_HI))
        != SPACC_RC_OK)
    {
        y = err;
        MxReleaseLock(&pCtx->lock);        
        ARM_INT_restore(irq);                
        return y;
    }

    MxReleaseLock(&pCtx->lock);
    ARM_INT_restore(irq);    

    return 0;
}

void SPAccSystemDoneCb(void *p)
{
    PCRYPTO_VECTOR_DATA pVecData = (PCRYPTO_VECTOR_DATA) p;

    pVecData->done = 1;
}

MXRC SPAccDrvRunBlocking(PCRYPTO_VECTOR_DATA inVecData)    
{
    UINT32 err, timer;

    inVecData->asynchMode = 1;
    inVecData->cbReqComp= SPAccSystemDoneCb;

    if (  (err = SPAccDrvRun(inVecData)) != SPACC_RC_OK)
    {
        return err;
    }

    timer = SysTimerGetTick();

    while ((!inVecData->done) && ((SysTimerGetTick() - timer) < MAX_SPACC_DIAG_TIMEOUT) );

    if (!inVecData->done)
    {
        uart_printf("SPAccDrvRunBlocking:CPU #%d crypto task (0x%x) wasn't done!\r\n",inVecData, MxGetCpuID());
        inVecData->errors = SPACC_RC_CRYPTO_TIMEOUT;
    }
    return err;  
            
}

void SPAccMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    MmuMgrCfgMemAP(SPACC_BASEADDR, 1024*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
}

#endif


