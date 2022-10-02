#pragma once
#include "ERGBColor.h"
#include "EHitRecord.h"
#include"EMath.h"

namespace Elite
{
	
	class Material
	{
	public:
		virtual ~Material() = default;
		virtual RGBColor Shade(const HitRecord& hitrecord, const FVector3& w0, const FVector3& w1) const = 0;

	};

}