#pragma once
#include <ntddk.h>

#define IO_READ_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x100, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x200, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

NTSTATUS IoControl(PDRIVER_OBJECT deviceObject, PIRP irp);

NTSTATUS CloseCall(PDRIVER_OBJECT deviceObject, PIRP irp);

NTSTATUS CreateCall(PDRIVER_OBJECT deviceObject, PIRP irp);

typedef struct _KERNEL_READ_REQUEST {
    ULONG processId;
    ULONG address;
    PVOID pBuf;
    ULONG size;
} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST {
    ULONG processId;
    ULONG address;
    PVOID pBuf;
    ULONG size;
} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;

NTSTATUS KernelReadVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size);
NTSTATUS KernelWriteVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size);
