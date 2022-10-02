#pragma once
#include <vector>
#include "ERay.h"
#include "EGeometricObject.h"
#include "ECamera.h"
#include "ELight.h"
#include <string>
#include <memory>
//#include <map>

namespace Elite
{

	struct HitRecord;

	class SceneGraph
	{
	public:
		SceneGraph(const FPoint3& CameraPos, float fovAngle);
		~SceneGraph();
		void AddpObject(GeometricObject* pGeoObject);
		void AddpObject(GeometricObject* pGeoObject, const std::string& matName);
		void AddpLight(Light* lpight);

		bool Hit(const Ray& ray,  HitRecord& hitRecord); //const;
		void CalculateLightsOnPoint(HitRecord& hitRecord, const FVector3& Viewdirection);
		Camera* GetCamera() const;
		void Update(float elapsedTime);


		void ToggleShadow();
		void ToggleFirstTypeLight(TypeLight tLight);
		void ToggleRightPointLight();
		void ToggleBRDFAndRadiance();



		SceneGraph(const SceneGraph& other) = delete;
		SceneGraph(SceneGraph&& other) = delete;
		SceneGraph& operator=(const SceneGraph& other) = delete;
		SceneGraph& operator=(SceneGraph&& other) = delete;

	private:
		bool HardShadowHit(const Ray& ray,  HitRecord& hitRecord) const;
		std::vector<GeometricObject*> m_pObjectVector;
		std::vector<Light*> m_pLightsVector;
		Camera* m_pCamera;
		HitRecord m_PrevHitRecord;
		bool m_RayHits;
		const float m_MaxColorValue;
		size_t m_GeoObjectIndex;
		bool m_EnableHardShadows;
		bool m_IrradianceOn;
		bool m_BRDFOn;
		Ray m_ShadowRay;
		RGBColor m_LightPointColor;
		RGBColor m_BRDFColor;
		const RGBColor m_ResetColor;
		float m_LambertLaw;
	};

}

