#include "ESceneGraph.h"
#include "EMaterialManager.h"
#include <SDL_pixels.h>

#include "EGeometricObject.h"
#include "Algorithm"
#include "SDL.h"

Elite::SceneGraph::SceneGraph(const FPoint3& CameraPos, float fovAngle) :
	m_RayHits{ false },
	m_PrevHitRecord{},
	m_pCamera{ new Camera(CameraPos,fovAngle) },
	m_MaxColorValue{ 1.f },
	m_GeoObjectIndex{ 0 },
	m_EnableHardShadows{ true },
	m_IrradianceOn{ true },
	m_BRDFOn{ true },
	m_ShadowRay{},
	m_LightPointColor{ 1.f,1.f ,1.f },
	m_BRDFColor{ 1.f ,1.f ,1.f },
	m_ResetColor{ 1.f ,1.f ,1.f },
	m_LambertLaw{0.f}
{

}

Elite::SceneGraph::~SceneGraph()
{
	


	for (GeometricObject* pObject : m_pObjectVector)
	{
		delete pObject;
		pObject = nullptr;
	}

	for (Light* pLight : m_pLightsVector)
	{
		delete pLight;
		pLight = nullptr;
	}
	delete m_pCamera;
	m_pCamera = nullptr;
}

void Elite::SceneGraph::AddpObject(GeometricObject* pGeoObject)//const?
{
	if (pGeoObject)
	{
		m_pObjectVector.push_back(pGeoObject);

	}
}

void Elite::SceneGraph::AddpObject(GeometricObject* pGeoObject, const std::string& matName)
{
	if (pGeoObject)
	{
	
		pGeoObject->SetMaterialIndex(MaterialManager::GetInstance().GetIndexOfMaterialVector(matName));
		m_pObjectVector.push_back(pGeoObject);

	}
}

void Elite::SceneGraph::AddpLight(Light* lpight)
{
	if (lpight)
	{
		m_pLightsVector.push_back(lpight);
	}
}


bool Elite::SceneGraph::Hit(const Ray& ray, HitRecord& hitRecord) //const
{
	//removing local objects here, increases  performance cause otherwise you need to make a new object x amount of time each frame
	m_RayHits = false;
	for (size_t i = 0; i < m_pObjectVector.size(); ++i)
	{
		if (m_pObjectVector[i]->Hit(ray, hitRecord))
		{
			if (!m_RayHits)
			{
				m_PrevHitRecord = hitRecord;
				m_RayHits = true;
				m_GeoObjectIndex = i;
				continue;
			}
			if (hitRecord.tValue < m_PrevHitRecord.tValue /*&& prevHitRecord.color != hitRecord.color*/)
			{
				m_GeoObjectIndex = i;
				m_PrevHitRecord = hitRecord;
			}
			else
			{
				hitRecord = m_PrevHitRecord;

			}
		}
	}
	return m_RayHits;

}
bool Elite::SceneGraph::HardShadowHit(const Ray& ray, HitRecord& hitRecord) const
{
	if (!m_EnableHardShadows)
	{
		return false;
	}
	HitRecord tmp = hitRecord;
	for (GeometricObject* pObject : m_pObjectVector)
	{
		if (pObject->Hit(ray, tmp))
		{
			return true;
		}
	}
	return false;
}

void Elite::SceneGraph::CalculateLightsOnPoint(HitRecord& hitRecord, const FVector3& Viewdirection)
{
	const auto pMaterial = MaterialManager::GetInstance().GetMaterialByIndex(m_pObjectVector[m_GeoObjectIndex]->GetMaterialIndex());
	m_ShadowRay.origin = hitRecord.hitPoint + hitRecord.normal * 0.05f;
	for (Light* pLight : m_pLightsVector)
	{
		if (!pLight->IsLightOn())
		{
			continue;
		}

		if (m_EnableHardShadows)
		{
			m_ShadowRay.direction = pLight->CalculateDirection(m_ShadowRay.origin);


			m_ShadowRay.tMax = pLight->CalculateDistance(m_ShadowRay.origin);
		}

		m_LambertLaw = Dot(hitRecord.normal, pLight->CalculateDirection(hitRecord.hitPoint));

		if (m_LambertLaw > 0.f && !HardShadowHit(m_ShadowRay, hitRecord))
		{
			if (m_BRDFOn)
			{
			
				m_BRDFColor = pMaterial->Shade(hitRecord, { pLight->GetDirection() }, { Viewdirection });
			}
			if (m_IrradianceOn)
			{
				m_LightPointColor = pLight->CalculateLightOnPoint(hitRecord);
			}
			hitRecord.color += m_LightPointColor * m_BRDFColor * m_LambertLaw;

		}

	}
	hitRecord.color.MaxToOne();
	m_BRDFColor = m_ResetColor;
	m_LightPointColor = m_ResetColor;
}

Elite::Camera* Elite::SceneGraph::GetCamera() const
{
	return m_pCamera;
}

void Elite::SceneGraph::Update(float elapsedTime)
{
	m_pCamera->Update();

	for (GeometricObject* pObject : m_pObjectVector)
	{
		pObject->Update(elapsedTime);
	}

}

void Elite::SceneGraph::ToggleShadow()
{
	m_EnableHardShadows = !m_EnableHardShadows;
}

void Elite::SceneGraph::ToggleFirstTypeLight(TypeLight tLight)
{
	TypeLight typeLight{ tLight };
	auto it = std::find_if(m_pLightsVector.begin(), m_pLightsVector.end(), [typeLight](const Light* pLight) {return pLight->GetTypeLight() == typeLight; });
	if (it != m_pLightsVector.end())
	{
		(*it)->ToggleLight();

	}
}



void Elite::SceneGraph::ToggleRightPointLight()
{
	TypeLight typeLight{ TypeLight::pointLight };
	size_t index{ 0 };
	for (size_t i = 0; i < m_pLightsVector.size(); i++)
	{
		if (m_pLightsVector[i]->GetTypeLight() == typeLight)
		{
			index = i;
		}
	}
	m_pLightsVector[index]->ToggleLight();
}

void Elite::SceneGraph::ToggleBRDFAndRadiance()
{

	if (m_IrradianceOn && m_BRDFOn)
	{
		m_BRDFOn = false;
	}
	else if (m_IrradianceOn)
	{
		m_BRDFOn = true;
		m_IrradianceOn = false;
	}
	else
	{
		m_IrradianceOn = true;;
	}

}









