/********************************************************************16**

                         (c) COPYRIGHT 2011 by 
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

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/**********************************************************************
     Name:    LTE MAC SC1 scheduler 

     Type:    C header file

     Desc:    Defines required by SC1 scheduler

     File:    rg_sch_sc1.h

     Sid:      rg_sch_sc1.h@@/main/2 - Sat Jul 30 02:22:11 2011

     Prg:      

*********************************************************************21*/


#ifndef __RGSCHSC1H__
#define __RGSCHSC1H__ 
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/

/* Scheduler1 tunable params */

#define RG_GET_SC1_CELL_UL(cell) \
             ((RgSchSc1UlCell *)((RgSchCmnCell*)((cell)->sc.sch))->ul.schSpfc)
#define RG_GET_SC1_CELL_DL(cell) \
             ((RgSchSc1DlCell *)((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc)

#define RG_GET_SC1_UE_DL(ue) \
             ((RgSchSc1DlUe *)((RgSchCmnUe *)((ue)->sch))->dl.schSpfc)
#define RG_GET_SC1_UE_UL(ue) \
             ((RgSchSc1UlUe *)((RgSchCmnUe *)((ue)->sch))->ul.schSpfc)

#define RG_GET_SC1_SVC_DL(dlLc) \
             ((RgSchSc1DlSvc *)((RgSchCmnDlSvc *)((dlLc)->sch))->schSpfc)
#define RG_GET_SC1_HQP_DL(hqP) \
             ((RgSchSc1DlHqProc *)((RgSchCmnDlHqProc *)((hqP)->sch))->schSpfc)

#define RG_SC1_SVC_HAS_DATA(svc) \
  (((RgSchSc1DlSvc *)(((RgSchCmnDlSvc *)((svc)->sch))->schSpfc))->effMbr != 0)

#define RG_SC1_ISGBRPRIO(qciPrio) ((qciPrio) >=2 && (qciPrio) <= 5)

/* SC1 configuration error types */ 
#define RGSCHERR_SCH_SC1_DL_CFG (RGSCHERR_SCH_CFG + 10)
#define RGSCHERR_SCH_SC1_UL_CFG (RGSCHERR_SCH_CFG + 11)

/* Scheduler1 fixed params */
#define RG_SC1_UL_INVALID_QID      255
#define RG_SC1_QCIPRIOVAL_MAX      9
#define RG_SC1_QCIPRIOVAL_MIN      1
#define RG_SC1_UL_CONT_RES_QID     0
#define RG_SC1_MAX_DL_AMBR         0xffffffff
#define RG_SC1_UL_NUM_Q            (RG_SCH_CMN_NUM_QCI + 1)
#define RG_SC1_DL_NUM_Q            (RG_SCH_CMN_MAX_PRIO + 1) 
#ifdef RG_UNUSED
#define RG_SC1_UL_INVALID_BETA8    0xffff
#endif

#ifdef __cplusplus
}
#endif
#endif /* __RGSCHSC1H__ */

/********************************************************************30**

         End of file:     rg_sch_sc1.h@@/main/2 - Sat Jul 30 02:22:11 2011

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
/main/1      ---       sm  1. LTE MAC 2.1 release
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
