#include <Windows.h>
#include "Stalin.h"
#include <Drawing.h>
#include <fstream>


const wchar_t* VersionDescription = L"命令与征服：曙光之战vβ0.7.3稳定版，mod交流1群：559496825，作者：wen_rui1245、云游四海";


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
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
    DSurface::Composite->DrawText(VersionDescription, &location, COLOR_RED);
    return 0;
}