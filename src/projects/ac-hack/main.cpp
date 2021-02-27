#include <chrono>
#include <cstring>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/multi-lvl-ptr.h"
#include "modules/aob-signature.h"
#include "modules/aob-signature-ptr.h"
#include "modules/win-api-process-memory-editor.h"
#include <winuser.h>
#include <bitset>

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

        auto addr = mem.findFirstAddressByAOBPattern(health_signature_ptr.getSignature(), mod_start, mod_size);

        if (addr == NULL)
        {
            throw std::exception("could not find pattern");
        }

        char* nops = new char[3];
        std::memset(nops, '\x90', 3);
        mem.write(addr + health_signature_ptr.getBegin(), nops, 3);
        delete[] nops;
        /* mem.nop(base + ammo_signature_ptr.getBegin(), 2); */

        /* InstructionNopCheatHandler health_cheat(mem, health_signature_ptr); */
        /* HeapTrainerCheatHandler health_cheat(mem, health_signature_ptr); */

        /* health_cheat.enable(); */
        /* health_cheat.disable(); */

    }
    catch(std::exception &e)
    {
        std::cerr << "error occured: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

