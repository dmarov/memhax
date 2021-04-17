#pragma once

#include <string>
#include <windows.h>
#include "modules/process-memory-editor.h"

namespace memhax {

class PEParser {

private:
    std::byte* buffer;
    PIMAGE_DOS_HEADER dos_header;
    PIMAGE_NT_HEADERS32 nt_headers32;
    PIMAGE_NT_HEADERS64 nt_headers64;
    bool is32bit;
    PIMAGE_EXPORT_DIRECTORY pimage_export_directory;

    IMAGE_FILE_HEADER file_header;
    IMAGE_OPTIONAL_HEADER32 opt_header32;
    IMAGE_OPTIONAL_HEADER64 opt_header64;
    IMAGE_DATA_DIRECTORY entry_directory;

public:
    PEParser(const std::wstring& path);

    uintptr_t getEntryDirectoryRVA();
    size_t getEntryDirectorySize();

    uintptr_t getNamesRVA(IMAGE_EXPORT_DIRECTORY dir);
    uintptr_t getFunctionsRVA(IMAGE_EXPORT_DIRECTORY dir);
    uintptr_t getNameOrdinalsRVA(IMAGE_EXPORT_DIRECTORY dir);

    size_t getNumberOfNames(IMAGE_EXPORT_DIRECTORY dir);
    size_t getNumberOfFunctions(IMAGE_EXPORT_DIRECTORY dir);

    bool isForAMD64Arch();
    bool isForX86Arch();

    uintptr_t getOptionalHeaderImageBase();
    size_t getOptionalHeaderImageSize();

    ~PEParser();
};

}
