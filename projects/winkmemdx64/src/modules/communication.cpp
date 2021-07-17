#include "communication.h"
#include "debug.h"

NTSTATUS IoControl(PDRIVER_OBJECT deviceObject, PIRP irp)
{
    UNREFERENCED_PARAMETER(deviceObject);

    NTSTATUS status = STATUS_UNSUCCESSFUL;
    ULONG byteIO = 0;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);

    ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;

    switch (controlCode) {
        case IO_READ_MEMORY:

            break;
        case IO_WRITE_MEMORY:

            break;
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
