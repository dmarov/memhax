#include <ntddk.h>
#include <wdf.h>
#include "main.h"
#include "modules/debug.h"
#include "modules/ctl.h"

PDEVICE_OBJECT pDeviceObject;
UNICODE_STRING dev, dos;

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

    RtlInitUnicodeString(&dev, L"\\Device\\winmemkdx64");
    RtlInitUnicodeString(&dos, L"\\DosDevices\\winmemkdx64");

    // IOCTL device
    IoCreateDevice(pDriverObject, 0 , &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
    IoCreateSymbolicLink(&dos, &dev);

    pDriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)CreateCall;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)CloseCall;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)IoControl;

    pDeviceObject->Flags |= DO_DIRECT_IO;
    pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    return STATUS_SUCCESS;
}

extern "C"
void UnloadDriver(
    _In_ PDRIVER_OBJECT pDriverObject
)
{
    PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
    IoDeleteSymbolicLink(&dos);
    IoDeleteDevice(pDriverObject->DeviceObject);

    Debug::info("driver unloaded");
}

void ImageLoadCallback(PUNICODE_STRING fullImageName, HANDLE processId, PIMAGE_INFO imageInfo)
{
    UNREFERENCED_PARAMETER(fullImageName);
    UNREFERENCED_PARAMETER(processId);
    UNREFERENCED_PARAMETER(imageInfo);
}
