#include <stdio.h>
#include "crateconfig.h"

static char git_version[] = "git_version: " GIT_VERSION;

int main(int argc, char *argv[])
{
  int slot;
  int bus;
  int device;
  int rc;

  printf("%s: %s\n", argv[0], git_version);
  printf("%s\n", libpciioconfig_version_s);
  printf("\n");
  printf("Testing slot -> bus-device\n");
  for(slot = 1; slot <= CPCI_CRATE_MAX_SLOT; slot++)
  {
    rc = getPCIPosition(slot, &bus, &device);
    if(rc == 0)
    {
      printf("Slot %d -> bus %d, device %x\n", slot, bus, device);
    }
    else
    {
      printf("Slot %d -> %d\n", slot, rc);      
    }
  }
  
  printf("Testing bus-device -> slot\n");
  for(bus = 2; bus < 5; bus++)
  {
    for(device = 8; device <= 16; device++)
    {
      rc = getSlot(bus, device, &slot);
      if(rc == 0)
      {
        printf("Bus %d, device %x -> slot %d\n", bus, device, slot);
      }
      else
      {
        printf("Bus %d, device %x -> %d\n", bus, device, rc);      
      }
    }
  }
  return 0; 
}
