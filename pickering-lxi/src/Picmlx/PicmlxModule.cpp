#include <PicmlxModule.h>
#include <PicmlxModuleAPI.h>
#include <Discovery.h>
#include <PicmlxFids.h>
#include <DebugTool.h>
#include <PicmlxResources.h>
#include <System.h>

#ifdef LINUX
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <string.h>  
#else // #ifdef LINUX
#include <time.h>
#endif // #else

enum PicmlxServerErrorCodes {
    ERR_SRV_INVALID_FID = 0x01000000,
    ERR_SRV_RPC_DATA,
    ERR_SRV_INVALID_CARDID,
    ERR_SRV_INVALID_ADDRESS,
    ERR_SRV_OPEN_CARD,
    ERR_SRV_ACCESS_DENIED,
    ERR_SRV_NOT_SUPPORTED,
    ERR_SRV_DEVICE_INFO,
    ERR_SRV_OUT_OF_RESOURCES,
    ERR_SRV_DATA_LENGTH,
    ERR_SRV_CARD_TYPE,
    ERR_SRV_FUNC_NOT_IMPL,
    ERR_SRV_CARD_USED_BYCLI,
    ERR_SRV_WAIT_TIMEOUT,
    ERR_SRV_DEVICE_LOCKED,
    ERR_SRV_INVALID_SESSION,
    ERR_SRV_INVALID_TOKEN,
	ERR_SRV_INVALID_EC,
	ERR_SRV_FATAL_01
};

#ifdef WIN32
extern handle_t __libPicmlx_Instance;  
#endif

/* ##################################################################################################### */

/*
* Private class functions
*/

session_t picmlx_module_c::conjureUniqueSessionID() {
    session_t sid;

    do {
        sid = (session_t)rand();
    } while(isSidExists(sid));
    return sid;
}

pu32 picmlx_module_c::convertServerError(pu32 error,convert_func_t *func) {
    if(!error) {
        return NO_ERROR;
    }
    switch(error) {
    case ERR_SRV_INVALID_FID: return ER_PICMLX_SRV_INVALID_FID;
    case ERR_SRV_RPC_DATA: return ER_PICMLX_SRV_RPC_DATA;
    case ERR_SRV_INVALID_CARDID: return ER_PICMLX_SRV_INVALID_CARDID;
    case ERR_SRV_INVALID_ADDRESS: return ER_PICMLX_SRV_INVALID_ADDRESS;
    case ERR_SRV_OPEN_CARD: return ER_PICMLX_SRV_OPEN_CARD;
    case ERR_SRV_ACCESS_DENIED: return ER_PICMLX_SRV_ACCESS_DENIED;
    case ERR_SRV_NOT_SUPPORTED: return ER_PICMLX_SRV_NOT_SUPPORTED;
    case ERR_SRV_DEVICE_INFO: return ER_PICMLX_SRV_DEVICE_INFO;
    case ERR_SRV_OUT_OF_RESOURCES: return ER_PICMLX_SRV_OUT_OF_RESOURCES;
    case ERR_SRV_DATA_LENGTH: return ER_PICMLX_SRV_DATA_LENGTH;
    case ERR_SRV_CARD_TYPE: return ER_PICMLX_SRV_CARD_TYPE;
    case ERR_SRV_FUNC_NOT_IMPL: return ER_PICMLX_SRV_FUNC_NOT_IMPL;
    case ERR_SRV_CARD_USED_BYCLI: return ER_PICMLX_SRV_CARD_USED_BYCLI;
    case ERR_SRV_WAIT_TIMEOUT: return ER_PICMLX_SRV_WAIT_TIMEOUT;
    case ERR_SRV_DEVICE_LOCKED: return ER_PICMLX_SRV_DEVICE_LOCKED;
    case ERR_SRV_INVALID_SESSION: return ER_PICMLX_SRV_INVALID_SESSION;
    case ERR_SRV_INVALID_TOKEN: return ER_PICMLX_SRV_INVALID_TOKEN;
	case ERR_SRV_INVALID_EC: return ER_PICMLX_SRV_INVALID_EC;
	case ERR_SRV_FATAL_01: return ER_PICMLX_SRV_FATAL_01;
	
    default:
        if(func != NULL) {
            return func(error);
        }
        break;
    }
    return error;
}

pu32 picmlx_module_c::error2Resource(pu32 error) {
    switch(error) {
        // No error
    case NO_ERROR: return RES_NO_ERROR;
        // Client side error & resource codes
    case ER_PICMLX_CLI_INVALID_SESSION: return RES_PICMLX_CLI_INVALID_SESSION;
    case ER_PICMLX_CLI_HOST_UNAVAILABLE: return RES_PICMLX_CLI_HOST_UNAVAILABLE;
    case ER_PICMLX_CLI_CONNECT: return RES_PICMLX_CLI_CONNECT;
    case ER_PICMLX_CLI_CONNECT_REFUSED: return RES_PICMLX_CLI_CONNECT_REFUSED;
	case ER_PICMLX_CLI_NAME_NOT_KNOWN: return RES_PICMLX_CLI_NAME_NOT_KNOWN;
    case ER_PICMLX_CLI_PARAM_SIZE: return RES_PICMLX_CLI_PARAM_SIZE;
    case ER_PICMLX_CLI_BEGINTRANS: return RES_PICMLX_CLI_BEGINTRANS;
    case ER_PICMLX_CLI_ENDTRANS: return RES_PICMLX_CLI_ENDTRANS;
    case ER_PICMLX_CLI_RECV_FAILED: return RES_PICMLX_CLI_RECV_FAILED;
    case ER_PICMLX_CLI_RECV_DATA: return RES_PICMLX_CLI_RECV_DATA;
    case ER_PICMLX_CLI_SEND_FAILED: return RES_PICMLX_CLI_SEND_FAILED;
    case ER_PICMLX_CLI_NOT_CONNECTED: return RES_PICMLX_CLI_NOT_CONNECTED;
    case ER_PICMLX_CLI_INVALID_INDEX: return RES_PICMLX_CLI_INVALID_INDEX;
    case ER_PICMLX_CLI_DIRECTECHO_FAILED: return RES_PICMLX_CLI_DIRECTECHO_FAILED;
    case ER_PICMLX_CLI_BROADCASTECHO_FAILED: return RES_PICMLX_CLI_BROADCASTECHO_FAILED;
    case ER_PICMLX_CLI_FUNC_NOT_LOCAL: return RES_PICMLX_CLI_FUNC_NOT_LOCAL;
	case ER_PICMLX_CLI_DATA_NOT_ACCEPT: return RES_PICMLX_CLI_DATA_NOT_ACCEPT;
        // Server side error & resource codes
    case ER_PICMLX_SRV_INVALID_FID: return RES_PICMLX_SRV_INVALID_FID;
    case ER_PICMLX_SRV_RPC_DATA: return RES_PICMLX_SRV_RPC_DATA;
    case ER_PICMLX_SRV_INVALID_CARDID: return RES_PICMLX_SRV_INVALID_CARDID;
    case ER_PICMLX_SRV_INVALID_ADDRESS: return RES_PICMLX_SRV_INVALID_ADDRESS;
    case ER_PICMLX_SRV_OPEN_CARD: return RES_PICMLX_SRV_OPEN_CARD;
    case ER_PICMLX_SRV_ACCESS_DENIED: return RES_PICMLX_SRV_ACCESS_DENIED;
    case ER_PICMLX_SRV_NOT_SUPPORTED: return RES_PICMLX_SRV_NOT_SUPPORTED;
    case ER_PICMLX_SRV_DEVICE_INFO: return RES_PICMLX_SRV_DEVICE_INFO;
    case ER_PICMLX_SRV_OUT_OF_RESOURCES: return RES_PICMLX_SRV_OUT_OF_RESOURCES;
    case ER_PICMLX_SRV_DATA_LENGTH: return RES_PICMLX_SRV_DATA_LENGTH;
    case ER_PICMLX_SRV_CARD_TYPE: return RES_PICMLX_SRV_CARD_TYPE;
    case ER_PICMLX_SRV_FUNC_NOT_IMPL: return RES_PICMLX_SRV_FUNC_NOT_IMPL;
    case ER_PICMLX_SRV_CARD_USED_BYCLI: return RES_PICMLX_SRV_CARD_USED_BYCLI;
    case ER_PICMLX_SRV_UNDEFINED: return RES_PICMLX_SRV_UNDEFINED;
    case ER_PICMLX_SRV_WAIT_TIMEOUT: return RES_PICMLX_SRV_WAIT_TIMEOUT;
    case ER_PICMLX_SRV_DEVICE_LOCKED: return RES_PICMLX_SRV_DEVICE_LOCKED;
    case ER_PICMLX_SRV_INVALID_SESSION: return RES_PICMLX_SRV_INVALID_SESSION;
    case ER_PICMLX_SRV_INVALID_TOKEN: return RES_PICMLX_SRV_INVALID_TOKEN;
	case ER_PICMLX_SRV_INVALID_EC: return RES_PICMLX_SRV_INVALID_EC;
	case ER_PICMLX_SRV_FATAL_01: return RES_PICMLX_SRV_FATAL_01;
    case ER_PICMLX_CLI_TIMEOUT_FAIL: return RES_PICMLX_CLI_TIMEOUT_FAIL;
		// Unknown error code
    default:
        return RES_UNKNOWN_ERROR;
    }
}

pbool picmlx_module_c::isRemote(pchar *address) {
    pbool result;
    pu32 length;
    pchar *temp;

    result = FALSE;  
    if(address) {
        length = (pu32)strlen(address) + 1;
        temp = new char[length];
        strcpy_s(temp,length,address);    
        _strupr_s(temp,length);
        //result = !(!strcmp(temp,"") || !strcmp(temp,"PXI") || (address == NULL));
        result = address != NULL;
        result &= strcmp(temp,"") != 0;
        result &= strcmp(temp,"PXI") != 0;
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"::isRemote.Remote?%s: %d",temp,!result);
#endif
        /* DEBUG RECORD END */
        delete temp;
    }
    return result;
}

pbool picmlx_module_c::isSidExists(session_t sid) {  
    return sessions->getSession(sid) != NULL;
}

/* ##################################################################################################### */

/*
* Public class functions
*/

/* Constructor/destructor */

picmlx_module_c::picmlx_module_c(const pchar *libName) : base_module_c(libName) {
    sessions = new session_list_c(TRUE);
    activeSession = NULL;
    activeOccupant = NULL;
    srand((unsigned)time(NULL));
    // Create error message list
#ifdef LINUX
    errorList->insert(make_pair(RES_UNKNOWN_ERROR,"Unknown error code."));
    errorList->insert(make_pair(RES_NO_ERROR,"Success, no error."));
    // client error messages
    errorList->insert(make_pair(RES_PICMLX_CLI_INVALID_SESSION,"Client: Invalid session ID."));
    errorList->insert(make_pair(RES_PICMLX_CLI_HOST_UNAVAILABLE,"Client: Destination host is unavailable."));
    errorList->insert(make_pair(RES_PICMLX_CLI_CONNECT,"Client: Connect failed."));
    errorList->insert(make_pair(RES_PICMLX_CLI_PARAM_SIZE,"Client: Argument is NULL or a value is outside the valid range."));
    errorList->insert(make_pair(RES_PICMLX_CLI_BEGINTRANS,"Client: Transmition wasn't initiated. First call PICMLX_BeginTransmition!"));
    errorList->insert(make_pair(RES_PICMLX_CLI_ENDTRANS,"Client: Previous transmition wasn't ended. Call PICMLX_EndTransmition!"));
    errorList->insert(make_pair(RES_PICMLX_CLI_RECV_FAILED,"Client: Receive data failed."));
    errorList->insert(make_pair(RES_PICMLX_CLI_RECV_DATA,"Client: Received corrupted data."));
    errorList->insert(make_pair(RES_PICMLX_CLI_SEND_FAILED,"Client: Send data failed."));
    errorList->insert(make_pair(RES_PICMLX_CLI_NOT_CONNECTED,"Client: Not connected."));
    errorList->insert(make_pair(RES_PICMLX_CLI_INVALID_INDEX,"Client: Invalid list item index."));
    errorList->insert(make_pair(RES_PICMLX_CLI_DIRECTECHO_FAILED,"Client: Direct discovery function failed."));
    errorList->insert(make_pair(RES_PICMLX_CLI_BROADCASTECHO_FAILED,"Client: Broadcast discovery function failed."));
    errorList->insert(make_pair(RES_PICMLX_CLI_FUNC_NOT_LOCAL,"Client: Function is not supported on local system."));
	errorList->insert(make_pair(RES_PICMLX_CLI_DATA_NOT_ACCEPT,"Client: Invalid parameter value."));
	errorList->insert(make_pair(RES_PICMLX_CLI_CONNECT_REFUSED,"Client: Connection refused."));
	errorList->insert(make_pair(RES_PICMLX_CLI_NAME_NOT_KNOWN,"Client: Name or IP address not known.")); 
    // server error messages
    errorList->insert(make_pair(RES_PICMLX_SRV_INVALID_FID,"Server: Invalid function ID."));
    errorList->insert(make_pair(RES_PICMLX_SRV_RPC_DATA,"Server: Corrupted RPC data."));
    errorList->insert(make_pair(RES_PICMLX_SRV_INVALID_CARDID,"Server: Invalid card ID."));
    errorList->insert(make_pair(RES_PICMLX_SRV_INVALID_ADDRESS,"Server: Address of card is invalid."));
    errorList->insert(make_pair(RES_PICMLX_SRV_OPEN_CARD,"Server: The card is used."));
    errorList->insert(make_pair(RES_PICMLX_SRV_ACCESS_DENIED,"Server: Access denied."));
    errorList->insert(make_pair(RES_PICMLX_SRV_NOT_SUPPORTED,"Server: Function is not supported by this LXI device."));
    errorList->insert(make_pair(RES_PICMLX_SRV_DEVICE_INFO,"Server: Information about LXI device isn't obtainable or isn't all."));
    errorList->insert(make_pair(RES_PICMLX_SRV_OUT_OF_RESOURCES,"Server: Out of resources."));
    errorList->insert(make_pair(RES_PICMLX_SRV_DATA_LENGTH,"Server: Required length of data is wrong."));
    errorList->insert(make_pair(RES_PICMLX_SRV_CARD_TYPE,"Server: LXI device hasn't card of this type."));
    errorList->insert(make_pair(RES_PICMLX_SRV_FUNC_NOT_IMPL,"Server: Function isn't implemented or it's reserved."));
    errorList->insert(make_pair(RES_PICMLX_SRV_CARD_USED_BYCLI,"Server: The card is used by the client."));
    errorList->insert(make_pair(RES_PICMLX_SRV_UNDEFINED,"Server: Undefined error."));
    errorList->insert(make_pair(RES_PICMLX_SRV_WAIT_TIMEOUT,"Server: Wait timeout expired."));
    errorList->insert(make_pair(RES_PICMLX_SRV_DEVICE_LOCKED,"Server: Device locked...another client has exclusive access."));
    errorList->insert(make_pair(RES_PICMLX_SRV_INVALID_SESSION,"Server: Invalid session."));
    errorList->insert(make_pair(RES_PICMLX_SRV_INVALID_TOKEN,"Server: Invalid token."));
	errorList->insert(make_pair(RES_PICMLX_SRV_INVALID_EC,"Server: Invalid error code."));
	errorList->insert(make_pair(RES_PICMLX_SRV_FATAL_01,"Server: Fatal error - 1."));
    errorList->insert(make_pair(RES_PICMLX_CLI_TIMEOUT_FAIL,"Client: Can not set recv/send socket timeout."));
#endif // #ifdef LINUX

#ifndef LABVIEWRT
    devices = new device_list_c();
#endif // #ifndef LABVIEWRT
    connectCount = 0;
}

picmlx_module_c::~picmlx_module_c() {

#ifndef LABVIEWRT
    delete devices;
#endif // #ifndef LABVIEWRT

    delete sessions;
    //base_module_c::~base_module_c();
}

/* ##################################################################################################### */

/* Communication connect/disconnect functions */

pu32 picmlx_module_c::connect(pu32 board,pchar *address,pu32 port,pu32 timeout,session_t *sid) {
    session_entry_s *session;
    occupant_info_s *occupant;
    pbool remote;
    pu32 result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"picmlxModule_c::connect - begin");
#endif
    /* DEBUG RECORD END */  
    result = NO_ERROR;
    //if((sid == NULL) || (address == NULL))
    if((sid == NULL) || ((port < 1) || (port > 65535)))
        result = ER_PICMLX_CLI_PARAM_SIZE;
    else {
        *sid = INVALID_SESSION;
        remote = this->isRemote(address);
        /*if(remote = this->isRemote(address)) {
            if(!ping(address,timeout)) {
                result = ER_PICMLX_CLI_HOST_UNAVAILABLE;
            }
        }*/
    }
    if(!result) {
        *sid = this->conjureUniqueSessionID();
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"::Session ID: %d",*sid);
#endif
        /* DEBUG RECORD END */
        // Create session info
        session = new session_entry_s;    
        session->occupants = new occupant_list_c(TRUE);
        // Fill session info data
        if(remote) {
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(FALSE,"::address: %s@%d, port: %d",address,strlen(address),port);
#endif
            /* DEBUG RECORD END */
            session->info.addr_len = (pu32)strlen(address) + 1;
            session->info.addr = new char[session->info.addr_len];
            //session->info.addr[session->info.addr_len] = 0;
            strcpy_s(session->info.addr,session->info.addr_len,address);      
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(FALSE,"::session->addr: %s@%d",session->info.addr,session->info.addr_len);
#endif
            /* DEBUG RECORD END */      
#ifndef LABVIEWRT
            session->socket = new socket_client_c(board,SOCKET_TCP,SOCKET_BUFFER_SIZE,true);      
#else // #ifndef LABVIEWRT
            session->socket = new socket_client_c(SOCKET_VISA,SOCKET_BUFFER_SIZE);
#endif // #else      
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(FALSE,"::Socket handle: %d",session->socket->getHandle());
#endif    
            /* DEBUG RECORD END */
        }    
        session->info.connected = FALSE;
        session->info.port = port;
        session->info.sid = *sid;
        session->info.remote = remote;
        // Add current process occupant
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"::Adding current session occupant.");
#endif
        /* DEBUG RECORD END */
        occupant = new occupant_info_s;
        occupant->id = PROCESS_ID;
        occupant->lastError = NO_ERROR;
        session->occupants->insert(occupant);
        // Now connect 
        if(remote) {
            if(((ps32)timeout) <= 0)
                timeout = 500;
            result = ER_PICMLX_CLI_CONNECT;
            if( (session->info.connected = session->socket->connect(session->info.addr,session->info.port,timeout)) ) {
                if(!session->socket->setRecvTimeout(timeout))
                {
#if defined(DEBUG) || defined(_DEBUG)
                    __DEBUGMSG(TRUE,"::setRecvTimeout: %d", GetLastError());
#endif
                    disconnect(session->info.sid);
                    return ER_PICMLX_CLI_TIMEOUT_FAIL;
                }
                if(!session->socket->setSendTimeout(timeout))
                {
#if defined(DEBUG) || defined(_DEBUG)
                    __DEBUGMSG(TRUE,"::setSendTimeout: %d", GetLastError());

#endif
                    disconnect(session->info.sid);
                    return ER_PICMLX_CLI_TIMEOUT_FAIL;
                }
                sessions->insert(session);
                connectCount++;
                result = NO_ERROR;                
            }
			else {
				result = session->socket->GetConnectResult();
			}
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(TRUE,"::Connected: %d",session->info.connected);
#endif    
            /* DEBUG RECORD END */
        } else {
            session->info.connected = TRUE;
            sessions->insert(session);
            result = NO_ERROR;
        }
        if(!session->info.connected) {
            delete session->occupants;
            if(remote)
                delete session->info.addr;
            delete session;
        }
    }
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"::connectCount: %d",connectCount);
    __DEBUGMSG(FALSE,"::result: %d",result);
    __DEBUGMSG(FALSE,"picmlxModule_c::connect - end");  
#endif
    /* DEBUG RECORD END */  
    return result;
}

pu32 picmlx_module_c::disconnect(session_t sid) {
    session_entry_s *session;
    pu32 result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"picmlxModule_c::disconnect - begin");
    __DEBUGMSG(FALSE,"::session: %d",sid);
#endif
    /* DEBUG RECORD END */
    result = ER_PICMLX_CLI_INVALID_SESSION;
    if((session = sessions->getSession(sid)) != NULL) {
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"::vadil session: %d",sid);
#endif
        /* DEBUG RECORD END */
        if(session->info.remote) {
            session->socket->disconnect();
            delete session->socket;
            delete session->info.addr;
        }
        delete session->occupants;    
        sessions->remove(sid);    
        connectCount--;
        result = NO_ERROR;
    }
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"::connectCount: %d",connectCount);
    __DEBUGMSG(FALSE,"::result: %d",result);
    __DEBUGMSG(FALSE,"picmlxModule_c::disconnect - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

/* ##################################################################################################### */

/* Error handling functions */

pbool picmlx_module_c::isError(session_t sid) {
    pu32 code;

    if(getLastError(sid,&code))
        return false;    
    return (pbool)code;
}

pu32 picmlx_module_c::getLastError(session_t sid,pu32 *code) {  
    session_data_s sesData;
    pu32 result;

    if(sid == INVALID_SESSION)
        return ER_PICMLX_CLI_INVALID_SESSION;
    if(code == NULL)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = beginTransmition(sid,&sesData);
    if(!result) {
        *code = sesData.lastError;  
        endTransmition(&sesData);
    }
    return result;
}

pu32 picmlx_module_c::getLastErrorMessage(session_t sid,pchar *buffer,pu32 size) {
    pu32 code,result,length;
    pchar fmtStr[512];
	::memset(fmtStr,0,512);

    if((buffer == NULL) || (((ps32)size) <= 0)) {
        return ER_PICMLX_CLI_PARAM_SIZE;
    }  
    result = getLastError(sid,&code);
    if(!result) {
        if(code >= 0) {
            code = errorCode2Message(code,buffer,size);          
            sprintf_s(fmtStr,sizeof(fmtStr),"%s [SysError: %d]",buffer,GetLastError());
            length = (pu32)strlen(fmtStr);
            if(size <= length) {
                fmtStr[size - 1] = 0;
            }
            strcpy_s(buffer,size,fmtStr);
        }
        result = NO_ERROR;
    }
    return result;
}

pu32 picmlx_module_c::errorCode2Message(pu32 code,pchar *buffer,pu32 size) {
    pu32 resCode;

    if((buffer == NULL) || (((ps32)size) <= 0)) {
        return ER_PICMLX_CLI_PARAM_SIZE;
    }  
    buffer[0] = 0;
    resCode = error2Resource(code);

#ifdef WIN32    
    LOADSTRING(__libPicmlx_Instance,resCode,buffer,size);
#elif LINUX // #ifdef WIN32
    LOADSTRING(errorList,resCode,buffer,size);
#endif // #elif LINUX

    return NO_ERROR;
}

/* ##################################################################################################### */

/* Another module develop API functions */

pu32 picmlx_module_c::beginTransmition(session_t sid,session_data_s *sessionData) {
    register ps32 pid;  

    if(activeSession != NULL) {
        return ER_PICMLX_CLI_ENDTRANS;
    }
    if(sessionData == NULL) {
        return ER_PICMLX_CLI_PARAM_SIZE;
    }
    activeSession = sessions->getSession(sid);
    if(activeSession == NULL) {
        return ER_PICMLX_CLI_INVALID_SESSION;
    }
    pid = PROCESS_ID;
    activeOccupant = activeSession->occupants->getOccupant(pid);
    if(activeOccupant == NULL) {
        activeOccupant = new occupant_info_s;
        activeOccupant->id = pid;
        activeOccupant->lastError = NO_ERROR;
        activeSession->occupants->insert(activeOccupant);
    }
    //sessionData = new session_data_s;
    sessionData->connected = activeSession->info.connected;
    sessionData->remote = activeSession->info.remote;
    sessionData->lastError = activeOccupant->lastError;
    return NO_ERROR;
}

pu32 picmlx_module_c::endTransmition(const session_data_s *sessionData) {
    if(activeSession == NULL) {
        return ER_PICMLX_CLI_BEGINTRANS;
    }
    activeOccupant->lastError = sessionData->lastError;
    activeSession = NULL;
    activeOccupant = NULL;
    //delete sessionData;
    return sessionData->lastError;
}

pbool picmlx_module_c::getOption(pu32 option,int *value) {
    if(activeSession == NULL) {
        return (pbool)ER_PICMLX_CLI_BEGINTRANS;
    }  
    if(!activeSession->info.connected) {
        return (pbool)(activeOccupant->lastError = ER_PICMLX_CLI_NOT_CONNECTED);
    }
    if(option == ON_RECVTIMEO) {
        activeOccupant->lastError = activeSession->socket->getRecvTimeout((pu32*)value);
    }
    return (pbool)activeOccupant->lastError;
    //(lastError ? lastError : activeOccupant->lastError);
}

pu32 picmlx_module_c::recvPacket(convert_func_t *func,const pchar *format,...) {
    va_list args;

    va_start(args, format);
    pu32 result = va_recvPacket(func, format, args);
    va_end(args);
    return result;
}

pu32 picmlx_module_c::va_recvPacket(convert_func_t *func,const pchar *format,va_list args) {
    //register va_list args;
    register pu32 result;
    register pu32 argCount;
    register pu32 argIndex;
    register pu32 arrayLength;
    register pu32 *lpArrayLength;

    result = NO_ERROR;
    if(activeSession == NULL) {
        return ER_PICMLX_CLI_BEGINTRANS;
    }
    if(!activeSession->info.connected) {
        return activeOccupant->lastError = ER_PICMLX_CLI_NOT_CONNECTED;
    }
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"picmlx_module_c::recvPacket - begin");    
#endif
    /* DEBUG RECORD END */
    activeOccupant->lastError = ER_PICMLX_CLI_RECV_FAILED;
    activeSession->socket->setOffset(0);
    if(activeSession->socket->recv()) {
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)      
        __DEBUGMSGHEX("::packet data valist: %s","%.2X ",activeSession->socket->getBuffer(),activeSession->socket->getRecvBytesCount());
#endif
        /* DEBUG RECORD END */
        if(!(result = activeSession->socket->readUInt32())) {
            activeOccupant->lastError = ER_PICMLX_CLI_RECV_DATA;
            if(activeSession->socket->getRecvBytesCount() == activeSession->socket->readUInt32() + 2 * INT_SIZE) {
                activeOccupant->lastError = NO_ERROR;
                if(format) {
                    argCount = (pu32)strlen(format);
                    //va_start(args,format);
                    for(argIndex = 0; argIndex < argCount; argIndex++) {
                        switch(format[argIndex]) {
                            // Result
                        case 'R':
                            *va_arg(args,pu32*) = result;
                            break;
                            // Array result
                        case 'a':
                            argIndex++;
                            arrayLength = va_arg(args,pu32);
                            switch(format[argIndex]) {
                                //  result
                            case 'c':
                                activeSession->socket->readString(va_arg(args,pchar*),arrayLength * CHAR_SIZE);
                                break;
                                //  result
                            case 'i':
                                activeSession->socket->readArray(va_arg(args,int*),arrayLength,INT_SIZE);
                                break;
                                //  result
                            case 'u':
                                activeSession->socket->readArray(va_arg(args,pu32*),arrayLength,INT_SIZE);
                                break;
                                //  result
                            case 'b':
                                activeSession->socket->readArray(va_arg(args,pbool*),arrayLength,CHAR_SIZE);
                                break;
                                //  result
                            case 'f':
                                activeSession->socket->readArray(va_arg(args,pfloat*),arrayLength,FLOAT_SIZE);
                                break;
                                //  result
                            case 'd':
                                activeSession->socket->readArray(va_arg(args,pdouble*),arrayLength,DOUBLE_SIZE);
                                break;
                            }
                            break;
                            // Array result with returning array size received from ServerBridge
                        case 'p':
                            argIndex++;
                            arrayLength = *(lpArrayLength = va_arg(args,pu32*));
                            switch(format[argIndex]) {
                                //  result
                            case 'c':
                                activeSession->socket->readArray(va_arg(args,pchar*),lpArrayLength,CHAR_SIZE);
                                break;
                                //  result
                            case 'i':
                                activeSession->socket->readArray(va_arg(args,int*),lpArrayLength,INT_SIZE);
                                break;
                                //  result
                            case 'u':
                                activeSession->socket->readArray(va_arg(args,pu32*),lpArrayLength,INT_SIZE);
                                break;
                                //  result
                            case 'b':
                                activeSession->socket->readArray(va_arg(args,pbool*),lpArrayLength,CHAR_SIZE);
                                break;
                                //  result
                            case 'f':
                                activeSession->socket->readArray(va_arg(args,pfloat*),lpArrayLength,FLOAT_SIZE);
                                break;
                                //  result
                            case 'd':
                                activeSession->socket->readArray(va_arg(args,pdouble*),lpArrayLength,DOUBLE_SIZE);
                                break;
                            }
                            break;
                            // pchar result
                        case 'c':
                            *va_arg(args,pchar*) = activeSession->socket->readChar();
                            break;
                            //  result
                        case 'i':
                            *va_arg(args,int*) = activeSession->socket->readInt32();
                            break;
                            //  result
                        case 'u':
                            *va_arg(args,pu32*) = activeSession->socket->readUInt32();
                            break;
                            //  pbool result
                        case 'b':
                            *va_arg(args,pbool*) = activeSession->socket->readChar();
                            break;
                            //  result
                        case 'f':
                            *va_arg(args,pfloat*) = activeSession->socket->readFloat();
                            break;
                            //  result
                        case 'd':
                            *va_arg(args,pdouble*) = activeSession->socket->readDouble();
                            break;
                        }
                    }
                    //va_end(args);          
                }
            }
        } else {
            activeOccupant->lastError = convertServerError(result,func);
        }
    }
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"::result: %d",activeOccupant->lastError);
    __DEBUGMSG(FALSE,"picmlx_module_c::recvPacket - end");  
#endif
    /* DEBUG RECORD END */
    return activeOccupant->lastError;
}

pu32 picmlx_module_c::sendPacket(pu32 command,const pchar *format,...) {
    va_list args;

    va_start(args, format);
    pu32 result = va_sendPacket(command, format, args);
    va_end(args);
    return result;
}

pu32 picmlx_module_c::va_sendPacket(pu32 command,const pchar *format,va_list args) {
    //register va_list args;
    register pu32 packetSize;
    register pu32 argCount;
    register pu32 argIndex;
    register pu32 arrayLength;

    if(activeSession == NULL) {
        return ER_PICMLX_CLI_BEGINTRANS;
    }  
    if(!activeSession->info.connected) {
        return activeOccupant->lastError = ER_PICMLX_CLI_NOT_CONNECTED;
    }
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"picmlx_module_c::sendPacket - begin");  
    __DEBUGMSG(FALSE,"::fid: %d",command);
#endif
    /* DEBUG RECORD END */
    activeOccupant->lastError = NO_ERROR;
    packetSize = MESH_HEADER_SIZE;
    activeSession->socket->setOffset(0);
    // Store command into a socket buffer
    activeSession->socket->writeUInt32(command);
    // Store start packet data size
    activeSession->socket->writeUInt32(0);
    // Now start storing packet data if any
    if(format) {
        argCount = (pu32)strlen(format);
        //va_start(args,format);
        for(argIndex = 0; argIndex < argCount; argIndex++) {
            switch(format[argIndex]) {
                /* Array types */
            case 'a':
                argIndex++;
                arrayLength = va_arg(args,pu32);
                switch(format[argIndex]) {
                    // Array of pchar
                case 'c':              
                    if((activeSession->socket->getOffset() + arrayLength) > activeSession->socket->getBufferSize()) {
                        activeOccupant->lastError = ER_PICMLX_CLI_PARAM_SIZE;        
                        goto BufferSizeExceeded;
                    }
                    activeSession->socket->writeString(va_arg(args,pchar*),arrayLength);
                    break;
                    // Array of int
                case 'i':
                    if(activeSession->socket->getOffset() + (arrayLength * DOUBLE_SIZE) > activeSession->socket->getBufferSize()) {
                        activeOccupant->lastError = ER_PICMLX_CLI_PARAM_SIZE;        
                        goto BufferSizeExceeded;
                    }
                    activeSession->socket->writeArray(va_arg(args,int*),arrayLength,INT_SIZE);
                    break;
                    // Array of pu32
                case 'u':
                    if(activeSession->socket->getOffset() + (arrayLength * DOUBLE_SIZE) > activeSession->socket->getBufferSize()) {
                        activeOccupant->lastError = ER_PICMLX_CLI_PARAM_SIZE;        
                        goto BufferSizeExceeded;
                    }
                    activeSession->socket->writeArray(va_arg(args,pu32*),arrayLength,INT_SIZE);
                    break;
                    // Array of pbool
                case 'b':
                    if(activeSession->socket->getOffset() + (arrayLength * DOUBLE_SIZE) > activeSession->socket->getBufferSize()) {
                        activeOccupant->lastError = ER_PICMLX_CLI_PARAM_SIZE;        
                        goto BufferSizeExceeded;
                    }
                    activeSession->socket->writeArray(va_arg(args,pbool*),arrayLength,BOOL_SIZE);
                    break;
                    // Array of pfloat
                case 'f':
                    if(activeSession->socket->getOffset() + (arrayLength * DOUBLE_SIZE) > activeSession->socket->getBufferSize()) {
                        activeOccupant->lastError = ER_PICMLX_CLI_PARAM_SIZE;        
                        goto BufferSizeExceeded;
                    }
                    activeSession->socket->writeArray(va_arg(args,pfloat*),arrayLength,FLOAT_SIZE);
                    break;
                    // Array of pdouble
                case 'd':
                    if(activeSession->socket->getOffset() + (arrayLength * DOUBLE_SIZE) > activeSession->socket->getBufferSize()) {
                        activeOccupant->lastError = ER_PICMLX_CLI_PARAM_SIZE;        
                        goto BufferSizeExceeded;
                    }
                    activeSession->socket->writeArray(va_arg(args,pdouble*),arrayLength,DOUBLE_SIZE);
                    break;
                }
                break;
                /* Simple types */
                // pchar value
            case 'c':
#ifdef WIN32
                activeSession->socket->writeChar(va_arg(args,pchar));
#elif LINUX // #ifdef WIN32
                activeSession->socket->writeChar((pchar)va_arg(args,int));
#endif // #elif LINUX
                break;
                // int value
            case 'i':
                activeSession->socket->writeInt32(va_arg(args,int));
                break;
                //  pu32 value
            case 'u':
                activeSession->socket->writeUInt32(va_arg(args,pu32));
                break;
                //  pbool value
            case 'b':
                activeSession->socket->writeChar((pbool)va_arg(args,int));
                break;
                //  pfloat value
            case 'f':
                activeSession->socket->writeFloat((pfloat)va_arg(args,pdouble));
                break;
                //  pdouble value
            case 'd':
                activeSession->socket->writeDouble(va_arg(args,pdouble));
                break;
            }
            if(activeSession->socket->getOffset() > activeSession->socket->getBufferSize()) {
                activeOccupant->lastError = ER_PICMLX_CLI_PARAM_SIZE;        
                break;
            }
        }
        //va_end(args);
    }
BufferSizeExceeded:
    // Now store real packet data size
    if(!activeOccupant->lastError) {
        if(packetSize != activeSession->socket->getOffset()) {
            packetSize = activeSession->socket->getOffset();
            activeSession->socket->setOffset(INT_SIZE);
            activeSession->socket->writeUInt32(packetSize - MESH_HEADER_SIZE);
            activeSession->socket->setOffset(packetSize);
        }
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)      
        __DEBUGMSGHEX("::packet data: %s","%.2X ",activeSession->socket->getBuffer(),packetSize);
#endif
        /* DEBUG RECORD END */
        // Now send the packet
        if(!activeSession->socket->send())
            activeOccupant->lastError = ER_PICMLX_CLI_SEND_FAILED;
    }
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"::result: %d",activeOccupant->lastError);
    __DEBUGMSG(FALSE,"picmlx_module_c::sendPacket - end");  
#endif
    /* DEBUG RECORD END */
    return activeOccupant->lastError;
}

pbool picmlx_module_c::setOption(pu32 option,int value) {
    if(activeSession == NULL) {
        return (pbool)ER_PICMLX_CLI_BEGINTRANS;
    }  
    if(!activeSession->info.connected) {
        return (pbool)(activeOccupant->lastError = ER_PICMLX_CLI_NOT_CONNECTED);
    }
    if(option == ON_RECVTIMEO) {
        activeOccupant->lastError = activeSession->socket->setRecvTimeout(value);
    }
    if(option == ON_SNDTIMEO) {
        activeOccupant->lastError = activeSession->socket->setSendTimeout(value);
    }
    return (pbool)activeOccupant->lastError;
}

/* ##################################################################################################### */

/* Discovery functions */

pu32 picmlx_module_c::echoDirect(pchar *deviceAddr,pu32 port,pu32 timeout,device_entry_t *device) {
    socket_client_c *socket;
    pu32 result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"picmlx_module_c::echoDirect - begin");
#endif
    /* DEBUG RECORD END */
    result = ER_PICMLX_CLI_PARAM_SIZE;
    if((device != NULL) && (deviceAddr != NULL) && (port > 0) && (port < 65536)) {
        result = ER_PICMLX_CLI_DIRECTECHO_FAILED;

#ifndef LABVIEWRT
        socket = new socket_client_c(0,SOCKET_TCP,SOCKET_BUFFER_SIZE,false);
#else // #ifndef LABVIEWRT
        socket = new socket_client_c(SOCKET_VISA,SOCKET_BUFFER_SIZE);
#endif // #else
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(TRUE,"::Socket pointer: %d",socket);
#endif
        /* DEBUG RECORD END */
        if(socket != NULL) {
            result = ER_PICMLX_CLI_HOST_UNAVAILABLE;
            if(((ps32)timeout) <= 0)
                timeout = 500;
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(TRUE,"::Ping ok");
#endif
            /* DEBUG RECORD END */
            result = ER_PICMLX_CLI_CONNECT;
            if(socket->connect(deviceAddr,port,timeout)) {
                socket->setRecvTimeout(timeout);
                result = ER_PICMLX_CLI_RECV_FAILED;
                if(socket->recv()) {
                    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
                    __DEBUGMSG(TRUE,"::Received %dbytes",socket->getRecvBytesCount());
#endif
                    /* DEBUG RECORD END */
                    if((result = convertServerError(socket->readUInt32(),NULL)) == NO_ERROR) {
                        result = ER_PICMLX_CLI_RECV_DATA;
                        if(socket->getRecvBytesCount() == socket->readUInt32() + 2 * INT_SIZE) {
                            device->CardCount = socket->readUInt32();
                            device->OpenedCardCount = socket->readUInt32();
                            device->ClientCount = socket->readUInt32();
                            device->ListenPort = socket->readUInt32();
                            socket->readString(device->Description,sizeof(device->Description));
                            socket->getPeerName(device->IP,sizeof(device->IP));
                            result = NO_ERROR;
                        }
                    }
                }
            }
            delete socket;
        }
    }
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"::result: %d",result);
    __DEBUGMSG(FALSE,"picmlx_module_c::echoDirect - end");
#endif
    return result;
}

pu32 picmlx_module_c::echoDirectEx(pchar *deviceAddr,pu32 port,pu32 timeout,pu32 *listenPort,pu32 *cardCount,
    pu32 *clientCount,pu32 *opennedCardCount,pchar *desc,
    pu32 descSize,pchar *addr,pu32 addrSize) {
        device_entry_t item = {0};
        pu32 result;

        if(!(result = echoDirect(deviceAddr,port,timeout,&item))) {
            if(listenPort != NULL)
                *listenPort = item.ListenPort;
            if(cardCount != NULL)
                *cardCount = item.CardCount;
            if(clientCount != NULL)
                *clientCount = item.ClientCount;
            if(opennedCardCount != NULL)
                *opennedCardCount = item.OpenedCardCount;
            if((desc != NULL) && (descSize > 0))
                strcpy_s(desc,min(descSize,sizeof(item.Description) + 1),item.Description);
            if((addr != NULL) && (addrSize > 0))
                strcpy_s(addr,min(addrSize,sizeof(item.IP) + 1),item.IP);
        }
        return result;
}

#ifndef LABVIEWRT

pu32 picmlx_module_c::echoBroadcast(pu32 listenPort,pu32 timeout,pu32 *count) {    
    return echoBroadcastEx(BROADCAST_ADDRESS,listenPort,timeout,count);
}

pu32 picmlx_module_c::echoBroadcastEx(const pchar *addr,pu32 listenPort,pu32 timeout,pu32 *count) {
    socket_client_c *client;
    socket_server_c *server;
    socket_client_c *response;
    device_entry_t *device;
    pu32 sysError;    

    if((addr == NULL) || (count == NULL))
        return ER_PICMLX_CLI_PARAM_SIZE;
    // Clear available device list
    devices->clear();
    *count = 0;    
    server = new socket_server_c(SOCKET_TCP,SOCKET_BUFFER_SIZE);
    if(server == NULL)
        return ER_PICMLX_CLI_BROADCASTECHO_FAILED;
    client = new socket_client_c(1,SOCKET_UDP,SOCKET_BUFFER_SIZE,false);
    if(client == NULL) {
        delete server;
        return ER_PICMLX_CLI_BROADCASTECHO_FAILED;
    }
    response = NULL;
    if(server->setBlockingMode(false) && client->setBroadcastMode(true) && server->listen(listenPort,DEFAULT_LISTEN_BACKLOG)) {      
        client->writeUInt32(listenPort);
        if(client->sendTo(0,addr,listenPort)) {
            client->disconnect();
            if(((ps32)timeout) <= 0)
                timeout = 500;
            timeout += getTickCount();
            SLEEPEX(10,FALSE);
            while((timeout > getTickCount()) || (response != NULL)) {
                while((response = server->accept()) != NULL) {
                    sysError = GetLastError();
                    if(response != NULL) {
                        if(response->recv()) 
                            if(response->readUInt32() == 0)
                                if(response->readUInt32() + 2 * INT_SIZE == response->getRecvBytesCount()) {
                                    device = new device_entry_t;
                                    device->CardCount = response->readUInt32();
                                    device->OpenedCardCount = response->readUInt32();
                                    device->ClientCount = response->readUInt32();
                                    device->ListenPort = response->readUInt32();
                                    response->readString(device->Description,sizeof(device->Description));
                                    response->getPeerName(device->IP,sizeof(device->IP));
                                    if(devices->search(device) == NULL)
                                        devices->insert(device);
                                }
                    }
                    response->disconnect();
                    delete response;
                    response = NULL;            
                }
            }        
        }      
        *count = devices->getCount();      
    }
    client->disconnect();
    delete client;
    server->disconnect();
    delete server;
    return NO_ERROR;
}

pu32 picmlx_module_c::getDevice(pu32 index,device_entry_t *device) {
    device_entry_t *item;

    if(device == NULL) {
        return ER_PICMLX_CLI_PARAM_SIZE;
    }
    if(((ps32)index) < 0)
        return ER_PICMLX_CLI_INVALID_INDEX;
    if((item = devices->getDevice(index)) != NULL) {
        memcpy(device,item,sizeof(device_entry_t));      
    } else
        return ER_PICMLX_CLI_INVALID_INDEX;
    return NO_ERROR;
}

pu32 picmlx_module_c::getDeviceEx(pu32 index,pu32 *listenPort,pu32 *cardCount,pu32 *clientCount,pu32 *opennedCardCount,
    pchar *desc,pu32 descSize,pchar *addr,pu32 addrSize) {
        device_entry_t item = {0};
        pu32 result;

        if(!(result = getDevice(index,&item))) {
            if(listenPort != NULL) *listenPort = item.ListenPort;
            if(cardCount != NULL) *cardCount = item.CardCount;
            if(clientCount != NULL) *clientCount = item.ClientCount;
            if(opennedCardCount != NULL) *opennedCardCount = item.OpenedCardCount;
            if((desc != NULL) && (descSize > 0)) strcpy_s(desc,min(descSize,sizeof(item.Description) + 1),item.Description);
            if((addr != NULL) && (addrSize > 0)) strcpy_s(addr,min(addrSize,sizeof(item.IP) + 1),item.IP);
        }
        return result;
}
#endif // #ifndef LABVIEWRT

/* ##################################################################################################### */

/* ServerBridge functions */

//regular expressions:
//find: {DWORD} PICMLX_{[a-zA-Z_]*}\({[a-zA-Z ,\*_\[\]0-9]*}\);

//replace: \1 picmlx_module_c::\2(\3) {\n\tregister pu32 result;\n\tsession_data_s sesData = {0};\n\n\t/* DEBUG RECORD BEGIN */\n\t#ifdef DEBUG\n\t\t__DEBUGMSG(FALSE,"picmlx_module_c::\2 - begin");\n\t#endif\n\t/* DEBUG RECORD END */\n\tresult = this->beginTransmition(SID,&sesData);\n\tif(!result) {\n\t\tif(sesData.remote) {\n\t\t\tif(sesData.connected) {\n\t\t\t\tif(!this->sendPacket(PICMLX_FID_\2, \3)) {\n\t\t\t\t\tsesData.lastError = this->recvPacket(NULL, \3);\n\t\t\t\t}\n\t\t\t} else {\n\t\t\t\tsesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;\n\t\t\t}\n\t\t} else {\n\t\t\tsesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;\n\t\t}\n\t}\n\tthis->endTransmition(&sesData);\n\t/* DEBUG RECORD BEGIN */\n\t#ifdef DEBUG\n\t\t__DEBUGMSG(TRUE,"picmlx_module_c::\2 - end");\n\t#endif\n\t/* DEBUG RECORD END */\n\treturn result;\n}\n

pu32 picmlx_module_c::deviceLock(session_t sid,pu32 timeout) {
    register pu32 result;
    session_data_s sesData = {0};

    result = this->beginTransmition(sid,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_SB_LOCK,"u",timeout)) {
                    sesData.lastError = this->recvPacket(NULL,NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {      
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = this->endTransmition(&sesData);
    }
    return result;
}

pu32 picmlx_module_c::deviceUnlock(session_t sid) {
    register pu32 result;
    session_data_s sesData = {0};

    result = this->beginTransmition(sid,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_SB_UNLOCK,NULL)) {
                    sesData.lastError = this->recvPacket(NULL,NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {      
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = this->endTransmition(&sesData);
    }
    return result;
}

pu32 picmlx_module_c::getCardCountType(session_t sid,pu32 type) {
    register pu32 result;
    session_data_s sesData = {0};

    result = this->beginTransmition(sid,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_SB_CARDCOUNTTYPE,"u",type)) {
                    sesData.lastError = this->recvPacket(NULL,"u",&result);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {      
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = this->endTransmition(&sesData) ? INVALID_RESULT : result;
    } else
        return INVALID_RESULT;
    return result;
}

pu32 picmlx_module_c::sbVersion(session_t sid) {
    register pu32 result;
    session_data_s sesData = {0};

    result = this->beginTransmition(sid,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_SB_VERSION,NULL)) {
                    sesData.lastError = this->recvPacket(NULL,"u",&result);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {      
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = this->endTransmition(&sesData) ? INVALID_RESULT : result;
    } else
        result = INVALID_RESULT;
    return result;
}

DWORD picmlx_module_c::GetTotalCardsCount(SESSION SID, DWORD *count) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetTotalCardsCount - begin");
#endif
    /* DEBUG RECORD END */
    if(!count)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_TOTAL_CARDS_COUNT, NULL)) {
                    sesData.lastError = this->recvPacket(NULL, "u", count);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetTotalCardsCount - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetTotalOpenedCards(SESSION SID, DWORD *count) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetTotalOpenedCards - begin");
#endif
    /* DEBUG RECORD END */
    if(!count)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_TOTAL_OPENED_CARDS, NULL)) {
                    sesData.lastError = this->recvPacket(NULL, "u", count);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetTotalOpenedCards - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetTotalModulesCount(SESSION SID, DWORD *count) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetTotalModulesCount - begin");
#endif
    /* DEBUG RECORD END */
    if(!count)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_TOTAL_MODULES_COUNT, NULL)) {
                    sesData.lastError = this->recvPacket(NULL, "u", count);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetTotalModulesCount - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetSessionsCount(SESSION SID, DWORD *count) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetSessionsCount - begin");
#endif
    /* DEBUG RECORD END */
    if(!count)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_SESSIONS_COUNT, NULL)) {
                    sesData.lastError = this->recvPacket(NULL, "u", count);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetSessionsCount - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetSessionsCountByCard(SESSION SID, DWORD cardType, DWORD card, DWORD *count) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetSessionsCountByCard - begin");
#endif
    /* DEBUG RECORD END */
    if(!count)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_SESSIONS_BY_CARD, "uu",cardType, card)) {
                    sesData.lastError = this->recvPacket(NULL, "u", count);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetSessionsCountByCard - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::UseAnotherSession(SESSION SID, DWORD session, CHAR *token, DWORD length) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::UseAnotherSession - begin");
#endif
    /* DEBUG RECORD END */
    if(!token || !length)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_USE_ANOTHER_SESSION, "uac", session, length, token)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::UseAnotherSession - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::ReleaseAnotherSession(SESSION SID, DWORD session) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::ReleaseAnotherSession - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_RELEASE_ANOTHER_SESSION, "u", session)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::ReleaseAnotherSession - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::ReleaseForUse(SESSION SID, DWORD* session, CHAR *token, DWORD length) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::ReleaseForUse - begin");
#endif
    /* DEBUG RECORD END */
    if(!token || !length)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_RELEASE_FOR_USE, "u", length)) {
                    sesData.lastError = this->recvPacket(NULL, "uac", session, length, token);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::ReleaseForUse - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::UseCard(SESSION SID, DWORD cardType, DWORD card, DWORD exclusive) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::UseCard - begin");
#endif
    /* DEBUG RECORD END */
    if(exclusive < 1 || exclusive > 3)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_USE_CARD, "uuu", cardType, card, exclusive)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::UseCard - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::UseCards(SESSION SID, DWORD cardType, DWORD exclusive) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::UseCards - begin");
#endif
    /* DEBUG RECORD END */
    if(exclusive < 1 || exclusive > 3)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_USE_CARDS, "uu", cardType, exclusive)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::UseCards - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::ReleaseCard(SESSION SID, DWORD cardType, DWORD card) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::ReleaseCard - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_RELEASE_CARD, "uu", cardType, card)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::ReleaseCard - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::ReleaseCards(SESSION SID, DWORD cardType) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::ReleaseCards - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_RELEASE_CARDS, "u", cardType)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::ReleaseCards - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::HasExclusiveAccess(SESSION SID, DWORD cardType, DWORD card, DWORD ownerType, DWORD *isExclusive) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::HasExclusiveAccess - begin");
#endif
    /* DEBUG RECORD END */
    if(!isExclusive)
        return ER_PICMLX_CLI_PARAM_SIZE;
    if(ownerType < 1 || ownerType > 7)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_HAS_EXCLUSIVE_ACCESS, "uuu", cardType, card, ownerType)) {
                    sesData.lastError = this->recvPacket(NULL, "u", isExclusive);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::HasExclusiveAccess - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::IsCardUsed(SESSION SID, DWORD cardType, DWORD card, DWORD ownerType, pu32* isUsed) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::IsCardUsed - begin");
#endif
    /* DEBUG RECORD END */
    if(!isUsed)
        return ER_PICMLX_CLI_PARAM_SIZE;
    if(ownerType < 1 || ownerType > 7)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_IS_CARD_USED, "uuu", cardType, card, ownerType)) {
                    sesData.lastError = this->recvPacket(NULL, "u", isUsed);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::IsCardUsed - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetConnectedClients(SESSION SID, CHAR *clients, DWORD length) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetConnectedClients - begin");
#endif
    /* DEBUG RECORD END */
    if(!clients || !length)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_CONNECTED_HOSTS, "u", length)) {
                    sesData.lastError = this->recvPacket(NULL, "ac", length, clients);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetConnectedClients - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetConnectedCardClients(SESSION SID, DWORD cardType, DWORD card, CHAR *clients, DWORD length) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetConnectedCardClients - begin");
#endif
    /* DEBUG RECORD END */
    if(!clients || !length)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_CONNECTED_CARD_HOSTS, "uuu", cardType, card, length)) {
                    sesData.lastError = this->recvPacket(NULL, "ac", length, clients);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetConnectedCardClients - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetUsedCards(SESSION SID, DWORD cardType, DWORD *cards, DWORD *numCards) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetUsedCards - begin");
#endif
    /* DEBUG RECORD END */
    if(!cards || !numCards || !*numCards)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_USED_CARDS, "uu", cardType, *numCards)) {
                    sesData.lastError = this->recvPacket(NULL, "pu", numCards, cards);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetUsedCards - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetUsableCards(SESSION SID, DWORD cardType, DWORD *cards, DWORD *numCards) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetUsableCards - begin");
#endif
    /* DEBUG RECORD END */
    if(!cards || !numCards || !*numCards)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_USABLE_CARDS, "uu", cardType, *numCards)) {
                    sesData.lastError = this->recvPacket(NULL, "pu", numCards, cards);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetUsableCards - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetForeignSessions(SESSION SID, DWORD *sessions, DWORD *numSessions) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetForeignSessions - begin");
#endif
    /* DEBUG RECORD END */
    if(!sessions || !numSessions || !*numSessions)
        return ER_PICMLX_CLI_PARAM_SIZE;
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_FOREIGN_SESSIONS, "u", *numSessions)) {
                    sesData.lastError = this->recvPacket(NULL, "pu", numSessions, sessions);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
        result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetForeignSessions - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetAllSessions(SESSION SID, DWORD *sessions, CHAR ipaddresses[][16], DWORD *numSessions) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetAllSessions - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_GET_ALL_SESSIONS, "uu", *numSessions, (*numSessions)*16)) {
                    sesData.lastError = this->recvPacket(NULL, "puac", numSessions, sessions, (*numSessions)*16, ipaddresses);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetAllSessions - end");
#endif
    /* DEBUG RECORD END */
    return result;
}


DWORD picmlx_module_c::KillSession(SESSION SID, SESSION session) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::KillSession - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
                if(!this->sendPacket(PICMLX_FID_KILL_SESSION, "u", session)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::KillSession - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetChassisVoltages(SESSION SID, double *voltages, DWORD *numVoltages) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetChassisVoltages - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
				if(!this->sendPacket(PICMLX_FID_GET_CHASSIS_VOLTAGES, "u", *numVoltages)) {
                    sesData.lastError = this->recvPacket(NULL, "pd", numVoltages, voltages);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetChassisVoltages - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetChassisTemperatures(SESSION SID, DWORD *temperatures, DWORD *numTemperatures) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetChassisTemperatures - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
				if(!this->sendPacket(PICMLX_FID_GET_CHASSIS_TEMPERATURES, "u", *numTemperatures)) {
                    sesData.lastError = this->recvPacket(NULL, "pu", numTemperatures, temperatures);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetChassisTemperatures - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::GetChassisFanSpeeds(SESSION SID, DWORD *fanSpeeds, DWORD *numFanSpeeds) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::GetChassisFanSpeeds - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
				if(!this->sendPacket(PICMLX_FID_GET_CHASSIS_FANSPEEDS, "u", *numFanSpeeds)) {
                    sesData.lastError = this->recvPacket(NULL, "pu", numFanSpeeds, fanSpeeds);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::GetChassisFanSpeeds - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::SetChassisFanSpeeds(SESSION SID, DWORD maxSpeed) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::SetChassisFanSpeeds - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
				if(!this->sendPacket(PICMLX_FID_SET_CHASSIS_FANSPEEDS, "u", maxSpeed)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::SetChassisFanSpeeds - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::PowerCycle(SESSION SID, DWORD mode) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::PowerCycle - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
				if(!this->sendPacket(PICMLX_FID_POWER_CYCLE, "u", mode)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::PowerCycle - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::Identify(SESSION SID, DWORD identify) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::Identify - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
				if(!this->sendPacket(PICMLX_FID_IDENTIFY, "u", identify)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::Identify - end");
#endif
    /* DEBUG RECORD END */
    return result;
}

DWORD picmlx_module_c::StoreStates(SESSION SID, DWORD action) {
    register pu32 result;
    session_data_s sesData = {0};

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"picmlx_module_c::StoreStates - begin");
#endif
    /* DEBUG RECORD END */
    result = this->beginTransmition(SID,&sesData);
    if(!result) {
        if(sesData.remote) {
            if(sesData.connected) {
				if(!this->sendPacket(PICMLX_FID_STORESTATES, "u", action)) {
                    sesData.lastError = this->recvPacket(NULL, NULL);
                }
            } else {
                sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
            }
        } else {
            sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
        }
		result = sesData.lastError;
    }
    this->endTransmition(&sesData);
    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(TRUE,"picmlx_module_c::StoreStates - end");
#endif
    /* DEBUG RECORD END */
    return result;
}
