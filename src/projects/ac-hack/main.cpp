#include <chrono>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/aob-signature.h"
#include "modules/win-api-external-process-memory-editor.h"
#include "modules/instruction-nop-cheat-handler.h"
#include <winuser.h>

// health
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

        WinApiExternalProcessMemoryEditor editor(exe, true);

        auto [mod_name, mod_start, mod_size] = editor.getModuleInfo(module);

        const AOBSignaturePtr health_signature_ptr("2B F8 29 7B ?? 8B C7 5F 5E 8B E5", 2, mod_start, mod_size);
        /* const AOBSignaturePtr health_signature_ptr("2B F8 29 7B ?? 8B C7 5F 5E 8B E5", 2, editor.getModules()); */
        const AOBSignaturePtr ammo_signature_ptr("8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24", 8, mod_start, mod_size);

        InstructionNopCheatHandler health_cheat_handler(editor, health_signature_ptr, 3);
        InstructionNopCheatHandler ammo_cheat_handler(editor, ammo_signature_ptr, 2);

        bool enabled = false;

        while (true)
        {
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

