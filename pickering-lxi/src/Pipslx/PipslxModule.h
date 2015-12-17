/*******************************************************************************
 *  Pickering Power Sequencer module header file.
 *
 *  Copyright (C) 1988 - 2011 Pickering Interfaces Ltd.
 *  
 *  Author: Michal Heczko
 *  Contact: support@pickeringswitch.com
 *  
 *******************************************************************************/

#ifndef _PIPSLXMODULE_H_
#define _PIPSLXMODULE_H_

#include <System.h>
#include <Pipslx.h>
#include <BaseModule.h>
#include <PicmlxModuleAPI.h>

class pipslx_module_c : public base_module_c {
private:
  static pu32 convertServerError(pu32 code);
  pu32 error2Resource(pu32 code);
public:
  pipslx_module_c(const pchar *libName);
  ~pipslx_module_c();
public:
  // Error handling functions
  pbool isError(session_t sid);
  pu32 getLastError(session_t sid,pu32 *code);
  pu32 getLastErrorMessage(session_t sid,pchar *buffer,pu32 size);
  pu32 errorCode2Message(pu32 code,pchar *buffer,pu32 size);
  // power sequencer functions
  pu32 Open(session_t sid);
  pu32 OpenForRead(session_t sid);
  pu32 Close(session_t sid);
  pu32 GetChanState(session_t sid,pu32 Chan,pbool* State);
  pu32 SetChanState(session_t sid,pu32 Chan,pbool State);
  pu32 GetChanTime(session_t sid,pu32 Chan,pu32 TimeType,pu32 *Time);
  pu32 SetChanTime(session_t sid,pu32 Chan,pu32 TimeType,pu32 Time);
  pu32 GetChanEnabled(session_t sid,pu32 Chan,pbool *Enabled);
  pu32 SetChanEnabled(session_t sid,pu32 Chan,pbool Enabled);
  pu32 ChanTest(session_t sid,pu32* BadChans,pu32* BadChansLen);
  pu32 Sequence(session_t sid,pu32 SequenceType);
  pu32 SettleTime(session_t sid,pu32* Time);
  pu32 Shutdown(session_t sid);
  pu32 GetChanCount(session_t sid,pu32* ChanCount);
};

#endif // #ifdef _PIPSLXMODULE_H_
