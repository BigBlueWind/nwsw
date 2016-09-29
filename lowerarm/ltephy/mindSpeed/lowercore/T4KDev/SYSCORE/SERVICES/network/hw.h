

#ifndef _NET_HW_H_
#define _NET_HW_H_

#include "net_types.h"
#include "config.h"
#include "4gmx.h"
#include <string.h>
#include "appids.h"
#include "hal.h"

MXRC net_hw_init(void);
MXRC net_hw_send(byte *hframe, size_t hsize, const byte *pframe, const size_t psize);

#endif

