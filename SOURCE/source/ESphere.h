#pragma once
#include "EGeometricObject.h"

namespace Elite
{

	class Sphere : public GeometricObject
	{
	public:

		Sphere(const FPoint3& origin,  const float radius);
		bool Hit(const Ray& ray, HitRecord& hitRecord) const override;
		void SetMaterialIndex(size_t index) override;
		size_t GetMaterialIndex() const override;
		void Update(float elapsedTime)override;
		~Sphere();
		Sphere(const Sphere&) = delete;
		Sphere(Sphere&&) noexcept = delete;
		Sphere& operator=(const Sphere&) = delete;
		Sphere& operator=(Sphere&&) noexcept = delete;


	private:

		const FPoint3 m_Origin;
		 const float m_Radius;


		 size_t m_MaterialIndex;
		
	};

}
