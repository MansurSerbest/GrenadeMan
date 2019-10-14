#include "../../OverlordEngine/Base/stdafx.h"
#include "../../OverlordEngine/Graphics/RenderTarget.h"
#include "PostTimer.h"

PostTimer::PostTimer()
	: PostProcessingMaterial(L"./Resources/Effects/TimerEffect.fx"),
	m_pTextureMapVariabele(nullptr), m_pIntensityVariable{nullptr}, m_Intensity{0.0f}, m_Range{0.1f}, m_CurrRange{ m_Range }, m_PulseSign{1.0f}
{
}

PostTimer::~PostTimer(void)
{
}

void PostTimer::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele) Logger::LogError(L"m_pTextureMapVariabele is not loaded!");

	m_pIntensityVariable = m_pEffect->GetVariableByName("gIntensity")->AsScalar();
	if (!m_pIntensityVariable) Logger::LogError(L"m_pIntensityVariable is not loaded!");

	m_pRangeVariable = m_pEffect->GetVariableByName("gRange")->AsScalar();
	if (!m_pRangeVariable) Logger::LogError(L"m_pRangeVariable is not loaded!");
}

void PostTimer::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Pulse Range

	float pulse{ 0.00075f };
	float upperLimit{ 0.15f };
	float bottomLimit{ 0.03f };

	if (m_Range > upperLimit)
	{
		m_PulseSign = -1.0f;
	}
	if (m_Range < bottomLimit)
	{
		m_PulseSign = 1.0f;
	}

	m_Range += pulse * m_PulseSign;

	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	m_pIntensityVariable->SetFloat(m_Intensity);
	m_pRangeVariable->SetFloat(m_Range);
}

void PostTimer::SetIntensity(const float intensity)
{
	m_Intensity = intensity;
}

void PostTimer::SetRange(const float range)
{
	m_Range = range;
}