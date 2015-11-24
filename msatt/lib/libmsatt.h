/**
 * @file libmsatt.h
 *
 * @brief MSATT driver library interface
 *
 * This file describes the external interface to the MSATT
 * driver and provides the definitios for proper communication
 * with the device
 *
 * Copyright (c) 2012 CERN
 * @author Luis Fernando Ruiz Gago <lfruiz@cern.ch>
 * @date June 14th 2012
 *
 * @section license_sec License
 * Released under the GPL v2. (and only v2, not any later version)
 */



#ifndef __LIBMSATT_H
#define __LIBMSATT_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @enum msatt_value_enum
 * @brief Valid relay values
 *
 */
enum msatt_value_enum {
	ATT_ERR	=	-1,	/**< Error */
	ATT_NONE = 	0x0,	/**< No attenuation */
	ATT_1 = 	0x01,	/**< Attenuator 1: 20dB */
	ATT_2 = 	0x03,	/**< Attenuator 1+2: 40dB */
};

/**
 * @brief Open a msatt device
 *
 * A board is selected by lun, and a handle is returned for further
 * reference to it.
 *
 * @param lun - LUN of msatt card
 *
 * @return >0 - on success, device file descriptor number
 */
int msatt_open(int lun);

/**
 * @brief close a channel handle
 *
 * @param fd  Handle to close
 */
int msatt_close(int fd);

/**
 * @brief Get attenuation value for a given channel
 * 
 * Each channel can be on one of these states: 
 * 		- No attenuation active (Relay value = 0x0)
 *		- 20dB attenuation 	(Relay value = 0x1)
 * 		- 40dB attenuation	(Relay value = 0x3)
 *
 * @param fd 		Device handle identifying board
 * @param channel 	Channel [1,2,3,4] on the board to read from
 * 
 * @return ATT_ERR on failure, another msatt_value_enum value on success
 */
enum msatt_value_enum msatt_get_relay(int fd, int channel);

/**
 * @brief Set attenuation value for a given channel
 * 
 * @param fd		Device handle identifying board
 * @param channel 	Channel on the board [1,2,3,4] 
 * @param val 		Valid msatt_value_enum relay value
 * 
 * @return Upon successful completion, returns a value other than -1. On failure it returns -1
 */
int msatt_set_relay(int fd, int channel, enum msatt_value_enum val);

/**
 * @brief Gets the number of channels (2 or 4)
 * 
 * @param fd		Device handle identifying board
 * 
 * @return -1 on fail, 2 for MSATTN2 and 4 for MSATTN4
 */
int msatt_get_nchannels(int fd);

/* libmsatt version string */
extern const char * const libmsatt_version_s;
#ifdef __cplusplus
}
#endif


#endif /* __LIBMSATT_H */
