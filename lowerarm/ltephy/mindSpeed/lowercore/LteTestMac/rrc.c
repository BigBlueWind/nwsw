// rrc.c
//

#include "rrc.h"
/*
OSAL_SIZE RrcSendData(U16 logicChanID, PTR dest, OSAL_SIZE *expSize)
{
    int i;
    U8 *ptr = (U8*) dest;

    if (*expSize >= sizeof(U16))
    {
        *ptr++ = (U8) (logicChanID >> 8);
        *ptr++ = (U8) logicChanID & 0xFF;
        for (i = 2; i < (int) *expSize; i++)
        {
            *ptr++ = (U8) i;
        }
    }
    else
    {
        return 0;
    }

    return *expSize;
}

OSAL_SIZE RrcRecvData(PTR dest, OSAL_SIZE *expSize)
{
    return 0;
}
*/
