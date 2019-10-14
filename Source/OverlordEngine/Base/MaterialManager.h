#pragma once
#include <map>

class Material;
class PostProcessingMaterial;

class MaterialManager final
{

public:
	MaterialManager();
	~MaterialManager();

	void Initialize(const GameContext& gameContext);

	void AddMaterial(Material *pMaterial, UINT materialId);
	void RemoveMaterial(UINT materialId);
	Material* GetMaterial(UINT materialId);

	void AddMaterial_PP(PostProcessingMaterial *pMaterial, UINT materialId);
	void RemoveMaterial_PP(UINT materialId);
	PostProcessingMaterial* GetMaterial_PP(UINT materialId);

private:

	bool m_IsInitialized = false;
	map<UINT, Material*> m_pMaterials;
	map<UINT, PostProcessingMaterial*> m_pMaterials_PP;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MaterialManager(const MaterialManager& t);
	MaterialManager& operator=(const MaterialManager& t);
};

