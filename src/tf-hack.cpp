#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "modules/multi-lvl-ptr.h"


void setFloat(float value, MultiLvlPtr ptr)
{

}

int main(int argc, char **argv)
{

    const char* module = "mono-2.0-bdwgc.dll";

    MultiLvlPtr health(           module, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x14 });
    MultiLvlPtr max_health(       module, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x10, 0x18 });
    MultiLvlPtr max_speed(        module, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x28, 0x18 });
    MultiLvlPtr turret_speed(     module, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x14 });
    MultiLvlPtr max_turret_speed( module, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x30, 0x18 });
    MultiLvlPtr turn_speed(       module, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x14 });
    MultiLvlPtr max_turn_speed(   module, { 0x00663038, 0xB08, 0x1C0, 0x248, 0x48, 0x18, 0x40, 0x18 });
    MultiLvlPtr current_ammo(     module, { 0x00491DE8, 0x90, 0xDF8, 0x68, 0x40, 0x20, 0x18, 0x14 });

    setFloat(1000000, health);
    setFloat(1000000, max_health);
    setFloat(1000, max_speed);
    setFloat(1000, turret_speed);
    setFloat(1000, max_turret_speed);
    setFloat(1000, turn_speed);
    setFloat(1000, max_turn_speed);

    while (true) {
        setFloat(1000, current_ammo);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
