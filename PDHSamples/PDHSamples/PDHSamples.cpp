// This program needs only the essential Windows header files.
#define WIN32_LEAN_AND_MEAN 1

#include "PdhCommon.h"

#pragma comment(lib, "pdh.lib")

void main(void)
{
    wchar_t tag[128] = L"Process";
    std::vector<std::wstring> infos = EnumTagInfo(tag);
    printf("num = %d\n", infos.size());
    for (std::wstring info : infos)
    {
        wprintf(L"%s\n",info.c_str());
    }
}