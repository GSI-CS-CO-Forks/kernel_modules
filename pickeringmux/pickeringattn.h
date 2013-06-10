/* $Id: pickeringattn.h,v 1.1 2006/03/21 15:01:07 sdeghaye Exp $ */
#ifndef PICKERINGATTN_H
#define PICKERINGATTN_H

#include <linux/pci.h>
#include "pickeringmuxdrv.h"

#define PICK_ATTN_SUBSYSTEM_ID 0X4000

int   pickAttnProbe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc);
void  pickAttnRemove(PickeringMuxDescr *muxDesc);
int   pickAttnSetAttn(PickeringMuxDescr *muxDesc, PickeringAttnData data);
int   pickAttnReset(PickeringMuxDescr *muxDesc);
int   pickAttnStatus(PickeringMuxDescr *muxDesc, int *status);
int   pickAttnMaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput);
int   pickAttnMaxInput(PickeringMuxDescr *muxDesc, int *maxInput);

#endif
