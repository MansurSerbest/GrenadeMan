//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "../../OverlordEngine/Base/stdafx.h"

#include "ColorMaterial.h"
#include "Base/GeneralStructs.h"

ID3DX11EffectVectorVariable* ColorMaterial::m_pColorVariable = nullptr;
ID3DX11EffectVectorVariable* ColorMaterial::m_pLightDirectionVariable = nullptr;

ColorMaterial::ColorMaterial(bool enableTransparency) : 
	Material(L"./Resources/Effects/PosNormCol3D.fx", enableTransparency?L"TransparencyTech":L"")
{
}


ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::SetColor(XMFLOAT4 color)
{
	m_Color = color;
}

void ColorMaterial::SetLightDirection(XMFLOAT3 dir)
{
	m_LightDirection = dir;
}

void ColorMaterial::LoadEffectVariables()
{
	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"ColorMaterial_Shadow::LoadEffectVariables() > \'gLightDirection\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	if (!m_pColorVariable)
	{
		m_pColorVariable = m_pEffect->GetVariableByName("gColor")->AsVector();
		if (!m_pColorVariable->IsValid())
		{
			Logger::LogWarning(L"ColorMaterial_Shadow::LoadEffectVariables() > \'gColor\' variable not found!");
			m_pColorVariable = nullptr;
		}
	}
}

void ColorMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable->SetFloatVector(reinterpret_cast<float*>(&m_LightDirection));
	}

	if (m_pColorVariable)
	{
		m_pColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_Color));
	}
}
