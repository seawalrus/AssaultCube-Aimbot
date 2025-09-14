#include <iostream>
#define _USE_MATH_DEFINES 
#include <math.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <thread>
#include <vector>
#include "offsets.h"
#include "aimbot.h"
#include "utils.h"
//std::vector<uintptr_t>& EntityList();
int main()
{
	Aimbot::FindTarget();
	//EntityList();
}


/*
std::vector<uintptr_t>& EntityList() 
{
	std::vector<uintptr_t> Entities;
	Vec3 Enemy{};
	DWORD pid = Utils::GetProcessIdByName(L"ac_client.exe");
	if (pid == NULL)
		throw std::runtime_error("Could not get PID (EntityList)");

	HANDLE gameHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (gameHandle == nullptr)
		throw std::runtime_error("Could not open handle");

	uintptr_t BaseAddress = Utils::GetModuleBase(gameHandle, L"ac_client.exe");
	// "ac_client.exe"+00110D90 + 0x4 = Entitylist[0]
	for (int i = 0; i < 3; i++)
	{


		//GOOD
		uintptr_t EntityListBuffer;
		// Singular class
		uintptr_t Entity;
		uintptr_t EntityList = BaseAddress + offsets::EntityListStart;
		ReadProcessMemory(gameHandle, (void*)EntityList, &EntityListBuffer, sizeof(EntityListBuffer), NULL);
		// HERE ERROR
		auto EntityListOffset = EntityListBuffer;
		EntityListOffset = EntityListOffset + ( i * 0x4);

		
		ReadProcessMemory(gameHandle, (void*)EntityListOffset, &Entity, sizeof(EntityListBuffer), NULL);
		if (EntityListOffset == NULL)
			continue;

		Entities.push_back(Entity);

	}
	
	
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		for (int i = 0; i < Entities.capacity(); i++)
		{

			if (Entities[i] == NULL)
				continue;
			ReadProcessMemory(gameHandle, (void*)(Entities[i] + offsets::X), &Enemy.x, sizeof(Enemy), nullptr);
			ReadProcessMemory(gameHandle, (void*)(Entities[i] + offsets::Y), &Enemy.y, sizeof(Enemy), nullptr);
			ReadProcessMemory(gameHandle, (void*)(Entities[i] + offsets::Z), &Enemy.z, sizeof(Enemy), nullptr);
			//system("cls");
			std::cout << "\t\tEntity " << i << "\t\t" << (float)Enemy.x << "\t\t" << (float)Enemy.y << "\t\t" << (float)Enemy.z << std::endl;

		}
		return Entities;
}
*/