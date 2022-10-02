#include "EDirectionLight.h"

Elite::DirectionLight::DirectionLight(const FVector3& direction,  const RGBColor& Lightcolor, const float lightIntesity):
    m_Direction{ direction },
    m_LightColor{ Lightcolor },
    m_LightIntesity{ lightIntesity },
    m_TypeLight{TypeLight::directionalLight},
    m_IsOn{true}

        
{
}

Elite::FVector3 Elite::DirectionLight::GetDirection() const
{
    return m_Direction;
}

//Elite::FVector3 Elite::DirectionLight::GetPosition() const
//{
//    return FVector3(0.f,0.f,0.f);
//}
void Elite::DirectionLight::ToggleLight()
{
    m_IsOn = !m_IsOn;
}
Elite::TypeLight Elite::DirectionLight::GetTypeLight() const
{
    return m_TypeLight;
}
bool Elite::DirectionLight::IsLightOn() const
{
    return m_IsOn;
}
Elite::RGBColor Elite::DirectionLight::CalculateLightOnPoint(const HitRecord& hitRecord)
{
    

        return m_LightColor * m_LightIntesity;
}

Elite::FVector3 Elite::DirectionLight::CalculateDirection(const FPoint3& )
{
    return  -m_Direction;
}

float Elite::DirectionLight::CalculateDistance(FPoint3 origin) const
{
    return FLT_MAX;
}