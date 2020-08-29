#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/multi-lvl-ptr.h"
#include "modules/win-api-process-memory-editor.h"
#include <winuser.h>
#include <bitset>

int main(int argc, char **argv)
{
    try {
        const wchar_t* exe = L"TankForce.exe";
        const wchar_t* module = L"mono-2.0-bdwgc.dll";

        WinApiProcessMemoryEditor mem(exe);

        auto [mod_start, mod_size] = mem.getModuleInfo(module);

        MultiLvlPtr           health(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x14 });
        MultiLvlPtr       max_health(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x18 });
        MultiLvlPtr        max_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x28, 0x18 });
        MultiLvlPtr     turret_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x14 });
        MultiLvlPtr max_turret_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x18 });
        MultiLvlPtr       turn_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x14 });
        MultiLvlPtr   max_turn_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x18 });
        MultiLvlPtr     current_ammo(mod_start, { 0x00491DE8, 0x90,  0xDF8, 0x68,  0x40, 0x20, 0x18, 0x14 });
        MultiLvlPtr  reload_progress(mod_start, { 0x00491DE8, 0x90,  0xDF8, 0x68,  0x40, 0x54 });

        std::cout << "press F8 to enable no cooldown mode" << std::endl;
        std::cout << "press F9 to apply memory patch" << std::endl;

        bool no_cooldown_enabled = false;

        while (true)
        {
            SHORT f9_key_state = GetAsyncKeyState(VK_F9);
            bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001);

            SHORT f8_key_state = GetAsyncKeyState(VK_F8);
            bool f8_key_down = (f8_key_state & 0x8000) && (f8_key_state & 0x0001);

            if (f8_key_down)
            {
                no_cooldown_enabled = !no_cooldown_enabled;

                if (no_cooldown_enabled)
                {
                    std::cout << "no cooldown mode enabled" << std::endl;
                }
                else
                {
                    std::cout << "no cooldown mode disabled" << std::endl;
                }
            }

            if (f9_key_down)
            {
                try
                {
                    mem.set(health, 1000000.0f);
                    mem.set(max_health, 1000000.0f);
                    mem.set(max_speed, 1000.0f);
                    mem.set(turret_speed, 1000.0f);
                    mem.set(max_turret_speed, 1000.0f);
                    mem.set(turn_speed, 50.0f);
                    mem.set(max_turn_speed, 50.0f);
                    mem.set(current_ammo, 1000.0f);

                    std::cout << "patch applied successfully" << std::endl;
                }
                catch(std::exception &e)
                {
                    std::cout << "patch error" << std::endl;
                    std::cout << e.what() << std::endl;
                }
            }

            if (no_cooldown_enabled)
            {
                try
                {
                    mem.set(reload_progress, 2.0f);
                }
                catch(std::exception &e)
                {

                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch(std::exception &e)
    {
        std::cout << "error occured" << std::endl;
        std::cout << e.what() << std::endl;
    }
    return 0;
}

