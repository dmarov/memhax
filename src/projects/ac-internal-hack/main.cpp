#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/instruction-nop-cheat-handler.h"
#include "modules/win-api-internal-process-memory-editor.h"

void cheat_main()
{

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
