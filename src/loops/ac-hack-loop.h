#pragma once

#include "modules/process-memory-editor.h"

class AcHackLoop {

private:
    const memhax::ProcessMemoryEditor* editor;
    bool interupted;
    const wchar_t* module_name = L"ac_client.exe";

    const unsigned infinity_health_instructions[14] = {
        0x83, 0xBB, 0xF0, 0x00, 0x00, 0x00, 0x00, // 1: cmp dword ptr [ebx+000000F0],00
        0x74, 0x03,                               // 2: je 4:
        0x29, 0x7B, 0x04,                         // 3: sub [ebx+04],edi
        0x8B, 0xC7,                               // 4: mov eax,edi
        // jump back
    };


public:
    AcHackLoop(const memhax::ProcessMemoryEditor& editor);
    void start();
};
