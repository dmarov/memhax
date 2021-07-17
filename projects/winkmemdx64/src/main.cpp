#include <ntddk.h>
#include <wdf.h>
#include "main.h"
#include "modules/debug.h"

extern "C"
NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT pDriverObject,
    _In_ PUNICODE_STRING pRegistryPath
)
{
    UNREFERENCED_PARAMETER(pRegistryPath);
    pDriverObject->DriverUnload = UnloadDriver;

    Debug::info("driver loaded");

    PsSetLoadImageNotifyRoutine(ImageLoadCallback);

    return STATUS_SUCCESS;
}

extern "C"
void UnloadDriver(
    _In_ PDRIVER_OBJECT pDriverObject
)
{
    UNREFERENCED_PARAMETER(pDriverObject);

    Debug::info("driver unloaded");

    PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
}

void ImageLoadCallback(PUNICODE_STRING fullImageName, HANDLE processId, PIMAGE_INFO imageInfo)
{
    Debug::info("processId=%s", processId);

    UNREFERENCED_PARAMETER(fullImageName);
    UNREFERENCED_PARAMETER(imageInfo);


}
