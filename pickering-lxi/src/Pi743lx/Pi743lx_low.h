/*******************************************************************************
 *
 * (C) Copyright 1988 - 2011 Pickering Interfaces ltd.
 *  
 *  Author: Petr Sojnek
 *  Contact: petr.sojnek@pickering.cz
 *  Version: 1.00
 *  First release: 29.06.2010
 *  Questions: tomas.wiszczor@pickering.cz
 *
 * Notes:
 *  Header file for PI743LX library - low level functions
 */

#ifndef _PI743LX_H_LOW
#define _PI743LX_H_LOW

#include "Pi743lx.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* ************************************************************************* */
/**
@fn: ReadAdc
@brief: Read direct from current/voltage ADC.

@param: sid          - Handle of Current session.
@param: CardHandle   - card driver session
@param: select       - select voltage (0) or current (1)
@param: averages     - no. of averages
@param: data         - read data
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_ReadAdc(SESSION sid, DWORD CardHandle, DWORD select, DWORD averages, DWORD *data);

/* ************************************************************************* */
/**
@fn: ReadCurrentDac
@brief: Read direct from current limit DAC.

@param: CardHandle   - card driver session
@param: data         - read data
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_ReadCurrentDac(SESSION sid, DWORD CardHandle, DWORD *data);

/* ************************************************************************* */
/**
@fn: ReadEeprom
@brief: Read direct from onboard SPI Eeprom

@param: CardHandle   - card driver session
@param: address      - eeprom address
@param: data         - read data (16 bits)
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_ReadEeprom(SESSION sid, DWORD CardHandle, DWORD address, DWORD *data);

/* ************************************************************************* */
/**
@fn: ReadOutputDac
@brief: Read direct from output voltage DAC.

@param: CardHandle   - card driver session
@param: data         - read data
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_ReadOutputDac(SESSION sid, DWORD CardHandle, DWORD *data);

/* ************************************************************************* */
/**
@fn: ReadRegister
@brief: Read module register. Exported function, meaning parameter checking.

@param: CardHandle   - card driver session
@param: offset       - local module offset address
@param: data         - read data
*/
/* ************************************************************************* */
DWORD PI743LX_API PI743LX_ReadRegister(SESSION sid, DWORD CardHandle, DWORD offset, DWORD *data);

/* ************************************************************************* */
/**
@fn: WriteCurrentDac
@brief: Write direct to current limit DAC.

@param: CardHandle   - card driver session
@param: data         - write data
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_WriteCurrentDac(SESSION sid, DWORD CardHandle, DWORD data);

/* ************************************************************************* */
/**
@fn: WriteEeprom
@brief: Write direct to onboard SPI Eeprom

@param: CardHandle   - card driver session
@param: address      - eeprom address
@param: data         - write data (16 bits)
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_WriteEeprom(SESSION sid, DWORD CardHandle, DWORD address, DWORD data);

/* ************************************************************************* */
/**
@fn: WriteID
@brief: Write ID to onboard SPI Eeprom

@param: CardHandle   - card driver session
@param: id           - ID (32 bits)
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_WriteID(SESSION sid, DWORD CardHandle, DWORD id);

/* ************************************************************************* */
/**
@fn: WriteOutputDac
@brief: Write direct to output voltage DAC.

@param: CardHandle   - card driver session
@param: data         - write data
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_WriteOutputDac(SESSION sid, DWORD CardHandle, DWORD data);

/* ************************************************************************* */
/**
@fn: WriteRegister
@brief: Write register of module. Exported function, meaning parameter checking.

@param: CardHandle   - card driver session
@param: offset       - local module offset address
@param: data         - write data
*/
/* ************************************************************************** */
DWORD PI743LX_API PI743LX_WriteRegister(SESSION sid, DWORD CardHandle, DWORD offset, DWORD data);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif
