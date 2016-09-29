//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Queue.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef __QUEUE_H__
#define __QUEUE_H__


#define QPUT(type, head, elem, lock)    \
{                                       \
    IRQTYPE irq = ARM_INT_disable();    \
    MxGetLock(&lock);                   \
    if (head == NULL)                   \
    {                                   \
        head = elem;                    \
    }                                   \
    else                                \
    {                                   \
        type e = head;                  \
        for (; e->Next; e = e->Next)    \
            ;                           \
        e->Next = elem;                 \
    }                                   \
    MxReleaseLock(&lock);               \
    ARM_INT_restore(irq);               \
}                                       \

#define QGET(type, head, elem, lock)    \
{                                       \
    IRQTYPE irq = ARM_INT_disable();    \
    type e;                             \
    MxGetLock(&lock);                   \
    e = head;                           \
    if (e != NULL)                      \
    {                                   \
        head = e->Next;                 \
        e->Next = NULL;                 \
    }                                   \
    MxReleaseLock(&lock);               \
    ARM_INT_restore(irq);               \
    elem = (volatile type)e;            \
}                                       \


#endif //__QUEUE_H__

#ifdef __cplusplus
}
#endif

