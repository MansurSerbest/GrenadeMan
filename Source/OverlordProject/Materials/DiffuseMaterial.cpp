#include "../../OverlordEngine/Base/stdafx.h"
#include "DiffuseMaterial.h"
#include "Content\ContentManager.h"
#include "Diagnostics\Logger.h"
#include "Graphics/TextureData.h"


 ID3DX11EffectShaderResourceVariable *DiffuseMaterial::m_pDiffuseSRVvariable = nullptr;

DiffuseMaterial::DiffuseMaterial()
	:Material(L"Resources/Effects/PosNormTex3D.fx")
{

}
DiffuseMaterial::~DiffuseMaterial()
{

}
void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);	
}

void DiffuseMaterial::LoadEffectVariables()
{
	m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseSRVvariable) Logger::LogError(L"m_pDiffuseSRVvariable is not loaded!");
}
void DiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
}