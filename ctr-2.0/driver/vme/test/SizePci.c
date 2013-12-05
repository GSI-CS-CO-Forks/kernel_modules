#include <stdio.h>
#include <stddef.h>

#define CTR_PCI

#include "ctrdrvr.h"

int main(int argc, char **argv)
{

printf("\n*** SIZES ***\n\n");

printf("sizeof(CtrDrvrConnection)             =%lu\n", sizeof(CtrDrvrConnection));
printf("sizeof(CtrDrvrClientList)             =%lu\n", sizeof(CtrDrvrClientList));
printf("sizeof(CtrDrvrClientConnections)      =%lu\n", sizeof(CtrDrvrClientConnections));
printf("sizeof(CtrDrvrReadBuf)                =%lu\n", sizeof(CtrDrvrReadBuf));
printf("sizeof(CtrDrvrWriteBuf)               =%lu\n", sizeof(CtrDrvrWriteBuf));
printf("sizeof(CtrDrvrAction)                 =%lu\n", sizeof(CtrDrvrAction));
printf("sizeof(CtrDrvrCtimBinding)            =%lu\n", sizeof(CtrDrvrCtimBinding));
printf("sizeof(CtrDrvrCtimObjects)            =%lu\n", sizeof(CtrDrvrCtimObjects));
printf("sizeof(CtrDrvrPtimBinding)            =%lu\n", sizeof(CtrDrvrPtimBinding));
printf("sizeof(CtrDrvrPtimObjects)            =%lu\n", sizeof(CtrDrvrPtimObjects));
printf("sizeof(CtrDrvrModuleAddress)          =%lu\n", sizeof(CtrDrvrModuleAddress));
printf("sizeof(CtrDrvrVersion)                =%lu\n", sizeof(CtrDrvrVersion));
printf("sizeof(CtrDrvrHptdcIoBuf)             =%lu\n", sizeof(CtrDrvrHptdcIoBuf));
printf("sizeof(CtrDrvrCounterConfigurationBuf)=%lu\n", sizeof(CtrDrvrCounterConfigurationBuf));
printf("sizeof(CtrDrvrCounterHistoryBuf)      =%lu\n", sizeof(CtrDrvrCounterHistoryBuf));
printf("sizeof(CtrdrvrRemoteCommandBuf)       =%lu\n", sizeof(CtrdrvrRemoteCommandBuf));
printf("sizeof(CtrDrvrCounterMaskBuf)         =%lu\n", sizeof(CtrDrvrCounterMaskBuf));
printf("sizeof(CtrDrvrTgmBuf)                 =%lu\n", sizeof(CtrDrvrTgmBuf));
printf("sizeof(CtrDrvrRawIoBlock)             =%lu\n", sizeof(CtrDrvrRawIoBlock));
printf("sizeof(CtrDrvrReceptionErrors)        =%lu\n", sizeof(CtrDrvrReceptionErrors));
printf("sizeof(CtrDrvrBoardId)                =%lu\n", sizeof(CtrDrvrBoardId));

printf("\n*** OFFSETS ***\n\n");

printf("CtrDrvrModuleAddress.PciSlot          = %lu\n", (unsigned long) offsetof(CtrDrvrModuleAddress,PciSlot));
printf("CtrDrvrModuleAddress.ModuleNumber     = %lu\n", (unsigned long) offsetof(CtrDrvrModuleAddress,ModuleNumber));
printf("CtrDrvrModuleAddress.DeviceId         = %lu\n", (unsigned long) offsetof(CtrDrvrModuleAddress,DeviceId));
printf("CtrDrvrModuleAddress.VendorId         = %lu\n", (unsigned long) offsetof(CtrDrvrModuleAddress,VendorId));
printf("CtrDrvrModuleAddress.MemoryMap        = %lu\n", (unsigned long) offsetof(CtrDrvrModuleAddress,MemoryMap));
printf("CtrDrvrModuleAddress.LocalMap         = %lu\n", (unsigned long) offsetof(CtrDrvrModuleAddress,LocalMap));

printf("\n*** HARDWARE STRUCT OFFSETS ***\n\n");

printf("CtrDrvrMemoryMap.InterruptSource=%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,InterruptSource));
printf("CtrDrvrMemoryMap.InterruptEnable=%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,InterruptEnable));
printf("CtrDrvrMemoryMap.HptdcJtag      =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,HptdcJtag));
printf("CtrDrvrMemoryMap.InputDelay     =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,InputDelay));
printf("CtrDrvrMemoryMap.CableId        =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,CableId));
printf("CtrDrvrMemoryMap.VhdlVersion    =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,VhdlVersion));
printf("CtrDrvrMemoryMap.OutputByte     =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,OutputByte));
printf("CtrDrvrMemoryMap.Status         =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Status));
printf("CtrDrvrMemoryMap.Command        =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Command));
printf("CtrDrvrMemoryMap.Pll            =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Pll));
printf("CtrDrvrMemoryMap.ReadTime       =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,ReadTime));
printf("CtrDrvrMemoryMap.SetTime        =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,SetTime));
printf("CtrDrvrMemoryMap.Counters       =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Counters));
printf("CtrDrvrMemoryMap.Telegrams      =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Telegrams));
printf("CtrDrvrMemoryMap.Trigs          =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Trigs));
printf("CtrDrvrMemoryMap.Configs        =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Configs));
printf("CtrDrvrMemoryMap.EventHistory   =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,EventHistory));
printf("CtrDrvrMemoryMap.Setup          =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,Setup));
printf("CtrDrvrMemoryMap.LastReset      =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,LastReset));
printf("CtrDrvrMemoryMap.PartityErrs    =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,PartityErrs));
printf("CtrDrvrMemoryMap.SyncErrs       =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,SyncErrs));
printf("CtrDrvrMemoryMap.TotalErrs      =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,TotalErrs));
printf("CtrDrvrMemoryMap.CodeViolErrs   =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,CodeViolErrs));
printf("CtrDrvrMemoryMap.QueueErrs      =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,QueueErrs));
printf("CtrDrvrMemoryMap.IdLSL          =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,IdLSL));
printf("CtrDrvrMemoryMap.IdMSL          =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,IdMSL));
printf("CtrDrvrMemoryMap.ModStats       =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,ModStats));
printf("CtrDrvrMemoryMap.SvnId          =%06X\n", (unsigned int) offsetof(CtrDrvrMemoryMap,SvnId));

printf("\n");

return 0;
}
