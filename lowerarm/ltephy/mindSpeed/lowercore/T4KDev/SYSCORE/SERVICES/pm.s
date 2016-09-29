;/*****************************************************************************
;*                                                                            *
;*       Copyright 2010 by Mindspeed Technologies, Inc.                       *
;*       All Rights Reserved                                                  *
;*                                                                            *
;*       Mindspeed Technologies, Inc.                                         *
;*       4000 MACARTHUR BLVD - EAST TOWER                                     *
;*       NEWPORT BEACH, CA 92660                                              *
;*                                                                            *
;******************************************************************************
;*       pm.s                                                                 *
;*                                                                            *
;*       Performance monitor                                                  *
;*                                                                            *
;*****************************************************************************/


    PRESERVE8

	AREA ARMA9CORE, CODE, READONLY


    EXPORT  pm_get_PMCR
    EXPORT  pm_set_PMCR
    EXPORT  pm_get_PMCNTENSET
    EXPORT  pm_set_PMCNTENSET
    EXPORT  pm_get_PMCNTENCLR
    EXPORT  pm_set_PMCNTENCLR
    EXPORT  pm_get_PMSELR
    EXPORT  pm_set_PMSELR
    EXPORT  pm_get_PMCCNTR
    EXPORT  pm_set_PMCCNTR       
    EXPORT  pm_get_PMXEVTYPER
    EXPORT  pm_set_PMXEVTYPER    
    EXPORT  pm_get_PMXEVCNTR
    EXPORT  pm_set_PMXEVCNTR
    EXPORT  pm_get_PMUSERENR
    EXPORT  pm_set_PMUSERENR
    EXPORT  pm_get_PMOVSR
    EXPORT  pm_set_PMOVSR
    EXPORT  pm_get_PMINTENCLR
    EXPORT  pm_set_PMINTENCLR
    
pm_get_PMCR
    MRC p15,0,r0,c9,c12,0
    MOV pc, lr

pm_set_PMCR
    MCR p15,0,r0,c9,c12,0
    MOV pc, lr

pm_get_PMCNTENSET
    MRC p15,0,r0,c9,c12,1
    MOV pc, lr

pm_set_PMCNTENSET
    MCR p15,0,r0,c9,c12,1
    MOV pc, lr

pm_get_PMCNTENCLR
    MRC p15,0,r0,c9,c12,2
    MOV pc, lr

pm_set_PMCNTENCLR
    MCR p15,0,r0,c9,c12,2
    MOV pc, lr

pm_get_PMSELR
    MRC p15,0,r0,c9,c12,5
    MOV pc, lr

pm_set_PMSELR
    MCR p15,0,r0,c9,c12,5
    MOV pc, lr

pm_get_PMCCNTR
    MRC p15,0,r0,c9,c13,0
    MOV pc, lr

pm_set_PMCCNTR
    MCR p15,0,r0,c9,c13,0
    MOV pc, lr

pm_get_PMXEVTYPER
    MRC p15,0,r0,c9,c13,1
    MOV pc, lr

pm_set_PMXEVTYPER
    MCR p15,0,r0,c9,c13,1
    MOV pc, lr

pm_get_PMXEVCNTR
    MRC p15,0,r0,c9,c13,2
    MOV pc, lr

pm_set_PMXEVCNTR
    MCR p15,0,r0,c9,c13,2
    MOV pc, lr
    
pm_get_PMUSERENR
    MRC p15,0,r0,c9,c14,0
    MOV pc, lr

pm_set_PMUSERENR
    MCR p15,0,r0,c9,c14,0
    MOV pc, lr

pm_get_PMOVSR
    MRC p15,0,r0,c9,c12,3
    MOV pc, lr

pm_set_PMOVSR
    MCR p15,0,r0,c9,c12,3
    MOV pc, lr

pm_get_PMINTENCLR
    MRC p15,0,r0,c9,c14,2
    MOV pc, lr

pm_set_PMINTENCLR
    MCR p15,0,r0,c9,c14,2
    MOV pc, lr
 
    END
