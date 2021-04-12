#include "pe-parser.h"
#include <exception>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include "modules/process-memory-editor.h"

namespace memhax {

PEParser::PEParser(const ProcessMemoryEditor& editor)
{
    std::ifstream lib_file;
    auto info = editor.getModuleInfo(L"KERNEL32.DLL");

    auto path = info.path;

    /* this->buffer = buf; */
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

    auto exp_addr = info.addr + this->entry_directory.VirtualAddress;
    auto mem = new std::byte[this->entry_directory.Size];

    editor.read_p(exp_addr, mem, this->entry_directory.Size);

    auto dir_mem = (PIMAGE_EXPORT_DIRECTORY)mem;
    std::cout << std::dec << dir_mem->NumberOfNames << std::endl;
    std::cout << std::dec << dir_mem->NumberOfFunctions << std::endl;
    std::cout << std::hex << info.addr + dir_mem->AddressOfFunctions << std::endl;
    std::cout << std::hex << info.addr + dir_mem->AddressOfNames << std::endl;
    std::cout << std::hex << info.addr + dir_mem->AddressOfNameOrdinals << std::endl;

    DWORD* names = new DWORD[dir_mem->NumberOfNames];
    DWORD* functions = new DWORD[dir_mem->NumberOfFunctions];
    WORD* ordinals = new WORD[dir_mem->NumberOfFunctions];

    editor.read_p(info.addr + dir_mem->AddressOfNames, names, sizeof(DWORD) * dir_mem->NumberOfNames);
    editor.read_p(info.addr + dir_mem->AddressOfFunctions, functions, sizeof(DWORD) * dir_mem->NumberOfFunctions);
    editor.read_p(info.addr + dir_mem->AddressOfNameOrdinals, ordinals, sizeof(WORD) * dir_mem->NumberOfNames);

    char strbuf[100];
    const char* c_name = "LoadLibraryA";

    for (auto i = 0; i < dir_mem->NumberOfNames; ++i)
    {
        editor.read_p(info.addr + names[i], strbuf, 100);
        /* std::cout << strbuf << std::endl; */

        if (!strcmp(c_name, strbuf))
        {
            auto ordinal = ordinals[i];
            std::cout << "ordinal: " << std::hex << ordinals[i] << std::endl;
            std::cout << "address: " << info.addr + functions[ordinal] << std::endl;
            break;
        }
    }

    /* this->pimage_export_directory = (PIMAGE_EXPORT_DIRECTORY)(this->buffer + dir.VirtualAddress); */

    /* std::cout << "Exports table char: " << this->pimage_export_directory->Characteristics << std::endl; */
    /* std::cout << "Exports table RVA: " << this->pimage_export_directory->Base << std::endl; */
    /* std::cout << "Exports table size: " << optHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size << std::endl; */
    /* std::cout << "file size: " << std::hex << length << std::endl; */
    /* std::cout << "Address of name ordinals: " << std::hex << this->pimage_export_directory->AddressOfNameOrdinals << std::endl; */
    /* std::cout << "Address of functions: " << std::hex << this->pimage_export_directory->AddressOfFunctions << std::endl; */
    /* std::cout << "Address of names: " << std::hex << this->pimage_export_directory->AddressOfNames << std::endl; */
    /* std::cout << "Number of names: " << std::dec << (this->pimage_export_directory->NumberOfNames) << std::endl; */
    /* std::cout << "Number of functions: " << std::dec << (this->pimage_export_directory->NumberOfFunctions) << std::endl; */
    /* std::cout << "Exports base: " << std::hex << (this->pimage_export_directory->Base) << std::endl; */
    /* std::cout << (char*)this->buffer[this->pimage_export_directory->AddressOfNames] << std::endl; */

    /* for (DWORD i = 0; i < this->pimage_export_directory->NumberOfNames; ++i) */
    /* { */

    /*     auto ord = (PDWORD)(this->buffer + this->pimage_export_directory->Base + this->pimage_export_directory->AddressOfNames + i); */
    /*     std::cout << *ord << std::endl; */
    /*     /1* auto addr = (PDWORD)(this->buffer + this->pimage_export_directory->AddressOfFunctions)[i]; *1/ */
    /*     /1* auto v = addr; *1/ */
    /*     /1* std::cout << v << std::endl; *1/ */
    /*     /1* std::cout << (int)*(this->buffer + arr[i]) << std::endl; *1/ */

    /*     /1* if (!lstrcmpiA((LPCSTR)c_name, (LPCSTR)(this->buffer + pent[i]))) *1/ */
    /*     /1* { *1/ */
    /*     /1*     ordinal = peot[i]; *1/ */
    /*     /1*     break; *1/ */
    /*     /1* } *1/ */
    /* } */
    /* char* nt_headers = (char*)(this->buffer + this->dos_header->e_lfanew); */
}

uintptr_t PEParser::getEntryDirectoryRVA()
{
    return this->entry_directory.VirtualAddress;
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

PEParser::~PEParser()
{
    delete[] this->buffer;
}

}
