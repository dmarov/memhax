#pragma once

#include <ntddk.h>

extern "C"
NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
);

extern "C"
NTSTATUS UnloadDriver(
    _In_ PDRIVER_OBJECT DriverObject
);
