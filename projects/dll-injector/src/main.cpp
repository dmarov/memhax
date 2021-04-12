#include <chrono>
#include <fstream>
#include <locale>
#include <thread>
#include <iostream>
#include <boost/program_options.hpp>
#include "main.h"
#include "modules/win-api-external-process-memory-editor.h"
#include "modules/pe-parser.h"
#include <windows.h>

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    po::options_description desc("Supported options");

    desc.add_options()
        ("help", "produce help message")
        ("version,v", "print version")
        ("lib", po::value<std::string>(), "specify library to inject")
        ("target", po::value<std::string>(), "specify target executable");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("version")) {
        std::cout << "v" << dll_injector_VERSION_MAJOR << "." << dll_injector_VERSION_MINOR << "." << dll_injector_VERSION_PATCH << std::endl;
        return 0;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!vm.count("lib")) {
        std::cout << "specify library file via --lib" << std::endl;
        return 1;
    }

    if (!vm.count("target")) {
        std::cout << "specify target process name via --target" << std::endl;
        return 1;
    }

    auto target_name = vm["target"].as<std::string>();
    auto library_file = vm["lib"].as<std::string>();
    std::wstring target_name_str(target_name.begin(), target_name.end());
    auto lib_cstr = library_file.c_str();
    std::ifstream dll_file(lib_cstr);

    try
    {
        if (!dll_file.good())
        {
            throw std::exception("dll file not found");
        }

        std::cout << "Injecting ..." << lib_cstr << std::endl;

        auto pid = memhax::WinApiExternalProcessMemoryEditor::getProcessId(target_name_str);
        memhax::WinApiExternalProcessMemoryEditor editor(target_name_str);

        auto modules = editor.getModules();

        auto info = editor.getModuleInfo(L"KERNEL32.DLL");

        memhax::PEParser parser(info.path);

        auto exp_addr = info.addr + parser.getEntryDirectoryRVA();
        auto mem = new std::byte[parser.getEntryDirectorySize()];

        editor.read_p(exp_addr, mem, parser.getEntryDirectorySize());

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
        uintptr_t liba_addr = NULL;

        for (auto i = 0; i < dir_mem->NumberOfNames; ++i)
        {
            editor.read_p(info.addr + names[i], strbuf, 100);

            if (!strcmp(c_name, strbuf))
            {
                auto ordinal = ordinals[i];
                liba_addr = info.addr + functions[ordinal];
                std::cout << "ordinal: " << std::hex << ordinals[i] << std::endl;
                std::cout << "address: " << std::hex << liba_addr << std::endl;
                break;
            }
        }

        if (!liba_addr)
        {
            throw new std::exception("could not find LoadLibraryA in target process");
        }

        auto length = strlen(lib_cstr) + 1;
        auto addr = editor.allocate(length, NULL);
        editor.write_p(addr, (void*)lib_cstr, length);
        HANDLE handle = editor.getHandle();

        HANDLE th = CreateRemoteThread(handle, NULL, 0, (LPTHREAD_START_ROUTINE)liba_addr, (LPVOID)addr, 0, NULL);
        WaitForSingleObject(th, INFINITE);
        editor.free(addr, length);
        CloseHandle(th);

        std::cout << "Injected successfully" << std::endl;

        delete[] names;
        delete[] functions;
        delete[] ordinals;
    }
    catch(std::exception e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    return 0;
}
