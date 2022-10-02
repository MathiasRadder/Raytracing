#pragma once
#include "EMaterial.h"
#include "EBRDF.h"
namespace Elite
{

	class LambertMaterial : public Material
	{
	public:
		LambertMaterial(float diffuseReflectance , const RGBColor& diffuseColor);
		~LambertMaterial() = default;
		RGBColor Shade(const HitRecord& hitrecord, const FVector3& w0, const FVector3& w1) const override;


	private:
		const float m_DiffuseReflectance; // range between 0 and 1
		const RGBColor m_DiffuseColor; // color of the material
		const BRDF m_BRDFLambert;

	};
}

