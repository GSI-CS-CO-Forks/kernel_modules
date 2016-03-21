#include <PiplxModule.h>
#include <PicmlxModuleAPI.h>
#include <PiplxResources.h>
#include <PiplxFids.h>
#include <System.h>
#include <DebugTool.h>

#ifdef WIN32
	extern handle_t __libPiplx_Instance;
#endif

/* ##################################################################################################### */

/*
 * Private class functions
 */

pu32 piplx_module_c::error2Resource(pu32 error) {
  switch(error) {
    // No error
    case NO_ERROR /* NO_ERR */ : return RES_NO_ERROR;
    case ER_NO_CARD: return RES_ER_NO_CARD;
    case ER_NO_INFO: return RES_ER_NO_INFO;
    case ER_CARD_DISABLED: return RES_ER_CARD_DISABLED;
    case ER_BAD_SUB: return RES_ER_BAD_SUB;
    case ER_BAD_BIT: return RES_ER_BAD_BIT;
    case ER_NO_CAL_DATA: return RES_ER_NO_CAL_DATA;
    case ER_BAD_ARRAY: return RES_ER_BAD_ARRAY;
    case ER_MUX_ILLEGAL: return RES_ER_MUX_ILLEGAL;
    case ER_EXCESS_CLOSURE: return RES_ER_EXCESS_CLOSURE;
    case ER_ILLEGAL_MASK: return RES_ER_ILLEGAL_MASK;
    case ER_OUTPUT_MASKED: return RES_ER_OUTPUT_MASKED;
    case ER_BAD_LOCATION: return RES_ER_BAD_LOCATION;
    case ER_READ_FAIL: return RES_ER_READ_FAIL;
    case ER_WRITE_FAIL: return RES_ER_WRITE_FAIL;
    case ER_DRIVER_OP: return RES_ER_DRIVER_OP;
    case ER_DRIVER_VERSION: return RES_ER_DRIVER_VERSION;
    case ER_SUB_TYPE: return RES_ER_SUB_TYPE;
    case ER_BAD_ROW: return RES_ER_BAD_ROW;
    case ER_BAD_COLUMN: return RES_ER_BAD_COLUMN;
    case ER_BAD_ATTEN: return RES_ER_BAD_ATTEN;
    case ER_BAD_VOLTAGE: return RES_ER_BAD_VOLTAGE;
    case ER_BAD_CAL_INDEX: return RES_ER_BAD_CAL_INDEX;
    case ER_BAD_SEGMENT: return RES_ER_BAD_SEGMENT;
    case ER_BAD_FUNC_CODE: return RES_ER_BAD_FUNC_CODE;
    case ER_BAD_SUBSWITCH: return RES_ER_BAD_SUBSWITCH;
    case ER_BAD_ACTION: return RES_ER_BAD_ACTION;
    case ER_STATE_CORRUPT: return RES_ER_STATE_CORRUPT;
    case ER_BAD_ATTR_CODE: return RES_ER_BAD_ATTR_CODE;
    case ER_EEPROM_WRITE_TMO: return RES_ER_EEPROM_WRITE_TMO;
    case ER_ILLEGAL_OP: return RES_ER_ILLEGAL_OP;
    case ER_BAD_POT: return RES_ER_BAD_POT;
    case ER_MATRIXR_ILLEGAL: return RES_ER_MATRIXR_ILLEGAL;
    case ER_MISSING_CHANNEL: return RES_ER_MISSING_CHANNEL;
    case ER_CARD_INACCESSIBLE: return RES_ER_CARD_INACCESSIBLE;
    case ER_BAD_FP_FORMAT: return RES_ER_BAD_FP_FORMAT;
    case ER_UNCALIBRATED: return RES_ER_UNCALIBRATED;
    case ER_BAD_RESISTANCE: return RES_ER_BAD_RESISTANCE;
    case ER_BAD_STORE: return RES_ER_BAD_STORE;
    case ER_BAD_MODE: return RES_ER_BAD_MODE;    
    case ER_SETTINGS_CONFLICT: return RES_ER_SETTINGS_CONFLICT;
    case ER_CARD_TYPE: return RES_ER_CARD_TYPE;
    case ER_BAD_POLE: return RES_ER_BAD_POLE;
    case ER_MISSING_CAPABILITY: return RES_ER_MISSING_CAPABILITY;
    case ER_MISSING_HARDWARE: return RES_ER_MISSING_HARDWARE;
    case ER_HARDWARE_FAULT: return RES_ER_HARDWARE_FAULT;
    case ER_EXECUTION_FAIL: return RES_ER_EXECUTION_FAIL;
    case ER_BAD_CURRENT: return RES_ER_BAD_CURRENT;
    case ER_BAD_RANGE: return RES_ER_BAD_RANGE;
    case ER_ATTR_UNSUPPORTED: return RES_ER_ATTR_UNSUPPORTED;
    case ER_BAD_REGISTER: return RES_ER_BAD_REGISTER;
    case ER_PIPLX_FUNC_NOT_LOADED: return RES_PIPLX_FUNC_NOT_LOADED;
    case ER_PIPLX_FUNC_NOT_LOCAL: return RES_PIPLX_FUNC_NOT_LOCAL;
    case ER_PIPLX_NOT_CONNECTED: return RES_PIPLX_NOT_CONNECTED;
    default:
      return RES_UNKNOWN_ERROR;
  }
}

pu32 piplx_module_c::status2Resource(pu32 *status) {
  /*if(*status == STAT_OK)
    return RES_STAT_OK;*/
  /*#ifdef DEBUG
    __DEBUGMSG(FALSE,"[1] Status code: 0x%lX",*status);
  #endif*/
  if((*status & STAT_NO_CARD) == STAT_NO_CARD) {
    *status &= ~STAT_NO_CARD;
    return RES_STAT_NO_CARD;
  }
  if((*status & STAT_WRONG_DRIVER) == STAT_WRONG_DRIVER) {
    *status &= ~STAT_WRONG_DRIVER;
    return RES_STAT_WRONG_DRIVER;
  }
  if((*status & STAT_EEPROM_ERR) == STAT_EEPROM_ERR) {
    *status &= ~STAT_EEPROM_ERR;
    return RES_STAT_EEPROM_ERR;
  }
  if((*status & STAT_DISABLED) == STAT_DISABLED) {
    *status &= ~STAT_DISABLED;
    return RES_STAT_DISABLED;
  }
  if((*status & STAT_NO_SUB) == STAT_NO_SUB) {
    *status &= ~STAT_NO_SUB;
    return RES_STAT_NO_SUB;
  }
  if((*status & STAT_BUSY) == STAT_BUSY) {
    *status &= ~STAT_BUSY;
    return RES_STAT_BUSY;
  }
  if((*status & STAT_HW_FAULT) == STAT_HW_FAULT) {
    *status &= ~STAT_HW_FAULT;
    return RES_STAT_HW_FAULT;
  }
  if((*status & STAT_PARITY_ERROR) == STAT_PARITY_ERROR) {
    *status &= ~STAT_PARITY_ERROR;
    return RES_STAT_PARITY_ERROR;
  }
  if((*status & STAT_PSU_INHIBITED) == STAT_PSU_INHIBITED) {
    *status &= ~STAT_PSU_INHIBITED;
    return RES_STAT_PSU_INHIBITED;
  }
  if((*status & STAT_PSU_SHUTDOWN) == STAT_PSU_SHUTDOWN) {
    *status &= ~STAT_PSU_SHUTDOWN;
    return RES_STAT_PSU_SHUTDOWN;
  }
  if((*status & STAT_PSU_CURRENT_LIMIT) == STAT_PSU_CURRENT_LIMIT) {
    *status &= ~STAT_PSU_CURRENT_LIMIT;
    return RES_STAT_PSU_CURRENT_LIMIT;
  }
  if((*status & STAT_CORRUPTED) == STAT_CORRUPTED) {
    *status &= ~STAT_CORRUPTED;
    return RES_STAT_CORRUPTED;
  }
  if((*status & STAT_CARD_INACCESSIBLE) == STAT_CARD_INACCESSIBLE) {
    *status &= ~STAT_CARD_INACCESSIBLE;
    return RES_STAT_CARD_INACCESSIBLE;
  }
  if((*status & STAT_UNCALIBRATED) == STAT_UNCALIBRATED) {
    *status &= ~STAT_UNCALIBRATED;
    return RES_STAT_UNCALIBRATED;
  }
  if((*status & STAT_CALIBRATION_DUE) == STAT_CALIBRATION_DUE) {
    *status &= ~STAT_CALIBRATION_DUE;
    return RES_STAT_CALIBRATION_DUE;
  }
  if((*status & STAT_BIRST_ENABLED) == STAT_BIRST_ENABLED) {
    *status &= ~STAT_BIRST_ENABLED;
    return RES_STAT_BIRST_ENABLED;
  }
  /*#ifdef DEBUG
    __DEBUGMSG(FALSE,"[2] Status code: 0x%lX",*status);
  #endif*/
  return RES_UNKNOWN_STATUS;
}

bool piplx_module_c::isBufferBigEnougth(int requestedSize) {
  return SOCKET_BUFFER_SIZE <= requestedSize;
}

void piplx_module_c::loadFunctions(module_t libraryHandle) {
  GETLIBPROC(PIL_CloseCards,pil_closecards_f,libraryHandle,"PIL_CloseCards");
  GETLIBPROC(PIL_CloseSpecifiedCard,pil_closespecifiedcard_f,libraryHandle,"PIL_CloseSpecifiedCard");  
  GETLIBPROC(PIL_OpenCards,pil_opencards_f,libraryHandle,"PIL_OpenCards");
  GETLIBPROC(PIL_OpenSpecifiedCard,pil_openspecifiedcard_f,libraryHandle,"PIL_OpenSpecifiedCard");

  GETLIBPROC(PIL_CardId,pil_cardid_f,libraryHandle,"PIL_CardId");
  GETLIBPROC(PIL_CardLoc,pil_cardloc_f,libraryHandle,"PIL_CardLoc");
  GETLIBPROC(PIL_ClosureLimit,pil_closurelimit_f,libraryHandle,"PIL_ClosureLimit");
  GETLIBPROC(PIL_CountFreeCards,pil_countfreecards_f,libraryHandle,"PIL_CountFreeCards");
  GETLIBPROC(PIL_Diagnostic,pil_diagnostic_f,libraryHandle,"PIL_Diagnostic");
  GETLIBPROC(PIL_EnumerateSubs,pil_enumeratesubs_f,libraryHandle,"PIL_EnumerateSubs");
  GETLIBPROC(PIL_FindFreeCards,pil_findfreecards_f,libraryHandle,"PIL_FindFreeCards");
  GETLIBPROC(PIL_SettleTime,pil_settletime_f,libraryHandle,"PIL_SettleTime");
  GETLIBPROC(PIL_Status,pil_status_f,libraryHandle,"PIL_Status");
  GETLIBPROC(PIL_SubAttribute,pil_subattribute_f,libraryHandle,"PIL_SubAttribute");
  GETLIBPROC(PIL_SubInfo,pil_subinfo_f,libraryHandle,"PIL_SubInfo");
  GETLIBPROC(PIL_SubStatus,pil_substatus_f,libraryHandle,"PIL_SubStatus");
  GETLIBPROC(PIL_SubType,pil_subtype_f,libraryHandle,"PIL_SubType");
  GETLIBPROC(PIL_Version,pil_version_f,libraryHandle,"PIL_Version");

  GETLIBPROC(PIL_ClearAll,pil_clearall_f,libraryHandle,"PIL_ClearAll");
  GETLIBPROC(PIL_ClearCard,pil_clearcard_f,libraryHandle,"PIL_ClearCard");
  GETLIBPROC(PIL_ClearSub,pil_clearsub_f,libraryHandle,"PIL_ClearSub");
  GETLIBPROC(PIL_OpBit,pil_opbit_f,libraryHandle,"PIL_OpBit");
  GETLIBPROC(PIL_ViewBit,pil_viewbit_f,libraryHandle,"PIL_ViewBit");
  GETLIBPROC(PIL_ViewSub,pil_viewsub_f,libraryHandle,"PIL_ViewSub");
  GETLIBPROC(PIL_WriteSub,pil_writesub_f,libraryHandle,"PIL_WriteSub");
  
  GETLIBPROC(PIL_ReadBit,pil_readbit_f,libraryHandle,"PIL_ReadBit");
  GETLIBPROC(PIL_ReadSub,pil_readsub_f,libraryHandle,"PIL_ReadSub");
  
  GETLIBPROC(PIL_ClearMask,pil_clearmask_f,libraryHandle,"PIL_ClearMask");
  GETLIBPROC(PIL_MaskBit,pil_maskbit_f,libraryHandle,"PIL_MaskBit");
  GETLIBPROC(PIL_MaskCrosspoint,pil_maskcrosspoint_f,libraryHandle,"PIL_MaskCrosspoint");
  GETLIBPROC(PIL_OpCrosspoint,pil_opcrosspoint_f,libraryHandle,"PIL_OpCrosspoint");
  GETLIBPROC(PIL_OpSwitch,pil_opswitch_f,libraryHandle,"PIL_OpSwitch");
  GETLIBPROC(PIL_ViewCrosspoint,pil_viewcrosspoint_f,libraryHandle,"PIL_ViewCrosspoint");
  GETLIBPROC(PIL_ViewMask,pil_viewmask_f,libraryHandle,"PIL_ViewMask");
  GETLIBPROC(PIL_ViewMaskBit,pil_viewmaskbit_f,libraryHandle,"PIL_ViewMaskBit");
  GETLIBPROC(PIL_ViewMaskCrosspoint,pil_viewmaskcrosspoint_f,libraryHandle,"PIL_ViewMaskCrosspoint");
  GETLIBPROC(PIL_WriteMask,pil_writemask_f,libraryHandle,"PIL_WriteMask");
  
  GETLIBPROC(PIL_ReadCal,pil_readcal_f,libraryHandle,"PIL_ReadCal");
  GETLIBPROC(PIL_WriteCal,pil_writecal_f,libraryHandle,"PIL_WriteCal");  
  
  GETLIBPROC(PIL_ReadCalFP,pil_readcalfp_f,libraryHandle,"PIL_ReadCalFP");
  GETLIBPROC(PIL_WriteCalFP,pil_writecalfp_f,libraryHandle,"PIL_WriteCalFP");
  GETLIBPROC(PIL_WriteCalDate,pil_writecaldate_f,libraryHandle,"PIL_WriteCalDate");
  GETLIBPROC(PIL_ReadCalDate,pil_readcaldate_f,libraryHandle,"PIL_ReadCalDate");  
  GETLIBPROC(PIL_SetCalPoint,pil_setcalpoint_f,libraryHandle,"PIL_SetCalPoint");

  GETLIBPROC(PIL_AttenGetAttenuation,pil_attengetattenuation_f,libraryHandle,"PIL_AttenGetAttenuation");
  GETLIBPROC(PIL_AttenInfo,pil_atteninfo_f,libraryHandle,"PIL_AttenInfo");
  GETLIBPROC(PIL_AttenPadValue,pil_attenpadvalue_f,libraryHandle,"PIL_AttenPadValue");
  GETLIBPROC(PIL_AttenSetAttenuation,pil_attensetattenuation_f,libraryHandle,"PIL_AttenSetAttenuation");
  GETLIBPROC(PIL_AttenType,pil_attentype_f,libraryHandle,"PIL_AttenType");
  
  GETLIBPROC(PIL_PsuEnable,pil_psuenable_f,libraryHandle,"PIL_PsuEnable");
  GETLIBPROC(PIL_PsuGetVoltage,pil_psugetvoltage_f,libraryHandle,"PIL_PsuGetVoltage");
  GETLIBPROC(PIL_PsuInfo,pil_psuinfo_f,libraryHandle,"PIL_PsuInfo");
  GETLIBPROC(PIL_PsuSetVoltage,pil_psusetvoltage_f,libraryHandle,"PIL_PsuSetVoltage");
  GETLIBPROC(PIL_PsuType,pil_psutype_f,libraryHandle,"PIL_PsuType");
  
  GETLIBPROC(PIL_SetMode,pil_setmode_f,libraryHandle,"PIL_SetMode");
  
  GETLIBPROC(PIL_ResSetResistance,pil_ressetresistance_f,libraryHandle,"PIL_ResSetResistance");
  GETLIBPROC(PIL_ResGetResistance,pil_resgetresistance_f,libraryHandle,"PIL_ResGetResistance");
  GETLIBPROC(PIL_ResInfo,pil_resinfo_f,libraryHandle,"PIL_ResInfo");

  // BIRST
  GETLIBPROC(PIL_BirstCapabilities,pil_birstcapabilities_f,libraryHandle,"PIL_BirstCapabilities");
  GETLIBPROC(PIL_BirstInfo,pil_birstinfo_f,libraryHandle,"PIL_BirstInfo");
  GETLIBPROC(PIL_BirstSetControls,pil_birstsetcontrols_f,libraryHandle,"PIL_BirstSetControls");
  GETLIBPROC(PIL_BirstGetControls,pil_birstgetcontrols_f,libraryHandle,"PIL_BirstGetControls");
  GETLIBPROC(PIL_BirstMeasurement,pil_birstmeasurement_f,libraryHandle,"PIL_BirstMeasurement");
  GETLIBPROC(PIL_BirstWriteCal,pil_birstwritecal_f,libraryHandle,"PIL_BirstWriteCal");
  GETLIBPROC(PIL_BirstReadCal,pil_birstreadcal_f,libraryHandle,"PIL_BirstReadCal");
  GETLIBPROC(PIL_BirstWriteCalDate,pil_birstwritecaldate_f,libraryHandle,"PIL_BirstWriteCalDate");
  GETLIBPROC(PIL_BirstReadCalDate,pil_birstreadcaldate_f,libraryHandle,"PIL_BirstReadCalDate");
  GETLIBPROC(PIL_BirstGetAttribute,pil_birstgetattribute_f,libraryHandle,"PIL_BirstGetAttribute");
  GETLIBPROC(PIL_BirstSetControlsEx,pil_birstsetcontrolsex_f,libraryHandle,"PIL_BirstSetControlsEx");
  GETLIBPROC(PIL_BirstGetControlsEx,pil_birstgetcontrolsex_f,libraryHandle,"PIL_BirstGetControlsEx");

  // Battery Simulator
  GETLIBPROC(PIL_BattGetCurrent,pil_battgetcurrent_f,libraryHandle,"PIL_BattGetCurrent");
  GETLIBPROC(PIL_BattGetEnable,pil_battgetenable_f,libraryHandle,"PIL_BattGetEnable");
  GETLIBPROC(PIL_BattGetVoltage,pil_battgetvoltage_f,libraryHandle,"PIL_BattGetVoltage");
  GETLIBPROC(PIL_BattReadInterlockState,pil_battreadinterlockstate_f,libraryHandle,"PIL_BattReadInterlockState");
  GETLIBPROC(PIL_BattSetCurrent,pil_battsetcurrent_f,libraryHandle,"PIL_BattSetCurrent");
  GETLIBPROC(PIL_BattSetEnable,pil_battsetenable_f,libraryHandle,"PIL_BattSetEnable");
  GETLIBPROC(PIL_BattSetVoltage,pil_battsetvoltage_f,libraryHandle,"PIL_BattSetVoltage");
  // Vsource
  GETLIBPROC(PIL_VsourceSetRange,pil_vsourcesetrange_f,libraryHandle,"PIL_VsourceSetRange");
  GETLIBPROC(PIL_VsourceGetRange,pil_vsourcegetrange_f,libraryHandle,"PIL_VsourceGetRange");
  GETLIBPROC(PIL_VsourceSetVoltage,pil_vsourcesetvoltage_f,libraryHandle,"PIL_VsourceSetVoltage");
  GETLIBPROC(PIL_VsourceGetVoltage,pil_vsourcegetvoltage_f,libraryHandle,"PIL_VsourceGetVoltage");
  GETLIBPROC(PIL_VsourceSetEnable,pil_vsourcesetenable_f,libraryHandle,"PIL_VsourceSetEnable");
  GETLIBPROC(PIL_VsourceGetEnable,pil_vsourcegetenable_f,libraryHandle,"PIL_VsourceGetEnable");
}

/* ##################################################################################################### */

/*
 * Public class functions
 */

// Constructor/destructor

piplx_module_c::piplx_module_c(const pchar *libName) : base_module_c(libName) {
  // Create error message list
  #if(defined(LINUX) || defined(LABVIEWRT))
    // Error messages
    errorList->insert(make_pair(RES_UNKNOWN_ERROR,"Unknown error code."));    
    // Piplx error messages
    errorList->insert(make_pair(RES_PIPLX_FUNC_NOT_LOADED,"Desired function not loaded from \"libpilpxi\" library."));
    errorList->insert(make_pair(RES_PIPLX_NOT_CONNECTED,"You are not connected to remote host."));
    errorList->insert(make_pair(RES_PIPLX_FUNC_NOT_LOCAL,"Function not supported on localhost."));
    // PilPxi error messages
    errorList->insert(make_pair(RES_ER_NO_CARD,"No Pickering card has the specified number."));
    errorList->insert(make_pair(RES_ER_NO_INFO,"Card information unobtainable - hardware problem."));
    errorList->insert(make_pair(RES_ER_CARD_DISABLED,"Specified card is disabled."));
    errorList->insert(make_pair(RES_ER_BAD_SUB,"Sub-unit value out of range for target card."));
    errorList->insert(make_pair(RES_ER_BAD_BIT,"Bit number out of range for target card."));
    errorList->insert(make_pair(RES_ER_NO_CAL_DATA,"Target sub-unit has no calibration data to read/write."));
    errorList->insert(make_pair(RES_ER_BAD_ARRAY,"SafeArray type, shape or size is incorrect."));
    errorList->insert(make_pair(RES_ER_MUX_ILLEGAL,"Non zero write data value is illegal for MUX sub-unit."));
    errorList->insert(make_pair(RES_ER_EXCESS_CLOSURE,"Execution woul cause closure limit to be exceeded."));
    errorList->insert(make_pair(RES_ER_ILLEGAL_MASK,"One or more of the specified channels can not be masked."));
    errorList->insert(make_pair(RES_ER_OUTPUT_MASKED,"Can not activate an output that is disabled."));
    errorList->insert(make_pair(RES_ER_BAD_LOCATION,"Can not open a Pickering card at the specified location."));
    errorList->insert(make_pair(RES_ER_READ_FAIL,"Failed read from hardware."));
    errorList->insert(make_pair(RES_ER_WRITE_FAIL,"Failed write to hardware."));
    errorList->insert(make_pair(RES_ER_DRIVER_OP,"Hardware driver failure."));
    errorList->insert(make_pair(RES_ER_DRIVER_VERSION,"Incompatible hardware driver version."));
    errorList->insert(make_pair(RES_ER_SUB_TYPE,"Function call incompatible with sub-unit type or capabilities."));
    errorList->insert(make_pair(RES_ER_BAD_ROW,"Matrix row value out of range."));
    errorList->insert(make_pair(RES_ER_BAD_COLUMN,"Matrix column value out of range."));
    errorList->insert(make_pair(RES_ER_BAD_ATTEN,"Attenuation value out of range."));
    errorList->insert(make_pair(RES_ER_BAD_VOLTAGE,"Voltage value out of range."));
    errorList->insert(make_pair(RES_ER_BAD_CAL_INDEX,"Calibration reference out of range."));
    errorList->insert(make_pair(RES_ER_BAD_SEGMENT,"Segment number out of range."));
    errorList->insert(make_pair(RES_ER_BAD_FUNC_CODE,"Function code value out of range."));
    errorList->insert(make_pair(RES_ER_BAD_SUBSWITCH,"Subswitch value out of range."));
    errorList->insert(make_pair(RES_ER_BAD_ACTION,"Action code out of range."));
    errorList->insert(make_pair(RES_ER_STATE_CORRUPT,"Can not execute due to sub-unit corrupt state."));
    errorList->insert(make_pair(RES_ER_BAD_ATTR_CODE,"Unrecognized attribute code."));
    errorList->insert(make_pair(RES_ER_EEPROM_WRITE_TMO,"Timeout writing to EEPROM."));
    errorList->insert(make_pair(RES_ER_ILLEGAL_OP,"Operation is illegal in the sub-unit's current state."));
    errorList->insert(make_pair(RES_ER_BAD_POT,"Unrecognized pot number."));
    errorList->insert(make_pair(RES_ER_MATRIXR_ILLEGAL,"Invalid write pattern for MATRIX sub-unit."));
    errorList->insert(make_pair(RES_ER_MISSING_CHANNEL,"Attempted operation on non-existent channel."));
    errorList->insert(make_pair(RES_ER_CARD_INACCESSIBLE,"Card can not be accessed (failed/removed/unpowered)."));
    errorList->insert(make_pair(RES_ER_BAD_FP_FORMAT,"Unsupported internal pfloating-point format (internal error)."));
    errorList->insert(make_pair(RES_ER_UNCALIBRATED,"Sub-unit is not calibrated."));
    errorList->insert(make_pair(RES_ER_BAD_RESISTANCE,"Unobtainable resistance value."));
    errorList->insert(make_pair(RES_ER_BAD_STORE,"Invalid calibration store number."));
    errorList->insert(make_pair(RES_ER_BAD_MODE,"Invalid mode value."));
    errorList->insert(make_pair(RES_ER_SETTINGS_CONFLICT,"Conflicting device settings."));
    errorList->insert(make_pair(RES_ER_CARD_TYPE,"Function call incompatible with card type or capabilities."));
    errorList->insert(make_pair(RES_ER_BAD_POLE,"Switch pole value out of range."));
    errorList->insert(make_pair(RES_ER_MISSING_CAPABILITY,"Attempt to activate a non-existent capability."));
    errorList->insert(make_pair(RES_ER_MISSING_HARDWARE,"Action requires hardware that is not present."));
    errorList->insert(make_pair(RES_ER_HARDWARE_FAULT,"Faulty hardware."));
    errorList->insert(make_pair(RES_ER_EXECUTION_FAIL,"Failed to execute (e.g. blocked by a hardware condition)."));
    errorList->insert(make_pair(RES_ER_BAD_CURRENT,"Current value out of range."));
    errorList->insert(make_pair(RES_ER_BAD_RANGE,"Invalid range value."));
    errorList->insert(make_pair(RES_ER_ATTR_UNSUPPORTED,"Attribute not supported."));
    errorList->insert(make_pair(RES_ER_BAD_REGISTER,"Register number out of range."));
    // Status messages
    errorList->insert(make_pair(RES_UNKNOWN_STATUS,"Unknown status code."));
    errorList->insert(make_pair(RES_STAT_NO_CARD,"No card with specified number."));
    errorList->insert(make_pair(RES_STAT_WRONG_DRIVER,"Card requires later driver version."));
    errorList->insert(make_pair(RES_STAT_EEPROM_ERR,"Error interpreting card EEPROM data."));
    errorList->insert(make_pair(RES_STAT_DISABLED,"Card is disabled."));
    errorList->insert(make_pair(RES_STAT_NO_SUB,"Card has no sub-unit with specified number."));
    errorList->insert(make_pair(RES_STAT_BUSY,"Busy (card or sub-unit)."));
    errorList->insert(make_pair(RES_STAT_HW_FAULT,"Hardware fault (eg. defective serial loop)."));
    errorList->insert(make_pair(RES_STAT_PARITY_ERROR,"9050 Parity error."));
    errorList->insert(make_pair(RES_STAT_PSU_INHIBITED,"PSU sub-unit - supply is disabled (by software)."));
    errorList->insert(make_pair(RES_STAT_PSU_SHUTDOWN,"PSU sub-unit - supply is shutdown (due to overload)."));
    errorList->insert(make_pair(RES_STAT_PSU_CURRENT_LIMIT,"PSU sub-unit - supply is operating in current-limited mode."));
    errorList->insert(make_pair(RES_STAT_CORRUPTED,"Sub-unit logical state is corrupted."));
    errorList->insert(make_pair(RES_STAT_CARD_INACCESSIBLE,"Card can not be accessed (failed/removed/unpowered)."));
    errorList->insert(make_pair(RES_STAT_UNCALIBRATED,"Calibration data is faulty (card or sub-unit)."));
    errorList->insert(make_pair(RES_STAT_CALIBRATION_DUE,"Calibration is due (card or sub-unit)."));
    errorList->insert(make_pair(RES_STAT_BIRST_ENABLED,"BIRST is active (card or sub-unit)."));
    errorList->insert(make_pair(RES_STAT_OK,"Sub-unit functional and stable."));
  #endif // #if(defined(LINUX) || defined(LABVIEWRT))
  init();
}

piplx_module_c::~piplx_module_c() {
  //base_module_c::~base_module_c();
}

/* ##################################################################################################### */

// Error handling functions

pbool piplx_module_c::isError(session_t sid) {
  pu32 code;

  if(getLastError(sid,&code))
    return true;    
  return (pbool)code;
}

pu32 piplx_module_c::getLastError(session_t sid,pu32 *code) {  
  return PICMLX_GetLastErrorCode(sid,code);
}

pu32 piplx_module_c::getLastErrorMessage(session_t sid,pchar *buffer,pu32 size) {
  pu32 code,result,length;
  pchar fmtStr[512];
    
  if((buffer == NULL) || (((ps32)size) <= 0)) {
    return ER_PICMLX_CLI_PARAM_SIZE;
  }  
  result = getLastError(sid,&code);
  if(!result)
    if(code >= 0) {
      code = errorCode2Message(code,buffer,size);          
		  sprintf_s(fmtStr,sizeof(fmtStr),"%s [SysError: %d]",buffer,GetLastError()); //sprintf_s(fmtStr,sizeof(fmtStr),"%s, (SystemErrorCode: %d)",buffer,GetLastError());
      length = (pu32)strlen(fmtStr);
      if(size <= length) {
        fmtStr[size - 1] = 0;
      }
      strcpy_s(buffer,size,fmtStr);
    }  
  return result;
}

pu32 piplx_module_c::errorCode2Message(pu32 code,pchar *buffer,pu32 size) {
  pu32 resCode;

  if((buffer == NULL) || (((ps32)size) <= 0))
    return ER_PICMLX_CLI_PARAM_SIZE;  
  buffer[0] = 0;
  resCode = error2Resource(code);  
  if(resCode != RES_UNKNOWN_ERROR) {

    #ifdef WIN32
      LOADSTRING(__libPiplx_Instance,resCode,buffer,size);
    #elif LINUX // #ifdef WIN32
      LOADSTRING(errorList,resCode,buffer,size);
    #endif // #elif LINUX

  } else {
    return PICMLX_ErrorCodeToMessage(code,buffer,size);
  }  
  return NO_ERROR;
}

/* ##################################################################################################### */

// Initialize and close

pu32 piplx_module_c::closeCards(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(FALSE,"piplx_module_c::closeCards - begin");
  #endif
  /* DEBUG RECORD END */
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_CLOSECARDS,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {
      sesData.lastError = ER_PIPLX_FUNC_NOT_LOADED;
      if(PIL_CloseCards != NULL) {
        PIL_CloseCards();
        sesData.lastError = NO_ERROR;
      }
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  /* DEBUG RECORD BEGIN */
  #ifdef DEBUG
    __DEBUGMSG(TRUE,"piplx_module_c::closeCards - end");
  #endif
  /* DEBUG RECORD END */
  return result;
}

pu32 piplx_module_c::closeSpecifiedCard(session_t sid,pu32 card) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_CLOSESPECIFIEDCARD,"u",card)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      }
    } else {      
      sesData.lastError = (PIL_CloseSpecifiedCard != NULL) ? PIL_CloseSpecifiedCard(card) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::openCards(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_OPENCARDS,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"u",&result);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PIPLX_FUNC_NOT_LOADED;
      if(PIL_OpenCards != NULL) {
        result = PIL_OpenCards();
        sesData.lastError = NO_ERROR;
      }      
    }    
    //result = sesData.lastError;
  } else
    sesData.lastError = ER_PICMLX_CLI_INVALID_SESSION;
  PICMLX_EndTransmition(&sesData);
  return sesData.lastError ? INVALID_RESULT : result;
}

pu32 piplx_module_c::openCardsForRead(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_OPENCARDSFORREAD,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"u",&result);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PIPLX_FUNC_NOT_LOCAL;
    }
    //result = sesData.lastError;
  } else
    sesData.lastError = ER_PICMLX_CLI_INVALID_SESSION;
  PICMLX_EndTransmition(&sesData);
  return sesData.lastError ? INVALID_RESULT : result;
}

pu32 piplx_module_c::openSpecifiedCard(session_t sid,pu32 bus,pu32 slot,pu32 *card) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(card == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_OPENSPECIFIEDCARD,"uu",bus,slot)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",card);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_OpenSpecifiedCard != NULL) ? PIL_OpenSpecifiedCard(bus,slot,card) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::openSpecifiedCardForRead(session_t sid,pu32 bus,pu32 slot,pu32 *card) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(card == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_OPENSPECIFIEDCARDFORREAD,"uu",bus,slot)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",card);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = ER_PIPLX_FUNC_NOT_LOCAL;
      }
    result = sesData.lastError;  
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Information and status

pu32 piplx_module_c::cardID(session_t sid,pu32 card,pchar *id,pu32 length) {
  register pu32 result;
  session_data_s sesData = {0};
  
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)length) <= 0) || (id == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_CARDID,"uu",card,length)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"ac",length,id);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_CardId != NULL) ? PIL_CardId(card,id) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::cardLoc(session_t sid,pu32 card,pu32 *bus,pu32 *slot) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((bus == NULL) || (slot == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_CARDLOC,"u",card)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"uu",bus,slot);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_CardLoc != NULL) ? PIL_CardLoc(card,bus,slot) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::closureLimit(session_t sid,pu32 card,pu32 subunit,pu32 *limit) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(limit == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_CLOSURELIMIT,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",limit);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ClosureLimit != NULL) ? PIL_ClosureLimit(card,subunit,limit) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::cardOpsCount(session_t sid,pu32 card,pu32 *count) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(count == NULL) 
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_CARDOPSCOUNT,"u",card)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",count);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_CardOpsCount != NULL) ? PIL_CardOpsCount(card,count) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::countFreeCards(session_t sid,pu32 *count) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(count == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_COUNTFREECARDS,NULL)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",count);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_CountFreeCards != NULL) ? PIL_CountFreeCards(count) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::diagnostic(session_t sid,pu32 card,pchar *diagStr,pu32 length) {
  register pu32 result;
  session_data_s sesData = {0};
  
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)length) <= 0) || (length > MAX_STRING_LENGTH) || (diagStr == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_DIAGNOSTIC,"uu",card,length)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"ac",length,diagStr);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_Diagnostic != NULL) ? PIL_Diagnostic(card,diagStr) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::enumerateSubs(session_t sid,pu32 card,pu32 *inSubs,pu32 *outSubs) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((inSubs == NULL) || (outSubs == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_ENUMERATESUBS,"u",card)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"uu",inSubs,outSubs);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_EnumerateSubs != NULL) ? PIL_EnumerateSubs(card,inSubs,outSubs) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::findFreeCards(session_t sid,pu32 numCards,pu32 *busList,pu32 *slotList) {
  register pu32 result;
  session_data_s sesData = {0};
  
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)numCards) <= 0) || ((numCards * 4) > (SOCKET_BUFFER_SIZE - 16)) || (busList == NULL) || (slotList == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_FINDFREECARDS,"uuu",numCards,numCards,numCards)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"auau",numCards,busList,numCards,slotList);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_FindFreeCards != NULL) ? PIL_FindFreeCards(numCards,busList,slotList) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::settleTime(session_t sid,pu32 card,pu32 subunit,pu32 *time) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(time == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_SETTLETIME,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",time);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_SettleTime != NULL) ? PIL_SettleTime(card,subunit,time) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::status(session_t sid,pu32 card) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_STATUS,"u",card)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"u",&result);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PIPLX_FUNC_NOT_LOADED;
      if(PIL_Status != NULL) {
        result = PIL_Status(card);
        sesData.lastError = NO_ERROR;
      }
    }
    //result = sesData.lastError;
  } else
    sesData.lastError = ER_PICMLX_CLI_INVALID_SESSION;
  PICMLX_EndTransmition(&sesData);
  return sesData.lastError ? INVALID_RESULT : result;
}

pu32 piplx_module_c::statusCode2Message(pu32 code,pchar *buffer,pu32 size) {
  pu32 resCode;  
  pchar status[MAX_STRING_LENGTH];
  //pchar temp[MAX_STRING_LENGTH];

  if((buffer == NULL) || (((ps32)size) <= 0))
    return ER_PICMLX_CLI_PARAM_SIZE;
  buffer[0] = 0;  
  status[0] = 0;
  if(code == 0) {
    #ifdef WIN32
      LOADSTRING(__libPiplx_Instance,RES_STAT_OK,buffer,size);
    #elif LINUX // #ifdef WIN32
      LOADSTRING(errorList,RES_STAT_OK,buffer,size);
    #endif // #elif LINUX
  }
  else
  {
    resCode = code;
    if(status2Resource(&resCode) == RES_UNKNOWN_STATUS) {
      #ifdef WIN32
        LOADSTRING(__libPiplx_Instance,RES_UNKNOWN_STATUS,buffer,size);
      #elif LINUX // #ifdef WIN32
        LOADSTRING(errorList,RES_UNKNOWN_STATUS,buffer,size);
      #endif // #elif LINUX
    } else {
      while((resCode = status2Resource(&code)) != RES_UNKNOWN_STATUS) {
        #ifdef WIN32
          LOADSTRING(__libPiplx_Instance,resCode,status,sizeof(status));
        #elif LINUX // #ifdef WIN32
          LOADSTRING(errorList,resCode,status,size);
        #endif // #elif LINUX
        if(strlen(buffer) + strlen(status) < size)
          strcat_s(buffer,size,status);
        else
          return ER_PICMLX_CLI_PARAM_SIZE;
      }
    }
  }
  return NO_ERROR;
}

pu32 piplx_module_c::subAttribute(session_t sid,pu32 card,pu32 subunit,ps32 out,pu32 attrCode,pu32 *attrValue) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(attrValue == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_SUBATTRIBUTE,"uubu",card,subunit,(pbool)out,attrCode)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",attrValue);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_SubAttribute != NULL) ? PIL_SubAttribute(card,subunit,out,attrCode,attrValue) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::subInfo(session_t sid,pu32 card,pu32 subunit,ps32 out,pu32 *type,pu32 *rows,pu32 *cols) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((type == NULL) || (rows == NULL) || (cols == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_SUBINFO,"uub",card,subunit,(pbool)out)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"uuu",type,rows,cols);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_SubInfo != NULL) ? PIL_SubInfo(card,subunit,out,type,rows,cols) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::subStatus(session_t sid,pu32 card,pu32 subunit) {
  register pu32 result;  
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_SUBSTATUS,"uu",card,subunit)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"u",&result);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PIPLX_FUNC_NOT_LOADED;
      if(PIL_SubStatus != NULL) {
        result = PIL_SubStatus(card,subunit);
        sesData.lastError = NO_ERROR;
      }
    }
    //result = sesData.lastError;
  } else
    sesData.lastError = ER_PICMLX_CLI_INVALID_SESSION;
  PICMLX_EndTransmition(&sesData);
  return sesData.lastError ? INVALID_RESULT : result;
}

pu32 piplx_module_c::subType(session_t sid,pu32 card,pu32 subunit,ps32 out,pchar *typeStr,pu32 length) {
  register pu32 result;
  session_data_s sesData = {0};
  
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)length) <= 0) || (length > MAX_STRING_LENGTH) || (typeStr == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_SUBTYPE,"uubu",card,subunit,(pbool)out,length)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"ac",length,typeStr);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_SubType != NULL) ? PIL_SubType(card,subunit,out,typeStr) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::version(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_VERSION,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"u",&result);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PIPLX_FUNC_NOT_LOADED;
      if(PIL_Version != NULL) {
        result = PIL_Version();
        sesData.lastError = NO_ERROR;
      }
    }
    //result = sesData.lastError;
  } else
    sesData.lastError = ER_PICMLX_CLI_INVALID_SESSION;
  PICMLX_EndTransmition(&sesData);
  return sesData.lastError ? INVALID_RESULT : result;
}

// General purpose output

pu32 piplx_module_c::clearAll(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_CLEARALL,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_ClearAll != NULL) ? PIL_ClearAll() : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::clearCard(session_t sid,pu32 card) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_CLEARCARD,"u",card)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_ClearCard != NULL) ? PIL_ClearCard(card) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::clearSub(session_t sid,pu32 card,pu32 subunit) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_CLEARSUB,"uu",card,subunit)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_ClearSub != NULL) ? PIL_ClearSub(card,subunit) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::opBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 bitState) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_OPBIT,"uuub",card,subunit,bit,(pbool)bitState)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      }
    } else {      
      sesData.lastError = (PIL_OpBit != NULL) ? PIL_OpBit(card,subunit,bit,bitState) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::viewBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 *state) {
  register pu32 result;
  register pbool data;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(state == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_VIEWBIT,"uuu",card,subunit,bit)) {
            if(!(sesData.lastError = PICMLX_RecvPacket(NULL,"b",&data)))
              *state = (ps32)data;
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ViewBit != NULL) ? PIL_ViewBit(card,subunit,bit,state) : ER_PIPLX_FUNC_NOT_LOADED;
        #ifdef DEBUG
          __DEBUGMSG(FALSE,"piplx_module_c::viewBit result: %d",result);
        #endif        
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::viewSub(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size) {
  register pu32 result;
  session_data_s sesData = {0};
    
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)size) <= 0) || ((size * 4) > MAX_STRING_LENGTH) || (data == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_VIEWSUB,"uuu",card,subunit,size)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"au",size,data);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ViewSub != NULL) ? PIL_ViewSub(card,subunit,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::writeSub(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size) {
  register pu32 result;
  session_data_s sesData = {0};  
    
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)size) <= 0) || ((size * 4) > MAX_STRING_LENGTH) || (data == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_WRITESUB,"uuau",card,subunit,size,data)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_WriteSub != NULL) ? PIL_WriteSub(card,subunit,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Input

pu32 piplx_module_c::readBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 *state) {
  register pu32 result;
  register pbool data;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(state == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_READBIT,"uuu",card,subunit,bit)) {
            if(!(sesData.lastError = PICMLX_RecvPacket(NULL,"b",&data)))
              *state = (ps32)data;
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ReadBit != NULL) ? PIL_ReadBit(card,subunit,bit,state) : ER_PIPLX_FUNC_NOT_LOADED;        
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::readSub(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)size) <= 0) || ((size * 4) > MAX_STRING_LENGTH) || (data == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          // First try send new packet format (new SB) ... included data size.
          if(!PICMLX_SendPacket(PIPLX_FID_READSUB,"uuu",card,subunit,size)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"au",size,data);
          } else {
            // Now try old packet format (old SB) ... excluded data size.
            if(!PICMLX_SendPacket(PIPLX_FID_READSUB,"uu",card,subunit)) {
              sesData.lastError = PICMLX_RecvPacket(NULL,"au",size,data);
            }
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ReadSub != NULL) ? PIL_ReadSub(card,subunit,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Switching

pu32 piplx_module_c::clearMask(session_t sid,pu32 card,pu32 subunit) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_CLEARMASK,"uu",card,subunit)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_ClearMask != NULL) ? PIL_ClearMask(card,subunit) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::maskBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 action) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_MASKBIT,"uuub",card,subunit,bit,(pbool)action)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_MaskBit != NULL) ? PIL_MaskBit(card,subunit,bit,action) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::maskCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 action) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_MASKCROSSPOINT,"uuuub",card,subunit,row,col,(pbool)action)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_MaskCrosspoint != NULL) ? PIL_MaskCrosspoint(card,subunit,row,col,action) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::opCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 action) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_OPCROSSPOINT,"uuuub",card,subunit,row,col,(pbool)action)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_OpCrosspoint != NULL) ? PIL_OpCrosspoint(card,subunit,row,col,action) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::opSwitch(session_t sid,pu32 card,pu32 subunit,pu32 func,pu32 segment,pu32 sw,
                              pu32 subSwitch,pu32 switchAction,ps32 *state) {
  register pu32 result;
  register pbool data;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(state == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_OPSWITCH,"uuuuuuu",card,subunit,func,segment,sw,subSwitch,switchAction)) {
            if(!(sesData.lastError = PICMLX_RecvPacket(NULL,"b",&data)))
              *state = data;
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_OpSwitch != NULL) ? PIL_OpSwitch(card,subunit,func,segment,sw,subSwitch,switchAction,state) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::viewCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 *state) {
  register pu32 result;
  register pbool data;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(state == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_VIEWCROSSPOINT,"uuuu",card,subunit,row,col)) {
            if(!(sesData.lastError = PICMLX_RecvPacket(NULL,"b",&data)))
              *state = (ps32)data;
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ViewCrosspoint != NULL) ? PIL_ViewCrosspoint(card,subunit,row,col,state) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::viewMask(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)size) <= 0) || ((size * 4) > MAX_STRING_LENGTH) || (data == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_VIEWMASK,"uuu",card,subunit,size)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"au",size,data);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ViewMask != NULL) ? PIL_ViewMask(card,subunit,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::viewMaskBit(session_t sid,pu32 card,pu32 subunit,pu32 bit,ps32 *state) {
  register pu32 result;
  register pbool data;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(state == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_VIEWMASKBIT,"uuu",card,subunit,bit)) {
            if(!(sesData.lastError = PICMLX_RecvPacket(NULL,"b",&data)))
              *state = (ps32)data;
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ViewMaskBit != NULL) ? PIL_ViewMaskBit(card,subunit,bit,state) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::viewMaskCrosspoint(session_t sid,pu32 card,pu32 subunit,pu32 row,pu32 col,ps32 *state) {
  register pu32 result;
  register pbool data;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(state == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_VIEWMASKCROSSPOINT,"uuuu",card,subunit,row,col)) {
            if(!(sesData.lastError = PICMLX_RecvPacket(NULL,"b",&data)))
              *state = (ps32)data;
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ViewMaskCrosspoint != NULL) ? PIL_ViewMaskCrosspoint(card,subunit,row,col,state) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::writeMask(session_t sid,pu32 card,pu32 subunit,pu32 *data,pu32 size) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)size) <= 0) || ((size * 4) > MAX_STRING_LENGTH) || (data == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_WRITEMASK,"uuau",card,subunit,size,data)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_WriteMask != NULL) ? PIL_WriteMask(card,subunit,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Calibration

pu32 piplx_module_c::readCal(session_t sid,pu32 card,pu32 subunit,pu32 index,pu32 *data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(data == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;    
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_READCAL,"uuu",card,subunit,index)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",data);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ReadCal != NULL) ? PIL_ReadCal(card,subunit,index,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::writeCal(session_t sid,pu32 card,pu32 subunit,pu32 index,pu32 data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_WRITECAL,"uuuu",card,subunit,index,data)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_WriteCal != NULL) ? PIL_WriteCal(card,subunit,index,data) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Floating-point calibration

pu32 piplx_module_c::writeCalFP(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 offset,pu32 numValues,
                                pdouble *data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)numValues) < 0) || (data == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_WRITECALFP,"uuuuad",card,subunit,store,offset,numValues,data)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_WriteCalFP != NULL) ? PIL_WriteCalFP(card,subunit,store,offset,numValues,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::readCalFP(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 offset,pu32 numValues,pdouble *data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)numValues) < 0) || (data == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else    
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_READCALFP,"uuuuu",card,subunit,store,offset,numValues)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"ad",numValues,data);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ReadCalFP != NULL) ? PIL_ReadCalFP(card,subunit,store,offset,numValues,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::writeCalDate(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 interval) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_WRITECALDATE,"uuuu",card,subunit,store,interval)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_WriteCalDate != NULL) ? PIL_WriteCalDate(card,subunit,store,interval) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::readCalDate(session_t sid,pu32 card,pu32 subunit,pu32 store,pu32 *year,pu32 *day,pu32 *interval) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((year == NULL) || (day == NULL) || (interval == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_READCALDATE,"uuu",card,subunit,store)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"uuu",year,day,interval);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ReadCalDate != NULL) ? PIL_ReadCalDate(card,subunit,store,year,day,interval) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::setCalPoint(session_t sid,pu32 card,pu32 subunit,pu32 index) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_SETCALPOINT,"uuu",card,subunit,index)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_SetCalPoint != NULL) ? PIL_SetCalPoint(card,subunit,index) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Attenuator

pu32 piplx_module_c::attenGetAttenuation(session_t sid,pu32 card,pu32 subunit,pfloat *atten) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(atten == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_ATTENGETATTENUATION,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"f",atten);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_AttenGetAttenuation != NULL) ? PIL_AttenGetAttenuation(card,subunit,atten) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::attenInfo(session_t sid,pu32 card,pu32 subunit,pu32 *type,pu32 *numSteps,
                               pfloat *stepSize) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((type == NULL) || (numSteps == NULL) || (stepSize == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_ATTENINFO,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"uuf",type,numSteps,stepSize);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_AttenInfo != NULL) ? PIL_AttenInfo(card,subunit,type,numSteps,stepSize) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::attenPadValue(session_t sid,pu32 card,pu32 subunit,pu32 pad,pfloat *atten) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(atten == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_ATTENPADVALUE,"uuu",card,subunit,pad)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"f",atten);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_AttenPadValue != NULL) ? PIL_AttenPadValue(card,subunit,pad,atten) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::attenSetAttenuation(session_t sid,pu32 card,pu32 subunit,pfloat atten) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_ATTENSETATTENUATION,"uuf",card,subunit,atten)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_AttenSetAttenuation != NULL) ? PIL_AttenSetAttenuation(card,subunit,atten) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::attenType(session_t sid,pu32 card,pu32 subunit,pchar *typeStr,pu32 length) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)length) <= 0) || (length > MAX_DIAG_LENGTH) || (typeStr == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_ATTENTYPE,"uuu",card,subunit,length)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"ac",length,typeStr);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_AttenType != NULL) ? PIL_AttenType(card,subunit,typeStr) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Power supply

pu32 piplx_module_c::psuEnable(session_t sid,pu32 card,pu32 subunit,ps32 state) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_PSUENABLE,"uub",card,subunit,(pbool)state)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_PsuEnable != NULL) ? PIL_PsuEnable(card,subunit,state) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::psuGetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble *voltage) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(voltage == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_PSUGETVOLTAGE,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"d",voltage);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_PsuGetVoltage != NULL) ? PIL_PsuGetVoltage(card,subunit,voltage) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::psuInfo(session_t sid,pu32 card,pu32 subunit,pu32 *type,pdouble *voltage,
                             pdouble *current,pu32 *precis,pu32 *caps) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((type == NULL) || (current == NULL) || (precis == NULL) || (caps == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_PSUINFO,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"udduu",type,voltage,current,precis,caps);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_PsuInfo != NULL) ? PIL_PsuInfo(card,subunit,type,voltage,current,precis,caps) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::psuSetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble voltage) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_PSUSETVOLTAGE,"uud",card,subunit,voltage)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_PsuSetVoltage != NULL) ? PIL_PsuSetVoltage(card,subunit,voltage) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::psuType(session_t sid,pu32 card,pu32 subunit,pchar *typeStr,pu32 length) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)length) <= 0) || (length > MAX_DIAG_LENGTH) || (typeStr == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_PSUTYPE,"uuu",card,subunit,length)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"ac",length,typeStr);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_PsuType != NULL) ? PIL_PsuType(card,subunit,typeStr) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

// Mode control

pu32 piplx_module_c::setMode(session_t sid,pu32 flags) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_SETMODE,"u",flags)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"u",&result);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {
      sesData.lastError = NO_ERROR;
      if(PIL_SetMode != NULL)
        result = PIL_SetMode(flags);
      else
        sesData.lastError = ER_PIPLX_FUNC_NOT_LOADED;
    }
  } else
    sesData.lastError = ER_PICMLX_CLI_INVALID_SESSION;
  PICMLX_EndTransmition(&sesData);
  return sesData.lastError ? INVALID_RESULT : result;
}

// Programmable resistor

pu32 piplx_module_c::resSetResistance(session_t sid,pu32 card,pu32 subunit,pu32 mode,pdouble resistance) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_RESSETRESISTANCE,"uuud",card,subunit,mode,resistance)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_ResSetResistance != NULL) ? PIL_ResSetResistance(card,subunit,mode,resistance) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::resGetResistance(session_t sid,pu32 card,pu32 subunit,pdouble *resistance) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(resistance == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_RESGETRESISTANCE,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"d",resistance);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ResGetResistance != NULL) ? PIL_ResGetResistance(card,subunit,resistance) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::resInfo(session_t sid,pu32 card,pu32 subunit,pdouble *minRes,pdouble *maxRes,pdouble *refRes,
    pdouble *precPC,pdouble *precDelta,pdouble *int1,pdouble *intDelta,pu32 * caps) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((minRes == NULL) || (maxRes == NULL) || (refRes == NULL) || (precPC == NULL) || (precDelta == NULL) || (int1 == NULL) || (intDelta == NULL) || (caps == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_RESINFO,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"dddddddu",minRes,maxRes,refRes,precPC,precDelta,int1,intDelta,caps);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_ResInfo != NULL) ? PIL_ResInfo(card,subunit,minRes,maxRes,refRes,precPC,precDelta,int1,intDelta,caps) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

/* ##################################################################################################### */

// BIRST functions

pu32 piplx_module_c::birstCapabilities(session_t sid,pu32 card,pu32 *topology,pu32 *caps,pu32 *refRes) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((topology == NULL) || (caps == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTCAPABILITIES,"u",card)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"uuu",topology,caps,refRes);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstCapabilities != NULL) ? PIL_BirstCapabilities(card,topology,caps,refRes) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstInfo(session_t sid,pu32 card,pu32 outSub,pu32* poles,pchar* axis1,pu32* line1,pchar* axis2,pu32* line2) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((poles == NULL) || (axis1 == NULL) || (line1 == NULL) || (axis2 == NULL) || (line2 == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTINFO,"uu",card,outSub)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"ucucu",poles,axis1,line1,axis2,line2);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstInfo != NULL) ? PIL_BirstInfo(card,outSub,poles,axis1,line1,axis2,line2) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstSetControls(session_t sid,pu32 card,pu32 data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_BIRSTSETCONTROLS,"uu",card,data)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_BirstSetControls != NULL) ? PIL_BirstSetControls(card,data) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstGetControls(session_t sid,pu32 card,pu32* data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(data == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTGETCONTROLS,"u",card)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",data);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstGetControls != NULL) ? PIL_BirstGetControls(card,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstMeasurement(session_t sid,pu32 card,pu32 outSub,pu32* value) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(value == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTMEASUREMENT,"uu",card,outSub)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",value);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstMeasurement != NULL) ? PIL_BirstMeasurement(card,outSub,value) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstWriteCal(session_t sid,pu32 card,pu32 outSub,pu32 pole,pu32 offset,pu32 numValues,pu32 *data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((data == NULL) || (((ps32)numValues) < 0))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTWRITECAL,"uuuuau",card,outSub,pole,offset,numValues,data)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstWriteCal != NULL) ? PIL_BirstWriteCal(card,outSub,pole,offset,numValues,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstReadCal(session_t sid,pu32 card,pu32 outSub,pu32 pole,pu32 offset,pu32 numValues,pu32 *data) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((data == NULL) || (((ps32)numValues) < 0))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTREADCAL,"uuuuu",card,outSub,pole,offset,numValues)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"au",numValues,data);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstReadCal != NULL) ? PIL_BirstReadCal(card,outSub,pole,offset,numValues,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstWriteCalDate(session_t sid,pu32 card,pu32 outSub) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {    
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_BIRSTWRITECALDATE,"uu",card,outSub)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_BirstWriteCalDate != NULL) ? PIL_BirstWriteCalDate(card,outSub) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstReadCalDate(session_t sid,pu32 card, pu32 outSub,pu32 *year,pu32 * day) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((year == NULL) || (day == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTREADCALDATE,"uu",card,outSub)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"uu",year,day);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstReadCalDate != NULL) ? PIL_BirstReadCalDate(card,outSub,year,day) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstGetAttribute(session_t sid,pu32 card, pu32 subNum, ps32 out, pu32 attrCode, pu32 *attrValue) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(attrValue == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTATTRIBUTE,"uubu",card,subNum,out,attrCode)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",attrValue);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstGetAttribute != NULL) ? PIL_BirstGetAttribute(card,subNum,out,attrCode,attrValue) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstSetControlsEx(session_t sid,pu32 card,pu32 ctrlReg,pu32 data)
{
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_BIRSTSETCONTROLSEX,"uuu",card,ctrlReg,data)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {
      sesData.lastError = (PIL_BirstSetControlsEx != NULL) ? PIL_BirstSetControlsEx(card,ctrlReg,data) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::birstGetControlsEx(session_t sid,pu32 card,pu32 ctrlReg,pu32* data)
{
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(data == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BIRSTGETCONTROLSEX,"uu",card,ctrlReg)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",data);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BirstGetControlsEx != NULL) ? PIL_BirstGetControlsEx(card,ctrlReg,data) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

/* ##################################################################################################### */

// Battery Simulator
pu32 piplx_module_c::battGetCurrent(session_t sid,pu32 card,pu32 subunit,pdouble *current) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(current == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BATTGETCURRENT,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"d",current);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BattGetCurrent != NULL) ? PIL_BattGetCurrent(card,subunit,current) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::battGetEnable(session_t sid,pu32 card,pu32 subunit,pu32 *pattern) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(pattern == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BATTGETENABLE,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"u",pattern);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BattGetEnable != NULL) ? PIL_BattGetEnable(card,subunit,pattern) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::battGetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble *voltage) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(voltage == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BATTGETVOLTAGE,"uu",card,subunit)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"d",voltage);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BattGetVoltage != NULL) ? PIL_BattGetVoltage(card,subunit,voltage) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::battReadInterlockState(session_t sid,pu32 card,pu32 subunit,ps32 *interlock) {
  register pu32 result;
  register pbool data;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(interlock == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_BATTREADINTERLOCKSTATE,"uu",card,subunit)) {
            if(!(sesData.lastError = PICMLX_RecvPacket(NULL,"b",&data)))
              *interlock = (ps32)data;
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_BattReadInterlockState != NULL) ? PIL_BattReadInterlockState(card,subunit,interlock) : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::battSetCurrent(session_t sid,pu32 card,pu32 subunit,pdouble current) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_BATTSETCURRENT,"uud",card,subunit,current)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_BattSetCurrent != NULL) ? PIL_BattSetCurrent(card,subunit,current) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::battSetEnable(session_t sid,pu32 card,pu32 subunit,pu32 pattern) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_BATTSETENABLE,"uuu",card,subunit,pattern)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_BattSetEnable != NULL) ? PIL_BattSetEnable(card,subunit,pattern) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::battSetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble voltage) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_BATTSETVOLTAGE,"uud",card,subunit,voltage)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_BattSetVoltage != NULL) ? PIL_BattSetVoltage(card,subunit,voltage) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

/* ##################################################################################################### */
// Vsource functions

pu32 piplx_module_c::vsourceSetRange(session_t sid,pu32 card,pu32 subunit,pdouble range) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_VSOURCESETRANGE,"uud",card,subunit,range)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_VsourceSetRange != NULL) ? PIL_VsourceSetRange(card,subunit,range) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::vsourceGetRange(session_t sid,pu32 card,pu32 subunit,pdouble *range) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_VSOURCEGETRANGE,"uu",card,subunit)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"d",range);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_VsourceGetRange != NULL) ? PIL_VsourceGetRange(card,subunit,range) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::vsourceSetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble voltage) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_VSOURCESETVOLTAGE,"uud",card,subunit,voltage)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_VsourceSetVoltage != NULL) ? PIL_VsourceSetVoltage(card,subunit,voltage) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::vsourceGetVoltage(session_t sid,pu32 card,pu32 subunit,pdouble *voltage) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_VSOURCEGETVOLTAGE,"uu",card,subunit)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"d",voltage);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_VsourceGetVoltage != NULL) ? PIL_VsourceGetVoltage(card,subunit,voltage) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::vsourceSetEnable(session_t sid,pu32 card,pu32 subunit,pu32 pattern) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_VSOURCESETENABLE,"uuu",card,subunit,pattern)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,NULL);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_VsourceSetEnable != NULL) ? PIL_VsourceSetEnable(card,subunit,pattern) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::vsourceGetEnable(session_t sid,pu32 card,pu32 subunit,pu32 *pattern) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {      
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPLX_FID_VSOURCEGETENABLE,"uu",card,subunit)) {
          sesData.lastError = PICMLX_RecvPacket(NULL,"u",pattern);
        }
      } else {
        sesData.lastError = ER_PIPLX_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = (PIL_VsourceGetEnable != NULL) ? PIL_VsourceGetEnable(card,subunit,pattern) : ER_PIPLX_FUNC_NOT_LOADED;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 piplx_module_c::getStatusMessage(session_t sid, pu32 card, pchar *buffer, pu32 size) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)size) <= 0) || (size > MAX_DIAG_LENGTH) || (buffer == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
		if(sesData.remote) {      
		  if(sesData.connected) {
			  if(!PICMLX_SendPacket(PIPLX_FID_GETSTATUSMESSAGE,"uu", card, size)) {
			  sesData.lastError = PICMLX_RecvPacket(NULL,"ac", size, buffer);
			}
		  } else {
			sesData.lastError = ER_PIPLX_NOT_CONNECTED;
		  }
		} else {      
		  sesData.lastError = ER_PIPLX_FUNC_NOT_LOADED;
		}
		result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}


/* ##################################################################################################### */

/*
{
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if()
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {      
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPLX_FID_,"",)) {
            sesData.lastError = PICMLX_RecvPacket(NULL,"",);
          }
        } else {
          sesData.lastError = ER_PIPLX_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = (PIL_Birst != NULL) ? PIL_Birst() : ER_PIPLX_FUNC_NOT_LOADED;
      }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}
*/
