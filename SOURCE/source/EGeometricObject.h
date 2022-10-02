#pragma once
#include "ERay.h"
#include "EHitRecord.h"
#include "EMaterial.h"
namespace Elite
{
	enum class Culling
	{
		frontFace,
		backFace,
		none
	};
	class GeometricObject
	{
	public:

		virtual ~GeometricObject() = default;
		virtual bool Hit(const Ray& ray,HitRecord& hitRecord) const = 0;
		virtual void SetMaterialIndex(size_t index) = 0;
		virtual size_t GetMaterialIndex() const = 0;
		virtual void Update(float elapsedTime) = 0;
	};
}