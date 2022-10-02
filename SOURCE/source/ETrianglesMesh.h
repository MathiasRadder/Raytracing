#pragma once
#pragma once
#include "EGeometricObject.h"
#include "ESphere.h"
#include <vector>
#include <string>
namespace Elite
{
	class TrianglesMesh :public GeometricObject
	{
	public:
		TrianglesMesh(const std::vector<FPoint3>& triangleVectors, std::vector<int> indexBuffer,const FPoint4& pivotPos, float anglePitch, Culling cullmode, float radiusSphereBox);
		TrianglesMesh(const std::string& fileName ,const FPoint4& pivotPos,bool isStatic ,float anglePitch, Culling cullmode, float radiusSphereBox);
		bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

		void SetMaterialIndex(size_t index) override;
		size_t GetMaterialIndex() const override;

		void Update(float elapsedTime)override;
		~TrianglesMesh() = default;
		TrianglesMesh(const TrianglesMesh&) = delete;
		TrianglesMesh(TrianglesMesh&&) noexcept = delete;
		TrianglesMesh& operator=(const TrianglesMesh&) = delete;
		TrianglesMesh& operator=(TrianglesMesh&&) noexcept = delete;

	private:
		void ReadObjectFile(const std::string& fileName);
		void ReadVertices(const std::string& string);
		void ReadIndexes(const std::string& string);
		void CalculateVectorsOnMatrix();
		void StartVectorsOnMatrix();
		void PitchRotate(float angle);
		bool WhichCulling(const Ray& ray, const FVector3& normal)const;
		bool InSphere(const Ray& ray,   const HitRecord& hitRecord ) const;

		std::vector<FPoint3> m_VertexBuffer;//frist one is top and the third on is bottom
		std::vector<int> m_IndexBuffer;
		const FMatrix4 m_TranslationMatrix;
		const FPoint4 m_Translation;
		FMatrix4 m_PitchRotationMatrix;
		const float m_PitchAngle;
		const Culling m_CullMode;
		const float m_RadiusSphereBox;
		const bool m_IsStatic;
		size_t m_MaterialIndex;

	};
}

