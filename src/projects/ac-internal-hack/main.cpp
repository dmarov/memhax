#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include <fstream>
#include <windows.h>
/* #include "modules/aob-signature-ptr.h" */
/* #include "modules/win-api-internal-process-memory-editor.h" */

/* void cheat_main() */
/* { */
/*     MessageBox(NULL, NULL, "Cheat is ready now", MB_OK); */
/* } */

/* DWORD WINAPI main_thread(LPVOID lpParam) */
/* { */
/*     cheat_main(); */
/*     return S_OK; */
/* } */


BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved)
{
            std::ofstream("C:\\Users\\mds3d\\ac-hack-test-success.txt");
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            MessageBox(NULL, NULL, "Cheat is ready now", MB_OK);
            /* CreateThread(0, 0x1000, &main_thread, 0, 0, NULL); */
            break;
        default:
            break;
    }

    return TRUE;
}
