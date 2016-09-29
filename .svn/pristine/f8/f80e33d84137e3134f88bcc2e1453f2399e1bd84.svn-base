//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL tc1700.h
 *
 * @brief Declarations required for Turbo Concept's encoders and decoders.
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------


/*******************************************************************************
 *  FILE               :  tc1700.h                                              *
 *                                                                              *
 *  Organization       :  TurboConcept                                          *
 *  Project            :  TC1700	WiMAX-LTE decoder											         *
 *                                                                              *
 *  Creation Date      :  Nov 25th, 2008                                        *
 *                                                                              *
 *  Description        :  TC1700	WiMAX-LTE decoder										           *
 ********************************************************************************/

#ifndef TC1700_H
#define TC1700_H

static char const tc1700_h[] = "$Id: tc1700.h,v 1.9 2012/09/17 12:21:58 melnico Exp $";

#ifndef TC1700_DLL_API
#ifdef TC1700_DLL_EXPORT
#define TC1700_DLL_API __declspec(dllexport)
#elif TC1700_DLL_IMPORT
#define TC1700_DLL_API __declspec(dllimport)
#else
#define TC1700_DLL_API
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef BIT_t_DEFINED
TC1700_DLL_API typedef unsigned char BIT_t;
#define BIT_t_DEFINED
#endif

#ifndef DEC_IN_t_DEFINED

#ifdef FLOATING_POINT
TC1700_DLL_API typedef double DEC_IN_t;
#else
TC1700_DLL_API typedef signed short DEC_IN_t;
#endif
#define DEC_IN_t_DEFINED
#endif

/******************************************************************************
 * tc1700decode
 ******************************************************************************/
TC1700_DLL_API int tc1700decode(
/* Core generics */
const int speed, const int d_width, const int wimax_use_rate13,
/* Core configuration */
const int mode, const int k, const int nb_bits_encoded, const int crc_select, const int rate_index,
                                const int modulation_index, const int halfit,
                                const int stop_criterion, const int threshold,
                                /* Data in/out */
                                DEC_IN_t *dec_in, BIT_t *dec_out, BIT_t *genie_enc_in,
                                int *genie_neb, int *ber, int *halfit_done, int *crc_error,
                                int debug_output);

#ifdef __cplusplus
}
#endif

#endif
