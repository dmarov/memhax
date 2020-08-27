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

        auto [mod_start, mod_size] = mem.getModuleInfo(module);

        std::cout << std::hex << mod_start << std::endl;
        std::cout << mod_size << std::endl;

        SignatureConfig cfg("\0x01\0x02\0x03\0x04", "x?xxx", 2, mod_start, mod_size);

        uintptr_t addr = mem.getRegularPointer(cfg);

        std::cout << addr << std::endl;


        /* MultiLvlPtr           health(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x14 }); */
        /* MultiLvlPtr       max_health(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x18 }); */
        /* MultiLvlPtr        max_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x28, 0x18 }); */
        /* MultiLvlPtr     turret_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x14 }); */
        /* MultiLvlPtr max_turret_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x18 }); */
        /* MultiLvlPtr       turn_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x14 }); */
        /* MultiLvlPtr   max_turn_speed(mod_start, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x18 }); */
        /* /1* MultiLvlPtr     current_ammo(mod_start, { 0x00491DE8, 0x90,  0xDF8, 0x68,  0x40, 0x20, 0x18, 0x14 }, p_size); *1/ */

        /* std::cout << "press F9 to apply memory patch" << std::endl; */

        /* while (true) */
        /* { */
        /*     SHORT key_state = GetAsyncKeyState(VK_F9); */
        /*     bool key_down = (key_state & 0x8000) && (key_state & 0x0001); */

        /*     if (key_down) */
        /*     { */
        /*         try */
        /*         { */
        /*             mem.set(health, 1000000.0f); */
        /*             mem.set(max_health, 1000000.0f); */
        /*             mem.set(max_speed, 1000.0f); */
        /*             mem.set(turret_speed, 1000.0f); */
        /*             mem.set(max_turret_speed, 1000.0f); */
        /*             mem.set(turn_speed, 50.0f); */
        /*             mem.set(max_turn_speed, 50.0f); */
        /*             /1* mem.set(current_ammo, 1000.0f); *1/ */
        /*             std::cout << "patch applied successfully" << std::endl; */
        /*         } */
        /*         catch(std::exception &e) */
        /*         { */
        /*             std::cout << "patch error" << std::endl; */
        /*             std::cout << e.what() << std::endl; */
        /*         } */
        /*     } */

        /*     std::this_thread::sleep_for(std::chrono::milliseconds(10)); */
        /* } */
    }
    catch(std::exception &e)
    {
        std::cout << "error occured" << std::endl;
        std::cout << e.what() << std::endl;
    }
    return 0;
}

