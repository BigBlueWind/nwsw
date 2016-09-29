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

*********************************************************************17*/


/********************************************************************20**
  
        Name:    PDCP - Database module file
    
        Type:    C source file
  
        Desc:    Source code for Database Module functions such as, 

                  -  pjDbmBufInit
                  -  pjDbmInsTxEnt
                  -  pjDbmGetTxEnt
                  -  pjDbmGetTxEntSn
                  -  pjDbmDelTxEnt
                  -  pjDbmTxDeInit
                  -  pjDbmInsRxEnt
                  -  pjDbmGetRxEnt
                  -  pjDbmDelRxEnt
                  -  pjDbmRxDeInit

        File:    gp_pj_dbm.c

        Sid:      gp_pj_dbm.c@@/main/1 - Fri Nov 13 14:13:44 2009
 
        Prg:     mn
  
*********************************************************************21*/

/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm5.h"                /* Timer Functions */
#include "cm_lte.h"             /* common LTE header file */
#include "cm_hash.h"            /* common hash module  file */
#include "cm_llist.h"           /* common list header file */
#include "cpj.h"                /* RRC layer */
#include "pju.h"                /* PDCP service user */
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
#include "lpj.h"                /* RRC layer */
#include "kwu.h"                /* RLC service user */
#include "lkw.h"                /* LM Interface */
#include "rgu.h"                /* MAC layer */
#include "kw_env.h"             /* RLC environment options */
#include "kw.h"                 /* RLC layer */
#include "kw_err.h"

/* extern (.x) include files */
#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* Timer Functions */
#include "cm_hash.x"            /* common hash module */
#include "cm_lte.x"             /* common LTE file */
#include "cm_llist.x"           /* common list header file */
#include "cpj.x"                /* RRC layer */
#include "pju.x"                /* PDCP service user */
#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "lpj.x"                /* LM Interface */
#include "rgu.x"                /* MAC later */
#include "kw.x"                 /* RLC layer */



/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */

/** @file gp_pj_dbm.c
@brief PDCP DBM Module
**/

/*****************************************************************************
 *                  HANDLER FOR THE TRANSMISSION BUFFER
 ****************************************************************************/

/**
 *
 * @brief Handler to initialize  Buffer
 *
 * @b Description
 *        This function is invoked by config to initialize the Buffer
 *        hash List
 *
 *  @param[in]    buf       Rx/Tx Buffer Control Point
 *  @param[in]    numBins   number of Bins
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmBufInit
(
PjBuf       *buf,            /* !< Rx/Tx Buffer */
U8          numBins          /* !< number of Bins */
)
#else
PUBLIC S16 pjDbmBufInit(buf, numBins)
PjBuf       *buf;            /* !< Rx/Tx Buffer */ 
U8          numBins;         /* !< number of Bins */             
#endif
{
   U8       hashIndex;        /* HashIndex of array */

   TRC3(pjDbmBufInit)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmBufInit(buf, numBins(%d)\n", numBins));

   /* Initialize CmLListCps*/
   PJ_ALLOC(buf->datQ, (sizeof(CmLListCp) * numBins));
#if (ERRCLASS & ERRCLS_DEBUG)
   if (buf->datQ == NULLP)
   {
      PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_ERROR), (_kwPBuf,
               "pjDbmBufInit: ALLOC Failed for Buf.datQ.\n"));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   for(hashIndex = 0; hashIndex < numBins; hashIndex++)
   {
      cmLListInit(&buf->datQ[hashIndex]);
   }

   /* Initialistations of buf */
   buf->numEntries = 0;
   buf->numBins    = numBins;

   RETVALUE(ROK);
} /* pjDbmBufInit */


/**
 *
 * @brief Handler to insert into a TX Buffer
 *
 * @b Description
 *        This function is invoked to insert into
 *        the transmission buffer hash List
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    txEnt       Transmission Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmInsTxEnt
(
PjBuf       *buf,             /* !< Tx Buffer */  
PjTxEnt     *txEnt            /* !< Tx Entry */              
)
#else
PUBLIC S16 pjDbmInsTxEnt(buf, txEnt)
PjBuf       *buf;             /* !< Tx Buffer */              
PjTxEnt     *txEnt;           /* !< Tx Entry */               
#endif
{
   U8       hashKey;          /* Computed HashKey */

   TRC3(pjDbmTxBufIns)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmInsTxEnt(buf, txEnt(%ld)\n", txEnt->count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmInsTxEnt(buf, txEnt(%d)\n", txEnt->count));
#endif
   /* Get the hash index */
   hashKey = PJ_HASH_FN(buf, txEnt->count);

   txEnt->lstEnt.node = (PTR)txEnt;
   cmLListAdd2Tail(&buf->datQ[hashKey], &txEnt->lstEnt);
   buf->numEntries ++;

   RETVALUE(ROK);
} /* pjDbmTxBufIns */


/**
 *
 * @brief Handler to fetch from a TX Buffer
 *
 * @b Description
 *        This function is invoked to fetch from
 *         the transmission buffer hash List
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    count       Count of the PjTxEnt to retrieve
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC PjTxEnt* pjDbmGetTxEnt
(
PjBuf       *buf,        /* !< Tx Buffer  */                    
U32          count       /* !< Count of the PjTxEnt to retrieve */
)
#else
PUBLIC PjTxEnt* pjDbmGetTxEnt(buf, count)
PjBuf       *buf;        /* !<Tx Buffer  */                     
U32         count;       /* !<Count of the PjTxEnt to retrieve */ 
#endif
{
   U8          hashKey;     /* Computed Hash Key */
   CmLListCp   *datQ;       /* Pointer to the ListCp */
   PjTxEnt     *tmpEnt;     /* Loop Var - Pointer to PjTxEnt */

   TRC3(pjDbmGetTxEnt)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmGetTxEnt(buf, txEnt(%ld)\n", count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmGetTxEnt(buf, txEnt(%d)\n", count));
#endif 
   /* Call the hash function */
   hashKey = PJ_HASH_FN(buf, count);

   datQ = &buf->datQ[hashKey];

   cmLListFirst(datQ);
   while(cmLListCrnt(datQ))
   {
      tmpEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(datQ));
      if(tmpEnt->count  == count)
      {
         RETVALUE(tmpEnt);
      }
      cmLListNext(datQ);
   }
   RETVALUE(NULLP);
} /* pjDbmGetTxEnt */


/**
 *
 * @brief Handler to fetch from a TX Buffer
 *
 * @b Description
 *        This function is fetch the node based on the SN.
 *        This function will be called only for the case of a 
 *        PDCP STATUS REPORT being received by the DRB AM. 
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    sn          SN to get the Ent
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC PjTxEnt* pjDbmGetTxEntSn
(
PjBuf       *buf,        /* !< Tx Buffer  */                     
U16          sn          /* !< SN of the PjTxEnt to retrieve */ 
)
#else
PUBLIC PjTxEnt* pjDbmGetTxEntSn(buf, sn)
PjBuf       *buf;        /* !< Tx Buffer  */                      
U16         sn;          /* !< SN of the PjTxEnt to retrieve */     
#endif
{
   U8          hashKey;  /* Computed hash index */
   CmLListCp   *datQ;    /* Pointer to the ListCP */
   PjTxEnt     *tmpEnt;  /* Loop Var - pointer to PjTxEnt */

   TRC3(pjDbmGetTxEnt)

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmGetTxEnt(buf, sn(%d)\n", sn));

   /* Call the hash function */
   hashKey = PJ_HASH_FN(buf, sn);

   datQ = &buf->datQ[hashKey];

   /* Loop through to find the entry that matches */
   cmLListFirst(datQ);
   while(cmLListCrnt(datQ))
   {
      tmpEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(datQ));
      if(tmpEnt->sn == sn)
      {
         RETVALUE(tmpEnt);
      }
      cmLListNext(datQ);
   }
   RETVALUE(NULLP);

} /* pjDbmGetTxEntSn */

/**
 *
 * @brief Handler to delete from a TX Buffer
 *
 * @b Description
 *        This function is invoked by to insert into
 *         the transaction buffer hash List
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    count       count to get the Ent
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelTxEnt
(
PjBuf       *buf,     /* !< Tx Buffer  */                     
U32         count     /* !< Count of the PjTxEnt to delete */ 
)
#else
PUBLIC S16 pjDbmDelTxEnt(buf, count)
PjBuf       *buf;     /* !< Tx Buffer  */                      
U32         count;    /* !< Count of the PjTxEnt to delete */    
#endif
{
   U8          hashKey;      /* Computed Hash Key */
   CmLListCp   *datQ;        /* Pointer to the ListCP */
   PjTxEnt     *delEnt;      /* Loop Var - pointer to PjTxEnt */

   TRC3(pjDbmDelTxEnt)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmDelTxEnt(buf,  count(%ld)\n", count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmDelTxEnt(buf,  count(%d)\n", count));
#endif

   /* search for the entry */
   delEnt   =  pjDbmGetTxEnt(buf, count);
   if(delEnt == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Compute the hash index */
   hashKey = PJ_HASH_FN(buf, delEnt->count);
   datQ  =  &buf->datQ[hashKey];

   /* delete the entry */
   cmLListDelFrm(datQ, &delEnt->lstEnt);

   /* NULLing off to prevent double dealloc */
   if(delEnt->pdu == delEnt->sdu)
   {
      delEnt->pdu =  NULLP;
   }

   PJ_FREE_BUF(delEnt->pdu);
   PJ_FREE_BUF(delEnt->sdu);
   PJ_FREE(delEnt, sizeof(PjTxEnt));
   buf->numEntries --;

   RETVALUE(ROK);
} /* pjDbmDelTxEnt */


/**
 *
 * @brief Handler to Delete all the elements in the hashlist
 *       
 *
 * @b Description
 *        This function is invoked to Delete all the elements in the
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmTxDelAll
(
PjBuf *buf        /* !< Tx Buffer  */                     
)
#else
PUBLIC S16 pjDbmTxDelAll(buf)
PjBuf *buf;       /* !< Tx Buffer  */                     
#endif
{
   U8         i;         /* index counter */
   CmLListCp *datQ;      /* CmLListcp Ptr */
   CmLList   *lstEnt;   /* CmLList Ptr - Loop var */
   PjTxEnt   *txEnt;     /* Tx Entry Ptr - Loop var */

   TRC3(pjDbmTxDelAll);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmTxDelAll(buf)\n"));

   /* cleanup the entries */
   for(i = 0; i< buf->numBins; i++)
   {
      datQ = &buf->datQ[i];
      while(cmLListFirst(datQ))
      {
         lstEnt = cmLListDelFrm(datQ, cmLListCrnt(datQ));
         if(lstEnt != NULLP)
         {
            txEnt =  (PjTxEnt *)cmLListNode(lstEnt);
            if ( txEnt->sdu == txEnt->pdu )
            {
               txEnt->sdu = NULLP;
            }
            PJ_FREE_BUF(txEnt->sdu);
            PJ_FREE_BUF(txEnt->pdu);
            PJ_FREE(txEnt, sizeof(PjTxEnt));
         }
      }
      cmLListInit(datQ);
   }

   RETVALUE(ROK);
} /* pjDbmTxDelAll */

/**
 *
 * @brief Handler to De initialize hash list
 *       
 *
 * @b Description
 *        This function is invoked by LMM to De initialize the 
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmTxDeInit
(
PjBuf *buf        /* !< Tx Buffer  */                     
)
#else
PUBLIC S16 pjDbmTxDeInit(buf)
PjBuf *buf;       /* !< Tx Buffer  */                     
#endif
{

   TRC3(pjDbmTxDeInit);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmTxDeInit(buf)\n"));

   /* cleanup the entries */
   pjDbmTxDelAll(buf);
   
   /* Cleanup the listCps */
   PJ_FREE(buf->datQ, (sizeof(CmLListCp) * buf->numBins));
   buf->numBins   =  NULLP;

   RETVALUE(ROK);
} /* pjDbmDeInit */

/*****************************************************************************
 *                  HANDLERS FOR THE RECEPTION BUFFER
 ****************************************************************************/

/**
 *
 * @brief Handler to insert into a RX Buffer
 *
 * @b Description
 *        This function is invoked by to insert into
 *         the reception buffer hash List
 *
 *  @param[in]  buf      Rx Buffer 
 *  @param[in]  rxEnt    Reception Entry
 *  @param[in]  dupFlag  Search for duplicates(Should be TRUE for AM)
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmInsRxEnt
(
PjBuf       *buf,          /* !< Rx Buffer  */
PjRxEnt     *rxEnt,        /* !< Rx Entry   */              
Bool        dupFlag        /* !< Boolean to check for duplicate  entries */
)
#else
PUBLIC S16 pjDbmInsRxEnt(buf, rxEnt, dupFlag)
PjBuf       *buf;          /* !< Rx Buffer  */
PjRxEnt     *rxEnt;        /* !< Rx Entry   */                              
Bool        dupFlag;       /* !< Boolean to check for duplicate  entries */ 
#endif
{
   U8       hashKey;          /* Computed hash key */
   PjRxEnt  *tmpEnt = NULLP;  /* Rx Entry Ptr */

   TRC3(pjDbmInsRxEnt)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmInsRxEnt buf, rxEnt(%ld), dupFlag(%d)\n",\
            rxEnt->count, dupFlag));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmInsRxEnt buf, rxEnt(%d), dupFlag(%d)\n",\
            rxEnt->count, dupFlag));
#endif

   if(dupFlag)
   {
      /* Search for duplicates */
      tmpEnt = pjDbmGetRxEnt(buf, rxEnt->count);
      if(tmpEnt != NULLP)
      {
         RETVALUE(ROKDUP);
      }
   }

   /* Compute the hash index */
   hashKey = PJ_HASH_FN(buf, rxEnt->count);

   rxEnt->lstEnt.node   =  (PTR) rxEnt;
   cmLListAdd2Tail(&buf->datQ[hashKey], &rxEnt->lstEnt);
   buf->numEntries ++;

   RETVALUE(ROK);
} /* pjDbmRxBufIns */


/**
 *
 * @brief Handler to fetch from a RX Buffer
 *
 * @b Description
 *        This function is invoked to fetch an entry from
 *         the transaction buffer hash List
 *
 *  @param[in]    buf         Rx Buffer 
 *  @param[in]    count       Count value of the entry to retrieve
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC PjRxEnt* pjDbmGetRxEnt
(
PjBuf  *buf,       /* !< Rx Buffer  */                               
U32    count       /* !< Count value of the entry to retrieve */                               
)
#else
PUBLIC PjRxEnt* pjDbmGetRxEnt(buf, count)
PjBuf  *buf;       /* !< Rx Buffer  */                               
U32    count;      /* !< Count value of the entry to retrieve */   
#endif
{
   U8         hashKey;   /* Computed hashKey */
   CmLListCp *datQ;      /* Ptr to the CmLListCp */
   PjRxEnt   *tmpEnt = NULLP;  /* Loop var - Rx Entr Ptr */

   TRC3(pjDbmGetRxEnt)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmGetRxEnt(buf, count(%ld)\n", count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmGetRxEnt(buf, count(%d)\n", count));
#endif

   /* Compute the hask key*/
   hashKey = PJ_HASH_FN(buf, count);

   datQ = &buf->datQ[hashKey];

   /* Loop through and search */
   cmLListFirst(datQ);
   while(cmLListCrnt(datQ))
   {
      tmpEnt = (PjRxEnt *)cmLListNode(cmLListCrnt(datQ));
      if((tmpEnt->count) == count)
      {
         RETVALUE(tmpEnt);
      }
      cmLListNext(datQ);
   }
   RETVALUE(NULLP);
} /* pjDbmGetRxEnt */


/**
 *
 * @brief Handler to delete from a RX Buffer
 *
 * @b Description
 *        This function is invoked by to delete from
 *         the transaction buffer hash List
 *
 *  @param[in]    buf         Rx Buffer Control Point
 *  @param[in]    count       Count value of the Rx Entry to delete
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelRxEnt
(
PjBuf  *buf,        /* !< Rx Buffer  */                                                          
U32    count        /* !< Count value of the entry to retrieve */                                
)
#else
PUBLIC S16 pjDbmDelRxEnt(buf, count)
PjBuf  *buf;        /* !< Rx Buffer  */                                                          
U32    count;       /* !< Count value of the entry to retrieve */                                
#endif
{
   U8          hashKey;      /* Computed hash value */
   CmLListCp   *datQ;        /* Pointer to the the ListCp */
   PjRxEnt     *delEnt;      /* Loop var - entry to be deleted */

   TRC3(pjDbmGetRxEnt)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmDelRxEnt(buf, count(%ld)\n", count));
#else
   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmDelRxEnt(buf, count(%d)\n", count));
#endif

   /* Get the node */
   delEnt   =  pjDbmGetRxEnt(buf, count);

   if(delEnt == NULLP)
   {
      RETVALUE(ROK);
   }

   hashKey = PJ_HASH_FN(buf, count);
   datQ = &buf->datQ[hashKey];

   /* delete the entity and free it*/
   cmLListDelFrm(datQ, &delEnt->lstEnt);
   PJ_FREE_BUF(delEnt->mBuf);
   PJ_FREE(delEnt, sizeof(PjRxEnt));
   buf->numEntries --;

   RETVALUE(ROK);
} /* pjDbmDelRxEnt */


/**
 *
 * @brief Handler to Delete all the elements in the hashlist
 *       
 *
 * @b Description
 *        This function is invoked by delete all the elements in th3
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmRxDelAll
(
PjBuf *buf        /* !< Rx Buffer  */                                                          
)                 
#else
PUBLIC S16 pjDbmRxDelAll(buf)
PjBuf *buf;       /* !< Rx Buffer  */                                                          
#endif
{
   U8         listInd;   /* loop index */
   CmLListCp *datQ;      /* CmLListCp Ptr */
   CmLList   *lstEnt;   /* loop ptr to CmLlistEnt */
   PjRxEnt   *delEnt;    /* loop ptr to entry to be deleted */

   TRC3(pjDbmRxDelAll);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmRxDelAll(buf)\n"));

   for(listInd = 0; listInd< buf->numBins; listInd++)
   {
      datQ = &buf->datQ[listInd];
      while(cmLListFirst(datQ))
      {
         lstEnt = cmLListDelFrm(datQ, cmLListCrnt(datQ));
         if(lstEnt != NULLP)
         {
            delEnt   =  (PjRxEnt *)cmLListNode(lstEnt);
            PJ_FREE_BUF(delEnt->mBuf);
            PJ_FREE(delEnt, sizeof(PjRxEnt));
         }
      }
      cmLListInit(datQ);
   }

   RETVALUE(ROK);
} /* pjDbmRxDelAll */

/**
 *
 * @brief Handler to De initialize hash list
 *       
 *
 * @b Description
 *        This function is invoked by LMM to De initialize the 
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmRxDeInit
(
PjBuf *buf        /* !< Rx Buffer  */                                                          
)                 
#else
PUBLIC S16 pjDbmRxDeInit(buf)
PjBuf *buf;       /* !< Rx Buffer  */                                                          
#endif
{

   TRC3(pjDbmRxDeInit);

   PJDBGP((KW_PJ_DBGMASK_UTL | KW_DBGMASK_DETAIL), (_kwPBuf,\
            "pjDbmRxDeInit(buf)\n"));

   /* Cleanup all entries */
   pjDbmRxDelAll(buf);

   /* Cleanup hashlist */
   PJ_FREE(buf->datQ, (sizeof(CmLListCp) * buf->numBins));
   buf->numBins   =  NULLP;

   RETVALUE(ROK);
} /* pjDbmDeInit */


/********************************************************************30**
  
         End of file:     gp_pj_dbm.c@@/main/1 - Fri Nov 13 14:13:44 2009
  
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
------------ -------- ---- ----------------------------------------------
/main/1      ---      mn   1. LTERLC Release 2.1
*********************************************************************91*/
