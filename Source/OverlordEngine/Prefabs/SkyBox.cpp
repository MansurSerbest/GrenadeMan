#include "stdafx.h"

#include "SkyBox.h"
#include "../Components/Components.h"
#include "SkyBoxMaterial.h"
#include "..\Graphics\Material.h"

SkyBox::SkyBox(std::wstring assetFile) :
	m_pModelComponent{nullptr},
	m_assetFile{assetFile}
{
}


SkyBox::~SkyBox(void)
{
}

void SkyBox::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pModelComponent = new ModelComponent(m_assetFile);
	AddComponent(m_pModelComponent);

	SkyBoxMaterial *pSkyBoxMaterial = new SkyBoxMaterial();
	pSkyBoxMaterial->SetDiffuseTexture(L"Resources/Textures/SkyBox.dds");
	if (!pSkyBoxMaterial) Logger::LogError(L"pSkyBoxMaterial is not loaded!");

	gameContext.pMaterialManager->AddMaterial(pSkyBoxMaterial, 2);
	m_pModelComponent->SetMaterial(2);

	
}