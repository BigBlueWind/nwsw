/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

**********************************************************************/

/************************************************************************

     Name:     LTE MAC Convergence Layer

     Type:     C source file

     Desc:     C source code for Midspeed Security Protocol Accelerator (SPAcc)

     File:     ys_ms_spacc.c

     Sid:

     Prg:      sbalakrishna

**********************************************************************/




#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines  */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTEE defines */
#include "ys_ms_err.h"        /* CL error header file */
#include "tfu.h"
#include "ctf.h"
#include "lys.h"
#include "spaccdrv.h"
#include "ys_ms.h"
#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif


#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_lib.x"
#include "tfu.x"
#include "ctf.x"
#include "lys.x"

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "supervisor.h"
#ifdef RNT_ARM_CC_UNSUPP
#include "maciface.h"
#include "msgroute.h"
#include "transport.h"
#include "ctrlmsg.h"
#include "auxiface.h"
#else
#include "resultcodes.h"
#include "basetypes.h"
#endif
#include "ys_ms.x"

#ifdef TENB_AS_SECURITY
/* Function Prototype Declarations */

PUBLIC PCRYPTO_VECTOR_DATA ysSPAccDiagCreateCryptVec(YsMsSecInfo *pSecInfo, 
                                                       YsMsSpaccSecOp secOp, Buffer *mBuf);


/* Global Declaration for UE Security Context Block */

YsUeMsSecCb ueSecCb;
/* Global Declaration of extern variables required by hardware accelerators */
EXTERN PSPACCDRVCTX pSPAccDrvCtx;
EXTERN SPACC_DIAG_CTX g_SpaccDiagCtx;

DDT_LST *paDdtPtLst[MAX_PRE_ALLOC_BUFF];

#define HMAC_SHA256_EN
#define AES

#ifdef ANSI
PRIVATE S16 ysMsPreAllocDdtLst
(
)
#else
PRIVATE S16 ysMsPreAllocDdtLst(void)
(
)
#endif
{
    U32 idx = 0;

    if(paDdtPtLst[0] == NULL)
    {
        for(idx = 0; idx < MAX_PRE_ALLOC_BUFF; idx++)
        {
#ifndef SPACC_DBG
            /*MSPD_DBG("\nBefore Allocation\t");
            MSPD_DBG("\tueSecCb.paDdtPtLst[%d] = %x\t",idx,ueSecCb.paDdtPtLst[idx]);
            MSPD_DBG(" ueSecCb.paDdtCtLst[%d] = %x\n",idx,ueSecCb.paDdtCtLst[idx]);*/
#endif
            paDdtPtLst[idx] = (DDT_LST *)MxHeapAlloc(NcnbHeap,sizeof(DDT_LST));
            
#ifndef SPACC_DBG
            /*MSPD_DBG("\nAfter Allocation\t");
            MSPD_DBG(" ueSecCb.paDdtPtLst[%d] = %x\t",idx,ueSecCb.paDdtPtLst[idx]);
            MSPD_DBG(" ueSecCb.paDdtCtLst[%d] = %x\n",idx,ueSecCb.paDdtCtLst[idx]);*/
#endif
        }/* End of for(idx = 0; idx < MAX_PRE_ALLOC_BUFF; idx++) */

    }/* End of if(ueSecCb->pDdtLst == NULL) */
    RETVALUE(ROK);

}

S16 ysMsSPAccDiagDeleteCryptVec(PCRYPTO_VECTOR_DATA inVecData)
{
    PDDT_LST delDDT = NULL, nextDDT = NULL;

    if (inVecData)
    {   
        delDDT = inVecData->ptLst;

        while(delDDT)
        {
            if (delDDT->ddt.ptr != NULL)
            {
                MxHeapFree(NcnbHeap, (PUINT32)delDDT->ddt.ptr);
            }
            
            nextDDT = delDDT->next;
            MxHeapFree(NcnbHeap, (PUINT32)delDDT);
            delDDT  = nextDDT;
        }

        delDDT = inVecData->ctLst;

        while(delDDT)
        {
            if (delDDT->ddt.ptr != NULL)
            {
                MxHeapFree(NcnbHeap, (PUINT32)delDDT->ddt.ptr);
            }
            
            nextDDT = delDDT->next;
            MxHeapFree(NcnbHeap, (PUINT32)delDDT);
            delDDT  = nextDDT;
        }
        MxHeapFree(NcnbHeap, (PUINT32)inVecData);
    }
    return ROK;
}


/**
 * @brief     - Funtion to prepare data structures for Ciphering and integrity protection.
 *
 * @details   - This function accepts inputs from PDCP/ TENB APP to prepare mindspeed internal data structure  
 *              before calling the funtion to perform actualy operation.
 *              
 *
 *
 * @param[in] - YsMsSecInfo *pSecInfo 
 *
 * @return    - ROK/RFAILED
 */
#if 1
#ifdef ANSI
PUBLIC PCRYPTO_VECTOR_DATA ysSPAccDiagCreateCryptVec
(
YsMsSecInfo *pSecInfo, 
YsMsSpaccSecOp secOp, 
Buffer *mBuf
)
#else
#endif
{
    PDDT_LST pPt = NULL, pCt = NULL;
    PCRYPTO_VECTOR_DATA pVec = NULL;
    Data flatBuf[2000]; /*flat buffer in mBuf data either for Ciphering or integrity protection is copied*/
    MsgLen  len; /*length of teh buffer to Ciphered/Integrity protected*/
    MsgLen  len1;
    U16 index = 0;
    U8 i;
    /*static int abc=0;*/


    pVec = (PCRYPTO_VECTOR_DATA) MxHeapAlloc (NcnbHeap, sizeof(CRYPTO_VECTOR_DATA));

    if (pVec == NULL)
    {
        MSPD_DBG("sSPAccDiagCreateCryptVec: can't allocate memory for PCRYPTO_VECTOR_DATA \n");
        return NULL;
    }
    cmMemset((U8*)pVec, 0x00, sizeof(CRYPTO_VECTOR_DATA));    

    if((secOp == YS_MS_CIPHERING) || (secOp == YS_MS_INTEGRITY))
    {
       #if 0 /*Optimization code commented for now*/
          tmpMBuf = mBuf->b_cont;
          bufIdx = 0;
         while((tmpMBuf) && (idx < MAX_PRE_ALLOC_BUFF))
         {
            pPt->ddt.ptr = tmpMBuf->b_rptr;
            pPt->ddt.len = tmpMBuf->b_wptr - tmpMBuf->b_rptr;
            pPt->next = &ueSecCb.paDdtPtLst[idx];
  
           tmpMBuf = tmpMBuf->b_cont;
           if(tmpMBuf != NULL)
	   {
              pCt = pCt->next;
              pPt = pPt->next;
              idx++;
	   }
         }
       #else
          SFndLenMsg(mBuf, &len);
 
          /*TODO: This copy is inefficient. Should try to remove this during optimization phase */
          SCpyMsgFix(mBuf, 0, len, flatBuf, &len1);
       #endif
     }

    /*ptLst is plain test list*/
    pVec->ptLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));
    pPt = pVec->ptLst;
    _ASSERT_PTR(pPt);            

    pVec->ctLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));            
    /*ctLst standsfor ciphered text*/   
    pCt = pVec->ctLst;
    _ASSERT_PTR(pCt);                        


    if(secOp == YS_MS_CIPHERING)
    {
       /*MSPD_DBG("pSecInfo->ciphAlgoType = %x ueAddresss %p\n",pSecInfo->ciphAlgoType, pSecInfo);*/
       switch (pSecInfo->ciphAlgoType)
       {
           /*AES alogorithm Ciphering*/
           case YS_MS_SPACC_EEA2:
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = CRYPTO_MODE_AES_CTR;         
               pVec->hash_mode = NULL;
   
               pVec->keysize = LTE_CIPHER_KEY_LEN;

			   /*depending on RB ID use control plane or user plane ciphering keys*/
    			if(pSecInfo->rbType == 1)
                  cmMemcpy(pVec->key, pSecInfo->upCiph_key, pVec->keysize);
    			else
                  cmMemcpy(pVec->key, pSecInfo->cpCiph_key, pVec->keysize);

               /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
               REG32(&pVec->iv[0]) = REG32(&pSecInfo->count[0]);
               REG32(&pVec->iv[4]) = (pSecInfo->rbId << 3) | (pSecInfo->dir << 2); 
               cmMemset(&pVec->iv[8], 0x00, 8);
               pVec->iv_size = 16;
   
               pPt->next = NULL;
               pPt->ddt.len = len /*% 8 ? len/8 +1: len/8*/;
               pPt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pPt->ddt.len);
               _ASSERT_PTR(pPt->ddt.ptr);            
               cmMemcpy (pPt->ddt.ptr, (unsigned char *) flatBuf, pPt->ddt.len);                  
               
   
               pCt->next = NULL;
               pCt->ddt.len = pPt->ddt.len;
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);
               break;
   
           case YS_MS_SPACC_EEA1:
			           /*SNOW3G alogorithm Ciphering*/
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = CRYPTO_MODE_SNOW3G_UEA2;
               pVec->hash_mode = NULL;
   
/*MSPD_DBG("*****rbType = %d, rbId = %d\n",pSecInfo->rbType, pSecInfo->rbId);*/

   
               if(pSecInfo->rbType == 1)
               {
	/*MSPD_DBG("Actual User Plane Ciphering key:");
		for(index=0;index<16;index++)
		MSPD_DBG("%x ", pSecInfo->upCiph_key[index]);
		MSPD_DBG("\n");
		MSPD_DBG("Ciphering the user plane packet\n");*/


                     /*Use User plane Ciphering key*/
                     REG32(&pVec->key[12]) = REG32(&pSecInfo->upCiph_key[0]);
                     REG32(&pVec->key[8]) = REG32(&pSecInfo->upCiph_key[4]);
                     REG32(&pVec->key[4]) = REG32(&pSecInfo->upCiph_key[8]);
                     REG32(&pVec->key[0]) = REG32(&pSecInfo->upCiph_key[12]);
                }
       			else
       			{
		/*MSPD_DBG("Actual Control Plane Ciphering key:");
			for(index=0;index<16;index++)
			MSPD_DBG("%x ", pSecInfo->cpCiph_key[index]);
			MSPD_DBG("\n");
			MSPD_DBG("Ciphering the control plane packet\n");*/

                     /*Use Control plane Ciphering key*/
                     REG32(&pVec->key[12]) = REG32(&pSecInfo->cpCiph_key[0]);
                     REG32(&pVec->key[8]) = REG32(&pSecInfo->cpCiph_key[4]);
                     REG32(&pVec->key[4]) = REG32(&pSecInfo->cpCiph_key[8]);
                     REG32(&pVec->key[0]) = REG32(&pSecInfo->cpCiph_key[12]);
       
       			}	
               /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
               REG32(&pVec->key[20]) = REG32(&pSecInfo->count[0]);
               pVec->key[16] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2);
               REG32(&pVec->key[28]) = REG32(&pSecInfo->count[0]);
               pVec->key[24] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2);        
   

		/*MSPD_DBG("*******dir = %x \n",pSecInfo->dir);

		MSPD_DBG("**********value of count is:\n");	   
		for(index =0 ; index<4; index++)
		MSPD_DBG("%x ", pSecInfo->count[index]);
		MSPD_DBG("\n");*/

               pVec->keysize = 32;
   
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;
   
               pPt->next = NULL;
               pPt->ddt.len = len/*% 8 ? len/8 +1: len/8*/;
	       pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
               _ASSERT_PTR(pPt->ddt.ptr);
               cmMemcpy(pPt->ddt.ptr,(UINT8 *) flatBuf, pPt->ddt.len);
   


               pCt->next = NULL;
               pCt->ddt.len = pPt->ddt.len;
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);
               break;
   
          default:
           break;
       }
    }
    else if(secOp == YS_MS_INTEGRITY)
    {
       switch (pSecInfo->intAlgoType)
       {
           case YS_MS_SPACC_EIA2:
		   	           /*AES alogorithm integrity*/
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = NULL;
               pVec->hash_mode = CRYPTO_MODE_MAC_CMAC;

               /*Copy KEY like below*/
               pVec->hmac_keysize = LTE_INTEG_KEY_LEN; /*16*/ 
               cmMemcpy(&pVec->hmac_key[0], pSecInfo->integ_key, pVec->hmac_keysize);
   
   
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;
   
               if(len < 8)
               {
                  /*As per new MSPD security library all data less that 8 bytes 
                    will be sent in one list instead of 2*/
                  pPt->ddt.len = 8 + len;
                  pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
                  _ASSERT_PTR(pPt->ddt.ptr);
                  cmMemset(pPt->ddt.ptr, 0x00, pPt->ddt.len);

	          /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
                  REG32(&pPt->ddt.ptr[0]) = REG32(&pSecInfo->count);
                  pPt->ddt.ptr[4] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2); 
                  pCt->ddt.len = pPt->ddt.len;
                  cmMemcpy(&(pPt->ddt.ptr[8]),(UINT8 *) flatBuf, pPt->ddt.len);
                  pPt->next = NULL;
                  for(i=0;i<pPt->ddt.len;i++)
                     MSPD_DBG("%x ", pPt->ddt.ptr[i]);
                  MSPD_DBG("\n");
               }
               else
               {
                  pPt->ddt.len = 8;
                  pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
                  _ASSERT_PTR(pPt->ddt.ptr);
                  cmMemset(pPt->ddt.ptr, 0x00, pPt->ddt.len);

   			   /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
                  REG32(&pPt->ddt.ptr[0]) = REG32(&pSecInfo->count);
                  pPt->ddt.ptr[4] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2); 
     
                  pCt->ddt.len = pPt->ddt.len;
    
                  pPt->next = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));;
   
                  pPt = pPt->next;
                  pPt->next = NULL;
                  pPt->ddt.len = len /*% 8 ? len/8 +1: len/8*/;
                  pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
                  _ASSERT_PTR(pPt->ddt.ptr);
                  cmMemcpy(pPt->ddt.ptr,(UINT8 *) flatBuf, pPt->ddt.len);
               }   
   
               pCt->next = NULL;
               pCt->ddt.len += pPt->ddt.len;
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);
   
               pPt = pVec->ptLst;
               break;
   
   
           case YS_MS_SPACC_EIA1:
		   			   	           /*Snow3G alogorithm integrity*/
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = NULL;
               pVec->hash_mode = CRYPTO_MODE_MAC_SNOW3G_UIA2;
  

               /*pVec->auxinfo_bit_align =  length_uia2[testSet]& 0x7; TODO what to fill here*/
               /*Copy KEY like below*/
               REG32(&pVec->hmac_key[12]) = REG32(&pSecInfo->integ_key[0]);
               REG32(&pVec->hmac_key[8]) = REG32(&pSecInfo->integ_key[4]);
               REG32(&pVec->hmac_key[4]) = REG32(&pSecInfo->integ_key[8]);
               REG32(&pVec->hmac_key[0]) = REG32(&pSecInfo->integ_key[12]);

               /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
			   REG32(&pVec->hmac_key[20]) = REG32(&pSecInfo->count[0]);
               pVec->hmac_key[20] = (pSecInfo->count[0] ^ pSecInfo->dir << 7);
                REG32(&pVec->hmac_key[16]) = (pVec->hmac_key[16] ^ (pSecInfo->rbId << 3));/*REG32(&pSecInfo->fresh[0]);*/
               pVec->hmac_key[18] = (pVec->hmac_key[18] ^ pSecInfo->dir << 7); 
               REG32(&pVec->hmac_key[28]) = REG32(&pSecInfo->count[0]);
               REG32(&pVec->hmac_key[24]) = (pVec->hmac_key[24] ^ (pSecInfo->rbId << 3));/*REG32(&pSecInfo->fresh[0]);*/
   
               pVec->hmac_keysize= 32;


   
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;
   
               pPt->next = NULL;
               pPt->ddt.len = len /*% 8 ? len/8 +1: len/8*/;
               pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
               _ASSERT_PTR(pPt->ddt.ptr);
               cmMemcpy(pPt->ddt.ptr,(UINT8 *) flatBuf, pPt->ddt.len);
               

               MSPD_DBG("dir = %x, fresh = %x\n",pSecInfo->dir, *(pSecInfo->fresh));

               MSPD_DBG("value of count is:\n");	   
	       for(index =0 ; index<4; index++)
	       MSPD_DBG("%x ", pSecInfo->count[index]);
	       MSPD_DBG("\n");

               MSPD_DBG("COUNT %x FRESH %x\n", REG32(&pSecInfo->count[0]), REG32(&pSecInfo->fresh[0]));   

               MSPD_DBG("value of fresh is:\n");	   
	       for(index =0 ; index<4; index++)
	       MSPD_DBG("%x ", pSecInfo->fresh[index]);
	       MSPD_DBG("\n");



               pCt->next = NULL;
          /*abc++;*/

	      if (pPt->ddt.len <4) 
		  pCt->ddt.len = 4;
	      else
		  pCt->ddt.len = pPt->ddt.len;
		  /*if(abc == 10)
		  	pCt->ddt.len = 4;*/
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);
               break;
   
          default:
               break;
           }
	}
        else
        {
               /*KDF*/
               
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = NULL;
               pVec->hash_mode = CRYPTO_MODE_HMAC_SHA256;
   
               pVec->hmac_keysize = YS_MS_KENB_LEN; /*32 bytes*/ 
               cmMemcpy(&pVec->hmac_key[0], pSecInfo->hash_key,YS_MS_KENB_LEN);
/* sidhartha */
               
 
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;

               pPt->next = NULL;
               pPt->ddt.len = pSecInfo->strSlen;
               pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
               _ASSERT_PTR(pPt->ddt.ptr);
               cmMemcpy(pPt->ddt.ptr,(UINT8 *) pSecInfo->strS, pPt->ddt.len);
                  


               pCt->next = NULL;
               pCt->ddt.len = YS_MS_KDF_OP_LEN;
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);

        }

        return pVec;

}
#else
#ifdef ANSI
PUBLIC PCRYPTO_VECTOR_DATA ysSPAccDiagCreateCryptVec
(
YsMsSecInfo *pSecInfo, 
YsMsSpaccSecOp secOp, 
Buffer *mBuf
)
#else
#endif
{
    PDDT_LST pPt = NULL, pCt = NULL;
    PCRYPTO_VECTOR_DATA pVec = NULL;
    /*Data flatBuf[300];*/ /*flat buffer in mBuf data either for Ciphering or integrity protection is copied*/
    Data *flatBuf;
    MsgLen  len; /*length of teh buffer to Ciphered/Integrity protected*/
    MsgLen  len1;
    U16 index = 0;
    /*static int abc=0;*/
    Buffer *tmpMBuf;


    pVec = (PCRYPTO_VECTOR_DATA) MxHeapAlloc (NcnbHeap, sizeof(CRYPTO_VECTOR_DATA));

    if (pVec == NULL)
    {
        MSPD_DBG("sSPAccDiagCreateCryptVec: can't allocate memory for PCRYPTO_VECTOR_DATA \n");
    }
    cmMemset((U8*)pVec, 0x00, sizeof(CRYPTO_VECTOR_DATA));    


    if(secOp == YS_MS_CIPHERING)
    {
        MSPD_DBG("################### calling prealloc DDT \n");
        ysMsPreAllocDdtLst();        

        pVec->ptLst = paDdtPtLst[index++];
        pVec->ctLst = pVec->ptLst;

        pPt = pVec->ptLst;
        pCt = pVec->ctLst;

        tmpMBuf = mBuf->b_cont;

        while(tmpMBuf && index < MAX_PRE_ALLOC_BUFF)
        {
            pPt->ddt.ptr = tmpMBuf->b_rptr;
            pPt->ddt.len = tmpMBuf->b_wptr - tmpMBuf->b_rptr;

            MxCacheClean(pPt->ddt.ptr,pPt->ddt.len);

            tmpMBuf = tmpMBuf->b_cont;
            if(tmpMBuf != NULL)
            {
                pPt->next = paDdtPtLst[index++];

                pCt = pCt->next;
                pPt = pPt->next;
            }
            else
            {
                pPt->next = NULL;
                pCt->next = NULL;
            }
        }
       MSPD_DBG("pSecInfo->ciphAlgoType = %x ueAddresss %p\n",pSecInfo->ciphAlgoType, pSecInfo);	
       switch (pSecInfo->ciphAlgoType)
       {
           /*AES alogorithm Ciphering*/
           case YS_MS_SPACC_EEA2:
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = CRYPTO_MODE_AES_CTR;         
               pVec->hash_mode = NULL;
   
               pVec->keysize = LTE_CIPHER_KEY_LEN;

			   /*depending on RB ID use control plane or user plane ciphering keys*/
    			if(pSecInfo->rbType == 1)
                  cmMemcpy(pVec->key, pSecInfo->upCiph_key, pVec->keysize);
    			else
                  cmMemcpy(pVec->key, pSecInfo->cpCiph_key, pVec->keysize);

               /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
               REG32(&pVec->iv[0]) = REG32(&pSecInfo->count[0]);
               REG32(&pVec->iv[4]) = (pSecInfo->rbId << 3) | (pSecInfo->dir << 2); 
               cmMemset(&pVec->iv[8], 0x00, 8);
               pVec->iv_size = 16;
   
               break;
   
           case YS_MS_SPACC_EEA1:
			           /*SNOW3G alogorithm Ciphering*/
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = CRYPTO_MODE_SNOW3G_UEA2;
               pVec->hash_mode = NULL;
   
MSPD_DBG("*****rbType = %d, rbId = %d\n",pSecInfo->rbType, pSecInfo->rbId);

   
               if(pSecInfo->rbType == 1)
               {
	MSPD_DBG("Actual User Plane Ciphering key:");
		for(index=0;index<16;index++)
		MSPD_DBG("%x ", pSecInfo->upCiph_key[index]);
		MSPD_DBG("\n");
		MSPD_DBG("Ciphering the user plane packet\n");


                     /*Use User plane Ciphering key*/
                     REG32(&pVec->key[12]) = REG32(&pSecInfo->upCiph_key[0]);
                     REG32(&pVec->key[8]) = REG32(&pSecInfo->upCiph_key[4]);
                     REG32(&pVec->key[4]) = REG32(&pSecInfo->upCiph_key[8]);
                     REG32(&pVec->key[0]) = REG32(&pSecInfo->upCiph_key[12]);
                }
       			else
       			{
		MSPD_DBG("Actual Control Plane Ciphering key:");
			for(index=0;index<16;index++)
			MSPD_DBG("%x ", pSecInfo->cpCiph_key[index]);
			MSPD_DBG("\n");
			MSPD_DBG("Ciphering the control plane packet\n");

                     /*Use Control plane Ciphering key*/
                     REG32(&pVec->key[12]) = REG32(&pSecInfo->cpCiph_key[0]);
                     REG32(&pVec->key[8]) = REG32(&pSecInfo->cpCiph_key[4]);
                     REG32(&pVec->key[4]) = REG32(&pSecInfo->cpCiph_key[8]);
                     REG32(&pVec->key[0]) = REG32(&pSecInfo->cpCiph_key[12]);
       
       			}	
               /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
               REG32(&pVec->key[20]) = REG32(&pSecInfo->count[0]);
               pVec->key[16] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2);
               REG32(&pVec->key[28]) = REG32(&pSecInfo->count[0]);
               pVec->key[24] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2);        
   

		MSPD_DBG("*******dir = %x \n",pSecInfo->dir);

		MSPD_DBG("**********value of count is:\n");	   
		for(index =0 ; index<4; index++)
		MSPD_DBG("%x ", pSecInfo->count[index]);
		MSPD_DBG("\n");

               pVec->keysize = 32;
   
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;
   
               break;
   
          default:
           break;
       }
    }
    else if(secOp == YS_MS_INTEGRITY)
    {
       SFndLenMsg(mBuf, &len);
 
       /*TODO: This copy is inefficient. Should try to remove this during optimization phase */
       /*SCpyMsgFix(mBuf, 0, len, flatBuf, &len1);*/
       YS_MS_ALLOC(flatBuf, len);
       SCpyMsgFix(mBuf, 0, len, flatBuf, &len1);

       /*ptLst is plain test list*/
       pVec->ptLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));
       pPt = pVec->ptLst;
       _ASSERT_PTR(pPt);            

       pVec->ctLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));            
       /*ctLst standsfor ciphered text*/   
       pCt = pVec->ctLst;
       _ASSERT_PTR(pCt);     

       switch (pSecInfo->intAlgoType)
       {
           case YS_MS_SPACC_EIA2:
		   	           /*AES alogorithm integrity*/
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = NULL;
               pVec->hash_mode = CRYPTO_MODE_MAC_CMAC;

               /*Copy KEY like below*/
               pVec->hmac_keysize = LTE_INTEG_KEY_LEN; /*16*/ 
               cmMemcpy(&pVec->hmac_key[0], pSecInfo->integ_key, pVec->hmac_keysize);
   
   
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;
   
               if(len < 8)
               {
                  /*As per new MSPD security library all data less that 8 bytes 
                    will be sent in one list instead of 2*/
                  pPt->ddt.len = 8 + len;
                  pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
                  _ASSERT_PTR(pPt->ddt.ptr);
                  cmMemset(pPt->ddt.ptr, 0x00, pPt->ddt.len);

	          /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
                  REG32(&pPt->ddt.ptr[0]) = REG32(&pSecInfo->count);
                  pPt->ddt.ptr[4] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2); 
                  /*pCt->ddt.len = pPt->ddt.len;*/
                  cmMemcpy(&(pPt->ddt.ptr[8]),(UINT8 *) flatBuf, pPt->ddt.len);
                  pPt->next = NULL;
               
               }
               else
               {
                  pPt->ddt.len = 8;
                  pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
                  _ASSERT_PTR(pPt->ddt.ptr);
                  cmMemset(pPt->ddt.ptr, 0x00, pPt->ddt.len);

   			   /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
                  REG32(&pPt->ddt.ptr[0]) = REG32(&pSecInfo->count);
                  pPt->ddt.ptr[4] = (pSecInfo->rbId<< 3) | (pSecInfo->dir<< 2); 
     
                  /*pCt->ddt.len = pPt->ddt.len;*/
    
                  pPt->next = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));;
   
                  pPt = pPt->next;
                  pPt->next = NULL;
                  pPt->ddt.len = len /*% 8 ? len/8 +1: len/8*/;
                  pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
                  _ASSERT_PTR(pPt->ddt.ptr);
                  cmMemcpy(pPt->ddt.ptr,(UINT8 *) flatBuf, pPt->ddt.len);
               }   

               pCt->next = NULL;
               pCt->ddt.len = pPt->ddt.len;
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);
   
               pPt = pVec->ptLst;
               break;
   
   
           case YS_MS_SPACC_EIA1:
		   			   	           /*Snow3G alogorithm integrity*/
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = NULL;
               pVec->hash_mode = CRYPTO_MODE_MAC_SNOW3G_UIA2;
  

               /*pVec->auxinfo_bit_align =  length_uia2[testSet]& 0x7; TODO what to fill here*/
               /*Copy KEY like below*/
               REG32(&pVec->hmac_key[12]) = REG32(&pSecInfo->integ_key[0]);
               REG32(&pVec->hmac_key[8]) = REG32(&pSecInfo->integ_key[4]);
               REG32(&pVec->hmac_key[4]) = REG32(&pSecInfo->integ_key[8]);
               REG32(&pVec->hmac_key[0]) = REG32(&pSecInfo->integ_key[12]);

               /*Put COUNT, BEARER and DIRECTION inforamtion in below format*/
			   REG32(&pVec->hmac_key[20]) = REG32(&pSecInfo->count[0]);
               pVec->hmac_key[20] = (pSecInfo->count[0] ^ pSecInfo->dir << 7);
                REG32(&pVec->hmac_key[16]) = (pVec->hmac_key[16] ^ (pSecInfo->rbId << 3));/*REG32(&pSecInfo->fresh[0]);*/
               pVec->hmac_key[18] = (pVec->hmac_key[18] ^ pSecInfo->dir << 7); 
               REG32(&pVec->hmac_key[28]) = REG32(&pSecInfo->count[0]);
               REG32(&pVec->hmac_key[24]) = (pVec->hmac_key[24] ^ (pSecInfo->rbId << 3));/*REG32(&pSecInfo->fresh[0]);*/
   
               pVec->hmac_keysize= 32;


   
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;
   
               pPt->next = NULL;
               pPt->ddt.len = len /*% 8 ? len/8 +1: len/8*/;
               pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
               _ASSERT_PTR(pPt->ddt.ptr);
               cmMemcpy(pPt->ddt.ptr,(UINT8 *) flatBuf, pPt->ddt.len);
               

               MSPD_DBG("dir = %x, fresh = %x\n",pSecInfo->dir, *(pSecInfo->fresh));

               MSPD_DBG("value of count is:\n");	   
	       for(index =0 ; index<4; index++)
	       MSPD_DBG("%x ", pSecInfo->count[index]);
	       MSPD_DBG("\n");

               MSPD_DBG("COUNT %x FRESH %x\n", REG32(&pSecInfo->count[0]), REG32(&pSecInfo->fresh[0]));   

               MSPD_DBG("value of fresh is:\n");	   
	       for(index =0 ; index<4; index++)
	       MSPD_DBG("%x ", pSecInfo->fresh[index]);
	       MSPD_DBG("\n");



               pCt->next = NULL;
          /*abc++;*/

	      if (pPt->ddt.len <4) 
		  pCt->ddt.len = 4;
	      else
		  pCt->ddt.len = pPt->ddt.len;
		  /*if(abc == 10)
		  	pCt->ddt.len = 4;*/
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);
               break;
   
          default:
               break;
           }
	}
        else
        {

              /*ptLst is plain test list*/
              pVec->ptLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));
              pPt = pVec->ptLst;
              _ASSERT_PTR(pPt);            

              pVec->ctLst = (PDDT_LST)MxHeapAlloc(NcnbHeap, sizeof(DDT_LST));            
             /*ctLst standsfor ciphered text*/   
             pCt = pVec->ctLst;
             _ASSERT_PTR(pCt);     

               /*KDF*/
               pVec->dir = OP_ENCRYPT;
               pVec->enc_mode = NULL;
               pVec->hash_mode = CRYPTO_MODE_HMAC_SHA256;
   
               pVec->hmac_keysize = YS_MS_KENB_LEN; /*32 bytes*/ 
               cmMemcpy(&pVec->hmac_key[0], pSecInfo->hash_key,YS_MS_KENB_LEN);
/* sidhartha */
             
 
               pVec->icv_mode = IM_ICV_IGNORE;
               pVec->icvpos = IP_ICV_IGNORE;

               pPt->next = NULL;
               pPt->ddt.len = pSecInfo->strSlen;
               pPt->ddt.ptr = MxHeapAlloc(NcnbHeap, pPt->ddt.len);
               _ASSERT_PTR(pPt->ddt.ptr);
               cmMemcpy(pPt->ddt.ptr,(UINT8 *) pSecInfo->strS, pPt->ddt.len);
                  


               pCt->next = NULL;
               pCt->ddt.len = YS_MS_KDF_OP_LEN;
               pCt->ddt.ptr = (PUINT8)MxHeapAlloc(NcnbHeap, pCt->ddt.len);
               _ASSERT_PTR(pCt->ddt.ptr);

        }
   if((secOp == YS_MS_CIPHERING) || (secOp == YS_MS_INTEGRITY))
    {
        YS_MS_FREE(flatBuf, len);
    }

        return pVec;

}
#endif

/**
 * @brief     - Derive the Keys using the SPA hardware accelerators
 *
 * @details   - This function opens a crypto context and initializes the contexts with the required     
 *              set of input parameters(Key,length,input string) and derives the output key using the
 *              hardware accelerator Key Derivation Funtion.
 *
 *
 * @param[in] - YsMsSecInfo *pSecInfo 
 *
 * @return    - ROK/RFAILED
 */

#ifdef ANSI
PUBLIC S16 ysMsKDF
(
  YsMsSecInfo *pSecInfo,
  U8          *key
)
#else
PUBLIC S16 ysMsKDF(*pSecInfo)
(
 YsMsSecInfo *pSecInfo;
)
#endif
{
   
    Buffer *mBuf=NULLP;
    PCRYPTO_VECTOR_DATA pVec = NULLP;
    DDT_LST *currDDT;
   


   /*Pass the plain text mBuf with other necessary information like Key, algorithm, DIRECTION, COUNT, 
      BEARER ID etc to setup the ciphering context*/
   
   pVec = ysSPAccDiagCreateCryptVec(pSecInfo, YS_MS_KDF, mBuf);
   if(pVec == NULL)
   {
      MSPD_DBG("ysSPAccDiagCreateCryptVec failure %s %d\n", __FILE__, __LINE__);
      RETVALUE(RFAILED);
   }

   /*NEW blocking API from mindspeed that actually does the ciphering*/
   if(SPACC_RC_OK != SPAccDrvRunBlocking(pVec))
   {
      MSPD_DBG("SPAccDrvRunBlocking failure %s %d\n", __FILE__, __LINE__);
      ysMsSPAccDiagDeleteCryptVec(pVec);
      RETVALUE(RFAILED);
   }

   /* Copy the derived AS keys*/
   currDDT = (pVec->dir == OP_ENCRYPT)? pVec->ctLst : pVec->ptLst;


  /*Copy the key in UE context*/
   if(pSecInfo->strSlen == 7)
   cmMemcpy((U8*)key,(U8*)&currDDT->ddt.ptr[16],16);
   else
     cmMemcpy((U8*)key,(U8*)&currDDT->ddt.ptr[0],32);

   /*release the context*/
   ysMsSPAccDiagDeleteCryptVec(pVec);

   
   RETVALUE(ROK);
   
}

/**
 * @brief         - This function is used to cipher the buffer received as 
 *                  input.
 *
 * @details       - This function sets the context for the ciphering / 
 *                  deciphering. Then specifies if the operation to be 
 *                  performed is encryption or decryption. The input packet 
 *                  buffer and its length is loaded to the DDT table. The 
 *                  packet is enqueued into the packet queue and the dequeue
 *                  function waits until the processed packet is received at 
 *                  the output queue.
 *                  
 * @param[in]     - YsMsSecInfo *pSecInfo
 * @param[in_out] - U8  *dataBuf
 * @param[in_out] - U32 dataLen
 *
 * @return ROK
 */

#ifdef ANSI
PUBLIC U32 ysMsSpaccCipherPkt
(
Void* *pSec,
Buffer *mBuf,
Buffer **opSdu
)
#else
PUBLIC U32 ysMsSpaccCipherPkt(*pSec, ueId, *mBuf)
YsMsSecInfo *pSec;
U16 ueId;
Buffer *mBuf
#endif
{

    PCRYPTO_VECTOR_DATA pVec = NULLP;
    DDT_LST *currDDT;
    YsMsSecInfo *pSecInfo = NULLP;


     pSecInfo = (YsMsSecInfo *) pSec;
    
   /*Pass the plain text mBuf with other necessary information like Key, algorithm, DIRECTION, COUNT, 
      BEARER ID etc to setup the ciphering context*/
   pVec = ysSPAccDiagCreateCryptVec(pSecInfo, YS_MS_CIPHERING, mBuf);
   if(pVec == NULL)
   {
      MSPD_DBG("ysSPAccDiagCreateCryptVec failure %s %d\n", __FILE__, __LINE__);
      RETVALUE(RFAILED);
   }

   /*NEW blocking API from mindspeed that actually does the ciphering*/
   if(SPACC_RC_OK != SPAccDrvRunBlocking(pVec))
   {
      MSPD_DBG("SPAccDrvRunBlocking failure %s %d\n", __FILE__, __LINE__);
      ysMsSPAccDiagDeleteCryptVec(pVec);
      RETVALUE(RFAILED);
   }

   /* Ciphered text will be in pVec->ctLst. This need to be copied/overwritten on buffer passed by PDCP*/
   currDDT = (pVec->dir == OP_ENCRYPT)? pVec->ctLst : pVec->ptLst;

   if (SAddPstMsgMult(currDDT->ddt.ptr, currDDT->ddt.len, *opSdu) != ROK) 
   {
	YS_DBG_ERR((_ysp, "SAddPstMsgMult: failed\n"));  
    ysMsSPAccDiagDeleteCryptVec(pVec);  
     RETVALUE(RFAILED);
   } 

   /*release the context*/
   ysMsSPAccDiagDeleteCryptVec(pVec);

      RETVALUE(ROK);
   
}


/**
 * @brief This function is used to Integrity Protect / Verify
 *        the buffer received as input.
 * @details This function sets the context for the integrity protection /
 *          verification.
 *          Then specifies if the operation to be performed
 *          is MAC- I generation or verification.
 *          The input packet buffer and its length is loaded to the DDT
 *          table.
 *          The packet is enqueued into the packet queue and the dequeue
 *          function waits until the processed packet is received at the
 *          output queue.
 * @param[in] YsMsSecInfo *pSecInfo
 * @param[in_out]  U8  *dataBuf
 * @param[in_out]  U32 dataLen
 * @return ROK
 */

#ifdef ANSI
PUBLIC U32 ysMsSpaccIntegProtPkt
(
Void *pSec,
Buffer *mBuf,
U32 macI
)
#else
PUBLIC U32 ysMsSpaccIntegProtPkt(*pSec, ueId, mBuf)
Void *pSec;
U16 ueId;
Buffer *mBuf;
U32 macI
#endif
{
    PCRYPTO_VECTOR_DATA pVec = NULLP;
    YsMsSecInfo *pSecInfo = NULLP;
    DDT_LST *currDDT;
   	U8 i;
    /*U8    macI[4];*/
 


   pSecInfo = (YsMsSecInfo *) pSec;
    
   pVec = ysSPAccDiagCreateCryptVec(pSecInfo, YS_MS_INTEGRITY, mBuf);
   if(pVec == NULL)
   {
      MSPD_DBG("ysSPAccDiagCreateCryptVec failure %s %d\n", __FILE__, __LINE__);
      RETVALUE(RFAILED);
   }

   if(SPACC_RC_OK != SPAccDrvRunBlocking(pVec))
   {
      MSPD_DBG("SPAccDrvRunBlocking failure %s %d\n", __FILE__, __LINE__);
      RETVALUE(RFAILED);
   }

   
   currDDT = (pVec->dir == OP_ENCRYPT)? pVec->ctLst : pVec->ptLst;

   MSPD_DBG("MAC-I: ");
   for(i=0;i<4;i++)
   	MSPD_DBG("%x ",currDDT->ddt.ptr[i]);
   MSPD_DBG("\n");
   #if 0
   	 SPrntMsg(mBuf, 0, 0);
   #endif
  if(pSecInfo->dir == YS_SEC_DIR_DL)
  {
      /*Append 4 bytes MAC-I to the end of the PDU buffer received in downlink direction.*/
      SAddPstMsgMult(currDDT->ddt.ptr,YS_MS_MAC_SIZE_BYTES,mBuf);

  }
  else if(pSecInfo->dir == YS_SEC_DIR_UL)
  {
     /*Comapre the calculated MACI with the received MACI from UE for a given message in UL direction.*/
      if(cmMemcmp((U8*)&macI, currDDT->ddt.ptr, YS_MS_MAC_SIZE_BYTES) != ROK)
      {
          ysMsSPAccDiagDeleteCryptVec(pVec);
          RETVALUE(RFAILED);
      }
  }

   ysMsSPAccDiagDeleteCryptVec(pVec);

   RETVALUE(ROK);

}
/* End of function ysMsSpaccIntegProtPkt() */
#endif /*TENB_AS_SECURITY*/


/********************************************************************30**

         End of file:     ys_ms_spacc.c@@/main/1 - Sun Mar 28 21:19:19 2010

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/
/********************************************************************90**

     ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
  /main/1      ---    sbalakrishna      1. initial release.
*********************************************************************91*/


