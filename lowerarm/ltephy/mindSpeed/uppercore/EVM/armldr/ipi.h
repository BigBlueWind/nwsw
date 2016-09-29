/* ipi.h - Inter CPU driver ioctl requests
 *
 * Copyright (C) 2011 Mindspeed Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ICPU_H_
#define __ICPU_H_

#ifndef __KERNEL__
#include <sys/ioctl.h>
#endif

#define ICPU_GET_INDEX	_IOWR('I', 2, unsigned long)
#define ICPU_GET_BASE	_IOR('I', 3, unsigned long)
#define ICPU_LARM_CRASH _IOWR('I', 4, unsigned long)
#endif
