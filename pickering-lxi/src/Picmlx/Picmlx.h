/*******************************************************************************
*  Communication module header file of ClientBridge for communication with
*  LXI device.
*
*  Copyright (C) 1988 - 2013, Pickering Interfaces ltd.
*   
*  Support: support@pickeringswitch.com
*  Version: 1.8.1
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

#ifndef _PICMLX_H_
#define _PICMLX_H_

/// \cond

#if defined(WIN32) || defined(WIN64)

#include <windows.h>

#ifdef PICMLX_EXPORTS
#define PICMLX_API __declspec(dllexport) __stdcall
#else
#define PICMLX_API __declspec(dllimport) __stdcall
#endif // #ifdef PICMLX_EXPORTS

#elif defined(LINUX)  // #ifdef WIN32

#define PICMLX_API
#define NO_ERROR 0
#define TRUE 1
#define FALSE 0

typedef signed long int   BOOL;  
typedef unsigned long int DWORD;  
typedef char			  CHAR;

#endif // #elif LINUX

/// \endcond

#define INVALID_SESSION (~0)  ///< Invalid session value
#define INVALID_RESULT (~0)   ///< Invalid result value

typedef CHAR*	LPCHAR;
typedef const CHAR*	LPCCHAR;
typedef signed long	session_t,SESSION,*LPSESSION;   ///< SID type

/// The LXIENTRY structure contains the information about found LXI device 
typedef struct _device_entry {
    DWORD ListenPort;				///< Communication port of LXI device
    DWORD CardCount;				///< Total count of cards
    DWORD ClientCount;			///< Count of connected clients
    DWORD OpenedCardCount;	///< Total count of openned cards
    CHAR Description[100];	///< Description of LXI device
    CHAR IP[20];						///< IP address of LXI device
} device_entry_t,LXIENTRY,*LPLXIENTRY;

/// Client error codes (includes converted server error codes).
enum PicmlxErrorCodes {
    // Client SIDe error codes
    ER_PICMLX_BASE = 0x00001000,          // Do not use, only for internal use
    ER_PICMLX_CLI_NOT_INIT,               ///< DEPRECATED! Library wasn't initialized!
    ER_PICMLX_CLI_INVALID_SESSION,	      ///< Client: Invalid session ID.  
    ER_PICMLX_CLI_HOST_UNAVAILABLE,       ///< Client: Destination host is unavailable  
    ER_PICMLX_CLI_ECHOB_SERVER,			      ///< DEPRECATED! Client: Can't start echo broadcast server
    ER_PICMLX_CLI_ECHOB_SEND,				      ///< DEPRECATED! Client: Can't send broadcast message
    ER_PICMLX_CLI_ECHOB_OPTION,			      ///< DEPRECATED! Client: Can't set/get echo server socket option
    ER_PICMLX_CLI_ECHOB_CREATE,			      ///< DEPRECATED! Client: Can't create echo server listen thread
    ER_PICMLX_CLI_ECHOD_DATA,				      ///< DEPRECATED! Client: Received corrupted data in EchoDirect function
    ER_PICMLX_CLI_ECHOD_RECV,				      ///< DEPRECATED! Client: Can't receive data in EchoDirect function
    ER_PICMLX_CLI_ECHOD_CREATE,			      ///< DEPRECATED! Client: Can't create echo direct socket
    ER_PICMLX_CLI_ECHOD_HOST,				      ///< DEPRECATED! Client: Echo direct request timeout
    ER_PICMLX_CLI_SESSION,				        ///< DEPRECATED! Client: SID can not be NULL!
    ER_PICMLX_CLI_CONNECT,					      ///< Client: Can't connect to remote host
	ER_PICMLX_CLI_CONNECT_REFUSED,				///< Client: Connection refused
	ER_PICMLX_CLI_NAME_NOT_KNOWN,         ///< Client: Name not known
    ER_PICMLX_CLI_RECV_FAILED,			      ///< Client: Can't receive data
    ER_PICMLX_CLI_RECV_DATA,				      ///< Client: Received corrputed data
    ER_PICMLX_CLI_SEND_FAILED,			      ///< Client: Can't send data
    ER_PICMLX_CLI_FUNC_NOT_LOCAL,         ///< Client: function is not supported on local system
    ER_PICMLX_CLI_NOT_CONNECTED,          ///< Client: not connected  
    ER_PICMLX_CLI_PARAM_SIZE,             ///< Client: parametr is NULL or size is invalid
	ER_PICMLX_CLI_DATA_NOT_ACCEPT,         ///< Client: Invalid parameter value
	    // Server SIDe error codes
    ER_PICMLX_SRV_INVALID_FID,			      ///< Server: Invalid function ID
    ER_PICMLX_SRV_RPC_DATA,					      ///< Server: RPC data are corrupted
    ER_PICMLX_SRV_INVALID_CARDID,		      ///< Server: Invalid card ID
    ER_PICMLX_SRV_INVALID_ADDRESS,	      ///< Server: Address of a card is invalid
    ER_PICMLX_SRV_OPEN_CARD,				      ///< Server: The card is used
    ER_PICMLX_SRV_ACCESS_DENIED,		      ///< Server: You haven't any rights to the card(s)
    ER_PICMLX_SRV_NOT_SUPPORTED,		      ///< Server: Function is not supported by this LXI device
    ER_PICMLX_SRV_DEVICE_INFO,			      ///< Server: Information about LXI device is not obtainable or is not all
    ER_PICMLX_SRV_OUT_OF_RESOURCES,	      ///< Server: Out of reasources (probably memory).
    ER_PICMLX_SRV_DATA_LENGTH,			      ///< Server: Required length of data is too long
    ER_PICMLX_SRV_CARD_TYPE,				      ///< Server: LXI device has not card this type
    ER_PICMLX_SRV_FUNC_NOT_IMPL,		      ///< Server: Function is not impelented or it is reserved
    ER_PICMLX_SRV_CARD_USED_BYCLI,	      ///< Server: The card is already used by the client
    ER_PICMLX_SRV_UNDEFINED,				      ///< Server: Undefined error
    ER_PICMLX_SRV_WAIT_TIMEOUT,			      ///< Server: Wait timeout expired
    ER_PICMLX_SRV_DEVICE_LOCKED,	        ///< Server: Device is locked - another client has exclusive access	
    // Client SIDe error code
    ER_PICMLX_CLI_BONJOUR_FAILED,         ///< DEPRECATED! Client: DNSService call failed.
    ER_PICMLX_CLI_BEGINTRANS,             ///< Client: Transmition wasn't initiated. First call PICMLX_BeginTransmition!
    ER_PICMLX_CLI_ENDTRANS,               ///< Client: Previous transmition wasn't ended. Call PICMLX_EndTransmition!
    ER_PICMLX_CLI_INVALID_INDEX,          ///< Client: Invalid list item index.
    ER_PICMLX_CLI_DIRECTECHO_FAILED,      ///< Client: Direct discovery function failed.
    ER_PICMLX_CLI_BROADCASTECHO_FAILED,   ///< Client: Broadcast discovery function failed.
    ER_PICMLX_SRV_INVALID_SESSION,        ///< Server: Invalid session
    ER_PICMLX_SRV_INVALID_TOKEN,	        ///< Server: Invalid token
	ER_PICMLX_SRV_INVALID_EC,				///< Server: Invalid error code 
	ER_PICMLX_SRV_FATAL_01,					///< Server: Fatal error - 1
    ER_PICMLX_CLI_TIMEOUT_FAIL,             ///< Client: Can not set recv/send socket timeout.
    ER_PICMLX_END_MARK                      // Do not use, only for internal use
};

/**
* <summary>Supported types of card.</summary> 
*/
enum CardTypes {
    CARD_PIC,   ///< Pickering standart switching cards
    CARD_PS,    ///< Pickering Power Sequencer card
    CARD_743    ///< Pickering 743 power supply card
};

enum OwnerTypes {
    CLIENT_SESSION			= 1,
    CLIENT_SHARED_SESSIONS	= 2,
    OTHER_SESSIONS			= 4,
};

enum AccessTypes {
    MULTIUSER_ACCESS		= 1,
    EXCLUSIVE_ACCESS		= 2, // if someone else is using the card the error code access denied is returned
    FORCE_EXCLUSIVE_ACCESS	= 3 // if more clients use card and no one of exclusive access then requirement will be forced and client gets exclusive access.
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LINUX

    // For backward compability ... now it's not used.
    void PICMLX_Init();

    // For backward compability ... now it's not used.
    void PICMLX_Free();

#endif

    /********************
    * VERSION FUNCTION *
    ********************/

    /**
    * <summary>
    *   Returns version of Picmlx.dll library.
    * </summary> 
    * <returns>Version of Picmlx.dll library.</returns>
    */
    DWORD PICMLX_API PICMLX_GetVersion(void);

    /************************
    * CONNECTION FUNCTIONS *
    ************************/

    /**
    * <summary>
    *   Connect to an LXI device and establish communication.
    * </summary>
    * <param name="Board">Reserved for future use.</param>
    * <param name="Address">
    *   IP address or name of LXI. IP Address, name or "localhost" and "127.0.0.1"
    *   will be alwayes trying to connect to ServerBridge. If you set it up to NULL or "PXI",
    *   than ClientBridge use an Pilpxi.dll functions => connection to PXI.
    * </param>
    * <param name="Port">Connection port of LXI.</param>
    * <param name="Timeout">Connection timeout in milliseconds.</param>
    * <param name="SID">Pointer to variable to receive valid handle of session.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_Connect(DWORD Board,const LPCHAR Address,DWORD Port,DWORD Timeout,LPSESSION SID);

    /**
    * <summary>
    *   Disconnect from LXI.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_Disconnect(SESSION SID);

    /****************************
    * ERROR HANDLING FUNCTIONS *
    ****************************/

    /**
    * <summary>
    *   Returns TRUE if an error occured or FALSE if none.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <returns>TRUE if an error occured or FALSE if none.</returns>
    */
    BOOL PICMLX_API PICMLX_IsError(SESSION SID);

    /**
    * <summary>
    *   Returns last error code.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="ErrorCode">Pointer to variable to receive last error code.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_GetLastErrorCode(SESSION SID,DWORD *ErrorCode);

    /**
    * <summary>
    *   Returns last error as a message.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="ErrorMsg">Pointer to variable to receive the null-terminated string containing the error message.</param>
    * <param name="Length">Maximum size of the buffer specified by the 'ErrorMsg' parameter.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_GetLastErrorMessage(SESSION SID,LPCHAR ErrorMsg,DWORD Length);

    /**
    * <summary>
    *   Convert error code to a message. 
    * </summary>
    * <param name="ErrorCode">Error code.</param>
    * <param name="ErrorMsg">Pointer to variable to receive the null-terminated string containing the error message.</param>
    * <param name="Length">Maximum size of the buffer specified by the 'ErrorMsg' parameter.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_ErrorCodeToMessage(DWORD ErrorCode,LPCHAR ErrorMsg,DWORD Length);

    /***********************
    * DISCOVERY FUNCTIONS *
    ***********************/

    /**
    * <summary>
    *   Send a direct inquiry to a LXI for getting information. If LXI is available, then you can get information
    *   about LXI. If LXI isn't available (cannot ping a target) return value is a 
    *   ER_PICMLX_CLI_PING_TIMEOUT.
    * </summary>
    * <param name="Address">IP address or name of LXI.</param>
    * <param name="Port">Connection port of LXI.</param>
    * <param name="Timeout">Connection timeout in milliseconds.</param>
    * <param name="LXIInfo">Pointer to variable to receive information about LXI. May be null.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_EchoDirect(LPCHAR Address,DWORD Port,DWORD Timeout,LPLXIENTRY LXIInfo);

    /**
    * <summary>
    *   Send a direct inquiry to a LXI for getting information. If LXI is available, then you can get information
    *   about LXI (return value is zero). If LXI isn't available (cannot ping a target) return value is a 
    *   ER_PICMLX_CLI_PING_TIMEOUT.
    * </summary>
    * <param name="Address">IP address or name of LXI.</param>
    * <param name="Port">Connection port of LXI.</param>
    * <param name="Timeout">Connection timeout in milliseconds.</param>
    * <param name="ListenPort">Pointer to variable to receive a server bridge listen port for communication.</param>
    * <param name="CardCount">Pointer to variable to receive an available cards count.</param>
    * <param name="ClientCount">Pointer to variable to receive a connected clients count.</param>
    * <param name="OpenCardCount">Pointer to variable to receive an openned cards count.</param>
    * <param name="Description">Pointer to variable to receive an information string.</param>
    * <param name="DescLength">Length of Description string.</param>
    * <param name="LXIAddress">Pointer to variable to receive an IP address of LXI.</param>
    * <param name="LXIAddrLength">Length of LXIAddress string.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_EchoDirectEx(LPCHAR Address,DWORD Port,DWORD Timeout,DWORD *ListenPort,
        DWORD *CardCount,DWORD *ClientCount,DWORD *OpenCardCount,
        LPCHAR Description,DWORD DescLength,
        LPCHAR LXIAddress,DWORD LXIAddrLength);

#ifndef LABVIEWRT

    /**
    * <summary>
    *   Send a broadcast message to all LXI in subnet defined by default broadcast address (255.255.255.255).   
    *   For getting information about available LXIs you must use functions PICMLX_GetAvailableLXICount
    *   and PICMLX_GetAvailableLXIEntry.
    * </summary>
    * <param name="ListenPort">LXI listen port for broadcast messages.</param>
    * <param name="Timeout">Listening timeout for receiving information from LXIs.</param>
    * <param name="AvailableLXICount">
    *   Pointer to variable to receive a count of available LXIs, may be NULL if you don't care of how many were found.
    *   Later you can use PICMLX_GetAvailableLXICount to discover it.
    * </param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_EchoBroadcast(DWORD ListenPort,DWORD Timeout,DWORD* AvailableLXICount);

    /**
    * <summary>
    *   Send a broadcast message to all LXI in subnet defined by Address. For getting information about available
    *   LXIs you must use functions PICMLX_GetAvailableLXICount and PICMLX_GetAvailableLXIEntry.
    * </summary>
    * <param name="Address">Broadcast address of specific network.</param>
    * <param name="ListenPort">LXI listen port for broadcast messages.</param>
    * <param name="Timeout">Listening timeout for receiving information from LXIs.</param>
    * <param name="AvailableLXICount">
    *   Pointer to variable to receive a count of available LXIs, may be NULL if you don't care of how many were found.
    *   Later you can use PICMLX_GetAvailableLXICount to discover it.
    * </param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_EchoBroadcastEx(LPCHAR Address,DWORD ListenPort,DWORD Timeout,DWORD* AvailableLXICount);

    /**
    * <summary>
    *   Returns count of available LXIs if any. Before using this function PICMLX_EchoBroadcast must be called.
    * </summary>
    * <returns>Count of available LXIs if any.</returns>
    */
    DWORD PICMLX_API PICMLX_GetAvailableLXICount(void);

    /**
    * <summary>
    *   Receive information about LXI by index. Before using this function PICMLX_EchoBroadcast must be called.
    * </summary>
    * <param name="Index">Index of LXI info record started from zero.</param>
    * <param name="LXIInfo">Pointer to variable to receive an information about LXI.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */ 
    DWORD PICMLX_API PICMLX_GetAvailableLXIEntry(DWORD Index,LPLXIENTRY LXIInfo);

    /**
    * <summary>
    *   Receive information about LXI by index. Before using this function PICMLX_EchoBroadcast must be called.
    * </summary>
    * <param name="Index">Index of LXI info record.</param>  
    * <param name="ListenPort">Pointer to variable to receive a server bridge listen port for communication.</param>
    * <param name="CardCount">Pointer to variable to receive an available cards count.</param>
    * <param name="ClientCount">Pointer to variable to receive a connected clients count.</param>
    * <param name="OpenCardCount">Pointer to variable to receive an openned cards count.</param>
    * <param name="Description">Pointer to variable to receive an information string.</param>
    * <param name="DescLength">Length of Description string.</param>
    * <param name="LXIAddress">Pointer to variable to receive an IP address of LXI.</param>
    * <param name="LXIAddrLength">Length of LXIAddress string.</param>
    * <returns>Zero for success, or non-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_GetAvailableLXIEntryEx(DWORD Index,DWORD *ListenPort,DWORD *CardCount,
        DWORD *ClientCount,DWORD *OpenCardCount,
        LPCHAR Description,DWORD DescLength,
        LPCHAR LXIAddress,DWORD LXIAddrLength);

#endif // #ifndef LABVIEWRT

    /***************************************
    * SERVER BRIDGE
    ***************************************/

    /**
    * <summary>
    *   Returns a version of ServerBridge on LXI device.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <returns>
    *   The ServerBridge version code, multiplied by 100 (i.e. a value of 100 represents version 1.00)
    *   or INVALID_RESULT value if error occured.
    * </returns>
    */
    DWORD PICMLX_API PICMLX_SbVersion(SESSION SID);

    /**
    * <summary>
    *   Returns count of cards by card type.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="Type">Type of card defined by 'CardTypes' enumerator.</param>
    * <returns>Returns count of cards by card type or INVALID_RESULT value if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetCountCardType(SESSION SID,DWORD Type);

    /**
    * <summary>
    *   Locks the LXI device so that no other client can open card (s). Clients which sooner opened card(s) or client
    *   with access �for read� can still communicate with LXI device or client with lock can do with the LXI device.
    *   Others have to wait until client with lock call s unlock function. Lock function tries to get the lock on LXI
    *   device. All calling process is blocked and wait on lock by timeout. This function is not defined in LXI standart,
    *   so signature could be 78changerd by time.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="Timeout">Time (ms) for which client try to lock a device. If time is up, then returns.</param>
    * <returns>Zero for success, or no-zero error code.</returns>
    */
    DWORD PICMLX_API PICMLX_DeviceLock(SESSION SID,DWORD Timeout);

    /**
    * <summary>
    *   Releases the lock and another client can get a lock or can communicate with LXI device. The lock is released if
    *   connection between Client and Server is unexpectedly interrupted. This function is not defined in LXI standart,
    *   so signature could be changerd by time.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_DeviceUnlock(SESSION SID);

    /**
    * <summary>
    *   Gets number of all cards (no mater on type of card).
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="count">Pointer to variable to receive a number of all cards.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetTotalCardsCount(SESSION SID, DWORD *count);

    /**
    * <summary>
    *   Gets number of all opened cards (no matter on type of card).
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="count">Pointer to variable to receive a number of all opened cards.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetTotalOpenedCards(SESSION SID, DWORD *count);

    /**
    * <summary>
    *   Gets number of all active modules.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="count">Pointer to variable to receive a number of all active modules.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetTotalModulesCount(SESSION SID, DWORD *count);

    /**
    * <summary>
    *   Gets number of all live sessions.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="count">Pointer to variable to receive a number of all live sessions.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetSessionsCount(SESSION SID, DWORD *count);

    /**
    * <summary>
    *   Gets number of sessions which use given card.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="card">Card ID.</param>
    * <param name="count">Pointer to variable to receive a number of sessions which uses given card.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetCardSessionsCount(SESSION SID, DWORD cardType, DWORD card, DWORD *count);
    //
    /**
    * <summary>
    *   Adds another session ID to actual session.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="session">Sesion ID.</param>
    * <param name="token">Unique confirmation token.</param>
    * <param name="length">Length of token string buffer.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_UseForeignSession(SESSION SID, DWORD session, CHAR *token, DWORD length);

    /**
    * <summary>
    *   Releases used foreign session.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="session">Sesion ID.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_ReleaseForeignSession(SESSION SID, DWORD session);

    /**
    * <summary>
    *   Gets active session for using.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="session">Sesion ID.</param>
    * <param name="token">Unique confirmation token.</param>
    * <param name="length">Length of token string buffer.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetActiveSession(SESSION SID, DWORD* session, CHAR *token, DWORD length);

    /**
    * <summary>
    *   Flags card for use.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="card">Card ID.</param>
    * <param name="exclusive">Define whether to use card in exclusive mode. See AccessTypes enum.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_UseCard(SESSION SID, DWORD cardType, DWORD card, DWORD exclusive);

    /**
    * <summary>
    *   Flags all cards of certain card type for use.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="exclusive">Define whether to use card in exclusive mode. See AccessTypes enum.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_UseCards(SESSION SID, DWORD cardType, DWORD exclusive);

    /**
    * <summary>
    *   Unflags card for use.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="card">Card ID.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_ReleaseCard(SESSION SID, DWORD cardType, DWORD card);

    /**
    * <summary>
    *   Unlags all cards of certain card type for use.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_ReleaseCards(SESSION SID, DWORD cardType);

    /**
    * <summary>
    *   Tests whether the card is opened for exclusive access or not.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="card">Card ID.</param>
    * <param name="OwnerType">Owner type. See OwnerTypes enum. Bote that this value if bit based.</param>
    * <param name="isExclusive">True if the card is opened for exclusive access; otherwise false.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_HasExclusiveAccess(SESSION SID, DWORD cardType, DWORD card, DWORD ownerType, BOOL* isExclusive);

    /**
    * <summary>
    *   Tests whether the card is used or not.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="card">Card ID.</param>
    * <param name="OwnerType">Owner type. See OwnerTypes enum. Bote that this value if bit based.</param>
    * <param name="isUsed">True if the card is used; otherwise false</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_IsCardUsed(SESSION SID, DWORD cardType, DWORD card, DWORD ownerType, BOOL* isUsed);

    /**
    * <summary>
    *   Gets clients which communicates with the LXI remotely.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="clients">List of clients separated by ; in form: count - hostname, where count is number of connections coming from the hostname.</param>
    * <param name="length">Length of clients string buffer.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetConnectedClients(SESSION SID, CHAR *clients, DWORD length);

    /**
    * <summary>
    *   Gets clients which use a specific card on the LXI remotely.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="card">Card ID.</param>
    * <param name="clients">List of clients separated by ; in form: count - hostname, where count is number of connections coming from the hostname.</param>
    * <param name="length">Length of clients string buffer.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetConnectedCardClients(SESSION SID, DWORD cardType, DWORD card, CHAR *clients, DWORD length);

    /**
    * <summary>
    *   Gets used cards.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="cards">Pointer to the one-dimensional array (vector) to receive cards' IDs.</param>
    * <param name="numCards">Pointer to variable containing maximum of cards for which information is to be obtained. On return this is changed to number of filled cards.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetUsedCards(SESSION SID, DWORD cardType, DWORD *cards, DWORD *numCards);

    /**
    * <summary>
    *   Gets usable cards.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="cardsType">Card type.</param>
    * <param name="cards">Pointer to the one-dimensional array (vector) to receive cards' IDs.</param>
    * <param name="numCards">Pointer to variable containing maximum of cards for which information is to be obtained. On return this is changed to number of filled cards.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetUsableCards(SESSION SID, DWORD cardType, DWORD *cards, DWORD *numCards);

    /**
    * <summary>
    *   Gets used foreign sessions of active session.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="sessions">Pointer to the one-dimensional array (vector) to receive sessions' IDs.</param>
    * <param name="numSessions">Pointer to variable containing maximum of sessions for which information is to be obtained. On return this is changed to number of filled sessions.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetForeignSessions(SESSION SID, DWORD *sessions, DWORD *numSessions);

	/**
    * <summary>
    *   Gets all monitored voltages values of the LXI chassis.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="voltages">Pointer to the one-dimensional array (vector) to receive the values.</param>
    * <param name="numVoltages">Pointer to variable containing maximum number of values for which information is to be obtained. On return this is changed to number of filled values.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_GetChassisVoltages(SESSION SID, double *voltages, DWORD *numVoltages);

	/**
    * <summary>
    *   Gets all monitored temperatures of the LXI chassis.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="temperatures">Pointer to the one-dimensional array (vector) to receive the temperatures.</param>
    * <param name="numTemperatures">Pointer to variable containing maximum number of values for which information is to be obtained. On return this is changed to number of filled values.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
	DWORD PICMLX_API PICMLX_GetChassisTemperatures(SESSION SID, DWORD *temperatures, DWORD *numTemperatures);

	/**
    * <summary>
    *   Gets all monitored fan speed values of the LXI chassis.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="speeds">Pointer to the one-dimensional array (vector) to receive the fan speed values.</param>
    * <param name="numSpeeds">Pointer to variable containing maximum number of values for which information is to be obtained. On return this is changed to number of filled values.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
	DWORD PICMLX_API PICMLX_GetChassisFanSpeeds(SESSION SID, DWORD *speeds, DWORD *numSpeeds);

	/**
    * <summary>
    *   Set the fan speed of the LXI chassis.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="maxSpeed">1 for max speed. 0 for automatic speed.</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
	DWORD PICMLX_API PICMLX_SetChassisFanSpeeds(SESSION SID, DWORD maxSpeed);

	/**
    * <summary>
    *    Perform reset function. Hard reset means power down and stay down, soft reset means power cycle.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="mode">Chassis power mode. (0 - for power down, 1 - for reboot or power cycle)</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_PowerCycle(SESSION SID, DWORD mode);

    /**
    * <summary>
    *   Identify chassis function.
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="identify">0 - for disable identify feature, 1 - for enable identify feature</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_Identify(SESSION SID, DWORD identify);

    /**
    * <summary>
    *   Enable or disable storing actual card configuration.
	*	Enabled means that the next startup of the chassis will restore last card configuration.
	*   Disabled - all relays opened (reset status).
    * </summary>
    * <param name="SID">Handle of current session.</param>
    * <param name="action">0 - for reset status, 1 - for last configuration status</param>
    * <returns>Zero or non-zero if error occured.</returns>
    */
    DWORD PICMLX_API PICMLX_StoreStates(SESSION SID, DWORD action);

#ifdef __cplusplus
};
#endif

#endif // #ifndef _PICMLX_H_
