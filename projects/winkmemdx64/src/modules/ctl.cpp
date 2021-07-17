#include <ntddk.h>
#include "ctl.h"
#include "debug.h"
#include "reactos.h"

extern "C"
NTSTATUS PsLookupProcessByProcessId(
    HANDLE ProcessId,
    PEPROCESS *Process
);

NTSTATUS IoControl(_DRIVER_OBJECT* deviceObject, _IRP* irp)
{
    UNREFERENCED_PARAMETER(deviceObject);

    NTSTATUS status = STATUS_UNSUCCESSFUL;
    ULONG byteIO = 0;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);

    ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;

    switch (controlCode) {
        case IO_READ_MEMORY: {
            Debug::info("Read Memory Requested");
            PKERNEL_READ_REQUEST input = (PKERNEL_READ_REQUEST)irp->AssociatedIrp.SystemBuffer;
            PEPROCESS process;

            if (NT_SUCCESS(PsLookupProcessByProcessId(&input->processId, &process)))
            {
                KernelReadVirtualMemory(process, &input->address, input->pBuf, input->size);
                status = STATUS_SUCCESS;
                byteIO = sizeof(KERNEL_READ_REQUEST);
                Debug::info("Read Memory Success");
            }
            else
            {
                Debug::info("Read Memory Error");
            }

            break;
        }
        case IO_WRITE_MEMORY: {
            Debug::info("Write Memory Requested");

            PKERNEL_WRITE_REQUEST input = (PKERNEL_WRITE_REQUEST)irp->AssociatedIrp.SystemBuffer;
            PEPROCESS process;

            if (NT_SUCCESS(PsLookupProcessByProcessId(&input->processId, &process)))
            {
                KernelWriteVirtualMemory(process, input->pBuf, &input->address, input->size);
                status = STATUS_SUCCESS;
                byteIO = sizeof(KERNEL_WRITE_REQUEST);
                Debug::info("Write Memory Success");
            }
            else
            {
                Debug::info("Write Memory Error");
            }

            break;
        }
    }

    irp->IoStatus.Status = status;
    irp->IoStatus.Information = byteIO;
    IoCompleteRequest(irp, IO_NO_INCREMENT);

    Debug::info("Performed action");

    return STATUS_SUCCESS;
}

NTSTATUS CloseCall(PDRIVER_OBJECT deviceObject, PIRP irp)
{
    UNREFERENCED_PARAMETER(deviceObject);

    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;
    IoCompleteRequest(irp, IO_NO_INCREMENT);

    Debug::info("Close Call called");

    return STATUS_SUCCESS;
}

NTSTATUS CreateCall(PDRIVER_OBJECT deviceObject, PIRP irp)
{
    UNREFERENCED_PARAMETER(deviceObject);

    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;
    IoCompleteRequest(irp, IO_NO_INCREMENT);

    Debug::info("Create Call called");

    return STATUS_SUCCESS;
}

NTSTATUS KernelReadVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size)
{
    return MmCopyVirtualMemory(process, sourceAddress, PsGetCurrentProcess(), targetAddress, size, KernelMode, NULL);
}

NTSTATUS KernelWriteVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size)
{
    return MmCopyVirtualMemory(PsGetCurrentProcess(), sourceAddress, process, targetAddress, size, KernelMode, NULL);
}

