#include "EPointLight.h"

Elite::PointLight::PointLight(const FVector3& position, const RGBColor& lightColor, const float lightIntesity):
	m_Position{position},
	m_LightColor{lightColor/**255.f*/},
	m_LightIntesity{lightIntesity},
	m_RadiantFlux{ (4.f * static_cast<float>(E_PI))* lightIntesity },//not used
	m_Direction{ GetNormalized(position) },
	m_TypeLight{TypeLight::pointLight},
	m_IsOn{true}
{

}

Elite::FVector3 Elite::PointLight::GetDirection() const
{
	return m_Direction;
}



Elite::FVector3 Elite::PointLight::CalculateDirection(const FPoint3& origin)
{
	return  GetNormalized(FVector3( m_Position) - FVector3{ origin.x,origin.y,origin.z });
	//return  (FVector3(m_Position) - FVector3{ origin.x,origin.y,origin.z });
}

Elite::RGBColor Elite::PointLight::CalculateLightOnPoint(const HitRecord& hitRecord)
{

		return (m_LightColor  * m_LightIntesity / SqrDistance(m_Position, hitRecord.hitPoint));
		


}

void Elite::PointLight::ToggleLight()
{
	m_IsOn = !m_IsOn;
}

Elite::TypeLight Elite::PointLight::GetTypeLight() const
{
	return m_TypeLight;
}

bool Elite::PointLight::IsLightOn() const
{
	return m_IsOn;
}

float Elite::PointLight::CalculateDistance(FPoint3 origin) const
{
	return Distance(m_Position, origin);
}
