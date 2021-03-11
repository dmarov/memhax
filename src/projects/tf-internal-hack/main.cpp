#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/instruction-nop-cheat-handler.h"
#include "modules/win-api-internal-process-memory-editor.h"

void cheat_main() {

    try
    {
        /* WinApiInternalProcessMemoryEditor editor; */

        /* auto regions = editor.getRegions(); */
        /* auto mod_span = editor.getModuleSpan(L"UnityPlayer.dll"); */

        /* AOBSignaturePtr player_z_ptr("89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? 8B 41 ?? 89 81 ?? ?? ?? ?? E9 ?? ?? ?? ?? 0F BA E6", 9, mod_span); */
        /* AOBSignaturePtr ammo_ptr("E8 F3 0F 11 68 ?? 48 8B 7D ?? 4C 8B 7D", 1, regions); */
        /* AOBSignaturePtr health_ptr_1("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 48 8B 7D ?? 48 8D 65 ?? 5D C3", 8, regions); */
        /* AOBSignaturePtr health_ptr_2("F3 0F 5A C0 F2 0F 5A E8 F3 0F 11 68 ?? 45 85 FF 0F 8D", 8, regions); */

        /* InstructionNopCheatHandler player_z_handler(editor, player_z_ptr, 6); */
        /* InstructionNopCheatHandler ammo_handler(editor, ammo_ptr, 5); */
        /* InstructionNopCheatHandler health_handler_1(editor, health_ptr_1, 5); */
        /* InstructionNopCheatHandler health_handler_2(editor, health_ptr_2, 5); */

        /* bool enabled_f8 = false; */
        /* bool enabled_f9 = false; */

        MessageBox(NULL, NULL, "Cheat is ready now", MB_OK | MB_ICONQUESTION);

        /* while (true) */
        /* { */
        /*     SHORT f9_key_state = GetAsyncKeyState(VK_F9); */
        /*     bool f9_key_down = (f9_key_state & 0x8000) && (f9_key_state & 0x0001); */

        /*     SHORT f8_key_state = GetAsyncKeyState(VK_F8); */
        /*     bool f8_key_down = (f8_key_state & 0x8000) && (f8_key_state & 0x0001); */

        /*     if (f9_key_down) */
        /*     { */
        /*         if (enabled_f9) */
        /*         { */
        /*             ammo_handler.disable(); */
        /*             health_handler_1.disable(); */
        /*             health_handler_2.disable(); */
        /*         } */
        /*         else */
        /*         { */
        /*             ammo_handler.enable(); */
        /*             health_handler_1.enable(); */
        /*             health_handler_2.enable(); */
        /*         } */

        /*         enabled_f9 = !enabled_f9; */
        /*     } */

        /*     if (f8_key_down) */
        /*     { */
        /*         if (enabled_f8) */
        /*         { */
        /*             player_z_handler.disable(); */
        /*         } */
        /*         else */
        /*         { */
        /*             player_z_handler.enable(); */
        /*         } */

        /*         enabled_f8 = !enabled_f8; */
        /*     } */

        /*     std::this_thread::sleep_for(std::chrono::milliseconds(11)); */
        /* } */
    }
    catch(std::exception &e)
    {
        MessageBox(NULL, NULL, "Error", MB_OK | MB_ICONQUESTION);
    }
}

DWORD WINAPI main_thread(LPVOID lpParam)
{
    cheat_main();
    return S_OK;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD _reason, LPVOID lpReserved)
{
    if (_reason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0x1000, &main_thread, 0, 0, NULL);
    }

    return 0;
}
