#include "ac-hack-loop.h"
#include <chrono>
#include <thread>
#include "modules/process-memory-editor.h"
#include "modules/instruction-nop-handler.h"
#include "modules/code-injection-handler.h"
#include <windows.h>

AcHackLoop::AcHackLoop(const memhax::ProcessMemoryEditor& editor)
{
    this->editor = &editor;
    this->interupted = false;
}

void AcHackLoop::start()
{
    const wchar_t* module_name = L"ac_client.exe";

    const memhax::AOBSignaturePtr health_signature_ptr(
        "2B F8 29 7B ?? 8B C7 5F 5E 8B E5",
        2,
        (*(this->editor)).getModuleSpan(module_name)
    );

    const memhax::AOBSignaturePtr ammo_signature_ptr(
        "8B 56 ?? 89 0A 8B 76 ?? FF 0E 57 8B 7C 24 ?? 8D 74 24",
        8,
        (*(this->editor)).getModuleSpan(module_name)
    );

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

    memhax::CodeInjectionHandler health_cheat_handler(*(this->editor), health_signature_ptr, 5, instr_vec);
    memhax::InstructionNopHandler ammo_cheat_handler(*(this->editor), ammo_signature_ptr, 2);

    bool enabled = false;

    while (true)
    {
        if (this->interupted)
        {
            break;
        }

        SHORT f9_key_state = GetAsyncKeyState(VK_F9);
        bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001);

        SHORT f10_key_state = GetAsyncKeyState(VK_F10);
        bool f10_key_down = (f10_key_state & 0x8000) && (f10_key_state & 0x0001);

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

        if (f10_key_down)
        {
            this->interupted = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
