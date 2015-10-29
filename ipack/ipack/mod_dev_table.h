/*
 * Industry-pack bus.
 *
 * Copyright (C) 2011-2012 CERN (www.cern.ch)
 * Author: Samuel Iglesias Gonsalvez <siglesias@igalia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; version 2 of the License.
 */

#ifndef _MOD_DEV_TABLE_H_
#define _MOD_DEV_TABLE_H_

/* Code below resides in the Kernel's source under:
 * include/linux/mod_devicetable.h
 * from somewhere around 3.7 */
#define IPACK_ANY_FORMAT 0xff
#define IPACK_ANY_ID (~0)
struct ipack_device_id {
        __u8  format;                   /* Format version or IPACK_ANY_ID */
        __u32 vendor;                   /* Vendor ID or IPACK_ANY_ID */
        __u32 device;                   /* Device ID or IPACK_ANY_ID */
};

#endif /* _MOD_DEV_TABLE_H_ */
