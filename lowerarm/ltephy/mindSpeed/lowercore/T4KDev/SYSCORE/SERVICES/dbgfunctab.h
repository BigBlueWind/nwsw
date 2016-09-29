#ifndef __DBG_FUNC_TAB__
#define __DBG_FUNC_TAB__

#include "rootinfo.h"

#define ARM_INSTR_PUSH 	0xE92D
#define ARM_INSTR_SUBSP 0xE24D


unsigned int rootinfo_count_bits (unsigned int x);
const RootInfo *rootinfo_found_func(UINT32 pc);
void rootinfo_print_stack(PVUINT32 pc, PVUINT32 sp, PVUINT32 lr, char *text);

#define DBG_ARMINST_OPCODE(x)  	((x >> 21) & 0xF)
#define DBG_ARMINST_COND(x)	  	((x >> 28) & 0xF)
#define DBG_ARMINST_IMM(x) 		((x >> 25 )& 0x1)
#define DBG_ARMINST_SC(x) 		((x >> 20) & 0x1)
#define DBG_ARMINST_RN(x)	 	((x >> 16) & 0xF)
#define DBG_ARMINST_RD(x)		((x >> 12) & 0xF)
#define DBG_ARMINST_OP2(x)		((x & 0xF)


#define DBG_ARMINST_AND  0000  // = AND - Rd:= Op1 AND Op2
#define DBG_ARMINST_SUB  0010  //= SUB - Rd:= Op1 - Op2
#define DBG_ARMINST_RSB  0011  //= RSB - Rd:= Op2 - Op1
#define DBG_ARMINST_ADD  0100  //= ADD - Rd:= Op1 + Op2
#define DBG_ARMINST_ADC  0101  //= ADC - Rd:= Op1 + Op2 + C
#define DBG_ARMINST_SBC  0110  //= SBC - Rd:= Op1 - Op2 + C
#define DBG_ARMINST_RSC  0111  //= RSC - Rd:= Op2 - Op1 + C
#define DBG_ARMINST_TST  1000  //= TST - set condition codes on Op1 AND Op2
#define DBG_ARMINST_TEQ  1001  //= TEQ - set condition codes on Op1 EOR Op2
#define DBG_ARMINST_CMP  1010  //= CMP - set condition codes on Op1 - Op2
#define DBG_ARMINST_CMN  1011  // = CMN - set condition codes on Op1 + Op2
#define DBG_ARMINST_ORR  1100  //= ORR - Rd:= Op1 OR Op2
#define DBG_ARMINST_MOV  1101  //= MOV - Rd:= Op2
#define DBG_ARMINST_BIC  1110  //= BIC - Rd:= Op1 AND NOT Op2
#define DBG_ARMINST_MVN  1111  //= MVN - Rd:= NOT Op2



PRootInfo DbgGetObjInfo (UINT32 addr);

#endif
