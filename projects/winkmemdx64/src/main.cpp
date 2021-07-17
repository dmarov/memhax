#include <ntddk.h>
#include <wdf.h>
#include "main.h"

extern "C"
NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
{
    KdPrintEx((
        DPFLTR_IHVDRIVER_ID,
        DPFLTR_INFO_LEVEL,
        "winkmemdx64: loaded\n"
    ));

    return STATUS_SUCCESS;
}

extern "C"
NTSTATUS UnloadDriver(
    _In_ PDRIVER_OBJECT DriverObject
)
{
    KdPrintEx((
        DPFLTR_IHVDRIVER_ID,
        DPFLTR_INFO_LEVEL,
        "winkmemdx64: unloaded\n"
    ));

    return STATUS_SUCCESS;
}
