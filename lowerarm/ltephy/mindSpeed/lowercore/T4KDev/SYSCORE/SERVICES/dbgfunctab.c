#include <string.h>
#include <signal.h>
#include "config.h"
#include "4gmx.h"
#include "appprintf.h"


#include "dbgfunctab.h"

unsigned int rootinfo_count_bits (unsigned int x)
{
 int r;
 for (r=0; x!=0; x/=2)
 {
  if ((x%2)!=0)
  {
   ++r;
  }
 }
 return r;
}

RootInfo none_func = {"none", 0, 0, 0};

const RootInfo *rootinfo_found_func(UINT32 pc)
{
	UINT32 pos = ROOTINFO_TAB_SIZE /2;
	UINT32 left = ROOTINFO_TAB_SIZE /2; 
	UINT32 done = 0;
	UINT32 plusone = 0;


	while(!done)
	{
		if(rootinfo[pos].ObjAddr> pc)
		{
			//uart_printf("----- \">\" pos %d, pc %x, func addr %x\r\n", pos, pc, rootinfo[pos].ObjAddr);

			if (left == 1)
			{
				plusone++;
				pos--;
			}
			else
			{
				pos -= left / 2; 	
				left /= 2;
			}			
		}
		else if(rootinfo[pos].ObjAddr < pc)
		{
			//uart_printf("----- \"<\" pos %d, pc %x, func addr %x\r\n", pos, pc, rootinfo[pos].ObjAddr);
			if(rootinfo[pos].ObjAddr + rootinfo[pos].ObjSize> pc)			
			{
				//uart_printf("----- pc in func size (%d) pos %d, pc %x, func addr %x\r\n", rootinfo[pos].ObjSize, pos, pc, rootinfo[pos].ObjAddr);
				return &rootinfo[pos];
			}
			else if(rootinfo[pos + 1].ObjAddr > pc)
			{
				//uart_printf("----- pc < next func addr %x, pos %d, pc %x, func addr %x\r\n", rootinfo[pos + 1].ObjAddr, pos, pc, rootinfo[pos].ObjAddr);
				return &rootinfo[pos];
			}

			
			if (left == 1)
			{
				plusone++;
				pos++;
			}
			else
			{
				pos += left / 2;
				left /= 2;			
			}
		}
		else
			return &rootinfo[pos];
		
		if(plusone > 3)
		{
			//uart_printf("not found\r\n");
			return &none_func;
		}
	}

	return &none_func;
}


PRootInfo DbgGetObjInfo (UINT32 addr)
{
    UINT32 i;

    for (i = 0; i < ROOTINFO_TAB_SIZE; i++)
    {
        if (rootinfo[i].ObjSize == 0 || rootinfo[i].ObjName[0]==0)
            break;

        if (rootinfo[i].ObjName[0] == '_')
            continue;

        if (rootinfo[i].ObjAddr <= addr && addr < rootinfo[i].ObjAddr + rootinfo[i].ObjSize)
            return (PRootInfo)&rootinfo[i];
    }

    return NULL;
}

void rootinfo_print_stack(PVUINT32 pc, PVUINT32 sp, PVUINT32 lr, char *text)	
{
	UINT32 cb, sb;
	const RootInfo *pfunc_el;

	pfunc_el = rootinfo_found_func((UINT32)pc);
	if(text)
		uart_printf("\r\n%s in %s (0x%X)\r\n", text, pfunc_el->ObjName, (UINT32)pc);

	uart_printf("\r\nCall stack is: \r\n");


	if(lr)
		if(*(PVUINT32)(pfunc_el->ObjAddr) >> 16 != ARM_INSTR_PUSH)
		{
			pc = --lr;
			uart_printf(" -- %s (0x%X)\r\n", rootinfo_found_func((UINT32)pc)->ObjName, (UINT32)pc);
		}

	while(pc >= (UINT32 *)0x10000001 && pc < (UINT32 *)0x100F0000)
	{
		switch(*pc >> 16)
		{
				case ARM_INSTR_PUSH:
			{				
				cb = rootinfo_count_bits(*pc & 0xFFFF);				
				pc = (UINT32 *)*(sp + (cb - 1));			
				sp += cb;
				uart_printf("-- %s (0x%X)\r\n", rootinfo_found_func((UINT32)pc)->ObjName, (UINT32)pc);
				continue;
			}
			case ARM_INSTR_SUBSP:
			{
				UINT32 my_bit;
				UINT16 org = ((*pc) & 0xFF);
				UINT32 rotate = (((*pc) & 0xF00) >> 8) * 2;

				sb = org;

				while (rotate != 0)
				{
					my_bit = (sb & 1);

					sb>>= 1;
					sb |= (my_bit << 31);

					rotate --;
				}
				
				*((PVUINT8 *)&sp) += sb;

			}	
			default:
				pc--;
		}
	}

	uart_printf("\r\n");
}
