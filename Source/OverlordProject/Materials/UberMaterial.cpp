#include "../../OverlordEngine/Base/stdafx.h"
#include "UberMaterial.h"
#include "Components\ModelComponent.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"

//LIGHT
//*****
ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable = nullptr;

//DIFFUSE
//*******
ID3DX11EffectScalarVariable* UberMaterial::m_pUseDiffuseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable = nullptr;

//SPECULAR
//********
ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable = nullptr;

//AMBIENT
//*******
ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable = nullptr;

//NORMAL MAPPING
//**************
ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable = nullptr;

//ENVIRONMENT MAPPING
//*******************
ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable = nullptr;

//OPACITY
//***************
ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable = nullptr;

//SPECULAR MODELS
//***************
ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable = nullptr;

//FRESNEL FALLOFF
//***************
ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable = nullptr;

UberMaterial::UberMaterial()
	:Material(L"Resources/Effects/UberShader.fx"),
	m_bDiffuseTexture{false},
	m_bSpecularLevelTexture{false},
	m_bFlipGreenChannel{false},
	m_bNormalMapping{false},
	m_bEnvironmentMapping{false},
	m_bOpacityMap{false},
	m_bSpecularBlinn{false},
	m_bSpecularPhong{false},
	m_bFresnelFaloff{false},
	m_ColorAmbient{1.0f, 1.0f, 1.0f, 1.0f},
	m_AmbientIntensity{0.5f},
	m_ReflectionStrength{0.0f},
	m_Opacity{1.0f}
{

}
UberMaterial::~UberMaterial()
{
	
}

//LIGHT
//*****
void UberMaterial::SetLightDirection(XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

//DIFFUSE
//*******
void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}
void UberMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}
void UberMaterial::SetDiffuseColor(XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

//SPECULAR
//********
void UberMaterial::SetSpecularColor(XMFLOAT4 color)
{
	m_ColorSpecular = color;
}
void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}
void UberMaterial::SetSpecularLevelTexture(const wstring& assetFile)
{
	m_pSpecularLevelTexture = ContentManager::Load<TextureData>(assetFile);
}
void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

//AMBIENT
//*******
void UberMaterial::SetAmbientColor(XMFLOAT4 color)
{
	m_ColorAmbient = color;
}
void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

//NORMAL MAPPING
//**************
void UberMaterial::FlipNormalGreenCHannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}
void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}
void UberMaterial::SetNormalMapTexture(const wstring& assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

//ENVIRONMENT MAPPING
//*******************
void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}
void UberMaterial::SetEnvironmentCube(const wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}
void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}
void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}
void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

//OPACITY
//*******
void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}
void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}
void UberMaterial::SetOpacityTexture(const wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

//SPECULAR MODELS
//***************
void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}
void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

//FRESNEL FALLOFF
//***************
void UberMaterial::EnableFresnelFaloff(bool enable)
{
	m_bFresnelFaloff = enable;
}
void UberMaterial::SetFresnelColor(XMFLOAT4 color)
{
	m_ColorFresnel = color;
}
void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}
void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}
void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}

void UberMaterial::LoadEffectVariables()
{
	//LIGHT
	m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
	if (!m_pLightDirectionVariable) Logger::LogError(L"m_pDiffuseSRVariable is not loaded!");

	//DIFFUSE
	m_pUseDiffuseTextureVariable = m_pEffect->GetVariableByName("gUseTextureDiffuse")->AsScalar();
	if (!m_pUseDiffuseTextureVariable) Logger::LogError(L"m_pUseDiffuseTextureVariable is not loaded!");
	m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gTextureDiffuse")->AsShaderResource();
	if (!m_pDiffuseSRVvariable) Logger::LogError(L"m_pDiffuseSRVvariable is not loaded!");
	m_pDiffuseColorVariable = m_pEffect->GetVariableByName("gColorDiffuse")->AsVector();
	if (!m_pDiffuseColorVariable) Logger::LogError(L"m_pDiffuseColorVariable is not loaded!");

	//SPECULAR
	m_pSpecularColorVariable = m_pEffect->GetVariableByName("gColorSpecular")->AsVector();
	if (!m_pSpecularColorVariable) Logger::LogError(L"m_pSpecularColorVariable is not loaded!");
	m_pUseSpecularLevelTextureVariable = m_pEffect->GetVariableByName("gUseTextureSpecularIntensity")->AsScalar();
	if (!m_pUseSpecularLevelTextureVariable) Logger::LogError(L"m_pUseSpecularLevelTextureVariable is not loaded!");
	m_pSpecularLevelSRVvariable = m_pEffect->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();
	if (!m_pSpecularLevelSRVvariable) Logger::LogError(L"m_pSpecularLevelSRVvariable is not loaded!");
	m_pShininessVariable = m_pEffect->GetVariableByName("gShininess")->AsScalar();
	if (!m_pShininessVariable) Logger::LogError(L"m_pShininessVariable is not loaded!");

	//AMBIENT
	m_pAmbientColorVariable = m_pEffect->GetVariableByName("gColorAmbient")->AsVector();
	if (!m_pAmbientColorVariable) Logger::LogError(L"m_pAmbientColorVariable is not loaded!");
	m_pAmbientIntensityVariable = m_pEffect->GetVariableByName("gAmbientIntensity")->AsScalar();
	if (!m_pAmbientIntensityVariable) Logger::LogError(L"m_pAmbientIntensityVariable is not loaded!");

	//NORMAL MAPPING
	m_pFlipGreenChannelVariable = m_pEffect->GetVariableByName("gFlipGreenChannel")->AsScalar();
	if (!m_pFlipGreenChannelVariable) Logger::LogError(L"m_pFlipGreenChannelVariable is not loaded!");
	m_pUseNormalMappingVariable = m_pEffect->GetVariableByName("gUseTextureNormal")->AsScalar();
	if (!m_pUseNormalMappingVariable) Logger::LogError(L"m_pUseNormalMappingVariable is not loaded!");
	m_pNormalMappingSRVvariable = m_pEffect->GetVariableByName("gTextureNormal")->AsShaderResource();
	if (!m_pNormalMappingSRVvariable) Logger::LogError(L"m_pNormalMappingSRVvariable is not loaded!");

	//ENVIRONMENT MAPPING
	m_pUseEnvironmentMappingVariable = m_pEffect->GetVariableByName("gUseTextureEnvironment")->AsScalar();
	if (!m_pUseEnvironmentMappingVariable) Logger::LogError(L"m_pUseEnvironmentMappingVariable is not loaded!");
	m_pEnvironmentSRVvariable = m_pEffect->GetVariableByName("gCubeEnvironment")->AsShaderResource();
	if (!m_pEnvironmentSRVvariable) Logger::LogError(L"m_pEnvironmentSRVvariable is not loaded!");
	m_pReflectionStrengthVariable = m_pEffect->GetVariableByName("gReflectionStrength")->AsScalar();
	if (!m_pReflectionStrengthVariable) Logger::LogError(L"m_pReflectionStrengthVariable is not loaded!");
	m_pRefractionStrengthVariable = m_pEffect->GetVariableByName("gRefractionStrength")->AsScalar();
	if (!m_pRefractionStrengthVariable) Logger::LogError(L"m_pRefractionStrengthVariable is not loaded!");
	m_pRefractionIndexVariable = m_pEffect->GetVariableByName("gRefractionIndex")->AsScalar();
	if (!m_pRefractionIndexVariable) Logger::LogError(L"m_pRefractionIndexVariable is not loaded!");

	//SPECULAR MODELS
	m_pUseBlinnVariable = m_pEffect->GetVariableByName("gUseSpecularBlinn")->AsScalar();
	if (!m_pUseBlinnVariable) Logger::LogError(L"m_pUseBlinnVariable is not loaded!");
	m_pUsePhongVariable = m_pEffect->GetVariableByName("gUseSpecularPhong")->AsScalar();
	if (!m_pUsePhongVariable) Logger::LogError(L"m_pUsePhongVariable is not loaded!");

	//SPECULAR MODELS
	m_pUseOpacityMapVariable = m_pEffect->GetVariableByName("gUseTextureOpacity")->AsScalar();
	if (!m_pUseOpacityMapVariable) Logger::LogError(L"m_pUseOpacityMapVariable is not loaded!");
	m_pOpacityVariable = m_pEffect->GetVariableByName("gOpacityIntensity")->AsScalar();
	if (!m_pOpacityVariable) Logger::LogError(L"m_pOpacityVariable is not loaded!");
	m_pOpacitySRVvariable = m_pEffect->GetVariableByName("gTextureOpacity")->AsShaderResource();
	if (!m_pOpacitySRVvariable) Logger::LogError(L"m_pOpacitySRVvariable is not loaded!");


	//FRESNEL FALLOFF
	m_pUseFresnelFalloffVariable = m_pEffect->GetVariableByName("gUseFresnelFalloff")->AsScalar();
	if (!m_pUseFresnelFalloffVariable) Logger::LogError(L"m_pUseFresnelFalloffVariable is not loaded!");
	m_pFresnelColorVariable = m_pEffect->GetVariableByName("gColorFresnel")->AsVector();
	if (!m_pFresnelColorVariable) Logger::LogError(L"m_pFresnelColorVariable is not loaded!");
	m_pFresnelPowerVariable = m_pEffect->GetVariableByName("gFresnelPower")->AsScalar();
	if (!m_pFresnelPowerVariable) Logger::LogError(L"m_pFresnelPowerVariable is not loaded!");
	m_pFresnelMultiplierVariable = m_pEffect->GetVariableByName("gFresnelMultiplier")->AsScalar();
	if (!m_pFresnelMultiplierVariable) Logger::LogError(L"m_pFresnelMultiplierVariable is not loaded!");
	m_pFresnelHardnessVariable = m_pEffect->GetVariableByName("gFresnelHardness")->AsScalar();
	if (!m_pFresnelHardnessVariable) Logger::LogError(L"m_pFresnelHardnessVariable is not loaded!");


}
void UberMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	//LIGHT
	m_pLightDirectionVariable->SetFloatVector(&m_LightDirection.x);

	//DIFFUSE
	m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);
	m_pDiffuseColorVariable->SetFloatVector(&m_ColorDiffuse.x);
	if (m_bDiffuseTexture)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	//SPECULAR
	m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);
	m_pSpecularColorVariable->SetFloatVector(&m_ColorSpecular.x);
	m_pShininessVariable->SetInt(m_Shininess);
	if (m_bSpecularLevelTexture)
	{
		m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());
	}

	//AMBIENT
	m_pAmbientColorVariable->SetFloatVector(&m_ColorAmbient.x);
	m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);

	//NORMAL MAPPING
	m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);
	m_pUseEnvironmentMappingVariable->SetBool(m_bNormalMapping);
	if (m_bNormalMapping)
	{
		m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());
	}

	//ENVIRONMENT MAPPING
	m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);
	m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	m_pRefractionStrengthVariable->SetFloat(m_RefractionStrength);
	m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);
	if (m_bEnvironmentMapping)
	{
		m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());
	}

	//OPACITY
	m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);
	m_pOpacityVariable->SetFloat(m_Opacity);
	if (m_bOpacityMap)
	{
		m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());
	}

	//SPECULAR MODELS
	m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);
	m_pUsePhongVariable->SetBool(m_bSpecularPhong);

	//////FRESNEL FALLOFF
	//m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);
	//m_pFresnelColorVariable->SetFloatVector(&m_ColorFresnel.x);
	//m_pFresnelPowerVariable->SetFloat(m_FresnelPower);
	//m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);
	//m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);

}