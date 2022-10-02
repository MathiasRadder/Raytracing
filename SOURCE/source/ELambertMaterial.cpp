#include "ELambertMaterial.h"


Elite::LambertMaterial::LambertMaterial(float diffuseReflectance , const RGBColor& diffuseColor):
       m_DiffuseReflectance{diffuseReflectance }, // range between 0 and 1
       m_DiffuseColor{diffuseColor },
          m_BRDFLambert{}


{
 
}

Elite::RGBColor Elite::LambertMaterial::Shade(const HitRecord& hitrecord, const FVector3& w0, const FVector3& w1) const
{
    return m_BRDFLambert.Lambert(RGBColor( m_DiffuseReflectance,m_DiffuseReflectance,m_DiffuseReflectance ), m_DiffuseColor);
}


 