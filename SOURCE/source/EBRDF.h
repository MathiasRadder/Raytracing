#pragma once
#include "ERGBColor.h"
#include "EMath.h"
#include "EHitRecord.h"
namespace Elite
{

	class BRDF
	{

	public:
		BRDF() = default;
		~BRDF() = default;

		RGBColor Lambert(const RGBColor& diffuseReflectance, const RGBColor& diffuseColor) const;
		RGBColor Phong(float specularReflactance, float phongExponent, const FVector3& l, const FVector3& v, const FVector3& hitRecordNormal) const;
		RGBColor CookTorrance(const FVector3& hitrecord, const FVector3& viewDirection, const FVector3& lightDirection, float NormalDistrubution, const RGBColor& FresnelSchlick, float smithMEthod) const;
		//spec and phon paramenters could be deffirent
	private:
	};
}

