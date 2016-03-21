/*******************************************************************************
 *  Pickering switching module header file.
 *
 *  Copyright (C) 1988 - 2011 Pickering Interfaces Ltd.
 *  
 *  Author: Michal Heczko
 *  Contact: support@pickeringswitch.com
 *  
 *******************************************************************************/

#ifndef _PIPLXMODULE_H_
#define _PIPLXMODULE_H_

#include <System.h>
#include <Piplx.h>
#include <BaseModule.h>
#include <PicmlxModuleAPI.h>

/* Pilpxi library function prototypes */

// Initialize and close
typedef void (_stdcall *pil_closecards_f)(void);
typedef pu32 (_stdcall *pil_closespecifiedcard_f)(pu32);
typedef pu32 (_stdcall *pil_opencards_f)(void);
typedef pu32 (_stdcall *pil_openspecifiedcard_f)(pu32,pu32,pu32*);

// Information and status
typedef pu32 (_stdcall *pil_cardid_f)(pu32,pchar*);
typedef pu32 (_stdcall *pil_cardloc_f)(pu32,pu32*,pu32*);
typedef pu32 (_stdcall *pil_closurelimit_f)(pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_cardopscount_f)(pu32,pu32*);
typedef pu32 (_stdcall *pil_countfreecards_f)(pu32*);
typedef pu32 (_stdcall *pil_diagnostic_f)(pu32,pchar*);
typedef pu32 (_stdcall *pil_enumeratesubs_f)(pu32,pu32*,pu32*);
typedef pu32 (_stdcall *pil_findfreecards_f)(pu32,pu32*,pu32*);
typedef pu32 (_stdcall *pil_settletime_f)(pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_status_f)(pu32);
typedef pu32 (_stdcall *pil_subattribute_f)(pu32,pu32,ps32,pu32,pu32*);
typedef pu32 (_stdcall *pil_subinfo_f)(pu32,pu32,ps32,pu32*,pu32*,pu32*);
typedef pu32 (_stdcall *pil_substatus_f)(pu32,pu32);
typedef pu32 (_stdcall *pil_subtype_f)(pu32,pu32,ps32,pchar*);
typedef pu32 (_stdcall *pil_version_f)(void);

// General purpose output

typedef pu32 (_stdcall *pil_clearall_f)(void);
typedef pu32 (_stdcall *pil_clearcard_f)(pu32);
typedef pu32 (_stdcall *pil_clearsub_f)(pu32,pu32);
typedef pu32 (_stdcall *pil_opbit_f)(pu32,pu32,pu32,ps32);
typedef pu32 (_stdcall *pil_viewbit_f)(pu32,pu32,pu32,ps32*);
typedef pu32 (_stdcall *pil_viewsub_f)(pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_writesub_f)(pu32,pu32,pu32*);

// Input

typedef pu32 (_stdcall *pil_readbit_f)(pu32,pu32,pu32,ps32*);
typedef pu32 (_stdcall *pil_readsub_f)(pu32,pu32,pu32*);

// Switching

typedef pu32 (_stdcall *pil_clearmask_f)(pu32,pu32);
typedef pu32 (_stdcall *pil_maskbit_f)(pu32,pu32,pu32,ps32);
typedef pu32 (_stdcall *pil_maskcrosspoint_f)(pu32,pu32,pu32,pu32,ps32);
typedef pu32 (_stdcall *pil_opcrosspoint_f)(pu32,pu32,pu32,pu32,ps32);
typedef pu32 (_stdcall *pil_opswitch_f)(pu32,pu32,pu32,pu32,pu32,pu32,pu32,ps32*);
typedef pu32 (_stdcall *pil_viewcrosspoint_f)(pu32,pu32,pu32,pu32,ps32*);
typedef pu32 (_stdcall *pil_viewmask_f)(pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_viewmaskbit_f)(pu32,pu32,pu32,ps32*);
typedef pu32 (_stdcall *pil_viewmaskcrosspoint_f)(pu32,pu32,pu32,pu32,ps32*);
typedef pu32 (_stdcall *pil_writemask_f)(pu32,pu32,pu32*);

// Calibration

typedef pu32 (_stdcall *pil_readcal_f)(pu32,pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_writecal_f)(pu32,pu32,pu32,pu32);

// Floating-point calibration data

typedef pu32 (_stdcall *pil_writecalfp_f)(pu32,pu32,pu32,pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_readcalfp_f)(pu32,pu32,pu32,pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_writecaldate_f)(pu32,pu32,pu32,pu32);
typedef pu32 (_stdcall *pil_readcaldate_f)(pu32,pu32,pu32,pu32*,pu32*,pu32*);
typedef pu32 (_stdcall *pil_setcalpoint_f)(pu32,pu32,pu32);

// Programmable attenuator

typedef pu32 (_stdcall *pil_attengetattenuation_f)(pu32,pu32,pfloat*);
typedef pu32 (_stdcall *pil_atteninfo_f)(pu32,pu32,pu32*,pu32*,pfloat*);
typedef pu32 (_stdcall *pil_attenpadvalue_f)(pu32,pu32,pu32,pfloat*);
typedef pu32 (_stdcall *pil_attensetattenuation_f)(pu32,pu32,pfloat);
typedef pu32 (_stdcall *pil_attentype_f)(pu32,pu32,pchar*);

// Power suplies

typedef pu32 (_stdcall *pil_psuenable_f)(pu32,pu32,ps32);
typedef pu32 (_stdcall *pil_psugetvoltage_f)(pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_psuinfo_f)(pu32,pu32,pu32*,pdouble*,pdouble*,pu32*,pu32*);
typedef pu32 (_stdcall *pil_psusetvoltage_f)(pu32,pu32,pdouble);
typedef pu32 (_stdcall *pil_psutype_f)(pu32,pu32,pchar*);

// Programmable resistor specific functions

typedef pu32 (_stdcall *pil_ressetresistance_f)(pu32,pu32,pu32,pdouble);
typedef pu32 (_stdcall *pil_resgetresistance_f)(pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_resinfo_f)(pu32,pu32,pdouble*,pdouble*,pdouble*,pdouble*,pdouble*,pdouble*,pdouble*,pu32*);

// Driver mode control

typedef pu32 (_stdcall *pil_setmode_f)(pu32);

// BIRST specific functions

typedef pu32 (_stdcall *pil_birstcapabilities_f)(pu32,pu32*,pu32*,pu32*);
typedef pu32 (_stdcall *pil_birstinfo_f)(pu32,pu32,pu32*,pchar*,pu32*,pchar*,pu32*);
typedef pu32 (_stdcall *pil_birstsetcontrols_f)(pu32,pu32);
typedef pu32 (_stdcall *pil_birstgetcontrols_f)(pu32,pu32*);
typedef pu32 (_stdcall *pil_birstmeasurement_f)(pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_birstwritecal_f)(pu32,pu32,pu32,pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_birstreadcal_f)(pu32,pu32,pu32,pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_birstwritecaldate_f)(pu32,pu32);
typedef pu32 (_stdcall *pil_birstreadcaldate_f)(pu32,pu32,pu32*,pu32*);
typedef pu32 (_stdcall *pil_birstgetattribute_f)(pu32,pu32,pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_birstsetcontrolsex_f)(pu32,pu32,pu32);
typedef pu32 (_stdcall *pil_birstgetcontrolsex_f)(pu32,pu32,pu32*);

// Battery Simulator functions

typedef pu32 (_stdcall *pil_battgetcurrent_f)(pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_battgetenable_f)(pu32,pu32,pu32*);
typedef pu32 (_stdcall *pil_battgetvoltage_f)(pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_battreadinterlockstate_f)(pu32,pu32,ps32*);
typedef pu32 (_stdcall *pil_battsetcurrent_f)(pu32,pu32,pdouble);
typedef pu32 (_stdcall *pil_battsetenable_f)(pu32,pu32,pu32);
typedef pu32 (_stdcall *pil_battsetvoltage_f)(pu32,pu32,pdouble);

// Vsource specific functions

typedef pu32 (_stdcall *pil_vsourcesetrange_f)(pu32,pu32,pdouble);
typedef pu32 (_stdcall *pil_vsourcegetrange_f)(pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_vsourcesetvoltage_f)(pu32,pu32,pdouble);
typedef pu32 (_stdcall *pil_vsourcegetvoltage_f)(pu32,pu32,pdouble*);
typedef pu32 (_stdcall *pil_vsourcesetenable_f)(pu32,pu32,pu32);
typedef pu32 (_stdcall *pil_vsourcegetenable_f)(pu32,pu32,pu32*);

// Aditional for CERN
// I am not sure about this (I have to use libvpilpxi.so library: VPIL_CheckStatus(char * stat))
//typedef pu32 (_stdcall *pil_getstatusmessage_f)(pchar*);

/* Pickering switching module class */

class piplx_module_c : public base_module_c {
private:
  // Initialize and close
  pil_closecards_f PIL_CloseCards;
  pil_closespecifiedcard_f PIL_CloseSpecifiedCard;  
  pil_opencards_f PIL_OpenCards;
  pil_openspecifiedcard_f PIL_OpenSpecifiedCard;
  // Information and status
  pil_cardid_f PIL_CardId;
  pil_cardloc_f PIL_CardLoc;
  pil_closurelimit_f PIL_ClosureLimit;
  pil_cardopscount_f PIL_CardOpsCount;
  pil_countfreecards_f PIL_CountFreeCards;
  pil_diagnostic_f PIL_Diagnostic;
  pil_enumeratesubs_f PIL_EnumerateSubs;
  pil_findfreecards_f PIL_FindFreeCards;
  pil_settletime_f PIL_SettleTime;
  pil_status_f PIL_Status;
  pil_subattribute_f PIL_SubAttribute;
  pil_subinfo_f PIL_SubInfo;
  pil_substatus_f PIL_SubStatus;
  pil_subtype_f PIL_SubType;
  pil_version_f PIL_Version;  
  // General output  
  pil_clearall_f PIL_ClearAll;
  pil_clearcard_f PIL_ClearCard;
  pil_clearsub_f PIL_ClearSub;  
  pil_opbit_f PIL_OpBit;
  pil_viewbit_f PIL_ViewBit;
  pil_viewsub_f PIL_ViewSub;
  pil_writesub_f PIL_WriteSub;
  // Input
  pil_readbit_f PIL_ReadBit;
  pil_readsub_f PIL_ReadSub;
  // Switching
  pil_clearmask_f PIL_ClearMask;
  pil_maskbit_f PIL_MaskBit;
  pil_maskcrosspoint_f PIL_MaskCrosspoint;
  pil_opcrosspoint_f PIL_OpCrosspoint;
  pil_opswitch_f PIL_OpSwitch;
  pil_viewcrosspoint_f PIL_ViewCrosspoint;
  pil_viewmask_f PIL_ViewMask;
  pil_viewmaskbit_f PIL_ViewMaskBit;
  pil_viewmaskcrosspoint_f PIL_ViewMaskCrosspoint;
  pil_writemask_f PIL_WriteMask;
  // Calibration
  pil_readcal_f PIL_ReadCal;
  pil_writecal_f PIL_WriteCal;
  // FP calibration data
  pil_writecalfp_f PIL_WriteCalFP;
  pil_readcalfp_f PIL_ReadCalFP;
  pil_writecaldate_f PIL_WriteCalDate;
  pil_readcaldate_f PIL_ReadCalDate;
  pil_setcalpoint_f PIL_SetCalPoint;
  // Programmable attenuator
  pil_attengetattenuation_f PIL_AttenGetAttenuation;
  pil_atteninfo_f PIL_AttenInfo;
  pil_attenpadvalue_f PIL_AttenPadValue;
  pil_attensetattenuation_f PIL_AttenSetAttenuation;
  pil_attentype_f PIL_AttenType;
  // Power supply
  pil_psuenable_f PIL_PsuEnable;
  pil_psugetvoltage_f PIL_PsuGetVoltage;
  pil_psuinfo_f PIL_PsuInfo;
  pil_psusetvoltage_f PIL_PsuSetVoltage;
  pil_psutype_f PIL_PsuType;
  // Programmable resistor specific functions
  pil_ressetresistance_f PIL_ResSetResistance;
  pil_resgetresistance_f PIL_ResGetResistance;
  pil_resinfo_f PIL_ResInfo;
  // Driver mode control
  pil_setmode_f PIL_SetMode;
  // BIRST specific functions
  pil_birstcapabilities_f PIL_BirstCapabilities;
  pil_birstinfo_f PIL_BirstInfo;
  pil_birstsetcontrols_f PIL_BirstSetControls;
  pil_birstgetcontrols_f PIL_BirstGetControls;
  pil_birstmeasurement_f PIL_BirstMeasurement;
  pil_birstwritecal_f PIL_BirstWriteCal;
  pil_birstreadcal_f PIL_BirstReadCal;
  pil_birstwritecaldate_f PIL_BirstWriteCalDate;
  pil_birstreadcaldate_f PIL_BirstReadCalDate;
  pil_birstgetattribute_f PIL_BirstGetAttribute;
  pil_birstsetcontrolsex_f PIL_BirstSetControlsEx;
  pil_birstgetcontrolsex_f PIL_BirstGetControlsEx;
  // Battery Simulator
  pil_battgetcurrent_f PIL_BattGetCurrent;
  pil_battgetenable_f PIL_BattGetEnable;
  pil_battgetvoltage_f PIL_BattGetVoltage;
  pil_battreadinterlockstate_f PIL_BattReadInterlockState;
  pil_battsetcurrent_f PIL_BattSetCurrent;
  pil_battsetenable_f PIL_BattSetEnable;
  pil_battsetvoltage_f PIL_BattSetVoltage;
  // Vsource
  pil_vsourcesetrange_f PIL_VsourceSetRange;
  pil_vsourcegetrange_f PIL_VsourceGetRange;
  pil_vsourcesetvoltage_f PIL_VsourceSetVoltage;
  pil_vsourcegetvoltage_f PIL_VsourceGetVoltage;
  pil_vsourcesetenable_f PIL_VsourceSetEnable;
  pil_vsourcegetenable_f PIL_VsourceGetEnable;
private:
  pu32 error2Resource(pu32 error);
  pu32 status2Resource(pu32 *status);
  void loadFunctions(module_t libraryHandle);
  bool isBufferBigEnougth(int requestedSize);
public:
  piplx_module_c(const pchar *libName);
  ~piplx_module_c();
public:
  // Error handling functions
  pbool isError(session_t sid);
  pu32 getLastError(session_t sid,pu32 *code);
  pu32 getLastErrorMessage(session_t sid,pchar *buffer,pu32 size);
  pu32 errorCode2Message(pu32 code,pchar *buffer,pu32 size);
  // Initialize and close
  pu32 closeCards(session_t sid);
  pu32 closeSpecifiedCard(session_t sid,pu32 card);
  pu32 openCards(session_t sid);
  pu32 openCardsForRead(session_t sid);
  pu32 openSpecifiedCard(session_t sid,pu32 bus,pu32 slot,pu32 *card);
  pu32 openSpecifiedCardForRead(session_t sid,pu32 bus,pu32 slot,pu32 *card);
  // Information and status
  pu32 cardID(session_t sid,pu32 card,pchar *id,pu32 length);
  pu32 cardLoc(session_t sid,pu32 card,pu32 *bus,pu32 *slot);
  pu32 closureLimit(session_t sid,pu32 card,pu32 subunit,pu32 *limit);
  pu32 cardOpsCount(session_t sid,pu32 card,pu32 *count);
  pu32 countFreeCards(session_t sid,pu32 *count);
  pu32 diagnostic(session_t sid,pu32 card,pchar *diagStr,pu32 length);
  pu32 enumerateSubs(session_t sid,pu32 card,pu32 *inSubs,pu32 *outSubs);
  pu32 findFreeCards(session_t sid,pu32 numCards,pu32 *busList,pu32 *slotList);
  pu32 settleTime(session_t sid,pu32 card,pu32 subunit,pu32 *time);
  pu32 status(session_t sid,pu32 card);
  pu32 statusCode2Message(pu32 code,pchar *buffer,pu32 size);
  pu32 subAttribute(session_t sid,pu32 card,pu32 subunit,ps32 out,pu32 attrCode,pu32 *attrValue);
  pu32 subInfo(session_t sid,pu32 card,pu32 subunit,ps32 out,pu32 *type,pu32 *rows,pu32 *cols);
  pu32 subStatus(session_t sid,pu32 card,pu32 subunit);
  pu32 subType(session_t sid,pu32 card,pu32 subunit,ps32 out,pchar *typeStr,pu32 length);
  pu32 version(session_t sid);
  // General purpose output
  pu32 clearAll(session_t sid);
  pu32 clearCard(session_t sid,pu32 card);
  pu32 clearSub(session_t sid,pu32 card,pu32 subunit);
  pu32 opBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 bitState);
  pu32 viewBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 *state);
  pu32 viewSub(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size);
  pu32 writeSub(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size);
  // Input
  pu32 readBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 *state);
  pu32 readSub(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size);
  // Switching
  pu32 clearMask(session_t sid,pu32 card,pu32 subunit);
  pu32 maskBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 action);
  pu32 maskCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 action);
  pu32 opCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 action);
  pu32 opSwitch(session_t sid,pu32 card,pu32 subunit,pu32 func,pu32 segment,pu32 sw,pu32 subSwitch,pu32 switchAction,ps32 *state);
  pu32 viewCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 *state);
  pu32 viewMask(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size);
  pu32 viewMaskBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 *state);
  pu32 viewMaskCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 *state);
  pu32 writeMask(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size);
  // Calibration
  pu32 readCal(session_t sid,pu32 card,pu32 subunit,pu32 index,pu32 *data);
  pu32 writeCal(session_t sid,pu32 card,pu32 subunit,pu32 index,pu32 data);
  // Floating-point calibration
  pu32 writeCalFP(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 offset,pu32 numValues,pdouble *data);
  pu32 readCalFP(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 offset,pu32 numValues,pdouble *data);
  pu32 writeCalDate(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 interval);
  pu32 readCalDate(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 *year,pu32 *day,pu32 *interval);
  pu32 setCalPoint(session_t sid,pu32 card,pu32 subunit,pu32 index);
  // Attenuator
  pu32 attenGetAttenuation(session_t sid,pu32 card,pu32 subunit,pfloat *atten);
  pu32 attenInfo(session_t sid,pu32 card,pu32 subunit,pu32 *type,pu32 *numSteps,pfloat *stepSize);
  pu32 attenPadValue(session_t sid,pu32 card,pu32 subunit,pu32 pad,pfloat *atten);
  pu32 attenSetAttenuation(session_t sid,pu32 card,pu32 subunit,pfloat atten);
  pu32 attenType(session_t sid,pu32 card,pu32 subunit,pchar *typeStr,pu32 length);
  // Power supply
  pu32 psuEnable(session_t sid,pu32 card,pu32 subunit,ps32 state);
  pu32 psuGetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble *voltage);
  pu32 psuInfo(session_t sid,pu32 card,pu32 subunit,pu32 *type,pdouble *voltage,pdouble *current,pu32 *precis,pu32 *caps);
  pu32 psuSetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble voltage);
  pu32 psuType(session_t sid,pu32 card,pu32 subunit,pchar *typeStr,pu32 length);
  // Mode control
  pu32 setMode(session_t sid,pu32 flags);
  // Programmable resistor
  pu32 resSetResistance(session_t sid,pu32 card,pu32 subunit,pu32 mode,pdouble resistance);
  pu32 resGetResistance(session_t sid,pu32 card,pu32 subunit,pdouble *resistance);
  pu32 resInfo(session_t sid,pu32 card,pu32 subunit,pdouble *minRes,pdouble *maxRes,pdouble *refRes,
    pdouble *precPC,pdouble *precDelta,pdouble *int1,pdouble *intDelta,pu32 * caps);
  // BIRST
  pu32 birstCapabilities(session_t sid,pu32 card,pu32 *topology,pu32 *caps,pu32 *refRes);
  pu32 birstInfo(session_t sid,pu32 card,pu32 outSub,pu32* poles,pchar* axis1,pu32* line1,pchar* axis2,pu32* line2);
  pu32 birstSetControls(session_t sid,pu32 card,pu32 data);
  pu32 birstGetControls(session_t sid,pu32 card,pu32* data);
  pu32 birstMeasurement(session_t sid,pu32 card,pu32 outSub,pu32* value);
  pu32 birstWriteCal(session_t sid,pu32 card,pu32 outSub,pu32 pole,pu32 offset,pu32 numValues,pu32 *data);
  pu32 birstReadCal(session_t sid,pu32 card,pu32 outSub,pu32 pole,pu32 offset,pu32 numValues,pu32 *data);
  pu32 birstWriteCalDate(session_t sid,pu32 card,pu32 outSub);
  pu32 birstReadCalDate(session_t sid,pu32 card, pu32 outSub,pu32 *year,pu32 * day);
  pu32 birstGetAttribute(session_t sid,pu32 card, pu32 subNum, ps32 out, pu32 attrCode, pu32 *attrValue);
  pu32 birstSetControlsEx(session_t sid,pu32 card,pu32 ctrlReg,pu32 data);
  pu32 birstGetControlsEx(session_t sid,pu32 card,pu32 ctrlReg,pu32* data);
  // Battery Simulator
  pu32 battGetCurrent(session_t sid,pu32 card,pu32 subunit,pdouble *current);
  pu32 battGetEnable(session_t sid,pu32 card,pu32 subunit,pu32 *pattern);
  pu32 battGetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble *voltage);
  pu32 battReadInterlockState(session_t sid,pu32 card,pu32 subunit,ps32 *interlock);
  pu32 battSetCurrent(session_t sid,pu32 card,pu32 subunit,pdouble current);
  pu32 battSetEnable(session_t sid,pu32 card,pu32 subunit,pu32 pattern);
  pu32 battSetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble voltage);
  // Vsource
  pu32 vsourceSetRange(session_t sid,pu32 card,pu32 subunit,pdouble range);
  pu32 vsourceGetRange(session_t sid,pu32 card,pu32 subunit,pdouble *range);
  pu32 vsourceSetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble voltage);
  pu32 vsourceGetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble *voltage);
  pu32 vsourceSetEnable(session_t sid,pu32 card,pu32 subunit,pu32 pattern);
  pu32 vsourceGetEnable(session_t sid,pu32 card,pu32 subunit,pu32 *pattern);
  // Aditional for CERN
  pu32 getStatusMessage(session_t sid, pu32 card, pchar *buffer, pu32 size);
};

#endif // #ifdef _PIPLXMODULE_H_
