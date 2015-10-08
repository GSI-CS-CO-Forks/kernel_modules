/* $Id: pickering9to8.h,v 1.3 2008/12/04 15:52:29 nmn Exp $ */
#ifndef PICKERING9TO8_H
#define PICKERING9TO8_H

#include <linux/pci.h>
#include "pickeringmuxdrv.h"

#define PICK_MUX_9_TO_8_SUBSYSTEM_ID 0X46

int   pickMux9to8Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc);
void  pickMux9to8Remove(PickeringMuxDescr *muxDesc);
int   pickMux9to8Read(struct file *file, char *buffer, size_t count, loff_t* ppos);
int   pickMux9to8Write(struct file *file, const char *buffer, size_t count, loff_t* ppos);
int   pickMux9to8MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput);
int   pickMux9to8MaxInput(PickeringMuxDescr *muxDesc, int *maxInput);
int   pickMux9to8Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data);
int   pickMux9to8Disconnect(PickeringMuxDescr *muxDesc, int output);
int   pickMux9to8Reset(PickeringMuxDescr *muxDesc);
int   pickMux9to8Status(PickeringMuxDescr *muxDesc, int *status);

#endif
