/**************************************************************************/
/* Open the Ctr driver                                                    */
/**************************************************************************/

#include <ctrdrvr.h>
#include <errno.h>

static CtrDrvrDevice ctr_device = CtrDrvrDeviceANY;
static char *devnames[CtrDrvrDEVICES] = {"ctr", "ctri", "ctrp", "ctrv", "ctre" };
/* ======================== */

int CtrOpen() {

	char fnm[32];
	int fn, i;

	/* 
	 * this loop ensures backward compatibility with the old driver
	 * which assign a single client per ctr.i device
	 * if the old driver is installed, this loop tries to find
	 * the first ctr.i device which is not BUSY
	 * if the new driver is installed the open succeed always with
	 * ctr.1 device.
	 */
	for(i=1; i<=CtrDrvrCLIENT_CONTEXTS; ++i) {
		sprintf(fnm,"/dev/%s.%1d",devnames[ctr_device], i);
   		if ((fn = open(fnm,O_RDWR,0)) > 0)
			break;
	}
	return(fn);
}
