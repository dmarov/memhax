#include <chrono>
#include <iostream>
#include <thread>
#include "modules/multi-lvl-ptr.h"
#include "modules/aob-signature-ptr.h"
#include "modules/win-api-process-memory-editor.h"
#include "modules/process-memory-editor.h"
#include <winuser.h>

//health
// 2B F8 29 7B ?? 8B C7 5F 5E 8B E5
//       == == ==

// ammo
// 8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24
//                         == ==

int main(int argc, char **argv)
{
    const wchar_t* exe = L"ac_client.exe";
    const wchar_t* module = L"ac_client.exe";

    try {

        WinApiProcessMemoryEditor mem(exe, true);

        auto [mod_start, mod_size] = mem.getModuleInfo(module);

        const AOBSignaturePtr health_signature_ptr("2B F8 29 7B ?? 8B C7 5F 5E 8B E5", 2, mod_start, mod_size);
        const AOBSignaturePtr ammo_signature_ptr("8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24", 8, mod_start, mod_size);

        char nops[100];

        for (size_t i = 0; i < 100; ++i)
        {
            nops[i] = 0x90;
        }
        /* std::memset(nops, '\x90', 10); */
        /* /1* std::byte nops[10] = {(std::byte)0x90u}; *1/ */
        mem.write(ammo_signature_ptr, nops, 2);
        mem.write(health_signature_ptr, nops, 3);

        /* InstructionNopCheatHandler ammo_cheat_handler(mem, ammo_signature_ptr, 3); */

        /* ammo_cheat_handler.enable(); */
        /* ammo_cheat_handler.disable(); */
    }
    catch(std::exception &e)
    {
        std::cerr << "error occured: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

