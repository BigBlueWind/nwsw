/**
 * @file    ueh_eut.x
 * @brief This file contains EUT Internal Intf. Data structures
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#ifndef  UEH_EUT_X_H
#define  UEH_EUT_X_H


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _euEGtpTunInf
{
   U32 locTeid;
   U32 remTeid;
   CmLtePdcpId pdcpId;
   CmTptAddr dstIpAddr;
   CmTptAddr frmDstIpAddr;
} EuEGtpTunInf;

typedef EuEGtpTunInf EgtpTunInf;


typedef S16 (*EgtpTunCreat)       ARGS((
        Pst *pst,
        SuId suId,
        EgtpTunInf *tunelInf 
        ));

typedef S16 (*EgtpTunCreatCfm)       ARGS((
        Pst *pst,
        SuId suId,
        CmLteEgtpTunCfm *egtpTunCfm
        ));


typedef S16 (*EgtpTunDel)      ARGS((
        Pst *pst,
        SpId spId,
        U32 locTeid
        ));
typedef S16 (*S1APRelReq) ARGS((
         Pst *pst,
         U32 ueId
        ));
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UEH_EUT_X_H */
