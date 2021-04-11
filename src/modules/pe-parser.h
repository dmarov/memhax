#pragma once

#include <string>
#include <windows.h>
#include "modules/process-memory-editor.h"

namespace memhax {

class PEParser {

private:

private:
    std::byte* buffer;
    PIMAGE_DOS_HEADER dos_header;
    PIMAGE_NT_HEADERS nt_headers;
    PIMAGE_EXPORT_DIRECTORY pimage_export_directory;
    PDWORD peat;
    PDWORD pent;
    PWORD peot;

public:
    PEParser(const ProcessMemoryEditor& editor);
    /* PEParser(std::byte* addr, size_t size); */

    /* size_t getExportRVA(const std::wstring name); */

    ~PEParser();
};

}