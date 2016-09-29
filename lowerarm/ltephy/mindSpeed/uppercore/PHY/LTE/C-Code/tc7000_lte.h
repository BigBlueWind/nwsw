//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL tc7000_lte.h
 *
 * @brief Declarations required for Turbo Concept's encoders and decoders.
 * @author Mindspeed Technologies
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------


/*******************************************************************************
*  FILE               :  tc7000_lte.h                                          *
*                                                                              *
*  Organization       :  TurboConcept                                          *
*  Project            :  TC7000	LTE											                         *
*                                                                              *
*  Creation Date      :  22/04/2007                                            *
*                                                                              *
*  Description        :  tc7000 LTE encoder / decoder										       *
********************************************************************************/

#ifndef TC7000_LTE_H
#define TC7000_LTE_H

static char const tc7000decode_h[]="$Id: tc7000_lte.h,v 1.12 2010/09/22 19:11:36 subrams2 Exp $";

#ifndef TC7000_LTE_DLL_API
  #ifdef TC7000_LTE_DLL_EXPORT
    #define TC7000_LTE_DLL_API __declspec(dllexport)
  #elif TC7000_LTE_DLL_IMPORT
    #define TC7000_LTE_DLL_API __declspec(dllimport)
  #else
    #define TC7000_LTE_DLL_API
  #endif
#endif

#ifdef __cplusplus
  extern "C" {
#endif

#ifndef BIT_t_DEFINED
    TC7000_LTE_DLL_API typedef unsigned char BIT_t;
  #define BIT_t_DEFINED
#endif


#ifndef DEC_IN_t_DEFINED

  #ifdef FLOATING_POINT
    TC7000_LTE_DLL_API typedef double DEC_IN_t;
  #else
    TC7000_LTE_DLL_API typedef signed short DEC_IN_t;
  #endif
  #define DEC_IN_t_DEFINED
#endif

/******************************************************************************
 * tc7000_lte_encode 													                                *
 ******************************************************************************/
TC7000_LTE_DLL_API int tc7000_lte_encode(
     const int fec_mode,
     const int nb_bits_syst,
     const int crc_select,
     const int nfiller_bits,
     const int scramble_en,
     const unsigned int scramble_mask,
     const int bypass_interleaving,
     const int bypass_enc,
     int *nb_bits_output,
     BIT_t *enc_in,
     BIT_t *enc_out,
     BIT_t *qfdb,
     BIT_t *enc_in_withcrc);

/******************************************************************************
 * tc7000_lte_decode 													                                *
 ******************************************************************************/
TC7000_LTE_DLL_API int tc7000_lte_decode(
                           const int nb_bits_syst,
                           const int crc_select,
                           const int bypass_deinterleaving,
                           const int nproc,
                           const int halfit_max,
                           const int d_width,
                           const int enable_logmap,
                           const int table_index,
                           int *custom_table,
                           const int stop_criterion,
                           const int threshold,
                           DEC_IN_t *dec_in,
                           BIT_t *dec_out,
                           BIT_t *genie_enc_in,
                           int *halfit_done,
                           int *neb,
                           int *crc_ok,
                           int *ber_value,
                           int debug_output);


#ifdef __cplusplus
}
#endif

#endif
