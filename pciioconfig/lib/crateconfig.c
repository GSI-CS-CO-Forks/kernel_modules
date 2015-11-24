#include <stdio.h>
#include "crateconfig.h"

const char * const libpciioconfig_version_s = "libpciioconfig version: " GIT_VERSION;

#define PCI_2_SLOT_FILE "/dsc/local/etc/crateconfig"

typedef struct
{
  int slot;
  int bus;
  int device;
}Slot2PCI;
  
static Slot2PCI crate[CPCI_CRATE_MAX_SLOT];
static int init = 0;

static int checkInit();

/***************************************************************************************************
 *
 */
int getPCIPosition(int slot, int *bus, int *device)
{
  int rc = checkInit();
  int i;
  if(rc == 0)
  {
    for(i = 0; i < CPCI_CRATE_MAX_SLOT; i++)
    {
      if(crate[i].slot == slot)
      {
        *bus = crate[i].bus;
        *device = crate[i].device;
        return 0;
      }      
    }
    /* Never found the right slot */
    return NO_SUCH_SLOT;
  }
  else
  {
    return rc;
  }
}

/***************************************************************************************************
 *
 */
int getSlot(int bus, int device, int *slot)
{
  int rc = checkInit();
  int i;
  if(rc == 0)
  {
    for(i = 0; i < CPCI_CRATE_MAX_SLOT; i++)
    {
      if(crate[i].bus == bus && crate[i].device == device)
      {
        *slot = crate[i].slot;
        return 0;
      }      
    }
    /* Never found the right slot */
    return NO_SUCH_PCI_POSITION;
  }
  else
  {
    return rc;
  }
}

/***************************************************************************************************
 *
 */
static int checkInit()
{
  static char linebuf[256];
  if(init == 0)
  {
    FILE *fp;
    int i = 0;
    
    fp = fopen(PCI_2_SLOT_FILE, "r");
    if(fp == NULL)
    {
      return ERROR_OPENING_FILE;
    }
    
    while (fgets(linebuf, sizeof(linebuf), fp) != NULL) {
      sscanf(linebuf, "%d %d %x", &crate[i].slot, &crate[i].bus, &crate[i].device);
      i++;
      
      if(i >= CPCI_CRATE_MAX_SLOT) {
        printf("The file is too long, breaking...\n");
        break;
      }
    }
    init = 1;
  }
  return 0;
}

