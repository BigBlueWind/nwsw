/**
 * @file    ueh_crm.h
 * @brief This file contains the function declarations for handling cell level rrm functionalities
 *		    such as call admission control
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 25 Oct 2010 :
 *    a) Created initial version
 */

#ifndef UEH_CRM_H
#define UEH_CRM_H

#include "envdep.h"
#include "ueh.h"

UehUeCb *AdmissionRequest(
    U32     establishmentCause,
    U16     crnti
    );
PUBLIC void CancelUe (
    UehUeCb* ueCb
    );
PUBLIC S16 initContxtAdmissionRequest(
    UehUeCb           *ueCb,
    U8             *causeType,
    U8             *causeVal
    );

PUBLIC void CancelUe ARGS ((UehUeCb* ueCb));
PUBLIC S32 allocateTeid( void );
PUBLIC void allocInitTeidPool( void );
PUBLIC void freeTeid( S32 teid);
PUBLIC void deallocTeidPool(void);
PUBLIC S16 uehInitSysInfSch(void);

#endif /* UEH_CRM_H */
