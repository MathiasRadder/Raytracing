#pragma once
#include "EMaterial.h"
#include "EBRDF.h"
namespace Elite
{

	class PhongLambertMaterial :public Material
	{
	public:
		PhongLambertMaterial(float diffuseReflectance, const RGBColor& diffuseColor,float specularReflectance, float phongExponent);
		RGBColor Shade(const HitRecord& hitrecord, const FVector3& w0, const FVector3& w1) const override;

	private:

		const float m_DiffuseReflectance; // range between 0 and 1
		const float m_SpecularReflectance;
		const RGBColor m_DiffuseColor;
		const float m_PhongExponent;
		const BRDF m_BRDF;

	};
}

