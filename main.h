#include <windows.h>
#include <TlHelp32.h>
#include <codecvt>
#include <random>

#define MEMBLOCK 4096
#define OPENFLAGS THREAD_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE
#define SNAPFLAGS TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32

namespace virtualmemory
{
	namespace virtual
	{
		template <class val>
		std::vector<DWORD> ScanProcessMemory(HANDLE ProcessHandle, std::vector<val> Value ,bool space, DWORD minimum, DWORD maximum) {
			std::vector<DWORD>Locations; bool break = false; INT64 mem; if (!space) { mem = 0; } else { mem = minimum; }
			if (!space) {
				MEMORY_BASIC_INFORMATION Memory;
				while (VirtualQueryEx(ProcessHandle, (LPVOID)mem, &Memory, sizeof(MEMORY_BASIC_INFORMATION))) {
					if (Memory.State == MEM_COMMIT) {
						std::vector<val> Buffer(Memory.RegionSize);
						if (ReadProcessMemory(ProcessHandle, (LPVOID)mem, &Buffer[0], Memory.RegionSize, 0)) {
							for (size_t i = 0; i < Buffer.size(); ++i) {
								for (auto f : Value) {
									if (Buffer[i] == f) {
										Locations.push_back(mem + ((i + 1) * sizeof(val)) - sizeof(val));
									}
								}
							}
						}
					} mem += Memory.RegionSize;
				}
			}
			else {
				while (mem < Max) {
					std::vector<val> Buffer(MEMBLOCK);
					if (ReadProcessMemory(ProcessHandle, (LPVOID)mem, &Buffer[0], MEMBLOCK, 0)) {
						for (size_t i = 0; i < Buffer.size(); ++i) {
							if (mem + ((i + 1) * sizeof(val)) - sizeof(val) > Max) {
								break = true;
								break;
							}
							for (auto f : Value) {
								if (Buffer[i] == f) {
									Locations.push_back(mem + ((i + 1) * sizeof(val)) - sizeof(val));
								}
							} if (break) { break; }
						}
					} if (break) { break; } mem += MEMBLOCK;
				}
			}return Locations;
		}
	}
}