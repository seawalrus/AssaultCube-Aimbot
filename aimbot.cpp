#define _USE_MATH_DEFINES 
#include <Windows.h>
#include <Psapi.h>
#include <stdexcept>
#include <thread>
#include "offsets.h"
#include "aimbot.h"
#include <iostream>
#include <vector>

class Entity {
private:
	uintptr_t EntityBase;

	HANDLE _gameHandle;
	uintptr_t _moduleBase;


	int Health;
	float X, Y, Z;
public:


	void Read() {
		ReadProcessMemory(_gameHandle, (void*)(_moduleBase + offsets::Health), &Health, sizeof(Health), nullptr);
		ReadProcessMemory(_gameHandle, (void*)(_moduleBase + offsets::X), &Health, sizeof(X), nullptr);
		ReadProcessMemory(_gameHandle, (void*)(_moduleBase + offsets::Y), &Health, sizeof(Y), nullptr);
		ReadProcessMemory(_gameHandle, (void*)(_moduleBase + offsets::Z), &Health, sizeof(Z), nullptr);
	}
	const float GetX() const { return this->X; }
	const float GetY() const { return this->Y; }
	const float GetZ() const { return this->Z; }
	const int GetHealth() const { return this->Health; }

};

std::vector<uintptr_t> Aimbot::EntityList()
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
	int PlayerCount;
	ReadProcessMemory(gameHandle, (void*)(BaseAddress + offsets::PlayerCount), &PlayerCount, sizeof(PlayerCount), nullptr);
	for (int i = 0; i < PlayerCount; i++)
	{



		uintptr_t EntityListBuffer;

		uintptr_t Entity;
		uintptr_t EntityList = BaseAddress + offsets::EntityListStart;
		ReadProcessMemory(gameHandle, (void*)EntityList, &EntityListBuffer, sizeof(EntityListBuffer), NULL);

		auto EntityListOffset = EntityListBuffer;
		EntityListOffset = EntityListOffset + (i * 0x4);


		ReadProcessMemory(gameHandle, (void*)EntityListOffset, &Entity, sizeof(EntityListBuffer), NULL);
		if (EntityListOffset == NULL)
			continue;

		Entities.push_back(Entity);


	}
	CloseHandle(gameHandle);

	return Entities;
}




Rotate Aimbot::CalculateAngle(Vec3 player, Vec3 enemy) noexcept
{
	Vec3 angles{};
	Vec3 delta{};
	Rotate rotate{};
	// Player normalized to 0,0,0
	delta.x = player.x - enemy.x;
	delta.y = player.y - enemy.y;
	delta.z = player.z - enemy.z;

	float hypotnuse = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	//float hypotnuse = sqrt(delta.x * delta.x + delta.y * delta.y);
	rotate.yaw = (atan2f(delta.y, delta.x));
	rotate.yaw *= (180 / M_PI); //+ 180;
	// facing 180 away from target (+= 90)
	rotate.yaw += 270.0f;

	// PITCH

	rotate.pitch = asinf(delta.z / hypotnuse);
	rotate.pitch *= (180 / M_PI);
	//rotate.pitch = enemy.z;
	if (player.z > enemy.z)
	{
		rotate.pitch *= -1;
	}
	if (player.z < enemy.z)
	{
		rotate.pitch *= -1;
	}
	return rotate;
}


void Aimbot::FindTarget()
{


	Vec3 player{};

	Vec3 enemy{};


	std::vector<uintptr_t> Entities = EntityList();
	DWORD pid = Utils::GetProcessIdByName(L"ac_client.exe");
	if (pid == 0)
		throw std::runtime_error("Failed to get PID");



	HANDLE gameHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (gameHandle == NULL)
		throw std::runtime_error("Failed to get game handle");


	uintptr_t baseaddress = Utils::GetModuleBase(gameHandle, L"ac_client.exe");
	if (baseaddress == NULL)
		throw std::runtime_error("Failed to get base address");
	if (sizeof(Entities) == 0)
		throw std::runtime_error("Entity list is null ");

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (GetAsyncKeyState(VK_RBUTTON))
		{



			for (auto& Entity : Entities)
			{



				int enemyHealth;
				ReadProcessMemory(gameHandle, (void*)(Entity + offsets::Health), &enemyHealth, sizeof(enemyHealth), nullptr);
				if (enemyHealth == 0)
					continue;
				Rotate result = Aimbot::CalculateAngle(player, enemy);

				ReadProcessMemory(gameHandle, (void*)(Entity + offsets::X), &enemy.x, sizeof(uintptr_t), nullptr);
				ReadProcessMemory(gameHandle, (void*)(Entity + offsets::Y), &enemy.y, sizeof(uintptr_t), nullptr);
				ReadProcessMemory(gameHandle, (void*)(Entity + offsets::Z), &enemy.z, sizeof(uintptr_t), nullptr);

				uintptr_t playerbuffer;
				ReadProcessMemory(gameHandle, (void*)(baseaddress + offsets::localpawn), &playerbuffer, sizeof(playerbuffer), nullptr);

				// Local X Y Z 
				ReadProcessMemory(gameHandle, (void*)(playerbuffer + offsets::X), &player.x, sizeof(playerbuffer), nullptr);
				ReadProcessMemory(gameHandle, (void*)(playerbuffer + offsets::Y), &player.y, sizeof(playerbuffer), nullptr);
				ReadProcessMemory(gameHandle, (void*)(playerbuffer + offsets::Z), &player.z, sizeof(playerbuffer), nullptr);

				float Player_Yaw;
				ReadProcessMemory(gameHandle, (void*)(playerbuffer + offsets::Yaw), &Player_Yaw, sizeof(playerbuffer), nullptr);

				float Player_Pitch;
				ReadProcessMemory(gameHandle, (void*)(playerbuffer + offsets::Pitch), &Player_Pitch, sizeof(playerbuffer), nullptr);
				std::cout << "\rEnemy: \tX" << Utils::Round(enemy.x) << "\tY" << Utils::Round(enemy.y) << "\tZ" << Utils::Round(enemy.z);

				WriteProcessMemory(gameHandle, (void*)(playerbuffer + offsets::Yaw), &result.yaw, sizeof(playerbuffer), nullptr);
				WriteProcessMemory(gameHandle, (void*)(playerbuffer + offsets::Pitch), &result.pitch, sizeof(playerbuffer), nullptr);


			}
		}
	}
}
