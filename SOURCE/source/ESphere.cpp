#include "ESphere.h"
#include "ELambertMaterial.h"


Elite::Sphere::Sphere(const FPoint3& origin, const float radius) :
	m_Origin(origin),
	m_Radius(radius),
	m_MaterialIndex{0}
{
}

bool Elite::Sphere::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	const FVector3 tmpVec{ ray.origin - m_Origin };
	//Quadratic equation 
	const float a = Dot(ray.direction, ray.direction);
	const float b = Dot((2 * ray.direction), tmpVec);
	const float c = Dot(tmpVec, tmpVec) - (m_Radius * m_Radius);


	const auto discriminant = (b * b) - (4 * a * c);
	if (discriminant <= 0.f)
	{
		return false;
	}

	const float t0 = (-b -/*+*/ sqrt(discriminant)) / (2 * a); 
	//const float t = (-b -/*+*/ sqrt((b * b) - (4 * a * c))) / (2 * a);2

	if (t0 >= ray.tMin && t0 <= ray.tMax)
	{
		

		hitRecord.tValue = t0;


		hitRecord.hitPoint = ray.origin + hitRecord.tValue * ray.direction;
		hitRecord.normal = GetNormalized(hitRecord.hitPoint - m_Origin);
		return true;
	}



	return false;
}

//Elite::Material* Elite::Sphere::GetPointerMaterial() const
//{
//	return m_pMaterial;
//}

void Elite::Sphere::Update(float )
{
}


void Elite::Sphere::SetMaterialIndex(size_t index)
{
	m_MaterialIndex = index;
}

size_t Elite::Sphere::GetMaterialIndex() const
{
	return m_MaterialIndex;
}

Elite::Sphere::~Sphere()
{
	/*delete m_pMaterial;
	m_pMaterial = nullptr;*/
}



