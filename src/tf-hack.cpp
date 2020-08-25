#include <chrono>
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

        uintptr_t base = mem.getModuleBaseAddr(module);
        unsigned short p_size = mem.getPointerSize();

        MultiLvlPtr           health(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x14 }, p_size);
        MultiLvlPtr       max_health(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x18 }, p_size);
        MultiLvlPtr        max_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x28, 0x18 }, p_size);
        MultiLvlPtr     turret_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x14 }, p_size);
        MultiLvlPtr max_turret_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x18 }, p_size);
        MultiLvlPtr       turn_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x14 }, p_size);
        MultiLvlPtr   max_turn_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x18 }, p_size);
        /* MultiLvlPtr     current_ammo(base, { 0x00491DE8, 0x90,  0xDF8, 0x68,  0x40, 0x20, 0x18, 0x14 }, p_size); */

        std::cout << "press F9 to apply memory patch" << std::endl;

        while (true)
        {
            SHORT key_state = GetAsyncKeyState(VK_F9);
            bool key_down = (key_state & 0x8000) && (key_state & 0x0001);

            if (key_down)
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
                    /* mem.set(current_ammo, 1000.0f); */
                    std::cout << "patch applied successfully" << std::endl;
                }
                catch(std::exception &e)
                {
                    std::cout << "patch error" << std::endl;
                    std::cout << e.what() << std::endl;
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

