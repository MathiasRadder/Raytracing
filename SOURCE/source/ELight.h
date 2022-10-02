#pragma once
#include "Emath.h"
#include "EHitRecord.h"
namespace Elite
{
	enum class TypeLight
	{
		none,
		pointLight,
		directionalLight
	};
	class Light
	{
	public:
		virtual ~Light() = default;
		virtual FVector3 GetDirection() const = 0;
		virtual float CalculateDistance(FPoint3 origin) const = 0;
		virtual void ToggleLight() = 0;
		virtual bool IsLightOn() const  = 0;
		virtual TypeLight GetTypeLight() const = 0;
		virtual FVector3 CalculateDirection(const FPoint3& origin)  = 0;
		virtual RGBColor CalculateLightOnPoint(const HitRecord& hitRecord) = 0;

	};



}