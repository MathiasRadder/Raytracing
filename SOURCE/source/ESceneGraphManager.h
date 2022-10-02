#pragma once
#include <string>
#include "ESceneGraph.h"
#include "ESingleton.h"
#include "vector"


namespace Elite
{

	struct HitRecord;
	struct Ray;
	
	class SceneGraphManager : public Singleton<SceneGraphManager>
	{
	public:
		SceneGraphManager();
		~SceneGraphManager();
		void CreateEmptySceneGraph(const FPoint3& CameraPos, float fovAngle, const std::string& nameScene );
		bool Hit(const Ray& ray, HitRecord& hitRecord);
		void AddGeoObjectToScene(GeometricObject* pGeoObject, const std::string& nameMat,const std::string& nameScene);
		void AddLightToScene(Light* pLight, const std::string& nameScene);
		void PlayScene(const std::string& nameScene);
		void DeleteScene(const std::string& nameScene);

		void UpdateCurrentScene(float elapsedTime);
		std::shared_ptr<SceneGraph> GetCurrentScene() const;
		std::shared_ptr<SceneGraph> GetChosenCurrentScene(const std::string& nameScen)const;
		bool UpdateKeys();
		
	
	private:
		void NextScene();
		void ShowControlsFromTxtFile()const;
		std::vector<std::pair<std::shared_ptr<SceneGraph>, std::string>> m_SceneGraphs;
		int m_SceneIndex;
		
		friend class Singleton<SceneGraphManager>;
	
	};

}
