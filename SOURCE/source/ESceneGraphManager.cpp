#include "ESceneGraphManager.h"


#include "ERay.h"
#include "EHitRecord.h"
#include "SDL.h"
#include <iostream>
#include <fstream>
#include <cassert>

Elite::SceneGraphManager::SceneGraphManager() :
	m_SceneIndex(0)
{
	std::cout << "Press T for controls.\n";
}

Elite::SceneGraphManager::~SceneGraphManager()
{
	
}

void Elite::SceneGraphManager::CreateEmptySceneGraph(const FPoint3& CameraPos, float fovAngle,const std::string& nameScene)
{


	if (std::any_of(m_SceneGraphs.begin(), m_SceneGraphs.end(), [nameScene](const auto a) {return a.second == nameScene; }))
	{
		return;
	}
	const auto scene = std::shared_ptr<SceneGraph>(new SceneGraph(CameraPos, fovAngle));
	m_SceneGraphs.push_back({ (scene), nameScene });

}



bool Elite::SceneGraphManager::Hit(const Ray& ray, HitRecord& hitRecord) 
{
	return m_SceneGraphs[m_SceneIndex].first->Hit(ray, hitRecord);
}
void Elite::SceneGraphManager::AddGeoObjectToScene(GeometricObject* pGeoObject, const std::string& nameMat, const std::string& nameScene)
{

	auto it = std::find_if(m_SceneGraphs.begin(), m_SceneGraphs.end(), [nameScene](const auto a) {return a.second == nameScene; });
	if (it != m_SceneGraphs.end())
	{
		it->first->AddpObject(pGeoObject, nameMat);
	}



	
}

void Elite::SceneGraphManager::AddLightToScene(Light* pLight, const std::string& nameScene)
{
	auto it = std::find_if(m_SceneGraphs.begin(), m_SceneGraphs.end(), [nameScene](const auto a) {return a.second == nameScene; });
	if (it != m_SceneGraphs.end())
	{
		it->first->AddpLight(pLight);
	}

	
}

void Elite::SceneGraphManager::PlayScene(const std::string& nameScene)
{
	for (int i = 0; i < m_SceneGraphs.size(); ++i)
	{
		if (m_SceneGraphs[i].second == nameScene)
		{
			m_SceneIndex = i;
		}
	}
}

void Elite::SceneGraphManager::DeleteScene(const std::string& nameScene)
{
	// source: https://stackoverflow.com/questions/39019806/using-erase-remove-if-idiom
	m_SceneGraphs.erase(std::remove_if(m_SceneGraphs.begin(),
		m_SceneGraphs.end(),
		[nameScene](const std::pair<std::shared_ptr<Elite::SceneGraph>, std::string> element)-> bool
		{ return element.second == nameScene; }),
		m_SceneGraphs.end());	
}

void Elite::SceneGraphManager::UpdateCurrentScene(float elapsedTime)
{
	if (m_SceneGraphs.size() != 0)
	{
		m_SceneGraphs[m_SceneIndex].first->Update(elapsedTime);
	}
}

std::shared_ptr<Elite::SceneGraph> Elite::SceneGraphManager::GetCurrentScene() const
{
	if (m_SceneGraphs.size() != 0)
	{
		return m_SceneGraphs[m_SceneIndex].first;
	}
	return nullptr;

}

std::shared_ptr<Elite::SceneGraph> Elite::SceneGraphManager::GetChosenCurrentScene(const std::string& nameScen) const
{

	auto it = std::find_if(m_SceneGraphs.begin(), m_SceneGraphs.end(), [nameScen](const auto a) {return a.second == nameScen; });

	if (it != m_SceneGraphs.end())
	{
		return (it)->first;

	}
	return m_SceneGraphs.end()->first;

}

bool Elite::SceneGraphManager::UpdateKeys()
{

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			if (e.key.keysym.scancode == SDL_SCANCODE_Q)
			 {
				m_SceneGraphs[m_SceneIndex].first->ToggleShadow();
				break;
			 }
			else if (e.key.keysym.scancode == SDL_SCANCODE_J)
			 {
				 m_SceneGraphs[m_SceneIndex].first->ToggleFirstTypeLight(TypeLight::directionalLight);
				 break;
			 }
			else if (e.key.keysym.scancode == SDL_SCANCODE_L)
			{
				m_SceneGraphs[m_SceneIndex].first->ToggleRightPointLight();
				break;
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_K)
			{
				m_SceneGraphs[m_SceneIndex].first->ToggleFirstTypeLight(TypeLight::pointLight);
				break;
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_E)
			{
				m_SceneGraphs[m_SceneIndex].first->ToggleBRDFAndRadiance();
				break;
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_G)
			{
				NextScene();
				break;
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_T)
			{
				ShowControlsFromTxtFile();
				break;
			}
		}
	}
	return true;
}



void Elite::SceneGraphManager::NextScene()
{
	if (m_SceneIndex < m_SceneGraphs.size()-1)
	{
		m_SceneIndex++;
	}
	else
	{
		m_SceneIndex = 0;
	}
}

void Elite::SceneGraphManager::ShowControlsFromTxtFile()const
{
	std::ifstream input{ "Controls.txt"};
	std::string gameLine{};

	while (std::getline(input, gameLine))
	{
		std::cout << gameLine << "\n";
	}
}
