#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/instruction-nop-cheat-handler.h"
#include "modules/win-api-external-process-memory-editor.h"
#include <tuple>
#include <winuser.h>
#include <bitset>
#include <stdio.h> 

bool interupted = false;

BOOL WINAPI consoleHandler(DWORD signal) {

    if (signal == CTRL_C_EVENT)
    {
        interupted = true;
    }

    return TRUE;
}

// 1F6917E8268 - 41 FF D3              - call r11
// 1F6917E826B - F3 0F5A C0            - cvtss2sd xmm0,xmm0
// 1F6917E826F - 48 8B 45 E0           - mov rax,[rbp-20]
// 1F6917E8273 - F2 0F5A E8            - cvtsd2ss xmm5,xmm0
// 1F6917E8277 - F3 0F11 6D F4         - movss [rbp-0C],xmm5
// 1F6917E827C - 83 38 00              - cmp dword ptr [rax],00
// 1F6917E827F - F3 0F10 45 F4         - movss xmm0,[rbp-0C]
// 1F6917E8284 - F3 0F5A C0            - cvtss2sd xmm0,xmm0
// 1F6917E8288 - F2 0F5A E8            - cvtsd2ss xmm5,xmm0
// 1F6917E828C - F3 0F11 68 14         - movss [rax+14],xmm5 <--------
// 1F6917E8291 - 48 8B 7D F8           - mov rdi,[rbp-08]
// 1F6917E8295 - 48 8D 65 00           - lea rsp,[rbp+00]
// 1F6917E8299 - 5D                    - pop rbp
// 1F6917E829A - C3                    - ret 
// 
// 
// 
// 
// 1F6917CC0CB - 48 8B D8              - mov rbx,rax
// 1F6917CC0CE - F3 41 0F2A C6         - cvtsi2ss xmm0,r14d
// 1F6917CC0D3 - F3 0F5A C0            - cvtss2sd xmm0,xmm0
// 1F6917CC0D7 - F2 0F5A E8            - cvtsd2ss xmm5,xmm0
// 1F6917CC0DB - F3 0F11 6D CC         - movss [rbp-34],xmm5
// 1F6917CC0E0 - 83 38 00              - cmp dword ptr [rax],00
// 1F6917CC0E3 - F3 0F10 45 CC         - movss xmm0,[rbp-34]
// 1F6917CC0E8 - F3 0F5A C0            - cvtss2sd xmm0,xmm0
// 1F6917CC0EC - F2 0F5A E8            - cvtsd2ss xmm5,xmm0
// 1F6917CC0F0 - F3 0F11 68 14         - movss [rax+14],xmm5 < -------
// 1F6917CC0F5 - 45 85 FF              - test r15d,r15d
// 1F6917CC0F8 - 0F8D 5D000000         - jnl 1F6917CC15B




int main(int argc, char **argv)
{
    try {

        const wchar_t* exe = L"TankForce.exe";
        WinApiExternalProcessMemoryEditor editor(exe);

        auto modules = editor.getModules();
        auto regions = editor.getRegions();

        AOBSignaturePtr ammo_ptr("F3 0F 11 68 ?? 48 8B 7D ?? 4C 8B 7D ?? 48 8D 65 ?? 5D C3", 0, regions);
 /* AOBSignaturePtr health_ptr_1("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 48 8B 7D ?? 48 8D 65 ?? 5D C3", 8, regions); */
 /* AOBSignaturePtr health_ptr_2("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 45 85 FF 0F 8D", 8, regions); */

// F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 48 8B 7D
//                         == == == == ==

// F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 45 85 FF 0F 8D
//                         == == == == ==
        InstructionNopCheatHandler ammo_handler(editor, ammo_ptr, 5);

        if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
        {
            throw std::exception("failed to set console handler");
        }

        /* InstructionNopCheatHandler health_handler_1(editor, health_ptr_1, 5); */
        /* InstructionNopCheatHandler health_handler_2(editor, health_ptr_2, 5); */

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
        /* std::cout << "press F9 to apply memory patch" << std::endl; */

        bool enabled = false;

        while (true)
        {
            if (interupted)
            {
                if (enabled)
                {
                    ammo_handler.disable();
                    return 0;
                }
            }

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
                if (enabled)
                {
                    ammo_handler.disable();
                    /* health_handler_1.disable(); */
                    /* health_handler_2.disable(); */
                }
                else
                {
                    ammo_handler.enable();
                    /* health_handler_1.enable(); */
                    /* health_handler_2.enable(); */
                }

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

