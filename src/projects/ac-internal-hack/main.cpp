#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/aob-signature-ptr.h"
#include "modules/instruction-nop-cheat-handler.h"
#include "modules/win-api-internal-process-memory-editor.h"

/* void cheat_main() */
/* { */
/*     MessageBox(NULL, NULL, "Cheat is ready now", MB_OK); */
/* } */

/* DWORD WINAPI main_thread(LPVOID lpParam) */
/* { */
/*     cheat_main(); */
/*     return S_OK; */
/* } */


BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    OutputDebugString("HERE WE ARE");
    /* switch (fdwReason) */
    /* { */
    /*     case DLL_PROCESS_ATTACH: */
    /*         /1* MessageBox(NULL, NULL, "Cheat is ready now", MB_OK); *1/ */
    /*         /1* CreateThread(0, 0x1000, &main_thread, 0, 0, NULL); *1/ */
    /*         break; */
    /*     default: */
    /*         MessageBox(NULL, NULL, "Error", MB_OK); */
    /*         break; */
    /* } */

    return TRUE;
}
