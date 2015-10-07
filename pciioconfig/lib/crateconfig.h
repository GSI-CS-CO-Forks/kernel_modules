#ifndef CRATECONFIG_H
#define CRATECONFIG_H

#define ERROR_OPENING_FILE    -1
#define NO_SUCH_SLOT          -2
#define NO_SUCH_PCI_POSITION  -3

#define CPCI_CRATE_MAX_SLOT   21

#ifdef __cplusplus
extern "C" {
#endif

int getPCIPosition(int slot, int *bus, int *device);
int getSlot(int bus, int device, int *slot);

#ifdef __cplusplus
}
#endif
  
#endif
