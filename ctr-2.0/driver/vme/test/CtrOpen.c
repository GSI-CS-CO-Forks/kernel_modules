/**************************************************************************/
/* Open the Ctr driver                                                    */
/**************************************************************************/

#ifdef CTR_PCI
#include <drm.h>
#endif

#include <ctrdrvr.h>

static CtrDrvrDevice ctr_device = CtrDrvrDeviceANY;
static char *devnames[CtrDrvrDEVICES] = {"ctr", "ctri", "ctrp", "ctrv", "ctre" };

/* ======================== */

int CtrOpen() {

char fnm[32];
int fn;

   sprintf(fnm,"/dev/%s.1",devnames[ctr_device]);
   if ((fn = open(fnm,O_RDWR,0)) > 0) return(fn);
   return(0);
}
