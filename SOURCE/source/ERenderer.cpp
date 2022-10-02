//External includes
#include "SDL.h"
#include "SDL_surface.h"
#include "EMaterialManager.h"
//Project includes
#include "ERenderer.h"

#include "ERay.h"
#include "ERGBColor.h"

#include "ERay.h"
#include "ESceneGraphManager.h"
#include "ELambertMaterial.h"
#include "EPhongLambertMaterial.h"
#include "ECookTorranceMaterial.h"
#include "EPointLight.h"
#include "EDirectionLight.h"
#include "ETriangle.h"
#include "ETrianglesMesh.h"

Elite::Renderer::Renderer(SDL_Window* pWindow):
	m_MaxColorValue{255.f},
	m_EmptyColor{0.f,0.f,0.f}
	


{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;

	m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	CreateMaterials();
	Createscene1();
	Createscene2();
	SceneGraphManager::GetInstance().PlayScene("Raytracer_Reference");
	//SceneGraphManager::GetInstance().PlayScene("Raytracer_Bunny");
}

void Elite::Renderer::Render() const
{
	SDL_LockSurface(m_pBackBuffer);


	auto currentScene = SceneGraphManager::GetInstance().GetCurrentScene();
	if (!currentScene)
	{
		return;
	}
	float FOV{ currentScene->GetCamera()->GetFov() };
	Ray tmpRay{};
	HitRecord tmpHitRecord{};
	tmpHitRecord.color = m_EmptyColor;
	FPoint4 rayPos{};
	FMatrix4 lookAtMatrix = { currentScene->GetCamera()->GetLookAtMatrix() };
	FPoint3 cameraPos{ currentScene->GetCamera()->GetPosition() };
	//Loop over all the pixels
	for (uint32_t r = 0; r < m_Height; ++r)
	{
		for (uint32_t c = 0; c < m_Width; ++c)
		{
			tmpRay.origin.x = (2.f * ((static_cast<float>(c) + 0.5f) / static_cast<float>(m_Width)) - 1.f)  * m_AspectRatio * FOV;
			tmpRay.origin.y = (1.f - 2.f * ((static_cast<float>(r) + 0.5f) / static_cast<float>(m_Height))) * FOV;
			tmpRay.origin.z = 1.f;
			rayPos = lookAtMatrix * static_cast<FPoint4>(tmpRay.origin);
			tmpRay.origin = cameraPos;
			tmpRay.direction = GetNormalized(static_cast<FPoint3>( rayPos) - cameraPos);
			//camera matrix times the ray matrix 4 on 4 times 1 on 4
			//Fill the pixels - pixel access demo
			// loop through all objects in scene
			// does ray hit object
			// draw color
			// else do nothing
			if (currentScene->Hit(tmpRay, tmpHitRecord))
			{
				tmpHitRecord.color = m_EmptyColor;
	
				currentScene->CalculateLightsOnPoint(tmpHitRecord, -tmpRay.direction);

				m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
					static_cast<uint8_t>(tmpHitRecord.color.r* m_MaxColorValue),
					static_cast<uint8_t>(tmpHitRecord.color.g* m_MaxColorValue),
					static_cast<uint8_t>(tmpHitRecord.color.b* m_MaxColorValue));
			}
			else
			{
				m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
					static_cast<uint8_t>(m_EmptyColor.r),
					static_cast<uint8_t>(m_EmptyColor.g),
					static_cast<uint8_t>(m_EmptyColor.b));

			}
	
		}
	}
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
			
			
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}

void Elite::Renderer::CreateMaterials()
{
	//creating materials
	Elite::LambertMaterial lambert_GreyBlue{ 1.f, { 0.49f, 0.57f, 0.57f } };
	Elite::LambertMaterial lambert_White{ 1.f, {1.f, 1.f,1.0f } };
	Elite::LambertMaterial lambert_Reddish{ 1.f, {1.f, 0.2f,0.5f } };

	Elite::CookTorranceMaterial PBR_RoughMetal{ 1.f, { 0.0f, 0.8f, 0.8f }, { 0.972f, 0.960f, 0.915f }, true,1.f };
	Elite::CookTorranceMaterial PBR_MediumMetal{ 1.f, { 0.0f, 0.8f, 0.8f }, { 0.972f, 0.960f, 0.915f }, true,0.6f };
	Elite::CookTorranceMaterial PBR_SmoothMetal{ 1.f, { 0.0f, 0.8f, 0.8f }, { 0.972f, 0.960f, 0.915f }, true,0.1f };

	Elite::CookTorranceMaterial PBR_RoughPLastic{ 1.f, { 0.75f, 0.75f, 0.75f }, { 0.972f, 0.960f, 0.915f }, false,1.f };
	Elite::CookTorranceMaterial PBR_MediumPlastic{ 1.f, { 0.75f, 0.75f, 0.75f }, { 0.972f, 0.960f, 0.915f }, false,0.4f };
	Elite::CookTorranceMaterial PBR_SmoothPLastic{ 1.f, { 0.75f, 0.75f, 0.75f }, { 0.972f, 0.960f, 0.915f }, false,0.1f };
	//adding materials
	MaterialManager::GetInstance().AddMaterial(new Elite::LambertMaterial(lambert_GreyBlue), "lambert_GreyBlue");
	MaterialManager::GetInstance().AddMaterial(new Elite::LambertMaterial(lambert_White), "lambert_White");
	MaterialManager::GetInstance().AddMaterial(new Elite::LambertMaterial(lambert_Reddish), "lambert_Reddish");

	MaterialManager::GetInstance().AddMaterial(new Elite::CookTorranceMaterial(PBR_RoughMetal), "PBR_RoughMetal");
	MaterialManager::GetInstance().AddMaterial(new Elite::CookTorranceMaterial(PBR_MediumMetal), "PBR_MediumMetal");
	MaterialManager::GetInstance().AddMaterial(new Elite::CookTorranceMaterial(PBR_SmoothMetal), "PBR_SmoothMetal");

	MaterialManager::GetInstance().AddMaterial(new Elite::CookTorranceMaterial(PBR_RoughPLastic), "PBR_RoughPLastic");
	MaterialManager::GetInstance().AddMaterial(new Elite::CookTorranceMaterial(PBR_MediumPlastic), "PBR_MediumPlastic");
	MaterialManager::GetInstance().AddMaterial(new Elite::CookTorranceMaterial(PBR_SmoothPLastic), "PBR_SmoothPLastic");
}

void Elite::Renderer::Createscene1()
{
	//creating scene
	SceneGraphManager::GetInstance().CreateEmptySceneGraph({ 0.f, 3.f, 9.f },45.f, "Raytracer_Reference");
	//adding geometric objects
	//adding planes
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(0.f, 0.f, 0.f), FVector3(0.f, 1.0f, 0.00f)), "lambert_GreyBlue", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(0.f, 0.f, -10.f), FVector3(0.f, 0.0f, 1.00f)),"lambert_GreyBlue", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(0.f, 10.f, 0.f), FVector3(0.f, -1.0f, 0.f)), "lambert_GreyBlue", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(5.f, 0.f, 0.f), FVector3(-1.f, 0.0f, 0.00f)), "lambert_GreyBlue", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(-5.f, 0.f, 0.f), FVector3(1.f, 0.0f, 0.00f)), "lambert_GreyBlue", "Raytracer_Reference");
	//adding sphere metal
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Sphere(FPoint3(-1.75f, 1.f, 0.f), 0.75f), "PBR_RoughMetal","Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Sphere(FPoint3(0.f, 1.f, 0.f), 0.75f), "PBR_MediumMetal", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Sphere(FPoint3(1.75f, 1.f, 0.f), 0.75f),"PBR_SmoothMetal", "Raytracer_Reference");
	//adding sphere plastic
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Sphere(FPoint3(-1.75f, 3.f, 0.f), 0.75f),"PBR_RoughPLastic", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Sphere(FPoint3(0.f, 3.f, 0.f),  0.75f),"PBR_MediumPlastic", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Sphere(FPoint3(1.75f, 3.f, 0.f), 0.75f), "PBR_SmoothPLastic", "Raytracer_Reference");
	//adding triangles
	std::vector<FPoint3> tmpVector{ {-0.75f, 1.5, 0},{-0.75f, 0, 0}, {0.75f,0,0} };
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Triangle({ tmpVector }, { -1.75f, 4.5f, 0.f }, 0.1f, Culling::backFace), "lambert_White", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Triangle({ tmpVector }, { 0.f, 4.5f, 0.f }, 0.1f, Culling::frontFace), "lambert_White", "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Triangle({ tmpVector }, { 1.75f, 4.5f, 0.f }, 0.1f, Culling::none), "lambert_White", "Raytracer_Reference");
	//Adding light
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(0.f, 5.f, -5.f), RGBColor(1.f, 0.61f, 0.45f), 50.f), "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(-2.5f, 5.f, 5.f), RGBColor(1.f, 0.8f, 0.45f), 70.f), "Raytracer_Reference");
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(2.5f, 2.5f, 5.f), RGBColor(0.34f, 0.47f, 0.68f), 50.f), "Raytracer_Reference");
}

void Elite::Renderer::Createscene2()
{
	//creating scene
	SceneGraphManager::GetInstance().CreateEmptySceneGraph({ 0.f, 3.f, 9.f },45.f, "Raytracer_Bunny");
	//adding geometric objects
	//adding planes
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(0.f, 0.f, 0.f), FVector3(0.f, 1.0f, 0.00f)), "lambert_GreyBlue", "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(0.f, 0.f, -10.f), FVector3(0.f, 0.0f, 1.00f)), "lambert_GreyBlue", "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(0.f, 10.f, 0.f), FVector3(0.f, -1.0f, 0.f)), "lambert_GreyBlue", "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(5.f, 0.f, 0.f), FVector3(-1.f, 0.0f, 0.00f)), "lambert_GreyBlue", "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new Plane(FPoint3(-5.f, 0.f, 0.f), FVector3(1.f, 0.0f, 0.00f)), "lambert_GreyBlue", "Raytracer_Bunny");
	//adding bunny
	SceneGraphManager::GetInstance().AddGeoObjectToScene(new TrianglesMesh("lowpoly_bunny.obj", {0.f,0.f,0.f,1.0f}, false,10.f, Elite::Culling::none, 3.2f),"lambert_Reddish", "Raytracer_Bunny");
	//adding lights
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(0.f, 5.f, -5.f), RGBColor(1.f,1.f,1.f), 70.f), "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(-2.5f, 5.f, 5.f), RGBColor(1.f, 1.f, 1.f), 70.f), "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(2.5f, 2.5f, 5.f), RGBColor(1.f, 1.f, 1.f), 70.f), "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(-4.5f, -4.5f, 5.f), RGBColor(1.f, 1.f, 1.f), 70.f), "Raytracer_Bunny");
	SceneGraphManager::GetInstance().AddLightToScene(new PointLight(FVector3(4.5f, 4.5f, 5.f), RGBColor(1.f, 1.f, 1.f), 70.f), "Raytracer_Bunny");
}


