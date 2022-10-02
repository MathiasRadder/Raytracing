#pragma once

#pragma once
#include "EMaterial.h"
#include "EBRDF.h"
namespace Elite
{

	class CookTorranceMaterial :public Material
	{
	public:
		CookTorranceMaterial(float diffuseReflectance, const RGBColor& diffuseColor, const RGBColor& FAlbedo, bool metalness, float roughness);
		RGBColor Shade(const HitRecord& hitrecord, const FVector3& w0, const FVector3& w1) const override;

	private:
		FVector3 CalculateHalfVector(const FVector3& l, const FVector3& v)const;
		float NormalDistrubution(const FVector3& normal, const FVector3& halfVector) const;
		RGBColor FresnelSchlick(const FVector3& viewDirection, const FVector3& halfVector)const;
		float GeometrySchlick(const FVector3& normal, const FVector3& direction)const;
		float SmithsMethod(const FVector3& normal, const FVector3& Lightdirection, const FVector3& ViewDirection) const;
		RGBColor m_FAlbedo;
		float m_Roughness;
		const float m_DiffuseReflectance; // range between 0 and 1
		const BRDF m_BRDF;
		const RGBColor m_DiffuseColor;
		const bool m_IsMetal;
		const RGBColor m_ResetColor;
		const RGBColor m_ZeroColor;
	};
}


