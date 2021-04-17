#include "pe-parser.h"
#include <exception>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include "modules/process-memory-editor.h"

namespace memhax {

PEParser::PEParser(const std::wstring& path)
{
    std::ifstream lib_file;
    lib_file.open(path, std::ios::binary);
    lib_file.seekg(0, std::ios::end);

    auto length = lib_file.tellg();

    lib_file.seekg(0, std::ios::beg);
    this->buffer = new std::byte[length];

    lib_file.read((char*)buffer, length);
    lib_file.close();

    this->dos_header = (PIMAGE_DOS_HEADER)this->buffer;

    if (this->dos_header->e_magic != IMAGE_DOS_SIGNATURE)
    {
        throw std::exception("wrong PE file magic number");
    }

    char* nt_headers = (char*)(this->buffer + this->dos_header->e_lfanew);

    this->nt_headers32 = (PIMAGE_NT_HEADERS32)nt_headers;
    this->nt_headers64 = (PIMAGE_NT_HEADERS64)nt_headers;

    if (this->nt_headers32->Signature != IMAGE_NT_SIGNATURE)
    {
        throw std::exception("nt signature mismatch");
    }

    this->is32bit = false;

    // 0x10b - PE32  (32bit)
    if (nt_headers32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
    {
        this->is32bit = true;
    }

    // 0x20b - PE32+ (64bit)
    if (nt_headers32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        this->is32bit = false;
    }

    if (this->is32bit)
    {
        auto rva = this->nt_headers32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        this->pimage_export_directory = (PIMAGE_EXPORT_DIRECTORY)(this->buffer + rva);
    }
    else
    {
        auto rva = this->nt_headers64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        this->pimage_export_directory = (PIMAGE_EXPORT_DIRECTORY)(this->buffer + rva);
    }

    // COFF file header
    this->file_header = (IMAGE_FILE_HEADER)(this->is32bit ? this->nt_headers32->FileHeader : this->nt_headers64->FileHeader);
    // Optional header
    if (this->is32bit)
    {
        this->opt_header32 = this->nt_headers32->OptionalHeader;
        this->entry_directory = this->opt_header32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    }
    else
    {
        this->opt_header64 = this->nt_headers64->OptionalHeader;
        this->entry_directory = this->opt_header64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    }
}

uintptr_t PEParser::getEntryDirectoryRVA()
{
    return this->entry_directory.VirtualAddress;
}

uintptr_t PEParser::getEntryDirectorySize()
{
    return this->entry_directory.Size;
}

uintptr_t PEParser::getNamesRVA(IMAGE_EXPORT_DIRECTORY dir)
{
    return dir.AddressOfNames;
}

uintptr_t PEParser::getFunctionsRVA(IMAGE_EXPORT_DIRECTORY dir)
{
    return dir.AddressOfFunctions;
}

uintptr_t PEParser::getNameOrdinalsRVA(IMAGE_EXPORT_DIRECTORY dir)
{
    return dir.AddressOfNameOrdinals;
}

size_t PEParser::getNumberOfNames(IMAGE_EXPORT_DIRECTORY dir)
{
    return dir.NumberOfNames;
}

size_t PEParser::getNumberOfFunctions(IMAGE_EXPORT_DIRECTORY dir)
{
    return dir.NumberOfFunctions;
}

bool PEParser::isForAMD64Arch()
{
    return this->file_header.Machine != IMAGE_FILE_MACHINE_AMD64;
}

bool PEParser::isForX86Arch()
{
    return this->file_header.Machine != IMAGE_FILE_MACHINE_I386;
}

uintptr_t PEParser::getOptionalHeaderImageBase()
{
    if (this->isForAMD64Arch())
    {
        return this->opt_header64.ImageBase;
    }
    else if(this->isForX86Arch())
    {
        return this->opt_header32.ImageBase;
    }

    throw std::exception("failed to identify architecture");
}

size_t PEParser::getOptionalHeaderImageSize()
{
    if (this->isForAMD64Arch())
    {
        return this->opt_header64.SizeOfImage;
    }
    else if(this->isForX86Arch())
    {
        return this->opt_header32.SizeOfImage;
    }

    throw std::exception("failed to identify architecture");
}

PEParser::~PEParser()
{
    delete[] this->buffer;
}

}
