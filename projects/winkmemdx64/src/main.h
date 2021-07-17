#pragma once

#include <ntddk.h>

extern "C"
NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
);

extern "C"
void UnloadDriver(
    _In_ PDRIVER_OBJECT DriverObject
);

void ImageLoadCallback(PUNICODE_STRING fullImageName, HANDLE processId, PIMAGE_INFO imageInfo);

