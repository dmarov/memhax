#include <ntddk.h>
#include "debug.h"
#include <stdio.h>
#include <stdarg.h>

void Debug::info(const char * format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    char buffer[200];
    auto res = sprintf(buffer, format, argptr);

    if (res < 0) {
        KdPrintEx((
            DPFLTR_IHVDRIVER_ID,
            DPFLTR_ERROR_LEVEL,
            "winkmemdx64:error: error while logging\n"
        ));
    } else {
        KdPrintEx((
            DPFLTR_IHVDRIVER_ID,
            DPFLTR_INFO_LEVEL,
            "winkmemdx64:info: %s\n", buffer
        ));
    }
}

void Debug::error(const char * format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    char buffer[200];
    auto res = sprintf(buffer, format, argptr);

    if (res < 0) {
        KdPrintEx((
            DPFLTR_IHVDRIVER_ID,
            DPFLTR_ERROR_LEVEL,
            "winkmemdx64:error: error while logging\n"
        ));
    } else {
        KdPrintEx((
            DPFLTR_IHVDRIVER_ID,
            DPFLTR_INFO_LEVEL,
            "winkmemdx64:error: %s\n", buffer
        ));
    }
}

void Debug::warn(const char * format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    char buffer[200];
    auto res = sprintf(buffer, format, argptr);

    if (res < 0) {
        KdPrintEx((
            DPFLTR_IHVDRIVER_ID,
            DPFLTR_ERROR_LEVEL,
            "winkmemdx64:error: error while logging\n"
        ));
    } else {
        KdPrintEx((
            DPFLTR_IHVDRIVER_ID,
            DPFLTR_WARNING_LEVEL,
            "winkmemdx64:warning: %s\n", buffer
        ));
    }
}
