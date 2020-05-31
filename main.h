#include <windows.h>
#include <TlHelp32.h>
#include <codecvt>
#include <random>

namespace virtualmemory
{
    namespace memoryScanner
    {
        std::vector<DWORD> ScanProcessMemory(HANDLE ProcessHandle, int Value, bool space, DWORD minimum, DWORD maximum) 
        {
            std::vector<DWORD>Locations; 
            bool breakop = false; 
            INT64 mem; 
            MEMORY_BASIC_INFORMATION Memory;
            
            if (!space) 
            { 
            mem = 0; 
            }
            else 
            { 
            mem = minimum; 
            }
            
            if (!space) {
                while (VirtualQueryEx(ProcessHandle, (LPVOID)mem, &mem, sizeof(MEMORY_BASIC_INFORMATION))) {
                    if (mem.State == MEM_COMMIT) 
                    {
                        std::vector<val> Buffer(Memory.RegionSize);
                        if (ReadProcessMemory(ProcessHandle, (LPVOID)mem, &Buffer[0], mem.RegionSize, 0)) 
                        {
                            for (size_t i = 0; i < Buffer.size(); ++i) 
                            {
                                for (int f : ) {
                                    if (Buffer[i] == f) 
                                    {
                                        Locations.push_back(mem + ((i + 1) * sizeof(val)) - sizeof(val));
                                    }
                                }
                            }
                        }
                    } 
                    mem += mem.RegionSize;
                }
            }
            else 
            {
                while (mem < maximum) 
                {
                    std::vector<val> Buffer(4096);
                    if (ReadProcessMemory(ProcessHandle, (LPVOID)mem, &Buffer[0], 4096, 0)) 
                    {
                        for (size_t i = 0; i < Buffer.size(); ++i) 
                        {
                            if (mem + ((i + 1) * sizeof(val)) - sizeof(val) > maximum) 
                            {
                                breakop = true;
                                break;
                            }
                            for (int f : Value) 
                            {
                                if (Buffer[i] == f) 
                                {
                                    Locations.push_back(mem + ((i + 1) * sizeof(val)) - sizeof(val));
                                }
                            } 
                            if (breakop) 
                            { 
                                break; 
                            }
                        }
                    } 
                    if (breakop) 
                    { 
                        break; 
                    } 
                    mem += 4096;
                }
            }
            return Locations;
        }
    }
}
