#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/instruction-nop-cheat-handler.h"
#include "modules/win-api-external-process-memory-editor.h"
#include <winuser.h>
#include <bitset>

// 1E6691B9D29 - 83 38 00              - cmp dword ptr [rax],00
// 1E6691B9D2C - F3 0F10 45 EC         - movss xmm0,[rbp-14]
// 1E6691B9D31 - F3 0F5A C0            - cvtss2sd xmm0,xmm0
// 1E6691B9D35 - F2 0F5A E8            - cvtsd2ss xmm5,xmm0
// 1E6691B9D39 - F3 0F11 68 14         - movss [rax+14],xmm5  <--
// 1E6691B9D3E - 48 8B 7D F0           - mov rdi,[rbp-10]
// 1E6691B9D42 - 4C 8B 7D F8           - mov r15,[rbp-08]
// 1E6691B9D46 - 48 8D 65 00           - lea rsp,[rbp+00]
// 1E6691B9D4A - 5D                    - pop rbp

// F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 48 8B 7D
//                         == == == == ==

int main(int argc, char **argv)
{
    try {
        const wchar_t* exe = L"TankForce.exe";
        /* const wchar_t* module = L"mono-2.0-bdwgc.dll"; */
        const wchar_t* module = L"TankForce.exe";

        WinApiExternalProcessMemoryEditor editor(exe, true);

        /* auto [mod_name, mod_start, mod_size] = editor.getModuleInfo(module); */

        AOBSignature ammo_ptr("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 48 8B 7D");
        std::cout << editor.countAOBSignatureMatches(ammo_ptr, editor.getModules()) << std::endl;
        /* std::cout << editor.getRegularPointer(ammo_ptr) << std::endl; */
        /* InstructionNopCheatHandler handler(editor, ammo_ptr, 5); */

        /* MultiLvlPtr           health(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x14 }); */
        /* MultiLvlPtr       max_health(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x18 }); */
        /* MultiLvlPtr        max_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x28, 0x18 }); */
        /* MultiLvlPtr     turret_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x14 }); */
        /* MultiLvlPtr max_turret_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x18 }); */
        /* MultiLvlPtr       turn_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x14 }); */
        /* MultiLvlPtr   max_turn_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x18 }); */
        /* MultiLvlPtr     current_ammo(mod_start, { 0x00491DE0, 0x88,  0x160, 0x68,  0x40, 0x20, 0x18, 0x14 }); */
        /* MultiLvlPtr  reload_progress(mod_start, { 0x00491DE0, 0x88,  0x160, 0x68,  0x40, 0x54 }); */
        /* MultiLvlPtr  is_auto_aim(mod_start, { 0x00491DE0, 0x88,  0x160, 0x68,  0x40, 0x30, 0x154 }); */
        /* MultiLvlPtr  tank_controller(mod_start, { 0x00491DE0, 0x88,  0x160, 0x68,  0x40 }); */
        /* MultiLvlPtr  engine_state(mod_start, { 0x00491DE0, 0x88,  0x160, 0x68,  0x40, 0x37C }); */
        /* MultiLvlPtr  tank_controller_data(mod_start, { 0x00491DE0, 0x88,  0x160, 0x68,  0x40, 0x28 }); */

        /* std::cout << "press F8 to enable no cooldown mode" << std::endl; */
        std::cout << "press F9 to apply memory patch" << std::endl;

        bool enabled = false;

        while (true)
        {
            SHORT f9_key_state = GetAsyncKeyState(VK_F9);
            bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001);

            /* SHORT f8_key_state = GetAsyncKeyState(VK_F8); */
            /* bool f8_key_down = (f8_key_state & 0x8000) && (f8_key_state & 0x0001); */

            /* if (f8_key_down) */
            /* { */
            /*     no_cooldown_enabled = !no_cooldown_enabled; */

            /*     if (no_cooldown_enabled) */
            /*     { */
            /*         std::cout << "no cooldown mode enabled" << std::endl; */
            /*     } */
            /*     else */
            /*     { */
            /*         std::cout << "no cooldown mode disabled" << std::endl; */
            /*     } */
            /* } */

            if (f9_key_down)
            {
                /* if (enabled) */
                /* { */
                /*     handler.disable(); */
                /* } */
                /* else */
                /* { */
                /*     handler.enable(); */
                /* } */

                enabled = !enabled;

                /* try */
                /* { */
                /*     mem.set(health, 1000000.0f); */
                /*     mem.set(max_health, 1000000.0f); */
                /*     mem.set(max_speed, 1000.0f); */
                /*     mem.set(turret_speed, 1000.0f); */
                /*     mem.set(max_turret_speed, 1000.0f); */
                /*     mem.set(turn_speed, 50.0f); */
                /*     mem.set(max_turn_speed, 50.0f); */
                /*     mem.set(current_ammo, 1000.0f); */

                /*     std::cout << "patch applied successfully" << std::endl; */
                /* } */
                /* catch(std::exception &e) */
                /* { */
                /*     std::cout << "patch error" << std::endl; */
                /*     std::cout << e.what() << std::endl; */
                /* } */
            }

            /* if (no_cooldown_enabled) */
            /* { */
            /*     try */
            /*     { */
            /*         mem.set(reload_progress, 2.0f); */
            /*     } */
            /*     catch(std::exception &e) */
            /*     { */

            /*     } */
            /* } */

            std::this_thread::sleep_for(std::chrono::milliseconds(11));
        }
    }
    catch(std::exception &e)
    {
        std::cout << "error occured: " << e.what() << std::endl;
    }
    return 0;
}

