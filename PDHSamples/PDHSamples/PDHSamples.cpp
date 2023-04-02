// This program needs only the essential Windows header files.
#define WIN32_LEAN_AND_MEAN 1

#include "PdhCommon.h"
#include <Windows.h>

#pragma comment(lib, "pdh.lib")

void main(void)
{
    while (true)
    {
        int utilization = GetAllGpu3DUtilization();
        printf("gpu utilization = %d\n", utilization);
        Sleep(1000);
    }
    //std::vector<std::wstring> objs = EnumObjects();
    //for (std::wstring info : objs)
    //{
    //    if (info.find(L"GPU") != std::wstring::npos)
    //    {
    //        wprintf(L"%s\n", info.c_str());

    //        wchar_t tag[128];
    //        wcscpy_s(tag, info.c_str());
    //        std::vector<std::wstring> infos = EnumTagInfo(tag);
    //        printf("num = %d\n", infos.size());
    //        for (std::wstring info : infos)
    //        {
    //            wprintf(L"%s\n", info.c_str());
    //        }
    //        printf("\n=======================================\n");

    //    }
    //}
    //wchar_t tag[128] = L"";
    //std::vector<std::wstring> infos = EnumTagInfo(tag);
    //printf("num = %d\n", infos.size());
    //for (std::wstring info : infos)
    //{
    //    wprintf(L"%s\n", info.c_str());
    //}



}