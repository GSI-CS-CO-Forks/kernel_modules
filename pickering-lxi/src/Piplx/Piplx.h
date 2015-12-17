/*******************************************************************************
*  Pickering switching module header file of ClientBridge for controlling
*  cards in LXI device.
*
*  Copyright (C) 1988 - 2013, Pickering Interfaces ltd.
*   
*  Support: support@pickeringswitch.com
*  Version: 1.6.0
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
*       tools to extract source code from Pickering Software.
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

#ifndef _PIPLX_H_
#define _PIPLX_H_

#include <Picmlx.h>

#ifdef WIN32

#ifdef PIPLX_EXPORTS
#define PIPLX_API __declspec(dllexport) __stdcall
#else
#define PIPLX_API __declspec(dllimport) __stdcall
#endif // #ifdef PIPLX_EXPORTS

#elif LINUX  // #ifdef WIN32

#define PIPLX_API  

#endif // #elif LINUX

/// \endcond

/// Card status bits for PIPLX_Status() and PIPLX_SubStatus() results 
enum CardStatus {
    STAT_NO_CARD						= 0x80000000UL,	///< No card with specified number 
    STAT_WRONG_DRIVER				= 0x40000000UL,	///< Card requires later driver version 
    STAT_EEPROM_ERR					= 0x20000000UL,	///< Error interpreting card EEPROM data 
    STAT_DISABLED						= 0x10000000UL,	///< Card is disabled 
    STAT_NO_SUB							= 0x08000000UL,	///< Card has no sub-unit with specified number 
    STAT_BUSY								= 0x04000000UL,	///< Busy (card or sub-unit) 
    STAT_HW_FAULT						= 0x02000000UL,	///< Hardware fault (eg. defective serial loop) 
    STAT_PARITY_ERROR				= 0x01000000UL,	///< 9050 Parity Error 
    STAT_PSU_INHIBITED			= 0x00800000UL,	///< PSU sub-unit - supply is disabled (by software) 
    STAT_PSU_SHUTDOWN		    = 0x00400000UL,	///< PSU sub-unit - supply is shutdown (due to overload) 
    STAT_PSU_CURRENT_LIMIT	= 0x00200000UL,	///< PSU sub-unit - supply is operating in current-limited mode 
    STAT_CORRUPTED					= 0x00100000UL,	///< Sub-unit logical state is corrupted 
    STAT_CARD_INACCESSIBLE	= 0x00080000UL,	///< Card cannot be accessed (failed/removed/unpowered)
    STAT_UNCALIBRATED		    = 0x00040000UL,	///< Calibration data is faulty (card or sub-unit)
    STAT_CALIBRATION_DUE	  = 0x00020000UL,	///< Calibration is due (card or sub-unit)
    STAT_BIRST_ENABLED      = 0x00010000UL,	///< BIRST is active (card or sub-unit)
    STAT_OK		= 0x00000000UL
};

/// Driver operating mode control bits for PIPLX_SetMode()
enum DriverModes {
    MODE_DEFAULT			= 0x00000000UL,	///< Default, no special modes set 
    MODE_NO_WAIT			= 0x00000001UL,	///< Function calls exclude output settling delay 
    MODE_UNLIMITED		= 0x00000002UL,	///< Disable maximium closure limits 
    MODE_REOPEN				= 0x00000004UL,	///< Allow re-open without clearing cards 
    MODE_IGNORE_TEST	= 0x00000008UL,	///< Enable card operation even if selftest fails 
    MODE_DEBUG        = 0x00010000UL  ///< Enable debug mode
};

/// OUTPUT sub-unit type specifier codes (as returned by PIPLX_SubInfo())
enum OutSubunitTypes {
    TYPE_SW			  = 1,	///< Uncommitted switches 
    TYPE_MUX		  = 2,	///< Relay multiplexer (single-channel only) 
    TYPE_MUXM		  = 3,	///< Relay multiplexer (multi-channel capable) 
    TYPE_MAT		  = 4,	///< Standard matrix 
    TYPE_MATR		  = 5,	///< RF matrix 
    TYPE_DIG		  = 6,	///< Digital outputs 
    TYPE_RES		  = 7,	///< Programmable Resistor 
    TYPE_ATTEN	  = 8,	///< Programmable Attenuator 
    TYPE_PSUDC	  = 9,	///< Power supply - DC 
    TYPE_BATT     = 10, ///< Battery simulator
    TYPE_VSOURCE  = 11, ///< Programmable voltage source
    TYPE_MATP     = 12  ///< Matrix with restricted operating modes
};

/// PSU capability flag bits for PIPLX_PsuInfo() 
enum PSUCapabilityFlags {
    PSU_CAP_OUTPUT_CONTROL		= 0x00000001,	///< Has output on/off control 
    PSU_CAP_OUTPUT_SENSE		= 0x00000002,	///< Has logic-level sensing of output active state 
    PSU_CAP_PROG_VOLTAGE		= 0x00000004,	///< Output voltage is programmable 
    PSU_CAP_PROG_CURRENT		= 0x00000008,	///< Output current is programmable 
    PSU_CAP_CURRENT_MODE_SENSE	= 0x00000010	///< Can sense if operating in current-limited mode 
};

/// AttrCode values for PIPLX_SubAttribute() 
enum AttrValueCodes {
    SUB_ATTR_CHANNEL_SUBSWITCHES		= 1,		///< Gets number of subswitches per logical channel 
    SUB_ATTR_X_ISO_SUBSWITCHES			= 2,		///< Gets number of subswitches per logical X-isolator 
    SUB_ATTR_Y_ISO_SUBSWITCHES			= 3,		///< Gets number of subswitches per logical Y-isolator 
    SUB_ATTR_X_LOOPTHRU_SUBSWITCHES	= 4,		///< Gets number of subswitches per logical X-loopthru 
    SUB_ATTR_Y_LOOPTHRU_SUBSWITCHES	= 5,		///< Gets number of subswitches per logical Y-loopthru 
    SUB_ATTR_MATRIXP_TOPOLOGY       = 6,    ///< Gets a code representing MATRIXP topology

    SUB_ATTR_NUM_X_SEGMENTS			= 0x100,	///< Gets number of X-axis segments 
    SUB_ATTR_X_SEGMENT01_SIZE		= 0x101,	///< Gets size of X-axis segment 1 
    SUB_ATTR_X_SEGMENT02_SIZE		= 0x102,	///< Gets size of X-axis segment 2 
    SUB_ATTR_X_SEGMENT03_SIZE		= 0x103,	///< Gets size of X-axis segment 3 
    SUB_ATTR_X_SEGMENT04_SIZE		= 0x104,	///< Gets size of X-axis segment 4 
    SUB_ATTR_X_SEGMENT05_SIZE		= 0x105,	///< Gets size of X-axis segment 5 
    SUB_ATTR_X_SEGMENT06_SIZE		= 0x106,	///< Gets size of X-axis segment 6 
    SUB_ATTR_X_SEGMENT07_SIZE		= 0x107,	///< Gets size of X-axis segment 7 
    SUB_ATTR_X_SEGMENT08_SIZE		= 0x108,	///< Gets size of X-axis segment 8 
    SUB_ATTR_X_SEGMENT09_SIZE		= 0x109,	///< Gets size of X-axis segment 9 
    SUB_ATTR_X_SEGMENT10_SIZE		= 0x10A,	///< Gets size of X-axis segment 10 
    SUB_ATTR_X_SEGMENT11_SIZE		= 0x10B,	///< Gets size of X-axis segment 11 
    SUB_ATTR_X_SEGMENT12_SIZE		= 0x10C,	///< Gets size of X-axis segment 12 

    SUB_ATTR_NUM_Y_SEGMENTS			= 0x200,	///< Gets number of Y-axis segments 
    SUB_ATTR_Y_SEGMENT01_SIZE		= 0x201,	///< Gets size of Y-axis segment 1 
    SUB_ATTR_Y_SEGMENT02_SIZE		= 0x202		///< Gets size of Y-axis segment 2 
};

/// SwitchFunc codes for PIPLX_OpSwitch() 
enum SwitchFuncCodes {
    SW_FUNC_CHANNEL		      = 0,	///< A channel (crosspoint) switch 
    SW_FUNC_X_ISO				    = 1,	///< An X-isolation switch 
    SW_FUNC_Y_ISO				    = 2,	///< A Y-isolation switch 
    SW_FUNC_X_LOOPTHRU	    = 3,	///< An X-loopthru switch 
    SW_FUNC_Y_LOOPTHRU	    = 4,  ///< A Y-loopthru switch
    SW_FUNC_X_BIFURCATION   = 5,  ///< An X-bifurcation (changeover) switch - in MATRIXP_RESTRICTIVE_Y topology
    SW_FUNC_Y_BIFURCATION   = 6   ///< An Y-bifurcation (changeover) switch - in MATRIXP_RESTRICTIVE_X topology
};

/// SwitchAction codes for PIPLX_OpSwitch() 
enum SwitchActionCodes {
    SW_ACT_NONE		= 0,	///< No switch change - just set State result 
    SW_ACT_OPEN		= 1,	///< Open switch 
    SW_ACT_CLOSE	= 2		///< Close switch 
};

/// Calibration store codes
enum CalStoreCodes {
    CAL_STORE_USER = 0,     ///< Access user calibration store
    CAL_STORE_FACTORY       ///< Access factory calibration store
};

/// Mode codes for PIPLX_ResSetResistance()
enum {
    RES_MODE_SET		= 0	///< Set the specified value
};

/// Maximum length for PIPLX_Diagnostic() returned string.
enum MaxDiagStrLength {
    MAX_DIAG_LENGTH = 1024
};

/// Capabilites flag bits for PIPLX_ResInfo()
enum ResInfoCapabilites {
    RES_CAP_NONE	= 0x00000000UL,	///< No special capabilities
    RES_CAP_PREC	= 0x00000001UL,	///< Supports precision setting (40-260 etc.)
    RES_CAP_ZERO	= 0x00000002UL,	///< Supports "zero ohms" setting
    RES_CAP_INF		= 0x00000004UL,	///< Supports infinity setting
    RES_CAP_REF		= 0x00000008UL	///< Supports reference calibration value (e.g. 40-265)
};

/// LXI error codes
enum LXIErrorCodes {
    // if no error then return value is zero, windows system constant: NO_ERROR	
    NO_ERR = 0,         ///< No error
    ER_NO_CARD,   			///< No Pickering card has the specified number 
    ER_NO_INFO,					///< Cannot obtain information for specified card 
    ER_CARD_DISABLED,		///< Specified card is disabled 
    ER_BAD_SUB,					///< Sub-unit value out-of-range for target card 
    ER_BAD_BIT,					///< Bit-number out-of-range for target sub-unit 
    ER_NO_CAL_DATA,			///< Target sub-unit has no calibration data to read/write 
    ER_BAD_ARRAY,				///< SafeArray type, shape or size is incorrect 
    ER_MUX_ILLEGAL,			///< Non-zero write data value is illegal for MUX sub-unit 
    ER_EXCESS_CLOSURE,	///< Execution would cause closure limit to be exceeded 
    ER_ILLEGAL_MASK,		///< One or more of the specified channels cannot be masked 
    ER_OUTPUT_MASKED,	  ///< Cannot activate an output that is masked
    ER_BAD_LOCATION,		///< Cannot open a Pickering card at the specified location 
    ER_READ_FAIL,				///< Failed read from hardware 
    ER_WRITE_FAIL,			///< Failed write to hardware 
    ER_DRIVER_OP,				///< Hardware driver failure 
    ER_DRIVER_VERSION,	///< Incompatible hardware driver version 
    ER_SUB_TYPE,				///< Function call incompatible with sub-unit type or capabilities 
    ER_BAD_ROW,					///< Matrix row value out-of-range 
    ER_BAD_COLUMN,			///< Matrix column value out-of-range 
    ER_BAD_ATTEN,				///< Attenuation value out-of-range 
    ER_BAD_VOLTAGE,			///< Voltage value out-of-range 
    ER_BAD_CAL_INDEX,		///< Calibration index value out-of-range 
    ER_BAD_SEGMENT,			///< Segment number out of range 
    ER_BAD_FUNC_CODE,		///< Function code value out of range 
    ER_BAD_SUBSWITCH,		///< Subswitch value out of range 
    ER_BAD_ACTION,			///< Action code out of range 
    ER_STATE_CORRUPT,		///< Cannot execute due to corrupt sub-unit state 
    ER_BAD_ATTR_CODE,		///< Unrecognised attribute code 
    ER_EEPROM_WRITE_TMO,///< Timeout writing to EEPROM
    ER_ILLEGAL_OP,      ///< Operation is illegal in the sub-unit's current state
    ER_BAD_POT,         ///< Unrecognised pot number requested
    ER_MATRIXR_ILLEGAL, ///< Invalid write pattern for MATRIXR sub-unit
    ER_MISSING_CHANNEL, ///< Attempted operation on non-existent channel
    ER_CARD_INACCESSIBLE, ///< Card cannot be accessed (failed/removed/unpowered)
    ER_BAD_FP_FORMAT,     ///< Unsupported internal floating-point format (internal error)
    ER_UNCALIBRATED,      ///< Sub-unit is not calibrated
    ER_BAD_RESISTANCE,    ///< Unobtainable resistance value
    ER_BAD_STORE,         ///< Invalid calibration store number
    ER_BAD_MODE,          ///< Invalid mode value
    ER_SETTINGS_CONFLICT,  ///< Conflicting device settings
    ER_CARD_TYPE,         ///< Function call incompatible with card type or capabilities
    ER_BAD_POLE,          ///< Switch pole value out of range
    ER_MISSING_CAPABILITY,  ///< Attempt to activate a non-existent capability
    ER_MISSING_HARDWARE,    ///< Action requires hardware that is not present
    ER_HARDWARE_FAULT,      ///< Faulty hardware
    ER_EXECUTION_FAIL,      ///< Failed to execute (e.g. blocked by a hardware condition)
    ER_BAD_CURRENT,         ///< Current value out of range
    ER_BAD_RANGE,			      ///< Invalid range value
    ER_ATTR_UNSUPPORTED,    ///< Attribute not supported
    ER_BAD_REGISTER,        ///< Register number out of range
    ER_MATRIXP_ILLEGAL      ///< Invalid channel closure or write pattern for MATRIXP sub-unit
};

/// PIPLX error codes
enum PiplxErrorCodes {		
    ER_PIPLX_BASE = 0x00002000, // Do not use, only for internal use
    ER_PIPLX_INVALID_SESSION,   /*!< DEPRECATED! Session ID is invalid => Not used anymore.
                                Replaced by ER_PICMLX_INVALID_SESSION */
                                ER_PIPLX_FUNC_NOT_LOADED,   ///< Desired function not loaded from 'Picmlx_w32.dll' library.
                                ER_PIPLX_NOT_CONNECTED,     ///< You are not connected to remote host.
                                ER_PIPLX_FUNC_NOT_LOCAL,    ///< Function not supported on localhost.
                                ER_PIPLX_NOT_INIT,          ///< DEPRECATED! Library wasn't initialized!
                                ER_PIPLX_PARAM_SIZE,        ///< DEPRECATED!  Parametr is NULL or size is invalid.
                                ER_PIPLX_END_MARK           ///< DEPRECATED!  Do not use, only for internal use
};

/// Battery simulator sub-unit type
enum BatterySimulatorSubunit {
    BATT_ALL_BATT_SUB_UNITS = 0x00    ///< All sub-units
};

/// Voltage Source
enum VoltageSource {
    VSOURCE_ALL_VSOURCE_SUB_UNITS	= 0UL	///< Sub-unit number to make operation affect all VSOURCE sub-units
};

/// MATRIXP Topologies
enum MatrixPTopology  {
    MATRIXP_NOT_APPLICABLE  = 0,  ///< Sub-unit is not MATRIXP type
    MATRIXP_RESTRICTIVE_X   = 1,  ///< MATRIXP allowing only one column (X) connection on any row (Y)
    MATRIXP_RESTRICTIVE_Y   = 2   ///< MATRIXP allowing only one row (Y) connection on any column (X)
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LINUX

    /// For backward compability ... now it's not used.
    void PIPLX_Init();

    /// For backward compability ... now it's not used.
    void PIPLX_Free();

#endif

    /* Version */

    /**
    * <summary>
    * Returns a version of this library.
    * </summary> 
    * <returns>Version of this library.</returns>
    */
    DWORD PIPLX_API PIPLX_GetVersion(void);

    /* Error handling functions */

    /**
    * <summary>
    * Returns TRUE if an error occured or FALSE if none.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <returns>TRUE if an error occured or FALSE if none.</returns>
    */
    BOOL PIPLX_API PIPLX_IsError(SESSION Sid);

    /**
    * <summary>
    * Returns last occured error code.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="ErrorCode">Pointer to variable to receive a last occured error code if any.</param>
    * <returns>Zero or non zero error code if Session is not valid.</returns>
    */
    DWORD PIPLX_API PIPLX_GetLastErrorCode(SESSION Sid,DWORD *ErrorCode);

    /**
    * <summary>
    * Returns last occured error as a message.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="ErrorMsg">Pointer to a variable to receive a error message.</param>
    * <param name="Length">Length of ErrorMsg string.</param>
    * <returns>Zero or non zero error code if Session is not valid.</returns>
    */
    DWORD PIPLX_API PIPLX_GetLastErrorMessage(SESSION Sid,LPCHAR ErrorMsg,DWORD Length);

    /**
    * <summary>
    * Convert error code to a message.
    * </summary>
    * <param name="ErrorCode">Error code.</param> 
    * <param name="ErrorMsg">Pointer to a variable to receive a error message.</param>
    * <param name="Length">Length of ErrorMsg string.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_ErrorCodeToMessage(DWORD ErrorCode,LPCHAR ErrorMsg,DWORD Length);

    /***************************************
    * INITIALISE AND CLOSE
    ***************************************/

    /**
    * <summary>
    * Closes all open Pickering cards, which must have been opened using PIPLX_OpenCards.
    * This function should be called when the application program has finished using them. 
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_CloseCards(SESSION Sid);

    /**
    * <summary>
    * Closes the specified Pickering card, which must have been opened using PIPLX_OpenSpecifiedCard. This function 
    * should be called when the application program has finished using the card. 
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_CloseSpecifiedCard(SESSION Sid,DWORD CardNum);

    /**
    * <summary>
    * Locates and opens all installed Pickering cards. Once cards have been opened, other functions may then be used
    * to access cards numbered 1 thru the value returned.
    * If cards have already been opened by the calling program, they are first closed - as though by
    * PIPLX_CloseCards - and then re-opened.
    * If cards are currently opened by some other program they cannot be accessed and the function returns zero.
    * </summary>
    * <param name="Sid">Handle of current session.</param> 
    * <returns>The number of Pickering cards located and opened or INVALID_RESULT if error occured.</returns> 
    * <remarks>
    * When multiple Pickering cards are installed, the assignment of card numbers depends upon their relative physical
    * locations in the system (or more accurately, on the order in which they are detected by the computer's operating
    * system at boot time).
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_OpenCards(SESSION Sid);

    /**
    * <summary>
    * 
    * </summary>
    * <param name="Sid">Handle of current session.</param> 
    * <returns>The number of Pickering cards located and opened or INVALID_RESULT if error occured.</returns> 
    */
    DWORD PIPLX_API PIPLX_OpenCardsForRead(SESSION Sid);

    /**
    * <summary>
    * Opens the specified Pickering card, clearing all of its outputs. Once a card has been opened, other driver
    * functions may then be used to access it.
    * If the card is currently opened by some other program it cannot be accessed and the function returns an error.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="Bus">The card's logical bus location.</param>
    * <param name="Slot">The card's logical slot location.</param>
    * <param name="CardNum">Pointer to variable to receive the card's logical card number.</param>
    * <returns>Zero for success, or non-zero for error code.</returns>
    * <remarks>
    * The logical Bus and Slot values corresponding to a particular card are determined by system topology;
    * values for cards that are operable by the driver can be discovered using PIPLX_FindFreeCards.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_OpenSpecifiedCard(SESSION Sid,DWORD Bus,DWORD Slot,DWORD *CardNum);

    /**
    * <summary>
    *
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="Bus">The card's logical bus location.</param>
    * <param name="Slot">The card's logical slot location.</param>
    * <param name="CardNum">Pointer to variable to receive the card's logical card number.</param>
    * <returns>Zero for success, or non-zero for error code.</returns>
    * <remarks>
    *
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_OpenSpecifiedCardForRead(SESSION Sid,DWORD Bus,DWORD Slot,DWORD *CardNum);

    /***************************************
    * INFORMATION AND STATUS
    ***************************************/

    /*
    * <summary>
    * Obtains the identification string of the specified card. The string contains these elements:
    * &lt;type code&gt;,&lt;serial number&gt;,&lt;revision code&gt;.
    * The <revision code> value represents the hardware version of the unit - cards have no firmware on-board.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="Str">Buffer to receive a card id.</param>
    * <param name="StrLen">Size of buffer in bytes</params>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_CardId(SESSION Sid,DWORD CardNum,LPCHAR Str,DWORD StrLen);

    /**
    * <summary>
    * Obtains the location of the specified card in terms of the logical PCI bus and slot number in which it is located.
    * These values can be cross-referenced to physical slot locations in a particular system.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="Bus">Pointer to variable to receive bus location.</param>
    * <param name="Slot">Pointer to variable to receive slot location.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_CardLoc(SESSION Sid,DWORD CardNum,DWORD *Bus,DWORD *Slot);

    /**
    * <summary>
    * Obtains the maximum number of switches that may be activated simultaneously in the specified sub-unit.
    * A single-channel multiplexer (MUX type) allows only one channel to be closed at any time. In some other models
    * such as high-density matrix types a limit is imposed to prevent overheating; although it is possible to disable
    * the limit for these types (see PIPLX_SetMode), doing so is not recommended.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output subunit.</param>
    * <param name="Limit">Pointer to variable to receive the result.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_ClosureLimit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Limit);

    /**
    * <summary>
    * Returns count of switched relays from operation system start. At the moment is this function deprecated.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="Count">Pointer to variable to receive result.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_CardOpsCount(SESSION Sid,DWORD CardNum,DWORD *Count);

    /**
    * <summary>
    * Obtains the number of installed cards that are operable by the driver but are not currently opened by it.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="NumCards">Pointer to variable to receive result.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_CountFreeCards(SESSION Sid,DWORD *NumCards);

    /**
    * <summary>
    * Obtains the diagnostic string of the specified card, giving expanded information on any fault conditons
    * indicated by the PIPLX_Status value.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="Str">Pointer to character string to receive the result.</param>
    * <param name="StrLen">Length of character string to receive the result.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    * <remarks>
    * The result string may include embedded newline characters, coded as the ASCII &lt;linefeed&gt; character ('\x0A').
    * The length of the result string will not exceed the value of the driver constant MAX_DIAG_LENGTH.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_Diagnostic(SESSION Sid,DWORD CardNum,LPCHAR Str,DWORD StrLen);

    /**
    * <summary>
    * Obtains the numbers of input and output sub-units implemented on the specified card.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="InSubs">Pointer to variable to receive the number of INPUT sub-units.</param>
    * <param name="OutSubs">Pointer to variable to receive the number of OUTPUT sub-units.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_EnumerateSubs(SESSION Sid,DWORD CardNum,DWORD *InSubs,DWORD *OutSubs);

    /**
    * <summary>
    * Obtains the logical bus and slot locations of installed cards that are operable by the driver and are
    * currently unopened. These values are used with PIPLX_OpenSpecifiedCard.
    * </summary>
    * <param name="Sid">Handle of current session.</param> 
    * <param name="NumCards">The number of cards (maximum) for which information is to be obtained.</param>
    * <param name="BusList">Pointer to the one-dimensional array (vector) to receive cards' bus location values.</param>
    * <param name="SlotList">Pointer to the one-dimensional array (vector) to receive cards' slot location values.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    * <remarks>
    * The bus and slot locations of the first card found are placed respectively in the least significant elements of
    * the BusList and SlotList arrays. Successive elements contain the values for further cards.
    * If the value given for NumCards is less than the number of cards currently accessible, information is obtained
    * only for the number of cards specified.
    * The arrays pointed to must have been assigned at least as many elements as the number of cards for which
    * information is being requested or adjacent memory will be overwritten, causing data corruption and/or a
    * program crash. The number of accessible cards can be discovered using PIPLX_CountFreeCards.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_FindFreeCards(SESSION Sid,DWORD NumCards,DWORD *BusList,DWORD *SlotList);

    /**
    * <summary>
    * Obtains a sub-unit's settling time (the period taken for its switches to stabilise). By default, driver
    * functions retain control during this period so that switches are guaranteed to have stabilised on completion.
    * This mode of operation can be overridden if required - see PIPLX_SetMode.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Time">Pointer to variable to receive the result (in us).</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_SettleTime(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Time);

    /**
    * <summary>
    * Obtains the current status flags for the specified card.
    *
    * Status bits are as follows:
    *   0x80000000 - STAT_NO_CARD (no card with specified number)
    *   0x40000000 - STAT_WRONG_DRIVER (card requires newer driver)
    *   0x20000000 - STAT_EEPROM_ERR (card EEPROM fault)
    *   0x10000000 - STAT_DISABLED (card disabled)
    *   0x04000000 - STAT_BUSY (card operations not completed)
    *   0x02000000 - STAT_HW_FAULT (card hardware defect)
    *   0x01000000 - STAT_PARITY_ERROR (PCIbus parity error)
    *   0x00000000 - STAT_OK (card functional and stable)
    * Corresponding enumerated constants are provided in piplx_w32.h
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <returns>A value representing the card's status flags or INVALID_RESULT if error occured.</returns> 
    * <remarks>
    * At card level, STAT_BUSY indicates if any of a card's sub-units have not yet stabilised.
    * Diagnostic information on fault conditions indicated in the status value can be obtained using PIPLX_Diagnostic.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_Status(SESSION Sid,DWORD CardNum);

    /**
    * <summary>
    * Convert status code to message.
    * </summary>
    * <param name="StatusCode">Status code.</param>
    * <param name="StatusMsg">Pointer to variable to receive a status message.</param>
    * <param name="Length">Length of StatusMsg string.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_StatusCodeToMessage(DWORD StatusCode,LPCHAR StatusMsg,DWORD Length);

    /**
    * <summary>
    * Obtains the value of a sub-unit attribute. These values facilitate operation using PIPLX_OpSwitch.
    * AttrCode:
    *   1 - SUB_ATTR_CHANNEL_SUBSWITCHES - Gets number of subswitches per logical channel (matrix crosspoint)
    *   2 - SUB_ATTR_X_ISO_SUBSWITCHES - Gets number of subswitches per logical X-isolator
    *   3 - SUB_ATTR_Y_ISO_SUBSWITCHES - Gets number of subswitches per logical Y-isolator
    *   4 - SUB_ATTR_X_LOOPTHRU_SUBSWITCHES - Gets number of subswitches per logical X-loopthru
    *   5 - SUB_ATTR_Y_LOOPTHRU_SUBSWITCHES - Gets number of subswitches per logical Y-loopthru
    *   0x100 - SUB_ATTR_NUM_X_SEGMENTS - Gets number of X-axis segments
    *   0x101 - SUB_ATTR_X_SEGMENT01_SIZE - Gets size of X-axis segment 1
    *   0x102 - SUB_ATTR_X_SEGMENT02_SIZE - Gets size of X-axis segment 2
    *   0x103 - SUB_ATTR_X_SEGMENT03_SIZE - Gets size of X-axis segment 3
    *   0x104 - SUB_ATTR_X_SEGMENT04_SIZE - Gets size of X-axis segment 4
    *   0x105 - SUB_ATTR_X_SEGMENT05_SIZE - Gets size of X-axis segment 5
    *   0x106 - SUB_ATTR_X_SEGMENT06_SIZE - Gets size of X-axis segment 6
    *   0x107 - SUB_ATTR_X_SEGMENT07_SIZE - Gets size of X-axis segment 7
    *   0x108 - SUB_ATTR_X_SEGMENT08_SIZE - Gets size of X-axis segment 8
    *   0x109 - SUB_ATTR_X_SEGMENT09_SIZE - Gets size of X-axis segment 9
    *   0x10A - SUB_ATTR_X_SEGMENT10_SIZE - Gets size of X-axis segment 10
    *   0x10B - SUB_ATTR_X_SEGMENT11_SIZE - Gets size of X-axis segment 11
    *   0x10C - SUB_ATTR_X_SEGMENT12_SIZE - Gets size of X-axis segment 12
    *   0x200 - SUB_ATTR_NUM_Y_SEGMENTS - Gets number of Y-axis segments
    *   0x201 - SUB_ATTR_Y_SEGMENT01_SIZE - Gets size of y-axis segment 1
    *   0x202 - SUB_ATTR_Y_SEGMENT02_SIZE - Gets size of y-axis segment 2
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Sub-unit number.</param>
    * <param name="Out">Sub-unit function: 0for INPUT, 1 for OUTPUT.</param>
    * <param name="AttrCode">A value indicating the sub-unit attribute to be queried, see below.</param>
    * <param name="AttrValue">Pointer to variable to receive the attribute's value.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_SubAttribute(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,DWORD AttrCode,DWORD *AttrValue);

    /**
    * <summary>
    * Obtains a description of a sub-unit, as numeric values.
    * Output sub-unit type codes are:
    *   1 - TYPE_SW (uncommitted switch)
    *   2 - TYPE_MUX (multiplexer single-channel)
    *   3 - TYPE_MUXM (multiplexer, multi-channel)
    *   4 - TYPE_MAT (matrix - LF)
    *   5 - TYPE_MATR (matrix - RF)
    *   6 - TYPE_DIG (digital outputs)
    *   7 - TYPE_RES (programmable resistor)
    *   8 - TYPE_ATTEN (programmable attenuator)
    *   9 - TYPE_PSUDC (DC power supply)
    *
    * Corresponding enumerated constants are provided in piplx_w32.h.
    * Input sub-unit type codes are:
    *   1 - INPUT
    * Row and column values give the dimensions of the sub-unit. For all types other than matrices the column value
    * contains the significant dimension: their row value is always '1'.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Sub-unit number.</param>
    * <param name="Out">Sub-unit function: 0for INPUT, 1 for OUTPUT.</param>
    * <param name="TypeNum">Pointer to variable to receive type code.</param>
    * <param name="Rows">Pointer to variable to receive row count.</param>
    * <param name="Cols">Pointer to variable to receive column count.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    * <remarks>
    * Some sub-unit types are supported by functions providing alternate and/or more detailed information.
    * These include:
    *   TYPE_ATTEN - PIPLX_AttenInfo
    *   TYPE_PSUDC - PIPLX_PsuInfo
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_SubInfo(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,DWORD *TypeNum,DWORD *Rows,DWORD *Cols);

    /**
    * <summary>
    * Obtains the current status flags for the specified output sub-unit. Status bits associated with significant
    * card-level conditions are also returned.
    * Status bits are as follows:
    *   0x80000000 - STAT_NO_CARD (no card with specified number)
    *   0x40000000 - STAT_WRONG_DRIVER (card requires newer driver)
    *   0x20000000 - STAT_EEPROM_ERR (card EEPROM fault)
    *   0x10000000 - STAT_DISABLED (card disabled)
    *   0x08000000 - STAT_NO_SUB (no sub-unit with specified number)
    *   0x04000000 - STAT_BUSY (sub-unit operations not completed)
    *   0x02000000 - STAT_HW_FAULT (card hardware defect)
    *   0x01000000 - STAT_PARITY_ERROR (PCIbus parity error)
    *   0x00800000 - STAT_PSU_INHIBITED (power supply output is disabled - by software)
    *   0x00400000 - STAT_PSU_SHUTDOWN (power supply output is shutdown - due to overload)
    *   0x00200000 - STAT_PSU_CURRENT_LIMIT (power supply is operating in current-limited mode)
    *   0x00100000 - STAT_CORRUPTED (sub-unit logical state is corrupted)
    *   0x00000000 - STAT_OK (card functional and stable)
    * Corresponding enumerated constants are provided in 
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Sub-unit number.</param>
    * <returns>A value representing the card's subunit status flags or INVALID_RESULT if error occured.</returns>
    * <remarks>
    * Certain status bits are relevant only for particular classes of sub-unit.
    * Diagnostic information on fault conditions indicated in the status value can be obtained using PIPLX_Diagnostic.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_SubStatus(SESSION Sid,DWORD CardNum,DWORD SubNum);

    /**
    * <summary>
    * Obtains a description of a sub-unit, as a text string.
    * Type strings:
    *   INPUT(&lt;size&gt;)            - Digital inputs
    *   SWITCH(&lt;size&gt;)           - Uncommitted switches
    *   MUX(&lt;size&gt;)              - Multiplexer, single-channel only
    *   MUXM(&lt;size&gt;)             - Multiplexer, multi-channel
    *   MATRIX(&lt;columns&gt;X&lt;rows&gt;)            - Matrix, LF
    *   MATRIXR(&lt;columns&gt;X&lt;rows&gt;)           - Matrix, RF
    *   DIGITAL(&lt;size&gt;)                     - Digital Outputs
    *   RES(&lt;number of resistors in chain&gt;) - Programmable resistor 
    *   ATTEN(&lt;number of pads&gt;)             - Programmable attenuator
    *   PSUDC(0)                 - DC Power Supply
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Sub-unit number.</param>
    * <param name="Out">Sub-unit function: 0 for INPUT, 1 for OUTPUT.</param>
    * <param name="Str">Pointer to character string to receive the result.</param>
    * <param name="StrLen">Length of character string buffer.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    * <remarks>
    * Some sub-unit types are supported by functions providing more detailed information. These include:
    *   ATTEN - PIPLX_AttenType
    *   PSUDC - PIPLX_PsuType
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_SubType(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,LPCHAR Str,DWORD StrLen);

    /**
    * <summary> 
    * Obtains the Pickering driver version code.
    * </summary>
    * <param name="Sid">Handle of current session.</param> 
    * <returns>The Pickering driver version code, multiplied by 100 (i.e. a value of 100 represents version 1.00) or INVALID_RESULT if
    * error occured.
    * .</returns> 
    */
    DWORD PIPLX_API PIPLX_Version(SESSION Sid);

    /************************************************
    * GENERAL PURPOSE OUTPUT & SWITCHING FUNCTIONS *
    ************************************************/

    /**
    * <summary> 
    * Clears (de-energises or sets to logic '0') all outputs of all sub-units of every open Pickering card.
    * </summary>
    * <param name="Sid">Handle of current session.</param> 
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_ClearAll(SESSION Sid);

    /**
    * <summary> 
    * Clears (de-energises or sets to logic '0') all outputs of all sub-units of the specified Pickering card.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param> 
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_ClearCard(SESSION Sid,DWORD CardNum);

    /**
    * <summary> 
    * Clears (de-energises or sets to logic '0') all outputs of a sub-unit.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param> 
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_ClearSub(SESSION Sid,DWORD CardNum,DWORD OutSub);

    /**
    * <summary> 
    * Operate a single output channel or bit.
    * Note that in the case of a single-channel multiplexer (MUX type) any existing channel closure will be cleared
    * automatically prior to selecting the new channel.
    * Note that PIPLX_OpCrosspoint allows more straightforward use of row/column co-ordinates with matrix sub-units.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="BitNum">Output bit number.</param>
    * <param name="Action">1 to energise, 0 to de-energise.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_OpBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL Action);

    /**
    * <summary> 
    * Obtains the state of an individual output.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="BitNum">Output bit number.</param>
    * <param name="State">Pointer to variable to receive the result (0 = OFF or logic '0', 1 = ON or logic '1').</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_ViewBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL *State);

    /**
    * <summary> 
    * Obtains the state of all outputs of a sub-unit. The result fills the number of least significant bits
    * corresponding to the size of the sub-unit.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Data">Pointer to the one-dimensional array (vector) to receive the result.</param>
    * <param name="DataLen">Size of one-dimensional array to receive result. Count of DWORDs.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    * <remarks>
    * The data array pointed to must contain sufficient bits to hold the bit-pattern for the specified sub-unit,
    * or adjacent memory will be overwritten, causing data corruption and/or a program crash.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_ViewSub(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Data,DWORD DataLen);

    /**
    * <summary> 
    * Sets all outputs of a sub-unit to the supplied bit-pattern. The number of least significant bits corresponding
    * to the size of the sub-unit are written.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Data">Pointer to the one-dimensional array (vector) containing the bit-pattern to be written.</param>
    * <param name="DataLen">Size of one-dimensional array containing data.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    * <remarks>
    * The data array pointed to must contain sufficient bits to represent the bit-pattern for the specified sub-unit,
    * or undefined data will be written to the more significant bits.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_WriteSub(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Data,DWORD DataLen);

    /**
    * <summary> 
    * Clears a sub-unit's switch mask, enabling operation of all outputs by the PIPLX_OpBit, PIPLX_OpCrosspoint and
    * PIPLX_WriteSub functions.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param> 
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_ClearMask(SESSION Sid,DWORD CardNum,DWORD OutSub);

    /**
    * <summary> 
    * Mask or unmask a single output bit.
    * Masking disables the corresponding switch for the PIPLX_OpBit, PIPLX_OpCrosspoint and PIPLX_WriteSub functions.
    * This facility is particularly useful for matrix sub-units, where it can be used to guard against programming
    * errors that could otherwise result in damage to matrix switches or external circuits. Note that PIPLX_MaskCrosspoint
    * allows more straightforward use of row/column co-ordinates with matrices.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="BitNum">Output bit number.</param>
    * <param name="Action">1 to mask, 0 to unmask.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_MaskBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL Action);

    /**
    * <summary> 
    * Mask or unmask a single matrix crosspoint.
    * Masking disables the corresponding switch for the PIPLX_OpBit, PIPLX_OpCrosspoint and PIPLX_WriteSub functions.
    * This facility can be used to guard against programming errors that could otherwise result in damage to matrix
    * switches or external circuits.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Row">Crosspoint row (Y) location.</param>
    * <param name="Column">Crosspoint column (X) location.</param>
    * <param name="Action">1 to mask, 0 to unmask.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * This function supports matrix operation using row/column co-ordinates in place of the linearized bit-number
    * method employed by PIPLX_MaskBit. It offers more straightforward matrix operation, and avoids the need for
    * re-coding if a matrix card is replaced by one having different dimensions.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_MaskCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Column,BOOL Action);

    /**
    * <summary> 
    * Operate a single matrix crosspoint.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Row">Crosspoint row (Y) location.</param>
    * <param name="Column">Crosspoint column (X) location.</param>
    * <param name="Action">1 to energize, 0 to de-energize.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * This function supports matrix operation using row/column co-ordinates in place of the linearized bit-number
    * method employed by PIPLX_OpBit. It offers more straightforward matrix operation, and avoids the need for
    * re-coding if a matrix card is replaced by one having different dimensions.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_OpCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Column,BOOL Action);

    /**
    * <summary> 
    * This function obtains, and optionally sets, the state of a switch. It allows explicit access to the individual
    * switches making up a sub-unit, in types where their operation is normally handled automatically by the driver.
    * The main purpose of this is in implementing fault diagnostic programs for such types; it can also be used where
    * normal automated behaviour does not suit an application.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="SwitchFunc">Code indicating the functional group of the switch, see below.</param>
    * <param name="SegNum">The segment location of the switch.</param>
    * <param name="SwitchNum">The number of the switch in its functional group (unity-based).</param>
    * <param name="SubSwitch">The number of the subswitch to operate (unity-based).</param>
    * <param name="SwitchAction">Code indicating the action to be performed, see below.</param>
    * <param name="State">Pointer to variable to receive the state of the switch (after performing any action).</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    * <remarks>
    * Applicable sub-unit types:
    * This function is only usable with matrix sub-units having auto-isolation and/or auto-loopthru features.
    * For further information see: segmented matrix, unsegmented matrix.
    *
    * SwitchFunc value (a value indicating the functional group of the switch to be accessed):
    *   0 - SW_FUNC_CHANNEL    - A channel (matrix crosspoint) switch
    *   1 - SW_FUNC_X_ISO      - A matrix X-isolation switch
    *   2 - SW_FUNC_Y_ISO      - A matrix Y-isolation switch 
    *   3 - SW_FUNC_X_LOOPTHRU - A matrix X-loopthru switch
    *   4 - SW_FUNC_Y_LOOPTHRU - A matrix Y-loopthru switch
    *
    * SegNum value:
    *   The segment location of the switch. The numbers and sizes of segments on each matrix axis can be obtained using PIPLX_SubAttribute.
    *
    * In an unsegmented matrix, use SegNum = 1.
    * In a segmented matrix, segment numbers for crosspoint and isolation switches are determined logically.
    *
    * SwitchNum value:
    *   The number of the switch in its functional group (unity-based). 
    * 
    * For channel (crosspoint) switches, the switch number can be either:
    *   if SegNum is zero, the global channel number of the switch (see output bit number)
    *   if SegNum is non-zero, the segment-local number of the switch, calculated in a similar way to the above
    *
    * SubSwitch value:
    *   The number of the subswitch to operate (unity-based). This parameter caters for a situation in which a logical
    *   channel, isolation or loopthru switch is served by more than one physical relay (as for example when 2-pole
    *   operation is implemented using independently-driven single-pole relays).
    *
    * The numbers of subswitches for each functional group can be obtained using PIPLX_SubAttribute.
    *
    * A code indicating the action to be performed.
    *   0 - SW_ACT_NONE  - No switch change, just set State result
    *   1 - SW_ACT_OPEN  - Open switch
    *   2 - SW_ACT_CLOSE - Close switch
    *
    * Loopthru switches
    *   Loopthru switches are initialised by the driver to a closed state, which may mean that they are either
    *   energised or de-energised depending upon their type. In normal automated operation loopthru switches open
    *   when any crosspoint on their associated line is closed. Actions SW_ACT_CLOSE and SW_ACT_OPEN close or open
    *   loopthru switch contacts as their names imply.
    * 
    * Operational considerations
    *   This function can be used to alter a pre-existing switch state in a sub-unit, set up by fuctions such as
    *   PIPLX_OpBit or PIPLX_WriteSub. However once the state of any switch is changed by PIPLX_OpSwitch the logical
    *   state of the sub-unit is considered to have been destroyed. This condition is flagged in the result of
    *   PIPLX_SubStatus (bit STAT_CORRUPTED). Subsequent attempts to operate it using 'ordinary' switch functions
    *   such as PIPLX_OpBit, PIPLX_ViewBit etc. will fail (result ER_STATE_CORRUPT). Normal operation can be restored
    *   by clearing the sub-unit using PIPLX_ClearSub, PIPLX_ClearCard or PIPLX_ClearAll.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_OpSwitch(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD SwitchFunc,DWORD SegNum,DWORD SwitchNum,
        DWORD SubSwitch,DWORD SwitchAction,BOOL *State);

    /**
    * <summary> 
    * Obtains the state of an individual matrix crosspoint.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Row">Crosspoint row (Y) location.</param>
    * <param name="Column">Crosspoint column (X) location.</param>
    * <param name="State">Pointer to variable to receive the result (0 = OFF, 1 = ON).</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * This function supports matrix operation using row/column co-ordinates in place of the linearized bit-number
    * method employed by PIPLX_ViewBit. It offers more straightforward matrix operation, and avoids the need for
    * re-coding if a matrix card is replaced by one having different dimensions.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_ViewCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Column,BOOL *State);

    /**
    * <summary> 
    * Obtains the switch mask of a sub-unit. The result fills the number of least significant bits corresponding to
    * the size of the sub-unit.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Data">Pointer to the one-dimensional array (vector) to receive the result.</param>
    * <param name="DataLen">Size of one-dimensional array for receive a data.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * For a Matrix sub-unit, the result is folded into the vector on its row-axis: see Data Formats.
    * The data array pointed to must contain sufficient bits to hold the bit-pattern for the specified sub-unit,
    * or adjacent memory will be overwritten, causing data corruption and/or a program crash.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_ViewMask(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Data,DWORD DataLen);

    /**
    * <summary> 
    * Obtains the state of an individual output's mask.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="BitNum">Output bit number.</param>
    * <param name="State">Pointer to variable to receive the result (0 = unmasked, 1 = masked).</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_ViewMaskBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL *State);

    /**
    * <summary> 
    * Obtains the state of an individual matrix crosspoint's mask.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Row">Crosspoint row (Y) location.</param>
    * <param name="Column">Crosspoint column (X) location.</param>
    * <param name="State">Pointer to variable to receive the result (0 = unmasked, 1 = masked).</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * This function supports matrix operation using row/column co-ordinates in place of the linearized bit-number
    * method employed by PIPLX_ViewMaskBit. It offers more straightforward matrix operation, and avoids the need for
    * re-coding if a matrix card is replaced by one having different dimensions.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_ViewMaskCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Column,BOOL *State);

    /**
    * <summary> 
    * Sets a sub-unit's switch mask to the supplied bit-pattern. The number of least significant bits corresponding
    * to the size of the sub-unit are written into the mask. A '1' bit in the mask disables the corresponding switch
    * for the PIPLX_OpBit, PIPLX_OpCrosspoint, PIPLX_WriteSub and PIPLX_WriteSubArray functions.
    * This facility is particularly useful for matrix sub-units, where it can be used to guard against programming
    * errors that could otherwise result in damage to matrix switches or external circuits.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Data">Pointer to the one-dimensional array (vector) containing the bit-pattern to be written.</param>
    * <param name="DataLen">Size of one-dimensional array containing data.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * The data array pointed to must contain sufficient bits to represent the mask pattern for the specified sub-unit,
    * or undefined data will be written to the more significant bits.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_WriteMask(SESSION Sid,DWORD CardNum,DWORD  OutSub,DWORD *Data,DWORD DataLen);

    /***************************************
    * INPUT
    ***************************************/

    /**
    * <summary> 
    * Obtains the state of an individual input.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="InSub">Input sub-unit number.</param>
    * <param name="BitNum">Input bit number.</param>
    * <param name="State">Pointer to variable to receive the result (0 = logic '0', 1 = logic '1').</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    */
    DWORD PIPLX_API PIPLX_ReadBit(SESSION Sid,DWORD CardNum,DWORD InSub,DWORD BitNum,BOOL *State);

    /**
    * <summary> 
    * Obtains the current state of all inputs of a sub-unit.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="InSub">Input sub-unit number.</param>
    * <param name="Data">Pointer to variable to receive result.</param>
    * <returns>Zero for success, or no-zero error code.</returns> 
    * <remarks>
    * Usable only for sub-units lower or equal to 32 bits. 
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_ReadSub(SESSION Sid,DWORD CardNum,DWORD InSub,DWORD *Data);

    /**
    * <summary> 
    * Obtains the current state of all inputs of a sub-unit.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="InSub">Input sub-unit number.</param>
    * <param name="Data">Pointer to variable to receive result.</param>
    * <param name="Data">Pointer to the one-dimensional array (vector) containing the sub-unit data.</param>
    * <param name="DataLen">Size of one-dimensional array containing data.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_ReadInputSub(SESSION Sid,DWORD CardNum,DWORD InSub,DWORD *Data,DWORD DataLen);

    /***************************************
    * CALIBRATION
    ***************************************/

    /**
    * <summary> 
    * Reads a 16-bit calibration value from on-card EEPROM.
    * The driver places no interpretation on the value obtained - an application program can utilise it in any way
    * it wishes.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Idx">Calibration value index.</param>
    * <param name="Data">Pointer to variable to receive result.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * This feature is only available in association with particular sub-unit types.
    * For programmable resistors, the valid range of Idx values corresponds to the number of bits, i.e. to the
    * range of output bit number values. A 16-bit resistor sub-unit therefore supports 16 x 16-bit values.
    * The storage capacity of other types supporting this feature is determined by their functionality.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_ReadCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Idx,DWORD *Data);

    /**
    * <summary> 
    * Writes a 16-bit calibration value to on-card EEPROM.
    * The driver places no interpretation on the value written - an application program can utilise it in any way it
    * wishes. 
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Output sub-unit number.</param>
    * <param name="Idx">Calibration value index.</param>
    * <param name="Data">Value to be written.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * This feature is only available in association with particular sub-unit types.
    * For programmable resistors, the valid range of Idx values corresponds to the number of bits, i.e. to the
    * range of output bit number values. A 16-bit resistor sub-unit therefore supports 16 x 16-bit values.
    * The storage capacity of other types supporting this feature is determined by their functionality.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_WriteCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Idx,DWORD Data);

    /* ********************************************************************************
    * Floating-point calibration data functions.
    * ********************************************************************************/

    /**
    * <summary>Store floating-point calibration value(s) in EEPROM</summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Pickering card reference of target (unity-based).</param>
    * <param name="OutSub">Sub-unit of target to access (unity-based).</param>
    * <param name="Store">
    *   Value indicating which store to access defined by CalStoreCodes enumerator
    *   (user = CAL_STORE_USER, factory = CAL_STORE_FACTORY).
    * </param>
    * <param name="Offset">Offset in the sub-unit's calibration table at which to start (zero-based).</param>
    * <param name="NumValues">The number of calibration values to write.</param>
    * <param name="Data">Pointer to array of calibration values.</param>
    * <returns>0 = success; Non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_WriteCalFP(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD Offset,DWORD NumValues,double *Data);

    /**
    * <summary>Read floating-point calibration value(s) from EEPROM</summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Pickering card reference of target (unity-based).</param>
    * <param name="OutSub">Sub-unit of target to access (unity-based).</param>
    * <param name="Store">Value indicating which store to access (user = 0, factory = 1).</param>
    * <param name="Offset">Offset in the sub-unit's calibration table at which to start (zero-based).</param>
    * <param name="NumValues">The number of calibration values to read.</param>
    * <param name="Data">Pointer to array to receive calibration values.</param>
    * <returns>0 = success; non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_ReadCalFP(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD Offset,DWORD NumValues,double *Data);

    /**
    * <summary>
    *   Write calibration date information to EEPROM. Date information is obtained from the current system date.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Pickering card reference of target (unity-based).</param>
    * <param name="OutSub">Sub-unit of target to access (unity-based).</param>
    * <param name="Store">Value indicating which store to access (user = 0, factory = 1).</param>
    * <param name="Interval">The desired calibration interval (in days).</param>
    * <returns>0 = success; non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_WriteCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD Interval);

    /**
    * <summary>Read calibration date information from EEPROM.</summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Pickering card reference of target (unity-based).</param>
    * <param name="OutSub">Sub-unit of target to access (unity-based).</param>
    * <param name="Store">Value indicating which store to access (user = 0, factory = 1).</param>
    * <param name="Year">Pointer to variable to receive year of calibration.</param>
    * <param name="Day">Pointer to variable to receive day in year of calibration.</param>
    * <param name="Interval">Pointer to variable to receive calibration interval (in days).</param>
    * <returns>0 = success; non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_ReadCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD *Year,DWORD *Day,DWORD *Interval);

    /**
    * <summary>Set a calibration point.</summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Pickering card reference of target (unity-based).</param>
    * <param name="OutSub">Sub-unit of target to access (unity-based).</param>
    * <param name="Index">Index number of calibration point.</param>
    * <returns>0 = success; non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_SetCalPoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Index);

    /***************************************
    * Programmable Attenuator
    ***************************************/

    /**
    * <summary> 
    * Obtains the current attenuation setting.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Atten">Pointer to variable to receive the attenuation value, in dB.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_AttenGetAttenuation(SESSION Sid,DWORD CardNum,DWORD SubNum,float *Atten);

    /**
    * <summary> 
    * Obtains a description of an attenuator sub-unit, as numeric values.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="TypeNum">Pointer to variable to receive type code.</param>
    * <param name="NumSteps">Pointer to variable to receive step count.</param>
    * <param name="StepSize">Pointer to variable to receive step size, in dB.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * The description obtained by this function is a logical one; a physical description indicating the number of
    * discrete pads in the attenuator can be obtained using PIPLX_SubInfo.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_AttenInfo(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *TypeNum,DWORD *NumSteps,float *StepSize);

    /**
    * <summary> 
    * Obtains the attenuation value of a numbered pad.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="PadNum">Pad number.</param>
    * <param name="Atten">Pointer to variable to receive the pad's attenuation value, in dB.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * This function facilitates explicit pad selection using PIPLX_OpBit or PIPLX_WriteSub, if the selections made by
    * PIPLX_attenSetAttenuation are not optimal for the application.
    * The number of pads in the sub-unit can be found using PIPLX_SubInfo.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_AttenPadValue(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD PadNum,float *Atten);

    /**
    * <summary> 
    * Sets the attenuation to the specified value. 
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Atten">The attenuation value to set, in dB.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    * <remarks>
    * The combination of pads inserted to achieve the desired attenuation level is determined by the driver for
    * best all-round performance. In some models it may be possible to optimise particular aspects of attenuator
    * performance by setting other pad combinations explicitly using PIPLX_OpBit or PIPLX_WriteSub. The pad value
    * associated with each output channel can be discovered with PIPLX_AttenPadValue.
    * </remarks>
    */
    DWORD PIPLX_API PIPLX_AttenSetAttenuation(SESSION Sid,DWORD CardNum,DWORD SubNum,float Atten);

    /**
    * <summary> 
    * Obtains a description of an attenuator sub-unit, as a text string.
    * The format of the result is "ATTEN(&lt;number of steps&gt;,&lt;step size in dB&gt;)".
    * The description obtained by this function is a logical one; a physical description indicating the number of
    * discrete pads employed in the attenuator can be obtained using PIPLX_SubType.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Str">Pointer to character string to receive the result.</param>
    * <param name="StrLen">Length of character string buffer.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_AttenType(SESSION Sid,DWORD CardNum,DWORD SubNum,LPCHAR Str,DWORD StrLen);

    /***************************************
    * Power Supply
    ***************************************/

    /**
    * <summary> 
    * Enables or disables a power supply's output.
    * This function is usable only with sub-units having the capability PSU_CAP_OUTPUT_CONTROL - see PIPLX_PsuInfo.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="State">1 to enable, 0 to disable output.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_PsuEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL State);

    /**
    * <summary> 
    * Obtains the voltage setting of a power supply sub-unit.
    * The result is the nominal value to which the output has been set, not necessarily the actual voltage being
    * output (which may be affected by device tolerances, current-limit conditions etc.).
    * This function is also usable with fixed-voltage supplies, returning the nominal output voltage.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Voltage">Pointer to variable to receive the output setting, in Volts.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_PsuGetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Voltage);

    /**
    * <summary> 
    * Obtains a description of a power supply sub-unit, as numeric values.
    * Power supply sub-unit type code is: 9 - TYPE_PSUDC (DC power supply)
    * A corresponding enumerated constant is provided in piplx_w32.h
    *
    * Capability flag bit definitions:
    *    0x00000010 - PSU_CAP_CURRENT_MODE_SENSE (can sense if operating in current-limited mode)
    *    0x00000008 - PSU_CAP_PROG_CURRENT (output current is programmable)
    *    0x00000004 - PSU_CAP_PROG_VOLTAGE (output voltage is programmable)
    *    0x00000002 - PSU_CAP_OUTPUT_SENSE (has logic-level sensing of output active state)
    *    0x00000001 - PSU_CAP_OUTPUT_CONTROL (has output on/off control)
    * 
    * Certain driver functions are only usable with sub-units having appropriate capabilities - examples being:
    *    PIPLX_PsuEnable
    *    PIPLX_PsuSetVoltage
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="TypeNum">Pointer to variable to receive type code.</param>
    * <param name="Voltage">Pointer to variable to receive rated voltage (in Volts).</param>
    * <param name="Current">Pointer to variable to receive rated current (in Amps).</param>
    * <param name="Precis">Pointer to variable to receive precision (in bits, meaningful only for programmable supplies).</param>
    * <param name="Capabilities">Pointer to variable to receive capability flags.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_PsuInfo(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *TypeNum,double *Voltage,double *Current,
        DWORD *Precis,DWORD *Capabilities);

    /**
    * <summary> 
    * Sets the output voltage of a power supply sub-unit to the specified value.
    * The voltage value specified is rounded to the precision of the supply's DAC. The actual voltage setting can be obtained
    * using PIPLX_PsuGetVoltage.
    * This function is usable only with sub-units having the capability PSU_CAP_PROG_VOLTAGE - see PIPLX_PsuInfo.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Voltage">The output voltage to set, in Volts.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_PsuSetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double Voltage);

    /**
    * <summary> 
    * Obtains a description of a power supply sub-unit, as a text string.
    * For a DC power supply the format of the result is "PSUDC(&lt;rated voltage&gt;,&lt;rated current&gt;)".
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Str">Pointer to character string to receive the result.</param>
    * <param name="StrLen">Length of character string buffer.</param>
    * <returns>Zero for success, or no-zero error code.</returns>  
    */
    DWORD PIPLX_API PIPLX_PsuType(SESSION Sid,DWORD CardNum,DWORD SubNum,LPCHAR Str,DWORD StrLen);

    /***************************************
    * Mode Control
    ***************************************/

    /**
    * <summary> 
    * Allows control flags affecting the driver's global behaviour to be set and read. This function gives access to
    * low-level control features of the driver and is intended for 'expert' use only - the default driver
    * behaviour should be satisfactory for the great majority of applications. And it takes affect on whole LXI not only
    * for session.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="ModeFlags">New value for driver mode flags.</param> 
    * <returns>The driver's mode flags prior to executing this function, or INVALID_RESULT if error occured.</returns>  
    */
    DWORD PIPLX_API PIPLX_SetMode(SESSION Sid,DWORD ModeFlags);

    /************************************************
    * Programmable resistor specific functions.
    ************************************************/

    /**
    * <summary>
    *   Sets a programmable resistor to the closest available setting to the value specified.
    *   This function is only usable with programmable resistor models that support it, such as 40-260-001.
    *   If the sub-unit permits, the resistance value can be set to:
    *   zero ohms (nominally), by passing the resistance value 0.0
    *   infinity, by passing the resistance value HUGE_VAL (#include <math.h>)
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Pickering card reference of target (unity-based).</param>
    * <param name="OutSub">Sub-unit of target to access (unity-based).</param>
    * <param name="Mode">A value indicating how the given resistance value is to be applied.</param>
    * <param name="Resistance">The resistance value.</param>
    * <returns>0 = success; non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_ResSetResistance(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Mode,double Resistance);

    /**
    * <summary>Obtain the set resistance value</summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Pickering card reference of target (unity-based).</param>
    * <param name="OutSub">Sub-unit of target to access (unity-based).</param>
    * <param name="Resistance">Pointer to variable to receive the resistance value.</param>
    * <returns>0 = success; non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_ResGetResistance(SESSION Sid,DWORD CardNum,DWORD OutSub,double *Resistance);

    /**
    * <summary>
    *   Obtains detailed information on a programmable resistor sub-unit.
    *   Capability bits are as follows:
    *     0x00000008 - RES_CAP_REF (supports reference calibration value)
    *     0x00000004 - RES_CAP_INF (supports setting "open-circuit")
    *     0x00000002 - RES_CAP_ZERO (supports setting "zero ohms")
    *     0x00000001 - RES_CAP_PREC (precision resistor - supporting function PIL_ResSetResistance etc.)
    *     0x00000000 - RES_CAP_NONE (no special capabilities)
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="OutSub">Sub-unit number.</param>
    * <param name="MinRes">Pointer to variable to receive minimum resistance setting.</param>
    * <param name="MaxRes">Pointer to variable to receive maximum resistance setting.</param>
    * <param name="RefRes">Pointer to variable to receive reference resistance value.</param>
    * <param name="PrecPC">Pointer to variable to receive percentage precision value.</param>
    * <param name="PrecDelta">Pointer to variable to receive offset precision, in ohms.</param>
    * <param name="Int1">Pointer to (currently unused) variable.</param>
    * <param name="IntDelta">Pointer to variable to receive internal precision, in ohms.</param>
    * <param name="Capabilities">Pointer to variable to receive capability flags (see below).</param>
    * <remarks>
    *   MinRes and MaxRes are the minimum and maximum values that can be set in the sub-unit's continuous
    *   range of adjustment. If capability RES_CAP_ZERO is flagged a setting of "zero ohms" is also possible.
    *   If RES_CAP_INF is flagged an open-circuit setting is also possible.
    *
    *   If capability RES_CAP_REF is flagged, RefRes is the reference resistance value -  such as in model
    *   40-265, where it gives the balanced state resistance.
    *
    *   PrecPC and PrecDelta representt  he sub-unit's precision specification, such as (±0.2%, ±0.1 ohms).
    *
    *   IntDelta is the notional precision to which the sub-unit works internally; this value will be less than or
    *   equal to the figure indicated by PrecPC and PrecDelta, indicating greater internal precision.
    *
    *   Where information is not available for the sub-unit concerned, null values are returned.
    * </remarks>
    * <returns>0 = success; non-zero = error code</returns>
    */
    DWORD PIPLX_API PIPLX_ResInfo(SESSION Sid,DWORD CardNum,DWORD OutSub,double *MinRes,double *MaxRes,
        double *RefRes,double *PrecPC,double *PrecDelta,double *Int1,
        double *IntDelta,DWORD *Capabilities);

    /****************************************
    * Battery simulator functions
    ****************************************/

    /**
    * <summary>
    *   Obtains the current sink setting of a battery simulator (BATT type) sub-unit.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Current">Pointer to variable to receive the output setting, in Amps.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_BattGetCurrent(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Current);

    /**
    * <summary>
    *   Obtains the output enable pattern of battery simulator (BATT type) sub-units.
    *
    *   Notes
    *   When SubNum corresponds to a BATT sub-unit, the function gets the output enable state
    *   of that sub-unit alone in the least significant bit of Pattern (0 = OFF, 1 = ON).
    *   If SubNum = 0 (BATT_ALL_BATT_SUB_UNITS), enable states of all the card's BATT sub-units are obtained;
    *   bits in Pattern are assigned in ascending order of BATT sub-unit, i.e.
    *   Pattern bit 0 = enable state of lowest numbered BATT sub-unit (0 = OFF, 1 = ON)
    *   Pattern bit 1 = enable state of next numbered BATT sub-unit (0 = OFF, 1 = ON)
    *   etc.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Pattern">Pointer to variable to receive the output enable pattern.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_BattGetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *Pattern);

    /**
    * <summary>
    *   Obtains the voltage setting of a battery simulator (BATT type) sub-unit.
    *
    *   Notes
    *   The result is the nominal value to which the output has been set, not necessarily the
    *   actual voltage being output (which could be affected by conditions such as current-limiting).
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Voltage">Pointer to variable to receive the output setting, in Volts.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_BattGetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Voltage);

    /**
    * <summary>
    *   Obtains the present state of a hardware interlock associated with battery simulator (BATT type) sub-units.
    *   
    *   Notes
    *   When SubNum corresponds to a BATT sub-unit, the function gets the state of the hardware interlock
    *   associated with that sub-unit:
    *   
    *   0 = interlock is "down"
    *   1 = interlock is "up"
    *   
    *   If SubNum = 0 (BATT_ALL_BATT_SUB_UNITS), the function gets the summary state of all BATT sub-unit interlocks :
    *   
    *   0 = one or more interlocks is "down"
    *   1 = all interlocks are "up"
    *   
    *   Model 41-752-001 has a single global interlock affecting all channels, and both modes above yield the same result.
    *   
    *   Interlock "up" state is hardware-latched from the physical wired interlock by the action of PIPLX_BattSetEnable,
    *   when that function succeeds. Hence:
    *   If the "up" state is indicated, the physical interlock has remained intact and outputs are enabled as
    *   previously set by PIPLX_BattSetEnable.
    *   If the "down" state is indicated, the physical interlock has been broken and all outputs will have been
    *   disabled automatically through hardware.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Interlock">Pointer to variable to receive the interlock state.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_BattReadInterlockState(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL *Interlock);

    /**
    * <summary>
    *   Sets the output sink current of battery simulator (BATT type) sub-units.
    *   
    *   Notes
    *   When SubNum corresponds to a BATT sub-unit, the function sets the sink current of that sub-unit alone.
    *   
    *   If SubNum = 0 (BATT_ALL_BATT_SUB_UNITS), all of the card's BATT sub-units are set to the given current.
    *   
    *   For non-zero values, output sink current is set to the nearest available value greater than that specified,
    *   typically using a low-precision DAC (e.g. 4-bit). The actual sink current setting can be obtained using PIPLX_BattGetCurrent.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Current">The output sink current to set, in Amps.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_BattSetCurrent(SESSION Sid,DWORD CardNum,DWORD SubNum,double Current);

    /**
    * <summary>
    *   Sets the output enable pattern of battery simulator (BATT type) sub-units.
    *   Notes
    *   When SubNum corresponds to a BATT sub-unit, the function sets the output enable state of that
    *   sub-unit alone according to the least significant bit of Pattern (0 = OFF, 1 = ON).
    *   
    *   If SubNum = 0 (BATT_ALL_BATT_SUB_UNITS), enable states of all the card's BATT sub-units are set;
    *   bits in the supplied Pattern are utilised in ascending order of BATT sub-unit, i.e.
    *   
    *   Pattern bit 0 = enable state of lowest numbered BATT sub-unit (0 = OFF, 1 = ON)
    *   
    *   Pattern bit 1 = enable state of next numbered BATT sub-unit (0 = OFF, 1 = ON)
    *   etc.
    *   
    *   Note that the operation can fail (returning ER_EXECUTION_FAIL) if a necessary hardware interlock
    *   is disconnected.
    *   The present enable pattern can be obtained using PIPLX_BattGetEnable.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Pattern">The pattern of output enables to set.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_BattSetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD Pattern);

    /**
    * <summary>
    *   Sets the output voltage of battery simulator (BATT type) sub-units.
    *   
    *   Notes
    *   When SubNum corresponds to a BATT sub-unit, the function sets the voltage of that sub-unit alone.
    *   
    *   If SubNum = 0 (BATT_ALL_BATT_SUB_UNITS), all of the card's BATT sub-units are set to the given
    *   voltage.
    *   The voltage value specified is rounded to the precision of the sub-unit's DAC. The actual voltage
    *   setting can be obtained using PIPLX_BattGetVoltage.
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name="Voltage">The output voltage to set, in Volts.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_BattSetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double Voltage);

    /* **************************************************************
    * Voltage Source (VSOURCE type) specific functions.
    * **************************************************************/

    /**
    * <summary>
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name=""></param> 
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_VsourceSetRange(SESSION Sid,DWORD CardNum,DWORD SubNum,double Range);

    /**
    * <summary>
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name=""></param> 
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_VsourceGetRange(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Range);

    /**
    * <summary>
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name=""></param> 
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_VsourceSetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double Voltage);

    /**
    * <summary>
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name=""></param> 
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_VsourceGetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Voltage);

    /**
    * <summary>
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name=""></param> 
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_VsourceSetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD Pattern);

    /**
    * <summary>
    * </summary>
    * <param name="Sid">Handle of current session.</param>
    * <param name="CardNum">Card number.</param>
    * <param name="SubNum">Output sub-unit number.</param>
    * <param name=""></param> 
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_VsourceGetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *Pattern);

	/**
    * <summary>
    * Returns message status of CERN board/cards/selftest
    * </summary>
    * <param name="Sid">Handle of current session.</param>
	* <param name="CardNum">Card number.</param>
    * <param name="StatusMsg">Pointer to variable to receive a status message.</param>
    * <param name="Length">Length of StatusMsg string.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PIPLX_API PIPLX_GetStatusMessage(SESSION Sid, DWORD CardNum, LPCHAR StatusMsg, DWORD Length);


#ifdef __cplusplus
};
#endif

#endif // #ifdef _PIPLX_H_
