#ifndef __PDHCOMMON_H__

#include <vector> 
#include <string>

std::vector<std::wstring> EnumObjects();

std::vector<std::wstring> EnumTagInfo(wchar_t tag[128]);

unsigned long GetProcessGpuUtilization(wchar_t process[256]);

unsigned long GetAllGpu3DUtilization();

#endif
