/* $Id: prfmuxlib.h,v 1.2 2004/03/08 10:43:37 sdeghaye Exp $ */
#ifndef PRFMUXLIB_H
#define PRFMUXLIB_H

/* Error codes returned by lib functions */
#define PRFMUXLIB_BAD_PARAMETER -3
#define PRFMUXLIB_NO_SUCH_MUX   -2
#define PRFMUXLIB_SYSTEM_ERROR  -1 /* More info in errno */

/* Text messages for the corresponding error codes */
#define PRFMUXLIB_BAD_PARAMETER_STRING "One parameter is out of range"
#define PRFMUXLIB_NO_SUCH_MUX_STRING "There is no Pickering RF mux in the given slot"

/* Lib functions */
int prfmuxOuputStatus(int aSlot, int anOutput);
int prfmuxConnect(int aSlot, int anInput, int anOutput);
void prfmuxPrintError(char *aMessage, int anError);

#endif
