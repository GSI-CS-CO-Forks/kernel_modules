#include <System.h>
#include <Piplx.h>
#include <PiplxBIRST.h>
#include <SafetyLock.h>
#include <PiplxModule.h>
#include <stdio.h>

#define PIPLX_MODULE_VERSION 161

/* ##################################################################################################### */

/* Global variables */

safety_lock_c *__libPiplx_Guard = NULL;
piplx_module_c *__libPiplx_Module = NULL;
pu32 __libPiplx_Reference = 0;
pbool __libPiplx_debugEnabled = FALSE;

#ifdef WIN32
  handle_t __libPiplx_Instance;

  #define PILPXI_LIBRARY_FILE "pilpxi.dll"
#else // #ifdef WIN32
  #define PILPXI_LIBRARY_FILE "libpilpxi.so"
#endif

/* ##################################################################################################### */

/* Exported DLL functions */

// Version function

DWORD PIPLX_API PIPLX_GetVersion(void) {
  return PIPLX_MODULE_VERSION;
}

/* ##################################################################################################### */

/* Error handling functions */

BOOL PIPLX_API PIPLX_IsError(SESSION Sid) {
  register BOOL result;

  __libPiplx_Guard->lock();
  result = __libPiplx_Module->isError(Sid);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_GetLastErrorCode(SESSION Sid,DWORD *ErrorCode) {
  register DWORD result;

  __libPiplx_Guard->lock();
  result = __libPiplx_Module->getLastError(Sid,ErrorCode);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_GetLastErrorMessage(SESSION Sid,LPCHAR ErrorMsg,DWORD Length) {
  register DWORD result;

  __libPiplx_Guard->lock();
  result = __libPiplx_Module->getLastErrorMessage(Sid,ErrorMsg,Length);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ErrorCodeToMessage(DWORD ErrorCode,LPCHAR ErrorMsg,DWORD Length) {
  register DWORD result;

  __libPiplx_Guard->lock();
  result = __libPiplx_Module->errorCode2Message(ErrorCode,ErrorMsg,Length);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Initialize and close */

DWORD PIPLX_API PIPLX_CloseCards(SESSION Sid) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_CloseCards +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->closeCards(Sid);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_CloseSpecifiedCard(SESSION Sid,DWORD CardNum) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_CloseSpecifiedCard +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->closeSpecifiedCard(Sid,CardNum);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_OpenCards(SESSION Sid) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_OpenCards +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->openCards(Sid);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_OpenCardsForRead(SESSION Sid) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_OpenCardsForRead +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->openCardsForRead(Sid);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_OpenSpecifiedCard(SESSION Sid,DWORD Bus,DWORD Slot,DWORD *CardNum) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_OpenSpecifiedCard +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->openSpecifiedCard(Sid,Bus,Slot,CardNum);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_OpenSpecifiedCardForRead(SESSION Sid,DWORD Bus,DWORD Slot,DWORD *CardNum) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_OpenSpecifiedCardForRead +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->openSpecifiedCardForRead(Sid,Bus,Slot,CardNum);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* information and status */

DWORD PIPLX_API PIPLX_CardId(SESSION Sid,DWORD CardNum,LPCHAR Str,DWORD StrLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_CardId +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->cardID(Sid,CardNum,Str,StrLen);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_CardLoc(SESSION Sid,DWORD CardNum,DWORD *Bus,DWORD *Slot) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_CardLoc +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->cardLoc(Sid,CardNum,Bus,Slot);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ClosureLimit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Limit) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ClosureLimit +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->closureLimit(Sid,CardNum,OutSub,Limit);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_CardOpsCount(SESSION Sid,DWORD CardNum,DWORD *Count) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_CardOpsCount +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->cardOpsCount(Sid,CardNum,Count);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_CountFreeCards(SESSION Sid,DWORD *NumCards) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_CountFreeCards +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->countFreeCards(Sid,NumCards);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_Diagnostic(SESSION Sid,DWORD CardNum,LPCHAR Str,DWORD StrLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_Diagnostic +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->diagnostic(Sid,CardNum,Str,StrLen);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_EnumerateSubs(SESSION Sid,DWORD CardNum,DWORD *InSubs,DWORD *OutSubs) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_EnumerateSubs +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->enumerateSubs(Sid,CardNum,InSubs,OutSubs);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_FindFreeCards(SESSION Sid,DWORD NumCards,DWORD *BusList,DWORD *SlotList) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_FindFreeCards +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->findFreeCards(Sid,NumCards,BusList,SlotList);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_SettleTime(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Time) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_SettleTime +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->settleTime(Sid,CardNum,OutSub,Time);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_Status(SESSION Sid,DWORD CardNum) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_Status +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->status(Sid,CardNum);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_StatusCodeToMessage(DWORD StatusCode,LPCHAR StatusMsg,DWORD Length) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_StatusCodeToMessage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->statusCode2Message(StatusCode,StatusMsg,Length);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_SubAttribute(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,DWORD AttrCode,DWORD *AttrValue) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_SubAttribute +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->subAttribute(Sid,CardNum,SubNum,Out,AttrCode,AttrValue);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_SubInfo(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,DWORD *TypeNum,DWORD *Rows,DWORD *Cols) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_SubInfo +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->subInfo(Sid,CardNum,SubNum,Out,TypeNum,Rows,Cols);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_SubStatus(SESSION Sid,DWORD CardNum,DWORD SubNum) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_SubStatus +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->subStatus(Sid,CardNum,SubNum);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_SubType(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,LPCHAR Str,DWORD StrLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_SubType +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->subType(Sid,CardNum,SubNum,Out,Str,StrLen);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_Version(SESSION Sid) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_Version +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->version(Sid);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* General purpose output */

DWORD PIPLX_API PIPLX_ClearAll(SESSION Sid) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ClearAll +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->clearAll(Sid);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ClearCard(SESSION Sid,DWORD CardNum) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ClearCard +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->clearCard(Sid,CardNum);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ClearSub(SESSION Sid,DWORD CardNum,DWORD OutSub) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ClearSub +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->clearSub(Sid,CardNum,OutSub);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_OpBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL Action) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_OpBit +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->opBit(Sid,CardNum,OutSub,BitNum,Action);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ViewBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL *State) {
  register DWORD result;  

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ViewBit +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->viewBit(Sid,CardNum,OutSub,BitNum,(ps32*)State);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ViewSub(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Data,DWORD DataLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ViewSub +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->viewSub(Sid,CardNum,OutSub,Data,DataLen);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_WriteSub(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Data,DWORD DataLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_WriteSub +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->writeSub(Sid,CardNum,OutSub,Data,DataLen);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ClearMask(SESSION Sid,DWORD CardNum,DWORD OutSub) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ClearMask +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->clearMask(Sid,CardNum,OutSub);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_MaskBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL Action) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_MaskBit +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->maskBit(Sid,CardNum,OutSub,BitNum,Action);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_MaskCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Col,BOOL Action) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_MaskCrosspoint +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->maskCrosspoint(Sid,CardNum,OutSub,Row,Col,Action);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_OpCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Col,BOOL Action) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_OpCrosspoint +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->opCrosspoint(Sid,CardNum,OutSub,Row,Col,Action);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_OpSwitch(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD SwitchFunc,DWORD SegNum,DWORD SwitchNum,
															 DWORD SubSwitch,DWORD SwitchAction,BOOL *State) {
  register DWORD result;  

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_OpSwitch +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->opSwitch(Sid,CardNum,OutSub,SwitchFunc,SegNum,SwitchNum,SubSwitch,SwitchAction,(ps32*)State);  
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ViewCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Col,BOOL *State) {
  register DWORD result;  

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ViewCrosspoint +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->viewCrosspoint(Sid,CardNum,OutSub,Row,Col,(ps32*)State);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ViewMask(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Data,DWORD DataLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ViewMask +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->viewMask(Sid,CardNum,OutSub,Data,DataLen);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ViewMaskBit(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD BitNum,BOOL *State) {
  register DWORD result;  

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ViewMaskBit +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->viewMaskBit(Sid,CardNum,OutSub,BitNum,(ps32*)State);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ViewMaskCrosspoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Row,DWORD Col,BOOL *State) {
  register DWORD result;  

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ViewMaskCrosspoint +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->viewMaskCrosspoint(Sid,CardNum,OutSub,Row,Col,(ps32*)State);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_WriteMask(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Data,DWORD DataLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_WriteMask +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->writeMask(Sid,CardNum,OutSub,Data,DataLen);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Input */

DWORD PIPLX_API PIPLX_ReadBit(SESSION Sid,DWORD CardNum,DWORD InSub,DWORD BitNum,BOOL *State) {
  register DWORD result;  

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ReadBit +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->readBit(Sid,CardNum,InSub,BitNum,(ps32*)State);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ReadSub(SESSION Sid,DWORD CardNum,DWORD InSub,DWORD *Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ReadSub +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->readSub(Sid,CardNum,InSub,Data,1);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ReadInputSub(SESSION Sid,DWORD CardNum,DWORD InSub,DWORD *Data,DWORD DataLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ReadSubEx +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->readSub(Sid,CardNum,InSub,Data,DataLen);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Calibration */

DWORD PIPLX_API PIPLX_ReadCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Idx,DWORD *Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ReadCal +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->readCal(Sid,CardNum,OutSub,Idx,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_WriteCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Idx,DWORD Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_WriteCal +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->writeCal(Sid,CardNum,OutSub,Idx,Data);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Floating-point calibration data functions */

DWORD PIPLX_API PIPLX_WriteCalFP(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD Offset,DWORD NumValues,double *Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_WriteCalFP +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->writeCalFP(Sid,CardNum,OutSub,Store,Offset,NumValues,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ReadCalFP(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD Offset,DWORD NumValues,double *Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ReadCalFP +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->readCalFP(Sid,CardNum,OutSub,Store,Offset,NumValues,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_WriteCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD Interval) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_WriteCalDate +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->writeCalDate(Sid,CardNum,OutSub,Store,Interval);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ReadCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Store,DWORD *Year,DWORD *Day,DWORD *Interval) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ReadCalDate +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->readCalDate(Sid,CardNum,OutSub,Store,Year,Day,Interval);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_SetCalPoint(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Index) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_SetCalPoint +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->setCalPoint(Sid,CardNum,OutSub,Index);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Programmable attenuator */

DWORD PIPLX_API PIPLX_AttenGetAttenuation(SESSION Sid,DWORD CardNum,DWORD SubNum,float *Atten) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_AttenGetAttenuation +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->attenGetAttenuation(Sid,CardNum,SubNum,Atten);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_AttenInfo(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *TypeNum,DWORD *NumSteps,float *StepSize) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_Atteninfo +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->attenInfo(Sid,CardNum,SubNum,TypeNum,NumSteps,StepSize);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_AttenPadValue(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD PadNum,float *Atten) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_AttenPadValue +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->attenPadValue(Sid,CardNum,SubNum,PadNum,Atten);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_AttenSetAttenuation(SESSION Sid,DWORD CardNum,DWORD SubNum,float Atten) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_AttenSetAttenuation +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->attenSetAttenuation(Sid,CardNum,SubNum,Atten);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_AttenType(SESSION Sid,DWORD CardNum,DWORD SubNum,LPCHAR Str,DWORD StrLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_AttenType +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->attenType(Sid,CardNum,SubNum,Str,StrLen);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Power supply */

DWORD PIPLX_API PIPLX_PsuEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL State) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_PsuEnable +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->psuEnable(Sid,CardNum,SubNum,(pbool)State);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_PsuGetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_PsuGetVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->psuGetVoltage(Sid,CardNum,SubNum,Voltage);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_PsuInfo(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *TypeNum,double *Voltage,double *Current,
															DWORD *Precis,DWORD *Capabilities) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_PsuInfo +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->psuInfo(Sid,CardNum,SubNum,TypeNum,Voltage,Current,Precis,Capabilities);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_PsuSetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_PsuSetVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->psuSetVoltage(Sid,CardNum,SubNum,Voltage);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_PsuType(SESSION Sid,DWORD CardNum,DWORD SubNum,LPCHAR Str,DWORD StrLen) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_PsuType +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->psuType(Sid,CardNum,SubNum,Str,StrLen);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Mode control */

DWORD PIPLX_API PIPLX_SetMode(SESSION Sid,DWORD ModeFlags) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_SetMode +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->setMode(Sid,ModeFlags);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Programmable resistor specific functions */

DWORD PIPLX_API PIPLX_ResSetResistance(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Mode,double Resistance) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ResSetResistance +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->resSetResistance(Sid,CardNum,OutSub,Mode,Resistance);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ResGetResistance(SESSION Sid,DWORD CardNum,DWORD OutSub,double *Resistance) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ResGetResistance +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->resGetResistance(Sid,CardNum,OutSub,Resistance);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_ResInfo(SESSION Sid,DWORD CardNum,DWORD OutSub,double *MinRes,double *MaxRes,
                              double *RefRes,double *PrecPC,double *PrecDelta,double *Int1,
                              double *IntDelta,DWORD *Capabilities) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_ResInfo +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->resInfo(Sid,CardNum,OutSub,MinRes,MaxRes,RefRes,PrecPC,PrecDelta,Int1,IntDelta,Capabilities);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* BIRST specific functions */

DWORD PIPLXBIRST_API PIPLX_BirstCapabilities(SESSION Sid,DWORD CardNum,DWORD *Topology,DWORD *Capabilities,DWORD *RefRes) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstCapabilities +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstCapabilities(Sid,CardNum,Topology,Capabilities,RefRes);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstInfo(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Poles,CHAR *Axis1,
                                     DWORD *Line1,CHAR *Axis2,DWORD *Line2) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstInfo +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstInfo(Sid,CardNum,OutSub,Poles,Axis1,Line1,Axis2,Line2);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstSetControls(SESSION Sid,DWORD CardNum,DWORD Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstSetControls +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstSetControls(Sid,CardNum,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstGetControls(SESSION Sid,DWORD CardNum,DWORD *Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstGetControls +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstGetControls(Sid,CardNum,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstMeasurement(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Value) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstMeasurement +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstMeasurement(Sid,CardNum,OutSub,Value);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstWriteCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Pole,DWORD Offset,
                                         DWORD NumValues,DWORD *Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstWriteCal +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstWriteCal(Sid,CardNum,OutSub,Pole,Offset,NumValues,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstReadCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Pole,DWORD Offset,
                                        DWORD NumValues,DWORD *Data) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstReadCal +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstReadCal(Sid,CardNum,OutSub,Pole,Offset,NumValues,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstWriteCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstWriteCalDate +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstWriteCalDate(Sid,CardNum,OutSub);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstReadCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Year,DWORD *Day) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstReadCalDate +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstReadCalDate(Sid,CardNum,OutSub,Year,Day);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstGetAttribute(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,DWORD AttrCode,DWORD *AttrValue) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BirstAttribute +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstGetAttribute(Sid,CardNum,SubNum,Out,AttrCode,(pu32*)AttrValue);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstSetControlsEx(SESSION Sid,DWORD CardNum,DWORD CtrlReg,DWORD Data)
{
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_Birst +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstSetControlsEx(Sid,CardNum,CtrlReg,Data);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLXBIRST_API PIPLX_BirstGetControlsEx(SESSION Sid,DWORD CardNum,DWORD CtrlReg,DWORD *Data)
{
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_Birst +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->birstGetControlsEx(Sid,CardNum,CtrlReg,Data);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */

/* Battery simulator functions */

DWORD PIPLX_API PIPLX_BattGetCurrent(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Current) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BattGetCurrent +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->battGetCurrent(Sid,CardNum,SubNum,Current);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_BattGetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *Pattern) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BattGetEnable +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->battGetEnable(Sid,CardNum,SubNum,Pattern);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_BattGetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BattGetVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->battGetVoltage(Sid,CardNum,SubNum,Voltage);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_BattReadInterlockState(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL *Interlock) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BattReadInterlockState +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->battReadInterlockState(Sid,CardNum,SubNum,(ps32*)Interlock);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_BattSetCurrent(SESSION Sid,DWORD CardNum,DWORD SubNum,double Current) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BattSetCurrent +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->battSetCurrent(Sid,CardNum,SubNum,Current);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_BattSetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD Pattern) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BattSetEnable +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->battSetEnable(Sid,CardNum,SubNum,Pattern);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_BattSetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_BattSetVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->battSetVoltage(Sid,CardNum,SubNum,Voltage);
  __libPiplx_Guard->unlock();
  return result;
}

/* ##################################################################################################### */
/* Voltage Source (VSOURCE type) specific functions. */

DWORD PIPLX_API PIPLX_VsourceSetRange(SESSION Sid,DWORD CardNum,DWORD SubNum,double Range) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_VsourceSetRange +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->vsourceSetRange(Sid,CardNum,SubNum,Range);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_VsourceGetRange(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Range) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_VsourceGetRange +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->vsourceGetRange(Sid,CardNum,SubNum,Range);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_VsourceSetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_VsourceSetVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->vsourceSetVoltage(Sid,CardNum,SubNum,Voltage);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_VsourceGetVoltage(SESSION Sid,DWORD CardNum,DWORD SubNum,double *Voltage) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_VsourceGetVoltage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->vsourceGetVoltage(Sid,CardNum,SubNum,Voltage);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_VsourceSetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD Pattern) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_VsourceSetEnable +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->vsourceSetEnable(Sid,CardNum,SubNum,Pattern);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_VsourceGetEnable(SESSION Sid,DWORD CardNum,DWORD SubNum,DWORD *Pattern) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_VsourceGetEnable +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->vsourceGetEnable(Sid,CardNum,SubNum,Pattern);
  __libPiplx_Guard->unlock();
  return result;
}

DWORD PIPLX_API PIPLX_GetStatusMessage(SESSION Sid, DWORD CardNum, LPCHAR StatusMsg, DWORD Length) {
  register DWORD result;

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"\n+++ PIPLX_GetStatusMessage +++");
  #endif
  /* DEBUG RECORD END */
  __libPiplx_Guard->lock();
  result = __libPiplx_Module->getStatusMessage(Sid, CardNum, StatusMsg, Length);
  __libPiplx_Guard->unlock();
  return result;
}


/* ##################################################################################################### */

/* DLL entry point and linux initialization functions */

BOOL APIENTRY Piplx_DllMain(handle_t handle,pu32 reason,void *reserved) {  
  switch(reason) {
    /* Process attached to DLL */
    case DLL_PROCESS_ATTACH:
      if(__libPiplx_Reference == 0) {
        /* DEBUG RECORD BEGIN */
        #ifdef DEBUG
          //__DEBUG_MEM_USAGE();
          __DEBUGMSG(FALSE,"Piplx library initialization, reference: %d",__libPiplx_Reference);
        #endif				
        /* DEBUG RECORD END */
        #ifdef WIN32
          __libPiplx_Instance = handle;
        #endif			 
        __libPiplx_Guard = new safety_lock_c();
        __libPiplx_Module = new piplx_module_c(PILPXI_LIBRARY_FILE);
        __libPiplx_Reference++;
      }        
      break;
    /* Process detached to DLL */
    case DLL_PROCESS_DETACH:
      /* DEBUG RECORD BEGIN */
      #ifdef DEBUG
        __DEBUGMSG(FALSE,"\nPiplx library deinitialization, reference: %d",__libPiplx_Reference);
      #endif
      /* DEBUG RECORD END */
      if(__libPiplx_Reference == 1) {        
        delete __libPiplx_Module;
        delete __libPiplx_Guard;
        /* DEBUG RECORD BEGIN */
        #ifdef DEBUG
          //__DEBUG_MEM_USAGE();        
        #endif
        /* DEBUG RECORD END */
        __libPiplx_Reference--;
      }        
      break;
  };
  return TRUE;
}

#if(defined(WIN32) || defined(LABVIEWRT))

  BOOL APIENTRY DllMain(handle_t handle,pu32 reason,void *reserved) {
    return Piplx_DllMain(handle,reason,reserved);
  }

#elif LINUX
	
/**
 * These functions are used in Linux system for initiallizing the library. This is similar to
 * DllMain function in Windows DLL files which is called automaticly by system.
 * PIPLX_Init and PIPLX_Free are there for backward application compability.
 */

// Initialize library resources
  void __attribute__((constructor)) PIPLX_InitializeLibrary() {
    Piplx_DllMain(NULL,DLL_PROCESS_ATTACH,NULL);
  }

  void PIPLX_Init() {}

  // Free library resources
  void __attribute__((destructor)) PIPLX_FinalizeLibrary() {
    Piplx_DllMain(NULL,DLL_PROCESS_DETACH,NULL);
  }

  void PIPLX_Free() {}
	
#endif // LINUX
