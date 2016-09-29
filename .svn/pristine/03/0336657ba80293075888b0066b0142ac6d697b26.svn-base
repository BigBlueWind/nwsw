#ifndef __CEVA_FUNCTIONS_H__
#define __CEVA_FUNCTIONS_H__

void DmaFromExtPram(void *p_ext_addr, void *p_int_addr, unsigned size);
void DmaFromExtDram(void *p_ext_addr, void *p_int_addr, unsigned size);
void DmaToExtDram(void *p_ext_addr, void *p_int_addr, unsigned size);
void MemCpy(void* dst, void* src, unsigned long size);
void MemCpyFromExt(void* dst, void* src, unsigned long size);
void MemCpyToExt(void* dst, void* src, unsigned long size);

#endif


void GetArmMsg(void);
void dwmemcpy(void *dst, void *src, unsigned size);


// TCB Extaction Functions
//------------------------

#define MAX_NUM_CEVA_TASKS		350    //max number of unique ceva tasks
typedef void functionCEVA(TCB *);
#define REGCEVA_OK           	0
#define REGCEVA_ERR		        1

//below keep here
unsigned RegCEVATask(U32 taskid, functionCEVA* fnptr);