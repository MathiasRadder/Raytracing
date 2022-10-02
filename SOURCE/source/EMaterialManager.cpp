#include "EMaterialManager.h"
#include "ELambertMaterial.h"

Elite::MaterialManager::MaterialManager()
{
	CreateFirstMaterial();
}

Elite::MaterialManager::~MaterialManager()
{
	for (auto& mat : m_MaterialsVector)
	{
		delete mat.first;
		mat.first = nullptr;
	}
}


size_t Elite::MaterialManager::GetIndexOfMaterialVector(const std::string& matName) const
{
	for (size_t i = 0; i < m_MaterialsVector.size(); i++)
	{
		if (m_MaterialsVector[i].second == matName)
		{
			return i;
		}
	}
	return 0;
}

void Elite::MaterialManager::AddMaterial(Material* mat, const std::string& name)
{
	if (!mat)
	{
		return;
	}
	if (std::any_of(m_MaterialsVector.begin(), m_MaterialsVector.end(), [name](const auto a) {return a.second == name; }))
	{
		return;
	}
	m_MaterialsVector.push_back({ mat,name });
}

 Elite::Material* Elite::MaterialManager::GetMaterialByIndex(size_t index) const
{
	if (m_MaterialsVector[index].first)
	{
		return m_MaterialsVector[index].first;
	}
	return m_MaterialsVector[0].first;
}

 void Elite::MaterialManager::CreateFirstMaterial()
 {
	 m_MaterialsVector.push_back({new Elite::LambertMaterial{ 1.f, {1.f, 1.f,1.0f } }, "FirstMatLambert" });
 }
