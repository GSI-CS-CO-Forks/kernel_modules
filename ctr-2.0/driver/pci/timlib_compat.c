
	if (ionr == CtrDrvrLIST_CTIM_OBJECTS) {
		iosz = sizeof(CtrDrvrCtimObjects);
		iodr = _IOC_READ;
		cmd  = CtrIoctlLIST_CTIM_OBJECTS;
	}

	if (ionr == CtrDrvrLIST_PTIM_OBJECTS) {
		iosz = sizeof(CtrDrvrPtimObjects);
		iodr = _IOC_READ;
		cmd  = CtrIoctlLIST_PTIM_OBJECTS;
	}

	if (ionr ==  CtrDrvrSET_SW_DEBUG) { 
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_SW_DEBUG;
	}

	if (ionr ==  CtrDrvrGET_SW_DEBUG) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_SW_DEBUG;
	}

	if (ionr ==  CtrDrvrGET_VERSION) { 
		iosz = sizeof(CtrDrvrVersion);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_VERSION;
	}

	if (ionr ==  CtrDrvrSET_TIMEOUT) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_TIMEOUT;
	}

	if (ionr ==  CtrDrvrGET_TIMEOUT) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_TIMEOUT;
	}

	if (ionr ==  CtrDrvrSET_QUEUE_FLAG) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_QUEUE_FLAG;
	}

	if (ionr ==  CtrDrvrGET_QUEUE_FLAG) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_QUEUE_FLAG;
	}

	if (ionr ==  CtrDrvrGET_QUEUE_SIZE) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_QUEUE_SIZE;
	}

	if (ionr ==  CtrDrvrGET_QUEUE_OVERFLOW) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_QUEUE_OVERFLOW;
	}

	if (ionr ==  CtrDrvrGET_MODULE_DESCRIPTOR) { 
		iosz = sizeof(CtrDrvrModuleAddress);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_MODULE_DESCRIPTOR;
	}

	if (ionr ==  CtrDrvrSET_MODULE) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_MODULE;
	}

	if (ionr ==  CtrDrvrGET_MODULE) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_MODULE;
	}

	if (ionr ==  CtrDrvrGET_MODULE_COUNT) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_MODULE_COUNT;
	}

	if (ionr ==  CtrDrvrRESET) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlRESET;
	}

	if (ionr ==  CtrDrvrENABLE) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlENABLE;
	}

	if (ionr ==  CtrDrvrGET_STATUS) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_STATUS;
	}

	if (ionr ==  CtrDrvrGET_INPUT_DELAY) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_INPUT_DELAY;
	}

	if (ionr ==  CtrDrvrSET_INPUT_DELAY) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_INPUT_DELAY;
	}

	if (ionr ==  CtrDrvrGET_CLIENT_LIST) {
		iosz = sizeof(CtrDrvrClientList);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_CLIENT_LIST;
	}

	if (ionr ==  CtrDrvrCONNECT) {
		iosz = sizeof(CtrDrvrConnection);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlCONNECT;
	}

	if (ionr ==  CtrDrvrDISCONNECT) {
		iosz = sizeof(CtrDrvrConnection);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlDISCONNECT;
	}

	if (ionr ==  CtrDrvrGET_CLIENT_CONNECTIONS) {
		iosz = sizeof(CtrDrvrClientConnections);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_CLIENT_CONNECTIONS;
	}

	if (ionr ==  CtrDrvrSET_UTC) {
		iosz = sizeof(CtrDrvrCTime);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_UTC;
	}

	if (ionr ==  CtrDrvrGET_UTC) {
		iosz = sizeof(CtrDrvrCTime);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_UTC;
	}

	if (ionr ==  CtrDrvrGET_CABLE_ID) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_CABLE_ID;
	}

	if (ionr ==  CtrDrvrGET_ACTION) {
		iosz = sizeof(CtrDrvrAction);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_ACTION;
	}

	if (ionr ==  CtrDrvrSET_ACTION) {
		iosz = sizeof(CtrDrvrAction);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_ACTION;
	}

	if (ionr ==  CtrDrvrCREATE_CTIM_OBJECT) {
		iosz = sizeof(CtrDrvrCtimBinding);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlCREATE_CTIM_OBJECT;
	}

	if (ionr ==  CtrDrvrDESTROY_CTIM_OBJECT) {
		iosz = sizeof(CtrDrvrCtimBinding);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlDESTROY_CTIM_OBJECT;
	}

	if (ionr ==  CtrDrvrCHANGE_CTIM_FRAME) {
		iosz = sizeof(CtrDrvrCtimBinding);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlCHANGE_CTIM_FRAME;
	}

	if (ionr ==  CtrDrvrCREATE_PTIM_OBJECT) {
		iosz = sizeof(CtrDrvrPtimBinding);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlCREATE_PTIM_OBJECT;
	}

	if (ionr ==  CtrDrvrDESTROY_PTIM_OBJECT) {
		iosz = sizeof(CtrDrvrPtimBinding);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlDESTROY_PTIM_OBJECT;
	}

	if (ionr ==  CtrDrvrGET_PTIM_BINDING) {
		iosz = sizeof(CtrDrvrPtimBinding);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_PTIM_BINDING;
	}

	if (ionr ==  CtrDrvrGET_OUT_MASK) {
		iosz = sizeof(CtrDrvrCounterMaskBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_OUT_MASK;
	}

	if (ionr ==  CtrDrvrSET_OUT_MASK) { 
		iosz = sizeof(CtrDrvrCounterMaskBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_OUT_MASK;
	}

	if (ionr ==  CtrDrvrGET_COUNTER_HISTORY) {
		iosz = sizeof(CtrDrvrCounterHistoryBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_COUNTER_HISTORY;
	}

	if (ionr ==  CtrDrvrGET_REMOTE) { 
		iosz = sizeof(CtrdrvrRemoteCommandBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_REMOTE;
	}

	if (ionr ==  CtrDrvrSET_REMOTE) { 
		iosz = sizeof(CtrdrvrRemoteCommandBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_REMOTE;
	}

	if (ionr ==  CtrDrvrREMOTE) { 
		iosz = sizeof(CtrdrvrRemoteCommandBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlREMOTE;
	}

	if (ionr ==  CtrDrvrGET_CONFIG) {
		iosz = sizeof(CtrDrvrCounterConfigurationBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_CONFIG;
	}

	if (ionr ==  CtrDrvrSET_CONFIG) {
		iosz = sizeof(CtrDrvrCounterConfigurationBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_CONFIG;
	}

	if (ionr ==  CtrDrvrGET_PLL) { 
		iosz = sizeof(CtrDrvrPll);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_PLL;
	}

	if (ionr ==  CtrDrvrSET_PLL) { 
		iosz = sizeof(CtrDrvrPll);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_PLL;
	}

	if (ionr ==  CtrDrvrSET_PLL_ASYNC_PERIOD) {
		iosz = sizeof(CtrDrvrPllAsyncPeriodNs);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_PLL_ASYNC_PERIOD;
	}

	if (ionr ==  CtrDrvrGET_PLL_ASYNC_PERIOD) {
		iosz = sizeof(CtrDrvrPllAsyncPeriodNs);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_PLL_ASYNC_PERIOD;
	}

	if (ionr ==  CtrDrvrREAD_TELEGRAM) {
		iosz = sizeof(CtrDrvrTgmBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlREAD_TELEGRAM;
	}

	if (ionr ==  CtrDrvrREAD_EVENT_HISTORY) { 
		iosz = sizeof(CtrDrvrEventHistoryBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlREAD_EVENT_HISTORY;
	}

	if (ionr ==  CtrDrvrHPTDC_OPEN) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlHPTDC_OPEN;
	}

	if (ionr ==  CtrDrvrHPTDC_IO) {
		iosz = sizeof(CtrDrvrHptdcIoBuf);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlHPTDC_IO;
	}

	if (ionr ==  CtrDrvrHPTDC_CLOSE) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlHPTDC_CLOSE;
	}

	if (ionr ==  CtrDrvrRAW_READ) { 
		iosz = sizeof(CtrDrvrRawIoBlock);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlRAW_READ;
	}

	if (ionr ==  CtrDrvrRAW_WRITE) { 
		iosz = sizeof(CtrDrvrRawIoBlock);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlRAW_WRITE;
	}

	if (ionr ==  CtrDrvrGET_RECEPTION_ERRORS) { 
		iosz = sizeof(CtrDrvrReceptionErrors);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_RECEPTION_ERRORS;
	}

	if (ionr ==  CtrDrvrGET_IO_STATUS) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_IO_STATUS;
	}

	if (ionr ==  CtrDrvrGET_IDENTITY) { 
		iosz = sizeof(CtrDrvrBoardId);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_IDENTITY;
	}

	if (ionr ==  CtrDrvrSET_DEBUG_HISTORY) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_DEBUG_HISTORY;
	}

	if (ionr ==  CtrDrvrSET_BRUTAL_PLL) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_BRUTAL_PLL;
	}

	if (ionr ==  CtrDrvrGET_MODULE_STATS) {
		iosz = sizeof(CtrDrvrModuleStats);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_MODULE_STATS;
	}

	if (ionr ==  CtrDrvrSET_CABLE_ID) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_CABLE_ID;
	}

	if (ionr ==  CtrDrvrLOCK) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlLOCK;
	}

	if (ionr ==  CtrDrvrUNLOCK) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlUNLOCK;
	}

	if (ionr ==  CtrDrvrGET_OUTPUT_BYTE) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_OUTPUT_BYTE;
	}

	if (ionr ==  CtrDrvrSET_OUTPUT_BYTE) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_OUTPUT_BYTE;
	}

	if (ionr ==  CtrDrvrSET_MODULE_BY_SLOT) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlSET_MODULE_BY_SLOT;
	}

	if (ionr ==  CtrDrvrGET_MODULE_SLOT) {
		iosz = sizeof(uint32_t);
		iodr = _IOC_READ | _IOC_WRITE;
		cmd =  CtrIoctlGET_MODULE_SLOT;
	}
