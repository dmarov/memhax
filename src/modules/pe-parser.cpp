#include "pe-parser.h"
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winbase.h>

namespace memhax {

PEParser::PEParser(const std::wstring path)
{
    std::ifstream lib_file;

    lib_file.open(path, std::ios::binary);
    lib_file.seekg(0, std::ios::end);

    auto length = lib_file.tellg();

    lib_file.seekg(0, std::ios::beg);
    this->buffer = new std::byte[length];

    lib_file.read((char*)buffer, length);
    lib_file.close();

    for (auto i = 0; i < 30; ++i)
    {
        std::cout << "0x" << std::hex << (int)buffer[i] << " ";
    }

    std::cout << std::endl;

    this->dos_header = (PIMAGE_DOS_HEADER)this->buffer;

    if (this->dos_header->e_magic != IMAGE_DOS_SIGNATURE)
    {
        throw std::exception("wrong PE file magic number");
    }

    if (this->dos_header->e_lfanew != IMAGE_DOS_SIGNATURE)
    {
        throw std::exception("dos signature mismatch");
    }

    this->nt_headers = (PIMAGE_NT_HEADERS)(this->dos_header + this->dos_header->e_lfanew);

    if (this->nt_headers->Signature != IMAGE_NT_SIGNATURE)
    {
        throw std::exception("nt signature mismatch");
    }

    auto export_directory_rva = this->nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    if (!export_directory_rva)
    {
        throw std::exception("failed to get export directory RVA");
    }

    this->pimage_export_directory = (PIMAGE_EXPORT_DIRECTORY)(this->buffer + export_directory_rva);

    this->peat = (PDWORD)(this->buffer + pimage_export_directory->AddressOfFunctions);
    this->pent = (PDWORD)(this->buffer + pimage_export_directory->AddressOfNames);
    this->peot = (PWORD)(this->buffer + pimage_export_directory->AddressOfNameOrdinals);
}

size_t PEParser::getExportRVA(const std::wstring name) {

    unsigned short ordinal = 0;
    auto c_name = name.c_str();

    for (DWORD i = 0; i < pimage_export_directory->NumberOfNames; ++i )
    {
        if (!lstrcmpiA( (LPCSTR)c_name, (LPCSTR)( this->buffer + pent[i])))
        {
            ordinal = peot[i];
            break;
        }
    }

    return ordinal ? peat[ordinal] : 0;
}

PEParser::~PEParser()
{
    delete[] this->buffer;
}

}
