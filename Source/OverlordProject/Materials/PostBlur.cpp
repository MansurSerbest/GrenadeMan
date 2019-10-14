//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "../../OverlordEngine/Base/stdafx.h"
#include "PostBlur.h"
#include "Graphics/RenderTarget.h"

PostBlur::PostBlur()
	: PostProcessingMaterial(L"./Resources/Effects/Blur.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostBlur::~PostBlur(void)
{
}

void PostBlur::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele) Logger::LogError(L"m_pTextureMapVariabele is not loaded!");
}

void PostBlur::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
}