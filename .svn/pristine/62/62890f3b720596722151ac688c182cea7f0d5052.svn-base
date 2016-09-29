//-------------------------------------------------------------------------------------------
/** @file gputil_fx.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.22 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _GPUTIL_FX_H
#define _GPUTIL_FX_H

void ini_vector_fx(S16 v[], S16 nl, S16 nh, S16 val);
void ini_ivector_fx(S32 v[], S16 nl, S16 nh, S32 val);
void mul_vector_fx(S16 v1[], S16 v2[], S16 v[], S16 nl, S16 nh);

short *svector(long nl, long nh);
short *svectoru(long nl, long nh);

void free_svector(short *v, long nl, long nh);

S32 *lvector(S32 nl, S32 nh);
S32 *lvectoru(S32 nl, S32 nh);

void free_lvector(S32 *v, S32 nl, S32 nh);

U16 *usvector(long nl, long nh);
U16 *usvectoru(long nl, long nh);
void free_usvector(U16 *v, long nl, long nh);

void nrerror(char error_text[]);

S16 block_norm(S16 * data_in, S16 *data_out, S32 size, S16 headroom);
S16 block_lnorm(S32 * data, S32 size, S16 headroom);

short Sqrt_lbc(long Num);

S16 **smatrix(S32, S32, S32, S32);
S16 **smatrix0(S32 nrh, S32 nch);

S16 **smatrixu(S32, S32, S32, S32);
S16 **smatrixu0(S32 nrh, S32 nch);

void free_smatrix(S16 **, S32, S32, S32, S32);
void free_smatrix0(S16 **m);

U8 *u8vector(long nl, long nh);
U8 *u8vectoru(long nl, long nh);

void free_u8vector(U8 *v, long nl, long nh);

U8 **u8matrix(S32, S32, S32, S32);
U8 **u8matrixu(S32, S32, S32, S32);

void free_u8matrix(U8 **, S32, S32, S32, S32);

#endif /* _GPUTIL_FX_H */
