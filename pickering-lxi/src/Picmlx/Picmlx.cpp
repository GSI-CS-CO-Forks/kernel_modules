#include <System.h>
#include <Picmlx.h>
#include <SafetyLock.h>
#include <PicmlxModule.h>
#include <PicmlxModuleAPI.h>
#include <PicmlxResources.h>
#include <DebugTool.h>

#define PICMLX_MODULE_VERSION 183

/* ##################################################################################################### */

/* Global variables */

safety_lock_c *__libPicmlx_Guard = NULL;
picmlx_module_c *__libPicmlx_Module = NULL;
pu32 __libPicmlx_Reference = 0;

/*#ifdef _DEBUGTOOL
pbool __libPicmlx_debugEnabled = FALSE;
handle_t __libPicmlx_debugLib = NULL;
#endif // #ifdef _DEBUGTOOL*/

#ifdef WIN32
handle_t __libPicmlx_Instance;
#endif // #ifdef WIN32

#ifdef LABVIEWRT
ViPSession __defaultResourceManager = NULL;
#endif // #ifdef LABVIEWRT

/* ##################################################################################################### */

/* Exported DLL functions */

// Version function

DWORD PICMLX_API PICMLX_GetVersion(void) {
    return PICMLX_MODULE_VERSION;
}

/* ##################################################################################################### */

//functions
//regular expressions:
//DWORD PICMLX_API {[a-zA-Z_]*} *\({[a-zA-Z ,\*_\[\]0-9]*}\);

//DWORD PICMLX_API \1(\2) {\n  register DWORD result;\n\n  /* DEBUG RECORD BEGIN */\n  #ifdef DEBUG\n    __DEBUGMSG(FALSE,"\\n+++ \1 +++");\n  #endif\n  /* DEBUG RECORD END */\n  __libPicmlx_Guard->lock();\n  result = __libPicmlx_Module->\1(\2);\n  __libPicmlx_Guard->unlock();\n  return result;\n}\n

// Communication functions

DWORD PICMLX_API PICMLX_Connect(DWORD Board,LPCHAR Address,DWORD Port,DWORD Timeout,SESSION *SID) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_Connect +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->connect(Board,Address,Port,Timeout,SID);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_Disconnect(SESSION SID) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_Disconnect +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->disconnect(SID);
    __libPicmlx_Guard->unlock();
    return result;
}

/* ##################################################################################################### */

// Error handling functions

BOOL PICMLX_API PICMLX_IsError(SESSION SID) {
    register BOOL result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_IsError +++ ");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->isError(SID);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetLastErrorCode(SESSION SID,DWORD *Code) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetLastErrorCode +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->getLastError(SID,Code);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetLastErrorMessage(SESSION SID,LPCHAR ErrorMsg,DWORD Length) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetLastErrorMessage +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->getLastErrorMessage(SID,ErrorMsg,Length);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_ErrorCodeToMessage(DWORD ErrorCode,LPCHAR ErrorMsg,DWORD Length) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_ErrorCodeToMessage +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->errorCode2Message(ErrorCode,ErrorMsg,Length);
    __libPicmlx_Guard->unlock();
    return result;
}

/* ##################################################################################################### */

// Another module develop API

DWORD PICMLX_API PICMLX_BeginTransmition(SESSION sid,session_data_s *data) {
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n--- PICMLX_BeginTransmition ---");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();  
    return __libPicmlx_Module->beginTransmition(sid,data);
}

DWORD PICMLX_API PICMLX_EndTransmition(const session_data_s *data) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n--- PICMLX_EndTransmition ---");
#endif
    /* DEBUG RECORD END */
    result = __libPicmlx_Module->endTransmition(data);  
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetOption(option_name_e option,int *value) {
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n--- PICMLX_GetOption ---");
#endif
    /* DEBUG RECORD END */
    return __libPicmlx_Module->getOption(option,value);
}

DWORD PICMLX_API PICMLX_RecvPacket(convert_func_t *func,const pchar *format,...) {
    va_list args;
    DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n--- PICMLX_RecvPacket ---");
#endif
    /* DEBUG RECORD END */
    va_start(args,format);
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"__libPicmlx_Module ptr: 0x%X",__libPicmlx_Module);
#endif
    /* DEBUG RECORD END */
    result = __libPicmlx_Module->va_recvPacket(func,format,args);
    va_end(args);
    return result;
}

DWORD PICMLX_API PICMLX_SendPacket(pu32 command,const pchar *format,...) {
    va_list args;
    DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n--- PICMLX_SendPacket ---");
#endif
    /* DEBUG RECORD END */
    va_start(args,format);  
    result = __libPicmlx_Module->va_sendPacket(command,format,args);
    va_end(args);
    return result;
}

DWORD PICMLX_API PICMLX_SetOption(option_name_e option,int value) {
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n--- PICMLX_SetOption ---");
#endif
    /* DEBUG RECORD END */
    return __libPicmlx_Module->setOption(option,value);
}

/* ##################################################################################################### */

/* Discovery functions */

DWORD PICMLX_API PICMLX_EchoDirect(LPCHAR Address,DWORD Port,DWORD Timeout,LPLXIENTRY LXIInfo) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_EchoDirect +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->echoDirect(Address,Port,Timeout,LXIInfo);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_EchoDirectEx(LPCHAR Address,DWORD Port,DWORD Timeout,DWORD *ListenPort,
    DWORD *CardCount,DWORD *ClientCount,DWORD *OpenCardCount,
    LPCHAR Description,DWORD DescLength,
    LPCHAR LXIAddress,DWORD LXIAddrLength) {
        register DWORD result;

        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"\n+++ PICMLX_EchoDirectEx +++");
#endif
        /* DEBUG RECORD END */
        __libPicmlx_Guard->lock();
        result = __libPicmlx_Module->echoDirectEx(Address,Port,Timeout,ListenPort,CardCount,ClientCount,OpenCardCount,
            Description,DescLength,LXIAddress,LXIAddrLength);
        __libPicmlx_Guard->unlock();
        return result;
}

#ifndef LABVIEWRT

DWORD PICMLX_API PICMLX_EchoBroadcast(DWORD ListenPort,DWORD Timeout,DWORD* AvailableLXICount) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_EchoBroadcast +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->echoBroadcast(ListenPort,Timeout,AvailableLXICount);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_EchoBroadcastEx(LPCHAR Address,DWORD ListenPort,DWORD Timeout,DWORD* AvailableLXICount) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_EchoBroadcastEx +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->echoBroadcastEx(Address,ListenPort,Timeout,AvailableLXICount);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetAvailableLXICount(void) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetAvailableLXICount +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->getDeviceCount();
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetAvailableLXIEntry(DWORD Index,LPLXIENTRY LXIInfo) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetAvailableLXIEntry +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->getDevice(Index,LXIInfo);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetAvailableLXIEntryEx(DWORD Index,DWORD *ListenPort,DWORD *CardCount,
    DWORD *ClientCount,DWORD *OpenCardCount,
    LPCHAR Description,DWORD DescLength,
    LPCHAR LXIAddress,DWORD LXIAddrLength) {
        register DWORD result;

        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"\n+++ PICMLX_GetAvailbaleLXIEntryEx +++");
#endif
        /* DEBUG RECORD END */
        __libPicmlx_Guard->lock();
        result = __libPicmlx_Module->getDeviceEx(Index,ListenPort,CardCount,ClientCount,OpenCardCount,Description,
            DescLength,LXIAddress,LXIAddrLength);
        __libPicmlx_Guard->unlock();
        return result;
}

#endif // #ifndef LABVIEWRT

/* ##################################################################################################### */

/* ServerBridge control functions */

DWORD PICMLX_API PICMLX_SbVersion(SESSION SID) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_SbVersion +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->sbVersion(SID);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetCountCardType(SESSION SID,DWORD Type) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetCountCardType +++");  
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->getCardCountType(SID,Type);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_DeviceLock(SESSION SID,DWORD Timeout) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_DeviceLock +++");  
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->deviceLock(SID,Timeout);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_DeviceUnlock(SESSION SID) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"\n+++ PICMLX_DeviceUnlock +++");  
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->deviceUnlock(SID);
    __libPicmlx_Guard->unlock();
    return result;
}

/* ##################################################################################################### */

/* DLL entry point and linux initialization functions */

BOOL APIENTRY Picmlx_DllMain(handle_t handle,pu32 reason,void *reserved) {
    switch(reason) {
        /* Process attached to DLL */
    case DLL_PROCESS_ATTACH:
        if(__libPicmlx_Reference == 0) {
            //VLDEnable();
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUG_INIT();
            //__DEBUG_MEM_USAGE();
            __DEBUGMSG(FALSE,"Picmlx library initialization, reference: %d",__libPicmlx_Reference);
#endif
            /* DEBUG RECORD END */
#if defined(WIN32) && !defined(LABVIEWRT)
            WSADATA wsaData = {0};
            WSAStartup(MAKEWORD(2,2),&wsaData);          
#elif LABVIEWRT // #if defined(WIN32) && !defined(LABVIEWRT)
            __defaultResourceManager = new ViSession;
            viOpenDefaultRM(__defaultResourceManager);
#endif // #elif LABVIEWRT
#ifdef WIN32
            __libPicmlx_Instance = handle;
#endif // #ifdef WIN32
            __libPicmlx_Guard = new safety_lock_c();
            __libPicmlx_Module = new picmlx_module_c(NULL);
            __libPicmlx_Reference++;
        }        
        break;
        /* Process detached to DLL */
    case DLL_PROCESS_DETACH:
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"\nPicmlx library deinitialization, reference: %d",__libPicmlx_Reference);
#endif
        /* DEBUG RECORD END */
        if(__libPicmlx_Reference == 1) {        
            delete __libPicmlx_Module;
            delete __libPicmlx_Guard;
#if defined(WIN32) && !defined(LABVIEWRT)
            WSACleanup();
#elif LABVIEWRT // #if defined(WIN32) && !defined(LABVIEWRT)
            viClose(*__defaultResourceManager);
            delete __defaultResourceManager;
#endif // LABVIEWRT
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            //__DEBUG_MEM_USAGE();
            __DEBUG_FREE();
#endif
            /* DEBUG RECORD END */
            __libPicmlx_Reference--;        
            //VLDDisable();
        }        
        break;
    };
    return TRUE;
}

DWORD PICMLX_API PICMLX_GetTotalCardsCount(SESSION SID, DWORD *count) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetTotalCardsCount +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetTotalCardsCount(SID, count);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetTotalOpenedCards(SESSION SID, DWORD *count) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetTotalOpenedCards +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetTotalOpenedCards(SID, count);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetTotalModulesCount(SESSION SID, DWORD *count) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetTotalModulesCount +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetTotalModulesCount(SID, count);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetSessionsCount(SESSION SID, DWORD *count) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetSessionsCount +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetSessionsCount(SID, count);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetCardSessionsCount(SESSION SID, DWORD cardType, DWORD card, DWORD *count) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetSessionsCountByCard +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetSessionsCountByCard(SID, cardType, card, count);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_UseForeignSession(SESSION SID, DWORD session, CHAR *token, DWORD length) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_UseAnotherSession +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->UseAnotherSession(SID, session, token, length);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_ReleaseForeignSession(SESSION SID, DWORD session) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_ReleaseAnotherSession +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->ReleaseAnotherSession(SID, session);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetActiveSession(SESSION SID, DWORD* session, CHAR *token, DWORD length) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_ReleaseForUse +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->ReleaseForUse(SID, session, token, length);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_UseCard(SESSION SID, DWORD cardType, DWORD card, DWORD exclusive) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_UseCard +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->UseCard(SID, cardType, card, exclusive);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_UseCards(SESSION SID, DWORD cardType, DWORD exclusive) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_UseCards +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->UseCards(SID, cardType, exclusive);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_ReleaseCard(SESSION SID, DWORD cardType, DWORD card) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_ReleaseCard +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->ReleaseCard(SID, cardType, card);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_ReleaseCards(SESSION SID, DWORD cardType) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_ReleaseCards +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->ReleaseCards(SID, cardType);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_HasExclusiveAccess(SESSION SID, DWORD cardType, DWORD card, DWORD ownerType, BOOL* isExclusive) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_HasExclusiveAccess +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->HasExclusiveAccess(SID, cardType, card, ownerType, (DWORD*)isExclusive);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_IsCardUsed(SESSION SID, DWORD cardType, DWORD card, DWORD ownerType, BOOL* isUsed) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_IsCardUsed +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->IsCardUsed(SID, cardType, card, ownerType, (DWORD*)isUsed);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetConnectedClients(SESSION SID, CHAR *clients, DWORD length) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetConnectedClients +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetConnectedClients(SID, clients, length);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetConnectedCardClients(SESSION SID, DWORD cardType, DWORD card, CHAR *clients, DWORD length) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetConnectedCardClients +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetConnectedCardClients(SID, cardType, card, clients, length);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetUsedCards(SESSION SID, DWORD cardType, DWORD *cards, DWORD *numCards) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetUsedCards +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetUsedCards(SID, cardType, cards, numCards);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetUsableCards(SESSION SID, DWORD cardType, DWORD *cards, DWORD *numCards) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetUsableCards +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetUsableCards(SID, cardType, cards, numCards);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetForeignSessions(SESSION SID, DWORD *sessions, DWORD *numSessions) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetForeignSessions +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetForeignSessions(SID, sessions, numSessions);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetChassisVoltages(SESSION SID, double *voltages, DWORD *numVoltages) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetChassisVoltages +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
	result = __libPicmlx_Module->GetChassisVoltages(SID, voltages, numVoltages);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetChassisTemperatures(SESSION SID, DWORD *temperatures, DWORD *numTemperatures) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetChassisVoltages +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
	result = __libPicmlx_Module->GetChassisTemperatures(SID, temperatures, numTemperatures);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_GetChassisFanSpeeds(SESSION SID, DWORD *speeds, DWORD *numSpeeds) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetChassisFanSpeeds +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
	result = __libPicmlx_Module->GetChassisFanSpeeds(SID, speeds, numSpeeds);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_SetChassisFanSpeeds(SESSION SID, DWORD maxSpeed) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_SetChassisFanSpeeds +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
	result = __libPicmlx_Module->SetChassisFanSpeeds(SID, maxSpeed);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_PowerCycle(SESSION SID, DWORD mode) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_PowerCycle +++");
#endif
    /* DEBUG RECORD END */
	if( mode != 0 && mode != 1 )
		return ER_PICMLX_CLI_DATA_NOT_ACCEPT;

    __libPicmlx_Guard->lock();
	result = __libPicmlx_Module->PowerCycle(SID, mode);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_Identify(SESSION SID, DWORD identify) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_Identify +++");
#endif
    /* DEBUG RECORD END */
	if( identify != 0 && identify != 1 )
		return ER_PICMLX_CLI_DATA_NOT_ACCEPT;

    __libPicmlx_Guard->lock();
	result = __libPicmlx_Module->Identify(SID, identify);
    __libPicmlx_Guard->unlock();
    return result;
}

DWORD PICMLX_API PICMLX_StoreStates(SESSION SID, DWORD action) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_StoreStates +++");
#endif
    /* DEBUG RECORD END */
	if( action != 0 && action != 1 )
		return ER_PICMLX_CLI_DATA_NOT_ACCEPT;

    __libPicmlx_Guard->lock();
	result = __libPicmlx_Module->StoreStates(SID, action);
    __libPicmlx_Guard->unlock();
    return result;
}


/* ##################################################################################################### */
/* Private functions of communication module of client bridge */

DWORD PICMLX_API PICMLX_GetAllSessions(SESSION sid, DWORD *sessions, CHAR ipaddresses[][16], DWORD *numSessions) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_GetAllSessions +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->GetAllSessions(sid, sessions, ipaddresses, numSessions);
    __libPicmlx_Guard->unlock();
    return result;
}


DWORD PICMLX_API PICMLX_KillSession(SESSION sid, SESSION session) {
    register DWORD result;

    /* DEBUG RECORD BEGIN */
#ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PICMLX_KillSession +++");
#endif
    /* DEBUG RECORD END */
    __libPicmlx_Guard->lock();
    result = __libPicmlx_Module->KillSession(sid, session);
    __libPicmlx_Guard->unlock();
    return result;
}


#if(defined(WIN32) || defined(LABVIEWRT))

BOOL APIENTRY DllMain(handle_t handle,pu32 reason,void *reserved) {
    return Picmlx_DllMain(handle,reason,reserved); 
}

#elif LINUX    

/**
* These functions are used in Linux system for initiallizing the library. This is similar to
* DllMain function in Windows DLL files which is called automaticly by system.
* PICMLX_Init and PICMLX_Free are there for backward application compability.
*/

// Initialize library resources
void __attribute__((constructor)) PICMLX_InitializeLibrary() {
    Picmlx_DllMain(NULL,DLL_PROCESS_ATTACH,NULL);
}

void PICMLX_Init() {}

// Free library resources
void __attribute__((destructor)) PICMLX_FinalizeLibrary() {
    Picmlx_DllMain(NULL,DLL_PROCESS_DETACH,NULL);
}

void PICMLX_Free() {}

#endif // LINUX

