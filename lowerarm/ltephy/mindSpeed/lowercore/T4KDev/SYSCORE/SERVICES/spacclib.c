//-------------------------------------------------------------------------------------------
/** @file spacclib.c
 *
 * @brief Spacclib module
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "spaccdrv.h"

#ifdef SPACC_DRV_ENABLED

extern PSPACCDRVCTX pSPAccDrvCtx;

/************************* Elliptic SPAcc Functions *************************/

void memcpy32(volatile UINT32 *dst, volatile UINT32 *src, int len)
{
    int i;

    for (i = len - 1; i >= 0; i--)
    {
        dst[i] = src[i];
    }
}

// Return a software handle back to the pool
// if failed return CRYPTO_CONTEXTS_MAX else the same handle value
MXRC free_handle(SINT32 handle)
{
    MXRC ret = CRYPTO_CONTEXTS_MAX;

    if (handle < CRYPTO_CONTEXTS_MAX)
    {
        //mutex_lock(&elp_context_lock);
        if (pSPAccDrvCtx->mctx[handle].taken == 1)
        {
            pSPAccDrvCtx->mctx[handle].taken = 0;
            //printf("free_handle %d\n", handle);
            ret = handle;
        }
        //mutex_unlock(&elp_context_lock);
    }
    return ret;
}

// Allocate a software context handle
MXRC alloc_handle(void)
{
    UINT8 i;
    MXRC ret = CRYPTO_CONTEXTS_MAX;

    for (i = 0; i < CRYPTO_CONTEXTS_MAX; i++)
    {
        if (pSPAccDrvCtx->mctx[i].taken == 0)
        {
            pSPAccDrvCtx->mctx[i].taken = 1;
            ret = i;
            break;
        }
    }

    //printf ("alloc_handle: %d [%d-%d]\n", ret, i, CRYPTO_CONTEXTS_MAX);

    return ret;
}

// Return a context structure for a handle
// or null if invalid
CRYPTOCTX *context_lookup(SINT32 handle)
{
    CRYPTOCTX *ctx= NULL;


    if ((handle >= CRYPTO_CONTEXTS_MAX ) || (handle < 0) || (pSPAccDrvCtx->mctx[handle].taken == 0))
    {
        ctx = NULL;
    }
    else
    {
        ctx = &pSPAccDrvCtx->mctx[handle];
    }
    return ctx;
}

// Release a crypto context back into appropriate module's pool
MXRC spacc_close(SINT32 handle)
{
    MXRC ret= SPACC_RC_OK;

    //ret = spacc_release_ddt(handle);
    spacc_release_ddt(handle);

    if (free_handle(handle) != handle)
    {
        ret = SPACC_RC_CRYPTO_INVALID_HANDLE;
    }

    return ret;
}

// Allocate a handle for spacc module context and initialize it with
// an appropriate type. Here we receive encryption and hashing algos
// as defined by the OCF layer. So we need to remap theses definitions
// to match the elliptic ones.
MXRC spacc_open(SINT32 enc, SINT32 hash)
{
    MXRC ret= SPACC_RC_OK;
    SINT32 handle = 0;
    CRYPTOCTX *ctx= NULL;
    ELPCTRL ctrl;

//    uart_printf("spacc_open: enc=%d hash=%d\n", enc, hash);

    ctrl.word = 0;

    if ((handle = alloc_handle()) >= CRYPTO_CONTEXTS_MAX)
    {
        //uart_printf("spacc_open: alloc_handle() failed\n");
        ret = SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        ctx = context_lookup(handle);
        ctx->icv_len = 0;

        // Expand the key on the first use of the context
        // Should be reset after the first call
        ctrl.bit.KeyExp = 1;
        switch (enc)
        {
            case CRYPTO_MODE_NULL:
                //uart_printf("spacc_open:CIPHER MODE_NULL\n");
                break;

            case CRYPTO_MODE_AES_ECB:
                //ctrl |= CTRL_SET_CIPH_ALG(C_AES);
                //ctrl |= CTRL_SET_CIPH_MODE(CM_ECB);
                ctrl.bit.CiphAlg = C_AES;
                ctrl.bit.CiphMode = CM_ECB;
                break;

            case CRYPTO_MODE_AES_CBC:
                //printf("MODE_AES_CBC\n");
                //ctrl |= CTRL_SET_CIPH_ALG(C_AES);
                //ctrl |= CTRL_SET_CIPH_MODE(CM_CBC);
                ctrl.bit.CiphAlg = C_AES;
                ctrl.bit.CiphMode = CM_CBC;
                break;

            case CRYPTO_MODE_AES_CTR:
                //ctrl |= CTRL_SET_CIPH_ALG(C_AES);
                //ctrl |= CTRL_SET_CIPH_MODE(CM_CTR);
                ctrl.bit.CiphAlg = C_AES;
                ctrl.bit.CiphMode = CM_CTR;
                break;

            case CRYPTO_MODE_AES_CCM:
                //ctrl |= CTRL_SET_CIPH_ALG(C_AES);
                //ctrl |= CTRL_SET_CIPH_MODE(CM_CCM);
                ctrl.bit.CiphAlg = C_AES;
                ctrl.bit.CiphMode = CM_CCM;
                ctx->icv_len = 16;
                break;

            case CRYPTO_MODE_AES_F8:
                ctrl.bit.CiphAlg = C_AES;
                ctrl.bit.CiphMode = CM_F8;
                break;

            case CRYPTO_MODE_3DES_CBC:
            case CRYPTO_MODE_DES_CBC:
                //printf("CRYPTO_DES_CBC\n");
                //ctrl |= CTRL_SET_CIPH_ALG(C_DES);
                //ctrl |= CTRL_SET_CIPH_MODE(CM_CBC);
                ctrl.bit.CiphAlg = C_DES;
                ctrl.bit.CiphMode = CM_CBC;
                break;

            case CRYPTO_MODE_3DES_ECB:
            case CRYPTO_MODE_DES_ECB:
                //ctrl |= CTRL_SET_CIPH_ALG(C_DES);
                //ctrl |= CTRL_SET_CIPH_MODE(CM_ECB);
                ctrl.bit.CiphAlg = C_DES;
                ctrl.bit.CiphMode = CM_ECB;
                break;

            case CRYPTO_MODE_KASUMI_ECB:
                ctrl.bit.CiphAlg = C_KASUMI;
                ctrl.bit.CiphMode = CM_ECB;
                break;

            case CRYPTO_MODE_KASUMI_F8:
                ctrl.bit.CiphAlg = C_KASUMI;
                ctrl.bit.CiphMode = CM_F8;
                break;

            case CRYPTO_MODE_SNOW3G_UEA2:
                //ctrl |= CTRL_SET_CIPH_ALG(C_SNOW3G_UEA2);
                //ctrl |= CTRL_SET_CIPH_MODE(CM_ECB);
                ctrl.bit.CiphAlg = C_SNOW3G_UEA2;
                ctrl.bit.CiphMode = CM_ECB;
                break;

            default:
                //printf("CRYPTO_INVALID_ALG\n");
                ret = SPACC_RC_CRYPTO_INVALID_ALG;
                break;
        }

        switch (hash)
        {
            case CRYPTO_MODE_NULL:
                //printf("CRYPTO_MODE_NULL\n");
                //ctrl |= CTRL_SET_HASH_ALG(H_NULL);
                ctrl.bit.HashAlg = H_NULL;
                break;

            case CRYPTO_MODE_HMAC_SHA1:
                //printf("CRYPTO_MODE_HASH_SHA1\n");
                //ctrl |= CTRL_SET_HASH_ALG(H_SHA1);
                //ctrl |= CTRL_SET_HASH_MODE(HM_HMAC);
                ctrl.bit.HashAlg = H_SHA1;
                ctrl.bit.HashMode = HM_HMAC;
                ctx->icv_len = CRYPTO_HASH_SHA1_SIZE;
                break;

/*            case CRYPTO_MODE_HMAC_MD5:
                ctrl.bit.HashAlg = H_MD5;
                ctrl.bit.HashMode = HM_HMAC;
                ctx->icv_len = CRYPTO_HASH_MD5_SIZE;
                break;*/
            case CRYPTO_MODE_HMAC_SHA224:
                ctrl.bit.HashAlg = H_SHA224;
                ctrl.bit.HashMode = HM_HMAC;
                ctx->icv_len = CRYPTO_HASH_SHA224_SIZE;
                break;

            case CRYPTO_MODE_HMAC_SHA256:
                ctrl.bit.HashAlg = H_SHA256;
                ctrl.bit.HashMode = HM_HMAC;
                ctx->icv_len = CRYPTO_HASH_SHA256_SIZE;
                break;

            case CRYPTO_MODE_HMAC_SHA384:
                ctrl.bit.HashAlg = H_SHA384;
                ctrl.bit.HashMode = HM_HMAC;
                ctx->icv_len = CRYPTO_HASH_SHA384_SIZE;
                break;

            case CRYPTO_MODE_HMAC_SHA512:
                ctrl.bit.HashAlg = H_SHA512;
                ctrl.bit.HashMode = HM_HMAC;
                ctx->icv_len = CRYPTO_HASH_SHA512_SIZE;
                break;

/*            case CRYPTO_MODE_SSLMAC_MD5:
                ctrl.bit.HashAlg = H_MD5;
                ctrl.bit.HashMode = HM_SSLMAC;
                ctx->icv_len = CRYPTO_HASH_MD5_SIZE;
                break;

            case CRYPTO_MODE_SSLMAC_SHA1:
                ctrl.bit.HashAlg = H_SHA1;
                ctrl.bit.HashMode = HM_SSLMAC;
                ctx->icv_len = CRYPTO_HASH_SHA1_SIZE;
                break;*/

            case CRYPTO_MODE_HASH_SHA1:
                ctrl.bit.HashAlg = H_SHA1;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_HASH_SHA1_SIZE;
                break;
            case CRYPTO_MODE_HASH_MD5:
                ctrl.bit.HashAlg = H_MD5;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_HASH_MD5_SIZE;
                break;

            case CRYPTO_MODE_HASH_SHA224:
                ctrl.bit.HashAlg = H_SHA224;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_HASH_SHA224_SIZE;
                break;

            case CRYPTO_MODE_HASH_SHA256:
                ctrl.bit.HashAlg = H_SHA256;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_HASH_SHA256_SIZE;
                break;

            case CRYPTO_MODE_HASH_SHA384:
                ctrl.bit.HashAlg = H_SHA384;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_HASH_SHA384_SIZE;
                break;

            case CRYPTO_MODE_HASH_SHA512:
                ctrl.bit.HashAlg = H_SHA512;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_HASH_SHA512_SIZE;
                break;

/*            case CRYPTO_MODE_MAC_XCBC:
                ctrl.bit.HashAlg = H_XCBC;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_MAC_XCBC_SIZE;
                break;*/

            case CRYPTO_MODE_MAC_CMAC:
                ctrl.bit.HashAlg = H_CMAC;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_MAC_CMAC_SIZE;
                break;

            case CRYPTO_MODE_MAC_KASUMI_F9:
                ctrl.bit.HashAlg = H_KF9;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_MAC_KASUMIF9_SIZE;
                break;

            case CRYPTO_MODE_MAC_SNOW3G_UIA2:
                //ctrl |= CTRL_SET_HASH_ALG(H_SNOW3G_UIA2);
                //ctrl |= CTRL_SET_HASH_MODE(HM_RAW);
                ctrl.bit.HashAlg = H_SNOW3G_UIA2;
                ctrl.bit.HashMode = HM_RAW;
                ctx->icv_len = CRYPTO_MAC_SNOW3GUIA2_SIZE;
                break;

            default:
                //uart_printf("spacc_open:CRYPTO_INVALID_ALG\n");
                ret = SPACC_RC_CRYPTO_INVALID_ALG;
                break;
        }
    }

    //ctrl |= CTRL_SET_MSG_BEGIN | CTRL_SET_MSG_END;
    ctrl.bit.MsgBegin = ctrl.bit.MsgEnd = 1;

    if (ret != SPACC_RC_OK)
    {
        free_handle(handle);
    }
    else
    {
        ret = handle;
        ctx->first_use = 1;
        ctx->enc_mode = enc;
        ctx->hash_mode = hash;
        //*((UINT32*)&ctx->ckey_sz) = 0;
        //*((UINT32*)&ctx->hkey_sz) = 0;
        ctx->ckey_sz.word = 0;
        ctx->hkey_sz.word = 0;
        ctx->job_done = 0;
        ctx->pkt_sz = 0;
        ctx->job_id = 0xFFFFFFFF;
        ctx->job_err = (UINT32)SPACC_RC_CRYPTO_INPROGRESS;
        //ctx->ctrl = ctrl | CTRL_SET_CTX_IDX(handle);
        ctrl.bit.CtxIdx = handle;
        ctx->ctrl.word = ctrl.word;
    }

    return ret;
}

// NOTE: endianness !!!!
MXRC spacc_set_context(SINT32 handle, SINT32 op, UINT8 * key, SINT32 ksz, UINT8 * iv, SINT32 ivsz)
{
    MXRC ret= SPACC_RC_OK;
    CRYPTOCTX *ctx= NULL;
    //int i;
    //UINT32 ckey_sz = 0;

    ctx = context_lookup(handle);

    if (ctx == NULL)
    {
        ret = SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        switch (op)
        {
            case SPACC_CRYPTO_OPERATION:
                switch (ctx->enc_mode)
                {
                    case CRYPTO_MODE_RC4_40:
                        memcpy(ctx->ciph_key, key, 2*sizeof(UINT32));
                        break;
                    case CRYPTO_MODE_RC4_128:
                        memcpy(ctx->ciph_key, key, 4*sizeof(UINT32));
                        break;
                    case CRYPTO_MODE_AES_ECB:
                    case CRYPTO_MODE_AES_CBC:
                    case CRYPTO_MODE_AES_CTR:
                    case CRYPTO_MODE_AES_CCM:
                    case CRYPTO_MODE_AES_GCM:
                        if (key)
                        {
                            memcpy32(&ctx->ciph_key[0], (UINT32*)key, ksz >> 2);
                            //printf("set cipher key at %x", (UINT32)&ctx->ciph_key[0]);
                            ctx->first_use = 1;
                        }
                        if (iv)
                        {
                            unsigned char one[4] =
                            { 0, 0, 0, 2 };
                            memcpy32(&ctx->ciph_key[8], (UINT32*)iv, ivsz >> 2);
                            //printf("set iv at %x", (UINT32)&ctx->ciph_key[8]);
                            if (ivsz == 12 && ctx->enc_mode == CRYPTO_MODE_AES_GCM)
                            {
                                memcpy32(&ctx->ciph_key[11], (UINT32*)&one, 1/**sizeof(UINT32)*/);
                            }
                        }
                        break;
                    case CRYPTO_MODE_AES_F8:
                        if (key)
                        {
                            //MEMCPY32(ctx->ciph_key, key + ksz, ksz >> 2);
                            //MEMCPY32(&ctx->ciph_key[12], key, ksz >> 2);
                            memcpy32(ctx->ciph_key, (UINT32*)(key + ksz), ksz >> 2);
                            memcpy32(&ctx->ciph_key[12], (UINT32*)key, ksz >> 2);
                        }
                        if (iv)
                            //MEMCPY32(&ctx->ciph_key[8], iv, ivsz >> 2);
                            memcpy32(&ctx->ciph_key[8], (UINT32*)iv, ivsz >> 2);
                        break;
                    case CRYPTO_MODE_MULTI2_ECB:
                    case CRYPTO_MODE_MULTI2_CBC:
                    case CRYPTO_MODE_MULTI2_OFB:
                    case CRYPTO_MODE_MULTI2_CFB:
                        // Number of rounds at the end of the IV
                        if (key)
                            //MEMCPY32(ctx->ciph_key, key, ksz >> 2);
                            memcpy32(ctx->ciph_key, (UINT32*)key, ksz >> 2);
                        if (iv)
                            //MEMCPY32(&ctx->ciph_key[10], iv, ivsz >> 2);
                            memcpy32(&ctx->ciph_key[10], (UINT32*)iv, ivsz >> 2);
                        break;
                    case CRYPTO_MODE_3DES_CBC:
                    case CRYPTO_MODE_3DES_ECB:
                    case CRYPTO_MODE_DES_CBC:
                    case CRYPTO_MODE_DES_ECB:
                        if (iv)
                            memcpy32(ctx->ciph_key, (UINT32*)iv, ivsz >> 2);
                        if (key)
                            memcpy32(&ctx->ciph_key[2], (UINT32*)key, ksz >> 2);
                        break;

                    case CRYPTO_MODE_KASUMI_ECB:
                    case CRYPTO_MODE_KASUMI_F8:
                        if (iv)
                            memcpy32(&ctx->ciph_key[4], (UINT32*)iv, 2/**sizeof(UINT32)*/);
                        if (key)
                            memcpy32(ctx->ciph_key, (UINT32*)key, 4/**sizeof(UINT32)*/);
                        break;

                    case CRYPTO_MODE_SNOW3G_UEA2:
                        if (key)
                            memcpy32(ctx->ciph_key, (UINT32*)key, 8/**sizeof(UINT32)*/);
                        break;

                    case CRYPTO_MODE_NULL:
                    default:

                        break;
                }

                // Set context might be called only to set iv separately
                // so only when having a key set the key size
                if (key)
                {
                    ctx->ctrl.bit.KeyExp = 1; // |= CTRL_SET_KEY_EXP;
                    ctx->ckey_sz.word = 0;
                    ctx->ckey_sz.bit.Size = ksz;
                    ctx->ckey_sz.bit.CtxIdx = handle;
                    ctx->ckey_sz.bit.Cipher = 1;
                }
                break;

            case SPACC_HASH_OPERATION:
                switch (ctx->hash_mode)
                {
                    case CRYPTO_MODE_MAC_XCBC:
                        memcpy32(&ctx->hash_key[8], (UINT32*)key + (ksz - 32), 32/4);
                        memcpy32(&ctx->hash_key[0], (UINT32*)key, (ksz - 32)/4);
                        ctx->hkey_sz.word = 0;
                        ctx->hkey_sz.bit.Size = ksz - 32;
                        ctx->hkey_sz.bit.CtxIdx = handle;
                        ctx->hkey_sz.bit.Cipher = 0;
                        break;
                    default:
                        if (ksz)
                        {
                            ctx->hkey_sz.word = 0;
                            ctx->hkey_sz.bit.Size = ksz;
                            ctx->hkey_sz.bit.CtxIdx = handle;
                            ctx->hkey_sz.bit.Cipher = 0;

                            memcpy32(ctx->hash_key, (UINT32*)key, (ksz + 3) >> 2);
                        }
                        break;
                }
/*
                if(ksz)
                {
                    //printf("HASH KEY: \n");
                    for(i = 0; i < ksz; i++)
                        //printf("%2x ", *((UINT8*)ctx->hash_key + i));
                    //printf("\n");
                }
                else
                {
                    //printf("HASH KEY not provided\n");
                }
*/
                break;
            default:
                ret = SPACC_RC_CRYPTO_INVALID_MODE;
                break;
        }
    }
    return ret;
}

MXRC spacc_set_operation(SINT32 handle, SINT32 op, UINT32 prot, UINT32 icvpos, UINT32 icvoff, UINT32 icvsz)
{
    MXRC ret= SPACC_RC_OK;
    CRYPTOCTX *ctx= NULL;

    ctx = context_lookup(handle);
    if (ctx == NULL)
    {
        ret = SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        if (op == OP_ENCRYPT)
        {
            ctx->op = OP_ENCRYPT;
            //ctx->ctrl |= CTRL_SET_ENCRYPT;
            ctx->ctrl.bit.Encrypt = 1;
        }
        else
        {
            ctx->op = OP_DECRYPT;
        }

        switch (prot)
        {
            case IM_ICV_IGNORE:
                break;
            case IM_ICV_HASH: /* HASH of plaintext */
                //ctx->ctrl |= CTRL_SET_ICV_PT;
                ctx->ctrl.bit.IcvPt = 1;
                break;
            case IM_ICV_HASH_ENCRYPT: /* HASH the plaintext and encrypt the lot */
                //ctx->ctrl |= CTRL_SET_ICV_PT;
                //ctx->ctrl |= CTRL_SET_ICV_ENC;
                //ctx->ctrl |= CTRL_SET_ICV_APPEND;
                ctx->ctrl.bit.IcvPt = 1;
                ctx->ctrl.bit.IcvEnc = 1;
                ctx->ctrl.bit.IcvAppend = 1;
                break;
            case IM_ICV_ENCRYPT_HASH: /* HASH the ciphertext */
                break;
            default:
                ret = SPACC_RC_CRYPTO_INVALID_MODE;
                break;
        }

        ctx->icv_len = icvsz;

        switch (icvpos)
        {
            case IP_ICV_OFFSET:
                ctx->icv_offset = icvoff;
                break;
            case IP_ICV_APPEND:
                //ctx->ctrl |= CTRL_SET_ICV_APPEND;
                ctx->ctrl.bit.IcvAppend = 1;
                break;
            case IP_ICV_IGNORE:
                break;
            default:
                ret = SPACC_RC_CRYPTO_INVALID_MODE;
                break;
        }
    }
    return ret;
}

MXRC spacc_set_auxinfo(SINT32 handle, UINT32 direction, UINT32 bitsize)
{
    MXRC ret= SPACC_RC_OK;
    CRYPTOCTX *ctx = context_lookup(handle);

    if ( NULL == ctx)
    {
        ret = SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        ctx->auxinfo_dir = direction;
        ctx->auxinfo_bit_align = bitsize;
    }
    return ret;
}

MXRC spacc_add_ddt(SINT32 handle, UINT8 * data, UINT32 len)
{
    MXRC ret= SPACC_RC_OK;
    CRYPTOCTX *ctx= NULL;
    //int i;
    ctx = context_lookup(handle);
    
    if (ctx == NULL)
    {
        //printf("spacc_add_ddt: context_lookup failed\n");
        ret = SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        UINT32 newlen;
        UINT32 nextsectoraddr = ((UINT32)data & 0xFFFF0000) + SPACC_MAX_PARTICLE_SIZE;

        // split things larger than the max particle size
/*        if (len > SPACC_MAX_PARTICLE_SIZE)
        {
            newlen = len - SPACC_MAX_PARTICLE_SIZE;
            len = SPACC_MAX_PARTICLE_SIZE;
        }
*/
        if (((UINT32)data + len) >= nextsectoraddr)
        {
        	newlen = len - (nextsectoraddr - (UINT32)data);
        	len = nextsectoraddr - (UINT32)data;
        }
        else
        {
            newlen = 0;
        }

        if ((ctx->ddt_desc[ctx->ddt_idx].map = data) == NULL)
        {
            //printf("spacc_add_ddt: MEM_MAP failed for data %lu bytes\n", len);
            return SPACC_RC_CRYPTO_FAILED;
        }

        //uart_printf("spacc_add_ddt:%d: ddt_idx=%d  buffer @%lX mapped buffer @%lX %lu bytes - ddt @%lX \n", handle, ctx->ddt_idx, (unsigned long int)data ,(unsigned long int)ctx->ddt_desc[ctx->ddt_idx].map, len, (unsigned long int)&ctx->ddt[ctx->ddt_idx]);
/*
        //printf("%s: SRC DDT DATA (len %d) data\n", __FUNCTION__, (unsigned char)len);
        for(i = 0; i < len; i++)
            //printf("%2x ", *((UINT8*)data + i));
        //printf("\n");
*/
        ctx->ddt_desc[ctx->ddt_idx].buf = data;
        ctx->ddt_desc[ctx->ddt_idx].len = len;
        ctx->ddt[ctx->ddt_idx].ptr = (UINT32) ctx->ddt_desc[ctx->ddt_idx].map;
        ctx->ddt[ctx->ddt_idx].len = (UINT32) len;
       // uart_printf("spacc_add_ddt: 0x%8x=0x%8x\n", &ctx->ddt[ctx->ddt_idx].ptr, ctx->ddt[ctx->ddt_idx].ptr);
       // uart_printf("spacc_add_ddt: 0x%8x=0x%8x\n", &ctx->ddt[ctx->ddt_idx].len, len);
        ctx->ddt[ctx->ddt_idx+1].ptr = (UINT32)NULL;
        ctx->ddt[ctx->ddt_idx+1].len = 0;
        ctx->ddt_len += len;
        ctx->ddt_idx++;

        //uart_printf("spacc_add_ddt: ctx->ddt_len = %d ctx->ddt_idx = %d\n", ctx->ddt_len, ctx->ddt_idx);

        // if there are remaining bytes, recurse and add it as a new DDT entry
        if (newlen != 0)
        {
//            return spacc_add_ddt(handle, data + SPACC_MAX_PARTICLE_SIZE, newlen);
        	return spacc_add_ddt(handle, (UINT8*)nextsectoraddr, newlen);
        }
    }

    return ret;
}

MXRC spacc_release_ddt(SINT32 handle)
{
    int i;

    CRYPTOCTX *ctx = context_lookup(handle);
    if (ctx == NULL)
    {
        return SPACC_RC_CRYPTO_INVALID_HANDLE;
    }

    for (i = 0; i < ctx->ddt_idx; i++)
    {
        if ((ctx->ddt_desc[i].map == NULL) || (ctx->ddt_desc[i].len == 0))
        {
            //printf("spacc_release_ddt:%d: Invalid entry %d map=%X len=%d\n",
            //(int) handle, i, (int) ctx->ddt_desc[i].map,
            //(int) ctx->ddt_desc[i].len);
        }
        else
        {
/*
            //printf("spacc_release_ddt:%d: ddt_idx=%d @ %lX %lu bytes\n",
                handle, i, (UINT32)ctx->ddt_desc[i].map, ctx->ddt_desc[i].len);
*/
            //MEM_UNMAP(ctx->dev, ctx->ddt_desc[i].map, ctx->ddt_desc[i].len);
            ctx->ddt_desc[i].map = 0;
            ctx->ddt_desc[i].buf = 0;
            ctx->ddt_desc[i].len = 0;
            ctx->ddt[i].ptr = 0;
            ctx->ddt[i].len = 0;
        }
    }
    ctx->ddt_idx = 0;
    ctx->ddt_len = 0;

    return SPACC_RC_OK;
}

MXRC spacc_add_dst_ddt(SINT32 handle, UINT8 * data, UINT32 len)
{
    MXRC ret= SPACC_RC_OK;
    CRYPTOCTX *ctx= NULL;

    ctx = context_lookup(handle);
    if (ctx == NULL)
    {
        ret = SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        UINT32 newlen;
        UINT32 nextsectoraddr = ((UINT32)data & 0xFFFF0000) + SPACC_MAX_PARTICLE_SIZE;

        // split things larger than the max particle size
/*        if (len > SPACC_MAX_PARTICLE_SIZE)
        {
            newlen = len - SPACC_MAX_PARTICLE_SIZE;
            len = SPACC_MAX_PARTICLE_SIZE;
        }
*/
        if (((UINT32)data + len) >= nextsectoraddr)
        {
            newlen = len - (nextsectoraddr - (UINT32)data);
            len = nextsectoraddr - (UINT32)data;
        }
        else
        {
            newlen = 0;
        }

        if ((ctx->sddt_desc[ctx->sddt_idx].map = data) == NULL)
        {
            //printf("spacc_add_dst_ddt: MEM_MAP failed for data %lu bytes\n", len);
            return SPACC_RC_CRYPTO_FAILED;
        }

        //uart_printf("spacc_add_dst_ddt:%d: ddt_idx=%d org buffer @%lX mapped buffer @%lX %lu bytes  - ddt @%lX \n", handle, ctx->sddt_idx, (unsigned long int)data ,(unsigned long int)ctx->sddt_desc[ctx->sddt_idx].map, len, (unsigned long int)&ctx->sddt[ctx->sddt_idx]);

        ctx->sddt_desc[ctx->sddt_idx].buf = data;
        ctx->sddt_desc[ctx->sddt_idx].len = len;
        ctx->sddt[ctx->sddt_idx].ptr = (UINT32) ctx->sddt_desc[ctx->sddt_idx].map;
        ctx->sddt[ctx->sddt_idx].len = (UINT32) len;
        //printf("sddt src: 0x%8x=0x%8x\n", &ctx->sddt[ctx->sddt_idx].ptr, ctx->sddt[ctx->sddt_idx].ptr);
        //printf("0x%8x=0x%8x\n", &ctx->sddt[ctx->sddt_idx].len, len);
        ctx->sddt[ctx->sddt_idx+1].ptr = (UINT32)NULL;
        ctx->sddt[ctx->sddt_idx+1].len = 0;
        ctx->sddt_len += len;
        ctx->sddt_idx++;

        // if there are remaining bytes, recurse and add it as a new DDT entry
        if (newlen != 0)
        {
//            return spacc_add_dst_ddt(handle, data + SPACC_MAX_PARTICLE_SIZE, newlen);
        	return spacc_add_dst_ddt(handle, (UINT8*)nextsectoraddr, newlen);
        }

    }

    return ret;
}

MXRC spacc_release_dst_ddt(SINT32 handle)
{
    int i;

    CRYPTOCTX *ctx = context_lookup(handle);
    if (ctx == NULL)
    {
        return SPACC_RC_CRYPTO_INVALID_HANDLE;
    }

    for (i = 0; i < ctx->sddt_idx; i++)
    {
        if ((ctx->sddt_desc[i].map == NULL) || (ctx->sddt_desc[i].len == 0))
        {
/*
            //printf("spacc_release_dst_ddt:%d: Invalid entry %d map=%X len=%d\n",
                (int) handle, i, (int) ctx->sddt_desc[i].map,
                (int) ctx->sddt_desc[i].len);
*/
        }
        else
        {
/*
            //printf("spacc_release_dst_ddt:%d: ddt_idx=%d @ %lX %lu bytes\n",
                handle, i,(UINT32) ctx->sddt_desc[i].map, ctx->sddt_desc[i].len);
*/
            //MEM_UNMAP(ctx->dev, ctx->sddt_desc[i].map, ctx->sddt_desc[i].len);
            ctx->sddt_desc[i].map = 0;
            ctx->sddt_desc[i].buf = 0;
            ctx->sddt_desc[i].len = 0;
            ctx->sddt[i].ptr = 0;
            ctx->sddt[i].len = 0;
        }
    }
    ctx->sddt_idx = 0;
    ctx->sddt_len = 0;

    return SPACC_RC_OK;
}

MXRC spacc_packet_enqueue_ddt(PSPACCDRVCTX pElpCtx, SINT32 handle, UINT32 proc_sz,
                                UINT32 aad_offset, UINT32 aad_sz, UINT32 prio)
{
    MXRC ret= SPACC_RC_OK, proc_len;
    CRYPTOCTX *ctx= NULL;
    volatile PELPSPACCREGS pSpaccRegs;

    pSpaccRegs = pElpCtx->pSpaccRegs;

    //uart_printf("spacc_packet_enqueue_ddt: handle = %d proc_sz = %d\r\n", handle, proc_sz);
    ctx = context_lookup(handle);
    if (ctx == NULL)
    {
        ret = SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        if (ctx->ddt_idx == 0)
        {
            //printf ("spacc_packet_enqueue_ddt:%d: DDT entry invalid\n", handle);
            return SPACC_RC_CRYPTO_FAILED;
        }
        // compute the length we must process, in decrypt mode with an ICV (hash, hmac or CCM modes)
        // we must subtract the icv length from the buffer size
        if (proc_sz == 0)
        {
            if ((ctx->op == OP_DECRYPT) && ((ctx->hash_mode > 0) || (ctx->enc_mode
                == CRYPTO_MODE_AES_CCM || ctx->enc_mode == CRYPTO_MODE_AES_GCM))
                && !ctx->ctrl.bit.IcvEnc)
            {
                proc_len = ctx->ddt_len - ctx->icv_len;
            }
            else
            {
                proc_len = ctx->ddt_len;
            }
        }
        else
        {
            proc_len = proc_sz;
        }

        if (aad_sz & SPACC_AADCOPY_FLAG)
        {
            ctx->ctrl.bit.AadCopy = 1;
            aad_sz &= ~(SPACC_AADCOPY_FLAG);
        }
        else
        {
            ctx->ctrl.bit.AadCopy = 0;
        }
        ctx->aad_sz = aad_sz;

        ctx->out_map = NULL;
        ctx->out_sz = 0;

        pSpaccRegs->SpaccRegSrcPtr = (UINT32)ctx->ddt_map;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegSrcPtr, ctx->ddt_map);
        pSpaccRegs->SpaccRegDstPtr = (UINT32)ctx->sddt_map;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegDstPtr, ctx->sddt_map);
        pSpaccRegs->SpaccRegProcLen = proc_len;
        //uart_printf("spacc_packet_enqueue_ddt:SpaccRegProcLen %x = %x\n", (UINT32)&pSpaccRegs->SpaccRegProcLen, proc_len);
        pSpaccRegs->SpaccRegIcvLen = ctx->icv_len;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegIcvLen, ctx->icv_len);
        pSpaccRegs->SpaccRegIcvOffset = ctx->icv_offset;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegIcvOffset, ctx->icv_offset);
        //*((UINT32*)&pSpaccRegs->SpaccRegKeySz) = *((UINT32*)&ctx->ckey_sz);
        //*((UINT32*)&pSpaccRegs->SpaccRegKeySz) = *((UINT32*)&ctx->hkey_sz);
        pSpaccRegs->SpaccRegKeySz.word = ctx->ckey_sz.word;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegKeySz, ctx->ckey_sz.word);
        pSpaccRegs->SpaccRegKeySz.word = ctx->hkey_sz.word;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegKeySz, ctx->hkey_sz.word);

        pSpaccRegs->SpaccRegAadLen = ctx->aad_sz;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegAadLen, ctx->aad_sz);
        *((UINT32*)&pSpaccRegs->SpaccRegOffset) = aad_offset; // ??? source or destination ???
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegOffset, aad_offset);

        //#if defined (SPACC_KASUMIF9) || defined (SPACC_SNOW3G)
        //    pSpaccRegs->SpaccRegAuxInfo   = (ctx->auxinfo_dir << _SPACC_AUX_INFO_DIR) | (ctx->auxinfo_bit_align << _SPACC_AUX_INFO_BIT_ALIGN);
        pSpaccRegs->SpaccRegAuxInfo.InfoDir = ctx->auxinfo_dir;
        pSpaccRegs->SpaccRegAuxInfo.BitAlign = ctx->auxinfo_bit_align;
        //#endif

        //ctx->job_id = (pElpCtx->pSpaccRegs->SpaccRegSwCtl >> SPACC_SW_ID_ID_O) & ((1U<<SPACC_SW_ID_ID_W)-1);
        //pElpCtx->pSpaccRegs->SpaccRegSwId = (ctx->job_id  << SPACC_SW_ID_ID_O) | (prio << SPACC_SW_ID_PRIO);
        ctx->job_id = pSpaccRegs->SpaccRegSwCtl.SwId;
        pSpaccRegs->SpaccRegSwCtl.Priority = prio;

        //uart_printf("spacc_packet_enqueue_ddt:ctx->ctrl.word = %d\r\n", ctx->ctrl.word);
        // start processing
        //*((UINT32*)&pSpaccRegs->SpaccRegCtrl) = *((UINT32*)&ctx->ctrl);
        pSpaccRegs->SpaccRegCtrl.word = ctx->ctrl.word;
        //printf("%x = %x\n", (UINT32)&pSpaccRegs->SpaccRegCtrl, ctx->ctrl.word);

        // clear an expansion key after the first call
        if (ctx->first_use == 1)
        {
            ctx->first_use = 1;
            ctx->ctrl.bit.KeyExp = 0;
        }

        MxDelayTicks(150*15);
    }
    return ret;
}

MXRC spacc_packet_enqueue(PSPACCDRVCTX pElpCtx, SINT32 handle, UINT8 * in_buf, UINT32 in_sz,
                            UINT8 * out_buf, UINT32 out_sz, UINT32 aad_sz, UINT32 prio)
{
    MXRC ret= SPACC_RC_OK;

    ret = spacc_add_ddt(handle, in_buf, in_sz);
    if ((ret = spacc_add_dst_ddt(handle, out_buf, out_sz)) == SPACC_RC_OK)
        ret = spacc_packet_enqueue_ddt(pElpCtx, handle, in_sz, 0, aad_sz, prio);

    return ret;
}

static MXRC _spacc_packet_dequeue(PSPACCDRVCTX pElpCtx, int demap)
{
    MXRC ret= SPACC_RC_CRYPTO_INPROGRESS;
    CRYPTOCTX *ctx= NULL;
    UINT32 /*status, cmdstat,*/pkt_sz, x;
    volatile ELPSPACCREGS* pSpaccRegs;
    UINT32 isPopped = 0;

    //#warning need mutex around dequeue()

    pSpaccRegs = pElpCtx->pSpaccRegs;

    /*uart_printf("_spacc_packet_dequeue: FifoStatStatCnt = %d, RetCode = %d SwId = %d\r\n", 
                                                                                                pSpaccRegs->SpaccRegFifoStat.FifoStatStatCnt, 
                                                                                                pSpaccRegs->SpaccRegStatus.RetCode, 
                                                                                                pSpaccRegs->SpaccRegStatus.SwId);*/

    if (pElpCtx->irqModeEnabled)
    {
        if (pElpCtx->popped)
        {
            for (x = 0; x < CRYPTO_CONTEXTS_MAX && pSPAccDrvCtx->mctx[x].job_id != pSpaccRegs->SpaccRegStatus.SwId; x++)
                ;

            if (x == CRYPTO_CONTEXTS_MAX)
            {
                return ret;
            }            

            pElpCtx->popped = 0;

            ctx = &pSPAccDrvCtx->mctx[x];

            if (ctx != NULL)
                 isPopped = 1;
         }

    }else
    {
        if (pSpaccRegs->SpaccRegFifoStat.FifoStatStatCnt > 0)
        {
            pSpaccRegs->SpaccRegStatPop = 0x1;   

            isPopped = 1;

            for (x = 0; x < CRYPTO_CONTEXTS_MAX && pSPAccDrvCtx->mctx[x].job_id != pSpaccRegs->SpaccRegStatus.SwId; x++)
                ;

            if (x == CRYPTO_CONTEXTS_MAX)
            {
                return SPACC_RC_CRYPTO_FAILED;
            }            

            ctx = &pSPAccDrvCtx->mctx[x];

        }
    }

    if (isPopped)
    {
        //switch (SPACC_GET_STATUS_RET_CODE(cmdstat)) {
        switch (pSpaccRegs->SpaccRegStatus.RetCode)
        {
            case SPACC_ICVFAIL:
                ret = SPACC_RC_CRYPTO_AUTHENTICATION_FAILED;
                if (ctx->ddt_idx > 0)
                {
                    spacc_release_ddt(x);
                    spacc_release_dst_ddt(x);
                }
                break;
            case SPACC_MEMERR:
                if (ctx->ddt_idx > 0)
                {
                    spacc_release_ddt(x);
                    spacc_release_dst_ddt(x);
                }
                ret = SPACC_RC_CRYPTO_MEMORY_ERROR;
                break;
            case SPACC_BLOCKERR:
                if (ctx->ddt_idx > 0)
                {
                    spacc_release_ddt(x);
                    spacc_release_dst_ddt(x);
                }
                ret = SPACC_RC_CRYPTO_INVALID_BLOCK_ALIGNMENT;
                break;

            case SPACC_OK:
                // Subtract the AAD length
                pkt_sz = pSpaccRegs->SpaccRegProcLen - (pSpaccRegs->SpaccRegAadLen) + ((ctx->op == OP_DECRYPT) ? 0 : ctx->icv_len);
                ctx->pkt_sz = pkt_sz;
                ctx->job_done = 1;
        /*
                if (ctx->out_buf)
                {
                    PDUMPWORD(EDDUMP, ctx->out_buf, pkt_sz, "spacc_packet_dequeue:PACKET OUT", BIGENDIAN);
                }

                printf (EDFIFO, ctx->ciph_key, SPACC_CTX_CIPH_PAGE,
                    "spacc_packet_dequeue:CIPH KEY + IV", BIGENDIAN);
        */
                // check if we are processing ddt chain or a single ddt
                if (ctx->ddt_idx > 0)
                {
                    //PDEBUG1(EDFIFO, "spacc_packet_dequeue:SPACC_OK: Calling spacc_release_ddt for %d\n",
                    //    ctx->ddt_idx);
                    if (demap == SPACC_DEQUEUE_UNMAP)
                    {
                        spacc_release_ddt(x);
                        ret = spacc_release_dst_ddt(x);
                    }
                    else
                    {
                        ret = SPACC_RC_OK;
                    }
                }
        /*
                else
                {
                    MEM_UNMAP(ctx->dev, ctx->in_map, ctx->in_sz);
                    if (ctx->out_map != ctx->in_map)
                        MEM_UNMAP(ctx->dev, ctx->out_map, ctx->out_sz);
                }
        */
                ctx->in_buf = ctx->in_map = ctx->out_map = ctx->out_buf = NULL;
                ctx->out_sz = ctx->in_sz = 0;
                ret = SPACC_RC_OK;
                break;
        }
  }

    if (ctx != NULL)
    {
        ctx->job_err = ret;
    }
    return ret;
}

MXRC spacc_packet_dequeue(PSPACCDRVCTX pElpCtx, SINT32 handle, UINT32 * size, int demap)
{
    MXRC ret = SPACC_RC_OK;
    PCRYPTOCTX ctx = NULL;
    IRQTYPE irq;

    irq = ARM_INT_disable();
    MxGetLock(&pElpCtx->lock);
    if (pElpCtx->irqModeEnabled)
        if (!pElpCtx->popped)
        {
            MxReleaseLock(&pElpCtx->lock);
            ARM_INT_restore(irq);            

            return SPACC_RC_CRYPTO_INPROGRESS;
        }

    ctx = context_lookup(handle);

    if (ctx == NULL)
    {
        ret = (UINT32)SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        _spacc_packet_dequeue(pElpCtx, demap);
        ret = ctx->job_err;
        if (ctx->job_err == SPACC_RC_OK)
        {
            if (ctx->job_done)
            {
                //printf("Size:  %d  and Packet size %d\n", *size, ctx->pkt_sz);
                *size = ctx->pkt_sz;
                ctx->job_done = 0;
                ctx->job_err = (UINT32)SPACC_RC_CRYPTO_INPROGRESS;
                ctx->job_id = 0xFFFFFFFF;
            }
            else
            {
                *size = 0;
                MxReleaseLock(&pElpCtx->lock);
                ARM_INT_restore(irq);            
                return SPACC_RC_CRYPTO_INPROGRESS;
            }
        }

        if (ret != (UINT32)SPACC_RC_CRYPTO_INPROGRESS && ret != SPACC_RC_OK)
        {
            //printf("error ! Size:  %d  and Packet size %d\n",*size, ctx->pkt_sz);
            *size = 0;
            ctx->job_done = 0;
            ctx->job_id = 0xFFFFFFFF;
        }
    }

    MxReleaseLock(&pElpCtx->lock);
    ARM_INT_restore(irq);            
    
    return ret;
}


MXRC spacc_packet_dequeue_irq(PSPACCDRVCTX pElpCtx, PCRYPTOCTX ctx, UINT32 * size, int demap)
{
    MXRC ret = SPACC_RC_OK;
    volatile ELPSPACCREGS* pSpaccRegs;  
    SINT32 handle = ctx->pVecData->handle;

    pSpaccRegs =  pElpCtx->pSpaccRegs;

    if (ctx == NULL)
    {
        ret = (UINT32)SPACC_RC_CRYPTO_FAILED;
    }
    else
    {
        switch (pSpaccRegs->SpaccRegStatus.RetCode)
        {
            case SPACC_ICVFAIL:
                ret = SPACC_RC_CRYPTO_AUTHENTICATION_FAILED;
                if (ctx->ddt_idx > 0)
                {
                    spacc_release_ddt(handle);
                    spacc_release_dst_ddt(handle);
                }
                break;
                
            case SPACC_MEMERR:
                if (ctx->ddt_idx > 0)
                {
                    spacc_release_ddt(handle);
                    spacc_release_dst_ddt(handle);
                }
                ret = SPACC_RC_CRYPTO_MEMORY_ERROR;
                break;

            case SPACC_BLOCKERR:
                if (ctx->ddt_idx > 0)
                {
                    spacc_release_ddt(handle);
                    spacc_release_dst_ddt(handle);
                }
                ret = SPACC_RC_CRYPTO_INVALID_BLOCK_ALIGNMENT;
                break;

            case SPACC_OK:
                ctx->pkt_sz = pSpaccRegs->SpaccRegProcLen - (pSpaccRegs->SpaccRegAadLen) + ((ctx->op == OP_DECRYPT) ? 0 : ctx->icv_len);
                ctx->job_done = 1;

                // check if we are processing ddt chain or a single ddt
                if (ctx->ddt_idx > 0)
                {
                    if (demap == SPACC_DEQUEUE_UNMAP)
                    {
                        spacc_release_ddt(handle);
                        ret = spacc_release_dst_ddt(handle);
                    }
                    else
                    {
                        ret = SPACC_RC_OK;
                    }
                }

                ctx->in_buf = ctx->in_map = ctx->out_map = ctx->out_buf = NULL;
                ctx->out_sz = ctx->in_sz = 0;
                ret = SPACC_RC_OK;
                break;
        }

        ctx->job_err = ret;

        if (ctx->job_err == SPACC_RC_OK)
        {
            if (ctx->job_done)
            {
                //printf("Size:  %d  and Packet size %d\n", *size, ctx->pkt_sz);
                *size = ctx->pkt_sz;
                ctx->job_done = 0;
                ctx->job_err = (UINT32)SPACC_RC_CRYPTO_INPROGRESS;
                ctx->job_id = 0xFFFFFFFF;
            }
            else
            {
                *size = 0;
                return SPACC_RC_CRYPTO_INPROGRESS;
            }
        }

        if (ret != (UINT32)SPACC_RC_CRYPTO_INPROGRESS && ret != SPACC_RC_OK)
        {
            //printf("error ! Size:  %d  and Packet size %d\n",*size, ctx->pkt_sz);
            *size = 0;
            ctx->job_done = 0;
            ctx->job_id = 0xFFFFFFFF;
        }

    }

    return ret;
}

PCRYPTOCTX spacc_get_context_done(PSPACCDRVCTX pElpCtx)
{
    PCRYPTOCTX ctx = NULL;
    UINT32 x;

        pElpCtx->pSpaccRegs->SpaccRegStatPop = 0x1;    
        
        for (x = 0; x < CRYPTO_CONTEXTS_MAX && pElpCtx->mctx[x].job_id != pElpCtx->pSpaccRegs->SpaccRegStatus.SwId; x++)
            ;

        if (x == CRYPTO_CONTEXTS_MAX)
        {
            //uart_printf("sspacc_get_context_done: not found context x = %d\r\n", x);        
            return NULL;
        }

        ctx = &pElpCtx->mctx[x];  

        if (ctx == NULL)
        {
            //uart_printf("sspacc_get_context_done: not found context\r\n");
            return ctx;
        }


    return ctx;
}

#endif

