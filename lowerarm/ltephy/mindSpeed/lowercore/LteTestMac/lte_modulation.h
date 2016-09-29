#ifndef _LTE_MODULATION
#define _LTE_MODULATION

U8 lte_modulation_set_mcs_index_ul(U16 modulation, U8 value);
U8 lte_modulation_get_mcs_index_ul(U16 modulation);


U8 lte_modulation_set_mcs_index_dl(U16 modulation, U16 cwId, U8 value);
U8 lte_modulation_get_mcs_index_dl(U16 modulation, U16 cwId);




#endif //_LTE_MODULATION

