#include "Memory.hpp"
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

namespace CodeRed::Memory
{
    uintptr_t GetEntryPoint()
    {
        return reinterpret_cast<uintptr_t>(GetModuleHandleW(NULL));
    }

    std::string GetProcessName()
    {
        char fileName[MAX_PATH];
        GetModuleFileNameA(nullptr, fileName, MAX_PATH);
        std::filesystem::path filePath(fileName);

        if (std::filesystem::exists(filePath) && filePath.has_extension())
        {
            return filePath.stem().string();
        }

        return "";
    }

    uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask)
    {
        MODULEINFO info = { };
        GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

        uintptr_t start = reinterpret_cast<uintptr_t>(module);
        size_t length = info.SizeOfImage;

        size_t pos = 0;
        size_t maskLength = (std::strlen(mask) - 1);

        for (uintptr_t retAddress = start; retAddress < start + length; retAddress++)
        {
            if (*reinterpret_cast<unsigned char*>(retAddress) == pattern[pos] || mask[pos] == '?')
            {
                if (pos == maskLength)
                {
                    return (retAddress - maskLength);
                }

                pos++;
            }
            else
            {
                retAddress -= pos;
                pos = 0;
            }
        }

        return NULL;
    }
}