#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/multi-lvl-ptr.h"
#include "modules/aob-signature.h"
#include "modules/win-api-process-memory-editor.h"
#include <winuser.h>
#include <bitset>

// health_1
// 89 8A ?? ?? ?? ?? 89 82 ?? ?? ?? ?? 0F 94 C1
//                   == == == == == ==

// ammo
// 8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24
//                         == ==
 
//health_2
// ac_client.exe+29D1D - 2B F8                 - sub edi,eax
// ac_client.exe+29D1F - 29 7B 04              - sub [ebx+04],edi
// ac_client.exe+29D22 - 8B C7                 - mov eax,edi
// ac_client.exe+29D24 - 5F                    - pop edi
// ac_client.exe+29D25 - 5E                    - pop esi
// ac_client.exe+29D26 - 8B E5                 - mov esp,ebp
// 2B F8 29 7B ?? 8B C7 5F 5E 8B E5
//       == == ==

int main(int argc, char **argv)
{
    const wchar_t* exe = L"ac_client.exe";
    const wchar_t* module = L"ac_client.exe";
    AOBSignature health_signature_1("89 8A ?? ?? ?? ?? 89 82 ?? ?? ?? ?? 0F 94 C1");
    AOBSignature health_signature_2("2B F8 29 7B ?? 8B C7 5F 5E 8B E5");
    AOBSignature ammo_signature("8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24");
    uintptr_t base = NULL;

    try {

        WinApiProcessMemoryEditor mem(exe, true);

        auto [mod_start, mod_size] = mem.getModuleInfo(module);

        base = mem.findFirstAddressByAOBPattern(health_signature_2, mod_start, mod_size);

        if (base == NULL)
        {
            throw std::exception("could not find pattern");
        }

        mem.nop(base + 2, 3);
    }
    catch(std::exception &e)
    {
        std::cerr << "error occured: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

