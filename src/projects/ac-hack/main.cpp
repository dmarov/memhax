#include <chrono>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/aob-signature.h"
#include "modules/win-api-external-process-memory-editor.h"
#include "modules/instruction-nop-handler.h"
#include "modules/code-injection-handler.h"
#include <winuser.h>

int main(int argc, char **argv)
{
    try {
        const wchar_t* exe = L"ac_client.exe";
        const wchar_t* module_name = L"ac_client.exe";

        WinApiExternalProcessMemoryEditor editor(exe);

        const AOBSignaturePtr health_signature_ptr("2B F8 29 7B ?? 8B C7 5F 5E 8B E5", 2, editor.getModuleSpan(module_name));
        const AOBSignaturePtr ammo_signature_ptr("8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24", 8, editor.getModuleSpan(module_name));

        int instructions[] = {
            0x83, 0xBB, 0xF0, 0x00, 0x00, 0x00, 0x00, // 1: cmp dword ptr [ebx+000000F0],00
            0x74, 0x07,                               // 2: je 5:
            0x0F, 0x1F, 0x40, 0x00,                   // 3: nop dword ptr [eax+00]
            0x29, 0x7B, 0x04,                         // 4: sub [ebx+04],edi
            0x8B, 0xC7,                               // 5: mov eax,edi
            // jump back
        };

        std::vector<std::byte> instr_vec;

        for (auto b : instructions)
        {
            instr_vec.push_back((std::byte)b);
        }

        CodeInjectionHandler health_cheat_handler(editor, health_signature_ptr, 5, instr_vec);
        /* CodeInjectionHandler ammo_cheat_handler(editor, ammo_signature_ptr, 2); */
        /* InstructionNopHandler health_cheat_handler(editor, health_signature_ptr, 3); */
        /* InstructionNopHandler ammo_cheat_handler(editor, ammo_signature_ptr, 2); */

        bool enabled = false;

        while (true)
        {
            SHORT f9_key_state = GetAsyncKeyState(VK_F9);
            bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001);

            if (f9_key_down)
            {
                if (enabled)
                {
                    /* ammo_cheat_handler.disable(); */
                    health_cheat_handler.disable();
                }
                else
                {
                    /* ammo_cheat_handler.enable(); */
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

