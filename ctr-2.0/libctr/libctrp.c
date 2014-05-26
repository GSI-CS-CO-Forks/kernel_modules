/**
 * Julian Lewis Fri-30th March 2012 BE/CO/HT
 * julian.lewis@cern.ch
 *
 * CTR timing library stub.
 * This library is to be used exclusivlely by the new open CBCM timing library.
 * It is up to the implementers of the open CBCM timing library to provide any
 * abstraction layers and to hide this completely from user code.
 */

#define CTR_PCI

#include <libctr.h>

#include <libctr_common.c>

/**
 * @brief Get the addresses of a module
 * @param A handle that was allocated in open
 * @param Pointer to where the module address will be stored
 * @return Zero means success else -1 is returned on error, see errno
 */

int ctr_get_module_address(void *handle, struct ctr_module_address_s *module_address)
{
	struct ctr_handle_s *h = handle;
	CtrDrvrModuleAddress moad;

	if (ioctl(h->fd,CtrIoctlGET_MODULE_DESCRIPTOR,&moad) < 0)
		return -1;
	if (moad.BusType == 1)
		module_address->device_type  = CtrDrvrDeviceCTRP;
	else
		module_address->device_type  = CtrDrvrDeviceCTRV;

	module_address->HardwareAddress = moad.HardwareAddress;
	module_address->InterruptVector = moad.InterruptVector;

	return 0;
}
