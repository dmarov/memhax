#include <windows.h>
#include "modules/win-api-internal-process-memory-editor.h"
#include "loops/ac-hack-loop.h"

HANDLE th;

DWORD WINAPI main_loop(LPVOID lpParam)
{
    memhax::WinApiInternalProcessMemoryEditor editor;
    AcHackLoop loop(editor);
    loop.start();

    return 0;
}


BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            th = CreateThread(NULL, NULL, &main_loop, NULL, 0, NULL);
            break;
        default:
            break;
    }

    return TRUE;
}
