#include "stdafx.h"
#include "SkyBoxMaterial.h"
#include "..\Content\ContentManager.h"
#include "..\Diagnostics\Logger.h"
#include "..\Graphics/TextureData.h"


ID3DX11EffectShaderResourceVariable *SkyBoxMaterial::m_pDiffuseSRVvariable = nullptr;

SkyBoxMaterial::SkyBoxMaterial()
	:Material(L"Resources/Effects/SkyBox.fx")
{

}
SkyBoxMaterial::~SkyBoxMaterial()
{

}
void SkyBoxMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	if (!m_pDiffuseTexture) Logger::LogError(L"m_pDiffuseTexture is not loaded!");
}

void SkyBoxMaterial::LoadEffectVariables()
{
	m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("m_CubeMap")->AsShaderResource();
	if (!m_pDiffuseSRVvariable) Logger::LogError(L"m_CubeMap is not loaded!");
}
void SkyBoxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
}