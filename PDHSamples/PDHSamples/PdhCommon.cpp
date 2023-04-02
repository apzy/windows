#include "PdhCommon.h"

#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <conio.h>

CONST ULONG SAMPLE_INTERVAL_MS = 10;
CONST PWSTR BROWSE_DIALOG_CAPTION = (const PWSTR)L"Select a counter to monitor.";

std::vector<std::wstring> EnumObjects()
{
    std::vector<std::wstring> objs;
    PDH_STATUS status = ERROR_SUCCESS;
    LPWSTR pwsCounterListBuffer = NULL;
    DWORD dwCounterListSize = 0;
    LPWSTR pTemp = NULL;

    status = PdhEnumObjects(
        NULL,
        NULL,
        pwsCounterListBuffer,
        &dwCounterListSize,
        PERF_DETAIL_WIZARD,
        TRUE);

    if (status == PDH_MORE_DATA)
    {
        pwsCounterListBuffer = (LPWSTR)malloc(dwCounterListSize * sizeof(WCHAR));
        if (pwsCounterListBuffer != NULL)
        {
            status = PdhEnumObjects(
                NULL,
                NULL,
                pwsCounterListBuffer,
                &dwCounterListSize,
                PERF_DETAIL_WIZARD,
                FALSE);
            if (status == ERROR_SUCCESS)
            {
                for (pTemp = pwsCounterListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1)
                {
                    std::wstring obj = pTemp;
                    objs.push_back(obj);
                }

            }
        }
    }

    return objs;
}

std::vector<std::wstring> EnumTagInfo(wchar_t tag[128])
{
    std::vector<std::wstring> infos;

    PDH_STATUS status = ERROR_SUCCESS;
    LPWSTR pwsCounterListBuffer = NULL;
    DWORD dwCounterListSize = 0;
    LPWSTR pwsInstanceListBuffer = NULL;
    DWORD dwInstanceListSize = 0;
    LPWSTR pTemp = NULL;

    // Determine the required buffer size for the data. 
    status = PdhEnumObjectItems(
        NULL,                   // real-time source
        NULL,                   // local machine
        tag,         // object to enumerate
        pwsCounterListBuffer,   // pass NULL and 0
        &dwCounterListSize,     // to get required buffer size
        pwsInstanceListBuffer,
        &dwInstanceListSize,
        PERF_DETAIL_WIZARD,     // counter detail level
        0);

    if (status == PDH_MORE_DATA)
    {
        // Allocate the buffers and try the call again.
        pwsCounterListBuffer = (LPWSTR)malloc(dwCounterListSize * sizeof(WCHAR));
        pwsInstanceListBuffer = (LPWSTR)malloc(dwInstanceListSize * sizeof(WCHAR));

        if (NULL != pwsCounterListBuffer && NULL != pwsInstanceListBuffer)
        {
            status = PdhEnumObjectItems(
                NULL,                   // real-time source
                NULL,                   // local machine
                tag,         // object to enumerate
                pwsCounterListBuffer,
                &dwCounterListSize,
                pwsInstanceListBuffer,
                &dwInstanceListSize,
                PERF_DETAIL_WIZARD,     // counter detail level
                0);

            if (status == ERROR_SUCCESS)
            {
                for (pTemp = pwsInstanceListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1)
                {
                    std::wstring info = pTemp;
                    infos.push_back(info);
                }
            }
            else
            {
                wprintf(L"Second PdhEnumObjectItems failed with %0x%x.\n", status);
            }
        }
        else
        {
            wprintf(L"Unable to allocate buffers.\n");
            status = ERROR_OUTOFMEMORY;
        }
    }
    else
    {
        wprintf(L"\nPdhEnumObjectItems failed with 0x%x.\n", status);
    }

    if (pwsCounterListBuffer != NULL)
        free(pwsCounterListBuffer);

    if (pwsInstanceListBuffer != NULL)
        free(pwsInstanceListBuffer);

    return infos;
}

unsigned long GetProcessGpuUtilization(wchar_t process[256])
{
    return 0;
}

unsigned long GetAllGpu3DUtilization()
{
    PDH_STATUS status;
    HQUERY query = NULL;
    HCOUNTER counter;
    DWORD counterType;
    PDH_FMT_COUNTERVALUE displayValue;

    double utilization = 0;

    status = PdhOpenQuery(NULL, NULL, &query);

    std::vector<std::wstring> infos = EnumTagInfo((wchar_t*)L"GPU Engine");
    for (std::wstring info : infos)
    {
        if (info.find(L"engtype_3D") != std::wstring::npos)
        {
            std::wstring counterPath = L"\\GPU Engine(" + info + L")\\Utilization Percentage";
            status = PdhAddCounter(query, counterPath.c_str(), 0, &counter);
            status = PdhCollectQueryData(query);
            if (status != ERROR_SUCCESS)
            {
                wprintf(L"\nPdhCollectqueryData failed with 0x%x.\n", status);
            }
            Sleep(SAMPLE_INTERVAL_MS);
            status = PdhCollectQueryData(query);
            if (status != ERROR_SUCCESS)
            {
                wprintf(L"\nPdhCollectqueryData failed with 0x%x.\n", status);
            }
            status = PdhGetFormattedCounterValue(counter,
                PDH_FMT_DOUBLE,
                &counterType,
                &displayValue);
            if (status != ERROR_SUCCESS)
            {
                wprintf(L"\nPdhGetFormattedCounterValue failed with status 0x%x.", status);
            }
            if (displayValue.doubleValue > 1)
            {
                wprintf(L",\"%.20g\"\n", displayValue.doubleValue);
            }
            utilization += displayValue.doubleValue;
        }
    }
    if (query)
    {
        PdhCloseQuery(query);
    }
    return utilization;
}

