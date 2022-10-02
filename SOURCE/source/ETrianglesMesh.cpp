#include "ETrianglesMesh.h"
#include <iostream>
#include <fstream>
#include <cassert>

Elite::TrianglesMesh::TrianglesMesh(const std::vector<FPoint3>& triangleVectors, std::vector<int> indexBuffer, const FPoint4& pivotPos, float anglePitch, Culling cullmode, float radiusSphereBox) :
	m_VertexBuffer{ triangleVectors },
	m_TranslationMatrix{ {1.f,0.f,0.f,0.f}, {0.f,1.f,0.f,0.f}, {0.f,0.f,1.f,0.f}, {0.f,2.f,0.f,1.f} },
	m_PitchAngle{ anglePitch },
	m_CullMode{ cullmode },
	m_PitchRotationMatrix{},
	m_Translation{ pivotPos },
	m_IndexBuffer{ indexBuffer },
	m_RadiusSphereBox{ radiusSphereBox },
	m_IsStatic{false},
	m_MaterialIndex{0}
{

	StartVectorsOnMatrix();


}
Elite::TrianglesMesh::TrianglesMesh(const std::string& fileName, const FPoint4& pivotPos, bool isStatic, float anglePitch, Culling cullmode, float radiusSphereBox) :
	m_VertexBuffer{  },
	m_TranslationMatrix{ {1.f,0.f,0.f,0.f}, {0.f,1.f,0.f,0.f}, {0.f,0.f,1.f,0.f}, {pivotPos.x,pivotPos.y,pivotPos.z,pivotPos.w} },
	m_PitchAngle{ anglePitch },
	m_CullMode{ cullmode },
	m_PitchRotationMatrix{},
	m_Translation{ pivotPos },
	m_IndexBuffer{  },
	m_RadiusSphereBox{ radiusSphereBox },

	m_IsStatic{ isStatic }
{

	ReadObjectFile(fileName);
	StartVectorsOnMatrix();
}




bool Elite::TrianglesMesh::Hit(const Ray& ray, HitRecord& hitRecord) const
{

	//	std::vector<Elite::FVector3> tmpVector{ CalculateVectorsOnMatrix() };
		//counterclock wise

	if (!InSphere(ray, hitRecord))
	{
		return false;
	}

	FPoint3 point{ 0.f,0.f,0.f };
	FVector3 normal{ 0.f,0.f,0.f };
	float t{ 0.f };
	float prevt{ ray.tMax+1 };
	int vertexIndex{ 0 };
	bool correctTiangle{ true };
	bool hasHitTriangle{ false };
	FPoint3 center{};
	FVector3 edge{};
	FVector3 pointToSide{};

	for (size_t i = 0; i < m_IndexBuffer.size(); i += 3)
	{
		normal = Cross(m_VertexBuffer[m_IndexBuffer[i + 1]] - m_VertexBuffer[m_IndexBuffer[i]], m_VertexBuffer[m_IndexBuffer[i + 2]] - m_VertexBuffer[m_IndexBuffer[i]]);
		
		if (WhichCulling(ray, normal))
		{
			continue;
		}
		center = static_cast<FPoint3>((static_cast<FVector3>(m_VertexBuffer[m_IndexBuffer[i]]) + static_cast<FVector3>(m_VertexBuffer[m_IndexBuffer[i+1]]) + static_cast<FVector3>(m_VertexBuffer[m_IndexBuffer[i+2]])) / 3.f);
		t = Dot((center - ray.origin), normal) / Dot(ray.direction, normal);
		if (t < ray.tMin || t > ray.tMax)
		{

			continue;
		}
		correctTiangle = true;
		point = ray.origin + t * ray.direction;
		vertexIndex = 1;
		for (size_t j = 0; j < 3; j++)
		{
			 edge = m_VertexBuffer[m_IndexBuffer[i + vertexIndex]] - m_VertexBuffer[m_IndexBuffer[i + j]];
			 pointToSide = point - m_VertexBuffer[m_IndexBuffer[i + j]];

			if (Dot(normal, Cross(edge, (pointToSide))) < 0.f)
			{
				correctTiangle = false;
				break;
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
		if (correctTiangle)
		{
			hasHitTriangle = true;
			if (t < prevt)
			{
				prevt = t;
				hitRecord.tValue = t;
				hitRecord.hitPoint = point;
				hitRecord.normal = normal;

			}
		}
	}
	
	return hasHitTriangle;
}

void Elite::TrianglesMesh::Update(float elapsedSec)
{
	if (!m_IsStatic)
	{
		PitchRotate(m_PitchAngle* elapsedSec);
		CalculateVectorsOnMatrix();
	}
}



void Elite::TrianglesMesh::CalculateVectorsOnMatrix()
{
	for (FPoint3& vector : m_VertexBuffer)
	{
		vector =(((m_TranslationMatrix *m_PitchRotationMatrix) * (static_cast<FPoint4>(vector)))).xyz;
	}
}

void Elite::TrianglesMesh::StartVectorsOnMatrix()
{
	for (FPoint3& vector : m_VertexBuffer)
	{
		vector = ((m_Translation)+static_cast<FVector4>(vector)).xyz;
	}
}

void Elite::TrianglesMesh::PitchRotate(float angle)
{
	angle = ToRadians(angle);
	m_PitchRotationMatrix = MakeRotation(angle, (m_TranslationMatrix[1]).xyz);
}

bool Elite::TrianglesMesh::WhichCulling(const Ray& ray, const FVector3& normal) const
{
	switch (m_CullMode)
	{
	case Culling::none:
		return (Dot(normal, ray.direction) == 0.f);
	case Culling::backFace:
		return (Dot(normal, ray.direction) >= 0.f);//maybe change in to >
	case Culling::frontFace:
		return (Dot(normal, ray.direction) <= 0.f);//maybe change in to >
	}
	return false;
}

bool Elite::TrianglesMesh::InSphere(const Ray& ray, const HitRecord& hitRecord) const//doesnt work
{

	const FVector3 tmpVec{ ray.origin - FPoint3{m_Translation.xyz} };
	//Quadratic equation 
	const float a = Dot(ray.direction, ray.direction);
	const float b = Dot((2 * ray.direction), tmpVec);
	const float c = Dot(tmpVec, tmpVec) - (m_RadiusSphereBox * m_RadiusSphereBox);
	const auto discriminant = (b * b) - (4 * a * c);
	if (discriminant <= 0.f)
	{
		return false;
	}
	const float t0 = (-b - sqrt(discriminant)) / (2 * a);
	if (/*t0 >= ray.tMin &&*/ t0 <= ray.tMax)
	{
		return true;
	}
	return false;
}


void Elite::TrianglesMesh::SetMaterialIndex(size_t index)
{
	m_MaterialIndex = index;
}

size_t Elite::TrianglesMesh::GetMaterialIndex() const
{
	return m_MaterialIndex;
}


void Elite::TrianglesMesh::ReadObjectFile(const std::string& fileName)
{
	std::ifstream input{ fileName };
	std::string gameLine{};
	while (std::getline(input, gameLine))
	{
		if (gameLine[0] == 'v')
		{
			ReadVertices(gameLine);
		}
		else if (gameLine[0] == 'f')
		{
			ReadIndexes(gameLine);
		}
	}
}

void Elite::TrianglesMesh::ReadVertices(const std::string& string)
{
	std::string copy{ string.substr(3, string.size() - 2) };
	size_t find = copy.find(' ');

	size_t size{ 3 };
	std::vector<float> tmpVecFloat{};
	for (size_t i = 0; i < size; i++)
	{
		tmpVecFloat.push_back(std::stof(copy.substr(0, find)));
		copy = copy.erase(0, find + 1);
		find = copy.find(' ');
	}
	m_VertexBuffer.push_back({ tmpVecFloat[0],tmpVecFloat[1],tmpVecFloat[2]});
}

void Elite::TrianglesMesh::ReadIndexes(const std::string& string)
{
	std::string copy{ string.substr(2, string.size() - 2) };
	size_t size{ 3 };
	for (size_t i = 0; i < size; i++)
	{
		size_t find = copy.find(' ');
		m_IndexBuffer.push_back(std::stoi(copy.substr(0, find)) - 1);
		copy = copy.erase(0, find + 1);
	}
}