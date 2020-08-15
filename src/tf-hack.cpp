#include <chrono>
#include <iostream>
#include <thread>
#include "modules/multi-lvl-ptr.h"
#include "modules/process-memory-editor.h"
#include "modules/win-api-process-memory-editor.h"

int main(int argc, char **argv)
{
    const wchar_t* exe = L"TankForce.exe";
    const wchar_t* module = L"mono-2.0-bdwgc.dll";

    WinApiProcessMemoryEditor mem(exe);

    uintptr_t base = mem.getModuleBaseAddr(module);

    MultiLvlPtr           health(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x14 });
    MultiLvlPtr       max_health(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x18 });
    MultiLvlPtr        max_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x28, 0x18 });
    MultiLvlPtr     turret_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x14 });
    MultiLvlPtr max_turret_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x18 });
    MultiLvlPtr       turn_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x14 });
    MultiLvlPtr   max_turn_speed(base, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x18 });
    MultiLvlPtr     current_ammo(base, { 0x00491DE8, 0x90,  0xDF8, 0x68,  0x40, 0x20, 0x18, 0x14 });

    try {
        mem.setFloat(health, 1000000);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    /* mem.setFloat(max_health, 1000000); */

    /* mem.setFloat(max_speed, 1000); */
    /* mem.setFloat(turret_speed, 1000); */
    /* mem.setFloat(max_turret_speed, 1000); */
    /* mem.setFloat(turn_speed, 1000); */
    /* mem.setFloat(max_turn_speed, 1000); */

    /* while (true) { */

    /*     try { */
    /*         mem.setFloat(current_ammo, 1000); */
    /*     } */
    /*     catch(std::exception &e) */
    /*     { */
    /*         std::cout << e.what() << std::endl; */
    /*     } */

    /*     std::this_thread::sleep_for(std::chrono::milliseconds(1000)); */
    /* } */

    return 0;
}
