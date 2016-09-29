//-------------------------------------------------------------------------------------------
/** @file tx_mapper.h
 *
 * @brief TX mapper
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef MAPPER_H
#define MAPPER_H

#define MOD_QPSK  1
#define MOD_16QAM 2
#define MOD_64QAM 3

extern short MODTAB_BPSK_256[];
extern short MODTAB_QPSK_256[];
extern short MODTAB_16QAM_256[];
extern short MODTAB_64QAM_64[];

void BPSK_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N);
void QPSK_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1);
void QAM16_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N, short N1);
void QAM64_TX_MAPPER(unsigned char *IN, short *OUT, short *pTABLE, short N);

void BPSK_TX_MAPPER_A(void*, void*, void*, long);
void QPSK_TX_MAPPER_A(void*, void*, void*, long, long);
void QAM16_TX_MAPPER_A(void*, void*, void*, long, long);
void QAM64_TX_MAPPER_A(void*, void*, void*, long);

//unsigned long tx_Mapper(unsigned char ctxdata[], unsigned long len,
//                        unsigned short modType, short MappingGainOut[]);
//unsigned long tx_Mapper_tcb(unsigned char ctxdata[], unsigned long len,
//                            unsigned short modType, short MappingGainOut[],
//                            short Qm, unsigned long length_b1);
void tx_Mapper(unsigned char ctxdata[], unsigned long len, short len1, 
                        unsigned short modType, short MappingGainOut[]);
void tx_Mapper_tcb(unsigned char ctxdata[], unsigned long len, short len1,
                            unsigned short modType, short MappingGainOut[]);

#endif /* MAPPER_H */
