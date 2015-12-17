/*******************************************************************************
*  Communication module header file.
*
*  Copyright (C) 1988 - 2011 Pickering Interfaces Ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _PICMLXMODULE_H_
#define _PICMLXMODULE_H_

#include <System.h>
#include <BaseModule.h>
#include <Session.h>
#include <Discovery.h>

class picmlx_module_c : public base_module_c {
private:
    session_list_c *sessions;
    pu32 connectCount;
    session_entry_s *activeSession;
    occupant_info_s *activeOccupant;
private:

#ifndef LABVIEWRT
    device_list_c *devices;    
#endif // #ifndef LABVIEWRT

private:
    session_t conjureUniqueSessionID();
    pu32 convertServerError(pu32 error,convert_func_t *func);
    pu32 error2Resource(pu32 error);
    pbool isRemote(pchar *address);
    pbool isSidExists(session_t sid);  
public:
    picmlx_module_c(const pchar *libName);
    ~picmlx_module_c();
    // Communication functions
    pu32 connect(pu32 board,pchar *address,pu32 port,pu32 timeout,session_t *sid);
    pu32 disconnect(session_t sid);
    // Error handling functions
    pbool isError(session_t sid);
    pu32 getLastError(session_t sid,pu32 *code);
    pu32 getLastErrorMessage(session_t sid,pchar *buffer,pu32 size);
    pu32 errorCode2Message(pu32 code,pchar *buffer,pu32 size);
    // Another module develop API
    pu32 beginTransmition(session_t sid,session_data_s *sessionData);
    pu32 endTransmition(const session_data_s *sessionData);  
    pbool getOption(pu32 option,int *value);
    pu32 recvPacket(convert_func_t *func,const pchar *format,...);
    pu32 va_recvPacket(convert_func_t *func,const pchar *format,va_list args);
    pu32 sendPacket(pu32 command,const pchar *format,...);
    pu32 va_sendPacket(pu32 command,const pchar *format,va_list args);
    pbool setOption(pu32 option,int value);
    // Discovery functions
    pu32 echoDirect(pchar *deviceAddr,pu32 port,pu32 timeout,device_entry_t *device);
    pu32 echoDirectEx(pchar *deviceAddr,pu32 port,pu32 timeout,pu32 *listenPort,pu32 *cardCount,pu32 *clientCount,
        pu32 *opennedCardCount,pchar *desc,pu32 descSize,pchar *addr,pu32 addrSize);
    // ServerBridge control functions
    pu32 deviceLock(session_t sid,pu32 timeout);
    pu32 deviceUnlock(session_t sid);
    pu32 getCardCountType(session_t sid,pu32 type);
    pu32 sbVersion(session_t sid);
public:

#ifndef LABVIEWRT
    pu32 echoBroadcast(pu32 listenPort,pu32 timeout,pu32 *count);
    pu32 echoBroadcastEx(const pchar *addr,pu32 listenPort,pu32 timeout,pu32 *count);
    pu32 getDeviceCount() { return devices->getCount(); }
    pu32 getDevice(pu32 index,device_entry_t *device);
    pu32 getDeviceEx(pu32 index,pu32 *listenPort,pu32 *cardCount,pu32 *clientCount,pu32 *opennedCardCount,
        pchar *desc,pu32 descSize,pchar *addr,pu32 addrSize);
#endif // #ifndef LABVIEWRT
    pu32 GetTotalCardsCount(session_t sid, pu32 *count);
    pu32 GetTotalOpenedCards(session_t sid, pu32 *count);
    pu32 GetTotalModulesCount(session_t sid, pu32 *count);
    pu32 GetSessionsCount(session_t sid, pu32 *count);
    pu32 GetSessionsCountByCard(session_t sid, pu32 cardType, pu32 card, pu32 *count);
    pu32 UseAnotherSession(session_t sid, pu32 session, pchar *token, pu32 length);
    pu32 ReleaseAnotherSession(session_t sid, pu32 session);
    pu32 ReleaseForUse(session_t sid, pu32* session, pchar *token, pu32 length);
    pu32 UseCard(session_t sid, pu32 cardType, pu32 card, pu32 exclusive);
    pu32 UseCards(session_t sid, pu32 cardType, pu32 exclusive);
    pu32 ReleaseCard(session_t sid, pu32 cardType, pu32 card);
    pu32 ReleaseCards(session_t sid, pu32 cardType);
    pu32 HasExclusiveAccess(session_t sid, pu32 cardType, pu32 card, pu32 ownerType, pu32* isExclusive);
    pu32 IsCardUsed(session_t sid, pu32 cardType, pu32 card, pu32 ownerType, pu32* isUsed);
    pu32 GetConnectedClients(session_t sid, pchar *clients, pu32 length);
    pu32 GetConnectedCardClients(session_t sid, pu32 cardType, pu32 card, pchar *clients, pu32 length);
    pu32 GetUsedCards(session_t sid, pu32 cardType, pu32 *cards, pu32 *numCards);
    pu32 GetUsableCards(session_t sid, pu32 cardType, pu32 *cards, pu32 *numCards);
    pu32 GetForeignSessions(session_t sid, pu32 *sessions, pu32 *numSessions);
    pu32 GetAllSessions(SESSION sid, DWORD *sessions, CHAR ipaddresses[][16], DWORD *numSessions);
    pu32 KillSession(session_t sid, session_t session);
	pu32 GetChassisVoltages(session_t sid, double *voltages, pu32 *numVoltages);
	pu32 GetChassisTemperatures(session_t sid, pu32 *temperatures, pu32 *numTemperatures);
	pu32 GetChassisFanSpeeds(session_t sid, pu32 *fanSpeeds, pu32 *numFanSpeeds);
	pu32 SetChassisFanSpeeds(session_t sid, pu32 maxSpeed);
	pu32 PowerCycle(session_t sid, pu32 mode);
	pu32 Identify(session_t sid, pu32 identify);
	pu32 StoreStates(session_t sid, pu32 action);
};

#endif // #ifndef _PICMLXMODULE_H_
