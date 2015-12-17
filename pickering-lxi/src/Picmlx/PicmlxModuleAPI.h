/*******************************************************************************
*  Pickering BIRST API header file.
*	
*  Copyright (C) 1988 - 2011, Pickering Interfaces ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _PICMLXMODULEAPI_H_
#define _PICMLXMODULEAPI_H_

/// \cond

#include <System.h>
#include <Picmlx.h>
#include <Session.h>

#ifdef __cplusplus
extern "C" {
#endif

    enum option_name_e {
        ON_RECVTIMEO,
        ON_SNDTIMEO
    };

    /// \endcond

    /*
    * <summary>
    * Function begins data transmition sequence by functions PICMLX_SendPacket and PICMLX_RecvPacket.
    * </summary>
    * <param name="sid">Handle of current session.</param>
    * <param name="data">Variable to receive information about current session.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_BeginTransmition(SESSION sid,session_data_s *data);

    /*
    * <summary>
    * Function ends data transmition sequence by functions PICMLX_SendPacket and PICMLX_RecvPacket.
    * </summary>
    * <param name="data">Variable with information about current session.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_EndTransmition(const session_data_s *data);

    /*
    */
    DWORD PICMLX_API PICMLX_GetOption(option_name_e option,int *value);

    /*
    * <summary>
    * Function receive command response data packet from server.
    * </summary>
    * <param name="func">Server error codes converting function.</param>
    * <param name="format">
    *  Specify the format of incoming packet data.
    *
    *  Data format for each data type:
    *    c - char
    *    i - int
    *    u - unsigned int
    *    f - float
    *    d - double
    *
    *  Arrays:
    *    a<data type> - array of <data type>
    *
    *  Specials:
    *    R - server result (error code or 0 as success)
    *
    * </param>
    * <param name="...">Data from incoming data packet</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_RecvPacket(convert_func_t *func, LPCCHAR format,...);

    /*
    * <summary>
    * Function send command packet with command data to server.
    * </summary>
    * <param name="command">Command ID of the packet.</param>
    * <param name="format">
    *  Specify the format of outgoing packet data.
    *
    *  Data format for each data type:
    *    c - char
    *    i - int
    *    u - unsigned int
    *    f - float
    *    d - double
    *
    *  Arrays:
    *    a<data type> - array of <data type>
    *
    *  Specials:
    *    R - server result (error code or 0 as success)
    *
    * </param>
    * <param name="...">Data of outgoing data packet</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_SendPacket(DWORD command, LPCCHAR format,...);

    /*
    */
    DWORD PICMLX_API PICMLX_SetOption(option_name_e option,int value);

    /**
    * <summary>
    *   Get information about all sessions and their connection addresses.
    * </summary>
    * <param name="sid">Handle of current session.</param>
    * <param name="sessions">Buffer to put array of sessions in.</param>
    * <param name="addresses">Array of strings containing connection ip addresses for respective sessions.</param>
    * <param name="numSessions">Maximum length of string buffers, returns real number of sessions.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetAllSessions(SESSION sid, DWORD *sessions, CHAR ipaddresses[][16], DWORD *numSessions);

    /**
    * <summary>
    *   Terminates session in server bridge.
    * </summary>
    * <param name="sid">Handle of current session.</param>
    * <param name="session">Number of session to terminate.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_KillSession(SESSION sid,SESSION session);

    /// \cond

#ifdef __cplusplus
};
#endif

/// \endcond

#endif // #ifndef _PICMLXMODULEAPI_H_
