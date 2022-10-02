#pragma once
#include "ELight.h"
namespace Elite
{

	class DirectionLight :public Light
	{
	public:
		DirectionLight(const FVector3& direction, const RGBColor& Lightcolor, const float lightIntesity);
		FVector3 GetDirection() const override;
		RGBColor CalculateLightOnPoint(const HitRecord& hitRecord) override;
		FVector3 CalculateDirection(const FPoint3& origin)  override;
		float CalculateDistance(FPoint3 origin) const override;
		void ToggleLight() override;
		TypeLight GetTypeLight() const override;
		bool IsLightOn() const override;


		~DirectionLight() = default;
	private:


		RGBColor m_LightColor;
		float  m_LightIntesity;
		FVector3 m_Direction;
		const TypeLight m_TypeLight;
		bool m_IsOn;
	

	};
}

