#pragma once
#include "utils.h"
#include <vector>
class Aimbot
{
private:
	static std::vector<uintptr_t> EntityList();
	static Rotate CalculateAngle(Vec3 player, Vec3 enemy) noexcept;
public:
	static void FindTarget();
};