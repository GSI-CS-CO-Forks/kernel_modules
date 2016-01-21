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

#ifdef ppc4
#include "libvmod12a22dioaio.h"
#include "libvmod12e162dioaio.h"
#include "libvmod16a22dioaio.h"
#include "libvmoddor2dioaio.h"
#include "libvmodttl2dioaio.h"
#else
#include "libvmod12a2.h"
#include "libvmod12a4.h"
#include "libvmod12e16.h"
#include "libvmod16a2.h"
#include "libvmoddor.h"
#include "libvmodttl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//! libvmodttl version string
extern const char * const libvmod_version_s;

#ifdef __cplusplus
}
#endif

#endif /* __LIBVMOD */
