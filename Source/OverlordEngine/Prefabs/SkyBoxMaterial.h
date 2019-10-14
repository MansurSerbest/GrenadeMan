#pragma once
#include "..\Graphics\Material.h"

class TextureData;

class SkyBoxMaterial : public Material
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial();
	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	TextureData * m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable  *m_pDiffuseSRVvariable;

	//-----------------------
	//Disabling default copy constructor and default
	//assignment operator.
	//-----------------------
	SkyBoxMaterial(const SkyBoxMaterial &obj) = delete;
	SkyBoxMaterial& operator=(const SkyBoxMaterial& obj) = delete;
};