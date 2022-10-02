#include "EPlane.h"
#include "ELambertMaterial.h"





Elite::Plane::Plane(const FPoint3& origin, const FVector3& normal):
m_Origin(origin),
m_Normal(GetNormalized(normal)),
m_MaterialIndex{0}

{
}

bool Elite::Plane::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	//finding t



	float t = (Dot((m_Origin - ray.origin), m_Normal)) / Dot(ray.direction, m_Normal);
	// check if the t is in the interval of tMin and tMax
	if (t >= ray.tMin && t <= ray.tMax)
	{
		
		// if this is true, now we can see if there is a point
		hitRecord.tValue = t;
		hitRecord.hitPoint = ray.origin + t * ray.direction;
		hitRecord.normal = m_Normal;

		
		return true;
		
	}

	
	// if it hits then the hitrecord copies the color and info
	return false;
}



void Elite::Plane::SetMaterialIndex(size_t index)
{
	m_MaterialIndex = index;
}

size_t Elite::Plane::GetMaterialIndex() const
{
	return m_MaterialIndex;
}

void Elite::Plane::Update(float)
{
}

Elite::Plane::~Plane()
{
	//delete m_pMaterial;
	//m_pMaterial = nullptr;
}
