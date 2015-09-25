/* $Id: pickering16to16.h,v 1.4 2006/03/21 15:01:52 sdeghaye Exp $ */
#ifndef PICKERING16TO16_H
#define PICKERING16TO16_H

#include <linux/pci.h>
#include "pickeringmuxdrv.h"

#define PICK_MUX_16_TO_16_SUBSYSTEM_ID 0XD9

int   pickMux16to16Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc);
void  pickMux16to16Remove(PickeringMuxDescr *muxDesc);
int   pickMux16to16MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput);
int   pickMux16to16MaxInput(PickeringMuxDescr *muxDesc, int *maxInput);
int   pickMux16to16Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data);
int   pickMux16to16Disconnect(PickeringMuxDescr *muxDesc, int output);
int   pickMux16to16Reset(PickeringMuxDescr *muxDesc);
int   pickMux16to16Status(PickeringMuxDescr *muxDesc, int *status);

#endif
