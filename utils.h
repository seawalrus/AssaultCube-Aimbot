#pragma once
#include <Windows.h>
class Utils
{
public:
	static DWORD GetProcessIdByName(const wchar_t* processName) noexcept;
	static uintptr_t GetModuleBase(HANDLE hProc, const wchar_t* name) noexcept;
	static float Round(float input) noexcept;
};

struct Vec3
{
	float x, y, z;
};

struct Rotate
{
	float pitch, yaw;
};

