#pragma warning(disable : 4100)

#include "main.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
    UNREFERENCED_PARAMETER(pRegistryPath);
    DbgPrintEx(0, 0, "Driver loaded");
    pDriverObject->DriverUnload = UnloadDriver;
    return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
    UNREFERENCED_PARAMETER(pDriverObject);
    DbgPrintEx(0, 0, "Driver unloaded");
    return STATUS_SUCCESS;
}
