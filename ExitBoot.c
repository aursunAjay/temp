#include <Uefi.h>
#include <Protocol/DriverBinding.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>

#define ABC_VERSION 0x10

EFI_EVENT EfiExitBootServicesEvent = NULL;
EFI_STATUS ExitBootServicesStatus = EFI_UNSUPPORTED;
UINTN                  MemoryMapSize = 0;
EFI_MEMORY_DESCRIPTOR* MemoryMap;
UINTN                  MapKey;
UINTN                  DescriptorSize;
UINT32                 DescriptorVersion;
EFI_STATUS Status;
EFI_DRIVER_BINDING_PROTOCOL PROTOCOL;
VOID EFIAPI ExitBootServicesEvent(IN EFI_EVENT Event, IN VOID* Context)
{
  Print(L"ExitBootServicesEvent is called!\n");
}

EFI_STATUS EFIAPI info(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{

  Status = gBS->CreateEvent(EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_NOTIFY, ExitBootServicesEvent, NULL, &EfiExitBootServicesEvent);

  if (EFI_ERROR(Status))
  {
    Print(L"CreateEvent failed: %r\n", Status);
    return Status;
  }

  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  MemoryMapSize = ( 2 * DescriptorSize ) + MemoryMapSize;
  SystemTable->BootServices->AllocatePool(2, MemoryMapSize, (void**)&MemoryMap);
  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

  return Status;
}
