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

        AOBSignaturePtr ammo_ptr("E8 F3 0F 11 68 ?? 48 8B 7D ?? 4C 8B 7D", 1, regions);
        AOBSignaturePtr health_ptr_1("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 48 8B 7D ?? 48 8D 65 ?? 5D C3", 8, regions);
        AOBSignaturePtr health_ptr_2("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 45 85 FF 0F 8D", 8, regions);

        InstructionNopCheatHandler ammo_handler(editor, ammo_ptr, 5);
        InstructionNopCheatHandler health_handler_1(editor, health_ptr_1, 5);
        InstructionNopCheatHandler health_handler_2(editor, health_ptr_2, 5);

        bool enabled = false;

        std::cout << "now cheat is ready" << std::endl;

        while (true)
        {
            if (interupted)
            {
                if (enabled)
                {
                    ammo_handler.disable();
                    health_handler_1.disable();
                    health_handler_2.disable();
                    return 0;
                }
            }

            SHORT f9_key_state = GetAsyncKeyState(VK_F9);
            bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001);

            if (f9_key_down)
            {
                if (enabled)
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

                enabled = !enabled;
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

