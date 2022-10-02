#pragma once
#include "EGeometricObject.h"
#include "ERGBColor.h"


namespace Elite
{
	class Plane : public GeometricObject
	{
	public:

		Plane(const FPoint3& origin, const FVector3& normal);
		bool Hit(const Ray& ray, HitRecord& hitRecord) const override;
		//Material* GetPointerMaterial()const override;
		void SetMaterialIndex(size_t index) override;
		 size_t GetMaterialIndex() const override;
		void Update(float elapsedTime)override;
		~Plane();

		Plane(const Plane&) = delete;
		Plane(Plane&&) noexcept = delete;
		Plane& operator=(const Plane&) = delete;
		Plane& operator=(Plane&&) noexcept = delete;
	private:
		const FPoint3 m_Origin;
		const FVector3 m_Normal;

		size_t m_MaterialIndex;

	};

}
