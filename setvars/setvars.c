#include <efi.h>
#include <efilib.h>

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    /* Making bootArgs[] static keeps it contiguous within the compiled binary.
     * This might be good in case there's a need later to do binary patching
     * of some kind later. It certainly does no harm that I can see.
     */
    static const char bootArgs[] = "-no_compat_check";
    const char csrVal[4] = {'w', 0x08, 0x00, 0x00};

    const EFI_GUID appleGUID = {0x7c436110, 0xab2a, 0x4bbb, {0xa8, 0x80, 0xfe, 0x41, 0x99, 0x5c, 0x9f, 0x82}};
    const UINT32 flags = EFI_VARIABLE_BOOTSERVICE_ACCESS|EFI_VARIABLE_RUNTIME_ACCESS|EFI_VARIABLE_NON_VOLATILE;
   
    EFI_RUNTIME_SERVICES* rt; 

    InitializeLib(ImageHandle, SystemTable);
    rt = SystemTable->RuntimeServices;

    uefi_call_wrapper(rt->SetVariable, 5, L"csr-active-config", &appleGUID, flags, 4, csrVal);
    uefi_call_wrapper(rt->SetVariable, 5, L"boot-args", &appleGUID, flags, sizeof(bootArgs), bootArgs);
    uefi_call_wrapper(rt->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);

    return EFI_SUCCESS;
}
