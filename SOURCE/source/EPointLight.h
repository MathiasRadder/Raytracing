#pragma once
#include "ELight.h"
#include "ERGBColor.h"
namespace Elite
{

	class PointLight : public Light
	{
	public:
		PointLight(const FVector3& position, const RGBColor& Lightcolor, const float lightIntesity);
		FVector3 GetDirection() const override;
		FVector3 CalculateDirection(const FPoint3& origin)  override;
		RGBColor CalculateLightOnPoint(const HitRecord& hitRecord) override;
		void ToggleLight() override;
		TypeLight GetTypeLight() const override;
		bool IsLightOn() const override;
		float CalculateDistance(FPoint3 origin) const override  ;
		~PointLight() = default;
	private:

		//FVector3 m_Position;
		FPoint3 m_Position;
		RGBColor m_LightColor;
		float  m_LightIntesity;
		FVector3 m_Direction;
		const float m_RadiantFlux;//maybe delete
		const TypeLight m_TypeLight;
		bool m_IsOn;
	};
}

