#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/instruction-nop-handler.h"
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

int main(int argc, char **argv)
{
    if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
    {
        return 1;
    }

    const wchar_t* exe = L"TankForce.exe";

    try
    {
        WinApiExternalProcessMemoryEditor editor(exe);

        auto regions = editor.getRegions();
        auto mod_span = editor.getModuleSpan(L"UnityPlayer.dll");

        AOBSignaturePtr player_z_ptr("89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? E9 ?? ?? ?? ?? 0F BA E6", 9, mod_span);
        AOBSignaturePtr ammo_ptr("E8 F3 0F 11 68 ?? 48 8B 7D ?? 4C 8B 7D", 1, regions);
        AOBSignaturePtr health_ptr_1("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 48 8B 7D ?? 48 8D 65 ?? 5D C3", 8, regions);
        AOBSignaturePtr health_ptr_2("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 45 85 FF 0F 8D", 8, regions);

        InstructionNopHandler player_z_handler(editor, player_z_ptr, 6);
        InstructionNopHandler ammo_handler(editor, ammo_ptr, 5);
        InstructionNopHandler health_handler_1(editor, health_ptr_1, 5);
        InstructionNopHandler health_handler_2(editor, health_ptr_2, 5);

        bool enabled_f8 = false;
        bool enabled_f9 = false;

        std::cout << "now cheat is ready" << std::endl;

        while (true)
        {
            if (interupted)
            {
                if (enabled_f8)
                {
                    player_z_handler.disable();
                }

                if (enabled_f9)
                {
                    ammo_handler.disable();
                    health_handler_1.disable();
                    health_handler_2.disable();
                }

                return 0;
            }

            SHORT f9_key_state = GetAsyncKeyState(VK_F9);
            bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001);

            SHORT f8_key_state = GetAsyncKeyState(VK_F8);
            bool f8_key_down = (f8_key_state & 0x8000) && (f8_key_state & 0x0001);

            if (f9_key_down)
            {
                if (enabled_f9)
                {
                    ammo_handler.disable();
                    health_handler_1.disable();
                    health_handler_2.disable();
                }
                else
                {
                    ammo_handler.enable();
                    health_handler_1.enable();
                    health_handler_2.enable();
                }

                enabled_f9 = !enabled_f9;
            }

            if (f8_key_down)
            {
                if (enabled_f8)
                {
                    player_z_handler.disable();
                }
                else
                {
                    player_z_handler.enable();
                }

                enabled_f8 = !enabled_f8;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(11));
        }
    }
    catch(std::exception &e)
    {
        std::cout << "error occured: " << e.what() << std::endl;
    }

    return 0;
}


// z
// 7FFAA4EDA4ED - 8B 41 30              - mov eax,[rcx+30]
// 7FFAA4EDA4F0 - 89 81 E0000000        - mov [rcx+000000E0],eax
// 7FFAA4EDA4F6 - 8B 41 34              - mov eax,[rcx+34]
// 7FFAA4EDA4F9 - 89 81 E4000000        - mov [rcx+000000E4],eax
// 7FFAA4EDA4FF - 8B 41 38              - mov eax,[rcx+38]
// 7FFAA4EDA502 - 89 81 E8000000        - mov [rcx+000000E8],eax
// 7FFAA4EDA508 - 8B 41 3C              - mov eax,[rcx+3C]
// 7FFAA4EDA50B - 89 81 EC000000        - mov [rcx+000000EC],eax
// 7FFAA4EDA511 - 8B 41 40              - mov eax,[rcx+40]
// 7FFAA4EDA514 - 89 81 F0000000        - mov [rcx+000000F0],eax <-------------x
// 7FFAA4EDA51A - 8B 41 44              - mov eax,[rcx+44]
// 7FFAA4EDA51D - 89 81 F4000000        - mov [rcx+000000F4],eax <-------------z
// 7FFAA4EDA523 - 8B 41 48              - mov eax,[rcx+48]
// 7FFAA4EDA526 - 89 81 F8000000        - mov [rcx+000000F8],eax <-------------y
// 7FFAA4EDA52C - E9 28040000           - jmp 7FFAA4EDA959
// 7FFAA4EDA531 - 0FBA E6 15            - bt esi,15
//
// UnityPlayer.dll + 10BA514
// 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? E9 ?? ?? ?? ?? 0FBA E6 ??
//                                                                                                                                                 == == == == == ==
//                                                                                                                                                                            -- -- -- -- -- --
//                                                                                                                      -- -- -- -- -- --
//                                                                                                                     |
