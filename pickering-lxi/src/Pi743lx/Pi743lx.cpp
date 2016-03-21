// Pi743lx.cpp : Defines the exported functions for the DLL application.

#include <System.h>
#include <SafetyLock.h>
#include <Pi743lx_low.h>
#include <Pi743lxModule.h>
#include <PicmlxModuleAPI.h>
#include <stdio.h>

#define PI743LX_MODULE_VERSION 103

/* ##################################################################################################### */

/* Global variables */

safety_lock_c *__libPi743lx_Guard = NULL;
pu32 __libPi743lx_Reference = 0;
pbool __libPi743lx_debugEnabled = FALSE;
pi743lx_module_c *__libPi743lx_Module = NULL;

#ifdef WIN32
	handle_t __libPi743_Instance;

	#define PI743LX_LIBRARY_FILE "pi743.dll"
#else // #ifdef WIN32
	#define PI743LX_LIBRARY_FILE "libpi743.so"
#endif

/* ##################################################################################################### */

/* DLL entry point and linux initialization functions */

BOOL APIENTRY Pi743lx_DllMain(handle_t handle,pu32 reason,void *reserved) {	
	switch(reason) {
		/* Process attached to DLL */
		case DLL_PROCESS_ATTACH:
			if(__libPi743lx_Reference == 0) {
				/* DEBUG RECORD BEGIN */
				#ifdef DEBUG
					__DEBUGMSG(FALSE,"Pi743lx library initialization, reference: %d",__libPi743lx_Reference);
				#endif				
				/* DEBUG RECORD END */
				#ifdef WIN32
			__libPi743_Instance = handle;
				#endif			 
				__libPi743lx_Guard = new safety_lock_c();
				__libPi743lx_Module = new pi743lx_module_c(PI743LX_LIBRARY_FILE);
				__libPi743lx_Reference++;
			}
			break;
		/* Process detached to DLL */
		case DLL_PROCESS_DETACH:
			/* DEBUG RECORD BEGIN */
			#ifdef DEBUG
				__DEBUGMSG(FALSE,"\nPi743lx library deinitialization, reference: %d",__libPi743lx_Reference);
			#endif
			/* DEBUG RECORD END */
			if(__libPi743lx_Reference == 1) {
				delete __libPi743lx_Module;
				delete __libPi743lx_Guard;
				/* DEBUG RECORD END */
				__libPi743lx_Reference--;
			}				
			break;
	};
	return TRUE;
}

#if(defined(WIN32) || defined(LABVIEWRT))

BOOL APIENTRY DllMain(handle_t handle,pu32 reason,void *reserved) {
	return Pi743lx_DllMain(handle,reason,reserved);
}

#elif LINUX

/**
 * These functions are used in Linux system for initiallizing the library. This is similar to
 * DllMain function in Windows DLL files which is called automaticly by system.
 * PIPLX_Init and PIPLX_Free are there for backward application compability.
 */


// Initialize library resources
void __attribute__((constructor)) PI743LX_InitializeLibrary() {
	Pi743lx_DllMain(NULL,DLL_PROCESS_ATTACH,NULL);
}

void PI743LX_Init() {}

// Free library resources
void __attribute__((destructor)) PI743LX_FinalizeLibrary() {
	Pi743lx_DllMain(NULL,DLL_PROCESS_DETACH,NULL);
}

void PI743LX_Free() {}
	
#endif // LINUX

/* ##################################################################################################### */

//functions
//regular expressions:
//DWORD PI743LX_API {[a-zA-Z_743]*} *\({[a-zA-Z ,\*_\[\]0-9]*}\);

//DWORD PI743LX_API \1(\2) {\n  register DWORD result;\n\n  /* DEBUG RECORD BEGIN */\n  #ifdef DEBUG\n    __DEBUGMSG(FALSE,"\\n+++ \1 +++");\n  #endif\n  /* DEBUG RECORD END */\n  __libPi743lx_Guard->lock();\n  result = __libPi743lx_Module->\1(\2);\n  __libPi743lx_Guard->unlock();\n  return result;\n}\n

void PI743LX_API PI743LX_GetVersion(DWORD *Version) {
	*Version = PI743LX_MODULE_VERSION;
}

BOOL PI743LX_API PI743LX_IsError(SESSION Sid) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_IsError +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->isError(Sid);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetLastErrorCode(SESSION Sid,DWORD *ErrorCode) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetLastErrorCode +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->getLastError(Sid, ErrorCode);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetLastErrorMessage(SESSION Sid,LPCHAR ErrorMsg,DWORD Length) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetLastErrorMessage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->getLastErrorMessage(Sid, ErrorMsg, Length);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ErrorCodeToMessage(SESSION Sid, DWORD ErrorCode,LPCHAR ErrorMsg,DWORD Length) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ErrorCodeToMessage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->errorCode2Message(Sid, ErrorCode, ErrorMsg, Length);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_ClearOverCurrentStatus(SESSION Sid, DWORD CardHandle) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ClearOverCurrentStatus +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ClearOverCurrentStatus(Sid, CardHandle);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ClearTriggerStatus(SESSION Sid, DWORD CardHandle) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ClearTriggerStatus +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ClearTriggerStatus(Sid, CardHandle);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_close(SESSION Sid, DWORD CardHandle) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_close +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->close(Sid, CardHandle);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetAddressCounter(SESSION Sid, DWORD CardHandle, DWORD* Addresscounter) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetAddressCounter +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetAddressCounter(Sid, CardHandle, Addresscounter);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetClockDivider(SESSION Sid, DWORD CardHandle, DWORD* Divider) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetClockDivider +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetClockDivider(Sid, CardHandle, Divider);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetMeasuredOutputCurrent(SESSION Sid, DWORD CardHandle, double* Current) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetMeasuredOutputCurrent +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetMeasuredOutputCurrent(Sid, CardHandle, Current);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetMeasuredOutputVoltage(SESSION Sid, DWORD CardHandle, double* Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetMeasuredOutputVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetMeasuredOutputVoltage(Sid, CardHandle, Voltage);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetOutputAll(SESSION Sid, DWORD CardHandle, double *Voltage, double *Current, double *Power, DWORD *Connection) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetOutputAll +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetOutputAll(Sid, CardHandle, Voltage, Current, Power, Connection);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetOutputConnection(SESSION Sid, DWORD CardHandle, DWORD* Connection) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetOutputConnection +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetOutputConnection(Sid, CardHandle, Connection);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetOutputCurrentLimit(SESSION Sid, DWORD CardHandle, double* Current) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetOutputCurrentLimit +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetOutputCurrentLimit(Sid, CardHandle, Current);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetOutputVoltage(SESSION Sid, DWORD CardHandle, double* Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetOutputVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetOutputVoltage(Sid, CardHandle, Voltage);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetOverCurrentStatus(SESSION Sid, DWORD CardHandle, DWORD* OverCurrent) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetOverCurrentStatus +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetOverCurrentStatus(Sid, CardHandle, OverCurrent);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetPowerSupplyStatus(SESSION Sid, DWORD CardHandle, DWORD* Psstatus) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetPowerSupplyStatus +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetPowerSupplyStatus(Sid, CardHandle, Psstatus);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetTrigger(SESSION Sid, DWORD CardHandle, DWORD* Source, DWORD* Action, DWORD* Level, DWORD *Mode) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetTrigger +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetTrigger(Sid, CardHandle, Source, Action, Level, Mode);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetTriggerCurrent(SESSION Sid, DWORD CardHandle, double* Current) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetTriggerCurrent +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetTriggerCurrent(Sid, CardHandle, Current);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetTriggerStatus(SESSION Sid, DWORD CardHandle, DWORD* Triggerstatus) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetTriggerStatus +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetTriggerStatus(Sid, CardHandle, Triggerstatus);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_GetTriggerVoltage(SESSION Sid, DWORD CardHandle, double* Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_GetTriggerVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->GetTriggerVoltage(Sid, CardHandle, Voltage);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_init(SESSION Sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD *CardHandle) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_init +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->initCard(Sid, resourceDescription, IDQuery, reset,  CardHandle);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_MemoryTest(SESSION Sid, DWORD CardHandle, DWORD LogSize, DWORD *Erroraddress, DWORD *Errordata, DWORD *Expectdata, DWORD *FoundErrors) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_MemoryTest +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->MemoryTest(Sid, CardHandle, LogSize, Erroraddress, Errordata, Expectdata, FoundErrors);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ReadCapturedCurrent(SESSION Sid, DWORD CardHandle, double* Current, DWORD Count) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadCapturedCurrent +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadCapturedCurrent(Sid, CardHandle, Current, Count);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ReadCapturedVoltage(SESSION Sid, DWORD CardHandle, double* Voltages, DWORD Count) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadCapturedVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadCapturedVoltage(Sid, CardHandle, Voltages, Count);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ReadCurrentCalValues(SESSION Sid, DWORD CardHandle, double* OffsetCurrentcdac, double* GainCurrentcdac, DWORD* Offsetcodecadc, DWORD* Gaincodecadc) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadCurrentCalValues +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadCurrentCalValues(Sid, CardHandle, OffsetCurrentcdac, GainCurrentcdac, Offsetcodecadc, Gaincodecadc);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ReadID(SESSION Sid, DWORD CardHandle, DWORD* Id) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadID +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadID(Sid, CardHandle, Id);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ReadMaximumPower(SESSION Sid, DWORD CardHandle, double* Power) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadMaximumPower +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadMaximumPower(Sid, CardHandle, Power);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_ReadVoltageCalValues(SESSION Sid, DWORD CardHandle, double* OffsetVoltagevdac, double* GainVoltagevdac, DWORD* Offsetcodevadc,DWORD* Gaincodevadc) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadVoltageCalValues +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadVoltageCalValues(Sid, CardHandle, OffsetVoltagevdac, GainVoltagevdac, Offsetcodevadc, Gaincodevadc);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_reset(SESSION Sid, DWORD CardHandle) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_reset +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->reset(Sid, CardHandle);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_self_test(SESSION Sid, DWORD CardHandle, BOOL* TestResult, CHAR ErrorMessage[], DWORD size) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_self_test +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->self_test(Sid, CardHandle, TestResult, ErrorMessage, size);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetAddressCounter(SESSION Sid, DWORD CardHandle, DWORD Addresscounter) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetAddressCounter +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetAddressCounter(Sid, CardHandle, Addresscounter);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetClockDivider(SESSION Sid, DWORD CardHandle, DWORD Divider) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetClockDivider +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetClockDivider(Sid, CardHandle, Divider);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetOutputAll(SESSION Sid, DWORD CardHandle, double Voltage, double Current, double Power, DWORD Connection) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetOutputAll +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetOutputAll(Sid, CardHandle, Voltage, Current, Power, Connection);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetOutputConnection(SESSION Sid, DWORD CardHandle, DWORD Connection) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetOutputConnection +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetOutputConnection(Sid, CardHandle, Connection);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetOutputCurrentLimit(SESSION Sid, DWORD CardHandle, double Current) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetOutputCurrentLimit +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetOutputCurrentLimit(Sid, CardHandle, Current);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetOutputVoltage(SESSION Sid, DWORD CardHandle, double Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetOutputVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetOutputVoltage(Sid, CardHandle, Voltage);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetPowerSupplyStatus(SESSION Sid, DWORD CardHandle, DWORD Psstatus) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetPowerSupplyStatus +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetPowerSupplyStatus(Sid, CardHandle, Psstatus);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetSoftwareTrigger(SESSION Sid, DWORD CardHandle) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetSoftwareTrigger +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetSoftwareTrigger(Sid, CardHandle);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetTrigger(SESSION Sid, DWORD CardHandle, DWORD Source, DWORD Action, DWORD Level, DWORD Mode) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetTrigger +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetTrigger(Sid, CardHandle, Source, Action, Level, Mode);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetTriggerCurrent(SESSION Sid, DWORD CardHandle, double Current) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetTriggerCurrent +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetTriggerCurrent(Sid, CardHandle, Current);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_SetTriggerVoltage(SESSION Sid, DWORD CardHandle, double Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_SetTriggerVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->SetTriggerVoltage(Sid, CardHandle, Voltage);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_StoreCurrentCalValues(SESSION Sid, DWORD CardHandle, double OffsetCurrent, double GainCurrentunsigned, DWORD Offsetcode, DWORD Gaincode) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_StoreCurrentCalValues +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->StoreCurrentCalValues(Sid, CardHandle, OffsetCurrent, GainCurrentunsigned, Offsetcode, Gaincode);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_StoreVoltageCalValues(SESSION Sid, DWORD CardHandle, double OffsetVoltage, double GainVoltage, DWORD Offsetcode, DWORD Gaincode) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_StoreVoltageCalValues +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->StoreVoltageCalValues(Sid, CardHandle, OffsetVoltage, GainVoltage, Offsetcode, Gaincode);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_WriteMaximumPower(SESSION Sid, DWORD CardHandle, double Power) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WriteMaximumPower +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WriteMaximumPower(Sid, CardHandle, Power);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_FindInstruments(SESSION Sid, DWORD Buslist[], DWORD Devicelist[], DWORD *Count) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_FindInstruments +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->FindInstruments(Sid, Buslist, Devicelist, Count);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_FindInstrumentsRsrc(SESSION Sid, CHAR Rsrclist[][20], DWORD *Count) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_FindInstrumentsRsrc +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->FindInstrumentsRsrc(Sid, Rsrclist, Count);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_IsStabilized(SESSION Sid, DWORD CardHandle, BOOL *Stabilized) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_IsStabilized +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->IsStabilized(Sid, CardHandle, Stabilized);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_WaitForStabilized(SESSION Sid, DWORD CardHandle, DWORD Timeout) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WaitForStabilized +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WaitForStabilized(Sid, CardHandle, Timeout);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_WriteMemory(SESSION Sid, DWORD CardHandle, DWORD* Memorydata, DWORD Length) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WriteMemory +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WriteMemory(Sid, CardHandle, Memorydata, Length);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_Version(SESSION Sid, DWORD *Version) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_Version +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->Version(Sid, Version);
  __libPi743lx_Guard->unlock();
  return result;
}


DWORD PI743LX_API PI743LX_initCardForRead(SESSION Sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD *CardHandle) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_initCardForRead +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->initCardForRead(Sid, resourceDescription, IDQuery, reset,  CardHandle);
  __libPi743lx_Guard->unlock();
  return result;
}
/*
DWORD PI743LX_API PI743LX_initCardsForRead(SESSION Sid, BOOL IDQuery, BOOL reset,  DWORD *Count) {
  register DWORD result;

  // DEBUG RECORD BEGIN 
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_initCardsForRead +++");
  #endif
  // DEBUG RECORD END 
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->initCardsForRead(Sid, IDQuery, reset,  Count);
  __libPi743lx_Guard->unlock();
  return result;
}
*/

//Low functions
DWORD PI743LX_API PI743LX_ReadAdc(SESSION sid, DWORD CardHandle, DWORD select, DWORD averages, DWORD *data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadAdc +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadAdc(sid, CardHandle, select, averages, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_ReadCurrentDac(SESSION sid, DWORD CardHandle, DWORD *data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadCurrentDac +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadCurrentDac(sid, CardHandle, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_ReadEeprom(SESSION sid, DWORD CardHandle, DWORD address, DWORD *data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadEeprom +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadEeprom(sid, CardHandle, address, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_ReadOutputDac(SESSION sid, DWORD CardHandle, DWORD *data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadOutputDac +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadOutputDac(sid, CardHandle, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_ReadRegister(SESSION sid, DWORD CardHandle, DWORD offset, DWORD *data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_ReadRegister +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->ReadRegister(sid, CardHandle, offset, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_WriteCurrentDac(SESSION sid, DWORD CardHandle, DWORD data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WriteCurrentDac +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WriteCurrentDac(sid, CardHandle, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_WriteEeprom(SESSION sid, DWORD CardHandle, DWORD address, DWORD data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WriteEeprom +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WriteEeprom(sid, CardHandle, address, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_WriteID(SESSION sid, DWORD CardHandle, DWORD id) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WriteID +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WriteID(sid, CardHandle, id);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_WriteOutputDac(SESSION sid, DWORD CardHandle, DWORD data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WriteOutputDac +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WriteOutputDac(sid, CardHandle, data);
  __libPi743lx_Guard->unlock();
  return result;
}

DWORD PI743LX_API PI743LX_WriteRegister(SESSION sid, DWORD CardHandle, DWORD offset, DWORD data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PI743LX_WriteRegister +++");
  #endif
  /* DEBUG RECORD END */
  __libPi743lx_Guard->lock();
  result = __libPi743lx_Module->WriteRegister(sid, CardHandle, offset, data);
  __libPi743lx_Guard->unlock();
  return result;
}

