#pragma once
#include "EGeometricObject.h"
#include <vector>
namespace Elite
{
	class Triangle :public GeometricObject
	{
	public:
		Triangle(const std::vector<FPoint3>& triangleVectors,const FVector3& pivotPos,float anglePitch, Culling cullmode);
		bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

		void SetMaterialIndex(size_t index) override;
		size_t GetMaterialIndex() const override;

		void Update(float elapsedTime)override;

		~Triangle() = default;
		Triangle(const Triangle&) = delete;
		Triangle(Triangle&&) noexcept = delete;
		Triangle& operator=(const Triangle&) = delete;
		Triangle& operator=(Triangle&&) noexcept = delete;

	private:
		void UpdateAngle(float elapsedTime);
		void CalculateVectorsOnMatrix();
	
		bool WhichCulling(const Ray& ray)const;
		void CalculateNormalAndCenter();
		std::vector<FPoint3> m_TransformedTriangleVectors;//frist one is top and the third on is bottom
		const std::vector<FPoint3> m_ObjectSPaceTriangleVectors;
		FPoint3 m_Center;
		FVector3 m_Normal;

		FMatrix4 m_TranslationMatrix;
		FMatrix4 m_PitchRotationMatrix;
		const float m_PitchAngle;
		const Culling m_CullMode;

		size_t m_MaterialIndex;
		float m_TotalTime;
	
	};
}

