/*******************************************************************************
 *  Pickering pi743 module header file of ClientBridge
 *
 *  Copyright (C) 1988 - 2013, Pickering Interfaces ltd.
 *   
 *  Support: support@pickeringswitch.com
 *  Version: 1.0.3
 *  Supported OS: Windows, Linux, LabViewRT
 *
 * Licence:
 * This agreement is made between Pickering Interfaces Ltd ("Pickering") and you,
 * the person who makes use of Pickering software products ("You"). You must agree
 * all terms in this agreement in order to use Pickering software legally. If you
 * don't agree all terms in the agreement, please don't use Pickering software, and
 * delete all related files from your computer.
 * 
 * 1. OWNERSHIP: Pickering software is fully owned by Pickering, this license
 * agreement doesn't change the ownership.
 * 
 * 2. LICENSE: Pickering grants You the license to use Pickering software, free of
 * charge, if you accept all the conditions listed in this agreement. "Use" means
 * loading the product to CPU, memory, and/or other storages of your computer.
 * 
 * 3. CONDITIONS: To be licensed to use Pickering software, You must: 
 *    a) Not modify any part of Pickering software; 
 *    b) Agree to release Pickering from all liabilities caused directly or
 *       indirectly by using Pickering software; 
 *    c) Agree not to attempt to reverse engineer, de-compile or use any other
 *       tools to extract Source code from Pickering Software.
 *       
 * 4. CONSEQUENTIAL LICENSES: Some functions of Pickering software requires
 * additional licenses to fully operate. Pickering accepts no responsibility for
 * the provision of said licenses.
 * 
 * 5. REDISTRIBUTION:. You may freely re-distribute Pickering software in any way 
 * unless explicitly stated to the contrary for a particular product.
 *
 *******************************************************************************/

/// \cond

#ifndef _PI743LX_H
#define _PI743LX_H

#include "Picmlx.h"

#ifdef LINUX
  typedef signed long int   BOOL;  
  typedef unsigned long int DWORD;  
  typedef char				CHAR;

  #define PI743LX_API
#else
  #include <windows.h>

  #ifdef PI743LX_EXPORTS
    #define PI743LX_API __declspec(dllexport) __stdcall
  #else
    #define PI743LX_API __declspec(dllimport) __stdcall
  #endif // #ifdef PI743LX_EXPORTS
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/// \endcond

/// PI743LX error codes
enum ErrorCodes {
	PI743LX_ERROR_FUNC_NOT_LOADED = 0x3000,		///< Desired function not loaded from 'pi743.dll' library.
	PI743LX_ERROR_FUNC_NOT_LOCAL,				///< Function not supported on localhost.
	PI743LX_SPECIFIC_ERROR_BASE = 0xBFFC0900L,	///< start of driver specific errors 
	PI743LX_ERROR_ALLOC,						///< memory allocation error
	PI743LX_ERROR_SESSION,						///< invalid pi743 session
	PI743LX_ERROR_EETIMEOUT,					///< eeprom Timeout
	PI743LX_ERROR_ADCTIMEOUT,					///< adc Timeout
	PI743LX_ERROR_ADCVCALERROR,					///< adc Voltage calibration error
	PI743LX_ERROR_ADCCCALERROR,					///< adc Current calibration error
	PI743LX_ERROR_DRIVER,						///< low Level driver error
	PI743LX_ERROR_MEMTEST,						///< memory test error
	PI743LX_ERROR_INVALID_RSCNAME,				///< invalid resource name
	PI743LX_ERROR_SETTLETIMEOUT,				///< settle operation timeouted
	PI743LX_ERROR_PARAMETER1 = 0xFFF80901L		///< parameter out of range, for next parameter incr. value with 1
};

/// PI743LX warning codes
enum WarningCodes {
	PI743LX_SPECIFIC_WARNING_BASE = 0x3FFC0900L,	///< base for driver specific warnings
	PI743LX_WARNING_EEPROMCHECK,					///< Eeprom check failed
	PI743LX_WARNING_NSUP_RESET,						///< Reset not supported
	PI743LX_WARNING_NSUP_SELF_TEST,					///< Self test not supported
	PI743LX_WARNING_NSUP_ERROR_QUERY,				///< Error query not supported
	PI743LX_WARNING_NSUP_REV_QUERY					///< Revision query not supported
};

/// Connection defines for OutputConnection
enum ConnectionStatus {
	PI743LX_CON_DISCONNECT		=	0,				///< Disconnect output
	PI743LX_CON_2WIRE_INTERNAL	=	1,				///< Connect 2 wire, internal sense
	PI743LX_CON_4WIRE_EXTERNAL	=	2				///< Connect 4 wire, external sense
};

/// Over current status defines
enum OverCurrentStatus {
	PI743LX_OC_STAT_NOOC			=	0,			///< Overcurrent not signaling
	PI743LX_OC_STAT_OCPENDING		=	1,			///< Current limit is breached, instrument limits output current
	PI743LX_OC_STAT_OCLATCHED		=	2,			///< Current limit was breached, flag is not cleared
	PI743LX_OC_STAT_OCLATCHPEND		=	3			///< Current limit is breached and flag is active
};

/// Trigger sources defines
enum TriggerSource {
	PI743LX_TRIG_SRC_NOTRIG			=	0,			///< Trigger disabled
	PI743LX_TRIG_SRC_FRONT			=	1,			///< Front trigger source
	PI743LX_TRIG_SRC_SOFTWARE		=	2,			///< Software trigger source
	PI743LX_TRIG_SRC_PXI0			=	3,			///< PXI0 trigger source
	PI743LX_TRIG_SRC_PXI1			=	4,			///< PXI1 trigger source
	PI743LX_TRIG_SRC_PXI2			=	5,			///< PXI2 trigger source
	PI743LX_TRIG_SRC_PXI3			=	6,			///< PXI3 trigger source
	PI743LX_TRIG_SRC_PXI4			=	7,			///< PXI4 trigger source
	PI743LX_TRIG_SRC_PXI5			=	8,			///< PXI5 trigger source
	PI743LX_TRIG_SRC_PXI6			=	9,			///< PXI6 trigger source
	PI743LX_TRIG_SRC_PXI7			=	10,			///< PXI7 trigger source
	PI743LX_TRIG_SRC_PXISTAR		=	11			///< PXISTAR trigger source
};

/// Trigger action defines
enum TriggerAction {
	PI743LX_TRIG_ACT_NOACT				=	0,		///< Trigger disabled
	PI743LX_TRIG_ACT_VC_UPDATE			=	1,		///< Update voltage and current limit upon trigger
	PI743LX_TRIG_ACT_V_CAPTURE			=	2,		///< Caputre output voltage upon trigger
	PI743LX_TRIG_ACT_C_CAPTURE			=	3,		///< Capture output current upon trigger
	PI743LX_TRIG_ACT_V_UPDATE_CAPTURE	=	4,		///< Update and capture output voltage upon trigger
	PI743LX_TRIG_ACT_C_UPDATE_CAPTURE	=	5		///< Update current limit and capture output current upon trigger
};

/// Trigger level defines
enum TriggerLevel {
	PI743LX_TRIG_LVL_LOW			=	0,			///< Trigger occurs when level is low/edge is falling
	PI743LX_TRIG_LVL_HIGH			=	1			///< Trigger occurs when level is high/edge is rising
};

/// Trigger mode defines
enum TriggerMode {
	PI743LX_TRIG_MODE_EDGE		=	0,				///< Trigger occurs proper edge happens (falling or rising, depends on level)
	PI743LX_TRIG_MODE_LEVEL		=	1				///< Trigger occurs when source is in proper level, capture action is periodically activated
};

/// Definition of "do not change" constant for SetOutputAll function
enum SetAllValues {
	PI743LX_SETALL_DO_NOT_CHANGE	=	-1			///< Do not change parameter of function
};

#define PI743LX_SUCCESS     (0L)

/* Version */

/**
 * <summary>
 * Returns a version of this library.
 * <param name="Version">Version of this library.</param>
 * </summary> 
 */
void PI743LX_API PI743LX_GetVersion(DWORD *Version);

/* Error handling functions */

/**
 * <summary>
 * Returns TRUE if an error occured or FALSE if none.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <returns>TRUE if an error occured or FALSE if none.</returns>
 */
BOOL PI743LX_API PI743LX_IsError(SESSION Sid);

/**
 * <summary>
 * Returns last occured error code.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="ErrorCode">Pointer to variable to receive a last occured error code if any.</param>
 * <returns>Zero or non zero error code if Session is not valid.</returns>
 */
DWORD PI743LX_API PI743LX_GetLastErrorCode(SESSION Sid,DWORD *ErrorCode);

/**
 * <summary>
 * Returns last occured error as a message.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="ErrorMsg">Pointer to a variable to receive a error message.</param>
 * <param name="Length">Length of ErrorMsg string.</param>
 * <returns>Zero or non zero error code if Session is not valid.</returns>
 */
DWORD PI743LX_API PI743LX_GetLastErrorMessage(SESSION Sid,LPCHAR ErrorMsg,DWORD Length);

/**
 * <summary>
 * Convert error code to a message.
 * </summary>
 * <param name="ErrorCode">Error code.</param> 
 * <param name="ErrorMsg">Pointer to a variable to receive a error message.</param>
 * <param name="Length">Length of ErrorMsg string.</param>
 * <returns>Zero for success, or non-zero error code.</returns>
 */
DWORD PI743LX_API PI743LX_ErrorCodeToMessage(SESSION Sid, DWORD ErrorCode, LPCHAR ErrorMsg, DWORD Length);

/**
 * <summary>
 *   Clears the OverCurrent status register bit.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ClearOverCurrentStatus (SESSION Sid, DWORD CardHandle);

/**
 * <summary>
 *   Clears the trigger status register bit and software trigger.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ClearTriggerStatus (SESSION Sid, DWORD CardHandle);

/**
 * <summary>
 *   Close the instrument.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_close (SESSION Sid, DWORD CardHandle);

/**
 * <summary>
 * Get position of memory address counter.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Addresscounter">Address counter position</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetAddressCounter (SESSION Sid, DWORD CardHandle, DWORD* Addresscounter);

/**
 * <summary>
 * Get clock Divider value.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Divider">Divider value</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetClockDivider (SESSION Sid, DWORD CardHandle, DWORD* Divider);

/**
 * <summary>
 * Read Current from ADC.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Measured Current</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetMeasuredOutputCurrent (SESSION Sid, DWORD CardHandle, double* Current);

/**
 * <summary>
 * Read Voltage from ADC.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Voltage">Measured Voltage</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetMeasuredOutputVoltage (SESSION Sid, DWORD CardHandle, double* Voltage);

/**
 * <summary>
 * Get various output parameters.
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Voltage">Output Voltage</param>
 * <param name="Current">Output Current limit</param>
 * <param name="Power">Maximum Power limit (<= 40 W)</param>
 * <param name="Connection">Connection status, see enum ConnectionStatus</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetOutputAll (SESSION Sid, DWORD CardHandle, double *Voltage, double *Current, double *Power, DWORD *Connection);

/**
 * <summary>
 * Get output Connection status.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Connection">Connection status, see enum ConnectionStatus</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetOutputConnection (SESSION Sid, DWORD CardHandle, DWORD* Connection);

/**
 * <summary>
 * Get output Current limit.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Output Current limit
                        Maximum total output Power is 40W
                        Maximum Current = 0.8A at 48V (= maximum output Voltage)
                        Maximum Current = 2A at 20V   (= maximum output Current)</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetOutputCurrentLimit (SESSION Sid, DWORD CardHandle, double* Current);

/**
 * <summary>
 * Get output Voltage.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Voltage">Output Voltage
                        Maximum total output Power is 40W
                        Maximum Current = 0.8A at 48V (= maximum output Voltage)
                        Maximum Current = 2A at 20V   (= maximum output Current)</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetOutputVoltage (SESSION Sid, DWORD CardHandle, double* Voltage);

/**
 * <summary>
 * Get OverCurrent status.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="OverCurrent">OverCurrent status, see OverCurrentStatus enum</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetOverCurrentStatus (SESSION Sid, DWORD CardHandle, DWORD* OverCurrent);

/**
 * <summary>
 * Get Power supply main dc/dc converter Power state.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Psstatus">Power supply status
                        0 : disable main dc/dc converter
                        1 : enable main dc/dc converter</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetPowerSupplyStatus (SESSION Sid, DWORD CardHandle, DWORD* Psstatus);

/**
 * <summary>
 * Get trigger Source, destination and Level.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Source">Trigger Source, see TriggerSource enum</param>
 * <param name="Action">Action select, see TriggerAction enum</param>
 * <param name="Level">Trigger Level select, see TriggerLevel enum</param>
 * <param name="Mode">Trigger Mode select, see TriggerMode enum</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetTrigger (SESSION Sid, DWORD CardHandle, DWORD* Source, DWORD* Action, DWORD* Level, DWORD* Mode);

/**
 * <summary>
 * Get trigger Current (Current limit after a trigger).
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Current limit to set on trigger</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetTriggerCurrent (SESSION Sid, DWORD CardHandle, double* Current);

/**
 * <summary>
 * Get the trigger status register bit.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Triggerstatus">trigger status:
                            bit 0 software trigger
                            bit 1 trigger status register</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetTriggerStatus (SESSION Sid, DWORD CardHandle, DWORD* Triggerstatus);

/**
 * <summary>
 * Get trigger Voltage (Voltage after a trigger).
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Voltage to set on trigger</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_GetTriggerVoltage (SESSION Sid, DWORD CardHandle, double* Voltage);

/**
 * <summary>
 *      Opens a session to the instrument specified by the resource 
        description parameter.  If IDQuery is true then this function 
        verifies that the instrument is a pi743.  If reset is true then
        this function resets the instrument to it's default state. 
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="ResourceDescription">Instrument description</param>
 * <param name="IDQuery">If true perform in system verification</param>
 * <param name="Reset">If true perform reset operation</param>
 * <param name="CardHandle">Reference for card driver session</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_init(SESSION Sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD* CardHandle);

/**
 * <summary>
 * Tests memory.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="LogSize">Number of errors to report; operation is aborted after [logsize] errors.</param>
 * <param name="Erroraddress">In case of an error, (array of) the error address</param>
 * <param name="Errordata">In case of an error, (array of) the error data</param>
 * <param name="Expectdata">In case of an error, (array of) the expected data</param>
 * <param name="FoundErrors">Number of found errors</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_MemoryTest (SESSION Sid, DWORD CardHandle, DWORD LogSize, DWORD *Erroraddress, DWORD *Errordata, DWORD *Expectdata, DWORD *FoundErrors);

/**
 * <summary>
 * Read captured Current from memory.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Array to put Current from memory</param>
 * <param name="Count">Number of memory places to read</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ReadCapturedCurrent(SESSION Sid, DWORD CardHandle, double* Current, DWORD Count);

/**
 * <summary>
 * Read captured Voltage from memory.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Voltage">Array to put Voltage from memory</param>
 * <param name="Count">Number of memory places to read</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ReadCapturedVoltage(SESSION Sid, DWORD CardHandle, double* Voltages, DWORD Count);

/**
 * <summary>
 * Read Current calibration values.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="OffsetCurrentcdac">Offset calibration Current for Current dac</param>
 * <param name="GainCurrentcdac">Gain calibration Current for Current dac</param>
 * <param name="Offsetcodecadc">Offset calibration code of adc Current measurement</param>
 * <param name="Gaincodecadc">Gain calibration code of adc Current measurement</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ReadCurrentCalValues (SESSION Sid, DWORD CardHandle, double* OffsetCurrentcdac, double* GainCurrentcdac, DWORD* Offsetcodecadc, DWORD* Gaincodecadc);

/**
 * <summary>
 * Read serial number from onboard SPI Eeprom
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Id">32bit serial number</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ReadID(SESSION Sid, DWORD CardHandle, DWORD* Id);

/**
 * <summary>
 * Read Maximum Power setting from onboard SPI Eeprom
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Power">Maximum Power limit (<= 40 W)</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ReadMaximumPower(SESSION Sid, DWORD CardHandle, double* Power);

/**
 * <summary>
 * Read Voltage calibration values.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="OffsetVoltagevdac">Offset calibration Voltage for Voltage dac</param>
 * <param name="GainVoltagevdac">Gain calibration Voltage for Voltage dac</param>
 * <param name="Offsetcodevadc">Offset calibration code of adc Voltage measurement</param>
 * <param name="Gaincodevadc">Gain calibration code of adc Voltage measurement</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_ReadVoltageCalValues (SESSION Sid, DWORD CardHandle, double* OffsetVoltagevdac, double* GainVoltagevdac, DWORD* Offsetcodevadc,DWORD* Gaincodevadc);

/**
 * <summary>
 * Reset the instrument.
 * Not implemented at the moment.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_reset(SESSION Sid, DWORD CardHandle);

/**
 * <summary>
 * Performs TestMemory self test.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="TestResult">Numeric result code</param>
 * <param name="ErrorMessage">Status message</param>
 * <param name="MesageSize">Size of ErrorMessage buffer</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_self_test (SESSION Sid, DWORD CardHandle, BOOL* TestResult, CHAR ErrorMessage[], DWORD MessageSize);

/**
 * <summary>
 * Set position of memory address counter.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Addresscounter">Address counter position</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetAddressCounter (SESSION Sid, DWORD CardHandle, DWORD Addresscounter);

/**
 * <summary>
 * Set clock Divider value.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Divider">Divider value</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetClockDivider (SESSION Sid, DWORD CardHandle, DWORD Divider);

/**
 * <summary>
 * Set various output parameters.
     Maximum total output Power is 40W
     Maximum Current = 0.8A at 48V (= maximum output Voltage)
     Maximum Current = 2A at 20V   (= maximum output Current) * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Voltage">Output Voltage</param>
 * <param name="Current">Output Current limit</param>
 * <param name="Power">Maximum Power limit (<= 40 W)</param>
 * <param name="Connection">Connection status, see enum ConnectionStatus</param>
 * <remarks>If certain parameter is to remain unchanged, use PI743LX_SETALL_DO_NOT_CHANGE constant</remarks>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetOutputAll (SESSION Sid, DWORD CardHandle, double Voltage, double Current, double Power, DWORD Connection);

/**
 * <summary>
 * Set output Connection status.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Connection">Connection status, see enum ConnectionStatus</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetOutputConnection (SESSION Sid, DWORD CardHandle, DWORD Connection);

/**
 * <summary>
 * Set output Current limit.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Output Current limit
                        Maximum total output Power is 40W
                        Maximum Current = 0.8A at 48V (= maximum output Voltage)
                        Maximum Current = 2A at 20V   (= maximum output Current)</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetOutputCurrentLimit (SESSION Sid, DWORD CardHandle, double Current);

/**
 * <summary>
 * Set output Voltage.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Voltage">Output Voltage
                        Maximum total output Power is 40W
                        Maximum Current = 0.8A at 48V (= maximum output Voltage)
                        Maximum Current = 2A at 20V   (= maximum output Current)</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetOutputVoltage (SESSION Sid, DWORD CardHandle, double Voltage);

/**
 * <summary>
 * Set Power supply main dc/dc converter Power state.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Psstatus">Power supply status
                        0 : disable main dc/dc converter
                        1 : enable main dc/dc converter</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetPowerSupplyStatus (SESSION Sid, DWORD CardHandle, DWORD Psstatus);

/**
 * <summary>
 * Generate a software trigger.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetSoftwareTrigger (SESSION Sid, DWORD CardHandle);

/**
 * <summary>
 * Set trigger Source, destination and Level.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Source">Trigger Source, see TriggerSource enum</param>
 * <param name="Action">Action select, see TriggerAction enum</param>
 * <param name="Level">Trigger Level select, see TriggerLevel enum</param>
 * <param name="Mode">Trigger Mode select, see TriggerMode enum</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetTrigger (SESSION Sid, DWORD CardHandle, DWORD Source, DWORD Action, DWORD Level, DWORD Mode);

/**
 * <summary>
 * Set trigger Current (Current limit after a trigger).
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Current limit to set on trigger</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetTriggerCurrent (SESSION Sid, DWORD CardHandle, double Current);

/**
 * <summary>
 * Set trigger Voltage (Voltage after a trigger).
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Current">Voltage to set on trigger</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_SetTriggerVoltage (SESSION Sid, DWORD CardHandle, double Voltage);

/**
 * <summary>
 * Store Current calibration values.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="OffsetCurrentcdac">Offset calibration Current for Current dac</param>
 * <param name="GainCurrentcdac">Gain calibration Current for Current dac</param>
 * <param name="Offsetcodecadc">Offset calibration code of adc Current measurement</param>
 * <param name="Gaincodecadc">Gain calibration code of adc Current measurement</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_StoreCurrentCalValues (SESSION Sid, DWORD CardHandle, double OffsetCurrent, double GainCurrentunsigned, DWORD Offsetcode, DWORD Gaincode);

/**
 * <summary>
 * Store Voltage calibration values.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="OffsetVoltagevdac">Offset calibration Voltage for Voltage dac</param>
 * <param name="GainVoltagevdac">Gain calibration Voltage for Voltage dac</param>
 * <param name="Offsetcodevadc">Offset calibration code of adc Voltage measurement</param>
 * <param name="Gaincodevadc">Gain calibration code of adc Voltage measurement</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_StoreVoltageCalValues (SESSION Sid, DWORD CardHandle, double OffsetVoltage, double GainVoltage, DWORD Offsetcode, DWORD Gaincode);

/**
 * <summary>
 * Write maximum Power setting from onboard SPI Eeprom
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Power">Maximum Power limit (<= 40 W)</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_WriteMaximumPower(SESSION Sid, DWORD CardHandle, double Power);

/**
 * <summary>
 *   Returns a list of available instruments (max 100).
 *   Minimum size for buslist and devicelist is 100.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="Buslist">bus of the instrument(s)</param>
 * <param name="Devicelist">device of the instrument</param>
 * <param name="Count">size of list arrays, returns number of available instruments</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_FindInstruments(SESSION Sid, DWORD Buslist[], DWORD Devicelist[], DWORD *Count);

/**
 * <summary>
 *   Returns a list of available instruments (max 100).
 *   Minimum size for rsrclist is 100.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="Rsrclist">array of strings containing resource list</param>
 * <param name="Count">size of list array, returns number of available instruments</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_FindInstrumentsRsrc(SESSION Sid, CHAR Rsrclist[][20], DWORD *Count);

/**
 * <summary>
 * Query if output Voltage Stabilized or not
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Stabilized">Variable to receive result. 1 = output Stabilized, 0 = output not Stabilized.</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_IsStabilized (SESSION Sid, DWORD CardHandle, BOOL *Stabilized);

/**
 * <summary>
 * Waits for output to stabilize or Timeout expires.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Timeout">Maximum time settling will take (in ms).</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_WaitForStabilized (SESSION Sid, DWORD CardHandle, DWORD Timeout);

/**
 * <summary>
 * Write capture memory.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="CardHandle">Card driver session</param>
 * <param name="Memorydata">memory data to write</param>
 * <param name="Length">number of memory places to write</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_WriteMemory (SESSION Sid, DWORD CardHandle, DWORD* Memorydata, DWORD Length);

/**
 * <summary> 
 * Obtains the pi743 driver version.
 * </summary>
 * <param name="Sid">Handle of current session.</param> 
 * <param name="Version">The 743 driver version code, multiplied by 100 (i.e. a value of 100 represents version 1.00)</param> 
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_Version(SESSION Sid, DWORD *Version);

//LXI only functions

/**
 * <summary>
 *      Opens a session to the instrument specified by the resource 
        description parameter for read only mode. This function is only
		available for cards placed in LXI unit.
 * </summary>
 * <param name="Sid">Handle of Current session.</param>
 * <param name="ResourceDescription">Instrument description</param>
 * <param name="IDQuery">If true perform in system verification</param>
 * <param name="Reset">If true perform reset operation</param>
 * <param name="CardHandle">Reference for card driver session</param>
 * <returns>Zero if operation is successful, error or warning code</returns>
 */
DWORD PI743LX_API PI743LX_initCardForRead(SESSION Sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD *CardHandle);

#ifdef __cplusplus
}
#endif

#endif //_PI743LX_H
