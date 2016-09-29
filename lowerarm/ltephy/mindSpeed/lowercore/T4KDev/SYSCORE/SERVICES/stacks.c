//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL stacks.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"

__align (32) STACKSHEAP stacks_cpu0;

__align (32) STACKSHEAP stacks_cpu1;

__align (32) STACKSHEAP stacks_cpu2;

__align (32) STACKSHEAP stacks_cpu3;
