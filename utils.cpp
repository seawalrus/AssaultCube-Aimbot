#include "utils.h"
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
DWORD Utils::GetProcessIdByName(const wchar_t* processName) noexcept {
	DWORD processes[1024], processCount;
	if (!EnumProcesses(processes, sizeof(processes), &processCount)) return 0;

	processCount /= sizeof(DWORD);
	for (size_t i = 0; i < processCount; i++) {
		if (processes[i] == 0) continue;

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		if (hProcess) {
			WCHAR exeName[MAX_PATH];
			if (GetModuleBaseNameW(hProcess, NULL, exeName, MAX_PATH) > 0) {
				if (_wcsicmp(exeName, processName) == 0) {
					CloseHandle(hProcess);
					return processes[i];
				}
			}
			CloseHandle(hProcess);
		}
	}
	return 0;
}
uintptr_t Utils::GetModuleBase(HANDLE hProc, const wchar_t* name) noexcept {
	HMODULE mods[1024];
	DWORD cbNeeded;
	if (EnumProcessModulesEx(hProc, mods, sizeof(mods), &cbNeeded,
		LIST_MODULES_ALL)) {
		for (DWORD i = 0; i < cbNeeded / sizeof(HMODULE); ++i) {
			wchar_t buf[MAX_PATH];
			if (GetModuleBaseNameW(hProc, mods[i], buf, _countof(buf)) &&
				!_wcsicmp(buf, name)) {
				return (uintptr_t)mods[i];
			}
		}
	}
	return 0;
}

float Utils::Round(float input) noexcept {
	return std::round(input * 100) / 100;
}
