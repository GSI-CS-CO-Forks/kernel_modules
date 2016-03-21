/*******************************************************************************
 *  Pickering power sequencer module header file of ClientBridge for controlling
 *  cards in LXI device.
 *
 *  Copyright (C) 1988 - 2012, Pickering Interfaces ltd.
 *   
 *  Support: support@pickeringswitch.com
 *  Version: 1.3.2
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

#ifndef _PIPSLX_H_
#define _PIPSLX_H_

#include "Picmlx.h"

#ifdef WIN32

  #ifdef PIPSLX_EXPORTS
    #define PIPSLX_API __declspec(dllexport) __stdcall
  #else
    #define PIPSLX_API __declspec(dllimport) __stdcall
  #endif // #ifdef PIPSLX_EXPORTS

#elif LINUX  // #ifdef WIN32

  #define PIPSLX_API  

#endif // #elif LINUX

/// \endcond

#define SEQUENCE_START 1    ///< Startup sequence
#define SEQUENCE_STOP 0     ///< Stop sequence

#define TIME_START 1        ///< Startup time 
#define TIME_STOP 0         ///< Stop time

// Power sequencer error codes.
enum PipslxErrorCodes {
  ER_PIPSLX_BASE = 0x00003000,    ///< Do not use, only for internal use
  ER_PIPSLX_INVALID_SESSION,      /* Session ID is invalid => !!! Do not use it anymore !!!
                                     Replaced by ER_PICMLX_INVALID_SESSION. */
  ER_PIPSLX_FUNC_NOT_LOCAL,       ///< Function not supported on localhost.
  ER_PIPSLX_NOT_CONNECTED,        ///< You are not connected to remote host.
  ER_PIPSLX_NOT_INIT,             ///< Library wasn't initialized!
  ER_PIPSLX_SWITCH_FAULT,         ///< Switch fault.
  ER_PIPSLX_BAD_FORMAT_DATA,      ///< Data or command format is bad.
  ER_PIPSLX_UNKNOWN_CMD,          ///< Unknown command.
  ER_PIPSLX_BUSY,                 ///< Busy.
  ER_PIPSLX_SEQUPINPROGRESS,      ///< Up sequence is in progress.
  ER_PIPSLX_SEQDOWNINPROGRESS,    ///< Down sequence is in progress.
  ER_PIPSLX_CHANTESTINPROGRESS,   ///< Channel test is running.
  ER_PIPSLX_EMERGENCY_STOP,       ///< Emergency stop button engaged.
  ER_PIPSLX_PARAM_SIZE,           ///< Parametr is NULL or size is invalid.
  ER_PIPSLX_END_MARK              ///< Do not use, only for internal use
};

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef LINUX

  // For backward compability ... now it's not used.
  void PIPSLX_Init();

  // For backward compability ... now it's not used.
  void PIPSLX_Free();

#endif

/**
 * <summary>
 * Returns last occured error code.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="ErrorCode">Pointer to variable to receive a last occured error code if any.</param>
 * <returns>Zero or non zero error code if Session is not valid.</returns>
 */
DWORD PIPSLX_API PIPSLX_GetLastErrorCode(SESSION Sid,DWORD* ErrorCode);

/**
 * <summary>
 * Returns last occured error as a message.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="ErrorMsg">Pointer to an character string to receive a error message.</param>
 * <param name="Length">Length of buffer ErrorMsg.</param>
 * <returns>Zero or non zero error code if Session is not valid.</returns>
 */
DWORD PIPSLX_API PIPSLX_GetLastErrorMessage(SESSION Sid,LPCHAR ErrorMsg,DWORD Length);

/**
 * <summary>
 * Convert error code to a message.
 * </summary>
 * <param name="ErrorCode">Error code.</param> 
 * <param name="ErrorMsg">Pointer to an character string to receive a error message.</param>
 * <param name="Length">Length of buffer ErrorMsg.</param>
 * <returns>Zero or non zero error code if Session is not valid.</returns>
 */
DWORD PIPSLX_API PIPSLX_ErrorCodeToMessage(DWORD ErrorCode,LPCHAR ErrorMsg,DWORD Length);

/**
 * <summary>
 * Returns TRUE if an error occured or FALSE if none.
 * </summary>
 * <param name="SID">Handle of current session.</param>
 */
BOOL PIPSLX_API PIPSLX_IsError(SESSION SID);

/**
 * <summary>
 * Returns a version of library Pipslx_w32.dll
 * </summary> 
 */
DWORD PIPSLX_API PIPSLX_GetVersion(void);

/***************************************
 * POWER SEQUENCER FUNCTIONS
 ***************************************/

/**
 * <summary>
 * Opens power sequencer session for full access mode. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_Open(SESSION SID);

/**
 * <summary>
 * Opens power sequencer session for getting information mode only. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_OpenForRead(SESSION SID);

/**
 * <summary>
 * Closes the power sequencer session. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_Close(SESSION SID);

/**
 * <summary>
 * Gets channel state. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="Chan">Index of channel.</param>
 * <param name="State">State of channel.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_GetChanState(SESSION SID,DWORD Chan,BOOL* State);

/**
 * <summary>
 * Sets channel state. Turn on or turn off a channel. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="Chan">Index of channel.</param>
 * <param name="State">State of channel.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_SetChanState(SESSION SID,DWORD Chan,BOOL State);

/**
 * <summary>
 * Gets a down sequence or up sequence time for a channel. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="Chan">Index of channel.</param>
 * <param name="TimeType">TimeType = 0 for down sequence, 1 for up sequence</param>
 * <param name="Time">Time delay</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_GetChanTime(SESSION SID,DWORD Chan,DWORD TimeType,DWORD *Time);

/**
 * <summary>
 * Sets a down sequence or up sequence time for a channel. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="Chan">Index of channel.</param>
 * <param name="TimeType">TimeType = 0 down sequence, 1 for up sequence</param>
 * <param name="Time">Time delay</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_SetChanTime(SESSION SID,DWORD Chan,DWORD TimeType,DWORD Time);

/**
 * <summary>
 * Retrieves information if channel is or isn't in sequence.
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="Chan">Index of channel.</param>
 * <param name="Enabled">True = channel is in sequence, false isn't.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_GetChanEnabled(SESSION SID,DWORD Chan,BOOL *Enabled);

/**
 * <summary>
 * Puts or removes channel in/from sequence.
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="Chan">Index of channel.</param>
 * <param name="Enabled">True = channel will be in sequence, false it won't.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_SetChanEnabled(SESSION SID,DWORD Chan,BOOL Enabled);

/**
 * <summary>
 * Testing all channels. Return value is an array of channel indexes which have a hardware problem. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="BadChans">Pointer to the one-dimensional array (vector) to receive the result of bad channels.</param>
 * <param name="BadChansLen">Length of one-dimensional array which is allocated, output value is bad channels count.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_ChanTest(SESSION SID,DWORD* BadChans,DWORD* BadChansLen);

/**
 * <summary>
 * Start a up or down sequence. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="SequenceType">SequenceType = 0 down sequence, 1 for up sequence</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_Sequence(SESSION SID,DWORD SequenceType);

/**
 * <summary>
 * Get a relay settling time for all relayes. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="Time">Settling time of relays</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_SettleTime(SESSION SID,DWORD* Time);

/**
 * <summary>
 * Emergency down power sequencer without sequence times. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_Shutdown(SESSION SID);

/**
 * <summary>
 * Get a count of power sequencer channels. 
 * </summary>
 * <param name="SID">Handle of current session.</param>
 * <param name="ChanCount">Channels present in power sequencer</param>
 * <returns>Zero for success or non-zero error code.</returns>
 */
DWORD PIPSLX_API PIPSLX_GetChanCount(SESSION SID,DWORD* ChanCount);

#ifdef __cplusplus
}
#endif

#endif // #define _PIPSLX_H_
