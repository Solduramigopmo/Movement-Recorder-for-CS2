#include <Windows.h>
#include "../include/hooks.h"

DWORD WINAPI CustomInit(LPVOID lpParam) {
    Hooks::Initialize();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, LPTHREAD_START_ROUTINE(CustomInit), hModule, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        Hooks::Shutdown();
        break;
    }
    return TRUE;
}