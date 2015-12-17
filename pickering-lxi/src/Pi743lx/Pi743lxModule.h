/*******************************************************************************
 *
 *******************************************************************************/

#ifndef __PI743LXMODULE_H__
#define __PI743LXMODULE_H__

#include <System.h>
#include <Picmlx.h>
#include <BaseModule.h>

/* pi743 library function prototypes */
//find:			{DWORD} _PI743_FUNC {[a-zA-Z743_]*} *\({[a-zA-Z ,\*_\[\]0-9]*}\);
//replace:		typedef \1 (_stdcall *\2_f)(\3);
typedef DWORD (_stdcall *pi743_ClearOverCurrentStatus_f)(DWORD cardHandle);
typedef DWORD (_stdcall *pi743_ClearTriggerStatus_f)(DWORD cardHandle);
typedef DWORD (_stdcall *pi743_close_f)(DWORD cardHandle);
typedef DWORD (_stdcall *pi743_error_message_f)(DWORD cardHandle, DWORD statusCode, CHAR errorMessage[]);
typedef DWORD (_stdcall *pi743_error_query_f)(DWORD cardHandle, DWORD* errorCode, CHAR errorMessage[]);
typedef DWORD (_stdcall *pi743_GetAddressCounter_f)(DWORD cardHandle, DWORD* addresscounter);
typedef DWORD (_stdcall *pi743_GetClockDivider_f)(DWORD cardHandle, DWORD* divider);
typedef DWORD (_stdcall *pi743_GetMeasuredOutputCurrent_f)(DWORD cardHandle, double* current);
typedef DWORD (_stdcall *pi743_GetMeasuredOutputVoltage_f)(DWORD cardHandle, double* voltage);
typedef DWORD (_stdcall *pi743_GetOutputAll_f)(DWORD cardHandle, double *voltage, double *current, double *power, DWORD *connection);
typedef DWORD (_stdcall *pi743_GetOutputConnection_f)(DWORD cardHandle, DWORD* connection);
typedef DWORD (_stdcall *pi743_GetOutputCurrentLimit_f)(DWORD cardHandle, double* current);
typedef DWORD (_stdcall *pi743_GetOutputVoltage_f)(DWORD cardHandle, double* voltage);
typedef DWORD (_stdcall *pi743_GetOverCurrentStatus_f)(DWORD cardHandle, DWORD* overcurrent);
typedef DWORD (_stdcall *pi743_GetPowerSupplyStatus_f)(DWORD cardHandle, DWORD* psstatus);
typedef DWORD (_stdcall *pi743_GetTrigger_f)(DWORD cardHandle, DWORD* source, DWORD* action, DWORD* level, DWORD *mode);
typedef DWORD (_stdcall *pi743_GetTriggerCurrent_f)(DWORD cardHandle, double* current);
typedef DWORD (_stdcall *pi743_GetTriggerStatus_f)(DWORD cardHandle, DWORD* triggerstatus);
typedef DWORD (_stdcall *pi743_GetTriggerVoltage_f)(DWORD cardHandle, double* voltage);
typedef DWORD (_stdcall *pi743_init_f) (CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD* session);
typedef DWORD (_stdcall *pi743_MemoryTest_f)(DWORD cardHandle,
                                   DWORD logSize,
                                   DWORD *erroraddress,
                                   DWORD *errordata,
                                   DWORD *expectdata,
                                   DWORD *foundErrors);
typedef DWORD (_stdcall *pi743_ReadCapturedCurrent_f)(DWORD cardHandle, double* current, DWORD count);
typedef DWORD (_stdcall *pi743_ReadCapturedVoltage_f)(DWORD cardHandle, double* voltages, DWORD count);
typedef DWORD (_stdcall *pi743_ReadCurrentCalValues_f)(DWORD cardHandle, double* offsetcurrentcdac, double* gaincurrentcdac, DWORD* offsetcodecadc, DWORD* gaincodecadc);
typedef DWORD (_stdcall *pi743_ReadID_f)(DWORD cardHandle, DWORD* id);
typedef DWORD (_stdcall *pi743_ReadMaximumPower_f)(DWORD cardHandle, double* power);
typedef DWORD (_stdcall *pi743_ReadVoltageCalValues_f)(DWORD cardHandle, double* offsetvoltagevdac, double* gainvoltagevdac, DWORD* offsetcodevadc,DWORD* gaincodevadc);
typedef DWORD (_stdcall *pi743_reset_f)(DWORD cardHandle);
typedef DWORD (_stdcall *pi743_revision_query_f)(DWORD cardHandle, CHAR driverRev[],CHAR instrumentRev[]);
typedef DWORD (_stdcall *pi743_self_test_f)(DWORD cardHandle, BOOL* testResult, CHAR errorMessage[]);
typedef DWORD (_stdcall *pi743_SetAddressCounter_f)(DWORD cardHandle, DWORD addresscounter);
typedef DWORD (_stdcall *pi743_SetClockDivider_f)(DWORD cardHandle, DWORD divider);
typedef DWORD (_stdcall *pi743_SetOutputAll_f)(DWORD cardHandle, double voltage, double current, double power, DWORD connection);
typedef DWORD (_stdcall *pi743_SetOutputConnection_f)(DWORD cardHandle, DWORD connection);
typedef DWORD (_stdcall *pi743_SetOutputCurrentLimit_f)(DWORD cardHandle, double current);
typedef DWORD (_stdcall *pi743_SetOutputVoltage_f)(DWORD cardHandle, double voltage);
typedef DWORD (_stdcall *pi743_SetPowerSupplyStatus_f)(DWORD cardHandle, DWORD psstatus);
typedef DWORD (_stdcall *pi743_SetSoftwareTrigger_f)(DWORD cardHandle);
typedef DWORD (_stdcall *pi743_SetTrigger_f)(DWORD cardHandle, DWORD source, DWORD action, DWORD level, DWORD mode);
typedef DWORD (_stdcall *pi743_SetTriggerCurrent_f)(DWORD cardHandle, double current);
typedef DWORD (_stdcall *pi743_SetTriggerVoltage_f)(DWORD cardHandle, double voltage);
typedef DWORD (_stdcall *pi743_StoreCurrentCalValues_f)(DWORD cardHandle, double offsetcurrent, double gaincurrentunsigned, DWORD offsetcode, DWORD gaincode);
typedef DWORD (_stdcall *pi743_StoreVoltageCalValues_f)(DWORD cardHandle, double offsetvoltage, double gainvoltage, DWORD offsetcode, DWORD gaincode);
typedef DWORD (_stdcall *pi743_WriteMaximumPower_f)(DWORD cardHandle, double power);
typedef DWORD (_stdcall *pi743_FindInstruments_f)(DWORD buslist[],
                                        DWORD devicelist[],
                                        DWORD *count);
typedef DWORD (_stdcall *pi743_FindInstrumentsRsrc_f)(CHAR rsrclist[][20],
											DWORD *count);
typedef DWORD (_stdcall *pi743_IsStabilized_f)(DWORD cardHandle, BOOL *stabilized);
typedef DWORD (_stdcall *pi743_WaitForStabilized_f)(DWORD cardHandle, DWORD timeout);
typedef DWORD (_stdcall *pi743_WriteMemory_f)(DWORD cardHandle, DWORD* memorydata, DWORD length);
typedef DWORD (_stdcall *pi743_revision_query_f)(DWORD cardHandle, CHAR driverRev[],CHAR instrumentRev[]);

//low level functions
typedef DWORD (_stdcall *pi743_ReadAdc_f)(DWORD cardHandle, DWORD select, DWORD averages, DWORD *data);
typedef DWORD (_stdcall *pi743_ReadCurrentDac_f)(DWORD cardHandle, DWORD *data);
typedef DWORD (_stdcall *pi743_ReadEeprom_f)(DWORD cardHandle, DWORD address, DWORD *data);
typedef DWORD (_stdcall *pi743_ReadOutputDac_f)(DWORD cardHandle, DWORD *data);
typedef DWORD (_stdcall *pi743_ReadRegister_f)(DWORD cardHandle, DWORD offset, DWORD *data);
typedef DWORD (_stdcall *pi743_WriteCurrentDac_f)(DWORD cardHandle, DWORD data);
typedef DWORD (_stdcall *pi743_WriteEeprom_f)(DWORD cardHandle, DWORD address, DWORD data);
typedef DWORD (_stdcall *pi743_WriteID_f)(DWORD cardHandle, DWORD id);
typedef DWORD (_stdcall *pi743_WriteOutputDac_f)(DWORD cardHandle, DWORD data);
typedef DWORD (_stdcall *pi743_WriteRegister_f)(DWORD cardHandle, DWORD offset, DWORD data);


/* Pickering pi743 module class */
class pi743lx_module_c : public base_module_c {
private:
//find:			{DWORD} _PI743_FUNC {[a-zA-Z743_]*} *\({[a-zA-Z ,\*_\[\]0-9]*}\);
//replace:		\t\2_f \2;
	pi743_ClearOverCurrentStatus_f PI743_ClearOverCurrentStatus;
	pi743_ClearTriggerStatus_f PI743_ClearTriggerStatus;
	pi743_close_f PI743_close;
	pi743_error_message_f PI743_error_message;
	pi743_error_query_f PI743_error_query;
	pi743_GetAddressCounter_f PI743_GetAddressCounter;
	pi743_GetClockDivider_f PI743_GetClockDivider;
	pi743_GetMeasuredOutputCurrent_f PI743_GetMeasuredOutputCurrent;
	pi743_GetMeasuredOutputVoltage_f PI743_GetMeasuredOutputVoltage;
	pi743_GetOutputAll_f PI743_GetOutputAll;
	pi743_GetOutputConnection_f PI743_GetOutputConnection;
	pi743_GetOutputCurrentLimit_f PI743_GetOutputCurrentLimit;
	pi743_GetOutputVoltage_f PI743_GetOutputVoltage;
	pi743_GetOverCurrentStatus_f PI743_GetOverCurrentStatus;
	pi743_GetPowerSupplyStatus_f PI743_GetPowerSupplyStatus;
	pi743_GetTrigger_f PI743_GetTrigger;
	pi743_GetTriggerCurrent_f PI743_GetTriggerCurrent;
	pi743_GetTriggerStatus_f PI743_GetTriggerStatus;
	pi743_GetTriggerVoltage_f PI743_GetTriggerVoltage;
	pi743_init_f PI743_init;
	pi743_MemoryTest_f PI743_MemoryTest;
	pi743_ReadCapturedCurrent_f PI743_ReadCapturedCurrent;
	pi743_ReadCapturedVoltage_f PI743_ReadCapturedVoltage;
	pi743_ReadCurrentCalValues_f PI743_ReadCurrentCalValues;
	pi743_ReadID_f PI743_ReadID;
	pi743_ReadMaximumPower_f PI743_ReadMaximumPower;
	pi743_ReadVoltageCalValues_f PI743_ReadVoltageCalValues;
	pi743_reset_f PI743_reset;
	pi743_revision_query_f PI743_revision_query;
	pi743_self_test_f PI743_self_test;
	pi743_SetAddressCounter_f PI743_SetAddressCounter;
	pi743_SetClockDivider_f PI743_SetClockDivider;
	pi743_SetOutputAll_f PI743_SetOutputAll;
	pi743_SetOutputConnection_f PI743_SetOutputConnection;
	pi743_SetOutputCurrentLimit_f PI743_SetOutputCurrentLimit;
	pi743_SetOutputVoltage_f PI743_SetOutputVoltage;
	pi743_SetPowerSupplyStatus_f PI743_SetPowerSupplyStatus;
	pi743_SetSoftwareTrigger_f PI743_SetSoftwareTrigger;
	pi743_SetTrigger_f PI743_SetTrigger;
	pi743_SetTriggerCurrent_f PI743_SetTriggerCurrent;
	pi743_SetTriggerVoltage_f PI743_SetTriggerVoltage;
	pi743_StoreCurrentCalValues_f PI743_StoreCurrentCalValues;
	pi743_StoreVoltageCalValues_f PI743_StoreVoltageCalValues;
	pi743_WriteMaximumPower_f PI743_WriteMaximumPower;
	pi743_FindInstruments_f PI743_FindInstruments;
	pi743_FindInstrumentsRsrc_f PI743_FindInstrumentsRsrc;
	pi743_IsStabilized_f PI743_IsStabilized;
	pi743_WaitForStabilized_f PI743_WaitForStabilized;
	pi743_WriteMemory_f PI743_WriteMemory;
//low functions
	pi743_ReadAdc_f pi743_ReadAdc;
	pi743_ReadCurrentDac_f pi743_ReadCurrentDac;
	pi743_ReadEeprom_f pi743_ReadEeprom;
	pi743_ReadOutputDac_f pi743_ReadOutputDac;
	pi743_ReadRegister_f pi743_ReadRegister;
	pi743_WriteCurrentDac_f pi743_WriteCurrentDac;
	pi743_WriteEeprom_f pi743_WriteEeprom;
	pi743_WriteID_f pi743_WriteID;
	pi743_WriteOutputDac_f pi743_WriteOutputDac;
	pi743_WriteRegister_f pi743_WriteRegister;
private:
	void loadFunctions(module_t libraryHandle);
	bool isBufferBigEnougth(int requestedSize);
	DWORD error_message(SESSION sid, DWORD cardHandle, DWORD statusCode, CHAR errorMessage[], DWORD length);
public:
	pi743lx_module_c(const pchar *libName);
	~pi743lx_module_c();
public:
	// Error handling functions
	pbool isError(SESSION sid);
	pu32 getLastError(SESSION sid,pu32 *code);
	pu32 getLastErrorMessage(SESSION sid,pchar *buffer,pu32 size);
	pu32 errorCode2Message(SESSION sid, pu32 code,pchar *buffer,pu32 size);
	DWORD ClearOverCurrentStatus(SESSION sid, DWORD cardHandle);
	DWORD ClearTriggerStatus(SESSION sid, DWORD cardHandle);
	DWORD close(SESSION sid, DWORD cardHandle);
	DWORD GetAddressCounter(SESSION sid, DWORD cardHandle, DWORD* addresscounter);
	DWORD GetClockDivider(SESSION sid, DWORD cardHandle, DWORD* divider);
	DWORD GetMeasuredOutputCurrent(SESSION sid, DWORD cardHandle, double* current);
	DWORD GetMeasuredOutputVoltage(SESSION sid, DWORD cardHandle, double* voltage);
	DWORD GetOutputAll(SESSION sid, DWORD cardHandle, double *voltage, double *current, double *power, DWORD *connection);
	DWORD GetOutputConnection(SESSION sid, DWORD cardHandle, DWORD* connection);
	DWORD GetOutputCurrentLimit(SESSION sid, DWORD cardHandle, double* current);
	DWORD GetOutputVoltage(SESSION sid, DWORD cardHandle, double* voltage);
	DWORD GetOverCurrentStatus(SESSION sid, DWORD cardHandle, DWORD* overcurrent);
	DWORD GetPowerSupplyStatus(SESSION sid, DWORD cardHandle, DWORD* psstatus);
	DWORD GetTrigger(SESSION sid, DWORD cardHandle, DWORD* source, DWORD* action, DWORD* level, DWORD *mode);
	DWORD GetTriggerCurrent(SESSION sid, DWORD cardHandle, double* current);
	DWORD GetTriggerStatus(SESSION sid, DWORD cardHandle, DWORD* triggerstatus);
	DWORD GetTriggerVoltage(SESSION sid, DWORD cardHandle, double* voltage);
	DWORD initCard (SESSION sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD* session);
	DWORD MemoryTest(SESSION sid, DWORD cardHandle,
                                   DWORD logSize,
                                   DWORD *erroraddress,
                                   DWORD *errordata,
                                   DWORD *expectdata,
                                   DWORD *foundErrors);
	DWORD ReadCapturedCurrent(SESSION sid, DWORD cardHandle, double* current, DWORD count);
	DWORD ReadCapturedVoltage(SESSION sid, DWORD cardHandle, double* voltages, DWORD count);
	DWORD ReadCurrentCalValues(SESSION sid, DWORD cardHandle, double* offsetcurrentcdac, double* gaincurrentcdac, DWORD* offsetcodecadc, DWORD* gaincodecadc);
	DWORD ReadID(SESSION sid, DWORD cardHandle, DWORD* id);
	DWORD ReadMaximumPower(SESSION sid, DWORD cardHandle, double* power);
	DWORD ReadVoltageCalValues(SESSION sid, DWORD cardHandle, double* offsetvoltagevdac, double* gainvoltagevdac, DWORD* offsetcodevadc,DWORD* gaincodevadc);
	DWORD reset(SESSION sid, DWORD cardHandle);
	DWORD self_test(SESSION sid, DWORD cardHandle, BOOL* testResult, CHAR errorMessage[], DWORD errorMessageSize);
	DWORD SetAddressCounter(SESSION sid, DWORD cardHandle, DWORD addresscounter);
	DWORD SetClockDivider(SESSION sid, DWORD cardHandle, DWORD divider);
	DWORD SetOutputAll(SESSION sid, DWORD cardHandle, double voltage, double current, double power, DWORD connection);
	DWORD SetOutputConnection(SESSION sid, DWORD cardHandle, DWORD connection);
	DWORD SetOutputCurrentLimit(SESSION sid, DWORD cardHandle, double current);
	DWORD SetOutputVoltage(SESSION sid, DWORD cardHandle, double voltage);
	DWORD SetPowerSupplyStatus(SESSION sid, DWORD cardHandle, DWORD psstatus);
	DWORD SetSoftwareTrigger(SESSION sid, DWORD cardHandle);
	DWORD SetTrigger(SESSION sid, DWORD cardHandle, DWORD source, DWORD action, DWORD level, DWORD mode);
	DWORD SetTriggerCurrent(SESSION sid, DWORD cardHandle, double current);
	DWORD SetTriggerVoltage(SESSION sid, DWORD cardHandle, double voltage);
	DWORD StoreCurrentCalValues(SESSION sid, DWORD cardHandle, double offsetcurrent, double gaincurrentunsigned, DWORD offsetcode, DWORD gaincode);
	DWORD StoreVoltageCalValues(SESSION sid, DWORD cardHandle, double offsetvoltage, double gainvoltage, DWORD offsetcode, DWORD gaincode);
	DWORD WriteMaximumPower(SESSION sid, DWORD cardHandle, double power);
	DWORD FindInstruments(SESSION sid, DWORD buslist[],
                                        DWORD devicelist[],
                                        DWORD *count);
	DWORD FindInstrumentsRsrc(SESSION sid, CHAR rsrclist[][20],
										DWORD *count);
	DWORD IsStabilized(SESSION sid, DWORD cardHandle, BOOL *stabilized);
	DWORD WaitForStabilized(SESSION sid, DWORD cardHandle, DWORD timeout);
	DWORD WriteMemory(SESSION sid, DWORD cardHandle, DWORD* memorydata, DWORD length);
	DWORD Version(SESSION Sid, DWORD *Version);
	DWORD initCardForRead(SESSION Sid, CHAR* resourceDescription, BOOL IDQuery, BOOL reset,  DWORD *CardHandle);
	DWORD initCardsForRead(SESSION Sid, BOOL IDQuery, BOOL reset,  DWORD *Count);
//low functions
	DWORD ReadAdc(SESSION sid, DWORD cardHandle, DWORD select, DWORD averages, DWORD *data);
	DWORD ReadCurrentDac(SESSION sid, DWORD cardHandle, DWORD *data);
	DWORD ReadEeprom(SESSION sid, DWORD cardHandle, DWORD address, DWORD *data);
	DWORD ReadOutputDac(SESSION sid, DWORD cardHandle, DWORD *data);
	DWORD ReadRegister(SESSION sid, DWORD cardHandle, DWORD offset, DWORD *data);
	DWORD WriteCurrentDac(SESSION sid, DWORD cardHandle, DWORD data);
	DWORD WriteEeprom(SESSION sid, DWORD cardHandle, DWORD address, DWORD data);
	DWORD WriteID(SESSION sid, DWORD cardHandle, DWORD id);
	DWORD WriteOutputDac(SESSION sid, DWORD cardHandle, DWORD data);
	DWORD WriteRegister(SESSION sid, DWORD cardHandle, DWORD offset, DWORD data);
};

#endif // __PI743LXMODULE_H__
