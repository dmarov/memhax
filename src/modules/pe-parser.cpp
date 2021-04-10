#include "pe-parser.h"
#include <string>
#include <fstream>
#include <iostream>

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

    if (*(short*)buffer != *((short*)this->MAGIC_BYTES[1]))
    {
        throw std::exception("wrong PE file magic number");
    }

    this->header = (DOSHeader*)this->buffer;

    if (this->header->e_lfanew != *((long*)this->DOS_SIGNATURE))
    {
        throw std::exception("signature mismatch");
    }

    /* if ( pimage_dos_header->e_magic != IMAGE_DOS_SIGNATURE ) */
    /*     return 0; */

    /* auto pimage_nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( module_base + pimage_dos_header->e_lfanew ); */

    /* if ( pimage_nt_headers->Signature != IMAGE_NT_SIGNATURE ) */
    /*     return 0; */

    /* auto export_directory_rva = pimage_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress; */

    /* if ( !export_directory_rva ) */
    /*     return 0; */

    /* auto pimage_export_directory = reinterpret_cast< PIMAGE_EXPORT_DIRECTORY >( module_base + export_directory_rva ); */
    /* auto peat = reinterpret_cast< PDWORD >( module_base + pimage_export_directory->AddressOfFunctions ); */
    /* auto pent = reinterpret_cast< PDWORD >( module_base + pimage_export_directory->AddressOfNames ); */
    /* auto peot = reinterpret_cast< PWORD >( module_base + pimage_export_directory->AddressOfNameOrdinals ); */

    /* unsigned short ordinal = 0; */

    /* for ( DWORD i = 0; i < pimage_export_directory->NumberOfNames; ++i ) */
    /* { */
    /*     if ( !lstrcmpiA( export_name, reinterpret_cast< const char* >( module_base + pent[i] ) ) ) */
    /*     { */
    /*         ordinal = peot[i]; */
    /*         break; */
    /*     } */
    /* } */
    /* return ordinal ? module_base + peat[ordinal] : 0; */
}


PEParser::~PEParser()
{
    delete[] this->buffer;
}

}
