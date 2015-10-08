/* $Id: pickering16to16a.h,v 1.1 2005/06/08 07:44:55 sdeghaye Exp $ */
#ifndef PICKERING16TO16A_H
#define PICKERING16TO16A_H

#include <linux/pci.h>
#include "pickeringmuxdrv.h"

#define PICK_MUX_16_TO_16A_SUBSYSTEM_ID 0XF6

int   pickMux16to16aProbe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc);
void  pickMux16to16aRemove(PickeringMuxDescr *muxDesc);
int   pickMux16to16aRead(struct file *file, char *buffer, size_t count, loff_t* ppos);
int   pickMux16to16aWrite(struct file *file, const char *buffer, size_t count, loff_t* ppos);
int   pickMux16to16aMaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput);
int   pickMux16to16aMaxInput(PickeringMuxDescr *muxDesc, int *maxInput);
int   pickMux16to16aConnect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data);
int   pickMux16to16aDisconnect(PickeringMuxDescr *muxDesc, int output);
int   pickMux16to16aReset(PickeringMuxDescr *muxDesc);
int   pickMux16to16aStatus(PickeringMuxDescr *muxDesc, int *status);

#endif
