#include <Windows.h>
#include "Stalin.h"
#include <Drawing.h>
#include <fstream>
#include <functional>
#include <winnt.h>
#include <atomic>
#include <MixFileClass.h>
#include <Dbghelp.h>
#include <tlhelp32.h>
#include <Shlwapi.h>
#include <stdexcept>
#include <ShlObj_core.h>
#include <combaseapi.h>
#include <ShlGuid.h>
#include <strsafe.h>
#include <shellapi.h>
#include <memory.h>
#include <CCINIClass.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <map>
#include <commdlg.h>
#include <Unsorted.h>
#include <winternl.h>
#include <cwchar>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <filesystem>
#include <ntddkbd.h>
#include <GameClasses.h>
#include <YRpp.h>

bool AddressSpace = false;
const wchar_t* VersionDescription = L"命令与征服：曙光之战vβ0.7.5测试版，MOD交流群：1045447835，作者：wen_rui1245、云游四海";


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
MessageBoxW(NULL,
			L"欢迎您游玩曙光之战\n\n"
			L"MOD作者wen_rui1245，云游四海",
			L"MOD交流群：1045447835", MB_OK);
    ; case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
bool EnableLargeAddressSpace(DWORD processId) {
    HANDLE process = OpenProcess(PROCESS_SET_QUOTA, FALSE, processId);
    if (!process) {
        //        Debug::Log
        return false;
    }

    SIZE_T minWorkingSetSize = 0;
    SIZE_T maxWorkingSetSize = 0x7FFFFFFFFFFFFFFF;

    BOOL result = SetProcessWorkingSetSizeEx(process, minWorkingSetSize, maxWorkingSetSize, QUOTA_LIMITS_HARDWS_MIN_DISABLE);
    if (!result) {
        //      Debug::Log
        CloseHandle(process);
        return false;
    }
    //    Debug::Log
    CloseHandle(process);
    return true;
}

DEFINE_HOOK(0x4F4583, GScreenClass_DrawText, 0x6)
{
    auto wanted = Drawing::GetTextDimensions(VersionDescription, { 0,0 }, 0, 2, 0);

    RectangleStruct rect = {
        DSurface::Composite->GetWidth() - wanted.Width - 10,
        0,
        wanted.Width + 10,
        wanted.Height + 10
    };

    Point2D location{ rect.X + 5,5 };

    DSurface::Composite->FillRect(&rect, COLOR_BLACK);
    DSurface::Composite->DrawRect(&rect, COLOR_CYAN);
    DSurface::Composite->DrawText(VersionDescription, &location, COLOR_CYAN);
    return 0;
}

DEFINE_HOOK(0x52CAE9, _YR_PostGameInit, 0x5)
{
    if (AddressSpace)
    {
        DWORD processId = GetCurrentProcessId();
        EnableLargeAddressSpace(processId);
    }
    return 0;
}
DEFINE_HOOK(0x5FACDF, Settings, 0x5)
{
    CCINIClass* pINI_RULESMD = CCINIClass::LoadINIFile(GameStrings::RULESMD_INI);
    AddressSpace = pINI_RULESMD->ReadBool("General", "WWSBImprove", false);
    CCINIClass::UnloadINIFile(pINI_RULESMD);
    return 0;
}
