/* $Id: pickering4to1.h,v 1.3 2008/12/04 15:52:29 nmn Exp $ */
#ifndef PICKERING4TO1_H
#define PICKERING4TO1_H

#include <linux/pci.h>
#include "pickeringmuxdrv.h"

#define PICK_MUX_4_TO_1_SUBSYSTEM_ID 0XA7

int   pickMux4to1Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc);
void  pickMux4to1Remove(PickeringMuxDescr *muxDesc);
int   pickMux4to1Read(struct file *file, char *buffer, size_t count, loff_t* ppos);
int   pickMux4to1Write(struct file *file, const char *buffer, size_t count, loff_t* ppos);
int   pickMux4to1MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput);
int   pickMux4to1MaxInput(PickeringMuxDescr *muxDesc, int *maxInput);
int   pickMux4to1Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data);
int   pickMux4to1Disconnect(PickeringMuxDescr *muxDesc, int output);
int   pickMux4to1Reset(PickeringMuxDescr *muxDesc);
int   pickMux4to1Status(PickeringMuxDescr *muxDesc, int *status);

#endif
