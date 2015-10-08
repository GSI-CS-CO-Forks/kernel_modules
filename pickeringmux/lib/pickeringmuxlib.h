/* $Id: pickeringmuxlib.h,v 1.6 2006/03/21 15:06:57 sdeghaye Exp $ */
#ifndef PICKERINGMUXLIB_H
#define PICKERINGMUXLIB_H

/* Error codes returned by lib functions */
#define PICKERINGMUX_BAD_PARAMETER -3
#define PICKERINGMUX_NO_SUCH_MUX   -2
#define PICKERINGMUX_SYSTEM_ERROR  -1 /* More info in errno */

/* Text messages for the corresponding error codes */
#define PICKERINGMUX_BAD_PARAMETER_STRING "One parameter is out of range"
#define PICKERINGMUX_NO_SUCH_MUX_STRING "There is no Pickering RF mux in the given slot"

/* Lib functions */
#ifdef __cplusplus
extern "C" {
#endif
  int  pickeringMuxConnect(int aSlot, int anInput, int anOutput);
  int  pickeringMuxDisconnect(int aSlot, int anOutput);
  int  pickeringMuxMaxInput(int aSlot);
  int  pickeringMuxMaxOutput(int aSlot);
  int  pickeringMuxOuputStatus(int aSlot, const int *outputs);
  void pickeringMuxPrintError(char *aMessage, int anError);
  int  pickeringMuxReset(int aSlot);
  int  pickeringMuxWidth(int aSlot);
  int  pickeringAttn(int aSlot, int aChannel, int anAttn);
#ifdef __cplusplus
}
#endif

#endif
