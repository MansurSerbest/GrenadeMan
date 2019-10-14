//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MaterialManager.h"
#include "..\Graphics\Material.h"
#include "..\Graphics\PostProcessingMaterial.h"
#include "..\Diagnostics\Logger.h"


MaterialManager::MaterialManager():
	m_pMaterials(map<UINT, Material*>()),
	m_pMaterials_PP(map<UINT, PostProcessingMaterial*>())
{
}


MaterialManager::~MaterialManager()
{
	//Delete Materials
	for (pair<UINT, Material*> materialEntry : m_pMaterials)
	{
		SafeDelete(materialEntry.second);
	}

	m_pMaterials.clear();

	//Delete Materials_PP
	for (pair<UINT, PostProcessingMaterial*> materialEntry : m_pMaterials_PP)
	{
		SafeDelete(materialEntry.second);
	}

	m_pMaterials_PP.clear();
}

void MaterialManager::Initialize(const GameContext& gameContext)
{
	if (m_IsInitialized)
		return;

	//Init Materials
	for (pair<UINT, Material*> materialEntry : m_pMaterials)
	{
		materialEntry.second->Initialize(gameContext);
	}

	//Init Materials_PP
	for (pair<UINT, PostProcessingMaterial*> materialEntry : m_pMaterials_PP)
	{
		materialEntry.second->Initialize(gameContext);
	}

	m_IsInitialized = true;
}

Material* MaterialManager::GetMaterial(UINT materialId)
{
	auto it = m_pMaterials.find(materialId);
	if (it == m_pMaterials.end())
	{
		Logger::LogFormat(LogLevel::Warning, L"MaterialManager::GetMaterial() > Material with ID %i not found!", materialId);
		return nullptr;
	}

	return it->second;
}

void MaterialManager::RemoveMaterial(UINT materialId)
{
	auto it = m_pMaterials.find(materialId);
	if (it == m_pMaterials.end())
	{
		Logger::LogFormat(LogLevel::Warning, L"MaterialManager::RemoveMaterial() > Material with ID %i not found!", materialId);
		return;
	}

	SafeDelete(it->second);
	m_pMaterials.erase(it);
}

void MaterialManager::AddMaterial(Material *pMaterial, UINT materialId)
{
#if _DEBUG
	if (pMaterial == nullptr)
	{
		Logger::LogError(L"MaterialManager::AddMaterial() > Material is NULL!");
		return;
	}

	for (pair<UINT, Material*> materialEntry : m_pMaterials)
	{
		if (materialEntry.first == materialId)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial() > MaterialID %i is already used!", materialId);
			return;
		}

		if (materialEntry.second == pMaterial)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial() > Material is already added to the manager (ID %i)!", materialId);
			return;
		}
	}
#endif

	m_pMaterials[materialId] = pMaterial;
}

void MaterialManager::AddMaterial_PP(PostProcessingMaterial* pMaterial_PP, UINT materialId_PP)
{
#if _DEBUG
	if (pMaterial_PP == nullptr)
	{
		Logger::LogError(L"MaterialManager::AddMaterial_PP() > Material_PP is NULL!");
		return;
	}

	for (pair<UINT, PostProcessingMaterial*> materialEntry : m_pMaterials_PP)
	{
		if (materialEntry.first == materialId_PP)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial_PP() > MaterialID_PP %i is already used!", materialId_PP);
			return;
		}

		if (materialEntry.second == pMaterial_PP)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial_PP() > Material_PP is already added to the manager (ID %i)!", materialId_PP);
			return;
		}
	}
#endif

	m_pMaterials_PP[materialId_PP] = pMaterial_PP;
}

void MaterialManager::RemoveMaterial_PP(UINT materialId_PP)
{
	auto it = m_pMaterials_PP.find(materialId_PP);
	if (it == m_pMaterials_PP.end())
	{
		Logger::LogFormat(LogLevel::Warning, L"MaterialManager::RemoveMaterial_PP() > Material_PP with ID %i not found!", materialId_PP);
		return;
	}

	SafeDelete(it->second);
	m_pMaterials_PP.erase(it);
}

PostProcessingMaterial* MaterialManager::GetMaterial_PP(UINT materialId_PP)
{
	auto it = m_pMaterials_PP.find(materialId_PP);
	if (it == m_pMaterials_PP.end())
	{
		Logger::LogFormat(LogLevel::Error, L"MaterialManager::GetMaterial_PP() > Material_PP with ID %i not found!", materialId_PP);
		return nullptr;
	}

	return it->second;
}
