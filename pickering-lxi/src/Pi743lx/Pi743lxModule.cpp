#include <Pi743lxModule.h>
#include <PicmlxModuleAPI.h>
#include <Pi743lxFids.h>
#include <Pi743lx.h>
#include <System.h>
#include <DebugTool.h>

#ifdef WIN32
	extern handle_t __libPi743_Instance;
#endif

/* ##################################################################################################### */

/*
 * Private class functions
 */

#define PI743_ERROR_MAXPARAMETERS 10

bool pi743lx_module_c::isBufferBigEnougth(int requestedSize) {
  return SOCKET_BUFFER_SIZE <= requestedSize;
}

//GETLIBPROC:
//find:			{DWORD} _PI743_FUNC {[a-zA-Z743_]*} *\({[a-zA-Z ,\*_\[\]0-9]*}\);
//replace:		\tGETLIBPROC(\2, \2_f, libraryHandle, "\2");

void pi743lx_module_c::loadFunctions(module_t libraryHandle) {
	GETLIBPROC(PI743_ClearOverCurrentStatus, pi743_ClearOverCurrentStatus_f, libraryHandle, "pi743_ClearOverCurrentStatus");
	GETLIBPROC(PI743_ClearTriggerStatus, pi743_ClearTriggerStatus_f, libraryHandle, "pi743_ClearTriggerStatus");
	GETLIBPROC(PI743_close, pi743_close_f, libraryHandle, "pi743_close");
	GETLIBPROC(PI743_error_message, pi743_error_message_f, libraryHandle, "pi743_error_message");
	GETLIBPROC(PI743_error_query, pi743_error_query_f, libraryHandle, "pi743_error_query");
	GETLIBPROC(PI743_GetAddressCounter, pi743_GetAddressCounter_f, libraryHandle, "pi743_GetAddressCounter");
	GETLIBPROC(PI743_GetClockDivider, pi743_GetClockDivider_f, libraryHandle, "pi743_GetClockDivider");
	GETLIBPROC(PI743_GetMeasuredOutputCurrent, pi743_GetMeasuredOutputCurrent_f, libraryHandle, "pi743_GetMeasuredOutputCurrent");
	GETLIBPROC(PI743_GetMeasuredOutputVoltage, pi743_GetMeasuredOutputVoltage_f, libraryHandle, "pi743_GetMeasuredOutputVoltage");
	GETLIBPROC(PI743_GetOutputAll, pi743_GetOutputAll_f, libraryHandle, "pi743_GetOutputAll");
	GETLIBPROC(PI743_GetOutputConnection, pi743_GetOutputConnection_f, libraryHandle, "pi743_GetOutputConnection");
	GETLIBPROC(PI743_GetOutputCurrentLimit, pi743_GetOutputCurrentLimit_f, libraryHandle, "pi743_GetOutputCurrentLimit");
	GETLIBPROC(PI743_GetOutputVoltage, pi743_GetOutputVoltage_f, libraryHandle, "pi743_GetOutputVoltage");
	GETLIBPROC(PI743_GetOverCurrentStatus, pi743_GetOverCurrentStatus_f, libraryHandle, "pi743_GetOverCurrentStatus");
	GETLIBPROC(PI743_GetPowerSupplyStatus, pi743_GetPowerSupplyStatus_f, libraryHandle, "pi743_GetPowerSupplyStatus");
	GETLIBPROC(PI743_GetTrigger, pi743_GetTrigger_f, libraryHandle, "pi743_GetTrigger");
	GETLIBPROC(PI743_GetTriggerCurrent, pi743_GetTriggerCurrent_f, libraryHandle, "pi743_GetTriggerCurrent");
	GETLIBPROC(PI743_GetTriggerStatus, pi743_GetTriggerStatus_f, libraryHandle, "pi743_GetTriggerStatus");
	GETLIBPROC(PI743_GetTriggerVoltage, pi743_GetTriggerVoltage_f, libraryHandle, "pi743_GetTriggerVoltage");
	GETLIBPROC(PI743_init, pi743_init_f, libraryHandle, "pi743_init");
	GETLIBPROC(PI743_MemoryTest, pi743_MemoryTest_f, libraryHandle, "pi743_MemoryTest");
	GETLIBPROC(PI743_ReadCapturedCurrent, pi743_ReadCapturedCurrent_f, libraryHandle, "pi743_ReadCapturedCurrent");
	GETLIBPROC(PI743_ReadCapturedVoltage, pi743_ReadCapturedVoltage_f, libraryHandle, "pi743_ReadCapturedVoltage");
	GETLIBPROC(PI743_ReadCurrentCalValues, pi743_ReadCurrentCalValues_f, libraryHandle, "pi743_ReadCurrentCalValues");
	GETLIBPROC(PI743_ReadID, pi743_ReadID_f, libraryHandle, "pi743_ReadID");
	GETLIBPROC(PI743_ReadMaximumPower, pi743_ReadMaximumPower_f, libraryHandle, "pi743_ReadMaximumPower");
	GETLIBPROC(PI743_ReadVoltageCalValues, pi743_ReadVoltageCalValues_f, libraryHandle, "pi743_ReadVoltageCalValues");
	GETLIBPROC(PI743_reset, pi743_reset_f, libraryHandle, "pi743_reset");
	GETLIBPROC(PI743_revision_query, pi743_revision_query_f, libraryHandle, "pi743_revision_query");
	GETLIBPROC(PI743_self_test, pi743_self_test_f, libraryHandle, "pi743_self_test");
	GETLIBPROC(PI743_SetAddressCounter, pi743_SetAddressCounter_f, libraryHandle, "pi743_SetAddressCounter");
	GETLIBPROC(PI743_SetClockDivider, pi743_SetClockDivider_f, libraryHandle, "pi743_SetClockDivider");
	GETLIBPROC(PI743_SetOutputAll, pi743_SetOutputAll_f, libraryHandle, "pi743_SetOutputAll");
	GETLIBPROC(PI743_SetOutputConnection, pi743_SetOutputConnection_f, libraryHandle, "pi743_SetOutputConnection");
	GETLIBPROC(PI743_SetOutputCurrentLimit, pi743_SetOutputCurrentLimit_f, libraryHandle, "pi743_SetOutputCurrentLimit");
	GETLIBPROC(PI743_SetOutputVoltage, pi743_SetOutputVoltage_f, libraryHandle, "pi743_SetOutputVoltage");
	GETLIBPROC(PI743_SetPowerSupplyStatus, pi743_SetPowerSupplyStatus_f, libraryHandle, "pi743_SetPowerSupplyStatus");
	GETLIBPROC(PI743_SetSoftwareTrigger, pi743_SetSoftwareTrigger_f, libraryHandle, "pi743_SetSoftwareTrigger");
	GETLIBPROC(PI743_SetTrigger, pi743_SetTrigger_f, libraryHandle, "pi743_SetTrigger");
	GETLIBPROC(PI743_SetTriggerCurrent, pi743_SetTriggerCurrent_f, libraryHandle, "pi743_SetTriggerCurrent");
	GETLIBPROC(PI743_SetTriggerVoltage, pi743_SetTriggerVoltage_f, libraryHandle, "pi743_SetTriggerVoltage");
	GETLIBPROC(PI743_StoreCurrentCalValues, pi743_StoreCurrentCalValues_f, libraryHandle, "pi743_StoreCurrentCalValues");
	GETLIBPROC(PI743_StoreVoltageCalValues, pi743_StoreVoltageCalValues_f, libraryHandle, "pi743_StoreVoltageCalValues");
	GETLIBPROC(PI743_WriteMaximumPower, pi743_WriteMaximumPower_f, libraryHandle, "pi743_WriteMaximumPower");
	GETLIBPROC(PI743_FindInstruments, pi743_FindInstruments_f, libraryHandle, "pi743_FindInstruments");
	GETLIBPROC(PI743_FindInstrumentsRsrc, pi743_FindInstrumentsRsrc_f, libraryHandle, "pi743_FindInstrumentsRsrc");
	GETLIBPROC(PI743_IsStabilized, pi743_IsStabilized_f, libraryHandle, "pi743_IsStabilized");
	GETLIBPROC(PI743_WaitForStabilized, pi743_WaitForStabilized_f, libraryHandle, "pi743_WaitForStabilized");
	GETLIBPROC(PI743_WriteMemory, pi743_WriteMemory_f, libraryHandle, "pi743_WriteMemory");
//low functions
	GETLIBPROC(pi743_ReadAdc, pi743_ReadAdc_f, libraryHandle, "pi743_ReadAdc");
	GETLIBPROC(pi743_ReadCurrentDac, pi743_ReadCurrentDac_f, libraryHandle, "pi743_ReadCurrentDac");
	GETLIBPROC(pi743_ReadEeprom, pi743_ReadEeprom_f, libraryHandle, "pi743_ReadEeprom");
	GETLIBPROC(pi743_ReadOutputDac, pi743_ReadOutputDac_f, libraryHandle, "pi743_ReadOutputDac");
	GETLIBPROC(pi743_ReadRegister, pi743_ReadRegister_f, libraryHandle, "pi743_ReadRegister");
	GETLIBPROC(pi743_WriteCurrentDac, pi743_WriteCurrentDac_f, libraryHandle, "pi743_WriteCurrentDac");
	GETLIBPROC(pi743_WriteEeprom, pi743_WriteEeprom_f, libraryHandle, "pi743_WriteEeprom");
	GETLIBPROC(pi743_WriteID, pi743_WriteID_f, libraryHandle, "pi743_WriteID");
	GETLIBPROC(pi743_WriteOutputDac, pi743_WriteOutputDac_f, libraryHandle, "pi743_WriteOutputDac");
	GETLIBPROC(pi743_WriteRegister, pi743_WriteRegister_f, libraryHandle, "pi743_WriteRegister");
}

/* ##################################################################################################### */

/*
 * Public class functions
 */

// Constructor/destructor

pi743lx_module_c::pi743lx_module_c(const pchar *libName) : base_module_c(libName) {
	init();
}

pi743lx_module_c::~pi743lx_module_c() {
	//base_module_c::~base_module_c();
}

/* ##################################################################################################### */

// Error handling functions

pbool pi743lx_module_c::isError(SESSION sid) {
	pu32 code;

	if(getLastError(sid,&code))
		return true;		
	return (pbool)code;
}

pu32 pi743lx_module_c::getLastError(SESSION sid,pu32 *code) {	
	return PICMLX_GetLastErrorCode(sid,code);
}

pu32 pi743lx_module_c::getLastErrorMessage(SESSION sid, pchar *buffer,pu32 size) {
	pu32 code,result,length;
	pchar fmtStr[512];
		
	if((buffer == NULL) || (((ps32)size) <= 0)) {
		return ER_PICMLX_CLI_PARAM_SIZE;
	}	
	result = getLastError(sid,&code);
	if(!result)
		if(code >= 0) {
			code = errorCode2Message(sid, code, buffer, size);					
			sprintf_s(fmtStr,sizeof(fmtStr),"%s [SysError: %d]",buffer,GetLastError()); //sprintf_s(fmtStr,sizeof(fmtStr),"%s, (SystemErrorCode: %d)",buffer,GetLastError());
			length = (pu32)strlen(fmtStr);
			if(size <= length) {
				fmtStr[size - 1] = 0;
			}
			strcpy_s(buffer,size,fmtStr);
		}	
	return result;
}

pu32 pi743lx_module_c::errorCode2Message(SESSION sid, pu32 code,pchar *buffer, pu32 size) {
	pu32 error = 0;

	if((buffer == NULL) || (((ps32)size) <= 0))
		code = ER_PICMLX_CLI_PARAM_SIZE;

	if(code == PI743LX_ERROR_FUNC_NOT_LOADED) {
		strcpy_s(buffer, size, "Desired function not loaded from \"pi743\" library.");
		return NO_ERROR;
	}

	if(code == PI743LX_ERROR_FUNC_NOT_LOCAL) {
		strcpy_s(buffer, size, "Desired function cannot be used on localhost.");
		return NO_ERROR;
	}

	if(code == 0 || 
		((code > PI743LX_SPECIFIC_ERROR_BASE) && (code <= PI743LX_ERROR_SETTLETIMEOUT)) || 
		((code > PI743LX_SPECIFIC_WARNING_BASE) && (code <= PI743LX_WARNING_NSUP_REV_QUERY)) ||
		(code >= PI743LX_ERROR_PARAMETER1 && code < PI743LX_ERROR_PARAMETER1+PI743_ERROR_MAXPARAMETERS))
		error = error_message(sid, 0, code, buffer, size);
	else
		error = code;

	if(error)
		error = PICMLX_ErrorCodeToMessage(code, buffer, size);

	return error;
}

/* ##################################################################################################### */

//regular expressions:
//find: \t{DWORD} {[a-zA-Z_]*}\({[a-zA-Z ,\*_\[\]0-9]*}\);

//replace: \1 pi743lx_module_c::\2(SESSION sid, \3) {\n\tregister pu32 result;\n\tsession_data_s sesData = {0};\n\n\t/* DEBUG RECORD BEGIN */\n\t#ifdef DEBUG\n\t\t__DEBUGMSG(FALSE,"pi743lx_module_c::\2 - begin");\n\t#endif\n\t/* DEBUG RECORD END */\n\tresult = PICMLX_BeginTransmition(sid,&sesData);\n\tif(!result) {\n\t\tif(sesData.remote) {\n\t\t\tif(sesData.connected) {\n\t\t\t\tif(!PICMLX_SendPacket(PI743LX_FID_\2, \3)) {\n\t\t\t\t\tsesData.lastError = PICMLX_RecvPacket(NULL, \3);\n\t\t\t\t}\n\t\t\t} else {\n\t\t\t\tsesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;\n\t\t\t}\n\t\t} else {\n\t\t\tsesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;\n\t\t\tif(PI743_\2 != NULL) {\n\t\t\t\tPI743_\2(\3);\n\t\t\t\tsesData.lastError = NO_ERROR;\n\t\t\t}\n\t\t}\n\t\tresult = sesData.lastError;\n\t}\n\tPICMLX_EndTransmition(&sesData);\n\t/* DEBUG RECORD BEGIN */\n\t#ifdef DEBUG\n\t\t__DEBUGMSG(TRUE,"pi743lx_module_c::\2 - end");\n\t#endif\n\t/* DEBUG RECORD END */\n\treturn result;\n}\n

DWORD pi743lx_module_c::error_message(SESSION sid, DWORD cardHandle, DWORD statusCode, CHAR errorMessage[], DWORD length) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::error_message - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_ERRORMESSAGE, "uuu", cardHandle, statusCode, length)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "ac", length, errorMessage);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_error_message != NULL) {
				PI743_error_message(cardHandle, statusCode, errorMessage);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::error_message - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ClearOverCurrentStatus(SESSION sid, DWORD cardHandle) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ClearOverCurrentStatus - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_CLEAROVERCURRENTSTATUS, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ClearOverCurrentStatus != NULL) {
				PI743_ClearOverCurrentStatus(cardHandle);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ClearOverCurrentStatus - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ClearTriggerStatus(SESSION sid, DWORD cardHandle) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ClearTriggerStatus - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_CLEARTRIGGERSTATUS, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ClearTriggerStatus != NULL) {
				PI743_ClearTriggerStatus(cardHandle);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ClearTriggerStatus - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::close(SESSION sid, DWORD cardHandle) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::close - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_CLOSE, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_close != NULL) {
				PI743_close(cardHandle);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::close - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetAddressCounter(SESSION sid, DWORD cardHandle, DWORD* addresscounter) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetAddressCounter - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETADDRESSCOUNTER, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", addresscounter);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetAddressCounter != NULL) {
				PI743_GetAddressCounter(cardHandle, addresscounter);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetAddressCounter - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetClockDivider(SESSION sid, DWORD cardHandle, DWORD* divider) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetClockDivider - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETCLOCKDIVIDER, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", divider);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetClockDivider != NULL) {
				PI743_GetClockDivider(cardHandle, divider);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetClockDivider - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetMeasuredOutputCurrent(SESSION sid, DWORD cardHandle, double* current) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetMeasuredOutputCurrent - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETMEASUREDOUTPUTCURRENT, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "d", current);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetMeasuredOutputCurrent != NULL) {
				PI743_GetMeasuredOutputCurrent(cardHandle, current);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetMeasuredOutputCurrent - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetMeasuredOutputVoltage(SESSION sid, DWORD cardHandle, double* voltage) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetMeasuredOutputVoltage - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETMEASUREDOUTPUTVOLTAGE, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "d", voltage);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetMeasuredOutputVoltage != NULL) {
				PI743_GetMeasuredOutputVoltage(cardHandle, voltage);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetMeasuredOutputVoltage - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetOutputAll(SESSION sid, DWORD cardHandle, double *voltage, double *current, double *power, DWORD *connection) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetOutputAll - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETOUTPUTALL, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "dddu", voltage, current, power, connection);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetOutputAll != NULL) {
				PI743_GetOutputAll(cardHandle, voltage, current, power, connection);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetOutputAll - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetOutputConnection(SESSION sid, DWORD cardHandle, DWORD* connection) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetOutputConnection - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETOUTPUTCONNECTION, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", connection);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetOutputConnection != NULL) {
				PI743_GetOutputConnection(cardHandle, connection);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetOutputConnection - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetOutputCurrentLimit(SESSION sid, DWORD cardHandle, double* current) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetOutputCurrentLimit - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETOUTPUTCURRENTLIMIT, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "d", current);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetOutputCurrentLimit != NULL) {
				PI743_GetOutputCurrentLimit(cardHandle, current);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetOutputCurrentLimit - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetOutputVoltage(SESSION sid, DWORD cardHandle, double* voltage) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetOutputVoltage - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETOUTPUTVOLTAGE, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "d", voltage);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetOutputVoltage != NULL) {
				PI743_GetOutputVoltage(cardHandle, voltage);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetOutputVoltage - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetOverCurrentStatus(SESSION sid, DWORD cardHandle, DWORD* overcurrent) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetOverCurrentStatus - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETOVERCURRENTSTATUS, "u" , cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", overcurrent);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetOverCurrentStatus != NULL) {
				PI743_GetOverCurrentStatus(cardHandle, overcurrent);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetOverCurrentStatus - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetPowerSupplyStatus(SESSION sid, DWORD cardHandle, DWORD* psstatus) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetPowerSupplyStatus - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETPOWERSUPPLYSTATUS, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", psstatus);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetPowerSupplyStatus != NULL) {
				PI743_GetPowerSupplyStatus(cardHandle, psstatus);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetPowerSupplyStatus - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetTrigger(SESSION sid, DWORD cardHandle, DWORD* source, DWORD* action, DWORD* level, DWORD *mode) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetTrigger - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETTRIGGER, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "uuuu", source, action, level, mode);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetTrigger != NULL) {
				PI743_GetTrigger(cardHandle, source, action, level, mode);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetTrigger - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetTriggerCurrent(SESSION sid, DWORD cardHandle, double* current) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetTriggerCurrent - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETTRIGGERCURRENT, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "d", current);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetTriggerCurrent != NULL) {
				PI743_GetTriggerCurrent(cardHandle, current);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetTriggerCurrent - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetTriggerStatus(SESSION sid, DWORD cardHandle, DWORD* triggerstatus) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetTriggerStatus - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETTRIGGERSTATUS, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", triggerstatus);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetTriggerStatus != NULL) {
				PI743_GetTriggerStatus(cardHandle, triggerstatus);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetTriggerStatus - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::GetTriggerVoltage(SESSION sid, DWORD cardHandle, double* voltage) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::GetTriggerVoltage - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_GETTRIGGERVOLTAGE, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "d", voltage);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_GetTriggerVoltage != NULL) {
				PI743_GetTriggerVoltage(cardHandle, voltage);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::GetTriggerVoltage - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::initCard(SESSION sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD* session) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::initCard - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_INIT, "acbb", strlen(resourceDescription) + 1,  resourceDescription, IDQuery, reset)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", session);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_init != NULL) {
                PI743_init(resourceDescription, IDQuery, reset,  session);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::initCard - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::MemoryTest(SESSION sid, DWORD cardHandle, DWORD logSize, DWORD *erroraddress, DWORD *errordata, DWORD *expectdata, DWORD *foundErrors) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::MemoryTest - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_MEMORYTEST, "uu", cardHandle, logSize)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "auauauu", logSize, erroraddress, logSize, errordata, logSize, expectdata, foundErrors);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_MemoryTest != NULL) {
				PI743_MemoryTest(cardHandle, logSize, erroraddress, errordata, expectdata, foundErrors);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::MemoryTest - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadCapturedCurrent(SESSION sid, DWORD cardHandle, double* current, DWORD count) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadCapturedCurrent - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READCAPTUREDCURRENT, "uu", cardHandle, count)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "ad", count, current);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ReadCapturedCurrent != NULL) {
				PI743_ReadCapturedCurrent(cardHandle, current, count);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadCapturedCurrent - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadCapturedVoltage(SESSION sid, DWORD cardHandle, double* voltages, DWORD count) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadCapturedVoltage - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READCAPTUREDVOLTAGE, "uu", cardHandle, count)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "ad", count, voltages);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ReadCapturedVoltage != NULL) {
				PI743_ReadCapturedVoltage(cardHandle, voltages, count);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadCapturedVoltage - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadCurrentCalValues(SESSION sid, DWORD cardHandle, double* offsetcurrentcdac, double* gaincurrentcdac, DWORD* offsetcodecadc, DWORD* gaincodecadc) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadCurrentCalValues - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READCURRENTCALVALUES, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "dduu", offsetcurrentcdac, gaincurrentcdac, offsetcodecadc, gaincodecadc);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ReadCurrentCalValues != NULL) {
				PI743_ReadCurrentCalValues(cardHandle, offsetcurrentcdac, gaincurrentcdac, offsetcodecadc, gaincodecadc);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadCurrentCalValues - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadID(SESSION sid, DWORD cardHandle, DWORD* id) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadID - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READID, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", id);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ReadID != NULL) {
				PI743_ReadID(cardHandle, id);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadID - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadMaximumPower(SESSION sid, DWORD cardHandle, double* power) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadMaximumPower - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READMAXIMUMPOWER, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "d", power);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ReadMaximumPower != NULL) {
				PI743_ReadMaximumPower(cardHandle, power);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadMaximumPower - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadVoltageCalValues(SESSION sid, DWORD cardHandle, double* offsetvoltagevdac, double* gainvoltagevdac, DWORD* offsetcodevadc,DWORD* gaincodevadc) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadVoltageCalValues - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READVOLTAGECALVALUES, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "dduu", offsetvoltagevdac, gainvoltagevdac, offsetcodevadc, gaincodevadc);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_ReadVoltageCalValues != NULL) {
				PI743_ReadVoltageCalValues(cardHandle, offsetvoltagevdac, gainvoltagevdac, offsetcodevadc,gaincodevadc);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadVoltageCalValues - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::reset(SESSION sid, DWORD cardHandle) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::reset - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_RESET, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_reset != NULL) {
				PI743_reset(cardHandle);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::reset - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::self_test(SESSION sid, DWORD cardHandle, BOOL* testResult, CHAR errorMessage[], DWORD errorMessageSize) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::self_test - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SELFTEST, "uu", cardHandle, errorMessageSize)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "bac", testResult, errorMessageSize, errorMessage);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_self_test != NULL) {
				PI743_self_test(cardHandle, testResult, errorMessage);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::self_test - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetAddressCounter(SESSION sid, DWORD cardHandle, DWORD addresscounter) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetAddressCounter - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETADDRESSCOUNTER, "uu", cardHandle, addresscounter)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetAddressCounter != NULL) {
				PI743_SetAddressCounter(cardHandle, addresscounter);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetAddressCounter - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetClockDivider(SESSION sid, DWORD cardHandle, DWORD divider) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetClockDivider - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETCLOCKDIVIDER, "uu", cardHandle, divider)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetClockDivider != NULL) {
				PI743_SetClockDivider(cardHandle, divider);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetClockDivider - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetOutputAll(SESSION sid, DWORD cardHandle, double voltage, double current, double power, DWORD connection) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetOutputAll - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETOUTPUTALL, "udddu", cardHandle, voltage, current, power, connection)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetOutputAll != NULL) {
				PI743_SetOutputAll(cardHandle, voltage, current, power, connection);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetOutputAll - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetOutputConnection(SESSION sid, DWORD cardHandle, DWORD connection) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetOutputConnection - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETOUTPUTCONNECTION, "uu", cardHandle, connection)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetOutputConnection != NULL) {
				PI743_SetOutputConnection(cardHandle, connection);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetOutputConnection - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetOutputCurrentLimit(SESSION sid, DWORD cardHandle, double current) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetOutputCurrentLimit - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETOUTPUTCURRENTLIMIT, "ud", cardHandle, current)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetOutputCurrentLimit != NULL) {
				PI743_SetOutputCurrentLimit(cardHandle, current);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetOutputCurrentLimit - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetOutputVoltage(SESSION sid, DWORD cardHandle, double voltage) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetOutputVoltage - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETOUTPUTVOLTAGE, "ud", cardHandle, voltage)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetOutputVoltage != NULL) {
				PI743_SetOutputVoltage(cardHandle, voltage);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetOutputVoltage - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetPowerSupplyStatus(SESSION sid, DWORD cardHandle, DWORD psstatus) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetPowerSupplyStatus - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETPOWERSUPPLYSTATUS, "uu", cardHandle, psstatus)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetPowerSupplyStatus != NULL) {
				PI743_SetPowerSupplyStatus(cardHandle, psstatus);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetPowerSupplyStatus - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetSoftwareTrigger(SESSION sid, DWORD cardHandle) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetSoftwareTrigger - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETSOFTWARETRIGGER, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetSoftwareTrigger != NULL) {
				PI743_SetSoftwareTrigger(cardHandle);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetSoftwareTrigger - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetTrigger(SESSION sid, DWORD cardHandle, DWORD source, DWORD action, DWORD level, DWORD mode) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetTrigger - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETTRIGGER, "uuuuu", cardHandle, source, action, level, mode)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetTrigger != NULL) {
				PI743_SetTrigger(cardHandle, source, action, level, mode);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetTrigger - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetTriggerCurrent(SESSION sid, DWORD cardHandle, double current) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetTriggerCurrent - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETTRIGGERCURRENT, "ud", cardHandle, current)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetTriggerCurrent != NULL) {
				PI743_SetTriggerCurrent(cardHandle, current);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetTriggerCurrent - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::SetTriggerVoltage(SESSION sid, DWORD cardHandle, double voltage) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::SetTriggerVoltage - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_SETTRIGGERVOLTAGE, "ud", cardHandle, voltage)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_SetTriggerVoltage != NULL) {
				PI743_SetTriggerVoltage(cardHandle, voltage);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::SetTriggerVoltage - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::StoreCurrentCalValues(SESSION sid, DWORD cardHandle, double offsetcurrent, double gaincurrentunsigned, DWORD offsetcode, DWORD gaincode) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::StoreCurrentCalValues - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_STORECURRENTCALVALUES, "udduu", cardHandle, offsetcurrent, gaincurrentunsigned, offsetcode, gaincode)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_StoreCurrentCalValues != NULL) {
				PI743_StoreCurrentCalValues(cardHandle, offsetcurrent, gaincurrentunsigned, offsetcode, gaincode);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::StoreCurrentCalValues - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::StoreVoltageCalValues(SESSION sid, DWORD cardHandle, double offsetvoltage, double gainvoltage, DWORD offsetcode, DWORD gaincode) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::StoreVoltageCalValues - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_STOREVOLTAGECALVALUES, "udduu", cardHandle, offsetvoltage, gainvoltage, offsetcode, gaincode)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_StoreVoltageCalValues != NULL) {
				PI743_StoreVoltageCalValues(cardHandle, offsetvoltage, gainvoltage, offsetcode, gaincode);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::StoreVoltageCalValues - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WriteMaximumPower(SESSION sid, DWORD cardHandle, double power) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WriteMaximumPower - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WRITEMAXIMUMPOWER, "ud", cardHandle, power)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_WriteMaximumPower != NULL) {
				PI743_WriteMaximumPower(cardHandle, power);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WriteMaximumPower - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::FindInstruments(SESSION sid, DWORD buslist[], DWORD devicelist[], DWORD *count) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::FindInstruments - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_FINDINSTRUMENTS, "uu", *count, *count)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "auauu", *count, buslist, *count, devicelist, count);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_FindInstruments != NULL) {
				PI743_FindInstruments(buslist, devicelist, count);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::FindInstruments - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::FindInstrumentsRsrc(SESSION sid, CHAR rsrclist[][20], DWORD *count) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::FindInstrumentsRsrc - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_FINDINSTRUMENTSRSRC, "u", (*count)*20)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "acu", (*count)*20, rsrclist, count);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_FindInstrumentsRsrc != NULL) {
				PI743_FindInstrumentsRsrc(rsrclist, count);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::FindInstrumentsRsrc - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::IsStabilized(SESSION sid, DWORD cardHandle, BOOL *stabilized) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::IsStabilized - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_ISSTABILIZED, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "b", stabilized);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_IsStabilized != NULL) {
				PI743_IsStabilized(cardHandle, stabilized);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::IsStabilized - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WaitForStabilized(SESSION sid, DWORD cardHandle, DWORD timeout) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WaitForStabilized - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WAITFORSTABILIZED, "uu", cardHandle, timeout)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_WaitForStabilized != NULL) {
				PI743_WaitForStabilized(cardHandle, timeout);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WaitForStabilized - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WriteMemory(SESSION sid, DWORD cardHandle, DWORD* memorydata, DWORD length) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WriteMemory - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WRITEMEMORY,"uau", cardHandle, length, memorydata)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_WriteMemory != NULL) {
				PI743_WriteMemory(cardHandle, memorydata, length);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WriteMemory - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::Version(SESSION sid, DWORD *Version) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::Version - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_VERSION, NULL)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", Version);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(PI743_revision_query != NULL) {
				CHAR driverRev[20], instrRev[20];
				PI743_revision_query(1, driverRev, instrRev);
				*Version = (DWORD)atof(driverRev)*100;
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::Version - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}


DWORD pi743lx_module_c::initCardForRead(SESSION sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD *session) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::initCardForRead - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_INITCARDFORREAD, "acbb", strlen(resourceDescription) + 1,  resourceDescription, IDQuery, reset)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", session);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOCAL;
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::initCardForRead - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}
/*
DWORD pi743lx_module_c::initCardsForRead(SESSION sid, BOOL IDQuery, BOOL reset,  DWORD *Count) {
	register pu32 result;
	session_data_s sesData = {0};

	// DEBUG RECORD BEGIN 
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::initCardsForRead - begin");
	#endif
	// DEBUG RECORD END 
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_INITCARDSFORREAD, "bb", IDQuery, reset)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", Count);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOCAL;
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	// DEBUG RECORD BEGIN
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::initCardsForRead - end");
	#endif
	// DEBUG RECORD END
	return result;
}
*/

//Low functions
DWORD pi743lx_module_c::ReadAdc(SESSION sid, DWORD cardHandle, DWORD select, DWORD averages, DWORD *data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadAdc - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READADC, "uuu", cardHandle, select, averages)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", data);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_ReadAdc != NULL) {
				pi743_ReadAdc(cardHandle, select, averages, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadAdc - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadCurrentDac(SESSION sid, DWORD cardHandle, DWORD *data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadCurrentDac - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READCURRENTDAC, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", data);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_ReadCurrentDac != NULL) {
				pi743_ReadCurrentDac(cardHandle, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadCurrentDac - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadEeprom(SESSION sid, DWORD cardHandle, DWORD address, DWORD *data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadEeprom - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READEEPROM, "uu", cardHandle, address)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", data);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_ReadEeprom != NULL) {
				pi743_ReadEeprom(cardHandle, address, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadEeprom - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadOutputDac(SESSION sid, DWORD cardHandle, DWORD *data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadOutputDac - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READOUTPUTDAC, "u", cardHandle)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", data);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_ReadOutputDac != NULL) {
				pi743_ReadOutputDac(cardHandle, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadOutputDac - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::ReadRegister(SESSION sid, DWORD cardHandle, DWORD offset, DWORD *data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::ReadRegister - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_READREGISTER, "uu", cardHandle, offset)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, "u", data);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_ReadRegister != NULL) {
				pi743_ReadRegister(cardHandle, offset, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::ReadRegister - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WriteCurrentDac(SESSION sid, DWORD cardHandle, DWORD data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WriteCurrentDac - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WRITECURRENTDAC, "uu", cardHandle, data)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_WriteCurrentDac != NULL) {
				pi743_WriteCurrentDac(cardHandle, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WriteCurrentDac - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WriteEeprom(SESSION sid, DWORD cardHandle, DWORD address, DWORD data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WriteEeprom - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WRITEEEPROM, "uuu", cardHandle, address, data)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_WriteEeprom != NULL) {
				pi743_WriteEeprom(cardHandle, address, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WriteEeprom - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WriteID(SESSION sid, DWORD cardHandle, DWORD id) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WriteID - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WRITEID, "uu", cardHandle, id)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_WriteID != NULL) {
				pi743_WriteID(cardHandle, id);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WriteID - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WriteOutputDac(SESSION sid, DWORD cardHandle, DWORD data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WriteOutputDac - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WRITEOUTPUTDAC, "uu", cardHandle, data)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_WriteOutputDac != NULL) {
				pi743_WriteOutputDac(cardHandle, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WriteOutputDac - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}

DWORD pi743lx_module_c::WriteRegister(SESSION sid, DWORD cardHandle, DWORD offset, DWORD data) {
	register pu32 result;
	session_data_s sesData = {0};

	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(FALSE,"pi743lx_module_c::WriteRegister - begin");
	#endif
	/* DEBUG RECORD END */
	result = PICMLX_BeginTransmition(sid,&sesData);
	if(!result) {
		if(sesData.remote) {
			if(sesData.connected) {
				if(!PICMLX_SendPacket(PI743LX_FID_WRITEREGISTER, "uuu", cardHandle, offset, data)) {
					sesData.lastError = PICMLX_RecvPacket(NULL, NULL);
				}
			} else {
				sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
			}
		} else {
			sesData.lastError = PI743LX_ERROR_FUNC_NOT_LOADED;
			if(pi743_WriteRegister != NULL) {
				pi743_WriteRegister(cardHandle, offset, data);
				sesData.lastError = NO_ERROR;
			}
		}
		result = sesData.lastError;
	}
	PICMLX_EndTransmition(&sesData);
	/* DEBUG RECORD BEGIN */
	#ifdef DEBUG
		__DEBUGMSG(TRUE,"pi743lx_module_c::WriteRegister - end");
	#endif
	/* DEBUG RECORD END */
	return result;
}



/* ##################################################################################################### */
