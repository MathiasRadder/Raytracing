#pragma once
#include "EMath.h"

namespace Elite
{
	struct Ray
	{
		FPoint3 origin{};
		FVector3 direction{};
		const float tMin{ 0.0001f };
		float tMax{ FLT_MAX };
	};
}
