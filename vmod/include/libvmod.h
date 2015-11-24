/**
 * @file libvmod.h
 *
 * @brief VMOD driver library interface
 *
 * This file describes the external interface to the VMOD.
 * So far it only exports library version.
 *
 * Copyright (c) 2015 CERN
 * @author Adam Wujek <adam.wujek@cern.ch>
 * @date Wed Nov 25 2015, 16:40:16 CET
 *
 * @section license_sec License
 * Released under the GPL v2. (and only v2, not any later version)
 */

#ifndef __LIBVMOD
#define __LIBVMOD

#ifdef __cplusplus
extern "C" {
#endif

//! libvmodttl version string
extern const char * const libvmod_version_s;

#ifdef __cplusplus
}
#endif

#endif /* __LIBVMOD */
