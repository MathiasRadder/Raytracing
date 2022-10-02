#pragma once
#include <string>
#include "EMaterial.h"
#include "ESingleton.h"
#include "vector"

namespace Elite
{

class MaterialManager : public Singleton<MaterialManager>
{
public:
	MaterialManager();
	~MaterialManager();

	size_t GetIndexOfMaterialVector(const std::string& matName)const;
	void AddMaterial(Material* mat, const std::string& name);

	 Material* GetMaterialByIndex(size_t index)const;

private:
	void CreateFirstMaterial();
	std::vector< std::pair<Material*, std::string>> m_MaterialsVector;

	friend class Singleton<MaterialManager>;
};
}

