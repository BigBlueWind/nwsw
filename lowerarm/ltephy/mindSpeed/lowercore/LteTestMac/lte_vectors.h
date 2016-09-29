#ifndef LTE_VECTORS_H
#define LTE_VECTORS_H

RESULTCODE lte_vectors_processing_ul(PULSUBFRDESC pSubFrame);

RESULTCODE lte_vectors_processing_dl(PDLSUBFRDESC pSubFrame);
RESULTCODE lte_vectors_processing_dl_ccode(PDLSUBFRDESC pSubFrame);
RESULTCODE lte_vectors_processing_dl_rt(PDLSUBFRDESC pSubFrame);

#endif
