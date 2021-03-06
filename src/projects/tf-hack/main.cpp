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


// 7FFAA4E9945F - F3 0F11 5C 24 50      - movss [rsp+50],xmm3
// 7FFAA4E99465 - F3 0F10 50 04         - movss xmm2,[rax+04]
// 7FFAA4E9946A - F3 0F11 54 24 54      - movss [rsp+54],xmm2
// 7FFAA4E99470 - F3 0F10 48 08         - movss xmm1,[rax+08]
// 7FFAA4E99475 - F3 0F11 4C 24 58      - movss [rsp+58],xmm1
// 7FFAA4E9947B - F3 0F10 40 0C         - movss xmm0,[rax+0C]
// 7FFAA4E99480 - F3 0F11 44 24 5C      - movss [rsp+5C],xmm0
// 7FFAA4E99486 - F3 44 0F11 5C 24 60   - movss [rsp+60],xmm11
// 7FFAA4E9948D - F3 0F11 9E 40010000   - movss [rsi+00000140],xmm3
// 7FFAA4E99495 - F3 0F11 96 44010000   - movss [rsi+00000144],xmm2
// 7FFAA4E9949D - F3 0F11 8E 48010000   - movss [rsi+00000148],xmm1
// 7FFAA4E994A5 - F3 0F11 86 4C010000   - movss [rsi+0000014C],xmm0
// 7FFAA4E994AD - F3 44 0F11 9E 50010000  - movss [rsi+00000150],xmm11
// 7FFAA4E994B6 - F3 44 0F11 96 54010000  - movss [rsi+00000154],xmm10 <--------------
// 7FFAA4E994BF - F3 44 0F11 A6 58010000  - movss [rsi+00000158],xmm12
// 7FFAA4E994C8 - 8B 46 68              - mov eax,[rsi+68]
// 7FFAA4E994CB - C1 E8 1E              - shr eax,1E
// 7FFAA4E994CE - F3 44 0F11 54 24 64   - movss [rsp+64],xmm10
// 7FFAA4E994D5 - F3 44 0F11 64 24 68   - movss [rsp+68],xmm12
// 7FFAA4E994DC - 83 F8 03              - cmp eax,03
// 7FFAA4E994DF - 74 25                 - je 7FFAA4E99506



// 7FFAA4EDA4ED - 8B 41 30              - mov eax,[rcx+30]
// 7FFAA4EDA4F0 - 89 81 E0000000        - mov [rcx+000000E0],eax
// 7FFAA4EDA4F6 - 8B 41 34              - mov eax,[rcx+34]
// 7FFAA4EDA4F9 - 89 81 E4000000        - mov [rcx+000000E4],eax
// 7FFAA4EDA4FF - 8B 41 38              - mov eax,[rcx+38]
// 7FFAA4EDA502 - 89 81 E8000000        - mov [rcx+000000E8],eax
// 7FFAA4EDA508 - 8B 41 3C              - mov eax,[rcx+3C]
// 7FFAA4EDA50B - 89 81 EC000000        - mov [rcx+000000EC],eax
// 7FFAA4EDA511 - 8B 41 40              - mov eax,[rcx+40]
// 7FFAA4EDA514 - 89 81 F0000000        - mov [rcx+000000F0],eax
// 7FFAA4EDA51A - 8B 41 44              - mov eax,[rcx+44]
// 7FFAA4EDA51D - 89 81 F4000000        - mov [rcx+000000F4],eax
// 7FFAA4EDA523 - 8B 41 48              - mov eax,[rcx+48]
// 7FFAA4EDA526 - 89 81 F8000000        - mov [rcx+000000F8],eax
// 7FFAA4EDA52C - E9 28040000           - jmp 7FFAA4EDA959
// 7FFAA4EDA531 - 0FBA E6 15            - bt esi,15
// 7FFAA4EDA535 - 72 15                 - jb 7FFAA4EDA54C




















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

