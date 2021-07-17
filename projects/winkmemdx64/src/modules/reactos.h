#pragma once
#include <ntddk.h>

extern "C"
NTSTATUS
NTAPI
MmCopyVirtualMemory(
    IN PEPROCESS SourceProcess,
    IN PVOID SourceAddress,
    IN PEPROCESS TargetProcess,
    OUT PVOID TargetAddress,
    IN SIZE_T BufferSize,
    IN KPROCESSOR_MODE PreviousMode,
    OUT PSIZE_T ReturnSize
);
