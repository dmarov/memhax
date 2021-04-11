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

    this->nt_headers = (PIMAGE_NT_HEADERS)(this->buffer + this->dos_header->e_lfanew);

    if (this->nt_headers->Signature != IMAGE_NT_SIGNATURE)
    {
        throw std::exception("nt signature mismatch");
    }

    // COFF file header
    auto fileHeader = (IMAGE_FILE_HEADER)(this->nt_headers->FileHeader);
    // Optional header
    auto optHeader = (IMAGE_OPTIONAL_HEADER)(this->nt_headers->OptionalHeader);

    std::cout << "COFF header" << std::endl;
    std::cout << "Machine: " << std::hex << fileHeader.Machine << std::endl;
    std::cout << "Size of optional header: " << std::hex << fileHeader.SizeOfOptionalHeader << std::endl;
    std::cout << "Characteristics: " << std::hex << fileHeader.Characteristics << std::endl;
    /* this->dos_header-> */
#ifdef _WIN64
    if (fileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
    {
        delete[] this->buffer;
        throw std::exception("invalid platform");
    }
#else
    if (fileHeader.Machine != IMAGE_FILE_MACHINE_I386)
        delete[] this->buffer;
        throw std::exception("invalid platform");
    }
#endif

// 0x10b - PE32  (32bit)
// 0x20b - PE32+ (64bit)
    std::cout << "Optional header" << std::endl;
    std::cout << "magic: " << std::hex << optHeader.Magic << std::endl;
    std::cout << "Linker maj: " << std::hex << optHeader.MajorLinkerVersion << std::endl;
    std::cout << "Linker min: " << std::hex << optHeader.MinorLinkerVersion << std::endl;
    std::cout << "OS maj: " << std::dec << optHeader.MajorOperatingSystemVersion << std::endl;
    std::cout << "OS min: " << std::dec << optHeader.MinorOperatingSystemVersion << std::endl;
    std::cout << "size in memory: " << std::hex << optHeader.SizeOfImage << std::endl;
    std::cout << "size of headers: " << std::hex << optHeader.SizeOfHeaders << std::endl;
    std::cout << "Heap commit: " << std::dec << optHeader.SizeOfHeapCommit << std::endl;
    std::cout << "data entries: " << std::dec << optHeader.NumberOfRvaAndSizes << std::endl;
    std::cout << "preferred base addr: " << std::hex << optHeader.ImageBase << std::endl;
    auto dir = (IMAGE_DATA_DIRECTORY)(optHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);

    auto exp_addr = info.addr + dir.VirtualAddress;
    auto mem = new std::byte[dir.Size];

    editor.read_p(exp_addr, mem, dir.Size);

    auto dir_mem = (PIMAGE_EXPORT_DIRECTORY)mem;
    std::cout << std::dec << dir_mem->NumberOfNames << std::endl;
    std::cout << std::dec << dir_mem->NumberOfFunctions << std::endl;
    std::cout << std::hex << info.addr + dir_mem->AddressOfFunctions << std::endl;
    std::cout << std::hex << info.addr + dir_mem->AddressOfNames << std::endl;
    std::cout << std::hex << info.addr + dir_mem->AddressOfNameOrdinals << std::endl;

    DWORD* names = new DWORD[dir_mem->NumberOfNames];
    DWORD* functions = new DWORD[dir_mem->NumberOfFunctions];
    WORD* ordinals = new WORD[dir_mem->NumberOfFunctions];

    editor.read_p(info.addr + dir_mem->AddressOfNames, names, sizeof(DWORD) * dir_mem->NumberOfFunctions);
    editor.read_p(info.addr + dir_mem->AddressOfFunctions, functions, sizeof(DWORD) * dir_mem->NumberOfFunctions);
    editor.read_p(info.addr + dir_mem->AddressOfNameOrdinals, ordinals, sizeof(WORD) * dir_mem->NumberOfFunctions);

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

    /* PIMAGE_NT_HEADERS32 nt_header32 = (PIMAGE_NT_HEADERS32)nt_headers; */
    /* PIMAGE_NT_HEADERS64 nt_header64 = (PIMAGE_NT_HEADERS64)nt_headers; */

    /* bool is32bit = false; */

    /* if (nt_header32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) */
    /* { */
    /*     is32bit = true; */
    /* } */
    /* else if (nt_header32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) */
    /* { */
    /*     is32bit = false; */
    /* } */

    /* if (is32bit) */
    /* { */
    /*     this->pimage_export_directory = (PIMAGE_EXPORT_DIRECTORY) */
    /*         (&nt_header32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress); */
    /*     /1* std::cout << std::hex << nt_header32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress << std::endl; *1/ */
    /* } */
    /* else */
    /* { */
    /*     this->pimage_export_directory = (PIMAGE_EXPORT_DIRECTORY) */
    /*         (this->buffer + nt_header64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress); */
    /*     std::cout << (DWORD)this->pimage_export_directory->NumberOfNames << std::endl; */
    /* } */

    /* if (this->pimage_export_directory == NULL) */
    /* { */
    /*     throw std::exception("failed to get export directory RVA"); */
    /* } */

    /* std::cout << (DWORD)this->pimage_export_directory->NumberOfNames << std::endl; */
    /* std::cout << (int)this->buffer[this->pimage_export_directory->AddressOfFunctions] << std::endl; */

    /* this->peat = (PDWORD)(this->buffer + this->pimage_export_directory->AddressOfFunctions); */
    /* this->pent = (PDWORD)(this->buffer + this->pimage_export_directory->AddressOfNames); */
    /* this->peot = (PWORD)(this->buffer + this->pimage_export_directory->AddressOfNameOrdinals); */



    /* std::cout << ">" << addrelib << std::endl; */

    /* std::cout << (char*)((addrelib + this->pent[2])) <<std::endl; */
}

/* size_t PEParser::getExportRVA(const std::wstring name) { */

/*     unsigned short ordinal = 0; */
/*     /1* auto c_name = name.c_str(); *1/ */
/*     /1* /2* std::cout << this->pimage_export_directory->NumberOfNames << std::endl; *2/ *1/ */
/*     /1* for (DWORD i = 0; i < this->pimage_export_directory->NumberOfNames; ++i ) *1/ */
/*     /1* { *1/ */
/*     /1*     /2* std::cout << (LPCSTR)(&this->pent[i]) <<std::endl; *2/ *1/ */

/*     /1*     /2* if (!lstrcmpiA((LPCSTR)c_name, (LPCSTR)(this->buffer + pent[i]))) *2/ *1/ */
/*     /1*     /2* { *2/ *1/ */
/*     /1*     /2*     ordinal = peot[i]; *2/ *1/ */
/*     /1*     /2*     break; *2/ *1/ */
/*     /1*     /2* } *2/ *1/ */
/*     /1* } *1/ */

/*     return ordinal ? peat[ordinal] : 0; */
/* } */

PEParser::~PEParser()
{
    delete[] this->buffer;
}

}
