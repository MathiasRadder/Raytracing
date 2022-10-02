#include "EBRDF.h"



Elite::RGBColor Elite::BRDF::Lambert(const RGBColor& diffuseReflectance, const RGBColor& diffuseColor) const
{
	//return (diffuseColor * diffuseReflectance) / static_cast<float>(E_PI);

	return (diffuseColor * diffuseReflectance) / static_cast<float>(E_PI);
}

Elite::RGBColor Elite::BRDF::Phong(float specularReflactance,  float phongExponent, const FVector3& l, const FVector3& v, const FVector3& hitRecordNormal) const
{
	FVector3 reflect = l - 2.f*(Dot(hitRecordNormal, l) * hitRecordNormal);
	float spec = Dot(reflect,v);
	float phongSpecReflect = specularReflactance * powf(spec, phongExponent);
	return RGBColor(phongSpecReflect, phongSpecReflect, phongSpecReflect);
}

Elite::RGBColor Elite::BRDF::CookTorrance(const FVector3& hitrecord, const FVector3& viewDirection, const FVector3& lightDirection, float normalDistrubution, const RGBColor& fresnelSchlick, float smithMEthod) const
{
	return (fresnelSchlick * smithMEthod * normalDistrubution) / (4.f* (Dot(viewDirection, hitrecord)* Dot(lightDirection, hitrecord)));
}
