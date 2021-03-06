/*****************************************************************/
/* XILINX's Receiver test                                        */
/*****************************************************************/

XilinxTest() {
int n; short f1,f2,er,t;

  dpm->Info.TestProg = T_XILINX;

  /* Enable Bus Monitor (BME) & Set BMT=64 Clocks */
  sim->SimProtect = Tg8ENABLE_BUSMONITOR;
  bus_int = 0;

  f1 =  xlx->XWsscRframe1; /* Read Frame 1 */
  if (bus_int) {
    dpm->Info.Xilinx.Err.BusInt = XI_READF1;
    dpm->Info.Xilinx.Err.Code = f1;
faila:
    dpm->Info.FaultType |= T_XILINX;
    dpm->Info.FaultCnt++;
    dpm->Info.N_of_bus += bus_int;
    dpm->Info.Xilinx.Err.N_of_bus = bus_int;
    bus_int = 0;
    return;
  };

  f2 = xlx->XRframe2;  /* Read Frame 2 */
  if (bus_int) {
    dpm->Info.Xilinx.Err.BusInt = XI_READF2;
    dpm->Info.Xilinx.Err.Code = f2;
    goto faila;
  };

  dpm->Info.Xilinx.Rcv = er= xlx->WClrRerr;  /* Read the receiver error */
  if (bus_int) {
    dpm->Info.Xilinx.Err.BusInt = XI_READERR;
    dpm->Info.Xilinx.Err.Code = er;
    goto faila;
  };

  xlx->WClrRerr = 0;  /* Reset an error */
  if (bus_int) {
    dpm->Info.Xilinx.Err.BusInt = XI_RESETERR;
    dpm->Info.Xilinx.Err.Code = dpm->Info.Xilinx.Rcv = xlx->WClrRerr;
    goto faila;
  };
  er = xlx->WClrRerr &0xFF;
  if (er) {
    dpm->Info.Xilinx.Rcv = er;  /* Keep the receiver error */
    if (er != XrMS_MISSING) {
      dpm->Info.FaultType |= T_XILINX;
      dpm->Info.FaultCnt++;
    };
  };

  xlx->XWsscRframe1 = 0x34; /* Set the CPS SSC (Start Super Cycle) event */
  if (bus_int) {
    dpm->Info.Xilinx.Err.BusInt = XI_SSC;
    dpm->Info.Xilinx.Err.Code = xlx->XWsscRframe1;
    goto faila;
  };
  dpm->Info.Xilinx.Ssc = 0x34;
  dpm->Info.TestPassed |= T_XILINX;
}

/* eof */
