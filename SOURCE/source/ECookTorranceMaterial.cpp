#include "ECookTorranceMaterial.h"
#include <iostream>

Elite::CookTorranceMaterial::CookTorranceMaterial(float diffuseReflectance, const RGBColor& diffuseColor, const RGBColor& FAlbedo, bool metalness, float roughness) :
	m_FAlbedo{ FAlbedo },
	m_Roughness{ roughness * roughness },
	m_DiffuseReflectance{ diffuseReflectance },
	m_BRDF{},
	m_DiffuseColor{ diffuseColor },
	m_IsMetal{ metalness },
	m_ResetColor{ 1.f,1.f,1.f },
	m_ZeroColor{ 0.f,0.f,0.f }

{
	if (!metalness)
	{
		m_FAlbedo = { 0.04f,0.04f,0.04f };//non metal color
	}

	const float minRoughness{ 0.01f };
	if (m_Roughness < minRoughness)
	{
		m_Roughness = minRoughness;
	}
	else if (m_Roughness > 1.f)
	{
		m_Roughness = 1.f;
	}
}

Elite::RGBColor Elite::CookTorranceMaterial::Shade(const HitRecord& hitrecord, const FVector3& lightDirection, const FVector3& viewDirection) const
{
	const FVector3 halfVector = CalculateHalfVector(lightDirection, viewDirection);
	const RGBColor F = FresnelSchlick(viewDirection, halfVector);
	if (m_IsMetal)
	{
		return m_BRDF.CookTorrance(hitrecord.normal, viewDirection, lightDirection, NormalDistrubution(hitrecord.normal, halfVector), F, SmithsMethod(hitrecord.normal, lightDirection, viewDirection));
	}

	return ((m_ResetColor - F) * m_BRDF.Lambert(RGBColor(m_DiffuseReflectance, m_DiffuseReflectance, m_DiffuseReflectance), m_DiffuseColor) +
		m_BRDF.CookTorrance(hitrecord.normal, viewDirection, lightDirection, NormalDistrubution(hitrecord.normal, halfVector), F, SmithsMethod(hitrecord.normal, lightDirection, viewDirection)));
}

Elite::FVector3 Elite::CookTorranceMaterial::CalculateHalfVector(const FVector3& l, const FVector3& v) const
{
	return (v + l) / Magnitude(v + l);
}

float  Elite::CookTorranceMaterial::NormalDistrubution(const FVector3& normal, const FVector3& halfVector) const
{
	const float roughness{ m_Roughness * m_Roughness };
	const float halfFormule = ((Dot(normal, halfVector) * (Dot(normal, halfVector))) * (roughness - 1.f) + 1.f);
	return roughness / (static_cast<float>(E_PI) * (halfFormule * halfFormule));
}

Elite::RGBColor Elite::CookTorranceMaterial::FresnelSchlick(const FVector3& viewDirection, const FVector3& halfVector) const
{
	const float hv{ Dot(halfVector, viewDirection) };
	return  m_FAlbedo + ((m_ResetColor - m_FAlbedo) * (std::powf((1.f - (hv)), 5.f)));
}

float Elite::CookTorranceMaterial::GeometrySchlick(const FVector3& normal, const FVector3& direction) const
{
	const float roughness{ m_Roughness * m_Roughness };
	float nv{ Dot(normal,direction) };
	if (nv < 0.f)
	{
		nv = 0.f;
	}
	const float kDirect = powf(roughness + 1.f, 2) / 8.f;
	return nv / ((nv * (1.f - kDirect)) + kDirect);
}

float Elite::CookTorranceMaterial::SmithsMethod(const FVector3& hitrecord, const FVector3& Lightdirection, const FVector3& ViewDirection) const
{
	return GeometrySchlick(hitrecord, ViewDirection) * GeometrySchlick(hitrecord, Lightdirection);
}


