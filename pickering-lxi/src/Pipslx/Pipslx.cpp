#include <System.h>
#include <Pipslx.h>
#include <SafetyLock.h>
#include <PipslxModule.h>

#define PIPSLX_MODULE_VERSION 132

/* ##################################################################################################### */

/* Global variables */

safety_lock_c *__libPipslx_Guard = NULL;
pipslx_module_c *__libPipslx_Module = NULL;
pu32 __libPipslx_Reference = 0;
pbool __libPipslx_debugEnabled = FALSE;

#ifdef WIN32
  handle_t __libPipslx_Instance;
#endif // #ifdef WIN32

/* ##################################################################################################### */

/* Exported DLL functions */

// Version function

DWORD PIPSLX_API PIPSLX_GetVersion(void) {
  return PIPSLX_MODULE_VERSION;
}

/* ##################################################################################################### */

// Error handling functions

BOOL PIPSLX_API PIPSLX_IsError(SESSION Session) {
  register BOOL result;

  __libPipslx_Guard->lock();
  result = __libPipslx_Module->isError(Session);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_GetLastErrorCode(SESSION Session,DWORD *Code) {
  register DWORD result;

  __libPipslx_Guard->lock();
  result = __libPipslx_Module->getLastError(Session,Code);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_GetLastErrorMessage(SESSION Session,LPCHAR ErrorMsg,DWORD Length) {
  register DWORD result;

  __libPipslx_Guard->lock();
  result = __libPipslx_Module->getLastErrorMessage(Session,ErrorMsg,Length);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_ErrorCodeToMessage(DWORD ErrorCode,LPCHAR ErrorMsg,DWORD Length) {
  register DWORD result;

  __libPipslx_Guard->lock();
  result = __libPipslx_Module->errorCode2Message(ErrorCode,ErrorMsg,Length);
  __libPipslx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

DWORD PIPSLX_API PIPSLX_Open(SESSION SID) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_Open +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->Open(SID);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_OpenForRead(SESSION SID) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_OpenForRead +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->OpenForRead(SID);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_Close(SESSION SID) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_Close +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->Close(SID);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_GetChanState(SESSION SID,DWORD Chan,BOOL* State) {
  register DWORD result;
  pbool state;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_GetChanState +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->GetChanState(SID,Chan,&state);
  *State = state;
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_SetChanState(SESSION SID,DWORD Chan,BOOL State) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_SetChanState +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->SetChanState(SID,Chan,State);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_GetChanTime(SESSION SID,DWORD Chan,DWORD TimeType,DWORD *Time) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_GetChanTime +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->GetChanTime(SID,Chan,TimeType,Time);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_SetChanTime(SESSION SID,DWORD Chan,DWORD TimeType,DWORD Time) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_SetChanTime +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->SetChanTime(SID,Chan,TimeType,Time);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_GetChanEnabled(SESSION SID,DWORD Chan,BOOL *Enabled) {
  register DWORD result;
  pbool state;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_GetChanEnabled +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->GetChanEnabled(SID,Chan,&state);
  *Enabled = state;
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_SetChanEnabled(SESSION SID,DWORD Chan,BOOL Enabled) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_SetChanEnabled +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->SetChanEnabled(SID,Chan,Enabled);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_ChanTest(SESSION SID,DWORD* BadChans,DWORD* BadChansLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_ChanTest +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->ChanTest(SID,BadChans,BadChansLen);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_Sequence(SESSION SID,DWORD SequenceType) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_Sequence +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module-> Sequence(SID,SequenceType);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_SettleTime(SESSION SID,DWORD* Time) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_SettleTime +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->SettleTime(SID,Time);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_Shutdown(SESSION SID) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_Shutdown +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->Shutdown(SID);
  __libPipslx_Guard->unlock();
  return result;
}

DWORD PIPSLX_API PIPSLX_GetChanCount(SESSION SID,DWORD* ChanCount) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPSLX_GetChanCount +++");
  #endif
  /* DEBUG RECORD END */
  __libPipslx_Guard->lock();
  result = __libPipslx_Module->GetChanCount(SID,ChanCount);
  __libPipslx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* DLL entry point and linux initialization functions */

BOOL Pipslx_DllMain(handle_t handle,pu32 reason,void *reserved) {
  switch(reason) {
    /* Process attached to DLL */
    case DLL_PROCESS_ATTACH:
      if(__libPipslx_Reference == 0) {
        // ----------------------------
        // - TODO: Insert debug stuff -
        // ----------------------------

        #ifdef WIN32
          __libPipslx_Instance = handle;
        #endif

        __libPipslx_Guard = new safety_lock_c();
        __libPipslx_Module = new pipslx_module_c(NULL);
        __libPipslx_Reference++;
      }        
      break;
    /* Process detached to DLL */
    case DLL_PROCESS_DETACH:
      if(__libPipslx_Reference == 1) {
        // ----------------------------
        // - TODO: Insert debug stuff -
        // ----------------------------
        delete __libPipslx_Module;
        delete __libPipslx_Guard;
        // ----------------------------
        // - TODO: Insert debug stuff -
        // ----------------------------
        __libPipslx_Reference--;
      }        
      break;
  };
  return TRUE;
}

#if(defined(WIN32) || defined(LABVIEWRT))

  BOOL APIENTRY DllMain(handle_t handle,pu32 reason,void *reserved) {
    return Pipslx_DllMain(handle,reason,reserved);
  }

#elif LINUX

/**
 * These functions are used in Linux system for initiallizing the library. This is similar to
 * DllMain function in Windows DLL files which is called automaticly by system.
 * PIPSLX_Init and PIPSLX_Free are there for backward application compability.
 */

  // Initialize library resources
  void __attribute__((constructor)) PIPSLX_InitializeLibrary() {
    Pipslx_DllMain(NULL,DLL_PROCESS_ATTACH,NULL);
  }

  void PIPSLX_Init() {}

  // Free library resources
  void __attribute__((destructor)) PIPSLX_FinalizeLibrary() {
    Pipslx_DllMain(NULL,DLL_PROCESS_DETACH,NULL);
  }

  void PIPSLX_Free() {}

#endif // LINUX
