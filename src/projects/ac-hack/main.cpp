#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include "modules/multi-lvl-ptr.h"
#include "modules/win-api-process-memory-editor.h"
#include <winuser.h>
#include <bitset>

// health
//ac_client.exe+2CA45 - 0F84 E8160000         - je ac_client.exe+2E133
//ac_client.exe+2CA4B - 8B 4C 24 28           - mov ecx,[esp+28]
//ac_client.exe+2CA4F - 83 7C 24 1C 1B        - cmp dword ptr [esp+1C],1B
//ac_client.exe+2CA54 - 8B 44 24 2C           - mov eax,[esp+2C]
//ac_client.exe+2CA58 - 89 8A FC000000        - mov [edx+000000FC],ecx
//ac_client.exe+2CA5E - 89 82 F8000000        - mov [edx+000000F8],eax  <<<
//ac_client.exe+2CA64 - 0F94 C1               - sete cl

//                              |                                                     |
// 8B 4C 24 ?? 83 7C 24 ?? ?? 8B 44 24 ?? 89 8A ?? ?? ?? ?? 89 82 ?? ?? ?? ?? 0F 94 C1
//                                                          == == == == == ==

//                                       |                                            |
// 8B 4C 24 ?? 83 7C 24 ?? ?? 8B 44 24 ?? 89 8A ?? ?? ?? ?? 89 82 ?? ?? ?? ?? 0F 94 C1
//                                                          == == == == == ==

int main(int argc, char **argv)
{
    const wchar_t* exe = L"ac_client.exe";
    const wchar_t* module = L"ac_client.exe";
    const char* sig = "\x89\x8A\x00\x00\x00\x00\x89\x82\x00\x00\x00\x00\x0F\x94\xC1";
    const char* mask = "xx????xx????xxx";

    WinApiProcessMemoryEditor mem(exe);

    auto [mod_start, mod_size] = mem.getModuleInfo(module);

    return 0;
}

