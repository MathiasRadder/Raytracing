#include "ETriangle.h"
#include <iostream>

Elite::Triangle::Triangle(const std::vector<FPoint3>& triangleVectors, const FVector3& pivotPos, float anglePitch, Culling cullmode) :

	m_ObjectSPaceTriangleVectors{ triangleVectors },
	m_Normal{  },
	m_PitchAngle{ anglePitch },
	m_CullMode{ cullmode },
	m_PitchRotationMatrix{},
	m_MaterialIndex{ 0 },
	m_TotalTime{ 0.f },
	m_TranslationMatrix{  },
	m_TransformedTriangleVectors{ triangleVectors }
{
	m_TranslationMatrix = MakeTranslation(pivotPos);
	CalculateVectorsOnMatrix();


	
}
bool Elite::Triangle::Hit(const Ray& ray, HitRecord& hitRecord) const
{

	if (WhichCulling( ray))
	{
		return false;
	}

	float t = Dot((m_Center - ray.origin), m_Normal)/Dot(ray.direction, m_Normal);


	if (t < ray.tMin || t > ray.tMax)
	{

		return false;
	}


	//FPoint3 point = ray.origin +t * ray.direction;
	int vertexIndex = 1;
	FVector3 edge{};
	FVector3 pointToSide{};
	for (size_t i = 0; i < m_TransformedTriangleVectors.size(); i++)
	{
		 edge = m_TransformedTriangleVectors[vertexIndex] - m_TransformedTriangleVectors[i];

		 pointToSide = (ray.origin + t * ray.direction) - m_TransformedTriangleVectors[i];
		
		if (Dot(m_Normal, Cross(edge, pointToSide))< 0.f)
		{
			return false;
		}

		if (vertexIndex < 2)
		{
			vertexIndex++;
		}
		else
		{
			vertexIndex = 0;
		}
	}
	hitRecord.tValue = t;
	hitRecord.hitPoint = (ray.origin + t * ray.direction);
	hitRecord.normal = m_Normal;

	return true;
}

void Elite::Triangle::Update(float elapsedTime)
{
	UpdateAngle(elapsedTime);
	CalculateVectorsOnMatrix();
	CalculateNormalAndCenter();
}


void Elite::Triangle::UpdateAngle(float elapsedTime)
{
	m_TotalTime += elapsedTime;
	m_PitchRotationMatrix = MakeRotationY((cos(m_TotalTime)) * static_cast<float>(E_PI));
}

void Elite::Triangle::CalculateVectorsOnMatrix()
{
	for (size_t i = 0; i < m_ObjectSPaceTriangleVectors.size(); i++)
	{
		const FPoint4 tmpf{ m_ObjectSPaceTriangleVectors[i].x , m_ObjectSPaceTriangleVectors[i].y, m_ObjectSPaceTriangleVectors[i].z,1.f};
		m_TransformedTriangleVectors[i] = ((m_TranslationMatrix * m_PitchRotationMatrix) * tmpf).xyz;	
	}
}


bool Elite::Triangle::WhichCulling(const Ray& ray) const
{
	switch (m_CullMode)
	{
	case Culling::none:
		return (Dot(m_Normal, ray.direction) == 0.f);
	case Culling::backFace:
		return (Dot(m_Normal, ray.direction) >= 0.f);
	case Culling::frontFace:
		return (Dot(m_Normal, ray.direction) <= 0.f);
	}
	return false;
}

void Elite::Triangle::CalculateNormalAndCenter()
{
	m_Normal = Cross(m_TransformedTriangleVectors[1] - m_TransformedTriangleVectors[0], m_TransformedTriangleVectors[2] - m_TransformedTriangleVectors[0]);
	m_Center = static_cast<FPoint3>((static_cast<FVector3>(m_TransformedTriangleVectors[0]) + static_cast<FVector3>(m_TransformedTriangleVectors[1]) + static_cast<FVector3>(m_TransformedTriangleVectors[2])) / 3.f);
}



void Elite::Triangle::SetMaterialIndex(size_t index)
{
	m_MaterialIndex = index;
}

size_t Elite::Triangle::GetMaterialIndex() const
{
	return m_MaterialIndex;
}