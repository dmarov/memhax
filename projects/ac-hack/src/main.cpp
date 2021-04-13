#include <chrono>
#include <iostream>
#include <thread>
#include "modules/win-api-external-process-memory-editor.h"
#include "modules/instruction-nop-handler.h"
#include "modules/code-injection-handler.h"
#include <boost/program_options.hpp>
#include "main.h"

namespace po = boost::program_options;

bool interupted = false;

BOOL WINAPI consoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        interupted = true;
    }

    return TRUE;
}

int main(int argc, char **argv)
{

    po::options_description desc("Supported options");

    desc.add_options()
        ("help", "produce help message")
        ("version,v", "print version");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("version")) {
        std::cout << "v" << ac_hack_VERSION_MAJOR << "." << ac_hack_VERSION_MINOR << "." << ac_hack_VERSION_PATCH << std::endl;
        return 0;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
    {
        return 1;
    }

    std::cout << "Searching for signatures..." << std::endl;

    try
    {
        const wchar_t* exe = L"ac_client.exe";
        const wchar_t* module_name = L"ac_client.exe";

        memhax::WinApiExternalProcessMemoryEditor editor(exe);

        const memhax::AOBSignaturePtr health_signature_ptr("2B F8 29 7B ?? 8B C7 5F 5E 8B E5", 2, editor.getModuleSpan(module_name));
        const memhax::AOBSignaturePtr ammo_signature_ptr("8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24", 8, editor.getModuleSpan(module_name));

        unsigned char instructions[] = {
            0x83, 0xBB, 0xF0, 0x00, 0x00, 0x00, 0x00, // 1: cmp dword ptr [ebx+000000F0],00
            0x74, 0x03,                               // 2: je 4:
            0x29, 0x7B, 0x04,                         // 3: sub [ebx+04],edi
            0x8B, 0xC7,                               // 4: mov eax,edi
            // jump back
        };

        std::vector<std::byte> instr_vec;

        for (auto b : instructions)
        {
            instr_vec.push_back((std::byte)b);
        }

        memhax::CodeInjectionHandler health_cheat_handler(editor, health_signature_ptr, 5, instr_vec);
        memhax::InstructionNopHandler ammo_cheat_handler(editor, ammo_signature_ptr, 2);

        bool enabled = false;

        std::cout << "Press F9 to toggle infinite health" << std::endl;
        std::cout << "Press Ctrl+C to quit" << std::endl;

        while (true)
        {
            if (interupted)
            {
                break;
            }

            SHORT f9_key_state = GetAsyncKeyState(VK_F9);
            bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001);

            if (f9_key_down)
            {
                if (enabled)
                {
                    ammo_cheat_handler.disable();
                    health_cheat_handler.disable();
                }
                else
                {
                    ammo_cheat_handler.enable();
                    health_cheat_handler.enable();
                }

                enabled = !enabled;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch(std::exception &e)
    {
        std::cerr << "error occured: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

