/* $Id: pickering22to8.h,v 1.2 2012/04/18 13:14:00 sdeghaye Exp $ */
#ifndef PICKERING22TO8_H
#define PICKERING22TO8_H

#include <linux/pci.h>
#include "pickeringmuxdrv.h"

#define PICK_MUX_22_TO_8_1_SUBSYSTEM_ID 0X1011
#define PICK_MUX_22_TO_8_2_SUBSYSTEM_ID 0X1010
#define PICK_MUX_22_TO_8_3_SUBSYSTEM_ID 0X1013


int   pickMux22to8Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc);
void  pickMux22to8Remove(PickeringMuxDescr *muxDesc);
int   pickMux22to8MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput);
int   pickMux22to8MaxInput(PickeringMuxDescr *muxDesc, int *maxInput);
int   pickMux22to8Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data);
int   pickMux22to8Disconnect(PickeringMuxDescr *muxDesc, int output);
int   pickMux22to8Reset(PickeringMuxDescr *muxDesc);
int   pickMux22to8Status(PickeringMuxDescr *muxDesc, int *status);

#endif
