#include "EPhongLambertMaterial.h"

Elite::PhongLambertMaterial::PhongLambertMaterial(float diffuseReflectance, const RGBColor& diffuseColor, float specularReflectance, float phongExponent):
	m_DiffuseReflectance{diffuseReflectance},
	m_DiffuseColor{ diffuseColor },
	m_SpecularReflectance{ specularReflectance },
	m_PhongExponent{ phongExponent },
	m_BRDF{}
	

{
	
}

Elite::RGBColor Elite::PhongLambertMaterial::Shade(const HitRecord& hitrecord, const FVector3& lightDirection, const FVector3& viewDirection) const
{

	return m_BRDF.Lambert(RGBColor(m_DiffuseReflectance, m_DiffuseReflectance, m_DiffuseReflectance),m_DiffuseColor) + m_BRDF.Phong(m_SpecularReflectance,m_PhongExponent,lightDirection,viewDirection,hitrecord.normal);
}

