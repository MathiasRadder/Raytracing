#pragma once
#include <cstdint>
#include "Emath.h"
#include "ERGBColor.h"


namespace Elite
{
	struct HitRecord
	{
		FPoint3 hitPoint{};
		RGBColor color{0.f,0.f,0.f};
		float tValue{FLT_MAX};
		FVector3 normal{};
	};
	
}
